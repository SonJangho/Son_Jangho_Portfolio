#pragma once

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>
#endif

#ifdef _DEBUG 
#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif 
#endif  // _DEBUG  



#include "Constant.h"
#include "Funtion.h"
#include "Typedef.h"
#include "Macro.h"
#include "Functor.h"
#include "Struct.h"
#include <time.h>

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace std;
