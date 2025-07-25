#include "../pch.h"
#include "../../Enginelib/Common/profile.h"
#include "../../Enginelib/DxTools/DebugSet.h"
#include "../../RanLogic/Msg/ServerMsg.h"
#include "../../RanLogic/Msg/GLContrlLottoSystemMsg.h"
#include "../Database/DBAction/DbActionGame.h"
#include "../Database/DBAction/DbActionGameItem.h"
#include "../Database/DBAction/DbActionUser.h"
#include "../Client/NetAgentClientMan.h"
#include "../Club/GLClubAgent.h"
#include "../InstanceSystem/InstanceSystem.h"
#include "./AgentServerConfigXml.h"
#include "./AgentServer.h"

#include "../TriggerManager/GLTriggerManagerAG.h"

// ----------------------------------------------------------------------------
#include "../../SigmaCore/DebugInclude.h"
// ----------------------------------------------------------------------------

//! 클라이언트->게임서버 : 캐릭터 선택후 조인
void CAgentServer::MsgGameJoinCA(DWORD dwClient, DWORD dwGaeaID, NET_MSG_GENERIC* pMsg)
{    
	if (!pMsg)
        return;

	// nProtect 서버 인증을 사용한다면 캐릭터가 접속할때 인증이 왔는지 체크한다.	
	if (m_bGameGuardInit == true && 
		m_bGameGuardAuth == true)
	{
#ifndef _NP30
		if ( m_pClientManager->nProtectFirstAuthPass( dwClient ) == false )		
		{
			// nProtect 인증 실패
			// 연결을 종료시킨다.
			CloseClient(dwClient, net::KICKOUT_GAMEGUARD);
			return;
		}
		else
		{
			// 현재 nProtect 받기상태라면
			if (m_bHeartBeat == false)
			{
				// 인증상태를 리셋 시킨다.
				m_pClientManager->nProtectResetAuth(dwClient);
				// 게임가드 인증 쿼리 가져오기 준비
				if (m_pClientManager->nProtectSetAuthQuery(dwClient) == true)
				{
					// 게임가드 인증 쿼리 전송
					NET_GAMEGUARD_AUTH nga;
					nga.ggad = m_pClientManager->nProtectGetAuthQuery(dwClient);
					SendClient(dwClient, &nga);
				}

			}
		}
#else
		//// 현재 nProtect 받기상태라면
		//if (m_bHeartBeat == false)
		//{
		//	if ( m_pClientManager->nProtectFirstAuthPass( dwClient ) == true )
		//	{
		//		// 인증상태를 리셋 시킨다.
		//		m_pClientManager->nProtectResetAuth(dwClient);
		//		// 게임가드 인증 쿼리 가져오기 준비
		//		int nReceivedPacketSize = m_pClientManager->nProtect30SetAuthQuery(dwClient);
		//		// 게임가드 인증 쿼리 전송
		//		BYTE *pNGA = m_pClientManager->nProtect30GetAuthQuery(dwClient);

		//		if( pNGA && nReceivedPacketSize > 0)
		//		{
		//			for(int i = 0; i < 8; i++)
		//			{
		//				// 다시 한번 인증쿼리 전송
		//				NET_GAMEGUARD_AUTH nga;
		//				nga.packetSize = nReceivedPacketSize;
		//				nga.packetSeq = i+1;
		//				memcpy(nga.packet, pNGA + (i*512), sizeof(BYTE)*512);
		//				SendClient(dwClient, &nga);
		//				sc::writeLogDebug(sc::string::format("NET_GAMEGUARD_AUTH - MsgGameJoinCA : idx %1%, PacketSize %2%", i+1, nReceivedPacketSize));
		//			}
		//		}
		//		else
		//		{
		//			sc::writeLogError(std::string("GameGuard Packet ERROR (MsgGameJoinCA)"));
		//		}
		//	}
		//}
#endif
	}

	// 인증을 통과하지 않은 상태에서 캐릭터 정보요청함
	if (m_pClientManager->IsAccountPass(dwClient) == false)
	{
		sc::writeLogInfo(std::string("MsgGameJoin IsAccountPass false"));
		CloseClient(dwClient, net::KICKOUT_UNDEFINE);
		return;
	}

	NET_LOBBY_CHAR_JOIN_CA* pPacket = (NET_LOBBY_CHAR_JOIN_CA*) pMsg;

	if (!pPacket)
        return;

	{
		DWORD dwUserDbNum = static_cast< DWORD >( m_pClientManager->UserDbNum( dwClient ) );

		// 2차비번
		// 기존에 캐릭 저장중인 경우와 일정시간내에 연속 로그인 못하도록 하는 부분이 한꺼번에 처리되므로 구분이 어려워서 분리 시킴
		if ( m_pGLAgentServer->IsChaSaving( dwUserDbNum ) )
		{
			// 아직 캐릭터 저장중입니다. 못들어온다.
			GLMSG::SNETLOBBY_CHARJOIN_FB NetMsgFB;
			NetMsgFB.dwChaNum = static_cast< DWORD >( pPacket->m_ChaDbNum );
			NetMsgFB.emCharJoinFB = EMCJOIN_FB_SAVE_CHA;
			SendClient( dwClient, &NetMsgFB );
			return;
		}

		if( false == m_pClientManager->VerifyUserNum( dwUserDbNum, GetServiceProvider() ) )
		{
			GLMSG::SNETLOBBY_CHARJOIN_FB NetMsgFB;
			NetMsgFB.dwChaNum = static_cast< DWORD >( pPacket->m_ChaDbNum );
			NetMsgFB.emCharJoinFB = EMCJOIN_FB_WAIT;
			SendClient( dwClient, &NetMsgFB );
			return;
		}
		if ( IsBlockUser( dwUserDbNum ) )
		{
			// EMCJOIN_FB_SAVE_CHA2 설명 참조
			GLMSG::SNETLOBBY_CHARJOIN_FB NetMsgFB;
			NetMsgFB.dwChaNum = static_cast< DWORD >( pPacket->m_ChaDbNum );
			NetMsgFB.emCharJoinFB = EMCJOIN_FB_SAVE_CHA2;
			SendClient( dwClient, &NetMsgFB );
			return;
		}
		////////////////////////////////////////////////
	}

	DWORD dwChaNum  = (DWORD) pPacket->m_ChaDbNum;
	DWORD dwUserID  = (DWORD) m_pClientManager->UserDbNum(dwClient);
	DWORD dwUserLvl = (DWORD) m_pClientManager->GetUserType(dwClient);
    __time64_t tPREMIUM   = m_pClientManager->GetPremiumDate(dwClient);
    __time64_t tCHATBLOCK = m_pClientManager->GetChatBlockDate(dwClient);

	//mjeon.AF
	if(!m_pClientManager->IsValidChaNum(dwClient, dwChaNum))
	{
		sc::writeLogError(sc::string::format("CAgentServer::MsgGameJoinCA() - The Char(%1%) is NOT belong to the User(%2%). Check it out !!", dwChaNum, dwUserID));
		CloseClient(dwClient, net::KICKOUT_UNDEFINE);
		return;
	}
    
	// 캐릭터 번호 세팅
	m_pClientManager->SetChaNum(dwClient, dwChaNum);

	// 2차비번
	// DBAction에서 쿼리 결과가 실패한다면 return 시켜버리는 부분이 있으므로 캐릭터 번호 세팅후 실패메세지를 전송해도 상관없을듯
	if( m_pClientManager->IsBlockBy2ndPassword(dwUserID) )
	{
		GLMSG::SNETLOBBY_CHARJOIN_2NDPASS NetMsg;
		NetMsg.nResult = -3; // 블럭상태는 -3
		NetMsg.tBlockExpiredTime = m_pClientManager->GetBlockExpiredTime(dwUserID);
		SendClient( dwClient, &NetMsg );
		return;
	}
	///////////////////////////////////////

	// db action에 케릭터 정보 가져오기 예약, 이후 케릭터 접속.    
    AddGameAdoJob(
        db::DbActionPtr(
            new db::CharacterInfoAndJoinAgent(
                dwClient,
                dwUserID,
                dwUserLvl,
                tPREMIUM,
                tCHATBLOCK,
                (DWORD) m_nServerGroup,
                dwChaNum,
				m_pClientManager->IsUse2ndPass(dwClient),
				pPacket->m_sz2ndPass)));	//mjeon.ado
}

