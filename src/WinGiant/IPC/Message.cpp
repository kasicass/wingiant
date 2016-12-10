#include "Message.hpp"
#include <stdlib.h>
#include <string.h>

namespace WinGiant { namespace IPC {

Message::Message()
{
	data_ = nullptr;
	length_ = 0;
}

Message::Message(unsigned int sz)
{
	data_ = (char*)malloc(sz);
	length_ = sz;
}

Message::Message(char *buf, unsigned int sz)
{
	data_ = (char*)malloc(sz);
	memcpy(data_, buf, sz);
	length_ = sz;
}

Message::~Message()
{
	if (data_)
	{
		free(data_);
		data_ = nullptr;
	}
}

void Message::internalSet(char* data, unsigned int length)
{
	if (data_) free(data_);
	data_   = data;
	length_ = length;
}

}}