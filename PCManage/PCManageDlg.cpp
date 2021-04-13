
// PCManageDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "PCManage.h"
#include "PCManageDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

OS_VERSION __OsVersion;
CDialog* __Dialog[20];
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPCManageDlg 对话框



CPCManageDlg::CPCManageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PCMANAGE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPCManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_TAB, m_MainCTabCtrl);
}

BEGIN_MESSAGE_MAP(CPCManageDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CPCManageDlg::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAIN_TAB, &CPCManageDlg::OnTcnSelchangeMainTab)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_ICONNOTIFY, (LRESULT(__thiscall CWnd::*)(WPARAM, LPARAM))OnIconNotify)
	ON_COMMAND(ID_ICON_NOTIFY_EXIT, &CPCManageDlg::OnIconNotifyExit)
	ON_COMMAND(ID_ICON_NOTIFY_SHOW, &CPCManageDlg::OnIconNotifyShow)
END_MESSAGE_MAP()


// CPCManageDlg 消息处理程序

BOOL CPCManageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, TRUE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);
	//ShowWindow(SW_MAXIMIZE);


	// TODO: 在此添加额外的初始化代码
	FaInitNotifyIconData();


	//获得系统版本
	__OsVersion = FaGetOsVersion();

	if (__OsVersion != WINDOWS_UNKNOW)
	{
		//提升权限
		if (!FaEnableDebugPrivilege(TRUE) || !FaEnableLoadDriverPrivilege(TRUE))
		{
			OutputDebugString(_T("FaEnableDebugPrivilege() Error"));
			ExitProcess(0);
		}


		//开启服务
		//加载驱动

		FaEnableLoadDriverPrivilege(FALSE);
	}
	else
	{
		OutputDebugString(_T("FaGetOsVersion() Error"));
		ExitProcess(0);
	}

	m_MainCTabCtrl.InsertItem(0, _T("进程"));
	m_MainCTabCtrl.InsertItem(1, _T("驱动模块"));
	m_MainCTabCtrl.InsertItem(2, _T("钩子"));
	m_MainCTabCtrl.InsertItem(3, _T("内核层"));
	m_MainCTabCtrl.InsertItem(4, _T("注册表"));
	m_MainCTabCtrl.InsertItem(5, _T("文件"));
	m_MainCTabCtrl.InsertItem(6, _T("服务"));
	
	m_ProcessDialog.Create(IDD_PROCESS_DIALOG, &m_MainCTabCtrl);
	m_DriverDialog.Create(IDD_DRIVER_DIALOG, &m_MainCTabCtrl);


	CRect Rect;
	m_MainCTabCtrl.GetClientRect(Rect);
	Rect.top += 20;
	Rect.bottom -= 2;
	Rect.left -=2 ;
	Rect.right -= 2;

	m_ProcessDialog.MoveWindow(&Rect);
	m_DriverDialog.MoveWindow(&Rect);
	__Dialog[0] = &m_ProcessDialog;
	__Dialog[1] = &m_DriverDialog;
	__Dialog[0]->ShowWindow(SW_SHOW);
	__Dialog[1]->ShowWindow(SW_HIDE);

	m_CurrentSelect = 0;


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}




void CPCManageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPCManageDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPCManageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//程序托盘
VOID CPCManageDlg::FaInitNotifyIconData()
{
	CString Tip = _T("PCManage.......");
	m_NotifyIconData.cbSize = sizeof(NOTIFYICONDATA);                //大小赋值	
	m_NotifyIconData.hWnd = m_hWnd;   //爷爷的爸爸的CWnd类中定义的                             
	m_NotifyIconData.uID = IDR_MAINFRAME;
	m_NotifyIconData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;     //托盘所拥有的状态  气泡提示
	m_NotifyIconData.hIcon = m_hIcon;
	m_NotifyIconData.uCallbackMessage = WM_ICONNOTIFY;              //回调消息(自定义)																//气泡提示
	_tcscpy(m_NotifyIconData.szTip, Tip.GetBuffer(NULL));
	Shell_NotifyIcon(NIM_ADD, &m_NotifyIconData);                  //显示托盘
																   //消息 (消息声明  消息与函数关联  函数的声明  函数的实现)
}

void CPCManageDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CPCManageDlg::OnTcnSelchangeMainTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	__Dialog[m_CurrentSelect]->ShowWindow(SW_HIDE);
	m_CurrentSelect = m_MainCTabCtrl.GetCurSel();
	__Dialog[m_CurrentSelect]->ShowWindow(SW_SHOW);

	if (m_CurrentSelect == 0)
	{
		//m_ProcessDialog.SeSetProcessList();
	}
	else if (m_CurrentSelect == 1)
	{
		//m_DriverDialog.ListDrivers();
	}
	*pResult = 0;
}


void CPCManageDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//if (MessageBox(_T("Are you sure to exit?"), _T("PCManage....")) != IDOK)
	{

		Shell_NotifyIcon(NIM_DELETE, &m_NotifyIconData);
		CDialogEx::OnClose();
	}


	
}


VOID CPCManageDlg::OnIconNotify(WPARAM ParameterData1, LPARAM ParameterData2)
{
	switch ((UINT)ParameterData2)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	{
		if (!IsWindowVisible())
		{
			//窗口不显示
			ShowWindow(SW_SHOW);
		}
		else
		{
			ShowWindow(SW_HIDE);
		}
		break;
	}

	case WM_RBUTTONDOWN:
	{
		CMenu Menu;
		Menu.LoadMenu(IDR_ICON_NOTIFY_MENU);
		CPoint Point;  //获得鼠标位置
		GetCursorPos(&Point);
		SetForegroundWindow();   //设置当前窗口
		Menu.GetSubMenu(0)->TrackPopupMenu(
			TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
			Point.x, Point.y, this, NULL);

		break;
	}
	}
}

void CPCManageDlg::OnIconNotifyExit()
{
	// TODO: 在此添加命令处理程序代码
	SendMessage(WM_CLOSE);
}


void CPCManageDlg::OnIconNotifyShow()
{
	// TODO: 在此添加命令处理程序代码
}