void CAgentServer::MsgAgentReqJoinDA(DWORD dwClient, DWORD dwGaeaID, NET_MSG_GENERIC* pMsg)
{
    // DB에서 온 패킷이 아니라면 처리할 수 없다.
    if( dwClient != DBACTION_CLIENT_ID )
        return;


	if (!pMsg)
        return;
	GLMSG::NET_CHAR_JOIN_DA* pPacket = (GLMSG::NET_CHAR_JOIN_DA*) pMsg;
	if (!pPacket)
        return;

	//	[주의] 이 호출 auto_ptr 구문이 가장 먼저 호출되어야 한다.
	//	이 함수에서 탈출시에 자동으로 제거되게 한다. ( 메시지 처리후 무효화. )
	std::auto_ptr<GLCHARAG_DATA> pCHAR_DATA(pPacket->m_pCharAgData);
	if (!pCHAR_DATA.get())
        return;

	//	메시지가 전달된 clientid.
	DWORD ClientSlot = pPacket->m_dwClientID;

	//GASSERT(m_pClientManager->IsClientSlot(ClientSlot)&&"MsgAgentReqJoinDA - Not ClientSlot!");

	if (!m_pClientManager->IsClientSlot(ClientSlot))
		return;

	if (!m_pClientManager->IsAccountPass(ClientSlot))
        return;

	//	Note : 클라이언트 메니져에 설정된 것과 틀릴 경우 취소됨.
	DWORD dwCharNum = (DWORD) m_pClientManager->ChaDbNum(ClientSlot);
	if (dwCharNum!=pCHAR_DATA->m_CharDbNum)
	{
		sc::writeLogError(std::string("dwCharNum!=pCHAR_DATA->m_dwCharID"));
		return;
	}

	// strcpy ( pCHAR_DATA->m_szUserName, m_pClientManager->GetUserID(dwClientID) );
	pCHAR_DATA->SetUserId(m_pClientManager->GetUserID(ClientSlot));
	pCHAR_DATA->SetIp(m_pClientManager->GetClientIP(ClientSlot).c_str());

	int nChannel = m_pClientManager->GetChannel(ClientSlot);

	bool bPKCHANNEL = m_pConfig->IsPkChannel(nChannel);

	if (!bPKCHANNEL && (pCHAR_DATA->m_nBright < GLCONST_CHAR::GetNonPkConnectBrightPoint()))
	{
		//	접속 시도자에게  메시지를 보냅니다.
		GLMSG::SNETLOBBY_CHARJOIN_FB NetMsgFB;
		NetMsgFB.dwChaNum = dwCharNum;
		NetMsgFB.emCharJoinFB = EMCJOIN_FB_PKPOINT;
		//m_pClientManager->SendClient(&NetMsgFB);
		SendClient(ClientSlot, &NetMsgFB);
		return;
	}

	switch ( GetServiceProvider() )
	{
	case SP_GS:
		{
			CTime NowTime = CTime::GetCurrentTime();
			CTime EndTime( 2016, 11, 24, 9, 0, 0, -1 );
			EMCHARCLASS emCharClass = static_cast< EMCHARCLASS >( pCHAR_DATA->m_emClass );
			if ( NowTime < EndTime && ( GLCC_ACTOR_M == pCHAR_DATA->m_emClass || GLCC_ACTOR_W == pCHAR_DATA->m_emClass ) )
			{
				GLMSG::SNETLOBBY_CHARJOIN_FB NetMsgFB;
				NetMsgFB.dwChaNum = dwCharNum;
				NetMsgFB.emCharJoinFB = EMCJOIN_FB_BLOCK_CLASS;
				SendClient( ClientSlot, &NetMsgFB );
				return;
			}
		}
		break;
	default:
		break;
	}

	EMIP_BONUS_CLASS emIPBonus = EMIP_BONUS_NONE;

	// 태국 요청으로 ip 보너스 사용하지 않는다.
	// http://crm.mincoms.com/Ticket/TicketView/1901
	// 우선 처음에 받아오는 것만 제거한다.
//	IP보너스
//#if defined(TH_PARAM) || defined(MYE_PARAM) || defined(MY_PARAM) || defined ( PH_PARAM ) //|| defined(_RELEASED) 
	//if (m_ServiceProvider == SP_PHILIPPINES || m_ServiceProvider == SP_MALAYSIA || m_ServiceProvider == SP_MALAYSIA_EN)
	{
		emIPBonus = m_pClientManager->GetIPBonusClass(ClientSlot);
	}
//#endif

	// 중국 누적 시간 셋팅
	SChinaTime sChinaTime;
	sChinaTime.Init();

	// 현재 중국이 아니더라도 이벤트 시간을 위해 현재 로그인한 시간을 구한다.
	// 현재 로그인한 시간
	CTime crtTime			   = CTime::GetCurrentTime();
	sChinaTime.loginTime	   = crtTime.GetTime();

	//////////////////////////////////////////////////////////////////////////
	// 중국 탐닉 방시 시스템 계산
#ifdef CH_PARAM_USEGAIN //chinaTest%%%
	// 적용 대상인지 아닌지
	sChinaTime.userAge   = m_pClientManager->GetChinaUserAge( ClientSlot );

	// 게임 플레이 누적 시간
	sChinaTime.gameTime		   = m_pClientManager->GetChinaGameTime(ClientSlot);

	// 오프라인 누적 시간
	sChinaTime.offlineTime     = m_pClientManager->GetChinaOfflineTime(ClientSlot);
	CTimeSpan offlineSpan( 0, sChinaTime.offlineTime / 60, sChinaTime.offlineTime % 60, 0 );

	// 마지막 로그오프 시간
	sChinaTime.lastOffLineTime = m_pClientManager->GetLastLoginDate(ClientSlot);

	// 현재 게임 누적 시간
	sChinaTime.currentGameTime = 0;

	if( sChinaTime.lastOffLineTime != 0 )
	{
		CTime lastOffTime		   = sChinaTime.lastOffLineTime;	
		CTimeSpan totalOffTime = (crtTime - lastOffTime) + offlineSpan;
		CTimeSpan totalGameTime( 0, (int)sChinaTime.gameTime / 60, (int)sChinaTime.gameTime % 60, 0 );
		// 로그오프한 시간이 5시간보다 크거나 같으면 누적시간을 초기화한다.
		if( totalOffTime.GetTotalHours() >= 5 )		
		//if( totalOffTime.GetTotalMinutes() >= 3 )		
		{
			sChinaTime.offlineTime	   = 0;
			sChinaTime.gameTime		   = 0;
			sChinaTime.lastOffLineTime = 0;
			sChinaTime.currentGameTime = 0;


			m_pClientManager->SetChinaOfflineTime(ClientSlot, 0 );
			m_pClientManager->SetChinaGameTime(ClientSlot, 0 );
		}else{
			if( totalGameTime.GetTotalHours() >= 5 && sChinaTime.userAge != 1 )
			//if( totalGameTime.GetTotalMinutes() >= 1 && sChinaTime.userAge != 1 )
			{
				// 중국 로그인시 플레이 누적시간이 5시간이 넘고 오프라인 누적시간이 5시간이 되지 않았을 경우 에러 메시지를 보낸다.
				GLMSG::SNETLOBBY_CHARJOIN_CHINA_ERROR NetMsg;
				m_pClientManager->SendClient( ClientSlot, &NetMsg );
				return;

			}else{
				m_pClientManager->SetChinaOfflineTime(ClientSlot, (int)totalOffTime.GetTotalMinutes() );
			}
		}
	}else{
		sChinaTime.offlineTime	   = 0;
		sChinaTime.gameTime		   = 0;
		sChinaTime.lastOffLineTime = 0;
		sChinaTime.currentGameTime = 0;
	}



#endif

	//////////////////////////////////////////////////////////////////////////
	// 베트남 탐닉 방지 시스템 계산
	SVietnamGainSystem sVietnamTime;
	sVietnamTime.Init();

//#if defined(VN_PARAM) //vietnamtest%%%
	if (m_ServiceProvider == SP_VIETNAM)
	{
		// 현재 로그인한 시간
		crtTime					   = CTime::GetCurrentTime();
		sVietnamTime.loginTime	   = crtTime.GetTime();

		// 게임 플레이 누적 시간
		sVietnamTime.gameTime		 = m_pClientManager->GetVTGameTime(ClientSlot);

		// 마지막 로그오프 시간
		sVietnamTime.lastOffLineTime = m_pClientManager->GetLastLoginDate(ClientSlot);

		// 현재 게임 누적 시간
		sVietnamTime.currentGameTime = 0;

		if( sVietnamTime.lastOffLineTime != 0 )
		{
			CTime lastOffTime		   = sVietnamTime.lastOffLineTime;	
			// 오프라인한 날짜가 다르면 초기화 한다.
			bool bInitialize = FALSE; // 테스트로 무조건 초기화 한다.

			// 년도, 달, 날짜가 다르면 초기화 한다.
			if( lastOffTime.GetYear() != crtTime.GetYear() ||
				lastOffTime.GetMonth() != crtTime.GetMonth() ||
				lastOffTime.GetDay() != crtTime.GetDay()  )
			{
	//			sc::writeLogInfo( _T("INFO:Vietnam Accumulation Time Init Last Off Time Month %d, Day %d CharID[%s], CharAccount[%s]" ), 
	//												   lastOffTime.GetMonth(), lastOffTime.GetDay(), pCHAR_DATA->m_szName, pCHAR_DATA->m_szUserName  );
				bInitialize = TRUE;
			}

			if( sVietnamTime.gameTime < 0 )
			{
	//			sc::writeLogInfo( _T("INFO:Vietnam GameTime not correct value...") );
				bInitialize = TRUE;
			}
				
			if( bInitialize )
			{
				sVietnamTime.gameTime		 = 0;
				sVietnamTime.lastOffLineTime = 0;
				sVietnamTime.currentGameTime = 0;

				m_pClientManager->SetVTGameTime(ClientSlot, 0 );

			}
		}else{
			sVietnamTime.gameTime		 = 0;
			sVietnamTime.lastOffLineTime = 0;
			sVietnamTime.currentGameTime = 0;

			m_pClientManager->SetVTGameTime(ClientSlot, 0 );
		}


		pCHAR_DATA->m_sVietnamSystem = sVietnamTime;
	}
//#endif

	if( CheckTracingUser( pCHAR_DATA->m_szUserName, pCHAR_DATA->m_UserDbNum ) != -1 )
	{
		pCHAR_DATA->m_bTracingUser   = FALSE;
	}
	
	// 캐릭터 생성
	GLCharAG* pChar = m_pGLAgentServer->CreateChar(
        pCHAR_DATA.get(),
        ClientSlot,
        nChannel,
        sChinaTime,
        emIPBonus,
        m_pClientManager->GetCountry(ClientSlot),
		m_pClientManager->GetCryptKey(ClientSlot));
	if (!pChar)
	{
		//	접속 시도자에게  메시지를 보냅니다.
		GLMSG::SNETLOBBY_CHARJOIN_FB NetMsgFB;
		NetMsgFB.dwChaNum = dwCharNum;
		NetMsgFB.emCharJoinFB = EMCJOIN_FB_ERROR;
		//m_pClientManager->SendClient(&NetMsgFB);
		SendClient(ClientSlot, &NetMsgFB);

		sc::writeLogError(std::string("Initialize character instance failed"));
		sc::writeLogError(sc::string::format("ClientID[%u], Channel[%d]", ClientSlot, nChannel ));
		sc::writeLogError(sc::string::format("CharIP[%s], CharIDNum[%u], UserIDNum[%u], CharID[%s], CharAccount[%s]",
            pCHAR_DATA->m_szIp,
            pCHAR_DATA->m_CharDbNum,
            pCHAR_DATA->m_UserDbNum,
            pCHAR_DATA->m_szName,
            pCHAR_DATA->m_szUserName));
		return;
	}	

	// 필리핀 이관 캐릭터 클럽 가입 처리
	switch ( GetServiceProvider() )
	{
	case SP_GS:
		m_pGLAgentServer->AutoClubJoinForMigration( pChar, ClientSlot );
		break;
	default:
		break;
	}

	// 학원 별 클럽 자동 가입, 학원이 추가되면 이 부분도 수정 필요
	if ( m_pConfig->GetAutoClubJoin() && pPacket->m_CharacterOption & OPTION_NEW_CHARACTER ? true : false )
	{
		if ( m_pGLAgentServer->AutoClubJoin( m_pConfig->GetAutoClubJoinClubDbNum( pChar->GetSchool() ), pChar, ClientSlot ) )
		{
			// 캐릭터옵션 업데이트
			int Option = pPacket->m_CharacterOption;
			Option &= ~OPTION_NEW_CHARACTER;
			AddGameAdoJob( db::DbActionPtr( new db::SetCharacterOption( pChar->ChaDbNum(), Option ) ) );
		}
	}

	/*if (pCHAR_DATA->m_bTracingUser)
	{
		CDebugSet::ToTracingFile(
			pCHAR_DATA->m_szUserName,
			"Character Select!, [%s][%s], IP:[%s], Channel:[%d], UserNum:[%u], CharNum:[%u]",
			pCHAR_DATA->m_szUserName,
			pCHAR_DATA->m_szName,
			pCHAR_DATA->m_szIp,
			pChar->m_nChannel,
			pCHAR_DATA->m_UserDbNum,
			pCHAR_DATA->m_CharDbNum);
	}*/

	DWORD dwChaGaeaID = pChar->GaeaId();
	GASSERT(dwChaGaeaID != GAEAID_NULL);

	//	가이아서버에 정보 세팅
	m_pClientManager->SetGaeaID(ClientSlot, dwChaGaeaID);

		
	// User DB 에 플레이 중인 캐릭터 이름을 세팅한다.
	if (pChar)
	{	
		m_pDbManager->AddUserJob(
            db::DbActionPtr(
                new CUserUpdateCha(
                    m_pClientManager->UserDbNum(ClientSlot),
                    pChar->m_szName)));
	}

    // 접속시 트리거
    triggermanager::GLTriggerManagerAG::GetInstance().Get_Trigger_Connect_To_Field().Do( pChar );
}

