#include "stdafx.h"
#include "./ParticleSystemOptimized.h"

#include "../../enginelib/DxTools/TextureManager.h"
#include "../../enginelib/DxTools/DxInputDevice.h"

#include <random>
#include <algorithm>

CParticleSystemOptimized::CParticleSystemOptimized(EngineDeviceMan* pEngineDevice)
    : m_pEngineDevice(pEngineDevice)
    , m_nMaxParticles(80)
    , m_nActiveParticles(0)
    , m_pVertexBuffer(NULL)
    , m_pIndexBuffer(NULL)
    , m_pParticleTexture(NULL)
    , m_pVertexDeclaration(NULL)
    , m_fEmissionTimer(0.0f)
    , m_bEffectActive(FALSE)
    , m_eQualityTier(CTechKillPanelManager::QUALITY_STANDARD)
    , m_nQualityMaxParticles(80)
    , m_fQualityEmissionRate(0.5f)
    , m_bParallaxEnabled(TRUE)
    , m_dwLastUpdateTime(0)
    , m_fLastRenderTime(0.0f)
    , m_nFrameCount(0)
    , m_dwOldCullMode(0)
    , m_dwOldZWriteEnable(0)
    , m_dwOldZFunc(0)
    , m_dwOldAlphaBlendEnable(0)
    , m_dwOldSrcBlend(0)
    , m_dwOldDestBlend(0)
    , m_dwOldPointSpriteEnable(0)
    , m_dwOldPointScaleEnable(0)
    , m_vLastMousePos(0, 0)
    , m_vLastCameraPos(0, 0)
    , m_bUseGPUInstancing(TRUE)
    , m_bUsePointSprites(TRUE)
    , m_bSortParticles(FALSE)
{
    // Initialize emitter properties with default values
    m_emitterProps.position = D3DXVECTOR3(0, 0, 0);
    m_emitterProps.direction = D3DXVECTOR3(0, -1, 0);
    m_emitterProps.emissionRate = 30.0f; // particles per second
    m_emitterProps.particleSpeed = 50.0f;
    m_emitterProps.particleLifetime = 3.0f;
    m_emitterProps.particleSize = 4.0f;
    m_emitterProps.startColor = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f); // Cyan
    m_emitterProps.endColor = D3DXCOLOR(0.0f, 0.5f, 1.0f, 0.0f);   // Blue fade
    m_emitterProps.bFollowMouse = TRUE;
    m_emitterProps.parallaxIntensity = 0.3f;
}

CParticleSystemOptimized::~CParticleSystemOptimized()
{
    Cleanup();
}

void CParticleSystemOptimized::Initialize()
{
    if (!m_pEngineDevice) return;
    
    LPDIRECT3DDEVICE9 pd3dDevice = m_pEngineDevice->GetDevice();
    if (!pd3dDevice) return;
    
    // Load particle texture
    TextureManager::GetInstance().LoadTexture(
        _T("UI\\TechPanel\\particle.dds"),
        TextureManager::TEXTURE_ADD_DATA::TEXUTRE_TYPE_2D,
        &m_pParticleTexture,
        0,
        0);
    
    // Create vertex buffer for GPU instancing
    HRESULT hr = pd3dDevice->CreateVertexBuffer(
        m_nMaxParticles * sizeof(PARTICLE_VERTEX),
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        PARTICLE_VERTEX::FVF,
        D3DPOOL_DEFAULT,
        &m_pVertexBuffer,
        NULL);
        
    if (FAILED(hr))
    {
        // Fallback to managed pool if dynamic fails
        hr = pd3dDevice->CreateVertexBuffer(
            m_nMaxParticles * sizeof(PARTICLE_VERTEX),
            D3DUSAGE_WRITEONLY,
            PARTICLE_VERTEX::FVF,
            D3DPOOL_MANAGED,
            &m_pVertexBuffer,
            NULL);
        m_bUseGPUInstancing = FALSE;
    }
    
    // Check for point sprite support
    D3DCAPS9 caps;
    pd3dDevice->GetDeviceCaps(&caps);
    if (caps.MaxPointSize < 64.0f)
    {
        m_bUsePointSprites = FALSE;
    }
    
    // Reserve particle vector
    m_vecParticles.reserve(m_nMaxParticles);
    
    // Set initial quality tier
    AdaptQualitySettings(m_eQualityTier);
}

void CParticleSystemOptimized::Cleanup()
{
    SAFE_RELEASE(m_pVertexBuffer);
    SAFE_RELEASE(m_pIndexBuffer);
    SAFE_RELEASE(m_pParticleTexture);
    SAFE_RELEASE(m_pVertexDeclaration);
    
    m_vecParticles.clear();
}

