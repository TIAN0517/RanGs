#include "stdafx.h"
#include "./PerformanceMonitor.h"

#include <algorithm>
#include <numeric>

CPerformanceMonitor::CPerformanceMonitor()
    : m_dwFrameStartTime(0)
    , m_dwLastFrameTime(0)
    , m_bFrameInProgress(FALSE)
    , m_nSmoothingFrames(30)
    , m_fFrameTimeSum(0.0f)
    , m_dwFPSCounterStart(0)
    , m_dwFPSFrameCount(0)
    , m_fFPSUpdateInterval(0.5f) // Update FPS counter every 0.5 seconds
    , m_eCurrentRecommendation(CTechKillPanelManager::QUALITY_STANDARD)
    , m_eLastRecommendation(CTechKillPanelManager::QUALITY_STANDARD)
    , m_fRecommendationConfidence(1.0f)
    , m_dwLastRecommendationChange(0)
    , m_dwRecommendationCooldown(3000) // 3 second cooldown between changes
    , m_fTargetFPS(60.0f)
    , m_bAutoAdjustEnabled(TRUE)
    , m_eQualityOverride(CTechKillPanelManager::QUALITY_STANDARD)
    , m_bUseOverride(FALSE)
    , m_fMinAcceptableFPS(30.0f)
    , m_fOptimalFPS(60.0f)
    , m_fMaxTargetFrameTime(16.67f) // 60 FPS = 16.67ms
    , m_fOptimalFrameTime(10.0f)    // Target: reduce from 15ms to <10ms
    , m_dwStabilityCheckFrames(60)
    , m_dwCurrentStabilityFrames(0)
    , m_fStabilityThreshold(5.0f)   // 5 FPS variance for stability
    , m_nTrendAnalysisFrames(180)   // 3 seconds at 60 FPS
{
    // Initialize statistics
    memset(&m_stats, 0, sizeof(PERFORMANCE_STATS));
    m_stats.minFPS = FLT_MAX;
    
    // Reserve memory for frame time collections
    m_vecFrameTimes.reserve(m_nSmoothingFrames);
    m_vecRecentFPS.reserve(m_nTrendAnalysisFrames);
}

CPerformanceMonitor::~CPerformanceMonitor()
{
    Cleanup();
}

void CPerformanceMonitor::Initialize()
{
    ResetStatistics();
    
    m_dwFPSCounterStart = GetTickCount();
    m_dwLastFrameTime = m_dwFPSCounterStart;
}

void CPerformanceMonitor::Cleanup()
{
    m_vecFrameTimes.clear();
    m_vecRecentFPS.clear();
}

void CPerformanceMonitor::Update(float fElapsedTime)
{
    UpdateFPSCalculation(fElapsedTime);
    
    if (m_bAutoAdjustEnabled && !m_bUseOverride)
    {
        UpdateQualityRecommendation();
    }
    
    m_stats.upTime += fElapsedTime;
}

void CPerformanceMonitor::BeginFrame()
{
    m_dwFrameStartTime = GetTickCount();
    m_bFrameInProgress = TRUE;
}

void CPerformanceMonitor::EndFrame()
{
    if (!m_bFrameInProgress) return;
    
    DWORD dwCurrentTime = GetTickCount();
    float frameTime = (dwCurrentTime - m_dwFrameStartTime);
    
    // Update frame time smoothing
    UpdateFrameTimeSmoothing(frameTime);
    
    // Update statistics
    m_stats.frameTime = frameTime;
    m_stats.totalFrames++;
    
    // Track for trend analysis
    if (m_stats.currentFPS > 0)
    {
        m_vecRecentFPS.push_back(m_stats.currentFPS);
        if ((int)m_vecRecentFPS.size() > m_nTrendAnalysisFrames)
        {
            m_vecRecentFPS.erase(m_vecRecentFPS.begin());
        }
    }
    
    m_bFrameInProgress = FALSE;
    m_dwLastFrameTime = dwCurrentTime;
}

CTechKillPanelManager::QUALITY_TIER CPerformanceMonitor::GetRecommendedQualityTier() const
{
    if (m_bUseOverride)
    {
        return m_eQualityOverride;
    }
    
    return m_eCurrentRecommendation;
}

CPerformanceMonitor::QUALITY_RECOMMENDATION CPerformanceMonitor::GetDetailedRecommendation() const
{
    QUALITY_RECOMMENDATION recommendation;
    recommendation.recommendedTier = GetRecommendedQualityTier();
    recommendation.confidence = m_fRecommendationConfidence;
    recommendation.reason = m_strRecommendationReason;
    
    return recommendation;
}