//! Field->Agent : 캐릭터가 게임에 조인했음
void CAgentServer::MsgLobbyCharJoinFA(DWORD dwClient, DWORD _dwGaeaID, NET_MSG_GENERIC* nmg)
{
	GASSERT(dwClient < (DWORD) m_nMaxClient);
	GASSERT(m_pClientManager->IsAgentFieldSlot(dwClient) && "Slot must be AgentFieldSlot!");
	
	NET_GAME_JOIN_FIELDSVR_FB* pPacket = (NET_GAME_JOIN_FIELDSVR_FB*) nmg;
	
	if (!pPacket)
        return;
	
	//DWORD dwClientID = dwClient; // - (DWORD) m_nMaxClient;	//mjeon.AF
	//int nChannel = m_pClientManager->GetChannel(dwClientID);

	DWORD dwChaNum		= pPacket->m_ChaDbNum;
	DWORD dwGaeaID		= pPacket->m_GaeaID;
	DWORD dwFieldNum	= pPacket->m_FieldServerNum;

	DWORD dwClientID	= m_pClientManager->GetClientID(dwChaNum);

	//GASSERT((dwClientID != CLIENTID_NULL)&&"dwClientID is CLIENTID_NULL");
	
	if (dwClientID == CLIENTID_NULL)
	{
		sc::writeLogError(sc::string::format("CAgentServer::MsgLobbyCharJoinFA() - Cannot get ClientID for the ChaNum(%1%).", dwChaNum));
		return;
	}

	if (dwGaeaID != m_pClientManager->GetGaeaID(dwClientID))
	{
		sc::writeLogError(sc::string::format("CAgentServer::MsgLobbyCharJoinFA() - Char(%1%)'s GaeaID(%2%) is different from ClientID(%3%)'s GaeaID(%4%).", dwChaNum, dwGaeaID, dwClientID, m_pClientManager->GetGaeaID(dwClientID)));
        return;
	}

	GLCharAG* pChar = m_pGLAgentServer->GetChar(dwGaeaID);
	if (!pChar)
	{
		sc::writeLogError(sc::string::format("CAgentServer::MsgLobbyCharJoinFA() - Cannot reset InstanceKey for the Char(%1%).", dwChaNum));
		return;
	}

	pChar->ResetInstanceKey();

	DWORD dwChannel	= m_pClientManager->GetChannel(dwClientID);
	
    F_SERVER_INFO* pField = GetFieldServerInfo(dwFieldNum, dwChannel);
	if (!pField)
	{
		sc::writeLogError(sc::string::format("CAgentServer::MsgLobbyCharJoinFA() - Cannot get FieldInfo with FieldNum(%1%) and Channel(%2%).", dwFieldNum, dwChannel));
        return;
	}

	const InstanceSystem::ManagerAgent* const _pInstanceManager(m_pGLAgentServer->GetInstanceManager());
	const MapID& mapIDPrevious(pChar->GetPreviousMap());
	NET_CONNECT_CLIENT_TO_FIELD_AC MsgConnect;
	MsgConnect.dwGaeaID		= dwGaeaID;	
	MsgConnect.dwChaNum		= dwChaNum;
	MsgConnect.emType		= pPacket->m_JoinType;	
	MsgConnect.typeAuthority		= _pInstanceManager->getAuthority(dwChaNum);
	MsgConnect.previousMapID	= mapIDPrevious;
	MsgConnect.SetIP(pField->szPublicIp);
	MsgConnect.nServicePort = pField->nServicePort;

	SendClient(dwClientID, &MsgConnect);

	//
	//mjeon.CaptureTheField
	//
	//  CTF의 상태 동기화
	//
	m_pGLAgentServer->CTFSyncState(dwClientID);

	//mjeon.groupchat
	m_pGLAgentServer->GroupChatSyncConfig(dwClientID);


    // 캐릭터의 대략적인 위치 세팅
	const MapID& _newMapID(pPacket->m_MapID);
    pChar->SetCurrentMap(_newMapID);
    int PosX = 0;
    int PosY = 0;
//    m_pGLAgentServer->ConvertWorldToMap(_newMapID.getBaseMapID(), pPacket->m_MapPos, PosX, PosY);    
    pChar->SetMapPosition(pPacket->m_MapPos, false);

    // 친구들에게 online 상태 전송, 클럽원들에게 online 상태 전송
    if (pPacket->m_JoinType == EMJOINTYPE_FIRST)
        pChar->Online();

	// Lotto System;
	GLMSG::NET_LOTTO_MESSAGE_SYNC_AGENT_MYSELF sMsg;
	sMsg.dwChaNum = dwChaNum;
	m_pGLAgentServer->SENDTOMYSELF( &sMsg );
	//m_pGLAgentServer->LottoSystemSyncState( dwClientID, dwChaNum );

	// Matching System;
	m_pGLAgentServer->MatchingSystemSyncState( dwClientID, dwChaNum );
}

