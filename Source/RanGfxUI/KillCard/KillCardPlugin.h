#pragma once

#include "../stdafx.h"
#include "../../enginelib/DxEffect/PostProcessing/DxPostProcess.h"
#include "../../enginelib/DxEffect/DxEffectMan.h"

// Plugin interface version for compatibility checking
#define KILLCARD_PLUGIN_VERSION 0x00010000
#define KILLCARD_PLUGIN_API_VERSION 1

// Forward declarations
class CKillCardManager;
class IKillCardPluginInterface;

/**
 * 击杀卡片插件核心接口
 * 实现外部插件架构，支持热插拔和独立进程通信
 */

// Plugin state enumeration
enum EKILLCARD_PLUGIN_STATE
{
    PLUGIN_STATE_UNLOADED = 0,
    PLUGIN_STATE_LOADING,
    PLUGIN_STATE_LOADED,
    PLUGIN_STATE_ACTIVE,
    PLUGIN_STATE_ERROR,
    PLUGIN_STATE_UNLOADING
};

// Card detection state from inventory system
enum EKILLCARD_EQUIPPED_STATE
{
    CARD_EQUIPPED_NONE = 0,
    CARD_EQUIPPED_QUANTUM = 1,      // 量子裂变卡
    CARD_EQUIPPED_HOLOGRAM = 2,     // 全息干扰卡  
    CARD_EQUIPPED_SPACETIME = 4,    // 时空扭曲卡
    CARD_EQUIPPED_PLASMA = 8,       // 等离子爆破卡
    CARD_EQUIPPED_NANO = 16,        // 纳米溶解卡
    CARD_EQUIPPED_VIRTUAL = 32      // 虚拟重构卡
};

// Performance monitoring data
struct SKillCardPerformanceData
{
    float fCurrentFPS;              // 当前帧率
    float fAverageFPS;              // 平均帧率
    DWORD dwMemoryUsage;            // 内存使用量 (MB)
    DWORD dwVMemoryUsage;           // 显存使用量 (MB)
    float fRenderTime;              // 渲染时间 (ms)
    DWORD dwActiveEffects;          // 活跃效果数量
    BOOL bAutoOptimizeEnabled;      // 自动优化是否启用
};

// Plugin configuration
struct SKillCardPluginConfig
{
    BOOL bEnabled;                  // 插件是否启用
    float fEffectIntensity;         // 效果强度 (0.1 - 3.0)
    BOOL bPerformanceMonitoring;    // 性能监控开关
    float fMinFPS;                  // 最低FPS阈值
    DWORD dwMaxMemoryMB;            // 最大内存限制
    BOOL bAutoQualityAdjust;        // 自动质量调节
    DWORD dwResolutionX;            // 目标分辨率X
    DWORD dwResolutionY;            // 目标分辨率Y
};

/**
 * 外部插件通信接口
 * 通过DLL导出函数实现跨进程通信
 */
class IKillCardPluginInterface
{
public:
    virtual ~IKillCardPluginInterface() {}
    
    // Plugin lifecycle
    virtual BOOL Initialize(HWND hGameWnd, LPDIRECT3DDEVICE9 pDevice) = 0;
    virtual void Shutdown() = 0;
    virtual EKILLCARD_PLUGIN_STATE GetState() const = 0;
    
    // Card system interface
    virtual BOOL IsCardEquipped(EKILLCARD_EQUIPPED_STATE cardType) const = 0;
    virtual void OnCardEquipped(EKILLCARD_EQUIPPED_STATE cardType) = 0;
    virtual void OnCardUnequipped(EKILLCARD_EQUIPPED_STATE cardType) = 0;
    virtual void TriggerKillEffect(EKILL_TYPE killType, const D3DXVECTOR3& position) = 0;
    
    // Performance monitoring
    virtual SKillCardPerformanceData GetPerformanceData() const = 0;
    virtual void SetConfiguration(const SKillCardPluginConfig& config) = 0;
    virtual SKillCardPluginConfig GetConfiguration() const = 0;
    
    // PostProcess integration
    virtual BOOL RegisterPostProcessHook() = 0;
    virtual void UnregisterPostProcessHook() = 0;
    virtual void OnPostProcessRender(LPDIRECT3DDEVICE9 pDevice) = 0;
};

/**
 * 击杀卡片插件管理器
 * 负责插件加载、卸载和生命周期管理
 */
class CKillCardPluginManager
{
private:
    static CKillCardPluginManager* s_pInstance;
    
    HMODULE m_hPluginDLL;                       // 插件DLL句柄
    IKillCardPluginInterface* m_pPluginInterface; // 插件接口指针
    EKILLCARD_PLUGIN_STATE m_eCurrentState;     // 当前插件状态
    SKillCardPluginConfig m_sConfig;            // 插件配置
    SKillCardPerformanceData m_sPerformanceData; // 性能数据
    
