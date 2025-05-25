#pragma once
#include "afxcmn.h"

#include <vector>
#include <strstream>

#include "../Util/CGmCfg.h"
#include "../Util/CConsoleMessage.h"
#include "../Database/DatabaseTable.h"
#include "../Database/ADO/ADOManager.h"
#include "../Database/ODBC/CGMOdbcManager.h"

#include "afxwin.h"

namespace gmce
{
	class IDBManager;
}

class CItemShopPage : public CFormView
{
	DECLARE_DYNCREATE(CItemShopPage)

protected:
	CItemShopPage();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CItemShopPage();

public:
	enum { IDD = IDD_MAINPAGE_ITEMSHOP };

	enum
    { 
        SEARCH_ITEMNAME=1,
		SEARCH_PRODNUM=2,
    };

    void SearchItemName(const CString& strUserName);
	void SearchProductNum(int nUserNum);

    void InitListHead();
    void FillData();

	CString m_strSearch;

    int     m_nRadio;    
	BOOL	m_bInit;
	SGMTOOL_SHOP_ITEM_VEC m_vecResult;


	gmce::IDBManager	*m_pDBMan;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	SGMTOOL_SHOP_ITEM_MAP gmtoolSHOPDATA;

	SGMTOOL_SHOP_ITEM* FindShopItem( DWORD _dwProductNum );
	BOOL DeleteShopItem( DWORD _dwProductNum );

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	CListCtrl m_ListSearch;
	CEdit m_EditSearch;
	afx_msg void OnBnClickedSearch();
	afx_msg void OnBnClickedRadioItemName();
	afx_msg void OnBnClickedRadioProductNum();
	afx_msg void OnLvnItemchangedListSearch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListSearch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListSearch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListSearch(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedItemshopPageButtonSearch();
	afx_msg void OnBnClickedSearch2();
	afx_msg void OnBnClicked1210();
};