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

public:
	void SetData( SPK_HISTORY sHistory );
};