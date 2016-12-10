#include "Mutex.hpp"

namespace WinGiant { namespace OS {

Mutex::Mutex()
{
	InitializeCriticalSection(&cs_);
}

Mutex::~Mutex()
{
	DeleteCriticalSection(&cs_);
}

void Mutex::lock()
{
	EnterCriticalSection(&cs_);
}

void Mutex::unlock()
{
	LeaveCriticalSection(&cs_);
}

}}