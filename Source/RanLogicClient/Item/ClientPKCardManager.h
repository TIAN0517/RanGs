#pragma once

#include "../../RanLogic/Item/PKCardManager.h"

class GLGaeaClient;

// 客户端PK卡片管理器
class CClientPKCardManager : public CPKCardManager
{
private:
    GLGaeaClient* m_pGaeaClient;

public:
    CClientPKCardManager(GLGaeaClient* pGaeaClient);
    virtual ~CClientPKCardManager();

    // 重写基类方法，添加客户端特定逻辑
    virtual BOOL UsePKEffectCard(EMPK_EFFECT_CARD_TYPE emType, DWORD dwDays = 0) override;
    virtual void OnPKCardExpired(EMPK_EFFECT_CARD_TYPE emType) override;
    
    // 客户端特定方法
    void NotifyServerCardUsage(EMPK_EFFECT_CARD_TYPE emType, DWORD dwDays);
    void UpdatePKNotificationEffect();
    
    // 从服务器同步卡片状态
    void SyncFromServer(const std::map<EMPK_EFFECT_CARD_TYPE, SPK_EFFECT_CARD>& serverCards);
};