// 2차비번
void CAgentServer::MsgChaJoin2ndPassword(DWORD dwClient, DWORD dwGaeaID, NET_MSG_GENERIC* pMsg)
{
    if( dwClient != DBACTION_CLIENT_ID )
        return;

	if (!pMsg)
        return;

	GLMSG::NET_IN_CHAJOIN_2NDPASSWORD* pPacket = (GLMSG::NET_IN_CHAJOIN_2NDPASSWORD*) pMsg;

	if (!pPacket)
        return;

	DWORD ClientSlot = pPacket->m_dwClientID;
	DWORD dwUserDBNum = pPacket->m_dwUserDBNum;

	if (!m_pClientManager->IsClientSlot(ClientSlot))
		return;

	if( pPacket->m_nResult == 0 ) // 성공
	{
		// 만약 2차비번 초기설정이 되지 않은 상태였다면 설정을 완료했으므로 상태를 변경해준다
		if(m_pClientManager->IsUse2ndPass(ClientSlot) == 2 )
			m_pClientManager->SetUse2ndPass(ClientSlot, 1);

		m_pClientManager->ResetCertification2ndPassword(dwUserDBNum);
	}
	else if( pPacket->m_nResult == -2 || pPacket->m_nResult == -3 )
	{
		m_pClientManager->Increase2ndPasswordError(dwUserDBNum);
	}
	else
	{
		sc::writeLogError(std::string("CAgentServer::MsgChaJoin2ndPassword() - Unknown 2nd Password Certification ERROR "));
	}

	GLMSG::SNETLOBBY_CHARJOIN_2NDPASS NetMsg;
	NetMsg.nResult = pPacket->m_nResult;
	NetMsg.tBlockExpiredTime = m_pClientManager->GetBlockExpiredTime(dwUserDBNum);
	SendClient(ClientSlot, &NetMsg);
}
/////////////////////////////////////

