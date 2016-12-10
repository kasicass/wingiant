#include "Pub.hpp"
#include <string>
#include <assert.h>

namespace WinGiant { namespace IPC {

Pub::Pub()
{
}

Pub::~Pub()
{
}

bool Pub::connect(const wchar_t* addr)
{
	std::wstring pipeName(L"\\\\.\\pipe\\");
	pipeName += addr;

	return cli_.connect(pipeName.c_str());
}

bool Pub::send(const char* buf, unsigned int sz)
{
	return cli_.send(buf, sz);
}

void Pub::close()
{
	cli_.close();
}

}}
