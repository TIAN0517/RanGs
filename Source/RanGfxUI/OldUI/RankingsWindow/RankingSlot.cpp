#include "StdAfx.h"
#include "../../../SigmaCore/String/StringFormat.h"

#include "../../../EngineLib/GUInterface/BasicButton.h"
#include "../../../EngineLib/GUInterface/BasicTextBox.h"
#include "../../../EngineLib/DxTools/DxFontMan.h"
#include "../../../EngineLib/DxTools/d3dfont.h"
#include "../../../EngineLib/GUInterface/UITextControl.h"
#include "../../../EngineLib/GUInterface/GameTextControl.h"
#include "../../../EngineLib/GUInterface/BasicTextButton.h"
#include "../../../EngineLib/GUInterface/BasicLineBox.h"
#include "../../../enginelib/GUInterface/BasicLineBoxSmart.h"
#include "../../../RanLogicClient/GLGaeaClient.h"
#include "../../../RanLogicClient/Stage//DxGlobalStage.h"
#include "../../../EngineLib/DxTools/DxClubMan.h"
#include "../../InnerInterface.h"
#include "RankingSlot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRankingSlot::CRankingSlot ( GLGaeaClient* pGaeaClient, EngineDeviceMan* pEngineDevice ) 
	: CUIGroup(pEngineDevice),
	m_pGaeaClient ( pGaeaClient ),
	m_pRankNum( NULL ),
	m_pRankName( NULL ),
	m_pRankKill( NULL ),
	m_pRankMoney( NULL ),
	m_pRankLevel( NULL ),
	m_pClubMark ( NULL ),
	m_bSelf(false)
{
	for ( int i=0; i<3; ++i )
	{
		m_pRankSchool[i] = NULL;
	}

	for ( int i=0; i < GLCI_NUM_TRICKER; ++ i )
	{
		m_pRankClass[i] = NULL;
	}
}

CRankingSlot::~CRankingSlot ()
{
}

void CRankingSlot::CreateSubControl ()
{
	CD3DFontPar* pFont = m_pEngineDevice->LoadFont ( _DEFAULT_FONT, 9, D3DFONT_SHADOW | D3DFONT_ASCII );

	CString strSchool[3] = 
	{
		"RANKING_SLOT_SCHOOL_SM",
		"RANKING_SLOT_SCHOOL_HA",
		"RANKING_SLOT_SCHOOL_BH"
	};

	CString strClass[GLCI_NUM_TRICKER] = 
	{
		"CTF_RESULT_RANKSLOT_IMAGE_BRAWLER_MALE",
		"CTF_RESULT_RANKSLOT_IMAGE_SWORDSMAN_MALE",
		"CTF_RESULT_RANKSLOT_IMAGE_ARCHER_FEMALE",
		"CTF_RESULT_RANKSLOT_IMAGE_SHAMAN_FEMALE",
		"CTF_RESULT_RANKSLOT_IMAGE_EXTREME_MALE",
		"CTF_RESULT_RANKSLOT_IMAGE_EXTREME_FEMALE",
		"CTF_RESULT_RANKSLOT_IMAGE_BRAWLER_FEMALE",
		"CTF_RESULT_RANKSLOT_IMAGE_SWORDSMAN_FEMALE",
		"CTF_RESULT_RANKSLOT_IMAGE_ARCHER_MALE",
		"CTF_RESULT_RANKSLOT_IMAGE_SHAMAN_MALE",
		"CTF_RESULT_RANKSLOT_IMAGE_SCIENCE_MALE",
		"CTF_RESULT_RANKSLOT_IMAGE_SCIENCE_FEMALE",
		"CTF_RESULT_RANKSLOT_IMAGE_ASSASIN_MALE",
		"CTF_RESULT_RANKSLOT_IMAGE_ASSASIN_FEMALE",
		"CTF_RESULT_RANKSLOT_IMAGE_TESTING_MALE",
		"CTF_RESULT_RANKSLOT_IMAGE_TESTING_FEMALE",
	};	

	for ( int i = 0; i < 3; ++ i )
	{
		m_pRankSchool[i] = CreateControl( strSchool[i].GetString() );
	}

	for ( int i=0; i < GLCI_NUM_TRICKER; ++ i )
	{
		//strTemp.Format ( "CTF_RESULT_ICON_CLASS_IMAGE%d",i); 
		m_pRankClass[i] = CreateControl( strClass[i].GetString() );;
	}
	
	//added by ejsayaaa
	m_pRankLevel = CreateStaticControl ( "RANKING_SLOT_LEVEL_TEXT", pFont, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_CENTER_X );
	m_pRankMoney = CreateStaticControl ( "RANKING_SLOT_MONEY_TEXT", pFont, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_CENTER_X );
	m_pRankNum = CreateStaticControl ( "RANKING_SLOT_NUM_TEXT", pFont, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_CENTER_X );
	m_pRankName = CreateStaticControl ( "RANKING_SLOT_NAME", pFont, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_CENTER_X );
	m_pRankKill = CreateStaticControl ( "RANKING_SLOT_KILL_TEXT", pFont, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_CENTER_X );
	m_pRankDeath = CreateStaticControl ( "RANKING_SLOT_DEATH_TEXT", pFont, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_CENTER_X );
	
	m_pClubMark = CreateControl ( "RANKING_SLOT_CLUB_MARK" );
	m_pClubMark->SetVisibleSingle ( FALSE );
}

