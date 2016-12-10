#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace WinGiant { namespace OS {

class Semaphore
{
public:
	Semaphore();
	~Semaphore();

	void push(long count = 1);
	void pull();
	bool pullTry();

private:
	HANDLE sema_;
};

}}
