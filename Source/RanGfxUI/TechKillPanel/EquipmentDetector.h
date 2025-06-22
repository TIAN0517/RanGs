#pragma once

/**
 * Equipment Detector
 * Detects if player has specific equipment items equipped
 * Used to check for Tech Display Card (ID=9999) for panel activation
 */
class CEquipmentDetector
{
public:
    CEquipmentDetector();
    virtual ~CEquipmentDetector();

public:
    // Initialization
    void Initialize();
    void Cleanup();
    
    // Main detection interface
    BOOL IsItemEquipped(DWORD itemID);
    BOOL IsTechDisplayCardEquipped() { return IsItemEquipped(9999); }
    
    // Equipment monitoring
    void Update(float fElapsedTime);
    void RefreshEquipmentCache();
    
    // Callback registration for equipment changes
    typedef void (*EquipmentChangeCallback)(DWORD itemID, BOOL bEquipped);
    void RegisterCallback(EquipmentChangeCallback callback);
    void UnregisterCallback(EquipmentChangeCallback callback);

private:
    // Internal helper functions
    BOOL CheckPlayerInventory(DWORD itemID);
    BOOL CheckEquippedSlots(DWORD itemID);
    void UpdateEquipmentCache();
    void NotifyEquipmentChange(DWORD itemID, BOOL bEquipped);
    
    // Hook integration (without modifying core files)
    void InstallHooks();
    void RemoveHooks();
    
    // Static hook functions
    static void OnItemEquipped(DWORD itemID);
    static void OnItemUnequipped(DWORD itemID);

private:
    // Equipment cache for performance
    std::map<DWORD, BOOL> m_mapEquippedItems;
    float m_fCacheUpdateTimer;
    float m_fCacheUpdateInterval;
    
    // Callback management
    std::vector<EquipmentChangeCallback> m_vecCallbacks;
    
    // Hook status
    BOOL m_bHooksInstalled;
    
    // Performance optimization
    DWORD m_dwLastUpdateTime;
    BOOL m_bForceRefresh;
    
    // Static instance for hook callbacks
    static CEquipmentDetector* s_pInstance;
};