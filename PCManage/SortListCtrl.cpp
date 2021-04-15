// SortListCtrl.cpp : implementation file


#include <afxtempl.h>
#include "common.h"
#include "pch.h"
#include "SortListCtrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL IsNumber(LPCTSTR szText)
{
	CString sText;
	int nLen,nPotCount = 0;

	if(szText == NULL)
	{
		return FALSE;
	}

	sText = szText;
	sText.TrimRight();//
	nLen = sText.GetLength();
	if(nLen <= 0)
	{
		return FALSE;
	}

	if((sText[0] == _T('-')) || (sText[0] == _T('+')))
	{
		sText = sText.Mid(1);
		nLen --;
	}
	
	for(int i=0;i<nLen;i++)
	{
		if((sText[i] >= _T('0')) && (sText[i] <= _T('9')))
		{
			continue;
		}
		
		if(sText[i] == _T('.'))
		{
			if(++nPotCount <= 1)
			{
				continue;
			}
		}
		
		return FALSE;
	}
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl

CSortListCtrl::CSortListCtrl()
{
	m_IsInitialize = FALSE;
	m_IsEnableSort = TRUE;

}

CSortListCtrl::~CSortListCtrl()
{
	
}

BEGIN_MESSAGE_MAP(CSortListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSortListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl message handlers
//单击表头时进行排序
void CSortListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{


	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if(m_IsEnableSort == FALSE)
	{
		return;
	}
	
	CWnd *v1 = GetParent();
//	v1->SendMessage(WM_CLICK_LIST_HEADER_START);

	//生成列头标示升序降序的图标
	if(m_IsInitialize == FALSE)
	{
		m_IsInitialize = TRUE;
		m_ImageList.Create(8, 8, ILC_COLOR24|ILC_MASK, 2, 0);
		GetHeaderCtrl()->SetImageList(&m_ImageList);
	}

	CHeaderCtrl *HeaderCtrl = GetHeaderCtrl();
	
	int i;

	//清空非当前列的排序图标
	for(i = HeaderCtrl->GetItemCount()-1; i>=0; i--)
	{
		if(i != pNMListView->iSubItem)
			ChangeHeardCtrlState(HeaderCtrl, i, TRUE);
	}

	_S_ v2;
	v2.SortListCtrl = this;
	v2.SubItem = pNMListView->iSubItem;
	
	//得到当前列的升序降序
	v2.IsAscending = ChangeHeardCtrlState(HeaderCtrl, pNMListView->iSubItem, FALSE);
	
	//给各行设置itemData
	int ItemCount;
	CString ItemText;
	DWORD_PTR ItemData;
	CArray<DWORD_PTR> v5;
		
	ItemCount = GetItemCount();
	v2.IsNumber = TRUE;
	for(i=0; i<ItemCount; i++)
	{
		if(v2.IsNumber)
		{
			ItemText = GetItemText(i, pNMListView->iSubItem);
			if( (ItemText.IsEmpty() == FALSE) && (IsNumber(ItemText) == FALSE) )
			{
				v2.IsNumber = FALSE;
			}
		}

		ItemData = GetItemData(i);
		v5.Add(ItemData);
		SetItemData(i, i);
	}

	//如果只有一列,ListCtrl将不能正确排序，所以添加一临时列，这是ListCtrl的一个BUG。
	BOOL IsOnlyOneColumn = FALSE;
	if(HeaderCtrl->GetItemCount() == 1)
	{
		InsertColumn(1, _T(""));
		IsOnlyOneColumn = TRUE;
	}

	//排序
	SortItems(CompareProcedure, (DWORD_PTR)&v2);

	if(IsOnlyOneColumn)
	{
		DeleteColumn(1);//去掉临时列
	}
	
	//恢复itemData
	for(i=0; i<ItemCount; i++)
	{
		ItemData = GetItemData(i);
		SetItemData(i, v5[ItemData]);
	}

//	v1->SendMessage(WM_CLICK_LIST_HEADER_END);
}

int CALLBACK CSortListCtrl::CompareProcedure(LPARAM ParameterData1, LPARAM ParameterData2, LPARAM ParameterData3)
{
	_S_* v1;
	v1 = (_S_*)ParameterData3;
	
	//ParameterData1和ParameterData2实际上是itemData，因为OnColumnclick中设置过，这里直接当作行索引使用。
	//理论上这样做不保险，因为列表排序过程中行索引可能就变化了，但实际使用中未发现问题，可能MS
	//在排序过程中没有交换行，在排序结束后才交换了。
	CString Item1 = v1->SortListCtrl->GetItemText((int)ParameterData1, v1->SubItem);
	CString Item2 = v1->SortListCtrl->GetItemText((int)ParameterData2, v1->SubItem);
	
	if(v1->IsNumber)
	{
		//数字
		double v2 = _tstof(Item1) - _tstof(Item2);
		if(v2 == 0)
			return 0;

		if(v1->IsAscending)
			return (v2 < 0? -1 : 1);	
		else
			return (v2 < 0? 1 : -1);	
	}
	else
	{
		if(v1->IsAscending)
			return Item1.CompareNoCase(Item2);
		else
			return Item2.CompareNoCase(Item1);
	}
}


BOOL CSortListCtrl::ChangeHeardCtrlState(CHeaderCtrl* HeaderCtrl, int Item, BOOL IsClear)
{
	HDITEM v1;
	v1.mask = HDI_IMAGE | HDI_LPARAM | HDI_FORMAT;
	
	HeaderCtrl->GetItem(Item, &v1);
	BOOL IsAscending = (v1.lParam == 0 ? 1 : 0);

	if(!IsClear)
		v1.lParam = IsAscending;

	if(IsClear)
	{
		v1.fmt &= ~HDF_BITMAP_ON_RIGHT;
		v1.fmt &= ~HDF_IMAGE;
		v1.mask &= ~HDI_IMAGE;
	}
	else
	{
		v1.iImage = (IsAscending ? 0 : 1);
		v1.fmt |= HDF_BITMAP_ON_RIGHT|HDF_IMAGE;
	}
	
	HeaderCtrl->SetItem(Item,&v1);
	
	return IsAscending;

}

