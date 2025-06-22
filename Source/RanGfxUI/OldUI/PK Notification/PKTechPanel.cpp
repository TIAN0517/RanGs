#include "stdafx.h"
#include "PKTechPanel.h"
#include "TechPanelPerformanceManager.h"
#include "../../../enginelib/Common/TechPanelCoreOptimization.h"
#include "../../enginelib/DxTools/D3DFont.h"
#include "../../../enginelib/GUInterface/UITextControl.h"
#include "../../../enginelib/GUInterface/GameTextControl.h"
#include "../../../enginelib/GUInterface/BasicLineBoxEx.h"
#include "../../../enginelib/GUInterface/BasicTextBoxEx.h"
#include "../../../RanLogicClient/Char/GLCharacter.h"
#include "../../../RanLogicClient/GLGaeaClient.h"
#include "../../../enginelib/DxSound/DxSoundLib.h"
#include "../../../enginelib/DxEffect/Single/DxEffGroupPlayer.h"

#include "../../InnerInterface.h"

// ----------------------------------------------------------------------------
#include "../../../SigmaCore/DebugInclude.h"
// ----------------------------------------------------------------------------

CPKTechPanel::CPKTechPanel(GLGaeaClient* pGaeaClient, EngineDeviceMan* pEngineDevice)
    : CUIGroup(pEngineDevice)
    , m_pGaeaClient(pGaeaClient)
    , m_pHologramPanel(NULL)
    , m_pBlueGlowEdge(NULL)
    , m_pDataStreamBG(NULL)
    , m_pParticleEffect(NULL)
    , m_pKillerAvatar(NULL)
    , m_pEnergyShield(NULL)
    , m_pKillIcon(NULL)
    , m_pTechKillText(NULL)
    , m_eCurrentState(TECH_PANEL_HIDDEN)
    , m_fAnimationTimer(0.0f)
    , m_fDisplayDuration(2.5f)
    , m_fNeonColorPhase(0.0f)
    , m_fFloatOffset(0.0f)
    , m_fGearRotation(0.0f)
    , m_fDataStreamOffset(0.0f)
    , m_fParticlePhase(0.0f)
{
    m_vStartPos = D3DXVECTOR3(-400.0f, 0.0f, 0.0f);  // Off-screen left
    m_vTargetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // Center screen
    
    // Initialize core optimizations for tech panel
    TechPanelCore::InitializeCoreOptimizations();
}

CPKTechPanel::~CPKTechPanel()
{
    // Shutdown core optimizations
    TechPanelCore::ShutdownCoreOptimizations();
}

void CPKTechPanel::CreateSubControl()
{
    CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 12, _DEFAULT_FONT_SHADOW_FLAG);

    // Create holographic main panel (280x120px)
    m_pHologramPanel = new CUIControl(m_pEngineDevice);
    m_pHologramPanel->CreateSub(this, "TECH_PANEL_HOLOGRAM_MAIN");
    m_pHologramPanel->SetVisibleSingle(FALSE);
    RegisterControl(m_pHologramPanel);

    // Create blue glow edge effect (0.5px glow)
    m_pBlueGlowEdge = new CUIControl(m_pEngineDevice);
    m_pBlueGlowEdge->CreateSub(this, "TECH_PANEL_BLUE_GLOW");
    m_pBlueGlowEdge->SetVisibleSingle(FALSE);
    RegisterControl(m_pBlueGlowEdge);

    // Create data stream background (Matrix-style code rain)
    m_pDataStreamBG = new CUIControl(m_pEngineDevice);
    m_pDataStreamBG->CreateSub(this, "TECH_PANEL_DATA_STREAM");
    m_pDataStreamBG->SetVisibleSingle(FALSE);
    RegisterControl(m_pDataStreamBG);

    // Create particle light background
    m_pParticleEffect = new CUIControl(m_pEngineDevice);
    m_pParticleEffect->CreateSub(this, "TECH_PANEL_PARTICLES");
    m_pParticleEffect->SetVisibleSingle(FALSE);
    RegisterControl(m_pParticleEffect);

    // Create killer avatar (circular with energy shield)
    m_pKillerAvatar = new CUIControl(m_pEngineDevice);
    m_pKillerAvatar->CreateSub(this, "TECH_PANEL_KILLER_AVATAR");
    m_pKillerAvatar->SetVisibleSingle(FALSE);
    RegisterControl(m_pKillerAvatar);

    // Create energy shield border
    m_pEnergyShield = new CUIControl(m_pEngineDevice);
    m_pEnergyShield->CreateSub(this, "TECH_PANEL_ENERGY_SHIELD");
    m_pEnergyShield->SetVisibleSingle(FALSE);
    RegisterControl(m_pEnergyShield);

    // Create rotating gear kill icon
    m_pKillIcon = new CUIControl(m_pEngineDevice);
    m_pKillIcon->CreateSub(this, "TECH_PANEL_KILL_ICON");
    m_pKillIcon->SetVisibleSingle(FALSE);
    RegisterControl(m_pKillIcon);

    // Create dynamic tech kill text
    m_pTechKillText = new CBasicTextBoxEx(m_pEngineDevice);
    m_pTechKillText->CreateSub(this, "TECH_PANEL_KILL_TEXT");
    m_pTechKillText->SetFont(pFont);
    m_pTechKillText->SetTextAlign(TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y);
    m_pTechKillText->SetVisibleSingle(FALSE);
    RegisterControl(m_pTechKillText);
}

