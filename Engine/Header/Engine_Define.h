#ifndef Engine_Define_h__
#define Engine_Define_h__

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <list>
#include <map>
#include <queue>
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <ctime>
#include <fstream>
#include <cassert>

#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"

#include <io.h>
#include <iostream>

#include "fmod.hpp"
#pragma comment(lib, "fmod_vc.lib")

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#pragma warning(disable : 4251)

#if defined(_DEBUG) && !defined(_TOOL)

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif 

#endif // defined(_DEBUG) && !defined(_TOOL)

using namespace std;

using namespace Engine;


#endif // Engine_Define_h__
