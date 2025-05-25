//PK Notification - Arvin.BC
#include "StdAfx.h"
#include "PKRankNotification.h"
#include "PKRankNotificationInfo.h"
#include "../../InnerInterface.h"
#include "../../../RanLogicClient/GLGaeaClient.h"
#include "../../../EngineLib/DxTools/DxViewPort.h"
#include "../../../RanLogicClient/Char/GLCharacter.h"

// ----------------------------------------------------------------------------
#include "../../../SigmaCore/DebugInclude.h"
// ----------------------------------------------------------------------------

CPKRankNotification::CPKRankNotification ( GLGaeaClient* pGaeaClient, CInnerInterface* pInterface, EngineDeviceMan* pEngineDevice ) :
CUIWindowEx( pInterface, pEngineDevice ),
	m_pInterface ( pInterface ),
	m_pGaeaClient ( pGaeaClient )
{
	for( int i = 0; i < PKNOTIF_NUM; ++ i )
	{
		m_pInfo[i] = NULL;
	}
}

CPKRankNotification::~CPKRankNotification()
{
}

void CPKRankNotification::CreateSubControl()
{
	std::string strInfo[PKNOTIF_NUM] = 
	{
		"PK_RANK_NOTIFICATION_0",
		"PK_RANK_NOTIFICATION_1",
		"PK_RANK_NOTIFICATION_2",
		"PK_RANK_NOTIFICATION_3",
		"PK_RANK_NOTIFICATION_4",
		"PK_RANK_NOTIFICATION_5",
		"PK_RANK_NOTIFICATION_6",
		"PK_RANK_NOTIFICATION_7",
		"PK_RANK_NOTIFICATION_8",
		"PK_RANK_NOTIFICATION_9",
	};

	for( int i = 0; i < PKNOTIF_NUM; ++ i )
	{
		m_pInfo[i] = new CPKRankNotificationInfo(m_pGaeaClient, m_pEngineDevice );
		m_pInfo[i]->CreateSub ( this, strInfo[i].c_str() );
		m_pInfo[i]->CreateSubControl ();
		RegisterControl ( m_pInfo[i] );
	}
}

void CPKRankNotification::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	for( int i = 0; i < PKNOTIF_NUM; ++ i )
	{
		if ( m_pInfo[i] )
			m_pInfo[i]->SetVisibleSingle( FALSE );
	}


	PK_HISTORY_VEC vecPKHistory = m_pGaeaClient->m_vecPKHistory;
	for( int i = 0; i < (int)vecPKHistory.size(); ++ i )
	{
		if ( i >= PKNOTIF_NUM )	continue;

		if ( m_pInfo[i] )
		{
			m_pInfo[i]->SetVisibleSingle( TRUE );
			m_pInfo[i]->SetData( vecPKHistory[i] );
		}
	}
}
