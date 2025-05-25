// DlgUserEdit.cpp : implementation file
//

#include "stdafx.h"
#include "../GMCharEdit.h"
#include "DlgUserEdit.h"
//#include "EtcFunction.h"

#include "../../MfcExLib/ExLibs/EtcFunction.h"


// ----------------------------------------------------------------------------
#include "../../SigmaCore/DebugInclude.h"
// ----------------------------------------------------------------------------


// CDlgUserEdit dialog

IMPLEMENT_DYNAMIC(CDlgUserEdit, CDialog)

USERLEVEL_INDEX UserTypeToIndex( EMUSERTYPE emTYPE )
{
	if ( emTYPE >= USER_MASTER ){
		return USERLEVEL_INDEX_ADMINISTRATOR;
	}else if ( emTYPE >= USER_GM3 && emTYPE < USER_MASTER ){
		return USERLEVEL_INDEX_GAMEMASTER;
	}else /*if ( emTYPE >= USER_COMMON && emTYPE < USER_GM3 )*/{
		return USERLEVEL_INDEX_NORMAL;
	}
};

EMUSERTYPE UserIndexToType( USERLEVEL_INDEX emINDEX )
{
	if ( emINDEX == USERLEVEL_INDEX_ADMINISTRATOR ){
		return USER_MASTER;
	}else if ( emINDEX == USERLEVEL_INDEX_GAMEMASTER ){
		return USER_GM3;
	}else {
		return USER_COMMON;	
	}
};

std::string	strUserLevelIndex[USERLEVEL_INDEX_SIZE] =
{
	_T("Common User"),
	_T("GameMaster"),
	_T("Administrator"),
};

std::string	strUserStatus[GMTOOL_USER_STATUS_SIZE] =
{
	_T("Offline"),
	_T("Online"),
};

std::string	strUserLoginType[GMTOOL_USER_LOGINTYPE_SIZE] =
{
	_T("Logout"),
	_T("Login"),
};

std::string	strBasic[GMTOOL_BASIC_SIZE] =
{
	_T("No"),
	_T("Yes"),
};

CDlgUserEdit::CDlgUserEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUserEdit::IDD, pParent)
	, m_pParentWnd(NULL)
	, m_nDialogID(-1)
	, m_dwID(0)
	, m_strNAME("")
	, m_pData(NULL)
{

}

CDlgUserEdit::CDlgUserEdit(int nDialogID, DWORD dwID, std::string strNAME, CWnd* pParent /*= NULL*/)
	: CDialog(CDlgUserEdit::IDD, pParent)
	, m_pParentWnd(pParent)
	, m_nDialogID(nDialogID)
	, m_dwID(dwID)
	, m_strNAME(strNAME)
	, m_pData(NULL)
{
	m_pDBMan = CGmCfg::GetInstance()->GetDBMan();//CADOManager::GetInstance();
}

CDlgUserEdit::~CDlgUserEdit()
{
	SAFE_DELETE( m_pData );
}

void CDlgUserEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IP, m_list_ip );
}


BEGIN_MESSAGE_MAP(CDlgUserEdit, CDialog)
	ON_BN_CLICKED(IDC_USEREDIT_BUTTON_MD5, OnBnClickedUsereditButtonMd5)
	ON_BN_CLICKED(IDC_USEREDIT_BUTTON_MD52, OnBnClickedUsereditButtonMd52)
	ON_BN_CLICKED(IDC_USEREDIT_BUTTON_USERBLOCK_SET, OnBnClickedUsereditButtonUserblockSet)
	ON_BN_CLICKED(IDC_USEREDIT_BUTTON_USERBLOCK_RESET, OnBnClickedUsereditButtonUserblockReset)
	ON_BN_CLICKED(IDC_USEREDIT_BUTTON_CHATBLOCK_SET, OnBnClickedUsereditButtonChatblockSet)
	ON_BN_CLICKED(IDC_USEREDIT_BUTTON_CHATBLOCK_RESET, OnBnClickedUsereditButtonChatblockReset)
	ON_BN_CLICKED(IDC_USEREDIT_BUTTON_PREMIUMDATE_SET, OnBnClickedUsereditButtonPremiumdateSet)
	ON_BN_CLICKED(IDC_USEREDIT_BUTTON_PREMIUMDATE_RESET, OnBnClickedUsereditButtonPremiumdateReset)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK2, OnBnClickedOk2)
	
END_MESSAGE_MAP()