void CParticleSystemOptimized::Update(float fElapsedTime, CTechKillPanelManager::QUALITY_TIER qualityTier)
{
    if (!m_bEffectActive) return;
    
    // Update quality settings if changed
    if (qualityTier != m_eQualityTier)
    {
        AdaptQualitySettings(qualityTier);
    }
    
    DWORD dwCurrentTime = GetTickCount();
    
    // Emit new particles
    EmitParticles(fElapsedTime);
    
    // Update existing particles
    UpdateParticles(fElapsedTime);
    
    // Update parallax effect if enabled
    if (m_bParallaxEnabled && m_emitterProps.bFollowMouse)
    {
        // Get mouse position (this would need to be passed from the UI system)
        POINT ptMouse;
        GetCursorPos(&ptMouse);
        D3DXVECTOR2 mousePos((float)ptMouse.x, (float)ptMouse.y);
        
        UpdateParallaxEffect(mousePos, m_vLastCameraPos);
    }
    
    // Update vertex buffer for GPU rendering
    UpdateVertexBuffer();
    
    m_dwLastUpdateTime = dwCurrentTime;
}

void CParticleSystemOptimized::Render(LPDIRECT3DDEVICE9 pd3dDevice, const D3DXVECTOR2& panelPosition)
{
    if (!pd3dDevice || !m_bEffectActive || m_nActiveParticles <= 0) return;
    
    DWORD dwStartTime = GetTickCount();
    
    // Setup render state
    SetupRenderState(pd3dDevice);
    
    // Set texture
    pd3dDevice->SetTexture(0, m_pParticleTexture);
    
    if (m_bUsePointSprites && m_pVertexBuffer)
    {
        // Use point sprites for optimal performance
        pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(PARTICLE_VERTEX));
        pd3dDevice->SetFVF(PARTICLE_VERTEX::FVF);
        
        // Enable point sprites
        pd3dDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
        pd3dDevice->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
        
        // Set point size
        float pointSize = m_emitterProps.particleSize;
        pd3dDevice->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&pointSize));
        
        // Render all particles in one draw call
        pd3dDevice->DrawPrimitive(D3DPT_POINTLIST, 0, m_nActiveParticles);
    }
    else
    {
        // Fallback to quad rendering
        for (int i = 0; i < m_nActiveParticles; ++i)
        {
            const PARTICLE_VERTEX& particle = m_vecParticles[i];
            
            // Create quad vertices for this particle
            PARTICLE_VERTEX quad[4];
            float halfSize = particle.size * 0.5f;
            
            // Top-left
            quad[0].position = D3DXVECTOR3(particle.position.x - halfSize, particle.position.y - halfSize, particle.position.z);
            quad[0].color = particle.color;
            
            // Top-right
            quad[1].position = D3DXVECTOR3(particle.position.x + halfSize, particle.position.y - halfSize, particle.position.z);
            quad[1].color = particle.color;
            
            // Bottom-left
            quad[2].position = D3DXVECTOR3(particle.position.x - halfSize, particle.position.y + halfSize, particle.position.z);
            quad[2].color = particle.color;
            
            // Bottom-right
            quad[3].position = D3DXVECTOR3(particle.position.x + halfSize, particle.position.y + halfSize, particle.position.z);
            quad[3].color = particle.color;
            
            pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
            pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, quad, sizeof(PARTICLE_VERTEX));
        }
    }
    
    // Restore render state
    RestoreRenderState(pd3dDevice);
    
    // Update performance tracking
    DWORD dwEndTime = GetTickCount();
    m_fLastRenderTime = (dwEndTime - dwStartTime) / 1000.0f;
    m_nFrameCount++;
}

void CParticleSystemOptimized::StartEffect(const D3DXVECTOR2& position, int maxParticles)
{
    m_emitterProps.position = D3DXVECTOR3(position.x, position.y, 0.0f);
    m_nMaxParticles = min(maxParticles, m_nQualityMaxParticles);
    m_bEffectActive = TRUE;
    m_fEmissionTimer = 0.0f;
    
    // Clear existing particles
    m_vecParticles.clear();
    m_nActiveParticles = 0;
}

void CParticleSystemOptimized::StopEffect()
{
    m_bEffectActive = FALSE;
}

void CParticleSystemOptimized::Reset()
{
    m_vecParticles.clear();
    m_nActiveParticles = 0;
    m_bEffectActive = FALSE;
    m_fEmissionTimer = 0.0f;
}

