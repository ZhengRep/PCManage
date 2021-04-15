#pragma once
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include "StringHelper.h"
#include "Ntdll.h"
#include <winver.h>
#include "SortListCtrl.h"

#pragma comment(lib,"Version.lib")
using namespace std;


CString FaGetFileCompanyName(CString ImagePath);
CString FaGetFileDescription(CString ImagePath);
void FaCheckFileAttributes(CString ImagePath);
BOOL FaCanOpenProcess(HANDLE ProcessIdentify);

void FaExportListToTxt(CSortListCtrl* SortListCtrl, CString StatusInfo);
BOOL FaGetDefaultTxtFilePath(CString& FilePath);