#include "StdAfx.h"
#include "../../../SigmaCore/String/StringFormat.h"

#include "../../../EngineLib/GUInterface/BasicButton.h"
#include "../../../EngineLib/GUInterface/BasicTextBox.h"
#include "../../../EngineLib/DxTools/DxFontMan.h"
#include "../../../EngineLib/DxTools/d3dfont.h"
#include "../../../EngineLib/GUInterface/UITextControl.h"
#include "../../../EngineLib/GUInterface/GameTextControl.h"
#include "../../../EngineLib/GUInterface/BasicTextButton.h"
#include "../../../EngineLib/GUInterface/BasicLineBox.h"
#include "../../../EngineLib/GUInterface/BasicLineBoxEx.h"
#include "../../../enginelib/GUInterface/BasicLineBoxSmart.h"
#include "../../../RanLogicClient/GLGaeaClient.h"
#include "../../InnerInterface.h"
#include "RankingDisplay.h"
#include "RankingSlot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRankingDisplay::CRankingDisplay ( CInnerInterface* pInterface, GLGaeaClient* pGaeaClient, EngineDeviceMan* pEngineDevice ) :
CUIWindowEx ( pInterface, pEngineDevice ),
	m_pInterface ( pInterface ),
	m_pGaeaClient ( pGaeaClient ),
	m_pBackGround( NULL ),
	m_pLineBoxUpper( NULL ),
	m_pLineBoxBottom( NULL ),
	m_pLineBoxTopWhite ( NULL ),
	m_pLineBoxBottomWhite ( NULL ), 
	m_nMaxPage(1),
	m_pButtonPrev(NULL),
	m_pButtonNext(NULL),
	m_nSelectIndexType(-1),
	m_nCurrentPage(1),
	m_pMySlot(NULL)
{
	
	for ( int i=0; i < _PLAYER_SLOT_MAX; ++ i )
	{
		m_pSlotPlayer[i] = NULL;
	}

	m_mapRanking.clear();
	m_vecRanking.clear();
}

CRankingDisplay::~CRankingDisplay ()
{
}

CBasicTextButton*  CRankingDisplay::CreateTextButton23 ( const char* szButton, UIGUID ControlID, const char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE22;
	CBasicTextButton* pTextButton = new CBasicTextButton ( m_pEngineDevice );
	pTextButton->CreateSub ( this, "BASIC_TEXT_BUTTON22", UI_FLAG_XSIZE|UI_FLAG_YSIZE, ControlID );
	pTextButton->CreateBaseButton ( szButton, nBUTTONSIZE, CBasicButton::RADIO_FLIP, szText, _DEFAULT_FONT_SHADOW_FLAG );
	RegisterControl ( pTextButton );
	return pTextButton;
}

CBasicTextButton* CRankingDisplay::CreateTextButton ( char* szButton, UIGUID ControlID , char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE19;
	CBasicTextButton* pButton = new CBasicTextButton ( m_pEngineDevice );
	pButton->CreateSub( this, "BASIC_TEXT_BUTTON19", UI_FLAG_XSIZE, ControlID );
	pButton->CreateBaseButton( szButton, nBUTTONSIZE, CBasicButton::CLICK_FLIP, szText , _DEFAULT_FONT_SHADOW_FLAG );
	RegisterControl( pButton );

	return pButton;
}

