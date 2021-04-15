#include "pch.h"
#include "ProcessHelper.h"
#include "ObjectHelper.h"
#include "Ntdll.h"
HANDLE FaOpenProcess(DWORD DesiredAccess, BOOL IsInheritHandle, HANDLE ProcessIdentify)
{

	HANDLE ProcessHandle = OpenProcess(DesiredAccess, IsInheritHandle, HandleToLong(ProcessIdentify));

	return ProcessHandle;
}
BOOL FaRing3EnumProcessList(vector<PROCESS_TABLE_ENTRY_INFO>& ParameterData)
{
	HANDLE SnapshotHandle;
	PROCESSENTRY32 ProcessEntry32;
	BOOL IsOk = FALSE;

	// 首先清空下链表
	ParameterData.clear();
	PROCESS_TABLE_ENTRY_INFO v1;
	SnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (SnapshotHandle != INVALID_HANDLE_VALUE)
	{
		ProcessEntry32.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(SnapshotHandle, &ProcessEntry32))
		{
			do
			{
				_tcscpy(v1.ImageName, ProcessEntry32.szExeFile);   //ImageName
				//v1.ImageName = 0x00d8c7c8 L"[System Process]"
				v1.ProcessIdentify = (HANDLE)ProcessEntry32.th32ProcessID;
				v1.ParentProcessIdentify = (HANDLE)ProcessEntry32.th32ParentProcessID;

				TCHAR* v5 = NULL;

				//通过进程ID获取进程完整路径
				if (FaGetProcessFullPathByProcessIdentify(v1.ProcessIdentify, &v5))
				{
					_tcscpy(v1.ImagePath, v5);

				}
				else
				{
					_tcscpy(v1.ImagePath, _T("获取进程完整路径失败"));
				}

				if (v5 != NULL)
				{
					VirtualFree(v5, 0, MEM_RELEASE);
				}


				ParameterData.push_back(v1);


			} while (Process32Next(SnapshotHandle, &ProcessEntry32));

			IsOk = TRUE;
		}
	}

	if (SnapshotHandle != INVALID_HANDLE_VALUE)
	{
		FaCloseHandle(SnapshotHandle);
	}


	return IsOk;
}
BOOL   FaGetProcessFullPathByProcessIdentify(HANDLE ProcessIdentify, TCHAR** ImagePath)
{
	TCHAR v1[MAX_PATH];
	BOOL IsOk = FALSE;
	HANDLE ProcessHandle = INVALID_HANDLE_VALUE;
	POBJECT_TYPE_INFORMATION v5 = NULL;
	ULONG  ReturnLength = 0;
	if (ImagePath == NULL)
	{
		return FALSE;
	}
	//通过进程ID获取进程句柄
	ProcessHandle = FaOpenProcess(PROCESS_QUERY_INFORMATION, FALSE, ProcessIdentify);
	if (ProcessHandle == NULL)
	{
		ProcessHandle = FaOpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, ProcessIdentify);
	}
	if (ProcessHandle == NULL)
	{
		goto Exit;
	}

	//判断一个句柄是否是一个进程句柄

	if (__NtQueryObject == NULL)   //Ntdll.dll模块下的函数
	{
		HMODULE ModuleHandle = NULL;
		ULONG   ReturnLength = 0;
		ModuleHandle = GetModuleHandle(_T("Ntdll.dll"));  //模块的导出表中获取函数的地址
		if (ModuleHandle == NULL)
		{
			goto Exit;
		}
		__NtQueryObject = (LPFN_NTQUERYOBJECT)GetProcAddress(ModuleHandle, "NtQueryObject");
		if (__NtQueryObject == NULL)
		{
			goto Exit;
		}
	}
	//通过句柄获得句柄类型

	NTSTATUS Status = __NtQueryObject(ProcessHandle, ObjectTypeInformation, NULL, 0, &ReturnLength);   //预测查询
	int v2 = sizeof(OBJECT_TYPE_INFORMATION);

	if (!NT_SUCCESS(Status))
	{
		if (Status == STATUS_INFO_LENGTH_MISMATCH)
		{
			v5 = (POBJECT_TYPE_INFORMATION)VirtualAlloc(NULL, ReturnLength, MEM_COMMIT, PAGE_READWRITE);
			Status = __NtQueryObject(ProcessHandle, ObjectTypeInformation, v5, ReturnLength, &ReturnLength);


			if (!NT_SUCCESS(Status))
			{
				IsOk = FALSE;
				goto Exit;
			}

			if (v5->TypeName.Length != 0 && v5->TypeName.Buffer != NULL)
			{

				if (memcmp(v5->TypeName.Buffer, L"Process", (wcslen(L"Process") * sizeof(WCHAR))) == 0)
				{
					if (GetProcessImageFileName(ProcessHandle, v1, MAX_PATH) == FALSE)
					{
						//v1 = 0x0136f5e4 L"\\Device\\HarddiskVolume4\\Windows\\System32\\Taskmgr.exe"

						goto Exit;
					}
					//v1 //HarddishVolume11//
					*ImagePath = (TCHAR*)FaDosFullPathToNtFullPath(v1);


					IsOk = (*ImagePath != NULL);
				}

			}
		}
	}
