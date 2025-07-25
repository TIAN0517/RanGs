#include "pch.h"
#include <shlobj.h>

#include <boost/cast.hpp>

#include "../SigmaCore/String/StringFormat.h"
#include "../SigmaCore/String/StringUtils.h"
#include "../SigmaCore/Util/SystemInfo.h"
#include "../SigmaCore/Util/MinMessageBox.h"
#include "../SigmaCore/Loading/NSLoadingDirect.h"
#include "../SigmaCore/Log/LogMan.h"

#include "../EngineLib/Common/SUBPATH.h"
#include "../EngineLib/G-Logic/GLogic.h"
#include "../EngineLib/DxTools/DxClubMan.h"
#include "../EngineLib/DxTools/DxShadowMap.h"
#include "../EngineLib/DxTools/DxViewPort.h"
#include "../EngineLib/DxTools/COLLISION.h"
#include "../EngineLib/DxTools/DxSurfaceTex.h"
#include "../EngineLib/DxTools/RENDERPARAM.h"
#include "../EngineLib/DxTools/MaterialSkin/NSMaterialSkinManager.h"
#include "../EngineLib/DxSound/DxSoundLib.h"
#include "../EngineLib/DxEffect/Single/DxEffGroupPlayer.h"
#include "../EngineLib/DxEffect/Frame/DxEffectMan.h"
#include "../EngineLib/G-Logic/GLPeriod.h"
#include "../EngineLib/DxResponseMan.h"
#include "../EngineLib/GUInterface/UITextControl.h"
#include "../EngineLib/GUInterface/GameTextControl.h"
#include "../EngineLib/DxMeshs/DxSimpleMeshMan.h"
#include "../EngineLib/DxMeshs/SkinMesh/DxSkinMeshManager.h"
#include "../EngineLib/DxTools/Material/NSMaterialManager.h"
#include "../EngineLib/DxTools/Light/NSShadowLight.h"
#include "../EngineLib/DxTools/DxEnvironment.h"
#include "../EngineLib/DxTools/DxClubMan.h"
#include "../EngineLib/DxTools/DebugSet.h"
#include "../EngineLib/DxSound/DxSoundLib.h"
#include "../EngineLib/DxEffect/Single/DxEffGroupPlayer.h"
#include "../EngineLib/Common/profile.h"
#include "../EngineLib/DxMeshs/SkinMesh/NSSkinAniThread.h"
#include "../RanLogic/Msg/PKRankData.h"
#include "../RanLogic/GLogicDataMan.h"
#include "../RanLogic/Item/GLItemMan.h"
#include "../RanLogic/stl_Func.h"
#include "../RanLogic/Crow/GLCrowDataMan.h"
#include "../RanLogic/Quest/GLQuest.h"
#include "../RanLogic/Quest/GLQuestMan.h"
#include "../RanLogic/Activity/ActivityBase.h"
#include "../RanLogic/RANPARAM.h"
#include "../RanLogic/GLUseFeatures.h"
#include "../RanLogic/Club/ClubLogManClient.h"
#include "../RanLogic/Msg/FriendMsg.h"
#include "../RanLogic/Market/GLSearchDelay.h"
#include "../RanLogic/Msg/PostRnMsg.h"
#include "../RanLogic/Msg/GLContrlCountry.h"
#include "../RanLogic/Prison/LimitPrison.h"
#include "../RanLogic/Msg/ServerMsg.h"
//PK Notification - Arvin.BC
#include "../RanLogic/Msg/PKRankData.h"

#include "../RanLogic/Character/CharacterSlot/GLCharSlotMan.h"

//#include "../RanUI/InnerInterface.h"

#include "./Command/dxincommand.h"
#include "./LoadingThread/LoadingThread.h"
#include "./LoadingThread/LoadingDirectClient.h"
#include "./Char/CharNameDbNumCache.h"
#include "./Club/GLClubClient.h"
#include "./Club/ClubCountryClientMan.h"
#include "./Command/dxincommand.h"
#include "./Crow/GLCrowRenList.h"
#include "./Effect/GLFactEffect.h"
#include "./Friend/GLFriendClient.h"
#include "./MiniGame/GLMiniGameManager.h"
#include "./Party/GLPartyManagerClient.h"
#include "./Party/GLPartyTenderManagerClient.h"
#include "./Party/PartyCacheClient.h"
#include "./PointShop/PointShopClient.h"
#include "./Attendance/GLAttendanceClient.h"
#include "./Activity/GLActivityClient.h"
#include "./Post/GLPostClient.h"
#include "./SNS/GLSNSClient.h"
#include "./Stage/DxGlobalStage.h"
#include "./Script/ScriptManager.h"
#include "./CapturedTheField/GLCaptureTheFieldClient.h"
#include "./Tutorial/GLBaseTutorial.h"
#include "./Land/GLLandManClient.h"
#include "./Input/GLInputManager.h"
#include "./ReferChar/ReferChar.h"
#include "./WishList/WishList.h"
#include "./Notify/NotifyClientMan.h"
#include "./GLGaeaClient.h"
#include "./GroupChat/GroupChatClient.h"
#include "./PrivateMarketSearchBuy/PrivateMarketClient.h"
#include "./PrivateMarketSearchBuy/ConsignmentSaleClient.h"
#include "./Product/GLProductClient.h"
#include "./DamageRecord/DamageRecord.h"
#include "./PVE/GLPVEClient.h"
#include "./RnAttendance/RnAttendanceClient.h"
#include "./InstanceSystem/InstanceSystemClient.h"
#include "./Tournament/GLTournamentClient.h"
#include "./TexasHoldem/GLTexasHoldemClient.h"
#include "./FlyCameraControl/GLFlyCameraControl.h"
#include "./Post/PostRnClient.h"
#include "./PingManager/PingManagerClient.h"
#include "./ItemPreview/GLItemPreviewClient.h"
#include "./Event/EventClient.h"
#include "./Macro/MacroManager.h"
// Lotto System
#include "./LottoSystem/GLLottoSystemManClient.h"
#include "./CostumeStat/CotStatClient.h"
#include "./InstanceSystem/GLCTFlagEffectMan.h"
//#include "./RanMobile/GLRanMobileClient.h"

#include "../enginelib/DxMeshs/SkinMesh/DxSkinAnimation.h"
#include "RanGfxUI/InnerInterface.h"
// ----------------------------------------------------------------------------
#include "../SigmaCore/DebugInclude.h"
// ----------------------------------------------------------------------------

extern BOOL g_bWIREFRAME_VIEW;
// GLGaeaClient& GLGaeaClient::GetInstance()
// {
// 	static GLGaeaClient Instance;
// 	return Instance;
// }

GLGaeaClient::GLGaeaClient(
    DxGlobalStage* pGlobalStage,
    EngineDeviceMan* pEngineDevice,
    EMSERVICE_PROVIDER ServiceProvider,
    SUBPATH* pPath,
    bool bToolMode) 
	: m_pEngineDevice(pEngineDevice)
    , m_ServiceProvider(ServiceProvider)
    , m_pGlobalStage(pGlobalStage)
    , m_pPath(pPath)
    , m_hWnd(NULL)
	, m_nServerGroup(0)
	, m_nChannel(0)
	, m_fAge(0.0f)
	, m_pd3dDevice(NULL)
	, m_pLandMClient(NULL)
	, m_vCharPos(0,0,0)
	, m_bPKServer(false)
	, m_bSCHOOL_FREEPK(false)
	, m_bBRIGHTEVENT(false)
	, m_bCLUBBATTLE(false)
	, m_bClubDeathMatch(false)
	, m_fClubDeathMatchTimer(0.0f)
	, m_dwRTTAgent(0)
	, m_dwRTTField(0)
    , m_pFriendClient(NULL)
	, m_pChatGroup(NULL)
	, m_pPrivateMarketSearchBuyData(NULL)
    , m_pPostClient(NULL)
    , m_pSNSClient(NULL)
    , m_pTradeClient(NULL)
    , m_pBaseTutorial(NULL)
    , m_pAttendance(NULL)
	, m_pActivity(NULL)
    , m_pCaptureTheField(NULL)	
    , m_nServerTimeBias(0)
    , m_ServerGroup(-1)
    , m_UserCountry(wb::UNKNOWN_COUNTRY)
	, m_Use2ndPass(0)
    , m_MaxClientNetworkProtocolNum(NET_MSG_GCTRL_MAX)
    , m_MaxServerNetworkProtocolNum(0)
	, m_fElapsedTime(0.f)
	, m_pRnAttendanceClient(NULL)
	, m_pLottoSystemMan(NULL)
	, m_pCountryMan(NULL)
	, m_pVCountryMan(NULL)
	, m_pItemPreviewClient(NULL)
	, m_bUseCustomChaseCamera(false)
    , m_fClubNameTimer(0)
	, m_pCostumeClient(NULL)
	, m_pLodingDirect(NULL)
	, m_dwVehicleRidingRequestTarget(-1)
	, m_vMacroTracePos(0.0f, 0.0f, 0.0f)
{
	memset(m_szAppPath, 0, sizeof(char) * (MAX_PATH));
	memset(m_szShopInfoFile, 0, sizeof(char) * (MAX_PATH));
	
    m_vecDetectID.reserve(200);

    m_pMapList         = new GLMapListClient;
    m_pPet             = new GLPetClient(this);
    m_pFriendClient    = new GLFriendClient(this);	
	m_pMiniGameManager = new MiniGame::GLManagerClient(this);
	m_pEventManager = new GLEvent::ManagerClient(this);
	m_pPartyManagerClient     = new GLPartyManagerClient(this);
	m_pPartyTenderManagerClient = new GLPartyTenderManagerClient(this);
	m_pChatGroup	   = new GLGroupChat(NULL, this);
	m_pPrivateMarketSearchBuyData = new PrivateMarketClient(this);
    m_pConsignmentSale = new ConsignmentSaleClient( this );

    m_pPostClient      = new GLPostClient(this);
    m_pSNSClient       = new GLSNSClient(this);
    m_pTradeClient     = new GLTradeClient(this);
    m_pBaseTutorial    = new GLBaseTutorial(this);
    m_pMyClub          = new GLClubClient;
    m_pCharacter       = new GLCharacter(this, m_pMyClub);
    m_pPointShop       = new pointshop::PointShopClient(this);
    m_pAttendance      = new attendance::GLAttendanceClient(this);
	m_pActivity		   = new GLActivityClient(this);
    m_pCaptureTheField = new PVP::GLCaptureTheFieldClient(this);
	m_pReferChar       = new ReferChar::CClient(this);
	m_pWishList        = new WishList::CClient(this);
	//m_pReferChar       = new ReferChar::CClient(this, m_pInterface);
    m_cServerTime      = CTime::GetCurrentTime();
    m_pGameCommand     = new GameCommand(this);
    m_pClientScript    = new ClientScriptManager(this);
    m_pClubCountryMan  = new wb::ClubCountryClientMan;
    m_pClubLog         = new club::LogManClient;
    m_pNotify          = new NotifyClientMan(this);
    m_pChaDbNumNameCache = new CharNameDbNumCache(this);
    m_pPartyCache      = new PartyClientCache;
	m_pDamageRecord = new DamageRecord::Manager;
	m_pPVEClient	   = new GLPVEClient(this);
	m_pRnAttendanceClient = new RnAttendanceClient(this);
    m_pInstanceManager = new InstanceSystem::ManagerClient(this);
	m_pPingManager = new Ping::ManagerClient(this);
	m_pProductClient   = new GLProductClient( this );//hsshin
	m_pPrivateMarketSearchDelay = new private_market::GLSearchDelay();
	m_pPrivateMarketLogDelay = new private_market::GLSearchDelay();
	m_pPrivateMarketPurchseQueue = new private_market::GLPrivateMarketPurchseQueue();
	m_pTexasHoldemMgr = new TexasHoldem::GLTexasHoldemClient( this );
	m_pItemPreviewClient = new ItemPreview::GLItemPreviewClient( this );
	m_pCTFlagEffect = new GLCTFlagEffectMan( this );
	m_pInterface  = new CInnerInterface ( this, m_pEngineDevice, false );

	// Lotto System;
	m_pLottoSystemMan = LottoSystem::GLLottoSystemManClient::Instance();
	if ( m_pLottoSystemMan )
		m_pLottoSystemMan->SetGaeaClient( this );

	// Country System;
	m_pCountryMan = Country::GLCountryManClient::Instance();
	m_pVCountryMan = Country::GLVictoriousCountryManClient::Instance();
	if ( m_pVCountryMan )
		m_pVCountryMan->SetClient( this );

	GLFlyCameraControl::GetInstance().InitFlyCameraControl(this);
	GLMacroManager::GetInstance().SetGaeaClient(this);
	m_pTournament	=new MatchingSystem::GLTouranmentClient(this);

	// PostRenew
	m_spPostRnClient = PostRnClientPtr( new PostRnClient(this) );

	m_pCostumeClient = new CCostumeStatClient(this);

	m_pLodingDirect = new LoadingDirectClient(FALSE);

    initPacketFunc();
}

GLGaeaClient::~GLGaeaClient()
{
	SAFE_DELETE(m_pPVEClient);
	SAFE_DELETE(m_pDamageRecord);
    SAFE_DELETE(m_pPartyCache);
    SAFE_DELETE(m_pChaDbNumNameCache);
    SAFE_DELETE(m_pNotify);
    SAFE_DELETE(m_pClientScript);
    SAFE_DELETE(m_pWishList);
    SAFE_DELETE(m_pClubLog);
    SAFE_DELETE(m_pReferChar);
    SAFE_DELETE(m_pClubCountryMan);
    SAFE_DELETE(m_pGameCommand);
    SAFE_DELETE(m_pCaptureTheField);
    SAFE_DELETE(m_pAttendance);
	SAFE_DELETE(m_pActivity);
    SAFE_DELETE(m_pPointShop);
    SAFE_DELETE(m_pCharacter);
    SAFE_DELETE(m_pMyClub);
    SAFE_DELETE(m_pBaseTutorial);
    SAFE_DELETE(m_pTradeClient);
    SAFE_DELETE(m_pPostClient);
    SAFE_DELETE(m_pSNSClient);
	SAFE_DELETE(m_pMiniGameManager);
	SAFE_DELETE(m_pEventManager);
    SAFE_DELETE(m_pPartyManagerClient);
	SAFE_DELETE(m_pPartyTenderManagerClient);
	SAFE_DELETE(m_pChatGroup);
	SAFE_DELETE(m_pPrivateMarketSearchBuyData);
	SAFE_DELETE(m_pConsignmentSale);
    SAFE_DELETE(m_pFriendClient);    
    SAFE_DELETE(m_pPet);    
	SAFE_DELETE(m_pProductClient);//hsshin
	SAFE_DELETE(m_pLandMClient);
    SAFE_DELETE(m_pMapList);
	SAFE_DELETE(m_pPrivateMarketSearchDelay);
	SAFE_DELETE(m_pPrivateMarketLogDelay);
	SAFE_DELETE(m_pPrivateMarketPurchseQueue);
	SAFE_DELETE(m_pRnAttendanceClient);
    SAFE_DELETE(m_pInstanceManager);
	SAFE_DELETE(m_pPingManager);
	SAFE_DELETE(m_pTexasHoldemMgr);
	SAFE_DELETE(m_pItemPreviewClient);
	SAFE_DELETE( m_pCTFlagEffect );
	SAFE_DELETE( m_pInterface );

	// Lotto System;
	if( m_pLottoSystemMan )
		m_pLottoSystemMan->Destroy();
	m_pLottoSystemMan = NULL;

	SAFE_DELETE( m_pTournament );
	SAFE_DELETE(m_pCostumeClient);

	SAFE_DELETE(m_pLodingDirect);

	ActivityBaseData::ReleaseInstance();
}

HRESULT GLGaeaClient::OneTimeSceneInit(const char* szAppPath, HWND hWnd)
{
	m_hWnd = hWnd;
	StringCchCopy(m_szAppPath, MAX_PATH, szAppPath);

    if (m_pPVEClient)
    {
        m_pPVEClient->Init();
    }

	if(m_pTournament)
	{
		m_pTournament->Init();
	}

	if (m_spPostRnClient)
		m_spPostRnClient->Reset();

	if ( m_pPrivateMarketSearchBuyData )
		m_pPrivateMarketSearchBuyData->LoadSearchFileFromITEM();

	GLMacroManager::GetInstance().OneTimeSceneInit();

	if ( m_pInterface ) m_pInterface->UiOneTimeSceneInit();

	return S_OK;
}

DxLandMan* GLGaeaClient::getactivemap()
{
    GLLandManClient* pMapClient = GetActiveMap();
    if (pMapClient)
        return pMapClient->GetLandMan();
    else
        return NULL;
}

bool GLGaeaClient::CanRecoveryExp()
{
    return GLUseFeatures::GetInstance().IsUsingRebirthRecoveryExp();
}

bool GLGaeaClient::CanRecoveryExpNpc()
{
    return GLUseFeatures::GetInstance().IsUsingRebirthRecoveryExpNpc();
}

HRESULT	GLGaeaClient::CreatePET (NET_MSG_GENERIC* nmg)
{
	GLMSG::SNETPET_REQ_USEPETCARD_FB* pNetMsg = ( GLMSG::SNETPET_REQ_USEPETCARD_FB* ) nmg;

	GLPET sPet;
	
	sPet.m_emTYPE			= pNetMsg->m_emTYPE;
	sPet.m_dwGUID			= pNetMsg->m_dwGUID;
	sPet.m_sPetID			= pNetMsg->m_sPetID;
	sPet.m_dwOwner			= pNetMsg->m_dwOwner;
	sPet.m_wStyle			= pNetMsg->m_wStyle;
	sPet.m_wColor			= pNetMsg->m_wColor;
	sPet.m_fWalkSpeed		= pNetMsg->m_fWalkSpeed;
	sPet.m_fRunSpeed		= pNetMsg->m_fRunSpeed;
	sPet.m_nFull			= pNetMsg->m_nFull;
	sPet.m_sMapID			= pNetMsg->m_sMapID;
	sPet.m_dwCellID			= pNetMsg->m_dwCellID;

	for ( WORD i=0; i<MAX_PETSKILLSLOT; ++i )
	{
		sPet.m_sActiveSkillID[i] = pNetMsg->m_sActiveSkillID[i];
	}

	sPet.m_wMaxSkillSlot = pNetMsg->m_wMaxSkillSlot;
	
	std::string strPetName = pNetMsg->m_szName;
	//if ( this->GetServiceProvider() == SP_TAIWAN )
	//	strPetName = sc::string::utf8_to_ansi( strPetName );

	sPet.SetName(strPetName.c_str());
	sPet.SetDbNum(pNetMsg->m_dwPetID);

	for ( WORD i = 0 ; i < pNetMsg->m_wSkillNum; ++i )
	{
		sPet.m_ExpSkills.insert ( std::make_pair ( pNetMsg->m_Skills[i].sNativeID.dwID, 
			pNetMsg->m_Skills[i] ) );
	}

	for ( WORD i = 0; i < ACCETYPESIZE; ++i )
	{
		sPet.m_PutOnItems[i] = pNetMsg->m_PutOnItems[i];
	}

	// 주인 옆에 PET 생성
	HRESULT hr = m_pPet->Create ( &sPet, pNetMsg->m_vPos, pNetMsg->m_vDir, m_pLandMClient->GetNaviMesh(), m_pd3dDevice );

	// 실패처리 ( 서버에 생성된 팻을 삭제하도록 )
	if ( FAILED(hr) )
	{
		SINVENITEM* pInvenItem = m_pCharacter->m_cInventory.FindItemByGenNumber ( 
			m_pCharacter->m_llPetCardGenNum );
		if ( pInvenItem )
		{
			GLMSG::SNETPET_REQ_UNUSEPETCARD NetMsg( sPet.m_dwGUID, sPet.DbNum() );
			NETSENDTOFIELD(&NetMsg);
		}
		// 실패시 GenNum 처리
		m_pCharacter->m_llPetCardGenNum.Reset();

		return FALSE;
	}

	// 생성 효과
	D3DXMATRIX matEffect;
	D3DXVECTOR3 vPos = m_pPet->GetPosition ();
	D3DXMatrixTranslation ( &matEffect, vPos.x, vPos.y, vPos.z );

	std::string strGEN_EFFECT = GLCONST_CHAR::strPET_GEN_EFFECT.c_str();
	STARGETID sTargetID(CROW_PET,m_pPet->m_dwGUID,vPos);
	DxEffGroupPlayer::GetInstance().NewEffGroup
	(
		strGEN_EFFECT.c_str(),
		matEffect,
		&sTargetID,
		TRUE, 
		FALSE
	);

	return S_OK;
}


GLLandManClient* GLGaeaClient::CreateLandMClient ( const MapID& mapID, D3DXVECTOR3 &vBasicPos )
{
	GASSERT(m_pd3dDevice&&"m_pd3dDevice 가 초기화 되지 않았습니다.");
	
	SMAPNODE* pMapNode = FindMapNode(mapID.getBaseMapID());
	if (pMapNode)
	{
        GLLandManClient* pLandMClient = new GLLandManClient(this);
		pLandMClient->Create(pMapNode->m_LevelFile.c_str(), m_pd3dDevice, vBasicPos, mapID, pMapNode->IsPeaceZone());
        return pLandMClient;
	}
    else
    {
        std::string ErrorMsg(
            sc::string::format(
                "Not exist map id (%1%/%2%). Check maplist file.",
                mapID.getBaseMapID().wMainID,
                mapID.getBaseMapID().wSubID));        
        sc::ErrorMessageBox(ErrorMsg, false);
        return NULL;
    }
}

void GLGaeaClient::ShowLogFactionInfo ()
{
	Faction::ManagerClient* pFactionMan = GetFactionClient();
	if ( NULL == pFactionMan )
		return;

	pFactionMan->SetShowFactionInfo();
}

void GLGaeaClient::JoinWorldBattleServer ()
{
	if ( GLUseFeatures::GetInstance().IsUsingJoinWorldBattle() &&
		false == GLUseFeatures::GetInstance().IsUsingWorldBattle() )
	{
		GLMSG::SNETPC_TRANS_TO_WORLDBATTLE NetMsg;

		//NETSENDTOFIELD( &NetMsg );
		NETSENDTOAGENT( &NetMsg );
	}
}

void GLGaeaClient::FirstJoin()
{
    SendPointShopUpdateDate();
    SendProtocolInfo();
    m_pClubLog->Reset();
    ReqFriendGroup();
    ReqFriendList();
    ReqNotifyCA();
    m_pChaDbNumNameCache->Clear();
    //ReqClubLog(club::LOG_ALL, 0, 100);
    if (m_pCharacter)
        m_pCharacter->ReqItemBankInfo(true);
    m_pPartyCache->Reset();
	m_pPartyManagerClient->Reset();
	m_pMiniGameManager->Reset();
	m_pEventManager->reset();
	//m_pRnAttendanceClient->GetAttendanceCompleted();

	m_pProductClient->ReqProductRecipeList();
	m_pProductClient->ReqProductTypeList();
	m_pProductClient->ReqProductExpList();
}

void GLGaeaClient::ReqFriendGroup()
{
    GLMSG::NET_FRIEND_GROUP_REQ_CA MsgCA;
    NETSENDTOAGENT(&MsgCA);
}

void GLGaeaClient::ReqFriendList()
{
    // 서버에 친구 리스트 전송 요청.
    m_pFriendClient->ReqFriendList();
}

void GLGaeaClient::SendProtocolInfo()
{
    NET_PROTOCOL_INFO_CA MsgCA(m_MaxClientNetworkProtocolNum);
    NETSENDTOAGENT(&MsgCA);
}

void GLGaeaClient::MsgProtocolInfo(NET_MSG_GENERIC* pMsg)
{
    NET_PROTOCOL_INFO_AC* pPacket = (NET_PROTOCOL_INFO_AC*) pMsg;
    m_MaxServerNetworkProtocolNum = pPacket->m_MaxServerProtocolNum;
    
#ifndef NDEBUG // _RELEASED // _DEBUG
    if (m_MaxClientNetworkProtocolNum != m_MaxServerNetworkProtocolNum)
    {
        std::string Msg(
            sc::string::format(
                "Protocol mismatch. client %1% server %2%",
                m_MaxClientNetworkProtocolNum,
                m_MaxServerNetworkProtocolNum));
        AfxMessageBox(Msg.c_str(), MB_OK | MB_ICONEXCLAMATION);            
    }
#endif
}

void GLGaeaClient::PrintProtocolInfo()
{
    std::string Msg(
        sc::string::format(
            "Protocol Client %1% Server %2%",
            m_MaxClientNetworkProtocolNum,
            m_MaxServerNetworkProtocolNum));
    PrintConsoleText(Msg);
}

void GLGaeaClient::NotifyReferCharPublic()
{
	if( GLUseFeatures::GetInstance().IsUsingReferChar() )
	{
		if( m_pReferChar )
			m_pReferChar->NotifyPublic( RANPARAM::bCharInfoPrivate ? false : true );
	}
}

void GLGaeaClient::CloseConnect()
{
	m_pCharacter->OffLine();
	m_pGlobalStage->GetNetClient()->CloseConnect();
}

// HRESULT GLGaeaClient::SetActiveMap ( char* szLandFileName, D3DXVECTOR3 &vBasicPos )
// {
//     ScriptStop();
// 	// 새로운 맵을 로드함
// 	GLLandManClient* pLandMClient = CreateLandMClient ( szLandFileName, vBasicPos );
// 	if (!pLandMClient)
//         return E_FAIL;
// 
//     // 이전 맵을 클리어
//     bool bFirstLoad = false;
//     if (m_pLandMClient)
//         delete m_pLandMClient;
//     else
//         bFirstLoad = true;
// 
// 	// 로드한 맵을 활성화
// 	m_pLandMClient = pLandMClient;
// 	m_pLandMClient->ActiveMap();
// 
//     if (bFirstLoad)
//         FirstJoin();
// 
// 	NotifyReferCharPublic();
// 
//     ScriptResume();
// 	return S_OK;
// }

void GLGaeaClient::ChangeCountry ( Country::SCOUNTRY_INFO sCountryInfo )
{
	GLMSG::NET_COUNTRY_CHANGE_CA sMsg( sCountryInfo );
	NETSENDTOAGENT( &sMsg );
}

HRESULT GLGaeaClient::SetActiveMap ( const MapID& mapID, D3DXVECTOR3 &vBasicPos )
{
    ScriptStop();
	// 새로운 맵을 로드함
	GLLandManClient* pLandMClient = CreateLandMClient ( mapID, vBasicPos );
	if (!pLandMClient)
    {
        return E_FAIL;
    }

    // 이전 맵을 클리어
    bool bFirstLoad = false;
	if (m_pLandMClient)
        delete m_pLandMClient;
    else
        bFirstLoad = true;

	// 로드한 맵을 활성화
	m_pLandMClient = pLandMClient;
	m_pLandMClient->ActiveMap();

    if (bFirstLoad)
        FirstJoin();

	NotifyReferCharPublic();

    ScriptResume();

	if ( GLUseFeatures::GetInstance().IsUsingTutorialMode() && GetCharacter()->m_bTutorial && !GetTutorial()->IsTutorial() && GetActiveMap()->GetBaseMapID() == SNATIVEID(254,0) )
	{
		GetTutorial()->InitData();
		GetTutorial()->StartTutorialInClient( GetCharacter() );
	}

	return S_OK;
}

GLLandManClient* GLGaeaClient::GetActiveMap()
{
	return m_pLandMClient;
}

