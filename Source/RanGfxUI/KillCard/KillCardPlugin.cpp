#include "../../stdafx.h"
#include "KillCardPlugin.h"
#include "KillCardManager.h"
#include "../../enginelib/DxTools/PostProcessing/NSGlow.h"
#include "../../enginelib/DxEffect/DxEffectMan.h"
#include "../../../SigmaCore/DebugInclude.h"

// Static instance
CKillCardPluginManager* CKillCardPluginManager::s_pInstance = NULL;

// PostProcess callback function pointer
static void (*g_pfnOriginalPostProcess)(LPDIRECT3DDEVICE9) = NULL;

// External PostProcess hook function
void CALLBACK KillCardPostProcessHook(LPDIRECT3DDEVICE9 pDevice)
{
    // Call original PostProcess first
    if (g_pfnOriginalPostProcess)
        g_pfnOriginalPostProcess(pDevice);
    
    // Apply kill card effects
    CKillCardPluginManager& manager = CKillCardPluginManager::GetInstance();
    if (manager.IsPluginLoaded())
    {
        manager.OnPostProcessCallback(pDevice);
    }
}

CKillCardPluginManager::CKillCardPluginManager()
    : m_hPluginDLL(NULL)
    , m_pPluginInterface(NULL)
    , m_eCurrentState(PLUGIN_STATE_UNLOADED)
    , m_dwLastFPSCheck(0)
    , m_nFPSHistoryIndex(0)
    , m_bPostProcessHooked(FALSE)
    , m_pD3DDevice(NULL)
    , m_dwMemoryCheckInterval(1000) // 1 second
    , m_dwLastMemoryCheck(0)
{
    // Initialize default configuration
    m_sConfig.bEnabled = TRUE;
    m_sConfig.fEffectIntensity = 1.0f;
    m_sConfig.bPerformanceMonitoring = TRUE;
    m_sConfig.fMinFPS = 60.0f;
    m_sConfig.dwMaxMemoryMB = 100;
    m_sConfig.bAutoQualityAdjust = TRUE;
    m_sConfig.dwResolutionX = 1920;
    m_sConfig.dwResolutionY = 1080;
    
    // Initialize performance data
    memset(&m_sPerformanceData, 0, sizeof(m_sPerformanceData));
    memset(m_fFPSHistory, 0, sizeof(m_fFPSHistory));
    
    // Initialize function pointers
    m_pfnCreatePlugin = NULL;
    m_pfnDestroyPlugin = NULL;
    m_pfnGetPluginVersion = NULL;
}

CKillCardPluginManager::~CKillCardPluginManager()
{
    UnloadPlugin();
}

CKillCardPluginManager& CKillCardPluginManager::GetInstance()
{
    if (!s_pInstance)
    {
        s_pInstance = new CKillCardPluginManager();
    }
    return *s_pInstance;
}

void CKillCardPluginManager::ReleaseInstance()
{
    if (s_pInstance)
    {
        delete s_pInstance;
        s_pInstance = NULL;
    }
}

BOOL CKillCardPluginManager::LoadPlugin(const char* szPluginPath)
{
    // Unload existing plugin first
    UnloadPlugin();
    
    m_eCurrentState = PLUGIN_STATE_LOADING;
    
    // Load the plugin DLL
    m_hPluginDLL = LoadLibraryA(szPluginPath);
    if (!m_hPluginDLL)
    {
        m_eCurrentState = PLUGIN_STATE_ERROR;
        return FALSE;
    }
    
    // Get function pointers
    m_pfnCreatePlugin = (CreatePluginFunc)GetProcAddress(m_hPluginDLL, "CreateKillCardPlugin");
    m_pfnDestroyPlugin = (DestroyPluginFunc)GetProcAddress(m_hPluginDLL, "DestroyKillCardPlugin");
    m_pfnGetPluginVersion = (GetPluginVersionFunc)GetProcAddress(m_hPluginDLL, "GetKillCardPluginVersion");
    
    if (!m_pfnCreatePlugin || !m_pfnDestroyPlugin || !m_pfnGetPluginVersion)
    {
        FreeLibrary(m_hPluginDLL);
        m_hPluginDLL = NULL;
        m_eCurrentState = PLUGIN_STATE_ERROR;
        return FALSE;
    }
    
    // Check plugin version compatibility
    DWORD dwPluginVersion = m_pfnGetPluginVersion();
    if (dwPluginVersion != KILLCARD_PLUGIN_VERSION)
    {
        FreeLibrary(m_hPluginDLL);
        m_hPluginDLL = NULL;
        m_eCurrentState = PLUGIN_STATE_ERROR;
        return FALSE;
    }
    
    // Create plugin interface
    m_pPluginInterface = m_pfnCreatePlugin();
    if (!m_pPluginInterface)
    {
        FreeLibrary(m_hPluginDLL);
        m_hPluginDLL = NULL;
        m_eCurrentState = PLUGIN_STATE_ERROR;
        return FALSE;
    }
    
    m_eCurrentState = PLUGIN_STATE_LOADED;
    return TRUE;
}

