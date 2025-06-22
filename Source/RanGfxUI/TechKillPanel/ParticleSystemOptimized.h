#pragma once

#include "../../enginelib/GUInterface/UIGroup.h"
#include "./TechKillPanelManager.h"

/**
 * Optimized Particle System for Tech Kill Panel
 * GPU-accelerated particle rendering with instancing support
 * Adaptive quality based on performance requirements
 */
class CParticleSystemOptimized
{
public:
    // Particle data structure optimized for GPU rendering
    struct PARTICLE_VERTEX
    {
        D3DXVECTOR3 position;
        D3DXVECTOR3 velocity;
        D3DXCOLOR color;
        float size;
        float life;
        float maxLife;
        float rotation;
        float rotationSpeed;
        
        static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_POINTSIZE;
    };
    
    // Emitter properties
    struct EMITTER_PROPERTIES
    {
        D3DXVECTOR3 position;
        D3DXVECTOR3 direction;
        float emissionRate;
        float particleSpeed;
        float particleLifetime;
        float particleSize;
        D3DXCOLOR startColor;
        D3DXCOLOR endColor;
        BOOL bFollowMouse;
        float parallaxIntensity;
    };

public:
    CParticleSystemOptimized(EngineDeviceMan* pEngineDevice);
    virtual ~CParticleSystemOptimized();

public:
    // Initialization and cleanup
    void Initialize();
    void Cleanup();
    
    // Main update and render functions
    void Update(float fElapsedTime, CTechKillPanelManager::QUALITY_TIER qualityTier);
    void Render(LPDIRECT3DDEVICE9 pd3dDevice, const D3DXVECTOR2& panelPosition);
    
    // Effect control
    void StartEffect(const D3DXVECTOR2& position, int maxParticles);
    void StopEffect();
    void Reset();
    
    // Configuration
    void SetEmitterProperties(const EMITTER_PROPERTIES& properties);
    void SetQualityTier(CTechKillPanelManager::QUALITY_TIER tier);
    
    // Performance monitoring
    int GetActiveParticleCount() const { return m_nActiveParticles; }
    float GetRenderTime() const { return m_fLastRenderTime; }

private:
    // Internal particle management
    void EmitParticles(float fElapsedTime);
    void UpdateParticles(float fElapsedTime);
    void UpdateParallaxEffect(const D3DXVECTOR2& mousePos, const D3DXVECTOR2& cameraPos);
    void SortParticlesByDistance(const D3DXVECTOR3& cameraPos);
    
    // GPU optimization functions
    void UpdateVertexBuffer();
    void SetupRenderState(LPDIRECT3DDEVICE9 pd3dDevice);
    void RestoreRenderState(LPDIRECT3DDEVICE9 pd3dDevice);
    
    // Quality adaptation
    void AdaptQualitySettings(CTechKillPanelManager::QUALITY_TIER tier);
    void OptimizeForPerformance();

private:
    EngineDeviceMan* m_pEngineDevice;
    
    // Particle data
    std::vector<PARTICLE_VERTEX> m_vecParticles;
    int m_nMaxParticles;
    int m_nActiveParticles;
    
    // GPU resources for instancing
    LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
    LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;
    LPDIRECT3DTEXTURE9 m_pParticleTexture;
    LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclaration;
    
    // Emitter properties
    EMITTER_PROPERTIES m_emitterProps;
    float m_fEmissionTimer;
    BOOL m_bEffectActive;
    
    // Quality settings
    CTechKillPanelManager::QUALITY_TIER m_eQualityTier;
    int m_nQualityMaxParticles;
    float m_fQualityEmissionRate;
    BOOL m_bParallaxEnabled;
    
    // Performance tracking
    DWORD m_dwLastUpdateTime;
    float m_fLastRenderTime;
    int m_nFrameCount;
    
    // Render state backup
    DWORD m_dwOldCullMode;
    DWORD m_dwOldZWriteEnable;
    DWORD m_dwOldZFunc;
    DWORD m_dwOldAlphaBlendEnable;
    DWORD m_dwOldSrcBlend;
    DWORD m_dwOldDestBlend;
    DWORD m_dwOldPointSpriteEnable;
    DWORD m_dwOldPointScaleEnable;
    
    // Mouse/camera tracking for parallax
    D3DXVECTOR2 m_vLastMousePos;
    D3DXVECTOR2 m_vLastCameraPos;
    
    // Optimization flags
    BOOL m_bUseGPUInstancing;
    BOOL m_bUsePointSprites;
    BOOL m_bSortParticles;
};