Exit:
	if (v5 != NULL)
	{
		VirtualFree(v5, 0, MEM_RELEASE);
		v5 = NULL;
	}

	if (ProcessHandle != NULL)
	{
		FaCloseHandle(ProcessHandle);

	}

	return IsOk;
}

void* FaDosFullPathToNtFullPath(TCHAR* ParameterData)
{
	LPVOID v5 = VirtualAlloc(NULL, 0x1000, MEM_COMMIT, PAGE_READWRITE); //pointer is returned
	TCHAR DeviceName[3] = _T("A:");
	TCHAR ComName[5] = _T("COM0");
	int v7 = 0;

	while (DeviceName[0] <= 0x5A)
	{
		RtlZeroMemory(v5, 0x1000);
		if (QueryDosDevice(DeviceName, (LPTSTR)v5, MAX_PATH * 2) > NULL)
		{

			v7 = _tcslen((LPTSTR)v5);
			_tcscat((LPTSTR)v5, (LPTSTR)(ParameterData + v7));
			if (_tcsicmp((LPTSTR)v5, ParameterData) == NULL)
			{
				RtlZeroMemory(v5, 0x1000);
				_tcscat((LPTSTR)v5, DeviceName);
				_tcscat((LPTSTR)v5, (LPTSTR)(ParameterData + v7));
				return(v5);
			}
		}
		DeviceName[0]++;
	}

	while (ComName[3] <= 0x39)
	{
		RtlZeroMemory(v5, 0x1000);
		if (QueryDosDevice(ComName, (LPTSTR)v5, MAX_PATH * 2) > NULL)
		{
			v7 = _tcslen((LPTSTR)v5);
			_tcscat((LPTSTR)v5, (LPTSTR)(ParameterData + v7));
			if (_tcsicmp((LPTSTR)v5, ParameterData) == NULL)
			{
				RtlZeroMemory(v5, 0x1000);
				_tcscat((LPTSTR)v5, ComName);
				_tcscat((LPTSTR)v5, (LPTSTR)(ParameterData + v7));
				return(v5);
			}
		}
		ComName[3]++;
	}

	VirtualFree(v5, NULL, MEM_RELEASE);

	return nullptr;
}

BOOL FaSortRule(PROCESS_TABLE_ENTRY_INFO ParameterData1, PROCESS_TABLE_ENTRY_INFO ParameterData2)
{
	return ParameterData1.ProcessIdentify < ParameterData2.ProcessIdentify;
}