void CKillCardPluginManager::UnloadPlugin()
{
    if (m_eCurrentState != PLUGIN_STATE_UNLOADED)
    {
        m_eCurrentState = PLUGIN_STATE_UNLOADING;
        
        // Unregister PostProcess hook
        UnregisterPostProcessLayer();
        
        // Destroy plugin interface
        if (m_pPluginInterface && m_pfnDestroyPlugin)
        {
            m_pPluginInterface->Shutdown();
            m_pfnDestroyPlugin(m_pPluginInterface);
            m_pPluginInterface = NULL;
        }
        
        // Unload DLL
        if (m_hPluginDLL)
        {
            FreeLibrary(m_hPluginDLL);
            m_hPluginDLL = NULL;
        }
        
        // Reset function pointers
        m_pfnCreatePlugin = NULL;
        m_pfnDestroyPlugin = NULL;
        m_pfnGetPluginVersion = NULL;
        
        m_eCurrentState = PLUGIN_STATE_UNLOADED;
    }
}

BOOL CKillCardPluginManager::InitializeDevice(LPDIRECT3DDEVICE9 pDevice)
{
    m_pD3DDevice = pDevice;
    
    if (m_pPluginInterface)
    {
        HWND hGameWnd = GetActiveWindow(); // Get game window handle
        if (!m_pPluginInterface->Initialize(hGameWnd, pDevice))
        {
            m_eCurrentState = PLUGIN_STATE_ERROR;
            return FALSE;
        }
        
        // Set configuration
        m_pPluginInterface->SetConfiguration(m_sConfig);
        
        // Register PostProcess hook
        RegisterPostProcessLayer();
        
        m_eCurrentState = PLUGIN_STATE_ACTIVE;
        return TRUE;
    }
    
    return FALSE;
}

void CKillCardPluginManager::OnDeviceLost()
{
    // Plugin handles device lost internally
    if (m_pPluginInterface)
    {
        UnregisterPostProcessLayer();
    }
}

void CKillCardPluginManager::OnDeviceReset()
{
    // Re-register PostProcess hook after device reset
    if (m_pPluginInterface && m_pD3DDevice)
    {
        RegisterPostProcessLayer();
    }
}

void CKillCardPluginManager::OnInventoryChanged(DWORD dwSlotType, DWORD dwItemID)
{
    if (!m_pPluginInterface)
        return;
    
    // Map item IDs to card types (this would be configured based on game data)
    EKILLCARD_EQUIPPED_STATE cardType = CARD_EQUIPPED_NONE;
    
    // Example mapping - these would be actual item IDs from the game
    switch (dwItemID)
    {
    case 50001: cardType = CARD_EQUIPPED_QUANTUM; break;   // 量子裂变卡
    case 50002: cardType = CARD_EQUIPPED_HOLOGRAM; break;  // 全息干扰卡
    case 50003: cardType = CARD_EQUIPPED_SPACETIME; break; // 时空扭曲卡
    case 50004: cardType = CARD_EQUIPPED_PLASMA; break;    // 等离子爆破卡
    case 50005: cardType = CARD_EQUIPPED_NANO; break;      // 纳米溶解卡
    case 50006: cardType = CARD_EQUIPPED_VIRTUAL; break;   // 虚拟重构卡
    default: return; // Not a kill card
    }
    
    // Check if card is being equipped or unequipped
    if (dwSlotType != 0) // Equipped to a slot
    {
        m_pPluginInterface->OnCardEquipped(cardType);
    }
    else // Removed from slot
    {
        m_pPluginInterface->OnCardUnequipped(cardType);
    }
}

