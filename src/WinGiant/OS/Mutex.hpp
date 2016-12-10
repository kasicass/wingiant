#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace WinGiant { namespace OS {

class Mutex
{
public:
	Mutex();
	~Mutex();

	void lock();
	void unlock();

private:
	CRITICAL_SECTION cs_;
};

}}
