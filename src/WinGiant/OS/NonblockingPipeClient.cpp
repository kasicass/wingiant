#include "NonblockingPipeClient.hpp"
#include <string>
#include <assert.h>

namespace WinGiant { namespace OS {

NonblockingPipeClient::NonblockingPipeClient() : hPipe_(INVALID_HANDLE_VALUE)
{
}

NonblockingPipeClient::~NonblockingPipeClient()
{
	close();
}

bool NonblockingPipeClient::connect(const wchar_t* addr)
{
	hPipe_ = ::CreateFile(
		addr,
		GENERIC_WRITE | FILE_WRITE_ATTRIBUTES,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (hPipe_ == INVALID_HANDLE_VALUE)
		return false;

	DWORD mode = PIPE_READMODE_MESSAGE | PIPE_NOWAIT;
	BOOL ret = ::SetNamedPipeHandleState(
		hPipe_,
		&mode,
		NULL,
		NULL);
	if (!ret)
	{
		::CloseHandle(hPipe_);
		hPipe_ = INVALID_HANDLE_VALUE;
		return false;
	}

	return true;
}

bool NonblockingPipeClient::send(const char* buf, unsigned int sz)
{
	assert(hPipe_ != INVALID_HANDLE_VALUE);

	DWORD written;
	BOOL success = ::WriteFile(
		hPipe_,
		buf,
		sz,
		&written,
		NULL);
	return !!success;
}

void NonblockingPipeClient::close()
{
	if (hPipe_ != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hPipe_);
		hPipe_ = INVALID_HANDLE_VALUE;
	}
}

}}
