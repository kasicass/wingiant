#include "WinGiant/All.hpp"
#include "MyData.hpp"

using namespace WinGiant;

static void showService(void *data, msgpack::object obj)
{
	MyData d = obj.as<MyData>();
	printf("data: %d, show: %s, %d, %f\n", (int)data, d.s.c_str(), d.v, d.f);
}

static void showService2(void *data, msgpack::object obj)
{
	MyData2 d = obj.as<MyData2>();
	printf("show2: %d\n", d.v);
}

int main()
{
	RPC::Service svc(L"MyService");
	svc.registerCall("showMeTheMoney", showService, (void*)10);
	svc.registerCall("killMe", showService2, NULL);
	if (!svc.start())
	{
		printf("Service start failed\n");
		return -1;
	}

	while (1)
	{
		if (!svc.runOnce())
		{
			::Sleep(100);
		}
	}

	return 0;
}