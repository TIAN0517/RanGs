#include "../../stdafx.h"
#include "KillCardAnimation.h"
#include "../../../enginelib/GUInterface/BasicAnimationBox.h"

#include "../../../SigmaCore/DebugInclude.h"

CKillCardAnimation::CKillCardAnimation(EngineDeviceMan* pEngineDevice)
    : CUIGroup(pEngineDevice)
    , m_pBackgroundEffect(NULL)
    , m_pParticleEffect(NULL)
    , m_pBorderEffect(NULL)
    , m_eAnimState(ANIM_STATE_NONE)
    , m_eCurrentCard(KILL_CARD_TYPE_SIZE)
    , m_fCurrentTime(0.0f)
    , m_fDisplayTime(3.0f)
    , m_fFadeTime(0.5f)
    , m_fAnimSpeed(1.0f)
    , m_wCurrentAlpha(0)
    , m_wTargetAlpha(255)
{
    // 初始化卡片动画数组
    for (int i = 0; i < KILL_CARD_TYPE_SIZE; i++)
    {
        m_pCardAnimations[i] = NULL;
    }
}

CKillCardAnimation::~CKillCardAnimation()
{
    // 子控件会由基类自动删除
}

void CKillCardAnimation::CreateSubControl()
{
    // 创建背景效果动画
    {
        m_pBackgroundEffect = new CBasicAnimationBox(m_pEngineDevice);
        m_pBackgroundEffect->CreateSubEx(this, "KILLCARD_BACKGROUND");
        m_pBackgroundEffect->CreateAni("KILLCARD_BG_00", CBasicAnimationBox::NORMAL_ANI);
        m_pBackgroundEffect->AddAni("KILLCARD_BG_01");
        m_pBackgroundEffect->AddAni("KILLCARD_BG_02");
        m_pBackgroundEffect->AddAni("KILLCARD_BG_03");
        m_pBackgroundEffect->SetAniTime(0.1f);
        m_pBackgroundEffect->SetEndTime(m_fDisplayTime);
        m_pBackgroundEffect->SetLoop(TRUE);
        m_pBackgroundEffect->SetTransparentOption(TRUE);
        m_pBackgroundEffect->SetVisibleSingle(FALSE);
        RegisterControl(m_pBackgroundEffect);
    }

    // 创建粒子效果动画
    {
        m_pParticleEffect = new CBasicAnimationBox(m_pEngineDevice);
        m_pParticleEffect->CreateSubEx(this, "KILLCARD_PARTICLES");
        m_pParticleEffect->CreateAni("KILLCARD_PARTICLE_00", CBasicAnimationBox::NORMAL_ANI);
        m_pParticleEffect->AddAni("KILLCARD_PARTICLE_01");
        m_pParticleEffect->AddAni("KILLCARD_PARTICLE_02");
        m_pParticleEffect->AddAni("KILLCARD_PARTICLE_03");
        m_pParticleEffect->AddAni("KILLCARD_PARTICLE_04");
        m_pParticleEffect->SetAniTime(0.05f);
        m_pParticleEffect->SetEndTime(m_fDisplayTime);
        m_pParticleEffect->SetLoop(TRUE);
        m_pParticleEffect->SetTransparentOption(TRUE);
        m_pParticleEffect->SetVisibleSingle(FALSE);
        RegisterControl(m_pParticleEffect);
    }

    // 创建边框效果动画
    {
        m_pBorderEffect = new CBasicAnimationBox(m_pEngineDevice);
        m_pBorderEffect->CreateSubEx(this, "KILLCARD_BORDER");
        m_pBorderEffect->CreateAni("KILLCARD_BORDER_00", CBasicAnimationBox::NORMAL_ANI);
        m_pBorderEffect->AddAni("KILLCARD_BORDER_01");
        m_pBorderEffect->AddAni("KILLCARD_BORDER_02");
        m_pBorderEffect->SetAniTime(0.2f);
        m_pBorderEffect->SetEndTime(m_fDisplayTime);
        m_pBorderEffect->SetLoop(TRUE);
        m_pBorderEffect->SetTransparentOption(TRUE);
        m_pBorderEffect->SetVisibleSingle(FALSE);
        RegisterControl(m_pBorderEffect);
    }

    // 创建各种卡片类型的动画
    const char* cardAnimNames[KILL_CARD_TYPE_SIZE] = {
        "KILLCARD_QUANTUM",      // 量子裂变卡
        "KILLCARD_HOLOGRAM",     // 全息干扰卡
        "KILLCARD_SPACETIME",    // 时空扭曲卡
        "KILLCARD_PLASMA",       // 等离子爆破卡
        "KILLCARD_NANO",         // 纳米溶解卡
        "KILLCARD_VIRTUAL"       // 虚拟重构卡
    };

    for (int i = 0; i < KILL_CARD_TYPE_SIZE; i++)
    {
        m_pCardAnimations[i] = new CBasicAnimationBox(m_pEngineDevice);
        m_pCardAnimations[i]->CreateSubEx(this, cardAnimNames[i]);
        
        // 设置每种卡片的动画帧
        char frameName[64];
        sprintf_s(frameName, "%s_00", cardAnimNames[i]);
        m_pCardAnimations[i]->CreateAni(frameName, CBasicAnimationBox::NORMAL_ANI);
        
        // 添加更多动画帧
        for (int frame = 1; frame <= 8; frame++)
        {
            sprintf_s(frameName, "%s_%02d", cardAnimNames[i], frame);
            m_pCardAnimations[i]->AddAni(frameName);
        }
        
        m_pCardAnimations[i]->SetAniTime(0.1f);
        m_pCardAnimations[i]->SetEndTime(m_fDisplayTime);
        m_pCardAnimations[i]->SetLoop(FALSE); // 卡片动画只播放一次
        m_pCardAnimations[i]->SetTransparentOption(TRUE);
        m_pCardAnimations[i]->SetVisibleSingle(FALSE);
        RegisterControl(m_pCardAnimations[i]);
    }
}

