#include "StdAfx.h"
#include "./CompetitionTopRanking.h"

//#include "../../../SigmaCore/String/StringFormat.h"

#include "../../../RanLogicClient/Stage/DxGlobalStage.h"
#include "../../../RanLogicClient/GLGaeaClient.h"
#include "../../../RanLogicClient/Char/GLCharacter.h"
#include "../../../RanLogicClient/CapturedTheField/GLCaptureTheFieldClient.h"

#include "../../../EngineLib/DeviceMan.h"
#include "../../../EngineLib/GUInterface/GameTextControl.h"
#include "../../../EngineLib/GUInterface/UITextControl.h"
#include "../../../EngineLib/GUInterface/BasicTextBox.h"
#include "../../../EngineLib/GUInterface/BasicTextButton.h"
#include "../../../EngineLib/GUInterface/BasicLineBoxSmart.h"
#include "../../../EngineLib/GUInterface/BasicScrollBarEx.h"
#include "../../../EngineLib/GUInterface/BasicScrollThumbFrame.h"
#include "../../../EngineLib/GUInterface/SwapImage.h"
#include "../../../EngineLib/DxTools/DxClubMan.h"

#include "../../../EngineLib/DxTools/d3dfont.h"

// ----------------------------------------------------------------------------
#include "../../../SigmaCore/DebugInclude.h"
// ----------------------------------------------------------------------------

CCompetitionTopRanking::CCompetitionTopRanking(GLGaeaClient* pGaeaClient, CInnerInterface* pInterface, EngineDeviceMan* pEngineDevice)
: CUIWindowEx(pInterface, pEngineDevice)
, m_pGaeaClient(pGaeaClient)
, m_pTextBase_Rank(NULL)
, m_pTextBase_School(NULL)
, m_pTextBase_Class(NULL)
, m_pTextBase_Name(NULL)
, m_pText_Time(NULL)
, m_pCloseButton(NULL)
, m_emCTFType(PVP::ENUM_CTF_TYPE_NULL)
, m_pSortButton_Total(NULL)
, m_pSortButton_CLUB(NULL)
, m_currentID ( COMPETITION_TOP_SORTBUTTON_TOTAL )
{
	//Blank
}

CCompetitionTopRanking::~CCompetitionTopRanking()
{
	//Blank
}

