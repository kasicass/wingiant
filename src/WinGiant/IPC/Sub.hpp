// IPC::Sub sub;
// if (sub.bind("MyService"))
// {
//   while (1)
//   {
//     IPC::Message msg;
//     if (sub.recv(&msg))
//     {
//       // msg.data(), msg.length()
//     }
//     else
//     {
//       ::Sleep(100); // sleep a while
//     }
//   }
// }
// sub.close();

#pragma once

#include "Message.hpp"
#include <memory>

namespace WinGiant { namespace IPC {

class Sub
{
public:
	Sub();
	~Sub();

	bool bind(const wchar_t* addr);
	bool recv(Message* msg);
	void close();

private:
	class Impl;
	std::unique_ptr<Impl> impl_;
};

}}

