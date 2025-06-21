#include "StdAfx.h"

#include "./StaticUIManager.h"
#include "InnerInterface.h"

#include "../SigmaCore/String/StringFormat.h"

#include "../RanLogic/RANPARAM.h"

#include "../EngineLib/Common/CommonWeb.h"
#include "../EngineLib/DxTools/DxViewPort.h"
#include "../EngineLib/GUInterface/BasicVarTextBox.h"
#include "../EngineLib/GUInterface/BasicTextBox.h"
#include "../EngineLib/GUInterface/BasicButton.h"
#include "../EngineLib/GUInterface/GameTextControl.h"
#include "../EngineLib/DxSound/DxSoundLib.h"

#include "../RanLogic/Quest/GLQuest.h"
#include "../RanLogic/GLUseFeatures.h"
#include "../RanLogic/GLogicDataMan.h"
#include "../RanLogic/MiniGame/IMiniGameDefine.h"

#include "../RanLogicClient/ReferChar/ReferChar.h"
#include "../RanLogicClient/Stage/DxGlobalStage.h"
#include "../RanLogicClient/Tool/DxParamSet.h"
#include "../RanLogicClient/GLGaeaClient.h"
#include "../RanLogicClient/Party/GLPartyClient.h"
#include "../RanLogicClient/Land/GLLandManClient.h"
#include "../RanLogicClient/WishList/WishList.h"
#include "../RanLogicClient/Tournament/GLTournamentClient.h"

#include "OldUI/Util/ModalWindowInner.h"
#include "OldUI/Util/UIColorSelectorWindow.h"
//#include "ModalCallerID.h"

#include "OldUI/StudentRecord/StudentRecordNotifyButton.h"
#include "OldUI/StudentRecord/ActivityWindowPage.h"
#include "OldUI/Interface/AdminMessageDisplay.h"
#include "OldUI/Interface/BasicChatRightBody.h"
#include "OldUI/Interface/EscMenuOpen.h"
#include "OldUI/Interface/BasicChat.h"
#include "OldUI/Interface/BasicQuickSkillSlot.h"
#include "OldUI/Interface/BasicGameMenu.h"
#include "OldUI/Interface/BasicEscMenu.h"
#include "OldUI/Interface/BasicInfoView.h"
#include "OldUI/Interface/QBoxButton.h"
#include "OldUI/Interface/MiniMap.h"
#include "OldUI/Interface/QuestAlarm.h"
#include "OldUI/Interface/SimpleMessageMan.h"
#include "OldUI/Interface/SkillTimeDisplay.h"
#include "OldUI/Interface/SkillTrayTab.h"
#include "OldUI/Interface/UILeftTopGroup.h"
#include "OldUI/Interface/WeaponDisplay.h"
#include "OldUI/Interface/CDMSafeTimeDisplay.h"
#include "OldUI/Interface/ChatOptionWindow.h"
#include "OldUI/Interface/LunchBoxTimeDisplay.h"
#include "OldUI/Interface/ChatType.h"
#include "OldUI/Interface/PetHungry.h"
#include "OldUI/Interface/GradeDisplay.h"
#include "OldUI/Option/BasicHWOptionWindow.h"
#include "OldUI/Option/ChatMacroWindow.h"
#include "OldUI/Option/KeySettingWindow.h"
#include "OldUI/Display/BonusTimeGauge.h"
#include "OldUI/Display/BonusTimeDisplay.h"
#include "OldUI/Display/DamageDisplayMan.h"
#include "OldUI/Display/AcademyConftDisplay.h"
#include "OldUI/Display/HeadChatDisplayMan.h"
#include "OldUI/Display/ItemShopIconMan.h"
#include "OldUI/Display/MapMoveDisplay.h"
#include "OldUI/Display/NameDisplayMan.h"
#include "OldUI/Display/QuestionItemDisplay.h"
#include "OldUI/Display/SimpleHP.h"
#include "OldUI/Display/TargetInfoDisplay.h"
#include "OldUI/Display/CursorTargetInfo.h"
#include "OldUI/Display/ThaiCCafeMark.h"
#include "OldUI/Display/SummonPosionDisplay.h"
#include "OldUI/Display/SummonNameDisplayMan.h"
#include "OldUI/Display/GatherGauge.h"
#include "OldUI/Display/CdmRankingDisplay.h"
#include "OldUI/RankingsWindow/RankingDisplay.h"
#include "OldUI/PK Notification/PKRankNotification.h"
#include "OldUI/Display/InfoDisplayLink.h"
#include "OldUI/Display/PartyLinkWindow.h"
#include "OldUI/Message/CountMsg.h"
#include "OldUI/Message/RebirthDialogue.h"
#include "OldUI/Message/SystemMessageWindow.h"
#include "OldUI/Message/WaitServerDialogue.h"
#include "OldUI/Message/WarningMsg.h"
#include "OldUI/Message/MapRequireCheck.h"
#include "OldUI/Club/ClubWindow.h"
#include "OldUI/Club/ClubMake.h"
#include "OldUI/Club/ClubBattleModalWindow.h"
#include "OldUI/Club/SubMasterSet.h"
#include "OldUI/Confront/ConftDisplayMan.h"
#include "OldUI/Confront/ConftConfirm.h"
#include "OldUI/Confront/ConftModalWindow.h"
#include "OldUI/Char/CharacterWindow.h"
#include "oldUI/Char/RnCharacterWindow.h"
#include "OldUI/Char/PtoPWindow.h"
#include "OldUI/NPC/DialogueWindow.h"
#include "OldUI/Friend/FriendWindow.h"
#include "OldUI/Feature/BusWindow.h"
#include "OldUI/Feature/HairColorCard.h"
#include "OldUI/Feature/HairStyleCard.h"
#include "OldUI/Feature/HelpWindow.h"
#include "OldUI/Feature/HelpWindowWeb.h"
#include "OldUI/Feature/VNGainSysInventory.h"
#include "OldUI/Feature/VNGainSysGauge.h"
#include "OldUI/Feature/ItemGarbage.h"
#include "OldUI/Feature/ItemRebuild.h"	// ITEMREBUILD_MARK
#include "OldUI/Feature/ItemRebuildRenewal.h"
#include "OldUI/Feature/LargeMapWindow.h"
#include "OldUI/Feature/RnMapWindow.h"
#include "OldUI/Feature/GenderChangeWindow.h"
#include "OldUI/Feature/AttenDanceBookWindow.h"
#include "OldUI/Feature/FaceStyleCard.h"
#include "OldUI/Feature/TaxiWindow.h"
#include "OldUI/Feature/StatsResetWindow.h"
#include "OldUI/Feature/CostumColorChangeWindow.h"
#include "OldUI/Feature/ItemMixWindow.h"
#include "OldUI/Feature/ItemMixInventoryWindow.h"
#include "OldUI/DPS/DPSWindow.h"
#include "OldUI/Pet/PetskinMixImage.h"
#include "OldUI/Pet/PetRebirthDialogue.h"//Monster7j
#include "OldUI/Pet/PetColorCard.h"
#include "OldUI/Pet/PetStyleCard.h"
#include "OldUI/Pet/PetWindow.h"
#include "OldUI/Pet/PetSkillWindow.h"
#include "OldUI/Item/InventoryWindow.h"
#include "OldUI/Item/ItemBankWindow.h"
#include "OldUI/Item/ItemMove.h"
#include "OldUI/Item/ItemShopWindowWeb.h"
#include "OldUI/Item/MarketWindow.h"
#include "OldUI/Item/ItemPreviewWindow.h"
#include "OldUI/Party/MiniPartyWindow.h"
#include "OldUI/Party/PartyModalWindow.h"
#include "OldUI/PrivateMarket/PrivateMarketWindow.h"
#include "OldUI/PrivateMarket/PrivateMarketSellWindow.h"
#include "OldUI/PrivateMarket/PrivateMarketMake.h"
#include "OldUI/PrivateMarket/PrivateMarketShowMan.h"
#include "OldUI/PrivateMarket/ShopItemSearchWindow.h"
#include "OldUI/PrivateMarket/ItemSearchResultWindow.h"
#include "OldUI/Party/PartyWindow.h"
#include "OldUI/Quest/QuestWindow.h"
#include "OldUI/Quest/QuestHelper.h"
#include "OldUI/Friend/ReceiveNoteWindow.h"
#include "OldUI/Friend/SMSSendWindow.h"
#include "OldUI/Friend/WriteNoteWindow.h"
#include "OldUI/Storage/StorageWindow.h"
#include "OldUI/Storage/StorageChargeCard.h"
#include "OldUI/Skill/SkillWindow.h"
#include "OldUI/Skill/SkillWindowToTray.h"
#include "OldUI/Trade/TradeWindow.h"
#include "OldUI/Trade/TradeInventoryWindow.h"
#include "OldUI/Vehicle/VehicleWindow.h"
#include "OldUI/Vehicle/BikeWindow.h"
#include "OldUI/Vehicle/BikeColorChangeWindow.h"
#include "OldUI/Vehicle/BikeGetOffWindow.h"
#include "OldUI/Vehicle/BikeMessage.h"
#include "OldUI/Vehicle/BikeBoostWindow.h"
#include "OldUI/Vehicle/CarWindow.h"
#include "OldUI/Vehicle/RearcarWindow.h"
#include "OldUI/Post/PostBoxWindow.h"
#include "OldUI/Post/PostNotifyButton.h"
#include "OldUI/SNS/SNSWindow.h"
#include "OldUI/SNS/FacebookWindow.h"
#include "OldUI/SNS/TwitterWindow.h"
#include "OldUI/Quest/WindowRenewal/QuestWindowRenewal.h"
#include "OldUI/PointShop/PointShopWindow.h"
#include "OldUI/Util/PopupMenu.h"
#include "OldUI/Display/ItemInfoTooltip.h"
#include "OldUI/Display/SkillInforTooltip.h"
#include "OldUI/Display/SkillLinkInfoTooltip.h"
#include "OldUI/Message/ForceRebirth.h"
#include "OldUI/Display/NotifyCTFSpurt.h"
#include "OldUI/NPC/DialogueWindowRenewal.h"
#include "OldUI/Product/ProductButton.h"
#include "OldUI/Product/ProductProgress.h"
#include "OldUI/Product/ProductWindow.h"
#include "OldUI/Product/ProductResetWindow.h"
#include "OldUI/ReferChar/ReferCharWindow.h"
#include "OldUI/Skill/RnSkillWindow.h"
#include "OldUI/Item/RnInventoryWindow.h"
#include "OldUI/NotifyWindow/NotifyButton.h"
#include "OldUI/NotifyWindow/NotifyWindow.h"
#include "OldUI/GroupChat/ChatGroupWindow.h"
#include "OldUI/GroupChat/MaxChatWindow.h"
#include "OldUI/ItemRepair/ItemRepairWindow.h"

// Lotto System;
#include "OldUI/LottoSystem/LottoMainUI.h"
#include "OldUI/LottoSystem/LottoBuyLottoUI.h"
#include "OldUI/LottoSystem/LottoBuyListInfo.h"
#include "OldUI/LottoSystem/LottoWinManListInfo.h"
#include "OldUI/LottoSystem/LottoDisplayUI.h"
#include "OldUI/LottoSystem/LottoNotifyButton.h"
#include "OldUI/LottoSystem/GLLottoSystemManUI.h"

// for tutorial UI. by luxes.
#include "../RanLogicClient/Tutorial/GLBaseTutorial.h"
#include "OldUI/Tutorial/MouseTip.h"
#include "OldUI/Tutorial/TutorialDialogue.h"
#include "OldUI/Tutorial/ExplainInterface.h"
#include "OldUI/Tutorial/MouseControlDir.h"
#include "OldUI/Tutorial/BallonText.h"
#include "OldUI/Tutorial/MinimapTarget.h"
#include "OldUI/Tutorial/TutorialSkipButton.h"
#include "OldUI/Tutorial/TutorialViewStep.h"
//

// Tournament
#include "OldUI/Tournament/TournamentObserveWindow.h"
#include "OldUI/Tournament/TournamentMulty.h"
#include "OldUI/Tournament/TournamentEntranceConfirm.h"
#include "OldUI/Tournament/TournamentMiniExpeditionWindow.h"
#include "OldUI/Tournament/TournamentMiniExpeditionInfoGroup.h"
#include "OldUI/Tournament/TournamentExpeditionInfoGroup.h"
#include "OldUI/Tournament/TournamentIndicator.h"

#include "OldUI/Competition/DoublePoint.h"
#include "OldUI/FlyCameraControl/FlyCameraControlUIDefine.h"

#include "OldUI/Display/SummonStateDisplayUI.h"
#include "OldUI/CostumeStat/CotStatWindow.h"

#include "OldUI/FactionSystem/FactionInfoWindow.h"
#include "OldUI/WorldBattleSystem/WorldBattleButton.h"
#include "OldUI/SelectiveformBox/SelectiveformBoxWindow.h"
#include "OldUI/PK Streak/AnimatedPKStreak.h"
#include "KillCard/KillCardManager.h"


#include "../RanLogicClient/Command/dxincommand.h"

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>

//-----------------------------------------------------------------------------
#include "../SigmaCore/DebugInclude.h"
//-----------------------------------------------------------------------------

void CInnerInterface::WAITSERVER_DIALOGUE_OPEN ( const CString& strMessage, const int nAction, const float fTimer )
{
	if ( UiIsVisibleGroup ( WAITSERVER_DISPLAY ) ) return ;

	UiShowGroupFocus ( WAITSERVER_DISPLAY );
	m_pWaitServerDisplay->SetDisplayMessage ( strMessage, nAction, fTimer );
}

void CInnerInterface::WAITSERVER_DIALOGUE_CLOSE ()
{
	UiHideGroup ( WAITSERVER_DISPLAY );
}

void CInnerInterface::SetMiniMapInfo(const GLLevelFileClient& levFile, CString strMapName )
{
	if ( m_pMiniMap && m_pMapWindowInterface )
	{
        m_pMiniMap->SetMapAxisInfo(levFile.GetMapAxisInfo(), UI::ToString( strMapName ) );
		m_pMapWindowInterface->SetMapLevelFile( levFile, strMapName );
	}
}

const char* CInnerInterface::MakeString ( const char* szFormat, ... )
{
	const int MAX_STRLENGTH = 256;

	va_list argList;
	static	char szBuffer[MAX_STRLENGTH];

	va_start(argList, szFormat);
	StringCbVPrintf(szBuffer, MAX_STRLENGTH, szFormat, argList);
	va_end(argList);

	return szBuffer;
}

void CInnerInterface::ReferCharWindowOpen( const std::string& strCharName )
{
    ReferChar::CClient* pReferChar = m_pGaeaClient->GetReferChar();
    if( !pReferChar )
        return;

    if( pReferChar->Request( m_pGaeaClient->GetConnServerGroup(), strCharName ) )
        UiShowGroupFocus( REFERCHAR_WINDOW );
}

void CInnerInterface::ItemRepairWindowOpen( bool bOpen /*= true */ )
{
    if( GLUseFeatures::GetInstance().IsUsingItemDurability() && m_pItemRepairWindow )
    {
        if( bOpen )
            UiShowGroupFocus( ITEM_REPAIR_WINDOW );
        else
            UiHideGroup( ITEM_REPAIR_WINDOW );
    }
    //ITEMREPAIR_WINDOW
}
void CInnerInterface::ItemRepairCardUse( WORD wPosX, WORD wPosY )
{
    if( m_pItemRepairWindow )
        m_pItemRepairWindow->ItemRepairCardUseReq( wPosX, wPosY );
}


std::string	 CInnerInterface::GetSelectTextName()
{
	return m_pReferCharWindow->GetSelectTextName();
}

void CInnerInterface::SetVisibleReferCharacterAdditionalWindow( bool bVisible )
{
	return m_pReferCharWindow->SetVisibleReferCharacterAdditionalWindow( bVisible );
}

//! �ݱ� ������ true�� ����
const bool CInnerInterface::CloseFocusWindow()
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
			cID != QUICK_SKILL_TRAY_TAB_WINDOW &&
			cID != PRIVATE_MARKET_SHOW_MAN && 
			cID != MINIMAP && 
			cID != BIKE_BOOST_WINDOW &&
			cID != INFO_CTF_DISPLAY &&
			cID != CTF_RESULT_WINDOW && 
			cID != MIN_GROUP_CHAT_WINDOW &&
			cID != PROGESS_TIMER &&
			cID != RNCDM_PROGRESS &&
            cID != GUIDANCE_AUTH_PROGRESS_UI &&
            cID != GUIDANCE_PROGRESS_INFO_UI &&
            cID != GUIDANCE_RANK_INFO_UI &&
			cID != CTI_AUTH_PROGRESS_UI &&
			cID != CTI_PROGRESS_INFO_UI &&
			cID != CTI_RANK_INFO_UI &&
			cID != MINIPARTY_WINDOW &&
			cID != COMPETITION_TOURNAMENTENT_INDICATOR
            )
		{
			// Non Close UI�� ���� �ʴ´�;
			bool bNonCloseUI = false;
			for( unsigned int i=0; i<m_vNonCloseUI.size(); ++i )
			{
				if( cID == m_vNonCloseUI.at( i ) )
					bNonCloseUI = true;
			}

			if( bNonCloseUI )
				continue;

			if( cID == INVENTORY_WINDOW ||
				cID == INVENTORY_WINDOW_RENEW ||
				cID == MARKET_WINDOW ||
				cID == STORAGE_WINDOW ||
				cID == CLUB_STORAGE_WINDOW )
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
						DoModal ( ID2GAMEINTEXT("PRIVATE_MARKET_QUIT"), UI::MODAL_QUESTION, UI::YESNO, UI::MODAL_PRIVATE_MARKET_QUIT );
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
				// #ifdef CH_PARAM // �߱� �������̽� ����
				//					UpdatePotionTrayPosition();
				// #endif
				//bAnotherProcess = true;	
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
			else if( cID == ITEM_MIX_WINDOW || cID == ITEM_MIX_INVEN_WINDOW )	// ������ ����
			{
				SetItemMixWindowClose();
			}
			else if( cID == GAMBLE_DIALOGUE || cID == GAMBLE_SELECT_DIALOGUE || 
				cID == GAMBLE_AGAIN_DIALOGUE || cID == GAMBLE_ANIMATION_DIALOGUE || 
				cID == GAMBLE_RESULT_EVEN_DIALOGUE || cID == GAMBLE_RESULT_ODD_DIALOGUE  )
			{
				ToggleWindowMiniGame();
			}
			else if ( cID == QUEST_WINDOW )
			{
				m_pQuestWindowRenewal->SetShowPage();
			}
			else if ( cID == WAITSERVER_DISPLAY || cID == REBIRTH_DIALOGUE ||
				cID == MAP_REQUIRE_CHECK || cID == REBIRTH_WINDOW )
			{
				bAnotherProcess = true;
			}
			else if ( (MODAL_WINDOW + 1) <= cID )
			{
				//					CLOSE_MODAL ( cID, false );
				bAnotherProcess = true;
			}
			else if ( cID == MOUSETIP_WINDOW || cID == TUTORIAL_DIALOGUE_WINDOW
				|| cID == TUTORIAL_DIALOGUE_EXPLAIN_WINDOW || cID == EXPLAIN_INTERFACE
				|| cID == MOUSE_CONTROL_DIR || cID == TUTORIAL_INTERFACE_LEFT
				|| cID == TUTORIAL_INTERFACE_RIGHT || cID == TUTORIAL_INTERFACE_UP
				|| cID == TUTORIAL_INTERFACE_DOWN || cID == TUTORIAL_INTERFACE_ZOOM_IN
				|| cID == TUTORIAL_INTERFACE_ZOOM_OUT || cID == BALLOON_TEXT_WINDOW
				|| cID == MINIMAP_TARGET || cID == BALLOON_SPACE_BAR
				|| cID == TUTORIAL_VIEW_STEP
                 )
			{	// ESC�� ������ ���ƾ� �� Ʃ�丮�� UI üũ.
				bAnotherProcess = true;

				if ( UiIsVisibleGroup ( ESC_MENU ) )
					UiHideGroup ( ESC_MENU );
				else
					UiShowGroupFocus( ESC_MENU );
			}
			else if ( cID == STATSRESET_WINDOW )
			{
				bAnotherProcess = true;
				m_pStatsResetWindow->Close();
			}
			else if ( cID == PRODUCT_RESET_WINDOW )
			{
				bAnotherProcess = true;
				UiHideGroup( PRODUCT_RESET_WINDOW );
				//m_pProductResetWindow->Close();
			}
			else if ( cID == POSTBOX_WINDOW )
			{
				bAnotherProcess = true;
				m_pPostBoxWindow->Close();
			} 
			else if ( cID == STUDENTRECORD_WINDOW )
			{
				if ( m_pStudentRecordWindow->IsOpenModalWindow() )
				{
					bAnotherProcess = true;
					m_pStudentRecordWindow->CloseModalWindow();
				}
			}
			else if ( cID == CHATTYPE_LIST )
			{
				bAnotherProcess = true;
				SetChatTypeListClose();
			}
            else if( 
                cID == COMPETITION_PVEENTRANCECONFIRM ||
                cID == COMPETITION_PVERETRY )
            {
                // PVE �ʴ�, PVE �絵�� â�� ��� ESCŰ ���� ó��
                bAnotherProcess = true;
                // PVE ���̾�α�
            }
			else if ( cID == PARTY_TENDER_NOTIFY_BUTTON )
			{
				// ��Ƽ�����ۺй� �˸���ư ESCŰ ���� ó��
				bAnotherProcess = true;
			}
            else if ( cID == RNCOMPETITION_RESULT_WINDOW )
            {
                // ���� ���� ��� UI�� ��� ESCŰ ���� ó��;
                bAnotherProcess = true;
            }
            else if ( cID == FORCE_REBIRTH )
            {
                // ������Ȱ Ÿ�̸� UI�� ��� ESCŰ ������ �ʵ��� ó��;
                bAnotherProcess = true;
            }
			else if( cID == COSTUME_STATS_WINDOW )
			{
				OpenCostumeStatWindow();
			}			

			if ( !bAnotherProcess )	
			{
				UiHideGroup ( cID );
			}

			bALLHIDE = false;
			break;
		}
	}

	return !bALLHIDE;
}

void CInnerInterface::CloseAllWindow()
{	
	CUIFocusContainer::UICONTROL_FOCUSLIST list = uiman::GetInnerUIMan().GetFocusList()->GetFocusList ();
	CUIFocusContainer::UICONTROL_FOCUSLIST_CITER citer = list.begin ();
	CUIFocusContainer::UICONTROL_FOCUSLIST_CITER citer_end = list.end ();
	for ( ; citer != citer_end; ++citer )
	{
		UIGUID cID = citer->cID;

		if( !UiIsVisibleGroup ( cID ) )
			continue;

		if( cID == INVENTORY_WINDOW ||
			cID == INVENTORY_WINDOW_RENEW ||
			cID == MARKET_WINDOW ||
			cID == STORAGE_WINDOW ||
			cID == CLUB_STORAGE_WINDOW )
		{
			SetMarketWindowClose ();
			SetStorageWindowClose ();
			SetClubStorageWindowClose ();
		}
		else if ( cID == TRADEINVENTORY_WINDOW )
		{
			if ( UiIsVisibleGroup ( TRADE_WINDOW ) ) SetTradeWindowCloseReq ();
			else if ( UiIsVisibleGroup ( PRIVATE_MARKET_WINDOW ) ) SetPrivateMarketClose ();
		}
		else if ( cID == TRADE_WINDOW )
		{							
			SetTradeWindowCloseReq ();
		}
//		else if ( cID == MINIPARTY_WINDOW )
//		{
//            UiShowGroupBottom ( MINIPARTY_OPEN );
////			#ifdef CH_PARAM // �߱� �������̽� ����
////				UpdatePotionTrayPosition();
////			#endif
//		}
		else if ( cID == PRIVATE_MARKET_WINDOW )
		{
			SetPrivateMarketClose ();
		}
		else if( cID == ITEM_REBUILD_WINDOW )	// ITEMREBUILD_MARK
		{
			SetItemRebuildWindowClose();
		}
		else if( cID == ITEM_GARBAGE_WINDOW )	// ������
		{
			CloseItemGarbageWindow();
		}
		else if( cID == ITEM_MIX_WINDOW || cID == ITEM_MIX_INVEN_WINDOW )	// ������ ����
		{
			SetItemMixWindowClose();
		}
        else if ( cID == CHATTYPE_LIST )
        {
            SetChatTypeListClose();
        }
		else if ( cID == COMPETITION_WINDOW )
		{
			CompetitionWindowClose();
		}

		EMSERVICE_PROVIDER sp = GLogicData::GetInstance().GetServiceProvider();

		// ������� �ȵǴ� ��Ʈ���� ������ ��Ʈ���� �����.
		if(
			cID != BASIC_CHAT_BOX &&
			cID != BASIC_INFO_VIEW &&
			cID != QUICK_SKILL_TRAY_TAB_WINDOW &&
			cID != LEFTTOP_CONTROL_GROUP &&
			cID != MIN_GROUP_CHAT_WINDOW &&
			cID != MINIPARTY_WINDOW )
		{
			// Non Close UI�� ���� �ʴ´�;
			bool bNonCloseUI = false;
			for( unsigned int i=0; i<m_vNonCloseUI.size(); ++i )
			{
				if( cID == m_vNonCloseUI.at( i ) )
					bNonCloseUI = true;
			}

			if( bNonCloseUI )
				continue;

//#if defined(VN_PARAM) //vietnamtest%%%
			if(sp == SP_VIETNAM)
			{
				if (cID != VNGAINSYS_GAUGE_WINDOW)
					UiHideGroup ( citer->cID );
			}
			else
			{
//#endif
				UiHideGroup ( citer->cID );
			}
		}
	}
}

void CInnerInterface::SetPtoPWindowOpen(std::tr1::shared_ptr<GLCharClient> pCharClient, DWORD dwGaeaID, GLClubClient* pMyClub)
{
	if (!pCharClient)
        return;

		
	GetPtoPWindow()->SetPtoPMenu ( pCharClient, dwGaeaID, pMyClub );	
//		GetPtoPWindow()->SetUIWindowGlobalPos ( D3DXVECTOR2( float(nPosX), float(nPosY) ) );
	UiShowGroupFocus( PTOPMENU_WINDOW );
}

void CInnerInterface::SetTradeWindowOpen ( CString szPlayerName, DWORD dwTargetID )
{
	if ( UiIsVisibleGroup ( TRADE_WINDOW ) )
		return;

	GetTradeWindow()->SetTradeInfo( UI::ToString( szPlayerName ), dwTargetID );
	CloseAllWindow();	

	const long lResolution = uiman::GetInnerUIMan().GetResolution();
	WORD X_RES = HIWORD ( lResolution );
	WORD Y_RES = LOWORD ( lResolution );

	{
		const UIRECT& rcTradeWindow = GetTradeWindow()->GetUIWindowGlobalPos ();

		D3DXVECTOR2 vPos;
		vPos.x = ((X_RES) / 2.0f) - rcTradeWindow.sizeX;
		vPos.y = (Y_RES - rcTradeWindow.sizeY) / 2.0f;
		GetTradeWindow()->SetUIWindowGlobalPos ( vPos );

		UiShowGroupFocus ( TRADE_WINDOW );
	}

	if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
	{
		m_pInventory->CallTapSelect( CRnInventoryWindow::ETapInven );
		UiShowGroupFocus( INVENTORY_WINDOW_RENEW );
	}
	else
	{
		const UIRECT& rcTradeInventoryWindow = GetTradeInventoryWindow()->GetUIWindowGlobalPos ();

		D3DXVECTOR2 vPos;
		vPos.x = (X_RES) / 2.0f;
		vPos.y = (Y_RES - rcTradeInventoryWindow.sizeY) / 2.0f;
		GetTradeInventoryWindow()->SetUIWindowGlobalPos ( vPos );

		UiShowGroupFocus ( TRADEINVENTORY_WINDOW );
	}
}

void CInnerInterface::OpenBalloonText( UIRECT rcWindow )
{
	if ( UiIsVisibleGroup ( BALLOON_TEXT_WINDOW ) ) return ;

	GetBalloonText()->SetUIWindowLocalPos( rcWindow );
	GetBalloonText()->SetUIWindowGlobalPos( rcWindow );

	UiShowGroupTop ( BALLOON_TEXT_WINDOW );
}

void CInnerInterface::OpenBalloonText()
{
	if ( UiIsVisibleGroup ( BALLOON_TEXT_WINDOW ) ) return ;

	UiShowGroupTop ( BALLOON_TEXT_WINDOW );

}


void CInnerInterface::OpenMouseTip()
{
	if ( UiIsVisibleGroup ( MOUSETIP_WINDOW ) ) return ;

	const long lResolution = uiman::GetInnerUIMan().GetResolution();
	WORD X_RES = HIWORD ( lResolution );
	WORD Y_RES = LOWORD ( lResolution );

	{
		const UIRECT& rcMouseTipWindow = m_pMouseTip->GetGlobalPos ();

		D3DXVECTOR2 vPos;
		vPos.x = X_RES - rcMouseTipWindow.sizeX - 12.0f;
		vPos.y = ( Y_RES - rcMouseTipWindow.sizeY ) / 2.0f;
		m_pMouseTip->SetGlobalPos ( vPos );
		m_pMouseTip->SetLocalPos ( vPos );
	}

	UiShowGroupFocus ( MOUSETIP_WINDOW );

}

void CInnerInterface::OpenTutorialViewStep()
{
	if ( UiIsVisibleGroup( TUTORIAL_VIEW_STEP ) ) return ;

	{
		const UIRECT& rcWindow = m_pTutorialViewStep->GetUIWindowGlobalPos();
		const UIRECT& rcTarget = GetSkillTrayTab()->GetUIWindowGlobalPos();

		D3DXVECTOR2 vPos;
		vPos.x = rcTarget.right + rcWindow.sizeX * 0.01f;
		vPos.y = rcTarget.top + rcTarget.sizeY * 0.5f - rcWindow.sizeY * 0.5f;

		m_pTutorialViewStep->SetUIWindowLocalPos( vPos );
		m_pTutorialViewStep->SetUIWindowGlobalPos( vPos );
	}

	UiShowGroupBottom( TUTORIAL_VIEW_STEP );
}

void CInnerInterface::SetDamage ( D3DXVECTOR3 vPos, int nDamage, DWORD dwDamageFlag, BOOL bAttack )
{
	//	ī�޶� ������ �����ȿ� ������ ��ġ�� �ִ��� �˻�.
	//
	const CLIPVOLUME &CV = DxViewPort::GetInstance().GetClipVolume();
	BOOL bOK = COLLISION::IsCollisionVolume(CV,vPos,vPos);
	if ( !bOK )				return;
	
	//	������ �������� �κп��� �������� ��ġ�ϴ��� �˻�.
	//
	GLLandManClient* pLandClient = m_pGaeaClient->GetActiveMap();
	GASSERT(pLandClient&&"m_pGaeaClient->GetActiveMap()");
	if ( !pLandClient )		return;

	DxLandMan *pLandMan = pLandClient->GetLandMan();
	GASSERT(pLandMan&&"pLandClient->GetLandMan()");
	if ( !pLandMan )		return;

    //ī�޶��� �þ߷� ���� �ø�
	D3DXVECTOR3 vPoint1 = DxViewPort::GetInstance().GetFromPt();
	D3DXVECTOR3 vPoint2 = vPos;
	D3DXVECTOR3 vCollision;
	BOOL bCollision(FALSE);
	LPDXFRAME pDxFrame;
	pLandMan->IsCollision( vPoint1, vPoint2, vCollision, bCollision, pDxFrame, EMCC_CULL_CCW );
	if ( bCollision )		return;

    //������Ʈ �ø� �ɼ��� �����ִ� ���� ���
    if ( pLandClient->IsCullByObjectMap() == true )
    {
        //ĳ������ �þ߷� ���� �ø�
        bCollision = FALSE;
        vPoint1 = m_pGaeaClient->GetCharacter()->GetPosition();
        pLandMan->IsCollision( vPoint1, vPoint2, vCollision, bCollision, pDxFrame, EMCC_CULL_CCW );
        if ( bCollision )		return;
    }

	D3DXVECTOR3 vScreen = DxViewPort::GetInstance().ComputeVec3Project ( &vPos, NULL );
	const UIRECT& rcOriginPos = m_pDamageDisplayMan->GetGlobalPos ();

	//	Note : ������ �¿�� ������� ��������.
	//	20 ������ ������ ����
    int nPosX = (int)( vScreen.x - ( rcOriginPos.sizeX * 0.5f ));
	int nPosY = (int)( vScreen.y - rcOriginPos.sizeY );

	static const int nMAXINTERVAL = 20;
	static const int nBASE = int(nMAXINTERVAL / 2);

	int xInterval = rand()%nMAXINTERVAL - nBASE;
	int yInterval = rand()%nMAXINTERVAL - nBASE;
	nPosX += xInterval;
	nPosY += yInterval;

	long lResolution = uiman::GetInnerUIMan().GetResolution ();
	WORD X_RES = HIWORD(lResolution);
	WORD Y_RES = LOWORD(lResolution);
	if ( ( nPosX < 0.0f ) || ( nPosY < 0.0f )  || 
		( float(X_RES) < nPosX )  || ( float(Y_RES) < nPosY ) )
	{
		return ;
	}

	m_pDamageDisplayMan->SetDamage ( nPosX, nPosY, nDamage, dwDamageFlag, bAttack );
}

void CInnerInterface::ResetTargetInfoMouseOver()
{
	if ( !m_pTargetInfoDisplay )
	{
		return;
	}

	m_pTargetInfoDisplay->SetMouseOverVisibleOff();
}

const bool CInnerInterface::IsVisibleTargetInfoMouseOver()
{
	if ( !m_pTargetInfoDisplay )
	{
		return false;
	}

	return m_pTargetInfoDisplay->IsMouseOverVisible();
}

void CInnerInterface::ResetTargetInfoSelect()
{
	if ( !m_pTargetInfoDisplay )
	{
		return;
	}

	m_pTargetInfoDisplay->SetSelectVisibleOff();
}

void CInnerInterface::SetTargetInfoMouseOver ( STARGETID sTargetID )
{
	SetTargetInfo( sTargetID, UI::EMTARGETINFO_TYPE_MOUSEOVER );
}

void CInnerInterface::SetTargetInfoSelect ( STARGETID sTargetID )
{
	SetTargetInfo( sTargetID, UI::EMTARGETINFO_TYPE_SELECT );
}

