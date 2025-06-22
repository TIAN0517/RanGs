#pragma once

#include "../stdafx.h"
#include "KillCardPlugin.h"
#include "../../enginelib/DxMeshs/DxAnimationMan.h"

/**
 * 击杀卡片资源管理系统
 * 动态加载DDS纹理、音效和着色器资源，支持热插拔和内存优化
 */

// 资源类型枚举
enum EKILLCARD_RESOURCE_TYPE
{
    RESOURCE_TYPE_TEXTURE_DDS = 0,   // DDS纹理
    RESOURCE_TYPE_TEXTURE_PNG,       // PNG纹理（透明通道）
    RESOURCE_TYPE_SOUND_WAV,         // WAV音效
    RESOURCE_TYPE_SHADER_HLSL,       // HLSL着色器
    RESOURCE_TYPE_ANIMATION_SEQUENCE, // 动画序列帧
    RESOURCE_TYPE_PARTICLE_CONFIG,   // 粒子配置
    RESOURCE_TYPE_COUNT
};

// 资源加载状态
enum EKILLCARD_RESOURCE_STATE
{
    RESOURCE_STATE_UNLOADED = 0,     // 未加载
    RESOURCE_STATE_LOADING,          // 加载中
    RESOURCE_STATE_LOADED,           // 已加载
    RESOURCE_STATE_ERROR,            // 加载错误
    RESOURCE_STATE_RELEASING         // 释放中
};

// 资源优先级
enum EKILLCARD_RESOURCE_PRIORITY
{
    RESOURCE_PRIORITY_LOW = 0,       // 低优先级
    RESOURCE_PRIORITY_NORMAL,        // 普通优先级
    RESOURCE_PRIORITY_HIGH,          // 高优先级
    RESOURCE_PRIORITY_CRITICAL       // 关键优先级（始终保持加载）
};

// 纹理压缩格式
enum EKILLCARD_TEXTURE_FORMAT
{
    TEXTURE_FORMAT_BC7,              // BC7压缩（高质量）
    TEXTURE_FORMAT_BC3,              // BC3压缩（透明通道）
    TEXTURE_FORMAT_BC1,              // BC1压缩（不透明）
    TEXTURE_FORMAT_UNCOMPRESSED      // 未压缩格式
};

// 资源信息结构
struct SKillCardResourceInfo
{
    std::string strName;             // 资源名称
    std::string strFilePath;         // 文件路径
    EKILLCARD_RESOURCE_TYPE eType;   // 资源类型
    EKILLCARD_RESOURCE_STATE eState; // 加载状态
    EKILLCARD_RESOURCE_PRIORITY ePriority; // 优先级
    
    DWORD dwFileSize;                // 文件大小
    DWORD dwMemorySize;              // 内存占用
    DWORD dwRefCount;                // 引用计数
    DWORD dwLastAccessTime;          // 最后访问时间
    
    // 类型特定数据
    union
    {
        struct // 纹理数据
        {
            DWORD dwWidth;
            DWORD dwHeight;
            DWORD dwMipLevels;
            D3DFORMAT format;
            EKILLCARD_TEXTURE_FORMAT eCompression;
        } texture;
        
        struct // 音效数据
        {
            DWORD dwSampleRate;
            DWORD dwChannels;
            DWORD dwBitsPerSample;
            float fDuration;
        } sound;
        
        struct // 着色器数据
        {
            DWORD dwConstantCount;
            DWORD dwInstructionCount;
            char szProfile[16];
        } shader;
        
        struct // 动画数据
        {
            DWORD dwFrameCount;
            float fFPS;
            float fDuration;
        } animation;
    } data;
};

// 资源句柄基类
class IKillCardResourceHandle
{
public:
    virtual ~IKillCardResourceHandle() {}
    virtual EKILLCARD_RESOURCE_TYPE GetType() const = 0;
    virtual const char* GetName() const = 0;
    virtual BOOL IsValid() const = 0;
    virtual void AddRef() = 0;
    virtual void Release() = 0;
    virtual DWORD GetRefCount() const = 0;
};

// 纹理资源句柄
class CKillCardTextureHandle : public IKillCardResourceHandle
{
private:
    LPDIRECT3DTEXTURE9 m_pTexture;
    std::string m_strName;
    DWORD m_dwRefCount;
    SKillCardResourceInfo m_sInfo;

public:
    CKillCardTextureHandle(const char* szName, LPDIRECT3DTEXTURE9 pTexture, const SKillCardResourceInfo& sInfo);
    virtual ~CKillCardTextureHandle();
    
    virtual EKILLCARD_RESOURCE_TYPE GetType() const override { return RESOURCE_TYPE_TEXTURE_DDS; }
    virtual const char* GetName() const override { return m_strName.c_str(); }
    virtual BOOL IsValid() const override { return m_pTexture != NULL; }
    virtual void AddRef() override { m_dwRefCount++; }
    virtual void Release() override;
    virtual DWORD GetRefCount() const override { return m_dwRefCount; }
    