CUIControl*	CRankingSlot::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl ( m_pEngineDevice );
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );
	return pControl;
}

CBasicTextBox* CRankingSlot::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox ( m_pEngineDevice );
	pStaticText->CreateSub ( this, szControlKeyword );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );
	RegisterControl ( pStaticText );
	return pStaticText;
}

void CRankingSlot::SetData ( const RANKING &sData )
{
	DWORD dwSize = (DWORD) (sizeof(DWORD)*EMCLUB_MARK_SX*EMCLUB_MARK_SY);
	m_sRANK.bLevel = sData.bLevel;
	m_sRANK.bRich = sData.bRich;
	m_sRANK.bPK = sData.bPK;

	m_sRANK.lMoney = sData.lMoney;
	m_sRANK.dwCharID = sData.dwCharID;
	m_sRANK.wClass = sData.wClass;
	m_sRANK.wDeaths = sData.wDeaths;
	m_sRANK.wKills = sData.wKills;
	m_sRANK.wSchool = sData.wSchool;
	m_sRANK.wLevel = sData.wLevel;
	m_sRANK.nOnline = sData.nOnline;
	m_sRANK.dwClub = sData.dwClub;
	m_sRANK.wLevel = sData.wLevel;
	m_sRANK.m_dwMarkVER = sData.m_dwMarkVER;
	memcpy(m_sRANK.m_aryMark, sData.m_aryMark, dwSize );
	StringCchCopy( m_sRANK.szName, CHAR_SZNAME+1, sData.szName );
	if ( m_sRANK.dwClub == 0 )
	{
		m_pClubMark->SetVisibleSingle ( FALSE );
	}
	else
	{
		const int nSERVER = m_pGaeaClient->GetCharacter()->m_dwServerID;
		const clubmark::DXDATA& sMarkData = DxClubMan::GetInstance().GetClubData ( m_pGaeaClient->GetGlobalStage()->GetD3dDevice(), nSERVER, m_sRANK.dwClub );

		m_pClubMark->SetTexturePos ( 0, sMarkData.vTex_1_LU );
		m_pClubMark->SetTexturePos ( 1, sMarkData.vTex_2_RU );
		m_pClubMark->SetTexturePos ( 3, sMarkData.vTex_3_LD );
		m_pClubMark->SetTexturePos ( 2, sMarkData.vTex_4_RD );

		m_pClubMark->SetTexture ( sMarkData.pddsTexture );
		m_pClubMark->SetVisibleSingle ( TRUE );
	}
}

