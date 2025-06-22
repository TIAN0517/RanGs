#pragma once

#include "../stdafx.h"
#include "KillCardPlugin.h"
#include "../../enginelib/DxTools/PostProcessing/DxPostProcess.h"

/**
 * Unity PostProcess Layer 风格的后处理集成
 * 为击杀卡片系统提供非侵入式的视觉效果处理
 */

// PostProcess Layer类型枚举
enum EKILLCARD_POSTPROCESS_LAYER
{
    POSTPROCESS_LAYER_BACKGROUND = 0,    // 背景层：全息背景、粒子效果
    POSTPROCESS_LAYER_EFFECT,            // 效果层：光晕、扭曲、电流
    POSTPROCESS_LAYER_UI,                // UI层：文字、图标、HUD元素
    POSTPROCESS_LAYER_OVERLAY,           // 覆盖层：最终合成、色彩校正
    POSTPROCESS_LAYER_COUNT
};

// PostProcess效果参数
struct SKillCardPostProcessParams
{
    // 全息效果参数
    float fHologramAlpha;               // 全息透明度
    float fHologramPhase;               // 全息相位
    D3DXVECTOR2 vHologramOffset;        // 全息偏移
    
    // 扭曲效果参数
    float fDistortionStrength;          // 扭曲强度
    float fDistortionRadius;            // 扭曲半径
    D3DXVECTOR2 vDistortionCenter;      // 扭曲中心
    
    // 辉光效果参数
    float fGlowIntensity;               // 辉光强度
    float fGlowRadius;                  // 辉光半径
    D3DCOLOR dwGlowColor;               // 辉光颜色
    
    // 粒子效果参数
    float fParticleScale;               // 粒子缩放
    float fParticleVelocity;            // 粒子速度
    int nParticleCount;                 // 粒子数量
    
    // 色彩效果参数
    float fSaturation;                  // 饱和度
    float fContrast;                    // 对比度
    float fBrightness;                  // 亮度
    D3DXVECTOR3 vColorTint;             // 色调偏移
    
    // 扫描线效果参数
    float fScanLineSpeed;               // 扫描线速度
    float fScanLineWidth;               // 扫描线宽度
    float fScanLineIntensity;           // 扫描线强度
    
    // 故障效果参数
    float fGlitchIntensity;             // 故障强度
    float fGlitchFrequency;             // 故障频率
    float fGlitchOffset;                // 故障偏移
};

// PostProcess渲染目标管理
struct SKillCardRenderTargets
{
    LPDIRECT3DTEXTURE9 pSceneTexture;       // 场景纹理
    LPDIRECT3DTEXTURE9 pBackgroundTexture;   // 背景层纹理
    LPDIRECT3DTEXTURE9 pEffectTexture;       // 效果层纹理
    LPDIRECT3DTEXTURE9 pUITexture;           // UI层纹理
    LPDIRECT3DTEXTURE9 pOverlayTexture;      // 覆盖层纹理
    LPDIRECT3DTEXTURE9 pFinalTexture;        // 最终输出纹理
    
    LPDIRECT3DSURFACE9 pSceneSurface;        // 场景表面
    LPDIRECT3DSURFACE9 pBackgroundSurface;   // 背景层表面
    LPDIRECT3DSURFACE9 pEffectSurface;       // 效果层表面
    LPDIRECT3DSURFACE9 pUISurface;           // UI层表面
    LPDIRECT3DSURFACE9 pOverlaySurface;      // 覆盖层表面
    LPDIRECT3DSURFACE9 pFinalSurface;        // 最终输出表面
    
    DWORD dwWidth;                           // 渲染目标宽度
    DWORD dwHeight;                          // 渲染目标高度
};

/**
 * 击杀卡片PostProcess层管理器
 * 管理分层渲染和后处理效果管线
 */
class CKillCardPostProcessLayer
{
private:
    static CKillCardPostProcessLayer* s_pInstance;
    
    LPDIRECT3DDEVICE9 m_pD3DDevice;          // D3D设备
    LPD3DXEFFECT m_pPostProcessEffect;       // PostProcess着色器
    
    SKillCardRenderTargets m_sRenderTargets; // 渲染目标
    SKillCardPostProcessParams m_sParams;     // 效果参数
    
    // 着色器句柄
    D3DXHANDLE m_hTechnique[POSTPROCESS_LAYER_COUNT];
    D3DXHANDLE m_hSceneTexture;
    D3DXHANDLE m_hBackgroundTexture;
    D3DXHANDLE m_hEffectTexture;
    D3DXHANDLE m_hUITexture;
    D3DXHANDLE m_hOverlayTexture;
    
