#pragma once

namespace WinGiant { namespace IPC {

class Message
{
public:
	Message();
	Message(unsigned int sz);
	Message(char *buf, unsigned int sz);
	~Message();

	char* data() { return data_; }
	unsigned int length() { return length_; }

private:
	friend class Sub;
	void internalSet(char* data, unsigned int length); // internal use

private:
	char *data_;
	unsigned int length_;
};

}}

