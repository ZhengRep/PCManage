#pragma once
#include "ProcessHelper.h"
#include "CProcessModuleCDialog.h"


// CProcessDetailInfoCDialog 对话框

class CProcessDetailInfoCDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessDetailInfoCDialog)

public:
	CProcessDetailInfoCDialog(CWnd* pParent = NULL, PROCESS_TABLE_ENTRY_INFO* ProcessTableEntryInfo = NULL);    // 标准构造函数
	virtual ~CProcessDetailInfoCDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESS_DETAIL_INFO_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CProcessModuleCDialog      m_ProcessModuleCDialog;
	CTabCtrl m_ProcessDetailInfoCTabkCtrl;
	PPROCESS_TABLE_ENTRY_INFO m_ProcessTableEntryInfo;
	CDialog* m_Dialog[20];
	int m_CurrentSelect;

public:
	virtual BOOL OnInitDialog();
};
