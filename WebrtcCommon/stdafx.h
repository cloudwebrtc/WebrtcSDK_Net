// stdafx.h: 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容


#ifndef PB_API
#define PB_API  extern "C" __declspec(dllexport)
#endif // !PB_API
// Windows 头文件 
#include <Windows.h>  

#include <string> 
 
