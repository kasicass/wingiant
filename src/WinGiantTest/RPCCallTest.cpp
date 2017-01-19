#include "WinGiant/All.hpp"
#include "MyData.hpp"

using namespace WinGiant;

int main()
{
	MyData d;
	d.s = "Hello";
	d.v = 10;
	d.f = 20.6f;

	RPC::Call call(L"MyService");
	call.call("showMeTheMoney", d);

	MyData2 d2;
	d2.v = 30;

	// it may fails as NonblockingPipeServer response slowly
	::Sleep(50);

	RPC::Call call2(L"MyService");
	call2.call("killMe", d2);
	return 0;
}