void CCompetitionTopRanking::CreateSubControl ()
{
	std::string strBaseControl = GetControlNameEx();
	const int nAlignCenter = TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y;	

	//! ���ιڽ�
	{
		CBasicLineBoxSmart* pLineBox = new CBasicLineBoxSmart(m_pEngineDevice);
		pLineBox->CreateSub( this, "COMPETITION_TOPRANKING_LINEBOX", UI_FLAG_DEFAULT );
		pLineBox->CreateSubControl( "COMPETITION_TOPRANKING_LINEBOX_TEXINFO" );
		RegisterControl( pLineBox );
	}

	{
		CD3DFontPar* pFont = m_pEngineDevice->LoadFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );
		m_pText_Time = new CBasicTextBox(m_pEngineDevice);
		m_pText_Time->CreateSub ( this, "RNCOMPETITION_CTF_RANK_TIME", UI_FLAG_DEFAULT );
		m_pText_Time->SetFont ( pFont );
		m_pText_Time->SetTextAlign ( TEXT_ALIGN_RIGHT | TEXT_ALIGN_CENTER_Y );	
		RegisterControl ( m_pText_Time );
	}

	//! �⺻ �ؽ�Ʈ
	{
		CD3DFontPar* pFont = m_pEngineDevice->LoadFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
		CD3DFontPar* pFont_Shadow = m_pEngineDevice->LoadFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

		m_pTextBase_Rank	= CreateStaticControl("COMPETITION_TOPRANKING_TEXT_BASE_RANK", pFont_Shadow, NS_UITEXTCOLOR::ENABLE, nAlignCenter);
		m_pTextBase_School	= CreateStaticControl("COMPETITION_TOPRANKING_TEXT_BASE_SCHOOL", pFont_Shadow, NS_UITEXTCOLOR::ENABLE, nAlignCenter);
		m_pTextBase_Class	= CreateStaticControl("COMPETITION_TOPRANKING_TEXT_BASE_CLASS", pFont_Shadow, NS_UITEXTCOLOR::ENABLE, nAlignCenter);
		m_pTextBase_Name	= CreateStaticControl("COMPETITION_TOPRANKING_TEXT_BASE_NAME", pFont_Shadow, NS_UITEXTCOLOR::ENABLE, nAlignCenter);

		m_pTextBase_Rank->SetOneLineText( ID2GAMEWORD("CTF_RESULT_TEXT_BASE", 0), NS_UITEXTCOLOR::LIGHTGREY );
		m_pTextBase_School->SetOneLineText( ID2GAMEWORD("CTF_RESULT_TEXT_BASE", 1), NS_UITEXTCOLOR::LIGHTGREY );
		m_pTextBase_Class->SetOneLineText( ID2GAMEWORD("CTF_RESULT_TEXT_BASE", 2), NS_UITEXTCOLOR::LIGHTGREY );
		m_pTextBase_Name->SetOneLineText( ID2GAMEWORD("CTF_RESULT_TEXT_BASE", 3), NS_UITEXTCOLOR::LIGHTGREY );
	}

	//! ��ŷ ����
	{
		m_vecSlot.resize(nMAX_SLOT);

		for ( UINT i=0; i<nMAX_SLOT; ++i )
		{
			RANKSLOT* &pSlot = m_vecSlot[i];

			pSlot = new RANKSLOT(m_pGaeaClient, m_pEngineDevice);
			pSlot->CreateSub( this, "COMPETITION_TOPRANKING_SLOT_BASE", UI_FLAG_DEFAULT );
			pSlot->CreateSubControl( this );
			RegisterControl( pSlot );

			const UIRECT& rcSlotRect = pSlot->GetLocalPos();
			pSlot->SetLocalPos( D3DXVECTOR2(rcSlotRect.left, rcSlotRect.top + (rcSlotRect.sizeY * static_cast<float>(i)) ) );
		}
	}

	{
		m_pSortButton_Total = new SORTBUTTON(m_pEngineDevice);
		m_pSortButton_Total->CreateSub( this, "COMPETITION_TOPRANKING_SORTBUTTON_TOTAL", UI_FLAG_DEFAULT, COMPETITION_TOP_SORTBUTTON_TOTAL);
		m_pSortButton_Total->CreateSubControl( ID2GAMEWORD("CTF_RESULT_BUTTON", 0) );
		RegisterControl( m_pSortButton_Total );

		m_pSortButton_CLUB = new SORTBUTTON(m_pEngineDevice);
		m_pSortButton_CLUB->CreateSub( this, "COMPETITION_TOPRANKING_SORTBUTTON_CLUB", UI_FLAG_DEFAULT, COMPETITION_TOP_SORTBUTTON_CLUB);
		m_pSortButton_CLUB->CreateSubControl( ID2GAMEWORD("CTF_RESULT_BUTTON", 5) );
		RegisterControl( m_pSortButton_CLUB );
	}

	//! �ݱ� ��ư
	{
		m_pCloseButton = new CBasicTextButton(m_pEngineDevice);
		m_pCloseButton->CreateSub(this, "BASIC_TEXT_BUTTON19", UI_FLAG_XSIZE, COMPETITION_TOP_RANKING_BUTTON);
		m_pCloseButton->CreateBaseButton( "COMPETITION_TOPRANKING_BUTTON_CLOSE", CBasicTextButton::SIZE19, 
			CBasicButton::CLICK_FLIP, ID2GAMEWORD("COMPETITION_TOPRANKING", 0) );
		RegisterControl( m_pCloseButton );
	}

	m_currentID = COMPETITION_TOP_SORTBUTTON_TOTAL;
}

void CCompetitionTopRanking::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIWindowEx::Update(x,y,LB,MB,RB,nScroll,fElapsedTime,bFirstControl);
}

void CCompetitionTopRanking::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindowEx::TranslateUIMessage( ControlID, dwMsg );

	switch ( ControlID )
	{
	case COMPETITION_TOP_RANKING_BUTTON:	
		{
			if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				SetVisibleSingle(FALSE);
			}
		}
		break;
	case COMPETITION_TOP_SORTBUTTON_TOTAL:
	case COMPETITION_TOP_SORTBUTTON_CLUB:
		{
			if ( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
			{
				SelectSortButton(ControlID);
			}
		}
		break;
	};
}