GLLandManClient* GLGaeaClient::GetLandManClient()
{
    return GetActiveMap();
}

const MapID GLGaeaClient::GetActiveMapID(void) const
{
	if (!m_pLandMClient)
        return MapID();
    else	
	    return m_pLandMClient->GetMapID();
}

HRESULT GLGaeaClient::MoveActiveMap ( const MapID& mapID, D3DXVECTOR3 &vPos )
{
	m_pEngineDevice->DoInterimClean_Prev();

    ScriptStop();

	// 현재 맵이 CTF 맵일 경우 Ui를 끈다;
	if ( IsCTFMap() )
	{
		//GLWidgetScript::GetInstance().LuaCallFunc< void >(NSWIDGET_SCRIPT::g_strFunc_CTFDisplayInfo_Close);

		//!! SF_TODO
		// TODO : #RM 695
		CInnerInterface* pInter = dynamic_cast<CInnerInterface*>(GetInterface());
		
		if( pInter )
			pInter->ctf_display_hide();
	}

	//GfxInterfaceBase::GetInstance()->ClearDisplay();
	m_pInterface->ClearNameList();
	m_pInterface->CloseAllWindow();

	// MEMO : 맵이동시 열린 윈도우를 닫기 위해서 추가했다. 엄한 창이 닫힐수도 있으니 지켜봐야겠다.
	// 아래 함수가 정확한 의미로 쓰이는지 모르겠다.
	//GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_EnterGameStage );

	// 맵을 이동하는 것이니 맵 참조값을 줄이는 방향으로 간다.
	m_pPet->LandShredPtrRefSub();
	m_pCharacter->LandShredPtrRefSub();

	// [shhan][2014.12.1] DoInterimClean_Prev 후에 m_pLandMClient Delete 를 해야 한다.
	//						그렇지 않으면 맵 이동시 Crash 가 발생한다.
	//						DxLandMan 의 안에서 Collsion 작업 중 DxLandMan::CleanUp 이 호출되어 버려서 난리가 난다.
	//
	//	Note : 이전 맵을 클리어.
	//
	SAFE_DELETE(m_pLandMClient);

	// [shhan][2014.12.1] Skin 을 지워줘야 한다.
	//						지우지 않으면 DxCharPart::m_rSkinPiece 이 값을 읽을 때 변환된 값이라서 Crash 가 생긴다.
	m_pPet->DeleteSkin();

	// [shhan][2014.12.10] Texture Quality 가 변경이 되지 않았으면 내 형상에 있는 Texture 정보를 지울 필요가 없게 된다.
	//						지우면 맵이동시 로딩 Lag 만 생기기 때문에 변경될 때만 삭제 하는 방향으로 한다.
	//						DoInterimClean 안의 TextureManager::GetInstance().DeleteCacheDataAll 서 사용안하는 것 삭제한다.
	if ( RENDERPARAM::g_bChangeTexQuality )
	{
		m_pCharacter->DeleteSkin();
	}

	// 맵을 이동하는 것이니 맵 참조값을 줄이는 방향으로 간다.
	m_pPet->LandShredPtrRefSub();
	m_pCharacter->LandShredPtrRefSub();

	// [shhan][2014.12.1] DoInterimClean_Prev 후에 m_pLandMClient Delete 를 해야 한다.
	//						그렇지 않으면 맵 이동시 Crash 가 발생한다.
	//						DxLandMan 의 안에서 Collsion 작업 중 DxLandMan::CleanUp 이 호출되어 버려서 난리가 난다.
	//
	//	Note : 이전 맵을 클리어.
	//
	SAFE_DELETE(m_pLandMClient);

	// [shhan][2014.12.1] Skin 을 지워줘야 한다.
	//						지우지 않으면 DxCharPart::m_rSkinPiece 이 값을 읽을 때 변환된 값이라서 Crash 가 생긴다.
	m_pPet->DeleteSkin();

	// [shhan][2014.12.10] Texture Quality 가 변경이 되지 않았으면 내 형상에 있는 Texture 정보를 지울 필요가 없게 된다.
	//						지우면 맵이동시 로딩 Lag 만 생기기 때문에 변경될 때만 삭제 하는 방향으로 한다.
	//						DoInterimClean 안의 TextureManager::GetInstance().DeleteCacheDataAll 서 사용안하는 것 삭제한다.
	if ( RENDERPARAM::g_bChangeTexQuality )
	{
		m_pCharacter->DeleteSkin();
	}

	//	Note : 장치 데이터를 리셋.
	//
	m_pEngineDevice->DoInterimClean ( m_pd3dDevice );

	//	Note : 케릭터 기본 형상을 다시 부른다.
	for ( int i=0; i<GLCI_NUM_ACTOR; ++i )
		DxSkinCharDataContainer::GetInstance().LoadData( GLCONST_CHAR::szCharSkin[i], m_pd3dDevice );

	//	Note : 새로운 맵을 로드함.
	//
	GLLandManClient* pLandMClient = CreateLandMClient ( mapID, vPos );
	if (!pLandMClient)
        return E_FAIL;

	//	Note : 로드한 맵을 활성화.
	//
	m_pLandMClient = pLandMClient;
	m_pLandMClient->ActiveMap ();

	//	Note : 자기 케릭의 스킨을 다시 읽어 온다.
	//
	m_pCharacter->SkinLoad ( m_pd3dDevice, m_pLandMClient->GetLandMan() );

	CString strText = GetMapName ( m_pLandMClient->GetBaseMapID() );

	// 여기서 클럽 정보는 의미가없다;
	/*const GLMapAxisInfo& sMapAxisInfo = m_pLandMClient->GetMapAxisInfo();
	GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_Minimap_MoveActiveMap,
		"-s, -s, -b, -dw, -dw, -s",
		strText.GetBuffer(),
		sMapAxisInfo.GetMinMapTex(),
		pLandMClient->IsUIMapSelect() == false,
		GetCharacter()->m_dwServerID,
		pLandMClient->m_dwGuidClub,
		pLandMClient->m_szGuidClubName );*/


	/*SMAPNODE *pMapNode = FindMapNode ( m_pLandMClient->GetBaseMapID() );
	if( pMapNode )
	{
		GLLevelFileClient cLevelFile(this);
		if( cLevelFile.LoadFile ( pMapNode->m_LevelFile.c_str(), NULL ) )
		{	
			GetInterface()->SetMiniMapInfo( cLevelFile, strText );
		}
	}*/

	// 개인상점 닫기
	m_pCharacter->DoPrivateMarketClose();

	if ( IsCTFMap() )
	{
		PVP::GLCaptureTheFieldClient::CTFGLOBALSTATE& sCTFState = m_pCaptureTheField->GetGlobalState( PVP::ENUM_CTF_TYPE_POWERSTATION );
		int nAuthenticactorSize;
		GASSERT( (nAuthenticactorSize=sCTFState.vecAUTHENTICATOR.size()) <= 3 );

		/*GLWidgetScript::GetInstance().LuaCallFunc< void >(
			NSWIDGET_SCRIPT::g_strFunc_CTFDisplayInfo_Open );

		GLWidgetScript::GetInstance().LuaCallFunc< void >(
			NSWIDGET_SCRIPT::g_strFunc_CTFieldDI_SetState,
			"-n", sCTFState.emSTATE );*/
	}

	// 미니맵 마우스 이동 목표지점 해제
//	GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_LargeMap_HideMoveIcon );
    
   // NotifyReferCharPublic();

    ScriptResume();

	//GLRanMobileClient::GetInstance().ReqInfo();

	if ( GLUseFeatures::GetInstance().IsUsingTutorialMode() && GetCharacter()->m_bTutorial && !GetTutorial()->IsTutorial() && GetActiveMap()->GetBaseMapID() == SNATIVEID(254,0) )
	{
		GetTutorial()->InitData();
		GetTutorial()->StartTutorialInClient( GetCharacter() );
	}

	return S_OK;
}

HRESULT GLGaeaClient::CreateInstantMap(const MapID& mapID, D3DXVECTOR3 &vBasicPos )
{
	m_pEngineDevice->DoInterimClean_Prev();

	//GfxInterfaceBase::GetInstance()->ClearDisplay();
	m_pInterface->ClearNameList();
	m_pInterface->CloseAllWindow();

	// MEMO : 맵이동시 열린 윈도우를 닫기 위해서 추가했다. 엄한 창이 닫힐수도 있으니 지켜봐야겠다.
	// 아래 함수가 정확한 의미로 쓰이는지 모르겠다.
	//GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_EnterGameStage );

	// 맵을 이동하는 것이니 맵 참조값을 줄이는 방향으로 간다.
	m_pPet->LandShredPtrRefSub();
	m_pCharacter->LandShredPtrRefSub();

	// [shhan][2014.12.1] DoInterimClean_Prev 후에 m_pLandMClient Delete 를 해야 한다.
	//						그렇지 않으면 맵 이동시 Crash 가 발생한다.
	//						DxLandMan 의 안에서 Collsion 작업 중 DxLandMan::CleanUp 이 호출되어 버려서 난리가 난다.
	//
	//	Note : 이전 맵을 클리어.
	//
	SAFE_DELETE(m_pLandMClient);

	// [shhan][2014.12.1] Skin 을 지워줘야 한다.
	//						지우지 않으면 DxCharPart::m_rSkinPiece 이 값을 읽을 때 변환된 값이라서 Crash 가 생긴다.
	m_pPet->DeleteSkin();

	// [shhan][2014.12.10] Texture Quality 가 변경이 되지 않았으면 내 형상에 있는 Texture 정보를 지울 필요가 없게 된다.
	//						지우면 맵이동시 로딩 Lag 만 생기기 때문에 변경될 때만 삭제 하는 방향으로 한다.
	//						DoInterimClean 안의 TextureManager::GetInstance().DeleteCacheDataAll 서 사용안하는 것 삭제한다.
	if ( RENDERPARAM::g_bChangeTexQuality )
	{
		m_pCharacter->DeleteSkin();
	}

	// 맵을 이동하는 것이니 맵 참조값을 줄이는 방향으로 간다.
	m_pPet->LandShredPtrRefSub();
	m_pCharacter->LandShredPtrRefSub();

	// [shhan][2014.12.1] DoInterimClean_Prev 후에 m_pLandMClient Delete 를 해야 한다.
	//						그렇지 않으면 맵 이동시 Crash 가 발생한다.
	//						DxLandMan 의 안에서 Collsion 작업 중 DxLandMan::CleanUp 이 호출되어 버려서 난리가 난다.
	//
	//	Note : 이전 맵을 클리어.
	//
	SAFE_DELETE(m_pLandMClient);

	// [shhan][2014.12.1] Skin 을 지워줘야 한다.
	//						지우지 않으면 DxCharPart::m_rSkinPiece 이 값을 읽을 때 변환된 값이라서 Crash 가 생긴다.
	m_pPet->DeleteSkin();

	// [shhan][2014.12.10] Texture Quality 가 변경이 되지 않았으면 내 형상에 있는 Texture 정보를 지울 필요가 없게 된다.
	//						지우면 맵이동시 로딩 Lag 만 생기기 때문에 변경될 때만 삭제 하는 방향으로 한다.
	//						DoInterimClean 안의 TextureManager::GetInstance().DeleteCacheDataAll 서 사용안하는 것 삭제한다.
	if ( RENDERPARAM::g_bChangeTexQuality )
	{
		m_pCharacter->DeleteSkin();
	}
	//	Note : 장치 데이터를 리셋.
	//
	m_pEngineDevice->DoInterimClean ( m_pd3dDevice );

	//	Note : 케릭터 기본 형상을 다시 부른다.
	for ( int i=0; i<GLCI_NUM_ACTOR; ++i )
		DxSkinCharDataContainer::GetInstance().LoadData( GLCONST_CHAR::szCharSkin[i], m_pd3dDevice );

	//	Note : 새로운 맵을 로드함.
	//
	GLLandManClient* pLandMClient(CreateLandMClient(mapID, vBasicPos ));
	if ( !pLandMClient )
        return E_FAIL;

	//	Note : 로드한 맵을 활성화.
	//
	m_pLandMClient = pLandMClient;
	m_pLandMClient->ActiveMap ();
	m_pLandMClient->SetUseInstantMap(GLLevelFile::EMINSTANCE_OLD_INSTANCE);

	//	Note : 자기 케릭의 스킨을 다시 읽어 온다.
	//
	m_pCharacter->SkinLoad ( m_pd3dDevice, m_pLandMClient->GetLandMan() );

	CString strText = GetMapName ( m_pLandMClient->GetBaseMapID() );

	/*const GLMapAxisInfo& sMapAxisInfo = m_pLandMClient->GetMapAxisInfo();
	GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_Minimap_MoveActiveMap,
		"-s, -s, -b, -dw, -dw, -s",
		strText.GetBuffer(),
		sMapAxisInfo.GetMinMapTex(),
		pLandMClient->IsUIMapSelect() == false,
		GetCharacter()->m_dwServerID,
		pLandMClient->m_dwGuidClub,
		pLandMClient->m_szGuidClubName );*/


	/*SMAPNODE *pMapNode = FindMapNode ( m_pLandMClient->GetBaseMapID() );
	if( pMapNode )
	{
		GLLevelFileClient cLevelFile(this);
		BOOL bOk = cLevelFile.LoadFile( pMapNode->m_LevelFile.c_str(), NULL );
		if( bOk )
		{	
			GetInterface()->SetMiniMapInfo( cLevelFile, strText );
		}
	}*/

	// 미니맵 마우스 이동 목표지점 해제
//	GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_LargeMap_HideMoveIcon );

	NotifyReferCharPublic();

//	GLRanMobileClient::GetInstance().ReqInfo();

	if ( GLUseFeatures::GetInstance().IsUsingTutorialMode() && GetCharacter()->m_bTutorial && !GetTutorial()->IsTutorial() && GetActiveMap()->GetBaseMapID() == SNATIVEID(254,0) )
	{
		GetTutorial()->InitData();
		GetTutorial()->StartTutorialInClient( GetCharacter() );
	}

	return S_OK;
}

void GLGaeaClient::MsgMoveActiveMap ( NET_MSG_GENERIC *nmg )
{
	if ( m_pPet->IsVALID() )
	{
		m_pPet->Delete();
		m_pCharacter->m_bIsPetActive = TRUE;
	}
	
	HRESULT hr=S_OK;

	GLMSG::SNETREQ_GATEOUT_FB *pNetMsg = (GLMSG::SNETREQ_GATEOUT_FB *) nmg;

	m_pCharacter->m_mapMarketClick.clear();

	m_pCharacter->ReSetActState(EM_REQ_GATEOUT);

	if ( pNetMsg->emFB!=EMCHAR_GATEOUT_OK )
	{
		CDebugSet::ToListView ( "[MSG] SNETREQ_GATEOUT_FB FAIL : %d", pNetMsg->emFB );
		
        CString strText;
		switch(pNetMsg->emFB)
		{
		case EMCHAR_GATEOUT_FAIL:			
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_GATEOUT_FB_FAIL"), pNetMsg->emFB));
			break;
		case EMCHAR_GATEOUT_MAPID:
		case EMCHAR_GATEOUT_GATEID:
		case EMCHAR_GATEOUT_TARMAPID:
		case EMCHAR_GATEOUT_TARGATEID:			
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_GATEOUT_FB_DATA_ERR"), pNetMsg->emFB));
			break;
		case EMCHAR_GATEOUT_FIELDSVR:			
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_GATEOUT_FB_SVR_ERR"), pNetMsg->emFB));
			break;
		case EMCHAR_GATEOUT_HOLD:
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                ID2GAMEINTEXT("EMCHAR_GATEOUT_HOLD"));
			break;
		case EMCHAR_GATEOUT_CONDITION:			
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("EMCHAR_GATEOUT_CONDITION"), pNetMsg->emFB));
			break;
		case EMCHAR_GATEOUT_CLUBBATTLE:			
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("EMCHAR_GATEOUT_CLUBBATTLE"), GLCONST_CHAR::dwCLUB_BATTLE_GUID_TIME));
			break;
		case EMCHAR_GATEOUT_CLUBBATTLE2:
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                ID2GAMEINTEXT("EMCHAR_GATEOUT_CLUBBATTLE2"));
			break;
        case EMCHAR_GATEOUT_CTF:
            PrintMsgText(
				NS_UITEXTCOLOR::DISABLE,
				ID2GAMEINTEXT("EMCHAR_GATEOUT_CTF"));
            break;

        case EMCHAR_GATEOUT_CDM_MEMS_LIMITED:
            {
                PrintMsgText(
                    NS_UITEXTCOLOR::DISABLE,
                    ID2GAMEINTEXT("EMCHAR_GATEOUT_CDM_MEMBERS_LIMITED"));
            }
            break;

		case EMCHAR_GATEOUT_PARTYMAP :
			{
				const SMAPNODE *pMapNode = FindMapNode( pNetMsg->sMapID.getBaseMapID() );
				if( NULL == pMapNode )
					return;

				// 파티맵?
				if( true == pMapNode->IsPartyMap() && false == pMapNode->IsExpeditionMap() )
					PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT( "EMCHAR_GATEOUT_PARTYMAP" ) );
				// 원정대맵?
				else if( false == pMapNode->IsPartyMap() && true == pMapNode->IsExpeditionMap() )
					PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT( "EMREQUIRE_EXPEDITION" ) );
				// 둘다?
				else if( true == pMapNode->IsPartyMap() && true == pMapNode->IsExpeditionMap() )
					PrintMsgTextDlg( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT( "EMREQUIRE_PARTY_OR_EXPEDITION" ) );
			}
			break;
		};

		return;
	}

	CDebugSet::ToListView ( "[MSG] SNETREQ_GATEOUT_FB OK" );

	const MapID& _mapID(pNetMsg->sMapID);
	const SNATIVEID& _baseMapID(_mapID.getBaseMapID());
	const SNATIVEID& _gaeaMapID(_mapID.getGaeaMapID());
	const SMAPNODE *pMapNode = FindMapNode( _baseMapID );
	if ( !pMapNode )
        return;

	//GfxInterfaceBase::GetInstance()->SetLoadingData( _baseMapID, pMapNode->m_LoadingImageName, false );
	//GfxInterfaceBase::GetInstance()->OnBeginLoading( ELoadingSceneType::MoveMap );

    LoadingThread::SetData(
        pMapNode->m_LoadingImageName.c_str(),
        _baseMapID );

    //LoadingDirectClient sLodingDirect( FALSE );
	if (m_pLodingDirect)
	{
		m_pLodingDirect->Initialize(FALSE);
		NSLoadingDirect::Set( m_pLodingDirect, EM_LT_NORMAL );
	}

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( TRUE );
    //---------------------------------------------------//

	if ( m_pCharacter->VehicleState() )
	{
		m_pCharacter->ReqSetVehicle( false );
		m_pCharacter->SetVehicle ( false );
		m_pCharacter->m_bIsVehicleActive = TRUE;
	}

	m_pCharacter->m_mapMarketClick.clear();

	m_pCharacter->VehicleActiveValueSet(false);
	m_pCharacter->VehicleReset();

	m_pCharacter->ResetAction();             // 주병진

	m_pLandMClient->ClearDropObj ();

	// 다른 맵으로 이동하면
	if ( _gaeaMapID != m_pLandMClient->GetGaeaMapID() )
	{
		// 맵이동시 자신에게 걸린 버프와 상태이상 제거
		m_pCharacter->InitAllSkillFactByMoveMap ();

		hr = MoveActiveMap ( _mapID, pNetMsg->vPos );
		if ( SUCCEEDED(hr) )
		{
			m_pCharacter->MoveActiveMap ( m_pLandMClient->GetLandMan()->GetNaviMesh(), pNetMsg->vPos );
		}
	}
	else
	{
		m_pCharacter->InitializeActorMove(pNetMsg->vPos);
		m_pCharacter->DoActWait ( DoActWait_MsgMoveActiveMap );		
	}

	m_pLandMClient->SetCullByObjectMap(pNetMsg->bCullByObjectMap);

	m_pEngineDevice->SetRenderState ();

	// Note : 맵 이동시 워닝 메세지를 종료한다.
	//m_pInterface->WARNING_MSG_OFF();

	// 미니맵 마우스 이동 목표지점 해제
//	GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_LargeMap_HideMoveIcon );

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( TRUE );
    //---------------------------------------------------//

	NSLoadingDirect::Reset();

	if ( GLUseFeatures::GetInstance().IsUsingTutorialMode() && GetCharacter()->m_bTutorial && !GetTutorial()->IsTutorial() && GetActiveMap()->GetBaseMapID() == SNATIVEID(254,0) )
	{
		GetTutorial()->InitData();
		GetTutorial()->StartTutorialInClient( GetCharacter() );
	}

	//GfxInterfaceBase::GetInstance()->OnEndLoading();
}

void GLGaeaClient::ReqCreateTutorialMap ( NET_MSG_GENERIC *nmg )
{
	/*
	if ( m_pPet->IsVALID() )
	{
		m_pPet->Delete ();
		m_pCharacter->m_bIsPetActive = TRUE;
	}

	HRESULT hr=S_OK;

	GLMSG::SNET_CREATE_TUTORIAL_MAP_FB *pNetMsg = (GLMSG::SNET_CREATE_TUTORIAL_MAP_FB *) nmg;

	m_pCharacter->ReSetSTATE(EM_REQ_GATEOUT);

	if ( pNetMsg->emFB!=EMCHAR_CREATE_TUTORIAL_MAP_OK )
	{
		CDebugSet::ToListView ( "[MSG] SNET_CREATE_TUTORIAL_MAP_FB FAIL : %d", pNetMsg->emFB );

		CString strText;
		switch(pNetMsg->emFB)
		{
		case EMCHAR_CREATE_TUTORIAL_MAP_FAIL:			
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_CREATE_TUTORIAL_MAP_FAIL"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_TUTORIAL_MAP_FAIL_CREATE:
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_CREATE_TUTORIAL_MAP_FAIL"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_TUTORIAL_MAP_FAIL_MAPID:			
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_CREATE_TUTORIAL_MAP_DATA_ERR"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_TUTORIAL_MAP_FAIL_GATEID:			
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_CREATE_TUTORIAL_MAP_DATA_ERR"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_TUTORIAL_MAP_FAIL_TARMAPID:			
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_CREATE_TUTORIAL_MAP_DATA_ERR"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_TUTORIAL_MAP_FAIL_CONDITION:			
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format( ID2GAMEINTEXT("REQ_CREATE_TUTORIAL_MAP_FAIL_CONDITION"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_TUTORIAL_MAP_FAIL_FIELDSVR: 			
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_CREATE_TUTORIAL_MAP_FAIL"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_TUTORIAL_MAP_FAIL_NOT:			
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_CREATE_TUTORIAL_MAP_FAIL"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_TUTORIAL_MAP_FAIL_CREATEMAX:			
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_CREATE_TUTORIALMAP_FAIL"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_TUTORIAL_MAP_FAIL_NOT_STATE:			
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_CREATE_TUTORIAL_MAP_FAIL_NOT_STATE"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_TUTORIAL_MAP_FAIL_REENTRYLIMIT:				
			// 인던 진입시간에 대한 정보가 없으면 넣는다.
			const SMAPNODE *pBaseMapNode = FindMapNode ( pNetMsg->sBaseMapID.dwID );
			GLLevelFileClient cLevelFile;
			BOOL bOk = cLevelFile.LoadFile ( pBaseMapNode->strFile.c_str(), NULL );
			if ( bOk )
			{		
				MAPENTRYINFO_ITER mapIter = m_pCharacter->m_mapEntryLimitInfo.find(pNetMsg->sBaseMapID.dwID);
				if( mapIter != m_pCharacter->m_mapEntryLimitInfo.end() )
				{
					SENTRYLIMITINFO sEntryInfo;
					sEntryInfo.SetData( pNetMsg->tEntryTime, cLevelFile.GetLevelEtcFunc()->m_dwReEntryTime, pNetMsg->sBaseMapID );
					
					PrintMsgTextDlg(
                        NS_UITEXTCOLOR::DISABLE,
                        sc::string::format(
                            ID2GAMEINTEXT("REQ_CREATE_TUTORIAL_MAP_FAIL_REENTRYLIMIT"), 
                            pBaseMapNode->strMapName.c_str(),
                            sEntryInfo.GetEntryLimitMinutes()));

					m_pCharacter->m_mapEntryLimitInfo[pNetMsg->sBaseMapID.dwID] = sEntryInfo;
				}
			}
			break;
		};

        return;
	}

	CDebugSet::ToListView ( "[MSG] SNET_CREATE_INSTANT_MAP_FB OK" );

	const SMAPNODE *pMapNode = FindMapNode ( pNetMsg->sTutorialMapID.dwID );
	if( pMapNode == NULL )
        return;

    LoadingThread::SetData(
        pMapNode->strLoadingImageName.c_str(),
        pNetMsg->sTutorialMapID );

    LoadingDirectClient sLodingDirect( FALSE );
    NSLoadingDirect::Set( &sLodingDirect, EM_LT_NORMAL );

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( TRUE );
    //---------------------------------------------------//

	if ( m_pCharacter->VehicleState() )
	{
		m_pCharacter->ReqSetVehicle( false );
		m_pCharacter->SetVehicle ( false );
		m_pCharacter->m_bIsVehicleActive = TRUE;
	}

	m_pCharacter->VehicleActiveValueSet(false);
	m_pCharacter->VehicleReset();

	m_pCharacter->ResetAction();             // 주병진

	m_pLandMClient->ClearDropObj ();

	// 맵이동시 자신에게 걸린 버프와 상태이상 제거
	m_pCharacter->InitAllSkillFactByMoveMap ();

	hr = CreateInstantMap ( pNetMsg->sBaseMapID, pNetMsg->sTutorialMapID, pNetMsg->vPos );
	if ( SUCCEEDED(hr) )
	{
		m_pCharacter->MoveActiveMap ( m_pLandMClient->GetLandMan()->GetNaviMesh(), pNetMsg->vPos );
	}

	m_pEngineDevice->SetRenderState();

	// Note : 맵 이동시 워닝 메세지를 종료한다.
	m_pInterface->WARNING_MSG_OFF();

	// 미니맵 마우스 이동 목표지점 해제
	GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_LargeMap_HideMoveIcon );

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( TRUE );
    //---------------------------------------------------//

	NSLoadingDirect::Reset();

	if( GetActiveMap()->GetUseLevelEtcFunc(EMETCFUNC_REENTRYLIMIT) )
	{
		SENTRYLIMITINFO sEntryInfo;
		CTime time = CTime::GetCurrentTime();
		sEntryInfo.SetData( time.GetTime(), GetActiveMap()->GetLevelEtcFunc()->m_dwReEntryTime, GetActiveMap()->GetMapID() );
		m_pCharacter->m_mapEntryLimitInfo[pNetMsg->sBaseMapID.dwID] = sEntryInfo;
	}
	*/
}


