#include "pch.h"
#include "InjectionHelper.h"

BOOL FaInjectModule1(const TCHAR* ModulePath, HANDLE ProcessIdentify)
{

	if (NULL == ModulePath || 0 == ::_tcslen(ModulePath)
		|| ProcessIdentify == 0)       //_taccess(ModulePath, 0) 只支持双字
	{
		return FALSE;
	}

	HANDLE ProcessHandle = NULL;
	HANDLE ThreadHandle = NULL;
	DWORD ViewSize = 0;
	TCHAR* VirtualAddress = NULL;
	LPTHREAD_START_ROUTINE v1 = NULL;
	BOOL IsOk = FALSE;


	ProcessHandle = FaOpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, ProcessIdentify);
	if (NULL == ProcessHandle)
	{

		return FALSE;
	}
	ViewSize = (DWORD)::_tcslen(ModulePath) + 1;
	VirtualAddress = (TCHAR*)::VirtualAllocEx(ProcessHandle, NULL,
		ViewSize * sizeof(TCHAR), MEM_COMMIT, PAGE_READWRITE);
	if (NULL == VirtualAddress)
	{
		goto Exit;
	}

	// 在目标进程的内存空间中写入所需参数(模块名)
	if (FALSE == WriteProcessMemory(ProcessHandle, VirtualAddress,
		(LPVOID)ModulePath, ViewSize * sizeof(TCHAR), NULL))
	{

		goto Exit;
	}

	// 从 Kernel32.dll 中获取 LoadLibrary 函数地址
#ifdef _UNICODE
	v1 = (PTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandle(_T("Kernel32")), "LoadLibraryW");
#else
	v1 = (PTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandle(_T("Kernel32")), "LoadLibraryA");
#endif

	if (NULL == v1)
	{

		goto Exit;
	}


	ThreadHandle = CreateRemoteThread(ProcessHandle, NULL, 0, v1, VirtualAddress, 0, NULL);
	if (NULL == ThreadHandle)
	{

		goto Exit;
	}

	// 等待远程线程结束
	WaitForSingleObject(ThreadHandle, INFINITE);
	// 清理
	IsOk = TRUE;
Exit:
	if (VirtualAddress != NULL)
	{
		VirtualFreeEx(ProcessHandle, VirtualAddress, ViewSize, MEM_DECOMMIT);
	}

	FaCloseHandle(ThreadHandle);
	FaCloseHandle(ProcessHandle);

	return IsOk;
}
