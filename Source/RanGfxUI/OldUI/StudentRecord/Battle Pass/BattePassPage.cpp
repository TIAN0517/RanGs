#include "StdAfx.h"
#include "./BattePassPage.h"

//#include "../../../SigmaCore/String/StringFormat.h"

#include "../../../RanLogicClient/GLGaeaClient.h"
#include "../../../RanLogicClient/Stage/DxGlobalStage.h"
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
#include "../../../EngineLib/DxTools/DxClubMan.h"

#include "../../../EngineLib/DxTools/d3dfont.h"
#include "../Item/ItemImage.h"

// ----------------------------------------------------------------------------
#include "../../../SigmaCore/DebugInclude.h"
// ----------------------------------------------------------------------------

CBattePassPage::CBattePassPage ( GLGaeaClient* pGaeaClient, CInnerInterface* pInterface, EngineDeviceMan* pEngineDevice )
: CUIGroup(pEngineDevice)
, m_pGaeaClient(pGaeaClient)
, m_pInterface(pInterface)
, m_pTextBase_ITEM(NULL)
, m_pTextBase_ITEMNAME(NULL)
, m_pTextBase_ACQ_LVL(NULL)
, m_pTextBase_ACTION(NULL)
, m_pTextBase_DESCRIPTION(NULL)
, m_pScroll(NULL)
, m_nScrollPos(-1)
{
	//Blank
}

CBattePassPage::~CBattePassPage()
{
	m_vecSlot.clear();
}

void CBattePassPage::CreateSubControl ()
{
	std::string strBaseControl = GetControlNameEx();
	const int nAlignCenter = TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y;	

	//! 라인박스
	{
		CBasicLineBoxSmart* pLineBox = new CBasicLineBoxSmart(m_pEngineDevice);
		pLineBox->CreateSub( this, "BATTLE_PASSING_LINEBOX", UI_FLAG_DEFAULT );
		pLineBox->CreateSubControl( "BATTLE_PASSING_LINEBOX_TEXINFO" );
		RegisterControl( pLineBox );

		//! 라인
		CreateColorBackgroundControl( "BATTLE_PASSING_LINEBOX_HEAD", NS_UITEXTCOLOR::WHITE );
	}

	//! 기본 텍스트
	{
		CD3DFontPar* pFont = m_pEngineDevice->LoadFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
		CD3DFontPar* pFont_Shadow = m_pEngineDevice->LoadFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

		
	}

	//! 랭킹 슬롯
	{
		m_vecSlot.resize(nMAX_SLOT);

		for ( UINT i=0; i<nMAX_SLOT; ++i )
		{
			RANKSLOT* &pSlot = m_vecSlot[i];

			pSlot = new RANKSLOT(m_pGaeaClient, m_pEngineDevice);
			pSlot->CreateSub( this, "BATTLE_PASSSLOT_BASE", UI_FLAG_DEFAULT );
			pSlot->CreateSubControl( this );
			RegisterControl( pSlot );

			const UIRECT& rcSlotRect = pSlot->GetLocalPos();
			pSlot->SetLocalPos( D3DXVECTOR2(rcSlotRect.left, rcSlotRect.top + (rcSlotRect.sizeY * static_cast<float>(i)) ) );
		}
	}

	//! 자신의 랭킹
	{
		//! 라인
		CreateColorBackgroundControl( "CTF_RESULT_MYRANK_LINE_HEAD", NS_UITEXTCOLOR::BLACK );
		CreateColorBackgroundControl( "CTF_RESULT_MYRANK_LINE_TAIL", NS_UITEXTCOLOR::BLACK );
	}

	//! 스크롤 바
	{
		//! 배경
		CreateColorBackgroundControl( "BATTLE_PASS_SCROLLBAR_BACKGROUND", NS_UITEXTCOLOR::BLACK, UI_FLAG_RIGHT | UI_FLAG_YSIZE );

		//! 스크롤
		CBasicScrollBarEx* pScrollBar = new CBasicScrollBarEx(m_pEngineDevice);
		pScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, BATTLE_PASS_SCROLLBAR );
		pScrollBar->CreateBaseScrollBar ( "BATTLE_PASS_SCROLLBAR" );
		pScrollBar->GetThumbFrame()->SetState ( 150, nMAX_SLOT );

		//  해당 컨트롤 위에서만 마우스 휠이 동작한다.
		pScrollBar->GetThumbFrame()->SetScrollParent( this ); 
		RegisterControl ( pScrollBar );  
		m_pScroll = pScrollBar;
	}
}

void CBattePassPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	{
		CBasicScrollThumbFrame* const pThumbFrame = m_pScroll->GetThumbFrame ();
		const int nTotal = pThumbFrame->GetTotal ();
		const int nViewPerPage = pThumbFrame->GetViewPerPage ();

		if ( nViewPerPage < nTotal )
		{
			int nCurPos = 0;
			const int nMovableLine = nTotal - nViewPerPage;
			float fPercent = pThumbFrame->GetPercent ();

			nCurPos = (int)floor(fPercent * nMovableLine);
			if ( nCurPos < 0 ) nCurPos = 0;

			if ( m_nScrollPos == nCurPos ) return;

			m_nScrollPos = nCurPos;

			Update();
		}
	}
}

void CBattePassPage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	//switch ( ControlID )
	//{
	//default:
	//	break;
	//};
}

void CBattePassPage::Update()
{
}

void CBattePassPage::ResetScrollBar()
{
	m_nScrollPos = 0;
	m_pScroll->GetThumbFrame ()->SetPercent( 0.0f );
	Update();
}

void CBattePassPage::Update_BP()
{
	m_nScrollPos = 0;
	m_emSortType = EMMAINSORTTYPE_TOTAL;
	m_pScroll->GetThumbFrame ()->SetPercent( 0.0f );

	for ( UINT i=0; i<nMAX_SLOT; ++i )
	{
		m_vecSlot[i]->Reset();
	}

	Update();
}


CBasicTextBox* CBattePassPage::CreateStaticControl (const char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID )
{
	CBasicTextBox* pStaticText = new CBasicTextBox(m_pEngineDevice);
	pStaticText->CreateSub ( this, szControlKeyword, UI_FLAG_DEFAULT, cID );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );

	return pStaticText;
}

CUIControl* CBattePassPage::CreateColorBackgroundControl ( char* szControlKeyword, const D3DCOLOR& cColor, int nAlign )
{
	CUIControl* pStaticBackground = new CUIControl(m_pEngineDevice);
	pStaticBackground->CreateSub ( this, szControlKeyword, nAlign );
	pStaticBackground->SetDiffuse( cColor );
	pStaticBackground->SetUseRender( TRUE );
	RegisterControl ( pStaticBackground );

	return pStaticBackground;
}

//----------------------------------------------------------------------------------------------------//

void CBattePassPage::RANKSLOT::CreateSubControl ( CBattePassPage* pBase )
{
	GASSERT(pBase);

	ppBase = pBase;

	CD3DFontPar* pFont = m_pEngineDevice->LoadFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
	CD3DFontPar* pFont_Shadow = m_pEngineDevice->LoadFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	const int nAlignCenter = TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y;	

	{
		

		pIMAGECLUB = new CUIControl (m_pEngineDevice);
		pIMAGECLUB->CreateSub ( this, "BATTLE_PASSSLOT_IMAGE_BASE_CLUB", UI_FLAG_DEFAULT );
		RegisterControl ( pIMAGECLUB );
	}

	{
		
	}

	{
		
	}

	bENABLE = true;
}

void CBattePassPage::RANKSLOT::Reset()
{
	SetEnable( false );
	strNAME = "";
}

void CBattePassPage::BATTLEITEMSLOT::SetEnable( const bool bEnable )
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

void CBattePassPage::BATTLEITEMSLOT::SetData( const UINT nindex, const SBATTLEPASSITEM* pBattleItem )
{
	GASSERT(pBattleItem);

}

void CBattePassPage::BATTLEITEMSLOT::SetTextPosition( CBasicTextBox* pTextBox, const UIRECT& rcBase )
{
	GASSERT(pTextBox);

	UIRECT rcTextBox = pTextBox->GetLocalPos();
	rcTextBox.left  = rcBase.left;
	rcTextBox.right = rcBase.right;
	rcTextBox.sizeX = rcBase.sizeX;

	pTextBox->SetLocalPos( rcTextBox );
}

void CBattePassPage::BATTLEITEMSLOT::SetImagePosition( CUIControl* pImage, const UIRECT& rcBase, const UIRECT& rcParent )
{
	GASSERT(pImage);

	UIRECT rcImage = pImage->GetLocalPos();
	rcImage.left   = (rcBase.left + rcBase.right)/2.0f - rcImage.sizeX/2.0f;
	rcImage.right  = rcImage.left + rcImage.sizeX;
	rcImage.top    = rcImage.top + rcParent.sizeY/2.0f - rcImage.sizeY/2.0f;
	rcImage.bottom = rcImage.top + rcImage.sizeY;

	pImage->SetLocalPos( rcImage );
}

CBasicTextBox* CBattePassPage::BATTLEITEMSLOT::CreateStaticControl (const char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID )
{
	CBasicTextBox* pStaticText = new CBasicTextBox(m_pEngineDevice);
	pStaticText->CreateSub ( this, szControlKeyword, UI_FLAG_DEFAULT, cID );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );

	return pStaticText;
}