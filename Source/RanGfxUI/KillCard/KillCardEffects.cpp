#include "../../stdafx.h"
#include "KillCardEffects.h"

#include "../../../SigmaCore/DebugInclude.h"

CKillCardEffects::CKillCardEffects(EngineDeviceMan* pEngineDevice)
    : CUIGroup(pEngineDevice)
    , m_eActiveEffect(KILL_CARD_TYPE_SIZE)
    , m_nMaxParticles(200)
    , m_nActiveParticles(0)
    , m_bEffectActive(FALSE)
    , m_fEffectTime(0.0f)
    , m_fGlobalIntensity(1.0f)
    , m_nEffectQuality(2)
    , m_pParticleTexture(NULL)
    , m_pNoiseTexture(NULL)
    , m_pGradientTexture(NULL)
    , m_pParticleVB(NULL)
    , m_pParticleIB(NULL)
    , m_fQuantumPhase(0.0f)
    , m_fGlitchIntensity(0.0f)
    , m_fDistortionRadius(0.0f)
    , m_fPlasmaExpansion(0.0f)
    , m_fDissolutionProgress(0.0f)
    , m_fReconstructionPhase(0.0f)
{
    // 初始化效果参数
    for (int i = 0; i < KILL_CARD_TYPE_SIZE; i++)
    {
        m_EffectParams[i].fIntensity = 1.0f;
        m_EffectParams[i].fDuration = 3.0f;
        m_EffectParams[i].nParticleCount = 50;
        m_EffectParams[i].fParticleSize = 1.0f;
        
        // 设置卡片特定的颜色
        SetupEffectParams((EKILL_CARD_TYPE)i);
    }
    
    // 初始化粒子系统
    InitParticleSystem();
}

CKillCardEffects::~CKillCardEffects()
{
    // 释放DirectX资源
    SAFE_RELEASE(m_pParticleTexture);
    SAFE_RELEASE(m_pNoiseTexture);
    SAFE_RELEASE(m_pGradientTexture);
    SAFE_RELEASE(m_pParticleVB);
    SAFE_RELEASE(m_pParticleIB);
}

void CKillCardEffects::CreateSubControl()
{
    // 在实际项目中，这里会加载纹理资源
    /*
    // 示例纹理加载代码:
    D3DXCreateTextureFromFile(m_pEngineDevice->GetDevice(), 
                              "textures/particle.dds", 
                              &m_pParticleTexture);
                              
    D3DXCreateTextureFromFile(m_pEngineDevice->GetDevice(), 
                              "textures/noise.dds", 
                              &m_pNoiseTexture);
                              
    D3DXCreateTextureFromFile(m_pEngineDevice->GetDevice(), 
                              "textures/gradient.dds", 
                              &m_pGradientTexture);
    */
}

void CKillCardEffects::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
    CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);
    
    if (m_bEffectActive)
    {
        UpdateEffects(fElapsedTime);
    }
}

void CKillCardEffects::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{
    if (!pd3dDevice || !m_bEffectActive)
        return;
        
    // 渲染粒子系统
    RenderParticles(pd3dDevice);
    
    // 根据当前效果类型渲染特殊效果
    switch (m_eActiveEffect)
    {
    case KILL_CARD_QUANTUM_FISSION:
        RenderEnergyWaves(pd3dDevice);
        break;
        
    case KILL_CARD_HOLOGRAM_DISRUPTION:
        RenderGlitchEffect(pd3dDevice);
        break;
        
    case KILL_CARD_SPACETIME_DISTORTION:
        RenderDistortionRipples(pd3dDevice);
        break;
        
    case KILL_CARD_PLASMA_EXPLOSION:
        RenderElectricArcs(pd3dDevice);
        break;
        
    case KILL_CARD_NANO_DISSOLUTION:
        RenderMetallicDissolution(pd3dDevice);
        break;
        
    case KILL_CARD_VIRTUAL_RECONSTRUCTION:
        RenderWireframeReconstruction(pd3dDevice);
        break;
    }
}

void CKillCardEffects::TriggerEffect(EKILL_CARD_TYPE cardType, float fIntensity)
{
    if (cardType >= KILL_CARD_TYPE_SIZE)
        return;
        
    // 停止当前效果
    StopAllEffects();
    
    // 启动新效果
    m_eActiveEffect = cardType;
    m_bEffectActive = TRUE;
    m_fEffectTime = 0.0f;
    m_fGlobalIntensity = fIntensity;
    
    // 重置效果特定的参数
    m_fQuantumPhase = 0.0f;
    m_fGlitchIntensity = 0.0f;
    m_fDistortionRadius = 0.0f;
    m_fPlasmaExpansion = 0.0f;
    m_fDissolutionProgress = 0.0f;
    m_fReconstructionPhase = 0.0f;
    
    // 发射粒子
    int particleCount = (int)(m_EffectParams[cardType].nParticleCount * fIntensity);
    EmitParticles(cardType, particleCount);
}