void CAgentServer::MsgJoinInstance(DWORD dwClient, DWORD _dwGaeaID, NET_MSG_GENERIC* nmg)
{
	GASSERT(dwClient<(DWORD)m_nMaxClient);
	GASSERT(m_pClientManager->IsMatchSlot(dwClient)&&"Slot must be MatchSlot!");
	
	NET_MSG_JOIN_INSTANCESVR_INFO* pMsg = (NET_MSG_JOIN_INSTANCESVR_INFO*)nmg;
	
	if (!pMsg)
        return;

	DWORD dwChaNum		= pMsg->dwChaNum;
	DWORD dwGaeaID		= pMsg->dwGaeaID;

	DWORD dwClientID	= m_pClientManager->GetClientID(dwChaNum);

	GASSERT((dwClientID != CLIENTID_NULL)&&"dwClientID is CLIENTID_NULL");
	
	if (dwClientID == CLIENTID_NULL)
	{
		sc::writeLogError(sc::string::format("CAgentServer::MsgJoinInstance() - Cannot get ClientID for the ChaNum(%1%).", dwChaNum));
		return;
	}

	if (dwGaeaID != m_pClientManager->GetGaeaID(dwClientID))
	{
		sc::writeLogError(sc::string::format("CAgentServer::MsgJoinInstance() - Char(%1%)'s GaeaID(%2%) is different from ClientID(%3%)'s GaeaID(%4%).", dwChaNum, dwGaeaID, dwClientID, m_pClientManager->GetGaeaID(dwClientID)));
        return;
	}

	SendClient(dwClientID, pMsg);
}



