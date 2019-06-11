// SDKTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <stdio.h> 
#include <string>
#include <cstring>
#include <tchar.h>
#include <iostream> 
#include <windows.h>  
using namespace std;
 
HMODULE  webrtcCommonDll = LoadLibrary(_T("WebrtcCommon.dll"));
//Test方法
typedef bool (*TestMethod)();
TestMethod Test = (TestMethod)GetProcAddress(webrtcCommonDll, "Test"); 

 
 
int main()
{
	//HMODULE  webrtcCommonDll = LoadLibrary(exepathWCHAR.data());	 
	Test();
 

	std::cout << "Hello World!\n";
}



