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


CPKRankNotificationInfo::CPKRankNotificationInfo ( GLGaeaClient* pGaeaClient, EngineDeviceMan* pEngineDevice ) 
: CUIGroup(pEngineDevice),
	m_pGaeaClient ( pGaeaClient ),
	m_pNameKiller( NULL ),
	m_pNameKilled( NULL ),
	m_pLineBox( NULL ),
	m_pKillIcon( NULL ),
	m_bAnimationsEnabled( true ),
	m_eLastAnimationType( KILL_ANIM_SLASH ),
	m_fAnimationDelay( 0.0f )
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
}

void CPKRankNotificationInfo::TranslateUIMessage( UIGUID cID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage( cID, dwMsg );
}

void CPKRankNotificationInfo::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
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

	if ( dwCharID == sHistory.dwKilled )
	{
		m_pLineBox->SetUseRender ( TRUE );
		m_pLineBox->SetDiffuse( NS_UITEXTCOLOR::DISABLE );
	}
	else if ( dwCharID == sHistory.dwKiller )
	{
		m_pLineBox->SetUseRender ( TRUE );
		m_pLineBox->SetDiffuse( NS_UITEXTCOLOR::GREENYELLOW );
		
		// Trigger kill animation for the player when they get a kill
		if (m_bAnimationsEnabled)
		{
			// Calculate animation position and direction
			D3DXVECTOR3 vAnimationPos = CalculateAnimationPosition(sHistory);
			D3DXVECTOR3 vAnimationDir = CalculateAnimationDirection(sHistory);
			
			// Select appropriate animation based on classes
			KILL_ANIMATION_TYPE eAnimType = SelectAnimationByClass(
				static_cast<EMCHARCLASS>(sHistory.cClassKiller),
				static_cast<EMCHARCLASS>(sHistory.cClassKilled)
			);
			
			// Trigger the kill animation
			TriggerKillAnimation(vAnimationPos, vAnimationDir, eAnimType);
		}
	}
	else
	{
		m_pLineBox->SetUseRender ( FALSE );
		
		// Show kill animation for other players' kills if enabled
		if (m_bAnimationsEnabled)
		{
			D3DXVECTOR3 vAnimationPos = CalculateAnimationPosition(sHistory);
			D3DXVECTOR3 vAnimationDir = CalculateAnimationDirection(sHistory);
			
			// Use a more subtle animation for other players
			KILL_ANIMATION_TYPE eAnimType = SelectAnimationByClass(
				static_cast<EMCHARCLASS>(sHistory.cClassKiller),
				static_cast<EMCHARCLASS>(sHistory.cClassKilled)
			);
			
			// Reduced intensity for other players' kills
			KILL_ANIMATION_CONFIG config;
			config.fIntensity = 0.6f;
			config.fScale = 0.8f;
			config.bUseSound = false; // No sound for other players
			
			KillAnimationManager::GetInstance().PlayKillAnimation(eAnimType, vAnimationPos, vAnimationDir, &config);
		}
	}
}

// Trigger kill animation at specified position and direction
void CPKRankNotificationInfo::TriggerKillAnimation(const D3DXVECTOR3& vKillerPos, const D3DXVECTOR3& vKilledPos, KILL_ANIMATION_TYPE eType)
{
	// Use the provided animation type or select one
	KILL_ANIMATION_TYPE eAnimType = eType;
	
	// Calculate position between killer and killed
	D3DXVECTOR3 vAnimationPos = (vKillerPos + vKilledPos) * 0.5f;
	
	// Calculate direction from killer to killed
	D3DXVECTOR3 vAnimationDir = vKilledPos - vKillerPos;
	D3DXVec3Normalize(&vAnimationDir, &vAnimationDir);
	
	// Play the animation with full intensity for player kills
	KILL_ANIMATION_CONFIG config;
	config.fIntensity = 1.2f;
	config.fScale = 1.0f;
	config.bUseSound = true;
	config.bUseScreenEffect = true;
	
	bool result = KillAnimationManager::GetInstance().PlayKillAnimation(eAnimType, vAnimationPos, vAnimationDir, &config);
	
	if (result)
	{
		m_eLastAnimationType = eAnimType;
	}
}