void CRankingSlot::DataUpdate( int nNUM, std::string strNAME, int nSCHOOL, int nKILL, int nDEATH  , LONGLONG llMoney , WORD wClass , WORD wLevel, int nOnline )
{
	DataReset();

	if ( nSCHOOL < 0 || nSCHOOL >=3 )	return;

	if ( m_pRankSchool[nSCHOOL] )
		m_pRankSchool[nSCHOOL]->SetVisibleSingle( TRUE );

	D3DCOLOR dwColor =  NS_UITEXTCOLOR::WHITE;

	if  ( m_pRankLevel )
	{
		CString strTEXT;
		if ( wLevel == 0 ) strTEXT.Format( "-" );
		else
		strTEXT.Format( "Lv.%d", wLevel );
		m_pRankLevel->SetText( strTEXT.GetString(), dwColor );
	}

	EMCHARINDEX emIndex = CharClassToIndex((EMCHARCLASS)wClass);

	if ( m_pRankClass[(WORD)CharClassToIndex((EMCHARCLASS)wClass)] )
		m_pRankClass[(WORD)CharClassToIndex((EMCHARCLASS)wClass)]->SetVisibleSingle( TRUE );

	if ( m_pRankMoney )
	{
		CString strTEXT;
		if ( llMoney == 0 ) strTEXT.Format( "-" );
		else
		strTEXT.Format( NS_UITEXTUTIL::MAKE_MONEY_FORMAT(llMoney,3,",") );
		m_pRankMoney->SetText( strTEXT.GetString(), dwColor );
	}

	if ( m_pRankNum )
	{
		CString strTEXT;
		strTEXT.Format( "%d", nNUM );
		m_pRankNum->SetText( strTEXT.GetString(), dwColor );
	}

	if ( m_pRankName )
	{
		CString strTemp;
		int nIndex = m_pRankName->AddText( strNAME.c_str(), dwColor );
		strTemp.Format ( "RANKING_SLOT_STATUS_IMG%d",nOnline);
		m_pRankName->SetTextImage ( nIndex, strTemp.GetString() );	
	}

	if ( m_pRankKill )
	{
		CString strTEXT;
		if ( nKILL == 0 ) strTEXT.Format( "-" );
		else
		strTEXT.Format( "%d", nKILL );
		m_pRankKill->SetText( strTEXT.GetString(), dwColor );
	}

	if ( m_pRankDeath )
	{
		CString strTEXT;
		if ( nDEATH == 0 ) strTEXT.Format( "-" );
		else
		strTEXT.Format( "%d", nDEATH );
		m_pRankDeath->SetText( strTEXT.GetString(), dwColor );
	}
}

void CRankingSlot::DataReset()
{
	for ( int i = 0; i < 3; ++ i )
	{
		if ( m_pRankSchool[i] )
			m_pRankSchool[i]->SetVisibleSingle( FALSE );
	}

	for ( int i=0; i < GLCI_NUM_TRICKER; ++ i )
	{
		if ( m_pRankClass[i] )
			m_pRankClass[i]->SetVisibleSingle( FALSE );
	}

	m_sRANK = RANKING();

	if ( m_pRankNum )	m_pRankNum->ClearText();
	if ( m_pRankName )	m_pRankName->ClearText();
	if ( m_pRankKill )	m_pRankKill->ClearText();
	if ( m_pRankDeath ) m_pRankDeath->ClearText();
	if ( m_pRankMoney ) m_pRankMoney->ClearText();
	if  ( m_pRankLevel ) m_pRankLevel->ClearText();
	if ( m_pClubMark ) m_pClubMark->SetVisibleSingle ( FALSE );
}

void CRankingSlot::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if (!IsVisible()) return;	

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}