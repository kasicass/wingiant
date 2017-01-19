#include "Service.hpp"
#include "../IPC/Message.hpp"

namespace WinGiant { namespace RPC {

Service::Service(const std::wstring& serviceAddress) : serviceAddress_(serviceAddress)
{
}

Service::~Service()
{
	sub_.close();
}

void Service::registerCall(const std::string& proto, SERVICE_CALLBACK func, void *data)
{
	protoMap_[proto] = std::make_pair(func, data);
}

bool Service::start()
{
	return sub_.bind(serviceAddress_.c_str());
}

bool Service::runOnce()
{
	IPC::Message msg;
	if (!sub_.recv(&msg))
		return false;

	size_t offset = 0;
	msgpack::object_handle oh = msgpack::unpack(msg.data(), msg.length(), offset);
	std::string proto = oh.get().as<std::string>();

	auto it = protoMap_.find(proto);
	if (it != protoMap_.end())
	{
		oh = msgpack::unpack(msg.data(), msg.length(), offset);
		it->second.first(it->second.second, oh.get());
	}

	return true;
}

}}