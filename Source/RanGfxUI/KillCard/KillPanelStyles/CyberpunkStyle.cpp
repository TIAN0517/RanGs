#include "pch.h"
#include "CyberpunkStyle.h"
#include <random>

//-----------------------------------------------------------------------------------------------//
// Cyberpunk Style Implementation - Jy技術團隊
//-----------------------------------------------------------------------------------------------//

CCyberpunkStyle::CCyberpunkStyle()
    : m_pd3dDevice(nullptr)
    , m_pGlitchTexture(nullptr)
    , m_pCircuitTexture(nullptr)
    , m_pNoiseTexture(nullptr)
    , m_bInitialized(false)
    , m_fEffectIntensity(1.0f)
    , m_fGlobalTime(0.0f)
    , m_fGlitchTimer(0.0f)
    , m_fGlitchInterval(0.3f)
    , m_nMaxRainParticles(100)
    , m_fRainSpawnTimer(0.0f)
    , m_fNeonPulsePhase(0.0f)
    , m_fNeonIntensity(1.0f)
    , m_colorNeonPink(1.0f, 0.0f, 0.498f, 1.0f)     // #FF007F
    , m_colorElectricBlue(0.0f, 1.0f, 1.0f, 1.0f)   // #00FFFF
    , m_colorGlitch(1.0f, 0.0f, 0.0f, 0.8f)
{
}

CCyberpunkStyle::~CCyberpunkStyle()
{
    Shutdown();
}

bool CCyberpunkStyle::Initialize(LPDIRECT3DDEVICE9 pd3dDevice)
{
    if (m_bInitialized)
        return true;
        
    if (!pd3dDevice)
        return false;
        
    m_pd3dDevice = pd3dDevice;
    
    // TODO: 載入紋理資源
    // D3DXCreateTextureFromFile(pd3dDevice, "Resources/Textures/cyberpunk_glitch.dds", &m_pGlitchTexture);
    // D3DXCreateTextureFromFile(pd3dDevice, "Resources/Textures/circuit_pattern.dds", &m_pCircuitTexture);
    // D3DXCreateTextureFromFile(pd3dDevice, "Resources/Textures/noise.dds", &m_pNoiseTexture);
    
    // 初始化數位雨滴粒子系統
    InitDigitalRainParticles();
    
    m_bInitialized = true;
    return true;
}

void CCyberpunkStyle::Shutdown()
{
    if (m_pGlitchTexture)
    {
        m_pGlitchTexture->Release();
        m_pGlitchTexture = nullptr;
    }
    
    if (m_pCircuitTexture)
    {
        m_pCircuitTexture->Release();
        m_pCircuitTexture = nullptr;
    }
    
    if (m_pNoiseTexture)
    {
        m_pNoiseTexture->Release();
        m_pNoiseTexture = nullptr;
    }
    
    m_vecGlitchData.clear();
    m_vecRainParticles.clear();
    m_bInitialized = false;
}

void CCyberpunkStyle::RenderEffect(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea, float fElapsedTime)
{
    if (!m_bInitialized || !pd3dDevice)
        return;
        
    // 更新全局時間
    m_fGlobalTime += fElapsedTime;
    
    // 渲染電路板背景圖案
    RenderCircuitPattern(pd3dDevice, rcArea);
    
    // 渲染霓虹邊框
    RenderNeonBorder(pd3dDevice, rcArea);
    
    // 渲染數位雨滴
    RenderDigitalRain(pd3dDevice, rcArea);
    
    // 渲染故障閃爍效果
    RenderGlitchEffect(pd3dDevice, rcArea);
}

void CCyberpunkStyle::UpdateEffect(float fElapsedTime)
{
    if (!m_bInitialized)
        return;
        
    // 更新各種效果
    UpdateGlitchEffect(fElapsedTime);
    UpdateDigitalRain(fElapsedTime);
    UpdateNeonPulse(fElapsedTime);
}

void CCyberpunkStyle::TriggerGlitchEffect(float fIntensity)
{
    GLITCH_DATA glitch;
    glitch.fStartTime = m_fGlobalTime;
    glitch.fDuration = 0.1f + (rand() % 20) * 0.01f; // 0.1 - 0.3 秒
    glitch.fIntensity = fIntensity;
    glitch.vOffset = D3DXVECTOR2((rand() % 20 - 10) * 0.5f, (rand() % 10 - 5) * 0.2f);
    glitch.bActive = true;
    
    m_vecGlitchData.push_back(glitch);
}

void CCyberpunkStyle::StartDigitalRain()
{
    // 增加雨滴生成頻率
    m_fRainSpawnTimer = 0.0f;
}

void CCyberpunkStyle::StopAllEffects()
{
    m_vecGlitchData.clear();
    m_vecRainParticles.clear();
}

void CCyberpunkStyle::RenderGlitchEffect(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea)
{
    // 渲染故障閃爍效果
    for (auto& glitch : m_vecGlitchData)
    {
        if (!glitch.bActive)
            continue;
            
        // 創建故障區域
        UIRECT glitchRect = rcArea;
        glitchRect.left += glitch.vOffset.x;
        glitchRect.top += glitch.vOffset.y;
        
        // 設置故障顏色
        DWORD dwGlitchColor = D3DCOLOR_ARGB(
            (BYTE)(255 * glitch.fIntensity * 0.7f),
            255, 0, 0
        );
        
        // TODO: 渲染故障條紋
        // RenderColoredRect(pd3dDevice, glitchRect, dwGlitchColor);
    }
}

