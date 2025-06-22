#pragma once

#include "StyleManager.h"

//-----------------------------------------------------------------------------------------------//
// Steampunk Style Kill Panel - Jy技術團隊
// 蒸汽朋克風格：復古銅色 + 蒸汽白，齒輪轉動，蒸汽煙霧效果
//-----------------------------------------------------------------------------------------------//

class CSteampunkStyle
{
public:
    CSteampunkStyle();
    ~CSteampunkStyle();

public:
    // 初始化和清理
    bool Initialize(LPDIRECT3DDEVICE9 pd3dDevice);
    void Shutdown();
    
    // 渲染接口
    void RenderEffect(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea, float fElapsedTime);
    void UpdateEffect(float fElapsedTime);
    
    // 效果控制
    void TriggerSteamBurst();
    void TriggerGearRotation();
    void SetMechanicalSounds(bool bEnabled) { m_bMechanicalSounds = bEnabled; }
    
    // 配置接口
    void SetEffectIntensity(float fIntensity) { m_fEffectIntensity = fIntensity; }
    float GetEffectIntensity() const { return m_fEffectIntensity; }

private:
    // 效果狀態
    bool m_bInitialized;
    float m_fEffectIntensity;
    float m_fGlobalTime;
    
    // 齒輪動畫
    float m_fGearRotation;
    float m_fGearSpeed;
    bool m_bGearsActive;
    
    // 蒸汽效果
    float m_fSteamFlow;
    float m_fSteamIntensity;
    bool m_bSteamBurstActive;
    
    // 機械音效
    bool m_bMechanicalSounds;
    
    // 維多利亞裝飾
    float m_fOrnatePatternPhase;
    
    // 顏色配置
    D3DXCOLOR m_colorVintageBronze; // #B87333
    D3DXCOLOR m_colorSteamWhite;    // #F5F5DC
};