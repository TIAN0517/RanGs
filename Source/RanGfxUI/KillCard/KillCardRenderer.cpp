#include "../../stdafx.h"
#include "KillCardRenderer.h"

#include "../../../SigmaCore/DebugInclude.h"

CKillCardRenderer::CKillCardRenderer(EngineDeviceMan* pEngineDevice)
    : CUIGroup(pEngineDevice)
    , m_eCurrentCard(KILL_CARD_TYPE_SIZE)
    , m_bFloatingEffect(TRUE)
    , m_bHologramEffect(TRUE)
    , m_bParticleEffect(TRUE)
    , m_fGlobalAlpha(1.0f)
    , m_fGlowIntensity(1.0f)
    , m_fCardScale(1.0f)
    , m_fRenderTime(0.0f)
    , m_fFloatingPhase(0.0f)
    , m_fFlickerPhase(0.0f)
    , m_pVertexBuffer(NULL)
    , m_pIndexBuffer(NULL)
    , m_pShaderEffect(NULL)
    , m_hTechnique(NULL)
    , m_hWorldViewProj(NULL)
    , m_hTime(NULL)
    , m_hIntensity(NULL)
{
    // 初始化渲染信息
    for (int i = 0; i < KILL_CARD_TYPE_SIZE; i++)
    {
        m_CardInfo[i].vPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_CardInfo[i].vRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_CardInfo[i].vScale = D3DXVECTOR2(1.0f, 1.0f);
        m_CardInfo[i].fDepth = 0.0f;
        m_CardInfo[i].dwColor = 0xFFFFFFFF;
        m_CardInfo[i].fIntensity = 1.0f;
        
        m_pCardTextures[i] = NULL;
    }
}

CKillCardRenderer::~CKillCardRenderer()
{
    // 释放DirectX资源
    SAFE_RELEASE(m_pVertexBuffer);
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pShaderEffect);
    
    for (int i = 0; i < KILL_CARD_TYPE_SIZE; i++)
    {
        SAFE_RELEASE(m_pCardTextures[i]);
    }
}

void CKillCardRenderer::CreateSubControl()
{
    // 在实际项目中，这里会加载纹理和着色器资源
    // 由于无法访问实际的DirectX设备，这里只做框架实现
    
    /*
    // 示例纹理加载代码:
    const char* textureNames[KILL_CARD_TYPE_SIZE] = {
        "killcard_quantum.dds",
        "killcard_hologram.dds", 
        "killcard_spacetime.dds",
        "killcard_plasma.dds",
        "killcard_nano.dds",
        "killcard_virtual.dds"
    };
    
    for (int i = 0; i < KILL_CARD_TYPE_SIZE; i++)
    {
        D3DXCreateTextureFromFile(m_pEngineDevice->GetDevice(), 
                                  textureNames[i], 
                                  &m_pCardTextures[i]);
    }
    
    // 加载着色器效果
    D3DXCreateEffectFromFile(m_pEngineDevice->GetDevice(),
                            "KillCardShaders/KillCardEffect.fx",
                            NULL, NULL,
                            D3DXSHADER_DEBUG,
                            NULL,
                            &m_pShaderEffect,
                            NULL);
                            
    if (m_pShaderEffect)
    {
        m_hTechnique = m_pShaderEffect->GetTechniqueByName("KillCardTech");
        m_hWorldViewProj = m_pShaderEffect->GetParameterByName(NULL, "WorldViewProj");
        m_hTime = m_pShaderEffect->GetParameterByName(NULL, "Time");
        m_hIntensity = m_pShaderEffect->GetParameterByName(NULL, "Intensity");
    }
    */
}

void CKillCardRenderer::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
    CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);
    
    m_fRenderTime += fElapsedTime;
    
    if (m_eCurrentCard < KILL_CARD_TYPE_SIZE)
    {
        UpdateCardTransform(fElapsedTime);
    }
}

void CKillCardRenderer::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{
    if (!pd3dDevice || m_eCurrentCard >= KILL_CARD_TYPE_SIZE)
        return;
        
    // 在实际实现中，这里会进行DirectX渲染
    RenderCard(pd3dDevice, m_eCurrentCard);
}

void CKillCardRenderer::SetupCardRendering(EKILL_CARD_TYPE cardType)
{
    if (cardType >= KILL_CARD_TYPE_SIZE)
        return;
        
    m_eCurrentCard = cardType;
    
    // 根据卡片类型设置渲染参数
    switch (cardType)
    {
    case KILL_CARD_QUANTUM_FISSION:
        m_CardInfo[cardType].dwColor = 0xFF4080FF; // 蓝白色
        m_CardInfo[cardType].fIntensity = 1.2f;
        break;
        
    case KILL_CARD_HOLOGRAM_DISRUPTION:
        m_CardInfo[cardType].dwColor = 0xFF20FF40; // 绿色
        m_CardInfo[cardType].fIntensity = 1.0f;
        break;
        
    case KILL_CARD_SPACETIME_DISTORTION:
        m_CardInfo[cardType].dwColor = 0xFF8040FF; // 紫色
        m_CardInfo[cardType].fIntensity = 1.5f;
        break;
        
    case KILL_CARD_PLASMA_EXPLOSION:
        m_CardInfo[cardType].dwColor = 0xFFFF4080; // 紫红色
        m_CardInfo[cardType].fIntensity = 1.3f;
        break;
        
    case KILL_CARD_NANO_DISSOLUTION:
        m_CardInfo[cardType].dwColor = 0xFFC0C0C0; // 银色
        m_CardInfo[cardType].fIntensity = 0.9f;
        break;
        
    case KILL_CARD_VIRTUAL_RECONSTRUCTION:
        m_CardInfo[cardType].dwColor = 0xFF00FFFF; // 青色
        m_CardInfo[cardType].fIntensity = 1.1f;
        break;
    }
    
    // 重置动画时间
    m_fRenderTime = 0.0f;
    m_fFloatingPhase = 0.0f;
    m_fFlickerPhase = 0.0f;
}

