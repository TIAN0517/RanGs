#pragma once

#include "../stdafx.h"
#include "KillCardPlugin.h"
#include <vector>
#include <deque>

/**
 * 击杀卡片性能监控系统
 * 实现实时性能监控、自动优化和阈值预警机制
 */

// 性能监控级别
enum EKILLCARD_MONITOR_LEVEL
{
    MONITOR_LEVEL_DISABLED = 0,      // 禁用监控
    MONITOR_LEVEL_BASIC,             // 基础监控（FPS、内存）
    MONITOR_LEVEL_DETAILED,          // 详细监控（渲染时间、Draw Call等）
    MONITOR_LEVEL_ADVANCED,          // 高级监控（GPU使用率、纹理内存等）
    MONITOR_LEVEL_PROFILING          // 性能分析模式（包含详细统计）
};

// 性能警告类型
enum EKILLCARD_PERFORMANCE_WARNING
{
    WARNING_NONE = 0,
    WARNING_LOW_FPS = 1,             // FPS过低
    WARNING_HIGH_MEMORY = 2,         // 内存使用过高
    WARNING_HIGH_VRAM = 4,           // 显存使用过高
    WARNING_LONG_RENDER = 8,         // 渲染时间过长
    WARNING_HIGH_DRAWCALLS = 16,     // Draw Call过多
    WARNING_TEXTURE_OVERLOAD = 32    // 纹理负载过高
};

// 自动优化策略
enum EKILLCARD_AUTO_OPTIMIZE_STRATEGY
{
    OPTIMIZE_STRATEGY_CONSERVATIVE,   // 保守策略：优先保证稳定性
    OPTIMIZE_STRATEGY_BALANCED,       // 平衡策略：平衡性能和质量
    OPTIMIZE_STRATEGY_AGGRESSIVE,     // 激进策略：优先保证性能
    OPTIMIZE_STRATEGY_CUSTOM         // 自定义策略
};

// 性能统计数据结构
struct SKillCardPerformanceStats
{
    // FPS统计
    float fCurrentFPS;               // 当前帧率
    float fAverageFPS;               // 平均帧率（1秒）
    float fMinFPS;                   // 最低帧率（1秒）
    float fMaxFPS;                   // 最高帧率（1秒）
    float f1PercentLowFPS;           // 1% Low FPS
    float f0_1PercentLowFPS;         // 0.1% Low FPS
    
    // 时间统计
    float fFrameTime;                // 帧时间 (ms)
    float fCPUTime;                  // CPU时间 (ms)
    float fGPUTime;                  // GPU时间 (ms)
    float fKillCardRenderTime;       // 击杀卡片渲染时间 (ms)
    float fPostProcessTime;          // 后处理时间 (ms)
    
    // 内存统计
    DWORD dwSystemMemoryMB;          // 系统内存使用 (MB)
    DWORD dwProcessMemoryMB;         // 进程内存使用 (MB)
    DWORD dwKillCardMemoryMB;        // 击杀卡片内存使用 (MB)
    DWORD dwVRAMUsageMB;             // 显存使用 (MB)
    DWORD dwTextureMemoryMB;         // 纹理内存使用 (MB)
    
    // 渲染统计
    DWORD dwDrawCalls;               // Draw Call数量
    DWORD dwTriangles;               // 三角形数量
    DWORD dwVertices;                // 顶点数量
    DWORD dwTextureChanges;          // 纹理切换次数
    DWORD dwShaderChanges;           // 着色器切换次数
    
    // 击杀卡片特定统计
    DWORD dwActiveEffects;           // 活跃效果数量
    DWORD dwParticleCount;           // 粒子数量
    DWORD dwAnimationsPlaying;       // 播放中的动画数量
    float fEffectComplexity;         // 效果复杂度评分
    
    // 警告标志
    DWORD dwWarningFlags;            // 性能警告标志位
    
    // 时间戳
    DWORD dwTimestamp;               // 统计时间戳
};

// 性能阈值配置
struct SKillCardPerformanceThresholds
{
    // FPS阈值
    float fMinAcceptableFPS;         // 最低可接受FPS
    float fTargetFPS;                // 目标FPS
    float fWarningFPS;               // 警告FPS阈值
    
    // 时间阈值
    float fMaxFrameTime;             // 最大帧时间 (ms)
    float fMaxRenderTime;            // 最大渲染时间 (ms)
    float fMaxPostProcessTime;       // 最大后处理时间 (ms)
    
    // 内存阈值
    DWORD dwMaxMemoryMB;             // 最大内存使用 (MB)
    DWORD dwMaxVRAMMB;               // 最大显存使用 (MB)
    DWORD dwWarningMemoryMB;         // 内存警告阈值 (MB)
    