void CInnerInterface::SetTargetInfo( STARGETID sTargetID, const UI::EMTARGETINFO_TYPE emTargetInfoType )
{
	if ( sTargetID == STARGETID() )
	{
		return;
	}
		
	if ( !m_pTargetInfoDisplay ) 
	{
		return;
	}

	if ( !m_pTargetInfoDisplay->CheckPriority( emTargetInfoType, sTargetID ) )
	{
		return;
	}

	boost::function<void (ITargetInfoDisplay*, const STARGETID&, float, UI::String, D3DCOLOR)>		 fnSetTargetInfo;
	boost::function<void (ITargetInfoDisplay*, const STARGETID&, float, float, float, ENUM_SCHOOL)>  fnSetAuthenticatorInfo;
	boost::function<void (ITargetInfoDisplay*, const D3DXVECTOR2&)>									 fnSetGlobalPos;
	boost::function<const UIRECT& (ITargetInfoDisplay*)>											 fnGetGlobalPos;

	switch(emTargetInfoType)
	{
	case UI::EMTARGETINFO_TYPE_MOUSEOVER:
		{
			fnSetTargetInfo			= &ITargetInfoDisplay::SetMouseOverTargetInfo;
			fnSetAuthenticatorInfo	= &ITargetInfoDisplay::SetMouseOverAuthenticatorInfo;
			fnSetGlobalPos			= &ITargetInfoDisplay::SetMouseOverGlobalPos;
			fnGetGlobalPos			= &ITargetInfoDisplay::GetMouseOverGlobalPos;
		}
		break;
	
	case UI::EMTARGETINFO_TYPE_SELECT:
		{
			fnSetTargetInfo			= &ITargetInfoDisplay::SetSelectTargetInfo;
			fnSetAuthenticatorInfo	= &ITargetInfoDisplay::SetSelectAuthenticatorInfo;
			fnSetGlobalPos			= &ITargetInfoDisplay::SetSelectGlobalPos;
			fnGetGlobalPos			= &ITargetInfoDisplay::GetSelectGlobalPos;
		}
		break;

	default:
		return;
	};

	CString strName;
	D3DCOLOR dwColor = NS_UITEXTCOLOR::DEFAULT;
	GLDWDATA sHP;

	ClientActor *pCOPY = m_pGaeaClient->GetCopyActor ( sTargetID );
	if ( !pCOPY ) return;

	strName = pCOPY->GetName();
	sHP = pCOPY->GetHp();

	switch ( pCOPY->GetCrow() )
	{
	case CROW_MOB:
	case CROW_NPC:
		{
			GLCrowClient* pCrow = dynamic_cast<GLCrowClient*>(pCOPY);

			if ( !pCrow )
			{
				return;
			}

			dwColor = m_pGaeaClient->GetMobNameColor ( sTargetID.GaeaId );

			switch ( pCrow->m_pCrowData->m_emNPCType )
			{
            case NPC_TYPE_AUTHENTICATOR:
                // �������� ��� HP ǥ�ø� �ٸ��� �Ѵ�.;
				{
					float fPercentSM = float(pCrow->m_arrHP4School[SCHOOL_SM]) / float(sHP.dwMax);
					float fPercentHA = float(pCrow->m_arrHP4School[SCHOOL_HA]) / float(sHP.dwMax);
					float fPercentBH = float(pCrow->m_arrHP4School[SCHOOL_BH]) / float(sHP.dwMax);

					fnSetAuthenticatorInfo( m_pTargetInfoDisplay, sTargetID, fPercentSM, fPercentHA, fPercentBH, ENUM_SCHOOL(pCrow->m_wSchoolAuthed) );
				}
				break;

            case NPC_TYPE_NORMAL:
            default:
                {
                    float fPercent = float(sHP.dwNow) / float(sHP.dwMax);

                    if ( !pCrow->IsValidBody() )
                    {
                        fPercent = 0.0f;
                    }

                    fnSetTargetInfo( m_pTargetInfoDisplay, sTargetID, fPercent, UI::ToString( strName ), dwColor );
                }
                break;
			}
		}
		break;

		case CROW_PC:
		{
            GLLandManClient* pLand = m_pGaeaClient->GetActiveMap();
            if ( pLand == NULL )
                return;

            std::tr1::shared_ptr<GLCharClient> pCHAR = m_pGaeaClient->GetChar ( sTargetID.GaeaId );
            if ( !pCHAR )
				return;

			if ( !pCHAR->IsValidBody() )
				return;
            
            bool bOldVersion(false);
            bool bHpVisible(true);

            Faction::ManagerClient* const pFactionMan = pLand->GetFactionManager();
            if ( pFactionMan )
            {
                const Faction::EMRELATION_FLAG_TYPE _emHpVIsible = pFactionMan->getHPVisible();
                if ( _emHpVIsible == Faction::EMRELATION_FLAG_ERROR )
                {
                    bOldVersion = true;
                }
                else
                {
                    const Faction::EMRELATION_TYPE _emRelation = pFactionMan->getRelationClient(m_pGaeaClient->GetCharacter(), pCHAR.get());

                    switch ( _emRelation )
                    {
                    case Faction::EMRELATION_ENEMY:
                        bHpVisible = (_emHpVIsible & Faction::EM_RELATION_FLAG_ENEMY);
                        break;
                    case Faction::EMRELATION_NEUTRAL_ENEMY:
					case Faction::EMRELATION_NEUTRAL_ALLY:
                        bHpVisible = (_emHpVIsible & Faction::EM_RELATION_FLAG_NEUTRAL);
                        break;
                    case Faction::EMRELATION_ALLY:
                        bHpVisible = (_emHpVIsible & Faction::EM_RELATION_FLAG_ALLY);
                        break;
                    default:
                        bOldVersion = true;
                        break;
                    }
                }
            }
            else
            {
                bOldVersion = true;
            }
            
            if ( bOldVersion )
            {
                // ������ HPǥ�� ��� : ���,Ư�� PVP �������̿�� ����ó���� HP ǥ�ñ���� ���ѽ�Ŵ;
                bool bPKServer		= m_pGaeaClient->IsPKServer();
                bool bBRIGHTEVENT   = m_pGaeaClient->IsBRIGHTEVENT();
                bool bSCHOOL_FREEPK = m_pGaeaClient->IsSchoolFreePk();
                bool bCTFPlayer	    = m_pGaeaClient->IsCTFPlayer();
                bool bPKMap         = pLand->IsPKMap();
                bool bGuidBattleMap = ( pLand->m_bClubBattle || pLand->m_bClubDeathMatch );

                bHpVisible = true;
                //	��� �Ǵ� PK�� �ƴϸ�, �Ǹ� ǥ������ �ʴ´�.
                if ( !m_pGaeaClient->GetCharacter()->ISCONFRONT_TAR ( sTargetID ) &&
                    !m_pGaeaClient->GetCharacter()->IS_PLAYHOSTILE ( pCOPY->GetCharID() ) )
                {
                    if ( !(bBRIGHTEVENT || bSCHOOL_FREEPK || bGuidBattleMap || bPKServer || bPKMap || bCTFPlayer) )
                    {
                        bHpVisible = false;
                    }
                }
            }

            if ( bHpVisible == false )
            {
                //	��ǥ�ø� ���� ( �ƽø� )
                sHP.TO_FULL ();
            }

			//	�̸� �� (PK_COLOR)
			//dwColor = pCHAR->GET_PK_OR_GM_COLOR();
			if( pCHAR->m_dwUserLvl < USER_USER_GM )
				dwColor = pCHAR->GET_PK_COLOR();
			else
				dwColor = NS_UITEXTCOLOR::GM_CHAR_NAME;

			if( pCHAR->GetCharData().m_Base.m_bGM )
			{
				dwColor = NS_UITEXTCOLOR::GM_CHAR_NAME;
			}

			float fPercent = float(sHP.dwNow) / float(sHP.dwMax);

			if ( !pCHAR->IsValidBody() )
			{
				fPercent = 0.0f;
			}

			fnSetTargetInfo( m_pTargetInfoDisplay, sTargetID, fPercent, UI::ToString( strName ), dwColor );
		}
		break;

		case CROW_SUMMON:
		{
            std::tr1::shared_ptr<GLSummonClient> spSummon = m_pGaeaClient->GetSummon(sTargetID.GaeaId);
			if (!spSummon)
                return;

            std::tr1::shared_ptr<GLCharClient> pCHAR = m_pGaeaClient->GetChar(spSummon->m_dwOwnerGaeaID);
			if (!pCHAR)
                return;

			//dwColor = pCHAR->GET_PK_OR_GM_COLOR();
			if( pCHAR->m_dwUserLvl < USER_USER_GM )
				dwColor = pCHAR->GET_PK_COLOR();
			else
				dwColor = NS_UITEXTCOLOR::GM_CHAR_NAME;

			if( pCHAR->GetCharData().m_Base.m_bGM )
			{
				dwColor = NS_UITEXTCOLOR::GM_CHAR_NAME;
			}

			float fPercent = float(sHP.dwNow) / float(sHP.dwMax);

			if (!spSummon->IsValidBody())
				fPercent = 0.0f;

			fnSetTargetInfo( m_pTargetInfoDisplay, sTargetID, fPercent, UI::ToString( strName ), dwColor );
		}
		break;
	};


	//	NOTE
	//		������ ����
	{
        UiHideGroup( TARGETINFO_DISPLAY );

		// Note : �̸�ǥ�� ��������
		D3DXVECTOR3 vPos = pCOPY->GetPosBodyHeight();
		D3DXVECTOR3 vScreen = DxViewPort::GetInstance().ComputeVec3Project( &vPos, NULL );

        bool bNameVisible = true;

        long lResolution = CUIMan::GetResolution();
        WORD X_RES = HIWORD(lResolution) - 30;
        WORD Y_RES = LOWORD(lResolution) - 30;
        if ( (vScreen.x<40) || (vScreen.y<40) || (X_RES<vScreen.x) || (Y_RES<vScreen.y) )
            bNameVisible = false;

        D3DXVECTOR3 LookAtPt = m_pGaeaClient->GetCharacter()->GetPosition() - DxViewPort::GetInstance().GetFromPt();
        LookAtPt.y = 0.0f;
        D3DXVECTOR3 TargetPos = pCOPY->GetPosBodyHeight() - DxViewPort::GetInstance().GetFromPt();
        TargetPos.y = 0.0f;
        float LookAtLength = D3DXVec3Length( &LookAtPt );
        float PosLength = D3DXVec3Length( &TargetPos );
        float CosTheta = D3DXVec3Dot( &LookAtPt, &TargetPos ) / ( LookAtLength * PosLength );
        if ( CosTheta < 0 )
            bNameVisible = false;

        if ( bNameVisible )
        {
            D3DXVECTOR2 vNamePos;
		    const UIRECT& rcOriginPos = fnGetGlobalPos( m_pTargetInfoDisplay );
		    vNamePos.x = floor( vScreen.x - ( rcOriginPos.sizeX * 0.5f ) + 0.5f );
		    vNamePos.y = floor( vScreen.y - rcOriginPos.sizeY + 0.5f );
		    if ( m_pTargetInfoDisplay )
		    {
			    if ( abs( rcOriginPos.left - vNamePos.x ) > 1 || abs( rcOriginPos.top - vNamePos.y ) > 1 )
			        fnSetGlobalPos( m_pTargetInfoDisplay, vNamePos );
		    }
		    UiShowGroupBottom( TARGETINFO_DISPLAY );
        }

		//	�ߺ� �̸� �����
		if ( m_pNameDisplayMan )	
			m_pNameDisplayMan->VisibleTargetInfoCtrl ( sTargetID.emCrow, sTargetID.GaeaId, FALSE );
	}
}


void CInnerInterface::SetMarketWindowOpen ( const SNATIVEID sNPCID, const DWORD dwNPCGlobalID )
{
	if ( m_pMarketWindow )
    {
        CloseAllWindowFromNPC();

		if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
			SetDefaultPosInterface( INVENTORY_WINDOW_RENEW );
		else
			SetDefaultPosInterface( INVENTORY_WINDOW );

		SetDefaultPosInterface( MARKET_WINDOW );

		m_pMarketWindow->InitMarket ( sNPCID, dwNPCGlobalID );

		UiShowGroupFocus ( MARKET_WINDOW );

		if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
		{
			m_pInventory->CallTapSelect( CRnInventoryWindow::ETapInven );
			UiShowGroupFocus( INVENTORY_WINDOW_RENEW );
		}
		else
		{
			UiShowGroupFocus ( INVENTORY_WINDOW );
		}
	}
}

void CInnerInterface::SetMarketWindowClose ()
{
    UiHideGroup ( MARKET_WINDOW );

	if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
		UiHideGroup( INVENTORY_WINDOW_RENEW );
	else
		UiHideGroup ( INVENTORY_WINDOW );
}

void CInnerInterface::SetStorageWindowNpcOpen ( const SNATIVEID sNPCID, const DWORD dwNPCGlobalID )
{
    if ( UiIsVisibleGroup(STORAGE_WINDOW) )
    {
        return;
    }

    if ( m_pStorageWindow )
    {
        CloseAllWindowFromNPC();
        
        //2013-03-29 ��ȹ�ǵ��� ���� PostWindow�� ���� �� �� ����.
        SetPostWindowClose();

		if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
			SetDefaultPosInterface( INVENTORY_WINDOW_RENEW );
		else
			SetDefaultPosInterface( INVENTORY_WINDOW );

        SetDefaultPosInterface( STORAGE_WINDOW );

        m_pStorageWindow->InitStorage ( sNPCID, dwNPCGlobalID );

		UiShowGroupFocus ( STORAGE_WINDOW );

		if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
		{
			m_pInventory->CallTapSelect( CRnInventoryWindow::ETapInven );
			UiShowGroupFocus( INVENTORY_WINDOW_RENEW );
		}
		else
		{
			UiShowGroupFocus ( INVENTORY_WINDOW );
		}
    }
}

void CInnerInterface::SetStorageWindowCardOpen ()
{
    SetStorageWindowNpcOpen ( SNATIVEID(false), EMTARGET_NULL );
}

void CInnerInterface::SetStorageWindowClose ()
{
    UiHideGroup ( STORAGE_WINDOW );

	if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
		UiHideGroup( INVENTORY_WINDOW_RENEW );
	else
		UiHideGroup ( INVENTORY_WINDOW );

    m_pGaeaClient->GetCharacter()->ReqStorageCloseCard(); 
}

void CInnerInterface::SetClubStorageWindowOpen ( const SNATIVEID sNPCID, const DWORD dwNPCGlobalID )
{
    if ( m_pClubStorageWindow )
    {
        CloseAllWindowFromNPC();

		if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
			SetDefaultPosInterface( INVENTORY_WINDOW_RENEW );
		else
			SetDefaultPosInterface( INVENTORY_WINDOW );

		SetDefaultPosInterface( CLUB_STORAGE_WINDOW );

        m_pClubStorageWindow->InitStorage ( sNPCID, dwNPCGlobalID );

		UiShowGroupFocus ( CLUB_STORAGE_WINDOW );

		if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
		{
			m_pInventory->CallTapSelect( CRnInventoryWindow::ETapInven );
			UiShowGroupFocus( INVENTORY_WINDOW_RENEW );
		}
		else
		{
			UiShowGroupFocus ( INVENTORY_WINDOW );
		}
    }
}

void CInnerInterface::SetClubStorageWindowClose ()
{
    UiHideGroup ( CLUB_STORAGE_WINDOW );

	if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
		UiHideGroup( INVENTORY_WINDOW_RENEW );
	else
		UiHideGroup ( INVENTORY_WINDOW );
}

void CInnerInterface::SetBusWindowOpen(
    const SNATIVEID sNPCID,
    const DWORD dwNPCGlobalID,
    std::tr1::shared_ptr<SNpcTalk> spNpcTalk)
{
    if (!spNpcTalk)
        return;

	if (m_pBusWindow)
    {
        CloseAllWindowFromNPC();
		if (m_pBusWindow->SetBusData(sNPCID, dwNPCGlobalID, spNpcTalk, m_bToolMode))
		    UiShowGroupFocus(BUS_WINDOW, true);
	}
}


void CInnerInterface::SetBusWindowClose()
{
    UiHideGroup ( BUS_WINDOW );
}

// TODO : NPC ��ȭ �ŷ� â ����.
void CInnerInterface::SetDialogueWindowOpen( const SNATIVEID sNPCID, const DWORD dwNPCGlobalID, GLCHARLOGIC* pCHAR )
{
    if ( m_pDialogueWindowRenewal )
    {
        CloseAllWindowFromNPC();

        //	TO DO :
        //		������ �߰� �Ǵ���, �� �Լ��� ��ġ�� �ȴ�.
        //		����, ���� �뼱�� ǥ���ؾ��ϴ� ������,		
        //		��ȭ�� ǥ���ؾ��ϴ� �������� ���⿡�� �����ؾ��Ѵ�.

        //	NOTE :
        //		sCrowID�� '����'����, '�Ϲ� NPC'���� Ȯ���� �� �ִ�.

        if ( !m_pDialogueWindowRenewal->SetDialogueData( sNPCID, dwNPCGlobalID, pCHAR ) )
        {
            //GASSERT( 0 && "NPC ��ȭ â���� ��ȭ ���� �б� ����." );
            return;
        }

        UiShowGroupFocus( DIALOGUE_WINDOW_RENEWAL );
    }
}

// TODO : NPC ��ȭ �ŷ� â ����.
//void CInnerInterface::SetDialogueWindowOpen ( const SNATIVEID sNPCID, const DWORD dwNPCGlobalID, GLCHARLOGIC* pCHAR )
//{
//    if ( m_pDialogueWindow )
//    {
//        CloseAllWindowFromNPC();
//
//		//	TO DO :
//		//		������ �߰� �Ǵ���, �� �Լ��� ��ġ�� �ȴ�.
//		//		����, ���� �뼱�� ǥ���ؾ��ϴ� ������,		
//		//		��ȭ�� ǥ���ؾ��ϴ� �������� ���⿡�� �����ؾ��Ѵ�.
//
//		//	NOTE :
//		//		sCrowID�� '����'����, '�Ϲ� NPC'���� Ȯ���� �� �ִ�.
//
//		if ( !m_pDialogueWindow->SetDialogueData ( sNPCID, dwNPCGlobalID, pCHAR ) )
//		{
////			GASSERT ( 0 && "����! npc���� �б� ����!" );
//			return;
//		}
//
//		UiShowGroupFocus ( DIALOGUE_WINDOW );
//	}
//}

void CInnerInterface::SetDialogueWindowClose()
{
    // TODO : NPC ��ȭ �ŷ� â ����.
    UiHideGroup( DIALOGUE_WINDOW_RENEWAL );
    ItemRepairWindowOpen( false );

    //UiHideGroup ( DIALOGUE_WINDOW );
}

void CInnerInterface::SetItemRebuildWindowNpcOpen( const SNATIVEID sNPCID, const DWORD dwNPCGlobalID )
{
	const bool bUsingStampCard(GLUseFeatures::GetInstance().IsUsingStampCard());
	if ( bUsingStampCard == true )
	{
		if ( UiIsVisibleGroup(ITEM_REBUILD_RENEWAL_WINDOW) )
			return;

		if( !m_pItemRebuildWindow )
			return;
	}
	else
	{
		if ( UiIsVisibleGroup(ITEM_REBUILD_WINDOW) )
			return;

		if( !m_pItemRebuildWindow )
			return;
	}    

    CloseAllWindow();

	if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
		UiShowGroupFocus( INVENTORY_WINDOW_RENEW );
	else
		UiShowGroupFocus( INVENTORY_WINDOW );

	UIRECT rcInventoryWindow;
	if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
		rcInventoryWindow = m_pInventory->GetUIWindowGlobalPos();
	else
		rcInventoryWindow = m_pInventoryWindow->GetUIWindowGlobalPos();

    // ItemRebuild Window
	const UIRECT& rcRebuildWindow = bUsingStampCard == true ? m_pItemRebuildRenewal->GetGlobalPos() : m_pItemRebuildWindow->GetGlobalPos();

    D3DXVECTOR2 vPos;
    vPos.x = rcInventoryWindow.left - rcRebuildWindow.sizeX;
    vPos.y = rcInventoryWindow.top;

	if ( bUsingStampCard == true )
	{
		m_pItemRebuildRenewal->SetGlobalPos( vPos );
		m_pItemRebuildRenewal->SetNpc( sNPCID, dwNPCGlobalID );
		UiShowGroupFocus( ITEM_REBUILD_RENEWAL_WINDOW );

		m_pGaeaClient->GetCharacter()->ReqRebuildOpen( sNPCID, dwNPCGlobalID );
	}
	else
	{
		m_pItemRebuildWindow->SetGlobalPos( vPos );
		m_pItemRebuildWindow->SetNpc( sNPCID, dwNPCGlobalID );
		UiShowGroupFocus( ITEM_REBUILD_WINDOW );

		m_pGaeaClient->GetCharacter()->ReqRebuildOpen( sNPCID, dwNPCGlobalID );
	}
}

void CInnerInterface::SetItemRebuildWindowCardOpen()
{
	const bool bUsingStampCard(GLUseFeatures::GetInstance().IsUsingStampCard());
	if ( bUsingStampCard == true )
	{
		if ( UiIsVisibleGroup(ITEM_REBUILD_RENEWAL_WINDOW) )
			return;

		if( !m_pItemRebuildWindow )
			return;
	}
	else
	{
		if ( UiIsVisibleGroup(ITEM_REBUILD_WINDOW) )
			return;

		if( !m_pItemRebuildWindow )
			return;
	}    

    SetItemRebuildWindowNpcOpen ( SNATIVEID(false), EMTARGET_NULL );
}

void CInnerInterface::SetItemRebuildWindowClose()
{
	UiHideGroup( ITEM_REBUILD_RENEWAL_WINDOW );
	UiHideGroup( ITEM_REBUILD_WINDOW );

	m_pGaeaClient->GetCharacter()->ReqRebuildClose();

	const bool bUsingStampCard(GLUseFeatures::GetInstance().IsUsingStampCard());
	if ( bUsingStampCard == true )
		m_pItemRebuildRenewal->ResetPreInventoryItem();
	else
		m_pItemRebuildWindow->ResetPreInventoryItem();
}

void CInnerInterface::SetItemRebuildResetCheckBox()
{
	const bool bUsingStampCard(GLUseFeatures::GetInstance().IsUsingStampCard());
	if ( bUsingStampCard == true )
		m_pItemRebuildRenewal->SetItemRebuildResetCheckBox();
}

void CInnerInterface::SetItemPreviousInformation ( const SITEMCUSTOM& sItemCustom )
{
	m_pItemRebuildRenewal->SetItemPreviousInformation( sItemCustom );
}

void CInnerInterface::SetItemRebuildSetCheckBox(const SITEMCUSTOM& sItemCustom, const ITEM_RANDOMOP_UTILL::FixOption& fixOption, bool bPrevious)
{
	const bool bUsingStampCard(GLUseFeatures::GetInstance().IsUsingStampCard());
	if ( bUsingStampCard == true )
	{
		GLCharacter* const pCharacter(m_pGaeaClient->GetCharacter());
		const unsigned int sizeFixed(fixOption.getNSize());
		const unsigned int nNumStampItem(pCharacter->GetCountItemInInven(ITEM_STAMP_CARD, ITEM_VAlUE_SUIT_REMODELNUM, sizeFixed));
		if ( nNumStampItem != 0 )
		{
			WORD wStampPosX = USHRT_MAX;
			WORD wStampPosY = USHRT_MAX;
			m_pGaeaClient->GetCharacter()->GetItemPosInInven(ITEM_STAMP_CARD, ITEM_VAlUE_SUIT_REMODELNUM, sizeFixed, wStampPosX, wStampPosY);
			if ( wStampPosX != USHRT_MAX && wStampPosY != USHRT_MAX )
			{
				pCharacter->ReqInvenTo( wStampPosX, wStampPosY);
				pCharacter->ReqRebuildMoveStamp();
				pCharacter->m_sStampItem.SET( wStampPosX, wStampPosY );
			}
		}
		m_pItemRebuildRenewal->SetItemRebuildSetCheckBox(sItemCustom, fixOption, bPrevious);
	}
}

void CInnerInterface::CheckItemRebuildWindowClose(const bool bUseStamp, const ITEM_RANDOMOP_UTILL::FixOption& fixOption)
{
    const unsigned int nNumRemodelItem(m_pGaeaClient->GetCharacter()->GetCountItemInInven( ITEM_REMODEL ));
	// unsigned int nNumStampItem = m_pGaeaClient->GetCharacter()->GetCountItemInInven(ITEM_STAMP_CARD);
	//  [3/11/2015 gbgim];
	// - ����ī�� Ÿ���� �ϳ��̳�, ���� �ɼ� ������ ���� �������� �ٸ��� ����Ѵ�;
	// - �ΰ� �����ϴ� ����ī�� ����� �ΰ�����ϴ� ����ī�尡 ������ ����â�� �������ϳ�;
	//   ���� ���� ������� ���� ī�����ϸ� �����ɼ�1~n���� �� ã�⶧���� �������ʴ´�;
	const unsigned int nNumStampItem(m_pGaeaClient->GetCharacter()->GetCountItemInInven(ITEM_STAMP_CARD, ITEM_VAlUE_SUIT_REMODELNUM, fixOption.getNSize()));

    if ( (nNumRemodelItem == 0) || (bUseStamp == true && nNumStampItem == 0) )
        SetItemRebuildWindowClose();
    else
    {
        // TODO : �ٽ� ���� �� �� �ִ� ���·� ����.
        WORD wRemodelPosX = USHRT_MAX;
        WORD wRemodelPosY = USHRT_MAX;

        m_pGaeaClient->GetCharacter()->GetItemPosInInven( ITEM_REMODEL, wRemodelPosX, wRemodelPosY );

        if ( wRemodelPosX != USHRT_MAX && wRemodelPosY != USHRT_MAX )
        {
            WORD wPreInvenItemPosX = USHRT_MAX;
            WORD wPreInvenItemPosY = USHRT_MAX;

            m_pItemRebuildWindow->GetPreInventoryItemPos( wPreInvenItemPosX, wPreInvenItemPosY );
            m_pGaeaClient->GetCharacter()->ReqRemodelOpenCard( wRemodelPosX, wRemodelPosY );
            m_pGaeaClient->GetCharacter()->ReqInvenTo( wPreInvenItemPosX, wPreInvenItemPosY );
            m_pGaeaClient->GetCharacter()->ReqRebuildMoveItem();
        }
        else
            SetItemRebuildWindowClose();
    }
}

void CInnerInterface::SetRebuildWindowPreInvenItem( WORD wPosX, WORD wPosY )
{
	const bool bUsingStampCard(GLUseFeatures::GetInstance().IsUsingStampCard());
	if ( bUsingStampCard == true )
	{
		if ( m_pItemRebuildRenewal )
			m_pItemRebuildRenewal->SetPreInventoryItem( wPosX, wPosY );
	}
	else
	{
		if ( m_pItemRebuildWindow )
			m_pItemRebuildWindow->SetPreInventoryItem( wPosX, wPosY );
	}
}

void CInnerInterface::SetRebuildWindowPreStampItem( WORD wPosX, WORD wPosY )
{
	const bool bUsingStampCard(GLUseFeatures::GetInstance().IsUsingStampCard());
	if ( bUsingStampCard == true )
	{
		if ( m_pItemRebuildRenewal )
			m_pItemRebuildRenewal->SetPreStampItem( wPosX, wPosY );
	}	
}

void CInnerInterface::SetItemMixWindowOpen( const SNATIVEID sNPCID, const DWORD dwNPCGlobalID )
{
    if( !m_pItemMixWindow || !m_pItemMixInvenWindow )
        return;

    CloseAllWindow();

    const long lResolution = uiman::GetInnerUIMan().GetResolution();
    WORD X_RES = HIWORD( lResolution );
    WORD Y_RES = LOWORD( lResolution );
    D3DXVECTOR2 vPos;

    const UIRECT& rcInvenWindow = m_pItemMixInvenWindow->GetGlobalPos();
    vPos.x = ( X_RES ) / 2.0f;
    vPos.y = ( Y_RES - rcInvenWindow.sizeY ) / 3.0f;
    m_pItemMixInvenWindow->SetGlobalPos( vPos );
    UiShowGroupFocus( ITEM_MIX_INVEN_WINDOW );

    // ItemRebuild Window
    const UIRECT& rcRebuildWindow = m_pItemMixWindow->GetGlobalPos();
    vPos.x = ( X_RES / 2.0f ) - rcRebuildWindow.sizeX;
    m_pItemMixWindow->SetGlobalPos( vPos );
    m_pItemMixWindow->SetNpc( sNPCID, dwNPCGlobalID );
    UiShowGroupFocus( ITEM_MIX_WINDOW );
}

void CInnerInterface::SetItemMixWindowClose()
{
    UiHideGroup( ITEM_MIX_INVEN_WINDOW );
    UiHideGroup( ITEM_MIX_WINDOW );

    m_pGaeaClient->GetCharacter()->ResetItemMix();
}

SNATIVEID CInnerInterface::GetDialogueWindowNPCID()
{
    return m_pDialogueWindowRenewal->GetNPCID();
}

DWORD CInnerInterface::GetDialogueWindowNPCGlobID()
{
    return m_pDialogueWindowRenewal->GetNPCGlobalID();
}

SNATIVEID CInnerInterface::GetBusWindowNPCID()
{
    return m_pBusWindow->GetNPCID();
}

DWORD CInnerInterface::GetBusWindowNPCGlobID()
{
    return m_pBusWindow->GetNPCGlobalID();
}

SNATIVEID CInnerInterface::GetStorageWindowNPCID()
{
    return m_pStorageWindow->GetNPCID();
}

DWORD CInnerInterface::GetStorageWindowNPCGlobID()
{
    return m_pStorageWindow->GetNPCGlobalID();
}

SNATIVEID CInnerInterface::GetClubStorageWindowNPCID()
{
    return m_pClubStorageWindow->GetNPCID();
}

DWORD CInnerInterface::GetClubStorageWindowNPCGlobID()
{
    return m_pClubStorageWindow->GetNPCGlobalID();
}

SNATIVEID CInnerInterface::GetMarketWindowNPCID()
{
    return m_pDialogueWindowRenewal->GetNPCID();
}

DWORD CInnerInterface::GetMarketWindowNPCGlobID()
{
    return m_pDialogueWindowRenewal->GetNPCGlobalID();
}

SNATIVEID CInnerInterface::GetItemRebuildWindowNPCID()
{
    return m_pItemRebuildWindow->GetNPCID();
}

DWORD CInnerInterface::GetItemRebuildWindowNPCGlobID()
{
    return m_pItemRebuildWindow->GetNPCGlobalID();
}

SNATIVEID CInnerInterface::GetItemMixWindowNPCID()
{
    return m_pItemMixWindow->GetNPCID();
}

DWORD CInnerInterface::GetItemMixWindowNPCGlobID()
{
    return m_pItemMixWindow->GetNPCGlobalID();
}

void CInnerInterface::SetChatTypeListOpen()
{
    if ( UiIsVisibleGroup( CHATTYPE_LIST ) )
    {
        return;
    }

    if ( !m_pBasicChat || !m_pChatTypeList )
    {
        return;
    }

    const UIRECT& rcChangStateButton = m_pBasicChat->GetUIChangStateButtonGlobalPos();
    const UIRECT& rcChatTypeList     = m_pChatTypeList->GetUIGlobalPos();
    
    m_pChatTypeList->SetUIGlobalPos( UIRECT( rcChangStateButton.left, rcChangStateButton.top - rcChatTypeList.sizeY, rcChatTypeList.sizeX, rcChatTypeList.sizeY ) );
    UiShowGroupFocus( CHATTYPE_LIST );
}

void CInnerInterface::SetChatTypeListClose()
{
    UiHideGroup( CHATTYPE_LIST );
}

// ----------------------------------------------------------------------------

void CInnerInterface::PrintMsgText(D3DCOLOR dwColor, const std::string& strText)
{	
    if ( GetBasicChatWindow() )
        GetBasicChatWindow()->AddStringToSystemMessage(strText.c_str(), dwColor);
}

void CInnerInterface::PrintConsoleText(const std::string& strText)
{
    if ( GetBasicChatWindow() )
    {
        CString strTemp;
        strTemp.Format("[%s]%s", ID2GAMEWORD("CHAT_TYPE",8), strText.c_str());
        GetBasicChatWindow()->AddStringToChatEx( UI::ToString( strTemp ), CHAT_CONST::CHAT_SYSTEM );
    }
}

void CInnerInterface::PrintMsgTextDlg(D3DCOLOR dwColor, const std::string& strText)
{	
    if ( GetBasicChatWindow() )
    {
        GetBasicChatWindow()->AddStringToSystemMessage(strText.c_str(), dwColor);
        m_pSystemMessageWindow->SetSystemMessage(strText.c_str(), dwColor);
    }
}

/*
void CInnerInterface::PrintConsoleTextDlg(const CString& strText)
{
	if (GetBasicChatWindow())
	{
		CString strTemp;
		strTemp.Format ( "[%s]%s", ID2GAMEWORD("CHAT_TYPE",8), strText );
		GetBasicChatWindow()->AddStringToChatEx ( strTemp, CHAT_CONST::CHAT_SYSTEM );
		m_pSystemMessageWindow->SetSystemMessage ( strText, NS_UITEXTCOLOR::HOTPINK );
	}
}
*/

void CInnerInterface::PrintConsoleTextDlg(const std::string& strText)
{
    if (GetBasicChatWindow())
    {
        CString strTemp;
        strTemp.Format("[%s]%s", ID2GAMEWORD("CHAT_TYPE",8), strText.c_str());
        GetBasicChatWindow()->AddStringToChatEx( UI::ToString( strTemp ), CHAT_CONST::CHAT_SYSTEM );
        m_pSystemMessageWindow->SetSystemMessage(strText.c_str(), NS_UITEXTCOLOR::HOTPINK);
    }
}

/*
void CInnerInterface::PrintMsgDlg(D3DCOLOR dwColor, const CString& strText)
{
	GASSERT(m_pSystemMessageWindow);
	if (m_pSystemMessageWindow )
		m_pSystemMessageWindow->SetSystemMessage(strText, dwColor);
}
*/

void CInnerInterface::PrintMsgDlg(D3DCOLOR dwColor, const std::string& strText)
{
    GASSERT(m_pSystemMessageWindow);
    if (m_pSystemMessageWindow )
        m_pSystemMessageWindow->SetSystemMessage(strText.c_str(), dwColor);
}

void CInnerInterface::PrintAddString( D3DCOLOR dwColor, const std::string& strText)
{
	if(GetBasicChatWindow())
		GetBasicChatWindow()->AddStringToNomal(strText.c_str(), dwColor);
}
// ----------------------------------------------------------------------------

void CInnerInterface::SetTradeWindowClose ()
{
	UiHideGroup( TRADE_WINDOW );

	if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
		UiHideGroup( INVENTORY_WINDOW_RENEW );
	else
		UiHideGroup ( TRADEINVENTORY_WINDOW );
}

void	CInnerInterface::SetTradeWindowCloseReq ()
{
	//	��� ��ư ����
	//
	GLMSG::SNET_TRADE_CANCEL NetMsg;
	m_pGaeaClient->NETSENDTOAGENT(&NetMsg);
}

void	CInnerInterface::SetFightBegin( const int nIndex )
{
	GetConftDisplayMan()->SetFightBegin( nIndex );
	UiShowGroupTop( CONFT_DISPLAY_MAN );
}

void CInnerInterface::SET_PKCOMBO ( int nIndex )
{
	if ( m_pAnimatedPKStreak )
	{
		m_pAnimatedPKStreak->SetAnimation ( nIndex );
	}
}

void CInnerInterface::TriggerKillCard ( int killType, DWORD targetID )
{
	if ( m_pKillCardManager )
	{
		m_pKillCardManager->TriggerKillCard ( (EKILL_TYPE)killType, targetID );
	}
}

void	CInnerInterface::SetFightEnd( const int nResult )
{
	GetConftDisplayMan()->SetFightEnd( nResult );
	UiShowGroupTop( CONFT_DISPLAY_MAN );
}

int CInnerInterface::SkilSlotTrayGetTab()
{
	if ( m_pSkillTrayTab )
	{
		return m_pSkillTrayTab->GetTabIndex();
	}

	return 0;
}

void CInnerInterface::SkilSlotTraySetTab( int nIndex )
{
	if ( m_pSkillTrayTab )
	{
		m_pSkillTrayTab->SetTabIndex( nIndex );
	}
}

void CInnerInterface::SkilSlotTraySelectSlot( int nIndex )
{
	if ( m_pSkillTrayTab )
	{
		m_pSkillTrayTab->SelectSlot( nIndex );
	}
}

void CInnerInterface::SkilSlotTrayToggle ()
{
	if ( m_pGaeaClient->GetTutorial()->IsTutorial() ) // Ʃ�丮�� �߿��� ������.
	{
		m_pGaeaClient->PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("TUTORIAL_CANNOT_USE") );
	}
	else if ( m_pSkillTrayTab )
	{
        m_pSkillTrayTab->TrayToggle();
	}
}

void CInnerInterface::SkilSlotTrayOpen ()
{
    if ( m_pSkillTrayTab )
    {
        m_pSkillTrayTab->TrayOpen();
    }
}

void CInnerInterface::CLEAR_TEXT ( int nAlignFlag )
{
	if ( m_nINFO_DISPLAY_TYPE == EM_INFO_DISPLAY_BASIC )
	{
		m_pInfoDisplay->ClearText ();
		m_pInfoDisplay->SetTextAlign( nAlignFlag );
	}
	else if ( m_nINFO_DISPLAY_TYPE == EM_INFO_DISPLAY_EX )
	{
		m_pInfoDisplayEx->ClearText ();	
		m_pInfoDisplayEx->SetTextAlign ( nAlignFlag );
	}
	else if ( m_nINFO_DISPLAY_TYPE == EM_INFO_DISPLAY_LINK )
	{
		m_pInfoDisplayLink->ClearText ();	
		m_pInfoDisplayLink->SetTextAlign ( nAlignFlag );
	}
}

CItemToolTipGroup* CInnerInterface::GetInvenItemTooltipGroup()
{
    return m_pInvenItemTooltipGroup;
}

CItemToolTipGroup* CInnerInterface::GetWishItemTooltipGroup()
{
    return m_pWishItemTooltipGroup;
}

CItemToolTipGroup* CInnerInterface::GetMarketWishItemTooltipGroup()
{
    return m_pMarketWishItemTooltipGroup;
}

void CInnerInterface::RESET_INFO ()
{
	RESET ();
    INFO_DISPLAY_ITEM_RESET ();
	INFO_DISPLAY_SKILL_RESET ();
	m_strGeneralInfoBack.Empty ();
	if( m_pInfoDisplay ) m_pInfoDisplay->ClearText ();
}

void CInnerInterface::ADDTEXT_NOSPLIT ( const CString& strText, const D3DCOLOR& dwColor )
{
	if ( m_nINFO_DISPLAY_TYPE == EM_INFO_DISPLAY_BASIC )
	{
		m_pInfoDisplay->AddTextNoSplit ( strText, dwColor );
	}
	else if ( m_nINFO_DISPLAY_TYPE == EM_INFO_DISPLAY_EX )
	{
		m_pInfoDisplayEx->AddTextNoSplit ( strText, dwColor );
	}
	else if ( m_nINFO_DISPLAY_TYPE == EM_INFO_DISPLAY_LINK )
	{
		m_pInfoDisplayLink->AddTextNoSplit ( strText, dwColor );
	}
}

void CInnerInterface::ADDTEXT_LONGESTLINE_SPLIT ( const CString& strText, const D3DCOLOR& dwColor )
{

	if ( m_nINFO_DISPLAY_TYPE == EM_INFO_DISPLAY_BASIC )
	{
		m_pInfoDisplay->AddTextLongestLineSplit ( strText, dwColor );
	}
	else if ( m_nINFO_DISPLAY_TYPE == EM_INFO_DISPLAY_EX )
	{
		m_pInfoDisplayEx->AddTextLongestLineSplit ( strText, dwColor );
	}
	else if ( m_nINFO_DISPLAY_TYPE == EM_INFO_DISPLAY_LINK )
	{
		m_pInfoDisplayLink->AddTextLongestLineSplit ( strText, dwColor );
	}
    else
    {
    }
}

