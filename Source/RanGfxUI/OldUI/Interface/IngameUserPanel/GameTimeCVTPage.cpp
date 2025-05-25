#include "StdAfx.h"
#include "GameTimeCVTPage.h"
#include "../../../../SigmaCore/DebugSet.h"
#include "../../../../SigmaCore/String/StringFormat.h"
#include "../../../../SigmaCore/String/StringUtils.h"

#include "../../../../EngineLib/GUInterface/BasicTextButton.h"
#include "../../../../EngineLib/GUInterface/GameTextControl.h"
#include "../../../../EngineLib/GUInterface/BasicTextBoxEx.h"
#include "../../../../EngineLib/GUInterface/UITextControl.h"
#include "../../../../EngineLib/GUInterface/BasicLineBox.h"
#include "../../../../EngineLib/DxTools/DxFontMan.h"
#include "../../../../EngineLib/GUInterface/UIEditBoxMan.h"
#include "../../../../EngineLib/GUInterface/UIKeyCheck.h"
#include "../../../../EngineLib/Common/DXInputString.h"
#include "../../../../EngineLib/DxResponseMan.h"
#include "../../../../EngineLib/DxTools/RENDERPARAM.h"


#include "../../../../RanLogicClient/Network/s_NetClient.h"
#include "../../../../RanLogicClient/Stage/DxGlobalStage.h"
#include "../../../../RanLogicClient/GLGaeaClient.h"

#include "../../../../RanLogic/RANPARAM.h"

#include "../../../OuterInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int	CGameTimeCVTPage::nLIMIT_CP = 7;

CGameTimeCVTPage::CGameTimeCVTPage (GLGaeaClient* pGaeaClient, COuterInterface* pInterface, EngineDeviceMan* pEngineDevice)
	: CUIOuterWindow(pInterface, pEngineDevice)
	, m_pGaeaClient(pGaeaClient)
	, m_pInterface ( pInterface )
	,  m_pRandTextBox(NULL)
	, m_nRandPassNumber(0)
	, m_nRPUpdateCnt(0)
{
	memset( m_szRandomString, 0, sizeof( m_szRandomString ) );
}

CGameTimeCVTPage::~CGameTimeCVTPage ()
{
}

void CGameTimeCVTPage::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9,_DEFAULT_FONT_FLAG );

	const int nBUTTONSIZE = CBasicTextButton::SIZE18;

	CBasicTextBox* pTextBox = NULL;
	WORD dumm = (WORD)60,dumm2 = (WORD)3;
	const DWORD dwGameTime = m_pInterface->GetGameTime();
	WORD wMinutes = dwGameTime / 60;
	WORD wTotal = wMinutes;
	WORD wMinutesTotal = dwGameTime-60*wTotal;
	WORD wPMinutes = dwGameTime/dumm;
	WORD wPTotal = wPMinutes * dumm2;
	CString strText,strText2;
	pTextBox = CreateStaticControl ( "GTCVT_PAGE_STATIC", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT );
	pTextBox->SetOneLineText ( (char*)ID2GAMEWORD ( "GTCVT_PAGE_INFO", 0 ) );
	pTextBox = CreateStaticControl ( "GTCVT_PAGE_STATIC1", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT );
	if ( dwGameTime == 0 )
	{
		strText.Format("You haven't yet collected GameTime");
	}
	else if ( wTotal<=0 )
	{
		strText.Format((char*)ID2GAMEWORD ( "GTCVT_PAGE_INFO", 1 ) ,dwGameTime);
		strText += "Min(s)";
	}else{
		strText.Format((char*)ID2GAMEWORD ( "GTCVT_PAGE_INFO", 1 ) ,wTotal);
		strText += "hr(s) ";
	}
	pTextBox->SetOneLineText ( strText );
	m_pStaticText2Box = pTextBox;
	pTextBox = CreateStaticControl ( "GTCVT_PAGE_STATIC2", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT );
	strText.Format((char*)ID2GAMEWORD ( "GTCVT_PAGE_INFO", 2 ) ,dumm ,dumm2 );
	pTextBox->SetOneLineText ( strText );
	m_pStaticText3Box = pTextBox;
	pTextBox = CreateStaticControl ( "GTCVT_PAGE_STATIC3", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT );
	strText.Format( "At this time you will get ( %d )", wPTotal );
	pTextBox->SetOneLineText ( strText );
	m_pStaticText4Box = pTextBox;
	m_pRandTextBox = CreateStaticControl ( "GTCVT_PAGE_CP", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT );

	
	{
		CBasicLineBox* pBasicLineBox = new CBasicLineBox(m_pEngineDevice);
		pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBox->CreateBaseBoxOuter ( "GTCVT_PAGE_CP_BACK" );
		RegisterControl ( pBasicLineBox );

	}

	{
		CUIEditBoxMan* pEditBoxMan = new CUIEditBoxMan(m_pEngineDevice);
		pEditBoxMan->CreateSub ( this, "GTCVT_EDITMAN", UI_FLAG_DEFAULT, GTCVT_EDIT_MAN );
		pEditBoxMan->CreateEditBox ( GTCVT_EDIT_CP, "GTCVT_EDIT_RP", "GTCVT_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_CP );
		RegisterControl( pEditBoxMan );
		m_pEditBoxMan = pEditBoxMan;
	}

	CBasicTextButton* pOKButton = new CBasicTextButton(m_pEngineDevice);
	pOKButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, GTCVT_OK );
	pOKButton->CreateBaseButton ( "GTCVT_PAGE_OK", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "GTCVT_PAGE_OKCANCEL", 0 ) );
	pOKButton->SetShortcutKey ( DIK_RETURN, DIK_NUMPADENTER );
	RegisterControl ( pOKButton );

	CBasicTextButton* pQuitButton = new CBasicTextButton(m_pEngineDevice);
	pQuitButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, GTCVT_CANCEL );
	pQuitButton->CreateBaseButton ( "GTCVT_PAGE_CANCEL", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "GTCVT_PAGE_OKCANCEL", 1 ) );
	pQuitButton->SetShortcutKey ( DIK_ESCAPE );
	RegisterControl ( pQuitButton );

}	

void CGameTimeCVTPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	CUIWindow::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	CNetClient* pNetClient =m_pGaeaClient->GetGlobalStage()->GetNetClient ();
	if ( !pNetClient->IsOnline() )		//네트워크 연결이 끊어진 경우
	{
		if ( !m_pInterface->UiIsVisibleGroup ( MODAL_WINDOW_OUTER ) )
		{
			if( !m_pInterface->IsLoginCancel() )
			{
				 m_pInterface->DoModal ( ID2GAMEEXTEXT ("TOPUPSTAGE_1"), UI::MODAL_INFOMATION, UI::OK, OUTER_MODAL_RECONNECT );
			}
		}
	}
	else
	{
		WORD dumm = (WORD)60,dumm2 = (WORD)3;
		const DWORD dwGameTime = m_pInterface->GetGameTime();
		WORD wMinutes = dwGameTime / 60;
		WORD wTotal = wMinutes;
		WORD wMinutesTotal = dwGameTime-60*wTotal;
		WORD wPMinutes = dwGameTime/dumm;
		WORD wPTotal = wPMinutes * dumm2;
		CString strText,strText2;
		if ( dwGameTime == 0 )
		{
			strText.Format("You haven't yet collected GameTime");
		}
		else if ( wTotal<=0 )
		{
			strText.Format((char*)ID2GAMEWORD ( "GTCVT_PAGE_INFO", 1 ) ,dwGameTime);
			strText += "Min(s)";
		}else{
			strText.Format((char*)ID2GAMEWORD ( "GTCVT_PAGE_INFO", 1 ) ,wTotal);
			strText += "hr(s)";
		}
		m_pStaticText2Box->SetOneLineText ( strText );
		strText.Format((char*)ID2GAMEWORD ( "GTCVT_PAGE_INFO", 2 ) ,dumm ,dumm2 );
		m_pStaticText3Box->SetOneLineText ( strText );
		strText.Format( "At this time you will get ( %d )", wPTotal );
		m_pStaticText4Box->SetOneLineText ( strText );
		if (m_nRandPassNumber == -1 )
		{
			CString strTemp;
			CString RandomStr;
			sc::string::generateRandomString(RandomStr, static_cast<int> (7), 1, 1, 1);
			StringCchCopy( m_szRandomString, 7, RandomStr.GetString()  );
			strTemp.Format( "Captcha %s", m_szRandomString  );
			m_nRandPassNumber = 1;
			if( m_pRandTextBox ) m_pRandTextBox->SetOneLineText( strTemp );	
		}
		else
		{
			if( m_nRPUpdateCnt++ > 5 && m_nRandPassNumber == 0 )
			{
				pNetClient->SndRequestRandomKey();
				m_nRandPassNumber = -1;
				m_nRPUpdateCnt = 0;
			}
		}
	}

	//	탭 이동
	if ( m_pEditBoxMan )
	{
		CUIControl* pParent = m_pEditBoxMan->GetTopParent ();
		if ( !pParent )	pParent = this;	//	만약 이클래스가 최상위 컨트롤인 경우
		BOOL bFocus = ( pParent->IsFocusControl() );

		if ( bFocus )
		{
			if ( UIKeyCheck::GetInstance()->Check ( DIK_TAB, DXKEY_DOWN ) )
			{
				m_pEditBoxMan->GoNextTab ();
			}
		}
	}
}

