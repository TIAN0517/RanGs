#pragma once

#include "StyleManager.h"

//-----------------------------------------------------------------------------------------------//
// Space Sci-Fi Style Kill Panel - Jy技術團隊
// 太空科幻風格：深空藍 + 星光銀，星空背景，星雲流動效果
//-----------------------------------------------------------------------------------------------//

class CSpaceSciFiStyle
{
public:
    CSpaceSciFiStyle();
    ~CSpaceSciFiStyle();

public:
    // 初始化和清理
    bool Initialize(LPDIRECT3DDEVICE9 pd3dDevice);
    void Shutdown();
    
    // 渲染接口
    void RenderEffect(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea, float fElapsedTime);
    void UpdateEffect(float fElapsedTime);
    
    // 效果控制
    void TriggerGravityWave();
    void OpenTransportPortal();
    void ShowEnergyShield(bool bShow) { m_bShowShield = bShow; }
    
    // 配置接口
    void SetEffectIntensity(float fIntensity) { m_fEffectIntensity = fIntensity; }
    float GetEffectIntensity() const { return m_fEffectIntensity; }

private:
    // 效果狀態
    bool m_bInitialized;
    float m_fEffectIntensity;
    float m_fGlobalTime;
    
    // 星雲流動
    float m_fNebulaFlowPhase;
    float m_fNebulaFlowSpeed;
    
    // 引力波
    bool m_bGravityWaveActive;
    float m_fGravityWavePhase;
    
    // 星際傳送門
    bool m_bPortalOpen;
    float m_fPortalRadius;
    
    // 能量護盾
    bool m_bShowShield;
    float m_fShieldPhase;
    
    // 顏色配置
    D3DXCOLOR m_colorDeepSpaceBlue; // #191970
    D3DXCOLOR m_colorStarSilver;    // #C0C0C0
};