    // 渲染阈值
    DWORD dwMaxDrawCalls;            // 最大Draw Call数量
    DWORD dwMaxTriangles;            // 最大三角形数量
    DWORD dwMaxActiveEffects;        // 最大活跃效果数量
    DWORD dwMaxParticles;            // 最大粒子数量
};

// 自动优化配置
struct SKillCardAutoOptimizeConfig
{
    EKILLCARD_AUTO_OPTIMIZE_STRATEGY eStrategy;  // 优化策略
    BOOL bEnabled;                               // 是否启用自动优化
    float fOptimizeInterval;                     // 优化检查间隔 (秒)
    float fStabilityTime;                        // 稳定性检查时间 (秒)
    
    // 优化参数
    float fEffectIntensityStep;                  // 效果强度调整步长
    float fParticleCountStep;                    // 粒子数量调整步长
    float fLODDistanceStep;                      // LOD距离调整步长
    
    // 优化限制
    float fMinEffectIntensity;                   // 最小效果强度
    float fMaxEffectIntensity;                   // 最大效果强度
    DWORD dwMinParticleCount;                    // 最小粒子数量
    DWORD dwMaxParticleCount;                    // 最大粒子数量
};

/**
 * 性能监控器核心类
 */
class CKillCardPerformanceMonitor
{
private:
    static CKillCardPerformanceMonitor* s_pInstance;
    
    // 监控配置
    EKILLCARD_MONITOR_LEVEL m_eMonitorLevel;
    SKillCardPerformanceThresholds m_sThresholds;
    SKillCardAutoOptimizeConfig m_sAutoOptimize;
    
    // 统计数据
    SKillCardPerformanceStats m_sCurrentStats;
    std::deque<SKillCardPerformanceStats> m_deqStatsHistory;
    const static int MAX_STATS_HISTORY = 600; // 10分钟历史（每秒1次）
    
    // FPS计算
    std::deque<float> m_deqFrameTimes;
    std::deque<float> m_deqFPSHistory;
    const static int MAX_FRAME_SAMPLES = 60; // 1秒的帧样本
    
    // 时间管理
    DWORD m_dwLastUpdateTime;
    DWORD m_dwLastOptimizeTime;
    DWORD m_dwLastWarningTime;
    float m_fAccumulatedTime;
    
    // D3D设备
    LPDIRECT3DDEVICE9 m_pD3DDevice;
    
    // 查询对象
    LPDIRECT3DQUERY9 m_pTimestampQuery;
    LPDIRECT3DQUERY9 m_pFrequencyQuery;
    LPDIRECT3DQUERY9 m_pDisjointQuery;
    
    // 状态
    BOOL m_bInitialized;
    BOOL m_bEnabled;

public:
    CKillCardPerformanceMonitor();
    ~CKillCardPerformanceMonitor();
    
    static CKillCardPerformanceMonitor& GetInstance();
    static void ReleaseInstance();

public:
    // 初始化和清理
    BOOL Initialize(LPDIRECT3DDEVICE9 pDevice);
    void Shutdown();
    void OnDeviceLost();
    void OnDeviceReset();
    
    // 监控控制
    void SetMonitorLevel(EKILLCARD_MONITOR_LEVEL eLevel) { m_eMonitorLevel = eLevel; }
    EKILLCARD_MONITOR_LEVEL GetMonitorLevel() const { return m_eMonitorLevel; }
    
    void SetEnabled(BOOL bEnabled) { m_bEnabled = bEnabled; }
    BOOL IsEnabled() const { return m_bEnabled; }
    
    // 性能数据收集
    void BeginFrame();
    void EndFrame();
    void BeginKillCardRender();
    void EndKillCardRender();
    void BeginPostProcess();
    void EndPostProcess();
    
    // 统计数据访问
    const SKillCardPerformanceStats& GetCurrentStats() const { return m_sCurrentStats; }
    void GetStatsHistory(std::vector<SKillCardPerformanceStats>& outHistory, DWORD dwSeconds) const;
    void GetFPSHistory(std::vector<float>& outFPSHistory, DWORD dwSeconds) const;
    
    // 阈值配置
    void SetThresholds(const SKillCardPerformanceThresholds& sThresholds) { m_sThresholds = sThresholds; }
    const SKillCardPerformanceThresholds& GetThresholds() const { return m_sThresholds; }
    
    // 自动优化
    void SetAutoOptimizeConfig(const SKillCardAutoOptimizeConfig& sConfig) { m_sAutoOptimize = sConfig; }
    const SKillCardAutoOptimizeConfig& GetAutoOptimizeConfig() const { return m_sAutoOptimize; }
    
    BOOL CheckPerformanceWarnings();
    void TriggerAutoOptimization();
    void ApplyOptimizationSettings(const SKillCardPluginConfig& sConfig);
    
    // 手动优化建议
    void GetOptimizationSuggestions(std::vector<std::string>& outSuggestions) const;
    float CalculatePerformanceScore() const;
    
