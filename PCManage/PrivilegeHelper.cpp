#include "pch.h"
#include "PrivilegeHelper.h"

BOOL FaEnableDebugPrivilege(BOOL IsEnable)
{
	BOOL IsOk = FALSE;
	HANDLE TokenHandle;


	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &TokenHandle))
	{
		TOKEN_PRIVILEGES TokenPrivileges;

		TokenPrivileges.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &TokenPrivileges.Privileges[0].Luid);
		TokenPrivileges.Privileges[0].Attributes = IsEnable ? SE_PRIVILEGE_ENABLED : 0;//根据IsEnable判断是否提升权限
		AdjustTokenPrivileges(TokenHandle, FALSE, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
		IsOk = (GetLastError() == ERROR_SUCCESS);
		CloseHandle(TokenHandle);
	}

	return IsOk;
}
BOOL FaEnableLoadDriverPrivilege(BOOL IsEnable)
{
	BOOL IsOk = FALSE;
	HANDLE TokenHandle;

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &TokenHandle))
	{
		TOKEN_PRIVILEGES TokenPrivileges;

		TokenPrivileges.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_LOAD_DRIVER_NAME, &TokenPrivileges.Privileges[0].Luid);
		TokenPrivileges.Privileges[0].Attributes = IsEnable ? SE_PRIVILEGE_ENABLED : 0;
		AdjustTokenPrivileges(TokenHandle, FALSE, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
		IsOk = (GetLastError() == ERROR_SUCCESS);
		CloseHandle(TokenHandle);
	}

	return IsOk;
}