    LPDIRECT3DTEXTURE9 GetTexture() const { return m_pTexture; }
    const SKillCardResourceInfo& GetInfo() const { return m_sInfo; }
};

// 音效资源句柄
class CKillCardSoundHandle : public IKillCardResourceHandle
{
private:
    void* m_pSoundBuffer;            // 音频缓冲区
    std::string m_strName;
    DWORD m_dwRefCount;
    SKillCardResourceInfo m_sInfo;

public:
    CKillCardSoundHandle(const char* szName, void* pSoundBuffer, const SKillCardResourceInfo& sInfo);
    virtual ~CKillCardSoundHandle();
    
    virtual EKILLCARD_RESOURCE_TYPE GetType() const override { return RESOURCE_TYPE_SOUND_WAV; }
    virtual const char* GetName() const override { return m_strName.c_str(); }
    virtual BOOL IsValid() const override { return m_pSoundBuffer != NULL; }
    virtual void AddRef() override { m_dwRefCount++; }
    virtual void Release() override;
    virtual DWORD GetRefCount() const override { return m_dwRefCount; }
    
    void* GetSoundBuffer() const { return m_pSoundBuffer; }
    void PlaySound(float fVolume = 1.0f, float fPitch = 1.0f);
};

// 着色器资源句柄
class CKillCardShaderHandle : public IKillCardResourceHandle
{
private:
    LPD3DXEFFECT m_pEffect;
    std::string m_strName;
    DWORD m_dwRefCount;
    SKillCardResourceInfo m_sInfo;

public:
    CKillCardShaderHandle(const char* szName, LPD3DXEFFECT pEffect, const SKillCardResourceInfo& sInfo);
    virtual ~CKillCardShaderHandle();
    
    virtual EKILLCARD_RESOURCE_TYPE GetType() const override { return RESOURCE_TYPE_SHADER_HLSL; }
    virtual const char* GetName() const override { return m_strName.c_str(); }
    virtual BOOL IsValid() const override { return m_pEffect != NULL; }
    virtual void AddRef() override { m_dwRefCount++; }
    virtual void Release() override;
    virtual DWORD GetRefCount() const override { return m_dwRefCount; }
    
    LPD3DXEFFECT GetEffect() const { return m_pEffect; }
};

/**
 * 资源加载器基类
 */
class IKillCardResourceLoader
{
public:
    virtual ~IKillCardResourceLoader() {}
    virtual EKILLCARD_RESOURCE_TYPE GetSupportedType() const = 0;
    virtual IKillCardResourceHandle* LoadResource(const char* szFilePath, const char* szName) = 0;
    virtual BOOL CanLoadAsync() const = 0;
    virtual void LoadResourceAsync(const char* szFilePath, const char* szName, void* pUserData) = 0;
};

// DDS纹理加载器
class CKillCardDDSLoader : public IKillCardResourceLoader
{
private:
    LPDIRECT3DDEVICE9 m_pDevice;

public:
    CKillCardDDSLoader(LPDIRECT3DDEVICE9 pDevice);
    virtual ~CKillCardDDSLoader();
    
    virtual EKILLCARD_RESOURCE_TYPE GetSupportedType() const override { return RESOURCE_TYPE_TEXTURE_DDS; }
    virtual IKillCardResourceHandle* LoadResource(const char* szFilePath, const char* szName) override;
    virtual BOOL CanLoadAsync() const override { return TRUE; }
    virtual void LoadResourceAsync(const char* szFilePath, const char* szName, void* pUserData) override;

private:
    BOOL ValidateDDSFile(const char* szFilePath);
    EKILLCARD_TEXTURE_FORMAT DetermineCompressionFormat(const char* szFilePath);
};

// WAV音效加载器
class CKillCardWAVLoader : public IKillCardResourceLoader
{
public:
    CKillCardWAVLoader();
    virtual ~CKillCardWAVLoader();
    
    virtual EKILLCARD_RESOURCE_TYPE GetSupportedType() const override { return RESOURCE_TYPE_SOUND_WAV; }
    virtual IKillCardResourceHandle* LoadResource(const char* szFilePath, const char* szName) override;
    virtual BOOL CanLoadAsync() const override { return TRUE; }
    virtual void LoadResourceAsync(const char* szFilePath, const char* szName, void* pUserData) override;

private:
    BOOL ValidateWAVFile(const char* szFilePath);
    void* LoadWAVData(const char* szFilePath, SKillCardResourceInfo& outInfo);
};

// HLSL着色器加载器
class CKillCardShaderLoader : public IKillCardResourceLoader
{
private:
    LPDIRECT3DDEVICE9 m_pDevice;

public:
    CKillCardShaderLoader(LPDIRECT3DDEVICE9 pDevice);
    virtual ~CKillCardShaderLoader();
    