///////////////////////////////////////////////////////////////////////////////
// Agent->Client
// 클라이언트가 캐릭터 선택 화면에서 디스플레이 해야할
// 캐릭터의 고유번호를 전송한다.
void CAgentServer::MsgSndChaBasicBAInfo(DWORD dwClient, DWORD dwGaeaID, NET_MSG_GENERIC* nmg)
{
	if (!nmg)
        return;

	DWORD dwSndBytes   = 0;	
	int nUserNum       = 0;
	int nSvrGrp        = 0;
	int nServerChannel = 0;
//	int nChannel;

	// 인증을 통과하지 않은 상태에서 캐릭터 정보요청함
	if (!m_pClientManager->IsAccountPass(dwClient))
	{
		sc::writeLogInfo(std::string("MsgSndChaBasicBAInfo IsAccountPass false"));
		CloseClient(dwClient, net::KICKOUT_UNDEFINE);
		return;
	}

	NET_CHA_REQ_BA_INFO* pPacket = (NET_CHA_REQ_BA_INFO*) nmg;
    // Crc 체크
    unsigned int Crc = sc::string::getCrc32(pPacket, NET_CHA_REQ_BA_INFO_DATA_SIZE);
    if (pPacket->m_Crc != Crc)
    {
        sc::writeLogError(
            sc::string::format(
                "MsgSndChaBasicBAInfo invalid crc %1%/%2% UserID %3%",
                Crc,
                pPacket->m_Crc,
                m_pClientManager->GetUserID(dwClient)));
        return;
    }

    // 버전 체크
    if (pPacket->m_LauncherVer != m_LauncherVersion || pPacket->m_PatchVer != m_PatchVersion)
    {
        sc::writeLogError(
            sc::string::format(
                "Invalid client version. Launcher %1%/%2% Patch %3%/%4% UserID %5%",
                m_LauncherVersion,
                pPacket->m_LauncherVer,
                m_PatchVersion,
                pPacket->m_PatchVer,
                m_pClientManager->GetUserID(dwClient)));
        NET_CLIENT_VERSION_AC MsgAC(m_LauncherVersion, m_PatchVersion);
        SendClient(dwClient, &MsgAC);
        return;
    }


	/*
	nChannel = pNcrbi->nChannel;

	// 채널 번호가 올바른지 검사한다.
	if (nChannel < 0 || nChannel >= MAX_CHANNEL_NUMBER)
	{
		sc::writeLogError(
			                                  _T("MsgSndChaBasicBAInfo Invalid Channel Number (Channel:%d)"),
			                                  nChannel);
		return;
	}

	// 해당 채널을 조사해서 full 인지 검사해야 한다.
	if (IsChannelFull(nChannel) == true)
	{
		// 해당 채널이 full 이면 연결을 끊어야 한다.
		// Full 일 경우 처리한다.		
		CloseClient(dwClient);
		sc::writeLogError(
											  _T("CH(%d) is FULL"),
			                                  nChannel);
		return;
	}
	else
	{
		// Full 이 아닐경우 해당 채널 유저수를 1 증가시킨다.
		IncreaseChannelUser(nChannel);
		// 클라이언트에 채널번호를 세팅한다.
		m_pClientManager->SetChannel(dwClient,nChannel);                
	}
	*/

	nUserNum = m_pClientManager->UserDbNum(dwClient);
	if (nUserNum < 1) // 치명적 에러, 허가되지 않은 사용자 번호
	{
		sc::writeLogError(sc::string::format("MsgSndChaBasicBAInfo nUserNum(%d)", nUserNum ));
		return;
	}

	nSvrGrp = m_nServerGroup;

    // Cache 서버에서 처리. 2011-07-18 jgkim
    NET_CHA_LIST_AH MsgAH(
        nUserNum,
        nSvrGrp,
        dwClient,
        m_pClientManager->GetClientIP(dwClient).c_str(),
        m_pClientManager->GetClientPort(dwClient));
    SendCache(&MsgAH);
}

