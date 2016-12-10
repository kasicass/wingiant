#pragma once

#include "Mutex.hpp"
#include "Semaphore.hpp"
#include <deque>

namespace WinGiant { namespace OS {

template <typename T>
class ThreadQueue
{
public:
	ThreadQueue();
	~ThreadQueue();

	void put(T v);
	void get(T& v);    // blocking
	bool getTry(T& v); // nonblocking

private:
	Mutex mutex_;
	Semaphore signal_;
	std::deque<T> queue_;
};

template <typename T>
ThreadQueue<T>::ThreadQueue()
{
}

template <typename T>
ThreadQueue<T>::~ThreadQueue()
{
}

template <typename T>
void ThreadQueue<T>::put(T v)
{
	mutex_.lock();
	queue_.push_back(v);
	mutex_.unlock();

	signal_.push();
}

template <typename T>
void ThreadQueue<T>::get(T& v)
{
	signal_.pull();

	mutex_.lock();
	v = queue_.front();
	queue_.pop_front();
	mutex_.unlock();
}

template <typename T>
bool ThreadQueue<T>::getTry(T& v)
{
	if (!signal_.pullTry())
		return false;

	mutex_.lock();
	v = queue_.front();
	queue_.pop_front();
	mutex_.unlock();

	return true;
}

}}