// CDlgUserEdit message handlers
BOOL CDlgUserEdit::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strWindowText;
	strWindowText.Format( "Edit User:(%d) %s", m_dwID, m_strNAME.c_str() );
	SetWindowText( strWindowText.GetString() );

	std::string	strUserLevel[USERLEVEL_INDEX_SIZE];
	for( int i=0; i<USERLEVEL_INDEX_SIZE; ++i )
	{
		CString strText;
		strText.Format( "(%d)%s", UserIndexToType((USERLEVEL_INDEX)i), strUserLevelIndex[i].c_str());
		strUserLevel[i] = strText.GetString();
	}

	SetWin_Combo_Init( this, IDC_USEREDIT_COMBO_USERTYPE, strUserLevel, USERLEVEL_INDEX_SIZE );
	SetWin_Combo_Init( this, IDC_USEREDIT_COMBO_LOGINSTATE, strUserStatus, GMTOOL_USER_STATUS_SIZE );
	SetWin_Combo_Init( this, IDC_USEREDIT_COMBO_AVAILABLE, strBasic, GMTOOL_BASIC_SIZE );
	SetWin_Combo_Init( this, IDC_USEREDIT_COMBO_USERBLOCK, strBasic, GMTOOL_BASIC_SIZE );

	m_pData = new SGMTOOL_DATA_USER;

	m_pData->dwUserNum = m_dwID;
	StringCchCopy ( m_pData->szUserID, USR_ID_LENGTH+1, m_strNAME.c_str() );

	int nRetCode = m_pDBMan->SearchUssr( m_pData );
	DataDefault();

	if (nRetCode == DB_ERROR) 
	{
		MessageBox(_T("Database Error"));
		return TRUE;  
	}
	else
	{
		if ( !m_pData )
		{
			MessageBox(_T("No Result"));	
			return TRUE;  
		}
		else
		{
			DataShow();
		}
	}

	{
		RECT rectCtrl;
		LONG lnWidth;
		LV_COLUMN lvColumn;

		m_list_ip.SetExtendedStyle ( m_list_ip.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
		m_list_ip.GetClientRect ( &rectCtrl );
		lnWidth = rectCtrl.right - rectCtrl.left;

		const int nColumnCount = 4;	
		char* szColumnName1[nColumnCount] = { "","IP","Login Date",""};
		int nColumnWidthPercent[nColumnCount] = { 1, 43, 43, 1 };

		for ( int i = 0; i < nColumnCount; i++ )
		{		
			lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvColumn.fmt = LVCFMT_LEFT;
			lvColumn.pszText = szColumnName1[i];
			lvColumn.iSubItem = i;
			lvColumn.cx = ( lnWidth*nColumnWidthPercent[i] ) / 100;
			m_list_ip.InsertColumn (i, &lvColumn );
		}
	}

	int sNum = 0;
	char szTempChar[256];
	CString strCombine;
	CString strDate;
	m_list_ip.DeleteAllItems();
	if ( m_pData->m_vecIP.size() > 0 )
		for ( ; sNum < m_pData->m_vecIP.size() && sNum <= 200; )
		{
			m_list_ip.InsertItem(sNum, "" );
			strCombine.Format (m_pData->m_vecIP[sNum].c_str() );
			m_list_ip.SetItemText(sNum,1,strCombine.GetString() );

			strDate.Format (m_pData->m_LogDate[sNum].c_str() );
			m_list_ip.SetItemText(sNum,2,strDate.GetString() );

			m_list_ip.SetItemText(sNum,3, " " );
			sNum++;
		}
	
	return TRUE;  
}

void CDlgUserEdit::PostNcDestroy()
{
	CDialog::PostNcDestroy();

	delete this;
}

void CDlgUserEdit::OnOK()
{
	if(UpdateData(true)){
		DestroyWindow();
	}
}

void CDlgUserEdit::OnCancel()
{
	DestroyWindow();
}

