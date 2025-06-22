//PK Notification - Arvin.BC
#include "StdAfx.h"
#include "PKRankNotificationInfo.h"
#include "../EngineLib/DxTools/D3DFont.h"
#include "../../../EngineLib/GUInterface/UITextControl.h"
#include "../../../EngineLib/GUInterface/GameTextControl.h"
#include "../../../EngineLib/GUInterface/BasicLineBoxEx.h"
#include "../../../EngineLib/GUInterface/BasicTextBoxEx.h"
#include "../../../RanLogicClient/Char/GLCharacter.h"
#include "../../../RanLogicClient/GLGaeaClient.h"

#include "../../InnerInterface.h"

// ----------------------------------------------------------------------------
#include "../../../SigmaCore/DebugInclude.h"
// ----------------------------------------------------------------------------

// 4D科技感動畫配置常量
const float CPKRankNotificationInfo::GLOW_ANIMATION_SPEED = 3.0f;
const float CPKRankNotificationInfo::GLOW_MIN_INTENSITY = 0.8f;
const float CPKRankNotificationInfo::GLOW_MAX_INTENSITY = 1.0f;


CPKRankNotificationInfo::CPKRankNotificationInfo ( GLGaeaClient* pGaeaClient, EngineDeviceMan* pEngineDevice ) 
: CUIGroup(pEngineDevice),
	m_pGaeaClient ( pGaeaClient ),
	m_pNameKiller( NULL ),
	m_pNameKilled( NULL ),
	m_pLineBox( NULL ),
	m_pKillIcon( NULL ),
	m_fAnimationTime( 0.0f ),
	m_fGlowIntensity( GLOW_MAX_INTENSITY ),
	m_bAnimationActive( FALSE ),
	m_dwTechStyle( TECH_STYLE_OBSERVER )
{
	for( int i = 0; i < RANK_INFO_ICON_SCHOOL; ++i )
	{
		m_pSchoolIconKiller[i] = NULL;
		m_pSchoolIconKilled[i] = NULL;
	}
}


CPKRankNotificationInfo::~CPKRankNotificationInfo()
{
}