void GLGaeaClient::ReqCreateInstantMap ( NET_MSG_GENERIC *nmg )
{
	if ( m_pPet->IsVALID() )
	{
		m_pPet->Delete ();
		m_pCharacter->m_bIsPetActive = TRUE;
	}

	HRESULT hr=S_OK;

	GLMSG::SNET_CREATE_INSTANT_MAP_FB *pNetMsg = (GLMSG::SNET_CREATE_INSTANT_MAP_FB *) nmg;
	const MapID& _mapID(pNetMsg->mapID);
	const SNATIVEID& _baseMapID(_mapID.getBaseMapID());

	m_pCharacter->ReSetActState(EM_REQ_GATEOUT);

	if ( pNetMsg->emFB!=EMCHAR_CREATE_INSTANT_MAP_OK )
	{
		CDebugSet::ToListView ( "[MSG] SNET_CREATE_INSTANT_MAP_FB FAIL : %d", pNetMsg->emFB );

        CString strText;
		switch (pNetMsg->emFB)
		{
		case EMCHAR_CREATE_INSTANT_MAP_FAIL:				
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_CREATE_INSTANT_MAP_FAIL"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_INSTANT_MAP_FAIL_CREATE:
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_CREATE_INSTANT_MAP_FAIL"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_INSTANT_MAP_FAIL_MAPID:				
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_CREATE_INSTANT_MAP_DATA_ERR"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_INSTANT_MAP_FAIL_GATEID:				
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_CREATE_INSTANT_MAP_DATA_ERR"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_INSTANT_MAP_FAIL_TARMAPID:				
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_CREATE_INSTANT_MAP_DATA_ERR"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_INSTANT_MAP_FAIL_CONDITION:				
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_CREATE_INSTANT_MAP_FAIL_CONDITION"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_INSTANT_MAP_FAIL_FIELDSVR:
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_CREATE_INSTANT_MAP_FAIL"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_INSTANT_MAP_FAIL_NOT:				
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_CREATE_INSTANT_MAP_FAIL"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_INSTANT_MAP_FAIL_CREATEMAX:				
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_CREATE_INSTANT_MAP_FAIL_CREATEMAX"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_INSTANT_MAP_FAIL_NOT_STATE:				
			PrintMsgText(
                NS_UITEXTCOLOR::DISABLE,
                sc::string::format(ID2GAMEINTEXT("REQ_CREATE_INSTANT_MAP_FAIL_NOT_STATE"), pNetMsg->emFB));
			break;
		case EMCHAR_CREATE_INSTANT_MAP_FAIL_REENTRYLIMIT:				
			// 인던 진입시간에 대한 정보가 없으면 넣는다.
			const SMAPNODE *pBaseMapNode = FindMapNode ( _baseMapID );
			GLLevelFileClient cLevelFile(this);
			BOOL bOk = cLevelFile.LoadFile ( pBaseMapNode->m_LevelFile.c_str(), NULL );
			if ( bOk )
			{		
				MAPENTRYINFO_ITER mapIter = m_pCharacter->m_mapEntryLimitInfo.find(_baseMapID.Id());
				if( mapIter != m_pCharacter->m_mapEntryLimitInfo.end() )
				{
					SENTRYLIMITINFO sEntryInfo;
					sEntryInfo.SetData( pNetMsg->tEntryTime, cLevelFile.GetLevelEtcFunc()->m_dwReEntryTime, _baseMapID );						
					
					PrintMsgTextDlg(
                        NS_UITEXTCOLOR::DISABLE,
                        sc::string::format(
                            ID2GAMEINTEXT("REQ_CREATE_INSTANT_MAP_FAIL_REENTRYLIMIT"), 
                            GetMapName( _baseMapID ),
                            sEntryInfo.GetEntryLimitMinutes()));

					m_pCharacter->m_mapEntryLimitInfo[_baseMapID.Id()] = sEntryInfo;
				}
			}
	
			break;
		};

        return;
	}

	CDebugSet::ToListView ( "[MSG] SNET_CREATE_INSTANT_MAP_FB OK" );

	const SMAPNODE* pMapNode = FindMapNode ( _baseMapID );
	if (!pMapNode)
        return;

	//GfxInterfaceBase::GetInstance()->SetLoadingData( _baseMapID, pMapNode->m_LoadingImageName, false );
	//GfxInterfaceBase::GetInstance()->OnBeginLoading( ELoadingSceneType::MoveMap );

    LoadingThread::SetData(
        pMapNode->m_LoadingImageName.c_str(),
        _baseMapID );

    //LoadingDirectClient sLodingDirect( FALSE );
	if (m_pLodingDirect)
	{
		m_pLodingDirect->Initialize(FALSE);
		NSLoadingDirect::Set( m_pLodingDirect, EM_LT_NORMAL );
	}

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( TRUE );
    //---------------------------------------------------//

	if ( m_pCharacter->VehicleState() )
	{
		m_pCharacter->ReqSetVehicle( false );
		m_pCharacter->SetVehicle ( false );
		m_pCharacter->m_bIsVehicleActive = TRUE;
	}

	m_pCharacter->VehicleActiveValueSet(false);
	m_pCharacter->VehicleReset();

	m_pCharacter->ResetAction();             // 주병진

	m_pLandMClient->ClearDropObj ();

	// 맵이동시 자신에게 걸린 버프와 상태이상 제거
	m_pCharacter->InitAllSkillFactByMoveMap ();
	
	hr = CreateInstantMap ( _mapID, pNetMsg->vPos );
	if ( SUCCEEDED(hr) )
	{
		m_pCharacter->MoveActiveMap ( m_pLandMClient->GetLandMan()->GetNaviMesh(), pNetMsg->vPos );
	}

	m_pLandMClient->SetCullByObjectMap(pNetMsg->bCullByObjectMap);

	m_pEngineDevice->SetRenderState();

	// Note : 맵 이동시 워닝 메세지를 종료한다.
	//m_pInterface->WARNING_MSG_OFF();

	// 미니맵 마우스 이동 목표지점 해제
//	GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_LargeMap_HideMoveIcon );

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( TRUE );
    //---------------------------------------------------//

	NSLoadingDirect::Reset();

	if( GetActiveMap()->GetUseLevelEtcFunc(EMETCFUNC_REENTRYLIMIT) )
	{
		SENTRYLIMITINFO sEntryInfo;
		CTime time = CTime::GetCurrentTime();
		sEntryInfo.SetData( time.GetTime(), GetActiveMap()->GetLevelEtcFunc()->m_dwReEntryTime, _baseMapID );
		m_pCharacter->m_mapEntryLimitInfo[_baseMapID.Id()] = sEntryInfo;
	}

	//GfxInterfaceBase::GetInstance()->OnEndLoading();
}

void GLGaeaClient::MsgResurrect(NET_MSG_GENERIC* nmg)
{
    if ( m_pPet->IsVALID() )
    {
        m_pPet->Delete ();
        m_pCharacter->m_bIsPetActive = TRUE;
    }

    if ( m_pCharacter->VehicleState() )
    {
        m_pCharacter->ReqSetVehicle( false );
        m_pCharacter->SetVehicle ( false );
        m_pCharacter->m_bIsVehicleActive = TRUE;
    }

    m_pCharacter->ResetAction();             // 주병진;

    HRESULT hr=S_OK;

    GLMSG::NET_CHAR_RESURRECT_FAFC* pPacket = (GLMSG::NET_CHAR_RESURRECT_FAFC*) nmg;

	const MapID& _mapID(pPacket->sMapID);
	const SNATIVEID& _baseMapID(_mapID.getBaseMapID());
	const SNATIVEID& _gaeaMapID(_mapID.getGaeaMapID());
    const SMAPNODE *pMapNode = FindMapNode ( _baseMapID);
    if( pMapNode == NULL )
        return;

	//GfxInterfaceBase::GetInstance()->SetLoadingData( _baseMapID, pMapNode->m_LoadingImageName, false );
	//GfxInterfaceBase::GetInstance()->OnBeginLoading( ELoadingSceneType::MoveMap );

    LoadingThread::SetData(
        pMapNode->m_LoadingImageName.c_str(),
        _baseMapID );

    //LoadingDirectClient sLodingDirect( FALSE );
	if (m_pLodingDirect)
	{
		m_pLodingDirect->Initialize(FALSE);
		NSLoadingDirect::Set( m_pLodingDirect, EM_LT_NORMAL );
	}

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( TRUE );
    //---------------------------------------------------//

    m_pLandMClient->ClearDropObj ();

    //	부활할 Map이 틀릴 경우.;
    if ( _gaeaMapID != m_pLandMClient->GetGaeaMapID() )
    {
        hr = MoveActiveMap ( _mapID, pPacket->vPos );
        if ( SUCCEEDED(hr) )
        {
            m_pCharacter->MoveActiveMap ( m_pLandMClient->GetLandMan()->GetNaviMesh(), pPacket->vPos );
        }
    }
    else
    {
        m_pCharacter->InitializeActorMove( pPacket->vPos);
        m_pCharacter->DoActWait ( DoActWait_MsgResurrect );
    }

    m_pLandMClient->SetCullByObjectMap(pPacket->bCullByObjectMap);

    // 미니맵 마우스 이동 목표지점 해제;
//	GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_LargeMap_HideMoveIcon );

    //	부활처리.;
    m_pCharacter->Resurrect ( pPacket->nNowHP, pPacket->nNowMP, pPacket->nNowSP, pPacket->vPos, false );

	//!! SF_TODO
	if ( m_pInterface->IsVisible( FORCE_REBIRTH ) )
	{
		m_pInterface->UiHideGroup(FORCE_REBIRTH);
	}

	if ( m_pInterface->IsVisible( FORCE_REBIRTH ) )
	{
		m_pInterface->UiHideGroup( FORCE_REBIRTH );
	}

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( TRUE );
    //---------------------------------------------------//

    NSLoadingDirect::Reset();

	//GfxInterfaceBase::GetInstance()->OnEndLoading();
}

void GLGaeaClient::MsgResurrectInfo(NET_MSG_GENERIC* nmg)
{
    GLMSG::NET_CHAR_RESURRECT_INFO_FC* pPacket = (GLMSG::NET_CHAR_RESURRECT_INFO_FC*) nmg;
	if ( NULL == pPacket )
		return;

    m_pCharacter->m_sResurrectAbleState = pPacket->sResurrectAbleState;

	// 스킬 부활 가능한지 확인;
	const bool bSkillResurrection = m_pCharacter->m_sResurrectAbleState.isState( EM_RESURRECTION_SKILL );

	// 강제 부활 제외하고 가능한지 확인;
	const bool bGeneralResurrection(
		m_pCharacter->m_sResurrectAbleState.isState(EM_RESURRECTION_NOMAL) | 
		m_pCharacter->m_sResurrectAbleState.isState(EM_RESURRECTION_SPECIAL) |
		m_pCharacter->m_sResurrectAbleState.isState(EM_RESURRECTION_SPECIAL_ITEM) |
		m_pCharacter->m_sResurrectAbleState.isState(EM_RESURRECTION_SKILL) );

	// 강제 부활 가능한지 확인;
	const bool bForcedResurrection = m_pCharacter->m_sResurrectAbleState.isState( EM_RESURRECTION_FORCED );

	float fResurrectTime = 0.0f;

	// 강제 부활 가능할 경우 부활시간 세팅;
	if ( bForcedResurrection  )
	{
		fResurrectTime = pPacket->fResurrectTime;

		// 강제 부활시간이 서버와 다른 부분 보정 [8/21/2014 hsshin];
		// 강제부활의 경우 서버에서는 죽음과 동시에 카운트를 시작하지만
		// 클라이언트의 경우 Falling(쓰러지는 모션)후에 Die(죽어있는 상태)로 변한 시점에서
		// 카운트가 시작되므로 부활 시간이 차이가 나게 된다.
		// 따라서 이런형태로 보정을 해줌;
		if( 0.0f < fResurrectTime )
		{
			float fFallingAniTime = 0.0f;
			GLCharacter* pChar = GetCharacter();
			if( NULL != pChar )
			{
				DxSkinChar* pSkinChar = pChar->GetSkinChar();
				if( NULL != pSkinChar )
				{
					const PANIMCONTNODE pAnim = pSkinChar->GETANI( AN_DIE, AN_SUB_NONE );
					DxSkinAnimation* pSkinAni = pAnim->m_pDxSkinAni;
					if( NULL != pSkinAni )
					{
						const DWORD dwStartTime = pSkinAni->m_dwSTime;
						const DWORD dwEndTime = pSkinAni->m_dwETime;
						const DWORD dwUnitTime = pSkinAni->m_dwETime;

						fFallingAniTime = (float) (dwEndTime - dwStartTime) / (float) dwUnitTime;
					}
				}
			}
			fResurrectTime = fResurrectTime - fFallingAniTime;
		}
		GetInterface()->SetForceResurrect( fResurrectTime );
	}

	GetInterface()->SetReviveSkill( bSkillResurrection );

	/*GLWidgetScript::GetInstance().LuaCallFunc< void >(
		NSWIDGET_SCRIPT::g_strFunc_Resurrection_ReceiveInfo,
		"-b, -b, -b, -f", bSkillResurrection, bGeneralResurrection, bForcedResurrection, fResurrectTime );*/
}

void GLGaeaClient::MsgResurrectFB(NET_MSG_GENERIC* nmg)
{
    GLMSG::NET_CHAR_REQ_RESURRECT_FB* pPacket = (GLMSG::NET_CHAR_REQ_RESURRECT_FB*) nmg;
    
    switch ( pPacket->resurrectType )
    {
    case EM_RESURRECTION_NOMAL:
        {
            switch ( pPacket->emResult )
            {
            case EMREQ_RESURRECT_NOMAL_FB_OK:
                break;
            case EMREQ_RESURRECT_NOMAL_FB_ENTRY_FAIL:
				//GLWidgetScript::GetInstance().LuaCallFunc< void >(
				//	NSWIDGET_SCRIPT::g_strFunc_RegenEntryFail );
                break;
            }
        }
        break;

    case EM_RESURRECTION_SPECIAL:
        {
            // 경험치회복 부활 UseFeatures 기능이 가능한가?;
            if ( CanRecoveryExp() == false )
            {
                // 실패시 일반 부활 요청;
                m_pCharacter->ReqResurrect(EM_RESURRECTION_NOMAL);
				
				//GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_EnterGameStage );
                return;
            }

            switch ( pPacket->emResult )
            {
            case EMREQ_RESURRECT_SPECIAL_FB_FAIL:
                PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECOVERY_FB_FAIL") );
                break;
            case EMREQ_RESURRECT_SPECIAL_FB_OK:
                {
                    std::string strTextExp = sc::string::format( ID2GAMEINTEXT("EMREQ_RECOVERY_FB_OK"),  pPacket->lnRestorableEXP).c_str();
                    PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, strTextExp );
                    std::string strTextMoney = sc::string::format( ID2GAMEINTEXT("EMREQ_RECOVERY_FB_OK_MONEY"), pPacket->lnRestoreExpCost ).c_str();
					PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, strTextMoney );
					m_pCharacter->FactEffectProcessing( EMFACTEFFECTPROCESSING_REBIRTH );
                }
                break;
            case EMREQ_RESURRECT_SPECIAL_FB_NOMONEY:
                PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECOVERY_FB_NOMONEY") );
                break;
            case EMREQ_RESURRECT_SPECIAL_FB_NOEXP:
                PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECOVERY_FB_NOREEXP") );
                break;
            case EMREQ_RESURRECT_SPECIAL_FB_NOTUSE:
                PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECOVERY_FB_NOTUSE") );
                break;
            };

			//GLWidgetScript::GetInstance().LuaCallFunc< void >(
			//	NSWIDGET_SCRIPT::g_strFunc_Resurrection_Close );

            if ( pPacket->emResult != EMREQ_RESURRECT_SPECIAL_FB_OK )
            {
                // 실패시 일반 부활 요청;
                m_pCharacter->ReqResurrect(EM_RESURRECTION_NOMAL);
				
				//GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_EnterGameStage );
            }
        }
        break;

    case EM_RESURRECTION_SPECIAL_ITEM:
        {
            switch ( pPacket->emResult )
            {
            case EMREQ_RESURRECT_SPECIAL_ITEM_FB_FAIL:
                PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_REVIVE_FB_FAIL") );
                break;
            case EMREQ_RESURRECT_SPECIAL_ITEM_FB_OK:
                PrintMsgText ( NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMREQ_REVIVE_FB_OK") );
				m_pCharacter->FactEffectProcessing( EMFACTEFFECTPROCESSING_REBIRTH );
                break;
            case EMREQ_RESURRECT_SPECIAL_ITEM_FB_NOITEM:
                PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_REVIVE_FB_NOITEM") );
                break;
            case EMREQ_RESURRECT_SPECIAL_ITEM_FB_NOTUSE:
                PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_REVIVE_FB_NOTUSE") );
                break;
            case EMREQ_RESURRECT_SPECIAL_ITEM_FB_COOLTIME:
                break;
            };

            if ( pPacket->emResult != EMREQ_RESURRECT_SPECIAL_ITEM_FB_OK )
            {
                // 실패시 일반 부활 요청;
                m_pCharacter->ReqResurrect(EM_RESURRECTION_NOMAL);
				
				//GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_EnterGameStage );
            }
        }
        break;

    case EM_RESURRECTION_SKILL:
        {
            switch ( pPacket->emResult ) 
            {   
			case EMSKILL_REVIVE_FB_OK:
				m_pCharacter->FactEffectProcessing( EMFACTEFFECTPROCESSING_REBIRTH );
                break;
            case EMSKILL_REVIVE_FB_FAIL:
                {
                    PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMSKILL_REVIVE_FB_FAIL") );
                }
                break;
            case EMSKILL_REVIVE_FB_NOTVALUE:
                {
                    PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMSKILL_REVIVE_FB_NOTVALUE") );
                }
                break;
            }

            if ( pPacket->emResult != EMSKILL_REVIVE_FB_OK )
            {
                // 실패시 일반 부활 요청;
                m_pCharacter->ReqResurrect(EM_RESURRECTION_NOMAL);

				//GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_EnterGameStage );
            }
        }
        break;
    }
}

void GLGaeaClient::ReLvMustLeaveFB(NET_MSG_GENERIC* nmg)
{
	HRESULT hr=S_OK;

	GLMSG::SNETPC_REQ_MUST_LEAVE_MAP_FB *pNetMsg = (GLMSG::SNETPC_REQ_MUST_LEAVE_MAP_FB *) nmg;
	if ( pNetMsg->emFB != EMREQ_MUST_LEAVE_MAP_FB_OK )
	{
		PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_MUST_LEAVE_MAP_FB_FAIL") );
		return;
	}

	// 이동이 성공 했을 경우에만 펫을 삭제한다.
	if ( m_pPet->IsVALID() )
	{
		m_pPet->Delete ();
		m_pCharacter->m_bIsPetActive = TRUE;
	}

	if ( m_pCharacter->VehicleState() )

	{
		m_pCharacter->ReqSetVehicle( false );
		m_pCharacter->SetVehicle ( false );
		m_pCharacter->m_bIsVehicleActive = TRUE;
	}

	m_pCharacter->ResetAction();

	const MapID& _mapID(pNetMsg->sMAPID);
	const SNATIVEID& _baseMapID(_mapID.getBaseMapID());
	const SNATIVEID& _gaeaMapID(_mapID.getGaeaMapID());
	const SMAPNODE *pMapNode = FindMapNode(_baseMapID);
	if( pMapNode == NULL )
        return;

	//GfxInterfaceBase::GetInstance()->SetLoadingData( _baseMapID, pMapNode->m_LoadingImageName, false );
	//GfxInterfaceBase::GetInstance()->OnBeginLoading( ELoadingSceneType::MoveMap );

	LoadingThread::SetData(
        pMapNode->m_LoadingImageName.c_str(),
        _baseMapID );

    //LoadingDirectClient sLodingDirect( FALSE );
	if (m_pLodingDirect)
	{
		m_pLodingDirect->Initialize(FALSE);
		NSLoadingDirect::Set( m_pLodingDirect, EM_LT_NORMAL );
	}

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( TRUE );
    //---------------------------------------------------//

	m_pLandMClient->ClearDropObj ();

	//	이동할 Map이 틀릴 경우.
	if ( _gaeaMapID != m_pLandMClient->GetGaeaMapID() )
	{
		// 맵이동시 자신에게 걸린 버프와 상태이상 제거
		m_pCharacter->InitAllSkillFactByMoveMap ();

		hr = MoveActiveMap(_mapID, pNetMsg->vPOS );
		if ( SUCCEEDED(hr) )
		{
			m_pCharacter->MoveActiveMap ( m_pLandMClient->GetLandMan()->GetNaviMesh(), pNetMsg->vPOS );
		}
	}
	else
	{
    	m_pCharacter->InitializeActorMove ( pNetMsg->vPOS );
		m_pCharacter->DoActWait ( DoActWait_ReLvMustLeaveFB );
	}

	m_pLandMClient->SetCullByObjectMap(pNetMsg->bCullByObjectMap);

	// 미니맵 마우스 이동 목표지점 해제
//	GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_LargeMap_HideMoveIcon );

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( TRUE );
    //---------------------------------------------------//

	NSLoadingDirect::Reset();

	//GfxInterfaceBase::GetInstance()->OnEndLoading();
}

void GLGaeaClient::MoveMapResultAFB ( NET_MSG_GENERIC* pMsg )
{
	GLMSG::SNETPC_NOTICE_MOVEMAP_RESULT_AFB *pNetMsg =
		static_cast< GLMSG::SNETPC_NOTICE_MOVEMAP_RESULT_AFB* >( pMsg );
	if ( NULL == pNetMsg )
		return;

	if ( pNetMsg->emFB != EMREQ_MOVEMAP_RESULT_AFB_OK )
	{
		if (EMREQ_MOVEMAP_RESULT_AFB_LOADING != pNetMsg->emFB)
		{
			m_pCharacter->ReSetActState( EM_REQ_GATEOUT );

			NSLoadingDirect::Reset();
		}

		switch ( pNetMsg->emFB )
		{
		case EMREQ_MOVEMAP_RESULT_AFB_LOADING:
			{
				const MapID& _mapID(pNetMsg->sMAPID);	
				const SNATIVEID& _baseMapID(_mapID.getBaseMapID());
				const SNATIVEID& _gaeaMapID(_mapID.getGaeaMapID());
				const SMAPNODE *pMapNode = FindMapNode ( _baseMapID );
				if( pMapNode == NULL )
					return;

				LoadingThread::SetData(
					pMapNode->m_LoadingImageName.c_str(),
					_baseMapID );

				//LoadingDirectClient sLodingDirect( FALSE );
				if (m_pLodingDirect)
				{
					m_pLodingDirect->Initialize(FALSE);
					NSLoadingDirect::Set( m_pLodingDirect, EM_LT_NORMAL );
				}

				//---------------------------------------------------//
				NSLoadingDirect::FrameMoveRender( TRUE );
				//---------------------------------------------------//
			}
			return;

		case EMREQ_MOVEMAP_RESULT_AFB_FAIL:
			switch ( pNetMsg->emMoveType )
			{
			case MoveSystem::EMMOVE_TYPE_RECALL:
				PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_FAIL") );
				break;

			case MoveSystem::EMMOVE_TYPE_TELEPORT:
				PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_FAIL") );
				break;

			default:
				PrintMsgText(NS_UITEXTCOLOR::DISABLE, sc::string::format( ID2GAMEINTEXT("REQ_GATEOUT_FB_FAIL"), 1 ) );
				break;
			}

			return;

		case EMREQ_MOVEMAP_RESULT_AFB_ITEM:
			switch ( pNetMsg->emMoveType )
			{
			case MoveSystem::EMMOVE_TYPE_RECALL:
				PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_ITEM") );
				break;

			case MoveSystem::EMMOVE_TYPE_TELEPORT:
				PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_ITEM") );
				break;

			default:
				PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_ITEM") );
				break;
			}

			return;

		case EMREQ_MOVEMAP_RESULT_AFB_CONDITION:
			switch ( pNetMsg->emMoveType )
			{
			case MoveSystem::EMMOVE_TYPE_RECALL:
				PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_CONDITION") );
				break;

			case MoveSystem::EMMOVE_TYPE_TELEPORT:
				PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_CONDITION") );
				break;

			default:
				PrintMsgText(NS_UITEXTCOLOR::DISABLE, sc::string::format( ID2GAMEINTEXT("EMCHAR_GATEOUT_CONDITION"), 8 ) );
				break;
			}

			return;

		case EMREQ_MOVEMAP_RESULT_AFB_NOTLASTCALL:
			PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_NOTLASTCALL") );
			return;

		case EMREQ_MOVEMAP_RESULT_AFB_INSTANTMAP:
			PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_INSTANTMAP") );
			return;

		case EMREQ_MOVEMAP_RESULT_AFB_JOINCON:
			switch ( pNetMsg->emMoveType )
			{
			case MoveSystem::EMMOVE_TYPE_RECALL:
				PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_JOINCON") );
				break;

			case MoveSystem::EMMOVE_TYPE_TELEPORT:
				PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_JOINCON") );
				break;

			default:
				PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_JOINCON") );
				break;
			}

			return;
			
		case EMREQ_MOVEMAP_RESULT_AFB_IMMOVABLE:
			switch ( pNetMsg->emMoveType )
			{
			case MoveSystem::EMMOVE_TYPE_RECALL:
				PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_IMMOVABLE") );
				break;

			case MoveSystem::EMMOVE_TYPE_TELEPORT:
				PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_IMMOVABLE") );
				break;

			default:
				PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_RECALL_FB_IMMOVABLE") );
				break;
			}

			return;
		};
	}

	m_pCharacter->ReSetActState( EM_REQ_GATEOUT );

	m_pCharacter->m_mapMarketClick.clear();

	// 이동이 성공 했을 경우에만 펫을 삭제한다.
	if ( m_pPet->IsVALID() )
	{
		m_pPet->Delete ();
		m_pCharacter->m_bIsPetActive = TRUE;
	}

	if ( m_pCharacter->VehicleState() )
	{
		m_pCharacter->ReqSetVehicle( false );
		m_pCharacter->SetVehicle ( false );
		m_pCharacter->m_bIsVehicleActive = TRUE;
	}

	m_pCharacter->VehicleActiveValueSet(false);
	m_pCharacter->VehicleReset();

	m_pCharacter->ResetAction();

	const MapID& _mapID(pNetMsg->sMAPID);	
	const SNATIVEID& _baseMapID(_mapID.getBaseMapID());
	const SNATIVEID& _gaeaMapID(_mapID.getGaeaMapID());
	const SMAPNODE *pMapNode = FindMapNode ( _baseMapID );
	if( pMapNode == NULL )
	{
		sc::writeLogError(sc::string::format(
			"[ Move Map ] [ pMapNode is NULL, Character : %1% ]",
			m_pCharacter->CharDbNum()));

		NSLoadingDirect::Reset();

        return;
	}

	//GfxInterfaceBase::GetInstance()->SetLoadingData( _baseMapID, pMapNode->m_LoadingImageName, false );
	//GfxInterfaceBase::GetInstance()->OnBeginLoading( ELoadingSceneType::MoveMap );

    //---------------------------------------------------//
	NSLoadingDirect::FrameMoveRender( TRUE );
    //---------------------------------------------------//

	m_pLandMClient->ClearDropObj ();

	//	이동할 Map이 틀릴 경우.
	if ( _gaeaMapID != m_pLandMClient->GetGaeaMapID() )
	{
		// 맵이동시 자신에게 걸린 버프와 상태이상 제거
		m_pCharacter->InitAllSkillFactByMoveMap ();

		HRESULT hr = MoveActiveMap ( _mapID, pNetMsg->vPOS );
		if ( SUCCEEDED(hr) )
		{
			m_pCharacter->MoveActiveMap ( m_pLandMClient->GetLandMan()->GetNaviMesh(), pNetMsg->vPOS );
		}
	}
	else
	{
		m_pCharacter->InitializeActorMove ( pNetMsg->vPOS );
		m_pCharacter->DoActWait ( DoActWait_ReCallFB );
	}

	m_pLandMClient->SetCullByObjectMap(pNetMsg->bCullByObjectMap);

	m_pEngineDevice->SetRenderState ();

	// 미니맵 마우스 이동 목표지점 해제
//	GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_LargeMap_HideMoveIcon );

    //---------------------------------------------------//
	NSLoadingDirect::FrameMoveRender( TRUE );
    //---------------------------------------------------//

	NSLoadingDirect::Reset();

	//GfxInterfaceBase::GetInstance()->OnEndLoading();
}

