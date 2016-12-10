// int main()
// {
//   ProcessRunOnce once("YourUniqueProcessName");
//   if (!once.canRun())
//     return;
//   ...
// }

#pragma once

#include <memory>

namespace WinGiant { namespace UTIL {

class ProcessRunOnce
{
public:
	ProcessRunOnce(const wchar_t* uniqueName);
	~ProcessRunOnce();

	bool canRun();
	void release();

private:
	class Impl;
	std::unique_ptr<Impl> impl_;
};

}}