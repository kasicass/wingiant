#pragma once

// 3rd party
#pragma warning(disable: 4244)  // conversion from 'double' to 'float', possible loss of data
#include "MSGPACK/include/msgpack.hpp"
// msgpack.hpp undefined the min/max macros, take care of including something relies on min/max (e.g., <GdiplusTypes.h>) after including this file

#include "OS/Thread.hpp"
#include "OS/ThreadQueue.hpp"

#include "UTIL/RunOnce.hpp"
#include "UTIL/Log.hpp"

#include "IPC/Pub.hpp"
#include "IPC/Sub.hpp"