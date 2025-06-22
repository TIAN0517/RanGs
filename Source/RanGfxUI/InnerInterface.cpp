#include "StdAfx.h"

#include "./StaticUIManager.h"
#include "InnerInterface.h"

#include "../SigmaCore/String/StringFormat.h"
#include "../SigmaCore/Log/LogMan.h"

#include "../EngineLib/GUInterface/BasicVarTextBox.h"
#include "../EngineLib/GUInterface/GameTextControl.h"
#include "../EngineLib/GUInterface/UITextControl.h"
#include "../EngineLib/GUInterface/UIKeyCheck.h"
#include "../EngineLib/GUInterface/NSGUI.h"
#include "../EngineLib/DxTools/RENDERPARAM.h"
#include "../enginelib/DxEffect/Single/DxEffGroupPlayer.h"
#include "../RanLogic/Crow//GLCrowData.h"
#include "../RanLogic/Crow//GLCrowDataMan.h"

#include "../EngineLib/Common/CommonWeb.h"
#include "../EngineLib/Common/DXInputString.h"
#include "../EngineLib/DxResponseMan.h"
#include "../EngineLib/GUInterface/UIMutuallyExclusive.h"


#include "../RanLogic/Item/GLItemMan.h"
#include "../RanLogic/SNS/GLSNSData.h"
#include "../RanLogic/GLogicDataMan.h"

#include "../RanLogicClient/Stage/DxGlobalStage.h"
#include "../RanLogicClient/GLGaeaClient.h"
#include "../RanLogicClient/Party/GLPartyClient.h"
#include "../RanLogicClient/Tutorial/GLBaseTutorial.h" // by luxes.
#include "../RanLogicClient/Land/GLLandManClient.h"
#include "../RanLogicClient/Input/GLInputManager.h"
#include "../RanLogicClient/Club/GLClubClient.h"
#include "../RanLogicClient/GroupChat/GroupChatClient.h"
#include "../RanLogicClient/Party/GLPartyManagerClient.h"
#include "../RanLogicClient/TriggerSystem/ClientRebirthMessage.h"
#include "../RanLogic/GLUseFeatures.h"
#include "../RanLogicClient/Tournament/GLTournamentClient.h"
#include "../Ranlogicclient/GLGaeaClient.h"

#include "../RanLogic/RANPARAM.h"

#include "../RanLogic/Network/s_CHttpPatch.h"
#include "../RanLogic/SNS/GLSNSData.h"

#include "OldUI/Util/ModalWindow.h"
#include "OldUI/Interface/UILeftTopGroup.h"
#include "OldUI/Interface/AdminMessageDisplay.h"
#include "OldUI/Interface/BasicChat.h"
#include "OldUI/Interface/BasicInfoView.h"
#include "OldUI/Interface/BasicQuickSkillSlot.h"
#include "OldUI/Interface/QuestAlarm.h"
#include "OldUI/Interface/SkillTrayTab.h"
#include "OldUI/Interface/SkillTimeDisplay.h"
#include "OldUI/Interface/CDMSafeTimeDisplay.h"
#include "OldUI/Confront/ConftModalWindow.h"
#include "OldUI/Party/PartyModalWindow.h"
#include "OldUI/Item/InventoryWindow.h"
#include "OldUI/Item/RnInventoryWindow.h"
#include "OldUI/Item/ItemShopWindowWeb.h"
#include "OldUI/Display/MapMoveDisplay.h"
#include "OldUI/Display/NameDisplayMan.h"
#include "OldUI/Display/ProgressDisplay.h"
#include "OldUI/Display/GatherGauge.h"
#include "OldUI/Display/CdmRankingDisplay.h"
#include "OldUI/PrivateMarket/PrivateMarketWindow.h"
#include "OldUI/PrivateMarket/ShopItemSearchWindow.h"
#include "OldUI/PrivateMarket/ItemSearchResultWindow.h"
#include "OldUI/Quest/QuestWindow.h"
#include "OldUI/Quest/WindowRenewal/QuestWindowRenewal.h"
#include "OldUI/Message/RebirthDialogue.h"
#include "OldUI/Message/MapRequireCheck.h"
#include "OldUI/Message/SystemMessageWindow.h"
#include "OldUI/Feature/ItemMixWindow.h"
#include "OldUI/Feature/StatsResetWindow.h"
#include "OldUI/Vehicle/BikeGetOffWindow.h"
#include "OldUI/Vehicle/BikeMessage.h"
#include "OldUI/Vehicle/BikeBoostWindow.h"
#include "OldUI/Post/PostBoxWindow.h"
#include "OldUI/SNS/SNSWindow.h"
#include "OldUI/StudentRecord/ActivityWindowPage.h"
#include "OldUI/Char/CharacterWindow.h"
#include "OldUI/Interface/QBoxButton.h" // by luxes.
#include "OldUI/PointShop/PointShopWindow.h"
#include "OldUI/Message/ForceRebirth.h"
#include "OldUI/Display/ItemInfoTooltip.h"
#include "OldUI/Display/NotifyCTFSpurt.h"
#include "OldUI/NPC/DialogueWindowRenewal.h"
#include "OldUI/Item/ItemPreviewWindow.h"
#include "OldUI/ReferChar/ReferCharWindow.h"
#include "OldUI/Char/CharacterView.h"
#include "OldUI/Char/RnCharacterWindowInterface.h"
#include "OldUI/GroupChat/MaxChatWindow.h"
#include "OldUI/Confront/ConftConfirm.h"
#include "OldUI/Feature/RnMapWindow.h"

// Lotto System
#include "OldUI/LottoSystem/GLLottoSystemManUI.h"

#include "OldUI/Display/QuestionItemDisplay.h"
#include "OldUI/Competition/DoublePoint.h"
#include "OldUI/SelectiveformBox/SelectiveformBoxWindow.h"
#include "OldUI/WorldBattleSystem/WorldBattleButton.h"


#include <boost/tr1/tuple.hpp> 
#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>
#include <boost/function.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>

//-----------------------------------------------------------------------------
#include "../SigmaCore/DebugInclude.h"
//-----------------------------------------------------------------------------

#include "./OldUI/Display/SkillInforTooltip.h"

#define CHAT_MACRO_SIZE	10

/*
CInnerInterface& CInnerInterface::GetInstance()
{
	static CInnerInterface Instance;
	return Instance;
}
*/

const float	CInnerInterface::fMENU_LIFTUP = 90.0f;
const int CInnerInterface::nOUTOFRANGE = -1;

CInnerInterface::CInnerInterface(GLGaeaClient* pGaeaClient, EngineDeviceMan* pEngineDevice, bool bToolMode)
    : InterfaceBase(pGaeaClient)
    , m_pEngineDevice(pEngineDevice)
    , m_emConflictReqType(EMCONFT_ONE)
    , m_ConflictReqID(GAEAID_NULL)
    , m_bBlockProgramFound(false)
    , m_dwEventQuestID(NATIVEID_NULL().dwID)
    , m_dwEventQuestStep(UINT_MAX)
    , m_nINFO_DISPLAY_TYPE(EM_INFO_DISPLAY_BASIC)
    , m_bCHANNEL(false)
    , m_bFirstVNGainSysCall(true)
    , m_bDuplicateLogin(FALSE)
    //, m_bArmsSwapReserve(false)
    , m_bItemShopLoad(false)
    , m_bInitUpdateOrder(false)
    , m_INFO_DISPLAY_SKILL_sNativeID_Previous(NATIVEID_NULL())
    , m_INFO_DISPLAY_SKILL_bNextLevel_Previous(FALSE)
    , m_INFO_DISPLAY_SKILL_wLevel_Previous(0)
    , m_bToolMode(bToolMode)
    , m_ReqChaDbNum(0)
    , m_pInvenItemTooltipGroup( NULL )
    , m_pWishItemTooltipGroup( NULL )
    , m_pMarketWishItemTooltipGroup( NULL )
	, m_bBlockModalUI(false)
	, m_pCostumeStatWindow(NULL)
	, m_pSelformBoxWindow(NULL)
	, m_pAnimatedPKStreak(NULL)
	, m_pKillCardManager(NULL)
{
    m_pPath = m_pGaeaClient->GetSubPath();

	ResetControl();
	
	m_mapLinkData.clear();
	m_mapItemLinkData.clear();
	m_mapPartyLinkData.clear();

	m_dwLinkDataIndex = 0;
	m_dwItemLinkDataIndex = 0;
	m_dwPartyLinkDataIndex = 0;
}

CInnerInterface::~CInnerInterface ()
{
}
	void	CInnerInterface::RESET ()
	{
		m_INFO_DISPLAY_ITEM_sItemCustomBACK.GetNativeID() = NATIVEID_NULL ();
		m_INFO_DISPLAY_ITEM_bShopOpenBACK = FALSE;
		m_INFO_DISPLAY_ITEM_bInMarketBACK = FALSE;
		m_INFO_DISPLAY_ITEM_bInPrivateMarketBACK = FALSE;
		m_INFO_DISPLAY_ITEM_bIsWEAR_ITEMBACK = FALSE;
		m_INFO_DISPLAY_ITEM_wPosXBACK = 0;
		m_INFO_DISPLAY_ITEM_wPosYBACK = 0;

	}

	void CInnerInterface::AddTextNoSplit ( CString strText, D3DCOLOR dwColor )
	{
		ADDTEXT_NOSPLIT ( strText, dwColor );
	}

	void CInnerInterface::AddTextLongestLineSplit ( CString strText, D3DCOLOR dwColor )
	{
		ADDTEXT_LONGESTLINE_SPLIT ( strText, dwColor );
	}

	void CInnerInterface::RemoveAllInfo ()
	{
		CLEAR_TEXT ();
	}

	char*	CInnerInterface::GetNumberWithSign ( int nValue )
	{
		static	char szNumber[128];

		if ( 0 < nValue )
		{
			StringCchPrintf ( szNumber, 128, "(+%d)", nValue );
		}
		else if ( nValue < 0 )
		{
			StringCchPrintf ( szNumber, 128, "(%d)", nValue );
		}

		return szNumber;
	}

	void CInnerInterface::AddInfoItemAddon ( int nBasic, int nAddon, CString strFormat )
	{
		CString Text;
		if ( nAddon )
		{
			Text.Format ( "%s:%d%s", strFormat, nBasic, GetNumberWithSign ( nAddon ) );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::PRIVATE );
		}
		else if ( nBasic )
		{
			Text.Format ( "%s:%d", strFormat, nBasic );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::DEFAULT );
		}
	}

	void CInnerInterface::APPEND_ITEM_GRADE ( CString& strOrigin, BYTE uGRADE )
	{
		if ( !uGRADE ) return ;

		CString strGRADE;
		strGRADE.Format ( "[+%d]", uGRADE );
		strOrigin += strGRADE;
	}

	void CInnerInterface::AddInfoItemAddon ( int nBasic, int nAddon, BYTE uGRADE, CString strFormat )
	{
		CString Text;
		if ( nAddon )
		{
			Text.Format ( "%s:%d%s", strFormat, nBasic, GetNumberWithSign( nAddon ) );
			APPEND_ITEM_GRADE ( Text, uGRADE );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::PRIVATE );
		}
		else if ( nBasic )
		{
			Text.Format ( "%s:%d", strFormat, nBasic );
			APPEND_ITEM_GRADE ( Text, uGRADE );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::DEFAULT );
		}
	}

	void CInnerInterface::AddInfoItemAddonRange ( int nBasicMin, int nBasicMax, int nAddon, BYTE uGRADE, CString strFormat )
	{
		CString Text;

		if ( nAddon )
		{
			Text.Format ( "%s:%d%s~%d%s", strFormat, nBasicMin, GetNumberWithSign( nAddon ), nBasicMax, GetNumberWithSign( nAddon ) );
			APPEND_ITEM_GRADE ( Text, uGRADE );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::PRIVATE );
		}
		else if ( nBasicMin || nBasicMax )
		{
			Text.Format ( "%s:%d~%d", strFormat, nBasicMin, nBasicMax );
			APPEND_ITEM_GRADE ( Text, uGRADE );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::DEFAULT );
		}
	}

	void CInnerInterface::AddItemTurnInfo ( const SITEMCUSTOM &sItemCustom, const BOOL bInMarket, const BOOL bInPrivateMarket )
	{
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.GetNativeID() );

		CString strText;

		//switch ( pItemData->sBasicOp.emItemType )
		//{
		//case ITEM_CHARM:
		//case ITEM_ARROW:
		//case ITEM_CURE:
		//case ITEM_GRINDING:
		if ( pItemData->sDrugOp.wPileNum > 1 )
		{
			WORD wPileNum = pItemData->sDrugOp.wPileNum;
			WORD wTurnNum = sItemCustom.wTurnNum;				
			if ( bInPrivateMarket )	//	���� ����
			{
				strText.Format("%s:%d", ID2GAMEWORD("ITEM_TURN_INFO", 0 ), wTurnNum);
				AddTextNoSplit(strText,NS_UITEXTCOLOR::CHARTREUSE);

				return ;
			}

			if ( bInMarket )	wTurnNum = pItemData->GETAPPLYNUM();
			strText.Format("%s:%d/%d", ID2GAMEWORD("ITEM_TURN_INFO", 0 ), wTurnNum, wPileNum);
			AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
		}
		//break;
		//}
	}

	void CInnerInterface::AddInfoPetSkillItem( const SITEMCUSTOM &sItemCustom )
	{
		CString strText;

		SNATIVEID sNativeID = sItemCustom.GetNativeID();
		GLPetClient* pPetClient = GetGaeaClient()->GetPetClient();
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sNativeID );
		SNATIVEID sSkillID = pItemData->sSkillBookOp.sSkill_ID;

		//	Note : ��ų ���� ������.
		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSkillID.wMainID, sSkillID.wSubID );
		if ( pSkill )
		{
			//	�⺻ ����
			//{
			//	//	3. �Ӽ�
			//	strText.Format("%s:%s",ID2GAMEWORD("SKILL_BASIC_INFO", 2), COMMENT::BRIGHT[pSkill->m_sLEARN.emBRIGHT].c_str());
			//	AddTextNoSplit(strText,NS_UITEXTCOLOR::PALEGREEN);
			//}

			//{
			//	//	��ǥ ���ɰŸ�
			//	const SKILL::SBASIC& sBASIC = pSkill->m_sBASIC;		

			//	if ( sBASIC.wTARRANGE )
			//	{
			//		strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_TARGET_RANGE", 0), sBASIC.wTARRANGE);
			//		AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
			//	}
			//}

			//	�̹� ��� ��ų
			if ( pPetClient->ISLEARNED_SKILL( sSkillID ) )
			{
				AddTextNoSplit ( ID2GAMEWORD ( "ITEM_SKILL_CONDITION", 0 ), NS_UITEXTCOLOR::RED );	
				return ;
			}
		}
	}

	void CInnerInterface::AddInfoSkillItem ( const SITEMCUSTOM &sItemCustom )
	{
		CString strText;

		SNATIVEID sNativeID = sItemCustom.GetNativeID();
		GLCharacter* pCharacter = GetGaeaClient()->GetCharacter();	
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sNativeID );
		SNATIVEID sSkillID = pItemData->sSkillBookOp.sSkill_ID;

		//	Note : ��ų ���� ������.
		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSkillID.wMainID, sSkillID.wSubID );
		if ( pSkill )
		{
			//	�⺻ ����
			{
				//	2. ���
				strText.Format("%s:%d",ID2GAMEWORD("SKILL_BASIC_INFO", 1), pSkill->m_sBASIC.dwGRADE);
				AddTextNoSplit(strText,NS_UITEXTCOLOR::PALEGREEN);

				//	3. �Ӽ�
				strText.Format("%s:%s",ID2GAMEWORD("SKILL_BASIC_INFO", 2), COMMENT::BRIGHT[pSkill->m_sLEARN.emBRIGHT].c_str());
				AddTextNoSplit(strText,NS_UITEXTCOLOR::PALEGREEN);

				//	4. ����
				strText.Format ("%s:", ID2GAMEWORD("SKILL_BASIC_INFO", 3) );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_FIGHTER_M )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[0].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_FIGHTER_W )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[6].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ARMS_M )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[1].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ARMS_W )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[7].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ARCHER_M )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[8].c_str() );				
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ARCHER_W )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[2].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_SPIRIT_M )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[9].c_str() );				
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_SPIRIT_W )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[3].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_EXTREME_M )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[4].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_EXTREME_W )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[5].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_SCIENTIST_M )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[10].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_SCIENTIST_W )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[11].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ASSASSIN_M )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[12].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ASSASSIN_W )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[13].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_TRICKER_M )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[14].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_TRICKER_W )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[15].c_str() );

				AddTextNoSplit(strText,NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.dwReqCharClass & pCharacter->m_emClass ) );
			}

			{
				//	��ǥ ���ɰŸ�, �޼�, ������ ����
				const SKILL::SBASIC& sBASIC = pSkill->m_sBASIC;		

				if ( sBASIC.wTARRANGE )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_TARGET_RANGE", 0), sBASIC.wTARRANGE);
					AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
				}

				if ( (sBASIC.emUSE_LITEM && (sBASIC.emUSE_LITEM != glold_108::SKILLATT_NOCARE)) || 
					(sBASIC.emUSE_RITEM && (sBASIC.emUSE_RITEM != glold_108::SKILLATT_NOCARE)) )
				{
					AddTextNoSplit(ID2GAMEWORD("SKILL_CATEGORY", 1),NS_UITEXTCOLOR::LIGHTSKYBLUE);
				}

				CString strUSEITEM;
				if ( sBASIC.emUSE_LITEM )
				{
					if ( sBASIC.emUSE_LITEM != glold_108::SKILLATT_NOCARE )
					{
						strText.Format( "%s:%s", ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM_LR_HAND", 0), COMMENT::SKILLATTACK[sBASIC.emUSE_LITEM].c_str() );
						AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
					}
				}

				if ( sBASIC.emUSE_RITEM )
				{
					if ( sBASIC.emUSE_RITEM != glold_108::SKILLATT_NOCARE )
					{
						strText.Format ( "%s:%s", ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM_LR_HAND", 1), COMMENT::SKILLATTACK[sBASIC.emUSE_RITEM].c_str() );
						AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT );
					}
				}
			}

			//	�̹� ��� ��ų
			if ( pCharacter->GETLEARNED_SKILL ( sSkillID ) )
			{
				AddTextNoSplit ( ID2GAMEWORD ( "ITEM_SKILL_CONDITION", 0 ), NS_UITEXTCOLOR::RED );	
				return ;
			}

			//	�䱸ġ ����
			{
				AddTextNoSplit( ID2GAMEWORD("SKILL_CATEGORY", 7), NS_UITEXTCOLOR::LIGHTSKYBLUE);

				const WORD wLevel = 0;
				SKILL::SLEARN& sLEARN = pSkill->m_sLEARN;
				SKILL::SLEARN_LVL& sLVL = sLEARN.sLVL_STEP[wLevel];

				//	1. �䱸������ų
				SNATIVEID NeedSkillID = pSkill->m_sLEARN.sSKILL;			
				if ( NeedSkillID != NATIVEID_NULL() )
				{
					BOOL bVALID = FALSE;
					BOOL bNeedSkillLevel = FALSE;

					CString strNeedSkillName;
					CString strNeedSkillLevel;

					PGLSKILL pNeedSkill = GLSkillMan::GetInstance().GetData ( NeedSkillID.wMainID, NeedSkillID.wSubID );
					strNeedSkillName.Format("%s:%s", ID2GAMEWORD( "SKILL_ADVANCED_INFO", 0), pNeedSkill->GetName() );
					bVALID = pCharacter->ISLEARNED_SKILL ( NeedSkillID );			

					//	2. �䱸������ų����
					if ( 0 < sLVL.dwSKILL_LVL )
					{
						strNeedSkillLevel.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 1), (sLVL.dwSKILL_LVL + 1) );
						bNeedSkillLevel = TRUE;

						SKILL_MAP& map = pCharacter->m_ExpSkills;				
						SKILL_MAP_ITER iter = map.find ( NeedSkillID.dwID );
						if ( iter != map.end() )
						{
							SCHARSKILL& rCharSkill = (*iter).second;

							//	�� ����
							bVALID = rCharSkill.wLevel >= sLVL.dwSKILL_LVL;						
						}
					}

					AddTextNoSplit(strNeedSkillName,NS_UITEXTCONTROL::GetEvaluateColor ( bVALID ));

					if ( bNeedSkillLevel )
						AddTextNoSplit(strNeedSkillLevel, NS_UITEXTCONTROL::GetEvaluateColor ( bVALID ) );
				}

				//	3. �䱸����ġ
				if ( 0 < sLVL.dwSKP )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 2), sLVL.dwSKP);				
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_dwSkillPoint >= sLVL.dwSKP ) );
				}

				//	4. �䱸����
				if ( 0 < sLVL.dwLEVEL )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 3), sLVL.dwLEVEL);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->GETLEVEL () >= int(sLVL.dwLEVEL) ) );
				}

				//	5. ���� - �ϱ�
				BOOL bValue = TRUE;
				strText.Format ( "%s", COMMENT::BRIGHT[pItemData->sBasicOp.emReqBright].c_str() );
				if ( pItemData->sBasicOp.emReqBright != BRIGHT_BOTH )
				{
					if ( pCharacter->GETBRIGHT() != pItemData->sBasicOp.emReqBright )
					{
						bValue = FALSE;
					}
				}
				AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( bValue ) );


				///*UserNum ItemReq, Juver, 2017/06/27 */
				//if ( pItemData->sBasicOp.dwReqUserNum != 0 )
				//{
				//	strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_EXTRA", 0 ), pItemData->sBasicOp.dwReqUserNum );
				//	AddTextNoSplit(strText,NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.dwReqUserNum == pCharacter->GetUserID() ));
				//}

				//	Stats
				//	1. �䱸��
				if ( 0 < sLVL.sSTATS.wPow )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 0), sLVL.sSTATS.wPow);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.wPow >= sLVL.sSTATS.wPow ) );
				}

				//	2. �䱸ü��
				if ( 0 < sLVL.sSTATS.wStr )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 1), sLVL.sSTATS.wStr);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.wStr >= sLVL.sSTATS.wStr ) );
				}

				//	3. �䱸����
				if ( 0 < sLVL.sSTATS.wSpi )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 2), sLVL.sSTATS.wSpi);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.wSpi >= sLVL.sSTATS.wSpi ));
				}

				//	4. �䱸��ø
				if ( 0 < sLVL.sSTATS.wDex )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 3), sLVL.sSTATS.wDex);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.wDex >= sLVL.sSTATS.wDex ) );
				}

				//	5. �䱸����
				if ( 0 < sLVL.sSTATS.wInt )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 4), sLVL.sSTATS.wInt);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.wInt >= sLVL.sSTATS.wInt ) );
				}

				//	6. �䱸�ٷ�
				if ( 0 < sLVL.sSTATS.wSta )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 5), sLVL.sSTATS.wSta);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.wPow >= sLVL.sSTATS.wSta ) );
				}
			}
		}
	}

	void CInnerInterface::AddTextAddValue( const SITEMCUSTOM & sItemCustom, const ITEM::SSUIT & sSUIT )
	{
		CString strText;
		BOOL bLEAST_HAVE_ONE(TRUE);
		BOOL bADD_HP(FALSE), bADD_MP(FALSE), bADD_SP(FALSE), bADD_MA(FALSE);
		INT arrVALUE[EMADD_SIZE];
		float arrFVALUE[EMADD_SIZE];
		SecureZeroMemory( arrVALUE, sizeof(arrVALUE) );
		SecureZeroMemory( arrFVALUE, sizeof(arrFVALUE) );

		for ( int i = 0; i < ITEM::SSUIT::ADDON_SIZE; ++i )
		{
			if ( sSUIT.sADDON[i].IsNoneType() ) continue;

			EMITEM_ADDON emTYPE = sSUIT.sADDON[i].emTYPE;
			{
				//	NOTE
				//		����ȿ���� �����Ұ�쿡�� Ÿ��Ʋ��
				//		����ϱ� ����, ��� �ϳ��� �����Ҷ�
				//		�Ѹ��ٴ� ���� üũ�Ѵ�.
				if ( bLEAST_HAVE_ONE && emTYPE < EMADDEX_INCR_NONE )
				{
					AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 10 ),NS_UITEXTCOLOR::LIGHTSKYBLUE);
					bLEAST_HAVE_ONE = FALSE;
				}

				ITEM::SSUIT sSUITOp = sSUIT;

				const int nVALUE = sSUITOp.GetAddonValue_CompareExchange< const int >( i, ITEM::bAddonValueInteger[emTYPE] );
				const float fVALUE = sSUITOp.GetAddonValue_CompareExchange< const float >( i, ITEM::bAddonValueInteger[emTYPE] );

				switch ( emTYPE )
				{
				case EMADD_HP:
					if( !bADD_HP )
					{
						arrVALUE[EMADD_HP] = sItemCustom.GETADDHP();
						bADD_HP = TRUE;
					}
					break;
				case EMADD_MP:
					if( !bADD_MP )
					{
						arrVALUE[EMADD_MP] = sItemCustom.GETADDMP();
						bADD_MP = TRUE;
					}
					break;
				case EMADD_SP:
					if( !bADD_SP )
					{
						arrVALUE[EMADD_SP] = sItemCustom.GETADDSP();
						bADD_SP = TRUE;
					}
					break;
				case EMADD_MA:
					if( !bADD_MA )
					{
						arrVALUE[EMADD_MA] = sItemCustom.GETADDMA();
						bADD_MA = TRUE;
					}
					break;
				case EMADD_HITRATE:		arrVALUE[EMADD_HITRATE] += nVALUE;		break;
				case EMADD_AVOIDRATE:	arrVALUE[EMADD_AVOIDRATE] += nVALUE;	break;
				case EMADD_DAMAGE:		arrVALUE[EMADD_DAMAGE] += nVALUE;		break;
				case EMADD_DEFENSE:		arrVALUE[EMADD_DEFENSE] += nVALUE;		break;
				case EMADD_STATS_POW:	arrVALUE[EMADD_STATS_POW] += nVALUE;	break;
				case EMADD_STATS_STR:	arrVALUE[EMADD_STATS_STR] += nVALUE;	break;
				case EMADD_STATS_SPI:	arrVALUE[EMADD_STATS_SPI] += nVALUE;	break;
				case EMADD_STATS_DEX:	arrVALUE[EMADD_STATS_DEX] += nVALUE;	break;
				case EMADD_STATS_INT:	arrVALUE[EMADD_STATS_INT] += nVALUE;	break;
				case EMADD_STATS_STA:	arrVALUE[EMADD_STATS_STA] += nVALUE;	break;
				case EMADD_PA:			arrVALUE[EMADD_PA] += nVALUE;			break;
				case EMADD_SA:			arrVALUE[EMADD_SA] += nVALUE;			break;
				default:				
					{
						if ( ITEM::bAddonValueInteger[emTYPE] )	arrVALUE[emTYPE] += nVALUE;
						else arrFVALUE[emTYPE] += fVALUE;
					}
					break;
				};
			}
		}


		if ( !bLEAST_HAVE_ONE ) bLEAST_HAVE_ONE = TRUE;
		DWORD dwColor = NS_UITEXTCOLOR::ORANGE;
		for( int i=0; i<ITEM::SSUIT::ADDON_SIZE; ++i)
		{
			if ( sSUIT.sADDON[i].IsNoneType() )	continue;

			EMITEM_ADDON emTYPE = sSUIT.sADDON[i].emTYPE;
			if ( bLEAST_HAVE_ONE && emTYPE > EMADDEX_INCR_NONE )
			{
				AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 5 ),NS_UITEXTCOLOR::LIGHTSKYBLUE);
				dwColor = NS_UITEXTCOLOR::GOLD;
				bLEAST_HAVE_ONE = FALSE;
			}

			if ( ITEM::bAddonValueInteger[emTYPE] )
			{
				if( arrVALUE[emTYPE] != 0 )
				{
					strText.Format( "%s:%s", ID2GAMEWORD( "ITEM_ADDON_INFO", emTYPE ), GetNumberWithSign( arrVALUE[emTYPE] ) );
					AddTextNoSplit( strText, dwColor );
				}
			}
			else
			{
				if(  arrFVALUE[emTYPE] != 0.0f )
				{
					if ( arrFVALUE[emTYPE] > 0.0f )
						strText.Format( "%s:+%.2f%%", ID2GAMEWORD( "ITEM_ADDON_INFO", emTYPE ), arrFVALUE[emTYPE] );
					else
						strText.Format( "%s:%.2f%%", ID2GAMEWORD( "ITEM_ADDON_INFO", emTYPE ), arrFVALUE[emTYPE] );
					
					AddTextNoSplit( strText, dwColor );
				}
			}
		}
	}

	void CInnerInterface::AddTextAddValueCostume( SITEM* pItemDisguiseCombine )
	{
		if ( pItemDisguiseCombine )
		{
			CString strText;
			BOOL bLEAST_HAVE_ONE(TRUE);
			INT arrVALUE[EMADD_SIZE];
			float arrFVALUE[EMADD_SIZE];
			SecureZeroMemory( arrVALUE, sizeof(arrVALUE) );
			SecureZeroMemory( arrFVALUE, sizeof(arrFVALUE) );

			for ( int i = 0; i < ITEM::SSUIT::ADDON_SIZE; ++i )
			{
				EMITEM_ADDON emTYPE = pItemDisguiseCombine->sSuitOp.sADDON[i].emTYPE;

				if ( emTYPE != EMADD_NONE )
				{
					if ( bLEAST_HAVE_ONE )
					{
						AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 10 ),NS_UITEXTCOLOR::LIGHTSKYBLUE);
						bLEAST_HAVE_ONE = FALSE;
					}

					const int nVALUE = pItemDisguiseCombine->sSuitOp.GetAddonValue_CompareExchange< const int >( i, ITEM::bAddonValueInteger[emTYPE] );
					const float fVALUE = pItemDisguiseCombine->sSuitOp.GetAddonValue_CompareExchange< const float >( i, ITEM::bAddonValueInteger[emTYPE] );

					switch ( emTYPE )
					{
					case EMADD_HP:			arrVALUE[EMADD_HP] += nVALUE;		break;
					case EMADD_MP:			arrVALUE[EMADD_MP] += nVALUE;		break;
					case EMADD_SP:			arrVALUE[EMADD_SP] += nVALUE;		break;
					case EMADD_MA:			arrVALUE[EMADD_MA] += nVALUE;		break;
					case EMADD_HITRATE:		arrVALUE[EMADD_HITRATE] += nVALUE;		break;
					case EMADD_AVOIDRATE:	arrVALUE[EMADD_AVOIDRATE] += nVALUE;	break;
					case EMADD_DAMAGE:		arrVALUE[EMADD_DAMAGE] += nVALUE;		break;
					case EMADD_DEFENSE:		arrVALUE[EMADD_DEFENSE] += nVALUE;		break;
					case EMADD_STATS_POW:	arrVALUE[EMADD_STATS_POW] += nVALUE;	break;
					case EMADD_STATS_STR:	arrVALUE[EMADD_STATS_STR] += nVALUE;	break;
					case EMADD_STATS_SPI:	arrVALUE[EMADD_STATS_SPI] += nVALUE;	break;
					case EMADD_STATS_DEX:	arrVALUE[EMADD_STATS_DEX] += nVALUE;	break;
					case EMADD_STATS_INT:	arrVALUE[EMADD_STATS_INT] += nVALUE;	break;
					case EMADD_STATS_STA:	arrVALUE[EMADD_STATS_STA] += nVALUE;	break;
					case EMADD_PA:			arrVALUE[EMADD_PA] += nVALUE;			break;
					case EMADD_SA:			arrVALUE[EMADD_SA] += nVALUE;			break;
					default:				
						{
							if ( ITEM::bAddonValueInteger[emTYPE] )	arrVALUE[emTYPE] += nVALUE;
							else arrFVALUE[emTYPE] += fVALUE;
						}
						break;
					};
				}
			}

			/*for( int i=1; i<EMADD_SIZE; ++i)
			{
			if( arrVALUE[i] != 0 )
			{
				strText.Format( "%s:%s", ID2GAMEWORD( "ITEM_ADDON_INFO", i ), GetNumberWithSign( arrVALUE[i] ) );
				AddTextNoSplit( strText, NS_UITEXTCOLOR::ORANGE );
			}
			}*/

			if ( !bLEAST_HAVE_ONE ) bLEAST_HAVE_ONE = TRUE;
			DWORD dwColor = NS_UITEXTCOLOR::HOTPINK;
			for( int i=0; i<ITEM::SSUIT::ADDON_SIZE; ++i)
			{
				if ( pItemDisguiseCombine->sSuitOp.sADDON[i].IsNoneType() )	continue;

				EMITEM_ADDON emTYPE = pItemDisguiseCombine->sSuitOp.sADDON[i].emTYPE;

				if ( bLEAST_HAVE_ONE && emTYPE > EMADDEX_INCR_NONE )
				{
					AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 5 ),NS_UITEXTCOLOR::LIGHTSKYBLUE);
					dwColor = NS_UITEXTCOLOR::HOTPINK;
					bLEAST_HAVE_ONE = FALSE;
				}

				if ( ITEM::bAddonValueInteger[emTYPE] )
				{
					if( arrVALUE[emTYPE] != 0 )
					{
						strText.Format( "%s:%s", ID2GAMEWORD( "ITEM_ADDON_INFO", emTYPE ), GetNumberWithSign( arrVALUE[emTYPE] ) );
						AddTextNoSplit( strText, dwColor );
					}
				}
				else
				{
					if(  arrFVALUE[emTYPE] != 0.0f )
					{
						if ( arrFVALUE[emTYPE] > 0.0f )
							strText.Format( "%s:+%.2f%%", ID2GAMEWORD( "ITEM_ADDON_INFO", emTYPE ), arrFVALUE[emTYPE] );
						else
							strText.Format( "%s:%.2f%%", ID2GAMEWORD( "ITEM_ADDON_INFO", emTYPE ), arrFVALUE[emTYPE] );

						AddTextNoSplit( strText, dwColor );
					}
				}
			}
		}
	}

	void CInnerInterface::CutZero( CString& cstr, int ncount  )
	{
		// '.' ���� ���ٴ� ���� �Ҽ��� ���� �ڸ��� �����Ƿ� ����
		if (cstr.Find('.') == -1)
			return;

		CString temp = cstr;

		//ncount �Ҽ��� ��° �ڸ� ���� �߶��� ���Ѵ�
		for (int i = 0; i < ncount + 1; i++ )
		{
			temp = cstr.Right( 1 );

			if ( temp.Find('0') != -1 || temp.Find('.') != -1 )
				cstr = cstr.Left( cstr.GetLength() - 1 );
			else
				break;
		}
	}

	void CInnerInterface::LOAD_SIMPLE ( const SITEMCUSTOM &sItemCustom )
	{
		if ( m_INFO_DISPLAY_ITEM_sItemCustomBACK == sItemCustom ) return ;

		m_INFO_DISPLAY_ITEM_sItemCustomBACK = sItemCustom;

		RemoveAllInfo();

		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.GetNativeID() );
		if ( !pItemData ) return ;
		
		DWORD dwLevel = pItemData->sBasicOp.emLevel;
		//	�̸�
		AddTextNoSplit ( pItemData->GetName(), COMMENT::ITEMCOLOR[dwLevel] );
	}

	void CInnerInterface::LOAD ( const SITEMCUSTOM &sItemCustom_base, const BOOL bShopOpen, const BOOL bInMarket, const BOOL bInPrivateMarket, const BOOL bIsWEAR_ITEM, WORD wPosX, WORD wPosY, SNATIVEID sNpcNativeID,
		 const BOOL bItemLink /*= FALSE*/, const BOOL bInInventory /*= FALSE*/ )  
	{	
		if ( m_INFO_DISPLAY_ITEM_sItemCustomBACK == sItemCustom_base && m_INFO_DISPLAY_ITEM_bShopOpenBACK == bShopOpen
			&& m_INFO_DISPLAY_ITEM_bInMarketBACK == bInMarket && m_INFO_DISPLAY_ITEM_bInPrivateMarketBACK == bInPrivateMarket 
			&& m_INFO_DISPLAY_ITEM_bIsWEAR_ITEMBACK == bIsWEAR_ITEM && m_INFO_DISPLAY_ITEM_wPosXBACK == wPosX && m_INFO_DISPLAY_ITEM_wPosYBACK == wPosY)	return ;

		m_INFO_DISPLAY_ITEM_sItemCustomBACK = sItemCustom_base;
		m_INFO_DISPLAY_ITEM_bShopOpenBACK = bShopOpen;
		m_INFO_DISPLAY_ITEM_bInMarketBACK = bInMarket;
		m_INFO_DISPLAY_ITEM_bInPrivateMarketBACK = bInPrivateMarket;
		m_INFO_DISPLAY_ITEM_bIsWEAR_ITEMBACK = bIsWEAR_ITEM;
		m_INFO_DISPLAY_ITEM_wPosXBACK = wPosX;
		m_INFO_DISPLAY_ITEM_wPosYBACK = wPosY;

		RemoveAllInfo();

		/*item wrapper, Juver, 2018/01/12 */
		SITEMCUSTOM sItemCustom = sItemCustom_base;

		/*item link, Juver, 2017/07/31 */
		if ( bItemLink )
		{
			AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
		}

		if ( bIsWEAR_ITEM )
		{
			AddTextNoSplit ( ID2GAMEWORD ("WEAR_ITEM" ), NS_UITEXTCOLOR::RED );
		}

		CString strText, strText2;
		BOOL bValue = FALSE;
		int nExtraValue = 0;

		GLCharacter* pCharacter = GetGaeaClient()->GetCharacter();
		SITEM* pItemData = GLogicData::GetInstance().GetItem ( sItemCustom.GetNativeID() );

		if ( !pItemData ) return;

		/*item wrapper, Juver, 2018/01/12 */
		BOOL bWrapped = FALSE;
		std::string strorigname = "";
		if ( sItemCustom.GetBoxWrappedItemID() != NATIVEID_NULL() && pItemData->sBasicOp.emItemType == ITEM_WRAPPED_BOX )
		{
			strorigname = pItemData->GetName();
			bWrapped = TRUE;
		}
		
		if ( bWrapped )
		{
			pItemData = NULL;
			pItemData = GLItemMan::GetInstance().GetItem( sItemCustom.nidDISGUISE );
			sItemCustom.SetNativeID(sItemCustom.nidDISGUISE);
			sItemCustom.nidDISGUISE = NATIVEID_NULL();
			sItemCustom.tDISGUISE = 0;
		}

		if ( !pItemData )
		{
			bWrapped = FALSE;
			pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.GetNativeID() );
		}

		{
			AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 0 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

			//////////////////////////////////////////////////////////////////////////////////////////////////////
			//	BasicInfo
			//	1.������ �̸�		
			BOOL bInsert = FALSE;

			/*item wrapper, Juver, 2018/01/12 */
			if ( bWrapped && strorigname.size() )
				strText.Format ( "%s:%s %s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 0 ), strorigname.c_str(), pItemData->GetName() );
			else
				strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 0 ), pItemData->GetName() );

			BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_NUNE);
			APPEND_ITEM_GRADE ( strText, uGRADE );
			AddTextNoSplit ( strText, NS_UITEXTCOLOR::PALEGREEN );
			
			DWORD dwLevel = (DWORD)pItemData->sBasicOp.emLevel;
