#pragma once

#include "../../../enginelib/GUInterface/UIGroup.h"
#include "../../RanLogic/Item/GLItemDef.h"

class CKillCardAnimation;
class CKillCardRenderer;
class CKillCardEffects;
class GLGaeaClient;

// 击杀卡片类型枚举
enum EKILL_CARD_TYPE
{
    KILL_CARD_QUANTUM_FISSION = 0,    // 量子裂变卡 (暴击击杀)
    KILL_CARD_HOLOGRAM_DISRUPTION,    // 全息干扰卡 (技能击杀)
    KILL_CARD_SPACETIME_DISTORTION,   // 时空扭曲卡 (连续击杀 2+)
    KILL_CARD_PLASMA_EXPLOSION,       // 等离子爆破卡 (远程击杀)
    KILL_CARD_NANO_DISSOLUTION,       // 纳米溶解卡 (近战击杀)
    KILL_CARD_VIRTUAL_RECONSTRUCTION, // 虚拟重构卡 (爆头击杀)
    
    KILL_CARD_TYPE_SIZE
};

// 击杀类型条件
enum EKILL_TYPE
{
    KILL_TYPE_CRITICAL = 0x01,    // 暴击
    KILL_TYPE_SKILL = 0x02,       // 技能
    KILL_TYPE_CONSECUTIVE = 0x04,  // 连续击杀
    KILL_TYPE_RANGED = 0x08,      // 远程
    KILL_TYPE_MELEE = 0x10,       // 近战
    KILL_TYPE_HEADSHOT = 0x20     // 爆头
};

// 科技显示卡常量定义
namespace TECH_DISPLAY_CARD
{
    const SNATIVEID CARD_ID(9999, 0);  // 科技显示卡ID (MID=9999, SID=0)
    const char* CARD_NAME = "科技显示卡"; // 卡片名称
}

/**
 * 击杀卡片管理器
 * 负责管理和触发6种科技感击杀动画效果
 */
class CKillCardManager : public CUIGroup
{
private:
    static const float CARD_DISPLAY_TIME;    // 卡片显示时间
    static const float FADE_TIME;            // 淡入淡出时间
    static const int MAX_CONSECUTIVE_KILLS;  // 最大连续击杀计数

public:
    CKillCardManager(EngineDeviceMan* pEngineDevice);
    virtual ~CKillCardManager();
    
    // 设置GLGaeaClient引用用于访问玩家数据
    void SetGaeaClient(GLGaeaClient* pGaeaClient) { m_pGaeaClient = pGaeaClient; }

public:
    void CreateSubControl();
    
    virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
    virtual void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
    virtual void SetVisibleSingle(BOOL bVisible);

public:
    // 主要接口
    void TriggerKillCard(EKILL_TYPE killType, DWORD targetID = 0);
    void Reset();
    
    // 配置接口
    void SetEnabled(BOOL bEnabled) { m_bEnabled = bEnabled; }
    BOOL IsEnabled() const { return m_bEnabled; }
    
    void SetEffectIntensity(float fIntensity) { m_fEffectIntensity = fIntensity; }
    float GetEffectIntensity() const { return m_fEffectIntensity; }

private:
    // 内部方法
    EKILL_CARD_TYPE DetermineCardType(EKILL_TYPE killType);
    void ShowKillCard(EKILL_CARD_TYPE cardType);
    void UpdateConsecutiveKills();
    void ResetConsecutiveKills();
    
    // 卡片激活检查方法
    BOOL CheckTechDisplayCard();
    void TriggerOriginalKillDisplay(EKILL_TYPE killType, DWORD targetID);
    
    // 平滑过渡方法 (0.3秒过渡，避免闪屏)
    void StartTransitionToOriginal(EKILL_TYPE killType, DWORD targetID);
    void StartTransitionToTechPanel(EKILL_TYPE killType, DWORD targetID);
    void UpdateTransition(float fElapsedTime);
    
    // 系统健康检查
    BOOL IsSystemHealthy() const;

private:
    // 动画和渲染组件
    CKillCardAnimation* m_pAnimation;
    CKillCardRenderer* m_pRenderer;
    CKillCardEffects* m_pEffects;
    
    // 状态变量
    BOOL m_bEnabled;
    BOOL m_bActive;
    float m_fCurrentTime;
    float m_fEffectIntensity;
    
    // 击杀统计
    int m_nConsecutiveKills;
    DWORD m_dwLastKillTime;
    
    // 当前显示的卡片
    EKILL_CARD_TYPE m_eCurrentCardType;
    
    // 性能统计
    DWORD m_dwFrameCount;
    float m_fPerformanceTimer;
    
    // 平滑过渡控制（避免闪屏和逻辑冲突）
    BOOL m_bInTransition;           // 是否处于过渡状态
    float m_fTransitionTime;        // 过渡时间计时器
    static const float TRANSITION_DURATION; // 过渡持续时间 (0.3秒)
    
    // GLGaeaClient引用用于访问玩家数据
    GLGaeaClient* m_pGaeaClient;
};