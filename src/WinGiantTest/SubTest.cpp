#include "WinGiant/All.hpp"

using namespace WinGiant;

int main()
{
	LOG::init("", LOG::GIANT_LOG_STDOUT);

	IPC::Sub sub;
	if (!sub.bind(L"mynamedpipe"))
	{
		printf("bind failed\n");
		return -1;
	}

	while (1)
	{
		IPC::Message msg;
		if (sub.recv(&msg))
		{
			printf("recv: %s, %u\n", msg.data(), msg.length());
		}
		else
		{
			::Sleep(100);
		}
	}

	sub.close();
	return 0;
}