    // Performance monitoring
    DWORD m_dwLastFPSCheck;                     // 上次FPS检查时间
    float m_fFPSHistory[60];                    // FPS历史记录
    int m_nFPSHistoryIndex;                     // FPS历史索引
    
    // PostProcess hook management
    BOOL m_bPostProcessHooked;                  // PostProcess挂钩状态
    LPDIRECT3DDEVICE9 m_pD3DDevice;            // D3D设备指针
    
    // Memory management
    DWORD m_dwMemoryCheckInterval;              // 内存检查间隔
    DWORD m_dwLastMemoryCheck;                  // 上次内存检查时间

public:
    CKillCardPluginManager();
    ~CKillCardPluginManager();
    
    static CKillCardPluginManager& GetInstance();
    static void ReleaseInstance();
    
public:
    // Plugin management
    BOOL LoadPlugin(const char* szPluginPath);
    void UnloadPlugin();
    BOOL IsPluginLoaded() const { return m_pPluginInterface != NULL; }
    EKILLCARD_PLUGIN_STATE GetPluginState() const { return m_eCurrentState; }
    
    // Device management
    BOOL InitializeDevice(LPDIRECT3DDEVICE9 pDevice);
    void OnDeviceLost();
    void OnDeviceReset();
    
    // Card detection integration
    void OnInventoryChanged(DWORD dwSlotType, DWORD dwItemID);
    EKILLCARD_EQUIPPED_STATE GetEquippedCards() const;
    void TriggerKillAnimation(EKILL_TYPE killType, const D3DXVECTOR3& worldPos);
    
    // Configuration management
    void SetConfiguration(const SKillCardPluginConfig& config);
    SKillCardPluginConfig GetConfiguration() const { return m_sConfig; }
    void SaveConfiguration();
    void LoadConfiguration();
    
    // Performance monitoring
    void UpdatePerformanceData(float fElapsedTime);
    SKillCardPerformanceData GetPerformanceData() const { return m_sPerformanceData; }
    BOOL CheckPerformanceThresholds();
    void AutoOptimizeSettings();
    
    // PostProcess integration
    BOOL RegisterPostProcessLayer();
    void UnregisterPostProcessLayer();
    void OnPostProcessCallback(LPDIRECT3DDEVICE9 pDevice);
    
    // External interface (for DLL communication)
    static void CALLBACK OnExternalKillTrigger(EKILL_TYPE killType, float x, float y, float z);
    static BOOL CALLBACK OnExternalCardCheck(EKILLCARD_EQUIPPED_STATE cardType);
    static void CALLBACK OnExternalConfigChange(const SKillCardPluginConfig* pConfig);

private:
    // Internal methods
    void UpdateFPSHistory(float fCurrentFPS);
    float CalculateAverageFPS() const;
    void CheckMemoryUsage();
    void ApplyQualitySettings(float fTargetFPS);
    
    // Plugin communication
    typedef IKillCardPluginInterface* (*CreatePluginFunc)();
    typedef void (*DestroyPluginFunc)(IKillCardPluginInterface*);
    typedef DWORD (*GetPluginVersionFunc)();
    
    CreatePluginFunc m_pfnCreatePlugin;
    DestroyPluginFunc m_pfnDestroyPlugin;
    GetPluginVersionFunc m_pfnGetPluginVersion;
};

// DLL export functions for external communication
extern "C" {
    __declspec(dllexport) BOOL WINAPI KillCard_Initialize(HWND hGameWnd, LPDIRECT3DDEVICE9 pDevice);
    __declspec(dllexport) void WINAPI KillCard_Shutdown();
    __declspec(dllexport) void WINAPI KillCard_TriggerKill(int killType, float x, float y, float z);
    __declspec(dllexport) BOOL WINAPI KillCard_IsCardEquipped(int cardType);
    __declspec(dllexport) void WINAPI KillCard_SetConfig(const SKillCardPluginConfig* pConfig);
    __declspec(dllexport) void WINAPI KillCard_GetPerformanceData(SKillCardPerformanceData* pData);
}

// Utility macros for external interface
#define KILLCARD_PLUGIN_CALL(func, ...) \
    do { \
        CKillCardPluginManager& manager = CKillCardPluginManager::GetInstance(); \
        if (manager.IsPluginLoaded()) { \
            func(__VA_ARGS__); \
        } \
    } while(0)

#define KILLCARD_PLUGIN_CALL_RETURN(func, defaultReturn, ...) \
    (CKillCardPluginManager::GetInstance().IsPluginLoaded() ? \
     func(__VA_ARGS__) : defaultReturn)