void CPKTechPanel::TranslateUIMessage(UIGUID cID, DWORD dwMsg)
{
    CUIGroup::TranslateUIMessage(cID, dwMsg);
}

void CPKTechPanel::SetVisibleSingle(BOOL bVisible)
{
    CUIGroup::SetVisibleSingle(bVisible);
    
    if (!bVisible)
    {
        // Hide all components when panel is hidden
        if (m_pHologramPanel) m_pHologramPanel->SetVisibleSingle(FALSE);
        if (m_pBlueGlowEdge) m_pBlueGlowEdge->SetVisibleSingle(FALSE);
        if (m_pDataStreamBG) m_pDataStreamBG->SetVisibleSingle(FALSE);
        if (m_pParticleEffect) m_pParticleEffect->SetVisibleSingle(FALSE);
        if (m_pKillerAvatar) m_pKillerAvatar->SetVisibleSingle(FALSE);
        if (m_pEnergyShield) m_pEnergyShield->SetVisibleSingle(FALSE);
        if (m_pKillIcon) m_pKillIcon->SetVisibleSingle(FALSE);
        if (m_pTechKillText) m_pTechKillText->SetVisibleSingle(FALSE);
        
        m_eCurrentState = TECH_PANEL_HIDDEN;
        m_fAnimationTimer = 0.0f;
    }
}

void CPKTechPanel::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
    if (!IsVisible()) return;

    CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);
    
    // Update core optimizations
    TechPanelCore::UpdateCoreOptimizations(fElapsedTime);
    
    // Update performance monitoring system
    TECH_PERF_MGR.Update();
    
    // Update animations based on current state
    UpdateAnimations(fElapsedTime);
    
    // Update visual effects based on performance manager settings
    if (TECH_PERF_MGR.IsNeonTextEnabled())
    {
        UpdateNeonTextEffect(fElapsedTime);
    }
    
    if (TECH_PERF_MGR.Is3DFloatingEnabled())
    {
        Update3DFloatingEffect(fElapsedTime);
    }
    
    if (TECH_PERF_MGR.IsDataStreamEnabled())
    {
        UpdateDataStreamBackground(fElapsedTime);
    }
    
    if (TECH_PERF_MGR.AreParticleEffectsEnabled())
    {
        UpdateParticleBackground(fElapsedTime);
    }
    
    UpdateGearRotation(fElapsedTime);
    
    // Update component visibility based on performance settings
    UpdateComponentVisibility();
}