//			AddTextNoSplit ( strText, COMMENT::ITEMCOLOR[dwLevel] );

			//	�ڽ���
			if ( sItemCustom.nidDISGUISE!=SNATIVEID(false) )
			{
				SITEM* pDisguiseData = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );

				strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 3 ), pDisguiseData->GetName() );			
				AddTextNoSplit ( strText, NS_UITEXTCOLOR::GREENYELLOW );
			}

			//	�ڽ��� �Ⱓ ǥ��
			if ( sItemCustom.tDISGUISE!=0 )
			{				
				CTime cTime(sItemCustom.tDISGUISE);
				if ( cTime.GetYear()!=1970 )
				{
					CString strExpireDate;
					strExpireDate = MakeString( ID2GAMEWORD("ITEM_EXPIRE_DATE"),
																				(cTime.GetYear ()%2000), 
																				cTime.GetMonth (), 
																				cTime.GetDay (), 
																				cTime.GetHour (), 
																				cTime.GetMinute () );

					strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 7 ), strExpireDate );			
					AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKORANGE );
				}
			}

			DWORD dwCOMMISSION_MONEY = 0;

			//	2.����
			if ( pItemData->sBasicOp.dwBuyPrice || bInPrivateMarket )
			{
				if ( bInPrivateMarket )
				{
					bool bOPENER;
					DWORD dwPrivateMarketID;
					GetPrivateMarketInfo ( bOPENER, dwPrivateMarketID );
					D3DCOLOR dwColor = NS_UITEXTCOLOR::GOLD;
					if ( bOPENER )	//	�Ĵ� ���
					{
						GLPrivateMarket &sPMarket = GetGaeaClient()->GetCharacter()->m_sPMarket;

						const SSALEITEM *pSALE = sPMarket.GetItem ( SNATIVEID(wPosX,wPosY) ); 
						if ( pSALE )
						{
							CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( pSALE->llPRICE, 3, "," );
							strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 5 ), strMoney );	

							if ( pSALE->llPRICE > 100000000 && pSALE->llPRICE < 499999999 ) // 100m to 499m
								dwColor = NS_UITEXTCOLOR::VIOLET;
							else if ( pSALE->llPRICE >= 500000000 && pSALE->llPRICE <= 999999999 ) // 500m to 999m
								dwColor = NS_UITEXTCOLOR::FORESTGREEN;
							else if ( pSALE->llPRICE > 999999999 ) // 500m to 999m
								dwColor = NS_UITEXTCOLOR::DODGERBLUE;

							AddTextNoSplit ( strText, dwColor );
						}						
					}
					else		//	��� ���
					{
						//std::tr1::shared_ptr<GLCharClient> pCLIENT = GetGaeaClient()->GetChar ( dwPrivateMarketID );
						//if ( !pCLIENT ) return ;

						//GLPrivateMarket &sPMarket = pCLIENT->m_sPMarket;

						//const SSALEITEM *pSALE = sPMarket.GetItem ( SNATIVEID(wPosX,wPosY) ); 
						//if ( pSALE )
						//{
						//	CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( pSALE->llPRICE, 3, "," );
						//	strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 5 ), strMoney );		

						//	if ( pSALE->llPRICE > 100000000 && pSALE->llPRICE < 499999999 ) // 100m to 499m
						//		dwColor = NS_UITEXTCOLOR::VIOLET;
						//	else if ( pSALE->llPRICE >= 500000000 && pSALE->llPRICE <= 999999999 ) // 500m to 999m
						//		dwColor = NS_UITEXTCOLOR::FORESTGREEN;
						//	else if ( pSALE->llPRICE > 999999999 ) // 500m to 999m
						//		dwColor = NS_UITEXTCOLOR::DODGERBLUE;

						//	AddTextNoSplit ( strText, dwColor );
						//}
					}
				}				
				else if ( bShopOpen && bInMarket )	// �춧 ����
				{
					LONGLONG dwNpcSellPrice = 0;

					volatile LONGLONG dwPrice = 0;
					volatile float fSHOP_RATE = GetGaeaClient()->GetCharacter()->GetBuyRate();
					volatile float fSHOP_RATE_C = fSHOP_RATE * 0.01f;

					if( sNpcNativeID.wMainID != 0 && sNpcNativeID.wSubID != 0 )
					{
						PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( sNpcNativeID );
						if( pCrowData != NULL )
						{
							LONGLONG dwNpcPrice = pCrowData->GetNpcSellPrice( pItemData->sBasicOp.sNativeID.dwID );
							if( dwNpcPrice == 0 )
							{								
								dwNpcSellPrice = pItemData->sBasicOp.dwBuyPrice;
								dwPrice = DWORD ( (float)dwNpcSellPrice * fSHOP_RATE_C );
							}else{
								dwNpcSellPrice = dwNpcPrice;
								dwPrice = dwNpcSellPrice;								
							}
						}

					}
					
					CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( dwPrice, 3, "," );

					strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 1), strMoney );

					//	Ŀ�̼� �׼�
					dwCOMMISSION_MONEY = dwPrice - dwNpcSellPrice;
					
					D3DCOLOR dwColor = NS_UITEXTCOLOR::RED;
					if( dwPrice <= GetGaeaClient()->GetCharacterLogic().m_lnMoney )
					{
						dwColor = NS_UITEXTCOLOR::PALEGREEN;

						if ( dwPrice > 100000000 && dwPrice < 499999999 ) // 100m to 499m
							dwColor = NS_UITEXTCOLOR::VIOLET;
						else if ( dwPrice >= 500000000 && dwPrice <= 999999999 ) // 500m to 999m
							dwColor = NS_UITEXTCOLOR::FORESTGREEN;
						else if ( dwPrice > 999999999 ) // 500m to 999m
							dwColor = NS_UITEXTCOLOR::DODGERBLUE;
					}
					AddTextNoSplit ( strText, dwColor );
				}
				else if ( bShopOpen ) // �ȶ� ����
				{
					D3DCOLOR dwColor = NS_UITEXTCOLOR::PALEGREEN;
					volatile float fSHOP_RATE = GetGaeaClient()->GetCharacter()->GetSaleRate();
					volatile float fSALE_DISCOUNT = fSHOP_RATE * 0.01f;

					volatile DWORD dwPrice = pItemData->GETSELLPRICE ( sItemCustom.wTurnNum );
					volatile DWORD dwSALE_PRICE = DWORD ( dwPrice * fSALE_DISCOUNT );					

					//	Ŀ�̼� �׼�
					volatile DWORD dwDISPRICE = pItemData->GETSELLPRICE ( sItemCustom.wTurnNum );
					dwCOMMISSION_MONEY = dwDISPRICE - dwSALE_PRICE;

					CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( dwSALE_PRICE, 3, "," );
					strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 1), strMoney );

					if ( dwSALE_PRICE > 100000000 && dwSALE_PRICE < 499999999 ) // 100m to 499m
						dwColor = NS_UITEXTCOLOR::VIOLET;
					else if ( dwSALE_PRICE >= 500000000 && dwSALE_PRICE <= 999999999 ) // 500m to 999m
						dwColor = NS_UITEXTCOLOR::FORESTGREEN;
					else if ( dwSALE_PRICE > 999999999 ) // 500m to 999m
						dwColor = NS_UITEXTCOLOR::DODGERBLUE;

					AddTextNoSplit ( strText, dwColor );
				}
			}

			//	������ ǥ��
			if ( dwCOMMISSION_MONEY )
			{
				CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( dwCOMMISSION_MONEY, 3, "," );
				strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 6), strMoney );

				AddTextNoSplit ( strText, NS_UITEXTCOLOR::PALEGREEN );
			}

			//	����ǥ��
			AddItemTurnInfo ( sItemCustom, bInMarket, bInPrivateMarket );

			//	�Ⱓǥ��
			if ( sItemCustom.IsTimeLimit() )
			{
				CTime cTime(sItemCustom.tBORNTIME);
				if ( cTime.GetYear()!=1970 )
				{
					CTimeSpan sLMT(pItemData->sDrugOp.tTIME_LMT);
					cTime += sLMT;

					CString strExpireDate;
					strExpireDate = MakeString ( ID2GAMEWORD("ITEM_EXPIRE_DATE"),
						(cTime.GetYear ()%2000), cTime.GetMonth (), cTime.GetDay (), cTime.GetHour (), cTime.GetMinute () );

					strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 4 ), strExpireDate );			
					AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKORANGE );
				}
			}

			//	��������->�䱸 ����
			if ( pItemData->sBasicOp.wReqLevelDW || pItemData->sBasicOp.wReqLevelUP )
			{
				bool bReqLevel = true;
				strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 0), pItemData->sBasicOp.wReqLevelDW );
									
				if ( pItemData->sBasicOp.wReqLevelUP )
				{
					strText2.Format ( "~%d",pItemData->sBasicOp.wReqLevelUP );
					strText += strText2; 
					bReqLevel = (pItemData->sBasicOp.wReqLevelUP >= pCharacter->m_wLevel);
				}

				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqLevelDW <= pCharacter->m_wLevel && bReqLevel ) );
			}

			//	���� ���ð�
			if ( pItemData->sBasicOp.IsCoolTime() )
			{
				CString strTime = "";
				CTimeSpan cCoolTime( pItemData->sBasicOp.dwCoolTime );
				
				if ( cCoolTime.GetHours() > 0 )	
					strTime += MakeString( "%d%s ", cCoolTime.GetHours(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 0 ) );
				if ( cCoolTime.GetMinutes() > 0 )	
					strTime += MakeString( "%d%s ", cCoolTime.GetMinutes(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 1 ) );
				if ( cCoolTime.GetSeconds() > 0 )	
					strTime += MakeString( "%d%s ", cCoolTime.GetSeconds(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 2 ) );

				strText.Format ( "%s : %s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 8 ), strTime );			
				AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKORANGE );	
			}

			//	�����ð�


			GLCharacter* pCharacter = GetGaeaClient()->GetCharacter();
			if ( pCharacter && pCharacter->IsCoolTime( pItemData->sBasicOp.sNativeID ) )
			{
				CString strTime = "";
				__time64_t tCurTime =  GetGaeaClient()->GetCurrentTime().GetTime();
				__time64_t tCoolTime = pCharacter->GetMaxCoolTime( pItemData->sBasicOp.sNativeID );				

				CTimeSpan cReTime( tCoolTime - tCurTime );

                if ( cReTime.GetHours() > 0 )	
					strTime += MakeString( "%d%s ", cReTime.GetHours(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 0 ) );
				if ( cReTime.GetMinutes() > 0 )	
					strTime += MakeString( "%d%s ", cReTime.GetMinutes(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 1 ) );
				if ( cReTime.GetSeconds() > 0 )	
					strTime += MakeString( "%d%s ", cReTime.GetSeconds(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 2 ) );

				strText.Format ( "%s : %s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 9 ), strTime );			
				AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKORANGE );
			}

			//////////////////////////////////////////////////////////////////////////////////////////////////////
			//	AdvInfo
			SCHARSTATS& rItemStats = pItemData->sBasicOp.sReqStats;
			SCHARSTATS& rCharStats = pCharacter->m_sSUMSTATS;
			EMITEM_TYPE emItemType = pItemData->sBasicOp.emItemType;

			switch ( emItemType )
			{
			case ITEM_SUIT:
			case ITEM_VEHICLE:
			case ITEM_REVIVE:
			case ITEM_ANTI_DISAPPEAR:
			case ITEM_INC_GRINDINGRATE:
			case ITEM_ANTI_RESET:
				{
					AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 1 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					BYTE uGRADE = 0;

					//	���ݷ�
					GLPADATA &sDamage = sItemCustom.getdamage();
					nExtraValue = sItemCustom.GETGRADE_DAMAGE();
					uGRADE = sItemCustom.GETGRADE(EMGRINDING_DAMAGE);
					AddInfoItemAddonRange ( sDamage.wLow, sDamage.wHigh, nExtraValue, uGRADE, ID2GAMEWORD("ITEM_ADVANCED_INFO", 0) );

					//	���ġ
					nExtraValue = sItemCustom.GETGRADE_DAMAGE();
					if ( nExtraValue )
					{
						uGRADE = sItemCustom.GETGRADE(EMGRINDING_DAMAGE);
						strText.Format ( "%s:+%d", ID2GAMEWORD("ITEM_ADVANCED_INFO", 8), nExtraValue );
						APPEND_ITEM_GRADE ( strText, uGRADE );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::PRIVATE );
					}

					//	����
					short nDefense = sItemCustom.getdefense();
					nExtraValue = sItemCustom.GETGRADE_DEFENSE();
					uGRADE = sItemCustom.GETGRADE(EMGRINDING_DEFENSE);
					AddInfoItemAddon ( nDefense, nExtraValue, uGRADE, ID2GAMEWORD("ITEM_ADVANCED_INFO", 1) );

					//	���� �Ÿ�
					if ( sItemCustom.GETATTRANGE() )
					{
						strText.Format("%s:%d",ID2GAMEWORD("ITEM_ADVANCED_INFO", 2), sItemCustom.GETATTRANGE() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::DEFAULT );
					}

					//	���߷�
					if ( sItemCustom.GETHITRATE() )
					{
						nExtraValue = 0;
						AddInfoItemAddon ( sItemCustom.GETHITRATE(), nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 3) );		
					}

					//	ȸ����
					if ( sItemCustom.GETAVOIDRATE() )
					{
						nExtraValue = 0;
						AddInfoItemAddon ( sItemCustom.GETAVOIDRATE(), nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 4) );
					}

					//	������ġ
					strText.Format("%s:%s ", ID2GAMEWORD("ITEM_ADVANCED_INFO", 5), COMMENT::ITEMSUIT[pItemData->sSuitOp.emSuit].c_str() );
					AddTextNoSplit (strText, NS_UITEXTCOLOR::DEFAULT );

					//	���� �Ӽ�
					if ( pItemData->sSuitOp.emAttack != ITEMATT_NOTHING )
					{
						strText.Format("%s:%s", ID2GAMEWORD("ITEM_ADVANCED_INFO", 6), COMMENT::ITEMATTACK[pItemData->sSuitOp.emAttack].c_str() );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::DEFAULT );
					}

					//	SP �Ҹ�
					const WORD wReqSP = sItemCustom.GETREQ_SP();
					if ( 0 < wReqSP )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO", 7), wReqSP );
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::ORANGE );
					}
				
					//	��������-------------------------------------------------------------------------------------
					//	��������-���밡�� Ŭ����
					AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 2 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					if ( pItemData->sBasicOp.dwReqSchool!=GLSCHOOL_ALL )
					{
						CString strSchool;
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_00)
						{
							strSchool+=GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_00)].c_str();
							strSchool+=" ";
						}
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_01)
						{
							strSchool+=GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_01)].c_str();
							strSchool+=" ";
						}
						if ( pItemData->sBasicOp.dwReqSchool & GLSCHOOL_02)
						{
							strSchool+=GLCONST_CHAR::strSCHOOLNAME[school2index(GLSCHOOL_02)].c_str();
							strSchool+=" ";
						}						

						AddTextNoSplit ( strSchool, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.dwReqSchool & index2school(pCharacter->m_wSchool) ) );
					}

					/*if( sItemCustom.nidDISGUISE != NATIVEID_NULL() )
					{
						SITEM* pItemDisguise = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
						DWORD dwReqCharClass_Disguise = pItemDisguise->sBasicOp.dwReqCharClass;
						DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;
						CString szClass;

						if( dwReqCharClass )
						{
							if ( (dwReqCharClass==GLCC_ALL_TRICKER) || (dwReqCharClass_Disguise==GLCC_ALL_TRICKER) || (dwReqCharClass==GLCC_ALL_ASSASSIN) || (dwReqCharClass_Disguise==GLCC_ALL_ASSASSIN)
								|| (dwReqCharClass==GLCC_ALL_SCIENTIST) || (dwReqCharClass_Disguise==GLCC_ALL_SCIENTIST) || (dwReqCharClass==GLCC_ALL_ACTOR) || (dwReqCharClass_Disguise==GLCC_ALL_ACTOR) )
							{
								szClass = ID2GAMEWORD("ITEM_ALL_CLASS");
							}
							else
							{
								if ( (dwReqCharClass&GLCC_FIGHTER_M) && (dwReqCharClass_Disguise&GLCC_FIGHTER_M) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_FIGHTER_W) && (dwReqCharClass_Disguise&GLCC_FIGHTER_W) )	{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_ARMS_M)  && (dwReqCharClass_Disguise&GLCC_ARMS_M) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_ARMS_W)  && (dwReqCharClass_Disguise&GLCC_ARMS_W) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_ARCHER_M)  && (dwReqCharClass_Disguise&GLCC_ARCHER_M) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_ARCHER_W)  && (dwReqCharClass_Disguise&GLCC_ARCHER_W) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_SPIRIT_M)  && (dwReqCharClass_Disguise&GLCC_SPIRIT_M) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_SPIRIT_W)  && (dwReqCharClass_Disguise&GLCC_SPIRIT_W) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_EXTREME_M)  && (dwReqCharClass_Disguise&GLCC_EXTREME_M) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_EXTREME_W)  && (dwReqCharClass_Disguise&GLCC_EXTREME_W) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_SCIENTIST_M)  && (dwReqCharClass_Disguise&GLCC_SCIENTIST_M) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENTIST_M)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_SCIENTIST_W)  && (dwReqCharClass_Disguise&GLCC_SCIENTIST_W) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENTIST_W)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_ASSASSIN_M)  && (dwReqCharClass_Disguise&GLCC_ASSASSIN_M) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_M)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_ASSASSIN_W)  && (dwReqCharClass_Disguise&GLCC_ASSASSIN_W) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_W)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_TRICKER_M)  && (dwReqCharClass_Disguise&GLCC_TRICKER_M) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TRICKER_M)].c_str();	szClass+=" ";
								}

								if ( (dwReqCharClass&GLCC_TRICKER_W)  && (dwReqCharClass_Disguise&GLCC_TRICKER_W) ){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TRICKER_W)].c_str();
								}
							}

							strText.Format( "%s", szClass.GetString() );
							AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor( 
								(dwReqCharClass&pCharacter->m_emClass) && (dwReqCharClass_Disguise&pCharacter->m_emClass) ) );
						}
					}
					else
					{
						DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;
						CString szClass;
						
						if( dwReqCharClass )
						{
							if ( (dwReqCharClass==GLCC_ALL_TRICKER) || (dwReqCharClass==GLCC_ALL_ASSASSIN) || (dwReqCharClass==GLCC_ALL_SCIENTIST) || (dwReqCharClass==GLCC_ALL_ACTOR) )
							{
								szClass = ID2GAMEWORD("ITEM_ALL_CLASS");
							}
							else
							{
								if ( dwReqCharClass & GLCC_FIGHTER_M){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_FIGHTER_W){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_ARMS_M){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_ARMS_W){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_ARCHER_M){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_ARCHER_W){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_SPIRIT_M){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_SPIRIT_W){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_EXTREME_M){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_EXTREME_W){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_SCIENTIST_M){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENTIST_M)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_SCIENTIST_W){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENTIST_W)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_ASSASSIN_M){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_M)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_ASSASSIN_W){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_W)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_TRICKER_M){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TRICKER_M)].c_str();	szClass+=" ";
								}

								if ( dwReqCharClass & GLCC_TRICKER_W){
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TRICKER_W)].c_str();
								}
							}

							strText.Format( "%s", szClass.GetString() );
							AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( dwReqCharClass & pCharacter->m_emClass ) );
						}
					}*/

					if( sItemCustom.nidDISGUISE != NATIVEID_NULL() )
					{
						const SITEM* pItemDisguise = GLogicData::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
						DWORD dwReqCharClass_Disguise = pItemDisguise->sBasicOp.dwReqCharClass;
						DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;
						CString szClass;

						if( dwReqCharClass )
						{
							if ( (dwReqCharClass==GLCC_ALL_SCIENTIST) && (dwReqCharClass_Disguise==GLCC_ALL_SCIENTIST) || (dwReqCharClass==GLCC_ALL_ASSASSIN) && (dwReqCharClass_Disguise==GLCC_ALL_ASSASSIN) 
								|| (dwReqCharClass==GLCC_ALL_TRICKER) && (dwReqCharClass_Disguise==GLCC_ALL_TRICKER))
							{
								szClass = ID2GAMEWORD("ITEM_ALL_CLASS");
							}
							else
							{
								if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
								{
									if ( (dwReqCharClass&GLCC_FIGHTER_M) && (dwReqCharClass_Disguise&GLCC_FIGHTER_M) )
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_FIGHTER_M)).GetString();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_FIGHTER_W) && (dwReqCharClass_Disguise&GLCC_FIGHTER_W) )
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_FIGHTER_W)).GetString();
										szClass+=" ";
									}

									if ( (dwReqCharClass&GLCC_ARMS_M)  && (dwReqCharClass_Disguise&GLCC_ARMS_M) )
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_ARMS_M)).GetString();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_ARMS_W)  && (dwReqCharClass_Disguise&GLCC_ARMS_W) )
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_ARMS_W)).GetString();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_ARCHER_M)  && (dwReqCharClass_Disguise&GLCC_ARCHER_M) )
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_ARCHER_M)).GetString();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_ARCHER_W)  && (dwReqCharClass_Disguise&GLCC_ARCHER_W) )
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_ARCHER_W)).GetString();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_SPIRIT_M)  && (dwReqCharClass_Disguise&GLCC_SPIRIT_M) )
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_SPIRIT_M)).GetString();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_SPIRIT_W)  && (dwReqCharClass_Disguise&GLCC_SPIRIT_W) )
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_SPIRIT_W)).GetString();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_EXTREME_M)  && (dwReqCharClass_Disguise&GLCC_EXTREME_M) )
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_EXTREME_M)).GetString();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_EXTREME_W)  && (dwReqCharClass_Disguise&GLCC_EXTREME_W) )
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_EXTREME_W)).GetString();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_SCIENTIST_M)  && (dwReqCharClass_Disguise&GLCC_SCIENTIST_M) )
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_SCIENTIST_M)).GetString();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_SCIENTIST_W)  && (dwReqCharClass_Disguise&GLCC_SCIENTIST_W) )
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_SCIENTIST_W)).GetString();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_ASSASSIN_M)  && (dwReqCharClass_Disguise&GLCC_ASSASSIN_M) )
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_ASSASSIN_M)).GetString();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_ASSASSIN_W)  && (dwReqCharClass_Disguise&GLCC_ASSASSIN_W) )
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_ASSASSIN_W)).GetString();
										szClass+=" ";
									}
								}
								else
								{
									if ( (dwReqCharClass&GLCC_FIGHTER_M) && (dwReqCharClass_Disguise&GLCC_FIGHTER_M) )
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_FIGHTER_W) && (dwReqCharClass_Disguise&GLCC_FIGHTER_W) )
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str();
										szClass+=" ";
									}

									if ( (dwReqCharClass&GLCC_ARMS_M)  && (dwReqCharClass_Disguise&GLCC_ARMS_M) )
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_ARMS_W)  && (dwReqCharClass_Disguise&GLCC_ARMS_W) )
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_ARCHER_M)  && (dwReqCharClass_Disguise&GLCC_ARCHER_M) )
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_ARCHER_W)  && (dwReqCharClass_Disguise&GLCC_ARCHER_W) )
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_SPIRIT_M)  && (dwReqCharClass_Disguise&GLCC_SPIRIT_M) )
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_SPIRIT_W)  && (dwReqCharClass_Disguise&GLCC_SPIRIT_W) )
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_EXTREME_M)  && (dwReqCharClass_Disguise&GLCC_EXTREME_M) )
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_EXTREME_W)  && (dwReqCharClass_Disguise&GLCC_EXTREME_W) )
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_SCIENTIST_M)  && (dwReqCharClass_Disguise&GLCC_SCIENTIST_M) )
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENTIST_M)].c_str();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_SCIENTIST_W)  && (dwReqCharClass_Disguise&GLCC_SCIENTIST_W) )
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENTIST_W)].c_str();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_ASSASSIN_M)  && (dwReqCharClass_Disguise&GLCC_ASSASSIN_M) )
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_M)].c_str();
										szClass+=" ";
									}
									if ( (dwReqCharClass&GLCC_ASSASSIN_W)  && (dwReqCharClass_Disguise&GLCC_ASSASSIN_W) )
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_W)].c_str();
										szClass+=" ";
									}
								}
								if ( (dwReqCharClass&GLCC_ASSASSIN_M)  && (dwReqCharClass_Disguise&GLCC_ASSASSIN_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ASSASSIN_W)  && (dwReqCharClass_Disguise&GLCC_ASSASSIN_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_TRICKER_M)  && (dwReqCharClass_Disguise&GLCC_TRICKER_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TRICKER_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_TRICKER_W)  && (dwReqCharClass_Disguise&GLCC_TRICKER_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TRICKER_W)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ETC_M)  && (dwReqCharClass_Disguise&GLCC_ETC_M) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ETC_M)].c_str();
									szClass+=" ";
								}
								if ( (dwReqCharClass&GLCC_ETC_W)  && (dwReqCharClass_Disguise&GLCC_ETC_W) )
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ETC_W)].c_str();
									szClass+=" ";
								}
							}

							strText.Format( "%s", szClass.GetString() );
							INFO_DISPLAY_ITEM_AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor( 
								(dwReqCharClass&pCharacter->m_emClass) && (dwReqCharClass_Disguise&pCharacter->m_emClass) ) );
						}
					}
					else
					{
						DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;
						CString szClass;

						if( dwReqCharClass )
						{
							if ( dwReqCharClass==GLCC_ALL_TRICKER || dwReqCharClass==GLCC_ALL_ASSASSIN || dwReqCharClass==GLCC_ALL_SCIENTIST  )
							{
								szClass = ID2GAMEWORD("ITEM_ALL_CLASS");
							}
							else
							{
								if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
								{
									if ( dwReqCharClass & GLCC_FIGHTER_M)
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_FIGHTER_M)).GetString();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_FIGHTER_W)
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_FIGHTER_W)).GetString();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_ARMS_M)
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_ARMS_M)).GetString();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_ARMS_W)
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_ARMS_W)).GetString();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_ARCHER_M)
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_ARCHER_M)).GetString();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_ARCHER_W)
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_ARCHER_W)).GetString();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_SPIRIT_M)
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_SPIRIT_M)).GetString();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_SPIRIT_W)
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_SPIRIT_W)).GetString();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_EXTREME_M)
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_EXTREME_M)).GetString();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_EXTREME_W)
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_EXTREME_W)).GetString();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_SCIENTIST_M)
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_SCIENTIST_M)).GetString();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_SCIENTIST_W)
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_SCIENTIST_W)).GetString();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_ASSASSIN_M)
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_ASSASSIN_M)).GetString();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_ASSASSIN_W)
									{
										szClass+=CGameTextMan::GetInstance().GetCommentText("CHARCLASS",CharClassToIndex(GLCC_ASSASSIN_W)).GetString();
										szClass+=" ";
									}
								}
								else
								{
									if ( dwReqCharClass & GLCC_FIGHTER_M)
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_FIGHTER_W)
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_ARMS_M)
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_ARMS_W)
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_ARCHER_M)
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_ARCHER_W)
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_SPIRIT_M)
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_SPIRIT_W)
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_EXTREME_M)
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_EXTREME_W)
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_SCIENTIST_M)
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENTIST_M)].c_str();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_SCIENTIST_W)
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENTIST_W)].c_str();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_ASSASSIN_M)
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_M)].c_str();
										szClass+=" ";
									}
									if ( dwReqCharClass & GLCC_ASSASSIN_W)
									{
										szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_W)].c_str();
										szClass+=" ";
									}
								}
								if ( dwReqCharClass & GLCC_ASSASSIN_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ASSASSIN_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASSIN_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_TRICKER_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TRICKER_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_TRICKER_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_TRICKER_W)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ETC_M)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ETC_M)].c_str();
									szClass+=" ";
								}
								if ( dwReqCharClass & GLCC_ETC_W)
								{
									szClass+=COMMENT::CHARCLASS[CharClassToIndex(GLCC_ETC_W)].c_str();
									szClass+=" ";
								}
							}

							strText.Format( "%s", szClass.GetString() );
							INFO_DISPLAY_ITEM_AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( dwReqCharClass & pCharacter->m_emClass ) );
						}
					}

					//	���� - �ϱ�
					bValue = TRUE;
					strText.Format ( "%s", COMMENT::BRIGHT[pItemData->sBasicOp.emReqBright].c_str() );
					if ( pItemData->sBasicOp.emReqBright != BRIGHT_BOTH )
					{
						if ( pCharacter->GETBRIGHT() != pItemData->sBasicOp.emReqBright )
						{
							bValue = FALSE;
						}
					}
					AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( bValue ) );					

					//	��������->����ġ
					if ( pItemData->sBasicOp.wReqPA )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 1), pItemData->sBasicOp.wReqPA );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqPA <= pCharacter->m_nPA_ADD ) );
					}
					//	��������->���ġ
					if ( pItemData->sBasicOp.wReqSA )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 2 ), pItemData->sBasicOp.wReqSA );
						AddTextNoSplit(strText,NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqSA <= pCharacter->m_nSA_ADD ));
					}

					//	Stats
					//	��������->��
					if ( rItemStats.wPow )
					{
						strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 0 ), rItemStats.wPow );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.wPow <= rCharStats.wPow ) );
					}
					//	��������->ü��
					if ( rItemStats.wStr )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 1 ), rItemStats.wStr );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.wStr <= rCharStats.wStr ) );
					}
					//	��������->���ŷ�
					if ( rItemStats.wSpi)
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 2 ), rItemStats.wSpi );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.wSpi <= rCharStats.wSpi ) );
					}
					//	��������->��ø��
					if ( rItemStats.wDex )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 3 ), rItemStats.wDex );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.wDex <= rCharStats.wDex ) );
					}
					//	��������->����
					if ( rItemStats.wInt  )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 4 ), rItemStats.wInt );
						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.wInt <= rCharStats.wInt ) );
					}
					//	��������->�ٷ�
					if ( rItemStats.wSta )
					{
						strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 5 ), rItemStats.wSta );
						AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.wSta <= rCharStats.wSta ) );
					}
					//-----------------------------------------------------------------------------------------------
					//	���װ�
					const int nELEC   = sItemCustom.GETRESIST_ELEC();
					const int nFIRE   = sItemCustom.GETRESIST_FIRE();
					const int nICE    = sItemCustom.GETRESIST_ICE();
					const int nPOISON = sItemCustom.GETRESIST_POISON();
					const int nSPIRIT = sItemCustom.GETRESIST_SPIRIT();

					if ( nELEC || nFIRE || nICE || nPOISON || nSPIRIT )
					{
						AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 3 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

						if ( nELEC )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 0 ), nELEC );	
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_ELEC);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nFIRE )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 1 ), nFIRE );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_FIRE);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nICE )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 2 ), nICE );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_ICE);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nPOISON )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 3 ), nPOISON );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_POISON);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
						if ( nSPIRIT )
						{
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 4 ), nSPIRIT );
							BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_SPIRIT);
							APPEND_ITEM_GRADE ( strText, uGRADE );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
					}


					//	�����̻�
					EMSTATE_BLOW emBLOW = pItemData->sSuitOp.sBLOW.emTYPE;
					if ( emBLOW !=EMBLOW_NONE )
					{
						AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 4 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

						strText.Format("%s:%s", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 0 ), COMMENT::BLOW[emBLOW].c_str() );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

						strText.Format("%s:%.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 1 ),pItemData->sSuitOp.sBLOW.fLIFE );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

						strText.Format("%s:%.2f%%", ID2GAMEWORD("ITEM_ADVANCED_INFO_ABNORMAL", 2 ),pItemData->sSuitOp.sBLOW.fRATE );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);

						{
							float fVAR1 = pItemData->sSuitOp.sBLOW.fVAR1 * COMMENT::BLOW_VAR1_SCALE[emBLOW];

							if( fVAR1 != 0.0f )
							{
								if ( COMMENT::IsBLOW1_PER(emBLOW) )
									strText.Format("%s:%.2f%%", COMMENT::BLOW_VAR1[emBLOW].c_str(), fVAR1 );
								else
									strText.Format("%s:%.2f", COMMENT::BLOW_VAR1[emBLOW].c_str(), fVAR1 );

								AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
							}
						}

						{
							float fVAR2 = pItemData->sSuitOp.sBLOW.fVAR2 * COMMENT::BLOW_VAR2_SCALE[emBLOW];

							if( fVAR2 != 0.0f )
							{
								if ( COMMENT::IsBLOW2_PER(emBLOW) )
									strText.Format("%s:%.2f%%", COMMENT::BLOW_VAR2[emBLOW].c_str(), fVAR2 );
								else
									strText.Format("%s:%.2f", COMMENT::BLOW_VAR2[emBLOW].c_str(), fVAR2 );

								AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
							}
						}
					}


					//	NOTE
					//		���� ȿ��
					AddTextAddValue( sItemCustom, pItemData->sSuitOp );
					if( pItemData->sBasicOp.emItemType == ITEM_VEHICLE )
					{
						bool bInfo = true;

						VEHICLEITEMINFO_MAP_ITER iter = m_pGaeaClient->GetCharacter()->m_mapVEHICLEItemInfo.find ( sItemCustom.m_VehicleDbNum );
						if ( iter == m_pGaeaClient->GetCharacter()->m_mapVEHICLEItemInfo.end() )
						{
							iter = m_pGaeaClient->GetCharacter()->m_mapVEHICLEItemInfoTemp.find ( sItemCustom.m_VehicleDbNum );
							if (iter == m_pGaeaClient->GetCharacter()->m_mapVEHICLEItemInfoTemp.end())
								bInfo = false;
						}


						float fAddOnSpeed = 0.0f;

						if ( bInfo ) 
						{
							SVEHICLEITEMINFO sVehicleItemInfo = (*iter).second;

							for ( int i = 0; i < ACCE_TYPE_SIZE; ++i ) 
							{							
								fAddOnSpeed += sVehicleItemInfo.m_PutOnItems[i].GETMOVESPEED();
							}
						}

						if ( bInfo ) 
						{
							SVEHICLEITEMINFO sVehicleItemInfo = (*iter).second;

							for ( int i = 0; i < ACCE_TYPE_SIZE; ++i ) 
							{							
								const SITEM* pItem = GLogicData::GetInstance().GetItem ( sVehicleItemInfo.m_PutOnItems[i].GetNativeID() );
								if ( pItem )
								{
									strText.Format("%s %s", ID2GAMEWORD ("ITEM_VEHICLE_SLOT", i), pItem->GetName() );

									AddTextNoSplit( strText, ITEM_INFOR_COLOR::dwDefault );
								}
							}

							strText.Format( "%s", ID2GAMEWORD ("ITEM_VEHICLE_BOOST_LEARN", sVehicleItemInfo.m_dwBoosterID ) );
							AddTextNoSplit( strText, ITEM_INFOR_COLOR::dwDefault );

							strText.Format( "%s %.2f%%", ID2GAMEWORD ("ITEM_VEHICLE_BATTERY", 0), sVehicleItemInfo.m_nFull/10.0f );
							AddTextNoSplit( strText, ITEM_INFOR_COLOR::dwDefault );
						}
					}

					//	Ư�����
					//
					{
						BOOL bSPAC = sItemCustom.GETINCHP() || sItemCustom.GETINCMP() || sItemCustom.GETINCSP() || sItemCustom.GETINCAP();
						BOOL bVAR_HP(FALSE), bVAR_MP(FALSE), bVAR_SP(FALSE), bVAR_AP(FALSE);

						EMITEM_VAR emITEM_VAR = pItemData->sSuitOp.sVARIATE.emTYPE;
						EMITEM_VAR emITEM_VOL = pItemData->sSuitOp.sVOLUME.emTYPE;
						if ( (emITEM_VAR!=EMVAR_NONE) || (emITEM_VOL!=EMVAR_NONE) || bSPAC )
						{
							AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 5 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
						}
						
						//	Ư����� ( ��ȭ�� )
						//
						float fVAR_SCALE(0);
						if ( emITEM_VAR != EMVAR_NONE )
						{
							switch ( emITEM_VAR )
							{
							case EMVAR_HP:
								bVAR_HP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCHP();
								break;
							case EMVAR_MP:
								bVAR_MP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCMP();
								break;
							case EMVAR_SP:
								bVAR_SP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCSP();
								break;
							case EMVAR_AP:
								bVAR_AP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCAP();
								break;
							default:
								fVAR_SCALE = pItemData->sSuitOp.sVARIATE.fVariate;
								break;
							};

							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}

						if ( !bVAR_HP && sItemCustom.GETINCHP() )
						{
							fVAR_SCALE = sItemCustom.GETINCHP();
							emITEM_VAR = EMVAR_HP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}

						if ( !bVAR_MP && sItemCustom.GETINCMP() )
						{
							fVAR_SCALE = sItemCustom.GETINCMP();
							emITEM_VAR = EMVAR_MP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}

						if ( !bVAR_SP && sItemCustom.GETINCSP() )
						{
							fVAR_SCALE = sItemCustom.GETINCSP();
							emITEM_VAR = EMVAR_SP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}

						if ( !bVAR_AP && sItemCustom.GETINCAP() )
						{
							fVAR_SCALE = sItemCustom.GETINCAP();
							emITEM_VAR = EMVAR_AP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}

						// ����ġ ����
						float fExpMultiple = pItemData->GetExpMultiple();
						if( fExpMultiple != 1.0f )
						{
							strText.Format("%s %2.2f%s", 
												ID2GAMEWORD( "ITEM_ADVANCED_INFO_SPECIAL_EX", 0 ), 
												fExpMultiple, 
												ID2GAMEWORD( "ITEM_ADVANCED_INFO_SPECIAL_EX", 1 ) );

							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}

						//	Ư����� ( ��ȭ�� )
						//
						if ( emITEM_VOL != EMVAR_NONE)
						{
							if ( emITEM_VOL == EMVAR_MOVE_SPEED )
							{
								float fVOLUME = sItemCustom.GETMOVESPEED();
								strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
							}
							else
							{
								float fVOLUME = pItemData->sSuitOp.sVOLUME.fVolume;
								strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
							}
							
						}
					}

					/*costume combine stats, Juver, 2017/09/01 */
					SITEM*	pItemDisguiseCombine = GLItemMan::GetInstance().GetItem( sItemCustom.nidDISGUISE );
					if ( pItemDisguiseCombine && sItemCustom.nidDISGUISE != NATIVEID_NULL() )
					{
						AddTextAddValueCostume ( pItemDisguiseCombine );

						EMITEM_VAR emITEM_VAR = pItemDisguiseCombine->sSuitOp.sVARIATE.emTYPE;
						EMITEM_VAR emITEM_VOL = pItemDisguiseCombine->sSuitOp.sVOLUME.emTYPE;
						if ( (emITEM_VAR!=EMVAR_NONE) || (emITEM_VOL!=EMVAR_NONE))
						{
							AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 5 ), NS_UITEXTCOLOR::HOTPINK );
						}

						if ( emITEM_VAR != EMVAR_NONE )
						{
							float  fVAR_SCALE = pItemDisguiseCombine->sSuitOp.sVARIATE.fVariate;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE );
						}

						if ( emITEM_VOL != EMVAR_NONE)
						{
							float fVOLUME = pItemDisguiseCombine->sSuitOp.sVOLUME.fVolume;
							strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE );	
						}
					}


					// Info : ���� ��ġ
					{
						if( sItemCustom.IsSetRandOpt() )
						{
							AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 11 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
							DWORD dwRandomAbilityColor = ITEM_INFOR_COLOR::dwRandomAbility;

							if( (sItemCustom.GETOptTYPE1() > RandomOption::RANDOM_OPTION_NULL) && (sItemCustom.GETOptTYPE1() < RandomOption::RANDOM_OPTION_NSIZE) )
							{
								int nRandOptType = sItemCustom.GETOptTYPE1();
								float fVal = sItemCustom.GETOptVALUE1() * COMMENT::ITEM_RANDOM_OPT_SCALE[nRandOptType];
								bool bIsPerRandOpt = sItemCustom.IsPerRandOpt( nRandOptType );

								/*SetInformationLabel3_RandomOption( nRandOptType, fVal, bIsPerRandOpt, dwRandomAbilityColor );
								if( fVal != 0.0f )
								{
									if( fVal > 0.0f) strDescText += _T('+');

									strDescText.AppendFormat( "%.2f", fVal );
									if( sItemCustom.IsPerRandOpt( nRandOptType1 ) ) strDescText += _T('%');

									AddTextNoSplit( strDescText, NS_UITEXTCOLOR::PRIVATE );
								}*/

								std::string strDescText = "";

								//if( (nRandOptType <= RandomOption::RANDOM_OPTION_NULL) || (nRandOptType >= RandomOption::RANDOM_OPTION_NSIZE) )
								//	return;

								//if ( fVal == 0.0f )
								//	return;

								strDescText = sc::string::format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

								if ( fVal > 0.0f) 
									strDescText += _T('+');
								if ( fVal < 0.0f) 
									strDescText += _T('-');

								if ( bIsPerRandOpt )
								{
									CString strTemp = sc::string::format( "%.2f", fVal ).c_str();
									CutZero(strTemp,2);
									strDescText += sc::string::format("%s", strTemp);

									strDescText += _T('%');
								}
								else
									strDescText += sc::string::format( "%d", static_cast<int>(fVal) );

								const bool _bShowMinMax(GLInputManager::GetInstance().GetKeyState(EMSHORTCUTS_DOMINATE_CONTROL) & DXKEY_DOWNED ? true : false);
								if ( _bShowMinMax == true )
								{
									const RandomOption::Manager& _managerRandomOption(RandomOption::Manager::getInstance());
									const RandomOption::Entity* const _pEntity(_managerRandomOption.find(pItemData->sRandomOpt.szNAME));		
									if ( (_pEntity != NULL) && (GLUseFeatures::GetInstance().IsUsingRandomOptionClientView() == true) )
									{
										RandomOption::Information _information;
										if ( _pEntity->getInformation(RandomOption::GENERATE_REMODEL, RandomOption::RANDOM_OPTION(nRandOptType), _information) == true )
										{
											if ( bIsPerRandOpt == true )
												strDescText += sc::string::format( "(Min %.2f ~ Max %.2f)", _information.valueFloatLow, _information.valueFloatHigh);
											else
												strDescText += sc::string::format( "(Min %d ~ Max %d)", _information.valueIntegerLow, _information.valueIntegerHigh);
										}			
									}
								}	
								AddTextNoSplit( strDescText.c_str(), NS_UITEXTCOLOR::PRIVATE );

								//m_pItemLabel2->AddString( nIndex, strDescText.c_str(), dwAddAbility );
							}
							if( (sItemCustom.GETOptTYPE2() > RandomOption::RANDOM_OPTION_NULL) && (sItemCustom.GETOptTYPE2() < RandomOption::RANDOM_OPTION_NSIZE) )
							{
								int nRandOptType = sItemCustom.GETOptTYPE2();
								float fVal = sItemCustom.GETOptVALUE2() * COMMENT::ITEM_RANDOM_OPT_SCALE[nRandOptType];
								bool bIsPerRandOpt = sItemCustom.IsPerRandOpt( nRandOptType );

								//SetInformationLabel3_RandomOption( nRandOptType, fVal, bIsPerRandOpt, dwRandomAbilityColor );

								std::string strDescText = "";

								//if( (nRandOptType <= RandomOption::RANDOM_OPTION_NULL) || (nRandOptType >= RandomOption::RANDOM_OPTION_NSIZE) )
								//	return;

								//if ( fVal == 0.0f )
								//	return;

								strDescText = sc::string::format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

								if ( fVal > 0.0f) 
									strDescText += _T('+');
								if ( fVal < 0.0f) 
									strDescText += _T('-');

								if ( bIsPerRandOpt )
								{
									CString strTemp = sc::string::format( "%.2f", fVal ).c_str();
									CutZero(strTemp,2);
									strDescText += sc::string::format("%s", strTemp);

									strDescText += _T('%');
								}
								else
									strDescText += sc::string::format( "%d", static_cast<int>(fVal) );

								const bool _bShowMinMax(GLInputManager::GetInstance().GetKeyState(EMSHORTCUTS_DOMINATE_CONTROL) & DXKEY_DOWNED ? true : false);
								if ( _bShowMinMax == true )
								{
									const RandomOption::Manager& _managerRandomOption(RandomOption::Manager::getInstance());
									const RandomOption::Entity* const _pEntity(_managerRandomOption.find(pItemData->sRandomOpt.szNAME));		
									if ( (_pEntity != NULL) && (GLUseFeatures::GetInstance().IsUsingRandomOptionClientView() == true) )
									{
										RandomOption::Information _information;
										if ( _pEntity->getInformation(RandomOption::GENERATE_REMODEL, RandomOption::RANDOM_OPTION(nRandOptType), _information) == true )
										{
											if ( bIsPerRandOpt == true )
												strDescText += sc::string::format( "(Min %.2f ~ Max %.2f)", _information.valueFloatLow, _information.valueFloatHigh);
											else
												strDescText += sc::string::format( "(Min %d ~ Max %d)", _information.valueIntegerLow, _information.valueIntegerHigh);
										}			
									}
								}	
								AddTextNoSplit( strDescText.c_str(), NS_UITEXTCOLOR::PRIVATE );
							}
							if( (sItemCustom.GETOptTYPE3() > RandomOption::RANDOM_OPTION_NULL) && (sItemCustom.GETOptTYPE3() < RandomOption::RANDOM_OPTION_NSIZE) )
							{
								int nRandOptType = sItemCustom.GETOptTYPE3();
								float fVal = sItemCustom.GETOptVALUE3() * COMMENT::ITEM_RANDOM_OPT_SCALE[nRandOptType];
								bool bIsPerRandOpt = sItemCustom.IsPerRandOpt( nRandOptType );

								//SetInformationLabel3_RandomOption( nRandOptType, fVal, bIsPerRandOpt, dwRandomAbilityColor );

								std::string strDescText = "";

								//if( (nRandOptType <= RandomOption::RANDOM_OPTION_NULL) || (nRandOptType >= RandomOption::RANDOM_OPTION_NSIZE) )
								//	return;

								//if ( fVal == 0.0f )
								//	return;

								strDescText = sc::string::format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

								if ( fVal > 0.0f) 
									strDescText += _T('+');
								if ( fVal < 0.0f) 
									strDescText += _T('-');

								if ( bIsPerRandOpt )
								{
									CString strTemp = sc::string::format( "%.2f", fVal ).c_str();
									CutZero(strTemp,2);
									strDescText += sc::string::format("%s", strTemp);

									strDescText += _T('%');
								}
								else
									strDescText += sc::string::format( "%d", static_cast<int>(fVal) );

								const bool _bShowMinMax(GLInputManager::GetInstance().GetKeyState(EMSHORTCUTS_DOMINATE_CONTROL) & DXKEY_DOWNED ? true : false);
								if ( _bShowMinMax == true )
								{
									const RandomOption::Manager& _managerRandomOption(RandomOption::Manager::getInstance());
									const RandomOption::Entity* const _pEntity(_managerRandomOption.find(pItemData->sRandomOpt.szNAME));		
									if ( (_pEntity != NULL) && (GLUseFeatures::GetInstance().IsUsingRandomOptionClientView() == true) )
									{
										RandomOption::Information _information;
										if ( _pEntity->getInformation(RandomOption::GENERATE_REMODEL, RandomOption::RANDOM_OPTION(nRandOptType), _information) == true )
										{
											if ( bIsPerRandOpt == true )
												strDescText += sc::string::format( "(Min %.2f ~ Max %.2f)", _information.valueFloatLow, _information.valueFloatHigh);
											else
												strDescText += sc::string::format( "(Min %d ~ Max %d)", _information.valueIntegerLow, _information.valueIntegerHigh);
										}			
									}
								}	
								AddTextNoSplit( strDescText.c_str(), NS_UITEXTCOLOR::PRIVATE );
							}
							if( (sItemCustom.GETOptTYPE4() > RandomOption::RANDOM_OPTION_NULL) && (sItemCustom.GETOptTYPE4() < RandomOption::RANDOM_OPTION_NSIZE) )
							{
								int nRandOptType = sItemCustom.GETOptTYPE4();
								float fVal = sItemCustom.GETOptVALUE4() * COMMENT::ITEM_RANDOM_OPT_SCALE[nRandOptType];
								bool bIsPerRandOpt = sItemCustom.IsPerRandOpt( nRandOptType );

								//SetInformationLabel3_RandomOption( nRandOptType, fVal, bIsPerRandOpt, dwRandomAbilityColor );

								std::string strDescText = "";

								//if( (nRandOptType <= RandomOption::RANDOM_OPTION_NULL) || (nRandOptType >= RandomOption::RANDOM_OPTION_NSIZE) )
								//	return;

								//if ( fVal == 0.0f )
								//	return;

								strDescText = sc::string::format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

								if ( fVal > 0.0f) 
									strDescText += _T('+');
								if ( fVal < 0.0f) 
									strDescText += _T('-');

								if ( bIsPerRandOpt )
								{
									CString strTemp = sc::string::format( "%.2f", fVal ).c_str();
									CutZero(strTemp,2);
									strDescText += sc::string::format("%s", strTemp);

									strDescText += _T('%');
								}
								else
									strDescText += sc::string::format( "%d", static_cast<int>(fVal) );

								const bool _bShowMinMax(GLInputManager::GetInstance().GetKeyState(EMSHORTCUTS_DOMINATE_CONTROL) & DXKEY_DOWNED ? true : false);
								if ( _bShowMinMax == true )
								{
									const RandomOption::Manager& _managerRandomOption(RandomOption::Manager::getInstance());
									const RandomOption::Entity* const _pEntity(_managerRandomOption.find(pItemData->sRandomOpt.szNAME));		
									if ( (_pEntity != NULL) && (GLUseFeatures::GetInstance().IsUsingRandomOptionClientView() == true) )
									{
										RandomOption::Information _information;
										if ( _pEntity->getInformation(RandomOption::GENERATE_REMODEL, RandomOption::RANDOM_OPTION(nRandOptType), _information) == true )
										{
											if ( bIsPerRandOpt == true )
												strDescText += sc::string::format( "(Min %.2f ~ Max %.2f)", _information.valueFloatLow, _information.valueFloatHigh);
											else
												strDescText += sc::string::format( "(Min %d ~ Max %d)", _information.valueIntegerLow, _information.valueIntegerHigh);
										}			
									}
								}	
								AddTextNoSplit( strDescText.c_str(), NS_UITEXTCOLOR::PRIVATE );
							}
						}
					}
				}
				break;

			case ITEM_PET_CARD:
				{
					PETCARDINFO_MAP_ITER iter = m_pGaeaClient->GetCharacter()->m_mapPETCardInfo.find( sItemCustom.PetDbNum() );
					if ( iter == m_pGaeaClient->GetCharacter()->m_mapPETCardInfo.end() )
					{
						iter = m_pGaeaClient->GetCharacter()->m_mapPETCardInfoTemp.find ( sItemCustom.PetDbNum() );
						if ( iter == m_pGaeaClient->GetCharacter()->m_mapPETCardInfoTemp.end() ) return;
					}

					SPETCARDINFO sPetCardInfo = (*iter).second;

					//strText = sc::string::format( "%s %s", ID2GAMEWORD("ITEM_PET_NAME",0), sPetCardInfo.m_szName );
					//m_pTextBox->AddText( strText.c_str(), ITEM_INFOR_COLOR::dwDefault );

					if ( GLUseFeatures::GetInstance().IsUsingDataIntegrate() )
					{
						std::vector<std::string>	vecPET_TYPE;
						CGameTextMan::GetInstance().LoadHelpUnknownCommentString("PET_TYPE", vecPET_TYPE, CGameTextMan::EM_COMMENT_TEXT);
						strText.Format( "%s %s", ID2GAMEWORD("ITEM_PET_TYPE", 0), vecPET_TYPE[sPetCardInfo.m_emTYPE].c_str() );
					}
					else
					{
						strText.Format( "%s %s", ID2GAMEWORD("ITEM_PET_TYPE", 0), COMMENT::PET_TYPE[sPetCardInfo.m_emTYPE].c_str() );
					}
					AddTextNoSplit( strText, ITEM_INFOR_COLOR::dwDefault );

					strText.Format( "%s %.2f%%", ID2GAMEWORD("ITEM_PET_FULL", 0), sPetCardInfo.m_nFull/10.0f );
					AddTextNoSplit( strText, ITEM_INFOR_COLOR::dwDefault );

					if ( 2 <= sPetCardInfo.m_wMaxSkillSlot )
					{
						strText.Format( "%s", ID2GAMEWORD("ITEM_PET_DUALSKILL", 1) );
						AddTextNoSplit( strText, ITEM_INFOR_COLOR::dwDefault );
					}
					else
					{
						strText.Format( "%s", ID2GAMEWORD("ITEM_PET_DUALSKILL", 0) );
						AddTextNoSplit( strText, ITEM_INFOR_COLOR::dwDefault );
					}

					const SITEM* pItem = GLogicData::GetInstance().GetItem( sPetCardInfo.m_PutOnItems[ACCETYPEA].GetNativeID() );
					if ( pItem )
					{
						strText.Format("%s %s", ID2GAMEWORD("ITEM_PET_SLOTA", 0), pItem->GetName() );
						AddTextNoSplit( strText, ITEM_INFOR_COLOR::dwDefault );
					}

					pItem = GLogicData::GetInstance().GetItem( sPetCardInfo.m_PutOnItems[ACCETYPEB].GetNativeID() );
					if ( pItem )
					{
						strText.Format("%s %s", ID2GAMEWORD("ITEM_PET_SLOTB", 0), pItem->GetName() );
						AddTextNoSplit( strText, ITEM_INFOR_COLOR::dwDefault );
					}

					if ( sItemCustom.tDISGUISE != 0 )
					{
						const CTime& currentTime = m_pGaeaClient->GetCurrentTime();
						CTime startTime   = sItemCustom.tBORNTIME;
						CTimeSpan timeSpan = currentTime - startTime;
						if( timeSpan.GetTotalSeconds() < sItemCustom.tDISGUISE )
						{
							strText.Format("[%s]", ID2GAMEWORD("ITEM_PET_USE_SKINPACK", 0) );
							AddTextNoSplit( strText, ITEM_INFOR_COLOR::dwDefault );
							strText.Format("%s : %dsec", ID2GAMEWORD("CLUB_BATTLE_TIME", 0), sItemCustom.tDISGUISE - timeSpan.GetTotalSeconds() );
							AddTextNoSplit( strText, ITEM_INFOR_COLOR::dwDefault );
						}
					}

					bool bDisplayEnablePetSkill = false;
					PETSKILL_MAP_CITER pos = sPetCardInfo.m_ExpSkills.begin();
					PETSKILL_MAP_CITER pos_end = sPetCardInfo.m_ExpSkills.end();
					for ( ;pos != pos_end; ++pos )
					{
						const PETSKILL& sPetSkill = (*pos).second;
						PGLSKILL pSKILL = GLSkillMan::GetInstance().GetData( sPetSkill.sNativeID );
						if ( pSKILL )
						{
							if ( !bDisplayEnablePetSkill )
							{
								bDisplayEnablePetSkill = true;
								AddTextNoSplit( ID2GAMEWORD("ITEM_PET_SKILLS", 0 ), ITEM_INFOR_COLOR::dwDefault );
							}

							strText.Format("%s", pSKILL->GetName() );
							AddTextNoSplit( strText, ITEM_INFOR_COLOR::dwDefault );
						}
					}
				}
				break;

			case ITEM_CHARM:
			case ITEM_ARROW:
			case ITEM_BULLET: /*gun-bullet logic, Juver, 2017/05/27 */
				{
					AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 1 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					//	���ݷ�
					GLDWDATA &sDAMAGE = sItemCustom.GETDAMAGE();
					if ( sDAMAGE.dwLow || sDAMAGE.dwHigh  )
					{
						strText.Format("%s:%s~%s ", ID2GAMEWORD("ITEM_ARROW_INFO", 0 ), 
													GetNumberWithSign ( sDAMAGE.dwLow ), 
													GetNumberWithSign ( sDAMAGE.dwHigh ) );

						AddTextNoSplit(strText,NS_UITEXTCOLOR::DEFAULT);
					}
 

					//	NOTE
					//		���� ȿ��
					AddTextAddValue( sItemCustom, pItemData->sSuitOp );					
				}
				break;

			case ITEM_CURE:
				{
					AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY", 7 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

					if( pItemData->sDrugOp.dwCureVolume == 0 )
					{
						strText.Format("%s",COMMENT::ITEMDRUG[pItemData->sDrugOp.emDrug].c_str());
					}
					else
					{
						strText.Format("%s:%d",COMMENT::ITEMDRUG[pItemData->sDrugOp.emDrug].c_str(), pItemData->sDrugOp.dwCureVolume);
					}

					AddTextNoSplit(strText, NS_UITEXTCOLOR::DEFAULT);
				}
				break;

			case ITEM_SKILL:
				{
					AddInfoSkillItem ( sItemCustom );
				}
				break;

			case ITEM_PET_SKILL:
				{
					AddInfoPetSkillItem( sItemCustom );
				}
				break;

			case ITEM_GRINDING:
				{
					AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY_EX", 0 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
				}
				break;

				// ����ġ ȹ�� ī�� ����
			case ITEM_VIETNAM_EXPGET:
				{
					if ( pItemData->sDrugOp.bRatio )
					{
						strText.Format(ID2GAMEWORD ("ITEM_CATEGORY_VIETNAM_EXP", 1) , pItemData->sDrugOp.dwCureVolume );						
					}
					else
					{
						strText.Format(ID2GAMEWORD ("ITEM_CATEGORY_VIETNAM_EXP", 0), pItemData->sDrugOp.dwCureVolume );
					}

					AddTextNoSplit(strText,NS_UITEXTCOLOR::LIGHTSKYBLUE);
				}
				break;
				// ������ ȹ�� ī�� ����
			case ITEM_VIETNAM_ITEMGET:
				{
					strText.Format(ID2GAMEWORD ("ITEM_CATEGORY_VIETNAM_ITEM", 0) , pItemData->sDrugOp.dwCureVolume );
					AddTextNoSplit(strText,NS_UITEXTCOLOR::LIGHTSKYBLUE);
				}
				break;
			}

			if( emItemType != ITEM_PET_SKILL && emItemType != ITEM_PET_DUALSKILL
				&& emItemType != ITEM_POINT_CARD_REFUND && emItemType != ITEM_POINT_CARD_NOT_REFUND )
			{
				INFO_DISPLAY_ITEM_AddTextNoSplit( ID2GAMEWORD( "ITEM_CATEGORY", 8 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );

				// Note : �ñ�/���� ī�忡 ���̸� ��ǥ ���
				if( pItemData->sDrugOp.emDrug == ITEM_DRUG_CALL_REGEN )
				{
					GLCharacter* pCharacter = m_pGaeaClient->GetCharacter();	

					CString strName( m_pGaeaClient->GetMapName( pCharacter->m_sStartMapID ) );
					if( strName == _T("(null)") ) strName.Empty();

					strText.Format( "%s:%s", ID2GAMEWORD("ITEM_CALL_MAP", 0 ), strName );
					INFO_DISPLAY_ITEM_AddTextNoSplit( strText, NS_UITEXTCOLOR::DEFAULT);

					SMAPNODE *pMapNode = m_pGaeaClient->FindMapNode ( pCharacter->m_sStartMapID );
					if( pMapNode )
					{
						GLLevelFileClient cLevelFile(m_pGaeaClient);
						BOOL bOk = cLevelFile.LoadFile( pMapNode->m_LevelFile.c_str(), NULL );
						if( bOk )
						{	
							D3DXVECTOR3 vStartPos;
							PDXLANDGATE pGate = cLevelFile.GetLandGateMan()->FindLandGate( pCharacter->m_dwStartGate );
							if( pGate )
							{
								vStartPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );
							}
							else
							{
								pGate = cLevelFile.GetLandGateMan()->FindLandGate( DWORD(0) );
								if( pGate ) vStartPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );
							}

							int nPosX(0), nPosY(0);
							cLevelFile.GetMapAxisInfo().Convert2MapPos( vStartPos.x, vStartPos.z, nPosX, nPosY );

							strText.Format( "%s:%d,%d", ID2GAMEWORD("ITEM_CALL_MAP", 1 ), nPosX, nPosY );
							INFO_DISPLAY_ITEM_AddTextNoSplit( strText, NS_UITEXTCOLOR::DEFAULT);
						}
					}
				}
				else if( pItemData->sDrugOp.emDrug == ITEM_DRUG_CALL_LASTCALL )
				{
					GLCharacter* pCharacter = m_pGaeaClient->GetCharacter();

					if( pCharacter->m_sLastCallMapID.IsValidNativeID() )
					{
						CString strName( m_pGaeaClient->GetMapName( pCharacter->m_sLastCallMapID ) );
						if( strName == _T("(null)") ) strName.Empty();

						strText.Format( "%s:%s", ID2GAMEWORD("ITEM_CALL_MAP", 0 ), strName );
						INFO_DISPLAY_ITEM_AddTextNoSplit( strText, NS_UITEXTCOLOR::DEFAULT);

						SMAPNODE *pMapNode = m_pGaeaClient->FindMapNode ( pCharacter->m_sLastCallMapID );
						if( pMapNode )
						{
							GLLevelFileClient cLevelFile(m_pGaeaClient);
							BOOL bOk = cLevelFile.LoadFile( pMapNode->m_LevelFile.c_str(), NULL );
							if( bOk )
							{	
								int nPosX(0), nPosY(0);
								cLevelFile.GetMapAxisInfo().Convert2MapPos( pCharacter->m_vLastCallPos.x, pCharacter->m_vLastCallPos.z, nPosX, nPosY );

								strText.Format( "%s:%d,%d", ID2GAMEWORD("ITEM_CALL_MAP", 1 ), nPosX, nPosY );
								INFO_DISPLAY_ITEM_AddTextNoSplit( strText, NS_UITEXTCOLOR::DEFAULT);
							}
						}
					}
					else
					{
						INFO_DISPLAY_ITEM_AddTextNoSplit( ID2GAMEWORD("ITEM_CALL_MAP", 2 ), NS_UITEXTCOLOR::RED);
					}
				}
				else if ( pItemData->sDrugOp.emDrug == ITEM_DRUG_CALL_TELEPORT )
				{
					CString strName( m_pGaeaClient->GetMapName( pItemData->sBasicOp.sSubID ) );
					if( strName == _T("(null)") ) strName.Empty();

					strText.Format( "%s:%s", ID2GAMEWORD("ITEM_TELEPORT_MAP", 0 ), strName );
					INFO_DISPLAY_ITEM_AddTextNoSplit( strText, NS_UITEXTCOLOR::DEFAULT);

					strText.Format( "%s:%d,%d", ID2GAMEWORD("ITEM_TELEPORT_MAP", 1 ), pItemData->sBasicOp.wPosX, pItemData->sBasicOp.wPosY );
					INFO_DISPLAY_ITEM_AddTextNoSplit( strText, NS_UITEXTCOLOR::DEFAULT);
				}

				if ( emItemType != ITEM_POINT_CARD_REFUND && emItemType != ITEM_POINT_CARD_NOT_REFUND )
				{
					if ( pCharacter->ACCEPT_ITEM( sItemCustom ) )
					{
						strText.Format ( "[%s]", ID2GAMEWORD("ITEM_USABLE", 0 ) );
						INFO_DISPLAY_ITEM_AddTextNoSplit(strText, NS_UITEXTCOLOR::ENABLE );

						WORD wNeedSP = pCharacter->CALC_ACCEPTP ( sItemCustom.GetNativeID() );
						if ( 0 < wNeedSP )
						{
							strText.Format ( "%s:%d", ID2GAMEWORD("ITEM_NEEDSP" ), wNeedSP );
							INFO_DISPLAY_ITEM_AddTextNoSplit(strText, NS_UITEXTCOLOR::PALEGREEN );
						}
					}
					else
					{
						strText.Format ( "[%s]", ID2GAMEWORD("ITEM_USABLE", 1 ) );
						INFO_DISPLAY_ITEM_AddTextNoSplit(strText, NS_UITEXTCOLOR::RED );
					}
				}
			}


			//  ��������->�䱸Ȱ��
			if ( pItemData->sBasicOp.wReqActPointDW || pItemData->sBasicOp.wReqActPointUP )
			{
				int emType = pItemData->sBasicOp.emReqActivityType;
				bool bReqActPoint = true;
				// �Ʒ��� COMMENT�� �����Ϳ��� �о�����ʴ´�;
				strText.Format( "%s:%s(%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 3), 
					COMMENT::ACTIVITY_CLASS[emType].c_str(), pItemData->sBasicOp.wReqActPointDW );

				if ( pItemData->sBasicOp.wReqActPointUP )
				{
					strText2.Format ( "~%d)",pItemData->sBasicOp.wReqActPointUP );
					strText += strText2; 
					bReqActPoint = (pItemData->sBasicOp.wReqActPointUP >= pCharacter->m_llActivityPoint[emType] );
				}
				else
				{
					strText += ")";
				}

				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( 
					pItemData->sBasicOp.wReqActPointDW <= pCharacter->m_llActivityPoint[emType] && bReqActPoint ) );
			}


			//  ��������->�⿩��
			if ( pItemData->sBasicOp.dwReqContribution )
			{
				bool bReqPoint = true;

				strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 4), pItemData->sBasicOp.dwReqContribution );

				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( 
					pItemData->sBasicOp.dwReqContribution <= static_cast< DWORD >( pCharacter->m_nContributionPoint ) ) );
			}

			{ // ����
				const TCHAR * pszComment(NULL);

				if ( (emItemType != ITEM_SKILL) || (emItemType != ITEM_PET_SKILL) )
				{
					pszComment = pItemData->GetComment();
				}
				else
				{
					SNATIVEID sNativeID = sItemCustom.GetNativeID ();
					SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sNativeID );
					SNATIVEID sSkillID = pItemData->sSkillBookOp.sSkill_ID;

					//	Note : ��ų ���� ������.
					PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSkillID.wMainID, sSkillID.wSubID );
					if ( pSkill )
					{
						pszComment = pSkill->GetDesc();
					}
				}

				if( pszComment )
				{
					int StrLength = static_cast<int>( strlen( pszComment ) );
					if( StrLength )
					{
						AddTextNoSplit ( ID2GAMEWORD( "ITEM_CATEGORY", 9 ), NS_UITEXTCOLOR::LIGHTSKYBLUE );
						AddTextLongestLineSplit( pszComment, NS_UITEXTCOLOR::DEFAULT );
					}
				}
			}

			/*nondrop card, Juver, 2017/11/26 */
			if ( sItemCustom.IsGM_GENITEM() )
			{
				AddTextNoSplit ( ID2GAMEINTEXT( "ITEM_INFO_GM_GEN"),  NS_UITEXTCOLOR::GOLD);
			}
			else
			{	//itemflagsw
				CString strFlag;

				bool bSale = sItemCustom.IsSale();
				bool bExchange = sItemCustom.IsExchange();
				bool bLocker = bExchange;
				bool bThrow = sItemCustom.IsThrow();
				bool bCostume = sItemCustom.IsDisguise();
				bool bTrashCan = sItemCustom.IsGarbage();

				AddTextNoSplit( "", NS_UITEXTCOLOR::WHITE );

				strFlag.Format("%s:%s",ID2GAMEWORD ( "ITEM_INFO_TRADE", 0 ),ID2GAMEWORD ( "ITEM_INFO_TRADE_OPTION", bSale? 0:1 ) );
				AddTextNoSplit ( strFlag.GetString(), bSale? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::ENABLE );

				strFlag.Format("%s:%s",ID2GAMEWORD ( "ITEM_INFO_TRADE", 1 ),ID2GAMEWORD ( "ITEM_INFO_TRADE_OPTION", bThrow? 0:1 ) );
				AddTextNoSplit ( strFlag.GetString(), bThrow? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::ENABLE );

				strFlag.Format("%s:%s",ID2GAMEWORD ( "ITEM_INFO_TRADE", 2 ),ID2GAMEWORD ( "ITEM_INFO_TRADE_OPTION", bLocker? 0:1 ) );
				AddTextNoSplit ( strFlag.GetString(), bLocker? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::ENABLE );

				strFlag.Format("%s:%s",ID2GAMEWORD ( "ITEM_INFO_TRADE", 3 ),ID2GAMEWORD ( "ITEM_INFO_TRADE_OPTION", bExchange? 0:1 ) );
				AddTextNoSplit ( strFlag.GetString(), bExchange? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::ENABLE );

				strFlag.Format("%s:%s",ID2GAMEWORD ( "ITEM_INFO_TRADE", 4 ),ID2GAMEWORD ( "ITEM_INFO_TRADE_OPTION", bTrashCan? 0:1 ) );
				AddTextNoSplit ( strFlag.GetString(), bTrashCan? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::ENABLE );
			}

			{ /*rightclick wear/unwear, Juver, 2017/06/24 */
				if ( pCharacter && !pCharacter->ValidWindowOpen() && 
					( emItemType == ITEM_SUIT || emItemType == ITEM_ARROW || emItemType == ITEM_CHARM || emItemType == ITEM_BULLET ) )
				{
					BOOL bUseItem = pCharacter->ACCEPT_ITEM( sItemCustom.GetNativeID () );

					if( sItemCustom.nidDISGUISE != NATIVEID_NULL() )
						bUseItem = pCharacter->ACCEPT_ITEM( sItemCustom );

					if ( bUseItem )
						AddTextNoSplit(ID2GAMEINTEXT("INVENTOWEAR_ITEM_INFO"), NS_UITEXTCOLOR::GOLD );
				}
			}

			//{	/*item preview, Juver, 2017/07/27 */
			//	bool bCanPreview = CInnerInterface::GetInstance().PreviewItemCheckSimple( pItemData->sBasicOp.sNativeID );
			//	if ( bCanPreview )
			//	{
			//		AddTextNoSplit ( ID2GAMEWORD( "ITEM_USEPREVIEW", 0 ), NS_UITEXTCOLOR::GOLD );
			//	}
			//}

			{	/*box contents, Juver, 2017/08/30 */
				if ( emItemType == ITEM_BOX || emItemType == ITEM_PREMIUMSET )
				{
					AddTextNoSplit ( ID2GAMEINTEXT( "HELP_SHOW_ITEM_TOOLTIP" ), NS_UITEXTCOLOR::GOLD );
				}
			}

			/*item link, Juver, 2017/07/31 */
			if ( bInInventory )
			{
				AddTextNoSplit ( ID2GAMEINTEXT( "HELP_SHOW_ITEM_LINK" ), NS_UITEXTCOLOR::GOLD );
			}
			

			/*item wrapper, Juver, 2018/01/12 */
			if ( !bWrapped )
			{
				/*BOOL bWrappable = pItemData->isWrappable();
				if ( sItemCustom.nidDISGUISE != NATIVEID_NULL() )	bWrappable = FALSE;
				if ( !sItemCustom.IsGM_GENITEM() && pItemData->sBasicOp.IsEXCHANGE() )	bWrappable = FALSE;

				if ( bWrappable )
				{
					AddTextNoSplit ( ID2GAMEINTEXT( "ITEM_INFO_POSSIBLE_WRAP" ), NS_UITEXTCOLOR::WHITE );
				}*/
			}
			else
			{
				AddTextNoSplit ( ID2GAMEINTEXT( "ITEM_INFO_POSSIBLE_UNWRAP" ), NS_UITEXTCOLOR::WHITE );
			}
		}
	}

