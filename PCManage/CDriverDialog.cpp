// CDriverDialog.cpp: 实现文件
//

#include "pch.h"
#include "PCManage.h"
#include "CDriverDialog.h"
#include "afxdialogex.h"


// CDriverDialog 对话框

IMPLEMENT_DYNAMIC(CDriverDialog, CDialogEx)

CDriverDialog::CDriverDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DRIVER_DIALOG, pParent)
{

}

CDriverDialog::~CDriverDialog()
{
}

void CDriverDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_DRIVER_LIST, m_DriverCListCtrl);
}


BEGIN_MESSAGE_MAP(CDriverDialog, CDialogEx)
END_MESSAGE_MAP()


// CDriverDialog 消息处理程序