void CPKTechPanel::Render(LPDIRECT3DDEVICEQ pd3dDevice)
{
    if (!IsVisible() || m_eCurrentState == TECH_PANEL_HIDDEN) return;
    
    // Render holographic panel with transparency (always enabled)
    if (TECH_PERF_MGR.IsHolographicEnabled())
    {
        RenderHolographicPanel(pd3dDevice);
    }
    
    // Render blue glow edge (minimal performance impact)
    RenderBlueGlowEdge(pd3dDevice);
    
    // Render performance-sensitive effects
    if (TECH_PERF_MGR.IsDataStreamEnabled())
    {
        RenderDataStreamBackground(pd3dDevice);
    }
    
    if (TECH_PERF_MGR.AreParticleEffectsEnabled())
    {
        RenderParticleBackground(pd3dDevice);
    }
    
    // Render neon text
    if (TECH_PERF_MGR.IsNeonTextEnabled())
    {
        RenderNeonText(pd3dDevice);
    }
    
    // Render base UI components
    CUIGroup::Render(pd3dDevice);
}

bool CPKTechPanel::IsTechCardActive() const
{
    return CheckTechCardInInventory();
}

bool CPKTechPanel::CheckTechCardInInventory() const
{
    if (!m_pGaeaClient) return false;
    
    GLCharacter* pCharacter = m_pGaeaClient->GetCharacter();
    if (!pCharacter) return false;
    
    // Check for tech card item (ID=9999) in inventory
    SNATIVEID sTechCardID(TECH_CARD_ITEM_ID, 0);
    const SINVENITEM* pItem = pCharacter->m_cInventory.FindItem(sTechCardID);
    
    return (pItem != NULL);
}

void CPKTechPanel::ShowTechKillNotification(const SPK_HISTORY& sHistory)
{
    // Only show tech panel if player has the tech card
    if (!IsTechCardActive())
    {
        return; // Fall back to original system
    }
    
    // Set kill notification text
    char strKillText[256];
    sprintf_s(strKillText, 256, "[科技击杀] %s → %s", 
        sHistory.szCharKiller, sHistory.szCharKilled);
    
    if (m_pTechKillText)
    {
        m_pTechKillText->SetText(strKillText);
        m_pTechKillText->SetVisibleSingle(TRUE);
    }
    
    // Show all visual components based on performance settings
    if (m_pHologramPanel) m_pHologramPanel->SetVisibleSingle(TRUE);
    if (m_pBlueGlowEdge) m_pBlueGlowEdge->SetVisibleSingle(TRUE);
    
    if (TECH_PERF_MGR.IsDataStreamEnabled() && m_pDataStreamBG) 
        m_pDataStreamBG->SetVisibleSingle(TRUE);
    
    if (TECH_PERF_MGR.AreParticleEffectsEnabled() && m_pParticleEffect) 
        m_pParticleEffect->SetVisibleSingle(TRUE);
    
    if (m_pKillerAvatar) m_pKillerAvatar->SetVisibleSingle(TRUE);
    if (m_pEnergyShield) m_pEnergyShield->SetVisibleSingle(TRUE);
    if (m_pKillIcon) m_pKillIcon->SetVisibleSingle(TRUE);
    
    // Start fly-in animation
    m_eCurrentState = TECH_PANEL_FLYING_IN;
    m_fAnimationTimer = 0.0f;
    
    // Register tech panel with core system for optimizations
    TECH_CORE_MGR.RegisterTechPanel();
    
    // Play tech sound effect
    DxSoundLib::GetInstance()->PlaySound("TECH_KILL_NOTIFICATION");
    
    SetVisibleSingle(TRUE);
}

void CPKTechPanel::HideTechPanel()
{
    if (m_eCurrentState != TECH_PANEL_HIDDEN)
    {
        m_eCurrentState = TECH_PANEL_FADING_OUT;
        m_fAnimationTimer = 0.0f;
        
        // Unregister tech panel from core system
        TECH_CORE_MGR.UnregisterTechPanel();
    }
}

void CPKTechPanel::UpdateAnimations(float fElapsedTime)
{
    m_fAnimationTimer += fElapsedTime * m_sConfig.fAnimationSpeed;
    
    switch (m_eCurrentState)
    {
    case TECH_PANEL_FLYING_IN:
        UpdateFlyInAnimation(fElapsedTime);
        break;
        
    case TECH_PANEL_DISPLAYING:
        UpdateDisplayAnimation(fElapsedTime);
        break;
        
    case TECH_PANEL_FADING_OUT:
        UpdateFadeOutAnimation(fElapsedTime);
        break;
    }
}

