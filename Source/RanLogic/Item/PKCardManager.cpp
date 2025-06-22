#include "StdAfx.h"
#include "PKCardManager.h"
#include "../../SigmaCore/Util/SystemInfo.h"

CPKCardManager::CPKCardManager()
    : m_emCurrentActiveCard(PK_CARD_NONE)
{
    InitializeCards();
}

CPKCardManager::~CPKCardManager()
{
}

void CPKCardManager::InitializeCards()
{
    // 初始化所有卡片类型
    for (int i = PK_CARD_HOLOGRAM_BLUE; i < PK_CARD_NONE; ++i)
    {
        EMPK_EFFECT_CARD_TYPE emType = static_cast<EMPK_EFFECT_CARD_TYPE>(i);
        SPK_EFFECT_CARD card;
        card.emType = emType;
        card.bActive = FALSE;
        card.dwDuration = SPK_EFFECT_CARD::GetDefaultDuration(emType);
        m_mapPKCards[emType] = card;
    }
}

BOOL CPKCardManager::UsePKEffectCard(EMPK_EFFECT_CARD_TYPE emType, DWORD dwDays)
{
    if (emType >= PK_CARD_NONE) return FALSE;
    
    auto it = m_mapPKCards.find(emType);
    if (it == m_mapPKCards.end()) return FALSE;
    
    // 停用当前激活的卡片
    if (m_emCurrentActiveCard != PK_CARD_NONE)
    {
        DeactivateCard(m_emCurrentActiveCard);
    }
    
    // 激活新卡片
    SPK_EFFECT_CARD& card = it->second;
    card.tStartTime = CTime::GetCurrentTime();
    card.dwDuration = (dwDays > 0) ? dwDays : SPK_EFFECT_CARD::GetDefaultDuration(emType);
    card.bActive = TRUE;
    
    m_emCurrentActiveCard = emType;
    
    return TRUE;
}

EMPK_EFFECT_CARD_TYPE CPKCardManager::GetActivePKCard()
{
    // 检查当前激活卡片是否过期
    if (m_emCurrentActiveCard != PK_CARD_NONE)
    {
        auto it = m_mapPKCards.find(m_emCurrentActiveCard);
        if (it != m_mapPKCards.end() && it->second.IsExpired())
        {
            OnPKCardExpired(m_emCurrentActiveCard);
            return PK_CARD_NONE;
        }
    }
    
    return m_emCurrentActiveCard;
}

void CPKCardManager::OnPKCardExpired(EMPK_EFFECT_CARD_TYPE emType)
{
    DeactivateCard(emType);
    
    if (m_emCurrentActiveCard == emType)
    {
        m_emCurrentActiveCard = PK_CARD_NONE;
    }
}

const SPK_EFFECT_CARD* CPKCardManager::GetPKCardInfo(EMPK_EFFECT_CARD_TYPE emType)
{
    auto it = m_mapPKCards.find(emType);
    if (it != m_mapPKCards.end())
    {
        return &(it->second);
    }
    return nullptr;
}

void CPKCardManager::UpdatePKCards()
{
    // 检查所有卡片是否过期
    for (auto& pair : m_mapPKCards)
    {
        if (pair.second.bActive && pair.second.IsExpired())
        {
            OnPKCardExpired(pair.first);
        }
    }
}

void CPKCardManager::DeactivateCard(EMPK_EFFECT_CARD_TYPE emType)
{
    auto it = m_mapPKCards.find(emType);
    if (it != m_mapPKCards.end())
    {
        it->second.bActive = FALSE;
    }
}

void CPKCardManager::SaveToStream(sc::BaseStream& stream)
{
    stream << static_cast<DWORD>(m_emCurrentActiveCard);
    stream << static_cast<DWORD>(m_mapPKCards.size());
    
    for (const auto& pair : m_mapPKCards)
    {
        stream << static_cast<DWORD>(pair.first);
        stream << static_cast<DWORD>(pair.second.emType);
        stream << pair.second.tStartTime.GetTime();
        stream << pair.second.dwDuration;
        stream << pair.second.bActive;
    }
}

void CPKCardManager::LoadFromStream(sc::BaseStream& stream)
{
    DWORD dwActiveCard = 0;
    stream >> dwActiveCard;
    m_emCurrentActiveCard = static_cast<EMPK_EFFECT_CARD_TYPE>(dwActiveCard);
    
    DWORD dwSize = 0;
    stream >> dwSize;
    
    m_mapPKCards.clear();
    
    for (DWORD i = 0; i < dwSize; ++i)
    {
        DWORD dwCardType = 0;
        stream >> dwCardType;
        
        SPK_EFFECT_CARD card;
        DWORD dwType = 0;
        stream >> dwType;
        card.emType = static_cast<EMPK_EFFECT_CARD_TYPE>(dwType);
        
        __time64_t tTime = 0;
        stream >> tTime;
        card.tStartTime = CTime(tTime);
        
        stream >> card.dwDuration;
        stream >> card.bActive;
        
        m_mapPKCards[static_cast<EMPK_EFFECT_CARD_TYPE>(dwCardType)] = card;
    }
}