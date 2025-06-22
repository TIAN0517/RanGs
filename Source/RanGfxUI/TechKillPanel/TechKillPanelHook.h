#pragma once

#include "../../enginelib/GUInterface/UIGroup.h"
#include "./TechKillPanelManager.h"

// Forward declarations
class GLCharacter;
class CKillCardManager;

/**
 * TechKillPanel Integration Hook
 * Provides non-intrusive integration with existing game systems
 * Acts as a plugin layer without modifying core files
 */
class CTechKillPanelHook
{
public:
    // Hook callback function types
    typedef void (*KillEventCallback)(EKILL_TYPE killType, DWORD attackerID, DWORD targetID, const CString& attackerName, const CString& targetName);
    typedef BOOL (*EquipmentCheckCallback)(DWORD characterID, DWORD itemID);
    typedef void (*UIRenderCallback)(LPDIRECT3DDEVICE9 pd3dDevice);

public:
    CTechKillPanelHook();
    virtual ~CTechKillPanelHook();

public:
    // Initialization and cleanup
    BOOL Initialize(EngineDeviceMan* pEngineDevice);
    void Cleanup();
    
    // Hook installation/removal
    BOOL InstallHooks();
    void RemoveHooks();
    
    // Main integration interface
    void ProcessKillEvent(EKILL_TYPE killType, DWORD attackerID, DWORD targetID, const CString& attackerName = _T(""), const CString& targetName = _T(""));
    
    // Configuration
    void SetEnabled(BOOL bEnabled) { m_bEnabled = bEnabled; }
    BOOL IsEnabled() const { return m_bEnabled; }
    
    // Callback registration
    void RegisterKillEventCallback(KillEventCallback callback);
    void RegisterEquipmentCheckCallback(EquipmentCheckCallback callback);
    void RegisterUIRenderCallback(UIRenderCallback callback);

private:
    // Internal hook functions
    static void OnCharacterKilled(DWORD attackerID, DWORD targetID, DWORD damageFlags, BOOL bSkillAttack);
    static void OnPlayerEquipItem(DWORD itemID, DWORD slotID);
    static void OnPlayerUnequipItem(DWORD itemID, DWORD slotID);
    static void OnUIRender(LPDIRECT3DDEVICE9 pd3dDevice);
    
    // Helper functions
    EKILL_TYPE DetermineKillType(DWORD damageFlags, BOOL bSkillAttack);
    CString GetCharacterName(DWORD characterID);
    BOOL ShouldUseTechPanel(DWORD attackerID);
    void TriggerFallbackKillDisplay(EKILL_TYPE killType, DWORD targetID);

private:
    // Core components
    CTechKillPanelManager* m_pTechPanelManager;
    EngineDeviceMan* m_pEngineDevice;
    
    // State
    BOOL m_bEnabled;
    BOOL m_bHooksInstalled;
    BOOL m_bInitialized;
    
    // Callbacks
    std::vector<KillEventCallback> m_vecKillCallbacks;
    std::vector<EquipmentCheckCallback> m_vecEquipmentCallbacks;
    std::vector<UIRenderCallback> m_vecRenderCallbacks;
    
    // Integration with existing systems
    CKillCardManager* m_pOriginalKillCardManager;
    
    // Static instance for hook callbacks
    static CTechKillPanelHook* s_pInstance;
    
    // Performance tracking
    DWORD m_dwLastKillTime;
    DWORD m_dwKillEventCooldown;
};