#include "Log.hpp"
#include "../OS/Thread.hpp"
#include "../OS/ThreadQueue.hpp"
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

namespace WinGiant { namespace LOG {

enum CMD
{
	CMD_INIT = 1,
	CMD_LOG,
	CMD_QUIT,
};

struct LogInData
{
	CMD cmd;
	int flags;
	char *buf;
};

class LogThread : public OS::Thread
{
public:
	LogThread();
	virtual ~LogThread();

	void start(const char* logFile, int flags);
	void log(char* text);
	void quit();

	bool isStarted() { return init_; }

private:
	virtual void workFunc();

private:
	bool init_;
	int flags_;
	OS::ThreadQueue<LogInData> in_;
};

LogThread::LogThread() : init_(false)
{
}

LogThread::~LogThread()
{
	quit();
}

void LogThread::start(const char* logFile, int flags)
{
	assert(!init_);

	init_  = true;
	flags_ = flags;

	LogInData d;
	d.cmd    = CMD_INIT;
	d.flags  = flags_;
	d.buf    = _strdup(logFile);
	in_.put(d);

	run();
}

void LogThread::log(char* text)
{
	if (!init_)
		return;

	LogInData d;
	d.cmd    = CMD_LOG;
	d.flags  = flags_;
	d.buf    = text;
	in_.put(d);
}

void LogThread::quit()
{
	if (!init_)
		return;

	LogInData d;
	d.cmd    = CMD_QUIT;
	in_.put(d);
	
	join();
	init_ = false;
}

void LogThread::workFunc()
{
	FILE *fp = NULL;
	unsigned int n;

	while (1)
	{
		LogInData d;
		in_.get(d);

		switch (d.cmd)
		{
		case CMD_INIT:
			if (d.flags & GIANT_LOG_FILE) fp = fopen(d.buf, "w+");
			free(d.buf);
			break;

		case CMD_LOG:
			n = strlen(d.buf);
			if (d.flags & GIANT_LOG_FILE) { fwrite(d.buf, n, 1, fp); fflush(fp); }
			if (d.flags & GIANT_LOG_DBGVIEW) OutputDebugStringA(d.buf);
			if (d.flags & GIANT_LOG_STDOUT) fputs(d.buf, stdout);
			free(d.buf);
			break;

		case CMD_QUIT:
		default:
			if (fp) { fclose(fp); fp = NULL; }
			return;
		}
	}
}

static LogThread logThr;



static void shutdown()
{
	logThr.quit();
}

void init(const char* logFile, int flags)
{
	assert((flags & GIANT_LOG_FILE) && strlen(logFile) > 0);

	logThr.start(logFile, flags);
	atexit(shutdown);
}

void printf(const char* format, ...)
{
	if (!logThr.isStarted())
		return;

	unsigned int bufsz = 1024;
	unsigned int n;
	char *buf = (char*)malloc(bufsz);

	va_list args;
	va_start(args, format);
	n = vsnprintf(buf, bufsz-1, format, args);
	va_end(args);

	buf[n] = 0;
	logThr.log(buf);
}

}}