    // 参数句柄
    D3DXHANDLE m_hHologramParams;
    D3DXHANDLE m_hDistortionParams;
    D3DXHANDLE m_hGlowParams;
    D3DXHANDLE m_hParticleParams;
    D3DXHANDLE m_hColorParams;
    D3DXHANDLE m_hScanLineParams;
    D3DXHANDLE m_hGlitchParams;
    D3DXHANDLE m_hTimeParams;
    
    // 状态管理
    BOOL m_bInitialized;                     // 初始化状态
    BOOL m_bEnabled;                         // 启用状态
    float m_fCurrentTime;                    // 当前时间
    EKILLCARD_POSTPROCESS_LAYER m_eCurrentLayer; // 当前渲染层
    
    // 性能监控
    DWORD m_dwRenderTime;                    // 渲染时间
    DWORD m_dwLastRenderTime;                // 上次渲染时间

public:
    CKillCardPostProcessLayer();
    ~CKillCardPostProcessLayer();
    
    static CKillCardPostProcessLayer& GetInstance();
    static void ReleaseInstance();

public:
    // 初始化和清理
    BOOL Initialize(LPDIRECT3DDEVICE9 pDevice, DWORD dwWidth, DWORD dwHeight);
    void Shutdown();
    void OnDeviceLost();
    void OnDeviceReset();
    
    // 渲染目标管理
    BOOL CreateRenderTargets(DWORD dwWidth, DWORD dwHeight);
    void ReleaseRenderTargets();
    BOOL SetRenderTarget(EKILLCARD_POSTPROCESS_LAYER eLayer);
    void RestoreRenderTarget();
    
    // PostProcess渲染
    void BeginPostProcess();
    void RenderLayer(EKILLCARD_POSTPROCESS_LAYER eLayer);
    void EndPostProcess();
    void PresentFinalResult();
    
    // 效果参数设置
    void SetHologramParams(float fAlpha, float fPhase, const D3DXVECTOR2& vOffset);
    void SetDistortionParams(float fStrength, float fRadius, const D3DXVECTOR2& vCenter);
    void SetGlowParams(float fIntensity, float fRadius, D3DCOLOR dwColor);
    void SetParticleParams(float fScale, float fVelocity, int nCount);
    void SetColorParams(float fSaturation, float fContrast, float fBrightness, const D3DXVECTOR3& vTint);
    void SetScanLineParams(float fSpeed, float fWidth, float fIntensity);
    void SetGlitchParams(float fIntensity, float fFrequency, float fOffset);
    
    // 状态控制
    void SetEnabled(BOOL bEnabled) { m_bEnabled = bEnabled; }
    BOOL IsEnabled() const { return m_bEnabled; }
    void Update(float fElapsedTime);
    
    // 性能监控
    DWORD GetRenderTime() const { return m_dwRenderTime; }
    void ResetPerformanceCounters();

private:
    // 内部方法
    BOOL LoadPostProcessShader();
    void InitializeShaderHandles();
    void UpdateShaderParams();
    void RenderFullScreenQuad();
    BOOL CreateTexture(LPDIRECT3DTEXTURE9* ppTexture, LPDIRECT3DSURFACE9* ppSurface, 
                       DWORD dwWidth, DWORD dwHeight, D3DFORMAT format);
};

/**
 * Unity风格PostProcess Volume组件
 * 提供空间化的后处理效果控制
 */
class CKillCardPostProcessVolume
{
private:
    D3DXVECTOR3 m_vPosition;                 // Volume位置
    float m_fRadius;                         // 影响半径
    float m_fFalloff;                        // 衰减系数
    SKillCardPostProcessParams m_sParams;    // 效果参数
    BOOL m_bEnabled;                         // 启用状态
    float m_fPriority;                       // 优先级

public:
    CKillCardPostProcessVolume();
    ~CKillCardPostProcessVolume();
    
    // 设置Volume属性
    void SetPosition(const D3DXVECTOR3& vPos) { m_vPosition = vPos; }
    void SetRadius(float fRadius) { m_fRadius = fRadius; }
    void SetFalloff(float fFalloff) { m_fFalloff = fFalloff; }
    void SetPriority(float fPriority) { m_fPriority = fPriority; }
    void SetEnabled(BOOL bEnabled) { m_bEnabled = bEnabled; }
    