    virtual EKILLCARD_RESOURCE_TYPE GetSupportedType() const override { return RESOURCE_TYPE_SHADER_HLSL; }
    virtual IKillCardResourceHandle* LoadResource(const char* szFilePath, const char* szName) override;
    virtual BOOL CanLoadAsync() const override { return FALSE; }
    virtual void LoadResourceAsync(const char* szFilePath, const char* szName, void* pUserData) override;

private:
    LPD3DXEFFECT CompileShader(const char* szFilePath, std::string& outErrorMsg);
    BOOL ValidateShaderProfile(const char* szProfile);
};

/**
 * 击杀卡片资源管理器
 * 管理所有资源的加载、缓存和释放
 */
class CKillCardAssetManager
{
private:
    static CKillCardAssetManager* s_pInstance;
    
    // 资源映射
    std::map<std::string, IKillCardResourceHandle*> m_mapResources;
    std::map<EKILLCARD_RESOURCE_TYPE, IKillCardResourceLoader*> m_mapLoaders;
    
    // 资源配置
    std::string m_strAssetRootPath;      // 资源根路径
    DWORD m_dwMaxCacheSize;              // 最大缓存大小 (MB)
    DWORD m_dwCurrentCacheSize;          // 当前缓存大小 (MB)
    BOOL m_bAsyncLoadingEnabled;         // 异步加载启用
    
    // 性能优化
    std::vector<std::string> m_vecPreloadQueue;     // 预加载队列
    std::vector<std::string> m_vecUnloadQueue;      // 卸载队列
    DWORD m_dwLastCleanupTime;                      // 上次清理时间
    DWORD m_dwCleanupInterval;                      // 清理间隔
    
    // 线程管理
    HANDLE m_hAsyncThread;               // 异步加载线程
    HANDLE m_hAsyncEvent;                // 异步事件
    CRITICAL_SECTION m_csResourceLock;   // 资源锁
    BOOL m_bAsyncThreadRunning;          // 异步线程运行状态
    
    // 设备管理
    LPDIRECT3DDEVICE9 m_pDevice;         // D3D设备

public:
    CKillCardAssetManager();
    ~CKillCardAssetManager();
    
    static CKillCardAssetManager& GetInstance();
    static void ReleaseInstance();

public:
    // 初始化和清理
    BOOL Initialize(LPDIRECT3DDEVICE9 pDevice, const char* szAssetRootPath);
    void Shutdown();
    void OnDeviceLost();
    void OnDeviceReset();
    
    // 资源路径管理
    void SetAssetRootPath(const char* szPath) { m_strAssetRootPath = szPath; }
    const char* GetAssetRootPath() const { return m_strAssetRootPath.c_str(); }
    std::string GetFullPath(const char* szRelativePath) const;
    
    // 资源加载
    IKillCardResourceHandle* LoadResource(const char* szName, EKILLCARD_RESOURCE_PRIORITY ePriority = RESOURCE_PRIORITY_NORMAL);
    IKillCardResourceHandle* LoadResourceFromFile(const char* szFilePath, const char* szName, EKILLCARD_RESOURCE_PRIORITY ePriority = RESOURCE_PRIORITY_NORMAL);
    void LoadResourceAsync(const char* szName, EKILLCARD_RESOURCE_PRIORITY ePriority = RESOURCE_PRIORITY_NORMAL);
    
    // 资源访问
    CKillCardTextureHandle* GetTexture(const char* szName);
    CKillCardSoundHandle* GetSound(const char* szName);
    CKillCardShaderHandle* GetShader(const char* szName);
    
    // 资源管理
    void UnloadResource(const char* szName);
    void UnloadAllResources();
    void UnloadUnusedResources();
    BOOL IsResourceLoaded(const char* szName) const;
    
    // 预加载管理
    void AddToPreloadQueue(const char* szName);
    void PreloadQueuedResources();
    void ClearPreloadQueue();
    
    // 缓存管理
    void SetMaxCacheSize(DWORD dwMaxSizeMB) { m_dwMaxCacheSize = dwMaxSizeMB; }
    DWORD GetMaxCacheSize() const { return m_dwMaxCacheSize; }
    DWORD GetCurrentCacheSize() const { return m_dwCurrentCacheSize; }
    void CleanupCache();
    
    // 异步加载控制
    void SetAsyncLoadingEnabled(BOOL bEnabled);
    BOOL IsAsyncLoadingEnabled() const { return m_bAsyncLoadingEnabled; }
    void WaitForAsyncLoading();
    
    // 资源信息
    void GetResourceInfo(const char* szName, SKillCardResourceInfo& outInfo) const;
    void GetAllResourceInfo(std::vector<SKillCardResourceInfo>& outInfoList) const;
    DWORD GetResourceCount() const { return (DWORD)m_mapResources.size(); }
    