void CKillCardEffects::StopAllEffects()
{
    m_bEffectActive = FALSE;
    m_eActiveEffect = KILL_CARD_TYPE_SIZE;
    m_fEffectTime = 0.0f;
    m_nActiveParticles = 0;
}

void CKillCardEffects::UpdateEffects(float fElapsedTime)
{
    m_fEffectTime += fElapsedTime;
    
    // 检查效果是否应该结束
    if (m_fEffectTime >= m_EffectParams[m_eActiveEffect].fDuration)
    {
        StopAllEffects();
        return;
    }
    
    // 更新粒子系统
    UpdateParticleSystem(fElapsedTime);
    
    // 更新特定效果
    switch (m_eActiveEffect)
    {
    case KILL_CARD_QUANTUM_FISSION:
        UpdateQuantumFissionEffect(fElapsedTime);
        break;
        
    case KILL_CARD_HOLOGRAM_DISRUPTION:
        UpdateHologramDisruptionEffect(fElapsedTime);
        break;
        
    case KILL_CARD_SPACETIME_DISTORTION:
        UpdateSpaceTimeDistortionEffect(fElapsedTime);
        break;
        
    case KILL_CARD_PLASMA_EXPLOSION:
        UpdatePlasmaExplosionEffect(fElapsedTime);
        break;
        
    case KILL_CARD_NANO_DISSOLUTION:
        UpdateNanoDissolutionEffect(fElapsedTime);
        break;
        
    case KILL_CARD_VIRTUAL_RECONSTRUCTION:
        UpdateVirtualReconstructionEffect(fElapsedTime);
        break;
    }
}

void CKillCardEffects::UpdateQuantumFissionEffect(float fElapsedTime)
{
    // 量子裂变效果更新
    m_fQuantumPhase += fElapsedTime * 3.0f;
    
    // 周期性发射新粒子
    if (fmod(m_fQuantumPhase, 0.5f) < fElapsedTime)
    {
        EmitParticles(KILL_CARD_QUANTUM_FISSION, 5);
    }
}

void CKillCardEffects::UpdateHologramDisruptionEffect(float fElapsedTime)
{
    // 全息干扰效果更新
    m_fGlitchIntensity = sin(m_fEffectTime * 8.0f) * 0.5f + 0.5f;
}

void CKillCardEffects::UpdateSpaceTimeDistortionEffect(float fElapsedTime)
{
    // 时空扭曲效果更新
    m_fDistortionRadius += fElapsedTime * 100.0f;
    
    if (m_fDistortionRadius > 300.0f)
        m_fDistortionRadius = 0.0f;
}

void CKillCardEffects::UpdatePlasmaExplosionEffect(float fElapsedTime)
{
    // 等离子爆炸效果更新
    m_fPlasmaExpansion += fElapsedTime * 150.0f;
}

void CKillCardEffects::UpdateNanoDissolutionEffect(float fElapsedTime)
{
    // 纳米溶解效果更新
    m_fDissolutionProgress += fElapsedTime / m_EffectParams[m_eActiveEffect].fDuration;
    
    if (m_fDissolutionProgress > 1.0f)
        m_fDissolutionProgress = 1.0f;
}

void CKillCardEffects::UpdateVirtualReconstructionEffect(float fElapsedTime)
{
    // 虚拟重构效果更新
    m_fReconstructionPhase += fElapsedTime * 2.0f;
}

void CKillCardEffects::InitParticleSystem()
{
    // 初始化粒子容器
    m_Particles.resize(m_nMaxParticles);
    m_nActiveParticles = 0;
}

void CKillCardEffects::UpdateParticleSystem(float fElapsedTime)
{
    // 更新所有活动粒子
    for (int i = 0; i < m_nActiveParticles; )
    {
        PARTICLE_DATA& particle = m_Particles[i];
        
        // 更新粒子生命
        particle.fLife -= fElapsedTime;
        
        if (particle.fLife <= 0.0f)
        {
            // 移除死亡粒子
            m_Particles[i] = m_Particles[m_nActiveParticles - 1];
            m_nActiveParticles--;
            continue;
        }
        
        // 更新粒子位置
        particle.vPosition += particle.vVelocity * fElapsedTime;
        
        // 应用重力等物理效果
        particle.vVelocity.y -= 98.0f * fElapsedTime; // 重力
        
        // 更新粒子颜色（淡出效果）
        float lifeRatio = particle.fLife / 3.0f; // 假设粒子最大生命为3秒
        particle.color.a = lifeRatio;
        
        i++;
    }
}

void CKillCardEffects::RenderParticles(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // 在实际项目中，这里会使用点精灵或四边形进行粒子渲染
    // 这需要设置适当的顶点缓冲区和着色器
}

