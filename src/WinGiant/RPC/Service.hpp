// struct MyData;
// MyData d;
//
// void addService(msgpack::object obj);
//
// RPC::Service svc("MyService");
// svc.registerCall("add", addService);
// if (svc.start())
// {
//   while (1)
//   {
//     if (!svc.runOnce())
//       ::Sleep(100);
//   }
// }

#pragma once

#pragma warning(disable: 4244)  // conversion from 'double' to 'float', possible loss of data
#include <msgpack.hpp>
#include <string>
#include <map>
#include "../IPC/Sub.hpp"

namespace WinGiant { namespace RPC {

class Service
{
public:
	typedef void (*SERVICE_CALLBACK)(void*,msgpack::object);

public:
	Service(const std::wstring& serviceAddress);
	~Service();

	void registerCall(const std::string& proto, SERVICE_CALLBACK func, void *data);
	bool start();
	bool runOnce(); // true if handle one call, false if not

private:
	std::wstring serviceAddress_;
	std::map<std::string, std::pair<SERVICE_CALLBACK, void*>> protoMap_;
	IPC::Sub sub_;
};

}}