void CInnerInterface::SHOW_PARTY_LINK_WINDOW ( int nLinkIndex, const GLPARTY_LINK& sPartyLink )
{
	if ( !m_pPartyLinkWindow ) return;
	
	m_pPartyLinkWindow->SetPartyLink( nLinkIndex, sPartyLink );
	m_pPartyLinkWindow->SetVisibleSingle( TRUE );
}

void CInnerInterface::SHOW_LINK_ITEM_INFO ( SITEMCUSTOM & sItemCustom )
{
	if ( m_etInfoType != ET_ITEM_INFO )
	{
		RESET_INFO ();
		m_etInfoType = ET_ITEM_INFO;
	}

	if ( GLUseFeatures::GetInstance().IsUsingItemTooltip() )
	{
		const UIRECT& rcBase     = m_pItemInforTooltipLink->GetBasePos();
		const UIRECT  rcPosition = m_pItemInforTooltipLink->AlignMainControl(rcBase);

		DisplayItemInformationLink(
			sItemCustom,
			FALSE, 
			FALSE, 
			FALSE, 
			FALSE, 
			FALSE, 
			(WORD)rcPosition.left, 
			(WORD)rcPosition.top, 
			NATIVEID_NULL() );

		UiShowGroupFocus( ITEM_INFOR_TOOLTIP_LINK );
	}
	else
	{
		m_nINFO_DISPLAY_TYPE = EM_INFO_DISPLAY_LINK;
		INFO_DISPLAY_ITEM_LOAD ( sItemCustom, FALSE, FALSE, FALSE, FALSE, FALSE, USHRT_MAX, USHRT_MAX, NATIVEID_NULL() );
		UiShowGroupFocus ( INFO_DISPLAY_LINK );
	}
	/*m_nINFO_DISPLAY_TYPE = EM_INFO_DISPLAY_LINK;
	LOAD ( sItemCustom, FALSE, FALSE, FALSE, FALSE, USHRT_MAX, USHRT_MAX, NATIVEID_NULL(),  TRUE, FALSE ); 
	UiShowGroupFocus ( INFO_DISPLAY_LINK );*/
}

void CInnerInterface::SHOW_ITEM_INFO(
    const SITEMCUSTOM& sItemCustom,
    BOOL bShopOpen,
    BOOL bInMarket,
    BOOL bInPrivateMarket,
    BOOL bPreviewUse,
	BOOL bCompare,
    WORD wPosX,
    WORD wPosY,
    SNATIVEID sNpcNativeID /* = SNATIVEID */,
	int  iPageIndex /* = -1 */)
{	
	if ( uiman::GetInnerUIMan().IsMouseInControl() )
		return;

	// �ٸ� â���� ���� ����Ÿ�� ���� �ִٴ� �̾߱��̴�.
	if ( UiIsVisibleGroup ( INFO_DISPLAY ) )
		return;
	
	if ( m_etInfoType != ET_ITEM_INFO )
	{
		RESET_INFO ();
		m_etInfoType = ET_ITEM_INFO;
	}

	m_nINFO_DISPLAY_TYPE = EM_INFO_DISPLAY_BASIC;


	//LOAD ( sItemCustom, bShopOpen, bInMarket, bInPrivateMarket, FALSE, wPosX, wPosY, sNpcNativeID,  FALSE, FALSE ); 
	//UiShowGroupTop ( INFO_DISPLAY );

   if ( GLUseFeatures::GetInstance().IsUsingItemTooltip() )
    {
        bool bResult = DisplayItemInformation(
						sItemCustom,
						bShopOpen,
						bInMarket,
						bInPrivateMarket,
						bPreviewUse,
						FALSE,
						wPosX,
						wPosY,
						sNpcNativeID,
						iPageIndex );

        UiShowGroupTop( ITEM_INFOR_TOOLTIP );

		const SITEM* pItem = GLogicData::GetInstance().GetItem ( sItemCustom.GetNativeID() );

		if( pItem && 
			pItem->sItemSkill.sSkillID.IsValidNativeID() ) {

			if( bResult )
				ShowSkillToolTipMulti( pItem->sItemSkill.sSkillID, pItem->sItemSkill.wSkillLv );

			UiShowGroupTop(SKILL_INFOR_TOOLTIP_MULTI);
		}
    }
    else
    {
	    INFO_DISPLAY_ITEM_LOAD(
            sItemCustom,
            bShopOpen,
            bInMarket,
            bInPrivateMarket,
            bPreviewUse,
            FALSE,
            wPosX,
            wPosY,
            sNpcNativeID,
			iPageIndex );
	    UiShowGroupTop ( INFO_DISPLAY );
    }

	m_bUSING_INFO_DISPLAY = true;

	//	�� ���� ( �ڱⰡ ���� �����ϰ� �ִ� �����۰� ���콺 ������ �������� ������� )
	BOOL bTRADE = UiIsVisibleGroup ( TRADE_WINDOW );
	if ( bTRADE || bInMarket || bInPrivateMarket || bCompare )
	{
		for( int nSLOT = SLOT_HEADGEAR; nSLOT < SLOT_NSIZE_S_2; ++nSLOT )
		{			
			if ( EMSLOTCHECK_OK == m_pGaeaClient->GetCharacter()->CHECKSLOT_ITEM ( sItemCustom.GetNativeID(), (EMSLOT) nSLOT ) )
			{
				SITEMCUSTOM sItemCustom = m_pGaeaClient->GetCharacter()->GET_SLOT_ITEM ( (EMSLOT) nSLOT );
				if ( sItemCustom.GetNativeID() == NATIVEID_NULL () )
					return ;

				m_nINFO_DISPLAY_TYPE = EM_INFO_DISPLAY_EX;

				//LOAD ( sItemCustom, FALSE, FALSE, FALSE, TRUE, wPosX, wPosY, sNpcNativeID, FALSE, FALSE );
				//UiShowGroupTop ( INFO_DISPLAY_EX );

                if ( GLUseFeatures::GetInstance().IsUsingItemTooltip() )
                {
                    DisplayItemInformation(
                        sItemCustom,
                        FALSE,
                        FALSE,
                        FALSE,
                        FALSE,
                        TRUE,
                        wPosX,
                        wPosY,
                        sNpcNativeID );
                    UiShowGroupTop( ITEM_INFOR_TOOLTIP_WEAR );
                }
                else
                {
				    {
					    //	ITEM ���� �̱�, ������ -1�� �ǹ̾��� ����
					    //
					    INFO_DISPLAY_ITEM_LOAD ( sItemCustom, FALSE, FALSE, FALSE, FALSE, TRUE, wPosX, wPosY, sNpcNativeID );
				    }

				    UiShowGroupTop ( INFO_DISPLAY_EX );
                }
			}
		}		
	}
}

void CInnerInterface::SHOW_ITEM_INFO_DURABILITY(
	const SITEMCUSTOM& sItemCustom,
	BOOL bShopOpen,
	BOOL bInMarket,
	BOOL bInPrivateMarket,
	BOOL bPreviewUse,
	BOOL bCompare,
	WORD wPosX,
	WORD wPosY,
	SNATIVEID sNpcNativeID /* = SNATIVEID */,
	int  iPageIndex /* = -1 */)
{	
	if ( uiman::GetInnerUIMan().IsMouseInControl() )
		return;

	// �ٸ� â���� ���� ����Ÿ�� ���� �ִٴ� �̾߱��̴�.
	if ( UiIsVisibleGroup ( INFO_DISPLAY ) )
		return;

	if ( m_etInfoType != ET_ITEM_INFO )
	{
		RESET_INFO ();
		m_etInfoType = ET_ITEM_INFO;
	}

	m_nINFO_DISPLAY_TYPE = EM_INFO_DISPLAY_BASIC;

	if ( GLUseFeatures::GetInstance().IsUsingItemTooltip() )
	{
		bool bResult = DisplayItemInformation_Durability(
			sItemCustom,
			bShopOpen,
			bInMarket,
			bInPrivateMarket,
			bPreviewUse,
			FALSE,
			FALSE,
			wPosX,
			wPosY,
			sNpcNativeID,
			iPageIndex );

		UiShowGroupTop( ITEM_INFOR_TOOLTIP );
		
		DisplayItemInformation_Durability(
			sItemCustom,
			bShopOpen,
			bInMarket,
			bInPrivateMarket,
			bPreviewUse,
			TRUE,
			TRUE,
			wPosX,
			wPosY,
			sNpcNativeID,
			iPageIndex );

		UiShowGroupTop( ITEM_INFOR_TOOLTIP_DURABILITY );


		const SITEM* pItem = GLogicData::GetInstance().GetItem ( sItemCustom.GetNativeID() );

		if( pItem && 
			pItem->sItemSkill.sSkillID.IsValidNativeID() ) {

				if( bResult )
					ShowSkillToolTipMulti( pItem->sItemSkill.sSkillID, pItem->sItemSkill.wSkillLv );

				UiShowGroupTop(SKILL_INFOR_TOOLTIP_MULTI);
		}
	}
	else
	{
		INFO_DISPLAY_ITEM_LOAD(
			sItemCustom,
			bShopOpen,
			bInMarket,
			bInPrivateMarket,
			bPreviewUse,
			FALSE,
			wPosX,
			wPosY,
			sNpcNativeID,
			iPageIndex );
		UiShowGroupTop ( INFO_DISPLAY );
	}

	m_bUSING_INFO_DISPLAY = true;

	//	�� ���� ( �ڱⰡ ���� �����ϰ� �ִ� �����۰� ���콺 ������ �������� ������� )
	BOOL bTRADE = UiIsVisibleGroup ( TRADE_WINDOW );
	if ( bTRADE || bInMarket || bInPrivateMarket || bCompare )
	{
		for( int nSLOT = SLOT_HEADGEAR; nSLOT < SLOT_NSIZE_S_2; ++nSLOT )
		{			
			if ( EMSLOTCHECK_OK == m_pGaeaClient->GetCharacter()->CHECKSLOT_ITEM ( sItemCustom.GetNativeID(), (EMSLOT) nSLOT ) )
			{
				SITEMCUSTOM sItemCustom = m_pGaeaClient->GetCharacter()->GET_SLOT_ITEM ( (EMSLOT) nSLOT );
				if ( sItemCustom.GetNativeID() == NATIVEID_NULL () )
					return ;

				m_nINFO_DISPLAY_TYPE = EM_INFO_DISPLAY_EX;

				if ( GLUseFeatures::GetInstance().IsUsingItemTooltip() )
				{
					DisplayItemInformation(
						sItemCustom,
						FALSE,
						FALSE,
						FALSE,
						FALSE,
						TRUE,
						wPosX,
						wPosY,
						sNpcNativeID );
					UiShowGroupTop( ITEM_INFOR_TOOLTIP_WEAR );
				}
				else
				{
					{
						//	ITEM ���� �̱�, ������ -1�� �ǹ̾��� ����
						//
						INFO_DISPLAY_ITEM_LOAD ( sItemCustom, FALSE, FALSE, FALSE, FALSE, TRUE, wPosX, wPosY, sNpcNativeID );
					}

					UiShowGroupTop ( INFO_DISPLAY_EX );
				}
			}
		}		
	}
}
void CInnerInterface::SHOW_ITEM_INFO( WishList::ItemSPtr pWishItem )
{
 	if( !pWishItem )
		return;

	if( uiman::GetInnerUIMan().IsMouseInControl () )
		return;

	if( !GLUseFeatures::GetInstance().IsUsingItemTooltip() )
		return;

	if( m_etInfoType != ET_ITEM_INFO )
	{
		RESET_INFO();
		m_etInfoType = ET_ITEM_INFO;
	}

	m_nINFO_DISPLAY_TYPE = EM_INFO_DISPLAY_BASIC;

    DisplayWishListItemInfo( pWishItem );

	UiShowGroupTop( ITEM_INFOR_TOOLTIP );

	m_bUSING_INFO_DISPLAY = true;

	//	�� ���� ( �ڱⰡ ���� �����ϰ� �ִ� �����۰� ���콺 ������ �������� ������� )
	for( int nSLOT = SLOT_HEADGEAR; nSLOT < SLOT_NSIZE_S_2; ++nSLOT )
	{			
		if ( EMSLOTCHECK_OK == m_pGaeaClient->GetCharacter()->CHECKSLOT_ITEM( pWishItem->m_sItem.GetNativeID(), (EMSLOT) nSLOT ) )
		{
			SITEMCUSTOM sItemCustom = m_pGaeaClient->GetCharacter()->GET_SLOT_ITEM( (EMSLOT) nSLOT );
			if ( sItemCustom.GetNativeID() == NATIVEID_NULL () )
				return;

			m_nINFO_DISPLAY_TYPE = EM_INFO_DISPLAY_EX;

			DisplayItemInformation(
                sItemCustom,
                FALSE,
                FALSE,
                FALSE,
                FALSE,
                TRUE,
                USHRT_MAX,
                USHRT_MAX,
                NATIVEID_NULL() );

			UiShowGroupTop( ITEM_INFOR_TOOLTIP_WEAR );
		}
	}
}

void CInnerInterface::CloseItemToolTipMulti( UIGUID ControlID )
{
    m_pInvenItemTooltipGroup->CloseItemToolTipMulti( ControlID );
    m_pWishItemTooltipGroup->CloseItemToolTipMulti( ControlID );
    m_pMarketWishItemTooltipGroup->CloseItemToolTipMulti( ControlID );
}

void CInnerInterface::CloseItemToolTipMultiAll()
{
	m_pInvenItemTooltipGroup->CloseItemToolTipMultiAll();
    m_pWishItemTooltipGroup->CloseItemToolTipMultiAll();
    m_pMarketWishItemTooltipGroup->CloseItemToolTipMultiAll();
}

void CInnerInterface::SHOW_SKILL_INFO ( SNATIVEID sNativeID, const BOOL bNextLevel, const WORD wAddTempLevel /* = 0*/ )
{
	if ( uiman::GetInnerUIMan().IsMouseInControl () ) return ;

	if ( UiIsVisibleGroup ( INFO_DISPLAY ) ) return ;
	if ( m_etInfoType != ET_SKILL_INFO )
	{
		RESET_INFO ();
		m_etInfoType = ET_SKILL_INFO;
	}

    if ( GLUseFeatures::GetInstance().IsUsingSkillTooltip() )
    {
        ShowSkillTooltip( sNativeID, bNextLevel, wAddTempLevel );
    }
    else
    {
	    INFO_DISPLAY_SKILL_LOAD( sNativeID, bNextLevel );
	    UiShowGroupTop( INFO_DISPLAY );
    }

	m_nINFO_DISPLAY_TYPE = EM_INFO_DISPLAY_BASIC;
	m_bUSING_INFO_DISPLAY = true;
}

void CInnerInterface::ShowSkillToolTipMulti( GLCHARLOGIC* pTarget, SNATIVEID sNativeID, int x, int y, BOOL bMultiSkill )
{
	if( uiman::GetInnerUIMan().IsExclusiveControl() )
		return;

	if( m_vecSkillToolTipPool.empty() )
		return;

	if( sNativeID == NATIVEID_NULL() )
		return;

	GLSKILL* pSkill = GLSkillMan::GetInstance().GetData( sNativeID );
	if( !pSkill )
		return;

	BOOST_FOREACH( SSkillToolTip& sToolTip, m_vecSkillToolTipUsed )
	{
		if( sToolTip.pControl->GetSkillID() == sNativeID )
			return;
	}

	SSkillToolTip sToolTip = m_vecSkillToolTipPool.back();
	m_vecSkillToolTipPool.pop_back();

	if (bMultiSkill)
		pSkill->SetMultiSkillFlag(TRUE);

    SCHARSKILL* pCharSkill = pTarget->GETLEARNED_SKILL( pSkill->GetId() );
    if ( !pCharSkill )
        return;
    sToolTip.pControl->DisplaySkillInformation( pTarget, pSkill, pCharSkill->wLevel);
	sToolTip.pControl->AllControlRePos( x, y );
	sToolTip.pControl->SetVisibleSingle( TRUE );
	UiShowGroupFocus( sToolTip.nID );

	m_vecSkillToolTipUsed.push_back( sToolTip );
}

void CInnerInterface::CloseSkillToolTipMulti( UIGUID ControlID )
{
	SkillToolTipVecIter it = m_vecSkillToolTipUsed.begin();
	for( ; it != m_vecSkillToolTipUsed.end(); ++it )
	{
		SSkillToolTip& sToolTip = *it;

		if( sToolTip.nID != ControlID )
			continue;

		sToolTip.pControl->SetVisibleSingle( FALSE );
		UiHideGroup( sToolTip.nID );

		m_vecSkillToolTipPool.push_back( sToolTip );
		m_vecSkillToolTipUsed.erase( it );

		return;
	}
}

void CInnerInterface::CloseSkillToolTipMultiAll()
{
	BOOST_FOREACH( SSkillToolTip& sToolTip, m_vecSkillToolTipUsed )
	{
		sToolTip.pControl->SetVisibleSingle( FALSE );
		UiHideGroup( sToolTip.nID );

		m_vecSkillToolTipPool.push_back( sToolTip );
	}

	m_vecSkillToolTipUsed.clear();
}

bool CInnerInterface::BEGIN_COMMON_LINEINFO_MULTI ( int nAlignFlag )
{
	if ( uiman::GetInnerUIMan().IsMouseInControl () ) return false;
	if ( UiIsVisibleGroup ( INFO_DISPLAY ) ) return false;
	if ( m_etInfoType != ET_GENERAL_INFO )
	{
		RESET_INFO ();
		m_etInfoType = ET_GENERAL_INFO;
	}

	m_strGeneralInfoBack.Empty ();
	m_pInfoDisplay->ClearText ();
	m_pInfoDisplay->SetTextAlign( nAlignFlag );

	m_nINFO_DISPLAY_TYPE = EM_INFO_DISPLAY_BASIC;
	m_bUSING_INFO_DISPLAY = true;

	return true;
}

bool CInnerInterface::ADD_COMMON_LINEINFO_MULTI ( const CString& strText, D3DCOLOR dwColor )
{
	if ( UiIsVisibleGroup ( INFO_DISPLAY ) ) return false;

	m_pInfoDisplay->AddTextNoSplit ( strText, dwColor );

	return true;
}

void CInnerInterface::END_COMMON_LINEINFO_MULTI ()
{
	UiShowGroupTop ( INFO_DISPLAY );
}

void CInnerInterface::MultiLineCommentUtil( const char* pText, CD3DFontPar* pFont, float fWidth, int nTextAlign, DWORD nColor )
{
	if( !pText || !pFont || fWidth <= 0.0f )
		return;

	NS_UITEXTUTIL::MULTILINETEXT vecLine = NS_UITEXTUTIL::GetMLTextWithoutCRLF( pText, fWidth, pFont );
	if( vecLine.empty() )
		return;

	BEGIN_COMMON_LINEINFO_MULTI( nTextAlign );

	for( size_t i=0; i<vecLine.size(); ++i )
		ADD_COMMON_LINEINFO_MULTI( vecLine[ i ].strLine, nColor );

	END_COMMON_LINEINFO_MULTI();
}

void CInnerInterface::SHOW_COMMON_LINEINFO ( const CString& strText, D3DCOLOR dwColor, int nAlignFlag )
{
	if ( uiman::GetInnerUIMan().IsMouseInControl () ) return ;
	if ( UiIsVisibleGroup ( INFO_DISPLAY ) ) return ;		
	if ( m_etInfoType != ET_GENERAL_INFO )
	{
		RESET_INFO ();
		m_etInfoType = ET_GENERAL_INFO;
	}

	if ( !strText.GetLength() ) return ;

	if ( m_strGeneralInfoBack != strText )
	{
		m_pInfoDisplay->ClearText ();
		m_pInfoDisplay->AddTextNoSplit ( strText, dwColor );
		m_pInfoDisplay->SetTextAlign( nAlignFlag );

		m_strGeneralInfoBack = strText;
	}

	UiShowGroupTop ( INFO_DISPLAY );

	m_nINFO_DISPLAY_TYPE = EM_INFO_DISPLAY_BASIC;
	m_bUSING_INFO_DISPLAY = true;
}

const bool CInnerInterface::IsPartyMember(const char* const szName) const
{
	if ( szName == NULL )
		return FALSE;

	GLPartyClient* const pMyParty = m_pGaeaClient->GetMyPartyClient();
	if ( pMyParty->isValid() == false )
		return FALSE;

	return pMyParty->isMember(szName);
}

void CInnerInterface::ClearNameList ()
{
	if ( m_pNameDisplayMan ) m_pNameDisplayMan->ClearList ();
}

void CInnerInterface::SetAcademyFightBegin ( const int& nLMARK, const int& nLNUMBER, const int& nRMARK, const int& nRNUMBER )
{
	m_pAcademyConftDisplay->SET_MARK_L ( nLMARK );
	m_pAcademyConftDisplay->SET_MARK_R ( nRMARK );
	m_pAcademyConftDisplay->SET_NUMBER_L ( nLNUMBER );
	m_pAcademyConftDisplay->SET_NUMBER_R ( nRNUMBER );
	m_pAcademyConftDisplay->UPDATE_ALPHA ( USHRT_MAX );

	UiShowGroupTop ( ACADEMY_CONFT_DISPLAY, true );

	m_bACADEMY_FIGHT_READY = true;
}

void CInnerInterface::SetAcademyUpdateNumber ( const int& nLNUMBER, const int& nRNUMBER )
{
	const int nLNUMBER_BACK = m_pAcademyConftDisplay->GET_NUMBER_L ();
	const int nRNUMBER_BACK = m_pAcademyConftDisplay->GET_NUMBER_R ();
	bool bUPDATE = false;

	if ( nLNUMBER_BACK != nLNUMBER )
	{
		m_pAcademyConftDisplay->SET_NUMBER_L ( nLNUMBER );
		bUPDATE = true;
	}
	if ( nRNUMBER_BACK != nRNUMBER )
	{
		m_pAcademyConftDisplay->SET_NUMBER_R ( nRNUMBER );
		bUPDATE = true;
	}

	if ( bUPDATE || m_bACADEMY_FIGHT_READY ) m_pAcademyConftDisplay->UPDATED_NUMBER ();

	m_bACADEMY_FIGHT_READY = false;
}

void CInnerInterface::SetAcademyFightEnd ()
{
	UiHideGroup ( ACADEMY_CONFT_DISPLAY, true );
}

void CInnerInterface::AUTO_LOAD_QUEST( DWORD dwID )
{
	if ( m_pQuestWindowRenewal )
		m_pQuestWindowRenewal->AutoLoadQuest( dwID );
}

void CInnerInterface::REFRESH_QUEST_WINDOW()
{
	// [shhan][2014.11.13] WorldEdit ���� ���������� ��Ÿ���� �˻���.
	if ( !m_pQuestWindowRenewal )
		return;

    if ( UiIsVisibleGroup( QUEST_WINDOW ) )
        m_pQuestWindowRenewal->RefreshQuestWindow();

    m_pQuestHelper->UpdateQuestInfo();
}

void CInnerInterface::QUEST_HELPTER_TO_WINDOW( DWORD dwQuestID )
{
    UiShowGroupFocus( QUEST_WINDOW );

    if ( m_pQuestWindowRenewal && m_pQuestWindowRenewal->GetWndID() == QUEST_WINDOW )
    {
        m_pQuestWindowRenewal->AutoLoadQuest( dwQuestID );
    }
}

void CInnerInterface::REFRESH_CLUB_LIST ()
{
	if ( UiIsVisibleGroup ( CLUB_WINDOW ) ) 
	{
		m_pClubWindow->RefreshClubList();
	}
}

void CInnerInterface::REFRESH_CLUB_ALLIANCE_LIST()
{
	if ( UiIsVisibleGroup ( CLUB_WINDOW ) )
        m_pClubWindow->LoadClubAllianceList ();
}

void CInnerInterface::REFRESH_CLUB_BATTLE_LIST()
{
	if ( UiIsVisibleGroup ( CLUB_WINDOW ) )
        m_pClubWindow->LoadClubBattleList ();
}

void CInnerInterface::REFRESH_CLUB_STATE ()
{
//	if ( UiIsVisibleGroup ( CLUB_WINDOW ) )
	if ( m_pClubWindow )
        m_pClubWindow->RefreshState ();
}

void CInnerInterface::REFRESH_CLUB_NOTICE()
{
	if ( UiIsVisibleGroup ( CLUB_WINDOW ) )
        m_pClubWindow->SetAnnouncement();
}

void CInnerInterface::REFRESH_ATTENDBOOK()
{
	if ( m_pAttendanceBookWindow )
        m_pAttendanceBookWindow->RefreshAttend();
}

void CInnerInterface::SHOW_ITEM_INFO_SIMPLE(const SITEMCUSTOM& sItemCustom)
{
	if ( uiman::GetInnerUIMan().IsMouseInControl () ) return ;

	//	NOTE
	//		�̹� �Ѹ��� �ִٴ� ����
	//		�ٸ� â���� ���� ����Ÿ�� ���� �ִٴ� �̾߱��̴�.
	if ( UiIsVisibleGroup ( INFO_DISPLAY ) ) return ;
	if ( m_etInfoType != ET_ITEM_INFO )
	{
		RESET_INFO ();
		m_etInfoType = ET_ITEM_INFO;
	}

	INFO_DISPLAY_ITEM_LOAD_SIMPLE ( sItemCustom );
	UiShowGroupTop ( INFO_DISPLAY );

	m_nINFO_DISPLAY_TYPE = EM_INFO_DISPLAY_BASIC;
	m_bUSING_INFO_DISPLAY = true;
}

void	CInnerInterface::UpdateStateQuestAlarm ()
{
	if ( !UiIsVisibleGroup ( QUEST_WINDOW ) )	//	����Ʈâ�� �ݱ� ����
	{
		GLQuestPlay& cQuestPlay = m_pGaeaClient->GetCharacter()->m_cQuestPlay;
		if ( !cQuestPlay.GetQuestProc ().empty () )	//	�������� ����Ʈ�� �����Ѵ�.
		{
			if ( !UiIsVisibleGroup ( QUEST_ALARM ) )
			{
				UiShowGroupBottom ( QUEST_ALARM );	//	����Ʈ ������ ǥ��
			}
			
			DWORD dwQuestID = NATIVEID_NULL().dwID;			
			if ( cQuestPlay.GetReqREADING ( dwQuestID ) )	//	����Ʈ �̺�Ʈ �߻�
			{
				GLQUESTPROG* pQuestProg = cQuestPlay.FindProc ( dwQuestID );
				if ( !pQuestProg ) return ;				
				const DWORD dwEventQuestStep = pQuestProg->m_dwSTEP;

				if ( m_dwEventQuestID != dwQuestID || dwEventQuestStep != m_dwEventQuestStep )
				{
					DxSoundLib::GetInstance()->PlaySound ( "QuestAlarm" );
					m_dwEventQuestID = dwQuestID;
					m_dwEventQuestStep = dwEventQuestStep;
					
					//RESET_QUEST_HELPER( dwQuestID );
				}

				if ( m_pQuestAlarm )
					m_pQuestAlarm->SET_ALARM( true );
//				m_pQuestWindow->SetQuestID ( m_dwEventQuestID );

				return ;
			}
			else
			{
				if ( m_pQuestAlarm )
					m_pQuestAlarm->SET_ALARM( false );
			}
		}
		else
		{
			UiHideGroup ( QUEST_ALARM );
		}
	}
	else
	{
		GLQuestPlay& cQuestPlay = m_pGaeaClient->GetCharacter()->m_cQuestPlay;
		if ( !cQuestPlay.GetQuestProc ().empty () )	//	�������� ����Ʈ�� �����Ѵ�.
		{
			DWORD dwQuestID = NATIVEID_NULL().dwID;
			if ( cQuestPlay.GetReqREADING ( dwQuestID ) )	//	����Ʈ �̺�Ʈ �߻�
			{
				GLQUESTPROG* pQuestProg = cQuestPlay.FindProc ( dwQuestID );
				if ( !pQuestProg ) return ;				
				const DWORD dwEventQuestStep = pQuestProg->m_dwSTEP;

				if ( m_dwEventQuestID != dwQuestID && dwEventQuestStep != m_dwEventQuestStep )
				{
					DxSoundLib::GetInstance()->PlaySound ( "QuestAlarm" );
					m_dwEventQuestID = dwQuestID;
					m_dwEventQuestStep = dwEventQuestStep;

					RESET_QUEST_HELPER( dwQuestID );
				}

				//	�̺�Ʈ �߻��� ID�� ���� ���� �ִ� ����Ʈ�� ID�� �ٸ����
				if ( m_pQuestWindowRenewal && m_dwEventQuestID != m_pQuestWindowRenewal->GetQuestMainID() )
				{
					if ( m_pQuestAlarm )
						m_pQuestAlarm->SET_ALARM( true );
//					m_pQuestWindow->SetQuestID ( m_dwEventQuestID );
					
					if ( !UiIsVisibleGroup ( QUEST_ALARM ) )
					{
						UiShowGroupBottom ( QUEST_ALARM );	//	����Ʈ ������ ǥ��
					}

					return ;
				}
				else
				{
					if ( m_pQuestAlarm )
						m_pQuestAlarm->SET_ALARM( false );
				}

				m_pGaeaClient->GetCharacter()->ReqQuestREADINGReset ( m_dwEventQuestID );					
				m_dwEventQuestID = NATIVEID_NULL().dwID;				
			}
			else
			{
				if ( m_pQuestAlarm )
					m_pQuestAlarm->SET_ALARM( false );
			}
		}

		//  [12/27/2012 gbgim]
		//UiHideGroup ( QUEST_ALARM );
	}

//#ifdef CH_PARAM // �߱� �������̽� ����
//	UpdatePotionTrayPosition();
//#endif
}

void CInnerInterface::DOMODAL_ADD_FRIEND_REQ( const CString& strName )
{
	if ( UiIsVisibleGroup ( MODAL_WINDOW ) )
	{
		return ;
	}

	m_strFriendName = strName;
	std::string strCombine = sc::string::format( ID2GAMEINTEXT("ADD_FRIEND_REQ"), strName );	
	DoModal ( strCombine, UI::MODAL_QUESTION, UI::YESNO, UI::MODAL_ADD_FRIEND_LURE_ANS );
}

void CInnerInterface::REFRESH_FRIEND_LIST()
{
	if ( !UiIsVisibleGroup( FRIEND_WINDOW ) ) return ;

	//	��� ���� ( �߰�, ����... �׿� )
	m_pFriendWindow->LoadFriendList();
	m_pBasicChat->RefreshFriendList();
}

void CInnerInterface::REFRESH_FRIEND_STATE()
{
	if ( !UiIsVisibleGroup ( FRIEND_WINDOW ) ) return ;

	//	���� ���� ( ��/�������� )
	m_pFriendWindow->LoadFriendList();
}

