#pragma once

#include "StyleManager.h"

//-----------------------------------------------------------------------------------------------//
// Quantum Tech Style Kill Panel - Jy技術團隊
// 量子科技風格：量子藍 + 能量白光，量子粒子波動，光子流效果
//-----------------------------------------------------------------------------------------------//

class CQuantumTechStyle
{
public:
    CQuantumTechStyle();
    ~CQuantumTechStyle();

public:
    // 初始化和清理
    bool Initialize(LPDIRECT3DDEVICE9 pd3dDevice);
    void Shutdown();
    
    // 渲染接口
    void RenderEffect(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea, float fElapsedTime);
    void UpdateEffect(float fElapsedTime);
    
    // 效果控制
    void TriggerQuantumBurst(float fIntensity = 1.0f);
    void StartPhotonFlow();
    void TriggerEnergyPulse();
    
    // 配置接口
    void SetEffectIntensity(float fIntensity) { m_fEffectIntensity = fIntensity; }
    float GetEffectIntensity() const { return m_fEffectIntensity; }

private:
    // 量子粒子
    struct QUANTUM_PARTICLE
    {
        D3DXVECTOR3 vPosition;
        D3DXVECTOR3 vVelocity;
        float fOrbitRadius;
        float fOrbitSpeed;
        float fOrbitPhase;
        float fLife;
        float fSize;
        D3DXCOLOR color;
        bool bActive;
    };
    
    // 光子流粒子
    struct PHOTON_PARTICLE
    {
        D3DXVECTOR2 vPosition;
        D3DXVECTOR2 vDirection;
        float fSpeed;
        float fLife;
        float fTrailLength;
        D3DXCOLOR color;
        bool bActive;
    };
    
    // 能量脈衝
    struct ENERGY_PULSE
    {
        D3DXVECTOR2 vCenter;
        float fRadius;
        float fExpansionSpeed;
        float fLife;
        float fIntensity;
        bool bActive;
    };

private:
    // 效果實現
    void RenderQuantumField(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea);
    void RenderPhotonStreams(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea);
    void RenderEnergyPulses(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea);
    void RenderHexagonalGrid(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea);
    void RenderQuantumEntanglement(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea);
    
    // 更新方法
    void UpdateQuantumParticles(float fElapsedTime);
    void UpdatePhotonFlow(float fElapsedTime);
    void UpdateEnergyPulses(float fElapsedTime);
    
    // 粒子管理
    void InitQuantumParticles();
    void SpawnQuantumParticle(const D3DXVECTOR3& vCenter);
    void SpawnPhotonParticle(const D3DXVECTOR2& vStart, const D3DXVECTOR2& vDirection);
    void CreateEnergyPulse(const D3DXVECTOR2& vCenter, float fIntensity);

private:
    // 設備和資源
    LPDIRECT3DDEVICE9 m_pd3dDevice;
    LPDIRECT3DTEXTURE9 m_pQuantumTexture;
    LPDIRECT3DTEXTURE9 m_pPhotonTexture;
    LPDIRECT3DTEXTURE9 m_pHexagonTexture;
    
    // 效果狀態
    bool m_bInitialized;
    float m_fEffectIntensity;
    float m_fGlobalTime;
    
    // 量子場
    std::vector<QUANTUM_PARTICLE> m_vecQuantumParticles;
    int m_nMaxQuantumParticles;
    float m_fQuantumSpawnTimer;
    
    // 光子流
    std::vector<PHOTON_PARTICLE> m_vecPhotonParticles;
    int m_nMaxPhotonParticles;
    float m_fPhotonSpawnTimer;
    
    // 能量脈衝
    std::vector<ENERGY_PULSE> m_vecEnergyPulses;
    float m_fPulseTimer;
    
    // 六邊形網格動畫
    float m_fHexGridPhase;
    float m_fHexGridScale;
    
    // 顏色配置
    D3DXCOLOR m_colorQuantumBlue;  // #0080FF
    D3DXCOLOR m_colorEnergyWhite;  // #FFFFFF
    D3DXCOLOR m_colorGlow;         // 發光效果
};