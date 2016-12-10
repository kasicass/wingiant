#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "NonblockingPipeServer.hpp"
#include "../UTIL/Log.hpp"
#include <stdio.h>
#include <string>

namespace WinGiant { namespace OS {

//
// NonblockingPipeServer::Impl
//

class NonblockingPipeServer::Impl
{
public:
	Impl();
	~Impl();

	bool bind(const wchar_t* pipeName);
	NonblockingPipeServer::RECV_STATE recv(char* buf, unsigned int* bufsz, unsigned int timeout);
	void close();

private:
	struct PIPEINST
	{
		OVERLAPPED overlap;
		HANDLE hPipe;
		char* buf;
		unsigned int* bufsz;
	};

private:
	static HANDLE createNamedPipe(const wchar_t* pipeName);
	static bool connectToNewClient(HANDLE hPipe, OVERLAPPED* oConnect, bool *pendingIO);

	static void disconnectAndClose(PIPEINST *pPipeInst);
	static void WINAPI completedReadRoutine(DWORD err, DWORD bytesRead, OVERLAPPED* overlap);

private:
	std::wstring pipeName_;
	HANDLE hPipe_;

	HANDLE hConnectEvent_;
	OVERLAPPED oConnect_;
	bool pendingIO_;
};

NonblockingPipeServer::Impl::Impl() :
	hConnectEvent_(NULL),
	hPipe_(INVALID_HANDLE_VALUE)
{
}

NonblockingPipeServer::Impl::~Impl()
{
	close();
}

bool NonblockingPipeServer::Impl::bind(const wchar_t* pipeName)
{
	pipeName_ = pipeName;

	hConnectEvent_ = CreateEvent(
		NULL,  // default security attribute
		TRUE,  // manual reset event
		TRUE,  // initial state = signaled
		NULL   // unnamed event object
	);
	if (hConnectEvent_ == NULL)
	{
		LOG::printf("[NonblockingPipeServerImpl::bind] CreateEvent() hConnectEvent_ failed, GLE=%d\n", GetLastError());
		return false;
	}

	oConnect_.hEvent = hConnectEvent_;

	hPipe_ = createNamedPipe(pipeName_.c_str());
	if (hPipe_ == INVALID_HANDLE_VALUE)
	{
		LOG::printf("[NonblockingPipeServerImpl::bind] createNamedPipe() failed, GLE=%d\n", GetLastError());
		close();
		return false;
	}

	if (!connectToNewClient(hPipe_, &oConnect_, &pendingIO_))
	{
		LOG::printf("[NonblockingPipeServerImpl::bind] connectToNewClient() failed\n");
		close();
		return false;
	}

	return true;
}

NonblockingPipeServer::RECV_STATE
NonblockingPipeServer::Impl::recv(char* buf, unsigned int* bufsz, unsigned int timeout)
{
	if (hConnectEvent_ == NULL)
		return NonblockingPipeServer::RECV_ERROR;

	while (1)
	{
		DWORD wait = WaitForSingleObjectEx(
			hConnectEvent_, // event object to wait for
			timeout,        // waits indefinitely
			TRUE            // alertable wait enabled
		);

		PIPEINST* pPipeInst;
		BOOL success, readok;
		DWORD dummy;
		switch (wait)
		{
		// The wait conditions are satisfied by a completed connect operation.
		case 0:
			// If an operation is pending, get the result of the connect operations.
			if (pendingIO_)
			{
				success = GetOverlappedResult(
					hPipe_,      // pipe handle
					&oConnect_,  // OVERLAPPED structure
					&dummy,      // bytes transferred
					FALSE        // does not wait
				);
				if (!success)
				{
					LOG::printf("[NonblockingPipeServerImpl::recv] GetOverlappedResult failed, GLE=%d\n", GetLastError());
					close();
					return NonblockingPipeServer::RECV_ERROR;
				}
			}

			// Allocate storage for this instance
			pPipeInst = (PIPEINST*) ::GlobalAlloc(GPTR, sizeof(PIPEINST));
			if (pPipeInst == NULL)
			{
				LOG::printf("[NonblockingPipeServerImpl::recv] GlobalAlloc failed, GLE=%d\n", GetLastError());
				close();
				return NonblockingPipeServer::RECV_ERROR;
			}

			pPipeInst->hPipe = hPipe_;
			pPipeInst->buf   = buf;
			pPipeInst->bufsz = bufsz;

			// Start the read operation for this client.
			readok = ReadFileEx(
				pPipeInst->hPipe,
				pPipeInst->buf,
				NonblockingPipeServer::BUFSIZE,
				(LPOVERLAPPED)pPipeInst,
				(LPOVERLAPPED_COMPLETION_ROUTINE)completedReadRoutine
			);

			if (!readok)
			{
				disconnectAndClose(pPipeInst);
			}

			// Create new pipe instance for the next client.
			hPipe_ = createNamedPipe(pipeName_.c_str());
			if (hPipe_ == INVALID_HANDLE_VALUE)
			{
				LOG::printf("[NonblockingPipeServerImpl::bind] createNamedPipe() failed, GLE=%d\n", GetLastError());
				close();
				return NonblockingPipeServer::RECV_ERROR;
			}

			if (!connectToNewClient(hPipe_, &oConnect_, &pendingIO_))
			{
				LOG::printf("[NonblockingPipeServerImpl::bind] connectToNewClient() failed\n");
				close();
				return NonblockingPipeServer::RECV_ERROR;
			}
			break;

		// The wait is satisfied by completed read or write
		// operation. This allows the system to execute the
		// completion routine.
		case WAIT_IO_COMPLETION:
			return NonblockingPipeServer::RECV_OK;

		case WAIT_TIMEOUT:
			return NonblockingPipeServer::RECV_TIMEOUT;
	
		// An error occurred in the wait function.
		default:
			LOG::printf("[NonblockingPipeServerImpl::recv] WaitForSingleObjectEx failed, GLE=%d\n", GetLastError());
			close();
			return NonblockingPipeServer::RECV_ERROR;
		}
	}
}

void NonblockingPipeServer::Impl::close()
{
	if (hPipe_ != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hPipe_);
		hPipe_ = INVALID_HANDLE_VALUE;
	}