void CInnerInterface::OPEN_MODAL ( const CString& strText, int nModalTitle, int nModalType, UIGUID CallerID, BOOL bHide )
{
	const int nMODAL_NEWID = CModalWindow::MAKE_MODAL_NEWID () + MODAL_WINDOW;	

	CModalWindow* pModalWindow = new CModalWindowInner ( this, m_pEngineDevice );
	pModalWindow->Create ( nMODAL_NEWID, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pModalWindow->CreateBaseModal ( "MODAL_WINDOW");
	pModalWindow->CreateSubControl ();
	pModalWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
	pModalWindow->DoModal ( strText.GetString(), nModalTitle, nModalType, CallerID );
	pModalWindow->SetHide ( bHide );
	pModalWindow->m_bTEST_MULTI = true;
	pModalWindow->InitDeviceObjects( uiman::GetInnerUIMan().GetDevice() );
	pModalWindow->RestoreDeviceObjects( uiman::GetInnerUIMan().GetDevice() );
	UiRegisterControl( pModalWindow );	

	UiShowGroupFocus( nMODAL_NEWID );
}

void CInnerInterface::OPEN_MODALESS ( const CString& strText, int nModalTitle, int nModalType, UIGUID CallerID, BOOL bHide )
{
	const int nMODAL_NEWID = CModalWindow::MAKE_MODAL_NEWID () + MODAL_WINDOW;

	CModalWindow* pModalWindow = new CModalWindowInner ( this, m_pEngineDevice );
	pModalWindow->Create ( nMODAL_NEWID, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pModalWindow->CreateBaseModal ( "MODAL_WINDOW");
	pModalWindow->CreateSubControl ();
	pModalWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
	pModalWindow->DoModal ( strText.GetString(), nModalTitle, nModalType, CallerID );
	pModalWindow->SetHide ( bHide );
	pModalWindow->m_bTEST_MULTI = true;
	pModalWindow->SetUseModaless ( true );
	pModalWindow->InitDeviceObjects( uiman::GetInnerUIMan().GetDevice() );
	pModalWindow->RestoreDeviceObjects( uiman::GetInnerUIMan().GetDevice() );
	UiRegisterControl( pModalWindow );	

	UiShowGroupFocus( nMODAL_NEWID );
}

void CInnerInterface::CLOSE_MODAL ( const UIGUID cID, bool bMakeMsg )
{		
	CUIControl* pControl = uiman::GetInnerUIMan().GetFocusControl();	
	if ( pControl && pControl->GetWndID() != cID )
	{
		if ( !((CModalWindow*)pControl)->IsUseModaless() )
		{
			GASSERT ( 0 && "����, ��� ������ ���������� �ʽ��ϴ�." );
		}
	}
	UiHideGroup( cID, bMakeMsg );
}

void	CInnerInterface::UpdateStateSimpleHP ()
{
	if ( DXPARAMSET::GetInstance().m_bSHOW_SIMPLEHP )
	{
		if ( !UiIsVisibleGroup ( SIMPLE_HP ) )
			UiShowGroupBottom ( SIMPLE_HP );
	}
	else
	{
		if ( UiIsVisibleGroup ( SIMPLE_HP ) )
			UiHideGroup ( SIMPLE_HP );
	}

	if ( UiIsVisibleGroup ( SIMPLE_HP ) )
	{
		const long lResolution = uiman::GetInnerUIMan().GetResolution();
		WORD X_RES = HIWORD ( lResolution );
		WORD Y_RES = LOWORD ( lResolution );

		D3DXVECTOR3 vPos = m_pGaeaClient->GetCharacter()->GetPosition ();	

		//	NOTE
		//		������ ����
		{
			vPos.y -= 2.0f;
			D3DXVECTOR3 vScreen = DxViewPort::GetInstance().ComputeVec3Project ( &vPos, NULL );	

			const UIRECT& rcOriginPos = m_pSimpleHP->GetGlobalPos ();
			D3DXVECTOR2 vNewPos;
			vNewPos.x = (X_RES - rcOriginPos.sizeX) / 2.0f;
			vNewPos.y = ( vScreen.y - rcOriginPos.sizeY );

			if ( fabs ( rcOriginPos.left - vNewPos.x ) > 1.0f ||
				fabs ( rcOriginPos.top - vNewPos.y ) > 1.0f ) 

			m_pSimpleHP->SetGlobalPos ( vNewPos );			
		}
	}

	/*
	GLSUMMONCLIENTLIST& SummonList  = m_pGaeaClient->GetSummonClientList ();
	GLSUMMONCLIENTNODE* pSummonNode = SummonList.m_pHead;

	while (pSummonNode)
	{
		GLSummonClient* pSummon = pSummonNode->Data;


		if( pSummon && pSummon->IsVALID() )
		{
			if ( !UiIsVisibleGroup ( SUMMON_HP ) )
				UiShowGroupBottom ( SUMMON_HP );

			D3DXVECTOR3 vPos = pSummon->GetPosition();


			vPos.y -= 2.0f;
			D3DXVECTOR3 vScreen = DxViewPort::GetInstance().ComputeVec3Project ( &vPos, NULL );	

			const UIRECT& rcOriginPos = m_pSummonHP->GetGlobalPos ();
			D3DXVECTOR2 vNewPos;
			vNewPos.x = vScreen.x - (rcOriginPos.sizeX/2.0f);
			vNewPos.y = ( vScreen.y - rcOriginPos.sizeY );

			if ( fabs ( rcOriginPos.left - vNewPos.x ) > 1.0f ||
				fabs ( rcOriginPos.top - vNewPos.y ) > 1.0f ) 

			m_pSummonHP->SetGlobalPos ( vNewPos );			
		}

		pSummonNode = pSummonNode->pNext;
	}
	*/
}

void	CInnerInterface::SetLottoOpen ( const DWORD& dwGaeaID )
{
	std::string strCombine = sc::string::format( ID2GAMEINTEXT("UI_LOTTO"), "2004�� 7�� 2����" );	
	DoModal ( strCombine.c_str(), UI::MODAL_INPUT, MODAL_EDITBOX_LOTTERY, UI::MODAL_LOTTO );	
}

void	CInnerInterface::SetStorageChargeOpen ( const WORD& wPosX, const WORD& wPosY )
{
	if ( m_pStorageChargeCard )
	{
		UiShowGroupFocus ( STORAGE_CHARGE_CARD );
		m_pStorageChargeCard->InitStorageChargeCard ( wPosX, wPosY );
	}
}

void	CInnerInterface::SetItemBankInfo ()
{     
	if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
	{
		m_pInventory->CallTapSelect( CRnInventoryWindow::ETapBank );
		UiShowGroupFocus( INVENTORY_WINDOW_RENEW );
	}
	else
	{
		if( m_pItemBankWindow )
		{
			UiShowGroupFocus ( ITEMBANK_WINDOW );
			m_pItemBankWindow->InitItemBank ();
		}
	}
}

void	CInnerInterface::SetItemBankWindowOpen ()
{
	if( IsBankOpen() )
		return;

	if ( m_pGaeaClient->GetTutorial()->IsTutorial() ) // Ʃ�丮�� �߿��� item bank ���� ����.
	{
		m_pGaeaClient->PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("TUTORIAL_CANNOT_USE") );
		return;
	}

	if( m_fItemBankDelay >= 5.0f )
	{
		m_fItemBankDelay = 0.0f;
	}
	else
	{
		m_pGaeaClient->PrintMsgText ( NS_UITEXTCOLOR::NEGATIVE, ID2GAMEINTEXT("ITEM_BANK_DELAY") );
		return;
	}

	ClearItemBank ();

	// ���⼭ ������ ��������� ���� ��û
	GLCharacter* pCharacter = m_pGaeaClient->GetCharacter ();
	if (pCharacter)
		pCharacter->ReqItemBankInfo(false);

	SetItemBankInfo ();
}

void	CInnerInterface::SetVNGainSysInfo ()
{     
	if ( m_pVNGainSysInventory )
	{
		UiShowGroupFocus ( VNGAINSYS_WINDOW );
		m_pVNGainSysInventory->InitVNGainSys();
	}
}

void	CInnerInterface::SetVNGainSysWindowOpen ()
{
	if ( m_bFirstVNGainSysCall )
	{
		// ���⼭ ������ ��������� ���� ��û
		//GLCharacter* pCharacter = m_pGaeaClient->GetCharacter ();
		/*if ( pCharacter )
		{
			pCharacter->ReqVNGainSysInfo ();
		}*/
		m_bFirstVNGainSysCall = false;
	}

	SetVNGainSysInfo ();
}

void CInnerInterface::SetItemShopSearchWindowOpen()
{
    SetDefaultPosInterface( ITEM_SHOP_SEARCH_WINDOW );
    UiShowGroupFocus( ITEM_SHOP_SEARCH_WINDOW );
}

void CInnerInterface::SetItemShopSearchWindowClose()
{
    UiHideGroup( ITEM_SHOP_SEARCH_WINDOW );
}

void CInnerInterface::SetItemSearchResultWindowOpen( )
{
    UiShowGroupFocus( ITEM_SEARCH_RESULT_WINDOW );
}

void CInnerInterface::SetItemSearchResultWindowClose()
{
    UiHideGroup( ITEM_SEARCH_RESULT_WINDOW );
}

void CInnerInterface::OpenItemGarbageWindow()	// ������
{
    if ( UiIsVisibleGroup( ITEM_GARBAGE_WINDOW ) )
        return;

	if( !m_pItemGarbageWindow )
		return;

	CloseAllWindow();

    if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
	{
		m_pInventory->CallTapSelect( CRnInventoryWindow::ETapInven );
		UiShowGroupFocus ( INVENTORY_WINDOW_RENEW );
	}
	else
	{
		UiShowGroupFocus ( INVENTORY_WINDOW );
	}

	UIRECT rcInventoryWindow;
	if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
		rcInventoryWindow = m_pInventory->GetUIWindowGlobalPos();
	else
		rcInventoryWindow = m_pInventoryWindow->GetUIWindowGlobalPos();

	// ItemGarbage Window
	const UIRECT& rcGarbageWindow = m_pItemGarbageWindow->GetGlobalPos();

    D3DXVECTOR2 vPos;
    vPos.x = rcInventoryWindow.left - rcGarbageWindow.sizeX;
    vPos.y = rcInventoryWindow.top;
	m_pItemGarbageWindow->SetGlobalPos( vPos );

    if ( m_pGaeaClient->GetCharacter()->HaveHoldItem() )
    {
        WORD wInsertPosX, wInsertPosY;

        if ( m_pGaeaClient->GetCharacter()->m_cInventory.FindInsertable( wInsertPosX, wInsertPosY ) )
            m_pGaeaClient->GetCharacter()->ReqInvenTo( wInsertPosX, wInsertPosY );
    }

	UiShowGroupFocus( ITEM_GARBAGE_WINDOW );

	m_pGaeaClient->GetCharacter()->ReqGarbageOpen();
}

void CInnerInterface::CloseItemGarbageWindow()
{
	UiHideGroup( ITEM_GARBAGE_WINDOW );

	m_pGaeaClient->GetCharacter()->ReqGarbageClose();
}

void CInnerInterface::CheckItemGarbageWindowClose()
{
    unsigned int nNumGarbageItem = m_pGaeaClient->GetCharacter()->GetCountItemInInven( ITEM_GARBAGE_CARD );

    if ( nNumGarbageItem == 0 )
	{
        CloseItemGarbageWindow();
	}
    else
    {
        WORD wPosX = USHRT_MAX;
        WORD wPosY = USHRT_MAX;
        m_pGaeaClient->GetCharacter()->GetItemPosInInven( ITEM_GARBAGE_CARD, wPosX, wPosY );

        if( wPosX != USHRT_MAX && wPosY != USHRT_MAX )
            m_pGaeaClient->GetCharacter()->ReqGabargeOpenCard( wPosX, wPosY );
        else
            CloseItemGarbageWindow();
    }
}

void	CInnerInterface::REFRESH_ITEMBANK ()
{
	if ( m_pItemBankWindow )
	{		
		m_pItemBankWindow->InitItemBank ();
	}
}

void	CInnerInterface::REFRESH_VNGAINSYSTEM ()
{
	if ( m_pVNGainSysInventory )
	{		
		m_pVNGainSysInventory->InitVNGainSys();
	}
}

void CInnerInterface::SetInventorySlotViewSize(const int& nSLOT_VIEW_SIZE)
{
	if ( m_pInventoryWindow )
	{
		m_pInventoryWindow->SetOneViewSlot ( nSLOT_VIEW_SIZE );
		m_pTradeInventoryWindow->SetOneViewSlot ( nSLOT_VIEW_SIZE );
	}
}

const bool CInnerInterface::IsInventoryLocked ()
{
	if( m_pInventory )
		return m_pInventory->IsLocked();

    if( m_pInventoryWindow )
        return m_pInventoryWindow->IsLocked();

    return true;
}

HRESULT CInnerInterface::FinalCleanup ()
{
    m_mmManageControl.clear();

	HRESULT hr = S_OK;
	hr = uiman::GetInnerUIMan().FinalCleanup();
	if ( FAILED ( hr ) ) return hr;

	return S_OK;
}

void CInnerInterface::SetPrivateMarketOpen( const bool& bOPENER, const DWORD& dwGaeaID )
{
	if ( UiIsVisibleGroup( PRIVATE_MARKET_WINDOW ) )
		return;

	if ( !m_pPrivateMarketWindow )
		return;

	m_pPrivateMarketWindow->InitPrivateMarket( bOPENER, dwGaeaID );

	if( m_pTradeInventoryWindow )
		m_pTradeInventoryWindow->SetOpener( bOPENER );

	CloseAllWindow();

	const long lResolution = uiman::GetInnerUIMan().GetResolution();
	WORD X_RES = HIWORD ( lResolution );
	WORD Y_RES = LOWORD ( lResolution );

	{
		const UIRECT& rcPrivateMarketWindow = m_pPrivateMarketWindow->GetGlobalPos();

		D3DXVECTOR2 vPos;
		vPos.x = ((X_RES) / 2.0f) - rcPrivateMarketWindow.sizeX;
		vPos.y = (Y_RES - rcPrivateMarketWindow.sizeY) / 2.0f;
		m_pPrivateMarketWindow->SetGlobalPos ( vPos );

		UiShowGroupFocus( PRIVATE_MARKET_WINDOW );
	}

	if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
	{
		m_pInventory->CallTapSelect( CRnInventoryWindow::ETapInven );
		UiShowGroupFocus( INVENTORY_WINDOW_RENEW );
	}
	else
	{
		const UIRECT& rcTradeInventoryWindow = GetTradeInventoryWindow()->GetUIWindowGlobalPos();

		D3DXVECTOR2 vPos;
		vPos.x = (X_RES) / 2.0f;
		vPos.y = (Y_RES - rcTradeInventoryWindow.sizeY) / 2.0f;
		GetTradeInventoryWindow()->SetUIWindowGlobalPos( vPos );

		UiShowGroupFocus( TRADEINVENTORY_WINDOW );
	}
}

void CInnerInterface::GetPrivateMarketInfo ( bool& bOPENER, DWORD& dwGaeaID )
{	
	bOPENER = m_pPrivateMarketWindow->IsOPENER ();
	dwGaeaID = m_pPrivateMarketWindow->GetPrivateMarketID ();;
}

void CInnerInterface::SetPrivateMarketClose ()
{
	UiHideGroup ( PRIVATE_MARKET_WINDOW );

	if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
		UiHideGroup ( INVENTORY_WINDOW_RENEW );
	else
		UiHideGroup ( TRADEINVENTORY_WINDOW );

	if ( m_pPrivateMarketWindow->IsOPENER () )
		m_pGaeaClient->GetCharacter()->ReqPMarketClose ();
	else
		m_pGaeaClient->GetCharacter()->ReqPMarketInfoRelease ( m_pPrivateMarketWindow->GetPrivateMarketID () );
}

void CInnerInterface::MODAL_PRIVATE_MARKET_SELLITEM ( const bool& bCOUNTABLE, const WORD& wPosX, const WORD& wPosY )
{
	if ( m_pPrivateMarketSellWindow )
	{
		m_pPrivateMarketSellWindow->SetType ( bCOUNTABLE );
		m_pPrivateMarketSellWindow->SetCallerID ( UI::MODAL_PRIVATE_MARKET_SELL );
		m_pPrivateMarketSellWindow->SetItemIndex ( wPosX, wPosY );
		UiShowGroupFocus ( PRIVATE_MARKET_SELL_WINDOW, true );
	}
}

const	CString& CInnerInterface::GetPrivateMarketTitle ()
{
	return m_pPrivateMarketMake->GetTITLE ();
}

void	CInnerInterface::SetPrivateMarketMake ()
{	
	if( !UiIsVisibleGroup ( PRIVATE_MARKET_MAKE ) )
	{
		if ( m_pPrivateMarketMake )
		{
			m_pPrivateMarketMake->Init ();
		}
	}

	UiShowGroupFocus ( PRIVATE_MARKET_MAKE, true ); // 
}

void	CInnerInterface::SetClubMake ( const DWORD& dwNpcGlobID )
{
	if ( !UiIsVisibleGroup ( CLUB_MAKE ) )
	{
		if ( m_pClubMake )
		{
			m_pClubMake->Init ( dwNpcGlobID );
			UiShowGroupFocus ( CLUB_MAKE, true );
		}
	}	
}

CString CInnerInterface::GetdwKeyToString( int dwKey )
{
    return UI::ToString( GetKeySettingWindow()->GetdwKeyToString( dwKey ) );
}

void 	CInnerInterface::SetShotcutText ( DWORD nID, CString& strTemp )
{
	if (nID < QUICK_SLOT_NUM )
	{
		m_pUILeftTopGroup->SetShotcutText( nID, UI::ToString( strTemp ) );
	}
	else if( nID >= QUICK_SLOT_NUM && nID < (QUICK_SLOT_NUM + QUICK_SKILL_NUM) )
	{
		m_pSkillTrayTab->SetShotcutText( nID - ( QUICK_SLOT_NUM) , UI::ToString( strTemp ) );
	}
	else if( nID >= (QUICK_SLOT_NUM + QUICK_SKILL_NUM) && 
			  nID < (QUICK_SLOT_NUM + QUICK_SKILL_NUM + MENU_SHOTCUT_NUM ))
	{
		int ranIndex = RANPARAM::KeySettingToRanparam[nID- (QUICK_SLOT_NUM + QUICK_SKILL_NUM) ];		
		int menuIndex;
		
		for (int i = 0; i < BASIC_MENU_NUM; ++i)
		{
			menuIndex = RANPARAM::BasicMenuToRanparam[i];
			if( ranIndex == menuIndex )
			{
				m_pGameMenu->SetShotcutText( i , strTemp);
				return;
			}
		}		
	}
}

void CInnerInterface::GetShotCutKey(){
	m_pKeySettingWindow->GetShotCutKey();
}

void CInnerInterface::GetChatMacro()
{
	// [shhan][2014.11.13] WorldEdit ���� ���������� ���� �˻縦 ��.
	if ( m_pChatMacroWindow )
		m_pChatMacroWindow->GetChatMacro();
}

void CInnerInterface::AddChatMacro(int nIndex)
{
	bool bincmd(false);
	bincmd = m_pGaeaClient->ExecuteCommand(UI::ToString( RANPARAM::ChatMacro[nIndex] ));
	if (!bincmd)
		GetBasicChatWindow()->AddChatMacro( UI::ToString( RANPARAM::ChatMacro[nIndex] ) );
}

void CInnerInterface::ChatLog( bool bChatLogt, int nChatLogType )
{
	GetBasicChatWindow()->ChatLog( bChatLogt, nChatLogType );
}

CString CInnerInterface::GET_RECORD_CHAT()
{
	if( !GetBasicChatWindow() ) return NULL;
	return UI::ToString( GetBasicChatWindow()->GetRecordChat() );
}

void CInnerInterface::DOMODAL_CLUB_JOIN_ASK ( const DWORD& dwMasterID, const CString& strClubName, const CString& strClubMaster )
{
	m_dwClubMasterID = dwMasterID;
	
	// ��ʸ�Ʈ�� Ŭ�� �Ҽ��� ����.
	// TOURNAMENT_UNIQ_DISABLE
	if( m_pGaeaClient->GetTournamentClient()->GetIsTournamentView() == true)
		return;

	std::string strCombine = sc::string::format( ID2GAMEINTEXT("CLUB_JOIN_ASK"), strClubName, strClubMaster );
	DoModal ( strCombine, UI::MODAL_QUESTION, UI::YESNO, UI::MODAL_CLUB_JOIN_ASK );
}

void CInnerInterface::DOMODAL_UNION_JOIN_ASK ( const DWORD& dwMasterID, const CString& strClubMaster )
{
	m_dwClubMasterID = dwMasterID;

	std::string strCombine = sc::string::format( ID2GAMEINTEXT("UNION_JOIN_ASK"), strClubMaster );
	DoModal ( strCombine, UI::MODAL_QUESTION, UI::YESNO, UI::MODAL_UNION_JOIN_ASK );
}

void CInnerInterface::DOMODAL_CLUB_BATTLE_ASK ( const DWORD& dwMasterID, const CString& strClubMaster, DWORD dwBattleTime, bool bAlliance )
{
	m_dwClubMasterID = dwMasterID;

	if ( bAlliance ) 
	{
		std::string strCombine  = sc::string::format( ID2GAMEINTEXT("ALLIANCE_BATTLE_ASK"), strClubMaster, dwBattleTime/60, dwBattleTime%60 );
		DoModal ( strCombine.c_str(), UI::MODAL_QUESTION, UI::YESNO, UI::MODAL_ALLIANCE_BATTLE_ASK );
	}
	else
	{
		std::string strCombine  = sc::string::format( ID2GAMEINTEXT("CLUB_BATTLE_ASK"), strClubMaster, dwBattleTime/60, dwBattleTime%60 );
		DoModal ( strCombine.c_str(), UI::MODAL_QUESTION, UI::YESNO, UI::MODAL_CLUB_BATTLE_ASK );
	}
}

void CInnerInterface::DOMODAL_CLUB_AUTHORITY_ASK( const CString& strClubName )
{
	std::string strCombine = sc::string::format( ID2GAMEINTEXT("CLUB_AUTHORITY_MEMBER_ASK"), strClubName );
	DoModal ( strCombine, UI::MODAL_QUESTION, UI::YESNO, UI::MODAL_CLUB_AUTHORITY_ASK );
}

void CInnerInterface::DOMODAL_CLUB_BATTLE_ARMISTICE_ASK(DWORD dwClubID, DWORD ReqChaDbNum, const CString& strClubName, bool bAlliance)
{
	// �ӽ� ����
	m_dwClubMasterID = dwClubID;
    m_ReqChaDbNum = ReqChaDbNum;
	std::string strCombine;
	
	if ( bAlliance ) 
	{
		strCombine = sc::string::format( ID2GAMEINTEXT("ALLIANCE_BATTLE_ARMISTICE_ASK"), strClubName );
		DoModal ( strCombine, UI::MODAL_QUESTION, UI::YESNO, UI::MODAL_ALLIANCE_BATTLE_ARMISTICE_ASK );
	}
	else 
	{
		strCombine = sc::string::format( ID2GAMEINTEXT("CLUB_BATTLE_ARMISTICE_ASK"), strClubName );
		DoModal ( strCombine, UI::MODAL_QUESTION, UI::YESNO, UI::MODAL_CLUB_BATTLE_ARMISTICE_ASK );
	}
}

bool CInnerInterface::SET_QUESTION_ITEM_ID ( int nID )
{
	UIGUID cID = NO_ID;
	switch ( nID )
	{
	case QUESTION_SPEED_UP:		cID = CQuestionItemDisplay::SPEED_UP;		break;
	case QUESTION_CRAZY:		cID = CQuestionItemDisplay::CRAZY_TIME;		break;
	case QUESTION_ATTACK_UP:	cID = CQuestionItemDisplay::POWER_UP;		break;
	case QUESTION_EXP_UP:		cID = CQuestionItemDisplay::EXP_TIME;		break;
	case QUESTION_EXP_GET:		cID = CQuestionItemDisplay::EXP_GET;		break;
	case QUESTION_LUCKY:		cID = CQuestionItemDisplay::LUCKY;			break;
	case QUESTION_BOMB:			cID = CQuestionItemDisplay::BOMB;			break;
	case QUESTION_MOBGEN:		cID = CQuestionItemDisplay::OH_NO;			break;
	case QUESTION_SPEED_UP_M:	cID = CQuestionItemDisplay::SPEED_UP_M;		break;
	case QUESTION_MADNESS:		cID = CQuestionItemDisplay::MADNESS;		break;
	case QUESTION_ATTACK_UP_M:	cID = CQuestionItemDisplay::ATTACK_UP_M;	break;
	case QUESTION_HEAL:			cID = CQuestionItemDisplay::HEAL;			break;
	}

	if ( cID == NO_ID ) return false;

	UiShowGroupTop ( QUESTION_ITEM_DISPLAY );
	m_pQuestionItemDisplay->START ( cID );

	return true;
}

bool	CInnerInterface::SET_KEEP_QUESTION_ITEM_ID ( int nID )
{
	UIGUID cID = NO_ID;
	switch ( nID )
	{
	case QUESTION_SPEED_UP:		cID = CQuestionItemDisplay::SPEED_UP;		break;
	case QUESTION_CRAZY:		cID = CQuestionItemDisplay::CRAZY_TIME;		break;
	case QUESTION_ATTACK_UP:	cID = CQuestionItemDisplay::POWER_UP;		break;
	case QUESTION_EXP_UP:		cID = CQuestionItemDisplay::EXP_TIME;		break;
	case QUESTION_EXP_GET:		cID = CQuestionItemDisplay::EXP_GET;		break;
	case QUESTION_LUCKY:		cID = CQuestionItemDisplay::LUCKY;			break;
	case QUESTION_BOMB:			cID = CQuestionItemDisplay::BOMB;			break;
	case QUESTION_MOBGEN:		cID = CQuestionItemDisplay::OH_NO;			break;
	case QUESTION_SPEED_UP_M:	cID = CQuestionItemDisplay::SPEED_UP_M;		break;
	case QUESTION_MADNESS:		cID = CQuestionItemDisplay::MADNESS;		break;
	case QUESTION_ATTACK_UP_M:	cID = CQuestionItemDisplay::ATTACK_UP_M;	break;
	case QUESTION_HEAL:			cID = CQuestionItemDisplay::HEAL;			break;
	}

	if ( cID == NO_ID ) return false;

	RESET_KEEP_QUESTION_ITEM();

	UiShowGroupTop ( QUESTION_ITEM_DISPLAY );
	m_pQuestionItemDisplay->KEEP_START ( cID );

	if ( m_pQBoxButton )
		m_pQBoxButton->SetQBoxType( nID ); // by luxes.

	return true;
}

void	CInnerInterface::RESET_KEEP_QUESTION_ITEM ()
{
	if( m_pQuestionItemDisplay )
	{
		m_pQuestionItemDisplay->KEEP_STOP ();
	}

    if ( m_pQBoxButton ) 
    {
	    if ( m_pQBoxButton->GetQBoxType() != QBOX_NONE )
		    m_pQBoxButton->ResetQBox(); // by luxes.
    }
}

void	CInnerInterface::SET_CONFT_CONFIRM_VISIBLE ( bool bVisible )
{
	if ( bVisible )	UiShowGroupTop( CONFT_CONFIRM );
	else			UiHideGroup( CONFT_CONFIRM );
}

void	CInnerInterface::SET_CONFT_CONFIRM_PERCENT ( float fPercent )
{
	if( m_pConftConfirm )
	{
		m_pConftConfirm->SetPercent ( fPercent );
	}
}

HRESULT CInnerInterface::InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	
    MyFactoryUI cFactory;

	TEXTURE_PRE_LOAD ( pd3dDevice );

    // �Ʒ����� �����츦 ������ �� UiShowGroup �ϴµ�,
    // �ε� ȭ�鿡�� �ѹ� �� ��Ʈ���� ��¦�̴� ������ �߻��Ѵ�
    // �׷��� �� ���������� �� ��Ʈ���� ������ �ʰ� �Ѵ�
    CWebWindowBase::s_bSkipVisibleWeb = TRUE;

	EMSERVICE_PROVIDER sp = GLogicData::GetInstance().GetServiceProvider();

    {
        AP_IPointShopWindow apPointShopWindow = cFactory.CreatePointShopWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pPointShopWindow = apPointShopWindow.release();
        m_pPointShopWindow->CreateUIWindowAndRegisterOwnership();
    }

	{
        AP_IKeySettingWindow apKeySettingWindow = cFactory.CreateKeySettingWindow( this, m_pEngineDevice );
        m_pKeySettingWindow = apKeySettingWindow.release();
        m_pKeySettingWindow->CreateUIWindowAndRegisterOwnership();
	}

	{
        AP_IChatMacroWindow apChatMacroWindow = cFactory.CreateChatMacroWindow( this, m_pEngineDevice );
        m_pChatMacroWindow = apChatMacroWindow.release();
        m_pChatMacroWindow->CreateUIWindowAndRegisterOwnership();
	}

	{
        AP_INameDisplayMan apNameDisplayMan = cFactory.CreateNameDisplayMan( m_pGaeaClient, this, m_pEngineDevice );
        m_pNameDisplayMan = apNameDisplayMan.release();
        m_pNameDisplayMan->CreateUIWindowAndRegisterOwnership();

        AP_IPrivateMarketShowMan apPrivateMarketShowMan = cFactory.CreatePrivateMarketShowMan( m_pGaeaClient, this, m_pEngineDevice );
        m_pPrivateMarketShowMan = apPrivateMarketShowMan.release();
        m_pPrivateMarketShowMan->CreateUIWindowAndRegisterOwnership();

        AP_ISummonNameDisplayMan apSummonNameDisplayMan = cFactory.CreateSummonNameDisplayMan( m_pGaeaClient, this, m_pEngineDevice );
        m_pSummonNameDisplayMan = apSummonNameDisplayMan.release();
        m_pSummonNameDisplayMan->CreateUIWindowAndRegisterOwnership();
	}

	{
        AP_ITargetInfoDisplay apTargetInfoDisplay = cFactory.CreateTargetInfoDisplay( m_pGaeaClient, this, m_pEngineDevice );
        m_pTargetInfoDisplay = apTargetInfoDisplay.release();
        m_pTargetInfoDisplay->CreateUIWindowAndRegisterOwnership();
	}

    {
        AP_IInfoCTFDisplay apInfoCTFDisplay = cFactory.CreateInfoCTFDisplay( m_pGaeaClient, this, m_pEngineDevice );
        m_pInfoCTFDisplay = apInfoCTFDisplay.release();
        m_pInfoCTFDisplay->CreateUIWindowAndRegisterOwnership();
    }

    {
        AP_IPopupMenu apPopupMenu = cFactory.CreatePopupMenu( m_pGaeaClient, this, m_pEngineDevice );
        m_pPopupMenu = apPopupMenu.release();
        m_pPopupMenu->CreateUIWindowAndRegisterOwnership();
    }

    {
        AP_ICTFResultWindow apCTFResultWindow = cFactory.CreateCTFResultWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pCTFResultWindow = apCTFResultWindow.release();
        m_pCTFResultWindow->CreateUIWindowAndRegisterOwnership();
    }

    {
        m_pNotifyCTFSpurt = new CNotifyCTFSpurt( m_pGaeaClient, this, m_pEngineDevice );
        m_pNotifyCTFSpurt->Create( NOTIFY_CTF_SPURT, "NOTIFY_CTF_SPURT" );
        m_pNotifyCTFSpurt->CreateSubControl();
        UiRegisterControl( m_pNotifyCTFSpurt );
        UiShowGroupTop( NOTIFY_CTF_SPURT );
    }

	{
		m_pDamageDisplayMan = new CDamageDisplayMan(m_pEngineDevice);
		m_pDamageDisplayMan->Create ( DAMAGE_MAN, "DAMAGE_MAN" );
		m_pDamageDisplayMan->CreateSubControl ();
		UiRegisterControl ( m_pDamageDisplayMan );
		UiShowGroupBottom ( DAMAGE_MAN );
	}

	{
        AP_IHeadChatDisplayMan apHeadChatDisplayMan = cFactory.CreateHeadChatDisplayMan( m_pGaeaClient, this, m_pEngineDevice );
        m_pHeadChatDisplayMan = apHeadChatDisplayMan.release();
        m_pHeadChatDisplayMan->CreateUIWindowAndRegisterOwnership();
	}

	{
        AP_IUILeftTopGroup apUILeftTopGroup = cFactory.CreateUILeftTopGroup( m_pGaeaClient, this, m_pEngineDevice );
        m_pUILeftTopGroup = apUILeftTopGroup.release();
        m_pUILeftTopGroup->CreateUIWindowAndRegisterOwnership();
	}

	{        
        DxGlobalStage* pGlobalStage = m_pGaeaClient->GetGlobalStage();
        const DxCharJoinData* pJoinData = pGlobalStage->GetGameStage()->GetCharJoinData();
		const EMCHARCLASS emCharClass = pJoinData->m_CharData2.m_emClass;
		const int nClassType = CharClassToIndex(emCharClass);

		CSkillTimeDisplay* pSkillTimeDisplay = new CSkillTimeDisplay ( m_pGaeaClient, this, m_pEngineDevice );

		// �ذ��ο� �Ϲ� ĳ���͸� �����ؼ� ����UI ��ġ ����
		if( nClassType == GLCI_EXTREME_W || nClassType == GLCI_EXTREME_M )
		{
			pSkillTimeDisplay->Create ( SKILL_TIME_DISPLAY, "SKILL_TIME_DISPLAY_EXTREME" );
		}
		// �ذ��θ� �����ϰ��� ������ ��ġ�� ����Ѵ�;
		else
		{
			pSkillTimeDisplay->Create ( SKILL_TIME_DISPLAY, "SKILL_TIME_DISPLAY" );
		}
		
		pSkillTimeDisplay->CreateSubControl ();
		UiRegisterControl ( pSkillTimeDisplay );
		m_pSkillTimeDisplay = pSkillTimeDisplay;
		UiShowGroupBottom ( SKILL_TIME_DISPLAY );

		// ���ö� (���Ĺ��� UI)
        AP_ILunchBoxTimeDisplay apLunchBoxTimeDisplay = cFactory.CreateLunchBoxTimeDisplay( m_pGaeaClient, this, m_pEngineDevice );
        m_pLunchBoxTimeDisplay = apLunchBoxTimeDisplay.release();
        m_pLunchBoxTimeDisplay->CreateUIWindowAndRegisterOwnership();

        // �ý��۹��� UI
        AP_ISystemBuffTimeDisplay apSystemBuffTimeDisplay = cFactory.CreateSystemBuffTimeDisplay( m_pGaeaClient, this, m_pEngineDevice );
        m_pSystemBuffTimeDisplay = apSystemBuffTimeDisplay.release();
        m_pSystemBuffTimeDisplay->CreateUIWindowAndRegisterOwnership();

		// �ذ��� ���� ǥ�� UI 
		if( nClassType == GLCI_EXTREME_W || nClassType == GLCI_EXTREME_M )
		{
			CWeaponDisplay* pWeaponDisplay = new CWeaponDisplay ( m_pGaeaClient, this, m_pEngineDevice );
			pWeaponDisplay->Create ( WEAPON_DISPLAY, "WEAPON_DISPLAY" );
			pWeaponDisplay->CreateSubControl ();
			UiRegisterControl ( pWeaponDisplay );
			UiShowGroupBottom ( WEAPON_DISPLAY );
		}

		{
			CCDMSafeTimeDisplay* pCDMSafeTimeDisplay = new CCDMSafeTimeDisplay ( m_pGaeaClient, this, m_pEngineDevice );

			if( nClassType == GLCI_EXTREME_W || nClassType == GLCI_EXTREME_M )
			{
				pCDMSafeTimeDisplay->Create ( CDM_SAFETIME_DISPLAY, "CDM_SAFETIME_DISPLAY_EXTREME" );
			}
			else
			{
				pCDMSafeTimeDisplay->Create ( CDM_SAFETIME_DISPLAY, "CDM_SAFETIME_DISPLAY" );
			}
			pCDMSafeTimeDisplay->CreateSubControl ();
			UiRegisterControl ( pCDMSafeTimeDisplay );
			m_pCdmSafeTimeDisplay = pCDMSafeTimeDisplay;
			UiShowGroupBottom ( CDM_SAFETIME_DISPLAY );
			UiHideGroup( CDM_SAFETIME_DISPLAY );

		}

	}

	{
        AP_IMiniMap apMiniMap = cFactory.CreateMiniMap( m_pGaeaClient, this, m_pEngineDevice );
        m_pMiniMap = apMiniMap.release();
        m_pMiniMap->CreateUIWindowAndRegisterOwnership();     
	}

	{
        AP_IQuestHelper apQuestHelper = cFactory.CreateQuestHelper( m_pGaeaClient, this, m_pEngineDevice );
        m_pQuestHelper = apQuestHelper.release();
        m_pQuestHelper->CreateUIWindowAndRegisterOwnership();
	}

	{
		m_pCdmRankingDisplay = new CCdmRankingDisplay ( m_pGaeaClient, m_pEngineDevice );
		m_pCdmRankingDisplay->Create ( CDM_RANKING_DISPLAY, "CDM_RANKING_DISPLAY", UI_FLAG_RIGHT );
		m_pCdmRankingDisplay->CreateSubControl ();
		UiRegisterControl ( m_pCdmRankingDisplay );
		UiShowGroupBottom ( CDM_RANKING_DISPLAY );	
		UiHideGroup( CDM_RANKING_DISPLAY );

	}

	{
		m_pRankingDisplay = new CRankingDisplay ( this, m_pGaeaClient, m_pEngineDevice );
		m_pRankingDisplay->Create ( RANKING_DISPLAY, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE  );
		m_pRankingDisplay->CreateBaseWindowLightGray ( "RANKING_DISPLAY", "Ranking Window" );
		m_pRankingDisplay->CreateSubControl ();
		m_pRankingDisplay->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		UiRegisterControl ( m_pRankingDisplay, true );
		UiShowGroupBottom ( RANKING_DISPLAY );	
		UiHideGroup( RANKING_DISPLAY );
	}
	
	{
		m_pPKRankNotification = new CPKRankNotification  ( m_pGaeaClient, this, m_pEngineDevice );
		m_pPKRankNotification->Create ( PK_NOTIFICATION, "PK_RANK_NOTIFICATION", UI_FLAG_RIGHT );
		m_pPKRankNotification->CreateSubControl ();
		UiRegisterControl ( m_pPKRankNotification );
		UiShowGroupBottom ( PK_NOTIFICATION );
	}
	
	{
		CEscMenuOpen* pEscMenuOpen = new CEscMenuOpen ( this, m_pEngineDevice );
		pEscMenuOpen->Create ( ESCMENU_OPEN, "ESCMENU_OPEN", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		pEscMenuOpen->CreateSubControl ();
		UiRegisterControl ( pEscMenuOpen );		
		UiShowGroupBottom ( ESCMENU_OPEN );

		CBasicButton* pMiniPartyOpen = new CBasicButton(m_pEngineDevice);
		pMiniPartyOpen->Create ( MINIPARTY_OPEN, "MINIPARTY_OPEN", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
//#ifdef CH_PARAM // �߱� �������̽� ����
//		pMiniPartyOpen->CreateMouseOver( "MINIPARTY_OPEN_F" );
//#else
		//  [12/28/2012 gbgim]
		//pMiniPartyOpen->CreateFlip ( "MINIPARTY_OPEN_F", CBasicButton::CLICK_FLIP );
		pMiniPartyOpen->SetUseDynamic ( TRUE );
//#endif
		pMiniPartyOpen->SetUseGlobalAction ( TRUE );
		UiRegisterControl ( pMiniPartyOpen );
		UiShowGroupBottom ( MINIPARTY_OPEN );

		if ( !GLUseFeatures::GetInstance().IsUsingWorldBattle() )
		{
			m_pQuestAlarm = new CQuestAlarm(m_pEngineDevice);
			m_pQuestAlarm->Create ( QUEST_ALARM, "QUEST_ALARM", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
			m_pQuestAlarm->CreateSubControl ();
			UiRegisterControl ( m_pQuestAlarm );
			UiShowGroupBottom ( QUEST_ALARM );
			UiHideGroup ( QUEST_ALARM );

			m_pQBoxButton = new CQBoxButton ( m_pGaeaClient, this, m_pEngineDevice );
			m_pQBoxButton->Create ( QBOX_BUTTON, "QBOX_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
			m_pQBoxButton->CreateSubControl ();
			UiRegisterControl ( m_pQBoxButton );		
			UiShowGroupBottom ( QBOX_BUTTON );

			AP_IPostNotifyButton apPostNotifyButton = cFactory.CreatePostNotifyButton( m_pGaeaClient, this, m_pEngineDevice );
			m_pPostNotifyButton = apPostNotifyButton.release();
			m_pPostNotifyButton->CreateUIWindowAndRegisterOwnership();

			m_pStudentRecordNotifyButton = new CStudentRecordNotifyButton( m_pGaeaClient, this, m_pEngineDevice );
			m_pStudentRecordNotifyButton->Create ( STUDENTRECORD_NOTIFY_BUTTON, "STUDENTRECORD_NOTIFY_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
			m_pStudentRecordNotifyButton->CreateSubControl ();
			UiRegisterControl ( m_pStudentRecordNotifyButton );
			UiShowGroupBottom ( STUDENTRECORD_NOTIFY_BUTTON );

			// Lotto System
			if( GLUseFeatures::GetInstance().IsUsingLottoSystem() )
			{
				AP_ILottoNotifyButton apLottoNotifyButton = cFactory.CreateLottoNotifyButton( this, m_pEngineDevice );
				m_pLottoNotifyButton = apLottoNotifyButton.release();
				m_pLottoNotifyButton->CreateUIWindowAndRegisterOwnership();
			}

			if( GLUseFeatures::GetInstance().IsUsingRenewProduct() )
			{
				AP_IProductButton apProductButton = cFactory.CreateProductButton( m_pGaeaClient, this, m_pEngineDevice );
				m_pProductButton = apProductButton.release();
				m_pProductButton->CreateUIWindowAndRegisterOwnership();
			}

			AP_ICompetitionNotifyButton apCompetitionNotifyButton = cFactory.CreateCompetitionNotifyButton( m_pGaeaClient, this, m_pEngineDevice );
			m_pCompetitionNotifyButton = apCompetitionNotifyButton.release();
			m_pCompetitionNotifyButton->CreateUIWindowAndRegisterOwnership();
		}

		

		// World Battle System
		if ( GLUseFeatures::GetInstance().IsUsingJoinWorldBattle() ||
			GLUseFeatures::GetInstance().IsUsingWorldBattle() )
		{
			AP_IWorldBattleButton apWorldBattleButton = cFactory.CreateWorldBattleButton( m_pGaeaClient, this, m_pEngineDevice );
			m_pWorldBattleButton = apWorldBattleButton.release();

			if ( m_pWorldBattleButton )
				m_pWorldBattleButton->CreateUIWindowAndRegisterOwnership();
		}

		
        if( GLUseFeatures::GetInstance().IsUsingNotifyWindow() )
        {
            {
                CNotifyButton* pNotifyButton = new CNotifyButton( m_pGaeaClient, this, m_pEngineDevice );

				if ( GLUseFeatures::GetInstance().IsUsingWorldBattle() )
					pNotifyButton->Create( NOTIFY_BUTTON, "NOTIFY_BUTTON_USING_WORLDBATTLE_SYSTEM", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
				else if ( GLUseFeatures::GetInstance().IsUsingLottoSystem() )
					pNotifyButton->Create( NOTIFY_BUTTON, "NOTIFY_BUTTON_USING_LOTTO_SYSTEM", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
				else
					pNotifyButton->Create( NOTIFY_BUTTON, "NOTIFY_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );

                pNotifyButton->CreateSubControl();
                UiRegisterControl( pNotifyButton );
                UiShowGroupBottom( NOTIFY_BUTTON );
                m_pNotifyButton = pNotifyButton;
            }

            {
                CNotifyWindow* pNotify = new CNotifyWindow( m_pGaeaClient, this, m_pEngineDevice );
                pNotify->Create( NOTIFY_WINDOW, "NOTIFY_MESSAGE", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
                pNotify->CreateSubControl();
                UiRegisterControl( pNotify );
                UiShowGroupBottom( NOTIFY_WINDOW );
                UiHideGroup( NOTIFY_WINDOW );
                m_pNotifyWindow = pNotify;
            }
        }

        m_pPetHungry = new CPetHungry ( m_pGaeaClient, this, m_pEngineDevice );
		if ( GLUseFeatures::GetInstance().IsUsingWorldBattle() )
			m_pPetHungry->Create ( PET_HUNGRY, "PET_HUNGRY_ICON_BOX_USING_WORLDBATTLE_SYSTEM", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		else if ( GLUseFeatures::GetInstance().IsUsingLottoSystem() )
			m_pPetHungry->Create ( PET_HUNGRY, "PET_HUNGRY_ICON_BOX_USING_LOTTO_SYSTEM", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		else
			m_pPetHungry->Create ( PET_HUNGRY, "PET_HUNGRY_ICON_BOX", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
        m_pPetHungry->CreateSubControl ();
        UiRegisterControl ( m_pPetHungry );		
        UiShowGroupBottom ( PET_HUNGRY );


// #ifndef CH_PARAM // �߱� �������̽� ����
		CBasicButton* pGameMenuOpen = new CBasicButton(m_pEngineDevice);
		pGameMenuOpen->Create ( GAME_MENU_OPEN_BUTTON, "GAME_MENU_OPEN_BUTTON", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		pGameMenuOpen->CreateFlip ( "GAME_MENU_OPEN_BUTTON_F", CBasicButton::MOUSEIN_FLIP );
		pGameMenuOpen->SetUseGlobalAction ( TRUE );
		UiRegisterControl ( pGameMenuOpen );
		UiShowGroupBottom ( GAME_MENU_OPEN_BUTTON );
		UiHideGroup( GAME_MENU_OPEN_BUTTON );
// #endif

		m_pGameMenu = new CBasicGameMenu ( m_pGaeaClient, this, m_pEngineDevice );
		m_pGameMenu->Create ( GAME_MENU, "GAME_MENU", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pGameMenu->CreateSubControl ();
		UiRegisterControl ( m_pGameMenu );
		UiShowGroupBottom ( GAME_MENU );
		{
			// ���� ĳ������ �޸��� ��带 �����Ѵ�.
			BOOL bRun = m_pGaeaClient->GetCharacter()->IsActState(EM_ACT_RUN);
			m_pGameMenu->SetFlipRunButton( bRun );
		}
	}

	{
        /*
        AP_IBasicQuickSkillSlot apBasicQuickSkillSlot = cFactory.CreateBasicQuickSkillSlot( m_pGaeaClient, this );
        m_pBasicQuickSkillSlot = apBasicQuickSkillSlot.release();
        m_pBasicQuickSkillSlot->CreateUIWindowAndRegisterOwnership();
        */
        /*
// #ifndef CH_PARAM // �߱� �������̽� ����
		CBasicButton* pButton = new CBasicButton(m_pRenderDevice);
		pButton->Create ( QUICK_SKILL_TRAY_OPEN_BUTTON, "QUICK_SKILL_TRAY_OPEN_BUTTON" );
		pButton->CreateFlip ( "QUICK_SKILL_TRAY_OPEN_BUTTON_F", CBasicButton::MOUSEIN_FLIP );
		pButton->SetUseGlobalAction ( TRUE );
		UiRegisterControl ( pButton );
		UiShowGroupBottom ( QUICK_SKILL_TRAY_OPEN_BUTTON );
		UiHideGroup ( QUICK_SKILL_TRAY_OPEN_BUTTON );
//#endif
        */
	}

	{
        AP_IBasicChatWindow apBasicChat = cFactory.CreateBasicChatWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pBasicChat = apBasicChat.release();
        m_pBasicChat->CreateUIWindowAndRegisterOwnership();
		
		//	ä��â �ɼ�â
        AP_IChatOptionWindow apChatOptionWindow = cFactory.CreateChatOptionWindow( this, m_pEngineDevice );
        m_pChatOptionWindow = apChatOptionWindow.release();
        m_pChatOptionWindow->CreateUIWindowAndRegisterOwnership();

        // ä�� Ÿ�� ����Ʈ
        AP_IChatTypeList apChatTypeList = cFactory.CreateChatTypeList( this, m_pEngineDevice );
        m_pChatTypeList = apChatTypeList.release();
        m_pChatTypeList->CreateUIWindowAndRegisterOwnership();
	}

	if( GLUseFeatures::GetInstance().IsUsingRenewCharWindow() )
	{
		RnCharacterWindow* pRnCharacterWindow = new RnCharacterWindow( m_pGaeaClient, this, m_pEngineDevice );
		pRnCharacterWindow->Create( CHARACTER_WINDOW,"BASIC_WINDOW",UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pRnCharacterWindow->CreateSubControl();
		UiRegisterControl( pRnCharacterWindow, true );
		UiShowGroupFocus( CHARACTER_WINDOW );
		m_pCharacterWindow = pRnCharacterWindow;
	}
	else
	{
		CCharacterWindow* pCharacterWindow = new CCharacterWindow(m_pGaeaClient, this, m_pEngineDevice);
		pCharacterWindow->Create( CHARACTER_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pCharacterWindow->CreateBaseWindowBlack( "CHARACTER_WINDOW", (char*)ID2GAMEWORD("CHARACTER_NAME_STATIC") );
		pCharacterWindow->CreateSubControl();
		pCharacterWindow->SetAlignFlag( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		UiRegisterControl( pCharacterWindow, true );
		UiShowGroupFocus( CHARACTER_WINDOW );
		m_pCharacterWindow = pCharacterWindow; 
	}

	// Note : ���� �ʱ�ȭ
	{
		m_pStatsResetWindow = new CStatsResetWindow ( m_pGaeaClient, this, m_pEngineDevice );
		m_pStatsResetWindow->Create ( STATSRESET_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pStatsResetWindow->CreateBaseWindowBlack ( "STATSRESET_WINDOW", (char*)ID2GAMEWORD("STATSRESET_NAME_STATIC") );
		m_pStatsResetWindow->CreateSubControl ();
		m_pStatsResetWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_CENTER_X );
		UiRegisterControl ( m_pStatsResetWindow, true );
		UiShowGroupFocus ( STATSRESET_WINDOW );
	}


	{
		CBasicHWOptionWindow* pOptionWindow = new CBasicHWOptionWindow ( m_pGaeaClient, this, m_pEngineDevice );
		pOptionWindow->Create ( OPTION_HW_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pOptionWindow->CreateBaseWindowBlack ( "HWOPTION_WINDOW", (char*)ID2GAMEWORD("HWOPTION_WINDOW_NAME_STATIC") );
		pOptionWindow->CreateSubControl ();
		pOptionWindow->SetControlNameEx ( "�ɼ�â" );
		pOptionWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		UiRegisterControl ( pOptionWindow, true );
		UiShowGroupFocus ( OPTION_HW_WINDOW );
	}

	{   // ��ų â
        AP_ISkillWindow apSkillWindow = cFactory.CreateSkillWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pSkillWindow = apSkillWindow.release();
        m_pSkillWindow->CreateUIWindowAndRegisterOwnership();
	}

	// Note : ���� �ý���
	{
        AP_IPostBoxWindow apPostBoxWindow = cFactory.CreatePostBoxWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pPostBoxWindow = apPostBoxWindow.release();
        m_pPostBoxWindow->CreateUIWindowAndRegisterOwnership( pd3dDevice );
	}

    // Note : SNS ����
    {
        m_pSNSWindow = new CSNSWindow(m_pGaeaClient, this, m_pEngineDevice);
        m_pSNSWindow->Create ( SNS_WINDOW, "SNS_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
        m_pSNSWindow->CreateSubControl ( pd3dDevice );
        m_pSNSWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
        UiRegisterControl ( m_pSNSWindow );	
        UiShowGroupFocus ( SNS_WINDOW );
    }

    if( COMMON_WEB::IsCreate() )
    {
        m_pFaceBook = new CFaceBookWindow(m_pGaeaClient, this, m_pEngineDevice);
        m_pFaceBook->Create ( FACEBOOK_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
        m_pFaceBook->CreateBaseWindowBlack ( "FACEBOOK_WINDOW", (char*)ID2GAMEWORD("SNS_TITLE", 0) );
        m_pFaceBook->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
        UiRegisterControl ( m_pFaceBook, true );	
        UiShowGroupFocus ( FACEBOOK_WINDOW );

        m_pTwitter = new CTwitterWindow(m_pGaeaClient, this, m_pEngineDevice);
        m_pTwitter->Create ( TWITTER_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
        m_pTwitter->CreateBaseWindowBlack ( "TWITTER_WINDOW", (char*)ID2GAMEWORD("SNS_TITLE", 1) );
        m_pTwitter->CreateSubControl();
        m_pTwitter->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
        UiRegisterControl ( m_pTwitter, true );	
        UiShowGroupFocus ( TWITTER_WINDOW );
    }

	{
		CBasicEscMenu* pEscMenu = new CBasicEscMenu ( this, m_pEngineDevice );
		pEscMenu->Create ( ESC_MENU, "ESC_MENU", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		pEscMenu->CreateSubControl ();
		UiRegisterControl ( pEscMenu );
		UiShowGroupFocus ( ESC_MENU );
	}

	{
		CGradeDisplay* pGradeDisplay = new CGradeDisplay(m_pEngineDevice);
		pGradeDisplay->Create ( GRADE_DISPLAY, "GRADE_DISPLAY", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		pGradeDisplay->CreateSubControl ();
		UiRegisterControl ( pGradeDisplay );
		UiShowGroupFocus ( GRADE_DISPLAY );			
	}

	{ // ��Ƽ ������;
		if( GLUseFeatures::GetInstance().IsUsingPartyWindowRenewal() )
		{
			AP_IPartyYesNoWindow apPartyYesNoWindow = cFactory.CreatePartyYesNoWindow( m_pGaeaClient, this, m_pEngineDevice );
			m_pPartyYesNoWindow= apPartyYesNoWindow.release();
			m_pPartyYesNoWindow->CreateUIWindowAndRegisterOwnership();

			AP_IPartyWindowRenewal apPartyWindowRenewal = cFactory.CreatePartyWindowRenewal( m_pGaeaClient, this, m_pEngineDevice );
			m_pPartyWindowRenewal = apPartyWindowRenewal.release();
			m_pPartyWindowRenewal->CreateUIWindowAndRegisterOwnership();			

			AP_IExpeditionInviteWindow apExpeditionInviteWindow = cFactory.CreateExpeditionInviteWindow( m_pGaeaClient, this, m_pEngineDevice );
			m_pExpeditionInviteWindow = apExpeditionInviteWindow.release();
			m_pExpeditionInviteWindow->CreateUIWindowAndRegisterOwnership();

			AP_IExpeditionCheckReadyWindow apExpeditionCheckReadyWindow = cFactory.CreateExpeditionCheckReadyWindow( m_pGaeaClient, this, m_pEngineDevice );
			m_pExpeditionCheckReadyWindow = apExpeditionCheckReadyWindow.release();
			m_pExpeditionCheckReadyWindow->CreateUIWindowAndRegisterOwnership();

			

			AP_IPartyStateWindow apPartyStateWindow = cFactory.CreatePartyStateWindow( m_pGaeaClient, this, m_pEngineDevice );
			m_pPartyStateWindow = apPartyStateWindow.release();
			m_pPartyStateWindow->CreateUIWindowAndRegisterOwnership();

			//AP_IMiniExpeditionWindow apMiniExpeditionWindow = cFactory.CreateMiniExpeditionWindow( m_pGaeaClient, this, m_pEngineDevice );
			//m_pMiniExpeditionWindow = apMiniExpeditionWindow.release();
			//m_pMiniExpeditionWindow->CreateUIWindowAndRegisterOwnership();

			AP_IPartyTenderWindow apPartyTenderWindow = cFactory.CreatePartyTenderWindow( m_pGaeaClient, this, m_pEngineDevice );
			m_pPartyTenderWindow = apPartyTenderWindow.release();
			m_pPartyTenderWindow->CreateUIWindowAndRegisterOwnership();

			AP_IPartyTenderNotifyButton apPartyTenderNotifyButton = cFactory.CreatePartyTenderNotifyButton( m_pGaeaClient, this, m_pEngineDevice );
			m_pPartyTenderNotifyButton = apPartyTenderNotifyButton.release();
			m_pPartyTenderNotifyButton->CreateUIWindowAndRegisterOwnership();

			AP_IPartyDistributionWindow apPartyDistribution = cFactory.CreatePartyDistributionWindow( m_pGaeaClient, this, m_pEngineDevice );
			m_pPartyDistributionWindow = apPartyDistribution.release();
			m_pPartyDistributionWindow->CreateUIWindowAndRegisterOwnership();

			AP_IPartyDistributionListWindow apPartyDistributionList = cFactory.CreatePartyDistributionListWindow( m_pGaeaClient, this, m_pEngineDevice );
			m_pPartyDistributionListWindow = apPartyDistributionList.release();
			m_pPartyDistributionListWindow->CreateUIWindowAndRegisterOwnership();
		}
		else
		{			
			AP_IPartyWindow apPartyWindow = cFactory.CreatePartyWindow( m_pGaeaClient, this, m_pEngineDevice );
			m_pPartyWindow = apPartyWindow.release();
			m_pPartyWindow->CreateUIWindowAndRegisterOwnership();
			
			// �̴� ��Ƽ
			/*CMiniPartyWindow* pMiniPartyWindow = new CMiniPartyWindow(m_pGaeaClient, this, m_pEngineDevice);
			pMiniPartyWindow->Create ( MINIPARTY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pMiniPartyWindow->CreateBaseWindowMiniParty ( "MINIPARTY_WINDOW", (char*)ID2GAMEWORD("MINIPARTY_NAME_STATIC") );
			pMiniPartyWindow->CreateSubControl ();
			pMiniPartyWindow->SetAlignFlag ( UI_FLAG_BOTTOM | UI_FLAG_RIGHT );
			UiRegisterControl ( pMiniPartyWindow, true );
			UiShowGroupFocus ( MINIPARTY_WINDOW );*/
		}
	}

	// �̴� ��Ƽ
	CMiniPartyWindow* pMiniPartyWindow = new CMiniPartyWindow(m_pGaeaClient, this, m_pEngineDevice);
	pMiniPartyWindow->Create ( MINIPARTY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pMiniPartyWindow->CreateBaseWindowMiniParty ( "MINIPARTY_WINDOW", (char*)ID2GAMEWORD("MINIPARTY_NAME_STATIC") );
	pMiniPartyWindow->CreateSubControl ();
	pMiniPartyWindow->SetAlignFlag ( UI_FLAG_BOTTOM | UI_FLAG_RIGHT );
	UiRegisterControl ( pMiniPartyWindow, true );
	UiShowGroupFocus ( MINIPARTY_WINDOW );

	{		
		AP_IDPSWindow apDPSWindow = cFactory.CreateDPSWindow(m_pGaeaClient, this, m_pEngineDevice);
		m_pDPSWindow= apDPSWindow.release();
		m_pDPSWindow->CreateUIWindowAndRegisterOwnership();
	}

	{
        AP_IMarketWindow apMarketWindow = cFactory.CreateMarketWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pMarketWindow = apMarketWindow.release();
        m_pMarketWindow->CreateUIWindowAndRegisterOwnership();
	}

	{
		if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
		{
			m_pInventory = new CRnInventoryWindow( m_pGaeaClient, this, m_pEngineDevice );
			m_pInventory->CreateUIWindowAndRegisterOwnership();
		}
		else
		{
			// �κ��丮
			AP_IInventoryWindow apInventoryWindow = cFactory.CreateInventoryWindow( m_pGaeaClient, this, m_pEngineDevice );
			m_pInventoryWindow = apInventoryWindow.release();
			m_pInventoryWindow->CreateUIWindowAndRegisterOwnership();

			// ��������� ����
			m_pItemBankWindow = new CItemBankWindow ( m_pGaeaClient, this, m_pEngineDevice );
			m_pItemBankWindow->Create ( ITEMBANK_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			m_pItemBankWindow->CreateBaseWindowLightGray ( "ITEMBANK_WINDOW", (char*)ID2GAMEWORD("ITEMBANK_NAME_STATIC") );
			m_pItemBankWindow->CreateSubControl ();
			m_pItemBankWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
			UiRegisterControl ( m_pItemBankWindow, true );
			UiShowGroupFocus ( ITEMBANK_WINDOW );
		}
	}

	{	//	�ŷ�
        AP_ITradeWindow apTradeWindow = cFactory.CreateTradeWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pTradeWindow = apTradeWindow.release();
        m_pTradeWindow->CreateUIWindowAndRegisterOwnership();

        AP_ITradeInventoryWindow apTradeInventoryWindow = cFactory.CreateTradeInventoryWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pTradeInventoryWindow = apTradeInventoryWindow.release();
        m_pTradeInventoryWindow->CreateUIWindowAndRegisterOwnership();
	}

	{
		if ( GLUseFeatures::GetInstance().IsUsingNewInstance() == true )
		{
			if ( !GLUseFeatures::GetInstance().IsUsingWorldBattle() )
			{
				// Rn���� UI
				AP_IRnCompetitionWindow apRnCompetitionWindow = cFactory.CreateRnCompetitionWindow( m_pGaeaClient, this, m_pEngineDevice );
				m_pRnCompetitionWindow = apRnCompetitionWindow.release();
				m_pRnCompetitionWindow->CreateUIWindowAndRegisterOwnership( pd3dDevice );

				AP_IRnCDMTopRanking apRnCDMTopRanking = cFactory.CreateRnCDMTopRanking( m_pGaeaClient, this, m_pEngineDevice );
				m_pRnCDMTopRanking = apRnCDMTopRanking.release();
				m_pRnCDMTopRanking->CreateUIWindowAndRegisterOwnership();
			}

//             AP_IRnCompetitionResultWindow apRnCompetitionResultWindow = cFactory.CreateRnCompetitionResultWindow( m_pGaeaClient, this, m_pEngineDevice );
//             m_pRnCompetitionResultWindow = apRnCompetitionResultWindow.release();
//             m_pRnCompetitionResultWindow->CreateUIWindowAndRegisterOwnership( pd3dDevice );

		}
		else
		{
			//���� UI
			AP_ICompetitionWindow apCompetitionWindow = cFactory.CreateCompetitionWindow( m_pGaeaClient, this, m_pEngineDevice );
			m_pCompetitionWindow = apCompetitionWindow.release();
			m_pCompetitionWindow->CreateUIWindowAndRegisterOwnership( pd3dDevice );
		}

		AP_ICompetitionTopRanking apCompetitionTopRanking = cFactory.CreateCompetitionTopRanking( m_pGaeaClient, this, m_pEngineDevice );
		m_pCompetitionTopRanking = apCompetitionTopRanking.release();
		m_pCompetitionTopRanking->CreateUIWindowAndRegisterOwnership();

        AP_IRnCompetitionResultWindow apRnCompetitionResultWindow = cFactory.CreateRnCompetitionResultWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pRnCompetitionResultWindow = apRnCompetitionResultWindow.release();
        m_pRnCompetitionResultWindow->CreateUIWindowAndRegisterOwnership( pd3dDevice );
	}

	{	//	â��
		m_pStorageWindow = new CStorageWindow ( m_pGaeaClient, this, m_pEngineDevice );		
		m_pStorageWindow->Create ( STORAGE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pStorageWindow->CreateBaseWindowLightGray ( "STORAGE_WINDOW", (char*)ID2GAMEWORD("STORAGE_NAME_STATIC") );
		m_pStorageWindow->CreateSubControl ();
		m_pStorageWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
        UiRegisterControl ( m_pStorageWindow, true );
        UiRegisterNpcActionableControl ( m_pStorageWindow );
		UiShowGroupFocus ( STORAGE_WINDOW );
	}

	m_pGaeaClient->GetCharacter()->ReqStorageCloseCard();

	{	//	â��
		m_pClubStorageWindow = new CClubStorageWindow ( m_pGaeaClient, this, m_pEngineDevice );		
		m_pClubStorageWindow->Create ( CLUB_STORAGE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pClubStorageWindow->CreateBaseWindowLightGray ( "STORAGE_WINDOW", (char*)ID2GAMEWORD("CLUB_STORAGE_NAME_STATIC") );
		m_pClubStorageWindow->CreateSubControl ();
		m_pClubStorageWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		UiRegisterControl ( m_pClubStorageWindow, true );
		UiShowGroupFocus ( CLUB_STORAGE_WINDOW );
	}	

    {
        AP_IModalWindowInner apModalWindow = cFactory.CreateModalWindow( this, m_pEngineDevice );
        m_pModalWindow = apModalWindow.release();
        m_pModalWindow->CreateUIWindowAndRegisterOwnership();
    }

	{
        AP_IPtoPWindow apPtoPWindow = cFactory.CreatePtoPWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pPtoPWindow = apPtoPWindow.release();
        m_pPtoPWindow->CreateUIWindowAndRegisterOwnership();
	}

	{
		m_pDialogueWindow = new CDialogueWindow ( m_pGaeaClient, this, m_pEngineDevice );
		m_pDialogueWindow->Create ( DIALOGUE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pDialogueWindow->CreateBaseWindowBlack ( "DIALOGUE_WINDOW", (char*)ID2GAMEWORD("DIALOGUE_WINDOW_STATIC") );
		m_pDialogueWindow->CreateSubControl ();
		m_pDialogueWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		UiRegisterControl ( m_pDialogueWindow, true );
        UiRegisterNpcActionableControl( m_pDialogueWindow );
		UiShowGroupFocus ( DIALOGUE_WINDOW );
	}

    // TODO : NPC ��ȭ �ŷ� â ����.
    {
        m_pDialogueWindowRenewal = new CDialogueWindowRenewal( m_pGaeaClient, this, m_pEngineDevice );
        m_pDialogueWindowRenewal->Create( DIALOGUE_WINDOW_RENEWAL, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
        m_pDialogueWindowRenewal->CreateBaseWindowLightGray( "DIALOGUE_WINDOW_RENEWAL", (char*)ID2GAMEWORD("DIALOGUE_WINDOW_STATIC") );
        m_pDialogueWindowRenewal->CreateSubControl();
        m_pDialogueWindowRenewal->SetAlignFlag( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
        UiRegisterControl( m_pDialogueWindowRenewal, true );
        UiRegisterNpcActionableControl( m_pDialogueWindowRenewal );
        UiShowGroupFocus( DIALOGUE_WINDOW_RENEWAL );
    }
	
	{
        AP_IItemMove apItemMove = cFactory.CreateItemMove( m_pGaeaClient, this, m_pEngineDevice );
        m_pItemMove = apItemMove.release();
        m_pItemMove->CreateUIWindowAndRegisterOwnership();
	}

    // TODO : ������ ����,,,
    {
        m_pItemInforTooltip = new CItemInfoTooltip( m_pGaeaClient, this, m_pEngineDevice );
        m_pItemInforTooltip->Create( ITEM_INFOR_TOOLTIP, "ITEM_INFOR_TOOLTIP_RN", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
        m_pItemInforTooltip->CreateSubControl();
        m_pItemInforTooltip->SetVisibleSingle( FALSE );
        UiRegisterControl( m_pItemInforTooltip );
        UiShowGroupTop( ITEM_INFOR_TOOLTIP );
		m_pItemInforTooltip->SetBGAlpa(180);
    }

    {
        m_pItemInforTooltipWear = new CItemInfoTooltip( m_pGaeaClient, this, m_pEngineDevice );
        m_pItemInforTooltipWear->Create( ITEM_INFOR_TOOLTIP_WEAR, "ITEM_INFOR_TOOLTIP_RN", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
        m_pItemInforTooltipWear->CreateSubControl();
        m_pItemInforTooltipWear->SetVisibleSingle( FALSE );
        UiRegisterControl( m_pItemInforTooltipWear );
        UiShowGroupTop( ITEM_INFOR_TOOLTIP_WEAR );
		m_pItemInforTooltipWear->SetBGAlpa(180);
    }

	{
		m_pItemInforTooltipDurability = new CItemInfoTooltip( m_pGaeaClient, this, m_pEngineDevice );
		m_pItemInforTooltipDurability->Create( ITEM_INFOR_TOOLTIP_DURABILITY, "ITEM_INFOR_TOOLTIP_RN", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemInforTooltipDurability->CreateSubControl();
		m_pItemInforTooltipDurability->SetVisibleSingle( FALSE );
		UiRegisterControl( m_pItemInforTooltipDurability );
		UiShowGroupTop( ITEM_INFOR_TOOLTIP_DURABILITY );
		m_pItemInforTooltipDurability->SetBGAlpa(180);
	}

	{
		m_pItemInforTooltipLink = new CItemInfoTooltip( m_pGaeaClient, this, m_pEngineDevice );
		m_pItemInforTooltipLink->Create( ITEM_INFOR_TOOLTIP_LINK, "ITEM_INFOR_TOOLTIP_LINK_RN", UI_FLAG_BOTTOM | UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemInforTooltipLink->CreateLinkBoxStyle();
		m_pItemInforTooltipLink->CreateSubControl();
		m_pItemInforTooltipLink->SetBlockMouseTracking( true );
		m_pItemInforTooltipLink->SetVisibleSingle( FALSE );
		UiRegisterControl( m_pItemInforTooltipLink );
		UiShowGroupFocus( ITEM_INFOR_TOOLTIP_LINK );

		m_pItemInforTooltipLink->SetBGAlpa(180);
	}
	
    {
        m_pSkillTooltip = new CSkillInfoTooltip( m_pGaeaClient, this, m_pEngineDevice );
        m_pSkillTooltip->Create( SKILL_INFOR_TOOLTIP, "SKILL_INFOR_TOOLTIP", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
        m_pSkillTooltip->CreateSubControl();
        m_pSkillTooltip->SetVisibleSingle( FALSE );
        UiRegisterControl( m_pSkillTooltip );
		m_pSkillTooltip->SetBGAlpha(180);
        UiShowGroupTop( SKILL_INFOR_TOOLTIP );
    }
	{
		m_pSkillTooltipSubSkill = new CSkillInfoTooltip( m_pGaeaClient, this, m_pEngineDevice );
		m_pSkillTooltipSubSkill->Create( SKILL_INFOR_TOOLTIP_MULTI, "SKILL_INFOR_TOOLTIP", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pSkillTooltipSubSkill->CreateSubControl();
		m_pSkillTooltipSubSkill->SetVisibleSingle( FALSE );
		UiRegisterControl( m_pSkillTooltipSubSkill );
		m_pSkillTooltipSubSkill->SetBGAlpha(180);
		UiShowGroupTop( SKILL_INFOR_TOOLTIP_MULTI );

        //��Ƽ������� ��ġ ������ ���� ���ΰ��� �����͸� ����
        m_pSkillTooltip->SetOtherMultiSkillTooltip( m_pSkillTooltipSubSkill );
        m_pSkillTooltipSubSkill->SetOtherMultiSkillTooltip( m_pSkillTooltip );
	}
	{
		m_pSkillTooltipLinkSkill = new CSkillLinkInfoTooltip( m_pGaeaClient, this, m_pEngineDevice );
		m_pSkillTooltipLinkSkill->Create( SKILL_INFOR_TOOLTIP_LINK, "SKILL_INFOR_TOOLTIP", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pSkillTooltipLinkSkill->CreateSubControl();
		m_pSkillTooltipLinkSkill->SetVisibleSingle( FALSE );
		UiRegisterControl( m_pSkillTooltipLinkSkill );
		m_pSkillTooltipLinkSkill->SetBGAlpha(180);
		UiShowGroupTop( SKILL_INFOR_TOOLTIP_LINK );

		m_pSkillTooltipLinkSkill->SetOtherMultiSkillTooltip( m_pSkillTooltipSubSkill );
		m_pSkillTooltipLinkSkill->SetMainSkillTooltip( m_pSkillTooltip );
		m_pSkillTooltip->SetLinkSkillTooltip( m_pSkillTooltipLinkSkill->GetSkillInfoTooltip() );
		m_pSkillTooltipSubSkill->SetLinkSkillTooltip( m_pSkillTooltipLinkSkill->GetSkillInfoTooltip() );
	}
	
	{
		m_pSkillTooltipDelayActionSkill = new CSkillInfoTooltip( m_pGaeaClient, this, m_pEngineDevice );
		m_pSkillTooltipDelayActionSkill->Create( SKILL_INFOR_TOOLTIP_DELAY_ACTION, "SKILL_INFOR_TOOLTIP", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pSkillTooltipDelayActionSkill->CreateSubControl();
		m_pSkillTooltipDelayActionSkill->SetVisibleSingle( FALSE );
		UiRegisterControl( m_pSkillTooltipDelayActionSkill );
		m_pSkillTooltipDelayActionSkill->SetBGAlpha(180);
		UiShowGroupTop( SKILL_INFOR_TOOLTIP_DELAY_ACTION );

		m_pSkillTooltipDelayActionSkill->SetOtherMultiSkillTooltip( m_pSkillTooltip );
		m_pSkillTooltipDelayActionSkill->SetLinkSkillTooltip( m_pSkillTooltipLinkSkill->GetSkillInfoTooltip() );
		m_pSkillTooltipLinkSkill->SetDealyActionSSkillTooltip( m_pSkillTooltipDelayActionSkill );
	}

    {
        m_pInvenItemTooltipGroup = new CItemToolTipGroup( this, m_pGaeaClient, m_pEngineDevice, INVEN_ITEM_TOOLTIP_BEGIN, EItemToolTipCount );
        m_pWishItemTooltipGroup = new CItemToolTipGroup( this, m_pGaeaClient, m_pEngineDevice, WISH_ITEM_TOOLTIP_BEGIN, EItemToolTipCount );
        m_pMarketWishItemTooltipGroup = new CItemToolTipGroup( this, m_pGaeaClient, m_pEngineDevice, MARKET_WISH_ITEM_TOOLTIP_BEGIN, EItemToolTipCount );
    }

    {
        for( int i=0; i<ESkillToolTipCount; ++i )
        {
            CSkillInfoTooltip* pToolTip = new CSkillInfoTooltip( m_pGaeaClient, this, m_pEngineDevice );
            pToolTip->Create( SKILL_TOOLTIP_BEGIN + i, "SKILL_INFOR_TOOLTIP", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
            pToolTip->CreateSubControl();
            pToolTip->CreateCloseButton();
            pToolTip->SetBlockMouseTracking( true );
            pToolTip->SetVisibleSingle( FALSE );
            UiRegisterControl( pToolTip, true );
            UiShowGroupFocus( SKILL_TOOLTIP_BEGIN + i );
            UiHideGroup( SKILL_TOOLTIP_BEGIN + i );

            SSkillToolTip sToolTip;
            sToolTip.nID = SKILL_TOOLTIP_BEGIN + i;
            sToolTip.pControl = pToolTip;
            m_vecSkillToolTipRegist.push_back( sToolTip );
            m_vecSkillToolTipPool.push_back( sToolTip );
        }
    }

	{
		m_pInfoDisplay = new CBasicVarTextBox(m_pEngineDevice);
		m_pInfoDisplay->Create ( INFO_DISPLAY, "BASIC_VAR_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pInfoDisplay->CreateSubControl ();
		m_pInfoDisplay->SetVisibleSingle ( FALSE );
		UiRegisterControl ( m_pInfoDisplay );
		UiShowGroupTop ( INFO_DISPLAY );
	}

	{
		m_pInfoDisplayEx = new CBasicVarTextBox(m_pEngineDevice);
		m_pInfoDisplayEx->Create ( INFO_DISPLAY_EX, "BASIC_VAR_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pInfoDisplayEx->CreateSubControl ();
		m_pInfoDisplayEx->SetVisibleSingle ( FALSE );
		UiRegisterControl ( m_pInfoDisplayEx );
		UiShowGroupTop ( INFO_DISPLAY_EX );
	}

	{
		m_pInfoDisplayLink = new CInfoDisplayLink ( this, m_pEngineDevice );
		m_pInfoDisplayLink->Create ( INFO_DISPLAY_LINK, "INFO_DISPLAY_LINK", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pInfoDisplayLink->CreateSubControl ();
		m_pInfoDisplayLink->SetVisibleSingle ( FALSE );
		UiRegisterControl ( m_pInfoDisplayLink );
		UiShowGroupFocus ( INFO_DISPLAY_LINK );
		UiHideGroup( INFO_DISPLAY_LINK );
	}

	{
		m_pAdminMessageDisplay = new CAdminMessageDisplay(m_pEngineDevice);
		m_pAdminMessageDisplay->Create ( ADMIN_MESSAGE_DISPLAY, "ADMIN_MESSAGE_DISPLAY" );
		m_pAdminMessageDisplay->CreateSubControl ();		
		UiRegisterControl ( m_pAdminMessageDisplay );
		UiShowGroupTop ( ADMIN_MESSAGE_DISPLAY );

		CSimpleMessageMan*	pSimpleMessageMan = new CSimpleMessageMan(m_pGaeaClient, this, m_pEngineDevice);
		pSimpleMessageMan->Create ( SIMPLE_MESSAGE_MAN, "SIMPLE_MESSAGE_MAN" );
		pSimpleMessageMan->CreateSubControl ();		
		UiRegisterControl ( pSimpleMessageMan );
		UiShowGroupBottom ( SIMPLE_MESSAGE_MAN );
		UiHideGroup ( SIMPLE_MESSAGE_MAN );
	}

	{	//	��� ��û ���
        AP_IConftModalWindow apConftModalWindow = cFactory.CreateConftModalWindow( this, m_pEngineDevice );
        m_pConftModalWindow = apConftModalWindow.release();
        m_pConftModalWindow->CreateUIWindowAndRegisterOwnership();
	}

	{	//	��Ƽ ��û ���
        AP_IPartyModalWindow apPartyModalWindow = cFactory.CreatePartyModalWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pPartyModalWindow = apPartyModalWindow.release();
        m_pPartyModalWindow->CreateUIWindowAndRegisterOwnership();
	}

	{	//	�׷�ä�� ��û ���
		AP_IChatGroupModalWindow apChatGroupModalWindow = cFactory.CreateChatGroupModalWindow( m_pGaeaClient, this, m_pEngineDevice );
		m_pChatGroupModalWindow = apChatGroupModalWindow.release();
		m_pChatGroupModalWindow->CreateUIWindowAndRegisterOwnership();
	}

	{	//	���� �ȱ� ���� ���
		m_pPrivateMarketSellWindow = new CPrivateMarketSellWindow ( this, m_pEngineDevice );
		m_pPrivateMarketSellWindow->Create ( PRIVATE_MARKET_SELL_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );		
		m_pPrivateMarketSellWindow->CreateBaseModal ( "PRIVATE_MARKET_SELL_WINDOW");
		m_pPrivateMarketSellWindow->CreateSubControl ();
		m_pPrivateMarketSellWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );

		//m_pPrivateMarketSellWindow->LoadDefaultCondition ();
		UiRegisterControl ( m_pPrivateMarketSellWindow );
		UiShowGroupFocus ( PRIVATE_MARKET_SELL_WINDOW );
	}

	{	//	�� �̵�
		m_pMapMoveDisplay = new CMapMoveDisplay ( m_pGaeaClient, m_pEngineDevice );
		m_pMapMoveDisplay->Create ( MAPMOVE_DISPLAY, "BASIC_MAPMOVE_DISPLAY", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pMapMoveDisplay->CreateSubControl ();
		UiRegisterControl ( m_pMapMoveDisplay );
		UiShowGroupFocus ( MAPMOVE_DISPLAY );

		m_pMapMoveDisplayDummy = new CUIControl(m_pEngineDevice);
		m_pMapMoveDisplayDummy->Create ( MAPMOVE_DISPLAY_DUMMY, "BASIC_MAPMOVE_DISPLAY", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
        UiRegisterControl ( m_pMapMoveDisplayDummy );

		m_pMultiMapMoveDisplay = new CMultiMapMoveDisplay ( m_pGaeaClient, m_pEngineDevice );
		m_pMultiMapMoveDisplay->Create ( MULTIMAPMOVE_DISPLAY, "MULTIMAPMOVE_WINDOW_BOX", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pMultiMapMoveDisplay->CreateSubControl ();
		UiRegisterControl ( m_pMultiMapMoveDisplay );
		UiShowGroupFocus ( MULTIMAPMOVE_DISPLAY );

		m_pMultiMapMoveDisplayDummy = new CUIControl(m_pEngineDevice);
		m_pMultiMapMoveDisplayDummy->Create ( MULTIMAPMOVE_DISPLAY_DUMMY, "MULTIMAPMOVE_WINDOW_BOX", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		UiRegisterControl ( m_pMultiMapMoveDisplayDummy );
		

		m_pSystemMessageWindow = new CSystemMessageWindow ( m_pGaeaClient, this, m_pEngineDevice );
		m_pSystemMessageWindow->Create ( SYSTEM_MESSAGE_WINDOW, "BASIC_SYSTEM_MESSAGE_DISPLAY", UI_FLAG_CENTER_X );
		m_pSystemMessageWindow->CreateSubControl ();
		UiRegisterControl ( m_pSystemMessageWindow );
		UiShowGroupTop ( SYSTEM_MESSAGE_WINDOW );
		UiHideGroup ( SYSTEM_MESSAGE_WINDOW );

		m_pSystemMessageWindowDummy = new CUIControl(m_pEngineDevice);
		m_pSystemMessageWindowDummy->Create ( SYSTEM_MESSAGE_WINDOW_DUMMY, "BASIC_SYSTEM_MESSAGE_DISPLAY", UI_FLAG_CENTER_X );
        UiRegisterControl ( m_pSystemMessageWindowDummy );
	}

	{		
		m_pWaitServerDisplay = new CWaitServerDialogue ( m_pGaeaClient, this, m_pEngineDevice );
		m_pWaitServerDisplay->Create ( WAITSERVER_DISPLAY, "WAITSERVER_DISPLAY", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pWaitServerDisplay->CreateSubControl ();
		UiRegisterControl ( m_pWaitServerDisplay );
		UiShowGroupFocus ( WAITSERVER_DISPLAY );
	}

	{	//	��� ī��Ʈ �Ŵ���
        AP_IConftDisplayMan apConftDisplayMan = cFactory.CreateConftDisplayMan( this, m_pEngineDevice );
        m_pConftDisplayMan = apConftDisplayMan.release();
        m_pConftDisplayMan->CreateUIWindowAndRegisterOwnership();
	}

	{
		// WebBrowser �����Ǿ� ������ CHelpWindowWeb ����, ������ CHelpWindow ����
		if( COMMON_WEB::IsCreate() )
		{
			CHelpWindowWeb* pHelpWindow = new CHelpWindowWeb ( this, m_pEngineDevice );
			pHelpWindow->Create ( HELP_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pHelpWindow->CreateBaseWindowBlack ( "HELP_WINDOW", (char*)ID2GAMEWORD("HELP_WINDOW_STATIC") );
			pHelpWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
			UiRegisterControl ( pHelpWindow, true );
		}
		else
		{
			CHelpWindow* pHelpWindow = new CHelpWindow ( this, m_pEngineDevice );
			pHelpWindow->Create ( HELP_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pHelpWindow->CreateBaseWindowBlack ( "HELP_WINDOW", (char*)ID2GAMEWORD("HELP_WINDOW_STATIC") );
			pHelpWindow->CreateSubControl ();
			pHelpWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
			UiRegisterControl ( pHelpWindow, true );
		}
		UiShowGroupFocus ( HELP_WINDOW );
	}

//#if defined ( CH_PARAM ) || defined ( TH_PARAM ) || defined( PH_PARAM ) || defined ( JP_PARAM ) || defined(_RELEASED) // �ʸ��� �����ۼ�
	
	/*if (sp == SP_THAILAND || sp == SP_PHILIPPINES || sp == SP_JAPAN || sp == SP_CHINA)
	{
		if( COMMON_WEB::IsCreate() )
		{
			m_pItemShopWindowWeb = new CItemShopWindowWeb(m_pGaeaClient, this, m_pEngineDevice);
			m_pItemShopWindowWeb->Create ( ITEMSHOP_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			m_pItemShopWindowWeb->CreateBaseWindowBlack ( "ITEMSHOP_WINDOW", (char*)ID2GAMEWORD("ITEMSHOP_WINDOW_STATIC") );
			m_pItemShopWindowWeb->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
			UiRegisterControl ( m_pItemShopWindowWeb, true );			
		}
		UiShowGroupFocus ( ITEMSHOP_WINDOW );
	}*/
//#endif

	if ( !GLUseFeatures::GetInstance().IsUsingWorldBattle() )
    {
        AP_IQuestWindowRenewal apQuestWindowRenewal = cFactory.CreateQuestWindowRenewal( m_pGaeaClient, this, m_pEngineDevice );
        m_pQuestWindowRenewal = apQuestWindowRenewal.release();
        m_pQuestWindowRenewal->CreateUIWindowAndRegisterOwnership();
    }

	{
		m_pSimpleHP = new CSimpleHP ( m_pGaeaClient, m_pEngineDevice );
		m_pSimpleHP->Create ( SIMPLE_HP, "SIMPLE_HP" );
		m_pSimpleHP->CreateSubControl ();
		UiRegisterControl ( m_pSimpleHP );
		UiShowGroupBottom ( SIMPLE_HP );
		UiHideGroup ( SIMPLE_HP );
	}

    AP_ISkillTrayTab apSkillTrayTab = cFactory.CreateSkillTrayTab( m_pGaeaClient, this, m_pEngineDevice );
    m_pSkillTrayTab = apSkillTrayTab.release();
    m_pSkillTrayTab->CreateUIWindowAndRegisterOwnership();

    AP_IBasicInfoView apBasicInfoView = cFactory.CreateBasicInfoView( m_pGaeaClient, this, m_pEngineDevice );
    m_pBasicInfoView = apBasicInfoView.release();
    m_pBasicInfoView->CreateUIWindowAndRegisterOwnership();

    AP_ISkillWindowToTray apSkillWindowToTray = cFactory.CreateSkillWindowToTray( m_pGaeaClient, this, m_pEngineDevice );
    m_pSkillWindowToTray = apSkillWindowToTray.release();
    m_pSkillWindowToTray->CreateUIWindowAndRegisterOwnership();

	AP_IReservedSkillAnimation apReservedSkillAnimation = cFactory.CreateReservedSkillAnimation( m_pGaeaClient, this, m_pEngineDevice );
	m_pReservedSkillAnimation = apReservedSkillAnimation.release();
	m_pReservedSkillAnimation->CreateUIWindowAndRegisterOwnership();

	{
		m_pBonusTimeGauge = new CBonusTimeGauge(m_pGaeaClient, this, m_pEngineDevice);
		m_pBonusTimeGauge->Create ( BONUSTIME_EVENT_GAUGE, "BONUSTIME_EVENT_GAUGE", UI_FLAG_RIGHT );
		m_pBonusTimeGauge->CreateSubControl ();
		UiRegisterControl ( m_pBonusTimeGauge );
		UiShowGroupBottom ( BONUSTIME_EVENT_GAUGE );
		UiHideGroup ( BONUSTIME_EVENT_GAUGE );

		m_pBonusTimeDisplay = new CBonusTimeDisplay(m_pGaeaClient, m_pEngineDevice);
		m_pBonusTimeDisplay->Create ( BONUSTIME_EVENT_DISPLAY, "BONUSTIME_EVENT_DISPLAY");
		m_pBonusTimeDisplay->CreateSubControl ();
		UiRegisterControl ( m_pBonusTimeDisplay );
		UiShowGroupTop ( BONUSTIME_EVENT_DISPLAY );
		UiHideGroup( BONUSTIME_EVENT_DISPLAY );
	}


	{
        AP_IFriendWindow apFriendWindow = cFactory.CreateFriendWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pFriendWindow = apFriendWindow.release();
        m_pFriendWindow->CreateUIWindowAndRegisterOwnership();
    }

	{
		AP_IChatGroupInviteWindow apChatGroupInviteWindow = cFactory.CreateChatGroupInviteWindow( m_pGaeaClient, this, m_pEngineDevice );
		m_pChatGroupInviteWindow = apChatGroupInviteWindow.release();
		m_pChatGroupInviteWindow->CreateUIWindowAndRegisterOwnership();
	}
	

    {
        AP_IClubWindow apClubWindow = cFactory.CreateClubWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pClubWindow = apClubWindow.release();
        m_pClubWindow->CreateUIWindowAndRegisterOwnership();
	}

	{
		if( GLUseFeatures::GetInstance().IsUsingPrivateMarketSearchBuy() )
		{
 		AP_IPrivateMarketSearchBuy apPrivateMarketSearchBuy = cFactory.CreatePrivateMarketSearchBuy( m_pGaeaClient, this, m_pEngineDevice );
 		m_pPrivateMarketSearchBuy = apPrivateMarketSearchBuy.release();
 		m_pPrivateMarketSearchBuy->CreateUIWindowAndRegisterOwnership();
		}
	}

	{
		m_pSubMasterSet = new CSubMasterSet(m_pGaeaClient, this, m_pEngineDevice);
		m_pSubMasterSet->Create( SUBMASTER_MAKE, "SUBMASTER_MAKE", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pSubMasterSet->CreateSubControl();
		UiRegisterControl ( m_pSubMasterSet );
		UiShowGroupFocus ( SUBMASTER_MAKE );
	}

	{
		CReceiveNoteWindow* pReceiveNote = new CReceiveNoteWindow ( this, m_pEngineDevice );
		pReceiveNote->Create( RECEIVE_NOTE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pReceiveNote->CreateBaseWindowLightGray( "RECEIVE_NOTE_WINDOW", (char*)ID2GAMEWORD("FRIEND_TAB_BUTTON", 5) );
		pReceiveNote->CreateSubControl();
		pReceiveNote->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );		
		UiRegisterControl( pReceiveNote );
		UiShowGroupFocus ( RECEIVE_NOTE_WINDOW );
	}

	{
		CWriteNoteWindow* pWirteNote = new CWriteNoteWindow ( this, m_pEngineDevice );
		pWirteNote->Create( WRITE_NOTE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pWirteNote->CreateBaseWindowLightGray( "WRITE_NOTE_WINDOW", (char*)ID2GAMEWORD("FRIEND_TAB_BUTTON", 4) );
		pWirteNote->CreateSubControl();
		pWirteNote->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );		
		UiRegisterControl( pWirteNote );
		UiShowGroupFocus ( WRITE_NOTE_WINDOW );
	}

// #if defined(KRT_PARAM)
// 	{
// 		CSMSSendWindow* pSMSSendWindow = new CSMSSendWindow(m_pGaeaClient, this);
// 		pSMSSendWindow->Create( SMS_SEND_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
// 		pSMSSendWindow->CreateBaseWindowLightGray( "SMS_SEND_WINDOW", (char*)ID2GAMEWORD("FRIEND_BUTTON", 12) );
// 		pSMSSendWindow->CreateSubControl();
// 		pSMSSendWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );		
// 		UiRegisterControl( pSMSSendWindow );
// 		UiShowGroupFocus ( SMS_SEND_WINDOW );
// 	}
// #endif

	{
        CUIColorSelectorWindow * pHairColorCard = new CUIColorSelectorWindow( m_pGaeaClient, this, m_pEngineDevice );
        pHairColorCard->Create ( HAIRCOLORCARD_WINDOW, "COLORSELECTOR_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
        pHairColorCard->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
        pHairColorCard->CreateSubControl( 
            pd3dDevice,
            (char*)ID2GAMEWORD("HAIRCOLORCARD_WINDOW_TITLE"),
            boost::bind( boost::mem_fn( &GLCharacter::CallbackHairColorCreate ),  m_pGaeaClient->GetCharacter(), _1 ),
            boost::bind( boost::mem_fn( &GLCharacter::CallbackHairColorInit ),    m_pGaeaClient->GetCharacter(), _1 ),
            boost::bind( boost::mem_fn( &GLCharacter::CallbackHairColorUpdate ),  m_pGaeaClient->GetCharacter(), _1 ), 
            boost::bind( boost::mem_fn( &GLCharacter::CallbackHairColorCommit ),  m_pGaeaClient->GetCharacter(), _1 ) 
            );

        UiRegisterControl( pHairColorCard );
        UiShowGroupFocus( HAIRCOLORCARD_WINDOW );


        CUIColorSelectorWindow * pPetColorCard = new CUIColorSelectorWindow( m_pGaeaClient, this, m_pEngineDevice );
        pPetColorCard->Create ( PETCOLORCARD_WINDOW, "COLORSELECTOR_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
        pPetColorCard->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
        pPetColorCard->CreateSubControl( 
            pd3dDevice,
            (char*)ID2GAMEWORD("PETCOLORCARD_WINDOW_TITLE"),
            boost::bind( boost::mem_fn( &GLPetClient::CallbackPetColorCreate ),  m_pGaeaClient->GetPetClient(), _1 ),
            boost::bind( boost::mem_fn( &GLPetClient::CallbackPetColorInit ),    m_pGaeaClient->GetPetClient(), _1 ),
            boost::bind( boost::mem_fn( &GLPetClient::CallbackPetColorUpdate ),  m_pGaeaClient->GetPetClient(), _1 ), 
            boost::bind( boost::mem_fn( &GLPetClient::CallbackPetColorCommit ),  m_pGaeaClient->GetPetClient(), _1 ) 
            );

		UiRegisterControl( pPetColorCard );
		UiShowGroupFocus( PETCOLORCARD_WINDOW );
	}
	
	{
		CHairStyleCard * pHairStyleCard = new CHairStyleCard ( m_pGaeaClient, this, m_pEngineDevice );
		pHairStyleCard->Create( HAIRSTYLECARD_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pHairStyleCard->CreateBaseWindowLightGray( "HAIRSTYLECARD_WINDOW", NULL );
		pHairStyleCard->CreateSubControl();
		pHairStyleCard->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		{	// Note : ���� ������ Ÿ��Ʋ�� ���⼭ �Է��ߴ�.
			// Ÿ��Ʋ �Է��� ������ ����� ����ǹǷ�
			// 80�� �Ѵ� ��Ʈ���� ©���� ������ �����.
			// �׷��� �����찡 �������� �� �Ŀ� Ÿ��Ʋ�� �����ϸ� ���������� ��µȴ�.
			pHairStyleCard->SetTitleName( (char*)ID2GAMEWORD("HAIRSTYLECARD_WINDOW_TITLE") );
		}
		UiRegisterControl( pHairStyleCard );

		CGenderChangeWindow * pGenderChangeWindow = new CGenderChangeWindow ( m_pGaeaClient, this, m_pEngineDevice );
		pGenderChangeWindow->Create( GENDER_CHANGE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pGenderChangeWindow->CreateBaseWindowLightGray( "GENDER_CHANGE_WINDOW", NULL );
		pGenderChangeWindow->CreateSubControl();
		pGenderChangeWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		{	
			pGenderChangeWindow->SetTitleName( (char*)ID2GAMEWORD("GENDER_CHANGE_WINDOW_TITLE") );
		}
		UiRegisterControl( pGenderChangeWindow );
		

		CPetStyleCard * pPetStyleCard = new CPetStyleCard ( m_pGaeaClient, this, m_pEngineDevice );
		pPetStyleCard->Create( PETSTYLECARD_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pPetStyleCard->CreateBaseWindowLightGray( "HAIRSTYLECARD_WINDOW", NULL );
		pPetStyleCard->CreateSubControl();
		pPetStyleCard->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		pPetStyleCard->SetTitleName( (char*)ID2GAMEWORD("PETSTYLECARD_WINDOW_TITLE") );
		UiRegisterControl( pPetStyleCard );
		//UiShowGroupFocus( PETSTYLECARD_WINDOW );
	}

	{
		if( GLUseFeatures::GetInstance().IsUsingRenewMapWindow() )
		{
			RnMapWindow* pMapWindow = new RnMapWindow(m_pGaeaClient, this, m_pEngineDevice);
			pMapWindow->Create( MAP_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pMapWindow->CreateSubControl();
			UiRegisterControl ( pMapWindow, true );
			UiShowGroupFocus ( MAP_WINDOW );
			m_pMapWindowInterface = pMapWindow;
		}
		else
		{
			CLargeMapWindow* pLargeMap = new CLargeMapWindow(m_pGaeaClient, this, m_pEngineDevice);
			pLargeMap->Create ( MAP_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pLargeMap->CreateBaseWindowLargeMap ( "LARGEMAP_WINDOW", (char*)ID2GAMEWORD("LARGEMAP_WINDOW_STATIC") );
			pLargeMap->CreateSubControl ();
			pLargeMap->SetProtectSizePoint ();
			pLargeMap->SetAlignFlag ( UI_FLAG_RIGHT | UI_FLAG_TOP );
			UiRegisterControl ( pLargeMap, true );
			UiShowGroupFocus ( MAP_WINDOW );
			m_pMapWindowInterface = pLargeMap; 
		}

	}

	{	// ITEMREBUILD_MARK
		m_pItemRebuildWindow = new CItemRebuild ( m_pGaeaClient, this, m_pEngineDevice );
		m_pItemRebuildWindow->Create( ITEM_REBUILD_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemRebuildWindow->CreateBaseWindowLightGray( "ITEM_REBUILD_WINDOW", NULL );
		m_pItemRebuildWindow->CreateSubControl();
		m_pItemRebuildWindow->ResetAlignFlag();
		m_pItemRebuildWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemRebuildWindow->SetTitleName( ID2GAMEINTEXT("ITEMREBUILD_WINDOW_TITLE") );
		UiRegisterControl( m_pItemRebuildWindow, true );
        UiRegisterNpcActionableControl( m_pItemRebuildWindow );
		UiShowGroupFocus( ITEM_REBUILD_WINDOW );
	}

	{	// ITEMREBUILD_MARK
		m_pItemGarbageWindow = new CItemGarbage ( m_pGaeaClient, this, m_pEngineDevice );
		m_pItemGarbageWindow->Create( ITEM_GARBAGE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemGarbageWindow->CreateBaseWindowLightGray( "ITEM_GARBAGE_WINDOW", NULL );
		m_pItemGarbageWindow->CreateSubControl();
		m_pItemGarbageWindow->ResetAlignFlag();
		m_pItemGarbageWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemGarbageWindow->SetTitleName( ID2GAMEINTEXT("ITEMGARBAGE_WINDOW_TITLE") );
        UiRegisterControl( m_pItemGarbageWindow, true );
		UiShowGroupFocus( ITEM_GARBAGE_WINDOW );
	}

	{
		m_pPetRebirthDialogue = new CPetRebirthDialogue ( m_pGaeaClient, this, m_pEngineDevice );	// Monster7j
		m_pPetRebirthDialogue->Create ( PET_REBIRTH_DIALOGUE,"BASIC_WINDOW",  UI_FLAG_XSIZE | UI_FLAG_YSIZE | UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y);
		m_pPetRebirthDialogue->CreateBaseWindowLightGray ( "PET_REBIRTH_DIALOGUE", (char*)ID2GAMEWORD("PET_REBIRTH_NAME_STATIC") );
		m_pPetRebirthDialogue->CreateSubControl ();
		UiRegisterControl ( m_pPetRebirthDialogue ,true);
		UiShowGroupFocus ( PET_REBIRTH_DIALOGUE );
	}	

	{
		m_pPetSkinMixImage = new CPetSkinMixImage(m_pGaeaClient, this, m_pEngineDevice);
		m_pPetSkinMixImage->CreateEx( PETSKIN_MIX_IMAGE_WINDOW, "PETSKIN_MIX_IMAGE_WINDOW" );
		m_pPetSkinMixImage->CreateSubControl();
		m_pPetSkinMixImage->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_CENTER_X );
		UiRegisterControl( m_pPetSkinMixImage );
		UiShowGroupFocus ( PETSKIN_MIX_IMAGE_WINDOW );
	}	

	{
		m_pBusWindow = new CBusWindow ( m_pGaeaClient, this, m_pEngineDevice );
		m_pBusWindow->Create ( BUS_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pBusWindow->CreateBaseWindowLightGray ( "BUS_WINDOW", (char*)ID2GAMEWORD("BUS_WINDOW_NAME_STATIC") );
		//m_pBusWindow->CreateBaseWindowBlack ( "BUS_WINDOW", (char*)ID2GAMEWORD("BUS_WINDOW_NAME_STATIC") );
		m_pBusWindow->CreateSubControl ();
		m_pBusWindow->ResetAlignFlag ();
		UiRegisterControl ( m_pBusWindow, true );
		UiShowGroupFocus ( BUS_WINDOW );
	}

    {
		m_pMaxChatWindow.clear();
		DWORD dwRoomCount = m_pGaeaClient->GetChatGroup()->GetMaxGroupChatOpen();
		if ( dwRoomCount > MAX_GROUP_CHAT_WINDOW_MAXCOUNT )
			dwRoomCount = MAX_GROUP_CHAT_WINDOW_MAXCOUNT;

		for ( DWORD i = 0; i < dwRoomCount; i++ )
		{
			CMaxChatWindow* pMaxChatWindow = new CMaxChatWindow ( m_pGaeaClient, this, m_pEngineDevice, pd3dDevice );
			pMaxChatWindow->Create ( MAX_GROUP_CHAT_WINDOW_START + i, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			//pMaxChatWindow->CreateBaseWindowBlack( "MAX_GROUP_CHAT_WINDOW", "" );
			pMaxChatWindow->CreateBaseWindowMaxGroupChat("MAX_GROUP_CHAT_WINDOW", "" );
			pMaxChatWindow->CreateSubControl ();
			pMaxChatWindow->ResetAlignFlag ();
			UiRegisterControl ( pMaxChatWindow, true );
			m_pMaxChatWindow.push_back(pMaxChatWindow);
			UiShowGroupFocus ( MAX_GROUP_CHAT_WINDOW_START + i );
		}

    }


    {
        m_pChatGroupWindow = new CChatGroupWindow ( m_pGaeaClient, this, m_pEngineDevice );
        //m_pChatGroupWindow->Create ( MIN_GROUP_CHAT_WINDOW, "BASIC_WINDOW", UI_FLAG_RIGHT );
        //m_pChatGroupWindow->CreateBaseWindowLightGray ( "MIN_GROUP_CHAT_WINDOW", (char*)ID2GAMEWORD("BUS_WINDOW_NAME_STATIC") );
        m_pChatGroupWindow->Create ( MIN_GROUP_CHAT_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
        m_pChatGroupWindow->CreateBaseWindowEmpty("MIN_GROUP_CHAT_WINDOW");
        m_pChatGroupWindow->CreateSubControl ();
        m_pChatGroupWindow->ResetAlignFlag ();
        m_pChatGroupWindow->SetAlignFlag( UI_FLAG_RIGHT);
        UiRegisterControl ( m_pChatGroupWindow, true );
        UiShowGroupFocus ( MIN_GROUP_CHAT_WINDOW );
    }


	{
		m_pAcademyConftDisplay = new CAcademyConftDisplay(m_pEngineDevice);
		m_pAcademyConftDisplay->Create ( ACADEMY_CONFT_DISPLAY, "ACADEMY_CONFT_DISPLAY", UI_FLAG_CENTER_X );
		m_pAcademyConftDisplay->CreateSubControl ();
		UiRegisterControl ( m_pAcademyConftDisplay );
		UiShowGroupTop ( ACADEMY_CONFT_DISPLAY );
		UiHideGroup ( ACADEMY_CONFT_DISPLAY );
	}

//#if defined(VN_PARAM) //vietnamtest%%%
	if (sp == SP_VIETNAM)
	{	//	��Ʈ�� Ž�� ���� �κ��丮
		m_pVNGainSysInventory = new CVNGainSysInventory(m_pGaeaClient, this, m_pEngineDevice);
		m_pVNGainSysInventory->Create ( VNGAINSYS_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pVNGainSysInventory->CreateBaseWindowLightGray ( "VNGAINSYS_WINDOW", (char*)ID2GAMEWORD("VNGAINSYSTEM_NAME_STATIC") );
		m_pVNGainSysInventory->CreateSubControl ();
		m_pVNGainSysInventory->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		UiRegisterControl ( m_pVNGainSysInventory, true );
		UiShowGroupFocus ( VNGAINSYS_WINDOW );
//		UiHideGroup ( VNGAINSYS_WINDOW );
		
		//	��Ʈ�� Ž�� ���� ������
		m_pVNGainSysGauge = new CVNGainSysGauge(m_pEngineDevice);
		m_pVNGainSysGauge->Create ( VNGAINSYS_GAUGE_WINDOW, "VNGAINSYS_GAUGE_WINDOW", UI_FLAG_RIGHT );
		m_pVNGainSysGauge->CreateSubControl ();
		UiRegisterControl ( m_pVNGainSysGauge );
		UiShowGroupBottom ( VNGAINSYS_GAUGE_WINDOW );
//		UiShowGroupFocus ( VNGAINSYS_GAUGE_WINDOW );
	}
//#endif

	
	{	
		m_pPrivateMarketWindow = new CPrivateMarketWindow ( m_pGaeaClient, this, m_pEngineDevice );		
		m_pPrivateMarketWindow->Create ( PRIVATE_MARKET_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPrivateMarketWindow->CreateBaseWindowLightGray ( "PRIVATE_MARKET_WINDOW", (char*)ID2GAMEWORD("PRIVATE_MARKET_NAME_STATIC") );
		m_pPrivateMarketWindow->CreateSubControl ();
		m_pPrivateMarketWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		UiRegisterControl ( m_pPrivateMarketWindow, true );
		UiShowGroupFocus ( PRIVATE_MARKET_WINDOW );
		UiHideGroup ( PRIVATE_MARKET_WINDOW );
	}

	{
		m_pStorageChargeCard = new CStorageChargeCard ( m_pGaeaClient, this, m_pEngineDevice );
		m_pStorageChargeCard->Create ( STORAGE_CHARGE_CARD, "STORAGE_CHARGE_CARD", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pStorageChargeCard->CreateSubControl ();
		UiRegisterControl ( m_pStorageChargeCard );
		UiShowGroupFocus ( STORAGE_CHARGE_CARD );
	
    }

    {
        AP_IRebirthWindow apRebirthWindow = cFactory.CreateRebirthWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pRebirthWindow = apRebirthWindow.release();
        m_pRebirthWindow->CreateUIWindowAndRegisterOwnership();
    }

    {
        m_pForceRebirth = new CForceRebirth( m_pGaeaClient, this, m_pEngineDevice );
        m_pForceRebirth->Create( FORCE_REBIRTH, "FORCE_REBIRTH", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
        m_pForceRebirth->CreateSubControl();
        UiRegisterControl( m_pForceRebirth );
        UiShowGroupFocus( FORCE_REBIRTH );
    }

	{
		m_pRebirthDialogue = new CRebirthDialogue ( m_pGaeaClient, this, m_pEngineDevice );
		m_pRebirthDialogue->Create ( REBIRTH_DIALOGUE, "REBIRTH_DIALOGUE", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pRebirthDialogue->CreateSubControl ();
		UiRegisterControl ( m_pRebirthDialogue );
		UiShowGroupFocus ( REBIRTH_DIALOGUE );
	}

	{
		CWarningMsg* pWarningMsg = new CWarningMsg(m_pEngineDevice);
		pWarningMsg->CreateEx( WARNING_MSG_WINDOW, "WARNING_MSG_WINDOW" );
		pWarningMsg->CreateSubControl();
		UiRegisterControl( pWarningMsg );
	}

	{
		m_pCountMsgWindow = new CCountMsg ( this, m_pEngineDevice );
		m_pCountMsgWindow->Create( COUNT_MSG_WINDOW, "COUNT_MSG_WINDOW", UI_FLAG_CENTER_Y );
		m_pCountMsgWindow->CreateSubControl();
		UiRegisterControl( m_pCountMsgWindow );
	}

	{
		m_pPrivateMarketMake = new CPrivateMarketMake ( m_pGaeaClient, this, m_pEngineDevice );
		m_pPrivateMarketMake->Create ( PRIVATE_MARKET_MAKE, "PRIVATE_MARKET_MAKE", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pPrivateMarketMake->CreateSubControl ();
		UiRegisterControl ( m_pPrivateMarketMake );
		UiShowGroupFocus ( PRIVATE_MARKET_MAKE );		

		m_pClubMake = new CClubMake(m_pGaeaClient, this, m_pEngineDevice);
		m_pClubMake->Create ( CLUB_MAKE, "PRIVATE_MARKET_MAKE", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pClubMake->CreateSubControl ();
		UiRegisterControl ( m_pClubMake );
		UiShowGroupFocus ( CLUB_MAKE );		
	}

	{
		m_pQuestionItemDisplay = new CQuestionItemDisplay ( m_pGaeaClient, this, m_pEngineDevice );		
		m_pQuestionItemDisplay->Create ( QUESTION_ITEM_DISPLAY, "QUESTION_ITEM_DISPLAY" );
		m_pQuestionItemDisplay->CreateSubControl ();
		UiRegisterControl ( m_pQuestionItemDisplay );
		UiShowGroupTop ( QUESTION_ITEM_DISPLAY );
	}

	{
		m_pConftConfirm = new CConftConfirm(m_pEngineDevice);
		m_pConftConfirm->Create ( CONFT_CONFIRM, "CONFT_CONFIRM", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pConftConfirm->CreateSubControl ();
		UiRegisterControl ( m_pConftConfirm );
		UiShowGroupTop ( CONFT_CONFIRM );
		UiHideGroup ( CONFT_CONFIRM );
	}

	{	//	��
		m_pPetWindow = new CPetWindow ( m_pGaeaClient, this, m_pEngineDevice );
		m_pPetWindow->Create ( PET_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPetWindow->CreateBaseWindowLightGray ( "PET_WINDOW", (char*)ID2GAMEWORD("PET_NAME_STATIC") );
		m_pPetWindow->CreateSubControl ();
		m_pPetWindow->SetAlignFlag ( UI_FLAG_CENTER_Y );
		UiRegisterControl ( m_pPetWindow, true );
		UiShowGroupFocus ( PET_WINDOW );

		CPetSkillWindow* pPetSkillWindow = new CPetSkillWindow ( m_pGaeaClient, this, m_pEngineDevice );
		pPetSkillWindow->Create ( PET_SKILL_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pPetSkillWindow->CreateBaseWindowLightGray ( "PET_SKILL_WINDOW", (char*)ID2GAMEWORD("PET_NAME_STATIC", 1) );
		pPetSkillWindow->CreateSubControl ();
		pPetSkillWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		UiRegisterControl( pPetSkillWindow, true );
		UiShowGroupFocus( PET_SKILL_WINDOW );
	}

	{
		// �ڽ�Ƭ �÷� ����
        m_pCostumColorChangeWindow = new CUIColorSelectorWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pCostumColorChangeWindow->Create       ( COSTUM_COLOR_CHANGE_WINDOW, "COLORSELECTOR_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
        m_pCostumColorChangeWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );

        m_pCostumColorChangeWindow->CreateSubControl( 
            uiman::GetInnerUIMan().GetDevice(),
            (char*)ID2GAMEWORD("COSTUM_COLOR_STATIC"),
            boost::bind( boost::mem_fn( &GLCharacter::CallbackCostumColorCreate ),  m_pGaeaClient->GetCharacter(), _1 ),
            boost::bind( boost::mem_fn( &GLCharacter::CallbackCostumColorInit ),    m_pGaeaClient->GetCharacter(), _1 ),
            boost::bind( boost::mem_fn( &GLCharacter::CallbackCostumColorUpdate ),  m_pGaeaClient->GetCharacter(), _1 ), 
            boost::bind( boost::mem_fn( &GLCharacter::CallbackCostumColorCommit ),  m_pGaeaClient->GetCharacter(), _1 ) 
            );

		UiRegisterControl ( m_pCostumColorChangeWindow, true );
		UiShowGroupFocus ( COSTUM_COLOR_CHANGE_WINDOW );
	}
	
	{	// Ż��
        AP_IVehicleWindow apVehicleWindow = cFactory.CreateVehicleWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pVehicleWindow = apVehicleWindow.release();
        m_pVehicleWindow->CreateUIWindowAndRegisterOwnership();

	}

	{
		//����ũ
		m_pBikeWindow = new CBikeWindow ( m_pGaeaClient, this, m_pEngineDevice );
		m_pBikeWindow->Create ( BIKE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pBikeWindow->CreateBaseWindowLightGray ( "BIKE_WINDOW", (char*)ID2GAMEWORD("VEHICLE_NAME_STATIC",1 ) );
		m_pBikeWindow->CreateSubControl ();
		m_pBikeWindow->SetAlignFlag ( UI_FLAG_CENTER_Y );
		UiRegisterControl ( m_pBikeWindow, true );
		UiShowGroupFocus ( BIKE_WINDOW );
	}

	{
		//Ż�� �÷� ����
		m_pVehicleColorChangeWindow = new CUIColorSelectorWindow ( m_pGaeaClient, this, m_pEngineDevice );
		m_pVehicleColorChangeWindow->Create ( VEHICLE_COLOR_CHANGE_WINDOW, "COLORSELECTOR_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
        m_pVehicleColorChangeWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );

        m_pVehicleColorChangeWindow->CreateSubControl( 
            uiman::GetInnerUIMan().GetDevice(),
            (char*)ID2GAMEWORD("VEHICLE_NAME_STATIC",0 ),
            boost::bind( boost::mem_fn( &GLCharacter::CallbackVehicleColorCreate ),  m_pGaeaClient->GetCharacter(), _1 ),
            boost::bind( boost::mem_fn( &GLCharacter::CallbackVehicleColorInit ),    m_pGaeaClient->GetCharacter(), _1 ),
            boost::bind( boost::mem_fn( &GLCharacter::CallbackVehicleColorUpdate ),  m_pGaeaClient->GetCharacter(), _1 ), 
            boost::bind( boost::mem_fn( &GLCharacter::CallbackVehicleColorCommit ),  m_pGaeaClient->GetCharacter(), _1 ) 
            );

		UiRegisterControl ( m_pVehicleColorChangeWindow, true );
		UiShowGroupFocus ( VEHICLE_COLOR_CHANGE_WINDOW );
	}

	{

		//����ũ ������
		m_pBikeGetOffWindow = new CBikeGetOffWindow ( m_pGaeaClient, m_pEngineDevice );
		m_pBikeGetOffWindow->Create ( BIKE_GET_OFF_WINDOW, "BIKE_GET_OFF_WINDOW", UI_FLAG_DEFAULT );
		m_pBikeGetOffWindow->CreateSubControl ();
		UiRegisterControl ( m_pBikeGetOffWindow );
		UiShowGroupTop ( BIKE_GET_OFF_WINDOW );
		UiHideGroup ( BIKE_GET_OFF_WINDOW );
	}

	{
		//����ũ �޼���
		m_pBikeMessageWindow = new CBikeMessageWindow(m_pEngineDevice);
		m_pBikeMessageWindow->Create ( BIKE_MESSAGE_WINDOW, "BIKE_MESSAGE_DISPLAY", UI_FLAG_CENTER_X );
		m_pBikeMessageWindow->CreateSubControl ();
		UiRegisterControl ( m_pBikeMessageWindow );
		UiShowGroupFocus ( BIKE_MESSAGE_WINDOW );
	}

	{
		//����ũ �ν���
		m_pBikeBoostWindow = new CBikeBoostWindow ( m_pGaeaClient, m_pEngineDevice );
		m_pBikeBoostWindow->Create ( BIKE_BOOST_WINDOW, "BOOST_WINDOW", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pBikeBoostWindow->CreateSubControl ();
		UiRegisterControl ( m_pBikeBoostWindow );
		UiShowGroupFocus ( BIKE_BOOST_WINDOW );
		UiHideGroup ( BIKE_BOOST_WINDOW );
	}

	{
		// �ڵ��� ������;
		m_pCarWindow = new CCarWindow( m_pGaeaClient,this, m_pEngineDevice );
		m_pCarWindow->Create( CAR_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pCarWindow->CreateBaseWindowLightGray ( "CAR_WINDOW", (char*)ID2GAMEWORD("VEHICLE_NAME_STATIC",1 ) );
		m_pCarWindow->CreateSubControl();
		m_pBikeWindow->SetAlignFlag ( UI_FLAG_CENTER_Y );
		UiRegisterControl( m_pCarWindow, true );
		UiShowGroupFocus( CAR_WINDOW );
	}

	{
		// ����ī ������;
		m_pRearcarWindow = new CRearcarWindow( m_pGaeaClient,this, m_pEngineDevice );
		m_pRearcarWindow->Create( REARCAR_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pRearcarWindow->CreateBaseWindowLightGray ( "REARCAR_WINDOW", (char*)ID2GAMEWORD("VEHICLE_NAME_STATIC",1 ) );
		m_pRearcarWindow->CreateSubControl();
		m_pRearcarWindow->SetAlignFlag ( UI_FLAG_CENTER_Y );
		UiRegisterControl( m_pRearcarWindow, true );
		UiShowGroupFocus( REARCAR_WINDOW );
	}


	{	// ������ �˻�
        AP_IShopItemSearchWindow apShopItemSearchWindow = cFactory.CreateShopItemSearchWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pShopItemSearchWindow = apShopItemSearchWindow.release();
        m_pShopItemSearchWindow->CreateUIWindowAndRegisterOwnership();
	
		// ������ �˻� ���
        AP_IItemSearchResultWindow apItemSearchResultWindow = cFactory.CreateItemSearchResultWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pItemSearchResultWindow = apItemSearchResultWindow.release();
        m_pItemSearchResultWindow->CreateUIWindowAndRegisterOwnership();
	}


	{
		m_pBasicInfoViewDummy = new CUIControl(m_pEngineDevice);
        m_pBasicInfoViewDummy->Create ( NO_ID, "BASIC_INFO_VIEW" );
		UiRegisterControl ( m_pBasicInfoViewDummy );
		m_pBasicInfoViewDummy->SetVisibleSingle( FALSE );

		m_pLeftTopGroupDummy = new CUIControl(m_pEngineDevice);
//#ifndef CH_PARAM // �߱� �������̽� ����
		m_pLeftTopGroupDummy->Create( NO_ID, "LEFTTOP_CONTROL_GROUP" );
//#else
//		m_pLeftTopGroupDummy->Create( NO_ID, "BASIC_QUICK_SKILL_SLOT" );
//#endif
		UiRegisterControl ( m_pLeftTopGroupDummy );
		m_pLeftTopGroupDummy->SetVisibleSingle( FALSE );

	}

/* // �߱� �������̽� ����
#ifdef CH_PARAM
		m_pBasicPotionTrayDummy = new CUIControl(m_pRenderDevice);
		m_pBasicPotionTrayDummy->CreateEx( NO_ID, "LEFTTOP_CONTROL_GROUP" );
		UiRegisterControl ( m_pBasicPotionTrayDummy );
		m_pBasicPotionTrayDummy->SetVisibleSingle( FALSE );

		m_pQuestAlarmDummy = new CUIControl(m_pRenderDevice);
		m_pQuestAlarmDummy->Create( NO_ID, "QUEST_ALARM", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		UiRegisterControl( m_pQuestAlarmDummy );
		m_pQuestAlarmDummy->SetVisibleSingle( FALSE );
#endif
*/

//	IP���ʽ�
//#if defined( TH_PARAM ) || defined( MYE_PARAM ) || defined(MY_PARAM) || defined( PH_PARAM ) //|| defined(_RELEASED)
	//if (sp == SP_THAILAND || sp == SP_MALAYSIA || sp == SP_MALAYSIA_EN || sp == SP_PHILIPPINES)
	{	// �±� ���̹����� ��ũ
		m_pThaiCCafeMark = new CThaiCCafeMark(m_pEngineDevice);
		m_pThaiCCafeMark->Create ( THAI_CCAFE_MARK, "THAI_CCAFE_MARK", UI_FLAG_RIGHT );
		m_pThaiCCafeMark->CreateSubControl ();
		UiRegisterControl ( m_pThaiCCafeMark );
		UiShowGroupBottom ( THAI_CCAFE_MARK );	
	}
//#endif

//#if defined ( JP_PARAM ) || defined(_RELEASED)
	if (sp == SP_JAPAN || sp == SP_OFFICE_TEST)
	{
		// ItemShopIcon
		m_pItemShopIconMan = new CItemShopIconMan(m_pGaeaClient, pd3dDevice, m_pEngineDevice);
		m_pItemShopIconMan->Create ( ITEM_SHOP_ICON_MAN, "ITEM_SHOP_ICON_MAN" );		
		UiRegisterControl ( m_pItemShopIconMan );
		UiShowGroupBottom ( ITEM_SHOP_ICON_MAN );
	}
//#endif 

	{
		m_pAttendanceBookWindow = new CAttendanceBookWindow ( m_pGaeaClient, this, m_pEngineDevice );
		m_pAttendanceBookWindow->Create ( ATTENDANCE_BOOK_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pAttendanceBookWindow->CreateBaseWindowLightGray ( "ATTENDANCE_BOOK_WINDOW", (char*)ID2GAMEWORD("ATTENDANCE_BOOK", 0) );
		m_pAttendanceBookWindow->CreateSubControl ();
		m_pAttendanceBookWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_CENTER_X );
		UiRegisterControl ( m_pAttendanceBookWindow, true );
		UiShowGroupFocus ( ATTENDANCE_BOOK_WINDOW );	
	}

	{
		m_pPartyLinkWindow = new CPartyLinkWindow ( m_pGaeaClient, this, m_pEngineDevice );
		m_pPartyLinkWindow->Create ( PARTY_LINK_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pPartyLinkWindow->CreateBaseWindowLightGray ( "PARTY_LINK_WINDOW", (char*)ID2GAMEWORD("PARTY_LINK_WINDOW", 0) );
		m_pPartyLinkWindow->CreateSubControl ();
		m_pPartyLinkWindow->ResetAlignFlag ();
		UiRegisterControl ( m_pPartyLinkWindow, true );
		UiShowGroupFocus ( PARTY_LINK_WINDOW );	
	}


	{
		CFaceStyleCard * pFaceChangeWindow = new CFaceStyleCard ( m_pGaeaClient, this, m_pEngineDevice );
		pFaceChangeWindow->Create( FACE_CHANGE_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pFaceChangeWindow->CreateBaseWindowLightGray( "FACESTYLECARD_WINDOW", NULL );
		pFaceChangeWindow->CreateSubControl();
		pFaceChangeWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		pFaceChangeWindow->SetTitleName( (char*)ID2GAMEWORD("FACE_CHANGE_WINDOW_TITLE") );
		
		UiRegisterControl( pFaceChangeWindow );
	}

	{
        AP_IMapRequireCheck apMapRequireCheckWindow = cFactory.CreateMapRequireCheck( m_pGaeaClient, this, m_pEngineDevice );
        m_pMapRequireCheckWindow = apMapRequireCheckWindow.release();
        m_pMapRequireCheckWindow->CreateUIWindowAndRegisterOwnership();
	}


	{
		m_pClubBattleModalWindow = new CClubBattleModal ( this, m_pEngineDevice );
		m_pClubBattleModalWindow->Create( CLUB_BATTLE_MODAL_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pClubBattleModalWindow->CreateBaseWindowLightGray( "CLUB_BATTLE_MODAL_WINDOW", NULL );
		m_pClubBattleModalWindow->CreateSubControl();
		m_pClubBattleModalWindow->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pClubBattleModalWindow->SetTitleName( (char*)ID2GAMEWORD("CLUB_BATTLE_MODAL_WINDOW_TITLE") );
		
		UiRegisterControl( m_pClubBattleModalWindow );

	}

	{
		m_pTaxiWindow = new CTaxiWindow ( m_pGaeaClient, this, m_pEngineDevice );
		m_pTaxiWindow->Create ( TAXI_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pTaxiWindow->CreateBaseWindowLightGray ( "TAXI_WINDOW", (char*)ID2GAMEWORD("TAXI_WINDOW_NAME_STATIC") );
		m_pTaxiWindow->CreateSubControl ();
		m_pTaxiWindow->ResetAlignFlag ();
        UiRegisterControl ( m_pTaxiWindow, true );
		UiShowGroupFocus ( TAXI_WINDOW );
	}

	{	// ITEMREBUILD_MARK
		m_pItemMixWindow = new CItemMixWindow ( m_pGaeaClient, this, m_pEngineDevice );
		m_pItemMixWindow->Create( ITEM_MIX_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemMixWindow->CreateBaseWindowLightGray( "ITEM_MIX_WINDOW", NULL );
		m_pItemMixWindow->CreateSubControl();
		m_pItemMixWindow->ResetAlignFlag();
		m_pItemMixWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemMixWindow->SetTitleName( ID2GAMEINTEXT("ITEM_MIX_WINDOW_TITLE") );
		UiRegisterControl( m_pItemMixWindow, true );
        UiRegisterNpcActionableControl( m_pItemMixWindow );
		UiShowGroupFocus( ITEM_MIX_WINDOW );
	}

	{
		m_pItemMixInvenWindow = new CItemMixInvenWindow ( m_pGaeaClient, this, m_pEngineDevice );
		m_pItemMixInvenWindow->Create( ITEM_MIX_INVEN_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pItemMixInvenWindow->CreateBaseWindowLightGray( "ITEM_MIX_INVEN_WINDOW", NULL );
		m_pItemMixInvenWindow->CreateSubControl();
		m_pItemMixInvenWindow->ResetAlignFlag();
		m_pItemMixInvenWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pItemMixInvenWindow->SetTitleName( ID2GAMEINTEXT("ITEM_MIX_INVEN_WINDOW_TITLE") );
		UiRegisterControl( m_pItemMixInvenWindow, true );
		UiShowGroupFocus( ITEM_MIX_INVEN_WINDOW );
		UiHideGroup( ITEM_MIX_INVEN_WINDOW );
	}

	{
		m_pGatherGauge = new CGatherGauge ( m_pGaeaClient, m_pEngineDevice );
		m_pGatherGauge->Create ( GATHER_GAUGE, "GATHER_GAUGE", UI_FLAG_CENTER_X );
		m_pGatherGauge->CreateSubControl ();
		UiRegisterControl ( m_pGatherGauge );
		UiShowGroupBottom ( GATHER_GAUGE );
		UiHideGroup ( GATHER_GAUGE );
	}

	// for tutorial UI. by luxes.
	{
		m_pMouseTip = new CMouseTip ( m_pGaeaClient, this, m_pEngineDevice );
		m_pMouseTip->Create ( MOUSETIP_WINDOW, "MOUSETIP_MAIN", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		//m_pMouseTip->Create ( MOUSETIP_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		//m_pMouseTip->CreateBaseWindowLightGray ( "MOUSETIP_MAIN", NULL );
		m_pMouseTip->CreateSubControl ();
		//m_pMouseTip->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		m_pMouseTip->SetAlignFlag ( UI_FLAG_CENTER_Y );
		UiRegisterControl ( m_pMouseTip, true );
		UiShowGroupFocus ( MOUSETIP_WINDOW );
		UiHideGroup( MOUSETIP_WINDOW );
	}

	{
        AP_ITutorialDialogue apTutorialDialogue = cFactory.CreateTutorialDialogue( m_pGaeaClient, this, m_pEngineDevice );
        m_pTutorialDialogue = apTutorialDialogue.release();
        m_pTutorialDialogue->CreateUIWindowAndRegisterOwnership();
	}

	{
		m_pTutorialDialogueExplainText = new CTutorialDialogueExplainText ( this, m_pEngineDevice );
		m_pTutorialDialogueExplainText->Create ( TUTORIAL_DIALOGUE_EXPLAIN_WINDOW, "TUTORIAL_DIALOGUE_EXPLAIN_TEXT_MAIN", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pTutorialDialogueExplainText->CreateSubControl ();
		m_pTutorialDialogueExplainText->SetAlignFlag ( UI_FLAG_CENTER_Y );
		UiRegisterControl ( m_pTutorialDialogueExplainText, true );
		UiShowGroupFocus ( TUTORIAL_DIALOGUE_EXPLAIN_WINDOW );
		UiHideGroup( TUTORIAL_DIALOGUE_EXPLAIN_WINDOW );
	}

	{
        AP_IExplainInterface apExplainInterface = cFactory.CreateExplainInterface( m_pGaeaClient, this, m_pEngineDevice );
        m_pExplainInterface = apExplainInterface.release();
        m_pExplainInterface->CreateUIWindowAndRegisterOwnership();
	}

	{
		m_pTutorialInterfaceLeft = new CTutorialInterfaceLeft ( m_pGaeaClient, this, m_pEngineDevice );
		m_pTutorialInterfaceLeft->Create ( TUTORIAL_INTERFACE_LEFT, "TUTORIAL_INTERFACE_EMPTY_LEFT" );
		m_pTutorialInterfaceLeft->CreateSubControl ();
		UiRegisterControl ( m_pTutorialInterfaceLeft, true );
		UiShowGroupFocus ( TUTORIAL_INTERFACE_LEFT );
		UiHideGroup( TUTORIAL_INTERFACE_LEFT );
	}

	{
		m_pTutorialInterfaceRight = new CTutorialInterfaceRight ( m_pGaeaClient, this, m_pEngineDevice );
		m_pTutorialInterfaceRight->Create ( TUTORIAL_INTERFACE_RIGHT, "TUTORIAL_INTERFACE_EMPTY_RIGHT" );
		m_pTutorialInterfaceRight->CreateSubControl ();
		UiRegisterControl ( m_pTutorialInterfaceRight, true );
		UiShowGroupFocus ( TUTORIAL_INTERFACE_RIGHT );
		UiHideGroup( TUTORIAL_INTERFACE_RIGHT );
	}

	{
		m_pTutorialInterfaceUp = new CTutorialInterfaceUp ( m_pGaeaClient, this, m_pEngineDevice );
		m_pTutorialInterfaceUp->Create ( TUTORIAL_INTERFACE_UP, "TUTORIAL_INTERFACE_EMPTY_UP" );
		m_pTutorialInterfaceUp->CreateSubControl ();
		UiRegisterControl ( m_pTutorialInterfaceUp, true );
		UiShowGroupFocus ( TUTORIAL_INTERFACE_UP );
		UiHideGroup( TUTORIAL_INTERFACE_UP );
	}

	{
		m_pTutorialInterfaceDown = new CTutorialInterfaceDown ( m_pGaeaClient, this, m_pEngineDevice );
		m_pTutorialInterfaceDown->Create ( TUTORIAL_INTERFACE_DOWN, "TUTORIAL_INTERFACE_EMPTY_DOWN" );
		m_pTutorialInterfaceDown->CreateSubControl ();
		UiRegisterControl ( m_pTutorialInterfaceDown, true );
		UiShowGroupFocus ( TUTORIAL_INTERFACE_DOWN );
		UiHideGroup( TUTORIAL_INTERFACE_DOWN );
	}

	{
		m_pTutorialInterfaceZoomIn = new CTutorialInterfaceZoomIn ( m_pGaeaClient, this, m_pEngineDevice );
		m_pTutorialInterfaceZoomIn->Create ( TUTORIAL_INTERFACE_ZOOM_IN, "TUTORIAL_INTERFACE_EMPTY_ZOOM_IN" );
		m_pTutorialInterfaceZoomIn->CreateSubControl ();
		UiRegisterControl ( m_pTutorialInterfaceZoomIn, true );
		UiShowGroupFocus ( TUTORIAL_INTERFACE_ZOOM_IN );
		UiHideGroup( TUTORIAL_INTERFACE_ZOOM_IN );
	}

	{
		m_pTutorialInterfaceZoomOut = new CTutorialInterfaceZoomOut ( m_pGaeaClient, this, m_pEngineDevice );
		m_pTutorialInterfaceZoomOut->Create ( TUTORIAL_INTERFACE_ZOOM_OUT, "TUTORIAL_INTERFACE_EMPTY_ZOOM_OUT" );
		m_pTutorialInterfaceZoomOut->CreateSubControl ();
		UiRegisterControl ( m_pTutorialInterfaceZoomOut, true );
		UiShowGroupFocus ( TUTORIAL_INTERFACE_ZOOM_OUT );
		UiHideGroup( TUTORIAL_INTERFACE_ZOOM_OUT );
	}

	{
        AP_IBalloonText apBalloonText = cFactory.CreateBalloonText( m_pGaeaClient, this, m_pEngineDevice );
        m_pBalloonText = apBalloonText.release();
        m_pBalloonText->CreateUIWindowAndRegisterOwnership();
	}

	{
		m_pMinimapTarget = new CMinimapTarget ( m_pGaeaClient, m_pEngineDevice );
		m_pMinimapTarget->Create ( MINIMAP_TARGET, "TUTORIAL_INTERFACE_MINIMAP_TARGET_MAIN" );
		m_pMinimapTarget->CreateSubControl ();
		UiRegisterControl ( m_pMinimapTarget, true );
		UiShowGroupTop ( MINIMAP_TARGET );
		UiHideGroup( MINIMAP_TARGET );
	}

	{
        AP_IBalloonSpaceBar apBalloonSpaceBar = cFactory.CreateBalloonSpaceBar( m_pGaeaClient, this, m_pEngineDevice );
        m_pBalloonSpaceBar = apBalloonSpaceBar.release();
        m_pBalloonSpaceBar->CreateUIWindowAndRegisterOwnership();
	}

	{
		m_pTutorialSkipButton = new CTutorialSkipButton ( this, m_pEngineDevice );
		m_pTutorialSkipButton->Create ( TUTORIAL_SKIP_BUTTON, "TUTORIAL_SKIP_BUTTON_BOX", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pTutorialSkipButton->CreateSubControl ();
		UiRegisterControl ( m_pTutorialSkipButton );
		UiShowGroupTop( TUTORIAL_SKIP_BUTTON );
		UiHideGroup( TUTORIAL_SKIP_BUTTON );
	}

	{
        AP_ITutorialViewStep apTutorialViewStep = cFactory.CreateTutorialViewStep( this, m_pEngineDevice );
        m_pTutorialViewStep = apTutorialViewStep.release();
        m_pTutorialViewStep->CreateUIWindowAndRegisterOwnership();
        
	}

	{
		m_pTutorialDamageAlarm = new CTutorialDamageAlarm ( this, m_pEngineDevice );
		m_pTutorialDamageAlarm->Create ( TUTORIAL_DAMAGE_ALARM, "TUTORIAL_DAMAGE_ALARM_BOX", UI_FLAG_RIGHT | UI_FLAG_BOTTOM );
		m_pTutorialDamageAlarm->CreateSubControl ();
		UiRegisterControl ( m_pTutorialDamageAlarm );		
		UiShowGroupTop( TUTORIAL_DAMAGE_ALARM );
		UiHideGroup( TUTORIAL_DAMAGE_ALARM );
	}

	{
		m_pExplainVarText = new CBasicVarTextBox(m_pEngineDevice);
		m_pExplainVarText->Create ( TUTORIAL_EXPLAIN_VAR_TEXT, "BASIC_VAR_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pExplainVarText->CreateSubControl ();
		m_pExplainVarText->SetVisibleSingle ( FALSE );
		UiRegisterControl ( m_pExplainVarText );

		m_pExplainVarText->SetTextNoSplit( ID2GAMEINTEXT("TUTORIAL_NEXT"), NS_UITEXTCOLOR::WHITE );

		UiShowGroupTop ( TUTORIAL_EXPLAIN_VAR_TEXT );
		UiHideGroup( TUTORIAL_EXPLAIN_VAR_TEXT );
	}

    {
        // �����Ȳ ���
        AP_IProgressDisplay apProgressDisplay = cFactory.CreateProgressDisplay( m_pGaeaClient, this, m_pEngineDevice );
        m_pProgressDisplay = apProgressDisplay.release();
        m_pProgressDisplay->CreateUIWindowAndRegisterOwnership();
    }

    {
        // ��Ȱ��Ϻ�
        AP_IStudentRecordWindow apStudentRecordWindow = cFactory.CreateStudentRecordWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pStudentRecordWindow = apStudentRecordWindow.release();
        m_pStudentRecordWindow->CreateUIWindowAndRegisterOwnership( pd3dDevice );

        if ( !IsStudentRecordOpenable() ) 
        {
            UiHideGroup( STUDENTRECORD_NOTIFY_BUTTON );
        }

        // �ӽ� Ư��Ȱ��
		//ActivityWindow* pWindow = new ActivityWindow ( m_pGaeaClient, this );
		//pWindow->Create ( SPECIAL_ACTIVITY_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		//pWindow->CreateBaseWindowLightGray ( "SPECIAL_ACTIVITY_WINDOW", (char*)ID2GAMEWORD("SPECIAL_ACTIVITY_WINDOW") );
		//pWindow->CreateSubControl ();
		//pWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_CENTER_X );
		//UiRegisterControl ( pWindow, true );
		//UiShowGroupFocus ( SPECIAL_ACTIVITY_WINDOW );
    }

    {
        m_pItemPreviewWindow = new CItemPreviewWindow(m_pGaeaClient, this, m_pEngineDevice);
        m_pItemPreviewWindow->Create ( ITEMPREVIEW_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
        m_pItemPreviewWindow->CreateBaseWindowLightGray ( "ITEMPREVIEW_WINDOW", (char*)ID2GAMEWORD("ITEMPREVIEW_TITLE") );
        m_pItemPreviewWindow->CreateSubControl ( pd3dDevice );
        m_pItemPreviewWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
        UiRegisterControl ( m_pItemPreviewWindow, true );
        UiShowGroupFocus ( ITEMPREVIEW_WINDOW );
    }

	if( GLUseFeatures::GetInstance().IsUsingRenewProduct() )
	{
        AP_IProductWindow apProductWindow = cFactory.CreateProductWindow( m_pGaeaClient, this, m_pEngineDevice );
        m_pProductWindow = apProductWindow.release();
        m_pProductWindow->CreateUIWindowAndRegisterOwnership();

        AP_IProductProgress apProductProgress = cFactory.CreateProductProgress( m_pGaeaClient, this, m_pEngineDevice );
        m_pProductProgress = apProductProgress.release();
        m_pProductProgress->CreateUIWindowAndRegisterOwnership();

		AP_IProductResetWindow apProductResetWindow = cFactory.CreateProductResetWindow( m_pGaeaClient, this, m_pEngineDevice );
		m_pProductResetWindow = apProductResetWindow.release();
		m_pProductResetWindow->CreateUIWindowAndRegisterOwnership();
	}


    if( GLUseFeatures::GetInstance().IsUsingReferChar() )
    {
        CReferCharWindow* pReferCharWindow = new CReferCharWindow( m_pGaeaClient, this, m_pEngineDevice );
        pReferCharWindow->Create( REFERCHAR_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
        pReferCharWindow->CreateSubControl();
        UiRegisterControl(pReferCharWindow);
        UiShowGroupFocus( REFERCHAR_WINDOW );

        m_pReferCharWindow = pReferCharWindow;
    }

    {
        ItemRepairWindow * pItemRepairWindow = new ItemRepairWindow( m_pGaeaClient, this, m_pEngineDevice );
        pItemRepairWindow->Create( ITEM_REPAIR_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
        pItemRepairWindow->CreateSubControl();
        UiRegisterControl(pItemRepairWindow);
        UiShowGroupFocus( ITEM_REPAIR_WINDOW );

        m_pItemRepairWindow = pItemRepairWindow;
    }

	{
		AP_IOddEvenWindow apOddEvenWindow = cFactory.CreateOddEvenWindow(m_pGaeaClient, this, m_pEngineDevice);
		m_pOddEvenWindow = apOddEvenWindow.release();
		m_pOddEvenWindow->CreateUIWindowAndRegisterOwnership();
	}

	if( GLUseFeatures::GetInstance().IsUsingMiniGameTexasHoldem() )
	{
		AP_ITexasHoldemWindow apTexasHoldemWindow = cFactory.CreateTexasHoldemWindow( m_pGaeaClient, this, m_pEngineDevice );
		m_pTexasHoldemWindow = apTexasHoldemWindow.release();
		m_pTexasHoldemWindow->CreateUIWindowAndRegisterOwnership();

		AP_ITexasHoldemWebWindow apTexasHoldemWebWindow = cFactory.CreateTexasHoldemWebWindow( m_pGaeaClient, this, m_pEngineDevice );
		m_pTexasHoldemWebWindow = apTexasHoldemWebWindow.release();
		m_pTexasHoldemWebWindow->CreateUIWindowAndRegisterOwnership();
	}

	if( GLUseFeatures::GetInstance().IsUsingCursorTargetInfo() )
	{
		AP_ICursorTargetInfo apCursorTargetInfo = cFactory.CreateCursorTargetInfo( m_pGaeaClient, this, m_pEngineDevice );
		m_pCursorTargetInfo = apCursorTargetInfo.release();
		m_pCursorTargetInfo->CreateUIWindowAndRegisterOwnership();
	}

	{
		AP_IPVEUserInterface apPVEUserInterface = cFactory.CreatePVEUserInterface( m_pGaeaClient, this, m_pEngineDevice );
		m_pPVEUserInterface = apPVEUserInterface.release();
		m_pPVEUserInterface->CreateUIWindowAndRegisterOwnership();
	}

	


	if ( GLUseFeatures::GetInstance().IsUsingStampCard() )
	{
		AP_IItemRebuildRenewal apItemRebuildRenewal = cFactory.CreateItemRebuildRenewal( m_pGaeaClient, this, m_pEngineDevice );
		m_pItemRebuildRenewal = apItemRebuildRenewal.release();
		m_pItemRebuildRenewal->CreateUIWindowAndRegisterOwnership();
	}

	//���α׷��� Ÿ�̸�
	{
		AP_IProgessTimer apProgessTimer = cFactory.CreateProgessTimer( m_pGaeaClient, this, m_pEngineDevice );
		m_pProgessTimer = apProgessTimer.release();
		m_pProgessTimer->CreateUIWindowAndRegisterOwnership();
	}

	// Lotto System;
	if( GLUseFeatures::GetInstance().IsUsingLottoSystem() )
	{
		AP_ILottoMainUI apLottoMainUI = cFactory.CreateLottoMainUI( this, m_pEngineDevice );
		m_pLottoMainUI = apLottoMainUI.release();
		m_pLottoMainUI->CreateUIWindowAndRegisterOwnership();  

		AP_ILottoBuyLottoUI apLottoBuyLottoUI = cFactory.CreateLottoBuyLottoUI( this, m_pEngineDevice );
		m_pLottoBuyLottoUI = apLottoBuyLottoUI.release();
		m_pLottoBuyLottoUI->CreateUIWindowAndRegisterOwnership();

		AP_ILottoBuyListInfo apLottoBuyListInfo = cFactory.CreateLottoBuyListInfo( this, m_pEngineDevice );
		m_pLottoBuyListInfo = apLottoBuyListInfo.release();
		m_pLottoBuyListInfo->CreateUIWindowAndRegisterOwnership();

		AP_ILottoWinManListInfo apLottoWinManListInfo = cFactory.CreateLottoWinManListInfo( this, m_pEngineDevice );
		m_pLottoWinManListInfo = apLottoWinManListInfo.release();
		m_pLottoWinManListInfo->CreateUIWindowAndRegisterOwnership();

		AP_ILottoDisplayUI apLottoDisplayUI = cFactory.CreateLottoDisplayUI( this, m_pEngineDevice );
		m_pLottoDisplayUI = apLottoDisplayUI.release();
		m_pLottoDisplayUI->CreateUIWindowAndRegisterOwnership();

		GLLottoSystemManUI::Instance()->SetInterface( this );
		GLLottoSystemManUI::Instance()->Initialize();
	}

    {
        AP_IColorProperty apColorProperty = cFactory.CreateColorProperty( m_pGaeaClient, this, m_pEngineDevice );
        m_pColorProperty = apColorProperty.release();
        m_pColorProperty->CreateUIWindowAndRegisterOwnership();
    }

    //�δ� ��ũ��Ʈ�� UI
    if ( GLUseFeatures::GetInstance().IsUsingNewInstance() )
    {
        //�δ��� ���� UI
        {
            AP_ISimpleSystemMessageDisplay apSimpleSystemMessageDisplay = cFactory.CreateSimpleSystemMessageDisplay( m_pGaeaClient, this, m_pEngineDevice );
            m_pSimpleSystemMessageDisplay = apSimpleSystemMessageDisplay.release();
            m_pSimpleSystemMessageDisplay->CreateUIWindowAndRegisterOwnerShip();

            AP_ISimpleTimerMessageBox apSimpleTimerMessageBox = cFactory.CreateSimpleTimerMessageBox( m_pGaeaClient, this, m_pEngineDevice );
            m_pSimpleTimerMessageBox = apSimpleTimerMessageBox.release();
            m_pSimpleTimerMessageBox->CreateUIWindowAndRegisterOwnership();
        }

        //�δ��� Renewal CDM 
        {
            //Rn CDM ���� ǥ��
            AP_IRnCDMProgress apRnCDMProgress = cFactory.CreateRnCDMProgress( m_pGaeaClient, this, m_pEngineDevice );
            m_pRnCDMProgress = apRnCDMProgress.release();
            m_pRnCDMProgress->CreateUIWindowAndRegisterOwnership();

            //Rn CDM ���� ǥ��
            AP_IRnCDMRankWindow apRnCDMRankWindow = cFactory.CreateRnCDMRankWindow( m_pGaeaClient, this, m_pEngineDevice );
            m_pRnCDMRankWindow = apRnCDMRankWindow.release();
            m_pRnCDMRankWindow->CreateUIWindowAndRegisterOwnership();

            //Rn CDM Ŭ������
            AP_IRnCDMClubInfo apRnCDMClubInfo = cFactory.CreateRnCDMClubInfo( m_pGaeaClient, this, m_pEngineDevice );
            m_pRnCDMClubInfo = apRnCDMClubInfo.release();
            m_pRnCDMClubInfo->CreateUIWindowAndRegisterOwnership();
        }        
        
        //�δ� CaptureTheFlag
        {
            AP_ICaptureTheFlagEffectMan apCaptureTheFlagEffectMan = cFactory.CreateCaptureTheFlagEffectMan(m_pGaeaClient, this, m_pEngineDevice);
            m_pCaptureTheFlagEffecMan = apCaptureTheFlagEffectMan.release();
            m_pCaptureTheFlagEffecMan->CreateUIWindowAndRegisterOwnership();
        }

        // �δ� GuidanceClubBattle
        {
            AP_IGuidanceAuthProgressUI apGuidanceAuthProgressUI = cFactory.CreateGuidanceAuthProgressUI(m_pGaeaClient, this, m_pEngineDevice);
            m_pGuidanceAuthProgressUI = apGuidanceAuthProgressUI.release();
            m_pGuidanceAuthProgressUI->CreateUIWindowAndRegisterOwnership();

            AP_IGuidanceProgressInfoUI apGuidanceProgressInfoUI = cFactory.CreateGuidanceProgressInfoUI(m_pGaeaClient, this, m_pEngineDevice);
            m_pGuidanceProgressInfoUI = apGuidanceProgressInfoUI.release();
            m_pGuidanceProgressInfoUI->CreateUIWindowAndRegisterOwnership();

            AP_IGuidanceRankInfoUI apGuidanceRankInfoUI = cFactory.CreateGuidanceRankInfoUI(m_pGaeaClient, this, m_pEngineDevice);
            m_pGuidanceRankInfoUI = apGuidanceRankInfoUI.release();
            m_pGuidanceRankInfoUI->CreateUIWindowAndRegisterOwnership();
        }

		// �δ� CTI
		{
			AP_ICTIAuthProgressUI apCTIAuthProgressUI = cFactory.CreateCTIAuthProgressUI(m_pGaeaClient, this, m_pEngineDevice);
			m_pCTIAuthProgressUI = apCTIAuthProgressUI.release();
			m_pCTIAuthProgressUI->CreateUIWindowAndRegisterOwnership();

			AP_ICTIProgressInfoUI apCTIProgressInfoUI = cFactory.CreateCTIProgressInfoUI(m_pGaeaClient, this, m_pEngineDevice);
			m_pCTIProgressInfoUI = apCTIProgressInfoUI.release();
			m_pCTIProgressInfoUI->CreateUIWindowAndRegisterOwnership();

			AP_ICTIRankInfoUI apCTIRankInfoUI = cFactory.CreateCTIRankInfoUI(m_pGaeaClient, this, m_pEngineDevice);
			m_pCTIRankInfoUI = apCTIRankInfoUI.release();
			m_pCTIRankInfoUI->CreateUIWindowAndRegisterOwnership();
		}
    }

	if ( GLUseFeatures::GetInstance().IsUsingTournament() )
	{
		AP_ITournamentWindowDashBoard apTournamentWindowDashBoard = cFactory.CreateTournamentDashBoardWindow(m_pGaeaClient,this,m_pEngineDevice);
		m_pTournamentDashBoard = apTournamentWindowDashBoard.release();
		m_pTournamentDashBoard->OnCreate();
	}

	if ( GLUseFeatures::GetInstance().IsUsingTournament() )
	{
		AP_ITournamentToToWindow apTournamentToToWindow = cFactory.CreateTournamentToToWindow(m_pGaeaClient,this,m_pEngineDevice);
		m_pTournamentToToWindow = apTournamentToToWindow.release();
		m_pTournamentToToWindow->OnCreate();
	}

	if ( GLUseFeatures::GetInstance().IsUsingTournament() )
	{
		AP_ITournamentMoreInfoWindow apTournamentMoreInfoWindow = cFactory.CreateTournamentMoreInfoWindow(m_pGaeaClient,this,m_pEngineDevice);
		m_pTournamentMoreInfoWindow = apTournamentMoreInfoWindow.release();
		m_pTournamentMoreInfoWindow->OnCreate();
	}

	if ( GLUseFeatures::GetInstance().IsUsingTournament() )
	{
		AP_ITournamentWindowUI apTournamentWindowUI= cFactory.CreateTournamentWindowUI(m_pGaeaClient,this,m_pEngineDevice);
		m_pTournamentUI = apTournamentWindowUI.release();
		m_pTournamentUI->OnCreate();
	}

	if ( GLUseFeatures::GetInstance().IsUsingTournament() )
	{
		AP_IPTournamentUserInterface apTournamentUserInterface = cFactory.CreateTournamentUserInterface( m_pGaeaClient, this, m_pEngineDevice );
		m_pTournamentUserInterface = apTournamentUserInterface.release();
		m_pTournamentUserInterface->CreateUIWindowAndRegisterOwnership();
	}

	if ( GLUseFeatures::GetInstance().IsUsingTournament() )
	{
		if (m_pEntranceConfirm == 0)
		{
			m_pEntranceConfirm = new CTournamentEntranceConfirm(m_pGaeaClient, this, m_pEngineDevice);

			if (m_pEntranceConfirm == 0)
			{
				return false;
			}

			m_pEntranceConfirm->Create(COMPETITION_TOURNAMENTENTRANCECONFIRM, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
			m_pEntranceConfirm->CreateBaseWindowBodyLightGray("COMPETITION_PVE_ENTRANCE_WINDOW");
			m_pEntranceConfirm->Create(COMPETITION_TOURNAMENTENTRANCECONFIRM, "COMPETITION_PVE_ENTRANCE_WINDOW", UI_FLAG_XRESRATE | UI_FLAG_YRESRATE);
			m_pEntranceConfirm->CreateSubControl();		
			
			UiRegisterControl(m_pEntranceConfirm);	
		}
	}

	if ( GLUseFeatures::GetInstance().IsUsingTournament() )
	{
		if(m_pTournamentObserveWindow == 0)
		{
			m_pTournamentObserveWindow = new TournamentObserveWindow(m_pGaeaClient, this, m_pEngineDevice);

			if (m_pTournamentObserveWindow == 0)
				return false;

			m_pTournamentObserveWindow->Create(COMPETITION_TOURNAMENT_OBSERVE, "TOURNAMENT_OBSERVE", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
			m_pTournamentObserveWindow->CreateSubControl();
			m_pTournamentObserveWindow->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );

			UiRegisterControl(m_pTournamentObserveWindow);

			m_pTournamentObserveWindow->SetVisibleSingle(false);
			UiHideGroup(COMPETITION_TOURNAMENT_OBSERVE);
			AddNonCloseUI( COMPETITION_TOURNAMENT_OBSERVE);

			//UI Ʈ���� �ֱ�

			MatchingSystem::GLTouranmentClient* glTour = m_pGaeaClient->GetTournamentClient();

			InstanceSystem::InstancePopUpTrigger * ptrigger = new InstanceSystem::InstancePopUpTrigger();
			ptrigger->RegistCallback(*this,&CInnerInterface::SetInstancePopUp);

			// Ʈ���� ����!
			glTour->SetInstancePopupEvent(ptrigger);

		}
	}


	if ( GLUseFeatures::GetInstance().IsUsingTournament() )
	{
		if(m_pTournamentMulty == NULL)
		{
			m_pTournamentMulty = new CTournamentMulty(m_pGaeaClient, this, m_pEngineDevice);
			if (m_pTournamentMulty == NULL)
			{
				return false;
			}

			m_pTournamentMulty->Create(COMPETITION_TOURNAMENT_MULTY, "TOURNAMENT_RIGHT_MULTY_TEAM_BG", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
			m_pTournamentMulty->CreateSubControl();
			m_pTournamentMulty->SetAlignFlag( UI_FLAG_CENTER_X );
			UiRegisterControl(m_pTournamentMulty);

			m_pTournamentMulty->SetVisibleSingle(false);
			UiHideGroup(COMPETITION_TOURNAMENT_MULTY);
			AddNonCloseUI( COMPETITION_TOURNAMENT_MULTY);
		}
	}

	if ( GLUseFeatures::GetInstance().IsUsingTournament() )
	{
		if(m_pBigGroupWindowRight == NULL)
		{

			m_pBigGroupWindowRight = new TournamentMiniExpeditionWindow(m_pGaeaClient, this, m_pEngineDevice);
			m_pBigGroupWindowRight->Create( COMPETITION_TOURNAMENTENT_BIG_RIGHT, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			m_pBigGroupWindowRight->CreateUIWindowAndRegisterOwnership(UI_FLAG_RIGHT | UI_FLAG_BOTTOM);
			m_pBigGroupWindowRight->CreateSubControl();
			UiRegisterControl(m_pBigGroupWindowRight);
			m_pBigGroupWindowRight->SetVisibleSingle(false);

			UiHideGroup(COMPETITION_TOURNAMENTENT_BIG_RIGHT);
			AddNonCloseUI( COMPETITION_TOURNAMENTENT_BIG_RIGHT);
		}
	}

	if ( GLUseFeatures::GetInstance().IsUsingTournament() )
	{
		if(m_pBigGroupWindowLeft == NULL)
		{

			m_pBigGroupWindowLeft = new TournamentMiniExpeditionWindow(m_pGaeaClient, this, m_pEngineDevice);
			m_pBigGroupWindowLeft->Create( COMPETITION_TOURNAMENTENT_BIG_LEFT, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			m_pBigGroupWindowLeft->CreateUIWindowAndRegisterOwnership(UI_FLAG_LEFT | UI_FLAG_BOTTOM);
			m_pBigGroupWindowLeft->CreateSubControl();
			UiRegisterControl(m_pBigGroupWindowLeft);
			m_pBigGroupWindowLeft->SetVisibleSingle(false);

			UiHideGroup(COMPETITION_TOURNAMENTENT_BIG_LEFT);
			AddNonCloseUI( COMPETITION_TOURNAMENTENT_BIG_LEFT);
		}
	}

	if ( GLUseFeatures::GetInstance().IsUsingTournament() )
	{
		if(m_pMiniGroupWindowRight == NULL)
		{

			m_pMiniGroupWindowRight = new CTournamentExpeditionInfoGroup(m_pGaeaClient, this, m_pEngineDevice);
			m_pMiniGroupWindowRight->Create( COMPETITION_TOURNAMENTENT_MINI_RIGHT, "MATCHING_WINDOW_RIGHT", UI_FLAG_RIGHT | UI_FLAG_CENTER_Y );
			//m_pMiniGroupWindowRight->CreateUIWindowAndRegisterOwnership(UI_FLAG_RIGHT | UI_FLAG_BOTTOM);
			m_pMiniGroupWindowRight->CreateSubControl();
			UiRegisterControl(m_pMiniGroupWindowRight);
			m_pMiniGroupWindowRight->SetVisibleSingle(false);

			UiHideGroup(COMPETITION_TOURNAMENTENT_MINI_RIGHT);
			AddNonCloseUI( COMPETITION_TOURNAMENTENT_MINI_RIGHT);
		}
	}

	if ( GLUseFeatures::GetInstance().IsUsingTournament() )
	{
		if(m_pMiniGroupWindowLeft == NULL)
		{

			m_pMiniGroupWindowLeft = new CTournamentExpeditionInfoGroup(m_pGaeaClient, this, m_pEngineDevice);
			m_pMiniGroupWindowLeft->Create( COMPETITION_TOURNAMENTENT_MINI_LEFT, "MATCHING_WINDOW_LEFT", UI_FLAG_LEFT | UI_FLAG_CENTER_Y );
			//m_pMiniGroupWindowLeft->CreateUIWindowAndRegisterOwnership(UI_FLAG_LEFT | UI_FLAG_BOTTOM);
			m_pMiniGroupWindowLeft->CreateSubControl();
			UiRegisterControl(m_pMiniGroupWindowLeft);
			m_pMiniGroupWindowLeft->SetVisibleSingle(false);

			UiHideGroup(COMPETITION_TOURNAMENTENT_MINI_LEFT);
			AddNonCloseUI( COMPETITION_TOURNAMENTENT_MINI_LEFT);
		}
	}

	if ( GLUseFeatures::GetInstance().IsUsingTournament() )
	{
		if (m_pTournamentIndicator == 0)
		{
			m_pTournamentIndicator = new CTournamentIndicator(m_pGaeaClient,this,m_pEngineDevice);

			m_pTournamentIndicator->Create(COMPETITION_TOURNAMENTENT_INDICATOR, "TOURNAMENT_INDICATOR_WINDOW", UI_FLAG_CENTER_X | UI_FLAG_TOP);
			//m_pIndicator->CreateSub(this, _T("CONTROL_PVE_INDICATOR"), UI_FLAG_XRESRATE | UI_FLAG_YRESRATE);
			m_pTournamentIndicator->CreateSubControl();

			m_pTournamentIndicator->SetVisibleSingle(false);

			UiRegisterControl(m_pTournamentIndicator);
			AddNonCloseUI( COMPETITION_TOURNAMENTENT_INDICATOR);
		}

	}

	{
		m_pDoublePoint = new DoublePoint ( m_pGaeaClient, this, m_pEngineDevice );		
		m_pDoublePoint->Create ( DOUBLE_POINT, "QUESTION_ITEM_DISPLAY" );
		m_pDoublePoint->CreateSubControl ();
		UiRegisterControl ( m_pDoublePoint, true  );
		UiShowGroupFocus( DOUBLE_POINT );
	}

	{
		AP_IFlyCameraControlWindow apFlyCameraControl = cFactory.CreateFlyCameraControlUI( m_pGaeaClient, this, m_pEngineDevice );
		m_pFlyCameraControl = apFlyCameraControl.release();
		m_pFlyCameraControl->CreateUIWindowAndRegisterOwnership();
	}

	{
		AP_ISetitemPreviewWindow apSetitemPreviewWindow = cFactory.CreateSetitemPreviewUI( m_pGaeaClient, this, m_pEngineDevice );
		m_pSetitemPreviewWindow = apSetitemPreviewWindow.release();
		m_pSetitemPreviewWindow->CreateUIWindowAndRegisterOwnership();
	}

	{
		AP_ISummonStateDisplayUI apSummonStateDisplayUI = cFactory.CreateSummonStateDisplayUI( m_pGaeaClient, this, m_pEngineDevice );
		m_pSummonStateDisplayUI = apSummonStateDisplayUI.release();
		m_pSummonStateDisplayUI->CreateUIWindowAndRegisterOwnership();
	}

	if ( GLUseFeatures::GetInstance().IsUsingFactionInfo() )
	{
		AP_IFactionInfoWindow apFactionInfoWindow = cFactory.CreateFactionInfoWindow( m_pGaeaClient, this, m_pEngineDevice );
		m_pFactionInfoWindow = apFactionInfoWindow.release();

		if ( m_pFactionInfoWindow )
			m_pFactionInfoWindow->CreateUIWindowAndRegisterOwnership();
	}

	{
		
		{
			m_pCostumeStatWindow = new CCostumeStatWindow( m_pGaeaClient, this, m_pEngineDevice );
			m_pCostumeStatWindow->CretaeWindow();
			m_pCostumeStatWindow->SetVisibleSingle(FALSE);
		}

	}
	
	{
		m_pSelformBoxWindow = new SelectiveformBoxWindow(m_pGaeaClient, this, m_pEngineDevice);
		m_pSelformBoxWindow->Create ( SELECTIVEFORMBOX_WINDOW, "BASIC_WINDOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pSelformBoxWindow->CreateBaseWindowLightGray ( "SELFORMBOX_WINDOW", (char*)ID2GAMEWORD("SELFORMBOX_WINDOW_NAME_STATIC") );
		m_pSelformBoxWindow->CreateSubControl ();
		m_pSelformBoxWindow->SetAlignFlag ( UI_FLAG_CENTER_Y | UI_FLAG_RIGHT );
		UiRegisterControl ( m_pSelformBoxWindow, true );
		UiShowGroupFocus ( SELECTIVEFORMBOX_WINDOW );
		UiHideGroup ( SELECTIVEFORMBOX_WINDOW );
	}


	{
		m_pAnimatedPKStreak = new CAnimatedPKStreak(m_pEngineDevice);
		m_pAnimatedPKStreak->Create ( PK_COMBO_DISPLAY, "PK_COMBO_DISPLAY", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pAnimatedPKStreak->CreateSubControl ();
		UiRegisterControl ( m_pAnimatedPKStreak, true );
		UiShowGroupTop ( PK_COMBO_DISPLAY );
	}

	// Kill Card System
	{
		m_pKillCardManager = new CKillCardManager(m_pEngineDevice);
		m_pKillCardManager->Create ( KILL_CARD_DISPLAY, "KILL_CARD_DISPLAY", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pKillCardManager->CreateSubControl ();
		UiRegisterControl ( m_pKillCardManager, true );
		UiShowGroupTop ( KILL_CARD_DISPLAY );
	}

    CWebWindowBase::s_bSkipVisibleWeb = FALSE;

	CloseAllWindow ();

	DxSoundLib::GetInstance()->CreateSound ( "QuestAlarm", ID2GAMEWORD("QUEST_ALARM"), SFX_SOUND );

	return uiman::GetInnerUIMan().InitDeviceObjects ( pd3dDevice );
}

HRESULT CInnerInterface::DeleteDeviceObjects ()
{
	GLCharacter *pCharacter = m_pGaeaClient->GetCharacter();
	if( pCharacter ) pCharacter->UPDATE_RECORD_CHAT( TRUE );

	TEXTURE_PRE_UNLOAD ();

	ResetControl();

	return uiman::GetInnerUIMan().DeleteDeviceObjects ();
}

HRESULT	CInnerInterface::TEXTURE_PRE_LOAD ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	CString strTemp = ID2GAMEWORD("SKILL_TEXTURE",0);
	int nCOUNT = atoi(strTemp);
	if ( nCOUNT < 1 )
        return S_FALSE;

	for ( int i=0; i<nCOUNT; ++i )
	{
		CString strKeyword = ID2GAMEWORD("SKILL_TEXTURE",i+1);

		TextureResource textureRes = NSTEXTUREMAN::TextureManager::GetInstance().LoadTexture(
            strKeyword.GetString(),
            false,
            TEXTURE_ADD_DATA::TEXUTRE_TYPE_2D,
            1,
            D3DFMT_UNKNOWN,
            TEXTURE_RESIZE_NONE,
            TEXTURE_UI,
			false,
			TRUE );

        //TextureManager::GetInstance().BackUpCache( textureRes );
	}

	return S_OK;
}

HRESULT	CInnerInterface::TEXTURE_PRE_UNLOAD ()
{
    CString strTemp = ID2GAMEWORD("SKILL_TEXTURE",0);
	int nCOUNT = atoi(strTemp);
	if ( nCOUNT < 1 )
        return S_FALSE;

	for ( int i=0; i<nCOUNT; ++i )
	{
		CString strKeyword = ID2GAMEWORD("SKILL_TEXTURE",i+1);

        NSTEXTUREMAN::TextureManager::GetInstance().DeleteData( strKeyword.GetString() );
	}

	return S_OK;
}

void CInnerInterface::ADD_FRIEND_NAME_TO_EDITBOX( const CString& strName )
{
	if( m_pFriendWindow ) 
		m_pFriendWindow->AddFriendName_To_EditBox( UI::ToString( strName ) );
}

void CInnerInterface::ADD_FRIEND( const CString& strName )
{
    if(strcmp( m_pGaeaClient->GetCharacter()->GetName(), strName ) == 0 )
	    return;

	if( m_pFriendWindow )
		m_pFriendWindow->AddFriend( UI::ToString( strName ) );
}

void CInnerInterface::ADD_FRIEND_BLOCK( const CString& strName, bool bBlock )
{
    if(strcmp( m_pGaeaClient->GetCharacter()->GetName(), strName ) == 0 )
        return;

    if( m_pFriendWindow )
        m_pFriendWindow->AddFriendBlock( strName.GetString(), bBlock );
}

void CInnerInterface::FRIEND_LIST( CONST CString & strName, bool bOnline )
{
	if( m_pBasicChat )
		m_pBasicChat->SetFriendOnlineList( UI::ToString( strName ), bOnline );
}

bool CInnerInterface::IsCHAT_BEGIN()
{
	if( m_pBasicChat )
		return m_pBasicChat->IsChatBegin();

	return false;
}

void CInnerInterface::ClearItemBank()
{
	if( m_pItemBankWindow )
		m_pItemBankWindow->ClearItemBank();
}

void CInnerInterface::ClearVNGainSys()
{
	if( m_pVNGainSysInventory )
		m_pVNGainSysInventory->ClearVNGainSys();
}

void CInnerInterface::DisableMinimapTarget()
{
	if (m_pMapWindowInterface)
		m_pMapWindowInterface->VISIBLE_TARGET_POS(FALSE);
}

void CInnerInterface::SET_COUNT_MSG( INT nCount )
{
	if( m_pCountMsgWindow )
	{
		m_pCountMsgWindow->SetCount( nCount );
		UiShowGroupTop( COUNT_MSG_WINDOW );
	}
}

void CInnerInterface::SetDefaultPosInterface(UIGUID ControlID)
{
	CUIControl * pControl = uiman::GetInnerUIMan().FindControl( ControlID );

	if( pControl )
	{
		pControl->RestoreDeviceObjects( uiman::GetInnerUIMan().GetDevice() );
	}
}

void CInnerInterface::ReqToggleRun()
{
	m_pGaeaClient->GetCharacter()->ReqToggleRun ();
}

VOID CInnerInterface::SET_VNGAINTYPE_GAUGE( int nPos, int nLimit )
{
	m_pVNGainSysGauge->SetGauge( nPos, nLimit );
}


void CInnerInterface::BONUS_TIME_EVENT_START( bool bCharging )
{
	m_pBonusTimeGauge->BONUS_TIME_EVENT_START( bCharging );
}
void CInnerInterface::BONUS_TIME_EVENT_END()
{
	m_pBonusTimeGauge->BONUS_TIME_EVENT_END();
}
void CInnerInterface::BONUS_TIME_BUSTER_START()
{
	m_pBonusTimeGauge->BONUS_TIME_BUSTER_START();
	UiShowGroupTop( BONUSTIME_EVENT_DISPLAY );
	m_pBonusTimeDisplay->START();
}

void CInnerInterface::BONUS_TIME_BUSTER_END()
{
	m_pBonusTimeGauge->BONUS_TIME_BUSTER_END();
	UiShowGroupTop( BONUSTIME_EVENT_DISPLAY );
	m_pBonusTimeDisplay->STOP();
}

bool CInnerInterface::SET_QUEST_HELPER( DWORD dwQuestID, bool bPrintMsg )
{
    bool bResult = m_pQuestHelper->SetQuestID( dwQuestID, bPrintMsg );

    if ( UiIsVisibleGroup(CDM_RANKING_DISPLAY) || UiIsVisibleGroup(GUIDANCE_RANK_INFO_UI) )
    {
        UiHideGroup( QUEST_HELPER_DISPLAY );
        return false;
    }

    return bResult;
}

void CInnerInterface::RESET_QUEST_HELPER( DWORD dwQuestID )
{
	m_pQuestHelper->ReSetQuestID( dwQuestID );
}

bool CInnerInterface::IS_SAME_QUEST_HELPER( DWORD dwQuestID )
{
    return m_pQuestHelper->IsSameQuest( dwQuestID );
}

void CInnerInterface::QUEST_HELPER_UPDATE () 
{
    m_pQuestHelper->UpdateQuestInfo();
}

void CInnerInterface::SetIPBonusClass( int nClass )
{
	if ( m_pThaiCCafeMark )	m_pThaiCCafeMark->SetClass( nClass );
}

void CInnerInterface::DoBattleModal( const CString strClubName, bool bAlliance )
{
	if ( !m_pClubBattleModalWindow ) return;
	
    m_pClubBattleModalWindow->SetClubName( strClubName );
	m_pClubBattleModalWindow->SetAlliance( bAlliance );
	UiShowGroupFocus( CLUB_BATTLE_MODAL_WINDOW );
}

BOOL CInnerInterface::ReqClubBattle( DWORD dwTime, bool bAlliance )
{
	DWORD dwGaeaID = m_pPtoPWindow->GetPlayerID ();

	if ( bAlliance ) 
		return m_pGaeaClient->ReqAllianceBattle( dwGaeaID, dwTime );
	else 
		return m_pGaeaClient->ReqClubBattle( dwGaeaID, dwTime );
}

void CInnerInterface::OPEN_TAXI_WINDOW( WORD wPosX, WORD wPosY )
{
	if ( !m_pTaxiWindow ) return;
	m_pTaxiWindow->SetInvenPos( wPosX, wPosY );
	UiShowGroupFocus( TAXI_WINDOW );
}

void CInnerInterface::SetMapCheckType(UI::EMMAPCHECK_TYPE emMapCheckType)
{
    m_pMapRequireCheckWindow->SetMapCheckType(emMapCheckType);
}

void CInnerInterface::UiRegisterNpcActionableControl( CUIControl* pControl )
{
    if ( !pControl )
    {
        return;
    }

    m_mmManageControl.insert( std::make_pair(UI::EMMANAGECONTROL_TYPE_NPCACTIONABLE, pControl) );
}

void CInnerInterface::InfoCTFDisplayUpdate()
{
    if ( m_pInfoCTFDisplay )
    {
        m_pInfoCTFDisplay->Update();
    }
}

void CInnerInterface::ReservedSkillAnimationShow( const SNATIVEID& sSkillID )
{
	if( m_pReservedSkillAnimation )
	{
		m_pReservedSkillAnimation->Show( sSkillID );
	}
}

void CInnerInterface::ReservedSkillAnimationHideAll()
{
	if ( m_pReservedSkillAnimation )
	{
		m_pReservedSkillAnimation->HideAll();
	}
}

void CInnerInterface::ResetKeyBinding()
{
	if ( m_pSkillTrayTab )
	{
		m_pSkillTrayTab->ResetSlotText();
	}

	if ( m_pUILeftTopGroup )
	{
		m_pUILeftTopGroup->ResetQuickSlotText();
	}

	if ( m_pGameMenu )
	{
		m_pGameMenu->InitShotCutString();
	}
}

void CInnerInterface::ToggleWindowEscMenu()
{
	if ( !UiIsVisibleGroup ( ESC_MENU ) )
	{
		UiShowGroupFocus ( ESC_MENU );
	}
	else
	{
		UiHideGroup ( ESC_MENU );
	}
}

void CInnerInterface::ToggleWindowInventory()
{
	if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
	{
		if ( !IsInventoryOpen() && !UiIsVisibleGroup ( TRADEINVENTORY_WINDOW ) )
		{
			m_pInventory->CallTapSelect( CRnInventoryWindow::ETapInven );
			UiShowGroupFocus( INVENTORY_WINDOW_RENEW );
		}
		else
		{
			UiHideGroup( INVENTORY_WINDOW_RENEW );
			if( UiIsVisibleGroup( PRODUCT_RESET_WINDOW ) )
			{
				UiHideGroup( PRODUCT_RESET_WINDOW );
			}
		}
	}
	else
	{
		if ( !IsInventoryOpen() && !UiIsVisibleGroup ( TRADEINVENTORY_WINDOW ) )
		{
			UiShowGroupFocus( INVENTORY_WINDOW );
		}
		else
		{
			UiHideGroup ( INVENTORY_WINDOW );
			if( UiIsVisibleGroup( PRODUCT_RESET_WINDOW ) )
			{
				UiHideGroup( PRODUCT_RESET_WINDOW );
			}
		}
	}
}

void CInnerInterface::ToggleWindowCharacter()
{
	if ( UiIsVisibleGroup( STATSRESET_WINDOW ) )
	{
		m_pGaeaClient->PrintMsgText( NS_UITEXTCOLOR::RED, ID2GAMEINTEXT("STATSRESET_ACTION_WARNING") );
	}
	else 
	{
		if ( !UiIsVisibleGroup ( CHARACTER_WINDOW ) )
			UiShowGroupFocus ( CHARACTER_WINDOW );
		else
			UiHideGroup ( CHARACTER_WINDOW );
	}
}

void CInnerInterface::ToggleWindowSkill()
{
	if ( UiIsVisibleGroup( STATSRESET_WINDOW ) )
	{
		m_pGaeaClient->PrintMsgText( NS_UITEXTCOLOR::RED, ID2GAMEINTEXT("STATSRESET_ACTION_WARNING") );
	}
	else 
	{
		if ( !UiIsVisibleGroup ( SKILL_WINDOW ) )
			UiShowGroupFocus ( SKILL_WINDOW );
		else
			UiHideGroup ( SKILL_WINDOW );
	}
}

void CInnerInterface::ToggleWindowMiniGame(const DWORD dwMiniGameID)
{
	if ( dwMiniGameID == GAEAID_NULL )
	{
		m_pOddEvenWindow->SetVisible(false);
		return;
	}

	CloseAllWindowFromNPC();

	switch ( dwMiniGameID )
	{
	case MiniGame::EMMINIGAME_ODDEVEN:
		m_pOddEvenWindow->SetVisible(true);
		break;
	default:
		return;
	}	
}

void CInnerInterface::OpenTexasHoldemWindow()
{
	if( GLUseFeatures::GetInstance().IsUsingMiniGameTexasHoldem() )
	{
		if( !UiIsVisibleGroup(MINIGAME_TEXASHOLDEM_WINDOW) )
		{
			m_pTexasHoldemWindow->SetVisible(true);
			m_pInventory->LockInven(MINIGAME_TEXASHOLDEM_WINDOW);
		}
	}	
}

void CInnerInterface::CloseTexasHoldemWindow()
{
	if( GLUseFeatures::GetInstance().IsUsingMiniGameTexasHoldem() )
	{
		if( UiIsVisibleGroup(MINIGAME_TEXASHOLDEM_WINDOW) )
		{
			m_pTexasHoldemWindow->SetVisible(false);
			m_pInventory->UnLockInven(MINIGAME_TEXASHOLDEM_WINDOW);
		}
	}
}

void CInnerInterface::RunTexasHoldemWindow()
{
	if( GLUseFeatures::GetInstance().IsUsingMiniGameTexasHoldem() )
	{
		if( !UiIsVisibleGroup(MINIGAME_TEXASHOLDEM_WEB_WINDOW) )
			m_pTexasHoldemWebWindow->RunTexasHoldemWindow(true);
	}
}

void CInnerInterface::StopTexasHoldemWindow()
{
	if( GLUseFeatures::GetInstance().IsUsingMiniGameTexasHoldem() )
	{
		if( UiIsVisibleGroup(MINIGAME_TEXASHOLDEM_WEB_WINDOW) )
			m_pTexasHoldemWebWindow->RunTexasHoldemWindow(false);
	}
}

void CInnerInterface::SetTexasHoldemURL( const std::string& strURL, const std::string& strPOST )
{
	if( GLUseFeatures::GetInstance().IsUsingMiniGameTexasHoldem() )
	{
		m_pTexasHoldemWebWindow->SetURL( strURL, strPOST );
	}
}

void CInnerInterface::ToggleWindowParty()
{
	if ( GLUseFeatures::GetInstance().IsUsingPartyWindowRenewal() )
	{
		if ( !UiIsVisibleGroup ( PARTY_WINDOW_RENEWAL ) )
			UiShowGroupFocus ( PARTY_WINDOW_RENEWAL );
		else
			UiHideGroup ( PARTY_WINDOW_RENEWAL );		
	}
	else
	{
		if ( !UiIsVisibleGroup ( PARTY_WINDOW ) )
			UiShowGroupFocus ( PARTY_WINDOW );
		else
			UiHideGroup ( PARTY_WINDOW );
	}
}

void CInnerInterface::CloseExpeditionInviteWindow()
{
	if ( GLUseFeatures::GetInstance().IsUsingPartyWindowRenewal() )
		UiHideGroup(EXPEDITION_INVITE_WINDOW);
}

void CInnerInterface::ToggleWindowYesNoParty(const DWORD nID /* = -1 */)
{
	if ( GLUseFeatures::GetInstance().IsUsingPartyWindowRenewal() == false )
		return;

	if ( nID == -1 )
		UiHideGroup ( PARTY_YES_NO_WINDOW );
	else
	{
		m_pPartyYesNoWindow->SetID(nID);
		UiShowGroupFocus ( PARTY_YES_NO_WINDOW );
	}		
}

void CInnerInterface::ToggleWindowTenderItem(const bool bShow)
{
	if ( bShow )
	{
		if ( !IsVisible(PARTY_TENDER_WINDOW) )
			UiShowGroupFocus ( PARTY_TENDER_WINDOW );
		UiHideGroup ( PARTY_DISTRIBUTION_WINDOW );
		UiHideGroup ( PARTY_DISTRIBUTION_LIST_WINDOW );
	}
	else
		UiHideGroup ( PARTY_TENDER_WINDOW );
}
void CInnerInterface::ToggleWindowMasterTransfer(const bool bShow)
{
	if ( bShow )
	{
		if ( !IsVisible(PARTY_DISTRIBUTION_WINDOW) || !IsVisible(PARTY_DISTRIBUTION_LIST_WINDOW) )
		{
			UiShowGroupFocus ( PARTY_DISTRIBUTION_WINDOW );
			UiShowGroupFocus ( PARTY_DISTRIBUTION_LIST_WINDOW );
		}		
		UiHideGroup ( PARTY_TENDER_WINDOW );
	}
	else
	{
		UiHideGroup ( PARTY_DISTRIBUTION_WINDOW );
		UiHideGroup ( PARTY_DISTRIBUTION_LIST_WINDOW );
	}
}

void CInnerInterface::ToggleWindowExpeditionInvite(void)
{
	if ( !UiIsVisibleGroup ( EXPEDITION_INVITE_WINDOW ) )
		UiShowGroupFocus ( EXPEDITION_INVITE_WINDOW );
	else
		UiHideGroup ( EXPEDITION_INVITE_WINDOW );
}

void CInnerInterface::ToggleWindowExpeditionCheckReady(const bool bShow)
{
	if ( GLUseFeatures::GetInstance().IsUsingPartyWindowRenewal() == false )
		return;

	if ( bShow )
		UiShowGroupFocus ( EXPEDITION_CHECK_READY_WINDOW );
	else
		UiHideGroup ( EXPEDITION_CHECK_READY_WINDOW );
}

void CInnerInterface::ToggleWindowColorProperty(const bool bShow)
{
	if ( bShow == true )
		UiShowGroupFocus ( COLOR_PROPERTY_WINDOW );
	else
		UiHideGroup ( COLOR_PROPERTY_WINDOW );
}

void CInnerInterface::ToggleWindowClub()
{
	if ( !UiIsVisibleGroup ( CLUB_WINDOW ) )
		UiShowGroupFocus ( CLUB_WINDOW );
	else
	{
		UiHideGroup ( POPUP_MENU );
		UiHideGroup ( CLUB_WINDOW );
	}
}

void CInnerInterface::ToggleWindowQuest()
{
	// ���弭���� ��� �ý��� �޽����� ����Ѵ�;
	if ( GLUseFeatures::GetInstance().IsUsingWorldBattle() )
	{
		PrintMsgText( NS_UITEXTCOLOR::RED,
			ID2GAMEINTEXT( "WORLDBATTLE_SERVER_QUEST_TOGGLE" ) );

		return;
	}

	if ( !UiIsVisibleGroup ( QUEST_WINDOW ) )
	{			
		if ( NATIVEID_NULL().dwID == m_dwEventQuestID )
		{
			UiShowGroupFocus ( QUEST_WINDOW );
			REFRESH_QUEST_WINDOW ();				
		}
		else
		{
			SetQuestWindowOpen ( m_dwEventQuestID );
		}
	}
	else
	{
		UiHideGroup ( QUEST_WINDOW );
	}
}

void CInnerInterface::ToggleWindowFriend()
{
	if ( UiIsVisibleGroup ( FRIEND_WINDOW ) )
	{
		UiHideGroup ( POPUP_MENU );	
		UiHideGroup ( FRIEND_WINDOW );
	}
	else										
		UiShowGroupFocus ( FRIEND_WINDOW );
}

void CInnerInterface::ToggleWindowItemBank()
{
	if( IsBankOpen() )
	{
		if( GLUseFeatures::GetInstance().IsUsingRenewInvenWindow() )
			UiHideGroup( INVENTORY_WINDOW_RENEW );
		else
			UiHideGroup ( ITEMBANK_WINDOW );
	}
	else
	{
		SetItemBankWindowOpen();
	}
}

void CInnerInterface::ToggleWindowMiniMap()
{
	/*if( m_pGaeaClient->IsInstantMap() )
	{
		UiHideGroup ( MAP_WINDOW );
		m_pGaeaClient->PrintMsgText (NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("MINIMAP_CANNOTSEE") );
	}else{*/
		if ( !UiIsVisibleGroup ( MAP_WINDOW ) )
			UiShowGroupFocus ( MAP_WINDOW );
		else
			UiHideGroup ( MAP_WINDOW );
	//}
}

void CInnerInterface::ToggleWindowDPS(void)
{
	if ( UiIsVisibleGroup(DPS_WINDOW) )
		UiHideGroup(DPS_WINDOW);		
	else
		UiShowGroupFocus(DPS_WINDOW);
}

void CInnerInterface::ToggleWindowFlyCameraControl()
{
	if ( UiIsVisibleGroup(FLYCAMERACONTROL_WINDOW) )
		UiHideGroup(FLYCAMERACONTROL_WINDOW);
	else
		UiShowGroupFocus(FLYCAMERACONTROL_WINDOW);
}

void CInnerInterface::ToggleWindowPointshop()
{ 
	if ( GLUseFeatures::GetInstance().IsUsingWorldBattle() )
	{
		PrintMsgText( NS_UITEXTCOLOR::RED,
			ID2GAMEINTEXT( "WORLDBATTLE_SERVER_ITEMSHOP_TOGGLE" ) );
	}

	if ( GLUseFeatures::GetInstance().IsUsingPointShop() )
	{
		if ( !UiIsVisibleGroup( POINT_SHOP_WINDOW ) )
		{
			UiShowGroupFocus( POINT_SHOP_WINDOW );

			GetPointShopWindow()->RefreshWindow();
		}
		else
		{
			UiHideGroup( POINT_SHOP_WINDOW );
		}	
	}
}

void CInnerInterface::ToggleWindowWebItemShop()
{
	if ( !UiIsVisibleGroup ( ITEMSHOP_WINDOW ) )
	{
		//	ItemShopAuth
		if ( ItemShopAuth() )
		{
			UiShowGroupFocus ( ITEMSHOP_WINDOW );
		}

		UiHideGroup ( HELP_WINDOW );
	}
	else
	{
		UiHideGroup ( ITEMSHOP_WINDOW );
	}
}

void CInnerInterface::ToggleWindowChatMacro()
{
	if ( UiIsVisibleGroup ( CHATMACRO_WINDOW ) )
	{
		UiHideGroup ( CHATMACRO_WINDOW );
	}
	else
	{
		GetChatMacro();
		UiShowGroupFocus ( CHATMACRO_WINDOW );
	}
}

void CInnerInterface::ToggleWindowSNS()
{
	if ( GLUseFeatures::GetInstance().IsUsingSNS() )
	{
		if ( !UiIsVisibleGroup ( SNS_WINDOW ) )
		{
			UiShowGroupFocus ( SNS_WINDOW );
		}
		else
		{
			UiHideGroup ( SNS_WINDOW );
		}
	}
}

void CInnerInterface::ToggleWindowHelp()
{
	if ( !UiIsVisibleGroup ( HELP_WINDOW ) )
	{
		UiHideGroup ( ITEMSHOP_WINDOW );
		UiShowGroupFocus ( HELP_WINDOW );
	}
	else
	{
		UiHideGroup ( HELP_WINDOW );
	}
}

void CInnerInterface::ToggleWindowQBox()
{
	if ( m_pQBoxButton )
	{
		m_pQBoxButton->QBOXButtonDown();
	}
}

void CInnerInterface::ToggleWindowPet()
{
	if( m_pGaeaClient->GetPetClient()->IsVALID() )
	{
		if ( m_pGaeaClient->GetTradeClient()->Valid() ) 
			return;

		if ( UiIsVisibleGroup( PET_WINDOW ) )		
			UiHideGroup ( PET_WINDOW );
		else									
			UiShowGroupFocus( PET_WINDOW );		
	}
}

void CInnerInterface::ToggleWindowVehicle()
{
	GLCharacter* pChar = m_pGaeaClient->GetCharacter();
	if (pChar->VehicleActiveValue())
	{
		if (m_pGaeaClient->GetTradeClient()->Valid())
			return;

		SITEMCUSTOM& sItemCustom = pChar->m_PutOnItems[SLOT_VEHICLE];
		if ( sItemCustom.GetNativeID() == NATIVEID_NULL() )
			return;
		if ( !pChar->VehicleActiveValue())
			return;


		SNATIVEID sNativeID = pChar->VehicleId();
		const SITEM* pItem = GLogicData::GetInstance().GetItem( sNativeID );
		if (!pItem)
			return;

		switch ( pItem->VehicleType() )
		{
		case VEHICLE_TYPE_BIKE:
			UiIsVisibleGroup( BIKE_WINDOW ) ? UiHideGroup ( BIKE_WINDOW ) : UiShowGroupFocus( BIKE_WINDOW );
			break;
		case VEHICLE_TYPE_BOARD:
			UiIsVisibleGroup( VEHICLE_WINDOW ) ? UiHideGroup ( VEHICLE_WINDOW ) : UiShowGroupFocus( VEHICLE_WINDOW );
			break;
		case VEHICLE_TYPE_CAR:
			UiIsVisibleGroup( CAR_WINDOW ) ? UiHideGroup ( CAR_WINDOW ) : UiShowGroupFocus( CAR_WINDOW );
			break;
		case VEHICLE_TYPE_REARCAR:
			UiIsVisibleGroup( REARCAR_WINDOW ) ? UiHideGroup ( REARCAR_WINDOW ) : UiShowGroupFocus( REARCAR_WINDOW );
			break;
		}
	}
}

void CInnerInterface::ToggleWindowProduct()
{
	if( GLUseFeatures::GetInstance().IsUsingRenewProduct() )
	{
		if( UiIsVisibleGroup( PRODUCT_RESET_WINDOW ) == false )
		{
			if( UiIsVisibleGroup( PRODUCT_WINDOW ) )
				UiHideGroup( PRODUCT_WINDOW );
			else
				UiShowGroupFocus( PRODUCT_WINDOW );
		}
	}	
}

void CInnerInterface::ToggleWindowProductReset(WORD wResetAbleNum, WORD wPosX, WORD wPosY)
{
	if( GLUseFeatures::GetInstance().IsUsingRenewProduct() )
	{
		if( UiIsVisibleGroup( PRODUCT_RESET_WINDOW ) )
			UiHideGroup( PRODUCT_RESET_WINDOW );
		else
		{
			m_pProductResetWindow->Open(wResetAbleNum, wPosX, wPosY);
			if( UiIsVisibleGroup( PRODUCT_WINDOW ) && UiIsVisibleGroup( PRODUCT_RESET_WINDOW ) )
				UiHideGroup( PRODUCT_WINDOW );
		}
	}
}

void CInnerInterface::ToggleWindowDice()
{
	if( UiIsVisibleGroup( DICE_WINDOW ) )
		UiHideGroup( DICE_WINDOW );
	else
		UiShowGroupFocus( DICE_WINDOW );
}

void CInnerInterface::ToggleWindowSchoolRecord()
{
	if ( IsStudentRecordOpenable() )
	{
		if ( UiIsVisibleGroup ( STUDENTRECORD_WINDOW ) )
		{
			UiHideGroup ( STUDENTRECORD_WINDOW );
		}
		else
		{
			UiShowGroupFocus ( STUDENTRECORD_WINDOW );
		}
	}
}

void CInnerInterface::ToggleWindowBattleGround()
{
	if ( !CompetitionWindowIsOpen() )
	{
		CompetitionWindowOpen();
	}
	else
	{
		CompetitionWindowClose();
	}
}

void CInnerInterface::QuickPotionSlotSetIndex(int nIndex)
{
	if ( m_pUILeftTopGroup )
	{
		m_pUILeftTopGroup->SetQuickPotionSlotIndex(nIndex);
	}
}

void CInnerInterface::RefreshClubWindowGuidanceMap()
{
    if ( m_pClubWindow )
        m_pClubWindow->RefreshGuidanceMap();
}

void CInnerInterface::ShowClubwindowSetGradePopup()
{
    if ( m_pClubWindow )
        m_pClubWindow->ShowGradePopup();
}

void CInnerInterface::ShowClubLogInClubWindow()
{
    if ( m_pClubWindow )
        m_pClubWindow->ShowClubLog();
}

void CInnerInterface::ClubStorageUpdateMoneyInClubWindow()
{
    if ( m_pClubWindow )
        m_pClubWindow->ClubStorageUpdateMoney();
}

void CInnerInterface::ClubUpdateNewbieNotice()
{
    if ( m_pClubWindow )
        m_pClubWindow->ClubUpdateNewbieNotice();
}

void CInnerInterface::PrivateMarketWindowOpen( bool bOpen /*= true */ )
{
    if( GLUseFeatures::GetInstance().IsUsingPrivateMarketSearchBuy() && m_pItemRepairWindow )
    {
        if( bOpen )
            UiShowGroupFocus( PRIVATE_MARKET_SEARCH_BUY );
        else
            UiHideGroup( PRIVATE_MARKET_SEARCH_BUY );
    }
}

void CInnerInterface::PrivatemarketRefreshSearchResult()
{
	if ( m_pPrivateMarketSearchBuy )
		m_pPrivateMarketSearchBuy->PrivatemarketRefreshSearchResult();
}

void CInnerInterface::SetPageWaiting( bool bIsWaiting )
{
	if ( m_pPrivateMarketSearchBuy )
		m_pPrivateMarketSearchBuy->SetPageWaiting(bIsWaiting);
}

bool CInnerInterface::IsPageWaiting()
{
	if ( m_pPrivateMarketSearchBuy )
		return m_pPrivateMarketSearchBuy->IsPageWaiting();
    else
        return false;
}


void CInnerInterface::SetWhishPageEvent( bool bBool )
{
	m_pPrivateMarketSearchBuy->SetWhishPageEvent(bBool);
}


void CInnerInterface::PrivattMarketWindowRanPointPageRefresh()
{
    m_pPrivateMarketSearchBuy->Refresh();
}

const D3DXVECTOR3& CInnerInterface::GetFlyCameraUILastInputPos(void)
{
	return m_pFlyCameraControl->GetLastInputPos();
}

void CInnerInterface::SetitemGroupComposition(SSETITEM* pSetitem, STARGETID sTargetID)
{
	m_pSetitemPreviewWindow->SetitemGroupComposition(pSetitem, sTargetID);
}

void CInnerInterface::ResetSetitemUIComposition(void)
{
	return m_pSetitemPreviewWindow->ResetSetitemComposition();
}

void CInnerInterface::OpenSetitemPreviewWindow(void)
{
	if ( !UiIsVisibleGroup(SETITEM_PREVIEWWINDOW))
		UiShowGroupFocus( SETITEM_PREVIEWWINDOW );
}

void CInnerInterface::WaitBattleServerDialogueOpen( bool bOpen )
{
	if( bOpen )
		WAITSERVER_DIALOGUE_OPEN ( ID2GAMEINTEXT("WAITSERVER_MESSAGE"), WAITSERVER_TO_WORLDBATTLE );
	else
		WAITSERVER_DIALOGUE_CLOSE();		
}

void	CInnerInterface::ReSetRankingInfo ()
{     
	if ( m_pRankingDisplay )	m_pRankingDisplay->ReSet();
}

void	CInnerInterface::SetRankingInfo ()
{     
	if ( m_pRankingDisplay )	m_pRankingDisplay->RefreshRanking();
	UiShowGroupFocus ( RANKING_DISPLAY );
}

void	CInnerInterface::SetRankingWindowOpen ()
{
	if( m_fItemBankDelay >= 5.0f )
	{
		m_fItemBankDelay = 0.0f;
	}else{
		return;
	}

	m_pGaeaClient->GetCharacter()->ReqRankingsInfo ();
	
}