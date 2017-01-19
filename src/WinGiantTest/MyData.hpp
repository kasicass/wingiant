#pragma once

#include "WinGiant/All.hpp"
#include <string>

struct MyData
{
	std::string s;
	int v;
	float f;

	MSGPACK_DEFINE(s, v, f);
};

struct MyData2
{
	int v;

	MSGPACK_DEFINE(v);
};
