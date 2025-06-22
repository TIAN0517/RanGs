#pragma once

#include "PKEffectCards.h"
#include "../../SigmaCore/File/BaseStream.h"
#include <map>

namespace sc
{
    class BaseStream;
}

// PK卡片管理器
class CPKCardManager : public CPKCardManagerBase
{
protected:
    std::map<EMPK_EFFECT_CARD_TYPE, SPK_EFFECT_CARD> m_mapPKCards;
    EMPK_EFFECT_CARD_TYPE m_emCurrentActiveCard;

public:
    CPKCardManager();
    virtual ~CPKCardManager();

    // 使用卡片
    virtual BOOL UsePKEffectCard(EMPK_EFFECT_CARD_TYPE emType, DWORD dwDays = 0) override;
    
    // 检查卡片状态
    virtual EMPK_EFFECT_CARD_TYPE GetActivePKCard() override;
    
    // 卡片过期处理
    virtual void OnPKCardExpired(EMPK_EFFECT_CARD_TYPE emType) override;
    
    // 获取卡片信息
    virtual const SPK_EFFECT_CARD* GetPKCardInfo(EMPK_EFFECT_CARD_TYPE emType) override;
    
    // 更新卡片状态
    virtual void UpdatePKCards() override;
    
    // 序列化
    void SaveToStream(sc::BaseStream& stream);
    void LoadFromStream(sc::BaseStream& stream);

private:
    void InitializeCards();
    void DeactivateCard(EMPK_EFFECT_CARD_TYPE emType);
};