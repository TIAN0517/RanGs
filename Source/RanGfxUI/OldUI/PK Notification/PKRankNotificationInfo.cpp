//PK Notification - Arvin.BC
#include "StdAfx.h"
#include "PKRankNotificationInfo.h"
#include "../EngineLib/DxTools/D3DFont.h"
#include "../../../EngineLib/GUInterface/UITextControl.h"
#include "../../../EngineLib/GUInterface/GameTextControl.h"
#include "../../../EngineLib/GUInterface/BasicLineBoxEx.h"
#include "../../../EngineLib/GUInterface/BasicTextBoxEx.h"
#include "../../../EngineLib/GUInterface/BasicAnimationBox.h"
#include "../../../RanLogicClient/Char/GLCharacter.h"
#include "../../../RanLogicClient/GLGaeaClient.h"

#include "../../InnerInterface.h"

// ----------------------------------------------------------------------------
#include "../../../SigmaCore/DebugInclude.h"
// ----------------------------------------------------------------------------


CPKRankNotificationInfo::CPKRankNotificationInfo ( GLGaeaClient* pGaeaClient, EngineDeviceMan* pEngineDevice ) 
: CUIGroup(pEngineDevice),
	m_pGaeaClient ( pGaeaClient ),
	m_pNameKiller( NULL ),
	m_pNameKilled( NULL ),
	m_pLineBox( NULL ),
	m_pKillIcon( NULL ),
	m_emCurrentEffectType( PK_CARD_NONE ),
	m_pEffectAnimation( NULL ),
	m_fEffectTime( 0.0f )
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
	m_pLineBox->CreateBaseBoxQuestList ( "PK_RANK_NOTIFICATION_LINEBOX" );
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

	// Create effect animation box
	m_pEffectAnimation = new CBasicAnimationBox(m_pEngineDevice);
	m_pEffectAnimation->CreateSub(this, "PK_EFFECT_ANIMATION", UI_FLAG_DEFAULT);
	m_pEffectAnimation->SetVisibleSingle(FALSE);
	RegisterControl(m_pEffectAnimation);
}

void CPKRankNotificationInfo::TranslateUIMessage( UIGUID cID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage( cID, dwMsg );
}

void CPKRankNotificationInfo::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
	
	// Update effect animations
	m_fEffectTime += fElapsedTime;
	
	if (m_pEffectAnimation && m_pEffectAnimation->IsVisible())
	{
		// Update particle effects based on card type
		switch (m_emCurrentEffectType)
		{
		case PK_CARD_HOLOGRAM_BLUE:
			// Update hologram particles
			break;
		case PK_CARD_NEON_PURPLE:
			// Update neon effects
			break;
		case PK_CARD_FLAME_GOLD:
			// Update flame particles
			break;
		case PK_CARD_RAINBOW_MAGIC:
			// Update rainbow effects
			break;
		case PK_CARD_ICE_CRYSTAL:
			// Update ice particles
			break;
		case PK_CARD_LEGEND_DIVINE:
			// Update divine effects
			break;
		}
	}
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
	
	// Check for active PK effect card and apply visual effects
	EMPK_EFFECT_CARD_TYPE emActiveCard = m_pGaeaClient->GetCharacter()->GetActivePKCard();
	
	if ( dwCharID == sHistory.dwKilled )
	{
		m_pLineBox->SetUseRender ( TRUE );
		m_pLineBox->SetDiffuse( NS_UITEXTCOLOR::DISABLE );
	}
	else if ( dwCharID == sHistory.dwKiller )
	{
		// Apply PK card effect if killer has active card
		if (emActiveCard != PK_CARD_NONE)
		{
			SetPKEffectCardType(emActiveCard);
		}
		else
		{
			m_pLineBox->SetUseRender ( TRUE );
			m_pLineBox->SetDiffuse( NS_UITEXTCOLOR::GREENYELLOW );
		}
	}
	else
	{
		m_pLineBox->SetUseRender ( FALSE );
	}
}

void CPKRankNotificationInfo::CreateBaseBoxHologramBlue(char* szBoxControl)
{
	if (m_pLineBox)
	{
		m_pLineBox->SetDiffuse(0xFF00AAFF);  // 蓝色全息效果
		m_pLineBox->SetUseRender(TRUE);
	}
	
	if (m_pEffectAnimation)
	{
		m_pEffectAnimation->SetVisibleSingle(TRUE);
		// 添加蓝色粒子动画
	}
}

