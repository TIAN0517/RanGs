#pragma once

#include "StyleManager.h"

//-----------------------------------------------------------------------------------------------//
// Holographic Style Kill Panel - Jy技術團隊
// 全息投影風格：全息藍 + 透明白光，全息掃描線，光線衍射
//-----------------------------------------------------------------------------------------------//

class CHolographicStyle
{
public:
    CHolographicStyle();
    ~CHolographicStyle();

public:
    // 初始化和清理
    bool Initialize(LPDIRECT3DDEVICE9 pd3dDevice);
    void Shutdown();
    
    // 渲染接口
    void RenderEffect(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea, float fElapsedTime);
    void UpdateEffect(float fElapsedTime);
    
    // 效果控制
    void TriggerProjectionFlicker();
    void TriggerHologramDistortion();
    void SetHologramAlpha(float fAlpha) { m_fHologramAlpha = fAlpha; }
    
    // 配置接口
    void SetEffectIntensity(float fIntensity) { m_fEffectIntensity = fIntensity; }
    float GetEffectIntensity() const { return m_fEffectIntensity; }

private:
    // 效果狀態
    bool m_bInitialized;
    float m_fEffectIntensity;
    float m_fGlobalTime;
    
    // 全息投影效果
    float m_fHologramAlpha;
    float m_fAlphaWavePhase;
    bool m_bProjectionFlicker;
    
    // 掃描線
    float m_fScanLinePosition;
    float m_fScanLineSpeed;
    
    // 光線衍射
    float m_fDiffractionPhase;
    float m_fDiffractionIntensity;
    
    // 顏色配置
    D3DXCOLOR m_colorHologramBlue;     // #40E0D0
    D3DXCOLOR m_colorTransparentWhite; // #FFFFFF80
};