#include "Sub.hpp"
#include "../OS/Thread.hpp"
#include "../OS/ThreadQueue.hpp"
#include "../OS/NonblockingPipeServer.hpp"
#include <string>
#include <assert.h>

namespace WinGiant { namespace IPC {

//
// SubWorkThread
//

struct ThreadOutData
{
	char *buf;
	unsigned int sz;
};

class SubWorkThread : public OS::Thread
{
public:
	SubWorkThread();

	bool isQuit();
	void threadQuit();
	void setPipeName(const std::wstring &name);
	bool getDataTry(ThreadOutData &d);

private:
	virtual void workFunc();

private:
	bool quit_;
	std::wstring pipeName_;
	OS::ThreadQueue<ThreadOutData> out_;
};

SubWorkThread::SubWorkThread() : quit_(false)
{
}

bool SubWorkThread::isQuit()
{
	return quit_;
}

void SubWorkThread::threadQuit()
{
	quit_ = true;
}

void SubWorkThread::setPipeName(const std::wstring &name)
{
	pipeName_ = L"\\\\.\\pipe\\";
	pipeName_ += name;
}

bool SubWorkThread::getDataTry(ThreadOutData &d)
{
	return out_.getTry(d);
}

void SubWorkThread::workFunc()
{
	OS::NonblockingPipeServer srv;
	if (!srv.bind(pipeName_.c_str()))
	{
		quit_ = true;
		return;
	}

	while (1)
	{
		unsigned int bufsz = OS::NonblockingPipeServer::BUFSIZE;
		char *buf = (char*)malloc(bufsz);
		OS::NonblockingPipeServer::RECV_STATE s = srv.recv(buf, &bufsz, 100);
		if (s == OS::NonblockingPipeServer::RECV_OK)
		{
			ThreadOutData d;
			d.buf = buf;
			d.sz = bufsz;
			out_.put(d);
		}
		else if (s == OS::NonblockingPipeServer::RECV_TIMEOUT)
		{
			free(buf);
			if (isQuit())
				break;
		}
		else
		{
			free(buf);
			quit_ = true;
			break;
		}
	}
}


//
// Sub::Impl
//
class Sub::Impl
{
public:
	Impl();
	~Impl();

	bool bind(const wchar_t* addr);
	bool recv(Message* msg);
	void close();

private:
	SubWorkThread thr_;
};

Sub::Impl::Impl()
{
}

Sub::Impl::~Impl()
{
	close();
}

bool Sub::Impl::bind(const wchar_t* addr)
{
	thr_.setPipeName(addr);
	thr_.run();
	::Sleep(100);
	return !thr_.isQuit();
}

bool Sub::Impl::recv(Message* msg)
{
	assert(msg->data() == nullptr);

	ThreadOutData d;
	if (!thr_.getDataTry(d))
		return false;

	msg->internalSet(d.buf, d.sz);
	return true;
}

void Sub::Impl::close()
{
	thr_.threadQuit();
}


//
// Sub
//
Sub::Sub() : impl_(new Impl)
{
}

Sub::~Sub()
{
}

bool Sub::bind(const wchar_t* addr)
{
	return impl_->bind(addr);
}

bool Sub::recv(Message* msg)
{
	return impl_->recv(msg);
}

void Sub::close()
{
	impl_->close();
}

}}