void CGameTimeCVTPage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindow::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case GTCVT_OK:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				SetMessageEx( dwMsg &= ~UIMSG_KEY_FOCUSED ); // Focus 문제로 메세지 삭제
				CString strRP;
				strRP = m_pEditBoxMan->GetEditString ( GTCVT_EDIT_CP );

				if( !strRP.GetLength () )
				{
					 m_pInterface->DoModal ( ID2GAMEEXTEXT ( "GTCVTSTAGE_6" ) );
					return ;
				}

				if ( !CheckString ( strRP ) )
				{
					 m_pInterface->DoModal ( ID2GAMEEXTEXT ( "GTCVT_PAGE_CP_ERROR" ) );
					return ;
				}
				
				CNetClient* pNetClient =m_pGaeaClient->GetGlobalStage()->GetNetClient ();
				
				CString strCheck;
				strCheck.Format("%s",m_szRandomString);
				if ( strCheck == strRP )
				{
					 pNetClient->SndGameTimeCVT( strRP );									
					  m_pInterface->DoModal ( ID2GAMEEXTEXT ("GTCVTSTAGE_7") );
				}
				else
				{
					 m_pInterface->DoModal ( ID2GAMEEXTEXT ("GTCVT_PAGE_CP_ERROR"), UI::MODAL_INFOMATION, UI::OK, OUTER_MODAL_TOPUPTOLOGIN );
				}
			}
		}
		break;

	case GTCVT_CANCEL:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				/*CNetClient* pNetClient =m_pGaeaClient->GetGlobalStage()->GetNetClient();
				if( pNetClient->IsOnline() == true )
				{
					pNetClient->CloseConnect();
					m_pInterface->SetCancelToLogin();
				}

				m_pInterface->ToSelectServerPage ( GetWndID () );*/
				
				m_pInterface->UiHideGroup ( GAMETIMECVT_PAGE, true );
			}
		}
		break;
	};
}

BOOL CGameTimeCVTPage::CheckString( CString strTemp )
{	
	strTemp = strTemp.Trim();

	// 문자열 체크 - 들어가면 안되는 특수문자 : ~!@#$%^&*+|":?><\=`',.;[]{}()
	if( STRUTIL::CheckString( strTemp ) )
	{
		 m_pInterface->DoModal ( ID2GAMEEXTEXT ( "TOPUPSTAGE_4" ), UI::MODAL_INFOMATION, UI::OK );		
		return FALSE;
	}

    return TRUE;
}

void CGameTimeCVTPage::ResetAll ()
{
	m_pEditBoxMan->EndEdit ();

	m_pEditBoxMan->ClearEdit ( GTCVT_EDIT_CP );

	m_nRandPassNumber = 0;
	m_szRandomString[7] = 0;
	m_nRPUpdateCnt = 0;


	m_pEditBoxMan->Init();
	m_pEditBoxMan->BeginEdit();

	if( m_pEditBoxMan->IsMODE_NATIVE() )
	{
		m_pEditBoxMan->DoMODE_TOGGLE();
	}
}

void CGameTimeCVTPage::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );

	if ( bVisible )
	{

		ResetAll();

		if( m_pRandTextBox ) m_pRandTextBox->ClearText();
	}
	else
	{
		m_pEditBoxMan->EndEdit ();

		CNetClient* pNetClient =m_pGaeaClient->GetGlobalStage()->GetNetClient();
		pNetClient->ResetRandomPassNumber();
	}
}

CBasicButton* CGameTimeCVTPage::CreateFlipButton ( char* szButton, char* szButtonFlip, UIGUID ControlID )
{
	CBasicButton* pButton = new CBasicButton(m_pEngineDevice);
	pButton->CreateSub ( this, szButton, UI_FLAG_DEFAULT, ControlID );
	pButton->CreateFlip ( szButtonFlip, CBasicButton::RADIO_FLIP );
	pButton->SetControlNameEx ( szButton );
	RegisterControl ( pButton );

	return pButton;
}

void CGameTimeCVTPage::SetCharToEditBox( TCHAR cKey )
{
	if( !m_pEditBoxMan ) return;
	
	CString strTemp;

	UIGUID nID = m_pEditBoxMan->GetBeginEditBox();

	strTemp = m_pEditBoxMan->GetEditString( nID );
	strTemp += cKey;

	m_pEditBoxMan->SetEditString( nID, strTemp );
}

void CGameTimeCVTPage::DelCharToEditBox()
{
	if( !m_pEditBoxMan ) return;

	CString strTemp;

	UIGUID nID = m_pEditBoxMan->GetBeginEditBox();

	strTemp = m_pEditBoxMan->GetEditString( nID );
	INT nLenth = strTemp.GetLength();
	strTemp = strTemp.Left( nLenth - 1 );

	m_pEditBoxMan->SetEditString( nID, strTemp );
}

void CGameTimeCVTPage::GoNextTab()
{
	if( m_pEditBoxMan )
		m_pEditBoxMan->GoNextTab();
}