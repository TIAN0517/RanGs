#include "StdAfx.h"
#include "ClientPKCardManager.h"
#include "../GLGaeaClient.h"

CClientPKCardManager::CClientPKCardManager(GLGaeaClient* pGaeaClient)
    : CPKCardManager()
    , m_pGaeaClient(pGaeaClient)
{
}

CClientPKCardManager::~CClientPKCardManager()
{
}

BOOL CClientPKCardManager::UsePKEffectCard(EMPK_EFFECT_CARD_TYPE emType, DWORD dwDays)
{
    // 调用基类方法
    BOOL bResult = CPKCardManager::UsePKEffectCard(emType, dwDays);
    
    if (bResult)
    {
        // 通知服务器
        NotifyServerCardUsage(emType, dwDays);
        
        // 更新UI效果
        UpdatePKNotificationEffect();
    }
    
    return bResult;
}

void CClientPKCardManager::OnPKCardExpired(EMPK_EFFECT_CARD_TYPE emType)
{
    // 调用基类方法
    CPKCardManager::OnPKCardExpired(emType);
    
    // 更新UI效果
    UpdatePKNotificationEffect();
    
    // 可以在这里添加过期通知给玩家
}

void CClientPKCardManager::NotifyServerCardUsage(EMPK_EFFECT_CARD_TYPE emType, DWORD dwDays)
{
    // TODO: 发送消息给服务器，通知卡片使用
    // 这里需要根据现有的网络消息系统来实现
}

void CClientPKCardManager::UpdatePKNotificationEffect()
{
    // 更新PK通知UI的视觉效果
    // 这里可以通过GLGaeaClient获取UI接口并更新PK通知效果
    if (m_pGaeaClient)
    {
        // TODO: 通过UI接口更新PK通知的视觉效果
        // 例如：m_pGaeaClient->GetInterface()->SetPKNotificationEffect(m_emCurrentActiveCard);
    }
}

void CClientPKCardManager::SyncFromServer(const std::map<EMPK_EFFECT_CARD_TYPE, SPK_EFFECT_CARD>& serverCards)
{
    // 从服务器同步卡片状态
    for (const auto& pair : serverCards)
    {
        auto it = m_mapPKCards.find(pair.first);
        if (it != m_mapPKCards.end())
        {
            it->second = pair.second;
        }
    }
    
    // 更新当前激活的卡片
    EMPK_EFFECT_CARD_TYPE emActiveType = PK_CARD_NONE;
    for (const auto& pair : m_mapPKCards)
    {
        if (pair.second.bActive && !pair.second.IsExpired())
        {
            emActiveType = pair.first;
            break;
        }
    }
    
    m_emCurrentActiveCard = emActiveType;
    UpdatePKNotificationEffect();
}