#include "pch.h"
#include "JyGMCommandProcessor.h"
#include "../FieldServer/GLGaeaServer.h"
#include "../../RanLogic/Msg/GLContrlServerMsg.h"
#include "../../RanLogic/Msg/GLContrlBaseMsg.h"

#include <windows.h>
#include <strsafe.h>
#include <mmsystem.h>

//-----------------------------------------------------------------------------------------------//
// Server-side Job Unlocking & Kill Animation System - Jy技術團隊
// 開發者：TIAN0517 - Jy技術團隊
//-----------------------------------------------------------------------------------------------//

// Static member definitions
std::map<DWORD, bool> JyGMCommandProcessor::s_mapPlayerGunnerUnlock;
std::map<DWORD, bool> JyGMCommandProcessor::s_mapPlayerNinjaUnlock;
std::map<DWORD, std::map<DWORD, JyGMCommandProcessor::KillCard>> JyGMCommandProcessor::s_mapPlayerKillCards;

bool JyGMCommandProcessor::ProcessUnlockGunner(GLChar* pChar, GLGaeaServer* pServer)
{
    if (!pChar || !pServer)
        return false;
    
    if (pChar->UserLevel() < USER_GM3)
    {
        SendGMMessage(pChar, "權限不足 (Insufficient permissions)");
        return false;
    }
    
    DWORD dwPlayerID = pChar->GetCharID();
    SetPlayerGunnerUnlock(dwPlayerID, true);
    
    SendGMMessage(pChar, "槍手職業已解鎖 (Gunner class unlocked)");
    return true;
}

bool JyGMCommandProcessor::ProcessUnlockNinja(GLChar* pChar, GLGaeaServer* pServer)
{
    if (!pChar || !pServer)
        return false;
    
    if (pChar->UserLevel() < USER_GM3)
    {
        SendGMMessage(pChar, "權限不足 (Insufficient permissions)");
        return false;
    }
    
    DWORD dwPlayerID = pChar->GetCharID();
    SetPlayerNinjaUnlock(dwPlayerID, true);
    
    SendGMMessage(pChar, "忍者職業已解鎖 (Ninja class unlocked)");
    return true;
}

bool JyGMCommandProcessor::ProcessUnlockAllJobs(GLChar* pChar, GLGaeaServer* pServer)
{
    if (!pChar || !pServer)
        return false;
    
    if (pChar->UserLevel() < USER_GM3)
    {
        SendGMMessage(pChar, "權限不足 (Insufficient permissions)");
        return false;
    }
    
    DWORD dwPlayerID = pChar->GetCharID();
    SetPlayerGunnerUnlock(dwPlayerID, true);
    SetPlayerNinjaUnlock(dwPlayerID, true);
    
    SendGMMessage(pChar, "所有職業已解鎖 (All jobs unlocked)");
    return true;
}

bool JyGMCommandProcessor::ProcessAddKillCard(GLChar* pChar, GLGaeaServer* pServer, DWORD dwCardID, DWORD dwExpireMinutes)
{
    if (!pChar || !pServer)
        return false;
    
    if (pChar->UserLevel() < USER_GM3)
    {
        SendGMMessage(pChar, "權限不足 (Insufficient permissions)");
        return false;
    }
    
    if (!IsValidKillCard(dwCardID))
    {
        SendGMMessage(pChar, "無效的卡片ID，範圍: 12001-12006 (Invalid card ID, range: 12001-12006)");
        return false;
    }
    
    DWORD dwPlayerID = pChar->GetCharID();
    DWORD dwExpireTime = 0;
    
    if (dwExpireMinutes > 0)
        dwExpireTime = timeGetTime() + (dwExpireMinutes * 60 * 1000); // Convert to milliseconds
    
    AddPlayerKillCard(dwPlayerID, dwCardID, dwExpireTime);
    
    std::string strCardName = GetKillCardName(dwCardID);
    std::string strMessage = "已添加擊殺卡片: " + strCardName + " (Added kill card: " + strCardName + ")";
    SendGMMessage(pChar, strMessage);
    
    return true;
}

