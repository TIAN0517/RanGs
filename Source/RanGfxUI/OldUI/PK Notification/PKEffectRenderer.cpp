#include "StdAfx.h"
#include "PKEffectRenderer.h"
#include "../../../EngineLib/DxTools/DxViewPort.h"
#include <cstdlib>
#include <cmath>

CPKEffectRenderer::CPKEffectRenderer(EngineDeviceMan* pEngineDevice)
    : m_pEngineDevice(pEngineDevice)
    , m_fTime(0.0f)
    , m_emCurrentType(PK_CARD_NONE)
{
    m_vecParticles.reserve(100);
}

CPKEffectRenderer::~CPKEffectRenderer()
{
}

void CPKEffectRenderer::SetEffectType(EMPK_EFFECT_CARD_TYPE emType)
{
    if (m_emCurrentType != emType)
    {
        m_emCurrentType = emType;
        m_vecParticles.clear();
        m_fTime = 0.0f;
    }
}

void CPKEffectRenderer::Update(float fElapsedTime)
{
    m_fTime += fElapsedTime;
    
    // 清理过期粒子
    UpdateParticles(fElapsedTime);
}

void CPKEffectRenderer::Render(const UIRECT& rcBase)
{
    if (m_emCurrentType == PK_CARD_NONE) return;
    
    // 根据类型更新特效
    switch (m_emCurrentType)
    {
    case PK_CARD_HOLOGRAM_BLUE:
        UpdateHologramParticles(0.016f, rcBase);  // 假设60FPS
        break;
    case PK_CARD_NEON_PURPLE:
        UpdateNeonEffects(0.016f, rcBase);
        break;
    case PK_CARD_FLAME_GOLD:
        UpdateFlameParticles(0.016f, rcBase);
        break;
    case PK_CARD_RAINBOW_MAGIC:
        UpdateRainbowEffects(0.016f, rcBase);
        break;
    case PK_CARD_ICE_CRYSTAL:
        UpdateIceParticles(0.016f, rcBase);
        break;
    case PK_CARD_LEGEND_DIVINE:
        UpdateDivineEffects(0.016f, rcBase);
        break;
    }
    
    // 渲染粒子 (简化版本)
    // 在实际实现中，这里应该使用D3D或其他图形API来渲染粒子
}

void CPKEffectRenderer::UpdateHologramParticles(float fElapsedTime, const UIRECT& rcBase)
{
    // 全息蓝光效果 - 生成蓝色流动粒子
    if (RandomFloat(0.0f, 1.0f) < 0.3f)  // 30%概率生成新粒子
    {
        float fX = rcBase.left + RandomFloat(0.0f, rcBase.sizeX);
        float fY = rcBase.top + rcBase.sizeY;  // 从底部生成
        AddParticle(fX, fY, RandomFloat(-10.0f, 10.0f), RandomFloat(-50.0f, -20.0f), 
                   0xFF00AAFF, RandomFloat(1.0f, 3.0f), RandomFloat(2.0f, 5.0f));
    }
}

void CPKEffectRenderer::UpdateNeonEffects(float fElapsedTime, const UIRECT& rcBase)
{
    // 霓虹紫光效果 - 边框闪烁粒子
    if (RandomFloat(0.0f, 1.0f) < 0.4f)
    {
        // 在边框上生成粒子
        float fX, fY;
        if (RandomFloat(0.0f, 1.0f) < 0.5f)
        {
            // 左右边框
            fX = (RandomFloat(0.0f, 1.0f) < 0.5f) ? rcBase.left : rcBase.right;
            fY = rcBase.top + RandomFloat(0.0f, rcBase.sizeY);
        }
        else
        {
            // 上下边框
            fX = rcBase.left + RandomFloat(0.0f, rcBase.sizeX);
            fY = (RandomFloat(0.0f, 1.0f) < 0.5f) ? rcBase.top : rcBase.bottom;
        }
        AddParticle(fX, fY, RandomFloat(-5.0f, 5.0f), RandomFloat(-5.0f, 5.0f),
                   0xFFAA00FF, RandomFloat(0.5f, 2.0f), RandomFloat(3.0f, 6.0f));
    }
}

void CPKEffectRenderer::UpdateFlameParticles(float fElapsedTime, const UIRECT& rcBase)
{
    // 烈焰金光效果 - 火焰粒子向上飘动
    if (RandomFloat(0.0f, 1.0f) < 0.5f)
    {
        float fX = rcBase.left + RandomFloat(0.0f, rcBase.sizeX);
        float fY = rcBase.bottom;
        DWORD dwColor = (RandomFloat(0.0f, 1.0f) < 0.7f) ? 0xFFFFAA00 : 0xFFFF6600;  // 金色或橙色
        AddParticle(fX, fY, RandomFloat(-15.0f, 15.0f), RandomFloat(-60.0f, -30.0f),
                   dwColor, RandomFloat(1.0f, 2.5f), RandomFloat(3.0f, 8.0f));
    }
}

