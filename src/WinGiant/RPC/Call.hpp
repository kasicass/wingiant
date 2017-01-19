// struct MyData;
// MyData d;
// 
// RPC::Call call("MyService");
// call.call("add", d);

#pragma once

#pragma warning(disable: 4244)  // conversion from 'double' to 'float', possible loss of data
#include <msgpack.hpp>
#include <string>
#include <sstream>
#include "../IPC/Pub.hpp"

namespace WinGiant { namespace RPC {

class Call
{
public:
	Call(const std::wstring& serviceAddress);
	~Call();

	void call(const std::string& proto);
	template <typename T> void call(const std::string& proto, const T& arg);

private:
	bool connected_;
	IPC::Pub pub_;
};

template <typename T>
void Call::call(const std::string& proto, const T& arg)
{
	if (!connected_)
		return;

	std::stringstream ss;
	msgpack::pack(ss, proto);
	msgpack::pack(ss, arg);

	std::string buf(ss.str());
	pub_.send(buf.data(), buf.size());
}

}}