void CRankingDisplay::CreateSubControl ()
{	
	CD3DFontPar* pFont = m_pEngineDevice->LoadFont ( _DEFAULT_FONT, 9, D3DFONT_SHADOW | D3DFONT_ASCII );
	m_pLineBoxUpper = new CBasicLineBoxEx ( m_pEngineDevice );
	m_pLineBoxUpper->CreateSub ( this, "BASIC_LINE_BOX_EX_DIALOGUE_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxUpper->CreateBaseBoxDialogue ( "RANKING_LINEBOX_UPPER" );
	m_pLineBoxUpper->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxUpper );
	
	m_pLineBoxBottom = new CBasicLineBoxEx ( m_pEngineDevice );
	m_pLineBoxBottom->CreateSub ( this, "BASIC_LINE_BOX_EX_DIALOGUE_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxBottom->CreateBaseBoxDialogue ( "RANKING_LINEBOX_BOTTOM" );
	m_pLineBoxBottom->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxBottom );
	
	m_pLineBoxTopWhite = new CBasicLineBoxEx ( m_pEngineDevice );
	m_pLineBoxTopWhite->CreateSub ( this, "BASIC_LINE_BOX_EX_DIALOGUE_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxTopWhite->CreateBaseBoxDialogue ( "RANKING_LINEBOX_TOP_WHITE" );
	m_pLineBoxTopWhite->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxTopWhite );
	
	m_pLineBoxBottomWhite = new CBasicLineBoxEx ( m_pEngineDevice );
	m_pLineBoxBottomWhite->CreateSub ( this, "BASIC_LINE_BOX_EX_DIALOGUE_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBoxBottomWhite->CreateBaseBoxDialogue ( "RANKING_LINEBOX_BOTTOM_WHITE" );
	m_pLineBoxBottomWhite->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pLineBoxBottomWhite );

	CString strRank[9] = 
	{
		"RANKING_BUTTON_RANK_LEVEL",
		"RANKING_BUTTON_RANK_PK",
		"RANKING_BUTTON_RANK_RICH",
		"RANKING_BUTTON_RANK_BR",
		"RANKING_BUTTON_RANK_SW",
		"RANKING_BUTTON_RANK_AR",
		"RANKING_BUTTON_RANK_SH",
		"RANKING_BUTTON_RANK_EX",
		"RANKING_BUTTON_RANK_SC"
	};

	int i = 0;
	for(i = 0; i < 3; i++ ) 
	{
		STAPBUTTON* pCharacterInfoTap = new STAPBUTTON(m_pEngineDevice);
		pCharacterInfoTap->CreateSub( this, (char*)strRank[i].GetString(), UI_FLAG_DEFAULT, RANKING_BUTTON_RANK + i );
		pCharacterInfoTap->CreateSubControl( (char*)ID2GAMEWORD ( "RANKING_WINDOW_TEXTS",i),
			"CHARACTER_WINDOW_TAP_BASE","CHARACTER_WINDOW_TAP_ACTIVE",
			"CHARACTER_WINDOW_TAP_BASE","CHARACTER_WINDOW_TAP_INACTIVE",
			"CHARACTER_WINDOW_TAP_BASE");

		RegisterControl ( pCharacterInfoTap );
		RegisterTapButton( pCharacterInfoTap );
	}

	for(; i < 9; i++ ) 
	{
		STAPBUTTON* pCharacterInfoTap = new STAPBUTTON(m_pEngineDevice);
		pCharacterInfoTap->CreateSub( this, (char*)strRank[i].GetString(), UI_FLAG_DEFAULT, RANKING_BUTTON_RANK + i );
		pCharacterInfoTap->CreateSubControl( (char*)ID2GAMEWORD ( "RANKING_WINDOW_TEXTS",i),
			"RANKING_WINDOW_TAP_BASE","RANKING_WINDOW_TAP_ACTIVE",
			"RANKING_WINDOW_TAP_BASE","RANKING_WINDOW_TAP_INACTIVE",
			"RANKING_WINDOW_TAP_BASE");

		RegisterControl ( pCharacterInfoTap );
		RegisterTapButton( pCharacterInfoTap );
	}

	//m_pButtonClose = CreateTextButton( "BUTTON_CLOSE", BUTTON_CLOSE, (char*)ID2GAMEWORD("RANKING_WINDOW_TEXTS",3)); 

	D3DCOLOR dwColor = NS_UITEXTCOLOR::WHITE;

	CString strText[9] = 
	{
		"RANKING_SLOT_TEXT_RANK",
		"RANKING_SLOT_TEXT_SCHOOL",
		"RANKING_SLOT_TEXT_NAME",
		"RANKING_SLOT_TEXT_LEVEL",
		"RANKING_SLOT_TEXT_KILL",
		"RANKING_SLOT_TEXT_DEATH",
		"RANKING_SLOT_TEXT_MONEY",
		"RANKING_SLOT_TEXT_CLASS",
		"RANKING_SLOT_TEXT_PAGE"
	};

	for(int i = 0; i < 8; i++ )
	{
		m_pText[i] = CreateStaticControl( (char*)strText[i].GetString(), pFont, dwColor, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
		m_pText[i]->SetText( ID2GAMEWORD("RANKING_SLOT_TEXT_STATIC",i), dwColor );
	}
	
	m_pText[8] = CreateStaticControl( (char*)strText[8].GetString(), pFont, dwColor, TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );

	std::string strSLOT_PLAYER[_PLAYER_SLOT_MAX] = 
	{
		"RANKING_SLOT_00",
		"RANKING_SLOT_01",
		"RANKING_SLOT_02",
		"RANKING_SLOT_03",
		"RANKING_SLOT_04",
		"RANKING_SLOT_05",
		"RANKING_SLOT_06",
		"RANKING_SLOT_07",
		"RANKING_SLOT_08",
		"RANKING_SLOT_09",
	};

	for ( int i=0; i < _PLAYER_SLOT_MAX; ++ i )
	{
		m_pSlotPlayer[i] = new CRankingSlot( m_pGaeaClient, m_pEngineDevice );
		m_pSlotPlayer[i]->CreateSub ( this, strSLOT_PLAYER[i].c_str() );
		m_pSlotPlayer[i]->CreateSubControl ();
		RegisterControl ( m_pSlotPlayer[i] );	
		m_pSlotPlayer[i]->SetVisibleSingle( FALSE );
	}

	m_pMySlot = new CRankingSlot( m_pGaeaClient, m_pEngineDevice );
	m_pMySlot->CreateSub ( this, "RANKING_SLOT_10" );
	m_pMySlot->CreateSubControl ();
	RegisterControl ( m_pMySlot );	
	m_pMySlot->SetVisibleSingle( FALSE );

	m_pButtonPrev = new CBasicButton ( m_pEngineDevice );
	m_pButtonPrev->CreateSub ( this, "RANKING_PRE_PAGE_BUTTON", UI_FLAG_DEFAULT, RANKING_PREV_BUTTON );
	m_pButtonPrev->CreateFlip ( "RANKING_PRE_PAGE_BUTTON_F", CBasicButton::CLICK_FLIP );
	m_pButtonPrev->CreateMouseOver ( "RANKING_PRE_PAGE_BUTTON_OVER" );
	RegisterControl ( m_pButtonPrev );

	m_pButtonNext = new CBasicButton ( m_pEngineDevice );
	m_pButtonNext->CreateSub ( this, "RANKING_NEXT_PAGE_BUTTON", UI_FLAG_DEFAULT, RANKING_NEXT_BUTTON );
	m_pButtonNext->CreateFlip ( "RANKING_NEXT_PAGE_BUTTON_F", CBasicButton::CLICK_FLIP );
	m_pButtonNext->CreateMouseOver ( "RANKING_NEXT_PAGE_BUTTON_OVER" );
	RegisterControl ( m_pButtonNext );
}

void CRankingDisplay::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
		case ET_CONTROL_TITLE:
		case ET_CONTROL_TITLE_F:
			{
				if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
				{
					m_pInterface->SetDefaultPosInterface( RANKING_DISPLAY );
				}
			}break;
		case ET_CONTROL_BUTTON:
		case BUTTON_CLOSE:
			{
				if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					m_pInterface->UiHideGroup ( GetWndID () );
				}
			}break;

		case RANKING_BUTTON_RANK:
		case RANKING_BUTTON_RANK_PK:
		case RANKING_BUTTON_RANK_RICH:
		case RANKING_BUTTON_RANK_BR:
		case RANKING_BUTTON_RANK_SW:
		case RANKING_BUTTON_RANK_AR:
		case RANKING_BUTTON_RANK_SH:
		case RANKING_BUTTON_RANK_EX:
		case RANKING_BUTTON_RANK_SC:
			{
				if ( (dwMsg & UIMSG_LB_UP) && CHECK_MOUSE_IN ( dwMsg ) )
				{
					int nSelect = ControlID - RANKING_BUTTON_RANK;
					OpenPage( nSelect + 1 );
					EMTAPSELECTORRESULT emResult = EMTAPSELECTORRESULT_NONE;
					TapSelectorUIMessage( ControlID, dwMsg, emResult );
				}
			}
			break;

	case RANKING_PREV_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					m_nCurrentPage --;
					if ( m_nCurrentPage < 1 )
						m_nCurrentPage = 1;
					RenderView( m_nCurrentPage );
				}
			}
		}break;

	case RANKING_NEXT_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					if ( m_nCurrentPage >= m_nMaxPage )	return;

					m_nCurrentPage ++;
					RenderView( m_nCurrentPage );
				}
			}
		}break;
	}

	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );
}

