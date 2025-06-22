#pragma once

#include "../../enginelib/GUInterface/UIGroup.h"
#include "../KillCard/KillCardManager.h"

class CTechPanelRenderer;
class CParticleSystemOptimized;
class CEquipmentDetector;
class CPerformanceMonitor;

// Tech card specific ID
const DWORD TECH_DISPLAY_CARD_ID = 9999;

// Panel display settings
const float TECH_PANEL_DISPLAY_TIME = 2.5f;
const float TECH_PANEL_FADE_TIME = 0.3f;
const int TECH_PANEL_WIDTH = 280;
const int TECH_PANEL_HEIGHT = 120;

// 4D Effect parameters
const float NEON_TRANSITION_FREQUENCY = 0.5f;
const float TEXT_WAVE_AMPLITUDE = 5.0f;
const float TEXT_WAVE_PERIOD = 1.0f;
const float GEAR_ROTATION_SPEED = 360.0f; // degrees per second

/**
 * 4D High-Tech Kill Panel Manager
 * Activates only when player has Tech Display Card (ID=9999)
 * Provides advanced 4D visual effects with performance optimization
 */
class CTechKillPanelManager : public CUIGroup
{
public:
    // Quality tiers for performance optimization
    enum QUALITY_TIER
    {
        QUALITY_PERFORMANCE = 0,  // 50 particles, 1/sec gradient, no parallax
        QUALITY_STANDARD = 1,     // 80 particles, 0.5/sec gradient, pseudo-3D parallax
        QUALITY_ULTIMATE = 2      // 120 particles, 0.3/sec gradient, real-time parallax
    };

private:
    static const float ACTIVATION_CHECK_INTERVAL; // Check equipment every 0.1s

public:
    CTechKillPanelManager(EngineDeviceMan* pEngineDevice);
    virtual ~CTechKillPanelManager();

public:
    void CreateSubControl();
    
    virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
    virtual void Render(LPDIRECT3DDEVICE9 pd3dDevice);
    virtual void SetVisibleSingle(BOOL bVisible);

public:
    // Main interface
    void TriggerTechKillPanel(EKILL_TYPE killType, DWORD targetID = 0, const CString& attackerName = _T(""), const CString& targetName = _T(""));
    void Reset();
    
    // Configuration interface
    void SetEnabled(BOOL bEnabled) { m_bEnabled = bEnabled; }
    BOOL IsEnabled() const { return m_bEnabled && m_bTechCardEquipped; }
    
    void SetQualityTier(QUALITY_TIER tier) { m_eQualityTier = tier; }
    QUALITY_TIER GetQualityTier() const { return m_eQualityTier; }

private:
    // Internal methods
    BOOL CheckTechCardEquipped();
    void ShowTechPanel(EKILL_TYPE killType, const CString& attackerName, const CString& targetName);
    void UpdatePanelAnimation(float fElapsedTime);
    void UpdateNeonEffects(float fElapsedTime);
    void UpdateTextWaveMotion(float fElapsedTime);
    void UpdateGearRotation(float fElapsedTime);
    
    // Fallback to original system
    void TriggerOriginalKillDisplay(EKILL_TYPE killType, DWORD targetID);

private:
    // Core components
    CTechPanelRenderer* m_pRenderer;
    CParticleSystemOptimized* m_pParticleSystem;
    CEquipmentDetector* m_pEquipmentDetector;
    CPerformanceMonitor* m_pPerformanceMonitor;
    
    // State variables
    BOOL m_bEnabled;
    BOOL m_bTechCardEquipped;
    BOOL m_bPanelActive;
    BOOL m_bFlyingIn;
    BOOL m_bFadingOut;
    
    // Timing
    float m_fCurrentTime;
    float m_fPanelShowTime;
    float m_fActivationCheckTimer;
    float m_fNeonPhase;
    float m_fTextWavePhase;
    float m_fGearRotation;
    
    // Display data
    CString m_strAttackerName;
    CString m_strTargetName;
    EKILL_TYPE m_eCurrentKillType;
    
    // Quality and performance
    QUALITY_TIER m_eQualityTier;
    float m_fCurrentFPS;
    DWORD m_dwLastFrameTime;
    
    // Panel position and animation
    D3DXVECTOR2 m_vPanelPosition;
    D3DXVECTOR2 m_vTargetPosition;
    D3DXVECTOR2 m_vStartPosition;
    float m_fAnimationProgress;
    
    // Render targets for effects
    LPDIRECT3DTEXTURE9 m_pBackgroundTexture;
    LPDIRECT3DSURFACE9 m_pBackgroundSurface;
};