//	스킬 이동
//
//	최초 작성자 : 성기엽
//	이후 수정자 : 
//	로그
//		[2003.12.8]
//			@ 작성
//

#pragma	once

#include "../../../EngineLib/GUInterface/UIGroup.h"

class GLGaeaClient;
class CBasicAnimationBox;

class	CAnimatedPKStreak : public CUIGroup
{
private:
	enum
	{
		PK_FIRST_BLOOD			= 0,
		PK_DOUBLE_KILL			= 1,
		PK_TRIPLE_KILL			= 2,
		PK_ULTRA_KILL			= 3,
		PK_RAMPAGE				= 4,
		PK_KILLING_SPREE		= 5,
		PK_DOMINATING			= 6,
		PK_MEGA_KILL			= 7,
		PK_UNSTOPPABLE			= 8,
		PK_WICKED_SICK			= 9,
		PK_MONSTER_KILL			= 10,
		PK_GODLIKE				= 11,
		PK_HOLY_SHIT			= 12,
		PK_OWNAGE				= 13,

		PK_COMBO_SIZE			= 14,
	};

public:
	CAnimatedPKStreak (EngineDeviceMan* pEngineDevice);
	virtual	~CAnimatedPKStreak ();

public:
	void	CreateSubControl ();

	virtual	void Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
	virtual void SetVisibleSingle( BOOL bVisible );

public:
	void    SetAnimation( int nIndex );
	void	Reset ( int nIndex );

private:
	CBasicAnimationBox*		m_pAniBox;

	bool	m_bOK;
	bool	m_bResult;
	float	m_fTime;
	int		m_nIndex;

	CUIControl*			m_pPKStreakText[PK_COMBO_SIZE];
};