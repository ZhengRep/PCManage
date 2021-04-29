// CProcessModuleCDialog.cpp: 实现文件
//

#include "pch.h"
#include "PCManage.h"
#include "CProcessModuleCDialog.h"
#include "afxdialogex.h"


// CProcessModuleCDialog 对话框

IMPLEMENT_DYNAMIC(CProcessModuleCDialog, CDialogEx)

CProcessModuleCDialog::CProcessModuleCDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROCESS_MODUEL_DIALOG, pParent)
{

}

CProcessModuleCDialog::~CProcessModuleCDialog()
{
}

void CProcessModuleCDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROCESS_MODULE_LIST, m_ProcessModuleCListCtrl);
}


BEGIN_MESSAGE_MAP(CProcessModuleCDialog, CDialogEx)
END_MESSAGE_MAP()



BOOL CProcessModuleCDialog::FaSetProcessModules(
	ULONG* ProcessModuleCount,
	ULONG* NotMicrosoftModuleCount, 
	CString& StatusInfo
)
{
	//PROCESS_TABLE_ENTRY_INFO
	//	_CProcessHelper v1(ProcessTableEntryInfo);

	//v1.FaOpenProcess();

	//v1.FaEnumModules();

	return 0;


}

BOOL CProcessModuleCDialog::FaEnumProcessModules
(
	PPROCESS_TABLE_ENTRY_INFO ProcessTableEntryInfo,
	ULONG* ProcessModuleCount, 
	ULONG* NotMicrosoftModuleCount, 
	CString& StatusInfo
)
{
	// 清理
	m_ProcessModuleCListCtrl.DeleteAllItems();
	m_ProcessModuleCount = m_NotMicrosoftModuleCount = 0;

	m_Ring3ProcessModuleList.clear();


	m_ProcessTableEntryInfo = ProcessTableEntryInfo;


	_CProcessHelper Object(ProcessTableEntryInfo);
	Object.FaOpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ);
	Object.FaRing3EnumProcessModules(m_Ring3ProcessModuleList, INLOADORDER, StatusInfo);


	if (m_Ring3ProcessModuleList.empty())
	{
		return FALSE;
	}
	//if (!m_IsShowMicrosoftModule)
	{
		FaAddAllProcessModuleItem();
	}
	//else
	{
		//SeAddNotMicrosoftModuleItem();
	}
	/*
		if (ProcessModuleCount)
		{
			*ProcessModuleCount = m_ProcessModuleCount;
		}

		if (NotMicrosoftModuleCount)
		{
			*NotMicrosoftModuleCount = m_NotMicrosoftModuleCount;
		}
	*/
	return TRUE;
}

void CProcessModuleCDialog::FaAddAllProcessModuleItem()
{
	for (vector <MODULE_TABLE_ENTRY_INFO>::iterator Iter = m_Ring3ProcessModuleList.begin();
		Iter != m_Ring3ProcessModuleList.end(); Iter++)
	{

		/*if (!SeIsMicrosoftModule(Iter->ModulePath))
		{
			m_NotMicrosoftModuleCount++;
		}*/


		FaAddProcessModuleItem(*Iter);
		m_ProcessModuleCount++;
	}
}

void CProcessModuleCDialog::FaAddProcessModuleItem(MODULE_TABLE_ENTRY_INFO ModuleTableEntryInfo)
{
	CString VirtualAddress, ViewSize;
	CString ModulePath = ModuleTableEntryInfo.ModulePath;
	CString ModuleName = ModulePath.Right(ModulePath.GetLength() - ModulePath.ReverseFind(_T('\\')) - 1);

	VirtualAddress.Format(_T("0x%llx"), ModuleTableEntryInfo.VirtualAddress);
	ViewSize.Format(_T("0x%llx"), ModuleTableEntryInfo.ViewSize);



	int ItemCount = m_ProcessModuleCListCtrl.GetItemCount();
	m_ProcessModuleCListCtrl.InsertItem(ItemCount, ModuleName);
	m_ProcessModuleCListCtrl.SetItemText(ItemCount, 1, VirtualAddress);
	m_ProcessModuleCListCtrl.SetItemText(ItemCount, 2, ViewSize);
	m_ProcessModuleCListCtrl.SetItemText(ItemCount, 3, ModulePath);;

	m_ProcessModuleCListCtrl.SetItemData(ItemCount, ItemCount);
}




// CProcessModuleCDialog 消息处理程序


BOOL CProcessModuleCDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ProcessModuleCListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ProcessModuleCListCtrl.InsertColumn(0, _T("模块名称"), LVCFMT_LEFT, 150);
	m_ProcessModuleCListCtrl.InsertColumn(1, _T("基地址"), LVCFMT_LEFT, 200);
	m_ProcessModuleCListCtrl.InsertColumn(2, _T("模块大小"), LVCFMT_LEFT, 200);
	m_ProcessModuleCListCtrl.InsertColumn(3, _T("模块路径"), LVCFMT_LEFT, 340);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