void CCompetitionTopRanking::SelectSortButton( const UIGUID cID )
{
	GASSERT( m_pSortButton_Total );
	GASSERT( m_pSortButton_CLUB );

	m_pSortButton_Total->SetEnable( false );
	m_pSortButton_CLUB->SetEnable( false );
	m_pTextBase_Class->SetOneLineText( ID2GAMEWORD("CTF_RESULT_TEXT_BASE", 2), NS_UITEXTCOLOR::LIGHTGREY );

	switch ( cID )
	{
	case COMPETITION_TOP_SORTBUTTON_TOTAL:
		{
			m_pSortButton_Total->SetEnable( true );
			Update();
		}
		break;

	case COMPETITION_TOP_SORTBUTTON_CLUB:
		{
			m_pSortButton_CLUB->SetEnable( true );
			Update_Club();
			m_pTextBase_Class->SetOneLineText( "Club", NS_UITEXTCOLOR::LIGHTGREY );
		}
		break;

	};
}

void CCompetitionTopRanking::Update()
{
	PVP::GLCaptureTheFieldClient* pCTFClient = m_pGaeaClient->GetCaptureTheField();
	PVP::GLCaptureTheFieldClient::CTFPREVSTATE& sState = pCTFClient->GetPrevState( m_emCTFType );

	for ( UINT i=0; i<nMAX_SLOT; ++i )
	{
		m_vecSlot[i]->SetEnable( false );
	}

	if ( sState.IsNULL() )
	{
		return;
	}

	for ( UINT i=0; i<(UINT)sState.vecRESULTDATA.size(); ++i )
	{
		PVP::CTF_RANKER_BASIC& sResultData = sState.vecRESULTDATA[i];
		if ( i >= nMAX_SLOT ) continue;
		m_vecSlot[i]->SetData( i+1, sResultData );
		m_vecSlot[i]->SetEnable( true );
	}
}

void CCompetitionTopRanking::Update_Club()
{
	PVP::GLCaptureTheFieldClient* pCTFClient = m_pGaeaClient->GetCaptureTheField();
	PVP::GLCaptureTheFieldClient::CTFPREVSTATE& sState = pCTFClient->GetPrevState( m_emCTFType );

	for ( UINT i=0; i<nMAX_SLOT; ++i )
	{
		m_vecSlot[i]->SetEnable( false );
	}

	if ( sState.IsNULL() )
	{
		return;
	}

	for ( UINT i=0; i<(UINT)sState.vecRESULTDATACLUB.size(); ++i )
	{
		PVP::CTF_RANKER_BASIC& sResultData = sState.vecRESULTDATACLUB[i];

		if ( i >= nMAX_SLOT ) continue;
		m_vecSlot[i]->SetData( i+1, sResultData );
		m_vecSlot[i]->SetEnable( true );
	}
}

void CCompetitionTopRanking::SetTextTime( const CString strTime )
{
	m_pText_Time->SetOneLineText(strTime);
}

void CCompetitionTopRanking::ClearTimeText()
{
	m_pText_Time->ClearText();
}

void CCompetitionTopRanking::SetVisibleSingle(BOOL bVisible)
{
	CUIControl::SetVisibleSingle( bVisible );

	if( !bVisible )
		ClearTimeText();
	else	SelectSortButton( m_currentID );
}

//----------------------------------------------------------------------------------------------------//

