#pragma once

#include "../../../enginelib/GUInterface/UIGroup.h"
#include "KillCardManager.h"

/**
 * 击杀卡片特效实现
 * 实现6种不同风格的科技感视觉效果
 */
class CKillCardEffects : public CUIGroup
{
private:
    struct PARTICLE_DATA
    {
        D3DXVECTOR3 vPosition;
        D3DXVECTOR3 vVelocity;
        D3DXCOLOR color;
        float fLife;
        float fSize;
    };

    struct EFFECT_PARAMS
    {
        float fIntensity;
        float fDuration;
        D3DXCOLOR primaryColor;
        D3DXCOLOR secondaryColor;
        int nParticleCount;
        float fParticleSize;
    };

public:
    CKillCardEffects(EngineDeviceMan* pEngineDevice);
    virtual ~CKillCardEffects();

public:
    void CreateSubControl();
    
    virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
    virtual void Render(LPDIRECT3DDEVICE9 pd3dDevice);

public:
    // 效果控制接口
    void TriggerEffect(EKILL_CARD_TYPE cardType, float fIntensity = 1.0f);
    void StopAllEffects();
    void UpdateEffects(float fElapsedTime);
    
    // 配置接口
    void SetEffectQuality(int nQuality) { m_nEffectQuality = nQuality; }
    void SetParticleCount(int nCount) { m_nMaxParticles = nCount; }
    void SetGlobalIntensity(float fIntensity) { m_fGlobalIntensity = fIntensity; }

private:
    // 效果实现方法
    void UpdateQuantumFissionEffect(float fElapsedTime);
    void UpdateHologramDisruptionEffect(float fElapsedTime);
    void UpdateSpaceTimeDistortionEffect(float fElapsedTime);
    void UpdatePlasmaExplosionEffect(float fElapsedTime);
    void UpdateNanoDissolutionEffect(float fElapsedTime);
    void UpdateVirtualReconstructionEffect(float fElapsedTime);
    
    // 粒子系统
    void InitParticleSystem();
    void UpdateParticleSystem(float fElapsedTime);
    void RenderParticles(LPDIRECT3DDEVICE9 pd3dDevice);
    void EmitParticles(EKILL_CARD_TYPE cardType, int nCount);
    
    // 特殊效果
    void RenderEnergyWaves(LPDIRECT3DDEVICE9 pd3dDevice);
    void RenderGlitchEffect(LPDIRECT3DDEVICE9 pd3dDevice);
    void RenderDistortionRipples(LPDIRECT3DDEVICE9 pd3dDevice);
    void RenderElectricArcs(LPDIRECT3DDEVICE9 pd3dDevice);
    void RenderMetallicDissolution(LPDIRECT3DDEVICE9 pd3dDevice);
    void RenderWireframeReconstruction(LPDIRECT3DDEVICE9 pd3dDevice);
    
    // 辅助方法
    void SetupEffectParams(EKILL_CARD_TYPE cardType);
    D3DXCOLOR GetCardPrimaryColor(EKILL_CARD_TYPE cardType);
    D3DXCOLOR GetCardSecondaryColor(EKILL_CARD_TYPE cardType);

private:
    // 效果参数
    EFFECT_PARAMS m_EffectParams[KILL_CARD_TYPE_SIZE];
    EKILL_CARD_TYPE m_eActiveEffect;
    
    // 粒子系统
    std::vector<PARTICLE_DATA> m_Particles;
    int m_nMaxParticles;
    int m_nActiveParticles;
    
    // 效果状态
    BOOL m_bEffectActive;
    float m_fEffectTime;
    float m_fGlobalIntensity;
    int m_nEffectQuality;
    
    // 纹理资源
    LPDIRECT3DTEXTURE9 m_pParticleTexture;
    LPDIRECT3DTEXTURE9 m_pNoiseTexture;
    LPDIRECT3DTEXTURE9 m_pGradientTexture;
    
    // Vertex/Index buffers
    LPDIRECT3DVERTEXBUFFER9 m_pParticleVB;
    LPDIRECT3DINDEXBUFFER9 m_pParticleIB;
    
    // 效果特定数据
    float m_fQuantumPhase;          // 量子裂变相位
    float m_fGlitchIntensity;       // 全息干扰强度
    float m_fDistortionRadius;      // 时空扭曲半径
    float m_fPlasmaExpansion;       // 等离子扩散
    float m_fDissolutionProgress;   // 纳米溶解进度
    float m_fReconstructionPhase;   // 虚拟重构相位
};