#pragma once

#include "../../../EngineLib/GUInterface/UIGroup.h"
#include "../../../RanLogic/Item/PKEffectCards.h"

class EngineDeviceMan;

// PK特效渲染器
class CPKEffectRenderer
{
public:
    struct SParticleData
    {
        float fPosX, fPosY;
        float fVelX, fVelY;
        float fLife;
        DWORD dwColor;
        float fSize;
    };

private:
    EngineDeviceMan* m_pEngineDevice;
    std::vector<SParticleData> m_vecParticles;
    float m_fTime;
    EMPK_EFFECT_CARD_TYPE m_emCurrentType;

public:
    CPKEffectRenderer(EngineDeviceMan* pEngineDevice);
    virtual ~CPKEffectRenderer();

    void SetEffectType(EMPK_EFFECT_CARD_TYPE emType);
    void Update(float fElapsedTime);
    void Render(const UIRECT& rcBase);
    
    // 各种特效更新函数
    void UpdateHologramParticles(float fElapsedTime, const UIRECT& rcBase);     // 全息粒子
    void UpdateNeonEffects(float fElapsedTime, const UIRECT& rcBase);           // 霓虹特效
    void UpdateFlameParticles(float fElapsedTime, const UIRECT& rcBase);        // 火焰粒子
    void UpdateRainbowEffects(float fElapsedTime, const UIRECT& rcBase);        // 彩虹效果
    void UpdateIceParticles(float fElapsedTime, const UIRECT& rcBase);          // 冰晶粒子
    void UpdateDivineEffects(float fElapsedTime, const UIRECT& rcBase);         // 神圣特效

private:
    void AddParticle(float fX, float fY, float fVelX, float fVelY, DWORD dwColor, float fLife, float fSize);
    void UpdateParticles(float fElapsedTime);
    DWORD GetRainbowColor(float fPhase);
    float RandomFloat(float fMin, float fMax);
};