void CDlgUserEdit::DataDefault()
{
	SetWin_Num_int( this, IDC_USEREDIT_EDIT_NUM, 0 );
	SetWin_Text( this, IDC_USEREDIT_EDIT_NAME, "" );
	SetWin_Text( this, IDC_USEREDIT_EDIT_PASS, "" );
	SetWin_Text( this, IDC_USEREDIT_EDIT_PASS2, "" );
	SetWin_Text( this, IDC_USEREDIT_EDIT_EMAIL, "" );
	SetWin_Text( this, IDC_USEREDIT_EDIT_USERBLOCK, "" );
	SetWin_Text( this, IDC_USEREDIT_EDIT_CHATBLOCK, "" );
	SetWin_Text( this, IDC_USEREDIT_EDIT_PREMIUMDATE, "" );
	SetWin_Text( this, IDC_USEREDIT_EDIT_CREATEDATE, "" );
	SetWin_Text( this, IDC_USEREDIT_EDIT_LOGINDATE, "" );
	SetWin_Num_int( this, IDC_USEREDIT_EDIT_SERVERNUMBER, 0 );
	SetWin_Num_int( this, IDC_USEREDIT_EDIT_SERVERGROUP, 0 );
	SetWin_Num_int( this, IDC_USEREDIT_EDIT_CHAREMAIN, 0 );
	SetWin_Num_int( this, IDC_USEREDIT_EDIT_USERPOINT, 0 );
	SetWin_Num_int( this, IDC_USEREDIT_EDIT_USERPOINT2, 0 );
	SetWin_Combo_Sel( this, IDC_USEREDIT_COMBO_USERTYPE, 0 );
	SetWin_Combo_Sel( this, IDC_USEREDIT_COMBO_LOGINSTATE, 0 );
	SetWin_Combo_Sel( this, IDC_USEREDIT_COMBO_AVAILABLE, 0 );
	SetWin_Combo_Sel( this, IDC_USEREDIT_COMBO_USERBLOCK, 0);
}

void CDlgUserEdit::DataShow()
{
	if ( !m_pData ){
		return;
	}

	SetWin_Num_int( this, IDC_USEREDIT_EDIT_NUM, m_pData->dwUserNum );
	SetWin_Text( this, IDC_USEREDIT_EDIT_NAME, m_pData->szUserID );
	SetWin_Text( this, IDC_USEREDIT_EDIT_PASS, m_pData->szUserPass );
	SetWin_Text( this, IDC_USEREDIT_EDIT_PASS2, m_pData->szUserPass2 );
	SetWin_Text( this, IDC_USEREDIT_EDIT_EMAIL, m_pData->szUserEmail );
	SetWin_Num_int( this, IDC_USEREDIT_EDIT_SERVERNUMBER, m_pData->wServerNumber );
	SetWin_Num_int( this, IDC_USEREDIT_EDIT_SERVERGROUP, m_pData->wServerGroup );
	SetWin_Num_int( this, IDC_USEREDIT_EDIT_CHAREMAIN, m_pData->wChaRemain );
	SetWin_Num_int( this, IDC_USEREDIT_EDIT_USERPOINT, m_pData->dwUserPoint );
	SetWin_Num_int( this, IDC_USEREDIT_EDIT_USERPOINT2, m_pData->dwUserPoint2 );
	SetWin_Combo_Sel( this, IDC_USEREDIT_COMBO_USERTYPE, UserTypeToIndex( (EMUSERTYPE)m_pData->wUserType ) );
	SetWin_Combo_Sel( this, IDC_USEREDIT_COMBO_LOGINSTATE, strUserStatus[m_pData->bUserLoginState].c_str() );
	SetWin_Combo_Sel( this, IDC_USEREDIT_COMBO_AVAILABLE, strBasic[m_pData->bUserAvailable].c_str() );

}