EKILLCARD_EQUIPPED_STATE CKillCardPluginManager::GetEquippedCards() const
{
    if (!m_pPluginInterface)
        return CARD_EQUIPPED_NONE;
    
    EKILLCARD_EQUIPPED_STATE result = CARD_EQUIPPED_NONE;
    
    // Check each card type
    if (m_pPluginInterface->IsCardEquipped(CARD_EQUIPPED_QUANTUM))
        result = (EKILLCARD_EQUIPPED_STATE)(result | CARD_EQUIPPED_QUANTUM);
    if (m_pPluginInterface->IsCardEquipped(CARD_EQUIPPED_HOLOGRAM))
        result = (EKILLCARD_EQUIPPED_STATE)(result | CARD_EQUIPPED_HOLOGRAM);
    if (m_pPluginInterface->IsCardEquipped(CARD_EQUIPPED_SPACETIME))
        result = (EKILLCARD_EQUIPPED_STATE)(result | CARD_EQUIPPED_SPACETIME);
    if (m_pPluginInterface->IsCardEquipped(CARD_EQUIPPED_PLASMA))
        result = (EKILLCARD_EQUIPPED_STATE)(result | CARD_EQUIPPED_PLASMA);
    if (m_pPluginInterface->IsCardEquipped(CARD_EQUIPPED_NANO))
        result = (EKILLCARD_EQUIPPED_STATE)(result | CARD_EQUIPPED_NANO);
    if (m_pPluginInterface->IsCardEquipped(CARD_EQUIPPED_VIRTUAL))
        result = (EKILLCARD_EQUIPPED_STATE)(result | CARD_EQUIPPED_VIRTUAL);
    
    return result;
}

void CKillCardPluginManager::TriggerKillAnimation(EKILL_TYPE killType, const D3DXVECTOR3& worldPos)
{
    if (!m_pPluginInterface || !m_sConfig.bEnabled)
        return;
    
    // Only trigger if appropriate cards are equipped
    EKILLCARD_EQUIPPED_STATE equippedCards = GetEquippedCards();
    if (equippedCards == CARD_EQUIPPED_NONE)
        return;
    
    // Check if the kill type matches equipped cards
    BOOL bShouldTrigger = FALSE;
    switch (killType)
    {
    case KILL_TYPE_CRITICAL:
        bShouldTrigger = (equippedCards & CARD_EQUIPPED_QUANTUM) != 0;
        break;
    case KILL_TYPE_SKILL:
        bShouldTrigger = (equippedCards & CARD_EQUIPPED_HOLOGRAM) != 0;
        break;
    case KILL_TYPE_CONSECUTIVE:
        bShouldTrigger = (equippedCards & CARD_EQUIPPED_SPACETIME) != 0;
        break;
    case KILL_TYPE_RANGED:
        bShouldTrigger = (equippedCards & CARD_EQUIPPED_PLASMA) != 0;
        break;
    case KILL_TYPE_MELEE:
        bShouldTrigger = (equippedCards & CARD_EQUIPPED_NANO) != 0;
        break;
    case KILL_TYPE_HEADSHOT:
        bShouldTrigger = (equippedCards & CARD_EQUIPPED_VIRTUAL) != 0;
        break;
    }
    
    if (bShouldTrigger)
    {
        m_pPluginInterface->TriggerKillEffect(killType, worldPos);
    }
}

void CKillCardPluginManager::UpdatePerformanceData(float fElapsedTime)
{
    DWORD dwCurrentTime = GetTickCount();
    
    // Update FPS
    if (dwCurrentTime - m_dwLastFPSCheck >= 100) // Update every 100ms
    {
        float fCurrentFPS = (fElapsedTime > 0.0f) ? (1.0f / fElapsedTime) : 0.0f;
        UpdateFPSHistory(fCurrentFPS);
        
        m_sPerformanceData.fCurrentFPS = fCurrentFPS;
        m_sPerformanceData.fAverageFPS = CalculateAverageFPS();
        
        m_dwLastFPSCheck = dwCurrentTime;
    }
    
    // Update memory usage
    if (dwCurrentTime - m_dwLastMemoryCheck >= m_dwMemoryCheckInterval)
    {
        CheckMemoryUsage();
        m_dwLastMemoryCheck = dwCurrentTime;
    }
    
    // Check performance thresholds and auto-optimize if needed
    if (m_sConfig.bAutoQualityAdjust && CheckPerformanceThresholds())
    {
        AutoOptimizeSettings();
    }
}

BOOL CKillCardPluginManager::CheckPerformanceThresholds()
{
    // Check if FPS is below threshold
    if (m_sPerformanceData.fAverageFPS < m_sConfig.fMinFPS)
        return TRUE;
    
    // Check if memory usage is too high
    if (m_sPerformanceData.dwMemoryUsage > m_sConfig.dwMaxMemoryMB)
        return TRUE;
    
    return FALSE;
}

void CKillCardPluginManager::AutoOptimizeSettings()
{
    float fTargetFPS = m_sConfig.fMinFPS;
    
    // Reduce effect intensity if FPS is low
    if (m_sPerformanceData.fAverageFPS < fTargetFPS * 0.8f)
    {
        m_sConfig.fEffectIntensity = max(0.1f, m_sConfig.fEffectIntensity * 0.8f);
    }
    // Increase effect intensity if FPS is good
    else if (m_sPerformanceData.fAverageFPS > fTargetFPS * 1.2f)
    {
        m_sConfig.fEffectIntensity = min(3.0f, m_sConfig.fEffectIntensity * 1.1f);
    }
    
    // Apply new settings
    if (m_pPluginInterface)
    {
        m_pPluginInterface->SetConfiguration(m_sConfig);
    }
}

