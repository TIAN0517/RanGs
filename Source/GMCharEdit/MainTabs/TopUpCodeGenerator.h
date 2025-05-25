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

class CTopUpCodeGenerator : public CFormView
{
	DECLARE_DYNCREATE(CTopUpCodeGenerator)

protected:
	CTopUpCodeGenerator();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CTopUpCodeGenerator();

public:
	enum { IDD = IDD_TOPUPCODEGENERATOR };

	enum
    { 
        SEARCH_CODEPINUSER=1,
        SEARCH_NUMBER=2,
    };

    void SearchUsed(const CString& strUsed);
    void SearchUsed(int nUsed);
    void SearchUnUsed(const CString& strUnUsed);
    void SearchUnUsed(int nUnUsed);

    void InitListHead();
    void FillData();

	CString m_strSearch;

    int     m_nRadio;    
	BOOL	m_bInit;
	std::vector<TopUpCodesInfo> m_vTopUpCodesInfo;

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
	CEdit m_EditGenNum;
	CEdit m_EditGenValue;
	CButton   m_CheckUsed;
	afx_msg void OnBnClickedSearch();
	afx_msg void OnBnClickedRadioUserName();
	afx_msg void OnBnClickedRadioUsernum();
	afx_msg void OnLvnItemchangedListSearch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListSearch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListSearch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContext1Makecroncharacter();
	afx_msg void OnNMCustomdrawListSearch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedGenerateCodes();
	
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};