#include "WinGiant/All.hpp"
using namespace WinGiant;

int main()
{
	UTIL::ProcessRunOnce once(L"YourSuperLongName");
	if (!once.canRun())
	{
		printf("one process exists, exit me!\n");
		return 0;
	}

	printf("I'm the only one!\n");
	::Sleep(10*1000);
	printf("I'm the only one, exit!\n");

	return 0;
}