void CParticleSystemOptimized::SetEmitterProperties(const EMITTER_PROPERTIES& properties)
{
    m_emitterProps = properties;
}

void CParticleSystemOptimized::SetQualityTier(CTechKillPanelManager::QUALITY_TIER tier)
{
    AdaptQualitySettings(tier);
}

void CParticleSystemOptimized::EmitParticles(float fElapsedTime)
{
    if (!m_bEffectActive) return;
    
    m_fEmissionTimer += fElapsedTime;
    
    float emissionInterval = 1.0f / (m_emitterProps.emissionRate * m_fQualityEmissionRate);
    
    while (m_fEmissionTimer >= emissionInterval && m_nActiveParticles < m_nMaxParticles)
    {
        // Create new particle
        PARTICLE_VERTEX newParticle;
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> angleDis(0.0f, D3DX_PI * 2.0f);
        std::uniform_real_distribution<> speedDis(0.5f, 1.5f);
        std::uniform_real_distribution<> sizeDis(0.8f, 1.2f);
        std::uniform_real_distribution<> lifeDis(0.8f, 1.2f);
        
        // Position with slight randomization
        float angle = angleDis(gen);
        float radius = 20.0f;
        
        newParticle.position = m_emitterProps.position;
        newParticle.position.x += cosf(angle) * radius;
        newParticle.position.y += sinf(angle) * radius;
        
        // Velocity
        D3DXVECTOR3 velocity = m_emitterProps.direction;
        D3DXVec3Normalize(&velocity, &velocity);
        velocity *= m_emitterProps.particleSpeed * speedDis(gen);
        
        // Add some randomization to velocity
        velocity.x += (angleDis(gen) - D3DX_PI) * 10.0f;
        velocity.y += (angleDis(gen) - D3DX_PI) * 10.0f;
        
        newParticle.velocity = velocity;
        newParticle.color = m_emitterProps.startColor;
        newParticle.size = m_emitterProps.particleSize * sizeDis(gen);
        newParticle.life = 0.0f;
        newParticle.maxLife = m_emitterProps.particleLifetime * lifeDis(gen);
        newParticle.rotation = angleDis(gen);
        newParticle.rotationSpeed = angleDis(gen) * 2.0f - D3DX_PI;
        
        m_vecParticles.push_back(newParticle);
        m_nActiveParticles++;
        
        m_fEmissionTimer -= emissionInterval;
    }
}

void CParticleSystemOptimized::UpdateParticles(float fElapsedTime)
{
    // Update particles and remove dead ones
    auto it = m_vecParticles.begin();
    m_nActiveParticles = 0;
    
    while (it != m_vecParticles.end())
    {
        // Update particle
        it->life += fElapsedTime;
        
        if (it->life >= it->maxLife)
        {
            // Remove dead particle
            it = m_vecParticles.erase(it);
        }
        else
        {
            // Update position
            it->position += it->velocity * fElapsedTime;
            
            // Update rotation
            it->rotation += it->rotationSpeed * fElapsedTime;
            
            // Update color (fade over lifetime)
            float t = it->life / it->maxLife;
            D3DXColorLerp(&it->color, &m_emitterProps.startColor, &m_emitterProps.endColor, t);
            
            // Apply gravity
            it->velocity.y += 50.0f * fElapsedTime; // Simple gravity
            
            // Size variation over lifetime
            it->size = m_emitterProps.particleSize * (1.0f - t * 0.5f);
            
            m_nActiveParticles++;
            ++it;
        }
    }
}

void CParticleSystemOptimized::UpdateParallaxEffect(const D3DXVECTOR2& mousePos, const D3DXVECTOR2& cameraPos)
{
    if (!m_bParallaxEnabled) return;
    
    // Calculate mouse delta
    D3DXVECTOR2 mouseDelta = mousePos - m_vLastMousePos;
    
    // Apply parallax offset to particles based on their depth
    for (auto& particle : m_vecParticles)
    {
        float depth = particle.position.z / 100.0f + 0.5f; // Normalize depth
        float parallaxStrength = m_emitterProps.parallaxIntensity * depth;
        
        particle.position.x += mouseDelta.x * parallaxStrength;
        particle.position.y += mouseDelta.y * parallaxStrength;
    }
    
    m_vLastMousePos = mousePos;
}

