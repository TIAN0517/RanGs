#include "stdafx.h"
#include "./TechKillPanelHook.h"
#include "../KillCard/KillCardManager.h"

// Static instance for hook callbacks
CTechKillPanelHook* CTechKillPanelHook::s_pInstance = NULL;

CTechKillPanelHook::CTechKillPanelHook()
    : m_pTechPanelManager(NULL)
    , m_pEngineDevice(NULL)
    , m_bEnabled(TRUE)
    , m_bHooksInstalled(FALSE)
    , m_bInitialized(FALSE)
    , m_pOriginalKillCardManager(NULL)
    , m_dwLastKillTime(0)
    , m_dwKillEventCooldown(100) // 100ms cooldown between kill events
{
    s_pInstance = this;
}

CTechKillPanelHook::~CTechKillPanelHook()
{
    Cleanup();
    s_pInstance = NULL;
}

BOOL CTechKillPanelHook::Initialize(EngineDeviceMan* pEngineDevice)
{
    if (m_bInitialized || !pEngineDevice)
        return FALSE;
    
    m_pEngineDevice = pEngineDevice;
    
    // Create the tech panel manager
    m_pTechPanelManager = new CTechKillPanelManager(pEngineDevice);
    if (!m_pTechPanelManager)
        return FALSE;
    
    m_pTechPanelManager->CreateSubControl();
    
    // Install hooks
    if (!InstallHooks())
    {
        Cleanup();
        return FALSE;
    }
    
    m_bInitialized = TRUE;
    return TRUE;
}

void CTechKillPanelHook::Cleanup()
{
    RemoveHooks();
    
    SAFE_DELETE(m_pTechPanelManager);
    
    m_vecKillCallbacks.clear();
    m_vecEquipmentCallbacks.clear();
    m_vecRenderCallbacks.clear();
    
    m_bInitialized = FALSE;
}

BOOL CTechKillPanelHook::InstallHooks()
{
    if (m_bHooksInstalled)
        return TRUE;
    
    // This is where we would install actual hooks into the game's systems
    // For now, we'll simulate the hook installation
    
    /*
    Example hook installation using MinHook or similar:
    
    // Hook into character death processing
    if (MH_CreateHook(&OriginalCharacterDeath, &HookedCharacterDeath, 
                     reinterpret_cast<LPVOID*>(&TrampolineCharacterDeath)) != MH_OK)
    {
        return FALSE;
    }
    
    // Hook into equipment change
    if (MH_CreateHook(&OriginalEquipItem, &HookedEquipItem,
                     reinterpret_cast<LPVOID*>(&TrampolineEquipItem)) != MH_OK)
    {
        return FALSE;
    }
    
    // Hook into UI render
    if (MH_CreateHook(&OriginalUIRender, &HookedUIRender,
                     reinterpret_cast<LPVOID*>(&TrampolineUIRender)) != MH_OK)
    {
        return FALSE;
    }
    
    // Enable all hooks
    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
    {
        return FALSE;
    }
    */
    
    m_bHooksInstalled = TRUE;
    return TRUE;
}

void CTechKillPanelHook::RemoveHooks()
{
    if (!m_bHooksInstalled)
        return;
    
    /*
    // Disable and remove hooks
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(&OriginalCharacterDeath);
    MH_RemoveHook(&OriginalEquipItem);
    MH_RemoveHook(&OriginalUIRender);
    */
    
    m_bHooksInstalled = FALSE;
}

void CTechKillPanelHook::ProcessKillEvent(EKILL_TYPE killType, DWORD attackerID, DWORD targetID, const CString& attackerName, const CString& targetName)
{
    if (!m_bEnabled || !m_bInitialized || !m_pTechPanelManager)
        return;
    
    DWORD dwCurrentTime = GetTickCount();
    
    // Prevent spam of kill events
    if (dwCurrentTime - m_dwLastKillTime < m_dwKillEventCooldown)
        return;
    
    m_dwLastKillTime = dwCurrentTime;
    
    // Check if we should use tech panel for this attacker
    if (ShouldUseTechPanel(attackerID))
    {
        // Use tech panel
        CString finalAttackerName = attackerName.IsEmpty() ? GetCharacterName(attackerID) : attackerName;
        CString finalTargetName = targetName.IsEmpty() ? GetCharacterName(targetID) : targetName;
        
        m_pTechPanelManager->TriggerTechKillPanel(killType, targetID, finalAttackerName, finalTargetName);
    }
    else
    {
        // Fallback to original kill display
        TriggerFallbackKillDisplay(killType, targetID);
    }
    
    // Notify registered callbacks
    for (auto callback : m_vecKillCallbacks)
    {
        if (callback)
        {
            callback(killType, attackerID, targetID, attackerName, targetName);
        }
    }
}

void CTechKillPanelHook::RegisterKillEventCallback(KillEventCallback callback)
{
    if (callback && std::find(m_vecKillCallbacks.begin(), m_vecKillCallbacks.end(), callback) == m_vecKillCallbacks.end())
    {
        m_vecKillCallbacks.push_back(callback);
    }
}

