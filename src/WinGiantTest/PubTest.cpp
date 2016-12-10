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

	bool ok = pub.send("Hello", 6);
	printf("send: %d\n", ok);
	return 0;
}