void CCompetitionTopRanking::RANKSLOT::CreateSubControl ( CCompetitionTopRanking* pBase )
{
	GASSERT(pBase);

	CD3DFontPar* pFont = m_pEngineDevice->LoadFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
	CD3DFontPar* pFont_Shadow = m_pEngineDevice->LoadFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	const int nAlignCenter = TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y;	

	{
		pTEXTRANK = CreateStaticControl("COMPETITION_TOPRANKING_SLOT_TEXT_BASE_RANK", pFont_Shadow, nAlignCenter);
		pTEXTNAME = CreateStaticControl("COMPETITION_TOPRANKING_SLOT_TEXT_BASE_NAME", pFont_Shadow, nAlignCenter);
	
		pIMAGESCHOOL = new CSwapImage(m_pEngineDevice);
		pIMAGESCHOOL->CreateSub ( this, "COMPETITION_TOPRANKING_SLOT_IMAGE_BASE_SCHOOL", UI_FLAG_DEFAULT );
		RegisterControl ( pIMAGESCHOOL );

		pIMAGECLASS = new CSwapImage(m_pEngineDevice);
		pIMAGECLASS->CreateSub ( this, "COMPETITION_TOPRANKING_SLOT_IMAGE_BASE_CLASS", UI_FLAG_DEFAULT );
		RegisterControl ( pIMAGECLASS );

		pIMAGECLUB = new CUIControl(m_pEngineDevice);
		pIMAGECLUB->CreateSub ( this, "COMPETITION_TOPRANKING_SLOT_IMAGE_BASE_CLUB", UI_FLAG_DEFAULT );
		RegisterControl ( pIMAGECLUB );
	}

	{
		SetTextPosition( pTEXTRANK, pBase->GetBaseRectRank() );
		SetTextPosition( pTEXTNAME, pBase->GetBaseRectName() );

		SetImagePosition( pIMAGESCHOOL, pBase->GetBaseRectSchool(), GetLocalPos() );
		SetImagePosition( pIMAGECLASS, pBase->GetBaseRectClass(), GetLocalPos() );
		SetImagePosition( pIMAGECLUB, pBase->GetBaseRectClass(), GetLocalPos() );
	}

	{
		pTEXTRANK->SetOneLineText( "", NS_UITEXTCOLOR::WHITE );
		pTEXTNAME->SetOneLineText( "", NS_UITEXTCOLOR::WHITE );

		pIMAGESCHOOL->SetImage( "COMPETITION_TOPRANKING_SLOT_IMAGE_SCHOOL0" );
		pIMAGECLASS->SetImage( "CREATE_CHAR_CLASS_BRAWLER_MALE" );
	}

	bENABLE = true;
}

void CCompetitionTopRanking::RANKSLOT::Reset()
{
	SetEnable( false );
	strNAME = "";
}

void CCompetitionTopRanking::RANKSLOT::SetEnable( const bool bEnable )
{
	bENABLE = bEnable;

	if ( bEnable )
	{
		SetVisibleSingle( TRUE );
	}
	else
	{
		SetVisibleSingle( FALSE );
	}
}

//----------------------------------------------------------------------------------------------------//

