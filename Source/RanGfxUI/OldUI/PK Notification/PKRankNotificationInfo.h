#pragma	once

#include "../../../EngineLib/GUInterface/UIGroup.h"
#include "../RanLogic/Character/GLCharDefine.h"
#include "../RanLogic/Msg/PKRankData.h"
#include "../RanLogic/Item/PKEffectCards.h"

class CBasicTextBoxEx;
class CBasicLineBox;
class GLGaeaClient;
class CBasicAnimationBox;

class CPKRankNotificationInfo : public CUIGroup
{
	enum
	{
		RANK_INFO_ICON_SCHOOL = 3,
		RANK_INFO_ICON_CLASS = GLCI_NUM_SCIENTIST,
	};

public:
	CPKRankNotificationInfo ( GLGaeaClient* pGaeaClient, EngineDeviceMan* pEngineDevice );
	virtual ~CPKRankNotificationInfo();

public:
	void CreateSubControl();

public:
	virtual	void TranslateUIMessage( UIGUID cID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:
	CUIControl*		m_pClassIconKiller[RANK_INFO_ICON_CLASS];
	CUIControl*		m_pSchoolIconKiller[RANK_INFO_ICON_SCHOOL];

	CUIControl*		m_pClassIconKilled[RANK_INFO_ICON_CLASS];
	CUIControl*		m_pSchoolIconKilled[RANK_INFO_ICON_SCHOOL];

	CBasicTextBoxEx*	m_pNameKiller;
	CBasicTextBoxEx*	m_pNameKilled;

	CBasicLineBox*		m_pLineBox;
	CUIControl*			m_pKillIcon;

private:
	GLGaeaClient*		m_pGaeaClient;

public:
	void SetData( SPK_HISTORY sHistory );
	
	// PK Effect Card visual effects
	void CreateBaseBoxHologramBlue(char* szBoxControl);    // 全息蓝光
	void CreateBaseBoxNeonPurple(char* szBoxControl);      // 霓虹紫光
	void CreateBaseBoxFlameGold(char* szBoxControl);       // 烈焰金光  
	void CreateBaseBoxRainbowMagic(char* szBoxControl);    // 彩虹幻光
	void CreateBaseBoxIceCrystal(char* szBoxControl);      // 冰晶寒光
	void CreateBaseBoxLegendDivine(char* szBoxControl);    // 传说神光
	
	void SetPKEffectCardType(EMPK_EFFECT_CARD_TYPE emType);
	
private:
	EMPK_EFFECT_CARD_TYPE m_emCurrentEffectType;
	CBasicAnimationBox* m_pEffectAnimation;
	float m_fEffectTime;
};