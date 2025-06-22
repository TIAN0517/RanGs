#pragma once

#include "../../../EngineLib/GUInterface/UIGroup.h"
#include "../../InnerInterface.h"

#include <string>
#include <list>
#include <map>

//-----------------------------------------------------------------------------------------------//
// Kill Animation System - Jy技術團隊
// 6種擊殺動畫卡片系統: 劍氣砍殺, 穿刺致命, 爆炸轟殺, 冰凍粉碎, 烈焰焚燒, 雷電轟擊
//-----------------------------------------------------------------------------------------------//

class CInnerInterface;
class GLGaeaClient;
class CSkillImage;

enum EMKILL_ANIMATION_TYPE
{
    KILL_ANIM_SWORD_SLASH    = 12001,  // 劍氣砍殺
    KILL_ANIM_PIERCE_FATAL   = 12002,  // 穿刺致命
    KILL_ANIM_EXPLOSION      = 12003,  // 爆炸轟殺
    KILL_ANIM_ICE_SHATTER    = 12004,  // 冰凍粉碎
    KILL_ANIM_FLAME_BURN     = 12005,  // 烈焰焚燒
    KILL_ANIM_THUNDER_STRIKE = 12006,  // 雷電轟擊
};

struct SKILL_ANIMATION_CARD
{
    DWORD dwCardID;
    DWORD dwExpireTime;
    std::string strAnimName;
    bool bActive;
    
    SKILL_ANIMATION_CARD()
        : dwCardID(0)
        , dwExpireTime(0)
        , bActive(false)
    {
    }
};

class CKillAnimationSystem : public CUIGroup
{
public:
    struct ANIMOBJECT : public CUIGroup
    {
        WORD wSLOT;
        CSkillImage* pIMAGE;
        CSkillImage* pIMAGE_EFFECT;
        bool bENABLE;
        float fANITIME;
        float fANITIMEEFFECT;
        GLGaeaClient* pGaeaClient;
        CInnerInterface* pInterface;
        EngineDeviceMan* pEngineDevice;

        ANIMOBJECT(GLGaeaClient* _pGaeaClient, CInnerInterface* _pInterface, EngineDeviceMan* _pEngineDevice)
            : CUIGroup(_pEngineDevice)
            , wSLOT(0)
            , pIMAGE(NULL)
            , pIMAGE_EFFECT(NULL)
            , bENABLE(false)
            , fANITIME(0.0f)
            , fANITIMEEFFECT(0.0f)
            , pGaeaClient(_pGaeaClient)
            , pInterface(_pInterface)
            , pEngineDevice(_pEngineDevice)
        {
        }

        virtual ~ANIMOBJECT();
        void CreateSubControl();
        virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
        void ResetAnimation();
    };

    typedef std::list<ANIMOBJECT*> LIST_ANIMOBJ;
    typedef LIST_ANIMOBJ::iterator LIST_ANIMOBJ_ITER;
    typedef std::map<DWORD, SKILL_ANIMATION_CARD> MAP_ANIMATION_CARDS;

public:
    CKillAnimationSystem(GLGaeaClient* pGaeaClient, CInnerInterface* pInterface, EngineDeviceMan* pEngineDevice);
    virtual ~CKillAnimationSystem();

public:
    void CreateSubControl();
    void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);

public:
    // Kill Animation Management - Jy技術團隊
    void TriggerKillAnimation(DWORD dwKillerID, DWORD dwVictimID);
    void AddAnimationCard(DWORD dwCardID, DWORD dwExpireTime);
    void RemoveAnimationCard(DWORD dwCardID);
    bool HasActiveCard(DWORD dwCardID);
    void UpdateCardExpiration();
    void ShowKillEffect(const EMKILL_ANIMATION_TYPE& emType);
    void HideAll();

protected:
    GLGaeaClient* m_pGaeaClient;
    CInnerInterface* m_pInterface;
    LIST_ANIMOBJ m_listAnimObj;
    MAP_ANIMATION_CARDS m_mapAnimCards;
    bool m_bShow;

private:
    EMKILL_ANIMATION_TYPE GetRandomAnimationType();
    std::string GetAnimationName(EMKILL_ANIMATION_TYPE emType);
};

//----------------------------------------------------------------------------------------------------//
// Interface for Kill Animation System - Jy技術團隊
//----------------------------------------------------------------------------------------------------//

class IKillAnimationSystem
{
public:
    virtual ~IKillAnimationSystem() {}
    virtual void CreateUIWindowAndRegisterOwnership() = 0;
    virtual void TriggerKillAnimation(DWORD dwKillerID, DWORD dwVictimID) = 0;
    virtual void AddAnimationCard(DWORD dwCardID, DWORD dwExpireTime) = 0;
    virtual void RemoveAnimationCard(DWORD dwCardID) = 0;
    virtual void HideAll() = 0;
};

class MyKillAnimationSystem : public IKillAnimationSystem, private CKillAnimationSystem
{
public:
    MyKillAnimationSystem(GLGaeaClient* pGaeaClient, CInnerInterface* pInterface, EngineDeviceMan* pEngineDevice);
    virtual ~MyKillAnimationSystem() {}

public:
    virtual void CreateUIWindowAndRegisterOwnership();
    virtual void TriggerKillAnimation(DWORD dwKillerID, DWORD dwVictimID);
    virtual void AddAnimationCard(DWORD dwCardID, DWORD dwExpireTime);
    virtual void RemoveAnimationCard(DWORD dwCardID);
    virtual void HideAll();
};