BOOL CKillCardPluginManager::RegisterPostProcessLayer()
{
    if (m_bPostProcessHooked || !m_pPluginInterface)
        return FALSE;
    
    // Register PostProcess hook with the plugin
    if (m_pPluginInterface->RegisterPostProcessHook())
    {
        m_bPostProcessHooked = TRUE;
        return TRUE;
    }
    
    return FALSE;
}

void CKillCardPluginManager::UnregisterPostProcessLayer()
{
    if (m_bPostProcessHooked && m_pPluginInterface)
    {
        m_pPluginInterface->UnregisterPostProcessHook();
        m_bPostProcessHooked = FALSE;
    }
}

void CKillCardPluginManager::OnPostProcessCallback(LPDIRECT3DDEVICE9 pDevice)
{
    if (m_pPluginInterface && m_sConfig.bEnabled)
    {
        m_pPluginInterface->OnPostProcessRender(pDevice);
    }
}

void CKillCardPluginManager::UpdateFPSHistory(float fCurrentFPS)
{
    m_fFPSHistory[m_nFPSHistoryIndex] = fCurrentFPS;
    m_nFPSHistoryIndex = (m_nFPSHistoryIndex + 1) % 60;
}

float CKillCardPluginManager::CalculateAverageFPS() const
{
    float fSum = 0.0f;
    int nCount = 0;
    
    for (int i = 0; i < 60; i++)
    {
        if (m_fFPSHistory[i] > 0.0f)
        {
            fSum += m_fFPSHistory[i];
            nCount++;
        }
    }
    
    return (nCount > 0) ? (fSum / nCount) : 0.0f;
}

void CKillCardPluginManager::CheckMemoryUsage()
{
    // Get process memory info
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
    {
        m_sPerformanceData.dwMemoryUsage = (DWORD)(pmc.WorkingSetSize / (1024 * 1024)); // Convert to MB
    }
    
    // Get video memory usage (simplified)
    if (m_pD3DDevice)
    {
        // This is a simplified approach - actual implementation would query D3D resources
        m_sPerformanceData.dwVMemoryUsage = 50; // Placeholder value
    }
}

// Static callback functions for external interface
void CALLBACK CKillCardPluginManager::OnExternalKillTrigger(EKILL_TYPE killType, float x, float y, float z)
{
    D3DXVECTOR3 worldPos(x, y, z);
    GetInstance().TriggerKillAnimation(killType, worldPos);
}

BOOL CALLBACK CKillCardPluginManager::OnExternalCardCheck(EKILLCARD_EQUIPPED_STATE cardType)
{
    EKILLCARD_EQUIPPED_STATE equipped = GetInstance().GetEquippedCards();
    return (equipped & cardType) != 0;
}

void CALLBACK CKillCardPluginManager::OnExternalConfigChange(const SKillCardPluginConfig* pConfig)
{
    if (pConfig)
    {
        GetInstance().SetConfiguration(*pConfig);
    }
}

// DLL Export functions
extern "C" {

BOOL WINAPI KillCard_Initialize(HWND hGameWnd, LPDIRECT3DDEVICE9 pDevice)
{
    return CKillCardPluginManager::GetInstance().InitializeDevice(pDevice);
}

void WINAPI KillCard_Shutdown()
{
    CKillCardPluginManager::GetInstance().UnloadPlugin();
    CKillCardPluginManager::ReleaseInstance();
}

void WINAPI KillCard_TriggerKill(int killType, float x, float y, float z)
{
    D3DXVECTOR3 worldPos(x, y, z);
    CKillCardPluginManager::GetInstance().TriggerKillAnimation((EKILL_TYPE)killType, worldPos);
}

BOOL WINAPI KillCard_IsCardEquipped(int cardType)
{
    EKILLCARD_EQUIPPED_STATE equipped = CKillCardPluginManager::GetInstance().GetEquippedCards();
    return (equipped & cardType) != 0;
}

void WINAPI KillCard_SetConfig(const SKillCardPluginConfig* pConfig)
{
    if (pConfig)
    {
        CKillCardPluginManager::GetInstance().SetConfiguration(*pConfig);
    }
}

void WINAPI KillCard_GetPerformanceData(SKillCardPerformanceData* pData)
{
    if (pData)
    {
        *pData = CKillCardPluginManager::GetInstance().GetPerformanceData();
    }
}

} // extern "C"