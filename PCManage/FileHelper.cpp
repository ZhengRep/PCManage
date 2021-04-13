#include "pch.h"
#include "FileHelper.h"


CString FaGetFileCompanyName(CString ImagePath)
{
	PVOID OldValue = NULL;
	Wow64DisableWow64FsRedirection(&OldValue);    //64 --->[System32/calc.exe][SysWow64/calc.exe]


	CString v1=_T("信息缺失");
	if (ImagePath.IsEmpty())
	{
		return v1;
	}

	if (!ImagePath.CompareNoCase(_T("Idle")) || !ImagePath.CompareNoCase(_T("System")))
	{

		return v1;
	}


	if (!PathFileExists(ImagePath))
	{
		int LastError = GetLastError();
		return v1;
	}

	struct _TRANSLATION_ {
		WORD Language;
		WORD CodePage;
	} *v9;

	LPTSTR v2 = (LPTSTR)(LPCTSTR)ImagePath;
	DWORD v3 = 0;
	DWORD v7 = GetFileVersionInfoSize(v2, &v3);

	if (v7)
	{
		LPVOID v5 = LocalAlloc(LMEM_ZEROINIT, v7);

		if (v5)
		{
			if (GetFileVersionInfo(v2, v3, v7, v5))  //
			{
				UINT v8 = 0;


				if (VerQueryValue(v5, _T("\\VarFileInfo\\Translation"), (LPVOID*)&v9, &v8))
				{
					LPCTSTR v10 = 0;
					UINT    BufferLength = 0;
					TCHAR   BufferData[MAX_PATH] = { 0 };

					if ((v8 / sizeof(struct _TRANSLATION_)) > 0)
					{
						StringCchPrintf(BufferData, sizeof(BufferData) / sizeof(TCHAR),
							_T("\\StringFileInfo\\%04x%04x\\CompanyName"), v9[0].Language, v9[0].CodePage);
					}

					if (VerQueryValue(v5, BufferData, (LPVOID*)&v10, &BufferLength))
					{
						TCHAR v20[MAX_PATH] = { 0 };

						StringCchCopy(v20, MAX_PATH / sizeof(TCHAR), (LPCTSTR)v10);
						v1 = v20;
					}
				}
			}

			LocalFree(v5);
		}

	}
	else
	{
		v1 = _T("信息缺失");
	}

	return v1;
}

CString FaGetFileDescription(CString ImagePath)
{
	CString v1;

	PVOID OldValue = NULL;
	Wow64DisableWow64FsRedirection(&OldValue);    //64 --->[System32/calc.exe][SysWow64/calc.exe]

	if (!ImagePath.IsEmpty() && PathFileExists(ImagePath))
	{
		LPTSTR v2 = (LPTSTR)(LPCTSTR)ImagePath;  //
		DWORD v7 = GetFileVersionInfoSize(v2, NULL);

		if (v7)
		{
			LPVOID v5 = malloc(v7);
			if (v5)
			{
				if (GetFileVersionInfo(v2, 0, v7, v5))
				{
					char* v3 = NULL;
					UINT v4 = 0;
					if (VerQueryValue(v5, TEXT("\\VarFileInfo\\Translation"), (LPVOID*)&v3, &v4))
					{
						CString v8, v9;
						v1.Format(_T("000%x"), *((unsigned short int*)v3));
						v9 = v1.Right(4);
						v1.Format(_T("000%x"), *((unsigned short int*) & v3[2]));
						v9 += v1.Right(4);

						v8.Format(_T("\\StringFileInfo\\%s\\FileDescription"), v9);
						if (VerQueryValue(v5, v8.GetBufferSetLength(256), (LPVOID*)&v3, &v4))
						{
							v8.ReleaseBuffer();
							v1.Format(_T("%s"), v3);
						}
					}
					else
					{
						int v1 = 0;
					}
				}
				else
				{
					int v1 = 0;
				}

				free(v5);
			}
		}
		else
		{
			v1 = _T("信息缺失");
		}
	}
	else
	{
		v1 = _T("信息缺失");
	}
	return v1;
}

void FaCheckFileAttributes(CString ImagePath)
{
	if (!ImagePath.IsEmpty() && PathFileExists(ImagePath))
	{

		SHELLEXECUTEINFO ShellExecuteInfo;
		memset(&ShellExecuteInfo, 0, sizeof(ShellExecuteInfo));
		ShellExecuteInfo.fMask = SEE_MASK_INVOKEIDLIST;//是否展示error弹窗
		ShellExecuteInfo.cbSize = sizeof(ShellExecuteInfo);
		ShellExecuteInfo.hwnd = NULL;
		ShellExecuteInfo.lpVerb = _T("properties");//表示展示为属性
		ShellExecuteInfo.lpFile = ImagePath;//打开的文件路径
		ShellExecuteInfo.lpParameters = NULL;
		ShellExecuteInfo.lpDirectory = NULL;
		ShellExecuteInfo.nShow = SW_SHOWNORMAL;//如果文件打开也可以正常显示
		ShellExecuteInfo.hProcess = NULL;
		ShellExecuteInfo.lpIDList = 0;
		ShellExecuteInfo.hInstApp = 0;
		ShellExecuteEx(&ShellExecuteInfo);
	}
	else
	{
		MessageBox(NULL, _T("文件不存在!"), _T("PCManage"), MB_OK | MB_ICONWARNING);
	}
}

BOOL FaCanOpenProcess(HANDLE ProcessIdentify)
{
	BOOL IsRingAccess = TRUE;

	HANDLE ProcessHandle = OpenProcess(PROCESS_TERMINATE | PROCESS_VM_OPERATION,
		TRUE, HandleToLong(ProcessIdentify));
	if (ProcessHandle)
	{
		FaCloseHandle(ProcessHandle);
	}
	else
	{
		IsRingAccess = FALSE;
	}

	return IsRingAccess;
}