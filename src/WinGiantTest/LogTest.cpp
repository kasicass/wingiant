#include "WinGiant/All.hpp"

using namespace WinGiant;

int main()
{
	LOG::init("mylog.txt", LOG::GIANT_LOG_FILE | LOG::GIANT_LOG_DBGVIEW | LOG::GIANT_LOG_STDOUT);

	LOG::printf("[mylog] Hello, Boy!\n");
	LOG::printf("[mylog] v = %d, f = %f\n", 10, 2.6);
	return 0;
}