void CCyberpunkStyle::RenderDigitalRain(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea)
{
    // 渲染數位雨滴
    for (auto& particle : m_vecRainParticles)
    {
        if (!particle.bActive)
            continue;
            
        // 計算透明度
        float fAlpha = particle.fLife / 3.0f; // 3秒生命週期
        DWORD dwColor = D3DCOLOR_ARGB(
            (BYTE)(255 * fAlpha),
            (BYTE)(particle.color.r * 255),
            (BYTE)(particle.color.g * 255),
            (BYTE)(particle.color.b * 255)
        );
        
        // TODO: 渲染字符
        // RenderCharacter(pd3dDevice, particle.cCharacter, particle.vPosition, dwColor);
    }
}

void CCyberpunkStyle::RenderNeonBorder(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea)
{
    // 計算霓虹脈衝強度
    float fPulseIntensity = 0.7f + 0.3f * sinf(m_fNeonPulsePhase);
    
    // 霓虹粉紫邊框
    DWORD dwNeonColor = D3DCOLOR_ARGB(
        (BYTE)(255 * fPulseIntensity),
        (BYTE)(m_colorNeonPink.r * 255),
        (BYTE)(m_colorNeonPink.g * 255),
        (BYTE)(m_colorNeonPink.b * 255)
    );
    
    // TODO: 渲染不規則切角邊框
    // RenderNeonFrame(pd3dDevice, rcArea, dwNeonColor, 3.0f);
}

void CCyberpunkStyle::RenderCircuitPattern(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rcArea)
{
    // 渲染動態電路板紋理
    if (m_pCircuitTexture)
    {
        // 計算UV偏移來創建流動效果
        float fUOffset = fmodf(m_fGlobalTime * 0.1f, 1.0f);
        float fVOffset = fmodf(m_fGlobalTime * 0.05f, 1.0f);
        
        // TODO: 渲染帶有UV動畫的電路板背景
        // RenderTexturedRect(pd3dDevice, rcArea, m_pCircuitTexture, fUOffset, fVOffset);
    }
}

void CCyberpunkStyle::UpdateGlitchEffect(float fElapsedTime)
{
    m_fGlitchTimer += fElapsedTime;
    
    // 隨機觸發故障效果
    if (m_fGlitchTimer >= m_fGlitchInterval)
    {
        if ((rand() % 100) < 20) // 20% 機率觸發
        {
            TriggerGlitchEffect(0.5f + (rand() % 50) * 0.01f);
        }
        m_fGlitchTimer = 0.0f;
        m_fGlitchInterval = 0.2f + (rand() % 10) * 0.1f; // 隨機間隔
    }
    
    // 更新現有故障效果
    auto it = m_vecGlitchData.begin();
    while (it != m_vecGlitchData.end())
    {
        float fElapsed = m_fGlobalTime - it->fStartTime;
        if (fElapsed >= it->fDuration)
        {
            it = m_vecGlitchData.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void CCyberpunkStyle::UpdateDigitalRain(float fElapsedTime)
{
    m_fRainSpawnTimer += fElapsedTime;
    
    // 生成新的雨滴粒子
    if (m_fRainSpawnTimer >= 0.05f && m_vecRainParticles.size() < m_nMaxRainParticles)
    {
        D3DXVECTOR2 spawnPos(rand() % 800, -20.0f); // 從頂部生成
        SpawnDigitalRainParticle(spawnPos);
        m_fRainSpawnTimer = 0.0f;
    }
    
    // 更新粒子
    auto it = m_vecRainParticles.begin();
    while (it != m_vecRainParticles.end())
    {
        it->vPosition.y += it->fSpeed * fElapsedTime;
        it->fLife -= fElapsedTime;
        
        // 移除過期粒子
        if (it->fLife <= 0.0f || it->vPosition.y > 600.0f)
        {
            it = m_vecRainParticles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void CCyberpunkStyle::UpdateNeonPulse(float fElapsedTime)
{
    m_fNeonPulsePhase += fElapsedTime * 3.0f; // 脈衝頻率
    if (m_fNeonPulsePhase > 2.0f * D3DX_PI)
        m_fNeonPulsePhase -= 2.0f * D3DX_PI;
}

void CCyberpunkStyle::InitDigitalRainParticles()
{
    m_vecRainParticles.reserve(m_nMaxRainParticles);
}

void CCyberpunkStyle::SpawnDigitalRainParticle(const D3DXVECTOR2& vPosition)
{
    DIGITAL_RAIN_PARTICLE particle;
    particle.vPosition = vPosition;
    particle.fSpeed = 50.0f + (rand() % 100); // 隨機速度
    particle.fLife = 3.0f + (rand() % 20) * 0.1f; // 隨機生命週期
    particle.cCharacter = GetRandomCharacter();
    
    // 隨機選擇霓虹色彩
    if (rand() % 2)
        particle.color = m_colorNeonPink;
    else
        particle.color = m_colorElectricBlue;
        
    particle.bActive = true;
    
    m_vecRainParticles.push_back(particle);
}

char CCyberpunkStyle::GetRandomCharacter()
{
    // 隨機生成數字和符號
    const char chars[] = "0123456789ABCDEF!@#$%^&*()+-=[]{}|;:,.<>?";
    return chars[rand() % (sizeof(chars) - 1)];
}