void GLGaeaClient::TeleportFB (NET_MSG_GENERIC* nmg)
{
	HRESULT hr=S_OK;

	GLMSG::SNETPC_REQ_TELEPORT_FB *pNetMsg = (GLMSG::SNETPC_REQ_TELEPORT_FB *) nmg;
	if ( pNetMsg->emFB!=EMREQ_TELEPORT_FB_OK )
	{
		switch ( pNetMsg->emFB )
		{
		case EMREQ_TELEPORT_FB_FAIL:
			PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_FAIL") );
			return;

		case EMREQ_TELEPORT_FB_ITEM:
			PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_ITEM") );
			return;

		case EMREQ_TELEPORT_FB_CONDITION:
			PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_CONDITION") );
			return;

		case EMREQ_TELEPORT_FB_JOINCON:
			PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_JOINCON") );
			return;
			
		case EMREQ_TELEPORT_FB_IMMOVABLE:
			PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMREQ_TELEPORT_FB_IMMOVABLE") );
			return;
		};
	}

	// 이동이 성공 했을 경우에만 펫을 삭제한다.
	if ( m_pPet->IsVALID() )
	{
		m_pPet->Delete ();
		m_pCharacter->m_bIsPetActive = TRUE;
	}

	if ( m_pCharacter->VehicleState() )
	{
		m_pCharacter->ReqSetVehicle( false );
		m_pCharacter->SetVehicle ( false );
		m_pCharacter->m_bIsVehicleActive = TRUE;
	}

	m_pCharacter->ResetAction();             // 주병진

	const MapID& _mapID(pNetMsg->sMAPID);
	const SNATIVEID& _baseMapID(_mapID.getBaseMapID());
	const SNATIVEID& _gaeaMapID(_mapID.getGaeaMapID());
	const SMAPNODE *pMapNode = FindMapNode ( _baseMapID );
	if( pMapNode == NULL )
        return;

	//GfxInterfaceBase::GetInstance()->SetLoadingData( _baseMapID, pMapNode->m_LoadingImageName, false );
	//GfxInterfaceBase::GetInstance()->OnBeginLoading( ELoadingSceneType::MoveMap );

	LoadingThread::SetData(
        pMapNode->m_LoadingImageName.c_str(),
        _baseMapID );

    //LoadingDirectClient sLodingDirect( FALSE );
	if (m_pLodingDirect)
	{
		m_pLodingDirect->Initialize(FALSE);
		NSLoadingDirect::Set( m_pLodingDirect, EM_LT_NORMAL );
	}

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( TRUE );
    //---------------------------------------------------//

	m_pLandMClient->ClearDropObj ();

	//	이동할 Map이 틀릴 경우.
	if ( _gaeaMapID != m_pLandMClient->GetGaeaMapID() )
	{
		// 맵이동시 자신에게 걸린 버프와 상태이상 제거
		m_pCharacter->InitAllSkillFactByMoveMap ();

		hr = MoveActiveMap ( _mapID, pNetMsg->vPOS );
		if ( SUCCEEDED(hr) )
		{
			m_pCharacter->MoveActiveMap ( m_pLandMClient->GetLandMan()->GetNaviMesh(), pNetMsg->vPOS );
		}
	}
	else
	{
		m_pCharacter->InitializeActorMove ( pNetMsg->vPOS );
		m_pCharacter->DoActWait ( DoActWait_TeleportFB );
	}

	m_pLandMClient->SetCullByObjectMap(pNetMsg->bCullByObjectMap);

	// 미니맵 마우스 이동 목표지점 해제
//	GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_LargeMap_HideMoveIcon );

	m_pCharacter->ReSetActState(EM_REQ_GATEOUT);

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( TRUE );
    //---------------------------------------------------//

	NSLoadingDirect::Reset();

	//GfxInterfaceBase::GetInstance()->OnEndLoading();
}

