#include "stdafx.h"
#include "./TechKillPanelManager.h"
#include "./TechPanelRenderer.h"
#include "./ParticleSystemOptimized.h"
#include "./EquipmentDetector.h"
#include "./PerformanceMonitor.h"

#include "../../enginelib/GUInterface/GameTextControl.h"
#include "../../enginelib/DxTools/DxFontMan.h"
#include "../../enginelib/DxEffect/Single/DxEffSingle.h"

// Static constants
const float CTechKillPanelManager::ACTIVATION_CHECK_INTERVAL = 0.1f;

CTechKillPanelManager::CTechKillPanelManager(EngineDeviceMan* pEngineDevice)
    : CUIGroup(pEngineDevice)
    , m_pRenderer(NULL)
    , m_pParticleSystem(NULL)
    , m_pEquipmentDetector(NULL)
    , m_pPerformanceMonitor(NULL)
    , m_bEnabled(TRUE)
    , m_bTechCardEquipped(FALSE)
    , m_bPanelActive(FALSE)
    , m_bFlyingIn(FALSE)
    , m_bFadingOut(FALSE)
    , m_fCurrentTime(0.0f)
    , m_fPanelShowTime(0.0f)
    , m_fActivationCheckTimer(0.0f)
    , m_fNeonPhase(0.0f)
    , m_fTextWavePhase(0.0f)
    , m_fGearRotation(0.0f)
    , m_eCurrentKillType(0)
    , m_eQualityTier(QUALITY_STANDARD)
    , m_fCurrentFPS(60.0f)
    , m_dwLastFrameTime(0)
    , m_vPanelPosition(0, 0)
    , m_vTargetPosition(0, 0)
    , m_vStartPosition(0, 0)
    , m_fAnimationProgress(0.0f)
    , m_pBackgroundTexture(NULL)
    , m_pBackgroundSurface(NULL)
{
}

CTechKillPanelManager::~CTechKillPanelManager()
{
    SAFE_RELEASE(m_pBackgroundTexture);
    SAFE_RELEASE(m_pBackgroundSurface);
    
    SAFE_DELETE(m_pRenderer);
    SAFE_DELETE(m_pParticleSystem);
    SAFE_DELETE(m_pEquipmentDetector);
    SAFE_DELETE(m_pPerformanceMonitor);
}

void CTechKillPanelManager::CreateSubControl()
{
    // Create renderer component
    m_pRenderer = new CTechPanelRenderer(m_pEngineDevice);
    if (m_pRenderer)
    {
        m_pRenderer->CreateSubControl();
    }
    
    // Create optimized particle system
    m_pParticleSystem = new CParticleSystemOptimized(m_pEngineDevice);
    if (m_pParticleSystem)
    {
        m_pParticleSystem->Initialize();
    }
    
    // Create equipment detector
    m_pEquipmentDetector = new CEquipmentDetector();
    if (m_pEquipmentDetector)
    {
        m_pEquipmentDetector->Initialize();
    }
    
    // Create performance monitor
    m_pPerformanceMonitor = new CPerformanceMonitor();
    if (m_pPerformanceMonitor)
    {
        m_pPerformanceMonitor->Initialize();
    }
    
    // Create background render target for effects
    LPDIRECT3DDEVICE9 pd3dDevice = m_pEngineDevice->GetDevice();
    if (pd3dDevice)
    {
        D3DDISPLAYMODE d3ddm;
        pd3dDevice->GetDisplayMode(0, &d3ddm);
        
        HRESULT hr = pd3dDevice->CreateTexture(
            TECH_PANEL_WIDTH, TECH_PANEL_HEIGHT, 1,
            D3DUSAGE_RENDERTARGET, d3ddm.Format,
            D3DPOOL_DEFAULT, &m_pBackgroundTexture, NULL);
            
        if (SUCCEEDED(hr) && m_pBackgroundTexture)
        {
            m_pBackgroundTexture->GetSurfaceLevel(0, &m_pBackgroundSurface);
        }
    }
    
    // Set initial panel position (center of screen)
    UIRECT rcWindow = GetGlobalPos();
    m_vTargetPosition.x = (rcWindow.sizeX - TECH_PANEL_WIDTH) / 2.0f;
    m_vTargetPosition.y = (rcWindow.sizeY - TECH_PANEL_HEIGHT) / 2.0f;
}

