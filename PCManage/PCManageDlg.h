
// PCManageDlg.h: 头文件
//

#pragma once
#include "SystemHelper.h"
#include "PrivilegeHelper.h"
#include "CDriverDialog.h"
#include "CProcessDialog.h"

#define WM_ICONNOTIFY WM_USER+1



// CPCManageDlg 对话框
class CPCManageDlg : public CDialogEx
{
// 构造
public:
	CPCManageDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PCMANAGE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	NOTIFYICONDATA      m_NotifyIconData;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	VOID FaInitNotifyIconData();
	afx_msg void OnBnClickedOk();
	CTabCtrl m_MainCTabCtrl;


	afx_msg void OnTcnSelchangeMainTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();

	afx_msg void OnIconNotify(WPARAM ParameterData1, LPARAM ParameterData2);
	afx_msg void OnIconNotifyExit();
	afx_msg void OnIconNotifyShow();

	CDriverDialog m_DriverDialog;
	CProcessDialog m_ProcessDialog;
	ULONG m_CurrentSelect;
};


