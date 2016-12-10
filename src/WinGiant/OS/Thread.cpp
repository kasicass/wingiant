#include "Thread.hpp"
#include <process.h>
#include <assert.h>

namespace WinGiant { namespace OS {

Thread::Thread() : hThread_(NULL)
{
}

Thread::~Thread()
{
	join();
}

unsigned int __stdcall Thread::threadFunc(void* arg)
{
	Thread *thr = (Thread *)arg;
	thr->workFunc();
	return 0;
}

void Thread::run()
{
	assert(hThread_ == NULL);
	hThread_ = HANDLE( _beginthreadex(NULL, 0, Thread::threadFunc, this, 0, NULL) );
}

void Thread::join()
{
	if (hThread_)
	{
		::WaitForSingleObject(hThread_, INFINITE);
		::CloseHandle(hThread_);
		hThread_ = NULL;
	}
}

}}