void CParticleSystemOptimized::UpdateVertexBuffer()
{
    if (!m_pVertexBuffer || m_nActiveParticles <= 0) return;
    
    PARTICLE_VERTEX* pVertices;
    HRESULT hr = m_pVertexBuffer->Lock(0, m_nActiveParticles * sizeof(PARTICLE_VERTEX), (void**)&pVertices, D3DLOCK_DISCARD);
    
    if (SUCCEEDED(hr))
    {
        memcpy(pVertices, &m_vecParticles[0], m_nActiveParticles * sizeof(PARTICLE_VERTEX));
        m_pVertexBuffer->Unlock();
    }
}

void CParticleSystemOptimized::SetupRenderState(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // Store old render state
    pd3dDevice->GetRenderState(D3DRS_CULLMODE, &m_dwOldCullMode);
    pd3dDevice->GetRenderState(D3DRS_ZWRITEENABLE, &m_dwOldZWriteEnable);
    pd3dDevice->GetRenderState(D3DRS_ZFUNC, &m_dwOldZFunc);
    pd3dDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &m_dwOldAlphaBlendEnable);
    pd3dDevice->GetRenderState(D3DRS_SRCBLEND, &m_dwOldSrcBlend);
    pd3dDevice->GetRenderState(D3DRS_DESTBLEND, &m_dwOldDestBlend);
    pd3dDevice->GetRenderState(D3DRS_POINTSPRITEENABLE, &m_dwOldPointSpriteEnable);
    pd3dDevice->GetRenderState(D3DRS_POINTSCALEENABLE, &m_dwOldPointScaleEnable);
    
    // Setup for particle rendering
    pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
    pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE); // Additive blending for glow effect
}

void CParticleSystemOptimized::RestoreRenderState(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // Restore old render state
    pd3dDevice->SetRenderState(D3DRS_CULLMODE, m_dwOldCullMode);
    pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, m_dwOldZWriteEnable);
    pd3dDevice->SetRenderState(D3DRS_ZFUNC, m_dwOldZFunc);
    pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, m_dwOldAlphaBlendEnable);
    pd3dDevice->SetRenderState(D3DRS_SRCBLEND, m_dwOldSrcBlend);
    pd3dDevice->SetRenderState(D3DRS_DESTBLEND, m_dwOldDestBlend);
    pd3dDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, m_dwOldPointSpriteEnable);
    pd3dDevice->SetRenderState(D3DRS_POINTSCALEENABLE, m_dwOldPointScaleEnable);
}

void CParticleSystemOptimized::AdaptQualitySettings(CTechKillPanelManager::QUALITY_TIER tier)
{
    m_eQualityTier = tier;
    
    switch (tier)
    {
    case CTechKillPanelManager::QUALITY_PERFORMANCE:
        m_nQualityMaxParticles = 50;
        m_fQualityEmissionRate = 1.0f; // 1/sec
        m_bParallaxEnabled = FALSE;
        m_bSortParticles = FALSE;
        break;
        
    case CTechKillPanelManager::QUALITY_STANDARD:
        m_nQualityMaxParticles = 80;
        m_fQualityEmissionRate = 0.5f; // 0.5/sec
        m_bParallaxEnabled = TRUE; // Pseudo-3D
        m_bSortParticles = FALSE;
        break;
        
    case CTechKillPanelManager::QUALITY_ULTIMATE:
        m_nQualityMaxParticles = 120;
        m_fQualityEmissionRate = 0.3f; // 0.3/sec
        m_bParallaxEnabled = TRUE; // Real-time
        m_bSortParticles = TRUE;
        break;
    }
    
    // Limit current max particles to quality setting
    if (m_nMaxParticles > m_nQualityMaxParticles)
    {
        m_nMaxParticles = m_nQualityMaxParticles;
        
        // Remove excess particles if needed
        while ((int)m_vecParticles.size() > m_nMaxParticles)
        {
            m_vecParticles.pop_back();
        }
        m_nActiveParticles = min(m_nActiveParticles, m_nMaxParticles);
    }
}

void CParticleSystemOptimized::OptimizeForPerformance()
{
    // Automatic optimization based on current performance
    if (m_fLastRenderTime > 0.016f) // Target: 60 FPS (16ms per frame)
    {
        // Reduce quality if rendering takes too long
        if (m_eQualityTier == CTechKillPanelManager::QUALITY_ULTIMATE)
        {
            AdaptQualitySettings(CTechKillPanelManager::QUALITY_STANDARD);
        }
        else if (m_eQualityTier == CTechKillPanelManager::QUALITY_STANDARD)
        {
            AdaptQualitySettings(CTechKillPanelManager::QUALITY_PERFORMANCE);
        }
    }
}