void CKillCardAnimation::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
    CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);
    
    UpdateAnimation(fElapsedTime);
}

void CKillCardAnimation::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
    CUIGroup::TranslateUIMessage(ControlID, dwMsg);
}

void CKillCardAnimation::PlayKillCardAnimation(EKILL_CARD_TYPE cardType)
{
    if (cardType >= KILL_CARD_TYPE_SIZE)
        return;

    // 停止当前动画
    StopAnimation();

    // 设置新动画
    SetupCardAnimation(cardType);
    
    m_eCurrentCard = cardType;
    m_eAnimState = ANIM_STATE_FADE_IN;
    m_fCurrentTime = 0.0f;
    m_wCurrentAlpha = 0;
    m_wTargetAlpha = 255;
}

void CKillCardAnimation::StopAnimation()
{
    if (m_eAnimState != ANIM_STATE_NONE)
    {
        m_eAnimState = ANIM_STATE_FADE_OUT;
        m_wTargetAlpha = 0;
    }
}

void CKillCardAnimation::Reset()
{
    m_eAnimState = ANIM_STATE_NONE;
    m_eCurrentCard = KILL_CARD_TYPE_SIZE;
    m_fCurrentTime = 0.0f;
    m_wCurrentAlpha = 0;
    m_wTargetAlpha = 255;

    // 隐藏所有动画元素
    if (m_pBackgroundEffect)
        m_pBackgroundEffect->SetVisibleSingle(FALSE);
    
    if (m_pParticleEffect)
        m_pParticleEffect->SetVisibleSingle(FALSE);
    
    if (m_pBorderEffect)
        m_pBorderEffect->SetVisibleSingle(FALSE);

    for (int i = 0; i < KILL_CARD_TYPE_SIZE; i++)
    {
        if (m_pCardAnimations[i])
            m_pCardAnimations[i]->SetVisibleSingle(FALSE);
    }
}

BOOL CKillCardAnimation::IsAnimationComplete() const
{
    return (m_eAnimState == ANIM_STATE_NONE) || 
           (m_eAnimState == ANIM_STATE_FADE_OUT && m_wCurrentAlpha == 0);
}

void CKillCardAnimation::UpdateAnimation(float fElapsedTime)
{
    if (m_eAnimState == ANIM_STATE_NONE)
        return;

    m_fCurrentTime += fElapsedTime * m_fAnimSpeed;

    switch (m_eAnimState)
    {
    case ANIM_STATE_FADE_IN:
        UpdateFadeIn(fElapsedTime);
        break;
        
    case ANIM_STATE_ACTIVE:
        UpdateActive(fElapsedTime);
        break;
        
    case ANIM_STATE_FADE_OUT:
        UpdateFadeOut(fElapsedTime);
        break;
    }
}

