#include "StdAfx.h"
#include "KillAnimationSystem.h"

#include "../../../EngineLib/DxTools/DxFontMan.h"
#include "../../../EngineLib/GUInterface/UIControl.h"
#include "../../../EngineLib/GUInterface/BasicTextBox.h"
#include "../../../EngineLib/GUInterface/SkillImage.h"
#include "../../../EngineLib/GUInterface/UITextControl.h"
#include "../../../EngineLib/GUInterface/GameTextControl.h"
#include "../../../RanLogicClient/GLGaeaClient.h"
#include "../../../RanLogic/GLogicDataMan.h"

// ----------------------------------------------------------------------------
#include "../../../SigmaCore/DebugInclude.h"
// ----------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------//
// Kill Animation System Implementation - Jy技術團隊
// 開發者：TIAN0517 - Jy技術團隊
//-----------------------------------------------------------------------------------------------//

CKillAnimationSystem::ANIMOBJECT::~ANIMOBJECT()
{
    SAFE_DELETE(pIMAGE);
    SAFE_DELETE(pIMAGE_EFFECT);
}

void CKillAnimationSystem::ANIMOBJECT::CreateSubControl()
{
    pIMAGE = new CSkillImage(pEngineDevice);
    pIMAGE->CreateSub(this, "KILL_ANIMATION_IMAGE", UI_FLAG_DEFAULT);
    pIMAGE->CreateSubControl();
    RegisterControl(pIMAGE);

    pIMAGE_EFFECT = new CSkillImage(pEngineDevice);
    pIMAGE_EFFECT->CreateSub(this, "KILL_ANIMATION_EFFECT", UI_FLAG_DEFAULT);
    pIMAGE_EFFECT->CreateSubControl();
    RegisterControl(pIMAGE_EFFECT);
}

void CKillAnimationSystem::ANIMOBJECT::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
    CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

    if (bENABLE)
    {
        fANITIME -= fElapsedTime;
        fANITIMEEFFECT -= fElapsedTime;

        if (fANITIME <= 0.0f)
        {
            bENABLE = false;
            SetVisibleSingle(FALSE);
        }
    }
}

void CKillAnimationSystem::ANIMOBJECT::ResetAnimation()
{
    bENABLE = false;
    fANITIME = 0.0f;
    fANITIMEEFFECT = 0.0f;
    SetVisibleSingle(FALSE);
}

CKillAnimationSystem::CKillAnimationSystem(GLGaeaClient* pGaeaClient, CInnerInterface* pInterface, EngineDeviceMan* pEngineDevice)
    : CUIGroup(pEngineDevice)
    , m_pGaeaClient(pGaeaClient)
    , m_pInterface(pInterface)
    , m_bShow(false)
{
}

CKillAnimationSystem::~CKillAnimationSystem()
{
    LIST_ANIMOBJ_ITER iter = m_listAnimObj.begin();
    LIST_ANIMOBJ_ITER iter_end = m_listAnimObj.end();
    for (; iter != iter_end; ++iter)
    {
        SAFE_DELETE(*iter);
    }
    m_listAnimObj.clear();
}

void CKillAnimationSystem::CreateSubControl()
{
    // Create animation objects for kill effects
    for (int i = 0; i < 6; ++i) // 6 different kill animations
    {
        ANIMOBJECT* pAnimObj = new ANIMOBJECT(m_pGaeaClient, m_pInterface, m_pEngineDevice);
        pAnimObj->CreateSub(this, "KILL_ANIMATION_SLOT", UI_FLAG_DEFAULT);
        pAnimObj->CreateSubControl();
        pAnimObj->wSLOT = static_cast<WORD>(i);
        RegisterControl(pAnimObj);
        m_listAnimObj.push_back(pAnimObj);
    }
}

void CKillAnimationSystem::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
    CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);
    
    // Update card expiration
    UpdateCardExpiration();
}

void CKillAnimationSystem::TriggerKillAnimation(DWORD dwKillerID, DWORD dwVictimID)
{
    // Check if player has any active kill animation cards
    if (m_mapAnimCards.empty())
        return;

    // Select random animation type from available cards
    EMKILL_ANIMATION_TYPE emType = GetRandomAnimationType();
    
    // Show the kill effect
    ShowKillEffect(emType);
}

void CKillAnimationSystem::AddAnimationCard(DWORD dwCardID, DWORD dwExpireTime)
{
    SKILL_ANIMATION_CARD card;
    card.dwCardID = dwCardID;
    card.dwExpireTime = dwExpireTime;
    card.bActive = true;
    card.strAnimName = GetAnimationName(static_cast<EMKILL_ANIMATION_TYPE>(dwCardID));
    
    m_mapAnimCards[dwCardID] = card;
}