void CTechKillPanelManager::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
    if (!m_bEnabled) return;
    
    m_fCurrentTime += fElapsedTime;
    
    // Check tech card equipment status periodically
    m_fActivationCheckTimer += fElapsedTime;
    if (m_fActivationCheckTimer >= ACTIVATION_CHECK_INTERVAL)
    {
        m_bTechCardEquipped = CheckTechCardEquipped();
        m_fActivationCheckTimer = 0.0f;
    }
    
    // Update performance monitor
    if (m_pPerformanceMonitor)
    {
        m_pPerformanceMonitor->Update(fElapsedTime);
        m_fCurrentFPS = m_pPerformanceMonitor->GetCurrentFPS();
        
        // Auto-adjust quality based on performance
        QUALITY_TIER recommendedTier = m_pPerformanceMonitor->GetRecommendedQualityTier();
        if (recommendedTier != m_eQualityTier)
        {
            SetQualityTier(recommendedTier);
        }
    }
    
    // Update panel animation if active
    if (m_bPanelActive)
    {
        UpdatePanelAnimation(fElapsedTime);
        UpdateNeonEffects(fElapsedTime);
        UpdateTextWaveMotion(fElapsedTime);
        UpdateGearRotation(fElapsedTime);
        
        // Update particle system
        if (m_pParticleSystem)
        {
            m_pParticleSystem->Update(fElapsedTime, m_eQualityTier);
        }
        
        // Check if panel should be hidden
        if (m_fPanelShowTime >= TECH_PANEL_DISPLAY_TIME && !m_bFadingOut)
        {
            m_bFadingOut = TRUE;
            m_fAnimationProgress = 0.0f;
        }
        
        // Check if fade out is complete
        if (m_bFadingOut && m_fAnimationProgress >= 1.0f)
        {
            Reset();
        }
    }
    
    CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);
}

void CTechKillPanelManager::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{
    if (!m_bEnabled || !m_bPanelActive || !m_bTechCardEquipped) return;
    
    // Render to background texture first
    if (m_pBackgroundSurface && m_pRenderer)
    {
        LPDIRECT3DSURFACE9 pOldTarget = NULL;
        pd3dDevice->GetRenderTarget(0, &pOldTarget);
        
        pd3dDevice->SetRenderTarget(0, m_pBackgroundSurface);
        pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
        
        // Render data stream background
        m_pRenderer->RenderDataStreamBackground(pd3dDevice, m_fCurrentTime);
        
        pd3dDevice->SetRenderTarget(0, pOldTarget);
        SAFE_RELEASE(pOldTarget);
    }
    
    // Render the main panel
    if (m_pRenderer)
    {
        m_pRenderer->RenderTechPanel(pd3dDevice, 
            m_vPanelPosition, 
            m_strAttackerName, 
            m_strTargetName,
            m_eCurrentKillType,
            m_fNeonPhase,
            m_fTextWavePhase,
            m_fGearRotation,
            m_fAnimationProgress,
            m_bFadingOut);
    }
    
    // Render particle effects
    if (m_pParticleSystem)
    {
        m_pParticleSystem->Render(pd3dDevice, m_vPanelPosition);
    }
    
    CUIGroup::Render(pd3dDevice);
}

void CTechKillPanelManager::SetVisibleSingle(BOOL bVisible)
{
    if (!bVisible)
    {
        Reset();
    }
    CUIGroup::SetVisibleSingle(bVisible);
}

void CTechKillPanelManager::TriggerTechKillPanel(EKILL_TYPE killType, DWORD targetID, const CString& attackerName, const CString& targetName)
{
    if (!m_bEnabled)
    {
        TriggerOriginalKillDisplay(killType, targetID);
        return;
    }
    
    // Check if tech card is equipped
    if (!CheckTechCardEquipped())
    {
        TriggerOriginalKillDisplay(killType, targetID);
        return;
    }
    
    // Show the tech panel
    ShowTechPanel(killType, attackerName, targetName);
}

void CTechKillPanelManager::Reset()
{
    m_bPanelActive = FALSE;
    m_bFlyingIn = FALSE;
    m_bFadingOut = FALSE;
    m_fPanelShowTime = 0.0f;
    m_fAnimationProgress = 0.0f;
    m_fNeonPhase = 0.0f;
    m_fTextWavePhase = 0.0f;
    m_fGearRotation = 0.0f;
    
    if (m_pParticleSystem)
    {
        m_pParticleSystem->Reset();
    }
}