void CAgentServer::MsgSndChaBasicBAInfoBack(NET_MSG_GENERIC* pMsg)
{
    NET_CHA_BBA_INFO_HA* pPacket = (NET_CHA_BBA_INFO_HA*) pMsg;
    DWORD ClientSlot = pPacket->m_AgentServerSlot;
    if (!m_pClientManager->IsOnline(ClientSlot))
    {
        sc::writeLogInfo(std::string("MsgSndChaBasicBAInfoBack Offline"));
        return;
    }

	// IP/PORT 가 동일할때만 처리
	if ((strcmp(pPacket->m_Ip, m_pClientManager->GetClientIP(ClientSlot).c_str()) == 0) &&
		(m_pClientManager->GetClientPort(ClientSlot) == pPacket->m_uPort))
	{
		m_pClientManager->ResetValidChaNumList(ClientSlot);

// 		std::vector< DWORD > vecChaNum;
// 		for ( int loop = 0; loop < pPacket->m_ChaServerTotalNum; loop++ )
// 		{
// 			vecChaNum.push_back( pPacket->m_ChaDbNum[loop] );
// 		}
// 
// 		AddGameAdoJob( db::DbActionPtr( new db::ItemTransfer( m_pClientManager->UserDbNum( ClientSlot ), vecChaNum, pPacket  )));
// 		return;

        NET_CHA_BBA_INFO_AC MsgAC;
        MsgAC.SetChaTotalNum(pPacket->m_ChaServerTotalNum);

        for (size_t i=0; i<MAX_ONESERVERCHAR_NUM; ++i)
		{			
			m_pClientManager->ValidateChaNum(ClientSlot, pPacket->m_ChaDbNum[i]);	//mjeon.AF
            MsgAC.SetChaNum(i, pPacket->m_ChaDbNum[i]);
		}
		SendClient(ClientSlot, &MsgAC);

		switch ( GetServiceProvider() )
		{
		case SP_GS:
			{
				NET_DEFAULT_GAME_OPTION Msg;
				Msg.bPARTY_2OTHERSCHOOL = GLCONST_CHAR::bPARTY_2OTHERSCHOOL;
				Msg.bCLUB_2OTHERSCHOOL = GLCONST_CHAR::bCLUB_2OTHERSCHOOL;
				Msg.fITEM_DROP_SCALE = GLCONST_CHAR::fITEM_DROP_SCALE;
				Msg.fMONEY_DROP_SCALE = GLCONST_CHAR::fMONEY_DROP_SCALE;
				Msg.fITEM_DROP_LOWER_SCALE = GLCONST_CHAR::fITEM_DROP_LOWER_SCALE;
				Msg.dwCRITICAL_MAX = GLCONST_CHAR::dwCRITICAL_MAX;
				Msg.fNONPK_ITEM_DROP = GLCONST_CHAR::fNONPK_ITEM_DROP;
				Msg.fPK_ITEM_DROP = GLCONST_CHAR::fPK_ITEM_DROP;
				SendClient(ClientSlot, &Msg);
			}
			break;
		default:
			break;
		}
	}
    else
    {
        sc::writeLogError(std::string("MsgSndChaBasicBAInfoBack Online but not same Address/Port"));
    }
}

