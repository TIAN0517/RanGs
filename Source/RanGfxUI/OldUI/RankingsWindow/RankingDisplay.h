#pragma	once
#include "../../../RanLogic/Character/GLCharDefine.h"
#include "../../../RanLogic/GLogicEx.h"
#include "../../InnerInterface.h"
#include "../Util/UIWindowEx.h"
#include "../Util/TapSelectorEx.h"

class	CInnerInterface;
class	GLGaeaClient;
class	CBasicTextButton;
class	CRankingPageRank;
class	CBasicLineBoxEx;
class	CBasicTextBox;
class	CBasicButton;
class	CRankingSlot;
class	CRankingDisplay : public CUIWindowEx, private CTapSelectorEx<DEFAULT_TAPBUTTON> 
{
public:
	typedef CTapSelectorEx::BUTTON STAPBUTTON;
protected:
	enum
	{
		RANKING_BUTTON_RANK  = ET_CONTROL_NEXT,
		RANKING_BUTTON_RANK_PK,
		RANKING_BUTTON_RANK_RICH,
		RANKING_BUTTON_RANK_BR,
		RANKING_BUTTON_RANK_SW,
		RANKING_BUTTON_RANK_AR,
		RANKING_BUTTON_RANK_SH,
		RANKING_BUTTON_RANK_EX,
		RANKING_BUTTON_RANK_SC,
		RANKING_BACKGROUND,
		RANKING_PAGE_RANK,
		RANKING_PREV_BUTTON,
		RANKING_NEXT_BUTTON,
		BUTTON_CLOSE,
		_PLAYER_SLOT_MAX = 10,
	};

public:
	CRankingDisplay ( CInnerInterface* pInterface, GLGaeaClient* pGaeaClient, EngineDeviceMan* pEngineDevice );
	virtual	~CRankingDisplay ();

public:
	CBasicTextButton*	CreateTextButton23 ( const char* szButton, UIGUID ControlID, const char* szText );
	CBasicTextButton*	CreateTextButton ( char* szButton, UIGUID ControlID , char* szText );

	void CreateSubControl ();
	void ReSet();

private:
	GLGaeaClient*					m_pGaeaClient;
	CInnerInterface*				m_pInterface;
	CUIControl*						m_pBackGround;
	CRankingSlot*					m_pSlotPlayer[_PLAYER_SLOT_MAX];
	CRankingSlot*					m_pMySlot;
	CRankingPageRank*				m_pPageRank;

	CBasicButton*		m_pButtonPrev;
	CBasicButton*		m_pButtonNext;

	CBasicLineBoxEx*				m_pLineBoxUpper;
	CBasicLineBoxEx*				m_pLineBoxBottom;
	CBasicLineBoxEx*				m_pLineBoxTopWhite;
	CBasicLineBoxEx*				m_pLineBoxBottomWhite;

	VECRANKING						m_vecRanking;
	RANKINGMAP						m_mapRanking;
	CBasicTextBox*	m_pText[9];

	int	m_nSelectIndexType;
	int	m_nCurrentPage;
	int m_nMaxPage;

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:
	void	RefreshRanking();
	void	OpenPage( int nPage );

	CBasicTextBox*	CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign );
	void	DataAdd( RANKING& sRANK );
	void	RenderView( int nPage );
};