void CKillAnimationSystem::RemoveAnimationCard(DWORD dwCardID)
{
    MAP_ANIMATION_CARDS::iterator iter = m_mapAnimCards.find(dwCardID);
    if (iter != m_mapAnimCards.end())
    {
        m_mapAnimCards.erase(iter);
    }
}

bool CKillAnimationSystem::HasActiveCard(DWORD dwCardID)
{
    MAP_ANIMATION_CARDS::iterator iter = m_mapAnimCards.find(dwCardID);
    return (iter != m_mapAnimCards.end() && iter->second.bActive);
}

void CKillAnimationSystem::UpdateCardExpiration()
{
    DWORD dwCurrentTime = timeGetTime();
    
    MAP_ANIMATION_CARDS::iterator iter = m_mapAnimCards.begin();
    while (iter != m_mapAnimCards.end())
    {
        if (iter->second.dwExpireTime > 0 && dwCurrentTime > iter->second.dwExpireTime)
        {
            iter = m_mapAnimCards.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void CKillAnimationSystem::ShowKillEffect(const EMKILL_ANIMATION_TYPE& emType)
{
    // Find available animation slot
    LIST_ANIMOBJ_ITER iter = m_listAnimObj.begin();
    LIST_ANIMOBJ_ITER iter_end = m_listAnimObj.end();
    
    for (; iter != iter_end; ++iter)
    {
        ANIMOBJECT* pAnimObj = *iter;
        if (!pAnimObj->bENABLE)
        {
            // Setup animation
            pAnimObj->bENABLE = true;
            pAnimObj->fANITIME = 3.0f; // 3 seconds animation
            pAnimObj->fANITIMEEFFECT = 3.0f;
            pAnimObj->SetVisibleSingle(TRUE);
            break;
        }
    }
}

void CKillAnimationSystem::HideAll()
{
    LIST_ANIMOBJ_ITER iter = m_listAnimObj.begin();
    LIST_ANIMOBJ_ITER iter_end = m_listAnimObj.end();
    
    for (; iter != iter_end; ++iter)
    {
        (*iter)->ResetAnimation();
    }
}

EMKILL_ANIMATION_TYPE CKillAnimationSystem::GetRandomAnimationType()
{
    if (m_mapAnimCards.empty())
        return KILL_ANIM_SWORD_SLASH;

    // Get random card from available cards
    int nRandom = rand() % m_mapAnimCards.size();
    MAP_ANIMATION_CARDS::iterator iter = m_mapAnimCards.begin();
    std::advance(iter, nRandom);
    
    return static_cast<EMKILL_ANIMATION_TYPE>(iter->first);
}

std::string CKillAnimationSystem::GetAnimationName(EMKILL_ANIMATION_TYPE emType)
{
    switch (emType)
    {
    case KILL_ANIM_SWORD_SLASH:    return "劍氣砍殺";
    case KILL_ANIM_PIERCE_FATAL:   return "穿刺致命";
    case KILL_ANIM_EXPLOSION:      return "爆炸轟殺";
    case KILL_ANIM_ICE_SHATTER:    return "冰凍粉碎";
    case KILL_ANIM_FLAME_BURN:     return "烈焰焚燒";
    case KILL_ANIM_THUNDER_STRIKE: return "雷電轟擊";
    default:                       return "未知動畫";
    }
}

//----------------------------------------------------------------------------------------------------//
// MyKillAnimationSystem Implementation - Jy技術團隊
//----------------------------------------------------------------------------------------------------//

MyKillAnimationSystem::MyKillAnimationSystem(GLGaeaClient* pGaeaClient, CInnerInterface* pInterface, EngineDeviceMan* pEngineDevice)
    : CKillAnimationSystem(pGaeaClient, pInterface, pEngineDevice)
{
}

void MyKillAnimationSystem::CreateUIWindowAndRegisterOwnership()
{
    CKillAnimationSystem::CreateSubControl();
}

void MyKillAnimationSystem::TriggerKillAnimation(DWORD dwKillerID, DWORD dwVictimID)
{
    CKillAnimationSystem::TriggerKillAnimation(dwKillerID, dwVictimID);
}

void MyKillAnimationSystem::AddAnimationCard(DWORD dwCardID, DWORD dwExpireTime)
{
    CKillAnimationSystem::AddAnimationCard(dwCardID, dwExpireTime);
}

void MyKillAnimationSystem::RemoveAnimationCard(DWORD dwCardID)
{
    CKillAnimationSystem::RemoveAnimationCard(dwCardID);
}

void MyKillAnimationSystem::HideAll()
{
    CKillAnimationSystem::HideAll();
}