bool JyGMCommandProcessor::ProcessRemoveKillCard(GLChar* pChar, GLGaeaServer* pServer, DWORD dwCardID)
{
    if (!pChar || !pServer)
        return false;
    
    if (pChar->UserLevel() < USER_GM3)
    {
        SendGMMessage(pChar, "權限不足 (Insufficient permissions)");
        return false;
    }
    
    DWORD dwPlayerID = pChar->GetCharID();
    RemovePlayerKillCard(dwPlayerID, dwCardID);
    
    std::string strCardName = GetKillCardName(dwCardID);
    std::string strMessage = "已移除擊殺卡片: " + strCardName + " (Removed kill card: " + strCardName + ")";
    SendGMMessage(pChar, strMessage);
    
    return true;
}

bool JyGMCommandProcessor::IsValidKillCard(DWORD dwCardID)
{
    return (dwCardID >= 12001 && dwCardID <= 12006);
}

std::string JyGMCommandProcessor::GetKillCardName(DWORD dwCardID)
{
    switch (dwCardID)
    {
    case 12001: return "劍氣砍殺";
    case 12002: return "穿刺致命";
    case 12003: return "爆炸轟殺";
    case 12004: return "冰凍粉碎";
    case 12005: return "烈焰焚燒";
    case 12006: return "雷電轟擊";
    default:    return "未知";
    }
}

void JyGMCommandProcessor::SendGMMessage(GLChar* pChar, const std::string& strMessage)
{
    if (!pChar)
        return;
    
    // Send message to player
    GLMSG::SNET_SERVER_GENERALCHAT NetMsg;
    NetMsg.emType = CHAT_TYPE_SYSTEM;
    NetMsg.dwChaDbNum = pChar->GetCharID();
    StringCchCopy(NetMsg.szName, CHAR_SZNAME, "Jy技術團隊");
    StringCchCopy(NetMsg.szChatMsg, GLOBAL_CHAT_MSG_SIZE, strMessage.c_str());
    
    // Send to client
    pChar->SendToClient(&NetMsg);
}

bool JyGMCommandProcessor::ProcessChineseGMCommand(GLChar* pChar, GLGaeaServer* pServer, const std::string& strCommand)
{
    if (!pChar || !pServer)
        return false;
    
    // Parse Chinese GM commands
    if (strCommand == "開啟槍手" || strCommand == "unlockgunner")
    {
        return ProcessUnlockGunner(pChar, pServer);
    }
    else if (strCommand == "開啟忍者" || strCommand == "unlockninja")
    {
        return ProcessUnlockNinja(pChar, pServer);
    }
    else if (strCommand == "解鎖所有職業" || strCommand == "unlockall")
    {
        return ProcessUnlockAllJobs(pChar, pServer);
    }
    else if (strCommand.find("添加擊殺卡片") == 0 || strCommand.find("addkillcard") == 0)
    {
        // Parse card ID and expiry from command
        // Format: "添加擊殺卡片 12001 60" (cardid, expire_minutes)
        size_t spacePos = strCommand.find(' ');
        if (spacePos != std::string::npos)
        {
            std::string strParams = strCommand.substr(spacePos + 1);
            size_t nextSpacePos = strParams.find(' ');
            
            DWORD dwCardID = 0;
            DWORD dwExpireMinutes = 0;
            
            if (nextSpacePos != std::string::npos)
            {
                dwCardID = atoi(strParams.substr(0, nextSpacePos).c_str());
                dwExpireMinutes = atoi(strParams.substr(nextSpacePos + 1).c_str());
            }
            else
            {
                dwCardID = atoi(strParams.c_str());
            }
            
            return ProcessAddKillCard(pChar, pServer, dwCardID, dwExpireMinutes);
        }
        else
        {
            SendGMMessage(pChar, "使用方法: 添加擊殺卡片 [卡片ID] [過期時間分鐘] (Usage: addkillcard [cardid] [expireminutes])");
            return false;
        }
    }
    else if (strCommand.find("設定擊殺面板風格") == 0 || strCommand.find("setkillpanelstyle") == 0)
    {
        // 解析風格ID參數 - Jy技術團隊新增
        size_t spacePos = strCommand.find(' ');
        if (spacePos != std::string::npos)
        {
            std::string strParams = strCommand.substr(spacePos + 1);
            int nStyleID = atoi(strParams.c_str());
            
            return ProcessSetKillPanelStyle(pChar, pServer, nStyleID);
        }
        else
        {
            SendGMMessage(pChar, "使用方法: 設定擊殺面板風格 [1-6] (1:賽博朋克 2:量子科技 3:軍事戰術 4:全息投影 5:太空科幻 6:蒸汽朋克)");
            return false;
        }
    }
    
    return false;
}

