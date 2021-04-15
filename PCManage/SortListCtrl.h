
#ifndef _SORTLISTCTRL_INCLUDE__
#define _SORTLISTCTRL_INCLUDE__

#include<iostream>
#include<tchar.h>
#include<windows.h>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////
//һ�������ͷʱ����������б���
class CSortListCtrl : public CListCtrl
{
// Construction
public:
	CSortListCtrl();
	void EnableSort(BOOL IsEnableSort = TRUE){m_IsEnableSort = IsEnableSort;};
// Attributes
public:
	struct _S_
	{
		CSortListCtrl* SortListCtrl;
		int  SubItem;
		BOOL IsAscending;		//�Ƿ�������
		BOOL IsNumber;	        //�����Ƿ�������
	};

	// Summary: �ı���ͷͼ�������״̬���������µ�����״̬��IsClearΪ��ʱ�����ͷͼ�ꡣ
	BOOL ChangeHeardCtrlState(CHeaderCtrl* HeaderCtrl, int Item, BOOL IsClear);

	static int CALLBACK CompareProcedure(LPARAM ParameterData1, LPARAM ParameterData2, LPARAM ParameterData3);

// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSortListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSortListCtrl();

	// Generated message map functions
protected:

	CImageList m_ImageList;
	BOOL m_IsInitialize;
	BOOL m_IsEnableSort;

public:
	//{{AFX_MSG(CSortListCtrl)
		afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif