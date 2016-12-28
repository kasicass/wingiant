#include "MyData.hpp"
using namespace WinGiant;

int main(void)
{
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
			msgpack::object_handle oh = msgpack::unpack(msg.data(), msg.length());
			msgpack::object obj = oh.get();

			MyData d;
			obj.convert(d);

			printf("recv: %s, %d, %.2f\n", d.s.c_str(), d.v, d.f);
		}
		else
		{
			::Sleep(100);
		}
	}

	sub.close();
	return 0;
}
