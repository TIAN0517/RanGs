#include "../../stdafx.h"
#include "KillCardManager.h"
#include "KillCardAnimation.h"
#include "KillCardRenderer.h"
#include "KillCardEffects.h"
#include "../../RanLogicClient/GLGaeaClient.h"
#include "../../RanLogicClient/Char/GLCharacter.h"

#include "../../../SigmaCore/DebugInclude.h"
#include "../../../SigmaCore/DebugSet.h"

// 静态常量定义
const float CKillCardManager::CARD_DISPLAY_TIME = 3.0f;
const float CKillCardManager::FADE_TIME = 0.5f;
const int CKillCardManager::MAX_CONSECUTIVE_KILLS = 10;
const float CKillCardManager::TRANSITION_DURATION = 0.3f; // 平滑过渡时间

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
    , m_bInTransition(FALSE)
    , m_fTransitionTime(0.0f)
    , m_pGaeaClient(NULL)
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

    // 更新平滑过渡
    if (m_bInTransition)
    {
        UpdateTransition(fElapsedTime);
    }

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
        
        // 每秒进行系统健康检查
        if (!IsSystemHealthy())
        {
            #ifdef _DEBUG
            CDebugSet::ToView(0, 0, "[KillCard] 系统健康检查失败，重置状态");
            #endif
            Reset(); // 重置系统状态
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

    // 避免在过渡期间触发新的击杀显示，防止闪屏
    if (m_bInTransition)
        return;

    // 核心功能：检查玩家是否持有科技显示卡
    if (!CheckTechDisplayCard())
    {
        // 无卡片时，启动平滑过渡到原厂击杀显示
        StartTransitionToOriginal(killType, targetID);
        return;
    }

    // 有卡片时，启动平滑过渡到科技面板
    StartTransitionToTechPanel(killType, targetID);
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
    
    // 重置过渡状态
    m_bInTransition = FALSE;
    m_fTransitionTime = 0.0f;
    
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

// 卡片激活检查：检测玩家背包中是否存在科技显示卡
BOOL CKillCardManager::CheckTechDisplayCard()
{
    // 安全检查：确保GLGaeaClient引用有效
    if (!m_pGaeaClient)
    {
        #ifdef _DEBUG
        CDebugSet::ToView(0, 0, "[KillCard] GLGaeaClient引用无效，回退到原厂显示");
        #endif
        return FALSE;
    }
        
    // 获取当前角色
    GLCharacter* pCharacter = m_pGaeaClient->GetCharacter();
    if (!pCharacter)
    {
        #ifdef _DEBUG
        CDebugSet::ToView(0, 0, "[KillCard] 角色指针无效，回退到原厂显示");
        #endif
        return FALSE;
    }
    
    try
    {
        // 检查背包中是否存在科技显示卡 (ID=9999)
        // 使用FindItem方法查找指定ID的道具
        SINVENITEM* pCardItem = pCharacter->m_cInventory.FindItem(TECH_DISPLAY_CARD::CARD_ID);
        
        BOOL bHasCard = (pCardItem != NULL);
        
        #ifdef _DEBUG
        if (bHasCard)
        {
            CDebugSet::ToView(0, 0, "[KillCard] 检测到科技显示卡，激活4D击杀面板");
        }
        else
        {
            CDebugSet::ToView(0, 0, "[KillCard] 未检测到科技显示卡，使用原厂显示");
        }
        #endif
        
        return bHasCard;
    }
    catch (...)
    {
        // 异常处理：如果背包检查出错，立即禁用插件并弹窗提示
        #ifdef _DEBUG
        CDebugSet::ToView(0, 0, "[KillCard] 背包检查异常，系统自动禁用");
        #endif
        
        // 禁用击杀卡片系统
        SetEnabled(FALSE);
        
        // 这里可以添加弹窗提示逻辑
        // 例如：弹出"请移除卡片以恢复默认"的提示
        
        return FALSE;
    }
}

// 原厂击杀显示：当无卡片时的回退机制
void CKillCardManager::TriggerOriginalKillDisplay(EKILL_TYPE killType, DWORD targetID)
{
    // 通过插件层实现隔离，不修改核心文件
    // 这里简单地忽略击杀显示，让游戏使用原厂UI
    // 在实际应用中，可以通过消息系统或其他机制触发原厂显示
    
    // 记录调试信息（仅在DEBUG模式下）
    #ifdef _DEBUG
    CDebugSet::ToView(0, 0, "[KillCard] 无科技显示卡，使用原厂击杀显示");
    #endif
    
    // 可以在这里添加其他原厂UI触发逻辑
    // 例如：触发原有的击杀提示系统
}

// 平滑过渡到原厂显示系统 (避免闪屏和逻辑冲突)
void CKillCardManager::StartTransitionToOriginal(EKILL_TYPE killType, DWORD targetID)
{
    m_bInTransition = TRUE;
    m_fTransitionTime = 0.0f;
    
    // 开始淡出当前科技面板 (如果有的话)
    if (m_bActive && m_pRenderer)
    {
        m_pRenderer->SetTransparency(1.0f); // 开始时完全不透明
    }
    
    #ifdef _DEBUG
    CDebugSet::ToView(0, 0, "[KillCard] 开始过渡到原厂显示 (0.3秒)");
    #endif
    
    // 存储参数以便过渡结束后使用
    // 在实际实现中可以存储这些参数
}

// 平滑过渡到科技面板系统
void CKillCardManager::StartTransitionToTechPanel(EKILL_TYPE killType, DWORD targetID)
{
    m_bInTransition = TRUE;
    m_fTransitionTime = 0.0f;
    
    // 更新连续击杀计数
    UpdateConsecutiveKills();

    // 确定卡片类型
    EKILL_CARD_TYPE cardType = DetermineCardType(killType);
    
    if (cardType < KILL_CARD_TYPE_SIZE)
    {
        // 开始淡入科技面板
        if (m_pRenderer)
        {
            m_pRenderer->SetTransparency(0.0f); // 开始时透明
        }
        
        ShowKillCard(cardType);
        
        #ifdef _DEBUG
        CDebugSet::ToView(0, 0, "[KillCard] 开始过渡到科技面板 (0.3秒)");
        #endif
    }
}

// 更新过渡动画
void CKillCardManager::UpdateTransition(float fElapsedTime)
{
    m_fTransitionTime += fElapsedTime;
    
    if (m_fTransitionTime >= TRANSITION_DURATION)
    {
        // 过渡完成
        m_bInTransition = FALSE;
        m_fTransitionTime = 0.0f;
        
        // 确保最终状态正确
        if (m_pRenderer)
        {
            m_pRenderer->SetTransparency(1.0f); // 完全不透明
        }
        
        #ifdef _DEBUG
        CDebugSet::ToView(0, 0, "[KillCard] 过渡完成");
        #endif
    }
    else
    {
        // 过渡进行中，计算当前透明度
        float fProgress = m_fTransitionTime / TRANSITION_DURATION;
        
        if (m_pRenderer)
        {
            // 使用缓动函数实现平滑过渡
            float fAlpha = fProgress; // 线性过渡，可以改为easeOutBack等
            m_pRenderer->SetTransparency(fAlpha);
        }
    }
}

// 系统健康检查：确保击杀卡片系统运行正常
BOOL CKillCardManager::IsSystemHealthy() const
{
    // 检查基本组件
    if (!m_pGaeaClient || !m_pAnimation || !m_pRenderer || !m_pEffects)
        return FALSE;
        
    // 检查效果强度是否在合理范围内
    if (m_fEffectIntensity < 0.1f || m_fEffectIntensity > 3.0f)
        return FALSE;
        
    // 检查是否有长时间的过渡状态（可能表示卡死）
    if (m_bInTransition && m_fTransitionTime > TRANSITION_DURATION * 2.0f)
        return FALSE;
        
    return TRUE;
}