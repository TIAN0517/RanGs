#include "stdafx.h"
#include "TechPanelPerformanceManager.h"
#include "../../../SigmaCore/DebugInclude.h"

CTechPanelPerformanceManager* CTechPanelPerformanceManager::s_pInstance = NULL;

CTechPanelPerformanceManager::CTechPanelPerformanceManager()
    : m_fFrameTime(0.0f)
    , m_fAverageFrameTime(0.0f)
    , m_nFrameCount(0)
    , m_fTargetFPS(60.0f)
    , m_fMinimumFPS(30.0f)
    , m_fCriticalFPS(20.0f)
    , m_bPerformanceMode(false)
    , m_bCriticalMode(false)
    , m_bMonitoringEnabled(true)
    , m_bParticleEffectsEnabled(true)
    , m_bDataStreamEnabled(true)
    , m_bNeonTextEnabled(true)
    , m_b3DFloatingEnabled(true)
    , m_bHolographicEnabled(true)
    , m_fMinFPS(999.0f)
    , m_fMaxFPS(0.0f)
    , m_nPerformanceSwitches(0)
{
    Initialize();
}

CTechPanelPerformanceManager::~CTechPanelPerformanceManager()
{
}

CTechPanelPerformanceManager& CTechPanelPerformanceManager::GetInstance()
{
    if (!s_pInstance)
    {
        s_pInstance = new CTechPanelPerformanceManager();
    }
    return *s_pInstance;
}

void CTechPanelPerformanceManager::DestroyInstance()
{
    if (s_pInstance)
    {
        delete s_pInstance;
        s_pInstance = NULL;
    }
}

void CTechPanelPerformanceManager::Initialize()
{
    // Initialize high-resolution timer
    if (!QueryPerformanceFrequency(&m_liFrequency))
    {
        m_bMonitoringEnabled = false;
        return;
    }
    
    QueryPerformanceCounter(&m_liLastTime);
    
    // Reset all performance metrics
    Reset();
}

void CTechPanelPerformanceManager::Reset()
{
    m_fFrameTime = 0.0f;
    m_fAverageFrameTime = 0.0f;
    m_nFrameCount = 0;
    m_fMinFPS = 999.0f;
    m_fMaxFPS = 0.0f;
    m_nPerformanceSwitches = 0;
    
    // Reset to normal performance mode
    m_bPerformanceMode = false;
    m_bCriticalMode = false;
    m_bParticleEffectsEnabled = true;
    m_bDataStreamEnabled = true;
    m_bNeonTextEnabled = true;
    m_b3DFloatingEnabled = true;
    m_bHolographicEnabled = true;
}

void CTechPanelPerformanceManager::Update()
{
    if (!m_bMonitoringEnabled) return;
    
    // Calculate frame time using high-resolution timer
    LARGE_INTEGER liCurrentTime;
    QueryPerformanceCounter(&liCurrentTime);
    
    m_fFrameTime = (float)(liCurrentTime.QuadPart - m_liLastTime.QuadPart) / (float)m_liFrequency.QuadPart;
    m_liLastTime = liCurrentTime;
    
    // Update average frame time (moving average over 60 frames)
    const int AVERAGE_FRAME_COUNT = 60;
    m_fAverageFrameTime = (m_fAverageFrameTime * (AVERAGE_FRAME_COUNT - 1) + m_fFrameTime) / AVERAGE_FRAME_COUNT;
    m_nFrameCount++;
    
    // Update performance state based on current FPS
    UpdatePerformanceState();
    
    // Update statistics
    UpdateStatistics();
}

float CTechPanelPerformanceManager::GetCurrentFPS() const
{
    if (m_fFrameTime <= 0.0f) return 0.0f;
    return 1.0f / m_fFrameTime;
}

float CTechPanelPerformanceManager::GetAverageFPS() const
{
    if (m_fAverageFrameTime <= 0.0f) return 0.0f;
    return 1.0f / m_fAverageFrameTime;
}

