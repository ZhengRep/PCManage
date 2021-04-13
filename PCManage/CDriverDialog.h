#pragma once


// CDriverDialog 对话框

class CDriverDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CDriverDialog)

public:
	CDriverDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDriverDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRIVER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_DriverCListCtrl;
};