void GLGaeaClient::ReqBusFB (NET_MSG_GENERIC* nmg)
{
	GLMSG::SNETPC_REQ_BUS_FB *pNetMsg = (GLMSG::SNETPC_REQ_BUS_FB *) nmg;
	switch ( pNetMsg->emFB )
	{
	case EMBUS_TAKE_FAIL:
		PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMBUS_TAKE_FAIL") );
		break;
	case EMBUS_TAKE_OK:
		PrintMsgText(NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMBUS_TAKE_OK") );
		break;
	case EMBUS_TAKE_TICKET:
		PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMBUS_TAKE_TICKET") );
		break;
	case EMBUS_TAKE_CONDITION:
		PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMBUS_TAKE_CONDITION") );
		break;
	};
}

void GLGaeaClient::ReqTaxiFB (NET_MSG_GENERIC* nmg)
{
	GLMSG::SNETPC_REQ_TAXI_FB *pNetMsg = (GLMSG::SNETPC_REQ_TAXI_FB *) nmg;
	switch ( pNetMsg->emFB )
	{
	case EMTAXI_TAKE_FAIL:
		PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_FAIL") );
		break;
	case EMTAXI_TAKE_OK:
		PrintMsgText(NS_UITEXTCOLOR::PALEGREEN, ID2GAMEINTEXT("EMTAXI_TAKE_OK") );
		break;
	case EMTAXI_TAKE_TICKET:
		PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_TICKET") );
		break;
	case EMTAXI_TAKE_MONEY:
		PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_MONEY") );
		break;
	case EMTAXI_TAKE_CONDITION:
		PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_CONDITION") );
		break;
	case EMTAXI_TAKE_MAPFAIL:
		PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_MAPFAIL") );
		break;
	case EMTAXI_TAKE_STATIONFAIL:
		PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_STATIONFAIL") );
		break;
	case EMTAXI_TAKE_NPCFAIL:
		PrintMsgText(NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAXI_TAKE_NPCFAIL") );
		break;	
	};
}

void GLGaeaClient::DropOutCrow ( NET_MSG_GENERIC *nmg )
{
	if ( !m_pLandMClient )
        return;
	GLMSG::SNETDROP_OUT *pNetMsg = (GLMSG::SNETDROP_OUT *) nmg;
	//CDebugSet::ToListView ( "DROPOUT_CROW [%s] : %d", COMMENT::CROW[pNetMsg->emCrow].c_str(), pNetMsg->dwID );

	STARID sTARID;
	const WORD wNUM = pNetMsg->GETAMOUNT();
	for ( WORD i=0; i < wNUM; ++i )
	{
		bool bOK = pNetMsg->POP_CROW( sTARID );
		if ( !bOK )
            return;

		switch ( sTARID.emCrow )
		{
		case CROW_PC:
			m_pLandMClient->DropOutChar ( sTARID.dwID );
			break;

		case CROW_NPC:
		case CROW_MOB:
			m_pLandMClient->DropOutCrow ( sTARID.dwID );
			break;
		case CROW_MATERIAL:
			m_pLandMClient->DropOutMaterial ( sTARID.dwID );
			break;

		case CROW_ITEM:
			m_pLandMClient->DropOutItem ( sTARID.dwID );
			break;

		case CROW_MONEY:
			m_pLandMClient->DropOutMoney ( sTARID.dwID );
			break;

		case CROW_PET:
			m_pLandMClient->DropOutPet ( sTARID.dwID );
			break;

		case CROW_SUMMON:
			m_pLandMClient->DropOutSummon ( sTARID.dwID );
			break;
		};
	}
}

BOOL GLGaeaClient::IsTutorialMap()
{
	/*
	GLLandManClient* pLand = GetActiveMap();

    for ( int i = 0; i < GLCONST_CHAR::wTUTORIA_NUM; i++ )
    {
    	if ( pLand->GetMapID().wMainID == GLCONST_CHAR::nidTUTORIALMAP[ i ].wMainID ) return TRUE; // 수정. 튜토리얼 맵은 인던 아님. by luxes.
    }
	*/

	return FALSE;
}

const bool GLGaeaClient::IsNewInstanceMap ( void )
{
	GLLandManClient* pLand = GetActiveMap();
	if ( NULL == pLand )
		return false;

	return pLand->IsNewInstantMap();
}

const bool GLGaeaClient::IsOldInstanceMap ( void )
{
	GLLandManClient* pLand = GetActiveMap();
	if ( NULL == pLand )
		return false;

	return pLand->IsOldInstantMap();
}

BOOL GLGaeaClient::IsInstantMap()
{
	GLLandManClient* pLand = GetActiveMap();

	// [shhan][2014.11.13] pLand 검사를 추가함. WorldEdit 에서 종료현상이 나타났었음.
    //! 진짜 인던일때만 체크
    if ( pLand && pLand->IsInstantMap() )
    {
        return TRUE;
    }

    return FALSE;
}

BOOL GLGaeaClient::IsVisibleCV ( const STARGETID & sTargetID )
{
	if ( !m_pLandMClient )
        return FALSE;

	if ( sTargetID.GaeaId == EMTARGET_NULL )
        return FALSE;

	if ( sTargetID.emCrow == CROW_PC )
	{
		std::tr1::shared_ptr<GLCharClient> pChar = m_pLandMClient->GetChar ( sTargetID.GaeaId );
		if ( pChar )
            return pChar->IsCollisionVolume();

		if ( GetCharacter()->GetGaeaID() == sTargetID.GaeaId )
			return TRUE;
	}
	else if ( sTargetID.emCrow == CROW_NPC || sTargetID.emCrow == CROW_MOB ) 
	{
        std::tr1::shared_ptr<GLCrowClient> spCrow = m_pLandMClient->GetCrow(sTargetID.GaeaId);
		if (spCrow)
            return spCrow->IsCollisionVolume();
	}
	else if ( sTargetID.emCrow == CROW_MATERIAL ) 
	{
        std::tr1::shared_ptr<GLMaterialClient> spMaterial = m_pLandMClient->GetMaterial(sTargetID.GaeaId);
		if (spMaterial)
            return spMaterial->IsCollisionVolume();
	}
	else if ( sTargetID.emCrow == CROW_SUMMON )
	{
		std::tr1::shared_ptr<GLSummonClient> pSummon = m_pLandMClient->GetSummon ( sTargetID.GaeaId );
		if ( pSummon )
            return pSummon->IsCollisionVolume();

	}
	else
		GASSERT(0&&"emCrow가 잘못된 지정자 입니다." );

	return FALSE;
}

std::tr1::shared_ptr<GLCharClient> GLGaeaClient::GetChar(DWORD dwID)
{
	if (!m_pLandMClient)
        return std::tr1::shared_ptr<GLCharClient> ((GLCharClient*) 0);
    else
	    return m_pLandMClient->GetChar(dwID);
}

std::tr1::shared_ptr<GLCharClient> GLGaeaClient::GetCharByDBnum(DWORD dwDBnum)
{
	if (!m_pLandMClient)
		return std::tr1::shared_ptr<GLCharClient> ((GLCharClient*) 0);
	else
		return m_pLandMClient->FindCharByDBnum(dwDBnum);
}


std::tr1::shared_ptr<GLSummonClient> GLGaeaClient::GetSummon(DWORD dwID)
{
	if (!m_pLandMClient)
        return std::tr1::shared_ptr<GLSummonClient> ((GLSummonClient*) 0);
    else
	    return m_pLandMClient->GetSummon(dwID);
}

BOOL GLGaeaClient::ValidCheckTarget ( const STARGETID & sTargetID )
{
	if ( !m_pLandMClient )
        return FALSE;

	if ( sTargetID.GaeaId == EMTARGET_NULL )
        return FALSE;

	if ( sTargetID.emCrow == CROW_PC )
	{
		std::tr1::shared_ptr<GLCharClient> pChar = m_pLandMClient->GetChar ( sTargetID.GaeaId );
		if ( pChar )
            return TRUE;

		if ( GetCharacter()->GetGaeaID() == sTargetID.GaeaId )
			return TRUE;
	}
	else if ( sTargetID.emCrow == CROW_NPC || sTargetID.emCrow == CROW_MOB )
	{
        std::tr1::shared_ptr<GLCrowClient> spCrow = m_pLandMClient->GetCrow(sTargetID.GaeaId);
		if (spCrow)
            return TRUE;
	}
	else if ( sTargetID.emCrow == CROW_MATERIAL )
	{
		std::tr1::shared_ptr<GLMaterialClient> spMaterial = m_pLandMClient->GetMaterial ( sTargetID.GaeaId );
		if (spMaterial)
            return TRUE;
	}
	else if ( sTargetID.emCrow == CROW_SUMMON )
	{
		std::tr1::shared_ptr<GLSummonClient> pSummon = m_pLandMClient->GetSummon ( sTargetID.GaeaId );
		if ( pSummon )
            return TRUE;
	}
	else
		GASSERT(0&&"emCrow가 잘못된 지정자 입니다." );

	return FALSE;
}

D3DXVECTOR3 GLGaeaClient::GetTargetPos ( const STARGETID &sTargetID )
{
	if ( !m_pLandMClient )
        return VERROR();

	//	Note : 타겟의 위치 정보를 가져옴.
	if ( sTargetID.emCrow == CROW_PC )
	{
		std::tr1::shared_ptr<GLCharClient> pChar = m_pLandMClient->GetChar ( sTargetID.GaeaId );
		if ( pChar )
			return pChar->GetPosition() + D3DXVECTOR3(0.0f, pChar->GLCHARLOGIC::m_sAirborne.getHeight(), 0.0f);

		if ( GetCharacter()->GetGaeaID() == sTargetID.GaeaId )
			return GetCharacterPos() + D3DXVECTOR3(0.0f, GetCharacter()->GLCHARLOGIC::m_sAirborne.getHeight(), 0.0f);
	}
	else if ( sTargetID.emCrow == CROW_NPC || sTargetID.emCrow == CROW_MOB )
	{
        std::tr1::shared_ptr<GLCrowClient> spCrow = m_pLandMClient->GetCrow(sTargetID.GaeaId);
		if (spCrow)
            return spCrow->GetPosition() + D3DXVECTOR3(0.0f, spCrow->GLCROWLOGIC::m_sAirborne.getHeight() + spCrow->m_fInfluenceY, 0.0f);
	}
	else if ( sTargetID.emCrow == CROW_MATERIAL ) 
	{
		std::tr1::shared_ptr<GLMaterialClient> spMaterial = m_pLandMClient->GetMaterial(sTargetID.GaeaId);
		if (spMaterial)
            return spMaterial->GetPosition();
	}
	else if (sTargetID.emCrow == CROW_ITEM)
	{
        std::tr1::shared_ptr<CItemClientDrop> spItem = m_pLandMClient->GetItem(sTargetID.GaeaId);
		if (spItem)
            return spItem->vPos;
	}
	else if ( sTargetID.emCrow == CROW_MONEY )
	{
        std::tr1::shared_ptr<CMoneyClientDrop> spMoney = m_pLandMClient->GetMoney(sTargetID.GaeaId);
		if (spMoney)
            return spMoney->vPos;
	}
	else if ( sTargetID.emCrow == CROW_PET )
	{
		if ( GetPetClient()->m_dwGUID == sTargetID.GaeaId )
            return GetPetClient()->GetPosition ();
		else
		{
			std::tr1::shared_ptr<GLAnyPet> pAnyPet = m_pLandMClient->GetPet ( sTargetID.GaeaId );
			if ( pAnyPet )
                return pAnyPet->GetPosition ();
		}
	}
	else if ( sTargetID.emCrow == CROW_SUMMON )
	{
		std::tr1::shared_ptr<GLSummonClient> spSummonClient = m_pLandMClient->GetSummon(sTargetID.GaeaId);
		if (spSummonClient)
            return spSummonClient->GetPosition() + D3DXVECTOR3(0.0f, spSummonClient->GLSUMMON::m_sAirborne.getHeight(), 0.0f);

	}
	else
    {
        GASSERT(0&&"emCrow가 잘못된 지정자 입니다." );
    }

	return VERROR();
}

CSkillCAIdx* GLGaeaClient::GetTargetCA ( const STARGETID &sTargetID )
{
	if ( !m_pLandMClient )
		return NULL;

	//	Note : 타겟의 위치 정보를 가져옴.
	if ( sTargetID.emCrow == CROW_PC )
	{
		std::tr1::shared_ptr<GLCharClient> pChar = m_pLandMClient->GetChar ( sTargetID.GaeaId );
		if ( pChar )
			return pChar->GetSkillCA();

		if ( GetCharacter()->GetGaeaID() == sTargetID.GaeaId )
			return GetCharacter()->GetSkillCA();
	}
	else if ( sTargetID.emCrow == CROW_NPC || sTargetID.emCrow == CROW_MOB )
	{
		std::tr1::shared_ptr<GLCrowClient> spCrow = m_pLandMClient->GetCrow(sTargetID.GaeaId);
		if (spCrow)
			return spCrow->GetSkillCA();
	}
	else if ( sTargetID.emCrow == CROW_SUMMON )
	{
		std::tr1::shared_ptr<GLSummonClient> spSummonClient = m_pLandMClient->GetSummon(sTargetID.GaeaId);
		if (spSummonClient)
			return spSummonClient->GetSkillCA();

	}

	return NULL;
}


namespace
{
	void MINDETECTAABB ( D3DXVECTOR3 &vMax, D3DXVECTOR3 &vMin, const float fRate, const float fMinLeng )
	{
		float fLength(0);
		fLength = vMax.x - vMin.x;
		vMax.x += ( fLength*fRate - fLength ) / 2.0f;
		vMin.x -= ( fLength*fRate - fLength ) / 2.0f;

		fLength = vMax.x - vMin.x;
		if ( fLength < fMinLeng )
		{
			vMax.x += fMinLeng/2.0f;
			vMin.x -= fMinLeng/2.0f;
		}

		fLength = vMax.y - vMin.y;
		vMax.y += ( fLength*fRate - fLength ) / 2.0f;
		vMin.y -= ( fLength*fRate - fLength ) / 2.0f;

		fLength = vMax.y - vMin.y;
		if ( fLength < fMinLeng )
		{
			vMax.y += fMinLeng/2.0f;
			vMin.y -= fMinLeng/2.0f;
		}

		fLength = vMax.z - vMin.z;
		vMax.z += ( fLength*fRate - fLength ) / 2.0f;
		vMin.z -= ( fLength*fRate - fLength ) / 2.0f;

		fLength = vMax.z - vMin.z;
		if ( fLength < fMinLeng )
		{
			vMax.z += fMinLeng/2.0f;
			vMin.z -= fMinLeng/2.0f;
		}
	}
}

VEC_SK_TAR* GLGaeaClient::DetectCrowDie ( const D3DXVECTOR3 &vFromPt, const D3DXVECTOR3 &vTargetPt )
{
	STARGETID TargetID(CROW_MOB,EMTARGET_NULL);
	if (!m_pLandMClient)
        return NULL;

	// DetectID 클리어
    m_vecDetectID.clear();

	STARGETID findTargetID;

	// 몹
    m_pLandMClient->DetectCrowDie(m_vecDetectID, vFromPt, vTargetPt);

	// 캐릭터
    m_pLandMClient->GetCollisionLineToAabbChar(m_vecDetectID, vFromPt, vTargetPt, true, true, false, false, NULL, FALSE);

	if (m_vecDetectID.empty())
        return NULL;
	
	std::sort(m_vecDetectID.begin(), m_vecDetectID.end(), STAR_ORDER());
	return &m_vecDetectID;
}
/**
 * 실린더 충돌하는 객체검출.
 *
 * @param emCrow			[in] 검출 원하는 type flag.
 * @param vFromPt			[in] 실린더 시작점.
 * @param vTargetPt			[in] 실린더 끝점.
 * @param fRadius			[in] 실린더 반지름.
 * @return 검출한 객체가 있다면 아이디 std:vector 
 */
VEC_SK_TAR* GLGaeaClient::DetectCrowWithCylinder( DWORD emCrow, const D3DXVECTOR3 &vFromPt, const D3DXVECTOR3 &vTargetPt, float fRadius )
{
	if ( !m_pLandMClient )
		return NULL;

	D3DXVECTOR3		vLineDir = vTargetPt - vFromPt;
	D3DXVec3Normalize( &vLineDir, &vLineDir );
	D3DXVECTOR3		vP0 = vFromPt;
	D3DXVECTOR3		vP1	= vTargetPt + ( vLineDir * 5.0f );

	m_vecDetectID.clear();
	STARGETID		findTargetID;
	if (emCrow & CROW_EX_MOB) // 몹
		m_pLandMClient->DetectCrowWithCylinder(m_vecDetectID, vLineDir, vP0, vP1, fRadius);

	// Material
	if (emCrow & CROW_EX_MATERIAL)
	{
		m_pLandMClient->GetCollisionLineToSphereDropMaterial(
            m_vecDetectID,
            vLineDir,
            vP0,
            vP1,
            vFromPt,
            vTargetPt,
            fRadius);
	}

	//	캐릭터.
	if (emCrow & CROW_EX_PC)
		m_pLandMClient->GetCollisionLineToSphereChar(m_vecDetectID, vLineDir, vP0, vP1, vFromPt, vTargetPt, fRadius, true, true);

	//	소환수
	if (emCrow & CROW_EX_SUMMON)
		m_pLandMClient->GetCollisionLineToSphereDropSummon(m_vecDetectID, vLineDir, vP0, vP1, vFromPt, vTargetPt, fRadius, true);

	if( m_vecDetectID.empty() )
		return NULL;

	std::sort(m_vecDetectID.begin(), m_vecDetectID.end(), STAR_ORDER());

	return &m_vecDetectID;
}

VEC_SK_TAR* GLGaeaClient::DetectCrow ( DWORD emCrow, const D3DXVECTOR3 &vFromPt, const D3DXVECTOR3 &vTargetPt )
{
	STARGETID TargetID(CROW_MOB,EMTARGET_NULL);
	if (!m_pLandMClient)
        return NULL;

	//	DetectID 클리어.
	m_vecDetectID.clear();

	STARGETID findTargetID;
	
	// 몹
	if (emCrow & CROW_EX_MOB)
		m_pLandMClient->GetCollisionLineToAabbDropCrow(m_vecDetectID, vFromPt, vTargetPt, true, true, false, false, NULL, FALSE);

	// Material
	if (emCrow & CROW_EX_MATERIAL)
		m_pLandMClient->GetCollisionLineToAabbDropMaterial(m_vecDetectID, vFromPt, vTargetPt, false, NULL, FALSE);

	//	캐릭터.
	if (emCrow & CROW_EX_PC)
		m_pLandMClient->GetCollisionLineToAabbChar(m_vecDetectID, vFromPt, vTargetPt, false, true, true, false, NULL, FALSE);

	//	소환수
	if (emCrow & CROW_EX_SUMMON)
		m_pLandMClient->GetCollisionLineToAabbDropSummon(m_vecDetectID, vFromPt, vTargetPt, true, false, NULL, FALSE);

	//	Item.
	if (emCrow & CROW_EX_ITEM)
        m_pLandMClient->GetCollisionDropItem(vFromPt, vTargetPt, m_vecDetectID);

	//	Money.
	if (emCrow & CROW_EX_MONEY)
		m_pLandMClient->GetCollisionDropMoney(vFromPt, vTargetPt, m_vecDetectID);

	if (m_vecDetectID.empty())
        return NULL;

	std::sort(m_vecDetectID.begin(), m_vecDetectID.end(), STAR_ORDER());
	return &m_vecDetectID;
}

//! 죽은거 상관없이 전부 검사한다
VEC_SK_TAR* GLGaeaClient::DetectCrowAll ( DWORD emCrow, const D3DXVECTOR3 &vTarPos, const float fLength )
{
	STARGETID TargetID(CROW_MOB,EMTARGET_NULL);
	if (!m_pLandMClient)
        return NULL;

	// DetectID 클리어
	m_vecDetectID.clear();

	STARGETID findTargetID;

	//	몹.
	if (emCrow & CROW_EX_MOB)
		m_pLandMClient->GetSpherePointCollisionDropCrow(m_vecDetectID, vTarPos, fLength, false);

	// Material
	if (emCrow & CROW_EX_MATERIAL)
		m_pLandMClient->GetSpherePointCollisionDropMaterial(m_vecDetectID, vTarPos, fLength, false);

	//	Summon
	if (emCrow & CROW_EX_SUMMON)
		m_pLandMClient->GetSpherePointCollisionDropSummon(m_vecDetectID, vTarPos, fLength, false);

	//	캐릭터.
	if (emCrow & CROW_EX_PC)
		m_pLandMClient->GetSpherePointCollisionChar(m_vecDetectID, vTarPos, fLength, true, false);

	//	Item.
	if (emCrow & CROW_EX_ITEM)
        m_pLandMClient->GetSpherePointCollisionDropItem(vTarPos, fLength, m_vecDetectID);

	//	Money.
	if (emCrow & CROW_EX_MONEY)
		m_pLandMClient->GetSpherePointCollisionDropMoney(vTarPos, fLength, m_vecDetectID);

	if (m_vecDetectID.empty())
        return NULL;

	std::sort(m_vecDetectID.begin(), m_vecDetectID.end(), STAR_ORDER());
	return &m_vecDetectID;
}

VEC_SK_TAR* GLGaeaClient::DetectCrow ( DWORD emCrow, const D3DXVECTOR3 &vTarPos, const float fLength )
{
	STARGETID TargetID(CROW_MOB,EMTARGET_NULL);
	if (!m_pLandMClient)
        return NULL;

	// DetectID 클리어
    m_vecDetectID.clear();

	STARGETID findTargetID;

	//	몹.//
	if (emCrow & CROW_EX_MOB)
		m_pLandMClient->GetSpherePointCollisionDropCrow(m_vecDetectID, vTarPos, fLength, true);

    // Material
	if (emCrow & CROW_EX_MATERIAL)
		m_pLandMClient->GetSpherePointCollisionDropMaterial(m_vecDetectID, vTarPos, fLength, true);

	// Summon
	if (emCrow & CROW_EX_SUMMON)
		m_pLandMClient->GetSpherePointCollisionDropSummon(m_vecDetectID, vTarPos, fLength, true);

	//	캐릭터.
	if (emCrow & CROW_EX_PC)
		m_pLandMClient->GetSpherePointCollisionChar(m_vecDetectID, vTarPos, fLength, true, true);

	//	Item.
	if (emCrow & CROW_EX_ITEM)
        m_pLandMClient->GetSpherePointCollisionDropItem(vTarPos, fLength, m_vecDetectID);

	//	Money.
	if (emCrow & CROW_EX_MONEY)
		m_pLandMClient->GetSpherePointCollisionDropMoney(vTarPos, fLength, m_vecDetectID);

	if (m_vecDetectID.empty())
        return NULL;
	
	std::sort(m_vecDetectID.begin(), m_vecDetectID.end(), STAR_ORDER());
	return &m_vecDetectID;
}


VEC_SK_TAR* GLGaeaClient::DetectReactionableCrowDie ( const D3DXVECTOR3 &vFromPt, const D3DXVECTOR3 &vTargetPt, BOOL bEnemy )
{
	STARGETID TargetID(CROW_MOB,EMTARGET_NULL);
	if (!m_pLandMClient)
        return NULL;

	//	DetectID 클리어.
	m_vecDetectID.clear();

	STARGETID findTargetID;

	// 몹
    m_pLandMClient->GetCollisionLineToAabbDropCrow(
        m_vecDetectID,
        vFromPt,
        vTargetPt,
        false,
        true, true, true, m_pCharacter, bEnemy);
    
	//	캐릭터
    m_pLandMClient->GetCollisionLineToAabbChar(
        m_vecDetectID,
        vFromPt, 
        vTargetPt,
        true,
        true,
        false,
        true,
        m_pCharacter,
        bEnemy);

	if (m_vecDetectID.empty())
        return NULL;

	std::sort(m_vecDetectID.begin(), m_vecDetectID.end(), STAR_ORDER());
	return &m_vecDetectID;
}

VEC_SK_TAR* GLGaeaClient::DetectReactionableCrow ( DWORD emCrow, const D3DXVECTOR3 &vFromPt, const D3DXVECTOR3 &vTargetPt, BOOL bEnemy )
{
	STARGETID TargetID(CROW_MOB,EMTARGET_NULL);
	if (!m_pLandMClient)
        return NULL;

	//	DetectID 클리어.
	m_vecDetectID.clear();

	// 몹
	if (emCrow & CROW_EX_MOB)
	{
		/// 픽킹 검출; 은신 중이면 픽킹 검출 안되게;
		m_pLandMClient->GetCollisionLineToAabbDropCrow(m_vecDetectID, vFromPt, vTargetPt, true, true, false, true, m_pCharacter, bEnemy);		
	}

	// Material
	if (emCrow & CROW_EX_MATERIAL)
		m_pLandMClient->GetCollisionLineToAabbDropMaterial(m_vecDetectID, vFromPt, vTargetPt, true, m_pCharacter, bEnemy);

	//	캐릭터.
	if (emCrow & CROW_EX_PC)
		m_pLandMClient->GetCollisionLineToAabbChar(m_vecDetectID, vFromPt, vTargetPt, false, true, true, true, m_pCharacter, bEnemy);

	//	소환수
	if (emCrow & CROW_EX_SUMMON)
		m_pLandMClient->GetCollisionLineToAabbDropSummon(m_vecDetectID, vFromPt, vTargetPt, true, true, m_pCharacter, bEnemy);

	//	Item.
	if (emCrow & CROW_EX_ITEM)
        m_pLandMClient->GetCollisionDropItem(vFromPt, vTargetPt, m_vecDetectID);

	//	Money.
	if (emCrow & CROW_EX_MONEY)
		m_pLandMClient->GetCollisionDropMoney(vFromPt, vTargetPt, m_vecDetectID);

	if (m_vecDetectID.empty())
        return NULL;

	std::sort(m_vecDetectID.begin(), m_vecDetectID.end(), STAR_ORDER());
	return &m_vecDetectID;
}

BOOL GLGaeaClient::IsCollisionMobToPoint ( const D3DXVECTOR3 &vPoint, const WORD wBodyRadius )
{
    if (!m_pLandMClient)
        return FALSE;
    else
        return m_pLandMClient->GetCollisionMobToPoint(vPoint, wBodyRadius);
}

const bool GLGaeaClient::IsCollision(const D3DXVECTOR3& vPoint0, const D3DXVECTOR3& vPoint1, EMCOLLMODE emCullMode) const
{
	if ( m_pLandMClient == NULL )
		return false;
	else
		return m_pLandMClient->IsCollision(vPoint0, vPoint1, emCullMode);
}

HRESULT GLGaeaClient::InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	GASSERT(pd3dDevice);
	m_pd3dDevice = pd3dDevice;

	if ( m_pLandMClient )
	{
		m_pLandMClient->ActiveMap();
		m_pLandMClient->InitDeviceObjects ( m_pd3dDevice );
	}

	DxEffectMan::GetInstance().SetCrowTracer ( this );

	DxSoundLib::GetInstance()->CreateSound ( "GRINDING_SUCCEED", GLCONST_CHAR::strGRINDING_SUCCEED.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "GRINDING_FAIL", GLCONST_CHAR::strGRINDING_FAIL.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "GRINDING_RESET", GLCONST_CHAR::strGRINDING_RESET.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "GRINDING_BROKEN", GLCONST_CHAR::strGRINDING_BROKEN.c_str(), SFX_SOUND );

	DxSoundLib::GetInstance()->CreateSound ( "ITEMDROP_SUIT", GLCONST_CHAR::strITEMDROP_SUIT.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "ITEMDROP_WAPON", GLCONST_CHAR::strITEMDROP_WAPON.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "ITEMDROP_SHOES", GLCONST_CHAR::strITEMDROP_SHOES.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "ITEMDROP_RING", GLCONST_CHAR::strITEMDROP_RING.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "ITEMDROP_?BOX", GLCONST_CHAR::strITEMDROP_QBOX.c_str(), SFX_SOUND );

	DxSoundLib::GetInstance()->CreateSound ( "ITEMDROP_SCROLL", GLCONST_CHAR::strITEMDROP_SCROLL.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "ITEMDROP_COIN", GLCONST_CHAR::strITEMDROP_COIN.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "ITEMDROP_DRUGS", GLCONST_CHAR::strITEMDROP_DRUGS.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "PICKUP_ITEM", GLCONST_CHAR::strPICKUP_ITEM.c_str(), SFX_SOUND );
	
	DxSoundLib::GetInstance()->CreateSound ( "QITEM_FACT", GLCONST_CHAR::strQITEM_FACT.c_str(), SFX_SOUND );
	
	
	DxSoundLib::GetInstance()->CreateSound ( "GAMBLING_SHUFFLE", GLCONST_CHAR::strGAMBLING_SHUFFLE.c_str(), SFX_SOUND ); //Monster7j
	DxSoundLib::GetInstance()->CreateSound ( "GAMBLING_WIN", GLCONST_CHAR::strGAMBLING_WIN.c_str(), SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "GAMBLING_LOSE", GLCONST_CHAR::strGAMBLING_LOSE.c_str(), SFX_SOUND );

	DxSoundLib::GetInstance()->CreateSound ( "PKCOMBO_FIRST", "firstblood.wav", SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "PKCOMBO_DOUBLE", "doublekill.wav", SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "PKCOMBO_TRIPLE", "triplekill.wav", SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "PKCOMBO_ULTRA", "ultrakill.wav", SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "PKCOMBO_RAMPAGE", "rampage.wav", SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "PKCOMBO_KILLING_SPREE", "killing_spree.wav", SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "PKCOMBO_DOMINATING", "dominating.wav", SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "PKCOMBO_MEGA_KILL", "megakill.wav", SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "PKCOMBO_UNSTOPPABLE", "unstoppable.wav", SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "PKCOMBO_WICKED_SICK", "whickedsick.wav", SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "PKCOMBO_MONSTER_KILL", "monster_kill.wav", SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "PKCOMBO_GODLIKE", "godlike.wav", SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "PKCOMBO_HOLY_SHIT", "holyshit.wav", SFX_SOUND );
	DxSoundLib::GetInstance()->CreateSound ( "PKCOMBO_OWNAGE", "ownage.wav", SFX_SOUND );

	for ( int i = 0; i < EMITEM_LEVEL_NSIZE; i++ )
	{
		std::string strItemRank = sc::string::format("ITEMDROP_RANK%d", i);
		DxSoundLib::GetInstance()->CreateSound ( strItemRank, GLCONST_CHAR::strITEMDROP_RANK[i].c_str(), SFX_SOUND );
	}

    m_pSNSClient->InitDeviceObjects ();
    GLSNSClient::BEGINTHREAD ( m_pSNSClient );

	m_pCTFlagEffect->OnInit( m_pd3dDevice );

	m_pInterface->UiInitDeviceObjects ( m_pd3dDevice );

	return S_OK;
}

HRESULT GLGaeaClient::RestoreDeviceObjects ()
{
	GASSERT(m_pd3dDevice&&"m_pd3dDevice 가 초기화 되지 않았습니다.");

	if ( m_pLandMClient )
	{
		m_pLandMClient->RestoreDeviceObjects ();
	}

	m_pCharacter->RestoreDeviceObjects ( m_pd3dDevice );
	m_pPet->RestoreDeviceObjects ( m_pd3dDevice );

	//GfxInterface::GetInstance()->GetOldInterface()->UiRestoreDeviceObjects(m_pd3dDevice);

	m_pInterface->UiRestoreDeviceObjects ( m_pd3dDevice );

	return S_OK;
}

HRESULT GLGaeaClient::InvalidateDeviceObjects ()
{
	if ( m_pLandMClient )
	{
		m_pLandMClient->InvalidateDeviceObjects ();
	}

	m_pCharacter->InvalidateDeviceObjects ();
	m_pPet->InvalidateDeviceObjects ();

	//GfxInterface::GetInstance()->GetOldInterface()->UiInvalidateDeviceObjects();
	if ( m_pInterface )
	m_pInterface->UiInvalidateDeviceObjects ();

	return S_OK;
}

HRESULT GLGaeaClient::DeleteDeviceObjects ()
{
	m_pCharacter->DeleteDeviceObjects();
	m_pPet->DeleteDeviceObjects ();
    m_pSNSClient->DeleteDeviceObjects();

	// 맵을 나가는 것이니 맵 참조값을 줄이는 방향으로 간다.
	m_pPet->LandShredPtrRefSub();
	m_pCharacter->LandShredPtrRefSub();

	m_pPet->DeleteSkin();
	m_pCharacter->DeleteSkin();

	if ( m_pLandMClient )
	{
		m_pLandMClient->DeleteDeviceObjects ();
		SAFE_DELETE(m_pLandMClient);
	}

	if ( m_pFriendClient )
		m_pFriendClient->Clear();

	if ( m_pInterface )
	m_pInterface->UiDeleteDeviceObjects ();

	return S_OK;
}

std::tr1::shared_ptr<GLCrowClient> GLGaeaClient::GETQUSETNPC(const GLQUEST_STEP& step)
{
	if (!step.IsNEED_NPCTALK())
        return std::tr1::shared_ptr<GLCrowClient> ((GLCrowClient*) 0);

	GLLandManClient* pLand = GetActiveMap();
	if (pLand)
        return pLand->GetQuestNpc(step);
    else
        return std::tr1::shared_ptr<GLCrowClient> ((GLCrowClient*) 0);
}

std::vector<DWORD> GLGaeaClient::CHECKQUESTNPCSTATE(std::tr1::shared_ptr<GLCrowClient> spCrow, const GLQUEST_STEP *pStep /* = NULL */, const GLQUESTPROG* pProg, const INT iStepNumber )
{

	std::vector<DWORD> vecQuestID;
	vecQuestID.clear();

	if( pStep && !pStep->IsNEED_NPCTALK() )
        return vecQuestID;

	GLQuestPlay::MAPQUEST& mapPROC = m_pCharacter->m_cQuestPlay.GetQuestProc();
	GLQuestPlay::MAPQUEST_ITER iter = mapPROC.begin();
	GLQuestPlay::MAPQUEST_ITER iter_end = mapPROC.end();
	GLQUESTPROG* pCHECKPROG = NULL;
	for ( ; iter!=iter_end; ++iter )
	{
		pCHECKPROG = (*iter).second;

		GLQUEST *pCEHCKQUEST = GLQuestMan::GetInstance().Find ( pCHECKPROG->m_sNID.dwID );
		if ( !pCEHCKQUEST )
            continue;

		GLQUEST_STEP* pCHECKSTEP = pCEHCKQUEST->GetSTEP ( pCHECKPROG->m_dwSTEP );
		if ( !pCHECKSTEP )
            continue;
		if ( pStep )
		{
			if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
			{
				CString strKey;
				strKey.Format("QN_%03d_%03d_P_%d", pProg->m_sNID.wMainID, pProg->m_sNID.wSubID, iStepNumber);
				if ( strcmp( pCHECKSTEP->GetTITLE(), CGameTextMan::GetInstance().GetQuestText(strKey.GetString()).GetString() ) == 0 
					&& pCHECKSTEP->m_dwNID_NPCTALK == pStep->m_dwNID_NPCTALK )//strcmp( pCHECKSTEP->GetOBJ_NPCTALK(), pStep->GetOBJ_NPCTALK() ) == 0 )
					continue;
			}
			else
			{
				if ( strcmp( pCHECKSTEP->GetTITLE(), pStep->GetTITLE() ) == 0 
					&& pCHECKSTEP->m_dwNID_NPCTALK == pStep->m_dwNID_NPCTALK )//strcmp( pCHECKSTEP->GetOBJ_NPCTALK(), pStep->GetOBJ_NPCTALK() ) == 0 )
					continue;
			}
		}
		if ( !pCHECKSTEP->IsNEED_NPCTALK() )
            continue;

		if( pCHECKSTEP->m_dwNID_NPCTALK == spCrow->m_sNativeID.dwID )
		{
			vecQuestID.push_back( pCEHCKQUEST->m_sNID.dwID );
		}
	}

	return vecQuestID;
}


void GLGaeaClient::AllCharClientUpdateSuit()
{
	m_pLandMClient->AllCharClientUpdateSuit();
}


HRESULT GLGaeaClient::FrameMove(float fTime, float fElapsedTime, const CLIPVOLUME &sCV, BOOL bNotRendering )
{
	m_fAge += fElapsedTime;
	m_fElapsedTime = fElapsedTime;

    //	Note : 게이트 검색후 게이트 상태 업대이트.
    //
	SetGateOpen(FALSE);
	GetInterface()->SetGateOpen( FALSE );
	
	GLMacroManager::GetInstance().FrameMove(fTime, fElapsedTime);
//	if ( GLInputManager::GetInstance().GetCurrentControlType() == EM_CONTROLTYPE_FLYCAMERA )
	GLFlyCameraControl::GetInstance().FrameMove(fTime, fElapsedTime);
	if ( GLInputManager::GetInstance().GetCurrentControlType() != EM_CONTROLTYPE_FLYCAMERA 
		&& m_bUseCustomChaseCamera == true )
	{
		const DWORD dwChaseTarget = m_sChaseCameraTarget.GaeaId;
		D3DXVECTOR3 vCameraPos(0.f, 0.f, 0.f);
		if ( m_pCharacter && m_pCharacter->GetGaeaID() == dwChaseTarget )
		{
			vCameraPos = m_pCharacter->GetPosition();
			vCameraPos.y += 10.0f;
			DxViewPort::GetInstance().CameraJump(vCameraPos);
		}
		else
		{
			PLANDMANCLIENT pLand = this->GetActiveMap();
			if ( pLand && dwChaseTarget != EMTARGET_NULL && m_sChaseCameraTarget.emCrow != CROW_NULL )
			{
				switch (m_sChaseCameraTarget.emCrow)
				{
				case CROW_PC :		
					{
						std::tr1::shared_ptr<GLCharClient> spChar = pLand->GetChar(dwChaseTarget);
						if ( spChar )
							vCameraPos = spChar->GetPosition(); 
					}
					break;
				case CROW_NPC :		
				case CROW_MOB :		
					{
						std::tr1::shared_ptr<GLCrowClient> spCrow = pLand->GetCrow(dwChaseTarget);
						if ( spCrow )
							vCameraPos = spCrow->GetPosition(); 
					}
					break;
				case CROW_ITEM :	
					{
						std::tr1::shared_ptr<CItemClientDrop> spItemDrop = pLand->GetItem(dwChaseTarget);
						if ( spItemDrop )
							vCameraPos = spItemDrop->vPos; 
					}
					break;
				case CROW_MONEY :	
					{
						std::tr1::shared_ptr<CMoneyClientDrop> spMoneyDrop = pLand->GetMoney(dwChaseTarget);
						if ( spMoneyDrop )
							vCameraPos = spMoneyDrop->vPos; 
					}
					break;
				case CROW_PET :		
					{
						std::tr1::shared_ptr<GLAnyPet> spPet = pLand->GetPet(dwChaseTarget);
						if ( spPet )
							vCameraPos = spPet->GetPosition();
					}
					break;
				case CROW_SUMMON :	
					{
						std::tr1::shared_ptr<GLSummonClient> spSummon = pLand->GetSummon(dwChaseTarget);
						if ( spSummon )
							vCameraPos = spSummon->GetPosition();
					}
					break;
				}
				vCameraPos.y += 10.f;
				DxViewPort::GetInstance().CameraJump(vCameraPos);
			}
		}
	}
	
	/*	
	else
	{
		if(m_bUseCustomChaseCamera == false)
		{
			//	Note : 케릭터 이동에 따른 카메라 제어.
			D3DXVECTOR3 vCamera = m_pCharacter->GetPosition();
			vCamera.y += 10.0f;	
			DxViewPort::GetInstance().CameraJump ( vCamera );
		}
		else
		{
			if( m_pCharacter && m_pCharacter->GetGaeaID() == m_dwChaseCameraTarget)
			{
				m_bUseCustomChaseCamera = true;
				D3DXVECTOR3 vCamera = m_pCharacter->GetPosition();
				vCamera.y += 10.0f;	
				DxViewPort::GetInstance().CameraJump ( vCamera );
			}
			else
			{
				std::tr1::shared_ptr<GLCharClient> _client =  GetChar(m_dwChaseCameraTarget);

				if(_client == NULL)
				{
					m_bUseCustomChaseCamera = true;

				}
				else
				{
					D3DXVECTOR3 vCamera = _client->GetPosition();
					vCamera.y += 10.0f;	
					DxViewPort::GetInstance().CameraJump ( vCamera );
				}
			}
		}
	}
	*/

	//////////////////////////////////////////////////////////////////////////
	// [shhan][2014.11.06]  
	if ( m_pLandMClient )
	{
		m_pLandMClient->RegistName( sCV );
	}

	//////////////////////////////////////////////////////////////////////////
	// [shhan][2014.08.08] 캐릭터 애니메이션 Thread 작업이 되면서 캐릭터가 중점에 있지 않는 현상이 생겨서
	//						카메라 위치를 정한 후( 캐릭터의 이전Frame 을 기준으로 잡게됨) 캐릭터의 이동을 시켜주도록 하자.
	// 캐릭터 이동.
	PROFILE_BEGIN("GLCharacter::FrameMove");
	m_pCharacter->FrameMove ( fTime, fElapsedTime );
	PROFILE_END("GLCharacter::FrameMove");

	CheckGateMove(); // 게이트 체크

	if ( m_pLandMClient )
	{
		PROFILE_BEGIN("GLLandManClient::FrameMove");
		m_pLandMClient->FrameMove(fTime, fElapsedTime, sCV, bNotRendering );
		PROFILE_END("GLLandManClient::FrameMove");
	}

    ScriptFrameMove(fTime, fElapsedTime);

	if ( m_pMiniGameManager )
	{
		PROFILE_BEGIN("m_pMiniGameManager::FrameMove");
		m_pMiniGameManager->FrameMove(fTime, fElapsedTime);
		PROFILE_END("m_pMiniGameManager::FrameMove");
	}

	if ( m_pEventManager )
	{
		PROFILE_BEGIN("m_pEventManager::FrameMove");
		m_pEventManager->frameMove(fTime, fElapsedTime);
		PROFILE_END("m_pEventManager::FrameMove");
	}

	if ( m_pPingManager )
	{
		PROFILE_BEGIN("m_pPingManager::FrameMove");
		m_pPingManager->_frameMove(fTime, fElapsedTime);
		PROFILE_END("m_pPingManager::FrameMove");
	}

	// PET
	PROFILE_BEGIN("GLPetClient::FrameMove");
	if (m_pPet->IsVALID())
        m_pPet->FrameMove ( GetLandManClient(), fTime, fElapsedTime );
	PROFILE_END("GLPetClient::FrameMove");

	// 선도클럽 결정전 시간 갱신
	if ( m_bCLUBBATTLE )
	{
        for( DWORD i = 0; i <m_vecClubGuidBattleTimer.size(); ++i )
        {
		    m_vecClubGuidBattleTimer[i] -= fElapsedTime;    //선도전 종료시간 갱신
            if( m_vecClubGuidBattleTimer[i] < 0.0f )
            {
                m_vecClubGuidBattleTimer[i] = 0.0f;
            }
        }
        if (m_fAge > 1.0f)  //1초 경과시
        {
			//!! SF_TODO
            GetInterface()->UpdateClubGuidBattleTime( m_vecClubGuidBattleTimer );    //UI 업데이트
        }
	}

	if ( m_bClubDeathMatch )
	{
		m_fClubDeathMatchTimer -= fElapsedTime; //데스메치 종료시간 갱신
		if( m_fClubDeathMatchTimer < 0.0f )
		{
			m_fClubDeathMatchTimer = 0.0f;
		}
		//!! SF_TODO
	    if (m_fAge > 1.0f)
            GetInterface()->UpdateClubDeathMatchTime( m_fClubDeathMatchTimer );   //UI 업데이트
	}

	m_pReferChar->Update( fElapsedTime );
	m_pPartyTenderManagerClient->FrameMove(fTime, fElapsedTime);

    if (m_pPVEClient)
    {
        m_pPVEClient->FrameMove(fElapsedTime);        
    }

	if ( m_pTexasHoldemMgr )
	{
		m_pTexasHoldemMgr->FrameMove( fElapsedTime );
	}

	if(m_pTournament)
	{
		m_pTournament->FrameMove(fElapsedTime);
	}

	// 서버 현재시간
	if ( m_fAge > 1.0f )
	{
		CTimeSpan cElapsedTime(0,0,0,1);
		m_cServerTime += cElapsedTime;

		m_fAge = m_fAge - 1.0f;
	}

	//PK Notification - Arvin.BC
	for( int i = 0; i < (int)m_vecPKHistory.size(); ++ i )
	{
		m_vecPKHistory[i].fTimer += fElapsedTime;

		if ( m_vecPKHistory[i].fTimer >= HISTORY_TIMER )
		{
			m_vecPKHistory.erase( m_vecPKHistory.begin() + i );
		}
	}

	// Lotto System;
	m_pLottoSystemMan->Update( fElapsedTime, m_cServerTime );

	// Instance Client;
	if ( m_pInstanceManager )
		m_pInstanceManager->OnFrameMove( fElapsedTime, m_cServerTime );


    // 클럽 이름 요청 타이머
    m_fClubNameTimer += fElapsedTime;
    if( m_fClubNameTimer < 15 )
    {
        ClubNameReq();
        m_fClubNameTimer = 0;
    }
    
	m_pCTFlagEffect->OnFrameMove( fElapsedTime );


	return S_OK;
}

HRESULT GLGaeaClient::Render ( CLIPVOLUME &CV, LPDIRECT3DSURFACEQ pColorBuffer )
{
	GASSERT(m_pd3dDevice);

	//if ( DxSurfaceTex::GetInstance().m_pColorSuf_1st && RENDERPARAM::g_emGroundQulity >= TnL_PS_3_0_DF && GetUsedMaterialSystem() )
	//{
	//	// Note : 
	//	D3DSURFACE_DESC	sDesc;
	//	LPDIRECT3DSURFACEQ pBackBuffer;
	//	m_pd3dDevice->GetBackBuffer ( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
	//	pBackBuffer->GetDesc ( &sDesc );
	//	pBackBuffer->Release();

	//	m_pd3dDevice->SetRenderTarget( 0, DxSurfaceTex::GetInstance().m_pColorBuffer );
	//}

	//if ( DxSurfaceTex::GetInstance().m_pColorSuf_1st && RENDERPARAM::g_emGroundQulity >= TnL_PS_3_0_NM && GetUsedMaterialSystem() )
	//{
	//	m_pLandMClient->Render_2Pass ( CV );
	//}
	
	float fFogStart(1.f);
	float fFogEnd(2.f);
	float fBias = -0.001f;
	DWORD dwBiasPrev(0);
	DWORD dwFogStartPrev(0);
	DWORD dwFogEndPrev(0);
	DWORD dwFogColor(0L);
	if ( g_bWIREFRAME_VIEW )
	{
		m_pd3dDevice->GetRenderState ( D3DRS_DEPTHBIAS,	&dwBiasPrev );
		m_pd3dDevice->GetRenderState ( D3DRS_FOGSTART,	&dwFogStartPrev );
		m_pd3dDevice->GetRenderState ( D3DRS_FOGEND,	&dwFogEndPrev );
		m_pd3dDevice->GetRenderState ( D3DRS_FOGCOLOR,	&dwFogColor );
		m_pd3dDevice->SetRenderState ( D3DRS_DEPTHBIAS,	*((DWORD*)&fBias) );
		m_pd3dDevice->SetRenderState ( D3DRS_FOGSTART,	*((DWORD *)(&fFogStart)) );
		m_pd3dDevice->SetRenderState ( D3DRS_FOGEND,	*((DWORD *)(&fFogEnd)) );
		m_pd3dDevice->SetRenderState ( D3DRS_FOGCOLOR,	0xffff00ff );

        NSMaterialManager::SetFogColorForEditor( 0xffff00ff );
	}



	//////////////////////////////////////////////////////////////////////////
	//							반사
	// Mirror 를 쓰는 반사라면 View, Projetion 쪽에 변화를 준다.
	if ( DxEnvironment::GetInstance().IsRecentReflect() )
	{
		float fWaterHeight(0.f);
		if ( m_pLandMClient->GetUsedMaterialSystem() )
		{
			fWaterHeight = m_pLandMClient->GetLandMan()->GetRealTimeWaterHeight();
		}
		else
		{
			fWaterHeight = DxEnvironment::GetInstance().GetCenter().y;
		}

		if ( fWaterHeight != FLT_MAX )
		{
			D3DXMATRIX matReflect, matReflectView;
			D3DXMatrixIdentity( &matReflect );
			matReflect._42 = -(fWaterHeight*2.f);
			D3DXMatrixMultiply_MIRROR( matReflect );
			D3DXMatrixMultiply( &matReflectView, &matReflect, &DxViewPort::GetInstance().GetMatView() );

			NSMaterialSkinManager::FrameMove( matReflectView, DxViewPort::GetInstance().GetMatProj() );

			m_pCharacter->RenderReflect( m_pd3dDevice, fWaterHeight );
			m_pPet->RenderReflect ( m_pd3dDevice, fWaterHeight );

			// 기본으로 되돌린다.
			NSMaterialSkinManager::FrameMove( DxViewPort::GetInstance().GetMatView(), DxViewPort::GetInstance().GetMatProj() );
		}
	}
	else
	{
		NSMaterialSkinManager::FrameMove( DxEnvironment::GetInstance().GetMatrix_ReflectView(), DxViewPort::GetInstance().GetMatProj() );

		m_pCharacter->RenderReflect( m_pd3dDevice, 0.f );
		m_pPet->RenderReflect ( m_pd3dDevice, 0.f );

		// 기본으로 되돌린다.
		NSMaterialSkinManager::FrameMove( DxViewPort::GetInstance().GetMatView(), DxViewPort::GetInstance().GetMatProj() );
	}

	// 이전맵일 경우 m_bReflectPlay 값에 초기화를 해준다.
	// MaterialSystem맵일 경우 m_bReflectPlay 값을 항시 TRUE 로 준다.
	DxEnvironment::GetInstance().EndReflection( m_pLandMClient->GetUsedMaterialSystem() );

	





	//////////////////////////////////////////////////////////////////////////
	//							그림자

	// Note : 그림자 Clear
	PROFILE_BEGIN2("DxShadowMap::Render");
	DxShadowMap::GetInstance().ClearShadow( m_pd3dDevice, m_pLandMClient->GetUsedMaterialSystem() );
	PROFILE_END2("DxShadowMap::Render");

	// 지형 그림자를 생성하도록 한다. 
	// PS_3_0 이상일 경우만 동작한다.
	if ( m_pLandMClient && RENDERPARAM::g_emGroundQulity >= TnL_PS_3_0_CSM )
	{
		// 그림자 렌더전에 NSShadowLight::FrameMove 를 호출해줘야한다.
		//NSShadowLight::FrameMove( m_fElapsedTime );

		m_pLandMClient->RenderShadow_SpotLight1( m_pd3dDevice );
		m_pLandMClient->RenderShadow_SpotLight2( m_pd3dDevice );
	}

	//////////////////////////////////////////////////////////////////////////
	// Shadow
	PROFILE_BEGIN2("m_Character::RenderShadow");
	m_pCharacter->RenderShadow( m_pd3dDevice, FALSE );
	PROFILE_END2("m_Character::RenderShadow");

	PROFILE_BEGIN2("m_Pet::RenderShadow");
	m_pPet->RenderShadow ( m_pd3dDevice, FALSE );
	PROFILE_END2("m_Pet::RenderShadow");

	PROFILE_BEGIN2("m_pLandMClient::char,crow,item");
	if ( m_pLandMClient )
	{
		m_pLandMClient->Render_MobItem_Shadow( CV, FALSE );
	}
	PROFILE_END2("m_pLandMClient::char,crow,item");

	if ( m_pLandMClient )
	{
		m_pLandMClient->RenderShadow();
	}


	if ( m_pLandMClient && RENDERPARAM::g_emGroundQulity >= TnL_PS_3_0_CSM )
	{
		m_pCharacter->RenderShadow_SpotLight( m_pd3dDevice, m_pLandMClient->GetUsedMaterialSystem(), 0 );
		m_pCharacter->RenderShadow_SpotLight( m_pd3dDevice, m_pLandMClient->GetUsedMaterialSystem(), 1 );

		m_pPet->RenderShadow_SpotLight( m_pd3dDevice, m_pLandMClient->GetUsedMaterialSystem(), 0 );
		m_pPet->RenderShadow_SpotLight( m_pd3dDevice, m_pLandMClient->GetUsedMaterialSystem(), 1 );
	}







	//////////////////////////////////////////////////////////////////////////
	//	Note : 지형 랜더링..
	PROFILE_BEGIN2("m_pLandMClient::Render");
	if ( m_pLandMClient )
	{
		m_pLandMClient->Render ( CV, pColorBuffer );
	}
	PROFILE_END2("m_pLandMClient::Render");





	//////////////////////////////////////////////////////////////////////////
	// Render(Opaque,HardAlpha)
	PROFILE_BEGIN2("m_Character::Render");
	m_pCharacter->Render( m_pd3dDevice, m_pLandMClient ? m_pLandMClient->GetLandMan() : NULL );
	PROFILE_END2("m_Character::Render");

	// PET
	PROFILE_BEGIN2("m_Pet::Render");
	m_pPet->Render ( m_pd3dDevice );
	PROFILE_END2("m_Pet::Render");


	PROFILE_BEGIN2("m_pLandMClient::char,crow,item");
	if ( m_pLandMClient )
	{
        m_pLandMClient->Render_MobItem( CV );
	}
	PROFILE_END2("m_pLandMClient::char,crow,item");

	if ( g_bWIREFRAME_VIEW )
	{
		m_pd3dDevice->SetRenderState ( D3DRS_DEPTHBIAS,	dwBiasPrev );
		m_pd3dDevice->SetRenderState ( D3DRS_FOGSTART,	dwFogStartPrev );
		m_pd3dDevice->SetRenderState ( D3DRS_FOGEND,	dwFogEndPrev );
		m_pd3dDevice->SetRenderState ( D3DRS_FOGCOLOR,	dwFogColor );

        NSMaterialManager::SetFogColorForEditor( dwFogColor );
	}

	m_pCTFlagEffect->OnFrameRedner();

	return S_OK;
}

HRESULT GLGaeaClient::RenderSoftAlpha ( CLIPVOLUME &CV, LPDIRECT3DSURFACEQ pColorBuffer )
{
	GASSERT(m_pd3dDevice);


	//////////////////////////////////////////////////////////////////////////
	// Shadow
	// 구맵일 경우 Shadow 를 구한다.
	if ( !GetUsedMaterialSystem() )
	{
		PROFILE_BEGIN2("m_Character::RenderShadow");
		m_pCharacter->RenderShadow( m_pd3dDevice, TRUE );
		PROFILE_END2("m_Character::RenderShadow");

		PROFILE_BEGIN2("m_Pet::RenderShadow");
		m_pPet->RenderShadow ( m_pd3dDevice, TRUE );
		PROFILE_END2("m_Pet::RenderShadow");

		PROFILE_BEGIN2("m_pLandMClient::char,crow,item");
		if ( m_pLandMClient )
		{
			m_pLandMClient->Render_MobItem_Shadow( CV, TRUE );
		}
		PROFILE_END2("m_pLandMClient::char,crow,item");


		// Note : ShadowMap ImageBlur
		PROFILE_BEGIN2("m_pLandMClient::LastImageBlur");
		DxShadowMap::GetInstance().LastImageBlur( m_pd3dDevice, m_pLandMClient->GetUsedMaterialSystem() );
		PROFILE_END2("m_pLandMClient::LastImageBlur");
	}


	// 지형.
	if ( m_pLandMClient )
	{
		m_pLandMClient->Render_AlphaObejct( CV, pColorBuffer );
	}

	//////////////////////////////////////////////////////////////////////////
	//						캐릭터의 반투명 처리
	DWORD dwAlphaBlend(0L);
	DWORD dwSrcBlend(0L);
	DWORD dwDestBlend(0L);
	//DWORD dwFogEnable(0L);
	m_pd3dDevice->GetRenderState ( D3DRS_ALPHABLENDENABLE,	&dwAlphaBlend );
	m_pd3dDevice->GetRenderState ( D3DRS_SRCBLEND,			&dwSrcBlend );
	m_pd3dDevice->GetRenderState ( D3DRS_DESTBLEND,			&dwDestBlend );
	//m_pd3dDevice->GetRenderState ( D3DRS_FOGENABLE,			&dwFogEnable );
	m_pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE,	TRUE );
	//m_pd3dDevice->SetRenderState ( D3DRS_FOGENABLE,			TRUE );

	// 캐릭터 반투명 부위는 여기서 처리한다.
	if ( m_pLandMClient )
	{
		m_pLandMClient->Render_MobItem_SoftAlpha( CV );	// 내부에서 D3DRS_SRCBLEND, D3DRS_DESTBLEND 값을 셋팅한다.
	}
	m_pPet->RenderSoftAlpha( m_pd3dDevice );		// 내부에서 D3DRS_SRCBLEND, D3DRS_DESTBLEND 값을 셋팅한다.
	m_pCharacter->RenderSoftAlpha( m_pd3dDevice );	// 내부에서 D3DRS_SRCBLEND, D3DRS_DESTBLEND 값을 셋팅한다.

	m_pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE,	dwAlphaBlend );
	m_pd3dDevice->SetRenderState ( D3DRS_SRCBLEND,			dwSrcBlend );
	m_pd3dDevice->SetRenderState ( D3DRS_DESTBLEND,			dwDestBlend );
	//m_pd3dDevice->SetRenderState ( D3DRS_FOGENABLE,			dwFogEnable );

	return S_OK;
}

