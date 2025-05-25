#pragma	once

#include "../../../EngineLib/GUInterface/UIGroup.h"
#include "../../InnerInterface.h"

class	GLGaeaClient;
class	CBasicTextBox;
class	CBasicProgressBar;
class	CBasicLineBox;

class	CRankingSlot : public CUIGroup
{
public:
	CRankingSlot ( GLGaeaClient* pGaeaClient, EngineDeviceMan* pEngineDevice );
	virtual	~CRankingSlot ();

public:
	void	CreateSubControl ();

private:
	GLGaeaClient*		m_pGaeaClient;
	CBasicTextBox*		m_pRankNum;
	CUIControl*			m_pRankSchool[3];
	CBasicTextBox*		m_pRankName;
	CBasicTextBox*		m_pRankKill;
	CBasicTextBox*		m_pRankDeath;
	CBasicTextBox*		m_pRankMoney;
	CBasicTextBox*		m_pRankLevel;
	CUIControl*			m_pRankClass[GLCI_NUM_TRICKER];
	CUIControl*			m_pClubMark;
	bool				m_bSelf;
protected:
	CUIControl*		CreateControl ( const char* szControl );
	CBasicTextBox*	CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR D3DCOLOR, int nAlign );
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	DWORD		m_dwCharID;
	void SetID ( DWORD dwID ) { m_dwCharID = dwID; }
	DWORD GetCharID() { return m_dwCharID; }
	
	RANKING	m_sRANK;

public:
	void SetData ( const RANKING &sData );

public:
	void	DataUpdate( int nNUM, std::string strNAME, int nSCHOOL, int nKILL, int nDEATH  , LONGLONG llMoney , WORD wClass , WORD wLevel, int nOnline );
	void	DataReset();
};