void CPKEffectRenderer::UpdateRainbowEffects(float fElapsedTime, const UIRECT& rcBase)
{
    // 彩虹幻光效果 - 彩色星光粒子
    if (RandomFloat(0.0f, 1.0f) < 0.4f)
    {
        float fX = rcBase.left + RandomFloat(0.0f, rcBase.sizeX);
        float fY = rcBase.top + RandomFloat(0.0f, rcBase.sizeY);
        DWORD dwColor = GetRainbowColor(RandomFloat(0.0f, 6.28f));
        AddParticle(fX, fY, RandomFloat(-20.0f, 20.0f), RandomFloat(-20.0f, 20.0f),
                   dwColor, RandomFloat(2.0f, 4.0f), RandomFloat(2.0f, 6.0f));
    }
}

void CPKEffectRenderer::UpdateIceParticles(float fElapsedTime, const UIRECT& rcBase)
{
    // 冰晶寒光效果 - 雪花粒子缓慢飘落
    if (RandomFloat(0.0f, 1.0f) < 0.25f)
    {
        float fX = rcBase.left + RandomFloat(-10.0f, rcBase.sizeX + 10.0f);
        float fY = rcBase.top - 5.0f;
        AddParticle(fX, fY, RandomFloat(-5.0f, 5.0f), RandomFloat(10.0f, 30.0f),
                   0xFF00FFFF, RandomFloat(3.0f, 6.0f), RandomFloat(2.0f, 4.0f));
    }
}

void CPKEffectRenderer::UpdateDivineEffects(float fElapsedTime, const UIRECT& rcBase)
{
    // 传说神光效果 - 神圣光环粒子
    if (RandomFloat(0.0f, 1.0f) < 0.3f)
    {
        float fCenterX = rcBase.left + rcBase.sizeX * 0.5f;
        float fCenterY = rcBase.top + rcBase.sizeY * 0.5f;
        float fAngle = RandomFloat(0.0f, 6.28f);
        float fRadius = RandomFloat(20.0f, 40.0f);
        float fX = fCenterX + cosf(fAngle) * fRadius;
        float fY = fCenterY + sinf(fAngle) * fRadius;
        AddParticle(fX, fY, cosf(fAngle) * 10.0f, sinf(fAngle) * 10.0f,
                   0xFFFFFFAA, RandomFloat(2.0f, 4.0f), RandomFloat(4.0f, 8.0f));
    }
}

void CPKEffectRenderer::AddParticle(float fX, float fY, float fVelX, float fVelY, DWORD dwColor, float fLife, float fSize)
{
    if (m_vecParticles.size() >= 100) return;  // 限制粒子数量
    
    SParticleData particle;
    particle.fPosX = fX;
    particle.fPosY = fY;
    particle.fVelX = fVelX;
    particle.fVelY = fVelY;
    particle.dwColor = dwColor;
    particle.fLife = fLife;
    particle.fSize = fSize;
    
    m_vecParticles.push_back(particle);
}

void CPKEffectRenderer::UpdateParticles(float fElapsedTime)
{
    auto it = m_vecParticles.begin();
    while (it != m_vecParticles.end())
    {
        it->fPosX += it->fVelX * fElapsedTime;
        it->fPosY += it->fVelY * fElapsedTime;
        it->fLife -= fElapsedTime;
        
        if (it->fLife <= 0.0f)
        {
            it = m_vecParticles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

DWORD CPKEffectRenderer::GetRainbowColor(float fPhase)
{
    // 简单的HSV到RGB转换，创建彩虹效果
    float fHue = fPhase;
    float fSat = 1.0f;
    float fVal = 1.0f;
    
    float fC = fVal * fSat;
    float fX = fC * (1.0f - fabsf(fmodf(fHue / 1.047f, 2.0f) - 1.0f));  // 1.047 ≈ π/3
    float fM = fVal - fC;
    
    float fR, fG, fB;
    if (fHue < 1.047f) { fR = fC; fG = fX; fB = 0; }
    else if (fHue < 2.094f) { fR = fX; fG = fC; fB = 0; }
    else if (fHue < 3.141f) { fR = 0; fG = fC; fB = fX; }
    else if (fHue < 4.188f) { fR = 0; fG = fX; fB = fC; }
    else if (fHue < 5.235f) { fR = fX; fG = 0; fB = fC; }
    else { fR = fC; fG = 0; fB = fX; }
    
    BYTE bR = (BYTE)((fR + fM) * 255);
    BYTE bG = (BYTE)((fG + fM) * 255);
    BYTE bB = (BYTE)((fB + fM) * 255);
    
    return 0xFF000000 | (bR << 16) | (bG << 8) | bB;
}

float CPKEffectRenderer::RandomFloat(float fMin, float fMax)
{
    return fMin + (float)rand() / RAND_MAX * (fMax - fMin);
}