void CTechPanelPerformanceManager::UpdatePerformanceState()
{
    float currentFPS = GetCurrentFPS();
    float averageFPS = GetAverageFPS();
    
    // Use average FPS for more stable performance decisions after initial frames
    float referenceFPS = (m_nFrameCount > 60) ? averageFPS : currentFPS;
    
    bool shouldEnableCritical = referenceFPS < m_fCriticalFPS;
    bool shouldEnablePerformance = referenceFPS < m_fMinimumFPS;
    
    // Critical mode check
    if (shouldEnableCritical && !m_bCriticalMode)
    {
        EnableCriticalMode();
    }
    else if (!shouldEnableCritical && m_bCriticalMode)
    {
        DisableCriticalMode();
    }
    // Performance mode check (only if not in critical mode)
    else if (shouldEnablePerformance && !m_bPerformanceMode && !m_bCriticalMode)
    {
        EnablePerformanceMode();
    }
    else if (!shouldEnablePerformance && m_bPerformanceMode && !m_bCriticalMode)
    {
        DisablePerformanceMode();
    }
}

void CTechPanelPerformanceManager::EnablePerformanceMode()
{
    if (m_bPerformanceMode) return;
    
    m_bPerformanceMode = true;
    m_nPerformanceSwitches++;
    
    // Disable heavy effects
    m_bParticleEffectsEnabled = false;
    m_bDataStreamEnabled = false;
    // Keep other effects enabled in performance mode
    
    // Log performance mode activation
    // OutputDebugString("Tech Panel: Performance mode activated\n");
}

void CTechPanelPerformanceManager::DisablePerformanceMode()
{
    if (!m_bPerformanceMode) return;
    
    m_bPerformanceMode = false;
    
    // Re-enable effects if not in critical mode
    if (!m_bCriticalMode)
    {
        m_bParticleEffectsEnabled = true;
        m_bDataStreamEnabled = true;
        m_bNeonTextEnabled = true;
        m_b3DFloatingEnabled = true;
        m_bHolographicEnabled = true;
    }
    
    // Log performance mode deactivation
    // OutputDebugString("Tech Panel: Performance mode deactivated\n");
}

void CTechPanelPerformanceManager::EnableCriticalMode()
{
    if (m_bCriticalMode) return;
    
    m_bCriticalMode = true;
    m_bPerformanceMode = true; // Critical mode includes performance mode
    m_nPerformanceSwitches++;
    
    // Disable all heavy effects
    m_bParticleEffectsEnabled = false;
    m_bDataStreamEnabled = false;
    m_bNeonTextEnabled = false;
    m_b3DFloatingEnabled = false;
    // Keep only basic holographic panel
    
    // Log critical mode activation
    // OutputDebugString("Tech Panel: Critical performance mode activated\n");
}

void CTechPanelPerformanceManager::DisableCriticalMode()
{
    if (!m_bCriticalMode) return;
    
    m_bCriticalMode = false;
    m_bPerformanceMode = false;
    
    // Re-enable all effects
    m_bParticleEffectsEnabled = true;
    m_bDataStreamEnabled = true;
    m_bNeonTextEnabled = true;
    m_b3DFloatingEnabled = true;
    m_bHolographicEnabled = true;
    
    // Log critical mode deactivation
    // OutputDebugString("Tech Panel: Critical performance mode deactivated\n");
}

void CTechPanelPerformanceManager::UpdateStatistics()
{
    float currentFPS = GetCurrentFPS();
    
    if (currentFPS < m_fMinFPS)
        m_fMinFPS = currentFPS;
    
    if (currentFPS > m_fMaxFPS)
        m_fMaxFPS = currentFPS;
}

void CTechPanelPerformanceManager::ForcePerformanceMode(bool bEnable)
{
    if (bEnable)
    {
        EnablePerformanceMode();
    }
    else
    {
        DisablePerformanceMode();
    }
}

void CTechPanelPerformanceManager::ForceCriticalMode(bool bEnable)
{
    if (bEnable)
    {
        EnableCriticalMode();
    }
    else
    {
        DisableCriticalMode();
    }
}