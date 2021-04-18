// CProcessDialog.cpp: 实现文件
//

#include "pch.h"
#include "PCManage.h"
#include "CProcessDialog.h"
#include "afxdialogex.h"
#include "common.h"


// CProcessDialog 对话框

IMPLEMENT_DYNAMIC(CProcessDialog, CDialogEx)

CProcessDialog::CProcessDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROCESS_DIALOG, pParent)
	, m_TotalCStatic(_T(""))
	, m_ProcessCEdit(_T(""))
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CProcessDialog::~CProcessDialog()
{
}

void CProcessDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROCESS_LIST, m_ProcessCListCtrl);
	DDX_Text(pDX, IDC_TOTAL_STATIC, m_TotalCStatic);
	DDX_Text(pDX, IDC_PROCESS_EDIT, m_ProcessCEdit);
}


BEGIN_MESSAGE_MAP(CProcessDialog, CDialogEx)
	ON_MESSAGE(WM_UPDATE_PROCESS_DATA, OnUpdateProcessData)
	ON_NOTIFY(NM_RCLICK, IDC_PROCESS_LIST, &CProcessDialog::OnNMRClickProcessList)
	ON_COMMAND(ID_PROCESS_REFRESH, &CProcessDialog::OnRefreshProcess)//刷新消息绑定
	ON_COMMAND(ID_KILL_PROCESS, &CProcessDialog::OnKillProcess)
	ON_COMMAND(ID_KILL_DELETE_PROCESS, &CProcessDialog::OnKillDeleteProcess)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROCESS_LIST, &CProcessDialog::OnNMCustomdrawProcessList)
	ON_COMMAND(ID_SUSPEND_PROCESS, &CProcessDialog::OnSuspendProcess)
	ON_COMMAND(ID_RESUME_PROCESS, &CProcessDialog::OnResumeProcess)
	ON_COMMAND(ID_COPY_IMAGE_NAME, &CProcessDialog::OnCopyImageName)
	ON_COMMAND(ID_COPY_IMAGE_PATH, &CProcessDialog::OnCopyImagePath)
	ON_COMMAND(ID_CHECK_FILE_ATTRIBUTES, &CProcessDialog::OnCheckFileAttributes)
	ON_COMMAND(ID_LOCATION_AT_EXPLORER, &CProcessDialog::OnLocationAtExplorer)
	ON_COMMAND(ID_EXPORT_TEXT, &CProcessDialog::OnExportText)
	ON_COMMAND(ID_INJECT_MODULE_1, &CProcessDialog::OnInjectModule1)
	ON_COMMAND(ID_SHOW_PROCESS_INFO, &CProcessDialog::OnShowProcessInfo)
	ON_COMMAND(ID_SHOW_PROCESS_DETAIL_INFO, &CProcessDialog::OnShowProcessDetailInfo)
END_MESSAGE_MAP()


