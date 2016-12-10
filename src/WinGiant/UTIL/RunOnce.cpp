#include "RunOnce.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace WinGiant { namespace UTIL {

class ProcessRunOnce::Impl
{
public:
	Impl(const wchar_t* uniqueName);
	~Impl();

	bool canRun() { return canRun_; }
	void release();

private:
	bool canRun_;
	HANDLE mutex_;
};

ProcessRunOnce::Impl::Impl(const wchar_t* uniqueName)
{
	mutex_  = ::CreateMutex(NULL, TRUE, uniqueName);
	canRun_ = (mutex_ != NULL && ::GetLastError() != ERROR_ALREADY_EXISTS);
}

ProcessRunOnce::Impl::~Impl()
{
	release();
}

void ProcessRunOnce::Impl::release()
{
	if (mutex_)
	{
		::CloseHandle(mutex_);
		mutex_ = NULL;
	}
}

ProcessRunOnce::ProcessRunOnce(const wchar_t* uniqueName) : impl_(new Impl(uniqueName))
{
}

ProcessRunOnce::~ProcessRunOnce()
{
}

bool ProcessRunOnce::canRun()
{
	return impl_->canRun();
}

void ProcessRunOnce::release()
{
	impl_->release();
}

}}