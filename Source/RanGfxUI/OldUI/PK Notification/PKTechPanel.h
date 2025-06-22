#pragma once

#include "../../../enginelib/GUInterface/UIGroup.h"
#include "../../../enginelib/DxTools/d3dfont.h"
#include "../../RanLogic/Character/GLCharDefine.h"
#include "../../RanLogic/Msg/PKRankData.h"

class CBasicTextBoxEx;
class CBasicLineBox;
class GLGaeaClient;

//! Tech Card Item ID for activation
const DWORD TECH_CARD_ITEM_ID = 9999;

//! 4D Tech Panel Visual Effects
struct TECH_PANEL_CONFIG
{
    float fPanelOpacity;        // Panel transparency (0.0-1.0)
    float fGlowIntensity;       // Blue glow edge intensity
    float fAnimationSpeed;      // Speed of animations
    bool bParticleEnabled;      // Enable particle background
    bool bNeonTextEnabled;      // Enable neon text effects
    bool b3DFloatEnabled;       // Enable 3D floating effects
    bool bDataStreamEnabled;    // Enable data stream background
    
    TECH_PANEL_CONFIG()
        : fPanelOpacity(0.85f)
        , fGlowIntensity(1.0f)
        , fAnimationSpeed(1.0f)
        , bParticleEnabled(true)
        , bNeonTextEnabled(true)
        , b3DFloatEnabled(true)
        , bDataStreamEnabled(true)
    {
    }
};

//! Animation states for smooth transitions
enum ETech_Panel_State
{
    TECH_PANEL_HIDDEN = 0,
    TECH_PANEL_FLYING_IN,
    TECH_PANEL_DISPLAYING,
    TECH_PANEL_FADING_OUT,
};

//! 4D Tech-Style Borderless Kill Panel
class CPKTechPanel : public CUIGroup
{
private:
    GLGaeaClient* m_pGaeaClient;
    
    // Visual Components
    CUIControl* m_pHologramPanel;        // Main holographic panel
    CUIControl* m_pBlueGlowEdge;         // Blue glow edge effect
    CUIControl* m_pDataStreamBG;         // Data stream background
    CUIControl* m_pParticleEffect;       // Particle light background
    
    // Avatar and Icons
    CUIControl* m_pKillerAvatar;         // Circular killer avatar with energy shield
    CUIControl* m_pEnergyShield;         // Energy shield border for avatar
    CUIControl* m_pKillIcon;             // Rotating gear kill icon
    
    // Dynamic Text
    CBasicTextBoxEx* m_pTechKillText;    // "[科技击杀] 玩家A → 玩家B"
    
    // Animation System
    ETech_Panel_State m_eCurrentState;
    float m_fAnimationTimer;
    float m_fDisplayDuration;
    D3DXVECTOR3 m_vStartPos;
    D3DXVECTOR3 m_vTargetPos;
    
    // Visual Effects Data
    TECH_PANEL_CONFIG m_sConfig;
    float m_fNeonColorPhase;             // RGB animation phase
    float m_fFloatOffset;                // Y-axis floating offset
    float m_fGearRotation;               // Kill icon rotation
    float m_fDataStreamOffset;           // Background scroll offset
    float m_fParticlePhase;              // Particle animation phase
    
public:
    CPKTechPanel(GLGaeaClient* pGaeaClient, EngineDeviceMan* pEngineDevice);
    virtual ~CPKTechPanel();
    
    void CreateSubControl();
    
    // Core UI overrides
    virtual void TranslateUIMessage(UIGUID cID, DWORD dwMsg) override;
    virtual void SetVisibleSingle(BOOL bVisible) override;
    virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl) override;
    virtual void Render(LPDIRECT3DDEVICEQ pd3dDevice) override;
    
    // Tech Panel API
    void ShowTechKillNotification(const SPK_HISTORY& sHistory);
    void HideTechPanel();
    bool IsTechCardActive() const;
    bool CheckTechCardInInventory() const;
    
    // Configuration
    void SetTechConfig(const TECH_PANEL_CONFIG& config) { m_sConfig = config; }
    const TECH_PANEL_CONFIG& GetTechConfig() const { return m_sConfig; }
    
    // Performance Management (now handled by TechPanelPerformanceManager)
    bool IsPerformanceMode() const;
    bool IsCriticalMode() const;
    
private:
    // Animation Methods
    void UpdateAnimations(float fElapsedTime);
    void UpdateFlyInAnimation(float fElapsedTime);
    void UpdateDisplayAnimation(float fElapsedTime);
    void UpdateFadeOutAnimation(float fElapsedTime);
    
    // Visual Effects
    void UpdateNeonTextEffect(float fElapsedTime);
    void Update3DFloatingEffect(float fElapsedTime);
    void UpdateDataStreamBackground(float fElapsedTime);
    void UpdateParticleBackground(float fElapsedTime);
    void UpdateGearRotation(float fElapsedTime);
    
    // Rendering Effects
    void RenderHolographicPanel(LPDIRECT3DDEVICEQ pd3dDevice);
    void RenderBlueGlowEdge(LPDIRECT3DDEVICEQ pd3dDevice);
    void RenderDataStreamBackground(LPDIRECT3DDEVICEQ pd3dDevice);
    void RenderParticleBackground(LPDIRECT3DDEVICEQ pd3dDevice);
    void RenderNeonText(LPDIRECT3DDEVICEQ pd3dDevice);
    
    // Utility Methods
    D3DXVECTOR3 CalculateEaseOutBack(const D3DXVECTOR3& start, const D3DXVECTOR3& end, float t);
    DWORD InterpolateColor(DWORD color1, DWORD color2, float t);
    D3DXCOLOR GetNeonColor(float phase);
    void UpdateComponentVisibility();
    
    // Tech Card Detection
    bool HasTechCardItem() const;
    
    // Legacy compatibility
    void MonitorPerformance(float fElapsedTime);
};