// CProcessDialog 消息处理程序
BOOL CProcessDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	LOGFONT  Logfont;//最好弄成类成员,全局变量,静态成员  
	CFont* v3 = m_ProcessCListCtrl.GetFont();
	v3->GetLogFont(&Logfont);
	Logfont.lfHeight = Logfont.lfHeight * 1.1;   //这里可以修改字体的高比例
	Logfont.lfWidth = Logfont.lfWidth * 1.1;     //这里可以修改字体的宽比例
	CFont  v4;
	v4.CreateFontIndirect(&Logfont);
	m_ProcessCListCtrl.SetFont(&v4);
	m_ProcessCListCtrl.SetFont(&v4);
	v4.Detach();

	m_ProcessCListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT/* | LVS_EX_GRIDLINES*/);
	m_ProcessCListCtrl.InsertColumn(0, _T("进程名称"), LVCFMT_LEFT, 130);
	m_ProcessCListCtrl.InsertColumn(1, _T("进程ID"), LVCFMT_LEFT, 65);
	m_ProcessCListCtrl.InsertColumn(2, _T("父进程ID"), LVCFMT_LEFT, 65);
	m_ProcessCListCtrl.InsertColumn(3, _T("进程路径"), LVCFMT_LEFT, 290);
	m_ProcessCListCtrl.InsertColumn(4, _T("应用层访问"), LVCFMT_CENTER, 75);
	m_ProcessCListCtrl.InsertColumn(5, _T("文件厂商"), LVCFMT_LEFT, 140);
	m_ProcessCListCtrl.InsertColumn(6, _T("文件描述"), LVCFMT_LEFT, 190);
	m_ProcessCListCtrl.InsertColumn(7, _T("启动时间"), LVCFMT_LEFT, 140);


	// 初始化默认的进程图标
	m_ProcessIcon = LoadIcon(NULL, IDI_APPLICATION);

	m_ProcessImageList.Create(15, 14, ILC_COLOR32 | ILC_MASK, 2, 2);//像素
	m_ProcessCListCtrl.SetImageList(&m_ProcessImageList, LVSIL_SMALL);//图标绑定

	FaSetProcessList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CProcessDialog::FaSetProcessList()   //显示所有进程到List列表中
{
	//统计进程数量
	m_ProcessCount = 0;
	m_DenyAccessCount = 0;

	m_ProcessCListCtrl.DeleteAllItems();   //销毁List列表中的所有项
	int ImageCount = m_ProcessImageList.GetImageCount();
	for (int j = 0; j < ImageCount; j++)
	{
		m_ProcessImageList.Remove(0);
	}


	m_Ring3ProcessList.clear();
	FaRing3EnumProcessList(m_Ring3ProcessList);//枚举进程列表



	FaSortProcessList(m_Ring3ProcessList);

	for (vector <PROCESS_TABLE_ENTRY_INFO>::iterator Iter = m_Ring3ProcessList.begin();
		Iter != m_Ring3ProcessList.end();
		Iter++)
	{
		FaAddProcessItem(*Iter);
	}

	m_TotalCStatic.Format(_T("进程：%d	应用层不可访问进程：%d"), m_ProcessCount, m_DenyAccessCount);
	SendMessage(WM_UPDATE_PROCESS_DATA);//发送消息

}

void CProcessDialog::FaSortProcessList(vector<PROCESS_TABLE_ENTRY_INFO>& ParameterData)
{
	sort(ParameterData.begin(), ParameterData.end(), FaSortRule);
}

