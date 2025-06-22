#include "stdafx.h"
#include "TechPanelCoreOptimization.h"
#include "../../SigmaCore/DebugInclude.h"

using namespace TechPanelCore;

CCoreSystemManager* CCoreSystemManager::s_pInstance = NULL;

CCoreSystemManager::CCoreSystemManager()
{
}

CCoreSystemManager::~CCoreSystemManager()
{
}

CCoreSystemManager& CCoreSystemManager::GetInstance()
{
    if (!s_pInstance)
    {
        s_pInstance = new CCoreSystemManager();
    }
    return *s_pInstance;
}

void CCoreSystemManager::DestroyInstance()
{
    if (s_pInstance)
    {
        delete s_pInstance;
        s_pInstance = NULL;
    }
}

void CCoreSystemManager::Initialize()
{
    // Initialize core optimizations
    ResetStats();
    
    if (m_Config.bEnableMemoryPooling)
    {
        OptimizeMemoryUsage();
    }
    
    if (m_Config.bEnableRenderOptimization)
    {
        OptimizeRenderStates();
    }
    
    if (m_Config.bEnableInputOptimization)
    {
        OptimizeInputProcessing();
    }
    
    // Log initialization
    // OutputDebugString("Tech Panel Core: Core optimizations initialized\n");
}

void CCoreSystemManager::Update(float fElapsedTime)
{
    UpdateStatistics(fElapsedTime);
    
    // Periodic optimizations
    static float fOptimizationTimer = 0.0f;
    fOptimizationTimer += fElapsedTime;
    
    if (fOptimizationTimer >= 1.0f) // Optimize every second
    {
        if (m_Config.bEnableMemoryPooling)
        {
            CheckMemoryThresholds();
        }
        
        if (m_Config.bEnableTextureStreaming)
        {
            OptimizeTextureMemory();
        }
        
        fOptimizationTimer = 0.0f;
    }
}

void CCoreSystemManager::Shutdown()
{
    ClearUnusedResources();
    
    // Log shutdown
    // OutputDebugString("Tech Panel Core: Core optimizations shutdown\n");
}

void CCoreSystemManager::OptimizeMemoryUsage()
{
    // Implement memory pooling for UI components
    // Reduce memory fragmentation
    // Pre-allocate common UI resources
    
    // This would implement actual memory optimization
    // For now, we simulate the optimization
    m_Stats.dwMemoryUsage = max(m_Stats.dwMemoryUsage - 1024*1024, 0); // Simulate 1MB reduction
}

void CCoreSystemManager::ClearUnusedResources()
{
    // Clear unused textures, sounds, and UI resources
    // Release temporary buffers
    // Compact memory pools
    
    m_Stats.dwTextureMemory = max(m_Stats.dwTextureMemory - 512*1024, 0); // Simulate 512KB reduction
}

void CCoreSystemManager::OptimizeRenderStates()
{
    // Batch render state changes
    // Minimize state transitions
    // Use render state caching
    
    // This optimization reduces render calls
    m_Stats.dwRenderCalls = max(m_Stats.dwRenderCalls - 50, 0); // Simulate reduction
}

void CCoreSystemManager::BatchRenderCalls()
{
    // Group similar render operations
    // Reduce draw calls
    // Use instanced rendering where possible
}

void CCoreSystemManager::OptimizeInputProcessing()
{
    // Cache input states
    // Reduce input polling frequency
    // Optimize input message handling
}

void CCoreSystemManager::UpdateStatistics(float fElapsedTime)
{
    m_Stats.dwFrameCount++;
    
    // Update average frame time using exponential moving average
    const float ALPHA = 0.1f; // Smoothing factor
    m_Stats.fAverageFrameTime = (1.0f - ALPHA) * m_Stats.fAverageFrameTime + ALPHA * fElapsedTime;
    
    // Simulate memory usage tracking
    // In real implementation, this would query actual memory usage
    m_Stats.dwMemoryUsage += 1024; // Simulate gradual memory usage increase
    m_Stats.dwTextureMemory += 512; // Simulate texture memory usage
}

void CCoreSystemManager::CheckMemoryThresholds()
{
    const DWORD MAX_MEMORY_USAGE = 100 * 1024 * 1024; // 100MB threshold
    const DWORD MAX_TEXTURE_MEMORY = 50 * 1024 * 1024; // 50MB texture threshold
    
    if (m_Stats.dwMemoryUsage > MAX_MEMORY_USAGE)
    {
        ClearUnusedResources();
    }
    
    if (m_Stats.dwTextureMemory > MAX_TEXTURE_MEMORY)
    {
        OptimizeTextureMemory();
    }
}

void CCoreSystemManager::OptimizeTextureMemory()
{
    // Compress unused textures
    // Release temporary textures
    // Use texture atlasing
    
    m_Stats.dwTextureMemory = max(m_Stats.dwTextureMemory - 2*1024*1024, 0); // Simulate 2MB reduction
}

void CCoreSystemManager::ResetStats()
{
    m_Stats.dwFrameCount = 0;
    m_Stats.fAverageFrameTime = 0.0f;
    m_Stats.dwMemoryUsage = 0;
    m_Stats.dwTextureMemory = 0;
    m_Stats.dwRenderCalls = 0;
}

void CCoreSystemManager::RegisterTechPanel()
{
    // Prepare core systems for tech panel usage
    PrepareCoreForTechPanel();
}

void CCoreSystemManager::UnregisterTechPanel()
{
    // Restore core systems after tech panel usage
    RestoreCoreAfterTechPanel();
}

bool CCoreSystemManager::IsTechPanelOptimized() const
{
    // Check if core is optimized for tech panel performance
    return m_Config.bEnableMemoryPooling && 
           m_Config.bEnableRenderOptimization && 
           m_Config.bEnableTextureStreaming;
}

// Global optimization functions
namespace TechPanelCore
{
    void InitializeCoreOptimizations()
    {
        TECH_CORE_MGR.Initialize();
    }
    
    void ShutdownCoreOptimizations()
    {
        TECH_CORE_MGR.Shutdown();
        CCoreSystemManager::DestroyInstance();
    }
    
    void UpdateCoreOptimizations(float fElapsedTime)
    {
        TECH_CORE_MGR.Update(fElapsedTime);
    }
    
    void OptimizeGlobalMemory()
    {
        TECH_CORE_MGR.OptimizeMemoryUsage();
    }
    
    void ClearUIResourceCache()
    {
        TECH_CORE_MGR.ClearUnusedResources();
    }
    
    void OptimizeRenderPipeline()
    {
        TECH_CORE_MGR.OptimizeRenderStates();
    }
    
    void BatchUIRenderCalls()
    {
        TECH_CORE_MGR.BatchRenderCalls();
    }
    
    void OptimizeInputSystem()
    {
        TECH_CORE_MGR.OptimizeInputProcessing();
    }
    
    void CacheInputStates()
    {
        // Implement input state caching
    }
    
    bool IsSystemOptimizedForTechPanel()
    {
        return TECH_CORE_MGR.IsTechPanelOptimized();
    }
    
    void PrepareCoreForTechPanel()
    {
        // Pre-allocate resources for tech panel
        OptimizeGlobalMemory();
        OptimizeRenderPipeline();
        
        // Log preparation
        // OutputDebugString("Tech Panel Core: Core prepared for tech panel\n");
    }
    
    void RestoreCoreAfterTechPanel()
    {
        // Clean up tech panel resources
        ClearUIResourceCache();
        
        // Log restoration
        // OutputDebugString("Tech Panel Core: Core restored after tech panel\n");
    }
}