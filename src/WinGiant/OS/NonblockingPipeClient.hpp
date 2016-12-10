// OS::NonblockingPipeClient cli;
// if (cli.connect("\\\\.\\pipe\\MyNamedPipe"))
// {
//   cli.send("Hello", 6);
// }
// cli.close();

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace WinGiant { namespace OS {

class NonblockingPipeClient
{
public:
	NonblockingPipeClient();
	~NonblockingPipeClient();

	bool connect(const wchar_t* addr);
	bool send(const char* buf, unsigned int sz); // return immediately
	void close();

private:
	HANDLE hPipe_;
};

}}