void CRankingDisplay::SetVisibleSingle ( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );
}

void CRankingDisplay::ReSet()
{
	m_mapRanking.clear();
	m_vecRanking.clear();

	m_pGaeaClient->GetCharacter()->m_mapRankings.clear();
	m_pGaeaClient->GetCharacter()->m_mapRankingsPK.clear();
	m_pGaeaClient->GetCharacter()->m_mapRankingsRich.clear();

	m_nCurrentPage = 1;
	m_nSelectIndexType = -1;
	m_nMaxPage = 1;

	for ( int i=0; i < _PLAYER_SLOT_MAX; ++ i )
	{
		m_pSlotPlayer[i]->DataReset();
		m_pSlotPlayer[i]->SetVisibleSingle(FALSE);
	}

	m_pMySlot->DataReset();
	m_pMySlot->SetVisibleSingle(FALSE);
}

void CRankingDisplay::RefreshRanking()
{
	OpenPage(1);
}

void CRankingDisplay::OpenPage( int wType )
{
	if ( wType < 1 ) return;

	m_vecRanking.clear();
	m_nCurrentPage = 1;

	m_pMySlot->DataReset();
	m_pMySlot->SetVisibleSingle(FALSE);

	RANKINGMAP_ITER iter;
	RANKINGMAP_ITER iter_end;

	switch ( wType ) 
	{
	case 1:
		iter = m_pGaeaClient->GetCharacter()->m_mapRankings.begin();
		iter_end = m_pGaeaClient->GetCharacter()->m_mapRankings.end();
		break;
	case 2:
		iter = m_pGaeaClient->GetCharacter()->m_mapRankingsPK.begin();
		iter_end = m_pGaeaClient->GetCharacter()->m_mapRankingsPK.end();
		break;
	case 3:
		iter = m_pGaeaClient->GetCharacter()->m_mapRankingsRich.begin();
		iter_end = m_pGaeaClient->GetCharacter()->m_mapRankingsRich.end();
		break;
	default:
		iter = m_pGaeaClient->GetCharacter()->m_mapRankingsPK.begin();
		iter_end = m_pGaeaClient->GetCharacter()->m_mapRankingsPK.end();
		break;
	};
	
	for( ;iter != iter_end; ++iter )
	{
		RANKING& ranking = (*iter).second;

		if ( wType == 1 || wType == 2 || wType == 3 )
		{
			if ( ranking.bLevel && wType != 1 )																continue;
			if ( ranking.bPK	&& wType != 2 )																continue;
			if ( ranking.bRich	&& wType != 3 )																continue;

			m_vecRanking.push_back( ranking );
		}
		else
		{
			if ( wType == 4 ) 
			{
				if ( ranking.wClass == GLCC_FIGHTER_M ) m_vecRanking.push_back( ranking );
				if ( ranking.wClass == GLCC_FIGHTER_W )	m_vecRanking.push_back( ranking );
			}
			else if ( wType == 5 ) 
			{
				if ( ranking.wClass == GLCC_ARMS_M ) m_vecRanking.push_back( ranking );
				if ( ranking.wClass == GLCC_ARMS_W ) m_vecRanking.push_back( ranking );
			}
			else if ( wType == 6 )
			{
				if ( ranking.wClass == GLCC_ARCHER_M ) m_vecRanking.push_back( ranking );
				if ( ranking.wClass == GLCC_ARCHER_W ) m_vecRanking.push_back( ranking );
			}
			else if ( wType == 7 )
			{
				if ( ranking.wClass == GLCC_SPIRIT_M ) m_vecRanking.push_back( ranking );
				if ( ranking.wClass == GLCC_SPIRIT_W ) m_vecRanking.push_back( ranking );
			}
			else if ( wType == 8 ) 
			{
				if ( ranking.wClass == GLCC_EXTREME_M ) m_vecRanking.push_back( ranking );
				if ( ranking.wClass == GLCC_EXTREME_W ) m_vecRanking.push_back( ranking );
			}
			else if ( wType == 9 )
			{
				if ( ranking.wClass == GLCC_SCIENTIST_M ) m_vecRanking.push_back( ranking );
				if ( ranking.wClass == GLCC_SCIENTIST_W ) m_vecRanking.push_back( ranking );
			}
		}
	}

	std::sort( m_vecRanking.begin(), m_vecRanking.end() );

	for ( int i = 0; i < m_vecRanking.size(); ++i )
	{
		m_vecRanking[i].nChaNum = i+1;
		if ( m_vecRanking[i].dwCharID == (int)m_pGaeaClient->GetCharacter()->GetCharID() )
		{
			m_pMySlot->DataUpdate( m_vecRanking[i].nChaNum,m_vecRanking[i].szName,m_vecRanking[i].wSchool,m_vecRanking[i].wKills,m_vecRanking[i].wDeaths,m_vecRanking[i].lMoney,m_vecRanking[i].wClass,m_vecRanking[i].wLevel,m_vecRanking[i].nOnline);
			m_pMySlot->SetData(m_vecRanking[i]);
			m_pMySlot->SetVisibleSingle( TRUE );
		}
	}

	m_nMaxPage = 1 + ( (int)m_vecRanking.size() / 10 );

	RenderView( m_nCurrentPage );
}

