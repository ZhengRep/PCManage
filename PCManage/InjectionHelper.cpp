#include "pch.h"
#include "InjectionHelper.h"

BOOL FaInjectModule1(const TCHAR* ModulePath, HANDLE ProcessIdentify)
{

	if (NULL == ModulePath || 0 == ::_tcslen(ModulePath)
		|| ProcessIdentify == 0)       //_taccess(ModulePath, 0) ֻ֧��˫��
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

	// ��Ŀ����̵��ڴ�ռ���д���������(ģ����)
	if (FALSE == WriteProcessMemory(ProcessHandle, VirtualAddress,
		(LPVOID)ModulePath, ViewSize * sizeof(TCHAR), NULL))
	{

		goto Exit;
	}

	// �� Kernel32.dll �л�ȡ LoadLibrary ������ַ
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

	// �ȴ�Զ���߳̽���
	WaitForSingleObject(ThreadHandle, INFINITE);
	// ����
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
