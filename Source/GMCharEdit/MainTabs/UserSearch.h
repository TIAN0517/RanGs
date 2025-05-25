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

class CUserSearch : public CFormView
{
	DECLARE_DYNCREATE(CUserSearch)

protected:
	CUserSearch();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CUserSearch();

public:
	enum { IDD = IDD_USERSEARCH };

	enum
    { 
        SEARCH_USERNAME=1,
		SEARCH_USERNUM=2,
		SEARCH_USERIP=3,
    };

    void SearchUserName(const CString& strUserName);
	void SearchUserNum(int nUserNum);
	void SearchUserByIP(std::string strIP);

    void InitListHead();
    void FillData();

	CString m_strSearch;

    int     m_nRadio;    
	BOOL	m_bInit;
	std::vector<UserInfo> m_vUserInfo;

	gmce::IDBManager	*m_pDBMan;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	CListCtrl m_ListSearch;
	CEdit m_EditSearch;
	afx_msg void OnBnClickedSearch();
	afx_msg void OnBnClickedRadioUserName();
	afx_msg void OnBnClickedRadioUsernum();
	afx_msg void OnBnClickedRadioIP();
	afx_msg void OnLvnItemchangedListSearch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListSearch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListSearch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContext1Makecroncharacter();
	afx_msg void OnNMCustomdrawListSearch(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};