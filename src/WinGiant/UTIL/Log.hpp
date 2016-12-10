// thread-safe log module

#pragma once

namespace WinGiant { namespace LOG {

enum GIANT_LOG_FLAGS
{
	GIANT_LOG_FILE    = 0x01, // output to logFile
	GIANT_LOG_DBGVIEW = 0x02, // win32 OutputDebugString()
	GIANT_LOG_STDOUT  = 0x04,
};

void init(const char* logFile, int flags);

void outputToStderr(bool to);  // default is false
void printf(const char* format, ...);

}};