void CInnerInterface::UiSetResolution( long lResolution )
{
    uiman::GetInnerUIMan().SetResolution( lResolution );
}

long CInnerInterface::UiGetResolution()
{
    return CUIMan::GetResolution();
}

HRESULT CInnerInterface::UiRender(LPDIRECT3DDEVICEQ pd3dDevice)
{
    return uiman::GetInnerUIMan().Render(pd3dDevice);
}

HRESULT CInnerInterface::UiRestoreDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice)
{
    return uiman::GetInnerUIMan().RestoreDeviceObjects(pd3dDevice);
}

HRESULT CInnerInterface::UiInvalidateDeviceObjects()
{
    return uiman::GetInnerUIMan().InvalidateDeviceObjects();
}

BOOL CInnerInterface::UiRegisterControl( CUIControl* pNewControl, bool bPosisionKeep )
{
    return uiman::GetInnerUIMan().RegisterControl(pNewControl, bPosisionKeep);
}

BOOL CInnerInterface::UiIsVisibleGroup(UIGUID ControlID)
{
    return uiman::GetInnerUIMan().IsVisibleGroup(ControlID);
}

void CInnerInterface::UiShowGroupTop(UIGUID ControlID, bool bMakeMsg)                   
{ 
    uiman::GetInnerUIMan().ShowGroupTop(ControlID, bMakeMsg); 
}

