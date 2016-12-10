// IPC::Pub pub;
// if (pub.connect("MyService"))
// {
//   pub.send("Hello", 6);
// }
// pub.close();

#pragma once

#include "../OS/NonblockingPipeClient.hpp"

namespace WinGiant { namespace IPC {

class Pub
{
public:
	Pub();
	~Pub();

	bool connect(const wchar_t* addr);
	bool send(const char* buf, unsigned int sz);
	void close();

private:
	OS::NonblockingPipeClient cli_;
};

}}

