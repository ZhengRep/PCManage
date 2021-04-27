#pragma once
#include "ProcessHelper.h"

// CProcessModuleCDialog 对话框

class CProcessModuleCDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessModuleCDialog)

public:
	CProcessModuleCDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CProcessModuleCDialog();

	BOOL CProcessModuleCDialog::FaSetProcessModules(ULONG* ProcessModuleCount,
		ULONG* NotMicrosoftModuleCount, CString& StatusInfo);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESS_MODUEL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
