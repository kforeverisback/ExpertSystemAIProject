#pragma once


#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: reference additional headers your program requires 
#include "klog.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <future>
#include <chrono>
#include <utility>
#include <atomic>
#include <thread>
#include <queue>
#include <vector>
#include <stack>

#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana\system\platform.hpp>
#include <nana\threads\pool.hpp>
#include <nana\c++defines.hpp>
#include <nana\filesystem\filesystem.hpp>
namespace fs = nana::experimental::filesystem;
#undef max
#undef min

#include "jsonxx.h"
#define jxx jsonxx

#if !defined(__GNUC__)
//For Memory Leak Detection
#include <vld.h>
#endif