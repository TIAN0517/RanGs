#include "StyleManager.h"
#include <cstring>
#include <string>

//-----------------------------------------------------------------------------------------------//
// Kill Panel Style Management System Implementation - Jy技術團隊
//-----------------------------------------------------------------------------------------------//

// 靜態實例
CStyleManager* CStyleManager::s_pInstance = nullptr;

CStyleManager::CStyleManager()
    : m_eCurrentStyle(STYLE_CYBERPUNK)
    , m_nStyleQuality(2) // 中等品質
    , m_bInitialized(false)
{
    // 初始化配置陣列
    memset(m_StyleConfigs, 0, sizeof(m_StyleConfigs));
}

CStyleManager::~CStyleManager()
{
    Shutdown();
}

CStyleManager* CStyleManager::GetInstance()
{
    if (!s_pInstance)
    {
        s_pInstance = new CStyleManager();
    }
    return s_pInstance;
}

void CStyleManager::ReleaseInstance()
{
    if (s_pInstance)
    {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

bool CStyleManager::Initialize()
{
    if (m_bInitialized)
        return true;
        
    // 初始化所有風格配置
    InitializeStyleConfigs();
    
    m_bInitialized = true;
    return true;
}

void CStyleManager::Shutdown()
{
    m_bInitialized = false;
}

void CStyleManager::InitializeStyleConfigs()
{
    SetupCyberpunkStyle();
    SetupQuantumTechStyle();
    SetupMilitaryTacticalStyle();
    SetupHolographicStyle();
    SetupSpaceSciFiStyle();
    SetupSteampunkStyle();
}

void CStyleManager::SetupCyberpunkStyle()
{
    STYLE_CONFIG& config = m_StyleConfigs[STYLE_CYBERPUNK];
    config.eStyle = STYLE_CYBERPUNK;
    config.primaryColor = D3DXCOLOR(1.0f, 0.0f, 0.498f, 1.0f);      // #FF007F 霓虹粉紫
    config.secondaryColor = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);      // #00FFFF 電藍色
    config.szStyleName = "賽博朋克風格";
    config.szShaderName = "CyberpunkEffect.hlsl";
    config.fEffectIntensity = 1.0f;
    config.nParticleCount = 150;
    config.bGlitchEffect = true;
    config.bDistortionEffect = true;
}

void CStyleManager::SetupQuantumTechStyle()
{
    STYLE_CONFIG& config = m_StyleConfigs[STYLE_QUANTUM_TECH];
    config.eStyle = STYLE_QUANTUM_TECH;
    config.primaryColor = D3DXCOLOR(0.0f, 0.502f, 1.0f, 1.0f);      // #0080FF 量子藍
    config.secondaryColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);      // #FFFFFF 能量白光
    config.szStyleName = "量子科技風格";
    config.szShaderName = "QuantumEffect.hlsl";
    config.fEffectIntensity = 0.85f;
    config.nParticleCount = 200;
    config.bGlitchEffect = false;
    config.bDistortionEffect = true;
}

void CStyleManager::SetupMilitaryTacticalStyle()
{
    STYLE_CONFIG& config = m_StyleConfigs[STYLE_MILITARY_TACTICAL];
    config.eStyle = STYLE_MILITARY_TACTICAL;
    config.primaryColor = D3DXCOLOR(0.294f, 0.486f, 0.349f, 1.0f);  // #4B7C59 軍綠色
    config.secondaryColor = D3DXCOLOR(1.0f, 0.549f, 0.0f, 1.0f);    // #FF8C00 橙色警告
    config.szStyleName = "軍事戰術風格";
    config.szShaderName = "MilitaryHUD.hlsl";
    config.fEffectIntensity = 0.9f;
    config.nParticleCount = 100;
    config.bGlitchEffect = false;
    config.bDistortionEffect = false;
}

void CStyleManager::SetupHolographicStyle()
{
    STYLE_CONFIG& config = m_StyleConfigs[STYLE_HOLOGRAPHIC];
    config.eStyle = STYLE_HOLOGRAPHIC;
    config.primaryColor = D3DXCOLOR(0.251f, 0.878f, 0.816f, 1.0f);  // #40E0D0 全息藍
    config.secondaryColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);      // #FFFFFF80 透明白光
    config.szStyleName = "全息投影風格";
    config.szShaderName = "HologramProjection.hlsl";
    config.fEffectIntensity = 0.7f;
    config.nParticleCount = 80;
    config.bGlitchEffect = true;
    config.bDistortionEffect = true;
}