    // 获取Volume属性
    const D3DXVECTOR3& GetPosition() const { return m_vPosition; }
    float GetRadius() const { return m_fRadius; }
    float GetPriority() const { return m_fPriority; }
    BOOL IsEnabled() const { return m_bEnabled; }
    
    // 效果计算
    float CalculateInfluence(const D3DXVECTOR3& vCameraPos) const;
    void ApplyEffectParams(SKillCardPostProcessParams& sParams, float fWeight) const;
    
    // 参数设置
    void SetEffectParams(const SKillCardPostProcessParams& sParams) { m_sParams = sParams; }
    const SKillCardPostProcessParams& GetEffectParams() const { return m_sParams; }
};

/**
 * PostProcess Volume管理器
 * 管理多个Volume的混合和优先级
 */
class CKillCardPostProcessVolumeManager
{
private:
    static CKillCardPostProcessVolumeManager* s_pInstance;
    
    std::vector<CKillCardPostProcessVolume*> m_vecVolumes; // Volume列表
    D3DXVECTOR3 m_vCameraPosition;                        // 摄像机位置
    SKillCardPostProcessParams m_sFinalParams;             // 最终混合参数

public:
    CKillCardPostProcessVolumeManager();
    ~CKillCardPostProcessVolumeManager();
    
    static CKillCardPostProcessVolumeManager& GetInstance();
    static void ReleaseInstance();

public:
    // Volume管理
    void AddVolume(CKillCardPostProcessVolume* pVolume);
    void RemoveVolume(CKillCardPostProcessVolume* pVolume);
    void ClearAllVolumes();
    
    // 更新和计算
    void SetCameraPosition(const D3DXVECTOR3& vPos) { m_vCameraPosition = vPos; }
    void UpdateVolumeBlending();
    const SKillCardPostProcessParams& GetFinalParams() const { return m_sFinalParams; }
    
    // 预设Volume创建
    CKillCardPostProcessVolume* CreateKillEffectVolume(const D3DXVECTOR3& vPos, EKILL_TYPE killType);
    void RemoveExpiredVolumes(float fMaxAge);

private:
    void BlendVolumeParams();
    void SortVolumesByPriority();
};

// 便捷函数和宏定义
#define KILLCARD_POSTPROCESS CKillCardPostProcessLayer::GetInstance()
#define KILLCARD_VOLUME_MANAGER CKillCardPostProcessVolumeManager::GetInstance()

// PostProcess效果预设
namespace KillCardPostProcessPresets
{
    // 量子裂变预设
    SKillCardPostProcessParams CreateQuantumPreset();
    
    // 全息干扰预设
    SKillCardPostProcessParams CreateHologramPreset();
    
    // 时空扭曲预设
    SKillCardPostProcessParams CreateSpaceTimePreset();
    
    // 等离子爆破预设
    SKillCardPostProcessParams CreatePlasmaPreset();
    
    // 纳米溶解预设
    SKillCardPostProcessParams CreateNanoPreset();
    
    // 虚拟重构预设
    SKillCardPostProcessParams CreateVirtualPreset();
}

// Unity风格的PostProcess Profile
class CKillCardPostProcessProfile
{
private:
    std::string m_strName;                   // Profile名称
    SKillCardPostProcessParams m_sParams;    // 参数设置
    float m_fBlendWeight;                    // 混合权重
    BOOL m_bEnabled;                         // 启用状态

public:
    CKillCardPostProcessProfile(const char* szName);
    ~CKillCardPostProcessProfile();
    
    // Profile管理
    void SetName(const char* szName) { m_strName = szName; }
    const char* GetName() const { return m_strName.c_str(); }
    
    void SetParams(const SKillCardPostProcessParams& sParams) { m_sParams = sParams; }
    const SKillCardPostProcessParams& GetParams() const { return m_sParams; }
    
    void SetBlendWeight(float fWeight) { m_fBlendWeight = fWeight; }
    float GetBlendWeight() const { return m_fBlendWeight; }
    
    void SetEnabled(BOOL bEnabled) { m_bEnabled = bEnabled; }
    BOOL IsEnabled() const { return m_bEnabled; }
    
    // 文件操作
    BOOL SaveToFile(const char* szFilePath) const;
    BOOL LoadFromFile(const char* szFilePath);
    
    // 插值操作
    static void BlendProfiles(const CKillCardPostProcessProfile& profile1,
                             const CKillCardPostProcessProfile& profile2,
                             float fWeight,
                             SKillCardPostProcessParams& outParams);
};