BOOL CTechKillPanelManager::CheckTechCardEquipped()
{
    if (!m_pEquipmentDetector)
        return FALSE;
        
    return m_pEquipmentDetector->IsItemEquipped(TECH_DISPLAY_CARD_ID);
}

void CTechKillPanelManager::ShowTechPanel(EKILL_TYPE killType, const CString& attackerName, const CString& targetName)
{
    // Store display data
    m_eCurrentKillType = killType;
    m_strAttackerName = attackerName;
    m_strTargetName = targetName;
    
    // Set up animation - fly in from left edge
    UIRECT rcWindow = GetGlobalPos();
    m_vStartPosition.x = -TECH_PANEL_WIDTH;
    m_vStartPosition.y = m_vTargetPosition.y;
    m_vPanelPosition = m_vStartPosition;
    
    // Activate panel
    m_bPanelActive = TRUE;
    m_bFlyingIn = TRUE;
    m_bFadingOut = FALSE;
    m_fPanelShowTime = 0.0f;
    m_fAnimationProgress = 0.0f;
    
    // Initialize particle system
    if (m_pParticleSystem)
    {
        int particleCount = 50; // Default for PERFORMANCE
        if (m_eQualityTier == QUALITY_STANDARD) particleCount = 80;
        else if (m_eQualityTier == QUALITY_ULTIMATE) particleCount = 120;
        
        m_pParticleSystem->StartEffect(m_vTargetPosition, particleCount);
    }
}

void CTechKillPanelManager::UpdatePanelAnimation(float fElapsedTime)
{
    if (m_bFlyingIn)
    {
        // Fly in animation using easeOutBack easing
        m_fAnimationProgress += fElapsedTime / 0.5f; // 0.5 second fly-in
        
        if (m_fAnimationProgress >= 1.0f)
        {
            m_fAnimationProgress = 1.0f;
            m_bFlyingIn = FALSE;
        }
        
        // EaseOutBack calculation: t*(2.7*t - 1.7)
        float easedProgress = m_fAnimationProgress * (2.7f * m_fAnimationProgress - 1.7f);
        if (easedProgress < 0) easedProgress = 0;
        
        m_vPanelPosition.x = m_vStartPosition.x + (m_vTargetPosition.x - m_vStartPosition.x) * easedProgress;
    }
    else if (m_bFadingOut)
    {
        // Fade out animation
        m_fAnimationProgress += fElapsedTime / TECH_PANEL_FADE_TIME;
        if (m_fAnimationProgress > 1.0f)
            m_fAnimationProgress = 1.0f;
    }
    else
    {
        // Panel is visible, increment show time
        m_fPanelShowTime += fElapsedTime;
    }
}

void CTechKillPanelManager::UpdateNeonEffects(float fElapsedTime)
{
    float frequency = NEON_TRANSITION_FREQUENCY;
    if (m_eQualityTier == QUALITY_PERFORMANCE) frequency = 1.0f;
    else if (m_eQualityTier == QUALITY_ULTIMATE) frequency = 0.3f;
    
    m_fNeonPhase += fElapsedTime * frequency;
    if (m_fNeonPhase > 1.0f)
        m_fNeonPhase -= 1.0f;
}

void CTechKillPanelManager::UpdateTextWaveMotion(float fElapsedTime)
{
    m_fTextWavePhase += fElapsedTime / TEXT_WAVE_PERIOD;
    if (m_fTextWavePhase > 1.0f)
        m_fTextWavePhase -= 1.0f;
}

void CTechKillPanelManager::UpdateGearRotation(float fElapsedTime)
{
    m_fGearRotation += fElapsedTime * GEAR_ROTATION_SPEED;
    if (m_fGearRotation >= 360.0f)
        m_fGearRotation -= 360.0f;
}

void CTechKillPanelManager::TriggerOriginalKillDisplay(EKILL_TYPE killType, DWORD targetID)
{
    // Fallback to original kill display system
    // This would integrate with the existing KillCardManager
    // For now, we'll leave this as a placeholder for the hook system
}