void CProcessDialog::FaAddProcessItem(PROCESS_TABLE_ENTRY_INFO ParameterData)
{
	//ImageName  ImagePath 
	//进程体
	CString ProcessIdentify, ParentProcessIdentify, EProcess;
	//获取生产产商
	CString CompanyName = FaGetFileCompanyName(ParameterData.ImagePath);

	CString FileDescription = FaGetFileDescription(ParameterData.ImagePath);
	CString CreateTime = FaGetProcessCreateTime(ParameterData.ProcessIdentify);

	ProcessIdentify.Format(_T("%d"), ParameterData.ProcessIdentify);
	ParentProcessIdentify.Format(_T("%d"), ParameterData.ParentProcessIdentify);

	// 处理进程图标
	SHFILEINFO v1;
	DWORD_PTR ReturnValue;
	HICON IconHwnd = NULL;

	ZeroMemory(&v1, sizeof(SHFILEINFO));


	ReturnValue = SHGetFileInfo(ParameterData.
		ImagePath,   //进程完整路径
		FILE_ATTRIBUTE_NORMAL,
		&v1,
		sizeof(SHFILEINFO),
		SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES
	);
	int v2 = -1;
	if (ReturnValue)
	{
		if (_tcsicmp(ParameterData.ImagePath, _T("获取进程完整路径失败")) == 0)
		{
			v2 = m_ProcessImageList.Add(m_ProcessIcon);
		}
		else
		{
			v2 = m_ProcessImageList.Add(v1.hIcon);
			DestroyIcon(v1.hIcon);
		}


	}
	else
	{
		v2 = m_ProcessImageList.Add(m_ProcessIcon);   //设置默认图标
	}

	//判断是否可以操作进程
	if (FaCanOpenProcess(ParameterData.ProcessIdentify))
	{
		IsRing3Access = _T("😊");
	}
	else
	{
		IsRing3Access = _T("拒绝");
		m_DenyAccessCount++;
	}



	int ItemCount = m_ProcessCListCtrl.GetItemCount();  //获取行

	int i = m_ProcessCListCtrl.InsertItem(ItemCount, ParameterData.ImageName,v2);
	m_ProcessCListCtrl.SetItemText(i, 1, ProcessIdentify);
	m_ProcessCListCtrl.SetItemText(i, 2, ParentProcessIdentify);
	m_ProcessCListCtrl.SetItemText(i, 3, ParameterData.ImagePath);
	m_ProcessCListCtrl.SetItemText(i, 4, IsRing3Access);
	m_ProcessCListCtrl.SetItemText(i, 5, CompanyName);
	m_ProcessCListCtrl.SetItemText(i, 6, FileDescription);
	m_ProcessCListCtrl.SetItemText(i, 7, CreateTime);

	//设置索引绑定每一个进程
	m_ProcessCListCtrl.SetItemData(ItemCount, ItemCount);

	//对特殊进行铺色
	COLOR_TYPE v10;
	v10 = BLACK;
	//如果不是微软的模块，那么就显示蓝色
	if (
		!_tcsicmp(ParameterData.ImageName, _T("System")) ||
		!_tcsicmp(ParameterData.ImageName, _T("Idle")))
	{
		v10 = BLUE;
	}

	else if (
		!_tcsicmp(ParameterData.ImageName, _T("Taskmgr.exe")) ||
		!_tcsicmp(ParameterData.ImageName, _T("Explorer.exe")) ||
		!_tcsicmp(ParameterData.ImageName, _T("PCManage.exe"))
		)
	{

		v10 = RED;
	}
	else if (0)
	{

	}
	else
	{

	}

	m_ProcessCount++;
	m_ColorListCtrl.push_back(v10);
}

LRESULT CProcessDialog::OnUpdateProcessData(WPARAM ParameterData1, LPARAM ParameterData2)
{
	UpdateData(FALSE);//内存到界面
	return 0;
}

	

