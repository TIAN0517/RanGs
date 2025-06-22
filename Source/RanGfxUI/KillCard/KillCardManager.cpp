#include "../../stdafx.h"
#include "KillCardPlugin.h"
#include "KillCardPostProcess.h"
#include "KillCardPerformanceMonitor.h"
#include "KillCardAssetManager.h"

// 静态常量定义
const float CKillCardManager::CARD_DISPLAY_TIME = 3.0f;
const float CKillCardManager::FADE_TIME = 0.5f;
const int CKillCardManager::MAX_CONSECUTIVE_KILLS = 10;

CKillCardManager::CKillCardManager(EngineDeviceMan* pEngineDevice)
    : CUIGroup(pEngineDevice)
    , m_pAnimation(NULL)
    , m_pRenderer(NULL)
    , m_pEffects(NULL)
    , m_bEnabled(TRUE)
    , m_bActive(FALSE)
    , m_fCurrentTime(0.0f)
    , m_fEffectIntensity(1.0f)
    , m_nConsecutiveKills(0)
    , m_dwLastKillTime(0)
    , m_eCurrentCardType(KILL_CARD_TYPE_SIZE)
    , m_dwFrameCount(0)
    , m_fPerformanceTimer(0.0f)
{
}

CKillCardManager::~CKillCardManager()
{
    // 子控件会由基类自动删除
}

void CKillCardManager::CreateSubControl()
{
    // 创建动画控制器
    {
        m_pAnimation = new CKillCardAnimation(m_pEngineDevice);
        m_pAnimation->CreateSub(this, "KILLCARD_ANIMATION_AREA");
        m_pAnimation->CreateSubControl();
        m_pAnimation->SetVisibleSingle(FALSE);
        RegisterControl(m_pAnimation);
    }

    // 创建渲染器
    {
        m_pRenderer = new CKillCardRenderer(m_pEngineDevice);
        m_pRenderer->CreateSub(this, "KILLCARD_RENDERER_AREA");
        m_pRenderer->CreateSubControl();
        m_pRenderer->SetVisibleSingle(FALSE);
        RegisterControl(m_pRenderer);
    }

    // 创建效果系统
    {
        m_pEffects = new CKillCardEffects(m_pEngineDevice);
        m_pEffects->CreateSub(this, "KILLCARD_EFFECTS_AREA");
        m_pEffects->CreateSubControl();
        m_pEffects->SetVisibleSingle(FALSE);
        RegisterControl(m_pEffects);
    }
}

void CKillCardManager::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
    if (!m_bEnabled)
        return;

    CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

    // 性能监控
    m_dwFrameCount++;
    m_fPerformanceTimer += fElapsedTime;

    if (m_fPerformanceTimer >= 1.0f)
    {
        // 每秒检查一次性能
        if (m_dwFrameCount < 55) // 如果FPS低于55，减少效果强度
        {
            m_fEffectIntensity = max(0.5f, m_fEffectIntensity - 0.1f);
        }
        else if (m_dwFrameCount > 58) // 如果FPS高于58，可以恢复效果强度
        {
            m_fEffectIntensity = min(1.0f, m_fEffectIntensity + 0.05f);
        }

        m_dwFrameCount = 0;
        m_fPerformanceTimer = 0.0f;
    }

    // 更新活动状态
    if (m_bActive)
    {
        m_fCurrentTime += fElapsedTime;

        // 检查是否应该结束显示
        if (m_fCurrentTime >= CARD_DISPLAY_TIME)
        {
            if (m_pAnimation && m_pAnimation->IsPlaying())
            {
                m_pAnimation->StopAnimation();
            }
            
            m_bActive = FALSE;
            m_fCurrentTime = 0.0f;
            m_eCurrentCardType = KILL_CARD_TYPE_SIZE;
        }
    }

    // 更新连续击杀计时器
    DWORD dwCurrentTime = GetTickCount();
    if (m_nConsecutiveKills > 0 && (dwCurrentTime - m_dwLastKillTime) > 5000) // 5秒后重置连续击杀
    {
        ResetConsecutiveKills();
    }
}