void CPerformanceMonitor::UpdateFPSCalculation(float fElapsedTime)
{
    DWORD dwCurrentTime = GetTickCount();
    m_dwFPSFrameCount++;
    
    // Update FPS every interval
    DWORD dwElapsed = dwCurrentTime - m_dwFPSCounterStart;
    if (dwElapsed >= (DWORD)(m_fFPSUpdateInterval * 1000))
    {
        m_stats.currentFPS = m_dwFPSFrameCount / (dwElapsed / 1000.0f);
        
        // Update min/max
        if (m_stats.currentFPS < m_stats.minFPS)
            m_stats.minFPS = m_stats.currentFPS;
        if (m_stats.currentFPS > m_stats.maxFPS)
            m_stats.maxFPS = m_stats.currentFPS;
        
        // Calculate average FPS
        if (m_stats.totalFrames > 0)
        {
            m_stats.averageFPS = m_stats.totalFrames / m_stats.upTime;
        }
        
        // Reset counters
        m_dwFPSCounterStart = dwCurrentTime;
        m_dwFPSFrameCount = 0;
    }
}

void CPerformanceMonitor::UpdateFrameTimeSmoothing(float frameTime)
{
    m_vecFrameTimes.push_back(frameTime);
    m_fFrameTimeSum += frameTime;
    
    // Remove old frame times if we exceed smoothing window
    if ((int)m_vecFrameTimes.size() > m_nSmoothingFrames)
    {
        m_fFrameTimeSum -= m_vecFrameTimes[0];
        m_vecFrameTimes.erase(m_vecFrameTimes.begin());
    }
    
    // Calculate smoothed average
    if (!m_vecFrameTimes.empty())
    {
        m_stats.averageFrameTime = m_fFrameTimeSum / m_vecFrameTimes.size();
    }
}

void CPerformanceMonitor::UpdateQualityRecommendation()
{
    DWORD dwCurrentTime = GetTickCount();
    
    // Don't change recommendations too frequently
    if (dwCurrentTime - m_dwLastRecommendationChange < m_dwRecommendationCooldown)
    {
        return;
    }
    
    // Need sufficient data for reliable recommendation
    if (m_vecRecentFPS.size() < 30) // At least 30 frames
    {
        return;
    }
    
    CTechKillPanelManager::QUALITY_TIER newRecommendation = m_eCurrentRecommendation;
    float confidence = 0.5f;
    CString reason = _T("Insufficient data");
    
    // Analyze current performance
    float avgFPS = std::accumulate(m_vecRecentFPS.begin(), m_vecRecentFPS.end(), 0.0f) / m_vecRecentFPS.size();
    float minRecentFPS = *std::min_element(m_vecRecentFPS.begin(), m_vecRecentFPS.end());
    float maxRecentFPS = *std::max_element(m_vecRecentFPS.begin(), m_vecRecentFPS.end());
    
    // Performance analysis
    if (avgFPS >= m_fOptimalFPS && minRecentFPS >= m_fOptimalFPS * 0.9f && 
        m_stats.averageFrameTime <= m_fOptimalFrameTime)
    {
        // Excellent performance - can upgrade quality
        if (m_eCurrentRecommendation == CTechKillPanelManager::QUALITY_PERFORMANCE)
        {
            newRecommendation = CTechKillPanelManager::QUALITY_STANDARD;
            confidence = 0.8f;
            reason = _T("Excellent performance, upgrading to Standard quality");
        }
        else if (m_eCurrentRecommendation == CTechKillPanelManager::QUALITY_STANDARD)
        {
            newRecommendation = CTechKillPanelManager::QUALITY_ULTIMATE;
            confidence = 0.9f;
            reason = _T("Optimal performance, upgrading to Ultimate quality");
        }
        else
        {
            confidence = 1.0f;
            reason = _T("Maintaining Ultimate quality - excellent performance");
        }
    }
    else if (avgFPS >= m_fTargetFPS && minRecentFPS >= m_fTargetFPS * 0.8f)
    {
        // Good performance - maintain current quality
        if (m_eCurrentRecommendation == CTechKillPanelManager::QUALITY_PERFORMANCE)
        {
            // Can try upgrading from Performance
            newRecommendation = CTechKillPanelManager::QUALITY_STANDARD;
            confidence = 0.7f;
            reason = _T("Good performance, upgrading to Standard quality");
        }
        else
        {
            confidence = 0.8f;
            reason = _T("Good performance, maintaining current quality");
        }
    }
    else if (avgFPS >= m_fMinAcceptableFPS && minRecentFPS >= m_fMinAcceptableFPS * 0.8f)
    {
        // Acceptable performance - might need to downgrade
        if (m_eCurrentRecommendation == CTechKillPanelManager::QUALITY_ULTIMATE)
        {
            newRecommendation = CTechKillPanelManager::QUALITY_STANDARD;
            confidence = 0.7f;
            reason = _T("Performance below optimal, downgrading to Standard quality");
        }
        else if (m_eCurrentRecommendation == CTechKillPanelManager::QUALITY_STANDARD)
        {
            confidence = 0.6f;
            reason = _T("Borderline performance, monitoring...");
        }
    }
    else
    {
        // Poor performance - definitely downgrade
        if (m_eCurrentRecommendation == CTechKillPanelManager::QUALITY_ULTIMATE)
        {
            newRecommendation = CTechKillPanelManager::QUALITY_STANDARD;
            confidence = 0.9f;
            reason = _T("Poor performance, downgrading to Standard quality");
        }
        else if (m_eCurrentRecommendation == CTechKillPanelManager::QUALITY_STANDARD)
        {
            newRecommendation = CTechKillPanelManager::QUALITY_PERFORMANCE;
            confidence = 0.9f;
            reason = _T("Poor performance, downgrading to Performance quality");
        }
        else
        {
            confidence = 0.8f;
            reason = _T("Poor performance, maintaining Performance quality");
        }
    }
    
    // Check for frame time spikes
    if (m_stats.averageFrameTime > m_fMaxTargetFrameTime * 1.5f)
    {
        // Severe frame time issues - aggressive downgrade
        if (newRecommendation > CTechKillPanelManager::QUALITY_PERFORMANCE)
        {
            newRecommendation = (CTechKillPanelManager::QUALITY_TIER)(newRecommendation - 1);
            confidence = min(confidence + 0.2f, 1.0f);
            reason = _T("Frame time spikes detected, reducing quality");
        }
    }
    
    // Update recommendation if it changed
    if (newRecommendation != m_eCurrentRecommendation)
    {
        m_eLastRecommendation = m_eCurrentRecommendation;
        m_eCurrentRecommendation = newRecommendation;
        m_dwLastRecommendationChange = dwCurrentTime;
    }
    
    m_fRecommendationConfidence = confidence;
    m_strRecommendationReason = reason;
}

