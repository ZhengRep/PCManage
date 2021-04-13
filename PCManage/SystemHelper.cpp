#include "pch.h"
#include "SystemHelper.h"

OS_VERSION FaGetOsVersion()
{
	OS_VERSION  OsVersion;
	int v1 = sizeof(OS_VERSION);   //测试枚举大小
	//SDK提供
	OSVERSIONINFOEX OsVersionInfoEx;
	memset(&OsVersionInfoEx, 0, sizeof(OSVERSIONINFOEX));
	OsVersionInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if (GetVersionEx((LPOSVERSIONINFO)&OsVersionInfoEx))
	{
		if (OsVersionInfoEx.dwMajorVersion == 6 && OsVersionInfoEx.dwMinorVersion == 1)
		{
			//当前驱动支持WIN7系统
			OsVersion = WINDOWS_7;
		}
		else if (OsVersionInfoEx.dwMajorVersion == 6 && OsVersionInfoEx.dwMinorVersion == 2)
		{
			OsVersion = WINDOWS_10;
		}
		else
		{
			OsVersion = WINDOWS_UNKNOW;
		}
	}

	return OsVersion;
}

BOOL FaSetStringToClipboard(CString ParameterData)//QQ.exe  Buffer  Length 
{
	if (ParameterData.IsEmpty())
	{
		return TRUE;
	}

	BOOL IsOk = FALSE;

	if (OpenClipboard(NULL))
	{
		HGLOBAL GlobalHandle = 0;
		TCHAR* v5 = NULL;

		EmptyClipboard();

		GlobalHandle = LocalAlloc(GMEM_ZEROINIT, (ParameterData.GetLength() + 1) * sizeof(TCHAR));
		if (GlobalHandle)
		{
			v5 = (TCHAR*)GlobalLock(GlobalHandle);//锁定内存，防止丢失
			if (v5)
			{
				_tcsncpy_s(v5, ParameterData.GetLength() + 1, ParameterData.GetBuffer(), ParameterData.GetLength());
				ParameterData.ReleaseBuffer();

#ifdef _UNICODE
				SetClipboardData(CF_UNICODETEXT, GlobalHandle);
#else
				SetClipboardData(CF_TEXT, GlobalHandle);
#endif // _UNICODE

				GlobalUnlock(GlobalHandle);

				IsOk = TRUE;
			}
		}

		CloseClipboard();
	}

	return IsOk;
}

void FaLocationExplorer(CString ImagePath)
{
	PVOID OldValue = NULL;
	Wow64DisableWow64FsRedirection(&OldValue);
	if (!ImagePath.IsEmpty() && PathFileExists(ImagePath))
	{
		CString v1;
		v1.Format(_T("/select,%s"), ImagePath);
		ShellExecute(NULL, _T("open"), _T("explorer.exe"), v1, NULL, SW_SHOW);

	}
	else
	{
		MessageBox(NULL, _T("文件不存在"), _T("PCManage"), MB_OK | MB_ICONWARNING);
	}
}