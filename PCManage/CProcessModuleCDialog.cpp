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

void CProcessModuleCDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProcessModuleCDialog, CDialogEx)
END_MESSAGE_MAP()


// CProcessModuleCDialog 消息处理程序