void CStyleManager::SetupSpaceSciFiStyle()
{
    STYLE_CONFIG& config = m_StyleConfigs[STYLE_SPACE_SCIFI];
    config.eStyle = STYLE_SPACE_SCIFI;
    config.primaryColor = D3DXCOLOR(0.098f, 0.098f, 0.439f, 1.0f);  // #191970 深空藍
    config.secondaryColor = D3DXCOLOR(0.753f, 0.753f, 0.753f, 1.0f); // #C0C0C0 星光銀
    config.szStyleName = "太空科幻風格";
    config.szShaderName = "SpaceNebulaEffect.hlsl";
    config.fEffectIntensity = 0.8f;
    config.nParticleCount = 120;
    config.bGlitchEffect = false;
    config.bDistortionEffect = true;
}

void CStyleManager::SetupSteampunkStyle()
{
    STYLE_CONFIG& config = m_StyleConfigs[STYLE_STEAMPUNK];
    config.eStyle = STYLE_STEAMPUNK;
    config.primaryColor = D3DXCOLOR(0.722f, 0.451f, 0.2f, 1.0f);    // #B87333 復古銅色
    config.secondaryColor = D3DXCOLOR(0.961f, 0.961f, 0.863f, 1.0f); // #F5F5DC 蒸汽白
    config.szStyleName = "蒸汽朋克風格";
    config.szShaderName = "SteampunkMechanical.hlsl";
    config.fEffectIntensity = 0.75f;
    config.nParticleCount = 90;
    config.bGlitchEffect = false;
    config.bDistortionEffect = false;
}

bool CStyleManager::SetCurrentStyle(EKILL_PANEL_STYLE eStyle)
{
    if (!IsValidStyle(eStyle))
        return false;
        
    m_eCurrentStyle = eStyle;
    return true;
}

const STYLE_CONFIG* CStyleManager::GetStyleConfig(EKILL_PANEL_STYLE eStyle) const
{
    if (!IsValidStyle(eStyle))
        return nullptr;
        
    return &m_StyleConfigs[eStyle];
}

const STYLE_CONFIG* CStyleManager::GetCurrentStyleConfig() const
{
    return GetStyleConfig(m_eCurrentStyle);
}

D3DXCOLOR CStyleManager::GetStylePrimaryColor(EKILL_PANEL_STYLE eStyle) const
{
    const STYLE_CONFIG* pConfig = GetStyleConfig(eStyle);
    return pConfig ? pConfig->primaryColor : D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

D3DXCOLOR CStyleManager::GetStyleSecondaryColor(EKILL_PANEL_STYLE eStyle) const
{
    const STYLE_CONFIG* pConfig = GetStyleConfig(eStyle);
    return pConfig ? pConfig->secondaryColor : D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
}

const char* CStyleManager::GetStyleName(EKILL_PANEL_STYLE eStyle) const
{
    const STYLE_CONFIG* pConfig = GetStyleConfig(eStyle);
    return pConfig ? pConfig->szStyleName : "未知風格";
}

const char* CStyleManager::GetStyleShaderName(EKILL_PANEL_STYLE eStyle) const
{
    const STYLE_CONFIG* pConfig = GetStyleConfig(eStyle);
    return pConfig ? pConfig->szShaderName : "";
}

void CStyleManager::SetStyleIntensity(EKILL_PANEL_STYLE eStyle, float fIntensity)
{
    if (!IsValidStyle(eStyle))
        return;
        
    m_StyleConfigs[eStyle].fEffectIntensity = max(0.0f, min(1.0f, fIntensity));
}

float CStyleManager::GetStyleIntensity(EKILL_PANEL_STYLE eStyle) const
{
    const STYLE_CONFIG* pConfig = GetStyleConfig(eStyle);
    return pConfig ? pConfig->fEffectIntensity : 1.0f;
}

bool CStyleManager::IsValidStyle(EKILL_PANEL_STYLE eStyle) const
{
    return (eStyle >= 0 && eStyle < KILL_PANEL_STYLE_COUNT);
}

bool CStyleManager::SaveStyleConfig(const char* szConfigFile)
{
    // TODO: 實現配置保存到文件
    return true;
}

bool CStyleManager::LoadStyleConfig(const char* szConfigFile)
{
    // TODO: 實現從文件載入配置
    return true;
}