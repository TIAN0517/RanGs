#pragma once

#include "../../../enginelib/GUInterface/UIGroup.h"
#include "KillCardManager.h"

/**
 * 击杀卡片4D渲染器
 * 负责高级视觉效果和3D空间定位
 */
class CKillCardRenderer : public CUIGroup
{
private:
    struct CARD_RENDER_INFO
    {
        D3DXVECTOR3 vPosition;      // 3D位置
        D3DXVECTOR3 vRotation;      // 旋转
        D3DXVECTOR2 vScale;         // 缩放
        float fDepth;               // 深度
        DWORD dwColor;              // 颜色
        float fIntensity;           // 强度
    };

public:
    CKillCardRenderer(EngineDeviceMan* pEngineDevice);
    virtual ~CKillCardRenderer();

public:
    void CreateSubControl();
    
    virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
    virtual void Render(LPDIRECT3DDEVICE9 pd3dDevice);

public:
    // 渲染控制接口
    void SetupCardRendering(EKILL_CARD_TYPE cardType);
    void UpdateCardTransform(float fElapsedTime);
    void RenderCard(LPDIRECT3DDEVICE9 pd3dDevice, EKILL_CARD_TYPE cardType);
    
    // 4D效果控制
    void SetFloatingEffect(BOOL bEnabled) { m_bFloatingEffect = bEnabled; }
    void SetHologramEffect(BOOL bEnabled) { m_bHologramEffect = bEnabled; }
    void SetParticleEffect(BOOL bEnabled) { m_bParticleEffect = bEnabled; }
    
    // 视觉参数
    void SetTransparency(float fAlpha) { m_fGlobalAlpha = fAlpha; }
    void SetGlowIntensity(float fIntensity) { m_fGlowIntensity = fIntensity; }
    void SetCardScale(float fScale) { m_fCardScale = fScale; }

private:
    // 内部渲染方法
    void RenderQuantumFission(LPDIRECT3DDEVICE9 pd3dDevice);
    void RenderHologramDisruption(LPDIRECT3DDEVICE9 pd3dDevice);
    void RenderSpaceTimeDistortion(LPDIRECT3DDEVICE9 pd3dDevice);
    void RenderPlasmaExplosion(LPDIRECT3DDEVICE9 pd3dDevice);
    void RenderNanoDissolution(LPDIRECT3DDEVICE9 pd3dDevice);
    void RenderVirtualReconstruction(LPDIRECT3DDEVICE9 pd3dDevice);
    
    // 效果渲染
    void RenderParticles(LPDIRECT3DDEVICE9 pd3dDevice);
    void RenderGlowEffect(LPDIRECT3DDEVICE9 pd3dDevice);
    void RenderScanLines(LPDIRECT3DDEVICE9 pd3dDevice);
    void RenderDistortionEffect(LPDIRECT3DDEVICE9 pd3dDevice);
    
    // 变换计算
    void CalculateFloatingMotion(float fTime);
    void CalculateHologramFlicker(float fTime);
    void UpdateParticleSystem(float fElapsedTime);

private:
    // 渲染信息
    CARD_RENDER_INFO m_CardInfo[KILL_CARD_TYPE_SIZE];
    EKILL_CARD_TYPE m_eCurrentCard;
    
    // 渲染状态
    BOOL m_bFloatingEffect;
    BOOL m_bHologramEffect;
    BOOL m_bParticleEffect;
    
    // 视觉参数
    float m_fGlobalAlpha;
    float m_fGlowIntensity;
    float m_fCardScale;
    
    // 动画时间
    float m_fRenderTime;
    float m_fFloatingPhase;
    float m_fFlickerPhase;
    
    // DirectX资源
    LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
    LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;
    LPDIRECT3DTEXTURE9 m_pCardTextures[KILL_CARD_TYPE_SIZE];
    
    // Shader 效果
    LPD3DXEFFECT m_pShaderEffect;
    D3DXHANDLE m_hTechnique;
    D3DXHANDLE m_hWorldViewProj;
    D3DXHANDLE m_hTime;
    D3DXHANDLE m_hIntensity;
};