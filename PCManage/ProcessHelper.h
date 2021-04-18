#pragma once
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h>
#include <Psapi.h>
#include<algorithm>
#include "StringHelper.h"

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


typedef struct _CURDIR
{
	UNICODE_STRING DosPath;
	HANDLE Handle;
} CURDIR, * PCURDIR;

typedef struct RTL_DRIVE_LETTER_CURDIR
{
	USHORT              Flags;
	USHORT              Length;
	ULONG               TimeStamp;
	UNICODE_STRING      DosPath;
} RTL_DRIVE_LETTER_CURDIR, * PRTL_DRIVE_LETTER_CURDIR;


typedef struct _RTL_USER_PROCESS_PARAMETERS
{
	ULONG MaximumLength;
	ULONG Length;
	ULONG Flags;
	ULONG DebugFlags;
	HANDLE ConsoleHandle;
	ULONG ConsoleFlags;
	HANDLE StandardInput;
	HANDLE StandardOutput;
	HANDLE StandardError;
	CURDIR CurrentDirectory;
	UNICODE_STRING DllPath;
	UNICODE_STRING ImagePathName;
	UNICODE_STRING CommandLine;
	PWSTR Environment;
	ULONG StartingX;
	ULONG StartingY;
	ULONG CountX;
	ULONG CountY;
	ULONG CountCharsX;
	ULONG CountCharsY;
	ULONG FillAttribute;
	ULONG WindowFlags;
	ULONG ShowWindowFlags;
	UNICODE_STRING WindowTitle;
	UNICODE_STRING DesktopInfo;
	UNICODE_STRING ShellInfo;
	UNICODE_STRING RuntimeData;
#define RTL_MAX_DRIVE_LETTERS 32
	RTL_DRIVE_LETTER_CURDIR CurrentDirectories[RTL_MAX_DRIVE_LETTERS];
#if (NTDDI_VERSION >= NTDDI_LONGHORN)
	SIZE_T EnvironmentSize;
#endif
#if (NTDDI_VERSION >= NTDDI_WIN7)
	SIZE_T EnvironmentVersion;
#endif
} RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;

typedef struct _PEB {
	BYTE Reserved1[2];
	BYTE BeingDebugged;
	BYTE Reserved2[1];
	PVOID Reserved3[2];
	PVOID Ldr;
	PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
	BYTE Reserved4[104];
	PVOID Reserved5[52];
	PVOID PostProcessInitRoutine;
	BYTE Reserved6[128];
	PVOID Reserved7[1];
	ULONG SessionId;
} PEB, * PPEB;

typedef struct _PROCESS_BASIC_INFORMATION {
	PVOID Reserved1;
	PPEB PebBaseAddress;   //指针
	PVOID Reserved2[2];
	ULONG_PTR UniqueProcessId;
	PVOID Reserved3;
} PROCESS_BASIC_INFORMATION;

typedef enum _PROCESSINFOCLASS {
	ProcessBasicInformation = 0,
	ProcessDebugPort = 7,
	ProcessWow64Information = 26,
	ProcessImageFileName = 27,
	ProcessBreakOnTermination = 29
} PROCESSINFOCLASS;


typedef long (_stdcall* LPFN_NTQUERYINFORMATIONPROCESS)
	(HANDLE,
	UINT,
	PVOID,
	ULONG,
	PULONG);

//extern LPFN_NTQUERYINFORMATIONPROCESS __NtQueryInformationProcess;

#ifdef _UNICODE 
#define FaGetEnvironmentStrings FaGetEnvironmentStringsW
#else
#define FaGetEnvironmentStrings FaGetEnvironmentStringsA
#endif


HANDLE FaOpenProcess(DWORD DesiredAccess, BOOL IsInheritHandle, HANDLE ProcessIdentify);
BOOL FaRing3EnumProcessList(vector<PROCESS_TABLE_ENTRY_INFO>& ParameterData);
BOOL   FaGetProcessFullPathByProcessIdentify(HANDLE ProcessIdentify, TCHAR** ImagePath);
void* FaDosFullPathToNtFullPath(TCHAR* ParameterData);
BOOL FaSortRule(PROCESS_TABLE_ENTRY_INFO ParameterData1, PROCESS_TABLE_ENTRY_INFO ParameterData2);
CString FaGetProcessCreateTime(HANDLE ProcessIdentify);//获取进程创建时间
BOOL  FaKillProcess(PPROCESS_TABLE_ENTRY_INFO ParameterData);
BOOL  FaKillDeleteProcess(PPROCESS_TABLE_ENTRY_INFO ParameterData);

void  FaSuspendOrResumeProcess(PPROCESS_TABLE_ENTRY_INFO ParameterData, BOOL IsSuspend);//挂起或恢复进程

CString FaGetPebAddress(HANDLE ProcessIdentify);
CString FaGetProcessCommandLine(HANDLE ProcessIdentify);
CString FaGetProcessCurrentDirectory(HANDLE ProcessIdentify);
