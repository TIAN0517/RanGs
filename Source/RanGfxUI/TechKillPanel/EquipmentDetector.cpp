#include "stdafx.h"
#include "./EquipmentDetector.h"

// Static instance for hook callbacks
CEquipmentDetector* CEquipmentDetector::s_pInstance = NULL;

CEquipmentDetector::CEquipmentDetector()
    : m_fCacheUpdateTimer(0.0f)
    , m_fCacheUpdateInterval(1.0f) // Update cache every 1 second
    , m_bHooksInstalled(FALSE)
    , m_dwLastUpdateTime(0)
    , m_bForceRefresh(FALSE)
{
    s_pInstance = this;
}

CEquipmentDetector::~CEquipmentDetector()
{
    Cleanup();
    s_pInstance = NULL;
}

void CEquipmentDetector::Initialize()
{
    // Clear equipment cache
    m_mapEquippedItems.clear();
    
    // Install hooks for real-time detection
    InstallHooks();
    
    // Initial equipment scan
    RefreshEquipmentCache();
}

void CEquipmentDetector::Cleanup()
{
    // Remove hooks
    RemoveHooks();
    
    // Clear cache and callbacks
    m_mapEquippedItems.clear();
    m_vecCallbacks.clear();
}

BOOL CEquipmentDetector::IsItemEquipped(DWORD itemID)
{
    // Check cache first for performance
    auto it = m_mapEquippedItems.find(itemID);
    if (it != m_mapEquippedItems.end())
    {
        return it->second;
    }
    
    // If not in cache, check directly
    BOOL bEquipped = CheckPlayerInventory(itemID);
    
    // Update cache
    m_mapEquippedItems[itemID] = bEquipped;
    
    return bEquipped;
}

void CEquipmentDetector::Update(float fElapsedTime)
{
    m_fCacheUpdateTimer += fElapsedTime;
    
    // Periodically refresh cache or when forced
    if (m_fCacheUpdateTimer >= m_fCacheUpdateInterval || m_bForceRefresh)
    {
        UpdateEquipmentCache();
        m_fCacheUpdateTimer = 0.0f;
        m_bForceRefresh = FALSE;
    }
}

void CEquipmentDetector::RefreshEquipmentCache()
{
    m_bForceRefresh = TRUE;
}

void CEquipmentDetector::RegisterCallback(EquipmentChangeCallback callback)
{
    if (callback && std::find(m_vecCallbacks.begin(), m_vecCallbacks.end(), callback) == m_vecCallbacks.end())
    {
        m_vecCallbacks.push_back(callback);
    }
}

void CEquipmentDetector::UnregisterCallback(EquipmentChangeCallback callback)
{
    auto it = std::find(m_vecCallbacks.begin(), m_vecCallbacks.end(), callback);
    if (it != m_vecCallbacks.end())
    {
        m_vecCallbacks.erase(it);
    }
}

BOOL CEquipmentDetector::CheckPlayerInventory(DWORD itemID)
{
    // This is a placeholder implementation that would integrate with the game's
    // character/inventory system. In a real implementation, this would:
    // 1. Get reference to the player character
    // 2. Check equipped items in all relevant slots
    // 3. Return TRUE if the item is found
    
    /*
    Example integration:
    
    GLCharacter* pChar = GLCharacter::GetInstance();
    if (!pChar) return FALSE;
    
    // Check all equipment slots
    for (int i = 0; i < SLOT_NSIZE_S_2; ++i)
    {
        const SITEMCUSTOM& item = pChar->GetSlotItem((EMSLOT)i);
        if (item.GetNativeID().dwID == itemID)
        {
            return TRUE;
        }
    }
    
    // Check inventory for additional items
    GLInventory& inventory = pChar->GetInventory();
    return inventory.FindItem(SNATIVEID(itemID)) != SINVEN_POS();
    */
    
    // For now, simulate that the tech card is equipped for testing
    // This should be replaced with actual game integration
    if (itemID == 9999) // Tech Display Card
    {
        // Simulate equipped state based on some condition
        // In real implementation, this would check actual character data
        return TRUE; // Always return TRUE for demonstration
    }
    
    return FALSE;
}