BOOL CDlgUserEdit::DataSave()
{
	if ( !m_pData ){
		return FALSE;
	}

	BOOL bONLINE = FALSE;

	std::string strPass = GetWin_Text( this, IDC_USEREDIT_EDIT_PASS );
	std::string strPass2 = GetWin_Text( this, IDC_USEREDIT_EDIT_PASS2 );
	std::string strEmail = GetWin_Text( this, IDC_USEREDIT_EDIT_EMAIL );
	int nCharRemain = GetWin_Num_int( this, IDC_USEREDIT_EDIT_CHAREMAIN );
	int nUserPoints = GetWin_Num_int( this, IDC_USEREDIT_EDIT_USERPOINT );
	int nUserPoints2 = GetWin_Num_int( this, IDC_USEREDIT_EDIT_USERPOINT2 );
	BOOL bLoginState = GetWin_Combo_Sel( this, IDC_USEREDIT_COMBO_LOGINSTATE );
	BOOL bAvailable = GetWin_Combo_Sel( this, IDC_USEREDIT_COMBO_AVAILABLE );
	BOOL bUserBlock = GetWin_Combo_Sel( this, IDC_USEREDIT_COMBO_USERBLOCK );
	USERLEVEL_INDEX emUserTypeIndex = static_cast<USERLEVEL_INDEX> ( GetWin_Combo_Sel( this, IDC_USEREDIT_COMBO_USERTYPE ) );

	if ( strPass.size() < MIN_USER_PASS || strPass.size() > MAX_USER_PASS ){
		MessageBox ( sc::string::format("UserPass can only have %d to %d characters", MIN_USER_PASS, MAX_USER_PASS).c_str() );
		GotoDlgCtrl(GetDlgItem(IDC_USEREDIT_EDIT_PASS)); 
		return FALSE;
	}

	if ( strPass2.size() < MIN_USER_PASS || strPass2.size() > MAX_USER_PASS ){
		MessageBox ( sc::string::format("UserPass2 can only have %d to %d characters", MIN_USER_PASS, MAX_USER_PASS).c_str() );
		GotoDlgCtrl(GetDlgItem(IDC_USEREDIT_EDIT_PASS2)); 
		return FALSE;
	}

	if ( strEmail.size() > USR_EMAIL_LENGTH ){
		MessageBox ( sc::string::format("UserEmail can only have 0 to %d characters", USR_EMAIL_LENGTH).c_str() );
		GotoDlgCtrl(GetDlgItem(IDC_USEREDIT_EDIT_EMAIL)); 
		return FALSE;
	}

	if ( nCharRemain < 0 || nCharRemain >= MAX_ONESERVERCHAR_NUM ){
		MessageBox ( sc::string::format("ChaRemain can only have 0 to %d value", MAX_ONESERVERCHAR_NUM).c_str() );
		GotoDlgCtrl(GetDlgItem(IDC_USEREDIT_EDIT_CHAREMAIN)); 
		return FALSE;
	}

	if ( nUserPoints < 0 ){
		MessageBox ( "User Points invalid" );
		GotoDlgCtrl(GetDlgItem(IDC_USEREDIT_EDIT_USERPOINT)); 
		return FALSE;
	}

	if ( nUserPoints2 < 0 ){
		MessageBox ( "User Points2 invalid" );
		GotoDlgCtrl(GetDlgItem(IDC_USEREDIT_EDIT_USERPOINT2)); 
		return FALSE;
	}

	StringCchCopy( m_pData->szUserPass, USR_PASS_LENGTH, strPass.c_str() );
	StringCchCopy( m_pData->szUserPass2, USR_PASS_LENGTH, strPass2.c_str() );
	StringCchCopy( m_pData->szUserEmail, USR_EMAIL_LENGTH, strEmail.c_str() );
	m_pData->wUserType = UserIndexToType( emUserTypeIndex );
	m_pData->wChaRemain = (WORD)nCharRemain;
	m_pData->dwUserPoint = (WORD)nUserPoints;
	m_pData->dwUserPoint2 = (WORD)nUserPoints2;
	m_pData->bUserLoginState = bLoginState;
	m_pData->bUserAvailable = bAvailable;
	m_pData->bUserBlock = bUserBlock;

	int nRetCode = m_pDBMan->SaveUser(*m_pData);

	if (nRetCode == DB_ERROR) 
	{
		MessageBox(_T("Database Error, Unable to Save UserData!"));
		return FALSE;
	}

	nRetCode = m_pDBMan->SearchUssr( m_pData );
	DataDefault();

	if (nRetCode == DB_ERROR) 
	{
		return FALSE;
	}
	else
	{
		MessageBox(_T("Succeeded"));
	}

	return TRUE;
}
void CDlgUserEdit::OnBnClickedOk()
{
	if ( DataSave()){
		OnOK();
	}
}

void CDlgUserEdit::OnBnClickedCancel()
{
	OnCancel();
}

void CDlgUserEdit::OnBnClickedOk2()
{
	if ( DataSave()){
		MessageBox ( "Save Done!","Saving....", MB_OK );
	}
}

void CDlgUserEdit::OnBnClickedUsereditButtonMd5(){}
void CDlgUserEdit::OnBnClickedUsereditButtonMd52(){}
void CDlgUserEdit::OnBnClickedUsereditButtonUserblockSet(){}
void CDlgUserEdit::OnBnClickedUsereditButtonUserblockReset(){}
void CDlgUserEdit::OnBnClickedUsereditButtonChatblockSet(){}
void CDlgUserEdit::OnBnClickedUsereditButtonChatblockReset(){}
void CDlgUserEdit::OnBnClickedUsereditButtonPremiumdateSet(){}
void CDlgUserEdit::OnBnClickedUsereditButtonPremiumdateReset(){}