void CKillCardManager::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
    CUIGroup::TranslateUIMessage(ControlID, dwMsg);
}

void CKillCardManager::SetVisibleSingle(BOOL bVisible)
{
    CUIGroup::SetVisibleSingle(bVisible);
}

void CKillCardManager::TriggerKillCard(EKILL_TYPE killType, DWORD targetID)
{
    if (!m_bEnabled)
        return;

    // 更新连续击杀计数
    UpdateConsecutiveKills();

    // 确定卡片类型
    EKILL_CARD_TYPE cardType = DetermineCardType(killType);
    
    if (cardType < KILL_CARD_TYPE_SIZE)
    {
        ShowKillCard(cardType);
    }
}

void CKillCardManager::Reset()
{
    if (m_pAnimation)
    {
        m_pAnimation->Reset();
    }

    if (m_pEffects)
    {
        m_pEffects->StopAllEffects();
    }

    m_bActive = FALSE;
    m_fCurrentTime = 0.0f;
    m_eCurrentCardType = KILL_CARD_TYPE_SIZE;
    ResetConsecutiveKills();
}

EKILL_CARD_TYPE CKillCardManager::DetermineCardType(EKILL_TYPE killType)
{
    // 按优先级确定卡片类型
    if (killType & KILL_TYPE_HEADSHOT)
        return KILL_CARD_VIRTUAL_RECONSTRUCTION;
    
    if (killType & KILL_TYPE_CRITICAL)
        return KILL_CARD_QUANTUM_FISSION;
    
    if (killType & KILL_TYPE_SKILL)
        return KILL_CARD_HOLOGRAM_DISRUPTION;
    
    if ((killType & KILL_TYPE_CONSECUTIVE) && m_nConsecutiveKills >= 2)
        return KILL_CARD_SPACETIME_DISTORTION;
    
    if (killType & KILL_TYPE_RANGED)
        return KILL_CARD_PLASMA_EXPLOSION;
    
    if (killType & KILL_TYPE_MELEE)
        return KILL_CARD_NANO_DISSOLUTION;

    // 默认返回量子裂变卡
    return KILL_CARD_QUANTUM_FISSION;
}

void CKillCardManager::ShowKillCard(EKILL_CARD_TYPE cardType)
{
    if (cardType >= KILL_CARD_TYPE_SIZE)
        return;

    // 停止当前动画
    if (m_bActive && m_pAnimation && m_pAnimation->IsPlaying())
    {
        m_pAnimation->StopAnimation();
    }

    // 设置新的卡片
    m_eCurrentCardType = cardType;
    m_bActive = TRUE;
    m_fCurrentTime = 0.0f;

    // 启动动画
    if (m_pAnimation)
    {
        m_pAnimation->SetVisibleSingle(TRUE);
        m_pAnimation->PlayKillCardAnimation(cardType);
    }

    // 启动渲染效果
    if (m_pRenderer)
    {
        m_pRenderer->SetVisibleSingle(TRUE);
        m_pRenderer->SetupCardRendering(cardType);
    }

    // 启动特效
    if (m_pEffects)
    {
        m_pEffects->SetVisibleSingle(TRUE);
        m_pEffects->TriggerEffect(cardType, m_fEffectIntensity);
    }
}

void CKillCardManager::UpdateConsecutiveKills()
{
    DWORD dwCurrentTime = GetTickCount();
    
    if ((dwCurrentTime - m_dwLastKillTime) <= 3000) // 3秒内的击杀算连续
    {
        m_nConsecutiveKills++;
        if (m_nConsecutiveKills > MAX_CONSECUTIVE_KILLS)
            m_nConsecutiveKills = MAX_CONSECUTIVE_KILLS;
    }
    else
    {
        m_nConsecutiveKills = 1;
    }
    
    m_dwLastKillTime = dwCurrentTime;
}

void CKillCardManager::ResetConsecutiveKills()
{
    m_nConsecutiveKills = 0;
    m_dwLastKillTime = 0;
}