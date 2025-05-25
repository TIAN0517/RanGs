#include "StdAfx.h"
#include "AnimatedPKStreak.h"

#include "../../../RanLogicClient/GLGaeaClient.h"
#include "../../../RanLogicClient/Char/GLCharacter.h"
#include "../../../SigmaCore/String/StringFormat.h"
#include "../../../EngineLib/GUInterface/GameTextControl.h"
#include "../../../EngineLib/DxTools/d3dfont.h"
#include "../../../EngineLib/GUInterface/BasicTextBox.h"
#include "../../../EngineLib/GUInterface/UITextControl.h"
#include "../../../EngineLib/GUInterface/BasicAnimationBox.h"
#include "../../../enginelib/GUInterface/UIMan.h"

// ----------------------------------------------------------------------------
#include "../../../SigmaCore/DebugInclude.h"
// ----------------------------------------------------------------------------

const float fRusltTime = 4.0f;

CAnimatedPKStreak::CAnimatedPKStreak(EngineDeviceMan* pEngineDevice)
: CUIGroup(pEngineDevice)
, m_pAniBox(NULL)
, m_bOK(false)
, m_bResult(false)
, m_fTime( 0.0f )
, m_nIndex ( -1 )
{
}

CAnimatedPKStreak::~CAnimatedPKStreak ()
{
	memset( m_pPKStreakText, NULL, sizeof( CUIControl* ) * PK_COMBO_SIZE );
}

void CAnimatedPKStreak::Reset( int nIndex )
{
	if ( nIndex < PK_COMBO_SIZE && nIndex >= 0 )	m_pPKStreakText[nIndex]->SetVisibleSingle( FALSE );

	for ( int i = 0; i < PK_COMBO_SIZE; ++i )
	{
		m_pPKStreakText[i]->SetVisibleSingle( FALSE );
	}

	m_bOK = false;
	m_pAniBox->Reset();
	m_pAniBox->SetVisibleSingle( FALSE );

	m_bResult = false;
	m_fTime = 0.0f;
	m_nIndex = -1;
}

void CAnimatedPKStreak::CreateSubControl ()
{    
	{
		CBasicAnimationBox* pAnibox = new CBasicAnimationBox(m_pEngineDevice);
		pAnibox->CreateSubEx(this,"PK_STREAK_BACK");
		pAnibox->CreateAni("PK_STREAK_IMAGE_00", CBasicAnimationBox::NORMAL_ANI);
		pAnibox->AddAni("PK_STREAK_IMAGE_01");
		pAnibox->AddAni("PK_STREAK_IMAGE_02");
		pAnibox->AddAni("PK_STREAK_IMAGE_03");
		pAnibox->AddAni("PK_STREAK_IMAGE_04");
		pAnibox->AddAni("PK_STREAK_IMAGE_05");
		pAnibox->AddAni("PK_STREAK_IMAGE_06");
		pAnibox->AddAni("PK_STREAK_IMAGE_07");
		pAnibox->AddAni("PK_STREAK_IMAGE_08");
		pAnibox->AddAni("PK_STREAK_IMAGE_04");
		pAnibox->AddAni("PK_STREAK_IMAGE_05");
		pAnibox->AddAni("PK_STREAK_IMAGE_06");
		pAnibox->AddAni("PK_STREAK_IMAGE_07");
		pAnibox->AddAni("PK_STREAK_IMAGE_08");
		pAnibox->AddAni("PK_STREAK_IMAGE_04");
		pAnibox->AddAni("PK_STREAK_IMAGE_05");
		pAnibox->AddAni("PK_STREAK_IMAGE_06");
		pAnibox->AddAni("PK_STREAK_IMAGE_07");
		pAnibox->AddAni("PK_STREAK_IMAGE_08");
		pAnibox->AddAni("PK_STREAK_IMAGE_09");
		pAnibox->AddAni("PK_STREAK_IMAGE_10");
		pAnibox->AddAni("PK_STREAK_IMAGE_11");
		pAnibox->AddAni("PK_STREAK_IMAGE_12");
		pAnibox->SetAniTime(0.1f);
		pAnibox->SetEndTime(4.0f);
		pAnibox->SetLoop(TRUE);
		pAnibox->SetTransparentOption( TRUE );
		pAnibox->SetVisibleSingle( FALSE );
		RegisterControl( pAnibox );
		m_pAniBox = pAnibox;
	}

	{
		CString PKStreak[PK_COMBO_SIZE] =
		{
			"FIRST_BLOOD",
			"DOUBLE_KILL",
			"TRIPPLE_KILL",
			"ULTRA_KILL",
			"RAMPAGE",
			"KILLING_SPREE",
			"DOMINATING",
			"MEGAKILL",
			"UNSTOPPABLE",
			"WICKED_SICK",
			"MONSTER_KILL",
			"GODLIKE",
			"HOLY_SHIT",
			"OWNAGE"
		};

		for ( int i = 0; i < PK_COMBO_SIZE; ++i )
		{
			m_pPKStreakText[i] = new CUIControl(m_pEngineDevice);
			m_pPKStreakText[i]->CreateSub ( this, (char*)PKStreak[i].GetString(), UI_FLAG_CENTER_X, PK_DOUBLE_KILL + i );	
			m_pPKStreakText[i]->SetTransparentOption( TRUE );
			m_pPKStreakText[i]->SetVisibleSingle( FALSE );
			RegisterControl ( m_pPKStreakText[i] );
		}
	}

	Reset(m_nIndex);
}