CString FaGetProcessCreateTime(HANDLE ProcessIdentify)
{
	CString v1;
	FILETIME CreationTime, ExitTime, KernelTime, UserTime;
	HANDLE ProcessHandle = FaOpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcessIdentify);
	if (!ProcessHandle)
	{
		v1 = _T("信息缺失");
		goto Exit;
	}
	if (GetProcessTimes(ProcessHandle, &CreationTime, &ExitTime, &KernelTime, &UserTime))
	{
		FILETIME LocalFileTime;
		if (FileTimeToLocalFileTime(&CreationTime, &LocalFileTime))
		{
			SYSTEMTIME SystemTime;
			if (FileTimeToSystemTime(&LocalFileTime, &SystemTime))
			{
				v1.Format(
					_T("%04d/%02d/%02d  %02d:%02d:%02d"),
					SystemTime.wYear,
					SystemTime.wMonth,
					SystemTime.wDay,
					SystemTime.wHour,
					SystemTime.wMinute,
					SystemTime.wSecond);
			}
		}
	}
	else
	{
		v1 = _T("信息缺失");
	}

Exit:
	if (ProcessHandle != NULL)
	{
		FaCloseHandle(ProcessHandle);
		ProcessHandle = NULL;
	}
	return v1;
}

BOOL  FaKillProcess(PPROCESS_TABLE_ENTRY_INFO ParameterData)
{
	BOOL IsOk = FALSE;
	HANDLE ProcessHandle = FaOpenProcess(PROCESS_TERMINATE, FALSE, ParameterData->ProcessIdentify);
	if (!ProcessHandle)
	{
		goto Exit;

	}
	if (TerminateProcess(ProcessHandle, 0) != 0)
	{
		IsOk = TRUE;
	}
Exit:
	if (ProcessHandle != NULL)
	{
		FaCloseHandle(ProcessHandle);
		ProcessHandle = NULL;
	}
	return IsOk;

}
BOOL  FaKillDeleteProcess(PPROCESS_TABLE_ENTRY_INFO ParameterData)
{
	BOOL IsOk = FALSE;
	PVOID OldValue = NULL;
	Wow64DisableWow64FsRedirection(&OldValue);


	if (!PathFileExists(ParameterData->ImagePath))
	{
		int LastError = GetLastError();

		return IsOk;
	}

	return DeleteFile(ParameterData->ImagePath);

}

void  FaSuspendOrResumeProcess(PPROCESS_TABLE_ENTRY_INFO ParameterData, BOOL IsSuspend)
{


	HANDLE              SnapshotHandle;   //快照句柄
	THREADENTRY32       ThreadEntry32;
	HANDLE ThreadHandle;
	int LastError = 0;
	SnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (SnapshotHandle == INVALID_HANDLE_VALUE)
	{
		goto Exit;
	}
	ThreadEntry32.dwSize = sizeof(ThreadEntry32);
	if (Thread32First(SnapshotHandle, &ThreadEntry32))//第一个快照
	{
		do
		{
			if (ThreadEntry32.th32OwnerProcessID == HandleToUlong(ParameterData->ProcessIdentify))
			{
				//打开一个线程
				ThreadHandle = OpenThread(THREAD_SUSPEND_RESUME, FALSE, ThreadEntry32.th32ThreadID);
				if (ThreadHandle == NULL)
				{
					continue;
				}
				if (IsSuspend == TRUE)//快照期间就进行判断进程挂起、恢复状态
				{
					SuspendThread(ThreadHandle);
					//If the function succeeds,
					//the return value is the thread's previous suspend count;
					//otherwise, it is -1. To get extended error information,
					//use the GetLastError function. 
					LastError = GetLastError();
					if (LastError == 0)
					{

						ParameterData->ActivityStatus = SUSPEND;
					}
				}
				else
				{
					int ReturnValue = ResumeThread(ThreadHandle);   //0 1  
					LastError = GetLastError();
					if (LastError == 0 && ReturnValue == 0)
					{
						//进程权限太高没有被恢复
					}
					else if (LastError == 0 && ReturnValue >= 1)
					{
						ParameterData->ActivityStatus = RESUME;
					}
				}

				FaCloseHandle(ThreadHandle);

			}

		} while (Thread32Next(SnapshotHandle, &ThreadEntry32));
	}

Exit:
	if (SnapshotHandle != INVALID_HANDLE_VALUE)
	{
		FaCloseHandle(SnapshotHandle);
	}

	return;
}