    // 热重载支持
    void EnableHotReload(BOOL bEnabled);
    void CheckForFileChanges();
    void ReloadResource(const char* szName);
    
    // 资源验证
    BOOL ValidateAllResources();
    void GetCorruptedResources(std::vector<std::string>& outCorruptedList);

private:
    // 内部方法
    void RegisterDefaultLoaders();
    IKillCardResourceLoader* GetLoader(EKILLCARD_RESOURCE_TYPE eType);
    void UpdateCacheSize();
    void CleanupOldResources();
    std::string GenerateResourceKey(const char* szName, EKILLCARD_RESOURCE_TYPE eType);
    
    // 异步加载线程
    static DWORD WINAPI AsyncLoadingThread(LPVOID pParam);
    void ProcessAsyncQueue();
    
    // 文件监控
    HANDLE m_hFileWatchHandle;
    void StartFileWatching();
    void StopFileWatching();
};

/**
 * 资源预设管理器
 * 管理不同击杀卡片类型的资源预设
 */
class CKillCardAssetPresets
{
private:
    struct SAssetPreset
    {
        std::vector<std::string> vecTextures;
        std::vector<std::string> vecSounds;
        std::vector<std::string> vecShaders;
        EKILLCARD_RESOURCE_PRIORITY ePriority;
    };
    
    std::map<EKILL_CARD_TYPE, SAssetPreset> m_mapPresets;

public:
    CKillCardAssetPresets();
    ~CKillCardAssetPresets();
    
    // 预设管理
    void LoadPresets(const char* szConfigFile);
    void SavePresets(const char* szConfigFile);
    void CreateDefaultPresets();
    
    // 预设应用
    void LoadPresetResources(EKILL_CARD_TYPE eCardType);
    void UnloadPresetResources(EKILL_CARD_TYPE eCardType);
    void PreloadAllPresets();
    
    // 预设配置
    void SetPresetPriority(EKILL_CARD_TYPE eCardType, EKILLCARD_RESOURCE_PRIORITY ePriority);
    void AddTextureToPreset(EKILL_CARD_TYPE eCardType, const char* szTextureName);
    void AddSoundToPreset(EKILL_CARD_TYPE eCardType, const char* szSoundName);
    void AddShaderToPreset(EKILL_CARD_TYPE eCardType, const char* szShaderName);

private:
    void CreateQuantumPreset();
    void CreateHologramPreset();
    void CreateSpaceTimePreset();
    void CreatePlasmaPreset();
    void CreateNanoPreset();
    void CreateVirtualPreset();
};

// 全局资源管理器访问
extern CKillCardAssetManager& g_KillCardAssets;

// 便捷的资源访问宏
#define KILLCARD_TEXTURE(name) g_KillCardAssets.GetTexture(name)
#define KILLCARD_SOUND(name) g_KillCardAssets.GetSound(name)
#define KILLCARD_SHADER(name) g_KillCardAssets.GetShader(name)

// 智能资源句柄 - RAII管理
template<typename T>
class TKillCardResourcePtr
{
private:
    T* m_pHandle;

public:
    TKillCardResourcePtr() : m_pHandle(nullptr) {}
    TKillCardResourcePtr(T* pHandle) : m_pHandle(pHandle) 
    {
        if (m_pHandle) m_pHandle->AddRef();
    }
    
    TKillCardResourcePtr(const TKillCardResourcePtr& other) : m_pHandle(other.m_pHandle)
    {
        if (m_pHandle) m_pHandle->AddRef();
    }
    
    ~TKillCardResourcePtr()
    {
        if (m_pHandle) m_pHandle->Release();
    }
    
    TKillCardResourcePtr& operator=(const TKillCardResourcePtr& other)
    {
        if (this != &other)
        {
            if (m_pHandle) m_pHandle->Release();
            m_pHandle = other.m_pHandle;
            if (m_pHandle) m_pHandle->AddRef();
        }
        return *this;
    }
    
    T* operator->() const { return m_pHandle; }
    T& operator*() const { return *m_pHandle; }
    T* Get() const { return m_pHandle; }
    BOOL IsValid() const { return m_pHandle && m_pHandle->IsValid(); }
    
    void Reset(T* pHandle = nullptr)
    {
        if (m_pHandle) m_pHandle->Release();
        m_pHandle = pHandle;
        if (m_pHandle) m_pHandle->AddRef();
    }
};

// 类型别名
typedef TKillCardResourcePtr<CKillCardTextureHandle> KillCardTexturePtr;
typedef TKillCardResourcePtr<CKillCardSoundHandle> KillCardSoundPtr;
typedef TKillCardResourcePtr<CKillCardShaderHandle> KillCardShaderPtr;