#pragma once
#include <windows.h>
#include <tchar.h>
#include <iostream>

using namespace std;


//定义一个系统版本枚举的大小4个字节
typedef enum _OS_VERSION_ {
	WINDOWS_UNKNOW,
	WINDOWS_7,
	WINDOWS_10
}OS_VERSION;

OS_VERSION FaGetOsVersion();

BOOL FaSetStringToClipboard(CString ParameterData);//将字符串转到粘贴板
void FaLocationExplorer(CString ImagePath);