void CPKRankNotificationInfo::CreateSubControl()
{
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	m_pLineBox = new CBasicLineBox(m_pEngineDevice);
	m_pLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	m_pLineBox->CreateBaseBoxTech4D ( "PK_RANK_NOTIFICATION_LINEBOX" );  // 使用4D科技感樣式
	RegisterControl ( m_pLineBox );

	std::string strSchoolKiller[RANK_INFO_ICON_SCHOOL] = 
	{
		"RANK_INFO_CHARACTER_SCHOOL_1_SM",
		"RANK_INFO_CHARACTER_SCHOOL_1_HA",
		"RANK_INFO_CHARACTER_SCHOOL_1_BH",
	};

	std::string strSchoolKilled[RANK_INFO_ICON_SCHOOL] = 
	{
		"RANK_INFO_CHARACTER_SCHOOL_2_SM",
		"RANK_INFO_CHARACTER_SCHOOL_2_HA",
		"RANK_INFO_CHARACTER_SCHOOL_2_BH",
	};

	std::string strClassKiller[RANK_INFO_ICON_CLASS] = 
	{
		"RANK_INFO_CHARACTER_CLASS_1_BRAWLER_MALE",
		"RANK_INFO_CHARACTER_CLASS_1_SWORDMAN_MALE",
		"RANK_INFO_CHARACTER_CLASS_1_ARCHER_FEMALE",
		"RANK_INFO_CHARACTER_CLASS_1_SHAMAN_FEMALE",
		"RANK_INFO_CHARACTER_CLASS_1_EXTREME_MALE",
		"RANK_INFO_CHARACTER_CLASS_1_EXTREME_FEMALE",
		"RANK_INFO_CHARACTER_CLASS_1_BRAWLER_FEMALE",
		"RANK_INFO_CHARACTER_CLASS_1_SWORDMAN_FEMALE",
		"RANK_INFO_CHARACTER_CLASS_1_ARCHER_MALE",
		"RANK_INFO_CHARACTER_CLASS_1_SHAMAN_MALE",
		"RANK_INFO_CHARACTER_CLASS_1_GUNNER_MALE",
		"RANK_INFO_CHARACTER_CLASS_1_GUNNER_FEMALE",
		//"RANK_INFO_CHARACTER_CLASS_1_ASSASIN_MALE",
		//"RANK_INFO_CHARACTER_CLASS_1_ASSASIN_FEMALE",
	};

	std::string strClassKilled[RANK_INFO_ICON_CLASS] = 
	{
		"RANK_INFO_CHARACTER_CLASS_2_BRAWLER_MALE",
		"RANK_INFO_CHARACTER_CLASS_2_SWORDMAN_MALE",
		"RANK_INFO_CHARACTER_CLASS_2_ARCHER_FEMALE",
		"RANK_INFO_CHARACTER_CLASS_2_SHAMAN_FEMALE",
		"RANK_INFO_CHARACTER_CLASS_2_EXTREME_MALE",
		"RANK_INFO_CHARACTER_CLASS_2_EXTREME_FEMALE",
		"RANK_INFO_CHARACTER_CLASS_2_BRAWLER_FEMALE",
		"RANK_INFO_CHARACTER_CLASS_2_SWORDMAN_FEMALE",
		"RANK_INFO_CHARACTER_CLASS_2_ARCHER_MALE",
		"RANK_INFO_CHARACTER_CLASS_2_SHAMAN_MALE",
		"RANK_INFO_CHARACTER_CLASS_2_GUNNER_MALE",
		"RANK_INFO_CHARACTER_CLASS_2_GUNNER_FEMALE",
		//"RANK_INFO_CHARACTER_CLASS_2_ASSASIN_MALE",
		//"RANK_INFO_CHARACTER_CLASS_2_ASSASIN_FEMALE",
	};

	for( int i = 0; i < RANK_INFO_ICON_SCHOOL; ++i )
	{
		m_pSchoolIconKiller[i] = new CUIControl(m_pEngineDevice);
		m_pSchoolIconKiller[i]->CreateSub ( this, strSchoolKiller[i].c_str() );	
		m_pSchoolIconKiller[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pSchoolIconKiller[i] );

		m_pSchoolIconKilled[i] = new CUIControl(m_pEngineDevice);
		m_pSchoolIconKilled[i]->CreateSub ( this, strSchoolKilled[i].c_str() );	
		m_pSchoolIconKilled[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pSchoolIconKilled[i] );
	}

	for( int i = 0; i < RANK_INFO_ICON_CLASS; ++i )
	{
		m_pClassIconKiller[i] = new CUIControl(m_pEngineDevice);
		m_pClassIconKiller[i]->CreateSub ( this, strClassKiller[i].c_str() );	
		m_pClassIconKiller[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pClassIconKiller[i] );

		m_pClassIconKilled[i] = new CUIControl(m_pEngineDevice);
		m_pClassIconKilled[i]->CreateSub ( this, strClassKilled[i].c_str() );	
		m_pClassIconKilled[i]->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pClassIconKilled[i] );
	}

	m_pNameKiller = new CBasicTextBoxEx(m_pEngineDevice);
	m_pNameKiller->CreateSub ( this, "RANK_INFO_CHARACTER_NAME_1" );
	m_pNameKiller->SetFont ( pFont );		
	m_pNameKiller->SetTextAlign( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	RegisterControl ( m_pNameKiller );

	m_pNameKilled = new CBasicTextBoxEx(m_pEngineDevice);
	m_pNameKilled->CreateSub ( this, "RANK_INFO_CHARACTER_NAME_2" );
	m_pNameKilled->SetFont ( pFont );		
	m_pNameKilled->SetTextAlign( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );
	RegisterControl ( m_pNameKilled );

	m_pKillIcon = new CUIControl(m_pEngineDevice);
	m_pKillIcon->CreateSub ( this, "RANK_INFO_CHARACTER_KILL" );
	RegisterControl ( m_pKillIcon );
}

void CPKRankNotificationInfo::TranslateUIMessage( UIGUID cID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage( cID, dwMsg );
}

void CPKRankNotificationInfo::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	// 4D科技感動畫更新
	if ( m_bAnimationActive )
	{
		UpdateTechAnimation( fElapsedTime );
	}

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CPKRankNotificationInfo::UpdateTechAnimation( float fElapsedTime )
{
	if ( !m_bAnimationActive || !m_pLineBox ) 
		return;

	m_fAnimationTime += fElapsedTime;
	
	// 計算光暈強度（呼吸效果）
	m_fGlowIntensity = GLOW_MIN_INTENSITY + 
		(GLOW_MAX_INTENSITY - GLOW_MIN_INTENSITY) * 
		(0.5f + 0.5f * sinf( m_fAnimationTime * GLOW_ANIMATION_SPEED ));
	
	// 更新光效顏色
	DWORD dwCurrentColor = 0;
	
	switch ( m_dwTechStyle )
	{
	case TECH_STYLE_KILLER: // 擊殺者 - 金色光暈
		dwCurrentColor = D3DCOLOR_ARGB( (DWORD)(255 * m_fGlowIntensity), 0xFF, 0xD7, 0x00 );
		break;
	case TECH_STYLE_KILLED: // 被擊殺者 - 紅色警告
		dwCurrentColor = D3DCOLOR_ARGB( (DWORD)(255 * m_fGlowIntensity), 0xFF, 0x30, 0x30 );
		break;
	default: // 觀戰者 - 藍色透明
	case TECH_STYLE_OBSERVER:
		dwCurrentColor = D3DCOLOR_ARGB( (DWORD)(128 * m_fGlowIntensity), 0xE0, 0xFF, 0xFF );
		break;
	}
	
	m_pLineBox->SetDiffuse( dwCurrentColor );
}

