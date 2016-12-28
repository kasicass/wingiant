#include "WinGiant/All.hpp"
using namespace WinGiant;

int main()
{
	LOG::init("", LOG::GIANT_LOG_STDOUT);

	IPC::Pub pub;
	if (!pub.connect(L"mynamedpipe"))
	{
		printf("connect failed\n");
		return -1;
	}

	char data[] = "Hello";
	bool ok = pub.send(data, strlen(data));
	printf("send: %d\n", ok);

	// failed here, as IPC::Sub close the pipe after recv data once
	bool ok2 = pub.send(data, strlen(data));
	printf("send2: %d\n", ok2);

	return 0;
}