#pragma once

//! Core System Optimization for 4D Tech Panel Integration
//! Provides main core enhancements for better performance and compatibility

#include <map>
#include <string>

namespace TechPanelCore
{
    // Core optimization flags
    struct CORE_OPTIMIZATION_CONFIG
    {
        bool bEnableMemoryPooling;       // Enable object pooling for UI components
        bool bEnableTextureStreaming;    // Enable texture streaming for visual effects
        bool bEnableRenderOptimization;  // Enable render state optimization
        bool bEnableInputOptimization;   // Enable input processing optimization
        bool bEnableUpdateOptimization;  // Enable update loop optimization
        
        CORE_OPTIMIZATION_CONFIG()
            : bEnableMemoryPooling(true)
            , bEnableTextureStreaming(true)
            , bEnableRenderOptimization(true)
            , bEnableInputOptimization(true)
            , bEnableUpdateOptimization(true)
        {
        }
    };
    
    // Core system manager for tech panel integration
    class CCoreSystemManager
    {
    private:
        static CCoreSystemManager* s_pInstance;
        
        CORE_OPTIMIZATION_CONFIG m_Config;
        
        // Performance statistics
        struct CoreStats
        {
            DWORD dwFrameCount;
            float fAverageFrameTime;
            DWORD dwMemoryUsage;
            DWORD dwTextureMemory;
            DWORD dwRenderCalls;
            
            CoreStats() : dwFrameCount(0), fAverageFrameTime(0.0f), 
                         dwMemoryUsage(0), dwTextureMemory(0), dwRenderCalls(0) {}
        } m_Stats;
        
    public:
        CCoreSystemManager();
        ~CCoreSystemManager();
        
        static CCoreSystemManager& GetInstance();
        static void DestroyInstance();
        
        // Core optimization methods
        void Initialize();
        void Update(float fElapsedTime);
        void Shutdown();
        
        // Configuration
        void SetOptimizationConfig(const CORE_OPTIMIZATION_CONFIG& config) { m_Config = config; }
        const CORE_OPTIMIZATION_CONFIG& GetOptimizationConfig() const { return m_Config; }
        
        // Memory management optimization
        void OptimizeMemoryUsage();
        void ClearUnusedResources();
        
        // Render optimization
        void OptimizeRenderStates();
        void BatchRenderCalls();
        
        // Input optimization
        void OptimizeInputProcessing();
        
        // Statistics
        const CoreStats& GetCoreStats() const { return m_Stats; }
        void ResetStats();
        
        // Tech panel specific optimizations
        void RegisterTechPanel();
        void UnregisterTechPanel();
        bool IsTechPanelOptimized() const;
        
    private:
        void UpdateStatistics(float fElapsedTime);
        void CheckMemoryThresholds();
        void OptimizeTextureMemory();
    };
    
    // Main core optimization functions
    void InitializeCoreOptimizations();
    void ShutdownCoreOptimizations();
    void UpdateCoreOptimizations(float fElapsedTime);
    
    // Memory optimization
    void OptimizeGlobalMemory();
    void ClearUIResourceCache();
    
    // Rendering optimization
    void OptimizeRenderPipeline();
    void BatchUIRenderCalls();
    
    // Input system optimization
    void OptimizeInputSystem();
    void CacheInputStates();
    
    // Tech panel integration helpers
    bool IsSystemOptimizedForTechPanel();
    void PrepareCoreForTechPanel();
    void RestoreCoreAfterTechPanel();
}

// Convenience macro
#define TECH_CORE_MGR TechPanelCore::CCoreSystemManager::GetInstance()