void CPKRankNotificationInfo::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle ( bVisible );

	if ( bVisible )
	{
		for( int i = 0; i < RANK_INFO_ICON_SCHOOL; ++i )
		{
			m_pSchoolIconKiller[i]->SetVisibleSingle ( FALSE );
			m_pSchoolIconKilled[i]->SetVisibleSingle ( FALSE );
		}

		for( int i = 0; i < RANK_INFO_ICON_CLASS; ++i )
		{
			m_pClassIconKiller[i]->SetVisibleSingle ( FALSE );	
			m_pClassIconKilled[i]->SetVisibleSingle ( FALSE );
		}

		m_pNameKiller->ClearText();
		m_pNameKilled->ClearText();
		
		// 重置動畫狀態
		m_bAnimationActive = FALSE;
		m_fAnimationTime = 0.0f;
		m_fGlowIntensity = GLOW_MAX_INTENSITY;
		m_dwTechStyle = TECH_STYLE_OBSERVER;
	}
}

void CPKRankNotificationInfo::SetData( SPK_HISTORY sHistory )
{
	if ( sHistory.cClassKiller >= RANK_INFO_ICON_CLASS )	return;
	if ( sHistory.cClassKilled >= RANK_INFO_ICON_CLASS )	return;

	if ( sHistory.cSchoolKiller >= RANK_INFO_ICON_SCHOOL )	return;
	if ( sHistory.cSchoolKilled >= RANK_INFO_ICON_SCHOOL )	return;

	m_pSchoolIconKiller[sHistory.cSchoolKiller]->SetVisibleSingle ( TRUE );
	m_pSchoolIconKilled[sHistory.cSchoolKilled]->SetVisibleSingle ( TRUE );

	m_pClassIconKiller[sHistory.cClassKiller]->SetVisibleSingle ( TRUE );	
	m_pClassIconKilled[sHistory.cClassKilled]->SetVisibleSingle ( TRUE );

	m_pNameKiller->SetText( sHistory.szCharKiller );
	m_pNameKilled->SetText( sHistory.szCharKilled );

	DWORD dwCharID = m_pGaeaClient->GetCharacter()->GetCharID();

	// 4D科技感動態顏色系統
	m_bAnimationActive = TRUE;
	m_fAnimationTime = 0.0f;
	
	if ( dwCharID == sHistory.dwKilled )
	{
		// 被擊殺：紅色警告 + 震動效果
		if ( m_pLineBox )
		{
			m_pLineBox->SetUseRender ( TRUE );
			m_pLineBox->SetDiffuse( NS_UITEXTCOLOR::TECH_RED_WARNING );
		}
		m_dwTechStyle = TECH_STYLE_KILLED;  // 被擊殺者樣式
		
		// 設置玻璃質感半透明效果
		if ( m_pNameKilled ) m_pNameKilled->SetTextColor( 0, NS_UITEXTCOLOR::TECH_RED_WARNING );
		if ( m_pNameKiller ) m_pNameKiller->SetTextColor( 0, NS_UITEXTCOLOR::TECH_GOLD_VICTORY );
	}
	else if ( dwCharID == sHistory.dwKiller )
	{
		// 擊殺者：金色光暈 + 勝利粒子效果
		if ( m_pLineBox )
		{
			m_pLineBox->SetUseRender ( TRUE );
			m_pLineBox->SetDiffuse( NS_UITEXTCOLOR::TECH_GOLD_VICTORY );
		}
		m_dwTechStyle = TECH_STYLE_KILLER;  // 擊殺者樣式
		
		// 設置勝利者光效
		if ( m_pNameKiller ) m_pNameKiller->SetTextColor( 0, NS_UITEXTCOLOR::TECH_GOLD_VICTORY );
		if ( m_pNameKilled ) m_pNameKilled->SetTextColor( 0, NS_UITEXTCOLOR::TECH_GLASS_ALPHA );
	}
	else
	{
		// 觀戰者：藍色透明 + 流動光效
		if ( m_pLineBox )
		{
			m_pLineBox->SetUseRender ( TRUE );
			m_pLineBox->SetDiffuse( NS_UITEXTCOLOR::TECH_GLASS_ALPHA );
		}
		m_dwTechStyle = TECH_STYLE_OBSERVER;  // 觀戰者樣式
		
		// 設置觀戰者科技感顏色
		if ( m_pNameKiller ) m_pNameKiller->SetTextColor( 0, NS_UITEXTCOLOR::TECH_CYAN_GLOW );
		if ( m_pNameKilled ) m_pNameKilled->SetTextColor( 0, NS_UITEXTCOLOR::TECH_BLUE_BRIGHT );
	}
}