void CAgentServer::MsgSndChaBasicInfo(DWORD dwClient, int nChaNum)
{
	// 해킹을 통해서 nChaNum 을 0 으로 만들수 있음	
	if (nChaNum <= 0) 
		return;

	// 인증을 통과하지 않은 상태에서 캐릭터 정보요청함
	if (!m_pClientManager->IsAccountPass(dwClient))
	{
		sc::writeLogInfo(std::string("MsgSndChaBasicInfo IsAccountPass false"));
		CloseClient(dwClient, net::KICKOUT_UNDEFINE);
		return;
	}

	int nUserNum = m_pClientManager->UserDbNum(dwClient);
    
    //! Cache 서버로 이동. 2011-07-19 jgkim
    NET_LOBBY_CHAR_INFO_AH MsgAH(
        nUserNum,
        nChaNum,
        dwClient,
        m_pClientManager->GetClientIP(dwClient).c_str(),
        m_pClientManager->GetClientPort(dwClient));

    SendCache(&MsgAH);
    /*
	m_pDbManager->AddGameJob(
        db::DbActionPtr(
            new db::CharacterBasicInfo(
                nUserNum,
                nChaNum, 
                dwClient, 
                m_pClientManager->GetClientIP(dwClient),
                m_pClientManager->GetClientPort(dwClient))));
    */
}


void CAgentServer::MsgSndChaBasicInfoEnd(DWORD dwClient)
{
	int nUserNum = m_pClientManager->UserDbNum(dwClient);

    NET_LOBBY_CHAR_INFO_AH_END MsgAH(
        nUserNum,
        dwClient,
        m_pClientManager->GetClientIP(dwClient).c_str(),
        m_pClientManager->GetClientPort(dwClient));

    SendCache(&MsgAH);
}

void CAgentServer::MsgSndChaBasicInfoBack(NET_MSG_GENERIC* pMsg)
{
    // NET_MSG_LOBBY_CHAR_INFO_HA
    // GLMSG::SNETLOBBY_CHARINFO* snci
    // NET_LOBBY_CHAR_INFO_HA* pPacket = (NET_LOBBY_CHAR_INFO_HA*) pMsg;
    NET_MSG_PACK* pPacket = (NET_MSG_PACK*) pMsg;
    NET_LOBBY_CHAR_INFO_HA MsgData;
    if (!msgpack::BufferToObject(pPacket->DataBuffer(), pPacket->DataSize(), MsgData))
    {
        sc::writeLogError("CAgentServer::MsgSndChaBasicInfoBack");
        return;
    }

    DWORD ClientSlot = MsgData.m_AgentSlot;
    if (!m_pClientManager->IsOnline(ClientSlot))
        return;

    if (MsgData.m_Port != m_pClientManager->GetClientPort(ClientSlot))
        return;

    SCHARINFO_LOBBY& CharData = MsgData.m_ChaData;

	EMCHARCLASS emCharClass = static_cast<EMCHARCLASS> (CharData.m_emClass);
	if (emCharClass == GLCC_EXTREME_M || emCharClass == GLCC_EXTREME_W)
		CharData.m_sExperience.lnMax = GLOGICEX::GLNEEDEXP2(CharData.m_wLevel);
	else
		CharData.m_sExperience.lnMax = GLOGICEX::GLNEEDEXP(CharData.m_wLevel);

    // 클럽정보 세팅    
    std::tr1::shared_ptr<GLClubAgent> spClub = m_pGLAgentServer->GetClub(CharData.m_ClubDbNum);
    if (spClub)
    {
        CharData.m_ClubRank = spClub->GetRank();
        CharData.SetClubName(spClub->Name());
    }

    GLMSG::NET_LOBBY_CHARINFO_AC DataAC(MsgData.m_ChaData);
    
    msgpack::sbuffer PackBuffer;
    msgpack::pack(PackBuffer, DataAC);

    NET_MSG_PACK MsgAC;
    if (MsgAC.SetData(NET_MSG_LOBBY_CHARINFO_AC, PackBuffer))
	    SendClient(ClientSlot, &MsgAC);
    else
        sc::writeLogError("CAgentServer::MsgSndChaBasicInfoBack");
}

void CAgentServer::MsgSndChaBasicInfoBackEnd(NET_MSG_GENERIC* pMsg)
{
	NET_LOBBY_CHAR_INFO_HA_END* pPacket = (NET_LOBBY_CHAR_INFO_HA_END*) pMsg;
	DWORD ClientSlot = pPacket->m_AgentSlot;

	GLMSG::NET_LOBBY_CHARINFO_AC_END MsgAC;
    SendClient(ClientSlot, &MsgAC);
}

void CAgentServer::MsgSndChaBasicInfo(DWORD dwClient, DWORD dwGaeaID, NET_MSG_GENERIC* nmg)
{
	if (nmg == NULL) return;

// 	DWORD dwSndBytes = 0;
// 	INT ChaDbNum = 0;
// 	INT nUsrNum = 0;
// 	INT nSvrGrp = 0;

	
    NET_MSG_PACK* pPacket = (NET_MSG_PACK*) nmg;
    NET_CHA_BA_INFO_CA_PACK RecvData;
    if (!msgpack::BufferToObject(pPacket->DataBuffer(), pPacket->DataSize(), RecvData))
        return;


    BOOST_FOREACH( const int& nVal, RecvData.vecChaDbNum )
        MsgSndChaBasicInfo(dwClient, nVal);

	// 캐릭터 정보 요청 전송이 완료되었음을 통지
	MsgSndChaBasicInfoEnd(dwClient);

	//ChaDbNum = pPacket->m_ChaDbNum;
	
}
