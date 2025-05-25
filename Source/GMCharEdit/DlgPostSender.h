#pragma once
#include <map>
#include <vector>
#include <strstream>

#include "./Util/CGmCfg.h"
#include "./Util/CConsoleMessage.h"
#include "./Database/DatabaseTable.h"
#include "./Database/ADO/ADOManager.h"
#include "./Database/ODBC/CGMOdbcManager.h"
#include "../../RanLogic/GLogicEx.h"
#include "../../RanLogic/Character/GLCharData.h"

namespace gmce
{
	class IDBManager;
}


// CDlgPostSender dialog

class CDlgPostSender : public CDialog
{
	DECLARE_DYNAMIC(CDlgPostSender)

public:
	CDlgPostSender(int nChaNum, int nUserNumber, UINT nIDCaption,    CWnd* pParentWnd = NULL);
	CDlgPostSender(int nChaNum, int nUserNumber, LPCTSTR pszCaption, CWnd* pParentWnd = NULL);
	virtual ~CDlgPostSender();

	BOOL Create(CWnd * pWnd){ 
		return CDialog::Create( CDlgPostSender::IDD, pWnd );
	}

// Dialog Data
	enum { IDD = IDD_DIALOG_POST_SENDER };

public:
    int m_nChaNum;
	int m_nUserNumber;
	GLCHARLOGIC* m_pChar;
	SNATIVEID	sitemid;

	gmce::IDBManager *m_pDBMan;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()

public:
	CWnd*		m_pParentWnd;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonItemsearch();
	afx_msg void OnBnClickedOk();
};
