#include "MyData.hpp"
#include <sstream>
using namespace WinGiant;

int main()
{
	IPC::Pub pub;
	if (!pub.connect(L"mynamedpipe"))
	{
		printf("connect failed\n");
		return -1;
	}

	MyData d;
	d.s = "HelloBoy!";
	d.v = 10;
	d.f = 20.6f;

	std::stringstream ss;
	msgpack::pack(ss, d);

	std::string str(ss.str());
	bool ok = pub.send(str.data(), str.size());
	printf("send: %d\n", ok);
	return 0;
}