void CPerformanceMonitor::ResetStatistics()
{
    memset(&m_stats, 0, sizeof(PERFORMANCE_STATS));
    m_stats.minFPS = FLT_MAX;
    
    m_vecFrameTimes.clear();
    m_vecRecentFPS.clear();
    m_fFrameTimeSum = 0.0f;
    m_dwFPSFrameCount = 0;
    m_dwCurrentStabilityFrames = 0;
}

BOOL CPerformanceMonitor::IsPerformanceStable() const
{
    if (m_vecRecentFPS.size() < m_dwStabilityCheckFrames)
        return FALSE;
    
    // Check last N frames for stability
    size_t startIdx = m_vecRecentFPS.size() - m_dwStabilityCheckFrames;
    float sum = 0.0f;
    float sumSq = 0.0f;
    
    for (size_t i = startIdx; i < m_vecRecentFPS.size(); ++i)
    {
        sum += m_vecRecentFPS[i];
        sumSq += m_vecRecentFPS[i] * m_vecRecentFPS[i];
    }
    
    float mean = sum / m_dwStabilityCheckFrames;
    float variance = (sumSq / m_dwStabilityCheckFrames) - (mean * mean);
    float stdDev = sqrtf(variance);
    
    return stdDev <= m_fStabilityThreshold;
}

BOOL CPerformanceMonitor::ShouldUpgradeQuality() const
{
    return m_stats.currentFPS >= m_fOptimalFPS && 
           m_stats.averageFrameTime <= m_fOptimalFrameTime &&
           IsPerformanceStable();
}

BOOL CPerformanceMonitor::ShouldDowngradeQuality() const
{
    return m_stats.currentFPS < m_fMinAcceptableFPS || 
           m_stats.averageFrameTime > m_fMaxTargetFrameTime;
}

float CPerformanceMonitor::GetPerformanceScore() const
{
    // Calculate overall performance score (0.0 to 1.0)
    float fpsScore = min(m_stats.currentFPS / m_fOptimalFPS, 1.0f);
    float frameTimeScore = min(m_fOptimalFrameTime / max(m_stats.averageFrameTime, 1.0f), 1.0f);
    float stabilityScore = IsPerformanceStable() ? 1.0f : 0.5f;
    
    return (fpsScore * 0.4f + frameTimeScore * 0.4f + stabilityScore * 0.2f);
}