void CTechKillPanelHook::RegisterEquipmentCheckCallback(EquipmentCheckCallback callback)
{
    if (callback && std::find(m_vecEquipmentCallbacks.begin(), m_vecEquipmentCallbacks.end(), callback) == m_vecEquipmentCallbacks.end())
    {
        m_vecEquipmentCallbacks.push_back(callback);
    }
}

void CTechKillPanelHook::RegisterUIRenderCallback(UIRenderCallback callback)
{
    if (callback && std::find(m_vecRenderCallbacks.begin(), m_vecRenderCallbacks.end(), callback) == m_vecRenderCallbacks.end())
    {
        m_vecRenderCallbacks.push_back(callback);
    }
}

// Static hook functions (these would be called by the actual game hooks)
void CTechKillPanelHook::OnCharacterKilled(DWORD attackerID, DWORD targetID, DWORD damageFlags, BOOL bSkillAttack)
{
    if (!s_pInstance)
        return;
    
    EKILL_TYPE killType = s_pInstance->DetermineKillType(damageFlags, bSkillAttack);
    s_pInstance->ProcessKillEvent(killType, attackerID, targetID);
}

void CTechKillPanelHook::OnPlayerEquipItem(DWORD itemID, DWORD slotID)
{
    if (!s_pInstance)
        return;
    
    // Notify equipment detector if it's the tech card
    if (itemID == 9999 && s_pInstance->m_pTechPanelManager)
    {
        // Force refresh of equipment cache
        // This would be handled by the EquipmentDetector's callback system
    }
}

void CTechKillPanelHook::OnPlayerUnequipItem(DWORD itemID, DWORD slotID)
{
    if (!s_pInstance)
        return;
    
    // Same as equip - notify about equipment changes
    if (itemID == 9999 && s_pInstance->m_pTechPanelManager)
    {
        // Force refresh of equipment cache
    }
}

void CTechKillPanelHook::OnUIRender(LPDIRECT3DDEVICE9 pd3dDevice)
{
    if (!s_pInstance || !s_pInstance->m_bEnabled || !s_pInstance->m_pTechPanelManager)
        return;
    
    // Render the tech panel
    s_pInstance->m_pTechPanelManager->Render(pd3dDevice);
    
    // Notify registered render callbacks
    for (auto callback : s_pInstance->m_vecRenderCallbacks)
    {
        if (callback)
        {
            callback(pd3dDevice);
        }
    }
}

EKILL_TYPE CTechKillPanelHook::DetermineKillType(DWORD damageFlags, BOOL bSkillAttack)
{
    // This function analyzes the damage flags and determines the kill type
    // Based on the existing KillCard implementation patterns
    
    EKILL_TYPE killType = 0;
    
    // Check for critical hit
    if (damageFlags & 0x01) // Assuming bit 0 is critical hit flag
    {
        killType |= KILL_TYPE_CRITICAL;
    }
    
    // Check for skill attack
    if (bSkillAttack)
    {
        killType |= KILL_TYPE_SKILL;
    }
    
    // Check for headshot (would need additional damage flag analysis)
    if (damageFlags & 0x02) // Assuming bit 1 is headshot flag
    {
        killType |= KILL_TYPE_HEADSHOT;
    }
    
    // For weapon type detection, we would need to check the attacker's equipped weapon
    // This would integrate with the character/inventory system
    
    /*
    GLCharacter* pAttacker = GetCharacterByID(attackerID);
    if (pAttacker)
    {
        const SITEMCUSTOM& weapon = pAttacker->GetSlotItem(SLOT_RHAND);
        if (weapon.GetNativeID() != NATIVEID_NULL())
        {
            const SITEM* pWeapon = GLogicData::GetInstance().GetItem(weapon.GetNativeID());
            if (pWeapon)
            {
                if (IsRangedWeapon(pWeapon->sSuitOp.emAttack))
                {
                    killType |= KILL_TYPE_RANGED;
                }
                else if (IsMeleeWeapon(pWeapon->sSuitOp.emAttack))
                {
                    killType |= KILL_TYPE_MELEE;
                }
            }
        }
    }
    */
    
    return killType;
}

CString CTechKillPanelHook::GetCharacterName(DWORD characterID)
{
    // This would integrate with the character system to get the name
    // For now, return a placeholder
    
    /*
    GLCharacter* pCharacter = GetCharacterByID(characterID);
    if (pCharacter)
    {
        return CString(pCharacter->GetName());
    }
    */
    
    CString name;
    name.Format(_T("Player_%d"), characterID);
    return name;
}

BOOL CTechKillPanelHook::ShouldUseTechPanel(DWORD attackerID)
{
    if (!m_pTechPanelManager)
        return FALSE;
    
    // Check if the attacker has the tech panel enabled and tech card equipped
    return m_pTechPanelManager->IsEnabled();
}

void CTechKillPanelHook::TriggerFallbackKillDisplay(EKILL_TYPE killType, DWORD targetID)
{
    // Trigger the original kill card system as fallback
    if (m_pOriginalKillCardManager)
    {
        // This would call the original system
        // m_pOriginalKillCardManager->TriggerKillCard(killType, targetID);
    }
    
    // Or integrate with the existing KillCard system
    /*
    CKillCardManager* pKillCardMgr = GetKillCardManager();
    if (pKillCardMgr)
    {
        pKillCardMgr->TriggerKillCard(killType, targetID);
    }
    */
}