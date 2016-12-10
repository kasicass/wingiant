#include "WinGiant/All.hpp"
#include <stdio.h>

class MyThread : public WinGiant::OS::Thread
{
private:
	virtual void workFunc()
	{
		printf("[%d] inThread\n", ::GetCurrentThreadId());
		::Sleep(5*1000);
	}
};

int main()
{
	MyThread thr;

	printf("[%d] mainThread begin\n", ::GetCurrentThreadId());
	thr.run();
	thr.join();
	printf("[%d] mainThread end\n", ::GetCurrentThreadId());

	return 0;
}