	if (hConnectEvent_ != NULL)
	{
		::CloseHandle(hConnectEvent_);
		hConnectEvent_ = NULL;
	}
}

HANDLE NonblockingPipeServer::Impl::createNamedPipe(const wchar_t* pipeName)
{
	return ::CreateNamedPipe(
		pipeName,
		PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		0,          // inbuf size
		NonblockingPipeServer::BUFSIZE,    // outbuf size
		0,          // default timeout
		NULL
	);
}

bool NonblockingPipeServer::Impl::connectToNewClient(HANDLE hPipe, OVERLAPPED* oConnect, bool *pendingIO)
{
	BOOL connected;
	connected = ::ConnectNamedPipe(hPipe, oConnect);

	// Overlapped ConnectNamedPipe should return zero.
	if (connected)
	{
		LOG::printf("[NonblockingPipeServerImpl::connectToNewClient] ConnectNamedPipe failed 1, GLE=%d.\n", GetLastError());
		return false;
	}

	switch (GetLastError())
	{
	// The overlapped connection in progress.
	case ERROR_IO_PENDING:
		*pendingIO = true;
		break;

	// Client is already connected, so signal an event.
	case ERROR_PIPE_CONNECTED:
		if (::SetEvent(oConnect->hEvent))
		{
			*pendingIO = false;
			break;
		}
		else
		{
			LOG::printf("[NonblockingPipeServerImpl::connectToNewClient] SetEvent failed, GLE=%d.\n", GetLastError());
			return false;
		}

	// If an error occurs during the connect operation...
	default:
		LOG::printf("[NonblockingPipeServerImpl::connectToNewClient] ConnectNamedPipe failed 2, GLE=%d.\n", GetLastError());
		return false;
	}

	return true;
}

void NonblockingPipeServer::Impl::disconnectAndClose(PIPEINST *pPipeInst)
{
	if (!::DisconnectNamedPipe(pPipeInst->hPipe))
	{
		LOG::printf("[NonblockingPipeServerImpl::disconnectAndClose] DisconnectNamedPipe failed, GLE=%d\n");
	}
	::CloseHandle(pPipeInst->hPipe);
	::GlobalFree(pPipeInst);
}

void WINAPI NonblockingPipeServer::Impl::completedReadRoutine(DWORD err, DWORD bytesRead, OVERLAPPED* overlap)
{
	PIPEINST *pPipeInst = (PIPEINST*) overlap;
	*pPipeInst->bufsz = bytesRead;
	disconnectAndClose(pPipeInst);

	// if ((err == 0) && (bytesRead != 0))
}


//
// NonblockingPipeServer
//

NonblockingPipeServer::NonblockingPipeServer() : impl_(new Impl)
{
}

NonblockingPipeServer::~NonblockingPipeServer()
{
}

bool NonblockingPipeServer::bind(const wchar_t* pipeName)
{
	return impl_->bind(pipeName);
}

NonblockingPipeServer::RECV_STATE
NonblockingPipeServer::recv(char* buf, unsigned int* bufsz, unsigned int timeout)
{
	return impl_->recv(buf, bufsz, timeout);
}

void NonblockingPipeServer::close()
{
	return impl_->close();
}



}}
