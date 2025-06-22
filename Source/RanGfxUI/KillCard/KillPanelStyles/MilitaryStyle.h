#pragma once

#include "StyleManager.h"

//-----------------------------------------------------------------------------------------------//
// Military Tactical Style Kill Panel - Jy技術團隊
// 軍事戰術風格：軍綠色 + 橙色警告，雷達掃描，戰術網格
//-----------------------------------------------------------------------------------------------//

class CMilitaryTacticalStyle
{
public:
    CMilitaryTacticalStyle();
    ~CMilitaryTacticalStyle();

public:
    // 初始化和清理
    bool Initialize(LPDIRECT3DDEVICE9 pd3dDevice);
    void Shutdown();
    
    // 渲染接口
    void RenderEffect(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea, float fElapsedTime);
    void UpdateEffect(float fElapsedTime);
    
    // 效果控制
    void TriggerRadarScan();
    void TriggerTargetLock();
    void ShowTacticalGrid(bool bShow) { m_bShowGrid = bShow; }
    
    // 配置接口
    void SetEffectIntensity(float fIntensity) { m_fEffectIntensity = fIntensity; }
    float GetEffectIntensity() const { return m_fEffectIntensity; }

private:
    // 效果狀態
    bool m_bInitialized;
    float m_fEffectIntensity;
    float m_fGlobalTime;
    
    // 雷達效果
    float m_fRadarScanAngle;
    float m_fRadarScanSpeed;
    bool m_bRadarActive;
    
    // 戰術網格
    bool m_bShowGrid;
    float m_fGridIntensity;
    
    // 目標鎖定
    bool m_bTargetLocked;
    float m_fLockTimer;
    
    // 顏色配置
    D3DXCOLOR m_colorMilitaryGreen; // #4B7C59
    D3DXCOLOR m_colorWarningOrange; // #FF8C00
};