    // 报告生成
    void GeneratePerformanceReport(std::string& outReport) const;
    void SavePerformanceLog(const char* szFilePath) const;
    
    // 事件通知
    typedef void (*PerformanceWarningCallback)(EKILLCARD_PERFORMANCE_WARNING eWarning, float fValue);
    typedef void (*OptimizationCallback)(const SKillCardPluginConfig& sNewConfig);
    
    void SetWarningCallback(PerformanceWarningCallback pfnCallback) { m_pfnWarningCallback = pfnCallback; }
    void SetOptimizationCallback(OptimizationCallback pfnCallback) { m_pfnOptimizationCallback = pfnCallback; }

private:
    // 内部方法
    void UpdateStats(float fElapsedTime);
    void CalculateFPSStats();
    void CollectMemoryStats();
    void CollectRenderStats();
    void CheckGPUTime();
    
    void CheckFPSWarnings();
    void CheckMemoryWarnings();
    void CheckRenderWarnings();
    
    void OptimizeForLowFPS();
    void OptimizeForHighMemory();
    void OptimizeForLongRender();
    
    float GetSmoothFPS() const;
    float GetStabilityFactor() const;
    
    // 回调函数
    PerformanceWarningCallback m_pfnWarningCallback;
    OptimizationCallback m_pfnOptimizationCallback;
    
    // GPU时间查询
    UINT64 m_ui64GPUFrequency;
    UINT64 m_ui64StartTimestamp;
    UINT64 m_ui64EndTimestamp;
    BOOL m_bGPUTimingSupported;
};

/**
 * 性能分析器 - 用于详细性能分析
 */
class CKillCardPerformanceProfiler
{
private:
    struct SProfilerEntry
    {
        std::string strName;
        DWORD dwStartTime;
        DWORD dwEndTime;
        DWORD dwDuration;
        DWORD dwCallCount;
        DWORD dwTotalTime;
        float fAvgTime;
        float fMinTime;
        float fMaxTime;
    };
    
    std::vector<SProfilerEntry> m_vecEntries;
    std::map<std::string, int> m_mapNameToIndex;
    BOOL m_bEnabled;
    DWORD m_dwStartTime;

public:
    CKillCardPerformanceProfiler();
    ~CKillCardPerformanceProfiler();
    
    void SetEnabled(BOOL bEnabled) { m_bEnabled = bEnabled; }
    BOOL IsEnabled() const { return m_bEnabled; }
    
    void BeginProfile(const char* szName);
    void EndProfile(const char* szName);
    void Reset();
    
    void GetResults(std::vector<SProfilerEntry>& outResults) const { outResults = m_vecEntries; }
    void GenerateReport(std::string& outReport) const;
};

// 便捷的性能分析宏
#define KILLCARD_PROFILE_BEGIN(name) CKillCardPerformanceProfiler::GetInstance().BeginProfile(name)
#define KILLCARD_PROFILE_END(name) CKillCardPerformanceProfiler::GetInstance().EndProfile(name)

// RAII性能分析类
class CKillCardProfileScope
{
private:
    std::string m_strName;
    
public:
    CKillCardProfileScope(const char* szName) : m_strName(szName)
    {
        KILLCARD_PROFILE_BEGIN(szName);
    }
    
    ~CKillCardProfileScope()
    {
        KILLCARD_PROFILE_END(m_strName.c_str());
    }
};

#define KILLCARD_PROFILE_SCOPE(name) CKillCardProfileScope _profile_scope(name)

// 全局性能监控接口
extern CKillCardPerformanceMonitor& g_KillCardPerfMonitor;

// 性能监控工具函数
namespace KillCardPerformanceUtils
{
    // 系统信息获取
    void GetSystemInfo(std::string& outInfo);
    void GetGPUInfo(std::string& outInfo);
    void GetMemoryInfo(DWORD& outTotalMB, DWORD& outAvailableMB);
    
    // 性能测试
    float RunFPSBenchmark(int nDurationSeconds);
    BOOL RunMemoryStressTest(int nDurationSeconds);
    
    // 优化建议
    void GenerateAutoOptimizeProfile(const SKillCardPerformanceStats& sStats,
                                   SKillCardPluginConfig& outConfig);
    
    // 性能等级评估
    enum EPERFORMANCE_TIER
    {
        PERFORMANCE_TIER_LOW,        // 低端设备
        PERFORMANCE_TIER_MEDIUM,     // 中端设备
        PERFORMANCE_TIER_HIGH,       // 高端设备
        PERFORMANCE_TIER_ULTRA       // 顶级设备
    };
    
    EPERFORMANCE_TIER EvaluatePerformanceTier();
    void GetRecommendedSettings(EPERFORMANCE_TIER eTier, SKillCardPluginConfig& outConfig);
}