void CInnerInterface::UiShowGroupBottom(UIGUID ControlID, bool bMakeMsg)                
{ 
    uiman::GetInnerUIMan().ShowGroupBottom(ControlID, bMakeMsg); 
}

void CInnerInterface::UiShowGroupUpdate(UIGUID ControlID, bool bMakeMsg)                   
{ 
	uiman::GetInnerUIMan().ShowGroupUpdate(ControlID, bMakeMsg); 
}

void CInnerInterface::UiShowGroupFocus(UIGUID ControlID, bool bMakeMsg)                 
{ 
    UIGUID uiID = UIMutuallyExclusive::GetInstance().GetMutually( ControlID );
    if( uiman::GetInnerUIMan().IsVisibleGroup(uiID) )
        uiman::GetInnerUIMan().HideGroup( ControlID );
    else
        uiman::GetInnerUIMan().ShowGroupFocus(ControlID, bMakeMsg);    
}

void CInnerInterface::UiSetGroupLock(UIGUID ControlID, bool bLock)
{
	uiman::GetInnerUIMan().SetGroupLock(ControlID, bLock);
}

void CInnerInterface::UISetModalGroup(UIGUID ControlID, bool bModal)
{
    uiman::GetInnerUIMan().SetModalGroup(ControlID, bModal);
}

void CInnerInterface::UiHideGroup(UIGUID ControlID, bool bMakeMsg)
{
	uiman::GetInnerUIMan().HideGroup(ControlID, bMakeMsg); 
}

void CInnerInterface::UiRefresh( UIGUID ControlID )
{
	uiman::GetInnerUIMan().Refresh( ControlID ); 
}

void CInnerInterface::ResetControl()
{
	// Lotto System
	m_pLottoMainUI = NULL;
	m_pLottoBuyLottoUI = NULL;
	m_pLottoBuyListInfo = NULL;
	m_pLottoWinManListInfo = NULL;
	m_pLottoDisplayUI = NULL;
	m_pLottoNotifyButton = NULL;
	GLLottoSystemManUI::Instance()->Destroy();

	m_pColorProperty = NULL;

	m_pSummonStateDisplayUI = NULL;
	m_pFactionInfoWindow = NULL;
	m_pWorldBattleButton = NULL;

	m_pSimpleHP = NULL;
	m_pBusWindow = NULL;
	m_pChatGroupWindow = NULL;
	m_pQuestAlarm = NULL;
	m_pDamageDisplayMan = NULL;
	m_pDialogueWindow = NULL;
	m_pStorageWindow = NULL;
	m_pItemBankWindow = NULL;
	m_pVNGainSysInventory = NULL;
	m_pAdminMessageDisplay = NULL;
	m_pInfoDisplay = NULL;
	m_pInfoDisplayEx = NULL;
	m_pInfoDisplayLink = NULL;
	m_pMapMoveDisplay = NULL;
	m_pWaitServerDisplay = NULL;
	m_pAcademyConftDisplay = NULL;
	m_pMapWindowInterface = NULL;
	m_pStorageChargeCard = NULL;
	m_pRebirthDialogue = NULL;
	m_pPrivateMarketWindow = NULL;
	m_pPrivateMarketMake = NULL;
	m_pPrivateMarketSellWindow = NULL;
	m_pClubWindow = NULL;
	m_pPrivateMarketSearchBuy = NULL;
	m_pProgessTimer = NULL;
	m_pRnCDMProgress = NULL;
	m_pRnCDMRankWindow = NULL;
	m_pRnCDMClubInfo = NULL;
	m_pSubMasterSet = NULL;
	m_pClubMake = NULL;
	m_pSystemMessageWindow = NULL;
	m_pQuestionItemDisplay = NULL;
	m_pClubStorageWindow = NULL;
	m_pConftConfirm = NULL;
	m_pCountMsgWindow = NULL;
	m_pGameMenu = NULL;
	m_pQBoxButton = NULL;
	m_pItemRebuildWindow = NULL;	// ITEMREBUILD_MARK
	m_pPetRebirthDialogue = NULL; // monster7j	
	m_pPetWindow = NULL;
	m_pCharacterWindow = NULL;
    m_pReferCharWindow = NULL;
	m_pVehicleColorChangeWindow = NULL;
	m_pBikeGetOffWindow = NULL;

	m_pSystemMessageWindowDummy = NULL;
	m_pMapMoveDisplayDummy = NULL;
	m_pLeftTopGroupDummy = NULL;
	m_pBasicInfoViewDummy = NULL;
	m_pBasicPotionTrayDummy = NULL;
	m_pQuestAlarmDummy = NULL;

	m_pBonusTimeGauge = NULL;
	m_pBonusTimeDisplay = NULL;
	m_pBikeWindow = NULL;
	m_pThaiCCafeMark = NULL;

	m_pItemGarbageWindow = NULL;

	m_pItemShopIconMan = NULL;

	m_pAttendanceBookWindow = NULL;
	m_pClubBattleModalWindow = NULL;

	m_pTaxiWindow = NULL;

	m_bFirstVNGainSysCall = true;	
    m_bInitUpdateOrder = false;

	m_bDuplicateLogin = FALSE;

	m_fItemBankDelay = 5.0f;
	/*m_fVehicleDelay = 0.0f;*/

	m_pItemMixWindow = NULL;
	m_pItemMixInvenWindow = NULL;
	m_pGatherGauge = NULL;
	m_pCdmRankingDisplay = NULL;
	m_pCdmSafeTimeDisplay = NULL;
	m_pSkillTimeDisplay = NULL;

	m_mapLinkData.clear();
	m_mapItemLinkData.clear();
	m_mapPartyLinkData.clear();
	
	m_dwLinkDataIndex = 0;
	m_dwItemLinkDataIndex = 0;
	m_dwPartyLinkDataIndex = 0;

	m_pPetHungry = NULL;
	m_pMouseTip = NULL;

    m_pStudentRecordNotifyButton = NULL;

    // interface pointer reset.
    m_pMiniMap = NULL;
    m_pKeySettingWindow = NULL;
    m_pChatMacroWindow = NULL;
    m_pNameDisplayMan = NULL;
    m_pPrivateMarketShowMan = NULL;
    m_pSummonNameDisplayMan = NULL;
    m_pSkillWindowToTray = NULL;
    m_pPartyWindow = NULL;
	m_pPartyWindowRenewal = NULL;
	m_pPartyStateWindow = NULL;
	m_pPartyTenderWindow = NULL;
	m_pPartyTenderNotifyButton = NULL;
	m_pPartyDistributionWindow = NULL;
	m_pPartyStateWindow = NULL;
	m_pPartyDistributionListWindow = NULL;
	m_pMiniExpeditionWindow = NULL;
	m_pExpeditionInviteWindow = NULL;
    m_pBasicChat = NULL;
    m_pChatOptionWindow = NULL;
    m_pChatTypeList = NULL;
    m_pTradeWindow = NULL;
    m_pTradeInventoryWindow = NULL;
    m_pPtoPWindow = NULL;
    m_pInventoryWindow = NULL;
	m_pInventory = NULL;
    m_pItemMove = NULL;
    m_pMarketWindow = NULL;
    m_pHeadChatDisplayMan = NULL;
    m_pConftModalWindow = NULL;
    m_pPartyModalWindow = NULL;
	m_pChatGroupModalWindow = NULL;
    m_pConftDisplayMan = NULL;
    m_pQuestWindowRenewal = NULL;
    m_pFriendWindow = NULL;
    m_pModalWindow = NULL;
    m_pShopItemSearchWindow = NULL;
    m_pItemSearchResultWindow = NULL;
    m_pMapRequireCheckWindow = NULL;
    m_pPostBoxWindow = NULL;
    m_pPostNotifyButton = NULL;
    m_pQuestHelper = NULL;
    m_pTutorialDialogue = NULL;
    m_pBalloonText = NULL;
    m_pSkillWindow = NULL;
    m_pBasicInfoView = NULL;
    m_pExplainInterface = NULL;
    m_pUILeftTopGroup = NULL;
    m_pSkillTrayTab = NULL;
    m_pBasicQuickSkillSlot = NULL;
    m_pTargetInfoDisplay = NULL;
	m_pCursorTargetInfo = NULL;
    m_pBalloonSpaceBar = NULL;
    m_pTutorialViewStep = NULL;
    m_pLunchBoxTimeDisplay = NULL;
    m_pVehicleWindow = NULL;
    m_pProgressDisplay = NULL;
    m_pStudentRecordWindow = NULL;
    m_pPointShopWindow = NULL;
    m_pRebirthWindow = NULL;
    m_pCompetitionNotifyButton = NULL;
    m_pPopupMenu = NULL;
    m_pInfoCTFDisplay = NULL;
    m_pCTFResultWindow = NULL;
	m_pCompetitionWindow = NULL;
	m_pCompetitionTopRanking = NULL;
	m_pReservedSkillAnimation = NULL;
	m_pProductButton = NULL;
    m_pProductProgress = NULL;
	m_pProductWindow = NULL;
    m_pItemShopWindowWeb = NULL;
    m_pItemInforTooltip = NULL;
    m_pItemInforTooltipWear = NULL;
	m_pItemInforTooltipLink = NULL;
    m_pSkillTooltip = NULL;
	m_pSkillTooltipSubSkill = NULL;
    m_pForceRebirth = NULL;
    m_pNotifyCTFSpurt = NULL;
    m_pDialogueWindowRenewal = NULL;
	m_pPVEUserInterface = NULL;
    m_mmManageControl.clear();
	m_pRnCompetitionWindow = NULL;
    m_pRnCompetitionResultWindow = NULL;
	m_pRnCDMTopRanking = NULL;
    m_pSimpleSystemMessageDisplay = NULL;
    m_pCaptureTheFlagEffecMan = NULL;

	//new instance system
	m_pSimpleSystemMessageDisplay = NULL;
	m_pSimpleTimerMessageBox = NULL;
	m_pRnCDMProgress = NULL;
	m_pRnCDMRankWindow = NULL;
	m_pRnCDMClubInfo = NULL;
	m_pRnCompetitionWindow = NULL;
	m_pRnCompetitionResultWindow = NULL;
	m_pTournamentDashBoard = NULL;
	m_pTournamentUI = NULL;
	m_pTournamentToToWindow = NULL;
	m_pTournamentMoreInfoWindow = NULL;
	m_pRnCDMTopRanking = NULL;
	m_pCaptureTheFlagEffecMan = NULL;
	m_pGuidanceAuthProgressUI = NULL;
	m_pGuidanceProgressInfoUI = NULL;
	m_pGuidanceRankInfoUI = NULL;
	m_pCTIAuthProgressUI = NULL;
	m_pCTIProgressInfoUI = NULL;
	m_pCTIRankInfoUI = NULL;

	m_pTournamentDashBoard	= NULL;
	m_pTournamentUI			= NULL;
	m_pTournamentToToWindow = NULL;
	m_pTournamentMoreInfoWindow = NULL;
	m_pTournamentUserInterface = NULL;
	m_pEntranceConfirm = NULL;
	m_pTournamentMulty = NULL;
	m_pBigGroupWindowRight = NULL;
	m_pBigGroupWindowLeft = NULL;
	m_pTournamentIndicator = NULL;
	m_pMiniGroupWindowLeft = NULL;
	m_pTournamentObserveWindow = NULL;
	m_pMiniGroupWindowRight = NULL;

    SAFE_DELETE( m_pInvenItemTooltipGroup );
    SAFE_DELETE( m_pWishItemTooltipGroup );
    SAFE_DELETE( m_pMarketWishItemTooltipGroup );

    m_vecSkillToolTipRegist.clear();
    m_vecSkillToolTipPool.clear();
    m_vecSkillToolTipUsed.clear();
	//for ( int i=0; i<MAX_GCHAT_SLOT; i ++)
	//{
	//	m_pMaxChatWindow[i] = NULL;
	//}

	m_pSkillTooltipLinkSkill = NULL;
	m_pDoublePoint = NULL;
	m_pSelformBoxWindow = NULL;

	m_pItemInforTooltipDurability = NULL;
}

HRESULT CInnerInterface::OneTimeSceneInit ()
{
	switch ( RENDERPARAM::emSERVICE_TYPE )
	{
	case RENDERPARAM::EMSERVICE_DEFAULT:
	case RENDERPARAM::EMSERVICE_KOREA:
	case RENDERPARAM::EMSERVICE_FEYA:
	case RENDERPARAM::EMSERVICE_MALAYSIA_CN:
	case RENDERPARAM::EMSERVICE_MALAYSIA_EN:
	case RENDERPARAM::EMSERVICE_JAPAN:
	case RENDERPARAM::EMSERVICE_THAILAND:
	case RENDERPARAM::EMSERVICE_CHINA:
	case RENDERPARAM::EMSERVICE_INDONESIA:
	case RENDERPARAM::EMSERVICE_PHILIPPINES:
	case RENDERPARAM::EMSERVICE_VIETNAM:
	case RENDERPARAM::EMSERVICE_GLOBAL:
	case RENDERPARAM::EMSERVICE_EU:
	case RENDERPARAM::EMSERVICE_US:
		m_bCHANNEL = true;
		break;

	default:
		m_bCHANNEL = false;
		break;
	};

	return S_OK;
}

