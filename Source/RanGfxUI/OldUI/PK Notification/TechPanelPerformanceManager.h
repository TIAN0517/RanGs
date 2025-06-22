#pragma once

#include <windows.h>

//! Performance monitoring system for 4D Tech Panel
//! Automatically disables heavy effects when frame rate drops
class CTechPanelPerformanceManager
{
private:
    static CTechPanelPerformanceManager* s_pInstance;
    
    // Performance monitoring
    LARGE_INTEGER m_liFrequency;
    LARGE_INTEGER m_liLastTime;
    float m_fFrameTime;
    float m_fAverageFrameTime;
    int m_nFrameCount;
    
    // Performance thresholds
    float m_fTargetFPS;              // Target FPS (default: 60)
    float m_fMinimumFPS;             // Minimum acceptable FPS (default: 30)
    float m_fCriticalFPS;            // Critical FPS threshold (default: 20)
    
    // Performance states
    bool m_bPerformanceMode;         // Reduced effects mode
    bool m_bCriticalMode;            // Minimal effects mode
    bool m_bMonitoringEnabled;       // Enable performance monitoring
    
    // Effect control flags
    bool m_bParticleEffectsEnabled;
    bool m_bDataStreamEnabled;
    bool m_bNeonTextEnabled;
    bool m_b3DFloatingEnabled;
    bool m_bHolographicEnabled;
    
    // Statistics
    float m_fMinFPS;
    float m_fMaxFPS;
    int m_nPerformanceSwitches;
    
public:
    CTechPanelPerformanceManager();
    ~CTechPanelPerformanceManager();
    
    // Singleton access
    static CTechPanelPerformanceManager& GetInstance();
    static void DestroyInstance();
    
    // Core monitoring functions
    void Initialize();
    void Update();
    void Reset();
    
    // Performance configuration
    void SetTargetFPS(float fFPS) { m_fTargetFPS = fFPS; }
    void SetMinimumFPS(float fFPS) { m_fMinimumFPS = fFPS; }
    void SetCriticalFPS(float fFPS) { m_fCriticalFPS = fFPS; }
    
    void EnableMonitoring(bool bEnable) { m_bMonitoringEnabled = bEnable; }
    
    // Performance state queries
    bool IsPerformanceMode() const { return m_bPerformanceMode; }
    bool IsCriticalMode() const { return m_bCriticalMode; }
    float GetCurrentFPS() const;
    float GetAverageFPS() const;
    float GetFrameTime() const { return m_fFrameTime; }
    
    // Effect control queries
    bool AreParticleEffectsEnabled() const { return m_bParticleEffectsEnabled; }
    bool IsDataStreamEnabled() const { return m_bDataStreamEnabled; }
    bool IsNeonTextEnabled() const { return m_bNeonTextEnabled; }
    bool Is3DFloatingEnabled() const { return m_b3DFloatingEnabled; }
    bool IsHolographicEnabled() const { return m_bHolographicEnabled; }
    
    // Statistics
    float GetMinFPS() const { return m_fMinFPS; }
    float GetMaxFPS() const { return m_fMaxFPS; }
    int GetPerformanceSwitches() const { return m_nPerformanceSwitches; }
    
    // Manual performance control
    void ForcePerformanceMode(bool bEnable);
    void ForceCriticalMode(bool bEnable);
    
private:
    void UpdatePerformanceState();
    void EnablePerformanceMode();
    void DisablePerformanceMode();
    void EnableCriticalMode();
    void DisableCriticalMode();
    void UpdateStatistics();
};

// Convenience macro for accessing the singleton
#define TECH_PERF_MGR CTechPanelPerformanceManager::GetInstance()