void CCompetitionTopRanking::SORTBUTTON::CreateSubControl ( const std::string& strLabel )
{
	CD3DFontPar* pFont = m_pEngineDevice->LoadFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	ACTIVE = new CBasicLineBoxSmart(m_pEngineDevice);
	ACTIVE->CreateSub( this, "CTF_RESULT_SORTBUTTON_BASE" );
	ACTIVE->CreateSubControl( "CTF_RESULT_SORTBUTTON_TEXINFO_ACTIVE" );
	RegisterControl( ACTIVE );

	INACTIVE = new CBasicLineBoxSmart(m_pEngineDevice);
	INACTIVE->CreateSub( this, "CTF_RESULT_SORTBUTTON_BASE" );
	INACTIVE->CreateSubControl( "CTF_RESULT_SORTBUTTON_TEXINFO_INACTIVE" );
	RegisterControl( INACTIVE );

	TEXTBOX = new CBasicTextBox(m_pEngineDevice);
	TEXTBOX->CreateSub( this, "CTF_RESULT_SORTBUTTON_BASE" );
	TEXTBOX->SetFont( pFont );
	TEXTBOX->SetTextAlign( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	TEXTBOX->SetText( strLabel.c_str() );
	TEXTBOX->SetUseTextColor( 0, TRUE );
	RegisterControl( TEXTBOX );
}

void CCompetitionTopRanking::SORTBUTTON::SetEnable ( const bool bEnable )
{
	ENABLE = bEnable;

	if ( bEnable )
	{
		ACTIVE->SetVisibleSingle( TRUE );
		INACTIVE->SetVisibleSingle( FALSE );
	}
	else
	{
		ACTIVE->SetVisibleSingle( FALSE );
		INACTIVE->SetVisibleSingle( TRUE );
	}
}

void CCompetitionTopRanking::RANKSLOT::SetData( const UINT nRank, const PVP::CTF_RANKER_BASIC& sResultData )
{
	static const std::string strClass[GLCI_NUM_ETC] = 
	{
		"CREATE_CHAR_CLASS_BRAWLER_MALE",
		"CREATE_CHAR_CLASS_SWORDMAN_MALE",
		"CREATE_CHAR_CLASS_ARCHER_FEMALE",
		"CREATE_CHAR_CLASS_SHAMAN_FEMALE",
		"CREATE_CHAR_CLASS_EXTREME_MALE",
		"CREATE_CHAR_CLASS_EXTREME_FEMALE",
		"CREATE_CHAR_CLASS_BRAWLER_FEMALE",
		"CREATE_CHAR_CLASS_SWORDMAN_FEMALE",
		"CREATE_CHAR_CLASS_ARCHER_MALE",
		"CREATE_CHAR_CLASS_SHAMAN_MALE",
		"CREATE_CHAR_CLASS_SCIENTIST_MALE",
		"CREATE_CHAR_CLASS_SCIENTIST_FEMALE",
		"CREATE_CHAR_CLASS_ASSASSIN_MALE",
		"CREATE_CHAR_CLASS_ASSASSIN_FEMALE",
		"CREATE_CHAR_CLASS_TRICKER_MALE",
		"CREATE_CHAR_CLASS_TRICKER_FEMALE",
		"CREATE_CHAR_CLASS_ETC_MALE",
		"CREATE_CHAR_CLASS_ETC_FEMALE",
	};

	//! �������� �ƴ�
	if ( strNAME == sResultData.szName)
	{
		return;
	}

	strNAME = sResultData.szName;

	EMCHARINDEX emIndex = CharClassToIndex(EMCHARCLASS(sResultData.wClass));

	pTEXTRANK->SetOneLineText( sc::string::format("%2d", nRank).c_str(), NS_UITEXTCOLOR::WHITE);
	pTEXTNAME->SetOneLineText( sResultData.szName, NS_UITEXTCOLOR::WHITE);

	GASSERT(sResultData.wSchool < SCHOOL_NUM);
	GASSERT(emIndex < GLCI_NUM_ETC);

	pIMAGESCHOOL->SetImage( sc::string::format("COMPETITION_TOPRANKING_SLOT_IMAGE_SCHOOL%1%", sResultData.wSchool ).c_str() );
	pIMAGECLASS->SetImage( strClass[emIndex].c_str() );
	pIMAGECLASS->SetVisibleSingle( TRUE );
	pIMAGECLUB->SetVisibleSingle( FALSE );

	if ( sResultData.dwClubID != CLUB_NULL )
	{
		const int nSERVER = pGAEACLIENT->GetCharacter()->m_dwServerID;
		const clubmark::DXDATA& sMarkData = DxClubMan::GetInstance().GetClubData(
			m_pEngineDevice->GetDevice(),
			nSERVER,
			sResultData.dwClubID );

		pIMAGECLUB->SetTexturePos ( 0, sMarkData.vTex_1_LU );
		pIMAGECLUB->SetTexturePos ( 1, sMarkData.vTex_2_RU );
		pIMAGECLUB->SetTexturePos ( 3, sMarkData.vTex_3_LD );
		pIMAGECLUB->SetTexturePos ( 2, sMarkData.vTex_4_RD );

		const UIRECT& rcGlobalPos = pIMAGECLASS->GetGlobalPos ();
		pIMAGECLUB->SetGlobalPos(
			UIRECT(
			ceil(rcGlobalPos.left),
			ceil(rcGlobalPos.top),
			ceil(rcGlobalPos.sizeX),
			ceil(rcGlobalPos.sizeY) ) );

		pIMAGECLUB->SetTexture ( sMarkData.pddsTexture );
		pIMAGECLUB->SetVisibleSingle( TRUE );
		pIMAGECLASS->SetVisibleSingle( FALSE );
	}
}

void CCompetitionTopRanking::RANKSLOT::SetTextPosition( CBasicTextBox* pTextBox, const UIRECT& rcBase )
{
	GASSERT(pTextBox);

	UIRECT rcTextBox = pTextBox->GetLocalPos();
	rcTextBox.left  = rcBase.left;
	rcTextBox.right = rcBase.right;
	rcTextBox.sizeX = rcBase.sizeX;

	pTextBox->SetLocalPos( rcTextBox );
}

void CCompetitionTopRanking::RANKSLOT::SetImagePosition( CSwapImage* pImage, const UIRECT& rcBase, const UIRECT& rcParent )
{
	GASSERT(pImage);

	UIRECT rcImage = pImage->GetLocalPos();
	rcImage.left   = (rcBase.left + rcBase.right)/2.0f - rcImage.sizeX/2.0f;
	rcImage.right  = rcImage.left + rcImage.sizeX;
	rcImage.top    = rcImage.top + rcParent.sizeY/2.0f - rcImage.sizeY/2.0f;
	rcImage.bottom = rcImage.top + rcImage.sizeY;

	pImage->SetLocalPos( rcImage );
}

void CCompetitionTopRanking::RANKSLOT::SetImagePosition( CUIControl* pImage, const UIRECT& rcBase, const UIRECT& rcParent )
{
	GASSERT(pImage);

	UIRECT rcImage = pImage->GetLocalPos();
	rcImage.left   = (rcBase.left + rcBase.right)/2.0f - rcImage.sizeX/2.0f;
	rcImage.right  = rcImage.left + rcImage.sizeX;
	rcImage.top    = rcImage.top + rcParent.sizeY/2.0f - rcImage.sizeY/2.0f;
	rcImage.bottom = rcImage.top + rcImage.sizeY;

	pImage->SetLocalPos( rcImage );
}

CBasicTextBox* CCompetitionTopRanking::RANKSLOT::CreateStaticControl (const char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID )
{
	CBasicTextBox* pStaticText = new CBasicTextBox(m_pEngineDevice);
	pStaticText->CreateSub ( this, szControlKeyword, UI_FLAG_DEFAULT, cID );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );

	return pStaticText;
}