HRESULT CInnerInterface::FrameMove( LPDIRECT3DDEVICEQ pd3dDevice, float fElapsedTime )
{
	GASSERT( pd3dDevice );

	//	������ �� ���̰� ����.
	//	���ÿ� Update()���� �Ѱ� ����.
    UiHideGroup( ITEM_INFOR_TOOLTIP );
    UiHideGroup( ITEM_INFOR_TOOLTIP_WEAR );
	UiHideGroup( ITEM_INFOR_TOOLTIP_DURABILITY );
    UiHideGroup( SKILL_INFOR_TOOLTIP );
	UiHideGroup( SKILL_INFOR_TOOLTIP_MULTI );
	UiHideGroup( SKILL_INFOR_TOOLTIP_LINK );
	UiHideGroup( SKILL_INFOR_TOOLTIP_DELAY_ACTION );
	UiHideGroup( INFO_DISPLAY );
	UiHideGroup( INFO_DISPLAY_EX );
	

	ResetTargetInfoMouseOver ();	//	Ÿ�� ����â ����
	NSGUI::ResetCharMoveAllBlock();	//	ĳ������ ��ü ������ ��� Ǯ��
	NSGUI::ResetCharMoveBlock ();	//	ĳ���� ������ ��� Ǯ��	
	ResetSnapItem ();		        //	������ ���� ����
	ResetFirstItemSlot ();	        //	�����۽��� �ٱ� ����

	m_bUSING_INFO_DISPLAY = false;
 
	// ������ ��ũâ ������ ����
	if( !IsBankOpen() && m_fItemBankDelay < 5.0f )
	{
		m_fItemBankDelay += fElapsedTime;
	}

	bool bKEYBOARD_BLOCK = GLInputManager::GetInstance().IsShortcutsBlock();

	// DXInputString* pInputString = m_pEngineDevice->GetInputString();
	//if (UiIsVisibleGroup ( MODAL_WINDOW ) ||
	//	UiIsVisibleGroup ( REBIRTH_DIALOGUE ) ||
	//	//UiIsVisibleGroup ( HELP_WINDOW ) ||	// �� ��������, ���� ��� ����
	//	UiIsVisibleGroup ( ITEMSHOP_WINDOW ) ||
	//  UiIsVisibleGroup ( FACEBOOK_WINDOW ) ||
	//  UiIsVisibleGroup ( TWITTER_WINDOW ) ||
	//	pInputString->IsOn())
	//{
	//	bKEYBOARD_BLOCK = true;
	//}

	if ( !bKEYBOARD_BLOCK )
		UpdateShortcutBefore();

	HRESULT hr = S_OK;
	hr = uiman::GetInnerUIMan().FrameMove( fElapsedTime, this );

	if( IsOpenWindowToMoveBlock() )
	{
        NSGUI::SetCharMoveBlock();
	}

	UpdateStatus();

	//	���̻� ������ ������ ǥ������ ���� ��쿡 �����ش�. 
	if ( !m_bUSING_INFO_DISPLAY )
		RESET_INFO();
	if ( !bKEYBOARD_BLOCK )
		UpdateShortcutAfter();

	if ( uiman::GetInnerUIMan().IsExclusiveControl() )
		NSGUI::SetCharMoveBlock();

 	int LB = (int)DxInputDevice::GetInstance().GetMouseState(DXMOUSE_LEFT);
 	if ( (LB & DXKEY_UP) || (LB & DXKEY_DUP) )
 	{
 		uiman::GetInnerUIMan().ResetExclusiveControl();
 	}

	MoveBasicInfoWindow ();

    UpdateFocusOrder ();

	GLCharacter* pCharacter = m_pGaeaClient->GetCharacter();
	m_vCharDir = pCharacter->GetDirect();
// 	Actor& actor = pCharacter->GetActorMove();
// 	if (actor.PathIsActive())
//         m_vCharDir = actor.Movement();

	{
        if ( GLUseFeatures::GetInstance().IsUsingItemTooltip() )
        {
			{
				CUIGroup* pTooltip = (CUIGroup*)uiman::GetInnerUIMan().FindControl(ITEM_INFOR_TOOLTIP);
				if ( pTooltip->IsVisible() == true )
				{
					UIRECT rcAllTooltip = pTooltip->GetGlobalPos();
					const long lResolution = uiman::GetInnerUIMan().GetResolution();
					INT nReleativePosX(rcAllTooltip.left)
						, X_RES(HIWORD ( lResolution ))
						, Y_RES(LOWORD ( lResolution ));

					INT uiidItemTooltip[3] = {
						// ITEM_INFOR_TOOLTIP,
						SKILL_INFOR_TOOLTIP_MULTI,				// ITEM_INFOR_TOOLTIP_LINK,
						ITEM_INFOR_TOOLTIP_DURABILITY,
						ITEM_INFOR_TOOLTIP_WEAR
					};

					D3DXVECTOR2 vArrTooltipPos[3];
					INT nPosY(rcAllTooltip.top);
					for ( INT i = 0; i < 3; ++i )
					{
						pTooltip = (CUIGroup*)uiman::GetInnerUIMan().FindControl(uiidItemTooltip[i]);
						if ( pTooltip->IsVisible() == false )
						{
							vArrTooltipPos[i] = D3DXVECTOR2(0.0f, 0.0f);
							continue;
						}

						UIRECT rcTooltip = pTooltip->GetGlobalPos();

						INT nGap(0), nAddWidth(rcAllTooltip.sizeX + rcTooltip.sizeX + 2);

						vArrTooltipPos[i] = D3DXVECTOR2(nReleativePosX + rcAllTooltip.sizeX + 2, rcTooltip.top);
						nPosY = min(nPosY, rcTooltip.top);
						if ( nReleativePosX + nAddWidth > X_RES )
						{
							nGap = X_RES - (nReleativePosX + nAddWidth);
						}

						if ( nReleativePosX + nGap < 0 )
						{
							nGap = -nReleativePosX;
						}

						nReleativePosX += nGap;

						for ( INT j = i; j >= 0; --j )
						{
							vArrTooltipPos[j].x += nGap;
						}

						rcAllTooltip.SetPosition(nReleativePosX, rcAllTooltip.top
							, nAddWidth
							, rcAllTooltip.sizeY);
					}

					UIRECT rcNormalInfo = m_pItemInforTooltip->GetGlobalPos();
					m_pItemInforTooltip->SetGlobalPos( D3DXVECTOR2(nReleativePosX, nPosY) );
					for ( INT i = 0; i < 3; ++i )
					{
						pTooltip = (CUIGroup*)uiman::GetInnerUIMan().FindControl(uiidItemTooltip[i]);
						if ( pTooltip->IsVisible() )
						{
							UIRECT rcTooltip = pTooltip->GetGlobalPos();
							pTooltip->SetGlobalPos( D3DXVECTOR2(vArrTooltipPos[i].x, nPosY) );
						}
					}
				}
			}


			/*
            if ( UiIsVisibleGroup( ITEM_INFOR_TOOLTIP_WEAR ) ) 
            {
                UIRECT rcGlobalPos = m_pItemInforTooltip->GetGlobalPos();
                UIRECT rcGlobalPosEx = m_pItemInforTooltipWear->GetGlobalPos();

                float x = rcGlobalPos.right + 2.0f;
                float y = min(rcGlobalPos.top,rcGlobalPosEx.top);

                const long lResolution = uiman::GetInnerUIMan().GetResolution();
                WORD X_RES = HIWORD ( lResolution );
                WORD Y_RES = LOWORD ( lResolution );

                if ( X_RES < x + rcGlobalPosEx.sizeX )
                {
                    x = X_RES - rcGlobalPosEx.sizeX;

                    m_pItemInforTooltip->SetGlobalPos(D3DXVECTOR2(x-2.0f-rcGlobalPos.sizeX,y));
                    m_pItemInforTooltipWear->SetGlobalPos(D3DXVECTOR2(x,y));
                }
                else
                {
                    m_pItemInforTooltip->SetGlobalPos(D3DXVECTOR2(rcGlobalPos.left,y));
                    m_pItemInforTooltipWear->SetGlobalPos(D3DXVECTOR2(x,y));
                }
            }

			if ( UiIsVisibleGroup( SKILL_INFOR_TOOLTIP_MULTI ) ) 
			{
				if ( UiIsVisibleGroup( ITEM_INFOR_TOOLTIP ) ) 
				{
					UIRECT rcGlobalPos = m_pItemInforTooltip->GetGlobalPos();
					UIRECT rcGlobalPosEx = m_pSkillTooltipSubSkill->GetGlobalPos();

					float x = rcGlobalPos.right + 2.0f;
					float y = min(rcGlobalPos.top,rcGlobalPosEx.top);

					const long lResolution = uiman::GetInnerUIMan().GetResolution();
					WORD X_RES = HIWORD ( lResolution );
					WORD Y_RES = LOWORD ( lResolution );

					if ( X_RES < x + rcGlobalPosEx.sizeX )
					{
						x = X_RES - rcGlobalPosEx.sizeX;

						m_pItemInforTooltip->SetGlobalPos(D3DXVECTOR2(x-2.0f-rcGlobalPos.sizeX,y));
						m_pSkillTooltipSubSkill->SetGlobalPos(D3DXVECTOR2(x,y));
					}
					else
					{
						m_pItemInforTooltip->SetGlobalPos(D3DXVECTOR2(rcGlobalPos.left,y));
						m_pSkillTooltipSubSkill->SetGlobalPos(D3DXVECTOR2(x,y));
					}
				}
			}

			if ( UiIsVisibleGroup( ITEM_INFOR_TOOLTIP_DURABILITY ) ) 
			{
				if( UiIsVisibleGroup( SKILL_INFOR_TOOLTIP_MULTI ) )
				{
					UIRECT rcGlobalPos = m_pSkillTooltipSubSkill->GetGlobalPos();
					UIRECT rcGlobalPosEx = m_pItemInforTooltipDurability->GetGlobalPos();

					float x = rcGlobalPos.right + 2.0f;
					float y = min(rcGlobalPos.top,rcGlobalPosEx.top);

					const long lResolution = uiman::GetInnerUIMan().GetResolution();
					WORD X_RES = HIWORD ( lResolution );
					WORD Y_RES = LOWORD ( lResolution );

					if ( X_RES < x + rcGlobalPosEx.sizeX )
					{
						x = X_RES - rcGlobalPosEx.sizeX;

						m_pSkillTooltipSubSkill->SetGlobalPos(D3DXVECTOR2(x-2.0f-rcGlobalPos.sizeX,y));
						m_pItemInforTooltipDurability->SetGlobalPos(D3DXVECTOR2(x,y));
					}
					else
					{
						m_pSkillTooltipSubSkill->SetGlobalPos(D3DXVECTOR2(rcGlobalPos.left,y));
						m_pItemInforTooltipDurability->SetGlobalPos(D3DXVECTOR2(x,y));
					}
				}
				else
				{
					UIRECT rcGlobalPos = m_pItemInforTooltip->GetGlobalPos();
					UIRECT rcGlobalPosEx = m_pItemInforTooltipDurability->GetGlobalPos();

					float x = rcGlobalPos.right + 2.0f;
					float y = min(rcGlobalPos.top,rcGlobalPosEx.top);

					const long lResolution = uiman::GetInnerUIMan().GetResolution();
					WORD X_RES = HIWORD ( lResolution );
					WORD Y_RES = LOWORD ( lResolution );

					if ( X_RES < x + rcGlobalPosEx.sizeX )
					{
						x = X_RES - rcGlobalPosEx.sizeX;

						m_pItemInforTooltip->SetGlobalPos(D3DXVECTOR2(x-2.0f-rcGlobalPos.sizeX,y));
						m_pItemInforTooltipDurability->SetGlobalPos(D3DXVECTOR2(x,y));
					}
					else
					{
						m_pItemInforTooltip->SetGlobalPos(D3DXVECTOR2(rcGlobalPos.left,y));
						m_pItemInforTooltipDurability->SetGlobalPos(D3DXVECTOR2(x,y));
					}	
				}				
			}
			
        }
		*/
		}
        else
        {
		    if ( UiIsVisibleGroup ( INFO_DISPLAY_EX ) ) 
		    {
			    UIRECT rcGlobalPos = m_pInfoDisplay->GetGlobalPos();
			    UIRECT rcGlobalPosEx = m_pInfoDisplayEx->GetGlobalPos();

			    float x = rcGlobalPos.right + 2.0f;
			    float y = min(rcGlobalPos.top,rcGlobalPosEx.top);

			    const long lResolution = uiman::GetInnerUIMan().GetResolution();
			    WORD X_RES = HIWORD ( lResolution );
			    WORD Y_RES = LOWORD ( lResolution );

			    if ( X_RES < x + rcGlobalPosEx.sizeX )
			    {
				    x = X_RES - rcGlobalPosEx.sizeX;

				    m_pInfoDisplay->SetGlobalPos(D3DXVECTOR2(x-2.0f-rcGlobalPos.sizeX,y));
				    m_pInfoDisplayEx->SetGlobalPos(D3DXVECTOR2(x,y));
			    }
			    else
			    {
				    m_pInfoDisplay->SetGlobalPos(D3DXVECTOR2(rcGlobalPos.left,y));
				    m_pInfoDisplayEx->SetGlobalPos(D3DXVECTOR2(x,y));
			    }
		    }
        }
	}

	{
		if (pCharacter->VehicleState() == EMVEHICLE_DRIVER && pCharacter->VehicleBoosterId())
			UiShowGroupTop( BIKE_BOOST_WINDOW);
		else
			UiHideGroup( BIKE_BOOST_WINDOW );
	}

	{
		if (pCharacter->VehicleState() ==  EMVEHICLE_DRIVER )
		{
			INT nNumber = pCharacter->VehicleePassengerNum();
			if( nNumber > 1 )
			{
				//if( UiIsVisibleGroup ( BIKE_GET_OFF_WINDOW ) )
				{
					const long lResolution = uiman::GetInnerUIMan().GetResolution();
					WORD X_RES = HIWORD ( lResolution );
					WORD Y_RES = LOWORD ( lResolution );

                    UIRECT rcGlobalPos = m_pBikeGetOffWindow->GetGlobalPos();
					D3DXVECTOR2 vNewPos;
					vNewPos.x = (X_RES - rcGlobalPos.sizeX) / 2.0f;
					vNewPos.y = ( Y_RES - 100.0f );

					m_pBikeGetOffWindow->SetGlobalPos( vNewPos );
					UiShowGroupTop( BIKE_GET_OFF_WINDOW );
				}
			}
			else
			{
				if( UiIsVisibleGroup ( BIKE_GET_OFF_WINDOW ) )
				{
					UiHideGroup( BIKE_GET_OFF_WINDOW );
					UiHideGroup ( BIKE_MESSAGE_WINDOW );
				}
			}

		}
		else if( m_pGaeaClient->GetCharacter()->VehicleState() ==  EMVEHICLE_PASSENGER )
		{
			//if( UiIsVisibleGroup ( BIKE_GET_OFF_WINDOW ) )
			{
				const long lResolution = uiman::GetInnerUIMan().GetResolution();
				WORD X_RES = HIWORD ( lResolution );
				WORD Y_RES = LOWORD ( lResolution );
				{
					UIRECT rcGlobalPos = m_pBikeGetOffWindow->GetGlobalPos();
					D3DXVECTOR2 vNewPos;
					vNewPos.x = (X_RES - rcGlobalPos.sizeX) / 2.0f;
					vNewPos.y = ( Y_RES - 100.0f );

					m_pBikeGetOffWindow->SetGlobalPos( vNewPos );
					UiShowGroupTop( BIKE_GET_OFF_WINDOW );
				}

				{
					UIRECT rcGlobalPos = m_pBikeMessageWindow->GetGlobalPos();
					D3DXVECTOR2 vNewPos;
					vNewPos.x = (X_RES - rcGlobalPos.sizeX) / 2.0f;
					vNewPos.y = rcGlobalPos.top;

					m_pBikeMessageWindow->SetGlobalPos( vNewPos );
					UiShowGroupFocus( BIKE_MESSAGE_WINDOW );
				}				
			}
		}
		else
		{
			if( UiIsVisibleGroup ( BIKE_GET_OFF_WINDOW ) )
			{
				UiHideGroup( BIKE_GET_OFF_WINDOW );
				UiHideGroup ( BIKE_MESSAGE_WINDOW );
			}
		}
	}

	if( m_pCursorTargetInfo )
		m_pCursorTargetInfo->UpdateInfo();

	// Lotto System
	GLLottoSystemManUI::Instance()->Update( fElapsedTime );

	if ( m_pGaeaClient )
	{
		GLCharacter* pChar = m_pGaeaClient->GetCharacter();
		if ( pChar )
		{
			if ( !pChar->m_sSummonable.bSummonable )
			{
				if ( UiIsVisibleGroup ( SUMMON_STATE_DISPLAY_UI ) )
					UiHideGroup( SUMMON_STATE_DISPLAY_UI );
			}
			else
			{
				if ( FALSE == UiIsVisibleGroup ( SUMMON_STATE_DISPLAY_UI ) )
					UiShowGroupFocus( SUMMON_STATE_DISPLAY_UI );
			}
		}
	}

	return hr;
}

void CInnerInterface::UpdateShortcutBefore ()
{	
	{
		if ( !m_pNameDisplayMan ) return ;		

		//if ( UIKeyCheck::GetInstance()->CheckSimple ( DIK_LMENU, DXKEY_PRESSED ) )
		//{
		//	for (int i= 0; i < CHAT_MACRO_SIZE; ++i){
		//		if ( UIKeyCheck::GetInstance()->CheckSimple ( DIK_1+i, DXKEY_DOWN ) )
		//		{			
		//			AddChatMacro(i);
		//			break;
		//		}
		//	}
		//}



		if( RANPARAM::bNAME_DISPLAY )
		{
			if( UiIsVisibleGroup( NAME_DISPLAY_MAN ) )
			{
				if( m_pNameDisplayMan->GetNameType() != NAME_DISPLAY_MOBCHAR )
				{
					UiHideGroup( NAME_DISPLAY_MAN );
					m_pNameDisplayMan->SetNameType( NAME_DISPLAY_MOBCHAR );
					UiShowGroupBottom( NAME_DISPLAY_MAN );
				}
			}
			else
			{
				m_pNameDisplayMan->SetNameType( NAME_DISPLAY_MOBCHAR );
				UiShowGroupBottom( NAME_DISPLAY_MAN );
			}

			return ;
		}

		UiHideGroup ( NAME_DISPLAY_MAN );		
		if ( UIKeyCheck::GetInstance()->Check ( DIK_LMENU, DXKEY_PRESSED ) )
		{
			m_pNameDisplayMan->SetNameType ( NAME_DISPLAY_ITEMMONEY );
			UiShowGroupBottom ( NAME_DISPLAY_MAN );
		}
		else if ( UIKeyCheck::GetInstance()->Check ( DIK_LCONTROL, DXKEY_PRESSED ) )
		{
			m_pNameDisplayMan->SetNameType ( NAME_DISPLAY_MOBCHAR );
			UiShowGroupBottom ( NAME_DISPLAY_MAN );
		}
		else if ( UIKeyCheck::GetInstance()->Check ( DIK_LSHIFT, DXKEY_PRESSED ) )
		{
			m_pNameDisplayMan->SetNameType ( NAME_DISPLAY_ALL );
			UiShowGroupBottom ( NAME_DISPLAY_MAN );
		}
		else
		{
			//	BEGIN : CLUB CONFT SETTING
			EMCONFT_TYPE emTYPE = m_pGaeaClient->GetCharacter()->GetConftingType();
			if ( emTYPE == EMCONFT_GUILD )
			{
				m_pNameDisplayMan->SetNameType ( NAME_DISPLAY_CLUB_CONFT );
				UiShowGroupBottom ( NAME_DISPLAY_MAN );                
				return ;
			}

			//	��Ƽ
			GLPartyClient* const pMyParty = m_pGaeaClient->GetMyPartyClient(); //	������
			if ( pMyParty->isValid() == true )
			{				
				if (m_pGaeaClient->GetCharacter()->IsConfting())	//	��Ƽ ��� ���ΰ�?
				{
					m_pNameDisplayMan->SetNameType ( NAME_DISPLAY_PARTY_CONFT );			
				}
				else	//	��Ƽ ���� �� ���
				{
					m_pNameDisplayMan->SetNameType ( NAME_DISPLAY_PARTY );		
				}
				UiShowGroupBottom ( NAME_DISPLAY_MAN );
			}
			else
			{
				//	������ �Ѹ�
				m_pNameDisplayMan->SetNameType ( NAME_DISPLAY_DEFAULT );
				UiShowGroupBottom ( NAME_DISPLAY_MAN );
			}
		}
	}	
}

void CInnerInterface::UpdateShortcutAfter ()
{
	// bjju.ControlType
	// ����Ű���� RanLogicClient/GLControlBase.cpp �� �Ű���
	// 11-10-31

	if ( UIKeyCheck::GetInstance()->Check ( DIK_ESCAPE, DXKEY_DOWN ) )
	{
		bool bALLHIDE = true;

		//	NOTE
		//		���ÿ� �� ������ FocusList��
		//		������ ���ɼ��� ���ٴ� ������ �־�� �Ѵ�.
		//		�����ϱ� ��.��
		CUIFocusContainer::UICONTROL_FOCUSLIST list = uiman::GetInnerUIMan().GetFocusList()->GetFocusList ();
		CUIFocusContainer::UICONTROL_FOCUSLIST_RITER riter = list.rbegin ();
		CUIFocusContainer::UICONTROL_FOCUSLIST_RITER riter_end = list.rend ();
		for ( ; riter != riter_end; ++riter )
		{
			bool bAnotherProcess = false;
			UIGUID cID = riter->cID;

			if ( UiIsVisibleGroup ( cID ) && 
				cID != BASIC_CHAT_BOX && 
				cID != BASIC_INFO_VIEW &&
				cID != PRIVATE_MARKET_SHOW_MAN && 
				cID != MINIMAP &&
				cID != QUICK_SKILL_TRAY_TAB_WINDOW &&
				cID != CTI_AUTH_PROGRESS_UI &&
				cID != CTI_PROGRESS_INFO_UI &&
				cID != INFO_CTF_DISPLAY &&
				cID != CTF_RESULT_WINDOW && 
				cID != CTI_RANK_INFO_UI && 
				cID != MINIPARTY_WINDOW &&
				cID != COMPETITION_TOURNAMENTENT_INDICATOR)
			{
				if ( cID == INVENTORY_WINDOW || cID == MARKET_WINDOW ||
					cID == STORAGE_WINDOW || cID == CLUB_STORAGE_WINDOW )
				{
					SetMarketWindowClose ();
					SetStorageWindowClose ();
					SetClubStorageWindowClose ();
				}
				else if ( cID == TRADEINVENTORY_WINDOW )
				{
					if ( UiIsVisibleGroup ( TRADE_WINDOW ) )
					{
						SetTradeWindowCloseReq ();
					}
					else if ( UiIsVisibleGroup ( PRIVATE_MARKET_WINDOW ) )
					{
						if ( m_pPrivateMarketWindow->IsOPENER () )
						{
							DoModal ( ID2GAMEINTEXT("PRIVATE_MARKET_QUIT"), UI::MODAL_QUESTION, YESNO, UI::MODAL_PRIVATE_MARKET_QUIT );
						}
						else
						{
							SetPrivateMarketClose ();
						}
						bAnotherProcess = true;
					}
				}
				else if ( cID == TRADE_WINDOW )
				{							
					SetTradeWindowCloseReq ();
				}
				else if ( cID == MINIPARTY_WINDOW )
				{
					//UiShowGroupBottom ( MINIPARTY_OPEN );
					//// #ifdef CH_PARAM // �߱� �������̽� ����
					////					UpdatePotionTrayPosition();
					//// #endif
					bAnotherProcess = true;	
				}
				else if ( cID == PRIVATE_MARKET_WINDOW )
				{
					if ( m_pPrivateMarketWindow->IsOPENER () )
					{
						DoModal ( ID2GAMEINTEXT("PRIVATE_MARKET_QUIT"), UI::MODAL_QUESTION, UI::YESNO, UI::MODAL_PRIVATE_MARKET_QUIT );
					}
					else
					{
						SetPrivateMarketClose ();
					}
					bAnotherProcess = true;					
				}
				else if( cID == ITEM_REBUILD_WINDOW )	// ITEMREBUILD_MARK
				{
					SetItemRebuildWindowClose();
				}
				else if( cID == ITEM_GARBAGE_WINDOW )	// ������
				{
					CloseItemGarbageWindow();
				}
				else if( cID == PRODUCT_WINDOW )	// ������ ����
				{
					ToggleWindowProduct();
				}
				else if ( cID == WAITSERVER_DISPLAY || cID == REBIRTH_DIALOGUE || cID == MAP_REQUIRE_CHECK  || cID == FORCE_REBIRTH )
				{
					bAnotherProcess = true;
				}
				else if ( (MODAL_WINDOW + 1) <= cID )
				{
					//					CLOSE_MODAL ( cID, false );
					bAnotherProcess = true;
				}


				if ( !bAnotherProcess )	
				{
					UiHideGroup ( cID );
				}

				bALLHIDE = false;
				break;
			}
		}

		if ( bALLHIDE )
		{
			if( UiIsVisibleGroup( INFO_DISPLAY_LINK ) )
			{
				CLEAR_TEXT ();
				bALLHIDE = FALSE;
			}
		}

		if ( bALLHIDE )
		{
			ToggleWindowEscMenu();
		}
	}

	if ( UiIsVisibleGroup ( ESC_MENU ) )			return ;
	if ( UiIsVisibleGroup ( HELP_WINDOW ) )			return ;
	if ( UiIsVisibleGroup ( ITEMSHOP_WINDOW ) )		return ;
	if ( UiIsVisibleGroup ( KEY_SETTING_WINDOW ) )	return ;


	GLCharacter* const pCharacter = m_pGaeaClient->GetCharacter ();
	//if ( pCharacter )
	//{
	//	//	�� ��ų
	//	for( int i = 0; i < QUICK_SKILL_SLOT_MAX; ++i)
	//	{
 //          		 if ( UIKeyCheck::GetInstance()->Check ( RANPARAM::SkillSlot[i], DXKEY_DOWN ) )
	//		{
	//			INT nIndex = m_pSkillTrayTab->GetTabIndex() * QUICK_SKILL_SLOT_MAX + i;
	//			if ( S_OK == pCharacter->ReqSkillRunSet( nIndex ) )
	//			{	
	//				// ��ų�� ����ÿ� ���� ���⿡ �ش� ��ų�� ����� �� �ִ� ���Ⱑ 
	//				// �������̶�� �����Ѵ�. ��, �̹� ���� ��� ����ÿ��� ���� �ʴ´�.
	//				AutoArmSwap();
	//			}
	//		}
	//	}
	//}


	if ( UIKeyCheck::GetInstance()->Check ( DIK_F7, DXKEY_DOWN) )
	{
		if ( !UiIsVisibleGroup ( RANKING_DISPLAY ) )	SetRankingWindowOpen();
		else UiHideGroup ( RANKING_DISPLAY );
	}


	


	////	��Ű ���� ( �̹� �������̸� ���� X )
	//if ( UIKeyCheck::GetInstance()->Check( DIK_TAB, DXKEY_DOWN ) && !m_bArmsSwapReserve )
	//{
	//	EMCHARCLASS emClass = m_pGaeaClient->GetCharacter()->m_emClass;

	//	if( emClass&GLCC_EXTREME_M || emClass&GLCC_EXTREME_W )
	//	{
	//		m_bArmsSwapReserve = true;

	//		// �̸� ��ų ������ ü���� �Ѵ�. AutoArmSwap����̶� �ߺ� ���� ����
 //           bool bMain = (0!=pCharacter->IsUseArmSub());
	//		AutoSkillSolt( bMain );
	//	}
	//}

	////	��Ű ���� ���̸�
	//if ( m_bArmsSwapReserve ) 
	//{
	//	if ( S_OK == m_pGaeaClient->GetCharacter()->ReqSlotChange() ) 
	//	{
	//		m_bArmsSwapReserve = false;
	//		// ��ų ���� ü����
	//	}
	//}

	//if ( UIKeyCheck::GetInstance()->CheckSimple ( DIK_V, DXKEY_DOWN ) )
	//{
	//	if (m_pGaeaClient->GetTradeClient()->Valid())
 //           return;

	//	GLCharacter* pCharacter = m_pGaeaClient->GetCharacter();
	//	if (pCharacter->VehicleActiveValue() && m_fVehicleDelay < 1.0f)
	//	{
	//		PrintMsgText(NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("VEHICLE_SET_FB_DELAY"));
	//		return;
	//	}

	//	pCharacter->ReqSetVehicle(!pCharacter->VehicleState());
 //       m_fVehicleDelay = 0.0f;
	//}

	if ( UIKeyCheck::GetInstance()->CheckSimple ( DIK_LMENU, DXKEY_PRESSED ) )
	{
		for (int i= 0; i < CHAT_MACRO_SIZE; ++i)
		{
			if ( UIKeyCheck::GetInstance()->CheckSimple ( DIK_1+i, DXKEY_DOWN ) )
			{			
				return ;
			}
		}
	}
	
	//if ( UIKeyCheck::GetInstance()->Check ( RANPARAM::MenuShotcut[SHOTCUT_INVEN], DXKEY_DOWN ) )
	//{		
	//	if ( !UiIsVisibleGroup ( INVENTORY_WINDOW ) && !UiIsVisibleGroup ( TRADEINVENTORY_WINDOW ) )
	//	{
	//		UiShowGroupFocus( INVENTORY_WINDOW );
	//	}
	//	else
	//	{
	//		UiHideGroup ( INVENTORY_WINDOW );
	//	}
	//}


	//if ( UIKeyCheck::GetInstance()->Check ( RANPARAM::MenuShotcut[SHOTCUT_SKILL], DXKEY_DOWN ) )
	//{		
	//	if ( UiIsVisibleGroup( STATSRESET_WINDOW ) )
	//	{
	//		PrintMsgText( NS_UITEXTCOLOR::RED, ID2GAMEINTEXT("STATSRESET_ACTION_WARNING") );
	//	}
	//	else 
	//	{
	//		if ( !UiIsVisibleGroup ( SKILL_WINDOW ) )
	//			UiShowGroupFocus ( SKILL_WINDOW );
	//		else
	//			UiHideGroup ( SKILL_WINDOW );
	//	}
	//}

	//if ( UIKeyCheck::GetInstance()->Check ( RANPARAM::MenuShotcut[SHOTCUT_PARTY], DXKEY_DOWN ) )
	//{		
	//	if ( !UiIsVisibleGroup ( PARTY_WINDOW ) )
	//		UiShowGroupFocus ( PARTY_WINDOW );
	//	else
	//		UiHideGroup ( PARTY_WINDOW );
	//}

	//if ( UIKeyCheck::GetInstance()->Check ( RANPARAM::MenuShotcut[SHOTCUT_CHAR], DXKEY_DOWN ) )
	//{		
	//	if ( UiIsVisibleGroup( STATSRESET_WINDOW ) )
	//	{
	//		PrintMsgText( NS_UITEXTCOLOR::RED, ID2GAMEINTEXT("STATSRESET_ACTION_WARNING") );
	//	}
	//	else 
	//	{
	//		if ( !UiIsVisibleGroup ( CHARACTER_WINDOW ) )
	//			UiShowGroupFocus ( CHARACTER_WINDOW );
	//		else
	//			UiHideGroup ( CHARACTER_WINDOW );
	//	}
	//}

	//if ( UIKeyCheck::GetInstance()->CheckSimple ( RANPARAM::MenuShotcut[SHOTCUT_MAP], DXKEY_DOWN ) )
	//{
	//	if( m_pGaeaClient->IsInstantMap() )
	//	{
	//		UiHideGroup ( LARGEMAP_WINDOW );
	//		PrintMsgText (NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("MINIMAP_CANNOTSEE") );
	//	}else{
	//		if ( !UiIsVisibleGroup ( LARGEMAP_WINDOW ) )
	//			UiShowGroupFocus ( LARGEMAP_WINDOW );
	//		else
	//			UiHideGroup ( LARGEMAP_WINDOW );
	//	}
	//}

	//if ( UIKeyCheck::GetInstance()->CheckSimple ( RANPARAM::MenuShotcut[SHOTCUT_CLUB], DXKEY_DOWN ) )
	//{
	//	if ( !UiIsVisibleGroup ( CLUB_WINDOW ) )
	//		UiShowGroupFocus ( CLUB_WINDOW );
	//	else
	//		UiHideGroup ( CLUB_WINDOW );
	//}

	//if ( UIKeyCheck::GetInstance()->Check ( RANPARAM::MenuShotcut[SHOTCUT_QUEST], DXKEY_DOWN ) )
	//{
	//	if ( !UiIsVisibleGroup ( QUEST_WINDOW ) )
	//	{			
	//		if ( NATIVEID_NULL().dwID == m_dwEventQuestID )
	//		{
	//			UiShowGroupFocus ( QUEST_WINDOW );
	//			REFRESH_QUEST_WINDOW ();				
	//		}
	//		else
	//		{
	//			SetQuestWindowOpen ( m_dwEventQuestID );
	//		}
	//	}
	//	else
	//	{
	//		UiHideGroup ( QUEST_WINDOW );
	//	}
	//}

	//if ( UIKeyCheck::GetInstance()->Check ( RANPARAM::MenuShotcut[SHOTCUT_HELP], DXKEY_DOWN) )
	//{
	//	if ( !UiIsVisibleGroup ( HELP_WINDOW ) )
	//	{
	//		UiHideGroup ( ITEMSHOP_WINDOW );
	//		UiShowGroupFocus ( HELP_WINDOW );
	//	}
	//	else
	//	{
	//		UiHideGroup ( HELP_WINDOW );
	//	}
	//}

    //// bjju.sns
    //if ( UIKeyCheck::GetInstance()->Check ( RANPARAM::MenuShotcut[SHOTCUT_SNS], DXKEY_DOWN) )
    //{
    //    if (GLUseFeatures::GetInstance().IsUsingSNS() )
    //    {
    //        if ( !UiIsVisibleGroup ( SNS_WINDOW ) )
    //        {
    //            UiShowGroupFocus ( SNS_WINDOW );
    //        }
    //        else
    //        {
    //            UiHideGroup ( SNS_WINDOW );
    //        }
    //    }
    //}

    //if ( UIKeyCheck::GetInstance()->Check ( RANPARAM::MenuShotcut[SHOTCUT_POINT_SHOP], DXKEY_DOWN) )
    //{
    //    if ( GLUseFeatures::GetInstance().IsUsingPointShop() )
    //    {
    //        if ( !UiIsVisibleGroup( POINT_SHOP_WINDOW ) )
    //        {
    //            UiShowGroupFocus( POINT_SHOP_WINDOW );

    //            GetPointShopWindow()->RefreshWindow();
    //        }
    //        else
    //        {
    //            UiHideGroup( POINT_SHOP_WINDOW );
    //        }
    //    }
    //}


//#if defined ( CH_PARAM ) || defined ( TH_PARAM ) || defined( PH_PARAM ) || defined ( JP_PARAM ) || defined(_RELEASED) // �ʸ��� �����ۼ�
//	if ( UIKeyCheck::GetInstance()->Check ( RANPARAM::MenuShotcut[SHOTCUT_POINT_SHOP], DXKEY_DOWN) )
//	{
//		if ( !UiIsVisibleGroup ( ITEMSHOP_WINDOW ) )
//		{
//			//	ItemShopAuth
//			if ( ItemShopAuth() )
//			{
//				UiShowGroupFocus ( ITEMSHOP_WINDOW );
//			}
//
//			UiHideGroup ( HELP_WINDOW );
//		}
//		else
//		{
//			UiHideGroup ( ITEMSHOP_WINDOW );
//		}
//	}
//	
//	//	ItemShopAuth	
//	ItemShopVisible();
//
//#endif	

	//if ( UIKeyCheck::GetInstance()->Check ( RANPARAM::MenuShotcut[SHOTCUT_FRIEND], DXKEY_DOWN ) )
	//{
	//	if ( UiIsVisibleGroup ( FRIEND_WINDOW ) )		UiHideGroup ( FRIEND_WINDOW );			
	//	else										UiShowGroupFocus ( FRIEND_WINDOW );
	//}

	//if ( UIKeyCheck::GetInstance()->Check ( RANPARAM::MenuShotcut[SHOTCUT_ITEMBANK], DXKEY_DOWN ) )
	//{
	//	if ( UiIsVisibleGroup ( ITEMBANK_WINDOW ) )	UiHideGroup ( ITEMBANK_WINDOW );
	//	else										SetItemBankWindowOpen ();

	//	/*if ( !UiIsVisibleGroup ( ITEMBANK_WINDOW ) )	
	//	{
	//		SetItemBankWindowOpen();			
	//	}*/
	//}

	/*if ( UIKeyCheck::GetInstance()->Check ( RANPARAM::MenuShotcut[SHOTCUT_VIETNAMGAINSYS], DXKEY_DOWN ) )
	{
		if ( UiIsVisibleGroup ( VNGAINSYS_WINDOW ) )	UiHideGroup ( VNGAINSYS_WINDOW );
		else										SetVNGainSysWindowOpen();
	}*/

	//if ( UIKeyCheck::GetInstance()->Check ( RANPARAM::MenuShotcut[SHOTCUT_CHATMACRO], DXKEY_DOWN ) )
	//{
	//	if ( UiIsVisibleGroup ( CHATMACRO_WINDOW ) )
	//	{
	//		UiHideGroup ( CHATMACRO_WINDOW );
	//	}
	//	else
	//	{
	//		GetChatMacro();
	//		UiShowGroupFocus ( CHATMACRO_WINDOW );
	//	}
	//}


	// ��ų ���� F4Ű�� �Ҵ��ϸ鼭 �̺κ��� ���� �Ѵ�.
/*
	if ( UIKeyCheck::GetInstance()->CheckSimple ( DIK_LMENU, DXKEY_PRESSED ) )
	{
		if ( UIKeyCheck::GetInstance()->CheckSimple ( DIK_F4, DXKEY_DOWN ) )
		{			
			DoModal ( ID2GAMEINTEXT("CLOSE_GAME"), UI::MODAL_QUESTION, UI::OKCANCEL, UI::MODAL_CLOSEGAME );
		}
	}
*/


////#ifndef CH_PARAM // �߱� �������̽� ����
//	if( UIKeyCheck::GetInstance()->Check ( DIK_GRAVE, DXKEY_DOWN ) )
////		UIKeyCheck::GetInstance()->Check ( DIK_Y, DXKEY_DOWN ) )
//	{
//		if ( m_pGaeaClient->GetTutorial()->IsTutorial() ) // Ʃ�丮�� �߿��� ������.
//			PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("TUTORIAL_CANNOT_USE") );
//		else
//		{
//			SkilSlotTrayToggle();
//		}
//	}
////#endif

	//if( UIKeyCheck::GetInstance()->Check( DIK_F1, DXKEY_DOWN ) )	
	//	m_pSkillTrayTab->SetTabIndex( 0 );
	//if( UIKeyCheck::GetInstance()->Check( DIK_F2, DXKEY_DOWN ) )	
	//	m_pSkillTrayTab->SetTabIndex( 1 );
	//if( UIKeyCheck::GetInstance()->Check( DIK_F3, DXKEY_DOWN ) )	
	//	m_pSkillTrayTab->SetTabIndex( 2 );
	//if( UIKeyCheck::GetInstance()->Check( DIK_F4, DXKEY_DOWN ) )	
	//	m_pSkillTrayTab->SetTabIndex( 3 );

	//if( UIKeyCheck::GetInstance()->Check( RANPARAM::MenuShotcut[SHOTCUT_PET], DXKEY_DOWN ) )
	//{
	//	if( m_pGaeaClient->GetPetClient()->IsVALID() )
	//	{
	//		if ( m_pGaeaClient->GetTradeClient()->Valid() ) return;
	//		
	//		if ( UiIsVisibleGroup( PET_WINDOW ) )		UiHideGroup ( PET_WINDOW );
	//		else									UiShowGroupFocus( PET_WINDOW );			
	//	}
	//}

    //if( UIKeyCheck::GetInstance()->Check( DIK_Y, DXKEY_DOWN ) )
    //{
    //    if ( IsStudentRecordOpenable() )
    //    {
    //        if ( UiIsVisibleGroup ( STUDENTRECORD_WINDOW ) )
    //        {
    //            UiHideGroup ( STUDENTRECORD_WINDOW );
    //        }
    //        else
    //        {
    //            UiShowGroupFocus ( STUDENTRECORD_WINDOW );
    //        }
    //    }
    //}

	//if( UIKeyCheck::GetInstance()->Check( DIK_U, DXKEY_DOWN ) )
	//{
	//	if ( m_pCompetitionWindow )
	//	{
	//		if ( !CompetitionWindowIsOpen() )
	//		{
	//			CompetitionWindowOpen();
	//		}
	//		else
	//		{
	//			CompetitionWindowClose();
	//		}
	//	}
	//}

	//if( UIKeyCheck::GetInstance()->Check( DIK_N, DXKEY_DOWN ) )
	//{
 //       GLCharacter* pChar = m_pGaeaClient->GetCharacter();
	//	if (pChar->VehicleActiveValue())
	//	{
	//		if (m_pGaeaClient->GetTradeClient()->Valid())
 //               return;
	//		
	//		SITEMCUSTOM& sItemCustom = pChar->m_PutOnItems[SLOT_VEHICLE];
	//		if ( sItemCustom.sNativeID == NATIVEID_NULL() )
 //               return;
	//		if ( !pChar->VehicleActiveValue())
 //               return;


	//		SNATIVEID sNativeID = pChar->VehicleId();
	//		SITEM* pItem = GLogicData::GetInstance().GetItem( sNativeID );
	//		if (!pItem)
 //               return;

	//		if( pItem->sVehicle.emVehicleType == VEHICLE_TYPE_BIKE )
	//		{
	//			if ( UiIsVisibleGroup( BIKE_WINDOW ) )	
	//			{
	//				UiHideGroup ( BIKE_WINDOW );
	//			}
	//			else									
	//			{
	//				UiShowGroupFocus( BIKE_WINDOW );
	//			}
	//		}
	//		else if( pItem->sVehicle.emVehicleType == VEHICLE_TYPE_BOARD )
	//		{
	//			if ( UiIsVisibleGroup( VEHICLE_WINDOW ) )	
	//			{
	//				UiHideGroup ( VEHICLE_WINDOW );
	//			}
	//			else									
	//			{
	//				UiShowGroupFocus( VEHICLE_WINDOW );
	//			}
	//		}			
	//	}
	//}

	//if( UIKeyCheck::GetInstance()->Check( RANPARAM::MenuShotcut[SHOTCUT_QBOX], DXKEY_DOWN ) )
	//{
	//	m_pQBoxButton->QBOXButtonDown();
	//}
}

