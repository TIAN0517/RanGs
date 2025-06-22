#pragma once

#include "StyleManager.h"

//-----------------------------------------------------------------------------------------------//
// Cyberpunk Style Kill Panel - Jy技術團隊
// 賽博朋克風格：霓虹粉紫 + 電藍色，故障閃爍效果，數位雨滴
//-----------------------------------------------------------------------------------------------//

class CCyberpunkStyle
{
public:
    CCyberpunkStyle();
    ~CCyberpunkStyle();

public:
    // 初始化和清理
    bool Initialize(LPDIRECT3DDEVICE9 pd3dDevice);
    void Shutdown();
    
    // 渲染接口
    void RenderEffect(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea, float fElapsedTime);
    void UpdateEffect(float fElapsedTime);
    
    // 效果控制
    void TriggerGlitchEffect(float fIntensity = 1.0f);
    void StartDigitalRain();
    void StopAllEffects();
    
    // 配置接口
    void SetEffectIntensity(float fIntensity) { m_fEffectIntensity = fIntensity; }
    float GetEffectIntensity() const { return m_fEffectIntensity; }

private:
    // 故障閃爍效果
    struct GLITCH_DATA
    {
        float fStartTime;
        float fDuration;
        float fIntensity;
        D3DXVECTOR2 vOffset;
        bool bActive;
    };
    
    // 數位雨滴粒子
    struct DIGITAL_RAIN_PARTICLE
    {
        D3DXVECTOR2 vPosition;
        float fSpeed;
        float fLife;
        char cCharacter;
        D3DXCOLOR color;
        bool bActive;
    };

private:
    // 效果實現
    void RenderGlitchEffect(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea);
    void RenderDigitalRain(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea);
    void RenderNeonBorder(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea);
    void RenderCircuitPattern(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea);
    
    // 更新方法
    void UpdateGlitchEffect(float fElapsedTime);
    void UpdateDigitalRain(float fElapsedTime);
    void UpdateNeonPulse(float fElapsedTime);
    
    // 粒子管理
    void InitDigitalRainParticles();
    void SpawnDigitalRainParticle(const D3DXVECTOR2& vPosition);
    char GetRandomCharacter();

private:
    // 設備和資源
    LPDIRECT3DDEVICE9 m_pd3dDevice;
    LPDIRECT3DTEXTURE9 m_pGlitchTexture;
    LPDIRECT3DTEXTURE9 m_pCircuitTexture;
    LPDIRECT3DTEXTURE9 m_pNoiseTexture;
    
    // 效果狀態
    bool m_bInitialized;
    float m_fEffectIntensity;
    float m_fGlobalTime;
    
    // 故障效果
    std::vector<GLITCH_DATA> m_vecGlitchData;
    float m_fGlitchTimer;
    float m_fGlitchInterval;
    
    // 數位雨滴
    std::vector<DIGITAL_RAIN_PARTICLE> m_vecRainParticles;
    int m_nMaxRainParticles;
    float m_fRainSpawnTimer;
    
    // 霓虹脈衝
    float m_fNeonPulsePhase;
    float m_fNeonIntensity;
    
    // 顏色配置
    D3DXCOLOR m_colorNeonPink;     // #FF007F
    D3DXCOLOR m_colorElectricBlue; // #00FFFF
    D3DXCOLOR m_colorGlitch;       // 故障顏色
};