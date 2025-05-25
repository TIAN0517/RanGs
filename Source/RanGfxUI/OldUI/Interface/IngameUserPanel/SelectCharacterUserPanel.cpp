#include "StdAfx.h"
#include "SelectCharacterUserPanel.h"
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

CSelectCharacterUserPanel::CSelectCharacterUserPanel (GLGaeaClient* pGaeaClient, COuterInterface* pInterface, EngineDeviceMan* pEngineDevice)
	: CUIGroupHelper(pEngineDevice)
	, m_pGaeaClient(pGaeaClient)
	, m_pInterface ( pInterface )
	//m_uCharRemain ( USHRT_MAX )
	, m_uUserPoint(0)
	, m_dwGameTime(0)
	, m_uBattlePoint(0)
{
}

CSelectCharacterUserPanel::~CSelectCharacterUserPanel ()
{
}

void CSelectCharacterUserPanel::CreateSubControl ()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

    //	우측 테두리
	{
		{
			CBasicLineBox* pBasicLineBox = new CBasicLineBox(m_pEngineDevice);
			pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pBasicLineBox->CreateBaseBoxOuterFat ( "SELECT_CHARACTER_USERPANEL_BACK" );
			RegisterControl ( pBasicLineBox );
		}
		
		{
			CBasicTextBox* pTextBox = new CBasicTextBox(m_pEngineDevice);
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_INFO_TEXTBOX_STATIC" );
			pTextBox->SetFont ( pFont );
			pTextBox->AddText ( ID2GAMEWORD("SELECT_CHARACTER_USER_STATIC",6), NS_UITEXTCOLOR::GOLD );
			RegisterControl ( pTextBox );
		}
	
		{
			CBasicTextBox* pTextBox = new CBasicTextBox(m_pEngineDevice);
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_EMAIL_TEXTBOX_STATIC" );
			pTextBox->SetFont ( pFont );
			pTextBox->AddText ( ID2GAMEWORD("SELECT_CHARACTER_USER_STATIC",0), NS_UITEXTCOLOR::PALEGOLDENROD );
			RegisterControl ( pTextBox );
		}
		
		{
			CBasicTextBox* pTextBox = new CBasicTextBox(m_pEngineDevice);
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_PASSWORD_TEXTBOX_STATIC" );
			pTextBox->SetFont ( pFont );
			pTextBox->AddText ( ID2GAMEWORD("SELECT_CHARACTER_USER_STATIC",1), NS_UITEXTCOLOR::PALEGOLDENROD );
			RegisterControl ( pTextBox );
		}
		
		{
			CBasicTextBox* pTextBox = new CBasicTextBox(m_pEngineDevice);
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_PIN_TEXTBOX_STATIC" );
			pTextBox->SetFont ( pFont );
			pTextBox->AddText ( ID2GAMEWORD("SELECT_CHARACTER_USER_STATIC",2), NS_UITEXTCOLOR::PALEGOLDENROD );
			RegisterControl ( pTextBox );
		}
		
		{
			CBasicTextBox* pTextBox = new CBasicTextBox(m_pEngineDevice);
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_GAMETIME_TEXTBOX_STATIC" );
			pTextBox->SetFont ( pFont );
			pTextBox->AddText ( ID2GAMEWORD("SELECT_CHARACTER_USER_STATIC",3), NS_UITEXTCOLOR::PALEGOLDENROD );
			RegisterControl ( pTextBox );
		}
		
		{
			CBasicTextBox* pTextBox = new CBasicTextBox(m_pEngineDevice);
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_USERPOINT_TEXTBOX_STATIC" );
			pTextBox->SetFont ( pFont );
			pTextBox->AddText ( ID2GAMEWORD("SELECT_CHARACTER_USER_STATIC",4), NS_UITEXTCOLOR::PALEGOLDENROD );
			RegisterControl ( pTextBox );
		}
		
		{
			CBasicTextBox* pTextBox = new CBasicTextBox(m_pEngineDevice);
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_BATTLEPOINT_TEXTBOX_STATIC" );
			pTextBox->SetFont ( pFont );
			pTextBox->AddText ( ID2GAMEWORD("SELECT_CHARACTER_USER_STATIC",5), NS_UITEXTCOLOR::PALEGOLDENROD );
			RegisterControl ( pTextBox );
		}
		
		{
			CBasicTextBox* pTextBox = new CBasicTextBox(m_pEngineDevice);
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_EMAIL_TEXTBOX" );
			pTextBox->SetFont ( pFont );
			pTextBox->SetText ( "D***y@yahoo.com", NS_UITEXTCOLOR::PALEGOLDENROD );
			RegisterControl ( pTextBox );
			m_pEmailTextBox = pTextBox;
			
		}
		
		{
			CBasicTextBox* pTextBox = new CBasicTextBox(m_pEngineDevice);
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_GAMETIME_TEXTBOX" );
			pTextBox->SetFont ( pFont );
			RegisterControl ( pTextBox );
			m_pGameTimeTextBox = pTextBox;
		}
		
		{
			CBasicTextBox* pTextBox = new CBasicTextBox(m_pEngineDevice);
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_USERPOINT_TEXTBOX" );
			pTextBox->SetFont ( pFont );
			RegisterControl ( pTextBox );
			m_pUserPointTextBox = pTextBox;
		}
		
		{
			CBasicTextBox* pTextBox = new CBasicTextBox(m_pEngineDevice);
			pTextBox->CreateSub ( this, "SELECT_CHARACTER_USERPANEL_BATTLEPOINT_TEXTBOX" );
			pTextBox->SetFont ( pFont );
			RegisterControl ( pTextBox );
			m_pBattlePointTextBox = pTextBox;
		}

		const int nBUTTONSIZE = CBasicTextButton::SIZE18;
		{
			m_pButtonCE = new CBasicTextButton(m_pEngineDevice);
			m_pButtonCE->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, SELECT_CHARACTER_USERPANEL_CE );
			m_pButtonCE->CreateBaseButton ( "SELECT_CHARACTER_USERPANEL_CE", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "SELECT_CHARACTER_USERPANEL_BUTTON", 0 ) );
			RegisterControl ( m_pButtonCE );

			m_pButtonCP = new CBasicTextButton(m_pEngineDevice);
			m_pButtonCP->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, SELECT_CHARACTER_USERPANEL_CP );
			m_pButtonCP->CreateBaseButton ( "SELECT_CHARACTER_USERPANEL_CP", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "SELECT_CHARACTER_USERPANEL_BUTTON", 1 ) );
			RegisterControl ( m_pButtonCP );

			m_pButtonCPN = new CBasicTextButton(m_pEngineDevice);
			m_pButtonCPN->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, SELECT_CHARACTER_USERPANEL_CPN );
			m_pButtonCPN->CreateBaseButton ( "SELECT_CHARACTER_USERPANEL_CPN", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "SELECT_CHARACTER_USERPANEL_BUTTON", 2 ) );
			RegisterControl ( m_pButtonCPN );

			m_pButtonRP = new CBasicTextButton(m_pEngineDevice);
			m_pButtonRP->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, SELECT_CHARACTER_USERPANEL_RN );
			m_pButtonRP->CreateBaseButton ( "SELECT_CHARACTER_USERPANEL_RN", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "SELECT_CHARACTER_USERPANEL_BUTTON", 3 ) );
			RegisterControl ( m_pButtonRP );

			m_pButtonGTCVT = new CBasicTextButton(m_pEngineDevice);
			m_pButtonGTCVT->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, SELECT_CHARACTER_USERPANEL_GTCVT );
			m_pButtonGTCVT->CreateBaseButton ( "SELECT_CHARACTER_USERPANEL_GTCVT", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "SELECT_CHARACTER_USERPANEL_BUTTON", 4 ) );
			RegisterControl ( m_pButtonGTCVT );

			/*m_pButtonPPCVT = new CBasicTextButton(m_pEngineDevice);
			m_pButtonPPCVT->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, SELECT_CHARACTER_USERPANEL_PPCVT );
			m_pButtonPPCVT->CreateBaseButton ( "SELECT_CHARACTER_USERPANEL_PPCVT", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "SELECT_CHARACTER_USERPANEL_BUTTON", 6 ) );
			RegisterControl ( m_pButtonPPCVT );*/

			m_pButtonTOPUP = new CBasicTextButton(m_pEngineDevice);
			m_pButtonTOPUP->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, SELECT_CHARACTER_USERPANEL_TOPUP );
			m_pButtonTOPUP->CreateBaseButton ( "SELECT_CHARACTER_USERPANEL_TOPUP", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD ( "SELECT_CHARACTER_USERPANEL_BUTTON", 5 ) );
			RegisterControl ( m_pButtonTOPUP );
		}
	}
}