void CInnerInterface::UpdateStatus ()
{
	GLCharacter* const pCharacter = m_pGaeaClient->GetCharacter();

	if ( UiIsVisibleGroup ( SYSTEM_MESSAGE_WINDOW ) )
	{
		UIRECT rcGlobalPos = m_pSystemMessageWindowDummy->GetGlobalPos ();
		//rcGlobalPos.top += fMENU_LIFTUP;
		m_pSystemMessageWindow->SetGlobalPos ( rcGlobalPos );
	}	
	
	
	//	ĳ���� ���� ���� Ȯ��
	if ( pCharacter->IsDie () && m_pRebirthWindow->CapableVisible() )
	{
        if ( !UiIsVisibleGroup ( REBIRTH_WINDOW ) )
		{
			SITEM* pReviveItem = pCharacter->FindRevive();

			bool bItemRebirth = ( NULL != pReviveItem ) ? true : false;
			bool bAutoRebirth = false;
			bool bCantUseRebirth = true;

			//	��ȥ�� �� Ÿ�� üũ
			if( true == bItemRebirth ) 
			{
				if( NULL != pReviveItem &&
					true == pCharacter->IsCoolTime( pReviveItem->sBasicOp.sNativeID ) )
				{
					bItemRebirth = false;
				}
			}

			if( pCharacter->IsRestartPossible() == FALSE )
			{
				bItemRebirth = false;
				bCantUseRebirth = true;
			}

			if( true == bItemRebirth )
			{
				// �Ҹ� ��ȥ���̸� �ڵ� ��ȥ�� ���
				if( NULL  != pReviveItem &&
					false == pReviveItem->ISINSTANCE() )
					bAutoRebirth = true;
			}

            if ( m_pForceRebirth->IsForceRebirth() )
            {
                const long lResolution = uiman::GetInnerUIMan().GetResolution();
                WORD X_RES = HIWORD( lResolution );
                WORD Y_RES = LOWORD( lResolution );

                const UIRECT& rcLP = m_pForceRebirth->GetLocalPos();

                D3DXVECTOR2 vPos;
                vPos.x = ( X_RES - rcLP.sizeX ) * 0.5f;
                vPos.y = Y_RES * 0.6f;
                m_pForceRebirth->SetGlobalPos( vPos );

                UiShowGroupFocus( FORCE_REBIRTH, true );
            }
            else
            {
                bool bItemRebirth = (pCharacter->FindRevive ())?true:false;
                bool bAutoRebirth = false;
                bool bCantUseRebirth = FALSE;

                //	��ȥ�� �� Ÿ�� üũ
                if ( bItemRebirth ) 
				{
					SITEM* pITEM = (GLUseFeatures::GetInstance().IsUsingRenewInvenWindow()) ? pCharacter->GET_SLOT_ITEMDATA(SLOT_DECORATION) : pCharacter->GET_SLOT_ITEMDATA(SLOT_NECK);
                    if ( pITEM && pCharacter->IsCoolTime( pITEM->sBasicOp.sNativeID ) )
                    {
                        bItemRebirth = FALSE;
                    }
                }

                if( pCharacter->IsRestartPossible() == FALSE )
                {
                    bItemRebirth = FALSE;
                    bCantUseRebirth = TRUE;
                }

                if ( bItemRebirth )
                {
                    // �Ҹ� ��ȥ���̸� �ڵ� ��ȥ�� ���
					SITEM* pITEM = (GLUseFeatures::GetInstance().IsUsingRenewInvenWindow()) ? pCharacter->GET_SLOT_ITEMDATA(SLOT_DECORATION) : pCharacter->GET_SLOT_ITEMDATA(SLOT_NECK);
                    if ( pITEM && !pITEM->ISINSTANCE() ) bAutoRebirth = true;
                }

                m_pRebirthWindow->SetItemRebirth(pCharacter->m_sResurrectAbleState.isState(EM_RESURRECTION_SPECIAL_ITEM), false, false);

                UiShowGroupFocus( REBIRTH_WINDOW, true );
            }

            UiShowGroupFocus( BASIC_CHAT_BOX, true );

            if ( GLUseFeatures::GetInstance().IsUsingGroupChat() && (!UiIsVisibleGroup ( MIN_GROUP_CHAT_WINDOW )) )
            {
                UiShowGroupFocus ( MIN_GROUP_CHAT_WINDOW, true );
            }

            TriggerSystem::SClientRebirthMessage msg;
            m_pGaeaClient->SendTriggerMessage(&msg);
		}
	}
	else
	{
		UiHideGroup ( REBIRTH_DIALOGUE, true );
		UiHideGroup( REBIRTH_WINDOW, true );
		
		if ( IsGateOpen () )
		{			
			MAPNAMEVECTOR mapNameVector;
			pCharacter->DetectGateToMapName(mapNameVector);
			switch ( mapNameVector.size() )
			{
			case 0:
				UiHideGroup( MAPMOVE_DISPLAY );
				UiHideGroup ( MULTIMAPMOVE_DISPLAY );
				break;
			case 1:
				{ //	���� �� �̸��� ���ٸ�, �̵��� �� ����.					
					if ( m_pMapMoveDisplay == NULL )
						break;
					
					GLCharacter* pChar(m_pGaeaClient->GetCharacter());
					
					if ( pChar != NULL )
					{
						switch ( pChar->VehicleState() )
						{
						case EMVEHICLE_DRIVER:
						case EMVEHICLE_OFF:
							{
								const CString& strMapName = mapNameVector[0];
								m_pMapMoveDisplay->SetMapName ( strMapName + ID2GAMEINTEXT ( "MAPMOVE_MESSAGE" ) );

								UIRECT rcGlobalPos = m_pMapMoveDisplayDummy->GetGlobalPos ();
								rcGlobalPos.top -= fMENU_LIFTUP;
								m_pMapMoveDisplay->SetGlobalPos ( rcGlobalPos );
								UiShowGroupFocus( MAPMOVE_DISPLAY );
							}
							break;
						default:
							UiHideGroup( MAPMOVE_DISPLAY );
							UiHideGroup ( MULTIMAPMOVE_DISPLAY );
						}
					}
					else
					{
						UiHideGroup( MAPMOVE_DISPLAY );
						UiHideGroup ( MULTIMAPMOVE_DISPLAY );
					}
				}
				break;
			default:
				{
					if ( m_pMultiMapMoveDisplay == NULL || GLUseFeatures::GetInstance().IsUsingMultiGate() == false )
						break;

					GLCharacter* pChar(m_pGaeaClient->GetCharacter());

					if ( pChar != NULL )
					{
						switch ( pChar->VehicleState() )
						{
						case EMVEHICLE_DRIVER:
						case EMVEHICLE_OFF:
							{
								for ( MAPNAMEVECTORCITER _iteratorMapName(mapNameVector.begin()); _iteratorMapName != mapNameVector.end(); ++_iteratorMapName )
								{
									const CString& strMapName = *_iteratorMapName;
									m_pMultiMapMoveDisplay->AddMapName(strMapName);
								}
								m_pMultiMapMoveDisplay->Done();
								UIRECT rcGlobalPos = m_pMultiMapMoveDisplayDummy->GetGlobalPos ();
								rcGlobalPos.top -= fMENU_LIFTUP;
								m_pMultiMapMoveDisplay->SetGlobalPos ( rcGlobalPos );
								UiShowGroupFocus( MULTIMAPMOVE_DISPLAY );
							}
							break;
						default:
							UiHideGroup( MAPMOVE_DISPLAY );
							UiHideGroup ( MULTIMAPMOVE_DISPLAY );
							break;
						}
					}	
					else
					{
						UiHideGroup( MAPMOVE_DISPLAY );
						UiHideGroup ( MULTIMAPMOVE_DISPLAY );
					}
				}
			}
		}
		else
		{
			UiHideGroup( MAPMOVE_DISPLAY );
			UiHideGroup ( MULTIMAPMOVE_DISPLAY );
		}
	}

    // TODO : �ҹ� ���α׷� ���� �׽�Ʈ..
    /*if ( !IsBlockProgramFound() )
        SetBlockProgramFound( true );*/
    //
    
	//	����, ���� ���μ��� üũ
	if ( IsBlockProgramFound () )
	{
		if ( !UiIsVisibleGroup ( WAITSERVER_DISPLAY ) )
			DoModal ( ID2GAMEINTEXT("BLOCK_PROGRAM_FOUND"), UI::MODAL_INFOMATION, UI::OK, UI::MODAL_CLOSEGAME );
	}

    DxGlobalStage* pGlobalStage = m_pGaeaClient->GetGlobalStage();
	if (!pGlobalStage->IsEmulator()  && !m_bDuplicateLogin)	//��Ʈ��ũ ������ �������� �ߺ� �α����� �ƴ� ���
	{
		if (!pGlobalStage->IsSTATE(DxGlobalStage::EM_CHANGE))
		{
			if (!pGlobalStage->GetNetClient()->IsOnline())
			{
				if ( !UiIsVisibleGroup ( WAITSERVER_DISPLAY ) )
					DoModal ( ID2GAMEINTEXT("PLAYINTERFACE_5"), UI::MODAL_INFOMATION, UI::OK, UI::MODAL_CLOSEGAME );				
			}
		}
	}

	if ( m_pAdminMessageDisplay )
	{
		if ( UiIsVisibleGroup ( ADMIN_MESSAGE_DISPLAY ) )
		{
			if ( m_pAdminMessageDisplay->GetLifeTime () < 0.0f )
			{
				UiHideGroup ( ADMIN_MESSAGE_DISPLAY );			
			}
		}
	}

	//if ( m_pSimpleMessageMan )
	//{
	//if ( UiIsVisibleGroup ( SIMPLE_MESSAGE_MAN ) )
	//{
	//	if ( m_pSimpleMessageMan->GetLifeTime () < 0.0f )
	//	{
	//		UiHideGroup ( SIMPLE_MESSAGE_MAN );			
	//	}
	//}
	//	}
	
	UpdateStateQuestAlarm ();
	UpdateStateSimpleHP ();	

	if ( UiIsVisibleGroup ( ACADEMY_CONFT_DISPLAY ) || UiIsVisibleGroup ( SYSTEM_MESSAGE_WINDOW )
		|| m_pGaeaClient->GetTutorial()->IsTutorial() ) // ���� : Ʃ�丮�� �߿� ǥ�� ���� ����. by luxes.
	{
		UiHideGroup ( SIMPLE_MESSAGE_MAN );

		if ( m_pGaeaClient->GetTutorial()->IsTutorial() && m_pGaeaClient->GetTutorial()->m_dwQuest != NATIVEID_NULL().dwID )
		{
			SET_QUEST_HELPER( m_pGaeaClient->GetTutorial()->m_dwQuest );
			m_pGaeaClient->GetTutorial()->m_dwQuest = NATIVEID_NULL().dwID;
		}
	}
	else
	{
		// ��ʸ�Ʈ�� ��Ƽ�� �Ҽ��� ����.
		// TOURNAMENT_UNIQ_DISABLE
		bool bTOURNAMNET = m_pGaeaClient->GetTournamentClient()->GetIsTournamentView();

		if ( RANPARAM::bSHOW_TIP && !bTOURNAMNET)
		{
			UiShowGroupBottom ( SIMPLE_MESSAGE_MAN, true );
		}
		else
		{
			UiHideGroup ( SIMPLE_MESSAGE_MAN, true );
		}
	}

    //! Progress Display ������Ʈ
    if( m_pProgressDisplay )
    {
        m_pProgressDisplay->Update();		
    }

    //! ������ ���� ǥ�� UI ������Ʈ
    if ( m_pInfoCTFDisplay )
    {
        m_pInfoCTFDisplay->Update();
    }
}

void CInnerInterface::UpdateFocusOrder ()
{
    using namespace boost;
    enum EMORDERSTATE{ EMORDERSTATE_NONE, EMORDERSTATE_CHECK, EMORDERSTATE_CHANGE };
    enum EMORDERTUPLE{ INDEX_SRCID, INDEX_DESTID, INDEX_FUNCTION, INDEX_STATE };

    // �ش�ID, ������ �ڿ� �־���� ID, üũ�Լ�, ����
    typedef tuple<UIGUID, UIGUID, function<bool (void)>, EMORDERSTATE>  ORDERTUPLE;
    typedef std::list<ORDERTUPLE>                                       ORDERLIST;
    typedef ORDERLIST::iterator                                         ORDERLIST_ITER;
    lambda::constant_type<bool>::type                                   DEFAULT_FUNCTION(lambda::constant(true));
    
    static ORDERLIST OrderList;

    if ( !m_bInitUpdateOrder )
    {
        if ( !OrderList.empty() )
        {
            OrderList.clear();
        }

        OrderList.push_back( make_tuple(BASIC_INFO_VIEW,             BASIC_CHAT_BOX, !bind( &IBasicChatWindow::IsChatBegin, GetBasicChatWindow() ), EMORDERSTATE_NONE ) );

        OrderList.push_back( make_tuple(QUICK_SKILL_TRAY_TAB_WINDOW, BASIC_CHAT_BOX, !bind( &IBasicChatWindow::IsChatBegin, GetBasicChatWindow() ), EMORDERSTATE_NONE ) );
		
		// Non Close UI�� ä�ùڽ����� ������ ���� �־�� �Ѵ�;
		for( unsigned int i=0; i<m_vNonCloseUI.size(); ++i )
			OrderList.push_back( make_tuple(m_vNonCloseUI.at( i ), BASIC_CHAT_BOX, !bind( &IBasicChatWindow::IsChatBegin, GetBasicChatWindow() ), EMORDERSTATE_NONE ) );

        m_bInitUpdateOrder = true;
    }

    // �ʱ�ȭ
    BOOST_FOREACH ( ORDERTUPLE& sTuple, OrderList )
    {
        EMORDERSTATE& emState = tuples::get<INDEX_STATE>(sTuple);
        emState = EMORDERSTATE_NONE;
    }

    DWORD dwCount = (DWORD)OrderList.size();

    // ������ �˻��Ѵ�
    BOOST_REVERSE_FOREACH ( const CUIFocusContainer::SUICONTROL& sControl,
        uiman::GetInnerUIMan().GetFocusList()->GetFocusList() )
    {
        UIGUID cID = sControl.cID;

        BOOST_FOREACH ( ORDERTUPLE& sTuple, OrderList )
        {
            if ( tuples::get<INDEX_STATE>(sTuple) != EMORDERSTATE_NONE )
            {
                continue;
            }

            if ( cID == tuples::get<INDEX_SRCID>(sTuple) )
            {
                EMORDERSTATE& emState = tuples::get<INDEX_STATE>(sTuple);
                emState = EMORDERSTATE_CHECK;
                --dwCount;
            }
            else if ( cID == tuples::get<INDEX_DESTID>(sTuple) )
            {
                EMORDERSTATE& emState = tuples::get<INDEX_STATE>(sTuple);

                if ( tuples::get<INDEX_FUNCTION>(sTuple)() )
                {
                    emState = EMORDERSTATE_CHANGE;
                }
                else
                {
                    emState = EMORDERSTATE_CHECK;
                }

                --dwCount;
            }
        }

        if ( dwCount == 0 )
        {
            break;
        }
    }

    // �ٲ���� ��Ʈ�ѵ��� ��ġ�� �ٲ۴�
    BOOST_FOREACH ( ORDERTUPLE& sTuple, OrderList )
    {   
        if ( tuples::get<INDEX_STATE>(sTuple) != EMORDERSTATE_CHANGE )
        {
            continue;
        }

        UIGUID cSrcID  = tuples::get<INDEX_SRCID> (sTuple);
        UIGUID cDestID = tuples::get<INDEX_DESTID>(sTuple);

		CUIControl* pControl =
			uiman::GetInnerUIMan().FindControl( cSrcID );
		if ( NULL == pControl )
			continue;

        uiman::GetInnerUIMan().GetFocusList()
			->InsertAfter ( cDestID, cSrcID, pControl );
    }

/*
    CUIFocusContainer::UICONTROL_FOCUSLIST focus_list = uiman::GetInnerUIMan().GetFocusList()->GetFocusList();
    CUIFocusContainer::UICONTROL_FOCUSLIST_RITER riter = focus_list.rbegin ();
    CUIFocusContainer::UICONTROL_FOCUSLIST_RITER riter_end = focus_list.rend ();

    bool bBasicChatBoxShow = false;
    for ( ; riter != riter_end; ++riter )
    {
        UIGUID cID = riter->cID;
        if ( BASIC_INFO_VIEW == cID )
        {
            if ( bBasicChatBoxShow )
            {
                if ( !GetBasicChatWindow()->IsChatBegin() )
                {
                    uiman::GetInnerUIMan().GetFocusList()->InsertAfter ( BASIC_CHAT_BOX, BASIC_INFO_VIEW, riter->pUIControl );
                }
            }
            break;
        }
        else if ( BASIC_CHAT_BOX == cID )
        {
            bBasicChatBoxShow = true;
        }
    }
*/
}


void CInnerInterface::MoveBasicInfoWindow ()
{
	IUILeftTopGroup* pUILeftTopGroup = GetUILeftTopGroup();

//#ifdef CH_PARAM // �߱� �������̽� ����
//	pControl = m_pBasicQuickSkillSlot;
//#else
	//pControl = m_pUILeftTopGroup;
//#endif

	if ( !pUILeftTopGroup ) return;

	const UIRECT& rcBasicInfoViewDummy = m_pBasicInfoViewDummy->GetGlobalPos ();
	const UIRECT& rcLeftTopGroupDummy = m_pLeftTopGroupDummy->GetGlobalPos ();
	const UIRECT& rcBasicInfoView = m_pBasicInfoView->GetUIWindowGlobalPos ();
	const UIRECT& rcLeftTopGroup = pUILeftTopGroup->GetUIWindowGlobalPos ();

	const D3DXVECTOR2 vLeftTopGroup ( rcLeftTopGroup.left, rcLeftTopGroup.top );
	const D3DXVECTOR2 vLeftTopGroupDummy ( rcLeftTopGroupDummy.left, rcLeftTopGroupDummy.top );

	const D3DXVECTOR2 vBasicInfoViewDummy (rcBasicInfoViewDummy.left,rcBasicInfoViewDummy.top);
	const D3DXVECTOR2 vBasicInfoView (rcBasicInfoView.left,rcBasicInfoView.top);
	const D3DXVECTOR2 vDist = vBasicInfoViewDummy - vBasicInfoView;
	const float fDist = D3DXVec2LengthSq ( &vDist );

	if ( fDist > 900.0f )	//	��Ʈ���� ��������?
	{	
		if ( vBasicInfoViewDummy != vLeftTopGroup ) pUILeftTopGroup->SetUIWindowGlobalPos ( vBasicInfoViewDummy );
	}
	else
	{
		if ( vLeftTopGroupDummy != vLeftTopGroup )		pUILeftTopGroup->SetUIWindowGlobalPos ( vLeftTopGroupDummy );
		if ( vBasicInfoViewDummy != vBasicInfoView )	m_pBasicInfoView->SetUIWindowGlobalPos( vBasicInfoViewDummy );
	}
}

void CInnerInterface::BasicInfoViewDoubleClick()
{
	IUILeftTopGroup* pUILeftTopGroup = GetUILeftTopGroup();

//#ifdef CH_PARAM // �߱� �������̽� ����
//	pControl = m_pBasicQuickSkillSlot;
//#else
	//pControl = m_pUILeftTopGroup;
//#endif

	const UIRECT& rcBasicInfoViewDummy = m_pBasicInfoViewDummy->GetGlobalPos ();
	const UIRECT& rcLeftTopGroupDummy = m_pLeftTopGroupDummy->GetGlobalPos ();

	const D3DXVECTOR2 vLeftTopGroupDummy ( rcLeftTopGroupDummy.left, rcLeftTopGroupDummy.top );
	const D3DXVECTOR2 vBasicInfoViewDummy (rcBasicInfoViewDummy.left,rcBasicInfoViewDummy.top);

	pUILeftTopGroup->SetUIWindowGlobalPos ( vLeftTopGroupDummy );
	m_pBasicInfoView->SetUIWindowGlobalPos( vBasicInfoViewDummy );
}

