#pragma once

#include "../../EngineLib/G-Logic/GLDefine.h"
#include "../Util/CTime.h"

// PK Effect Card Types
enum EMPK_EFFECT_CARD_TYPE
{
    PK_CARD_HOLOGRAM_BLUE = 0,      // 全息蓝光卡 (7天)
    PK_CARD_NEON_PURPLE = 1,        // 霓虹紫光卡 (15天)  
    PK_CARD_FLAME_GOLD = 2,         // 烈焰金光卡 (30天)
    PK_CARD_RAINBOW_MAGIC = 3,      // 彩虹幻光卡 (3天)
    PK_CARD_ICE_CRYSTAL = 4,        // 冰晶寒光卡 (10天)
    PK_CARD_LEGEND_DIVINE = 5,      // 传说神光卡 (60天)
    PK_CARD_NONE = 6
};

// PK Effect Card Data Structure
struct SPK_EFFECT_CARD
{
    EMPK_EFFECT_CARD_TYPE   emType;         // 卡片类型
    CTime                   tStartTime;     // 开始时间
    DWORD                   dwDuration;     // 持续天数
    BOOL                    bActive;        // 是否激活
    
    SPK_EFFECT_CARD()
        : emType(PK_CARD_NONE)
        , dwDuration(0)
        , bActive(FALSE)
    {
    }
    
    // 检查是否过期
    BOOL IsExpired() const
    {
        if (!bActive) return TRUE;
        
        CTime tCurrent = CTime::GetCurrentTime();
        CTimeSpan tSpan = tCurrent - tStartTime;
        return (tSpan.GetDays() >= (LONG)dwDuration);
    }
    
    // 获取剩余时间(秒)
    DWORD GetRemainingTime() const
    {
        if (!bActive) return 0;
        
        CTime tCurrent = CTime::GetCurrentTime();
        CTimeSpan tSpan = tCurrent - tStartTime;
        LONG lDaysUsed = tSpan.GetDays();
        
        if (lDaysUsed >= (LONG)dwDuration) return 0;
        
        LONG lDaysLeft = (LONG)dwDuration - lDaysUsed;
        return (DWORD)(lDaysLeft * 24 * 60 * 60 - tSpan.GetHours() * 60 * 60 - tSpan.GetMinutes() * 60 - tSpan.GetSeconds());
    }
    
    // 获取默认持续时间
    static DWORD GetDefaultDuration(EMPK_EFFECT_CARD_TYPE emType)
    {
        switch (emType)
        {
        case PK_CARD_HOLOGRAM_BLUE:  return 7;   // 7天
        case PK_CARD_NEON_PURPLE:    return 15;  // 15天
        case PK_CARD_FLAME_GOLD:     return 30;  // 30天
        case PK_CARD_RAINBOW_MAGIC:  return 3;   // 3天
        case PK_CARD_ICE_CRYSTAL:    return 10;  // 10天
        case PK_CARD_LEGEND_DIVINE:  return 60;  // 60天
        default:                     return 0;
        }
    }
};

// PK卡片管理器基类
class CPKCardManagerBase
{
public:
    CPKCardManagerBase() {}
    virtual ~CPKCardManagerBase() {}
    
    // 使用卡片
    virtual BOOL UsePKEffectCard(EMPK_EFFECT_CARD_TYPE emType, DWORD dwDays = 0) = 0;
    
    // 检查卡片状态
    virtual EMPK_EFFECT_CARD_TYPE GetActivePKCard() = 0;
    
    // 卡片过期处理
    virtual void OnPKCardExpired(EMPK_EFFECT_CARD_TYPE emType) = 0;
    
    // 获取卡片信息
    virtual const SPK_EFFECT_CARD* GetPKCardInfo(EMPK_EFFECT_CARD_TYPE emType) = 0;
    
    // 更新卡片状态
    virtual void UpdatePKCards() = 0;
};