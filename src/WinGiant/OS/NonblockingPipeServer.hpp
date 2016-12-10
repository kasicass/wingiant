// OS::NonblockingPipeServer srv;
// if (srv.bind("\\\\.\\pipe\\MyNamedPipe"))
// {
//   while (1)
//   {
//     unsigned int bufsz = OS::NonblockingPipeServer::BUFSIZE;
//     char buf[bufsz];
//     OS::NonblockingPipeServer::RECV_STATE s = srv.recv(buf, &bufsz, timeout);
//     if (s == OS::NonblockingPipeServer::RECV_OK)
//     {
//       // use buf
//     }
//     else if (s == OS::NonblockingPipeServer::RECV_TIMEOUT)
//     {
//       if (need quit)
//         break;
//     }
//     else
//     {
//       // error
//       break;
//     }
//   }
// }
// srv.close();

#pragma once

#include <memory>

namespace WinGiant { namespace OS {

class NonblockingPipeServer
{
public:
	enum { BUFSIZE = 4096 };

	enum RECV_STATE
	{
		RECV_OK,
		RECV_ERROR,
		RECV_TIMEOUT,
	};

public:
	NonblockingPipeServer();
	~NonblockingPipeServer();

	bool bind(const wchar_t* pipeName);
	RECV_STATE recv(char* buf, unsigned int* bufsz, unsigned int timeout);
	void close();

private:
	class Impl;
	std::unique_ptr<Impl> impl_;
};

}}
