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

void FaExportListToTxt(CSortListCtrl* SortListCtrl, CString StatusInfo)
{
	if (SortListCtrl->GetItemCount() > 0)
	{
		CString FilePath;

		if (!FaGetDefaultTxtFilePath(FilePath)) //存钱框
			return;

		CFile v1;

		TRY
		{
			if (v1.Open(FilePath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate, NULL))
			{
				int i = 0;
				LVCOLUMN ColumnData;
				CString ColumnName;
				int ColumnNum = 0;
				CString v3;
				TCHAR v4[0x4000] = { 0 };
				TCHAR v10[0x4000] = { 0 };

				int Length = StatusInfo.GetLength();
				_tcsncpy_s(v10, 0x4000, StatusInfo.GetBuffer(), Length);
				//v10 = 0x000000e2344e0cd0 L"进程：212\t应用层不可访问进程：12"
				StatusInfo.ReleaseBuffer();

#ifdef _UNICODE

				char v11[0x4000] = { 0 };
				WideCharToMultiByte(CP_ACP, 0, v10, -1, v11, 0x4000, NULL, NULL);
				v1.Write(v11, (UINT)strlen(v11));
				v1.Write(("\r\n\r\n"), (UINT)strlen(("\r\n\r\n")));

#else
				v1.Write(v10, (UINT)_tcslen(v10));
				v1.Write(_T("\r\n\r\n"), (UINT)_tcslen(_T("\r\n\r\n")));
#endif

				ColumnData.mask = LVCF_TEXT;


				ColumnData.cchTextMax = 100;
				ColumnData.pszText = ColumnName.GetBuffer(100);

				for (i = 0; SortListCtrl->GetColumn(i, &ColumnData); i++)
				{
					v3 = v3 + ColumnData.pszText + _T("\t\t");
				}

				v3 += _T("\r\n");
				Length = v3.GetLength();
				_tcsncpy_s(v4, 0x4000, v3.GetBuffer(), Length);
				v3.ReleaseBuffer();
#ifdef _UNICODE		
				char v12[0x4000] = { 0 };
				WideCharToMultiByte(CP_ACP, 0, v4, -1, v12, 0x4000, NULL, NULL);
				v1.Write(v12, (UINT)strlen(v12));

#else
				v1.Write(v4, (UINT)_tcslen(v4));
#endif
				ColumnName.ReleaseBuffer();
				ColumnNum = i;

				for (int ItemCount = 0; ItemCount < SortListCtrl->GetItemCount(); ItemCount++)
				{

					TCHAR v7[0x4000] = { 0 };
					CString v6;

					for (i = 0; i < ColumnNum; i++)
					{
						v6 = v6 + SortListCtrl->GetItemText(ItemCount, i) + _T("\t\t");
					}

					v6 += _T("\r\n");
					Length = v6.GetLength();
					_tcsncpy_s(v7, 0x4000, v6.GetBuffer(), Length);
					v6.ReleaseBuffer();


#ifdef _UNICODE		
					char v13[0x4000] = { 0 };
					WideCharToMultiByte(CP_ACP, 0, v7, -1, v13, 0x4000, NULL, NULL);
					v1.Write(v13, (UINT)strlen(v13));

#else
					v1.Write(v7, (UINT)_tcslen(v7));
#endif


				}

				v1.Close();
			}
		}
			CATCH_ALL(e)
		{
			v1.Abort();   // close file safely and quietly
						  //	THROW_LAST();
		}
		END_CATCH_ALL

			if (PathFileExists(FilePath))
			{
				ShellExecute(NULL, _T("open"), FilePath, NULL, NULL, SW_SHOW);   //一旦保存文件成功自动打开该文件
			}
			else
			{
				MessageBox(NULL, _T("导出到文本文件失败!"), _T("PCManager"), MB_OK | MB_ICONERROR);
			}
	}
}

BOOL FaGetDefaultTxtFilePath(CString& FilePath)
{
	CString v1;
	CTime Time = CTime::GetCurrentTime();
	int Day, Month, Year, Hour, Minute, Second;

	Day = Time.GetDay();
	Month = Time.GetMonth();
	Year = Time.GetYear();
	Hour = Time.GetHour();
	Minute = Time.GetMinute();
	Second = Time.GetSecond();
	v1.Format(_T("%04d%02d%02d%02d%02d%02d"), Year, Month, Day, Hour, Minute, Second);

	FilePath = v1 + _T(".txt");

	CFileDialog FileDialog(
		FALSE,
		0,
		FilePath,
		0,
		_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"),//文件类型
		0
	);

	if (IDOK == FileDialog.DoModal())
	{
		FilePath = FileDialog.GetFileName();

		CString v2 = FilePath.Right((int)_tcslen(_T(".txt")));

		if (v2.CompareNoCase(_T(".txt")) != 0)
		{
			FilePath += _T(".txt");
		}

		if (!PathFileExists(FilePath))
		{
			return TRUE;
		}
		else if (PathFileExists(FilePath) && MessageBox(NULL, _T("文件已经存在，是否覆盖?"), _T("PCManager"),
			MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			if (!DeleteFile(FilePath))
			{
				MessageBox(NULL, _T("覆盖文件失败!"), _T("PCManager"), MB_OK | MB_ICONERROR);
				return FALSE;
			}

			return TRUE;
		}
	}

	return FALSE;
}