// Management functions implementation
void JyGMCommandProcessor::SetPlayerGunnerUnlock(DWORD dwPlayerID, bool bUnlock)
{
    s_mapPlayerGunnerUnlock[dwPlayerID] = bUnlock;
}

void JyGMCommandProcessor::SetPlayerNinjaUnlock(DWORD dwPlayerID, bool bUnlock)
{
    s_mapPlayerNinjaUnlock[dwPlayerID] = bUnlock;
}

bool JyGMCommandProcessor::IsPlayerGunnerUnlocked(DWORD dwPlayerID)
{
    auto iter = s_mapPlayerGunnerUnlock.find(dwPlayerID);
    return (iter != s_mapPlayerGunnerUnlock.end()) ? iter->second : false;
}

bool JyGMCommandProcessor::IsPlayerNinjaUnlocked(DWORD dwPlayerID)
{
    auto iter = s_mapPlayerNinjaUnlock.find(dwPlayerID);
    return (iter != s_mapPlayerNinjaUnlock.end()) ? iter->second : false;
}

void JyGMCommandProcessor::AddPlayerKillCard(DWORD dwPlayerID, DWORD dwCardID, DWORD dwExpireTime)
{
    s_mapPlayerKillCards[dwPlayerID][dwCardID] = KillCard(dwCardID, dwExpireTime);
}

void JyGMCommandProcessor::RemovePlayerKillCard(DWORD dwPlayerID, DWORD dwCardID)
{
    auto playerIter = s_mapPlayerKillCards.find(dwPlayerID);
    if (playerIter != s_mapPlayerKillCards.end())
    {
        auto cardIter = playerIter->second.find(dwCardID);
        if (cardIter != playerIter->second.end())
        {
            playerIter->second.erase(cardIter);
        }
    }
}

bool JyGMCommandProcessor::HasPlayerKillCard(DWORD dwPlayerID, DWORD dwCardID)
{
    auto playerIter = s_mapPlayerKillCards.find(dwPlayerID);
    if (playerIter != s_mapPlayerKillCards.end())
    {
        auto cardIter = playerIter->second.find(dwCardID);
        return (cardIter != playerIter->second.end() && cardIter->second.bActive);
    }
    return false;
}

void JyGMCommandProcessor::CleanupExpiredCards()
{
    DWORD dwCurrentTime = timeGetTime();
    
    for (auto& playerPair : s_mapPlayerKillCards)
    {
        auto& playerCards = playerPair.second;
        auto cardIter = playerCards.begin();
        
        while (cardIter != playerCards.end())
        {
            if (cardIter->second.dwExpireTime > 0 && dwCurrentTime > cardIter->second.dwExpireTime)
            {
                cardIter = playerCards.erase(cardIter);
            }
            else
            {
                ++cardIter;
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------//
// Kill Panel Style Management - Jy技術團隊新增
//-----------------------------------------------------------------------------------------------//

bool JyGMCommandProcessor::ProcessSetKillPanelStyle(GLChar* pChar, GLGaeaServer* pServer, int nStyleID)
{
    if (!pChar || !pServer)
        return false;
        
    // 驗證風格ID範圍 (1-6)
    if (nStyleID < 1 || nStyleID > 6)
    {
        SendGMMessage(pChar, "錯誤：風格ID必須在1-6之間 (1:賽博朋克 2:量子科技 3:軍事戰術 4:全息投影 5:太空科幻 6:蒸汽朋克)");
        return false;
    }
    
    // 設定玩家的擊殺面板風格
    // TODO: 實際上需要將風格設定傳送到客戶端
    // 這裡暫時只在服務端記錄
    
    // 風格名稱對應
    const char* styleNames[] = {
        "賽博朋克風格",      // 1
        "量子科技風格",      // 2  
        "軍事戰術風格",      // 3
        "全息投影風格",      // 4
        "太空科幻風格",      // 5
        "蒸汽朋克風格"       // 6
    };
    
    std::string strMessage = "✅ 擊殺面板風格已設定為: ";
    strMessage += styleNames[nStyleID - 1];
    strMessage += " (ID: ";
    strMessage += std::to_string(nStyleID);
    strMessage += ")";
    
    SendGMMessage(pChar, strMessage);
    
    return true;
}
}