//************************************
// Method:    IsOpenWindowToMoveBlock
// FullName:  CInnerInterface::IsOpenWindowToMoveBlock
// Access:    public 
// Returns:   BOOL
// Qualifier: 
//************************************
BOOL CInnerInterface::IsOpenWindowToMoveBlock()
{
	// ���⿡ �߰��Ǵ� ��Ʈ���� �ݵ��� InitDeviceObjects���� ������ SetVisibleSingle( FALSE )�� ����� �Ѵ�.
	if( UiIsVisibleGroup( TRADE_WINDOW ) || 
		UiIsVisibleGroup( TRADEINVENTORY_WINDOW ) || 
		UiIsVisibleGroup( PRIVATE_MARKET_WINDOW ) ||
 		IsBankOpen() ||
		UiIsVisibleGroup( MAP_REQUIRE_CHECK ) ||
		UiIsVisibleGroup( REBIRTH_DIALOGUE ) ||
		UiIsVisibleGroup( PRIVATE_MARKET_MAKE ) ||
 		UiIsVisibleGroup( ITEM_GARBAGE_WINDOW ) || 
		UiIsVisibleGroup( ITEMSHOP_WINDOW ) ||
 		UiIsVisibleGroup( ITEM_SHOP_SEARCH_WINDOW ) ||
 		UiIsVisibleGroup( ITEM_SEARCH_RESULT_WINDOW ) || // ������ �˻� â
        UiIsVisibleGroup( PETSKIN_MIX_IMAGE_WINDOW ) ||
		UiIsVisibleGroup( COSTUME_STATS_WINDOW ) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool CInnerInterface::IsOpenWindowFromNPC()
{
    RANGE_MANAGEMENT_CONTROL rangeManagement = m_mmManageControl.equal_range( UI::EMMANAGECONTROL_TYPE_NPCACTIONABLE );

    for ( ITER_MANAGEMENT_CONTROL iter = rangeManagement.first; iter != rangeManagement.second; ++iter )
    {
        CUIControl* pControl = iter->second;

        if ( !pControl )
        {
            continue;
        }

        if ( pControl->IsVisible() )
        {
            return true;
        }
    }

    //if ( UiIsVisibleGroup( MARKET_WINDOW ) ||
    //    UiIsVisibleGroup( STORAGE_WINDOW ) ||
    //    UiIsVisibleGroup( CLUB_STORAGE_WINDOW ) ||
    //    UiIsVisibleGroup( DIALOGUE_WINDOW ) ||
    //    UiIsVisibleGroup( ITEM_REBUILD_WINDOW ) || 
    //    UiIsVisibleGroup( ITEM_GARBAGE_WINDOW ) || 
    //    UiIsVisibleGroup( ITEM_MIX_INVEN_WINDOW ) ||
    //    UiIsVisibleGroup( ITEM_MIX_WINDOW ) )
    //{
    //    return true;
    //}
    //

    return false;
}

void CInnerInterface::CloseAllWindowFromNPC()
{
    RANGE_MANAGEMENT_CONTROL rangeManagement = m_mmManageControl.equal_range( UI::EMMANAGECONTROL_TYPE_NPCACTIONABLE );

    for ( ITER_MANAGEMENT_CONTROL iter = rangeManagement.first; iter != rangeManagement.second; ++iter )
    {
        CUIControl* pControl = iter->second;

        if ( !pControl )
        {
            continue;
        }

        if ( !pControl->IsVisible() )
        {
            continue;
        }

        UIGUID cControlID = pControl->GetWndID();

        if ( cControlID == MARKET_WINDOW )
        {
            SetMarketWindowClose();
        }
        else if ( cControlID == STORAGE_WINDOW )
        {
            SetStorageWindowClose();
        }
        else if ( cControlID == CLUB_STORAGE_WINDOW )
        {
            SetClubStorageWindowClose();
        }
        else if ( cControlID == ITEM_REBUILD_WINDOW )
        {
            SetItemRebuildWindowClose();
        }
        else if ( cControlID == ITEM_MIX_WINDOW || cControlID == ITEM_MIX_INVEN_WINDOW )
        {
            SetItemMixWindowClose();
        }
        else if( cControlID == GAMBLE_DIALOGUE || cControlID == GAMBLE_SELECT_DIALOGUE || 
            cControlID == GAMBLE_AGAIN_DIALOGUE || cControlID == GAMBLE_ANIMATION_DIALOGUE || 
            cControlID == GAMBLE_RESULT_EVEN_DIALOGUE || cControlID == GAMBLE_RESULT_ODD_DIALOGUE  )
        {
            ToggleWindowMiniGame();
        }
        else
        {
            UiHideGroup( cControlID );
        }
    }
}

/*
#ifdef CH_PARAM // �߱� �������̽� ����
void CInnerInterface::UpdatePotionTrayPosition()
{
	m_pUILeftTopGroup->SetGlobalPos( m_pBasicPotionTrayDummy->GetGlobalPos() );
	m_pQuestAlarm->SetGlobalPos( m_pQuestAlarmDummy->GetGlobalPos() );

	BOOL bVisibleQuest = UiIsVisibleGroup( QUEST_ALARM);
	BOOL bVisibleMini = UiIsVisibleGroup( MINIPARTY_OPEN );

	if( bVisibleQuest && bVisibleMini ) return;

	UIRECT rcDummy = m_pUILeftTopGroup->GetGlobalPos();
	rcDummy.left += 41; // ����Ʈ, �̴���Ƽ ��ư�� ���� ũ�Ⱑ 41
	rcDummy.right = rcDummy.left + rcDummy.sizeX;
	m_pUILeftTopGroup->SetGlobalPos( rcDummy );

	CUIControl * pUIControl(NULL);

	if( !bVisibleQuest && !bVisibleMini )
	{
		pUIControl = m_pUILeftTopGroup;
	}
	else if( !bVisibleMini )
	{
		pUIControl = m_pQuestAlarm;
	}

	if( pUIControl != NULL )
	{
		rcDummy = pUIControl->GetGlobalPos();
		rcDummy.left += 41;
		rcDummy.right = rcDummy.left + rcDummy.sizeX;
		pUIControl->SetGlobalPos( rcDummy );
	}
}
#endif
*/
void CInnerInterface::SetArmSwap( BOOL bSwap )
{
	if(m_pInventoryWindow)
		m_pInventoryWindow->SetArmSwapTabButton( bSwap );

	if( m_pCharacterWindow )
		m_pCharacterWindow->SetArmSwapTabButton();
}

bool CInnerInterface::ItemShopAuth ()
{
//	ItemShopAuth
//#if defined ( JP_PARAM )
	EMSERVICE_PROVIDER sp = m_pGaeaClient->GetServiceProvider();
	
	if (sp == SP_JAPAN)
	{
		if( !m_pItemShopWindowWeb )
			return false;

		if ( m_bItemShopLoad )
			return false;

		GLCharacter* pChar = m_pGaeaClient->GetCharacter();
		if ( !pChar )
			return false;

		CString strData = "";
		strData.Format( "%s?id=%s& uuid=%s", RANPARAM::ItemShopAddress, pChar->m_szUID, RANPARAM::GETJAPANUUID() );

		m_pItemShopWindowWeb->SetAddress( strData.GetString() );
		COMMON_WEB::Navigate( m_pItemShopWindowWeb->GetWebID(), strData.GetString(), TRUE );

		m_bItemShopLoad = true;

		// �ε� �Ϸ� �޼����� �ö����� ��ٸ���.
		return false;
	}
//#endif
	return true;
}

void CInnerInterface::ItemShopVisible()
{
//	ItemShopAuth
//#if defined ( JP_PARAM ) 
	EMSERVICE_PROVIDER sp = m_pGaeaClient->GetServiceProvider();

	if (sp == SP_JAPAN)
	{
		if( !m_pItemShopWindowWeb )
			return;

		if ( !m_bItemShopLoad )
			return;

		if( !COMMON_WEB::IsCompleteLoad( m_pItemShopWindowWeb->GetWebID() ) )
			return;

		UiShowGroupFocus ( ITEMSHOP_WINDOW );
		UiHideGroup ( HELP_WINDOW );

		m_bItemShopLoad = false;
	}
//#endif
}

void CInnerInterface::SetItemMixResult( CString strMsg, bool bSuccess, bool bFail )
{
	m_pItemMixWindow->SetItemMixResult( strMsg, bSuccess, bFail );
}

void CInnerInterface::SET_GATHER_GAUGE( DWORD dwGaeaID, float fTime )
{
	m_pGatherGauge->SetTime( fTime );
	m_pGatherGauge->SetMaterial ( dwGaeaID );
}

void CInnerInterface::VisibleCDMRanking( bool bVisible )
{
	if ( bVisible )
	{
		UiHideGroup( QUEST_HELPER_DISPLAY );
		UiShowGroupBottom ( CDM_RANKING_DISPLAY );
	}
	else
	{
		UiHideGroup( CDM_RANKING_DISPLAY );
		UiShowGroupBottom ( QUEST_HELPER_DISPLAY );
	}
}

void CInnerInterface::RefreashCDMRanking()
{
	if ( m_pCdmRankingDisplay && m_pCdmRankingDisplay->IsVisible() )
		m_pCdmRankingDisplay->RefreashCdmRanking();
}

void CInnerInterface::VisibleCDMSafeTime( bool bVisible, float fCDM_SAFE_TIME )
{
	if ( !m_pCdmSafeTimeDisplay ) return;
    
	// ���� ��ų ����â ��ġ ����
	if ( bVisible )
	{
		m_pCdmSafeTimeDisplay->SetCDMSafeTime( fCDM_SAFE_TIME );
		UiShowGroupBottom( CDM_SAFETIME_DISPLAY );
	}
	else
	{
		UiHideGroup( CDM_SAFETIME_DISPLAY );
	}
}

void CInnerInterface::CalcSkillDisplayPos( bool bVisible )
{
	if ( !m_pSkillTimeDisplay || !m_pCdmSafeTimeDisplay ) return;
    
	// ���� ��ų ����â ��ġ ����
	if ( bVisible )
	{
		const UIRECT& rcPos = m_pCdmSafeTimeDisplay->GetGlobalPos ();
		const D3DXVECTOR2 vPos (rcPos.right+1,rcPos.top);
		m_pSkillTimeDisplay->SetGlobalPos( vPos );
	}
	else
	{
		const UIRECT& rcPos = m_pCdmSafeTimeDisplay->GetGlobalPos ();
		const D3DXVECTOR2 vPos (rcPos.left,rcPos.top);
		m_pSkillTimeDisplay->SetGlobalPos( vPos );	
	}
}

void CInnerInterface::SetChatOption(WORD wChatFlag)
{
	if (m_pBasicChat)
        m_pBasicChat->SetChatOption(wChatFlag);
}

void CInnerInterface::SetChatType(DWORD dwChatType)
{
    if (m_pBasicChat)
        m_pBasicChat->SetChatType(dwChatType);
}

void CInnerInterface::ChatToLink( const SLINK_DATA_BASIC& sLinkDataBasic, const SITEMCUSTOM *pItemCustom/*GroupChat������ SITEMCUSTOM�� ����Ѵ�*/)
{
	/*
	//GroupChat
	if ( �׷�ä���̸� )
	{
		GASSERT(pItemCustom);

		if (pItemCustom == NULL)
			return;

		m_pGroupChat->ChatToLink(pItemCustom);	

		return;
	}
	*/
	CString strLinkName;
	DWORD dwColor = NS_UITEXTUTIL::ENABLE;

	int MAX_GROUP_CHAT_WINDOW_INDEX = 0;
	int MaxGroupChatOpen = static_cast<int>(m_pGaeaClient->GetChatGroup()->GetMaxGroupChatOpen());
	for ( int EMROOMINDEX = 0; EMROOMINDEX < MaxGroupChatOpen; EMROOMINDEX++ )
	{
		MAX_GROUP_CHAT_WINDOW_INDEX = EMROOMINDEX + MAX_GROUP_CHAT_WINDOW_START;

		if ( IsVisible( MAX_GROUP_CHAT_WINDOW_INDEX ) )
		{
			if ( m_pMaxChatWindow[EMROOMINDEX]->IsBeginEdit() )
			{
				if ( !GetLinkData( sLinkDataBasic, strLinkName, dwColor ) )
					return;

				m_pMaxChatWindow[EMROOMINDEX]->ChatToLink( sLinkDataBasic, strLinkName , dwColor, pItemCustom );
				return;
			}
		}
	}

	//CString strLinkName;
	//DWORD dwColor = NS_UITEXTUTIL::ENABLE;

	if ( !GetLinkData( sLinkDataBasic, strLinkName, dwColor ) )
		return;

	if( m_pBasicChat )
		m_pBasicChat->ChatToLink( sLinkDataBasic, UI::ToString( strLinkName ), dwColor );
}

void  CInnerInterface::SetPartyLinkOnce ( int nLinkIndex )
{
	MAP_PARTY_LINK_DATA_INDEX_ITER pos = m_mapPartyLinkData.find ( nLinkIndex );	
	if ( pos == m_mapPartyLinkData.end() ) return;

	// ��Ƽ���� UIǥ��
	GLPARTY_LINK& sPartyLink = pos->second;
	sPartyLink.bOnce = true;
}

void CInnerInterface::DeleteLinkData ( const SLINK_DATA_INDEX& sLinkIndex )
{
	int nLinkIndex = sLinkIndex.nLinkIndex;
	if ( nLinkIndex < 0 ) return;

	switch ( sLinkIndex.sLinkType.wMType ) 
	{
	case ITEM_LINK:
		{
			switch ( sLinkIndex.sLinkType.wSType ) 
			{
			case ITEM_LINK_INVEN:
			case ITEM_LINK_PUTON:
			case ITEM_LINK_USER_STORAGE:
			case ITEM_LINK_CLUB_STORAGE:
			case ITEM_LINK_CROW_SALE:
				{
					MAP_ITEM_LINK_DATA_INDEX_ITER pos = m_mapItemLinkData.find ( nLinkIndex );	
					if ( pos != m_mapItemLinkData.end() )
						m_mapItemLinkData.erase( pos );
				}
				break;
			}
		}
		break;
	case PARTY_LINK:
		{
			switch ( sLinkIndex.sLinkType.wSType ) 
			{
			case PARTY_LINK_RECRUIT:
			case PARTY_LINK_ACCEPT:
			case PARTY_LINK_REFUSE:
				{
					MAP_PARTY_LINK_DATA_INDEX_ITER pos = m_mapPartyLinkData.find ( nLinkIndex );	
					if ( pos != m_mapPartyLinkData.end() )
						m_mapPartyLinkData.erase( pos );
				}
				break;
			}
		}
		break;
	}
}

void CInnerInterface::SetPartyOption(const SPartyOption& PartyOption)
{
	m_pPartyWindowRenewal->SetPartyOption(PartyOption);
}
void CInnerInterface::SetPartyInfo(const DWORD dwPartyID, const DWORD dwHashKey, const DWORD dwMasterID, const DWORD dwTimeKey)
{
	if ( m_pPartyWindow ) m_pPartyWindow->SetPartyInfo(dwPartyID, dwHashKey, dwMasterID, dwTimeKey);
	if ( m_pPartyWindowRenewal ) m_pPartyWindowRenewal->SetPartyInfo(dwPartyID, dwHashKey, dwMasterID, dwTimeKey);
}

BOOL CInnerInterface::GetLinkdataIndex(SLINK_DATA_ITEM& itrLinkData, int nIdx)
{
	MAP_ITEM_LINK_DATA_INDEX_ITER pos = m_mapItemLinkData.find( nIdx );
	if ( pos == m_mapItemLinkData.end() )
		return FALSE;

	itrLinkData = pos->second;
	return TRUE;
}

bool CInnerInterface::LinkDataProcess( int nLinkIndex )
{
	// �Լ� Ȯ���� �ʿ��ϴ�;
	// �����ۿ� ���� ���� Ÿ���� �ٸ����� ����Ű�� ���� ��찡�ִ�;
	// �� ���콺 �Է������� ���Լ��δ� ĳġ�س��Ⱑ �����;
	if ( nLinkIndex < 0 ) return false;
	
	SLINK_DATA_INDEX* pLinkBasic = GetLinkBasicData( nLinkIndex );
	if ( !pLinkBasic ) return false;
	if ( pLinkBasic->nLinkIndex < 0 ) return false;

	int nDataIndex = pLinkBasic->nLinkIndex;

	switch ( pLinkBasic->sLinkType.wMType ) 
	{
	case ITEM_LINK:
		{
			switch ( pLinkBasic->sLinkType.wSType ) 
			{
			case ITEM_LINK_INVEN:
			case ITEM_LINK_PUTON:
			case ITEM_LINK_USER_STORAGE:
			case ITEM_LINK_CLUB_STORAGE:
			case ITEM_LINK_CROW_SALE:
				{
					MAP_ITEM_LINK_DATA_INDEX_ITER pos = m_mapItemLinkData.find( nDataIndex );
					if ( pos == m_mapItemLinkData.end() )
						return false;

					SLINK_DATA_ITEM& sData = pos->second;

					if( DxInputDevice::GetInstance().GetKeyState( DIK_LSHIFT ) & DXKEY_PRESSED )
					{
						WishList::CClient* pWishList = m_pGaeaClient->GetWishList();
						if( pWishList )
							pWishList->NewChatLinkItem( sData.sItem, sData.strCharName );
					}
					else
					{
						SHOW_LINK_ITEM_INFO( sData.sItem );
					}
				}
				return true;
			}
		}
		break;

	case PARTY_LINK:
		{
			switch ( pLinkBasic->sLinkType.wSType ) 
			{
			case PARTY_LINK_RECRUIT:
				{
					MAP_PARTY_LINK_DATA_INDEX_ITER pos = m_mapPartyLinkData.find ( nDataIndex );	
					if ( pos == m_mapPartyLinkData.end() )
						return false;

					// ��Ƽ���� UIǥ��
					const GLPARTY_LINK& sPartyLink = pos->second;

					//	��Ƽ�� Ȯ��
					if ( !sPartyLink.sPartyMem[0].VALID() )
						return false;

					SHOW_PARTY_LINK_WINDOW( nDataIndex, sPartyLink );

					return true;
				}
				break;
			case PARTY_LINK_ACCEPT:
				{
					MAP_PARTY_LINK_DATA_INDEX_ITER pos = m_mapPartyLinkData.find ( nDataIndex );	
					if ( pos == m_mapPartyLinkData.end() )
						return false;

					// ��Ƽ���� UIǥ��
					GLPARTY_LINK& sPartyLink = pos->second;

					//	��Ƽ�� Ȯ��
					if ( !sPartyLink.sPartyMem[0].VALID() ) return false;
					if ( sPartyLink.bOnce ) return false;

					sPartyLink.bOnce = true;
										
					m_pGaeaClient->GetCharacter()->ReqPartyLinkLure( true, sPartyLink.dwHashKey, sPartyLink.dwTimeKey, sPartyLink.sPartyMem[0].dwGaeaID );

					return true;
				}
				break;
			case PARTY_LINK_REFUSE:
				{
					MAP_PARTY_LINK_DATA_INDEX_ITER pos = m_mapPartyLinkData.find ( nDataIndex );	
					if ( pos == m_mapPartyLinkData.end() ) return false;

					// ��Ƽ���� UIǥ��
					GLPARTY_LINK& sPartyLink = pos->second;

					//	��Ƽ�� Ȯ��
					if ( !sPartyLink.sPartyMem[0].VALID() ) return false;

					if ( sPartyLink.bOnce ) return false;

					sPartyLink.bOnce = true;

					m_pGaeaClient->GetCharacter()->ReqPartyLinkLure( false, sPartyLink.dwHashKey, sPartyLink.dwTimeKey, sPartyLink.sPartyMem[0].dwGaeaID );

					return true;
				}
				break;
			}
		}
		break;

	case QUEST_LINK:
	case SKILL_LINK:
	case CROW_LINK:
	case MAP_LINK:
		{
		}
		break;
	}

	return false;
}

bool CInnerInterface::GetLinkData( const SLINK_DATA_BASIC& sLinkDataBasic, CString& strLinkName, DWORD& dwColor )
{
	CString strName;
	GLCharacter* pCharacter = m_pGaeaClient->GetCharacter();
	if ( pCharacter )
	{
		if ( pCharacter->GetLinkData( sLinkDataBasic, strLinkName, dwColor ) )	return true;
	}

	return false;	
}

SLINK_DATA_INDEX* CInnerInterface::GetLinkBasicData ( int nLinkIndex )
{
	MAP_LINK_DATA_INDEX_ITER pos = m_mapLinkData.find ( nLinkIndex );	
	if ( pos == m_mapLinkData.end() ) return NULL;	
	
	return &pos->second;
}

bool CInnerInterface::GetLinkName( CString& strLinkName, int nLinkIndex )
{
	if ( nLinkIndex < 0 ) return false;
	
	SLINK_DATA_INDEX* pLinkBasic = GetLinkBasicData( nLinkIndex );
	if ( !pLinkBasic ) return false;
	if ( pLinkBasic->nLinkIndex < 0 ) return false;

	int nDataIndex = pLinkBasic->nLinkIndex;	

	switch ( pLinkBasic->sLinkType.wMType ) 
	{
	case ITEM_LINK:
		{
			switch ( pLinkBasic->sLinkType.wSType ) 
			{
			case ITEM_LINK_INVEN:
			case ITEM_LINK_PUTON:
			case ITEM_LINK_USER_STORAGE:
			case ITEM_LINK_CLUB_STORAGE:
			case ITEM_LINK_CROW_SALE:			

				//	�̹� �׸��� ������� �����ϰ� ���ο� �����ͷ� ����
				MAP_ITEM_LINK_DATA_INDEX_ITER pos = m_mapItemLinkData.find ( nDataIndex );	
				if ( pos == m_mapItemLinkData.end() )
					return false;

				SLINK_DATA_ITEM& sData = pos->second;

				const SITEM* pItem = GLogicData::GetInstance().GetItem( sData.sItem.GetNativeID() );
				if ( !pItem )
					return false;

				strLinkName.Format( "%s%s%s", strLINK_LEFT_SIMBOL, sData.sItem.GETNAME().c_str(), strLINK_RIGHT_SIMBOL );
				
				return true;			
			}
		}
		break;
	case PARTY_LINK:
		{
			switch ( pLinkBasic->sLinkType.wSType ) 
			{
			case PARTY_LINK_RECRUIT:
				{
					MAP_PARTY_LINK_DATA_INDEX_ITER pos = m_mapPartyLinkData.find ( nDataIndex );	
					if ( pos == m_mapPartyLinkData.end() ) return false;
 
                    CString strLocation("");

                    // ���� ���� ǥ��
                    if ( m_pGaeaClient )
                    {
                        GLPARTY_LINK& sPartyLink = pos->second;
                        strLocation = m_pGaeaClient->GetMapName(sPartyLink.sMapID);
                    }

					strLinkName.Format( "%s%s, %s%s", strLINK_LEFT_SIMBOL, strLocation, ID2GAMEWORD( "CHAT_LINK_PARTY_RECRUIT", 1 ), strLINK_RIGHT_SIMBOL );					
										
					return true;
				}
				break;
			case PARTY_LINK_ACCEPT:
				{
					MAP_PARTY_LINK_DATA_INDEX_ITER pos = m_mapPartyLinkData.find ( nDataIndex );	
					if ( pos == m_mapPartyLinkData.end() ) return false;

					strLinkName.Format( "%s%s%s", strLINK_LEFT_SIMBOL, ID2GAMEWORD( "CHAT_LINK_PARTY_RECRUIT", 2 ), strLINK_RIGHT_SIMBOL );								
					
					return true;
				}
				break;
			case PARTY_LINK_REFUSE:
				{
					MAP_PARTY_LINK_DATA_INDEX_ITER pos = m_mapPartyLinkData.find ( nDataIndex );	
					if ( pos == m_mapPartyLinkData.end() ) return false;

					strLinkName.Format( "%s%s%s", strLINK_LEFT_SIMBOL, ID2GAMEWORD( "CHAT_LINK_PARTY_RECRUIT", 3 ), strLINK_RIGHT_SIMBOL );					
					
					return true;
				}
				break;
			}
		}
		break;
	case QUEST_LINK:
	case SKILL_LINK:
	case CROW_LINK:
	case MAP_LINK:
		{
		}
		break;
	}

	return false;

}

DWORD CInnerInterface::GetLinkColor( int nLinkIndex )
{
	DWORD dwColor = NS_UITEXTCOLOR::CHAT_NORMAL;
	if ( nLinkIndex < 0 ) return dwColor;
	
	SLINK_DATA_INDEX* pLinkBasic = GetLinkBasicData( nLinkIndex );
	if ( !pLinkBasic ) return dwColor;
	if ( pLinkBasic->nLinkIndex < 0 ) return dwColor;

	int nDataIndex = pLinkBasic->nLinkIndex;	

	switch ( pLinkBasic->sLinkType.wMType ) 
	{
	case ITEM_LINK:
		{
			switch ( pLinkBasic->sLinkType.wSType ) 
			{
			case ITEM_LINK_INVEN:
			case ITEM_LINK_PUTON:
			case ITEM_LINK_USER_STORAGE:
			case ITEM_LINK_CLUB_STORAGE:
			case ITEM_LINK_CROW_SALE:			

				//	�̹� �׸��� ������� �����ϰ� ���ο� �����ͷ� ����
				MAP_ITEM_LINK_DATA_INDEX_ITER pos = m_mapItemLinkData.find ( nDataIndex );	
				if ( pos == m_mapItemLinkData.end() )
					return dwColor;

				SLINK_DATA_ITEM& sData = pos->second;

				const SITEM* pItem = GLogicData::GetInstance().GetItem( sData.sItem.GetNativeID() );
				if ( !pItem )
					return dwColor;

                //������ ��ũ���� ������ �ٸ����� ���� 
                //dwColor = COMMENT::ITEMCOLOR[pItem->sBasicOp.emLevel];
                dwColor = ITEM_INFOR_COLOR::dwItemRank[ pItem->sBasicOp.emLevel ];


				return dwColor;			
			}
		}
		break;
	case PARTY_LINK:
		{
			switch ( pLinkBasic->sLinkType.wSType ) 
			{
			case PARTY_LINK_RECRUIT:
				{
					MAP_PARTY_LINK_DATA_INDEX_ITER pos = m_mapPartyLinkData.find ( nDataIndex );	
					if ( pos == m_mapPartyLinkData.end() ) return dwColor;

					dwColor = NS_UITEXTCOLOR::CHAT_LINK_PARTY_RECRUIT;

					return dwColor;
				}
				break;
			case PARTY_LINK_ACCEPT:
				{
					MAP_PARTY_LINK_DATA_INDEX_ITER pos = m_mapPartyLinkData.find ( nDataIndex );	
					if ( pos == m_mapPartyLinkData.end() ) return dwColor;

					dwColor = NS_UITEXTCOLOR::CHAT_LINK_PARTY_RECRUIT;
					return dwColor;
				}
				break;
			case PARTY_LINK_REFUSE:
				{
					MAP_PARTY_LINK_DATA_INDEX_ITER pos = m_mapPartyLinkData.find ( nDataIndex );	
					if ( pos == m_mapPartyLinkData.end() ) return dwColor;

					dwColor = NS_UITEXTCOLOR::CHAT_LINK_PARTY_RECRUIT;
					return dwColor;
				}
				break;
			}
		}
		break;
	case QUEST_LINK:
	case SKILL_LINK:
	case CROW_LINK:
	case MAP_LINK:
		{
		}
		break;
	}

	return dwColor;
}

int CInnerInterface::AddLinkDataToList_Client( const SLINK_DATA_BASIC& sLinkDataBasic )
{
	int nLinkIndex = -1;

	if ( !sLinkDataBasic.VALID() ) return nLinkIndex;

	switch ( sLinkDataBasic.sLinkType.wMType ) 
	{
	case ITEM_LINK:
		{
			switch ( sLinkDataBasic.sLinkType.wSType ) 
			{
			case ITEM_LINK_INVEN:
				{
					SINVEN_POS sInvenPos;
					sInvenPos.dwPos = sLinkDataBasic.dwData1;

					GLCharacter* pCharacter = m_pGaeaClient->GetCharacter();
					if ( !pCharacter ) break;

					SINVENITEM* pInvenItem = pCharacter->m_cInventory.GetItem( sInvenPos.wPosX, sInvenPos.wPosY );
					if ( !pInvenItem ) break;

					SITEMCUSTOM sItemCustom = pInvenItem->sItemCustom;
                  
					nLinkIndex = AddLinkDataToList (
						std::string( pCharacter->GetName() ),
						sLinkDataBasic.sLinkType,
						&sItemCustom,
						sizeof(SITEMCUSTOM) );					

					return nLinkIndex;
				}
				break;

			case ITEM_LINK_PUTON:
				{
					EMSLOT emSlot = (EMSLOT)sLinkDataBasic.dwData1;
					if ( emSlot >= SLOT_TSIZE ) break;

					GLCharacter* pCharacter = m_pGaeaClient->GetCharacter();
					if ( !pCharacter ) break;
					if ( !pCharacter->VALID_SLOT_ITEM( emSlot ) ) break;

					SITEMCUSTOM sItemCustom = pCharacter->m_PutOnItems[emSlot];

					nLinkIndex = AddLinkDataToList(
						std::string( pCharacter->GetName() ),
						sLinkDataBasic.sLinkType,
						&sItemCustom,
						sizeof(SITEMCUSTOM) );

					return nLinkIndex;
				}
				break;

			case ITEM_LINK_USER_STORAGE:
				{
					DWORD dwChannel = sLinkDataBasic.dwData1;
					if ( dwChannel >= MAX_CLUBSTORAGE )
                        break;
					
					SINVEN_POS sInvenPos;
					sInvenPos.dwPos = sLinkDataBasic.dwData2;

					GLCharacter* pCharacter = m_pGaeaClient->GetCharacter();
					if ( !pCharacter ) break;
					
					SINVENITEM* pInvenItem = pCharacter->m_cStorage[dwChannel].FindPosItem ( sInvenPos.wPosX, sInvenPos.wPosY );
					if ( !pInvenItem )	break;

					SITEMCUSTOM sItemCustom = pInvenItem->sItemCustom;

					nLinkIndex = AddLinkDataToList(
						std::string( pCharacter->GetName() ),
						sLinkDataBasic.sLinkType,
						&sItemCustom,
						sizeof(SITEMCUSTOM) );

					return nLinkIndex;
				}
				break;

			case ITEM_LINK_CLUB_STORAGE:
				{
					GLCharacter* pCharacter = m_pGaeaClient->GetCharacter();
					if ( !pCharacter )
						break;

					if (!m_pGaeaClient->GetMyClub()->IsValid())
                        break;
					if ( m_pGaeaClient->GetMyClub()->MasterCharDbNum() != pCharacter->m_CharDbNum )
                        break;

					DWORD dwChannel = sLinkDataBasic.dwData1;
					if ( dwChannel >= MAX_CLUBSTORAGE )
                        break;

					SINVEN_POS sInvenPos;
					sInvenPos.dwPos = sLinkDataBasic.dwData2;
					
					SINVENITEM* pInvenItem = m_pGaeaClient->GetMyClub()->FindPosItem(dwChannel, sInvenPos.wPosX, sInvenPos.wPosY);
					if (!pInvenItem)
                        break;

					SITEMCUSTOM sItemCustom = pInvenItem->sItemCustom;

					nLinkIndex = AddLinkDataToList(
						std::string( pCharacter->GetName() ),
						sLinkDataBasic.sLinkType,
						&sItemCustom,
						sizeof(SITEMCUSTOM) );

					return nLinkIndex;
				}
				break;

			case ITEM_LINK_CROW_SALE:
				{
					GLCharacter* pCharacter = m_pGaeaClient->GetCharacter();
					if ( !pCharacter )
						break;

					DWORD dwNpcGlobID = ( sLinkDataBasic.dwData1 & 0xFFFF0000 ) >> 16;
					DWORD dwChannel = ( sLinkDataBasic.dwData1 & 0x0000FFFF );

					GLLandManClient* pLand = m_pGaeaClient->GetActiveMap();
					if ( !pLand )	break;
					std::tr1::shared_ptr<GLCrowClient> pCrow = pLand->GetCrow ( dwNpcGlobID );
					if ( !pCrow )
                        break;

					if ( pCrow->m_pCrowData->GetSaleNum() <= dwChannel )
                        break;

					GLInventory *pInven = pCrow->m_pCrowData->GetSaleInven(dwChannel);
					if ( !pInven )	 break;

					SINVEN_POS sInvenPos;
					sInvenPos.dwPos = sLinkDataBasic.dwData2;
					
					SINVENITEM* pSaleItem = pInven->GetItem ( sInvenPos.wPosX, sInvenPos.wPosY );
					if ( !pSaleItem ) break;

					SITEMCUSTOM sItemCustom = pSaleItem->sItemCustom;

					nLinkIndex = AddLinkDataToList(
						std::string( pCharacter->GetName() ),
						sLinkDataBasic.sLinkType,
						&sItemCustom,
						sizeof(SITEMCUSTOM) );

					return nLinkIndex;
				}
				break;
			}
		}
		break;

	case PARTY_LINK:
		{
			switch ( sLinkDataBasic.sLinkType.wSType ) 
			{
			case PARTY_LINK_RECRUIT:
				{
					// ���� ������ �Է��ϴ°��̴�.
					// ��Ƽ���� �ƴϸ� �ȵȴ�.
					
					GLCharacter* pCharacter = m_pGaeaClient->GetCharacter();
					if ( pCharacter == NULL )
						break;

					GLPartyClient* const pMyParty = m_pGaeaClient->GetMyPartyClient();
					const bool bParty = pMyParty->isValid();
					if ( (bParty == true) && (pMyParty->isMaster(pCharacter->GetGaeaID()) == false) )
						break;

					GLPARTY_LINK sPartyLink;

					int nPartyMem = 1;

					if ( bParty ) 
                    {						
                        sPartyLink.sMapID  = sLinkDataBasic.dwData1;						

						for ( DWORD _i = 0; _i < pMyParty->GetNMember(); ++_i )
						{
							GLPartySlotClient* const pMember = pMyParty->GetMember(_i);			
							if ( pMember == NULL )
								continue;

							sPartyLink.sPartyMem[_i + 1].dwGaeaID = pMember->m_dwGaeaID;
							sPartyLink.sPartyMem[_i + 1].emClass = pMember->m_emClass;
							sPartyLink.sPartyMem[_i + 1].SetName(pMember->m_szName);
						}
					}
					else
                    {
                        sPartyLink.sMapID  = sLinkDataBasic.dwData1;

                        sPartyLink.sPartyMem[0].dwGaeaID = pCharacter->GetGaeaID();
						sPartyLink.sPartyMem[0].emClass  = pCharacter->m_emClass;
						sPartyLink.sPartyMem[0].SetName(pCharacter->m_szName);
						
					}
					
					nLinkIndex = AddLinkDataToList(
						std::string( pCharacter->GetName() ),
						sLinkDataBasic.sLinkType,
						&sPartyLink,
						sizeof(GLPARTY_LINK_MEM) * nPartyMem + sizeof(SNATIVEID) );

					return nLinkIndex;
				}
				break;
			case PARTY_LINK_ACCEPT:				//	�ش� ������ ����.
				break;
			case PARTY_LINK_REFUSE:				//	�ش� ������ ����.
				break;
			}
		}
		break;
	case QUEST_LINK:
	case SKILL_LINK:
	case CROW_LINK:
	case MAP_LINK:
		{
		}
		break;
	}

	return nLinkIndex;


}

int CInnerInterface::AddLinkDataToList ( const std::string& strCharName, const SLINK_TYPE& sLinkType, void* pBuffer, DWORD dwSize )
{
	int nLinkIndex = -1;

	if ( !sLinkType.VALID() )
		return nLinkIndex;

	//	Index�� ��� �Ҹ��� ��� 0���� �ʱ�ȭ 
	if ( m_dwLinkDataIndex == CHAT_LINK_INDEX_MAX ) 
	{
		m_dwLinkDataIndex = 0;
	}

	int nDataIndex = AddLinkDataToTypeList( strCharName, sLinkType, pBuffer, dwSize );
	if ( nDataIndex >= 0 ) 
	{
		SLINK_DATA_INDEX sLinkDataAdd;
		sLinkDataAdd.sLinkType = sLinkType;
		sLinkDataAdd.nLinkIndex = nDataIndex;
		m_mapLinkData.insert( std::make_pair( m_dwLinkDataIndex, sLinkDataAdd ) );
		nLinkIndex = m_dwLinkDataIndex;

		if ( m_mapLinkData.size() > CHAT_LINK_BASIC_DATA_MAX ) 
		{
			int nDelete = m_dwLinkDataIndex - CHAT_LINK_BASIC_DATA_MAX; 

			// 0���� �۴ٴ°��� Index�� �ѹ��� ���Ҵٴ� ���̴�.
			if ( nDelete < 0 ) 
			{
				nDelete += CHAT_LINK_INDEX_MAX;
			}

			//	���� ������ ���� ����
			MAP_LINK_DATA_INDEX_ITER pos = m_mapLinkData.find ( nDelete );	
			if ( pos != m_mapLinkData.end() )
			{
				const SLINK_DATA_INDEX& sLinkIndex = pos->second;
				DeleteLinkData( sLinkIndex );
				m_mapLinkData.erase( pos );				
			}
		}	

		m_dwLinkDataIndex++;

		return nLinkIndex;
	}


	return nLinkIndex;
	
}

int	CInnerInterface::AddLinkDataToTypeList ( const std::string& strCharName, const SLINK_TYPE& sLinkType, void* pBuffer, DWORD dwSize )
{
	int nIndex = -1;
	switch ( sLinkType.wMType ) 
	{
	case ITEM_LINK:
		{
			switch ( sLinkType.wSType ) 
			{
			case ITEM_LINK_INVEN:
			case ITEM_LINK_PUTON:
			case ITEM_LINK_USER_STORAGE:
			case ITEM_LINK_CLUB_STORAGE:
			case ITEM_LINK_CROW_SALE:	
				{
					SITEMCUSTOM sItemCustom;

					if ( dwSize != sizeof( SITEMCUSTOM ) )
						return nIndex;

					memcpy ( &sItemCustom, pBuffer, dwSize );

					//	Index�� ��� �Ҹ��� ��� 0���� �ʱ�ȭ 
					if ( m_dwItemLinkDataIndex == CHAT_LINK_INDEX_MAX )
					{
						m_dwItemLinkDataIndex = 0;
					}

					SLINK_DATA_ITEM sData;
					sData.strCharName = strCharName;
					sData.sItem = sItemCustom;

					m_mapItemLinkData.insert( std::make_pair( m_dwItemLinkDataIndex, sData ) );
					nIndex = m_dwItemLinkDataIndex;

					//	CHAT_LINK_DATA_MAX �����ŭ�� ������ �ִ´�.
					if ( m_mapItemLinkData.size() > CHAT_LINK_DATA_MAX ) 
					{
						int nDelete = m_dwItemLinkDataIndex - CHAT_LINK_DATA_MAX; 

						// 0���� �۴ٴ°��� Index�� �ѹ��� ���Ҵٴ� ���̴�.
						if ( nDelete < 0 ) 
						{
							nDelete += CHAT_LINK_INDEX_MAX;
						}
						
						MAP_ITEM_LINK_DATA_INDEX_ITER pos = m_mapItemLinkData.find ( nDelete );	
						if ( pos != m_mapItemLinkData.end() )
							m_mapItemLinkData.erase( pos );
					}

					m_dwItemLinkDataIndex++;

					return nIndex;
				}
				break;
			}
		}
		break;

	case PARTY_LINK:
		{
			switch ( sLinkType.wSType ) 
			{
			case PARTY_LINK_RECRUIT:
				{
					GLPARTY_LINK sPartyLink;
                    DWORD        dwOffset  = 0;
                    DWORD        dwCurSize = 0;

					// �ʾ��̵𺹻�
					memcpy ( &sPartyLink.sMapID, pBuffer, sizeof(SNATIVEID) );
					dwOffset  += sizeof(SNATIVEID);

					if ( dwSize < dwOffset ) return nIndex;
					dwCurSize = dwSize - dwOffset;

					if ( ( dwCurSize % sizeof( GLPARTY_LINK_MEM )) != 0 )
						return nIndex;

					int nPartyNum = dwCurSize / sizeof( GLPARTY_LINK_MEM );
					if ( nPartyNum > MAXPARTY )
						return nIndex;

					memcpy ( sPartyLink.sPartyMem, (BYTE*)pBuffer+dwOffset, dwCurSize );

					//	Index�� ��� �Ҹ��� ��� 0���� �ʱ�ȭ 
					if ( m_dwPartyLinkDataIndex == CHAT_LINK_INDEX_MAX )
					{
						m_dwPartyLinkDataIndex = 0;
					}

					m_mapPartyLinkData.insert( std::make_pair( m_dwPartyLinkDataIndex, sPartyLink ) );
					nIndex = m_dwPartyLinkDataIndex;

					//	CHAT_LINK_DATA_MAX �����ŭ�� ������ �ִ´�.
					if ( m_mapPartyLinkData.size() > CHAT_LINK_DATA_MAX ) 
					{
						int nDelete = m_dwPartyLinkDataIndex - CHAT_LINK_DATA_MAX; 

						// 0���� �۴ٴ°��� Index�� �ѹ��� ���Ҵٴ� ���̴�.
						if ( nDelete < 0 ) 
						{
							nDelete += CHAT_LINK_INDEX_MAX;
						}
						
						MAP_PARTY_LINK_DATA_INDEX_ITER pos = m_mapPartyLinkData.find ( nDelete );	
						if ( pos != m_mapPartyLinkData.end() )
							m_mapPartyLinkData.erase( pos );
					}

					m_dwPartyLinkDataIndex++;

					return nIndex;
				}
				break;

			case PARTY_LINK_ACCEPT:
				{
					const DWORD* const pBuffer4Byte((const DWORD* const)pBuffer);
					GLPARTY_LINK sPartyLink;					
					sPartyLink.sPartyMem[0].dwGaeaID = pBuffer4Byte[0];
					sPartyLink.dwHashKey = pBuffer4Byte[1];
					sPartyLink.dwTimeKey = pBuffer4Byte[2];

					//	Index�� ��� �Ҹ��� ��� 0���� �ʱ�ȭ 
					if ( m_dwPartyLinkDataIndex == CHAT_LINK_INDEX_MAX )
					{
						m_dwPartyLinkDataIndex = 0;
					}

					m_mapPartyLinkData.insert( std::make_pair( m_dwPartyLinkDataIndex, sPartyLink ) );
					nIndex = m_dwPartyLinkDataIndex;

					//	CHAT_LINK_DATA_MAX �����ŭ�� ������ �ִ´�.
					if ( m_mapPartyLinkData.size() > CHAT_LINK_DATA_MAX ) 
					{
						int nDelete = m_dwPartyLinkDataIndex - CHAT_LINK_DATA_MAX; 

						// 0���� �۴ٴ°��� Index�� �ѹ��� ���Ҵٴ� ���̴�.
						if ( nDelete < 0 ) 
						{
							nDelete += CHAT_LINK_INDEX_MAX;
						}
						
						MAP_PARTY_LINK_DATA_INDEX_ITER pos = m_mapPartyLinkData.find ( nDelete );	
						if ( pos != m_mapPartyLinkData.end() ) m_mapPartyLinkData.erase( pos );
					}

					m_dwPartyLinkDataIndex++;

					return nIndex;
				}
				break;

			case PARTY_LINK_REFUSE:
				{
					GLPARTY_LINK sPartyLink;
					memcpy ( &sPartyLink.sPartyMem[0].dwGaeaID, pBuffer, dwSize );

					//	Index�� ��� �Ҹ��� ��� 0���� �ʱ�ȭ 
					if ( m_dwPartyLinkDataIndex == CHAT_LINK_INDEX_MAX )
					{
						m_dwPartyLinkDataIndex = 0;
					}

					m_mapPartyLinkData.insert( std::make_pair( m_dwPartyLinkDataIndex, sPartyLink ) );
					nIndex = m_dwPartyLinkDataIndex;

					//	CHAT_LINK_DATA_MAX �����ŭ�� ������ �ִ´�.
					if ( m_mapPartyLinkData.size() > CHAT_LINK_DATA_MAX ) 
					{
						int nDelete = m_dwPartyLinkDataIndex - CHAT_LINK_DATA_MAX; 

						// 0���� �۴ٴ°��� Index�� �ѹ��� ���Ҵٴ� ���̴�.
						if ( nDelete < 0 ) 
						{
							nDelete += CHAT_LINK_INDEX_MAX;
						}
						
						MAP_PARTY_LINK_DATA_INDEX_ITER pos = m_mapPartyLinkData.find ( nDelete );	
						if ( pos != m_mapPartyLinkData.end() ) m_mapPartyLinkData.erase( pos );
					}

					m_dwPartyLinkDataIndex++;

					return nIndex;
				}
				break;
			}
		}
		break;

	case QUEST_LINK:
	case SKILL_LINK:
	case CROW_LINK:
	case MAP_LINK:
		{
		}
		break;
	}

	return nIndex;
}

void CInnerInterface::ConvertLinkData ( const VEC_LINK_TEXT_DATA_EX& vecLinkDataIN, VEC_LINK_TEXT_DATA& vecLinkDataOut )
{
	// Ŭ���̾�Ʈ ��ũ �����͸� ���������ͷ� ��ȯ 
	for ( DWORD i =0; i < vecLinkDataIN.size(); ++i )
	{
		int nLinkIndex = AddLinkDataToList_Client( vecLinkDataIN[i].sLinkDataBasic );
		
		if ( nLinkIndex >= 0 ) 
		{
			SLINK_TEXT_DATA sLinkTextData;
			sLinkTextData.sLinkPos = vecLinkDataIN[i].sLinkPos;
			sLinkTextData.dwTextColor = vecLinkDataIN[i].dwTextColor;
			sLinkTextData.nLinkTypeIndex = nLinkIndex;

			vecLinkDataOut.push_back( sLinkTextData );
		}
	}	
}

// ������ ���� ������ �����Ѵ�.
void CInnerInterface::ConvertSendData(CString& strInput, const VEC_LINK_TEXT_DATA_EX& vecLinkData, VEC_LINK_DATA_BASIC& vecLinkDataBasic)
{
    size_t LinkDataSize = vecLinkData.size();
	if (LinkDataSize == 0)
        return;

	for ( int i = static_cast<int>(vecLinkData.size()-1); i >= 0; --i ) 
	{
		strInput.Delete(vecLinkData[i].sLinkPos.nStart, vecLinkData[i].sLinkPos.nEnd-vecLinkData[i].sLinkPos.nStart);

		CString strLink;
        
		strLink.Format(
            "%s%d%d%s",
            strLINK_LEFT_SIMBOL,
            vecLinkData[i].sLinkDataBasic.sLinkType.wMType, 
			vecLinkData[i].sLinkDataBasic.sLinkType.wSType,
            strLINK_RIGHT_SIMBOL);

		strInput.Insert(vecLinkData[i].sLinkPos.nStart, strLink);
	}

	for (size_t i=0; i<LinkDataSize; ++i) 
	{
		vecLinkDataBasic.push_back(vecLinkData[i].sLinkDataBasic);
	}
}


void CInnerInterface::ConvertRecvData( CString& strText, VEC_LINK_TEXT_DATA& vecLinkTextData )
{
	if ( vecLinkTextData.empty() ) return;

	int nStart = 0;
	CString strFind1 = strLINK_LEFT_SIMBOL;
	CString strFind2 = strLINK_RIGHT_SIMBOL;
	CString strCon;
	
	size_t nIndex = 0;

	while (true) 
	{
		int nFind1 = strText.Find ( strFind1, nStart );
		if ( nFind1 < 0 )	break;

		int nFind2 = strText.Find ( strFind2, nFind1 );
		if ( nFind2 < 0 )	break;

		if ( nFind1+2+strFind1.GetLength() != nFind2 )
		{
			nStart = ++nFind1;
			continue;
		}

		int mType = strText.GetAt(nFind1+1) - '0';
		int sType = strText.GetAt(nFind1+2) - '0';

		if ( static_cast<int>(vecLinkTextData.size()) <= nIndex )
		{
			nStart = ++nFind1;
			break;
		}

		SLINK_DATA_INDEX* sLinkDataIndex = GetLinkBasicData( vecLinkTextData[nIndex].nLinkTypeIndex );
		if ( !sLinkDataIndex )
		{
			nStart = ++nFind1;
			continue;		
		}

		if ( sLinkDataIndex->sLinkType.wMType != mType || sLinkDataIndex->sLinkType.wSType != sType ) 
		{
			nStart = ++nFind1;
			continue;	
		}
		
		if ( !GetLinkName( strCon, vecLinkTextData[nIndex].nLinkTypeIndex ) )
		{
			strCon = strLINK_LEFT_SIMBOL+strLINK_RIGHT_SIMBOL;
		}

		strText.Delete ( nFind1, nFind2-nFind1+strFind2.GetLength() );

		nStart = nFind1;

		strText.Insert ( nFind1, strCon );

		nStart = nFind1;
		nStart += strCon.GetLength();

		vecLinkTextData[nIndex].sLinkPos.nStart = nFind1;
		vecLinkTextData[nIndex].sLinkPos.nEnd = nStart;

		nIndex++;
	}

	if ( nIndex < static_cast<int>(vecLinkTextData.size()) )
	{
		vecLinkTextData.erase( vecLinkTextData.begin()+nIndex, vecLinkTextData.end() );
	}
}

//	���Ⱑ �ٲ��. ( �ٲ� �������� ���´�. ) 
void CInnerInterface::AutoSkillSolt( bool bMain )
{
	//	��ų ���� ü����
	int nIndex = m_pSkillTrayTab->GetTabIndex();
	
	//	���� ����
	if ( bMain ) 
	{
		//F3,F4 �ϰ�� F1,F2�� ����.
		if ( nIndex == 2 ) nIndex = 0;
		if ( nIndex == 3 ) nIndex = 1;
		
		m_pSkillTrayTab->SetTabIndex( nIndex );
	}
	//	��������
	else
	{
		// F1,F2 �ϰ�� F3,F4�� ����.
		if ( nIndex == 0 ) nIndex = 2;
		if ( nIndex == 1 ) nIndex = 3;

		m_pSkillTrayTab->SetTabIndex( nIndex );
	}	
}

void CInnerInterface::AutoArmSwap ()
{
	//if ( m_bArmsSwapReserve ) return;

	GLCharacter* pCharacter = m_pGaeaClient->GetCharacter();
	if ( !pCharacter ) return;

	if( pCharacter->IsReqSwapArms() )
	{
		return;
	}

	EMCHARCLASS emClass = pCharacter->m_emClass;
	if( emClass != GLCC_EXTREME_M && emClass != GLCC_EXTREME_W ) return;

	SNATIVEID sSkillID = pCharacter->GetskillRunSlot();

	if ( sSkillID == NATIVEID_NULL() ) return;

	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSkillID.wMainID, sSkillID.wSubID );
	
	if ( !pSkill ) return;
	if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE )	return;

	//	Note : ������ ���� ���� �˻�.
	//

	EMSLOT emRHand, emRHand_S;

	if ( pCharacter->IsUseArmSub() )
	{
		emRHand = SLOT_RHAND_S;
		emRHand_S = SLOT_RHAND;
	}
	else
	{
		emRHand = SLOT_RHAND;
		emRHand_S = SLOT_RHAND_S;
	}

	const bool bHiddenWeapon = pSkill->m_sLEARN.bHiddenWeapon;
	GLITEM_ATT emITEM_RIGHT = ITEMATT_NOTHING;
	SKILL::GLSKILL_ATT emSKILL_RIGHT = pSkill->m_sBASIC.emUSE_RITEM;

	if ( emSKILL_RIGHT!=SKILL::SKILLATT_NOCARE )
	{
		SITEM* pItem = pCharacter->GET_SLOT_ITEMDATA(emRHand);
		
		//	�������� ���ų� ��ų ���� ���� �������� �ƴҶ� 
		if ( !pItem || !CHECHSKILL_ITEM(emSKILL_RIGHT,pCharacter->CONVERT_ITEMATT( pItem->sSuitOp.emAttack ),bHiddenWeapon) )
		{
			// Sub���⸦ üũ�ؼ� ������� �����Ѵ�.
			SITEM* pItem_S = pCharacter->GET_SLOT_ITEMDATA(emRHand_S);
			if ( pItem_S && CHECHSKILL_ITEM(emSKILL_RIGHT,pCharacter->CONVERT_ITEMATT( pItem_S->sSuitOp.emAttack ),bHiddenWeapon) )
			{
				m_pGaeaClient->GetCharacter()->ReqSlotChange();
			}
		}
	}
}