HRESULT GLGaeaClient::RenderSingleEff ( CLIPVOLUME &CV )
{
	//////////////////////////////////////////////////////////////////////////
	//						캐릭터의 반투명 처리
	DWORD dwAlphaBlend(0L);
	DWORD dwSrcBlend(0L);
	DWORD dwDestBlend(0L);
	//DWORD dwFogEnable(0L);
	m_pd3dDevice->GetRenderState ( D3DRS_ALPHABLENDENABLE,	&dwAlphaBlend );
	m_pd3dDevice->GetRenderState ( D3DRS_SRCBLEND,			&dwSrcBlend );
	m_pd3dDevice->GetRenderState ( D3DRS_DESTBLEND,			&dwDestBlend );
	//m_pd3dDevice->GetRenderState ( D3DRS_FOGENABLE,			&dwFogEnable );
	m_pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE,	TRUE );
	//m_pd3dDevice->SetRenderState ( D3DRS_FOGENABLE,			TRUE );

	// 캐릭터 효과들 렌더링 부위는 여기서 처리한다.
	if ( m_pLandMClient )
	{
		m_pLandMClient->Render_MobItem_MaterialEffect( CV );	// 내부에서 D3DRS_SRCBLEND, D3DRS_DESTBLEND 값을 셋팅한다.
	}
	m_pPet->RenderMaterialSkinEffect( m_pd3dDevice );		// 내부에서 D3DRS_SRCBLEND, D3DRS_DESTBLEND 값을 셋팅한다.
	m_pCharacter->RenderMaterialSkinEffect( m_pd3dDevice );	// 내부에서 D3DRS_SRCBLEND, D3DRS_DESTBLEND 값을 셋팅한다.

	m_pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE,	dwAlphaBlend );
	m_pd3dDevice->SetRenderState ( D3DRS_SRCBLEND,			dwSrcBlend );
	m_pd3dDevice->SetRenderState ( D3DRS_DESTBLEND,			dwDestBlend );
	//m_pd3dDevice->SetRenderState ( D3DRS_FOGENABLE,			dwFogEnable );

	if ( m_pLandMClient )
	{
		m_pLandMClient->Render_SingleEffect( CV );
	}

	//////////////////////////////////////////////////////////////////////////
	//	Note : 하늘 렌더링..!!
	DxSkyMan::GetInstance().Render_AFTER( m_pd3dDevice, DxViewPort::GetInstance().GetFOV(), DxViewPort::GetInstance().GetAspect() );

	return S_OK;
}

//void GLGaeaClient::RenderPickAlpha()
//{
//	if ( m_pLandMClient )
//	{
//		m_pLandMClient->RenderPickAlpha();
//	}
//}

HRESULT GLGaeaClient::RenderStarGlow()
{
	if(m_pLandMClient)
		m_pLandMClient->RenderStarGlow();
	return S_OK;
}

HRESULT GLGaeaClient::Render_LightShafts()
{
	if(m_pLandMClient)
		m_pLandMClient->Render_LightShafts();
	return S_OK;
}

void GLGaeaClient::ProcessPingTraceMsg(NET_MSG_GENERIC* nmg)
{	
	switch (nmg->nType)
	{		
	case NET_MSG_GM_PINGTRACE_ON_FB:
		{
			PrintConsoleText("PingTrace: ON");
			break;
		}

	case NET_MSG_GM_PINGTRACE_OFF_FB:
		{		
			PrintConsoleText("PingTrace: OFF");
			break;
		}		
	}
}


void GLGaeaClient::ProcessPingMsg(NET_MSG_GENERIC* nmg)
{
	GLMSG::SNET_GM_PING *pMsgPing = (GLMSG::SNET_GM_PING *)nmg;

	//
	//mjeon
	//process the PingRequest from server	(NET USED YET!!)
	//
	switch (pMsgPing->nType)
	{
	case NET_MSG_GM_PING:
		{
			GLMSG::SNET_GM_PING MsgPingFB;
			MsgPingFB.nType = NET_MSG_GM_PING_FB;

			MsgPingFB.dwSent = pMsgPing->dwSent;
			MsgPingFB.dwRecvd = ::GetTickCount();
			MsgPingFB.dwGaeaID = GETMYGAEAID();
			MsgPingFB.bServerType = pMsgPing->bServerType;

			if (pMsgPing->bServerType == 0) //AgentServer
				NETSENDTOAGENT(&MsgPingFB);
			else
				NETSENDTOFIELD(&MsgPingFB);

			break;
		}

		//
		//mjeon
		//process the PingResponse from server
		//
	case NET_MSG_GM_PING_FB:
		{
			DWORD dwArrvd = ::GetTickCount();			

			if (pMsgPing->bServerType == 0)	//AgentServer
			{
				//
				//mjeon
				//RTT value is saved in member variable because it might be used for comparing of 
				//previous RTT and current RTT value.
				//
				m_dwRTTAgent =  dwArrvd - pMsgPing->dwSent;                
                PrintConsoleText(sc::string::format("Ping to AgentServer : %d ms", m_dwRTTAgent));
			}
			else							//FieldServer
			{
				m_dwRTTField =  dwArrvd - pMsgPing->dwSent;                
                PrintConsoleText(sc::string::format("Ping to FieldServer : %d ms", m_dwRTTField));
			}

			//
			//mjeon
			//
			//In case of VirtualClient, only one of the many clients sends ping 
			//and main class should be able to get this value.
			//Therefore, a global variable is used.
			//Ran-client does not need to store RTT to the global variable, each client will handle the RTT.
			//

			//DWORD dwPingTime = dwArrvd - pMsgPing->dwRecvd;	// 이 값은 필요할 경우만 사용하면 된다.
			//DWORD dwPongTime = m_dwRTTAgent - dwPingTime;	// 이 값은 필요할 경우만 사용하면 된다.

			break;
		}
	}
}


void GLGaeaClient::ProcessAttendanceMaintenanceFB(NET_MSG_GENERIC* nmg)
{
	GLMSG::SNET_ATTENDANCE_MAINTENANCE_MANUAL_START_CA_FB *pmsg = (GLMSG::SNET_ATTENDANCE_MAINTENANCE_MANUAL_START_CA_FB *)nmg;
	
	if (pmsg->bDone)
	{
		PrintConsoleText("AttendanceMaintenance request has been queued.");
	}
	else
	{
		PrintConsoleText("AttendanceMaintenance request ignored. Refer to server log for the reason.");
	}
}


void GLGaeaClient::ProcessCTFMoveStateByGMFB(NET_MSG_GENERIC* nmg)
{
	GLMSG::SNET_CTF_MOVESTATE_BY_GM_CA_FB *pmsg = (GLMSG::SNET_CTF_MOVESTATE_BY_GM_CA_FB *)nmg;
	
	/*
	if (pmsg->stateAfter == CTF_STATE_DONE)
	{
		PrintConsoleText("/CTF command failure. Current CTF state does not support moving. Try again later.");
	}
	else
	*/
	{
		std::string arrState[5] = {"Stop", "Ready", "Start", "Done", "Reward"};

		GASSERT(pmsg->stateBefore < 5);
		GASSERT(pmsg->stateAfter < 5);

		PrintConsoleText(sc::string::format("/CTF succeed. (%1% -> %2%)", arrState[pmsg->stateBefore], arrState[pmsg->stateAfter]));
	}
}

ClientActor* GLGaeaClient::GetCopyActor ( const EMCROW emCrow, const DWORD dwID )
{
	switch( emCrow )
	{
	case CROW_PC:
		{
			if (GetCharacter()->GetGaeaID() == dwID)
            {
				return GetCharacter();
            }
			else if (dwID != EMTARGET_NULL)
            {
                std::tr1::shared_ptr<GLCharClient> pChar = m_pLandMClient->GetChar(dwID);
                if (!pChar)
                    return NULL;
                else
                    return pChar.get();
            }
		}
		break;

	case CROW_NPC:
	case CROW_MOB:
		{
			if (dwID != EMTARGET_NULL)
            {
                std::tr1::shared_ptr<GLCrowClient> spCrow = m_pLandMClient->GetCrow(dwID);
                if (spCrow)
                    return spCrow.get();
                else
                    return NULL;
            }
			else
            {
                return NULL;
            }
		}
		break;

	case CROW_SUMMON:
		{
			if (dwID != EMTARGET_NULL)
            {
                std::tr1::shared_ptr<GLSummonClient> spSummon = m_pLandMClient->GetSummon(dwID);
                if (spSummon)
                    return spSummon.get();
                else
                    return NULL;
            }
			else
            {
                return NULL;
            }
		}
		break;;

	case CROW_MATERIAL:
		{
			if (dwID != EMTARGET_NULL)
            {                
                std::tr1::shared_ptr<GLMaterialClient> spMaterial = m_pLandMClient->GetMaterial(dwID);
                if (spMaterial)                    
                    return spMaterial.get();
                else
                    return NULL;
            }
			else
            {
                return NULL;
            }
		}
		break;

	default:
		{
			if ( !m_bBRIGHTEVENT )
			{
				//GASSERT(0&&"emCrow가 잘못된 지정자 입니다.");
			}
		}
		break;
	}

	return NULL;
}

ClientActor* GLGaeaClient::GetCopyActor ( const std::string &strName )
{
	if (strName == GetCharacter()->m_szName)
    {
        return GetCharacter();
    }
	else
    {
        std::tr1::shared_ptr<GLCharClient> spChar = m_pLandMClient->FindChar(strName);
        if (spChar)
            return boost::polymorphic_downcast<ClientActor*> (spChar.get());
        else
            return NULL;
    }
	return NULL;
}

D3DXVECTOR3* GLGaeaClient::FindCharPos (const std::string& strName )
{
	if ( strName==std::string(GetCharacter()->GetCharData().m_szName) )
	{
		m_vCharPos = GetCharacter()->GetPosition();
		return &m_vCharPos;
	}

    if (!m_pLandMClient)
        return &m_vCharPos;

	std::tr1::shared_ptr<GLCharClient> pCharClient = m_pLandMClient->FindChar ( strName );
	if ( !pCharClient )
        return &m_vCharPos;

	m_vCharPos = pCharClient->GetPosition();
	return &m_vCharPos;
}

D3DXVECTOR3* GLGaeaClient::FindCharHeadPos (const std::string& strName )
{
	if ( strName==std::string(GetCharacter()->GetCharData().m_szName) )
	{
		m_vCharPos = GetCharacter()->GetPosBodyHeight();
		return &m_vCharPos;
	}
    
    if (!m_pLandMClient)
        return &m_vCharPos;

	std::tr1::shared_ptr<GLCharClient> pCharClient = m_pLandMClient->FindChar ( strName );
	if (!pCharClient)
        return NULL;


	// 토너먼트 사용중에는 헤드쳇 할수없다.
	// TOURNAMENT_UNIQ_DISABLE
	if( GetTournamentClient()->GetIsTournamentView() == true)
	{
		return NULL;
	}
	

	m_vCharPos = pCharClient->GetPosBodyHeight();
	return &m_vCharPos;
}

D3DXVECTOR3* GLGaeaClient::FindCharHeadPos ( DWORD dwGaeaID ) 
{
	if ( dwGaeaID == GetCharacter()->GetGaeaID() )
	{
		m_vCharPos = GetCharacter()->GetPosBodyHeight();

		return &m_vCharPos;
	}

    if (!m_pLandMClient)
        return &m_vCharPos;

	std::tr1::shared_ptr<GLCharClient> pCharClient = m_pLandMClient->FindChar ( dwGaeaID );
	if ( !pCharClient )
        return NULL;

	m_vCharPos = pCharClient->GetPosBodyHeight();

	return &m_vCharPos;
}

D3DXVECTOR3* GLGaeaClient::FindMaterialHeadPos ( DWORD dwGaeaID ) 
{
	std::tr1::shared_ptr<GLMaterialClient> spMaterialClient = GetActiveMap()->GetMaterial ( dwGaeaID );
	if (!spMaterialClient)
        return NULL;

	m_vCharPos = spMaterialClient->GetPosBodyHeight();

	return &m_vCharPos;
}

DxSkinChar* GLGaeaClient::GetSkinChar ( const STARGETID &sTargetID )
{
	if ( !m_pLandMClient )	return NULL;

	//	Note : 타겟의 위치 정보를 가져옴.
	if ( sTargetID.emCrow == CROW_PC )
	{
		if ( GetCharacter()->GetGaeaID() == sTargetID.GaeaId )
            return GetCharacter()->GetSkinChar();

		std::tr1::shared_ptr<GLCharClient> pChar = m_pLandMClient->GetChar ( sTargetID.GaeaId );
		if ( pChar )
            return pChar->GetCharSkin();
	}
	else if ( sTargetID.emCrow == CROW_NPC || sTargetID.emCrow == CROW_MOB )
	{
        std::tr1::shared_ptr<GLCrowClient> spCrow = m_pLandMClient->GetCrow(sTargetID.GaeaId);
		if (spCrow)
            return spCrow->GetCharSkin();
	}
	else if ( sTargetID.emCrow == CROW_MATERIAL )
	{
		std::tr1::shared_ptr<GLMaterialClient> spMaterial = m_pLandMClient->GetMaterial ( sTargetID.GaeaId );
		if (spMaterial)
            return spMaterial->GetCharSkin();
	}
	else if ( sTargetID.emCrow == CROW_PET )
	{
		if ( GetPetClient()->m_dwGUID == sTargetID.GaeaId )
            return GetPetClient()->GetSkinChar ();

		std::tr1::shared_ptr<GLAnyPet> pAnyPet = m_pLandMClient->GetPet ( sTargetID.GaeaId );
		if ( pAnyPet )
            return pAnyPet->GetSkinChar ();
	}
	else if ( sTargetID.emCrow == CROW_SUMMON )
	{
		std::tr1::shared_ptr<GLSummonClient> pAnySummon = m_pLandMClient->GetSummon ( sTargetID.GaeaId );
		if ( pAnySummon )
            return pAnySummon->GetSkinChar ();
	}
	else
	{
		//GASSERT(0&&"emCrow가 잘못된 지정자 입니다." );
        sc::writeLogError(std::string("GLGaeaClient::GetSkinChar() emCrow가 잘못된 지정자 입니다."));
	}
		

	return NULL;
}

D3DXMATRIX GLGaeaClient::GetTargetTransMatrix(const STARGETID &sTargetID)
{
	D3DXMATRIX matTEMP;
	D3DXMatrixIdentity( &matTEMP );
	if ( !m_pLandMClient )
		return matTEMP;

	switch(sTargetID.emCrow)
	{
		case CROW_PC:
			{
				std::tr1::shared_ptr<GLCharClient> pChar = m_pLandMClient->GetChar ( sTargetID.GaeaId );
				if ( pChar )
					return pChar->GetTransMatrix();

				if ( GetCharacter()->GetGaeaID() == sTargetID.GaeaId )
					return m_pCharacter->GetTransMatrix();
			}

		case CROW_NPC:
		case CROW_MOB:
			{
				std::tr1::shared_ptr<GLCrowClient> spCrow = m_pLandMClient->GetCrow(sTargetID.GaeaId);
				if (spCrow)
					return spCrow->GetTransMatrix();
			}

		case CROW_MATERIAL:
			{
				std::tr1::shared_ptr<GLMaterialClient> spMaterial = m_pLandMClient->GetMaterial(sTargetID.GaeaId);
				if (spMaterial)
					return spMaterial->GetTransMatrix();
			}

		case CROW_ITEM:
			{
				std::tr1::shared_ptr<CItemClientDrop> spItem = m_pLandMClient->GetItem(sTargetID.GaeaId);
				if (spItem)
					return spItem->matWld;
			}

		case CROW_PET:
			{
				return matTEMP;
			}

		case CROW_MONEY:
			{
				std::tr1::shared_ptr<CMoneyClientDrop> spMoney = m_pLandMClient->GetMoney(sTargetID.GaeaId);
				if (spMoney)
					return spMoney->matWld;
			}
		case CROW_SUMMON:
			{
				std::tr1::shared_ptr<GLSummonClient> spSummonClient = m_pLandMClient->GetSummon(sTargetID.GaeaId);
				if (spSummonClient)
					return spSummonClient->GetTransMatrix();

				// 소환수가 존재하지 않을경우 이팩트를 출력하지 않는다
				return matTEMP;
			}

		default:
			{
				GASSERT(0&&"emCrow가 잘못된 지정자 입니다." );
				return matTEMP;
			}

	}
	//	Note : 타겟의 위치 정보를 가져옴.
}

float GLGaeaClient::GetCrowDir ( const STARGETID &sTargetID )
{
	if ( !m_pLandMClient )
        return 0.0f;

	ClientActor* 	pActor = GetCrow( sTargetID.emCrow, sTargetID.GaeaId );
	if( !pActor )
		return 0.0f;

	return pActor->GetDirection();
}

ClientActor* GLGaeaClient::GetCrow( const EMCROW emCrow, const DWORD dwGaeaId )
{
	if( !m_pLandMClient )
        return NULL;

	ClientActor*		pActor = NULL;
	if( emCrow == CROW_PC )
	{
		if( GetCharacter()->GetGaeaID() == dwGaeaId )
            pActor = GetCharacter();
		else
			pActor = m_pLandMClient->GetChar( dwGaeaId ).get();
	}
	else if( emCrow == CROW_NPC || emCrow == CROW_MOB )
	{
        pActor = m_pLandMClient->GetCrow( dwGaeaId ).get();
	}
	else if( emCrow == CROW_MATERIAL )
	{
		pActor = m_pLandMClient->GetMaterial ( dwGaeaId ).get();
	}
	else if( emCrow == CROW_SUMMON )
	{
		pActor = m_pLandMClient->GetSummon ( dwGaeaId ).get();
	}

	return pActor;
} //GLGaeaClient::GetCrowOnLand

const EMCROWCOLORTYPE GLGaeaClient::GetCrowIdentifyColorType ( const STARGETID& sTargetID )
{
    Faction::ManagerClient* const _factionMan = m_pLandMClient->GetFactionManager();
    if ( _factionMan )
    {
        const Faction::EMRELATION_TYPE _relation = _factionMan->getRelationClient(m_pCharacter, GetCopyActor(sTargetID));
        switch ( _relation )
        {
        case Faction::EMRELATION_ENEMY:
            return EMCROWCOLORTYPE_ENEMY;

        case Faction::EMRELATION_NEUTRAL_ENEMY:
		case Faction::EMRELATION_NEUTRAL_ALLY:
            return EMCROWCOLORTYPE_NPC;

        case Faction::EMRELATION_ALLY:
            return EMCROWCOLORTYPE_ALLIANCE;

        case Faction::EMRELATION_ERROR:
        default:
            break;
        }
    }

    return GetCrowIdentifyColorTypeNonFaction(sTargetID);
}

const EMCROWCOLORTYPE GLGaeaClient::GetCrowIdentifyColorTypeNonFaction ( const STARGETID& sTargetID )
{
	ClientActor* pActor = GetCopyActor(sTargetID);
	if ( !pActor )
	{
		return EMCROWCOLORTYPE_NPC;
	}

	switch ( pActor->GetCrow() )
	{
	case CROW_MOB:
		{
			return EMCROWCOLORTYPE_ENEMY;
		}
		break;

	case CROW_NPC:
		{
			return EMCROWCOLORTYPE_NPC;
		}
		break;

	case CROW_SUMMON:
	case CROW_PC:
		{
			if ( GetCharacter()->IsReActionable( sTargetID, EMIMPACT_SIDE_ENEMY ) )
			{
				return EMCROWCOLORTYPE_ENEMY;
			}
			else
			{
				return EMCROWCOLORTYPE_ALLIANCE;
			}
		}
		break;
	};

	return EMCROWCOLORTYPE_NPC;
}

