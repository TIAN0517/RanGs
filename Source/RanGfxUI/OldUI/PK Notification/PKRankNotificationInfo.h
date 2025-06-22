#pragma	once

#include "../../../EngineLib/GUInterface/UIGroup.h"
#include "../RanLogic/Character/GLCharDefine.h"
#include "../RanLogic/Msg/PKRankData.h"

class CBasicTextBoxEx;
class CBasicLineBox;
class GLGaeaClient;

class CPKRankNotificationInfo : public CUIGroup
{
	enum
	{
		RANK_INFO_ICON_SCHOOL = 3,
		RANK_INFO_ICON_CLASS = GLCI_NUM_SCIENTIST,
		
		// 4D科技感動畫常量
		TECH_STYLE_OBSERVER = 0,	// 觀戰者
		TECH_STYLE_KILLER = 1,		// 擊殺者  
		TECH_STYLE_KILLED = 2,		// 被擊殺者
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
	
	// 4D科技感動畫系統
	float				m_fAnimationTime;		// 動畫時間
	float				m_fGlowIntensity;		// 光暈強度
	BOOL				m_bAnimationActive;		// 動畫激活狀態
	DWORD				m_dwTechStyle;			// 科技樣式類型
	
	// 動畫配置常量
	static const float	GLOW_ANIMATION_SPEED;	// 光暈動畫速度
	static const float	GLOW_MIN_INTENSITY;		// 最小光暈強度
	static const float	GLOW_MAX_INTENSITY;		// 最大光暈強度

public:
	void SetData( SPK_HISTORY sHistory );
	
private:
	void UpdateTechAnimation( float fElapsedTime );		// 科技感動畫更新
};