void CPKTechPanel::UpdateFlyInAnimation(float fElapsedTime)
{
    const float FLYIN_DURATION = 0.5f; // easeOutBack animation duration
    
    if (m_fAnimationTimer >= FLYIN_DURATION)
    {
        // Transition to display state
        m_eCurrentState = TECH_PANEL_DISPLAYING;
        m_fAnimationTimer = 0.0f;
        return;
    }
    
    // Calculate easeOutBack position
    float t = m_fAnimationTimer / FLYIN_DURATION;
    D3DXVECTOR3 currentPos = CalculateEaseOutBack(m_vStartPos, m_vTargetPos, t);
    
    // Apply position to panel (this would be applied to render transforms)
    // For now, we'll use opacity to simulate the effect
    float fAlpha = t;
    
    // Set component alpha values
    if (m_pHologramPanel)
        m_pHologramPanel->SetDiffuse(D3DCOLOR_ARGB((BYTE)(255 * fAlpha), 255, 255, 255));
}

void CPKTechPanel::UpdateDisplayAnimation(float fElapsedTime)
{
    if (m_fAnimationTimer >= m_fDisplayDuration)
    {
        // Start fade out
        m_eCurrentState = TECH_PANEL_FADING_OUT;
        m_fAnimationTimer = 0.0f;
    }
}

void CPKTechPanel::UpdateFadeOutAnimation(float fElapsedTime)
{
    const float FADEOUT_DURATION = 0.8f; // Fade with trail effect
    
    if (m_fAnimationTimer >= FADEOUT_DURATION)
    {
        // Hide completely
        SetVisibleSingle(FALSE);
        return;
    }
    
    // Calculate fade alpha
    float t = m_fAnimationTimer / FADEOUT_DURATION;
    float fAlpha = 1.0f - t;
    
    // Apply fade to all components
    if (m_pHologramPanel)
        m_pHologramPanel->SetDiffuse(D3DCOLOR_ARGB((BYTE)(255 * fAlpha), 255, 255, 255));
    if (m_pTechKillText)
        m_pTechKillText->SetDiffuse(D3DCOLOR_ARGB((BYTE)(255 * fAlpha), 255, 255, 255));
}

void CPKTechPanel::UpdateNeonTextEffect(float fElapsedTime)
{
    if (!m_sConfig.bNeonTextEnabled) return;
    
    // RGB animation with 0.5 second frequency
    m_fNeonColorPhase += fElapsedTime * 2.0f; // 2.0f = 1/0.5s
    if (m_fNeonColorPhase > 2.0f * D3DX_PI)
        m_fNeonColorPhase -= 2.0f * D3DX_PI;
    
    // Apply neon color to text
    if (m_pTechKillText)
    {
        D3DXCOLOR neonColor = GetNeonColor(m_fNeonColorPhase);
        m_pTechKillText->SetDiffuse(neonColor);
    }
}

void CPKTechPanel::Update3DFloatingEffect(float fElapsedTime)
{
    if (!m_sConfig.b3DFloatEnabled) return;
    
    // Y-axis floating ±5px with 1 second cycle
    m_fFloatOffset += fElapsedTime * 2.0f * D3DX_PI; // Full cycle per second
    if (m_fFloatOffset > 2.0f * D3DX_PI)
        m_fFloatOffset -= 2.0f * D3DX_PI;
    
    float yOffset = sin(m_fFloatOffset) * 5.0f; // ±5px amplitude
    
    // Apply Y offset to text (this would normally modify render position)
    // For simplicity, we'll store it for the render phase
}

void CPKTechPanel::UpdateDataStreamBackground(float fElapsedTime)
{
    if (!m_sConfig.bDataStreamEnabled) return;
    
    // Scroll data stream at 2px/second
    m_fDataStreamOffset += fElapsedTime * 2.0f;
    
    // Wrap around when it goes beyond texture height
    if (m_fDataStreamOffset > 256.0f) // Assuming 256px texture height
        m_fDataStreamOffset -= 256.0f;
}