void CKillCardRenderer::UpdateCardTransform(float fElapsedTime)
{
    if (m_eCurrentCard >= KILL_CARD_TYPE_SIZE)
        return;
        
    CARD_RENDER_INFO& info = m_CardInfo[m_eCurrentCard];
    
    // 浮动效果
    if (m_bFloatingEffect)
    {
        CalculateFloatingMotion(m_fRenderTime);
    }
    
    // 全息闪烁效果
    if (m_bHologramEffect)
    {
        CalculateHologramFlicker(m_fRenderTime);
    }
    
    // 更新粒子系统
    if (m_bParticleEffect)
    {
        UpdateParticleSystem(fElapsedTime);
    }
}

void CKillCardRenderer::RenderCard(LPDIRECT3DDEVICE9 pd3dDevice, EKILL_CARD_TYPE cardType)
{
    if (cardType >= KILL_CARD_TYPE_SIZE)
        return;
        
    // 根据卡片类型调用相应的渲染方法
    switch (cardType)
    {
    case KILL_CARD_QUANTUM_FISSION:
        RenderQuantumFission(pd3dDevice);
        break;
        
    case KILL_CARD_HOLOGRAM_DISRUPTION:
        RenderHologramDisruption(pd3dDevice);
        break;
        
    case KILL_CARD_SPACETIME_DISTORTION:
        RenderSpaceTimeDistortion(pd3dDevice);
        break;
        
    case KILL_CARD_PLASMA_EXPLOSION:
        RenderPlasmaExplosion(pd3dDevice);
        break;
        
    case KILL_CARD_NANO_DISSOLUTION:
        RenderNanoDissolution(pd3dDevice);
        break;
        
    case KILL_CARD_VIRTUAL_RECONSTRUCTION:
        RenderVirtualReconstruction(pd3dDevice);
        break;
    }
}

void CKillCardRenderer::RenderQuantumFission(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // 量子裂变效果渲染
    RenderParticles(pd3dDevice);
    RenderGlowEffect(pd3dDevice);
}

void CKillCardRenderer::RenderHologramDisruption(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // 全息干扰效果渲染
    RenderScanLines(pd3dDevice);
    RenderGlowEffect(pd3dDevice);
}

void CKillCardRenderer::RenderSpaceTimeDistortion(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // 时空扭曲效果渲染
    RenderDistortionEffect(pd3dDevice);
}

void CKillCardRenderer::RenderPlasmaExplosion(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // 等离子爆炸效果渲染
    RenderParticles(pd3dDevice);
    RenderGlowEffect(pd3dDevice);
}

void CKillCardRenderer::RenderNanoDissolution(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // 纳米溶解效果渲染
    RenderParticles(pd3dDevice);
}

void CKillCardRenderer::RenderVirtualReconstruction(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // 虚拟重构效果渲染
    RenderScanLines(pd3dDevice);
}

void CKillCardRenderer::RenderParticles(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // 粒子渲染实现
    // 在实际项目中会使用顶点缓冲区和着色器进行渲染
}

void CKillCardRenderer::RenderGlowEffect(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // 辉光效果渲染实现
}

void CKillCardRenderer::RenderScanLines(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // 扫描线效果渲染实现
}

void CKillCardRenderer::RenderDistortionEffect(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // 扭曲效果渲染实现
}

void CKillCardRenderer::CalculateFloatingMotion(float fTime)
{
    if (m_eCurrentCard >= KILL_CARD_TYPE_SIZE)
        return;
        
    CARD_RENDER_INFO& info = m_CardInfo[m_eCurrentCard];
    
    // 浮动运动计算
    m_fFloatingPhase = fTime * 2.0f;
    
    info.vPosition.x = sin(m_fFloatingPhase) * 5.0f;
    info.vPosition.y = cos(m_fFloatingPhase * 0.7f) * 3.0f;
    info.vPosition.z = sin(m_fFloatingPhase * 0.5f) * 2.0f;
    
    // 旋转
    info.vRotation.z = sin(m_fFloatingPhase * 0.3f) * 0.1f;
}

void CKillCardRenderer::CalculateHologramFlicker(float fTime)
{
    if (m_eCurrentCard >= KILL_CARD_TYPE_SIZE)
        return;
        
    CARD_RENDER_INFO& info = m_CardInfo[m_eCurrentCard];
    
    // 全息闪烁效果
    m_fFlickerPhase = fTime * 8.0f;
    
    float flicker = sin(m_fFlickerPhase) * 0.2f + 0.8f;
    info.fIntensity = flicker * m_fGlowIntensity;
}

void CKillCardRenderer::UpdateParticleSystem(float fElapsedTime)
{
    // 粒子系统更新
    // 在实际项目中会更新粒子位置、生命周期等
}