DWORD GLGaeaClient::GetMobNameColor ( DWORD dwGlobID )
{
	DWORD dwCOLOR(CROWCOLOR::MOB_COLOR_GRAY);

    std::tr1::shared_ptr<GLCrowClient> spCROW = GetActiveMap()->GetCrow(dwGlobID);
	if (!spCROW)
        return dwCOLOR;

	if (spCROW->m_pCrowData->m_emCrow == CROW_NPC)
	{
		dwCOLOR = CROWCOLOR::MOB_COLOR_WHITE;
	}
	else
	{
		WORD wMY_LEVEL = m_pCharacter->GETLEVEL();
		WORD wMOB_LEVEL = spCROW->m_pCrowData->m_wLevel;
		float fEXP_RATE = GLCONST_CHAR::GETEXP_RATE ( wMY_LEVEL, wMOB_LEVEL );

		if (spCROW->IsNonDamaged())
            dwCOLOR = CROWCOLOR::MOB_COLOR_WHITE; // 공격받지 않는 Crow라면
		else if (fEXP_RATE == 0)
            dwCOLOR = CROWCOLOR::MOB_COLOR_GRAY;
		else
            dwCOLOR = CROWCOLOR::MOB_COLOR_RED;
	}
	return dwCOLOR;
}

BOOL GLGaeaClient::IsMapCollsion( D3DXVECTOR3& vTargetPt, D3DXVECTOR3& vFromPt, EMCOLLMODE emCullMode )
{
	if( !m_pLandMClient )
        return FALSE;

	DxLandMan* pLandMan = m_pLandMClient->GetLandMan();
	if( !pLandMan )	
        return FALSE;

	LPDXFRAME	pFrame(NULL);
	BOOL		bCollision(FALSE);
	D3DXVECTOR3 vCollision( 0.f, 0.f, 0.f );
	pLandMan->IsCollision( vTargetPt, vFromPt, vCollision, bCollision, pFrame, EMCC_CULL_NONE_ONLY_WALL );

	return bCollision;
}

void GLGaeaClient::PetSkinPackApplyEffect()
{

	if( m_pPet->IsVALID() )
	{
		// 생성 효과
		D3DXMATRIX matEffect;
		D3DXVECTOR3 vPos = m_pPet->GetPosition ();
		D3DXMatrixTranslation ( &matEffect, vPos.x, vPos.y, vPos.z );

		std::string strGEN_EFFECT = GLCONST_CHAR::strPET_GEN_EFFECT.c_str();
		STARGETID sTargetID(CROW_PET,m_pPet->m_dwGUID,vPos);
		DxEffGroupPlayer::GetInstance().NewEffGroup( strGEN_EFFECT.c_str(), matEffect, &sTargetID, TRUE, FALSE );				
		m_pPet->SkinLoad( m_pd3dDevice );

		// Note : 1.AABB Box를 가져온다. 2.높이를 계산해 놓는다.
		m_pPet->GetSkinChar()->GetAABBBox( m_pPet->m_vMaxOrg, m_pPet->m_vMinOrg );
		m_pPet->m_fHeight = ( m_pPet->m_vMaxOrg.y - m_pPet->m_vMinOrg.y ) * m_pPet->m_sPetSkinPackState.fPetScale;

		if( !m_pPet->IsUsePetSkinPack() )
            m_pPet->UpdateSuit ( TRUE );
	}

}

const bool GLGaeaClient::IsVisibleToTargetInfoMouseOver( const SACTIONTAR &sCursorTar )
{
	EMCROW emCROW = sCursorTar.sTARID.emCrow;

	//! 아이템이나 돈, 머터리얼이라면 불가능
	switch( emCROW )
	{
	case CROW_ITEM:
	case CROW_MONEY:
	case CROW_MATERIAL:
		return false;
	};

	// [shhan][2015.01.16] WorldEdit 에서 종료현상이 발생하여 수정함.
	if ( !m_pLandMClient )
		return false;

    Faction::ManagerClient* const _factionMan = m_pLandMClient->GetFactionManager();
    if ( _factionMan )
    {
        const Faction::EMRELATION_TYPE _relation = _factionMan->getRelationClient(m_pCharacter, GetCopyActor(sCursorTar.sTARID));
        switch(_relation)
        {
        case Faction::EMRELATION_ENEMY:
        case Faction::EMRELATION_ALLY:
        case Faction::EMRELATION_NEUTRAL_ENEMY:
		case Faction::EMRELATION_NEUTRAL_ALLY:
            return true;
        }
    }

	//! PVP 상태의 PC라면 가능
	if ( (emCROW==CROW_PC&&sCursorTar.emACTAR==EMACTAR_PC_PVP) )
	{
		return true;
	}

	//! 몹이라면 가능
	if ( sCursorTar.emACTAR==EMACTAR_MOB )
	{
		return true;
	}

	//! PVP 상태의 소환라면 가능
	if (emCROW==CROW_SUMMON&&sCursorTar.emACTAR==EMACTAR_SUMMON_ATTACK)
	{
		return true;
	}

	//! 성향이벤트 중이라면 가능
	if ( ( m_bBRIGHTEVENT&&sCursorTar.emACTAR!=EMACTAR_NPC ) )
	{
		return true;
	}

	return false;
}


HRESULT GLGaeaClient::CreateSummon(NET_MSG_GENERIC* nmg)
{
	/*
	GLMSG::SNETPC_REQ_USE_SUMMON_FB* pNetMsg = ( GLMSG::SNETPC_REQ_USE_SUMMON_FB* ) nmg;

	GLSUMMON sSummon;

	sSummon.m_emTYPE	 = pNetMsg->emTYPE;
	sSummon.m_sSummonID  = pNetMsg->sSummonID;
	sSummon.m_dwOwner	 = pNetMsg->dwOwner;
	sSummon.m_dwNowHP	 = pNetMsg->dwNowHP;
	sSummon.m_wNowMP	 = pNetMsg->wNowMP;
	
	// 주인 옆에 소환수 생성 emFB;

	GLSUMMONCLIENTNODE* pSummonNode = m_SummonList.ADDTAIL( new GLSummonClient );

	// 실패처리 ( 서버에 생성된 팻을 삭제하도록 )
	if ( !pSummonNode || !pSummonNode->Data )
	{
		return S_FALSE;
	}

	PGLSUMMONCLIENT pSummon = pSummonNode->Data;

	HRESULT hr = pSummon->Create ( &sSummon, pNetMsg->vPos, pNetMsg->vDir, m_pLandMClient->GetNaviMesh(), m_pd3dDevice );

	// 실패처리 ( 서버에 생성된 팻을 삭제하도록 )
	if ( FAILED(hr) )
	{
		return S_FALSE;
	}

	// 생성 효과
	D3DXMATRIX matEffect;
	D3DXVECTOR3 vPos = pSummon->GetPosition ();
	D3DXMatrixTranslation ( &matEffect, vPos.x, vPos.y, vPos.z );

	std::string strGEN_EFFECT = GLCONST_CHAR::strPET_GEN_EFFECT.c_str();
	STARGETID sTargetID(CROW_PET,pSummon->m_dwGlobID,vPos);
	DxEffGroupPlayer::GetInstance().NewEffGroup( strGEN_EFFECT.c_str(), matEffect, &sTargetID );
*/
	return S_OK;

}

void GLGaeaClient::ReqUseSummon( SNATIVEID sSummonID )
{
	GLMSG::SNETPC_REQ_USE_SUMMON NetMsg(sSummonID,SUMMON_TYPE_NORMAL);
	NETSENDTOFIELD(&NetMsg);
}

void GLGaeaClient::ReqSummonEventSkill ( UINT iIdx )
{
	GLMSG::SNETPC_REQ_USE_SUMMON_EVENT_SKILL_CF msg( iIdx );
	NETSENDTOFIELD( &msg );
}

void GLGaeaClient::ReloadAIFSM ()
{
	GLMSG::SNET_RELOAD_AI_FSM_CF msg;
	NETSENDTOFIELD( &msg );
}

void GLGaeaClient::ChangeBasicHireSummon( WORD wMainID, WORD wSubID )
{
	GLMSG::SNETPC_REQ_HIRE_BASICSUMMON_CF msg( wMainID, wSubID );
	NETSENDTOFIELD( &msg );
}

void GLGaeaClient::ShowFactionInformation ( )
{
	//!! SF_TODO
	/*if ( m_pInterface )
	{
		m_pInterface->UiShowGroupFocus( FACTION_INFO_WINDOW );

		m_pInterface->PrintConsoleText(
			"Open Faction Information Window ..." );
	}
	else
	{
		m_pInterface->PrintConsoleText(
			"Failed Open Faction Information Window !!!" );
	}*/
}

void GLGaeaClient::ShowLandUserCount ( Faction::EMFACTION_TYPE _emFactionType
									 , int _nInstanceMapID )
{
	GLMSG::SNETPC_REQ_USERCOUNT_LAND_CF sMsg( _emFactionType, _nInstanceMapID );
	NETSENDTOFIELD( &sMsg );
}

void GLGaeaClient::LockCharSlot ( const WORD _wCharSlotNum )
{
	GLCharacter* pChar = GetCharacter();
	if ( NULL == pChar )
		return;

	pChar->ReqLockCharSlot( _wCharSlotNum );
}

void GLGaeaClient::UnlockCharSlot ( const WORD _wCharSlotNum )
{
	GLCharacter* pChar = GetCharacter();
	if ( NULL == pChar )
		return;

	pChar->ReqUnlockCharSlot( _wCharSlotNum );
}

void GLGaeaClient::ChangeCharSlot ( const WORD _wCharSlotNum )
{
	GLCharacter* pChar = GetCharacter();
	if ( NULL == pChar )
		return;

	pChar->ReqChangeCharSlot( _wCharSlotNum );
}



HRESULT GLGaeaClient::ReqClubDeathMatchInfo()
{
	GLLandManClient* pLand = GetActiveMap();
	if ( !pLand || !pLand->m_bClubDeathMatch )
        return E_FAIL;

	GLMSG::SNET_CLUB_DEATHMATCH_RANKING_REQ NetMsg;
	NetMsg.dwMapID = pLand->GetBaseMapID().dwID;
	NETSENDTOFIELD(&NetMsg);

	return TRUE;
}

void  GLGaeaClient::ReqClubDeathMatchOnOff(DWORD GuidId, int nStateFlag)
{
	GLMSG::SNET_GM_DEATHMATCH_ON_OFF_CA NetMsgCA(GuidId, nStateFlag);
	NETSENDTOAGENT(&NetMsgCA);
}

void GLGaeaClient::GetConvertServerTime( CTime& cTime )
{
	tm t1, t2;
	cTime.GetLocalTm(&t1);
	cTime.GetGmtTm(&t2);
	
	__time64_t tLocalTime = _mktime64( &t1 );
	__time64_t tUtcTime = _mktime64( &t2 );;
	__time64_t tSpan = tLocalTime - tUtcTime;
	
	int nTime = (int)(tSpan / 3600); 
	nTime = nTime - m_nServerTimeBias;	// 서버타임과 중복 계산

	if ( nTime == 0 )
        return;

	CTimeSpan cTimeSpan( 0, -nTime, 0, 0 );
	cTime += cTimeSpan;
}

void GLGaeaClient::MsgServicerTimeBrd(NET_MSG_GENERIC* pMsg)
{
    GLMSG::SNET_MSG_SERVERTIME_BRD* pNetMsg = (GLMSG::SNET_MSG_SERVERTIME_BRD*) pMsg;
    SetServerTimeBias(pNetMsg->nTimeBias);
    SetCurrentTime(pNetMsg->t64Time);
}

void GLGaeaClient::MsgChangeNameMap(NET_MSG_GENERIC* pMsg)
{
    GLMSG::SNETPC_CHANGE_NAMEMAP *pNetMsg = (GLMSG::SNETPC_CHANGE_NAMEMAP*) pMsg;
    m_pLandMClient->ChangeCharMap ( pNetMsg->szOldName, pNetMsg->szNewName );
}

void GLGaeaClient::MsgEtnryFailed(NET_MSG_GENERIC* pMsg)
{
    //m_pInterface->SetMapCheckType(UI::EMMAPCHECK_ENTRYFAIL);
    //m_pInterface->UiShowGroupFocus(MAP_REQUIRE_CHECK);
}

void GLGaeaClient::MsgDuplicateCloseGame(NET_MSG_GENERIC* pMsg)
{
	CloseConnect();
	
	//GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_DuplicateLogin );
}

void GLGaeaClient::MsgLimitTimeOver(NET_MSG_GENERIC* pMsg)
{
    GLMSG::SNETLIMITTIME_OVER *pNetMsg = (GLMSG::SNETLIMITTIME_OVER *) pMsg;
    if (pNetMsg->nRemainTime != 0)
    {				
        PrintMsgText(
            NS_UITEXTCOLOR::DISABLE,
            sc::string::format(ID2GAMEINTEXT("LIMITTIME_REMAINTIME"), pNetMsg->nRemainTime));
    }
    /*else
    {
		m_pInterface->SetMapCheckType(UI::EMMAPCHECK_LIMITTIME);
        m_pInterface->UiShowGroupFocus(MAP_REQUIRE_CHECK);
    }*/
}

void GLGaeaClient::MsgCyberCafeClassUpdate(NET_MSG_GENERIC* pMsg)
{
    GLMSG::SNET_CYBERCAFECLASS_UPDATE *pNetMsg = (GLMSG::SNET_CYBERCAFECLASS_UPDATE *)pMsg;
    GetCharacter()->m_emIPBonus = pNetMsg->emIPBonus;
	//!! SF_TODO
	m_pInterface->SetIPBonusClass( (int)pNetMsg->emIPBonus );
	PrintMsgText(
		NS_UITEXTCOLOR::WHITE, "IP Bonus Activated : Exp x 150% Enjoy!" );
}

void GLGaeaClient::MsgServerInfo(NET_MSG_GENERIC* pMsg)
{
    GLMSG::SNETPC_SERVER_INFO *pNetMsg = (GLMSG::SNETPC_SERVER_INFO *)pMsg;

    if ( m_bSCHOOL_FREEPK != pNetMsg->bSCHOOL_FREEPK )
    {
        if ( pNetMsg->bSCHOOL_FREEPK )
        {
            PrintConsoleTextDlg ( ID2GAMEINTEXT("OTHER_SCHOOL_FREEPK_ON") );
        }
        else
        {
            PrintConsoleTextDlg ( ID2GAMEINTEXT("OTHER_SCHOOL_FREEPK_OFF") );

            // 적대관계 모두 삭제
            GetCharacter()->DEL_PLAYHOSTILE_ALL();
        }
    }

    m_bSCHOOL_FREEPK = pNetMsg->bSCHOOL_FREEPK;
}

void GLGaeaClient::MsgServerBrightEventInfo(NET_MSG_GENERIC* pMsg)
{
    GLMSG::SNETPC_SERVER_BRIGHTEVENT_INFO *pNetMsg = ( GLMSG::SNETPC_SERVER_BRIGHTEVENT_INFO *)pMsg;

    m_bBRIGHTEVENT = pNetMsg->bBRIGHTEVENT;
}

void GLGaeaClient::MsgKorShutdown(NET_MSG_GENERIC* pMsg)
{    
    //m_pInterface->DoModal(
    //    ID2GAMEINTEXT("KOR_SHUTDOWN_WARNING"),
    //    UI::MODAL_INFOMATION,
    //    UI::OK);
}

//! 컨트롤의 기본 키 세팅 파일을 로드한다
void GLGaeaClient::KeySettingLoad()
{
	GLInputManager::GetInstance().InitEnvironment( this );

	const SUBPATH* pPath = GetSubPath();
	std::string strFilePathBase(sc::getAppPath());
	strFilePathBase.append(pPath->GLogicScriptsPath());

	{
		std::string strControlTypeA = strFilePathBase;
		strControlTypeA.append(GLogicData::GetInstance().GetControlTypeAFileName());

		if ( strControlTypeA.empty() )
		{
			sc::writeLogError("GLGaeaClient::KeySettingLoad() - strControlTypeA is empty");
			GASSERT( !"GLGaeaClient::KeySettingLoad() - strControlTypeA is empty");
		}
		else
		{
			GLInputManager::GetInstance().LoadDefaultSetting( EM_CONTROLTYPE_A, strControlTypeA );
		}
	}

	{
		std::string strControlTypeB = strFilePathBase;
		strControlTypeB.append(GLogicData::GetInstance().GetControlTypeBFileName());

		if ( strControlTypeB.empty() )
		{
			sc::writeLogError("GLGaeaClient::KeySettingLoad() - strControlTypeB is empty");
			GASSERT( !"GLGaeaClient::KeySettingLoad() - strControlTypeB is empty");
		}
		else
		{
			GLInputManager::GetInstance().LoadDefaultSetting( EM_CONTROLTYPE_B, strControlTypeB );
		}
	}

	{
		std::string strControlTypeFlyCamera = strFilePathBase;
		strControlTypeFlyCamera.append(GLogicData::GetInstance().GetControlTypeFlyCameraFileName());

		if ( strControlTypeFlyCamera.empty() )
		{
			sc::writeLogError("GLGaeaClient::KeySettingLoad() - strControlTypeFlyCamera is empty");
			GASSERT( !"GLGaeaClient::KeySettingLoad() - strControlTypeFlyCamera is empty");
		}
		else
		{
			GLInputManager::GetInstance().LoadDefaultSetting( EM_CONTROLTYPE_FLYCAMERA, strControlTypeFlyCamera );
		}
	}

	GLInputManager::GetInstance().InitControlType( static_cast<EM_CONTROLTYPE>(GLCONST_CHAR::nUI_DEFAULTKEYTYPE) );
}

BOOL GLGaeaClient::IsEmulator() const
{
    if (m_pGlobalStage)
        return m_pGlobalStage->IsEmulator();
    else
        return FALSE;
}

BOOL GLGaeaClient::GetUsedMaterialSystem() const
{
	if (m_pLandMClient)
		return m_pLandMClient->GetUsedMaterialSystem();
	else
		return FALSE;
}

bool GLGaeaClient::ExecuteCommand(const std::string& Command)
{
	std::string strCommand = Command;
	if( this->GetServiceProvider() == SP_GS )//
		strCommand = sc::string::utf8_to_ansi(strCommand);

	return m_pGameCommand->command(strCommand);
}

void GLGaeaClient::DbUnlockUser(DWORD UserDbNum)
{
    if (m_pCharacter->m_dwUserLvl >= USER_GM3)
    {
        GLMSG::NET_GM_DB_UNLOCK_USER MsgCA(UserDbNum);
        NETSENDTOAGENT(&MsgCA);
    }
    else
    {
        PrintConsoleTextDlg("Have not permission");
    }
}

float GLGaeaClient::GetFlyCameraControl_MoveVelo() const
{
	return GLFlyCameraControl::GetInstance().GetMoveVelo();
}

float GLGaeaClient::GetFlyCameraControl_RotateVelo() const
{
	return GLFlyCameraControl::GetInstance().GetRotateVelo();
}

void GLGaeaClient::SetFlyCameraControl_MoveVelo( float fVelo ) const
{
	GLFlyCameraControl::GetInstance().SetMoveVelo(fVelo);
}

void GLGaeaClient::SetFlyCameraControl_RotateVelo( float fVelo ) const
{
	GLFlyCameraControl::GetInstance().SetRotateVelo(fVelo);
}

void GLGaeaClient::SetFlyCameraControl_MoveInterTime( float fInterTime ) const
{
	GLFlyCameraControl::GetInstance().SetMoveInterTime(fInterTime);
}

void GLGaeaClient::SetFlyCameraControl_RotateInterTime( float fInterTime ) const
{
	GLFlyCameraControl::GetInstance().SetRotateInterTime(fInterTime);
}

void GLGaeaClient::SetFlyCameraControl_TargetInterTime( float fInterTime ) const
{
	GLFlyCameraControl::GetInstance().SetTargetInterTime(fInterTime);
}

void GLGaeaClient::SetFlyCameraPos( const D3DXVECTOR3& vPos )
{
	if ( GLInputManager::GetInstance().GetCurrentControlType() == EM_CONTROLTYPE_FLYCAMERA )
		DxViewPort::GetInstance().SetFromPt(vPos);
}

void GLGaeaClient::MsgPostProcess( NET_MSG_GENERIC* pMsg )
{
	
	if( m_spPostRnClient == NULL )
		return;

	switch( pMsg->nType )
	{
	case NET_MSG_POST_RENEW_LIST_ACK:
		{
			NET_MSG_PACK* pPacket = (NET_MSG_PACK*)pMsg;

			GLMSG::NET_POST_RENEW_LIST_ACK RecvData;
			if (!msgpack::BufferToObject(pPacket->DataBuffer(), pPacket->DataSize(), RecvData))
				return;

			m_spPostRnClient->MsgListAck( RecvData.m_vecPostList );
		}
		
		break;
	case NET_MSG_POST_RENEW_SEND_ACK:
		{
			NET_MSG_PACK* pPacket = (NET_MSG_PACK*)pMsg;

			GLMSG::NET_POST_RENEW_SEND_ACK RecvData;
			if (!msgpack::BufferToObject(pPacket->DataBuffer(), pPacket->DataSize(), RecvData))
				return;

			int i=0;

		}

		break;
	case NET_MSG_POST_RENEW_DEL_ACK:
		{
			NET_MSG_PACK* pPacket = (NET_MSG_PACK*)pMsg;

			GLMSG::NET_POST_RENEW_DEL_ACK RecvData;
			if (!msgpack::BufferToObject(pPacket->DataBuffer(), pPacket->DataSize(), RecvData))
				return;

			int i=0;
		}

		break;
	case NET_MSG_POST_RENEW_ATTACH_ACK:
		{
			NET_MSG_PACK* pPacket = (NET_MSG_PACK*)pMsg;

			GLMSG::NET_POST_RENEW_ATTACH_ACK_FOR_CLIENT RecvData;
			if (!msgpack::BufferToObject(pPacket->DataBuffer(), pPacket->DataSize(), RecvData))
				return;

			int i=0;
		}

		break;

	case NET_MSG_POST_RENEW_RETURN_ACK:
		{
			NET_MSG_PACK* pPacket = (NET_MSG_PACK*)pMsg;

			GLMSG::NET_POST_RENEW_ATTACH_RETURN_ACK_FOR_CLIENT RecvData;
			if (!msgpack::BufferToObject(pPacket->DataBuffer(), pPacket->DataSize(), RecvData))
				return;

			int i=0;
		}

		break;

	case NET_MSG_POST_RENEW_NEW_POST:
		{
			NET_MSG_PACK* pPacket = (NET_MSG_PACK*)pMsg;

			GLMSG::NET_POST_RENEW_NEW_POST RecvData;
			if (!msgpack::BufferToObject(pPacket->DataBuffer(), pPacket->DataSize(), RecvData))
				return;

			int i=0;
		}

		break;

	}

}

void GLGaeaClient::FlyCamera_AddMsg( DWORD dwType, DWORD dwMsg )
{
	if ( GLInputManager::GetInstance().GetCurrentControlType() == EM_CONTROLTYPE_FLYCAMERA )
		GLFlyCameraControl::GetInstance().AddMessage(dwType, dwMsg);
}

//LuaTable GLGaeaClient::GetClubMarkInfo( const char* szClubName, bool bAllorHostile )
//{
//	LuaTable tbClubMarkInfo( GLWidgetScript::GetInstance().GetLuaState() );
//
//	if( NULL == szClubName )
//		return tbClubMarkInfo;
//
//	std::string strName = "";
//
//	//!! SF_TODO 클럽 마크
//	/*clubmark::DXDATA ClubMkark; 
//	if ( strcmp(m_pMyClub->Name(), szClubName) == 0 )
//	{
//		ClubMkark =	DxClubMan::GetInstance().GetClubData( 
//			m_pd3dDevice,
//			GetCharacter()->m_dwServerID,
//			m_pMyClub->DbNum(), m_pMyClub->GetMarkVersion(), &strName);
//	}
//	else if(bAllorHostile)
//	{
//		std::vector<GLCLUBALLIANCE> vData;
//		m_pMyClub->GetAllianceData(vData);	
//		int Size(vData.size());
//
//		for(int index = 0; index < Size; ++index)
//		{
//			if(!strcmp(szClubName, vData[index].GetClubName()))
//			{
//				//ClubMkark = GetClubMark(vData[index].GetClubDbNum(), vData[index].GetMarkVersion());
//				ClubMkark =	DxClubMan::GetInstance().GetClubData( 
//					m_pd3dDevice,
//					GetCharacter()->m_dwServerID,
//					vData[index].GetClubDbNum(), vData[index].GetMarkVersion(), &strName );
//			}
//		}
//	}
//	else
//	{
//		std::vector<GLCLUBBATTLE> vData;
//		m_pMyClub->GetClubBattleData(vData);
//		int Size(vData.size());
//		for(int index = 0; index < Size; ++index)
//		{
//			if(!strcmp(szClubName, vData[index].m_szClubName))
//			{
//				//ClubMkark = GetClubMark(vData[index].m_dwCLUBID, 0);
//				ClubMkark =	DxClubMan::GetInstance().GetClubData(
//					m_pd3dDevice,
//					GetCharacter()->m_dwServerID,
//					vData[index].m_dwCLUBID, 0, &strName);
//			}
//		}
//	}*/
//	std::string strPath = GetGlobalStage()->GetAppPath();
//	strPath.append( GetGlobalStage()->GetSubPath()->TextureFileClub() );
//	strPath.append(strName);
//
//	tbClubMarkInfo.set("Path", strPath.c_str());
//	tbClubMarkInfo.set("LUx", 0.0f);
//	tbClubMarkInfo.set("LUy", 0.0f);
//
//	return tbClubMarkInfo;
//}

void GLGaeaClient::CheckGateMove()
{
	GLCharacter* pChar = GetCharacter();
	if( NULL == pChar )
		return;

	// 캐릭터가 죽었나?
	if( TRUE == pChar->IsDie() )
		return;

	// 게이트 UI 열어야되나?
	if( FALSE == GetGateOpen() )
		return;

	MAPNAMEVECTOR mapNameVector;
	pChar->DetectGateToMapName( mapNameVector );

	// 멀티 게이트 사용?
	if( 1 < mapNameVector.size() && 
		false == GLUseFeatures::GetInstance().IsUsingMultiGate() )
		return;

	// 게이트가 없으면 UI 끄기
	if( 0 == mapNameVector.size() )
	{
		//GLWidgetScript::GetInstance().LuaCallFunc< void >(
			//NSWIDGET_SCRIPT::g_strFunc_GateClose );

		GetInterface()->SetGateOpen( FALSE );

		return;
	}

	// 캐릭터 탈것 상태에 따라 UI 열기
	switch( pChar->VehicleState() )
	{
	case EMVEHICLE_DRIVER :
	case EMVEHICLE_OFF :
		{
			/*LuaTable tbMapName( GLWidgetScript::GetInstance().GetLuaState() );

			int nIndex = 1;

			MAPNAMEVECTORCITER iter = mapNameVector.begin();
			for( ; iter != mapNameVector.end(); ++iter )
			{	
				CString StrMapName = *iter;
				tbMapName.set( nIndex++, StrMapName.GetBuffer() );
			}*/

			//GLWidgetCallFuncValue arg;
			//arg.SetTable( &tbMapName );

			//GLWidgetScript::GetInstance().LuaCallFunc< void >(
				//NSWIDGET_SCRIPT::g_strFunc_GateOpen, &arg, 1 );
			GetInterface()->SetGateOpen( TRUE );
		}
		break;

	default :
		{
			//GLWidgetScript::GetInstance().LuaCallFunc< void >(
				//NSWIDGET_SCRIPT::g_strFunc_GateClose );

			GetInterface()->SetGateOpen( FALSE );
		}
		break;
	}
}

