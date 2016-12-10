#include "Semaphore.hpp"

namespace WinGiant { namespace OS {

Semaphore::Semaphore()
{
	sema_ = ::CreateSemaphore(NULL, 0, 0x7FFFFFFF, NULL);
}

Semaphore::~Semaphore()
{
	::CloseHandle(sema_);
}

void Semaphore::push(long count)
{
	::ReleaseSemaphore(sema_, count, NULL);
}

void Semaphore::pull()
{
	::WaitForSingleObject(sema_, INFINITE);
}

bool Semaphore::pullTry()
{
	return ::WaitForSingleObject(sema_, 0) == WAIT_OBJECT_0;
}

}}

