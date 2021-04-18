#pragma once
#include "ProcessHelper.h"
#include "FileHelper.h"
#include "SystemHelper.h"
#include "SortListCtrl.h"
#include "InjectionHelper.h"

// CProcessDialog 对话框

typedef enum _COLOR_TYPE_
{
	BLACK = 0,	//微软的模块
	RED,			//危险的项
	BLUE,			//其他的非微软模块
}COLOR_TYPE;


class CProcessDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessDialog)

public:
	CProcessDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CProcessDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CString ProcessIdentify, ParentProcessIdentify, IsRing3Access;
	CString m_TotalCStatic;
	ULONG m_ProcessCount;//总的进程
	ULONG m_DenyAccessCount;//拒绝进程
	virtual BOOL OnInitDialog();
	//CListCtrl m_ProcessCListCtrl;
	CSortListCtrl m_ProcessCListCtrl;
	CImageList m_ProcessImageList;
	CString m_ProcessCEdit;
	HICON m_ProcessIcon;
	vector<COLOR_TYPE> m_ColorListCtrl;

	vector<PROCESS_TABLE_ENTRY_INFO> m_Ring3ProcessList;
	void CProcessDialog::FaSetProcessList();
	void CProcessDialog::FaSortProcessList(vector<PROCESS_TABLE_ENTRY_INFO>& ParameterData);
	void CProcessDialog::FaAddProcessItem(PROCESS_TABLE_ENTRY_INFO ParameterData);

	void CProcessDialog::OnKillProcess();
	void CProcessDialog::OnKillDeleteProcess();
	void CProcessDialog::OnNMCustomdrawProcessList(NMHDR* pNMHDR, LRESULT* pResult);
	int CProcessDialog::FaGetSelectedItem(CListCtrl* ParameterData);
	PPROCESS_TABLE_ENTRY_INFO CProcessDialog::FaGetProcessInfoByItem(int SelectedItem);

	ACTIVITY_STATUS CProcessDialog::FaGetProcessStatus();

	void CProcessDialog::OnShowProcessInfo(); 
	void CProcessDialog::OnShowProcessDetailInfo();

	afx_msg LRESULT OnUpdateProcessData(WPARAM ParameterData1, LPARAM ParameterData2);

	afx_msg void OnRefreshProcess();
	afx_msg void OnNMRClickProcessList(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnSuspendProcess();
	afx_msg void OnResumeProcess();
	afx_msg void OnCopyImageName();
	afx_msg void OnCopyImagePath();
	afx_msg void OnCheckFileAttributes();
	afx_msg void OnLocationAtExplorer();

	afx_msg void OnExportText();
	afx_msg void OnInjectModule1();


	
	
	
};