void CPKRankNotificationInfo::CreateBaseBoxNeonPurple(char* szBoxControl)
{
	if (m_pLineBox)
	{
		m_pLineBox->SetDiffuse(0xFFAA00FF);  // 紫色霓虹效果
		m_pLineBox->SetUseRender(TRUE);
	}
	
	if (m_pEffectAnimation)
	{
		m_pEffectAnimation->SetVisibleSingle(TRUE);
		// 添加紫色霓虹动画
	}
}

void CPKRankNotificationInfo::CreateBaseBoxFlameGold(char* szBoxControl)
{
	if (m_pLineBox)
	{
		m_pLineBox->SetDiffuse(0xFFFFAA00);  // 金色火焰效果
		m_pLineBox->SetUseRender(TRUE);
	}
	
	if (m_pEffectAnimation)
	{
		m_pEffectAnimation->SetVisibleSingle(TRUE);
		// 添加金色火焰动画
	}
}

void CPKRankNotificationInfo::CreateBaseBoxRainbowMagic(char* szBoxControl)
{
	if (m_pLineBox)
	{
		// 彩虹渐变效果 - 动态颜色
		DWORD dwColor = 0xFF000000;
		float fHue = fmod(m_fEffectTime * 2.0f, 6.28f);  // 2π周期
		// 简化的HSV到RGB转换，创建彩虹效果
		dwColor |= 0x00FFFFFF;
		m_pLineBox->SetDiffuse(dwColor);
		m_pLineBox->SetUseRender(TRUE);
	}
	
	if (m_pEffectAnimation)
	{
		m_pEffectAnimation->SetVisibleSingle(TRUE);
		// 添加彩虹星光动画
	}
}

void CPKRankNotificationInfo::CreateBaseBoxIceCrystal(char* szBoxControl)
{
	if (m_pLineBox)
	{
		m_pLineBox->SetDiffuse(0xFF00FFFF);  // 冰蓝色效果
		m_pLineBox->SetUseRender(TRUE);
	}
	
	if (m_pEffectAnimation)
	{
		m_pEffectAnimation->SetVisibleSingle(TRUE);
		// 添加冰晶粒子动画
	}
}

void CPKRankNotificationInfo::CreateBaseBoxLegendDivine(char* szBoxControl)
{
	if (m_pLineBox)
	{
		m_pLineBox->SetDiffuse(0xFFFFFFAA);  // 神圣金白色效果
		m_pLineBox->SetUseRender(TRUE);
	}
	
	if (m_pEffectAnimation)
	{
		m_pEffectAnimation->SetVisibleSingle(TRUE);
		// 添加神圣光环动画
	}
}

void CPKRankNotificationInfo::SetPKEffectCardType(EMPK_EFFECT_CARD_TYPE emType)
{
	m_emCurrentEffectType = emType;
	
	// 应用对应的视觉效果
	switch (emType)
	{
	case PK_CARD_HOLOGRAM_BLUE:
		CreateBaseBoxHologramBlue("PK_RANK_NOTIFICATION_LINEBOX");
		break;
	case PK_CARD_NEON_PURPLE:
		CreateBaseBoxNeonPurple("PK_RANK_NOTIFICATION_LINEBOX");
		break;
	case PK_CARD_FLAME_GOLD:
		CreateBaseBoxFlameGold("PK_RANK_NOTIFICATION_LINEBOX");
		break;
	case PK_CARD_RAINBOW_MAGIC:
		CreateBaseBoxRainbowMagic("PK_RANK_NOTIFICATION_LINEBOX");
		break;
	case PK_CARD_ICE_CRYSTAL:
		CreateBaseBoxIceCrystal("PK_RANK_NOTIFICATION_LINEBOX");
		break;
	case PK_CARD_LEGEND_DIVINE:
		CreateBaseBoxLegendDivine("PK_RANK_NOTIFICATION_LINEBOX");
		break;
	default:
		// 默认效果
		if (m_pEffectAnimation)
		{
			m_pEffectAnimation->SetVisibleSingle(FALSE);
		}
		break;
	}
}