CBasicTextBox* CRankingDisplay::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox ( m_pEngineDevice );
	pStaticText->CreateSub ( this, szControlKeyword );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );
	RegisterControl ( pStaticText );
	return pStaticText;
}

void CRankingDisplay::RenderView( int nPage )
{
	if ( nPage < 1 )	return;

	for ( int i=0; i < _PLAYER_SLOT_MAX; ++ i )
	{
		m_pSlotPlayer[i]->DataReset();
		m_pSlotPlayer[i]->SetVisibleSingle(FALSE);
	}

	int nEnd = nPage * 10;
	int nStart = nEnd - 10;
	int nSlot = 0;

	for ( int i=nStart; i<nEnd; ++i, nSlot++ )
	{
		if ( i >= m_vecRanking.size() )	continue;

		m_pSlotPlayer[nSlot]->DataUpdate( m_vecRanking[i].nChaNum,m_vecRanking[i].szName,m_vecRanking[i].wSchool,m_vecRanking[i].wKills,m_vecRanking[i].wDeaths,m_vecRanking[i].lMoney,m_vecRanking[i].wClass,m_vecRanking[i].wLevel,m_vecRanking[i].nOnline);
		m_pSlotPlayer[nSlot]->SetData(m_vecRanking[i]);
		m_pSlotPlayer[nSlot]->SetVisibleSingle( TRUE );
	}

	if ( m_pText[8] )
	{
		CString strTemp;
		strTemp.Format( "%d", nPage );
		m_pText[8]->ClearText();
		m_pText[8]->SetText( strTemp.GetString() );
	}
}

void CRankingDisplay::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if (!IsVisible()) return;	

	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}