BOOL CEquipmentDetector::CheckEquippedSlots(DWORD itemID)
{
    // Helper function to specifically check equipped slots only
    // This would be more efficient than checking entire inventory
    
    /*
    GLCharacter* pChar = GLCharacter::GetInstance();
    if (!pChar) return FALSE;
    
    // Check equipment slots only
    const EMSLOT slots[] = {
        SLOT_HEADGEAR, SLOT_UPPER, SLOT_LOWER, SLOT_HAND, SLOT_FOOT,
        SLOT_RHAND, SLOT_LHAND, SLOT_NECK, SLOT_WRIST, SLOT_RFINGER,
        SLOT_LFINGER, SLOT_RHAND_S, SLOT_LHAND_S, SLOT_DECORATION,
        SLOT_EARRINGS
    };
    
    for (int i = 0; i < sizeof(slots)/sizeof(EMSLOT); ++i)
    {
        const SITEMCUSTOM& item = pChar->GetSlotItem(slots[i]);
        if (item.GetNativeID().dwID == itemID)
        {
            return TRUE;
        }
    }
    */
    
    return CheckPlayerInventory(itemID); // Fallback for now
}

void CEquipmentDetector::UpdateEquipmentCache()
{
    // Update cache for all tracked items
    std::map<DWORD, BOOL> oldEquipmentState = m_mapEquippedItems;
    
    // Check each item in cache
    for (auto& pair : m_mapEquippedItems)
    {
        DWORD itemID = pair.first;
        BOOL oldState = pair.second;
        BOOL newState = CheckPlayerInventory(itemID);
        
        pair.second = newState;
        
        // Notify if state changed
        if (oldState != newState)
        {
            NotifyEquipmentChange(itemID, newState);
        }
    }
    
    // Also check for the tech display card specifically
    if (m_mapEquippedItems.find(9999) == m_mapEquippedItems.end())
    {
        BOOL techCardEquipped = CheckPlayerInventory(9999);
        m_mapEquippedItems[9999] = techCardEquipped;
    }
}

void CEquipmentDetector::NotifyEquipmentChange(DWORD itemID, BOOL bEquipped)
{
    // Notify all registered callbacks
    for (auto callback : m_vecCallbacks)
    {
        if (callback)
        {
            callback(itemID, bEquipped);
        }
    }
}

void CEquipmentDetector::InstallHooks()
{
    if (m_bHooksInstalled) return;
    
    // This is where we would install hooks into the game's equipment system
    // to get real-time notifications when items are equipped/unequipped
    
    /*
    Example hook installation:
    
    // Hook into item equip function
    if (MH_CreateHook(&OriginalEquipItem, &HookedEquipItem, 
                     reinterpret_cast<LPVOID*>(&TrampolineEquipItem)) == MH_OK)
    {
        MH_EnableHook(&OriginalEquipItem);
    }
    
    // Hook into item unequip function
    if (MH_CreateHook(&OriginalUnequipItem, &HookedUnequipItem,
                     reinterpret_cast<LPVOID*>(&TrampolineUnequipItem)) == MH_OK)
    {
        MH_EnableHook(&OriginalUnequipItem);
    }
    */
    
    m_bHooksInstalled = TRUE;
}

void CEquipmentDetector::RemoveHooks()
{
    if (!m_bHooksInstalled) return;
    
    // Remove installed hooks
    /*
    MH_DisableHook(&OriginalEquipItem);
    MH_RemoveHook(&OriginalEquipItem);
    
    MH_DisableHook(&OriginalUnequipItem);
    MH_RemoveHook(&OriginalUnequipItem);
    */
    
    m_bHooksInstalled = FALSE;
}

void CEquipmentDetector::OnItemEquipped(DWORD itemID)
{
    if (s_pInstance)
    {
        s_pInstance->m_mapEquippedItems[itemID] = TRUE;
        s_pInstance->NotifyEquipmentChange(itemID, TRUE);
    }
}

void CEquipmentDetector::OnItemUnequipped(DWORD itemID)
{
    if (s_pInstance)
    {
        s_pInstance->m_mapEquippedItems[itemID] = FALSE;
        s_pInstance->NotifyEquipmentChange(itemID, FALSE);
    }
}