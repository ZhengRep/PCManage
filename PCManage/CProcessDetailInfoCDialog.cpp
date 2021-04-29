// CProcessDetailInfoCDialog.cpp: 实现文件
//

#include "pch.h"
#include "PCManage.h"
#include "CProcessDetailInfoCDialog.h"
#include "afxdialogex.h"


// CProcessDetailInfoCDialog 对话框

IMPLEMENT_DYNAMIC(CProcessDetailInfoCDialog, CDialogEx)

CProcessDetailInfoCDialog::CProcessDetailInfoCDialog(CWnd* pParent, PROCESS_TABLE_ENTRY_INFO* ProcessTableEntryInfo)
	: CDialogEx(IDD_PROCESS_DETAIL_INFO_DIALOG, pParent)
	, m_ProcessTableEntryInfo(ProcessTableEntryInfo)
{

	m_CurrentSelect = 0;
}

CProcessDetailInfoCDialog::~CProcessDetailInfoCDialog()
{
}

void CProcessDetailInfoCDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROCESS_DETAIL_INFO_TAB, m_ProcessDetailInfoCTabkCtrl);
}

BOOL CProcessDetailInfoCDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString v1;
	v1.Format(_T("%s ( %s | %d )"), _T("进程信息"), m_ProcessTableEntryInfo->ImageName, m_ProcessTableEntryInfo->ProcessIdentify);
	SetWindowText(v1);


	m_ProcessDetailInfoCTabkCtrl.InsertItem(0, _T("模块"));
	m_ProcessDetailInfoCTabkCtrl.InsertItem(1, _T("句柄"));
	m_ProcessDetailInfoCTabkCtrl.InsertItem(2, _T("线程"));
	m_ProcessDetailInfoCTabkCtrl.InsertItem(3, _T("内存"));
	m_ProcessDetailInfoCTabkCtrl.InsertItem(4, _T("窗口"));
	m_ProcessDetailInfoCTabkCtrl.InsertItem(5, _T("热键"));
	m_ProcessDetailInfoCTabkCtrl.InsertItem(6, _T("定时器"));
	m_ProcessDetailInfoCTabkCtrl.InsertItem(7, _T("权限"));


	m_ProcessModuleCDialog.Create(IDD_PROCESS_MODUEL_DIALOG, &m_ProcessDetailInfoCTabkCtrl);

	CRect Rect;
	m_ProcessDetailInfoCTabkCtrl.GetClientRect(Rect);
	Rect.top += 22;
	Rect.bottom -= 3;
	Rect.left += 2;
	Rect.right -= 3;



	m_ProcessModuleCDialog.MoveWindow(&Rect);

	m_Dialog[0] = &m_ProcessModuleCDialog;


	m_Dialog[0]->ShowWindow(SW_SHOW);


	m_CurrentSelect = 0;

	FaEnumProcessModules(m_ProcessTableEntryInfo);
	return TRUE;
}

void CProcessDetailInfoCDialog::FaEnumProcessModules(PPROCESS_TABLE_ENTRY_INFO ProcessTableEntryInfo)
{
	if (ProcessTableEntryInfo->ProcessIdentify == 0)
	{
		return;
	}
	ULONG ProcessModuleCount = 0, NotMicrosoftModuleCount = 0;
	CString StatusInfo;
	/*	if (__IsFirst)
		{
			m_ProcessModuleDialog.m_IsShowMicrosoftModule = FALSE;
			__IsFirst = FALSE;
		}*/
	if (m_ProcessModuleCDialog.FaEnumProcessModules(ProcessTableEntryInfo, &ProcessModuleCount, &NotMicrosoftModuleCount, StatusInfo) == TRUE)
	{

		//m_ProcessInfoStatic.Format(_T("模块(非微软模块总数/模块总数)：%d/%d"), NotMicrosoftModuleCount, ProcessModuleCount);
	}
	else
	{
		//m_ProcessInfoStatic = StatusInfo;
		//m_ProcessInfoStatic.Format(_T("模块(非微软模块总数/模块总数)：%d/%d"), NotMicrosoftModuleCount, ProcessModuleCount);
	}

	UpdateData(FALSE);
}


BEGIN_MESSAGE_MAP(CProcessDetailInfoCDialog, CDialogEx)
END_MESSAGE_MAP()


// CProcessDetailInfoCDialog 消息处理程序
