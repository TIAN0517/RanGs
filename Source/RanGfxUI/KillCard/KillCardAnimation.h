#pragma once

#include "../../../enginelib/GUInterface/UIGroup.h"
#include "KillCardManager.h"

class CBasicAnimationBox;

/**
 * 击杀卡片动画控制器
 * 基于现有 CBasicAnimationBox 框架，实现6种击杀卡片的动画效果
 */
class CKillCardAnimation : public CUIGroup
{
private:
    enum EANIMATION_STATE
    {
        ANIM_STATE_NONE = 0,
        ANIM_STATE_FADE_IN,
        ANIM_STATE_ACTIVE,
        ANIM_STATE_FADE_OUT
    };

public:
    CKillCardAnimation(EngineDeviceMan* pEngineDevice);
    virtual ~CKillCardAnimation();

public:
    void CreateSubControl();
    
    virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
    virtual void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);

public:
    // 动画控制接口
    void PlayKillCardAnimation(EKILL_CARD_TYPE cardType);
    void StopAnimation();
    void Reset();
    
    // 状态查询
    BOOL IsPlaying() const { return m_eAnimState != ANIM_STATE_NONE; }
    BOOL IsAnimationComplete() const;
    
    // 动画参数
    void SetAnimationSpeed(float fSpeed) { m_fAnimSpeed = fSpeed; }
    void SetDisplayTime(float fTime) { m_fDisplayTime = fTime; }
    void SetFadeTime(float fTime) { m_fFadeTime = fTime; }

private:
    // 内部动画方法
    void UpdateAnimation(float fElapsedTime);
    void SetupCardAnimation(EKILL_CARD_TYPE cardType);
    void UpdateFadeIn(float fElapsedTime);
    void UpdateActive(float fElapsedTime);
    void UpdateFadeOut(float fElapsedTime);

private:
    // 动画盒子 - 每种卡片一个
    CBasicAnimationBox* m_pCardAnimations[KILL_CARD_TYPE_SIZE];
    
    // 背景和装饰元素
    CBasicAnimationBox* m_pBackgroundEffect;
    CBasicAnimationBox* m_pParticleEffect;
    CBasicAnimationBox* m_pBorderEffect;
    
    // 动画状态
    EANIMATION_STATE m_eAnimState;
    EKILL_CARD_TYPE m_eCurrentCard;
    
    // 时间控制
    float m_fCurrentTime;
    float m_fDisplayTime;
    float m_fFadeTime;
    float m_fAnimSpeed;
    
    // 透明度控制
    WORD m_wCurrentAlpha;
    WORD m_wTargetAlpha;
};