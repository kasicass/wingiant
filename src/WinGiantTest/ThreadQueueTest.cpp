#include "WinGiant/All.hpp"
#include <stdio.h>
#include <assert.h>

using namespace WinGiant;

struct ThreadData
{
	bool exit;
	int v;
	float f;
};

class MyThread : public OS::Thread
{
public:
	void putData(const ThreadData& d)
	{
		in_.put(d);
	}

	bool tryGetData(ThreadData& d)
	{
		return out_.getTry(d);
	}

private:
	virtual void workFunc()
	{
		printf("[%d] thread enter\n", ::GetCurrentThreadId());

		ThreadData d;

		while (1)
		{
			in_.get(d);
			if (d.exit)
				break;

			d.v += int(d.f);
			out_.put(d);
		}

		printf("[%d] thread leave\n", ::GetCurrentThreadId());
	}

private:
	OS::ThreadQueue<ThreadData> in_;
	OS::ThreadQueue<ThreadData> out_;
};

int main()
{
	MyThread thr;

	printf("[%d] mainThread begin\n", ::GetCurrentThreadId());
	thr.run();

	ThreadData d = { false, 2, 3.5};
	thr.putData(d);
	::Sleep(100);

	bool ok = thr.tryGetData(d);
	assert(ok);
	assert(d.v == 5);

	ok = thr.tryGetData(d);
	assert(!ok);

	d.exit = true;
	thr.putData(d);

	thr.join();
	printf("[%d] mainThread end\n", ::GetCurrentThreadId());

	return 0;
}