//----------------------------------------------------------------------------------------------------//

MyCompetitionTopRanking::MyCompetitionTopRanking( GLGaeaClient* pGaeaClient, CInnerInterface* pInterface, EngineDeviceMan* pEngineDevice )
: CCompetitionTopRanking( pGaeaClient, pInterface, pEngineDevice )
{
}

void MyCompetitionTopRanking::CreateUIWindowAndRegisterOwnership()
{
	CCompetitionTopRanking::Create( COMPETITION_TOPRANKING, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	CCompetitionTopRanking::CreateBaseWindowLightGray  ( "COMPETITION_TOPRANKING", (char*)ID2GAMEWORD("COMPETITION_TOPRANKING", 1) );
	CCompetitionTopRanking::CreateSubControl();
	CCompetitionTopRanking::SetAlignFlag( UI_FLAG_CENTER_Y | UI_FLAG_CENTER_X );
	CCompetitionTopRanking::m_pInterface->UiRegisterControl( this, true );
	CCompetitionTopRanking::m_pInterface->UiShowGroupFocus( COMPETITION_TOPRANKING );
}

void MyCompetitionTopRanking::Open( const PVP::ENUM_CTF_TYPE emType )
{
	UIRECT rcPos   = CCompetitionTopRanking::m_pInterface->GetCompetitionWindowPos();
	UIRECT rcMyPos = CCompetitionTopRanking::GetGlobalPos();
	
	rcPos = UIRECT(rcPos.right, rcPos.top, rcMyPos.sizeX, rcMyPos.sizeY);

	CCompetitionTopRanking::SetGlobalPos( rcPos );
	CCompetitionTopRanking::SetCTFType( emType );
	//CCompetitionTopRanking::SetVisibleSingle(TRUE);
	CCompetitionTopRanking::m_pInterface->UiShowGroupFocus( COMPETITION_TOPRANKING );
}

void MyCompetitionTopRanking::Close()
{
	CCompetitionTopRanking::SetVisibleSingle(FALSE);
}

void MyCompetitionTopRanking::SetTextTime( const CString strTime )
{
	CCompetitionTopRanking::SetTextTime(strTime);
}