void CProcessDialog::OnNMRClickProcessList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	CMenu Menu;
	Menu.CreatePopupMenu();
	Menu.AppendMenu(MF_STRING, ID_PROCESS_REFRESH, _T("刷新"));
	Menu.AppendMenu(MF_SEPARATOR);
	Menu.AppendMenu(MF_STRING, ID_SHOW_PROCESS_INFO, _T("查看进程基本信息"));
	Menu.AppendMenu(MF_STRING, ID_SHOW_PROCESS_DETAIL_INFO, _T("查看进程详细信息"));
	Menu.AppendMenu(MF_SEPARATOR);
	Menu.AppendMenu(MF_STRING, ID_KILL_PROCESS, _T("结束进程"));
	Menu.AppendMenu(MF_STRING, ID_KILL_DELETE_PROCESS, _T("结束进程并删除文件"));
	Menu.AppendMenu(MF_SEPARATOR);
	Menu.AppendMenu(MF_STRING, ID_SUSPEND_PROCESS, _T("挂起进程"));
	Menu.AppendMenu(MF_STRING, ID_RESUME_PROCESS, _T("恢复进程"));
	Menu.AppendMenu(MF_SEPARATOR);
	Menu.AppendMenu(MF_STRING, ID_COPY_IMAGE_NAME, _T("复制进程ID"));
	Menu.AppendMenu(MF_STRING, ID_COPY_IMAGE_PATH, _T("复制进程路径"));
	Menu.AppendMenu(MF_SEPARATOR);
	Menu.AppendMenu(MF_STRING, ID_CHECK_FILE_ATTRIBUTES, _T("查看进程文件属性"));
	Menu.AppendMenu(MF_STRING, ID_LOCATION_AT_EXPLORER, _T("在资源管理器中定位文件"));
	Menu.AppendMenu(MF_SEPARATOR);
	Menu.AppendMenu(MF_STRING, ID_CHECK_FILE_ATTRIBUTES, _T("查看进程文件属性"));
	Menu.AppendMenu(MF_STRING, ID_LOCATION_AT_EXPLORER, _T("在资源管理器中定位文件"));
	Menu.AppendMenu(MF_SEPARATOR);

	CMenu SubMenu1;
	SubMenu1.CreatePopupMenu();
	SubMenu1.AppendMenu(MF_STRING, ID_EXPORT_TEXT, _T("文本表格"));
	Menu.AppendMenu(MF_POPUP, (UINT)SubMenu1.m_hMenu, _T("导出到..."));


	CMenu SubMenu2;
	SubMenu2.CreatePopupMenu();
	SubMenu2.AppendMenu(MF_STRING, ID_INJECT_MODULE_1, _T("远程线程"));
	Menu.AppendMenu(MF_POPUP, (UINT)SubMenu2.m_hMenu, _T("注入模块..."));

	if (!m_ProcessCListCtrl.GetItemCount())
	{
		for (int i = 2; i < 36; i++)
		{
			Menu.EnableMenuItem(i, MF_BYPOSITION | MF_GRAYED | MF_DISABLED);
		}
	}
	else
	{
		//选择多项
		int SelectedCount = m_ProcessCListCtrl.GetSelectedCount();
		if (SelectedCount != 1)
		{
			for (int i = 2; i < 34; i++)
			{
				Menu.EnableMenuItem(i, MF_BYPOSITION | MF_GRAYED | MF_DISABLED);
			}

			if (SelectedCount >= 2)
			{
				Menu.EnableMenuItem(ID_KILL_PROCESS, MF_BYCOMMAND | MF_ENABLED);
			}
		}

		// 如果选择了一项
		else
		{
			ACTIVITY_STATUS v10 = FaGetProcessStatus();
			switch (v10)
			{
			case MYSELF:
				Menu.EnableMenuItem(ID_SUSPEND_PROCESS, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
				Menu.EnableMenuItem(ID_RESUME_PROCESS, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
				break;
			case SUSPEND:
				Menu.EnableMenuItem(ID_SUSPEND_PROCESS, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
				Menu.EnableMenuItem(ID_RESUME_PROCESS, MF_BYCOMMAND | MF_ENABLED);
				break;
			case UNKNOWN:
			case RESUME:
				Menu.EnableMenuItem(ID_SUSPEND_PROCESS, MF_BYCOMMAND | MF_ENABLED);
				Menu.EnableMenuItem(ID_RESUME_PROCESS, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
				break;
			}
		}
	}


	CPoint Point;
	GetCursorPos(&Point);
	Menu.TrackPopupMenu(TPM_RIGHTBUTTON, Point.x, Point.y, this);
	Menu.DestroyMenu();
	*pResult = 0;

}
void CProcessDialog::OnSuspendProcess()
{
	// 列表模式

	int Item = FaGetSelectedItem(&m_ProcessCListCtrl);//获取隐藏数据块的索引
	PPROCESS_TABLE_ENTRY_INFO v1 = FaGetProcessInfoByItem(Item);
	if (!v1)
	{
		return;
	}

	FaSuspendOrResumeProcess(v1, TRUE);//ture表示挂起
}
void CProcessDialog::OnResumeProcess()
{
	int SelectedItem = FaGetSelectedItem(&m_ProcessCListCtrl);
	PPROCESS_TABLE_ENTRY_INFO v1 = FaGetProcessInfoByItem(SelectedItem);
	if (!v1)
	{
		return;
	}

	FaSuspendOrResumeProcess(v1, FALSE);//false表示恢复
}
void CProcessDialog::OnCopyImageName()
{
	CString ImageName;
	POSITION Position = m_ProcessCListCtrl.GetFirstSelectedItemPosition();//有两种方法可以获取到选择的进程，但此种方法可以实现循环
	if (Position != NULL)
	{
		int SelectedItem = m_ProcessCListCtrl.GetNextSelectedItem(Position);
		ImageName = m_ProcessCListCtrl.GetItemText(SelectedItem, 1);
	}

	FaSetStringToClipboard(ImageName);
}
void CProcessDialog::OnCopyImagePath()
{
	CString ImagePath;
	POSITION Position = m_ProcessCListCtrl.GetFirstSelectedItemPosition();
	if (Position != NULL)
	{
		int SelectedItem = m_ProcessCListCtrl.GetNextSelectedItem(Position);
		ImagePath = m_ProcessCListCtrl.GetItemText(SelectedItem, 3);
	}

	FaSetStringToClipboard(ImagePath);
}
void CProcessDialog::OnCheckFileAttributes()
{
	CString ImagePath;
	POSITION Position = m_ProcessCListCtrl.GetFirstSelectedItemPosition();
	if (Position != NULL)
	{
		int SelectedItem = m_ProcessCListCtrl.GetNextSelectedItem(Position);
		ImagePath = m_ProcessCListCtrl.GetItemText(SelectedItem, 3);
	}
	FaCheckFileAttributes(ImagePath);
}
void CProcessDialog::OnLocationAtExplorer()
{
	CString ImagePath;
	POSITION Position = m_ProcessCListCtrl.GetFirstSelectedItemPosition();
	if (Position != NULL)
	{
		int SelectedItem = m_ProcessCListCtrl.GetNextSelectedItem(Position);
		ImagePath = m_ProcessCListCtrl.GetItemText(SelectedItem, 3);
	}
	FaLocationExplorer(ImagePath);
}
void CProcessDialog::OnExportText()
{
	FaExportListToTxt(&m_ProcessCListCtrl,m_TotalCStatic);
}
void CProcessDialog::OnInjectModule1()
{
	HANDLE ProcessIdentify = 0;


	int SelectedItem = FaGetSelectedItem(&m_ProcessCListCtrl);
	if (SelectedItem == -1)
	{
		return;
	}

	PPROCESS_TABLE_ENTRY_INFO v1 = FaGetProcessInfoByItem(SelectedItem);
	if (!v1)
	{
		return;
	}

	ProcessIdentify = v1->ProcessIdentify;

	if (ProcessIdentify == 0)
	{
		return;
	}

	//取钱框

	TCHAR v2[] = { 'I','n','j','e','c','t',' ','D','L','L','\0' };
	TCHAR v3[] = { 'D','L','L',' ','F','i','l','e','s','(','*','.','d','l','l',')','\0','*','.','d','l','l','\0','A','l','l',' ','F','i','l','e','s','(','*','.','*',')','\0','*','.','*','\0','\0','\0' };

	CFileDialog FileDialog(TRUE);
	FileDialog.m_ofn.lpstrTitle = v2;
	FileDialog.m_ofn.lpstrFilter = v3;

	if (IDOK != FileDialog.DoModal())
	{
		return;
	}

	CString v5 = FileDialog.GetPathName();
	if (PathFileExists(v5))
	{
		TCHAR v1[MAX_PATH] = { 0 };
		_tcsncpy_s(v1, v5.GetBuffer(), v5.GetLength());
		v5.ReleaseBuffer();

		if (!FaInjectModule1(v1, ProcessIdentify))
		{
			::MessageBox(NULL, _T("注入模块失败!"), _T("PCManager"), MB_OK | MB_ICONINFORMATION);
			//MessageBox(_T("注入模块失败!"), _T("PCManager"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			::MessageBox(NULL, _T("注入模块成功!"), _T("PCManager"), MB_OK | MB_ICONINFORMATION);
		}
	}
}
void CProcessDialog::OnRefreshProcess()
{
	FaSetProcessList();
}

void CProcessDialog::OnKillProcess()
{
	int SelectedCount = m_ProcessCListCtrl.GetSelectedCount();

	// 如果是单选
	if (SelectedCount == 1)
	{
		int SelectedItem = FaGetSelectedItem(&m_ProcessCListCtrl);
		if (SelectedItem != -1)
		{
			CString ImageName = m_ProcessCListCtrl.GetItemText(SelectedItem, 0);
			CString v1;
			v1.Format(_T("您确定要结束进程 %s 吗?"), ImageName);

			if (MessageBox(v1, _T("PCManage"), MB_YESNO | MB_ICONQUESTION) != IDYES)
			{
				return;
			}
		}
	}

	// 多选
	else if (SelectedCount > 1)
	{
		if (MessageBox(_T("您确定要结束这些进程吗?"), _T("PCManage"), MB_YESNO | MB_ICONQUESTION) != IDYES)
		{
			return;
		}
	}

	// 没有选中
	else
	{
		return;
	}



	POSITION Position = m_ProcessCListCtrl.GetFirstSelectedItemPosition();
	while (Position)
	{
		int SelectedItem = m_ProcessCListCtrl.GetNextSelectedItem(Position);
		PPROCESS_TABLE_ENTRY_INFO v1 = FaGetProcessInfoByItem(SelectedItem);

		if (v1)
		{
			FaKillProcess(v1);
		}

		m_ProcessCListCtrl.DeleteItem(SelectedItem);
		Position = m_ProcessCListCtrl.GetFirstSelectedItemPosition();
	}

	OnRefreshProcess();
}

int CProcessDialog::FaGetSelectedItem(CListCtrl* ParameterData)
{
	int SelectedItem = -1;

	if (ParameterData)
	{
		POSITION Position = ParameterData->GetFirstSelectedItemPosition();
		if (Position != NULL)
		{
			SelectedItem = ParameterData->GetNextSelectedItem(Position);
		}
	}

	return SelectedItem;
}

PPROCESS_TABLE_ENTRY_INFO CProcessDialog::FaGetProcessInfoByItem(int SelectedItem)
{
	PPROCESS_TABLE_ENTRY_INFO v1 = NULL;

	if (SelectedItem == -1)
	{
		return v1;
	}

	int ItemData = (int)m_ProcessCListCtrl.GetItemData(SelectedItem);
	v1 = &m_Ring3ProcessList.at(ItemData);

	return v1;
}

void CProcessDialog::OnKillDeleteProcess()
{
	int SelectedCount = m_ProcessCListCtrl.GetSelectedCount();
	if (SelectedCount == 1)
	{
		int SelectedItem = FaGetSelectedItem(&m_ProcessCListCtrl);
		if (SelectedItem != -1)
		{
			CString ImageName = m_ProcessCListCtrl.GetItemText(SelectedItem, 0);
			CString v1;
			v1.Format(_T("您确定要结束进程 %s 并删除相关文件吗?"), ImageName);
			if (MessageBox(v1, _T("PCManage"), MB_YESNO | MB_ICONQUESTION) != IDYES)
			{
				return;
			}
		}
	}
	else if (SelectedCount > 1)
	{
		if (MessageBox(_T("您确定要结束这些进程并删除相关文件吗?"), _T("PCManage"), MB_YESNO | MB_ICONQUESTION) != IDYES)
		{
			return;
		}
	}
	else
	{
		return;
	}

	//
	// 开始删除文件并结束进程
	//
	POSITION Position = m_ProcessCListCtrl.GetFirstSelectedItemPosition();
	while (Position)
	{
		int SelectedItem = m_ProcessCListCtrl.GetNextSelectedItem(Position);
		PPROCESS_TABLE_ENTRY_INFO v1 = FaGetProcessInfoByItem(SelectedItem);

		if (v1)
		{
			// 结束进程
			FaKillProcess(v1);

			Sleep(100);

			// 删除文件
			FaKillDeleteProcess(v1);

			Sleep(100);
		}

		m_ProcessCListCtrl.DeleteItem(SelectedItem);
		Position = m_ProcessCListCtrl.GetFirstSelectedItemPosition();
	}


	OnRefreshProcess();;
}

void CProcessDialog::OnNMCustomdrawProcessList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	COLORREF v1, v2;

	*pResult = CDRF_DODEFAULT;
	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{

		int Item = static_cast<int>(pLVCD->nmcd.dwItemSpec);
		int Index = (int)m_ProcessCListCtrl.GetItemData(Item);
		COLOR_TYPE v3 = m_ColorListCtrl.at(Index);
		if (v3 == RED)
		{
			v1 = RGB(255, 0, 0);
			v2 = RGB(255, 255, 255);
		}
		else if (v3 == BLUE)
		{
			v1 = RGB(0, 0, 255);
			v2 = RGB(255, 255, 255);
		}

		else {
			v1 = RGB(0, 0, 0);
			v2 = RGB(255, 255, 255);
		}
		pLVCD->clrText = v1;
		pLVCD->clrTextBk = v2;
		*pResult = CDRF_DODEFAULT;
	}
}

ACTIVITY_STATUS CProcessDialog::FaGetProcessStatus()
{
	ACTIVITY_STATUS v1 = UNKNOWN;

	//
	// 如果没有选中，就两个都置灰
	//
	int SelectedItem = FaGetSelectedItem(&m_ProcessCListCtrl);
	PPROCESS_TABLE_ENTRY_INFO v2 = FaGetProcessInfoByItem(SelectedItem);
	if (!v2)
	{
		return v1;
	}
	//
	// 如果选中的是我们的进程，就两个都置灰
	//
	if (v2->ProcessIdentify == (HANDLE)GetCurrentProcessId())
	{
		v1 = MYSELF;

	}
	else
	{
		v1 = v2->ActivityStatus;
	}

	return v1;
}

void CProcessDialog::OnShowProcessInfo()
{

	m_ProcessCEdit.Empty();
	UpdateData(FALSE);
	CString Peb;
	CString CommandLine;
	CString Environment;
	CString CurrentDirectory;
	int SelectedItem = FaGetSelectedItem(&m_ProcessCListCtrl);
	PPROCESS_TABLE_ENTRY_INFO v1 = FaGetProcessInfoByItem(SelectedItem);
	if (!v1)
	{
		return;
	}

	Peb = FaGetPebAddress(v1->ProcessIdentify);
	CommandLine = FaGetProcessCommandLine(v1->ProcessIdentify);
	CurrentDirectory = FaGetProcessCurrentDirectory(v1->ProcessIdentify);
	m_ProcessCEdit += _T("Peb地址：");
	m_ProcessCEdit += Peb;
	m_ProcessCEdit += _T("\r\n");
	m_ProcessCEdit += _T("命令行：");
	m_ProcessCEdit += CommandLine;
	m_ProcessCEdit += _T("\r\n");
	m_ProcessCEdit += _T("当前目录：");
	m_ProcessCEdit += CurrentDirectory;
	m_ProcessCEdit += _T("\r\n");
	m_ProcessCEdit += _T("环境变量：");
	m_ProcessCEdit += Environment;
	m_ProcessCEdit += _T("\r\n");


	//环境变量

	UpdateData(FALSE);
}
void CProcessDialog::OnShowProcessDetailInfo()
{

}