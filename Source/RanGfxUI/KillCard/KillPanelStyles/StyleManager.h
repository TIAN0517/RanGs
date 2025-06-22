#pragma once

#include "../../../../enginelib/GUInterface/UIGroup.h"
#include "../KillCardManager.h"
#include <d3dx9.h>

//-----------------------------------------------------------------------------------------------//
// Kill Panel Style Management System - Jy技術團隊
// 開發者：TIAN0517 - Jy技術團隊  
// 實現六種科技感擊殺面板風格的統一管理
//-----------------------------------------------------------------------------------------------//

// 風格枚舉定義
enum EKILL_PANEL_STYLE
{
    STYLE_CYBERPUNK = 0,        // 賽博朋克風格
    STYLE_QUANTUM_TECH,         // 量子科技風格
    STYLE_MILITARY_TACTICAL,    // 軍事戰術風格
    STYLE_HOLOGRAPHIC,          // 全息投影風格
    STYLE_SPACE_SCIFI,          // 太空科幻風格
    STYLE_STEAMPUNK,            // 蒸汽朋克風格
    
    KILL_PANEL_STYLE_COUNT
};

// 風格配置結構
struct STYLE_CONFIG
{
    EKILL_PANEL_STYLE eStyle;
    D3DXCOLOR primaryColor;
    D3DXCOLOR secondaryColor;
    const char* szStyleName;
    const char* szShaderName;
    float fEffectIntensity;
    int nParticleCount;
    bool bGlitchEffect;
    bool bDistortionEffect;
};

class CStyleManager
{
public:
    CStyleManager();
    ~CStyleManager();

public:
    // 風格管理接口
    bool Initialize();
    void Shutdown();
    
    // 風格切換
    bool SetCurrentStyle(EKILL_PANEL_STYLE eStyle);
    EKILL_PANEL_STYLE GetCurrentStyle() const { return m_eCurrentStyle; }
    
    // 風格配置獲取
    const STYLE_CONFIG* GetStyleConfig(EKILL_PANEL_STYLE eStyle) const;
    const STYLE_CONFIG* GetCurrentStyleConfig() const;
    
    // 風格效果參數
    D3DXCOLOR GetStylePrimaryColor(EKILL_PANEL_STYLE eStyle) const;
    D3DXCOLOR GetStyleSecondaryColor(EKILL_PANEL_STYLE eStyle) const;
    const char* GetStyleName(EKILL_PANEL_STYLE eStyle) const;
    const char* GetStyleShaderName(EKILL_PANEL_STYLE eStyle) const;
    
    // 效果強度控制
    void SetStyleIntensity(EKILL_PANEL_STYLE eStyle, float fIntensity);
    float GetStyleIntensity(EKILL_PANEL_STYLE eStyle) const;
    
    // 品質設定
    void SetStyleQuality(int nQuality) { m_nStyleQuality = nQuality; }
    int GetStyleQuality() const { return m_nStyleQuality; }
    
    // 配置保存/載入
    bool SaveStyleConfig(const char* szConfigFile);
    bool LoadStyleConfig(const char* szConfigFile);
    
    // 工具方法
    bool IsValidStyle(EKILL_PANEL_STYLE eStyle) const;
    int GetStyleCount() const { return KILL_PANEL_STYLE_COUNT; }

private:
    // 初始化風格配置
    void InitializeStyleConfigs();
    void SetupCyberpunkStyle();
    void SetupQuantumTechStyle();
    void SetupMilitaryTacticalStyle();
    void SetupHolographicStyle();
    void SetupSpaceSciFiStyle();
    void SetupSteampunkStyle();

private:
    // 當前狀態
    EKILL_PANEL_STYLE m_eCurrentStyle;
    int m_nStyleQuality;
    bool m_bInitialized;
    
    // 風格配置陣列
    STYLE_CONFIG m_StyleConfigs[KILL_PANEL_STYLE_COUNT];
    
    // 單例模式
    static CStyleManager* s_pInstance;
    
public:
    static CStyleManager* GetInstance();
    static void ReleaseInstance();
};