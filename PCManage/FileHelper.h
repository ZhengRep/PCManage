#pragma once
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include "StringHelper.h"
#include "Ntdll.h"
#include <winver.h>

#pragma comment(lib,"Version.lib")
using namespace std;


CString FaGetFileCompanyName(CString ImagePath);
CString FaGetFileDescription(CString ImagePath);
void FaCheckFileAttributes(CString ImagePath);
BOOL FaCanOpenProcess(HANDLE ProcessIdentify);