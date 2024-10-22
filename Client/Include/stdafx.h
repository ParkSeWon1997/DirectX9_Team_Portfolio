#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             .
#include <windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <ctime>
#include <cassert>
#include <process.h>

#include <io.h>
#include <iostream>

#include "fmod.hpp"

#if defined(_DEBUG) && !defined(_TOOL)

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif 

#endif // defined(_DEBUG) && !defined(_TOOL)

#define WINCX 1080
#define WINCY 720

extern HWND g_hWnd;
extern HINSTANCE g_hInst;
#include "FuncEvent.h"


using namespace std;