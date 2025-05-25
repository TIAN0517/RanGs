#include "stdafx.h"
#include "FieldServerPostSender.h"

#include "ItemDialog.h"
#include "../RanLogic/Item/GLItemMan.h"
#include "../RanLogic/GLogicDataMan.h"
#include "../MfcExLib/ExLibs/EtcFunction.h"


// CItemDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFieldServerPostSender, CDialog)
CFieldServerPostSender::CFieldServerPostSender(CWnd* pParent, CFieldServer* pServer)
: CDialog(CFieldServerPostSender::IDD, pParent),
 g_pServer ( pServer )
{
}

CFieldServerPostSender::~CFieldServerPostSender()
{
}

void CFieldServerPostSender::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFieldServerPostSender, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedSend)
	ON_BN_CLICKED(IDC_BUTTON_ITEMSEARCH, OnBnClickedButtonFindItem)
END_MESSAGE_MAP()


// CItemDialog 메시지 처리기입니다.

BOOL CFieldServerPostSender::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}

void CFieldServerPostSender::OnBnClickedSend()
{
	if ( !g_pServer ) return;

	std::string strCharName = GetWin_Text( this, IDC_EDIT_CHANAME );
	std::string strPostContent = GetWin_Text( this, IDC_EDIT_CONTENT );
	std::string strSender = "System";
	std::string strTitle = "Gift";
	int CharNum = GetWin_Num_int( this, IDC_EDIT_CHANUM );

	if ( g_pServer->GetGLGaeaServer() ) 
	{
		BOOL bSendPost = g_pServer->GetGLGaeaServer()->PostSendSystemMail(CharNum,strCharName,sitemid.dwID,0,0,0xffffffff,strSender,strTitle,strPostContent);
		if ( bSendPost )
		{
			
		}
	}
		/*
		
		PostSendSystemMail(
		pChar->CharDbNum(),
		std::string( pChar->GetName() ),
		NSRanMobile::GLRanMobile::GetInstance().GetDailyItemID().dwID,
		0,
		ATTENDANCE_REWARD_POST_DESIGN,
		ATTENDANCE_REWARD_POST_SENDER,
		std::string( ID2SERVERTEXT( "SYSTEM_POST_SENDER_NAME", 0 ) ),
		std::string( ID2SERVERTEXT( "TEXT_RANMOBILE_DAILYREWARD_TITLE", 0 ) ),
		std::string( ID2SERVERTEXT( "TEXT_RANMOBILE_DAILYREWARD_CONTENTS", 0 ) ) );
		
		*/

	OnOK();
}

void CFieldServerPostSender::OnBnClickedButtonFindItem()
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