// Select animation type based on character classes
KILL_ANIMATION_TYPE CPKRankNotificationInfo::SelectAnimationByClass(EMCHARCLASS eKillerClass, EMCHARCLASS eKilledClass)
{
	// Map character classes to appropriate animations
	switch (eKillerClass)
	{
	case GLCC_FIGHTER_M:
	case GLCC_FIGHTER_W:
		// Fighters use slash or explosion
		return (rand() % 2 == 0) ? KILL_ANIM_SLASH : KILL_ANIM_EXPLOSION;
		
	case GLCC_ARMS_M:
	case GLCC_ARMS_W:
		// Swordsmen use slash or stab
		return (rand() % 2 == 0) ? KILL_ANIM_SLASH : KILL_ANIM_STAB;
		
	case GLCC_ARCHER_M:
	case GLCC_ARCHER_W:
		// Archers use stab (piercing arrows)
		return KILL_ANIM_STAB;
		
	case GLCC_SPIRIT_M:
	case GLCC_SPIRIT_W:
		// Shamans use elemental attacks
		{
			int elementChoice = rand() % 3;
			switch (elementChoice)
			{
			case 0: return KILL_ANIM_FLAME;
			case 1: return KILL_ANIM_FREEZE;
			case 2: return KILL_ANIM_LIGHTNING;
			default: return KILL_ANIM_FLAME;
			}
		}
		
	case GLCC_EXTREME_M:
	case GLCC_EXTREME_W:
		// Extreme classes use explosive techniques
		return KILL_ANIM_EXPLOSION;
		
	case GLCC_SCIENTIST_M:
	case GLCC_SCIENTIST_W:
		// Scientists use lightning (technology)
		return KILL_ANIM_LIGHTNING;
		
	default:
		// Default to slash
		return KILL_ANIM_SLASH;
	}
}

// Calculate animation position based on game world data
D3DXVECTOR3 CPKRankNotificationInfo::CalculateAnimationPosition(const SPK_HISTORY& sHistory)
{
	// Try to get actual character positions from the game client
	if (m_pGaeaClient)
	{
		GLCharacter* pCharacter = m_pGaeaClient->GetCharacter();
		if (pCharacter)
		{
			// Use player's current position as base
			D3DXVECTOR3 vPlayerPos = pCharacter->GetPosition();
			
			// Add some randomization for visual variety
			float fOffsetX = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 10.0f;
			float fOffsetZ = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 10.0f;
			
			return D3DXVECTOR3(vPlayerPos.x + fOffsetX, vPlayerPos.y + 2.0f, vPlayerPos.z + fOffsetZ);
		}
	}
	
	// Fallback to default position
	return D3DXVECTOR3(0.0f, 2.0f, 0.0f);
}

// Calculate animation direction based on game context
D3DXVECTOR3 CPKRankNotificationInfo::CalculateAnimationDirection(const SPK_HISTORY& sHistory)
{
	// Try to get character orientation from game client
	if (m_pGaeaClient)
	{
		GLCharacter* pCharacter = m_pGaeaClient->GetCharacter();
		if (pCharacter)
		{
			// Use player's facing direction
			return pCharacter->GetDirect();
		}
	}
	
	// Fallback to forward direction
	return D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

// Play a random kill animation at the specified position
void CPKRankNotificationInfo::PlayRandomKillAnimation(const D3DXVECTOR3& vPosition, const D3DXVECTOR3& vDirection)
{
	KILL_ANIMATION_TYPE eRandomType = static_cast<KILL_ANIMATION_TYPE>(rand() % KILL_ANIM_MAX);
	KillAnimationManager::GetInstance().PlayKillAnimation(eRandomType, vPosition, vDirection);
}