void CAnimatedPKStreak::SetAnimation( int nIndex )
{
	if ( m_pAniBox && m_pAniBox->IsVisible() ) m_pAniBox->SetFadeTime( 0.2f );
	if ( m_pAniBox && m_pAniBox->IsVisible() ) m_pAniBox->SetFadeOut();

	if ( nIndex >= 0 && nIndex < PK_COMBO_SIZE )
	{
		m_pPKStreakText[nIndex]->SetFadeTime( 0.2f );
		m_pPKStreakText[nIndex]->SetFadeOut();
	}

	m_nIndex = nIndex;

	m_bResult = true;
	m_fTime = 0.2f;
}

void CAnimatedPKStreak::SetVisibleSingle( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );
}

VOID CAnimatedPKStreak::Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl )
{
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_pAniBox && m_bOK && m_pAniBox->GetEndAnimate() )
	{
		m_bOK = false;
		if ( m_pAniBox ) m_pAniBox->SetFadeTime( 0.2f );
		if ( m_pAniBox ) m_pAniBox->SetFadeOut();

		if ( m_nIndex >= 0 && m_nIndex < PK_COMBO_SIZE )
		{
			m_pPKStreakText[m_nIndex]->SetFadeTime( 0.2f );
			m_pPKStreakText[m_nIndex]->SetFadeOut();
		}
	}

	if ( m_bResult ) 
	{
		m_fTime -= fElapsedTime;

		if ( m_fTime < 0.0f )
		{
			m_bResult = false;
			m_fTime = 0.0f;
			m_bOK = true;

			/*m_bResult = false;
			m_fTime = 0.0f;
			
			if ( m_nIndex >= 0 && m_nIndex < PK_COMBO_SIZE )
			{
				m_pPKStreakText[m_nIndex]->SetFadeTime( 0.2f );
				m_pPKStreakText[m_nIndex]->SetFadeOut();
			}*/

			if ( m_pAniBox ) m_pAniBox->InitAnimateFrame();
			if ( m_pAniBox ) m_pAniBox->SetPause(FALSE);
			if ( m_pAniBox ) m_pAniBox->SetFadeTime( 0.2f );
			if ( m_pAniBox ) m_pAniBox->SetFadeIn();

			for ( int i = 0; i < PK_COMBO_SIZE; ++i )
			{
				m_pPKStreakText[i]->SetVisibleSingle( FALSE );
			}

			if ( m_nIndex >= 0 && m_nIndex < PK_COMBO_SIZE )
			{
				m_pPKStreakText[m_nIndex]->SetFadeTime( 0.2f );
				m_pPKStreakText[m_nIndex]->SetFadeIn();
			}
		}

	}
}

void CAnimatedPKStreak::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage( ControlID, dwMsg );
	switch ( ControlID )
	{
	case PK_FIRST_BLOOD:
	case PK_DOUBLE_KILL:
	case PK_TRIPLE_KILL:
	case PK_ULTRA_KILL:
	case PK_RAMPAGE:
	case PK_KILLING_SPREE:
	case PK_DOMINATING:
	case PK_MEGA_KILL:
	case PK_UNSTOPPABLE:
	case PK_WICKED_SICK:
	case PK_MONSTER_KILL:
	case PK_GODLIKE:
	case PK_HOLY_SHIT:
	case PK_OWNAGE:
		break;
	}
}