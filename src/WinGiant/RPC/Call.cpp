#include "Call.hpp"

namespace WinGiant { namespace RPC {

Call::Call(const std::wstring &serviceAddress)
{
	connected_ = pub_.connect(serviceAddress.c_str());
}

Call::~Call()
{
	pub_.close();
}

void Call::call(const std::string& proto)
{
	call(proto, 0);
}

}}