void CInnerInterface::DoModal(const std::string& strText, int nModalTitle, int nModalType, UIGUID CallerID, BOOL bHide)
{    
    if (m_pModalWindow && !m_bBlockModalUI )
    {
        // UiHideGroup ( MODAL_WINDOW, true );
        if (UiIsVisibleGroup(MODAL_WINDOW))
        {
            // GASSERT ( 0 && "�̹� �����ֽ��ϴ�." );
            UiHideGroup(MODAL_WINDOW, true);
        }

        m_pModalWindow->SetHide(bHide);
        m_pModalWindow->DoModal(strText, nModalTitle, nModalType, CallerID);
        UiShowGroupFocus(MODAL_WINDOW, true);
    }
}

void CInnerInterface::DoModalTipBox ( const std::string& strText, const std::string& strTip, int nModalTitle, int nModalType, UIGUID CallerID, BOOL bHide )
{
    if (m_pModalWindow)
    {
        // UiHideGroup ( MODAL_WINDOW, true );
        if (UiIsVisibleGroup(MODAL_WINDOW))
        {
            // GASSERT ( 0 && "�̹� �����ֽ��ϴ�." );
            UiHideGroup(MODAL_WINDOW, true);
        }

        m_pModalWindow->SetHide(bHide);
        m_pModalWindow->DoModalTipBox(strText, strTip, nModalTitle, nModalType, CallerID);
        UiShowGroupFocus(MODAL_WINDOW, true);
    }
}

void CInnerInterface::DoConftModal ( const std::string& strText, UIGUID CallerID, CString strMyName, CString strHisName )
{
	IConftModalWindow* pConftModalWindow = GetConftModalWindow();
	if ( pConftModalWindow )
	{
        pConftModalWindow->DoModal( strText, CallerID, UI::ToString( strMyName ), UI::ToString( strHisName ) );
		UiShowGroupFocus ( CONFT_MODAL_WINDOW );
	}
}

void CInnerInterface::DoPartyModal( const std::string& strText, UIGUID CallerID, bool bExpedition )
{
	IPartyModalWindow* pPartyModalWindow = GetPartyModalWindow();
	if ( pPartyModalWindow )
	{
		pPartyModalWindow->SetPartyMode( bExpedition );
		pPartyModalWindow->DoModal( strText.c_str(), CallerID );
		UiShowGroupFocus( PARTY_MODAL_WINDOW );
	}
}

void CInnerInterface::DoPartyModal( const std::string& strText, UIGUID CallerID, bool bExpedition, DWORD dwCharDbNum )
{
    IPartyModalWindow* pPartyModalWindow = GetPartyModalWindow();
    if ( pPartyModalWindow )
    {
        pPartyModalWindow->SetPartyMode( bExpedition );
        pPartyModalWindow->DoModal( strText.c_str(), CallerID );
		pPartyModalWindow->SetCharDbNum( dwCharDbNum );
        UiShowGroupFocus( PARTY_MODAL_WINDOW );
    }
}

void CInnerInterface::DoPartyModal ( const std::string& strText, UIGUID CallerID, const SPartyOption& PartyOption, const bool bExpedition)
{
	IPartyModalWindow* pPartyModalWindow = GetPartyModalWindow();
	IPartyWindowRenewal* pPartyWindowRenewal = GetPartyWindowRenewal();
	if ( pPartyModalWindow == NULL || pPartyWindowRenewal == NULL )
		return;

	// bExpedition : �޼��� �ٸ��� ����ؾ���;;

	pPartyModalWindow->DoModal( strText.c_str(), CallerID );
	pPartyModalWindow->SetPartyOption(PartyOption);
	UiShowGroupFocus( PARTY_MODAL_WINDOW );
}

void CInnerInterface::DoChatGroupModal( const std::string& strText, UIGUID CallerID, bool bExpedition, DWORD dwkey )
{
	IChatGroupModalWindow* pChatGroupModalWindow = GetChatGroupModalWindow();
	if ( pChatGroupModalWindow )
	{
		pChatGroupModalWindow->SetPartyMode( bExpedition );
		pChatGroupModalWindow->Setdwkey( dwkey );
		pChatGroupModalWindow->DoModal( strText.c_str(), CallerID );
		UiShowGroupFocus( CHAT_GROUP_MODAL_WINDOW );
	}
}

void CInnerInterface::SetReviveSkill( bool bAct )
{
	if ( RANPARAM::bCheckAutoRebirth )
	{
		m_pGaeaClient->GetCharacter()->ReqResurrect(EM_RESURRECTION_SKILL);
		CloseAllWindow ();
	}
	else
	{
		if ( IsVisible( REBIRTH_WINDOW ) || IsVisible( FORCE_REBIRTH ) == FALSE )
		{
			if ( m_pRebirthWindow )
				m_pRebirthWindow->SetReviveSkill( bAct );
		}
		else if ( IsVisible( FORCE_REBIRTH ) )
		{
			UiHideGroup ( REBIRTH_WINDOW );
			std::string strTemp = sc::string::format( "%s", ID2GAMEINTEXT("ForceRebirthSkill") );
			DoModal( strTemp, UI::MODAL_QUESTION, UI::YESNO, UI::MODAL_FORCE_REBIRTH_SKILL );
		}
	}
}

void CInnerInterface::ReloadPointShopItemList()
{
    if ( m_pPointShopWindow )
        m_pPointShopWindow->ReLoadAll();
}

void CInnerInterface::PointShopSetPoint( __int64 nCurPoint )
{
    if ( m_pPointShopWindow )
        m_pPointShopWindow->SetPointText( nCurPoint );
}

void CInnerInterface::PointShopSetBattlePoint( __int64 nCurPoint )
{
	if ( m_pPointShopWindow )
		m_pPointShopWindow->SetBattlePointText( nCurPoint );
}

__int64 CInnerInterface::PointShopGetPriceInCart ()
{
    if ( m_pPointShopWindow )
        return m_pPointShopWindow->GetPriceInCart();

    return 0;
}

void CInnerInterface::PointShopResetCartItem( SNATIVEID sItemID )
{
    if ( m_pPointShopWindow )
        m_pPointShopWindow->ResetCartItem( sItemID );
}

void CInnerInterface::SetForceClosePointShopWindow( bool _bForceClose )
{
    if ( m_pPointShopWindow )
        m_pPointShopWindow->SetForceCloseWindow( _bForceClose );
}

bool CInnerInterface::IsForceClosePointShopWindow()
{
    if ( m_pPointShopWindow )
        return m_pPointShopWindow->IsForceCloseWindow();

    return false;
}

void CInnerInterface::SetForceResurrect( const float _fTime )
{
    if ( m_pForceRebirth )
    {
        m_pForceRebirth->SetForceRebirth( _fTime );
    }
}

//! CTF Result Window
void CInnerInterface::CTFResultWindowShow()
{
	if ( m_pCTFResultWindow )
	{
        CloseAllWindow();
		m_pCTFResultWindow->Show();
	}
}

void CInnerInterface::CTFResultWindowUpdate()
{
	if ( m_pCTFResultWindow )
	{
		m_pCTFResultWindow->Update();
	}
}

void CInnerInterface::CheckCTFSpurt()
{
    if ( m_pNotifyCTFSpurt )
        m_pNotifyCTFSpurt->CheckCTFSpurt();
}

void CInnerInterface::ctf_display_hide()
{
	// TODO : #rm 695
	if( m_pProgressDisplay )
	{
		MyProgressDisplay *pProgress_ctf = dynamic_cast<MyProgressDisplay *>(m_pProgressDisplay);

		if(pProgress_ctf) {
			pProgress_ctf->ctf_hide_display();
		}
	}

	if ( m_pNotifyCTFSpurt )
		m_pNotifyCTFSpurt->SetVisibleSingle(FALSE);
}

BOOL CInnerInterface::IsInventoryOpen()
{
	if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
	{
		if( UiIsVisibleGroup( INVENTORY_WINDOW_RENEW ) &&
			m_pInventory->GetTapSelected() == CRnInventoryWindow::ETapInven )
		{
			return TRUE;
		}
	}
	else
	{
		if( UiIsVisibleGroup( INVENTORY_WINDOW ) )
			return TRUE;
	}

	return FALSE;
}

BOOL CInnerInterface::IsItemDecompose(void)
{
	return (m_pInventory && m_pInventory->IsItemDecompose()) ? TRUE : FALSE;
}

BOOL CInnerInterface::IsBankOpen()
{
	if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
	{
		if( UiIsVisibleGroup( INVENTORY_WINDOW_RENEW ) &&
			m_pInventory->GetTapSelected() == CRnInventoryWindow::ETapBank )
		{
			return TRUE;
		}
	}
	else
	{
		if( UiIsVisibleGroup( ITEMBANK_WINDOW ) )
			return TRUE;
	}

	return FALSE;
}

BOOL CInnerInterface::IsWishListOpen()
{
	if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
	{
		if( UiIsVisibleGroup( INVENTORY_WINDOW_RENEW ) &&
			m_pInventory->GetTapSelected() == CRnInventoryWindow::ETapWish )
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CInnerInterface::UpdateExChangeList()
{
	if ( m_pDialogueWindowRenewal )
		m_pDialogueWindowRenewal->UpdateExChangeTab();
}

void CInnerInterface::ResetCartListInDialogueWindowMarketPage( int nPageIndex, int nPosX, int nPosY )
{
    if ( m_pDialogueWindowRenewal )
        m_pDialogueWindowRenewal->ResetCartListInDialogueWindowMarketPage( nPageIndex, nPosX, nPosY );
}

void CInnerInterface::DialogueWindowLoadRebuyList()
{
    if ( m_pDialogueWindowRenewal )
        m_pDialogueWindowRenewal->LoadRebuyList();
}

void CInnerInterface::DialogueWindowCheckRebuySupplies( bool bCheck )
{
    if ( m_pDialogueWindowRenewal )
        m_pDialogueWindowRenewal->SetCheckRebuySupplies( bCheck );

}

void CInnerInterface::DialogueWindowSetRebuyCountText( int nCount )
{
    if ( m_pDialogueWindowRenewal )
        m_pDialogueWindowRenewal->SetRebuyTabCountText( nCount );
}

int CInnerInterface::GetDialogueWindowRebuyItemCount()
{
    if ( m_pDialogueWindowRenewal )
        return m_pDialogueWindowRenewal->GetRebuyItemCount();
    
    return 0;
}

void CInnerInterface::GetDialogueWindowMarketItemIndex( int* pnPosX, int* pnPosY )
{
    if ( m_pDialogueWindowRenewal )
		m_pDialogueWindowRenewal->GetItemIndex( pnPosX, pnPosY );

	if ( UiIsVisibleGroup( MARKET_WINDOW ) )
	{
		m_pMarketWindow->GetItemIndex( pnPosX, pnPosY );
	}
}

int CInnerInterface::GetDialogueWindowPageIndex()
{
    if ( m_pDialogueWindowRenewal )
		return m_pDialogueWindowRenewal->GetPageIndex();

	if ( UiIsVisibleGroup( MARKET_WINDOW ) )
	{
		return m_pMarketWindow->GetPageIndex();
	}

    return nOUTOFRANGE;
}

bool CInnerInterface::IsOpenDialogueWindowMarketPage()
{
    if ( UiIsVisibleGroup( DIALOGUE_WINDOW_RENEWAL ) )
	{
		if ( m_pDialogueWindowRenewal )
			return m_pDialogueWindowRenewal->IsOpenMarketPage();
	}

	return UiIsVisibleGroup( MARKET_WINDOW );
}

void CInnerInterface::SetEnableDialogueWindowMarketPage(bool bEnable)
{
    if (m_pDialogueWindowRenewal)
        m_pDialogueWindowRenewal->SetEnableMarketPage(bEnable);
}

void CInnerInterface::ProductWindowSetSeletedTabID( UIGUID ControlID )
{
	//���� ����Ǿ�����
	if( m_pProductWindow )
		m_pProductWindow->SetSeletedTabID( ControlID );
}

void CInnerInterface::ProductWindowSetSeletedItem( SProductRecipe* pProductRecipe )
{
    if( pProductRecipe == NULL )
        return;

	//���� ��Ͽ��� �������� ����������
	if( m_pProductWindow )
		m_pProductWindow->SetItemMix( pProductRecipe );
}

void CInnerInterface::MsgWishListInsert( BYTE nType )
{
	if( !m_pInventory )
		return;

	// �κ��丮�� ��ũ���� ���ø���Ʈ�� �������� �� �������� ����
	if( nType == WishList::ETypeInventory ||
		nType == WishList::ETypeBank )
	{
		return;
	}

	if( !UiIsVisibleGroup( INVENTORY_WINDOW_RENEW ) )
		UiShowGroupFocus( INVENTORY_WINDOW_RENEW );

	m_pInventory->CallTapSelect( CRnInventoryWindow::ETapWish );
}

void CInnerInterface::DeleteUIMeshData()
{
    if( m_pCharacterWindow )
	    m_pCharacterWindow->DeleteUIMeshData();
    if( m_pItemPreviewWindow )
	    m_pItemPreviewWindow->DeleteUIMeshData();
    if( m_pReferCharWindow )
        m_pReferCharWindow->DeleteUIMeshData();
}

void CInnerInterface::OpenPopupMenu( popupmenu::SP_POPUP_MENU PopupMenu )
{
    if ( m_pPopupMenu )
        m_pPopupMenu->OpenPopup( PopupMenu );
}

void CInnerInterface::UpdateWhenReceiveDamage()
{
    // TODO : �ǰ� �ÿ� UI���� �� �� �ϵ�.
}

void CInnerInterface::SetVisibleSkillEffectPartyStateWindow(const BOOL bChecked)
{
	m_pPartyStateWindow->SetVisibleSkillEffect(bChecked);
}

const DWORD CInnerInterface::AddPartyYesNoComponent(const std::string& strDescription, PARTY_YESNO_FUNCTION PartyYesNoFunction)
{
	return m_pPartyYesNoWindow->AddComponent(strDescription, PartyYesNoFunction);
}

void CInnerInterface::OpenPartyTenderNotifyButton( bool bShow )
{
	if( bShow )
	{
		UiShowGroupFocus( PARTY_TENDER_NOTIFY_BUTTON );
	}
	else
	{
		UiHideGroup( PARTY_TENDER_NOTIFY_BUTTON );
	}
}

void CInnerInterface::PartyTenderNotify( bool bIsNew )
{
	m_pPartyTenderNotifyButton->Notify( bIsNew );
}

void CInnerInterface::SetPostWindowClose()
{
    if( m_pPostBoxWindow )
        m_pPostBoxWindow->Close();
}

void CInnerInterface::ShowProgessTimer( UINT iTime,float fPosX, float fPosY, const char* szText, int nTextIndex )
{
	if ( m_pProgessTimer )
	{
		UiShowGroupFocus( PROGESS_TIMER );
		m_pProgessTimer->SetProgessTimer( iTime, fPosX, fPosY, szText, nTextIndex );
	}
}

void CInnerInterface::HideProgessTimer()
{
	UiHideGroup(PROGESS_TIMER);
}

void CInnerInterface::PrintSimpleSystemMessage( const CString& strMessage, const D3DCOLOR dwMessageColor, const DWORD dwFontSize, const float fLifeTime )
{
    m_pSimpleSystemMessageDisplay->PrintSimpleSystemMessage(strMessage, dwMessageColor, dwFontSize, fLifeTime);
}

void CInnerInterface::ClearSimpleSystemMessage()
{
    m_pSimpleSystemMessageDisplay->ClearSimpleSystemMessage();
}

void CInnerInterface::ShowSimpleTimerMessageBox(const NET_MSG_GENERIC* pMsg)
{
    m_pSimpleTimerMessageBox->ShowSimpleTimerMessageBox(pMsg);
}

void CInnerInterface::CloseSimpleTimerMessageBox()
{
    m_pSimpleTimerMessageBox->Close();
}

void CInnerInterface::RefreashCdmProgress( DWORD dwNumberOneClubMarkVER, DWORD dwNumberOneClubDbNum, char* szNumberOneName, int nNumberOneScore, int nMyRank, DWORD dwMyScore, int nHour, int nMin, int nSecond )
{
	m_pRnCDMProgress->RefreashCdmProgress( dwNumberOneClubMarkVER, dwNumberOneClubDbNum, szNumberOneName, nNumberOneScore, nMyRank, dwMyScore, nHour, nMin, nSecond );

}

void CInnerInterface::SetRankSloatData( InstanceSystem::RNCDM_RANK_SLOT vecRnCdmRankSlot )
{
	m_pRnCDMRankWindow->SetRankSloatData( vecRnCdmRankSlot );
}

void CInnerInterface::SetCLubMemberInfo( std::vector<InstanceSystem::CDM_MEMBER_INFO> vecCdmMemberInfo )
{
	int nIndex = 0;
	BOOST_FOREACH(InstanceSystem::CDM_MEMBER_INFO sCdmMemberInfo, vecCdmMemberInfo)
	{
		m_pRnCDMClubInfo->SetMemberImage( nIndex, static_cast<EMCHARINDEX>(sCdmMemberInfo.nEmCharIndex) );
		m_pRnCDMClubInfo->SetMemberName( nIndex, sCdmMemberInfo.strCharName.c_str() );
		nIndex++;
	}
}

InstanceSystem::RNCDM_RANK_SLOT CInnerInterface::GetRankSlotData( int nIndex )
{
	return m_pRnCDMRankWindow->GetRankSlotData( nIndex );
}

void CInnerInterface::RankListUpdate()
{
	m_pRnCDMRankWindow->RankListUpdate();
}

void CInnerInterface::SetTitleClubNameMemberNum( const CString strClubName, int nMembernum )
{
	m_pRnCDMClubInfo->SetTitleClubNameMemberNum( strClubName, nMembernum );
}

void CInnerInterface::SetCDMMapinfo( std::string strMapName, int nTimeLimit, int nPlayerLimite, int nLevelLimit )
{
	m_pRnCompetitionWindow->SetCDMMapinfo( strMapName, nTimeLimit, nPlayerLimite, nLevelLimit );
}

void CInnerInterface::SetCDMMapImageName( std::string strMapImageName )
{
	m_pRnCompetitionWindow->SetCDMMapImageName( strMapImageName );
}

void CInnerInterface::SetCDMBattelState( int nStartTime, int nEndTime )
{
	m_pRnCompetitionWindow->SetCDMBattelState( nStartTime, nEndTime );
}

void CInnerInterface::SetRnCDMClubInfoPos( int nPosX, int nPosY )
{
	m_pRnCDMClubInfo->SetRnCDMClubInfoPos( nPosX, nPosY );
}

void CInnerInterface::OpenCDMTopRanking()
{
	m_pRnCDMTopRanking->OpenCDMTopRanking();
}

void CInnerInterface::SetCDMTopList(std::vector<DWORD> vecClubId,
									LONGLONG lnStartTime,
									LONGLONG lnEndTime,
									std::vector<DWORD> vecRank )
{
	m_pRnCDMTopRanking->SetCDMTopList(vecClubId, lnStartTime, lnEndTime, vecRank );
}

void CInnerInterface::SetClubMasterClubName( std::vector<std::string> vecMasterName, std::vector<std::string> vecClubName )
{
	m_pRnCDMTopRanking->SetClubMasterClubName( vecMasterName, vecClubName );
}

void CInnerInterface::SetFlagGuideArrow( const int _flagNum, const STARGETID& sTargetID, const bool bBlink )
{
    m_pCaptureTheFlagEffecMan->SetFlagGuideArrow(_flagNum, sTargetID, bBlink);
}

void CInnerInterface::ShowRnCompetitionRewardTooltop(const int emTitleType, const InstanceSystem::RnCompetitionResultContentRewardObj* pReward)
{
    m_pRnCompetitionResultWindow->ShowRnCompetitionRewardTooltop(emTitleType, pReward);
}

const bool CInnerInterface::msgProc_RnCompetitionResult(const NET_MSG_GENERIC* _pMessage)
{
    return m_pRnCompetitionResultWindow->msgProc_RnCompetitionResult(_pMessage);
}

void CInnerInterface::SetGuidanceClubBattle_UI_Auth(const bool bVisible, const float fAuthTime)
{
    m_pGuidanceAuthProgressUI->SetAuthTime(bVisible, fAuthTime);
}

void CInnerInterface::SetGuidanceClubBattle_ProgressInfo(const NET_MSG_GENERIC* _pMessage)
{
    m_pGuidanceProgressInfoUI->SetProgressInfo(_pMessage);
}

void CInnerInterface::SetGuidanceClubBattle_RankInfo(const NET_MSG_GENERIC* _pMessage)
{
    m_pGuidanceRankInfoUI->SetRankInfo(_pMessage);
}

void CInnerInterface::SetGuidanceClubBattle_RankMyInfo(const NET_MSG_GENERIC* _pMessage)
{
    m_pGuidanceRankInfoUI->SetRankMyInfo(_pMessage);
}

void CInnerInterface::SetGuidanceClubBattle_AuthInfo(const NET_MSG_GENERIC* _pMessage)
{
    m_pMapWindowInterface->MsgMapGuidanceAuthInfo(_pMessage);
}

void CInnerInterface::SetCTI_UI_Auth(const bool bVisible, const float fAuthTime)
{
	m_pCTIAuthProgressUI->SetAuthTime(bVisible, fAuthTime);
}

void CInnerInterface::SetCTI_ProgressInfo(const NET_MSG_GENERIC* _pMessage)
{
	m_pCTIProgressInfoUI->SetProgressInfo(_pMessage);
}

void CInnerInterface::SetCTI_RankInfo(const NET_MSG_GENERIC* _pMessage)
{
	m_pCTIRankInfoUI->SetRankInfo(_pMessage);
}

void CInnerInterface::SetCTI_RankMyInfo(const NET_MSG_GENERIC* _pMessage)
{
	m_pCTIRankInfoUI->SetRankMyInfo(_pMessage);
}

void CInnerInterface::SetCTI_AuthInfo ( const NET_MSG_GENERIC* pMsg )
{
	m_pMapWindowInterface->MsgMapCTIAuthInfo( pMsg );
}

void CInnerInterface::OpenTournamentDashboard(DWORD dwPageNum)
{
	m_pTournamentDashBoard->SetPage(dwPageNum);
	m_pTournamentDashBoard->OnVisible(true);
	
}

void CInnerInterface::OpenTournamentBettingWindow(DWORD dwKagNum,DWORD dwPageNum)
{
	m_pTournamentToToWindow->SetPage(dwKagNum,dwPageNum);
	m_pTournamentToToWindow->OnVisible(true);
	
}

void CInnerInterface::OpenTournamentMoreInfoWindow(DWORD dwPageNum)
{
	m_pTournamentMoreInfoWindow->SetPage(dwPageNum);
	m_pTournamentMoreInfoWindow->OnVisible(true);
}


void CInnerInterface::OpenTournamentUI_HUD(bool isObserver)
{
	
	if(m_pTournamentUI != NULL)
		if(isObserver == true)
			m_pTournamentUI->OnVisible(isObserver);

	if(m_pTournamentUserInterface != NULL)
		if(isObserver == false)
			m_pTournamentUserInterface->SetCloseAll();

}

void CInnerInterface::OpenTournamentWinLose(bool isWin)
{
	m_pTournamentUI->WinLoseScoreVisible(isWin);
}

void CInnerInterface::ShowDoublePoint(BOOL bVisible)
{
	if ( bVisible )
	{
		UiShowGroupFocus ( DOUBLE_POINT );
		m_pDoublePoint->START ( 1 );
		m_pDoublePoint->KEEP_START ( 1 );
	}
	else
	{
		UiHideGroup ( DOUBLE_POINT );
	}
}

void CInnerInterface::OpenSelectiveformBox( std::vector<ITEM::SBOX_ITEM>& vecItemID )
{
	m_pSelformBoxWindow->Open(vecItemID);
}

void CInnerInterface::ToggleJoinWorldBattleButton(bool bOn)
{
    if ( m_pWorldBattleButton )
    {
        m_pWorldBattleButton->ToggleWorldBattleButton(bOn);
    }
}