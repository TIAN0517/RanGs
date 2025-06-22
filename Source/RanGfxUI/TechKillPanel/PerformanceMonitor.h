#pragma once

#include "./TechKillPanelManager.h"

/**
 * Performance Monitor
 * Real-time FPS monitoring with automatic quality adjustment
 * Target: Reduce rendering time from 15ms to <10ms for 60+ FPS
 */
class CPerformanceMonitor
{
public:
    // Performance statistics
    struct PERFORMANCE_STATS
    {
        float currentFPS;
        float averageFPS;
        float minFPS;
        float maxFPS;
        float frameTime;
        float averageFrameTime;
        DWORD totalFrames;
        float upTime;
    };
    
    // Quality recommendation
    struct QUALITY_RECOMMENDATION
    {
        CTechKillPanelManager::QUALITY_TIER recommendedTier;
        float confidence; // 0.0 to 1.0
        CString reason;
    };

public:
    CPerformanceMonitor();
    virtual ~CPerformanceMonitor();

public:
    // Initialization
    void Initialize();
    void Cleanup();
    
    // Main update function
    void Update(float fElapsedTime);
    
    // Performance measurement
    void BeginFrame();
    void EndFrame();
    
    // Statistics access
    const PERFORMANCE_STATS& GetStats() const { return m_stats; }
    float GetCurrentFPS() const { return m_stats.currentFPS; }
    float GetAverageFrameTime() const { return m_stats.averageFrameTime; }
    
    // Quality recommendation
    CTechKillPanelManager::QUALITY_TIER GetRecommendedQualityTier() const;
    QUALITY_RECOMMENDATION GetDetailedRecommendation() const;
    
    // Configuration
    void SetTargetFPS(float targetFPS) { m_fTargetFPS = targetFPS; }
    void SetAutoAdjustEnabled(BOOL bEnabled) { m_bAutoAdjustEnabled = bEnabled; }
    void SetSmoothingFrames(int frames) { m_nSmoothingFrames = max(1, min(frames, 120)); }
    
    // Manual quality override
    void SetQualityOverride(CTechKillPanelManager::QUALITY_TIER tier) { m_eQualityOverride = tier; m_bUseOverride = TRUE; }
    void ClearQualityOverride() { m_bUseOverride = FALSE; }

private:
    // Internal calculation functions
    void UpdateFPSCalculation(float fElapsedTime);
    void UpdateFrameTimeSmoothing(float frameTime);
    void UpdateQualityRecommendation();
    void ResetStatistics();
    
    // Quality analysis
    BOOL IsPerformanceStable() const;
    BOOL ShouldUpgradeQuality() const;
    BOOL ShouldDowngradeQuality() const;
    float GetPerformanceScore() const;

private:
    // Performance statistics
    PERFORMANCE_STATS m_stats;
    
    // Frame timing
    DWORD m_dwFrameStartTime;
    DWORD m_dwLastFrameTime;
    BOOL m_bFrameInProgress;
    
    // Smoothing and averaging
    std::vector<float> m_vecFrameTimes;
    int m_nSmoothingFrames;
    float m_fFrameTimeSum;
    
    // FPS calculation
    DWORD m_dwFPSCounterStart;
    DWORD m_dwFPSFrameCount;
    float m_fFPSUpdateInterval;
    
    // Quality recommendation
    CTechKillPanelManager::QUALITY_TIER m_eCurrentRecommendation;
    CTechKillPanelManager::QUALITY_TIER m_eLastRecommendation;
    float m_fRecommendationConfidence;
    CString m_strRecommendationReason;
    DWORD m_dwLastRecommendationChange;
    DWORD m_dwRecommendationCooldown;
    
    // Configuration
    float m_fTargetFPS;
    BOOL m_bAutoAdjustEnabled;
    CTechKillPanelManager::QUALITY_TIER m_eQualityOverride;
    BOOL m_bUseOverride;
    
    // Performance thresholds
    float m_fMinAcceptableFPS;
    float m_fOptimalFPS;
    float m_fMaxTargetFrameTime;
    float m_fOptimalFrameTime;
    
    // Stability tracking
    DWORD m_dwStabilityCheckFrames;
    DWORD m_dwCurrentStabilityFrames;
    float m_fStabilityThreshold;
    
    // Memory for trend analysis
    std::vector<float> m_vecRecentFPS;
    int m_nTrendAnalysisFrames;
};