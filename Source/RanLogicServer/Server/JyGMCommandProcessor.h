#pragma once

#include "../../Character/GLChar.h"
#include "../../../RanLogic/GLogicDataMan.h"

//-----------------------------------------------------------------------------------------------//
// Server-side Job Unlocking & Kill Animation System - Jy技術團隊
// 開發者：TIAN0517 - Jy技術團隊
//-----------------------------------------------------------------------------------------------//

class GLGaeaServer;

class JyGMCommandProcessor
{
public:
    // Job Unlocking Commands
    static bool ProcessUnlockGunner(GLChar* pChar, GLGaeaServer* pServer);
    static bool ProcessUnlockNinja(GLChar* pChar, GLGaeaServer* pServer);
    static bool ProcessUnlockAllJobs(GLChar* pChar, GLGaeaServer* pServer);
    
    // Kill Animation Commands
    static bool ProcessAddKillCard(GLChar* pChar, GLGaeaServer* pServer, DWORD dwCardID, DWORD dwExpireMinutes = 0);
    static bool ProcessRemoveKillCard(GLChar* pChar, GLGaeaServer* pServer, DWORD dwCardID);
    
    // Helper Functions
    static bool IsValidKillCard(DWORD dwCardID);
    static std::string GetKillCardName(DWORD dwCardID);
    static void SendGMMessage(GLChar* pChar, const std::string& strMessage);
    
    // Chinese Command Parser
    static bool ProcessChineseGMCommand(GLChar* pChar, GLGaeaServer* pServer, const std::string& strCommand);

private:
    // Job unlock states (server-side)
    static std::map<DWORD, bool> s_mapPlayerGunnerUnlock;  // PlayerID -> Unlock state
    static std::map<DWORD, bool> s_mapPlayerNinjaUnlock;   // PlayerID -> Unlock state
    
    // Kill card management
    struct KillCard
    {
        DWORD dwCardID;
        DWORD dwExpireTime;
        bool bActive;
        
        KillCard() : dwCardID(0), dwExpireTime(0), bActive(false) {}
        KillCard(DWORD id, DWORD expire) : dwCardID(id), dwExpireTime(expire), bActive(true) {}
    };
    
    static std::map<DWORD, std::map<DWORD, KillCard>> s_mapPlayerKillCards; // PlayerID -> (CardID -> KillCard)
    
public:
    // Management functions
    static void SetPlayerGunnerUnlock(DWORD dwPlayerID, bool bUnlock);
    static void SetPlayerNinjaUnlock(DWORD dwPlayerID, bool bUnlock);
    static bool IsPlayerGunnerUnlocked(DWORD dwPlayerID);
    static bool IsPlayerNinjaUnlocked(DWORD dwPlayerID);
    
    static void AddPlayerKillCard(DWORD dwPlayerID, DWORD dwCardID, DWORD dwExpireTime);
    static void RemovePlayerKillCard(DWORD dwPlayerID, DWORD dwCardID);
    static bool HasPlayerKillCard(DWORD dwPlayerID, DWORD dwCardID);
    static void CleanupExpiredCards();
};