//void GLGaeaClient::SetCursorTargetInfo( const STARGETID& sTargetID )
//{
//	if( EMTARGET_NULL == sTargetID.GaeaId )
//	{
//		GLWidgetScript::GetInstance().LuaCallFunc< void >( 
//			NSWIDGET_SCRIPT::g_strFunc_CursorTargetInfo_SetNone );
//		return;
//	}
//
//	if( CROW_PC != sTargetID.emCrow &&
//		CROW_NPC != sTargetID.emCrow &&
//		CROW_MOB != sTargetID.emCrow )
//	{
//		GLWidgetScript::GetInstance().LuaCallFunc< void >( 
//			NSWIDGET_SCRIPT::g_strFunc_CursorTargetInfo_SetNone );
//		return;
//	}
//
//	ClientActor* pActor = GetCopyActor( sTargetID );
//	if( NULL == pActor )
//	{
//		GLWidgetScript::GetInstance().LuaCallFunc< void >( 
//			NSWIDGET_SCRIPT::g_strFunc_CursorTargetInfo_SetNone );
//		return;
//	}
//
//	if( true == pActor->IsDie() )
//	{
//		GLWidgetScript::GetInstance().LuaCallFunc< void >( 
//			NSWIDGET_SCRIPT::g_strFunc_CursorTargetInfo_SetNone );
//		return;
//	}
//
//	switch( pActor->GetCrow() )
//	{
//	case CROW_PC :
//		{
//			GLCharClient* pChar = dynamic_cast< GLCharClient* >( pActor );
//			if( NULL == pChar )
//				return;
//
//			const char* szName = pChar->GetName();
//			const WORD wLevel = pChar->GetLevel();
//			
//			const WORD wSchool = pChar->GetCharSchool();
//			if( GLSCHOOL_NUM <= wSchool )
//				return;
//
//			EMCHARINDEX emClassIndex = CharClassToIndex( EMCHARCLASS( pChar->GetCharData().m_Base.emClass ) );
//			if( GLCI_NUM_ACTOR <= emClassIndex )
//				return;
//
//			int nPartyState = 0;
//			if( TRUE == pChar->IsPartyMaster() )
//				nPartyState = 1;
//			else if( TRUE == pChar->IsPartyMem() )
//				nPartyState = 2;
//
//			DWORD dwPartyID = pChar->GetPartyID().GetPartyID();
//			bool bPartyValid = pChar->GetPartyID().isValidParty();
//
//			DWORD dwClubID = pChar->GetClubID();
//			//bool bClubValid = ( dwClubID != 0 ) == true ; // 수정필요!
//			const char* szClubName = pChar->GetClubName();
//
//			bool bNotPkTime = ( GLCONST_CHAR::bSCHOOL_FREE_PK_ALWAYS == false && IsSchoolFreePk() == true
//				&& IsPKTime() == false ) ;
//
//			/*
//			2.4) bSCHOOL_FREE_PK_ALWAYS 값이 0이고, 
//	          bSCHOOL_FREE_PK 값이 1이며, 현재 학원 간 자유 PK 시간이 아닐때 bPkTime이 아니다.
//			*/
//
//			GLWidgetScript::GetInstance().LuaCallFunc< void >(
//				NSWIDGET_SCRIPT::g_strFunc_CursorTargetInfo_SetPC,
//				"-s, -n, -n, -n, -n, -dw, -b, -dw, -s, -b",
//				szName, wLevel, wSchool, (int) emClassIndex, nPartyState, dwPartyID, bPartyValid, dwClubID, szClubName, bNotPkTime );
//		}
//		break;
//
//	case CROW_NPC :
//		{
//			GLCrowClient* pCrow = dynamic_cast< GLCrowClient* >( pActor );
//			if( NULL == pCrow )
//				return;
//
//			// 툴팁 숨김?
//			if( true == pCrow->IsHideTooltip() )
//			{
//				GLWidgetScript::GetInstance().LuaCallFunc< void >( 
//					NSWIDGET_SCRIPT::g_strFunc_CursorTargetInfo_SetNone );
//			}
//			else
//			{
//				const SCROWDATA* pData = pCrow->m_pCrowData;
//				if( NULL == pData )
//					return;
//
//				CNpcTalkControl::NPCMAP* pTalkMap = m_pCharacter->FindDefaultTalk( pData->GetTalkFile() );
//				if( NULL == pTalkMap )
//					return;
//
//				const char* szName = pCrow->GetName();
//
//				LuaTable tbBasicTalk( GLWidgetScript::GetInstance().GetLuaState() );
//				bool bQuestStart = false;
//				bool bQuestStep = false;
//				{
//					CTime cCurSvrTime = GetCurrentTime();
//					const DWORD dwPartyMemeber = GetMyPartyClient()->GetNMember();
//
//					VEC_DWORD vecTalk;
//					BOOST_FOREACH( CNpcTalkControl::NPCMAP_VALUE& iter, *pTalkMap )
//					{
//						std::tr1::shared_ptr< SNpcTalk > spTalk = iter.second;
//						if( NULL == spTalk )
//							continue;
//
//						if( FALSE == spTalk->DoTEST( m_pCharacter, dwPartyMemeber, cCurSvrTime.GetHour() ) )
//							continue;
//
//						switch( spTalk->m_nACTION )
//						{
//						case SNpcTalk::EM_BASIC :
//							{
//								if( SNpcTalk::EM_BASIC_SIZE <= spTalk->m_dwACTION_NO ||
//									SNpcTalk::EM_MARKET == spTalk->m_dwACTION_NO ||
//									SNpcTalk::EM_CD_CERTIFY == spTalk->m_dwACTION_NO ||
//									SNpcTalk::EM_ODDEVEN == spTalk->m_dwACTION_NO ||
//									SNpcTalk::EM_RANDOM_PAGE == spTalk->m_dwACTION_NO ||
//									SNpcTalk::EM_ATTENDANCE_BOOK == spTalk->m_dwACTION_NO ||
//									SNpcTalk::EM_ITEM_MIX == spTalk->m_dwACTION_NO )
//								{
//									continue;
//								}
//
//								if( vecTalk.end() != std::find( vecTalk.begin(), vecTalk.end(), spTalk->m_dwACTION_NO ) )
//									continue;
//
//								vecTalk.push_back( spTalk->m_dwACTION_NO );
//							}
//							break;
//
//						case SNpcTalk::EM_QUEST_START :
//							{
//								bQuestStart = true;
//							}
//							break;
//
//						case SNpcTalk::EM_QUEST_STEP :
//							{
//								bQuestStep = true;
//							}
//							break;
//						}
//					}
//
//					if( false == vecTalk.empty() )
//					{
//						std::sort( vecTalk.begin(), vecTalk.end() );
//
//						int nTableIndex = 1;
//						BOOST_FOREACH( DWORD dwTalk, vecTalk )
//						{
//							tbBasicTalk.set( nTableIndex++, SNpcTalk::szBASICDESC[ dwTalk ].c_str() );
//						}
//					}
//				}
//
//				GLWidgetCallFuncValue args[ 4 ];
//				args[ 0 ].SetString( szName );
//				args[ 1 ].SetTable( &tbBasicTalk );
//				args[ 2 ].SetBool( bQuestStart );
//				args[ 3 ].SetBool( bQuestStep );
//
//				GLWidgetScript::GetInstance().LuaCallFunc< void >(
//					NSWIDGET_SCRIPT::g_strFunc_CursorTargetInfo_SetNPC,
//					args, 4 );
//			}
//		}
//		break;
//
//	case CROW_MOB :
//		{
//			GLCrowClient* pCrow = dynamic_cast< GLCrowClient* >( pActor );
//			if( NULL == pCrow )
//				return;
//
//			// 툴팁 숨김?
//			if( true == pCrow->IsHideTooltip() )
//			{
//				GLWidgetScript::GetInstance().LuaCallFunc< void >( 
//					NSWIDGET_SCRIPT::g_strFunc_CursorTargetInfo_SetNone );
//			}
//			else
//			{
//				const char* szName = pCrow->GetName();
//				WORD wLevel = pCrow->GetLevel();
//				bool bNonDmg = TRUE == pCrow->IsNonDamaged() ? true : false;
//
//				GLWidgetScript::GetInstance().LuaCallFunc< void >( 
//					NSWIDGET_SCRIPT::g_strFunc_CursorTargetInfo_SetMob,
//					"-s, -n, -b",
//					szName, wLevel, bNonDmg );
//			}
//		}
//		break;
//	}
//}

void GLGaeaClient::PartyMemberAddBuff( DWORD dwGaeaID, DWORD dwSkillID, DWORD dwSlot )
{
	if( SKILLFACT_INDEX_NORMAL_START > dwSlot ||
		SKILLFACT_INDEX_NORMAL_END <= dwSlot )
		return;

	if( false == GetMyPartyClient()->isMember( dwGaeaID ) )
		return;

	/*GLWidgetScript::GetInstance().LuaCallFunc< void >( 
		NSWIDGET_SCRIPT::g_strFunc_Party_AddBuff,
		"-dw, -dw, -dw",
		dwGaeaID, dwSkillID, dwSlot );*/
	// 갱신한다;
	//SfReqPartyMemberGetSkillFactInfo(dwGaeaID);
}

//LuaTable GLGaeaClient::SfReqPartyMemberGetSkillFactInfo(DWORD dwGaeaID)
//{
//	LuaTable tbCharBuffInfo( GLWidgetScript::GetInstance().GetLuaState() );
//
//	std::tr1::shared_ptr<GLCharClient> spChar = GetChar(dwGaeaID);
//	if( false == GetMyPartyClient()->isMember( dwGaeaID ) || !spChar )
//		return tbCharBuffInfo;
//
//	int nTableIndex = 0;
//	for( int i = SKILLFACT_INDEX_NORMAL_START; i < SKILLFACT_INDEX_NORMAL_END; ++i )
//	{
//		SSKILLFACT &sSKEFF = spChar->m_sSKILLFACT[ i ];
//		if( sSKEFF.sNATIVEID == NATIVEID_NULL() ||
//			FALSE == sSKEFF.bEnable ||
//			sSKEFF.fAGE <= 0.0f )
//			continue;
//
//		const GLSKILL* pSkill = GLSkillMan::GetInstance().GetData( sSKEFF.sNATIVEID );
//		const float fLife = pSkill->m_sAPPLY.sDATA_LVL[ sSKEFF.wLEVEL ].fLIFE;
//
//		LuaTable tbBuffInfo( GLWidgetScript::GetInstance().GetLuaState() );
//		tbBuffInfo.set( 1, sSKEFF.sNATIVEID.dwID );		// 스킬 아이디;
//		tbBuffInfo.set( 2, i );							// 버프 인덱스
//		tbBuffInfo.set( 3, sSKEFF.fAGE );				// 남은 시간;
//		tbBuffInfo.set( 4, fLife );						// 전체 시간;
//		tbBuffInfo.set( 5, sSKEFF.wLEVEL );				// 스킬 레벨;
//		tbCharBuffInfo.set( ++nTableIndex, tbBuffInfo );
//	}
//
//	return tbCharBuffInfo;
//}
//
//LuaTable GLGaeaClient::SfPartyDistribution_GetItemInfo(void)
//{
//	return m_pPartyTenderManagerClient->GetTenderItem();
//}

//void GLGaeaClient::SfPartyDistribution_AbadonItem(void)
//{
//	if (m_pPartyTenderManagerClient)
//		m_pPartyTenderManagerClient->AbandonItem();
//}
//
//void GLGaeaClient::SfPartyDistribution_SelectItem(DWORD dwSelectIdx)
//{
//	if (m_pPartyTenderManagerClient)
//		m_pPartyTenderManagerClient->SetSelect(dwSelectIdx);
//}
//
//void GLGaeaClient::SfPartyDistribution_Bidding(void)
//{
//	if (m_pPartyTenderManagerClient)
//		m_pPartyTenderManagerClient->TenderItem();
//}


//void GLGaeaClient::SfPartyDistribution_ResetSelectByItemID( DWORD dwSelectIdx )
//{
//	if (m_pPartyTenderManagerClient)
//		m_pPartyTenderManagerClient->ResetSelectByItemID(dwSelectIdx);
//}
//
//void GLGaeaClient::SfResetSelectAll()
//{
//	if (m_pPartyTenderManagerClient)
//		m_pPartyTenderManagerClient->ResetSelectAll();
//}
//
//
//void GLGaeaClient::SfPartyDistribution_SelectMember(DWORD dwSelectIdx)
//{
//	if (m_pPartyTenderManagerClient)
//		m_pPartyTenderManagerClient->SetSelectMember(dwSelectIdx);
//}

//LuaTable GLGaeaClient::SfPartyDistribution_GetMemberInfo(void)
//{
//	if (m_pPartyTenderManagerClient == NULL)
//		return NULL;
//
//	return m_pPartyTenderManagerClient->GetMemberInfo();
//}

//void GLGaeaClient::SfPartyDistribution_TransferItem(void)
//{
//	if ( m_pPartyTenderManagerClient )
//		m_pPartyTenderManagerClient->TransferItem();
//}
//
//void GLGaeaClient::SfPartyDistribution_JunkItem(void)
//{
//	if ( m_pPartyTenderManagerClient )
//		m_pPartyTenderManagerClient->JunkItem();
//}

void GLGaeaClient::PartyMemberRemoveBuff( DWORD dwGaeaID, DWORD dwSkillID, DWORD dwSlot )
{
	if( SKILLFACT_INDEX_NORMAL_START > dwSlot ||
		SKILLFACT_INDEX_NORMAL_END <= dwSlot )
		return;

	if( false == GetMyPartyClient()->isMember( dwGaeaID ) )
		return;

	/*GLWidgetScript::GetInstance().LuaCallFunc< void >(
		NSWIDGET_SCRIPT::g_strFunc_Party_RemoveBuff,
		"-dw, -dw, -dw",
		dwGaeaID, dwSkillID, dwSlot );*/
}

void GLGaeaClient::PartyMemberRemoveAllBuff( DWORD dwGaeaID )
{
	if( false == GetMyPartyClient()->isMember( dwGaeaID ) )
		return;

	/*GLWidgetScript::GetInstance().LuaCallFunc< void >(
		NSWIDGET_SCRIPT::g_strFunc_Party_RemoveAllBuff,
		"-dw", dwGaeaID );*/
}

bool GLGaeaClient::CanComposeSetItem( const STARGETID& sTargetID, DWORD& dwCharClass, SNATIVEID* pPutonItem )
{
	const bool bIsOtherClient = GetCharacter()->GetGaeaID() != sTargetID.GaeaId &&
		sTargetID.GaeaId != 0 && 
		sTargetID.emCrow == CROW_PC;

	if( true == bIsOtherClient )
	{
		std::tr1::shared_ptr< GLCharClient > pOther = GetChar( sTargetID.GaeaId );
		if( NULL != pOther )
		{
			dwCharClass = pOther->GetClass();

			for( WORD wSlot = SLOT_HEADGEAR; wSlot < SLOT_NSIZE_S_2; ++wSlot)
				pPutonItem[ wSlot ] = pOther->GET_SLOT_ITEM( (EMSLOT) wSlot ).sNativeID;
		}
		else
		{
			return false;
		}
	}
	else
	{
		GLCharacter* pChar = GetCharacter();
		if( NULL != pChar )
		{
			dwCharClass = pChar->m_emClass;

			for( WORD wSlot = SLOT_HEADGEAR; wSlot < SLOT_NSIZE_S_2; ++wSlot )
				pPutonItem[ wSlot ] = pChar->m_PutOnItems[ wSlot ].GetNativeID();
		}
		else
		{
			return false;
		}

		if( CROW_ITEM == sTargetID.emCrow )
		{
			SNATIVEID sItemID( sTargetID.GaeaId );
			const SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemID );
			if( NULL == pItem )
				return false;

			dwCharClass = pItem->sBasicOp.dwReqCharClass;
		}
	}

	return true;
}

bool GLGaeaClient::IsWoreSetItem( EMSUIT emSuit, const SNATIVEID& sNID, const SNATIVEID* sPutOnItemCollect )
{
	switch( emSuit )
	{
	case SUIT_HEADGEAR :
	case SUIT_UPPER :
	case SUIT_LOWER :
	case SUIT_HAND :
	case SUIT_FOOT :
		return sPutOnItemCollect[ emSuit ] == sNID;

	case SUIT_HANDHELD :
		return (sPutOnItemCollect[ SLOT_RHAND ] == sNID || sPutOnItemCollect[ SLOT_LHAND ] == sNID);

	case SUIT_NECK :
	case SUIT_WRIST :	// suit 타입으로 hand는 하나지만 slot은 2개다
		return sPutOnItemCollect[ emSuit + 1 ] == sNID;

	case SUIT_FINGER :
		return (sPutOnItemCollect[ SLOT_RFINGER ] == sNID || sPutOnItemCollect[ SLOT_LFINGER ] == sNID);

	case SUIT_VEHICLE :	// 핑거도 마찬가지
		return sPutOnItemCollect[ SLOT_VEHICLE ] == sNID;

	case SUIT_WAISTBAND :
		return sPutOnItemCollect[ SLOT_WAISTBAND ] == sNID;

	case SUIT_EARRING :
		return sPutOnItemCollect[ SLOT_EARRINGS ] == sNID;

	case SUIT_ACCESSORIES :
		return (sPutOnItemCollect[ SLOT_RACCESSORIES ] == sNID || sPutOnItemCollect[ SLOT_LACCESSORIES ] == sNID);

	case SUIT_DECORATION :
		return sPutOnItemCollect[ SLOT_DECORATION ] == sNID;
	}

	return false;
}

void GLGaeaClient::SetItemGroupComposition( const SNATIVEID& sItemID )
{
	const SITEM* pItemData = GLogicData::GetInstance().GetItem( sItemID );
	if( NULL == pItemData )
		return;

	SSETITEM* pSetItem = const_cast< SSETITEM* >( GLItemSet::GetInstance().GetInfoSetItem( pItemData->BasicId() ) );
	if( NULL == pSetItem )
		return;

	STARGETID sTargetID( CROW_ITEM, sItemID.dwID );
	DWORD dwCharClass = 0;
	SNATIVEID sPutOnItem[ SLOT_NSIZE_S_2 ];

	if( false == CanComposeSetItem( sTargetID, dwCharClass, sPutOnItem ) )
		return;

	struct SWoreSetItem
	{
		SNATIVEID	sID;
		std::string strName;
		bool		bWore;

		SWoreSetItem( const SNATIVEID& sID, const std::string& strName, bool bWore )
			: sID( sID )
			, strName( strName )
			, bWore( bWore )
		{
		}
	};

	typedef std::vector< SWoreSetItem >			VEC_WORESETITEM;
	typedef VEC_WORESETITEM::iterator			VEC_WORESETITEM_ITER;
	typedef VEC_WORESETITEM::reverse_iterator	VEC_WORESETITEM_RITER;

	struct SSortSuitSetItem
	{
		bool operator()( SWoreSetItem& sWoreItemL, SWoreSetItem& sWoreItemR )
		{
			SITEM* pItemL = GLItemMan::GetInstance().GetItem( sWoreItemL.sID );
			SITEM* pItemR = GLItemMan::GetInstance().GetItem( sWoreItemR.sID );
			if( NULL == pItemL || NULL == pItemR )
				return false;

			return ( pItemR->Suit() < pItemL->Suit() );
		}
	};

	struct SSortAttackItem
	{
		bool operator()( SWoreSetItem& sWoreItemL, SWoreSetItem& sWoreItemR )
		{
			SITEM* pItemL = GLItemMan::GetInstance().GetItem( sWoreItemL.sID );
			SITEM* pItemR = GLItemMan::GetInstance().GetItem( sWoreItemR.sID );
			if( NULL == pItemL || NULL == pItemR )
				return false;

			return ( pItemL->sSuitOp.emAttack < pItemR->sSuitOp.emAttack );
		}
	};

	// 극강부일 경우만 예외처리한다; 
	// 극강부가 낄 수 있는 무기가 많아 UI에 너무많이 출력된다;
	const DWORD dwExceptionClass =  
		sTargetID.emCrow != CROW_ITEM && (dwCharClass & GLCC_EXTREME) != 0 ? GLCC_EXTREME : 0;

	// 전체 세트 아이템 목록에서 착용가능아이템만 추려낸다;
	VEC_WORESETITEM vecWoreItemList;

	const VNATIVEID& vecItemList = pSetItem->vecItemList;
	int nWoreSetitemCount = 0;

	for( size_t i = 0; i < vecItemList.size(); ++i )
	{
		const SNATIVEID& sItemID = vecItemList[ i ];
		SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemID );
		if( NULL == pItem || (pItem->sBasicOp.dwReqCharClass & (dwCharClass ^ (~dwExceptionClass + 1))) == 0 )
			continue;

		const bool bWoreSetitem = IsWoreSetItem( pItem->Suit(), sItemID, sPutOnItem );
		if( true == bWoreSetitem )
			++nWoreSetitemCount;

		vecWoreItemList.push_back( SWoreSetItem( sItemID, pItem->GetName(), bWoreSetitem ) );
	}

	const int nTotal = static_cast< int >( vecWoreItemList.size() );

	// 착용 부위별 정렬;
	VEC_WORESETITEM_ITER iterBegin = vecWoreItemList.begin();
	VEC_WORESETITEM_ITER iterEnd = vecWoreItemList.end();
	std::sort( iterBegin, iterEnd, SSortSuitSetItem() );

	// 무기 아이템, 공격속성에 따른 정렬;
	iterBegin = vecWoreItemList.begin();
	iterEnd = vecWoreItemList.end();

	VEC_WORESETITEM_RITER riterBegin = vecWoreItemList.rbegin();
	VEC_WORESETITEM_RITER riterEnd = vecWoreItemList.rend();

	for( ; iterBegin != iterEnd; ++iterBegin )
	{
		SITEM* pItem = GLItemMan::GetInstance().GetItem( iterBegin->sID );
		if( NULL == pItem || ITEMATT_NOTHING != pItem->sSuitOp.emAttack )
			break;
	}

	for( ; riterBegin != riterEnd; ++riterBegin )
	{
		SITEM* pItem = GLItemMan::GetInstance().GetItem( riterBegin->sID );
		if( NULL == pItem || ITEMATT_NOTHING != pItem->sSuitOp.emAttack )
			break;
	}

	std::sort( iterBegin, riterBegin.base(), SSortAttackItem() );

	/*LuaTable tbItemList( GLWidgetScript::GetInstance().GetLuaState() );
	int nTableCount = 1;

	iterBegin = vecWoreItemList.begin();
	for( ; iterBegin != vecWoreItemList.end(); ++iterBegin )
	{
		LuaTable tbItem( GLWidgetScript::GetInstance().GetLuaState() );
		tbItem.set( 1, iterBegin->sID.Mid() );
		tbItem.set( 2, iterBegin->sID.Sid() );
		tbItem.set( 3, iterBegin->strName.c_str() );
		tbItem.set( 4, iterBegin->bWore );

		tbItemList.set( nTableCount++, tbItem );
	}

	GLWidgetCallFuncValue args[ 4 ];
	args[ 0 ].SetString( pSetItem->strName );
	args[ 1 ].SetInt( nWoreSetitemCount );
	args[ 2 ].SetInt( nTotal );
	args[ 3 ].SetTable( &tbItemList );

	GLWidgetScript::GetInstance().LuaCallFunc< void >(
		NSWIDGET_SCRIPT::g_strFunc_SetItem_Open, args, 4 );*/
}

void GLGaeaClient::VehicleWithAns(bool bOk)
{
	GLMSG::SNET_WITH_VEHICLE_TAR_ANS NetMsg;
	NetMsg.dwMasterID = m_dwVehicleRidingRequestTarget;
	NetMsg.emAns = bOk ? EMWITH_VEHICLE_FB_OK : EMWITH_VEHICLE_CANCEL;
	NETSENDTOAGENT(&NetMsg);
	m_dwVehicleRidingRequestTarget = -1;
}

void GLGaeaClient::Vehicle_GetoffwithVehicle()
{
	if ( m_pCharacter->VehicleState() == EMVEHICLE_OFF )
		return;

	GLMSG::SNET_GET_OFF_WITH_VEHICLE NetMsg;
	NetMsg.dwGaeaID = m_pCharacter->GetGaeaID();
	NetMsg.bDriver = m_pCharacter->VehicleState() == EMVEHICLE_DRIVER;
	//NetMsg.bDriver = bDriver;
	this->NETSENDTOAGENT(&NetMsg);
}

bool GLGaeaClient::IsCharPrison()
{

	if( m_pCharacter == NULL ) return false;

	PLANDMANCLIENT pLandManClient = GetActiveMap();

	if( pLandManClient == NULL ) return false;

	const SMAPNODE* pMapNode = FindMapNode( pLandManClient->GetBaseMapID());

	if( pMapNode && PRISONLIMIT.IsPrisionMap( pMapNode, m_pCharacter->UserLevel() ) ) {
		return true;
	}

	return false;
}

void GLGaeaClient::SfTournamentTeamTextInfoRecall()
{
	// LeftName
	//GLWidgetScript::GetInstance().LuaCallFunc< void >(
	//	NSWIDGET_SCRIPT::g_strFunc_CTFlag_SetName,
	//	"-b, -s, -dw", true, m_pTournament->m_strLeftTeamName.c_str(), m_pTournament->m_dwLeftTeamNameColor );

	//// RightName
	//GLWidgetScript::GetInstance().LuaCallFunc< void >(
	//	NSWIDGET_SCRIPT::g_strFunc_CTFlag_SetName,
	//	"-b, -s, -dw", false, m_pTournament->m_strRightTeamName.c_str(), m_pTournament->m_dwRightTeamNameColor );
}

void GLGaeaClient::MsgJackpotNotify( NET_MSG_GENERIC* pMsg )
{
	GLMSG::NET_JACKPOT_NOTIFY* pNetMsg = ( GLMSG::NET_JACKPOT_NOTIFY* ) pMsg;

	const SITEM* pITEM = GLogicData::GetInstance().GetItem( pNetMsg->ItemID );
	const std::string CrowName = GLogicData::GetInstance().GetCrowName( pNetMsg->CrowID );
	if ( !pITEM )
	{
		return;
	}
	
	PrintMsgText( NS_UITEXTCOLOR::ORNAGERED, sc::string::format( 
		ID2GAMEINTEXT( "JACKPOT_DROP" ), 
		GetMapName( pNetMsg->MapID ), CrowName.c_str(), pITEM->GetName() ) );

	PrintMsgText( NS_UITEXTCOLOR::ORNAGERED, sc::string::format( 
		ID2GAMEINTEXT( "JACKPOT_PICKUP" ), 
		pNetMsg->szName, pITEM->GetName() ) );
}

bool GLGaeaClient::SfIsWorldBattleStage(void)
{
	return m_pGlobalStage->GetActiveStage()->IsWBStage();
}

const int GLGaeaClient::GetOpenCharacterSlotCount ( void )
{
	return GLCharSlotMan::Instance()->GetOpenSlotCount();
}
//PK Notification - Arvin.BC
void GLGaeaClient::MsgPKNotifBrd  ( NET_MSG_GENERIC* pMsg )
{
	GLMSG::SNET_PK_RANK_HISTORY_UPDATE* pNetMsg = ( GLMSG::SNET_PK_RANK_HISTORY_UPDATE* ) pMsg;
	m_vecPKHistory.push_back( pNetMsg->sPKHistory );

	if ( m_vecPKHistory.size() > PKNOTIF_NUM )
	{
		m_vecPKHistory.erase( m_vecPKHistory.begin() );
	}
}