void CPKTechPanel::UpdateParticleBackground(float fElapsedTime)
{
    if (!m_sConfig.bParticleEnabled) return;
    
    // Update particle animation phase
    m_fParticlePhase += fElapsedTime;
    if (m_fParticlePhase > 2.0f * D3DX_PI)
        m_fParticlePhase -= 2.0f * D3DX_PI;
}

void CPKTechPanel::UpdateGearRotation(float fElapsedTime)
{
    // Rotate gear 360° per second
    m_fGearRotation += fElapsedTime * 2.0f * D3DX_PI;
    if (m_fGearRotation > 2.0f * D3DX_PI)
        m_fGearRotation -= 2.0f * D3DX_PI;
    
    // Apply rotation to kill icon (would be applied in render)
}

D3DXVECTOR3 CPKTechPanel::CalculateEaseOutBack(const D3DXVECTOR3& start, const D3DXVECTOR3& end, float t)
{
    // easeOutBack function implementation
    const float c1 = 1.70158f;
    const float c3 = c1 + 1.0f;
    
    float easedT = 1.0f + c3 * pow(t - 1.0f, 3.0f) + c1 * pow(t - 1.0f, 2.0f);
    
    return start + (end - start) * easedT;
}

D3DXCOLOR CPKTechPanel::GetNeonColor(float phase)
{
    // Cycle through RGB colors for neon effect
    float r = (sin(phase) + 1.0f) * 0.5f;
    float g = (sin(phase + 2.0f * D3DX_PI / 3.0f) + 1.0f) * 0.5f;
    float b = (sin(phase + 4.0f * D3DX_PI / 3.0f) + 1.0f) * 0.5f;
    
    // Ensure minimum brightness for visibility
    r = max(r, 0.3f);
    g = max(g, 0.3f);
    b = max(b, 0.3f);
    
    return D3DXCOLOR(r, g, b, 1.0f);
}

void CPKTechPanel::UpdateComponentVisibility()
{
    // Update component visibility based on current performance settings
    if (m_pDataStreamBG)
    {
        m_pDataStreamBG->SetVisibleSingle(TECH_PERF_MGR.IsDataStreamEnabled() ? TRUE : FALSE);
    }
    
    if (m_pParticleEffect)
    {
        m_pParticleEffect->SetVisibleSingle(TECH_PERF_MGR.AreParticleEffectsEnabled() ? TRUE : FALSE);
    }
}

void CPKTechPanel::MonitorPerformance(float fElapsedTime)
{
    // Legacy performance monitoring - now handled by TechPanelPerformanceManager
    // This method is kept for compatibility but functionality moved to performance manager
}

bool CPKTechPanel::IsLowPerformance() const
{
    return TECH_PERF_MGR.IsPerformanceMode() || TECH_PERF_MGR.IsCriticalMode();
}

bool CPKTechPanel::IsPerformanceMode() const
{
    return TECH_PERF_MGR.IsPerformanceMode();
}

bool CPKTechPanel::IsCriticalMode() const
{
    return TECH_PERF_MGR.IsCriticalMode();
}

bool CPKTechPanel::HasTechCardItem() const
{
    return CheckTechCardInInventory();
}

// Render methods (simplified implementations - would need proper D3D rendering)
void CPKTechPanel::RenderHolographicPanel(LPDIRECT3DDEVICEQ pd3dDevice)
{
    // Would render semi-transparent holographic panel with proper materials
}

void CPKTechPanel::RenderBlueGlowEdge(LPDIRECT3DDEVICEQ pd3dDevice)
{
    // Would render 0.5px blue glow edge effect
}

void CPKTechPanel::RenderDataStreamBackground(LPDIRECT3DDEVICEQ pd3dDevice)
{
    // Would render Matrix-style code rain with vertical scrolling
}

void CPKTechPanel::RenderParticleBackground(LPDIRECT3DDEVICEQ pd3dDevice)
{
    // Would render particle light background with parallax
}

void CPKTechPanel::RenderNeonText(LPDIRECT3DDEVICEQ pd3dDevice)
{
    // Would render text with neon edge effects and RGB animation
}