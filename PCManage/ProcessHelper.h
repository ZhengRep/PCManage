#pragma once
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h>
#include <Psapi.h>
#include<algorithm>

#pragma comment(lib,"Psapi.lib")
using namespace std;

typedef enum _ACTIVITY_STATUS_
{
	UNKNOWN,
	SUSPEND,
	RESUME,
	MYSELF
}ACTIVITY_STATUS;

typedef struct _PPROCESS_TABLE_ENTRY_INFO_
{
	TCHAR ImageName[MAX_PATH];
	HANDLE ProcessIdentify;
	HANDLE ParentProcessIdentify;
	ULONG_PTR EProcess;           //Ring0
	ULONGLONG CreateTime;
	TCHAR ImagePath[MAX_PATH];    //Ring3
	WCHAR ImagePath_[MAX_PATH];   //Ring0
	ACTIVITY_STATUS ActivityStatus = UNKNOWN;
}PROCESS_TABLE_ENTRY_INFO, * PPROCESS_TABLE_ENTRY_INFO;




HANDLE FaOpenProcess(DWORD DesiredAccess, BOOL IsInheritHandle, HANDLE ProcessIdentify);
BOOL FaRing3EnumProcessList(vector<PROCESS_TABLE_ENTRY_INFO>& ParameterData);
BOOL   FaGetProcessFullPathByProcessIdentify(HANDLE ProcessIdentify, TCHAR** ImagePath);
void* FaDosFullPathToNtFullPath(TCHAR* ParameterData);
BOOL FaSortRule(PROCESS_TABLE_ENTRY_INFO ParameterData1, PROCESS_TABLE_ENTRY_INFO ParameterData2);
CString FaGetProcessCreateTime(HANDLE ProcessIdentify);//获取进程创建时间
BOOL  FaKillProcess(PPROCESS_TABLE_ENTRY_INFO ParameterData);
BOOL  FaKillDeleteProcess(PPROCESS_TABLE_ENTRY_INFO ParameterData);

void  FaSuspendOrResumeProcess(PPROCESS_TABLE_ENTRY_INFO ParameterData, BOOL IsSuspend);//挂起或恢复进程
