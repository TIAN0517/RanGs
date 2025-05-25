// DlgPostSender.cpp : implementation file
//

#include "stdafx.h"
#include "GMCharEdit.h"
#include "DlgPostSender.h"
#include "../../MfcExLib/ExLibs/EtcFunction.h"
#include "ItemDialog.h"

#include "../../SigmaCore/Util/DateTime.h"
#include "../../SigmaCore/Math/SeqUniqueGuid.h"

#include "../../RanLogic/Item/GLItemCustom.h"
#include "../../RanLogic/GLogicDataMan.h"
#include "../../RanLogic/Item/GLItem.h"
#include "../../RanLogic/Item/GLItemMan.h"

#include "../../EngineLib/GUInterface/GameTextControl.h"

// ----------------------------------------------------------------------------
#include "../../SigmaCore/DebugInclude.h"
// ----------------------------------------------------------------------------


#define SYSTEM_REWARD_POST_SENDER		0xffffffff	//Max. of UINT (reserved ChaNum for SystemMail)
#define SYSTEM_REWARD_POST_DESIGN		0


// CDlgPostSender dialog

IMPLEMENT_DYNAMIC(CDlgPostSender, CDialog)
CDlgPostSender::CDlgPostSender(int nChaNum, int nUserNumber, UINT nIDCaption, CWnd* pParentWnd /* = NULL */)
	: CDialog(CDlgPostSender::IDD, pParentWnd)
{
    m_nChaNum = nChaNum;
	m_nUserNumber = nUserNumber;
	m_pParentWnd = pParentWnd;
}

CDlgPostSender::CDlgPostSender(int nChaNum, int nUserNumber, LPCTSTR pszCaption, CWnd* pParentWnd /* = NULL */)
	: CDialog(CDlgPostSender::IDD, pParentWnd)
{
    m_nChaNum = nChaNum;
    m_nUserNumber = nUserNumber;
	m_pParentWnd = pParentWnd;
}

CDlgPostSender::~CDlgPostSender()
{
	SAFE_DELETE ( m_pChar );
}

void CDlgPostSender::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgPostSender, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ITEMSEARCH, &CDlgPostSender::OnBnClickedButtonItemsearch)
	ON_BN_CLICKED(IDOK, &CDlgPostSender::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgPostSender message handlers
BOOL CDlgPostSender::OnInitDialog()
{
	bool bResult = CDialog::OnInitDialog();

	m_pDBMan = CGmCfg::GetInstance()->GetDBMan();//CADOManager::GetInstance();

	SetWin_Num_int( this, IDC_EDIT_CHANUM, m_nChaNum );
	//
	//mjeon.ado.Vehicle information will be processed in it's own PropertyPage(PropertyVehicle).
	//
	m_pChar = new GLCHARLOGIC;
	if (m_pDBMan->GetCharacterInfo(m_nChaNum, m_nUserNumber, m_pChar) == DB_ERROR)
	{
		MessageBox(_T("Database Error"));
	}
	else if (m_pDBMan->GetUserInven(m_pChar) == DB_ERROR)
	{
		MessageBox(_T("Database Error"));
	}
	else
	{

	}
	SetWin_Text( this, IDC_EDIT_CHANAME, m_pChar->GetName() );
  
    return bResult;
}

void CDlgPostSender::OnBnClickedButtonItemsearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CItemDialog dlg;
	if ( dlg.DoModal() )
	{
		SetWin_Num_int( this, IDC_EDIT_MID, dlg.m_nidITEM.wMainID );
		SetWin_Num_int( this, IDC_EDIT_SID, dlg.m_nidITEM.wSubID );

		SITEM* pItem = GLItemMan::GetInstance().GetItem(dlg.m_nidITEM);
		if ( pItem )
		{
			SetWin_Text( this, IDC_EDIT_ITEMNAME, pItem->GetName() );
			sitemid = dlg.m_nidITEM;
		}
	}
}

void CDlgPostSender::OnBnClickedOk()
{
	SPOSTINFO PostInfo;
	ZeroMemory(&PostInfo, sizeof(SPOSTINFO));

	PostInfo.byPostState	= EMPOST_UNREAD;

	const DWORD dwAttachedItemID = sitemid.dwID;
	if (dwAttachedItemID != SNATIVEID(false).dwID)
	{
		PostInfo.bAttach01 = TRUE;
		PostInfo.byPostType		= EMPOST_ITEM;
	}

	PostInfo.byPostClass	= EMPOST_SYSTEM;
	PostInfo.iSender		= SYSTEM_REWARD_POST_SENDER;
	PostInfo.iRecver		= m_nChaNum;

	StringCchCopy (PostInfo.RecverName, sizeof(PostInfo.RecverName), m_pChar->GetName());
	StringCchCopy (PostInfo.SenderName, sizeof(PostInfo.SenderName), "Post Item Sender" );

	PostInfo.iDesign		= SYSTEM_REWARD_POST_DESIGN;
	PostInfo.llCharge		= 0;
	PostInfo.llAttachMoney = GetWin_Num_int(this , IDC_EDIT_MONEY );
	StringCchCopy (PostInfo.Title, sizeof(PostInfo.Title), "Congrats!" );
	StringCchCopy (PostInfo.Content, sizeof(PostInfo.Content), GetWin_Text(this, IDC_EDIT_CONTENT));

	// Attach01	
	PostInfo.idAttach01 = dwAttachedItemID;

	//
	// 1.Does the post have valid title and content? not length 0.
	//
	if ( PostInfo.Title[0] == NULL || PostInfo.Content[0] == NULL )
	{
		return;	
	}	

	if( PostInfo.bAttach01 )
	{
		//
		// 2.Check whether the attachment is valid or not
		//
		const SITEM* pITEM = GLogicData::GetInstance().GetItem( PostInfo.idAttach01 );

		if ( !pITEM )
		{
			PostInfo.bAttach01 = FALSE;
		}
		else
			PostInfo.bAttach01 = TRUE;
	}


	SITEMCUSTOM itemcustomNew;
	CTime Time( CTime::GetCurrentTime() );

	// 불필요하게 복사하지 않도록 수정함
	itemcustomNew.Initialize( PostInfo.idAttach01 );
	itemcustomNew.BornTimeSet( Time.GetTime() );
	itemcustomNew.GenTypeSet( EMGEN_SYSTEM );
	// NewItem.ChannelNumSet( static_cast< BYTE >( m_nServerChannel ) );
	// NewItem.FieldNumSet( static_cast< BYTE >( m_dwFieldSvrID ) );
	sc::SeqUniqueGUID::Instance()->getGUID( itemcustomNew.guid );
	itemcustomNew.GenerateBasicStat( false );
	itemcustomNew.GenerateLinkSkill();
	itemcustomNew.GenerateAddOption();

	int nREsult = m_pDBMan->PostSend(PostInfo,itemcustomNew);
	if ( nREsult == DB_OK )	OnOK();
}


void CDlgPostSender::PostNcDestroy()
{
	CDialog::PostNcDestroy();

	delete this;
}