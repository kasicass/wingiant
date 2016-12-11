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
			char *tmp = (char*)malloc(msg.length()+1);
			memcpy(tmp, msg.data(), msg.length());
			tmp[msg.length()] = '\0';

			printf("recv: %s\n", tmp);
			free(tmp);
		}
		else
		{
			::Sleep(100);
		}
	}

	sub.close();
	return 0;
}