void CSelectCharacterUserPanel::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	const DWORD uUserPoint = m_pInterface->GetUserPoint();
	const DWORD uBattlePoint = m_pInterface->GetBattlePoint();
	const DWORD dwGameTime = m_pInterface->GetGameTime();
	const CString strEmail =  m_pInterface->GetEmail();
	
			//strText.Format("%s %d %d %d",nlfd->szUserEmail , nlfd->uUserPoints , nlfd->uBattlePoints , nlfd->dwGameTime );
			// m_pInterface->DoModal ( strText, MODAL_INFOMATION, OK, NULL );
	{
			m_pUserPointTextBox->ClearText();
			CString strTemp;
			strTemp.Format ( "%u", uUserPoint );
			m_pUserPointTextBox->SetText ( strTemp, NS_UITEXTCOLOR::PALEGOLDENROD );
			
			m_uUserPoint = uUserPoint;
	}
	{
			m_pBattlePointTextBox->ClearText();
			CString strTemp;
			strTemp.Format ( "%u", uBattlePoint );
			m_pBattlePointTextBox->SetText ( strTemp, NS_UITEXTCOLOR::WHITE );

			m_uBattlePoint = uBattlePoint;
	}
	{
			m_pGameTimeTextBox->ClearText();
			CString strTemp;
			strTemp.Format ( "%d", dwGameTime );
			m_pGameTimeTextBox->SetText ( strTemp, NS_UITEXTCOLOR::WHITE );

			m_dwGameTime = dwGameTime;
	}
	{
			m_pEmailTextBox->ClearText();
			CString strTemp;
			strTemp.Format ( "%s", strEmail.GetString() );
			m_pEmailTextBox->SetText ( strEmail, NS_UITEXTCOLOR::WHITE );

			m_strEmail = strEmail;
	}

}
void CSelectCharacterUserPanel::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );
	
	if ( CHECK_MOUSE_IN ( dwMsg ) )
	{
		AddMessageEx ( UIMSG_MOUSEIN_SELECT_USERPANEL );
	}

	switch ( ControlID )
	{
	case SELECT_CHARACTER_USERPANEL_CP:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CNetClient* pNetClient =m_pGaeaClient->GetGlobalStage()->GetNetClient();
				if( pNetClient->IsOnline() == true )
				{
					m_pInterface->ToCPPage ( GetWndID () );
				}
			}
		}
		break;

	case SELECT_CHARACTER_USERPANEL_CPN:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CNetClient* pNetClient =m_pGaeaClient->GetGlobalStage()->GetNetClient();
				if( pNetClient->IsOnline() == true )
				{
					m_pInterface->ToCPNPage ( GetWndID () );
				}
			}
		}
		break;

	case SELECT_CHARACTER_USERPANEL_RN:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				m_pInterface->ToRPNPage ( GetWndID () );
			}
		}
		break;

	case SELECT_CHARACTER_USERPANEL_CE:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CNetClient* pNetClient =m_pGaeaClient->GetGlobalStage()->GetNetClient();
				if( pNetClient->IsOnline() == true )
				{
					m_pInterface->ToCEPage ( GetWndID () );
				}
			}
		}
		break;

	case SELECT_CHARACTER_USERPANEL_GTCVT:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CNetClient* pNetClient =m_pGaeaClient->GetGlobalStage()->GetNetClient();
				if( pNetClient->IsOnline() == true )
				{
					m_pInterface->ToGTCVTPage ( GetWndID () );
				}
			}
		}
		break;

	/*case SELECT_CHARACTER_USERPANEL_PPCVT:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CNetClient* pNetClient =m_pGaeaClient->GetGlobalStage()->GetNetClient();
				if( pNetClient->IsOnline() == true )
				{
					m_pInterface->ToPPCVTPage ( GetWndID () );
				}
			}
		}
		break;*/

	case SELECT_CHARACTER_USERPANEL_TOPUP:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CNetClient* pNetClient =m_pGaeaClient->GetGlobalStage()->GetNetClient();
				if( pNetClient->IsOnline() == true )
				{
					m_pInterface->ToTOPUPPage ( GetWndID () );
				}
			}
		}
		break;
	}
}