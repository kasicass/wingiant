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

	// pub.send() 第二次会失败，因为 NonblockingPipeServer 收到一次消息后，就会 close pipe
	char data2[] = "Hello2";
	ok = pub.send(data2, strlen(data2));
	printf("send: %d, %u\n", ok, GetLastError());
	
	return 0;
}