void CKillCardAnimation::SetupCardAnimation(EKILL_CARD_TYPE cardType)
{
    if (cardType >= KILL_CARD_TYPE_SIZE)
        return;

    // 隐藏所有卡片
    for (int i = 0; i < KILL_CARD_TYPE_SIZE; i++)
    {
        if (m_pCardAnimations[i])
            m_pCardAnimations[i]->SetVisibleSingle(FALSE);
    }

    // 显示目标卡片
    if (m_pCardAnimations[cardType])
    {
        m_pCardAnimations[cardType]->SetVisibleSingle(TRUE);
        m_pCardAnimations[cardType]->InitAnimateFrame();
        m_pCardAnimations[cardType]->SetPause(FALSE);
    }

    // 启动背景效果
    if (m_pBackgroundEffect)
    {
        m_pBackgroundEffect->SetVisibleSingle(TRUE);
        m_pBackgroundEffect->InitAnimateFrame();
        m_pBackgroundEffect->SetPause(FALSE);
    }

    // 启动粒子效果
    if (m_pParticleEffect)
    {
        m_pParticleEffect->SetVisibleSingle(TRUE);
        m_pParticleEffect->InitAnimateFrame();
        m_pParticleEffect->SetPause(FALSE);
    }

    // 启动边框效果
    if (m_pBorderEffect)
    {
        m_pBorderEffect->SetVisibleSingle(TRUE);
        m_pBorderEffect->InitAnimateFrame();
        m_pBorderEffect->SetPause(FALSE);
    }
}

void CKillCardAnimation::UpdateFadeIn(float fElapsedTime)
{
    float fadeProgress = m_fCurrentTime / m_fFadeTime;
    
    if (fadeProgress >= 1.0f)
    {
        fadeProgress = 1.0f;
        m_eAnimState = ANIM_STATE_ACTIVE;
        m_fCurrentTime = 0.0f;
    }

    m_wCurrentAlpha = (WORD)(fadeProgress * m_wTargetAlpha);

    // 应用透明度到所有动画元素
    if (m_pBackgroundEffect)
        m_pBackgroundEffect->SetDiffuseAlpha(m_wCurrentAlpha);
    
    if (m_pParticleEffect)
        m_pParticleEffect->SetDiffuseAlpha(m_wCurrentAlpha);
    
    if (m_pBorderEffect)
        m_pBorderEffect->SetDiffuseAlpha(m_wCurrentAlpha);

    if (m_eCurrentCard < KILL_CARD_TYPE_SIZE && m_pCardAnimations[m_eCurrentCard])
        m_pCardAnimations[m_eCurrentCard]->SetDiffuseAlpha(m_wCurrentAlpha);
}

void CKillCardAnimation::UpdateActive(float fElapsedTime)
{
    if (m_fCurrentTime >= m_fDisplayTime - m_fFadeTime)
    {
        m_eAnimState = ANIM_STATE_FADE_OUT;
        m_fCurrentTime = 0.0f;
        m_wTargetAlpha = 0;
    }
}

void CKillCardAnimation::UpdateFadeOut(float fElapsedTime)
{
    float fadeProgress = m_fCurrentTime / m_fFadeTime;
    
    if (fadeProgress >= 1.0f)
    {
        fadeProgress = 1.0f;
        m_eAnimState = ANIM_STATE_NONE;
        Reset();
        return;
    }

    m_wCurrentAlpha = (WORD)((1.0f - fadeProgress) * 255);

    // 应用透明度到所有动画元素
    if (m_pBackgroundEffect)
        m_pBackgroundEffect->SetDiffuseAlpha(m_wCurrentAlpha);
    
    if (m_pParticleEffect)
        m_pParticleEffect->SetDiffuseAlpha(m_wCurrentAlpha);
    
    if (m_pBorderEffect)
        m_pBorderEffect->SetDiffuseAlpha(m_wCurrentAlpha);

    if (m_eCurrentCard < KILL_CARD_TYPE_SIZE && m_pCardAnimations[m_eCurrentCard])
        m_pCardAnimations[m_eCurrentCard]->SetDiffuseAlpha(m_wCurrentAlpha);
}