void CKillCardEffects::EmitParticles(EKILL_CARD_TYPE cardType, int nCount)
{
    if (m_nActiveParticles >= m_nMaxParticles)
        return;
        
    D3DXCOLOR primaryColor = GetCardPrimaryColor(cardType);
    D3DXCOLOR secondaryColor = GetCardSecondaryColor(cardType);
    
    for (int i = 0; i < nCount && m_nActiveParticles < m_nMaxParticles; i++)
    {
        PARTICLE_DATA& particle = m_Particles[m_nActiveParticles];
        
        // 设置初始位置（中心附近）
        particle.vPosition = D3DXVECTOR3(
            (rand() / (float)RAND_MAX - 0.5f) * 20.0f,
            (rand() / (float)RAND_MAX - 0.5f) * 20.0f,
            0.0f
        );
        
        // 设置初始速度
        particle.vVelocity = D3DXVECTOR3(
            (rand() / (float)RAND_MAX - 0.5f) * 100.0f,
            (rand() / (float)RAND_MAX - 0.5f) * 100.0f,
            (rand() / (float)RAND_MAX - 0.5f) * 50.0f
        );
        
        // 设置生命和颜色
        particle.fLife = 2.0f + (rand() / (float)RAND_MAX) * 2.0f;
        particle.color = D3DXCOLOR(
            primaryColor.r + (secondaryColor.r - primaryColor.r) * (rand() / (float)RAND_MAX),
            primaryColor.g + (secondaryColor.g - primaryColor.g) * (rand() / (float)RAND_MAX),
            primaryColor.b + (secondaryColor.b - primaryColor.b) * (rand() / (float)RAND_MAX),
            1.0f
        );
        particle.fSize = m_EffectParams[cardType].fParticleSize;
        
        m_nActiveParticles++;
    }
}

void CKillCardEffects::RenderEnergyWaves(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // 量子能量波渲染实现
}

void CKillCardEffects::RenderGlitchEffect(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // 全息故障效果渲染实现
}

void CKillCardEffects::RenderDistortionRipples(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // 时空扭曲波纹渲染实现
}

void CKillCardEffects::RenderElectricArcs(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // 电弧效果渲染实现
}

void CKillCardEffects::RenderMetallicDissolution(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // 金属溶解效果渲染实现
}

void CKillCardEffects::RenderWireframeReconstruction(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // 线框重构效果渲染实现
}

void CKillCardEffects::SetupEffectParams(EKILL_CARD_TYPE cardType)
{
    if (cardType >= KILL_CARD_TYPE_SIZE)
        return;
        
    EFFECT_PARAMS& params = m_EffectParams[cardType];
    
    // 设置卡片特定的参数
    switch (cardType)
    {
    case KILL_CARD_QUANTUM_FISSION:
        params.primaryColor = D3DXCOLOR(0.3f, 0.7f, 1.0f, 1.0f);   // 蓝色
        params.secondaryColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // 白色
        params.nParticleCount = 80;
        params.fParticleSize = 2.0f;
        break;
        
    case KILL_CARD_HOLOGRAM_DISRUPTION:
        params.primaryColor = D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f);   // 红色
        params.secondaryColor = D3DXCOLOR(0.2f, 1.0f, 0.3f, 1.0f); // 绿色
        params.nParticleCount = 60;
        params.fParticleSize = 1.5f;
        break;
        
    case KILL_CARD_SPACETIME_DISTORTION:
        params.primaryColor = D3DXCOLOR(0.8f, 0.4f, 1.0f, 1.0f);   // 紫色
        params.secondaryColor = D3DXCOLOR(1.0f, 0.8f, 0.5f, 1.0f); // 金色
        params.nParticleCount = 40;
        params.fParticleSize = 3.0f;
        break;
        
    case KILL_CARD_PLASMA_EXPLOSION:
        params.primaryColor = D3DXCOLOR(0.8f, 0.3f, 1.0f, 1.0f);   // 紫色
        params.secondaryColor = D3DXCOLOR(1.0f, 0.5f, 0.8f, 1.0f); // 粉色
        params.nParticleCount = 100;
        params.fParticleSize = 2.5f;
        break;
        
    case KILL_CARD_NANO_DISSOLUTION:
        params.primaryColor = D3DXCOLOR(0.8f, 0.85f, 0.9f, 1.0f);  // 银色
        params.secondaryColor = D3DXCOLOR(0.9f, 0.95f, 1.0f, 1.0f);// 亮银色
        params.nParticleCount = 120;
        params.fParticleSize = 1.0f;
        break;
        
    case KILL_CARD_VIRTUAL_RECONSTRUCTION:
        params.primaryColor = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);   // 青色
        params.secondaryColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f); // 黄色
        params.nParticleCount = 70;
        params.fParticleSize = 1.8f;
        break;
    }
}

D3DXCOLOR CKillCardEffects::GetCardPrimaryColor(EKILL_CARD_TYPE cardType)
{
    if (cardType >= KILL_CARD_TYPE_SIZE)
        return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        
    return m_EffectParams[cardType].primaryColor;
}

D3DXCOLOR CKillCardEffects::GetCardSecondaryColor(EKILL_CARD_TYPE cardType)
{
    if (cardType >= KILL_CARD_TYPE_SIZE)
        return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        
    return m_EffectParams[cardType].secondaryColor;
}