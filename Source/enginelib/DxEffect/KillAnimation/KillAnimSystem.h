#pragma once

#include <map>
#include <string>
#include "../../../SigmaCore/Memory/ObjectPool.h"
#include "../../DxTools/Collision.h"
#include "../../DxSound/MovableSound.h"
#include "../../G-Logic/GLDefine.h"

// Kill Animation Types (12001-12006)
enum EMKILL_ANIMATION_TYPE
{
    EMKILL_SWORD_SLASH      = 12001,    // 劍氣砍殺 - 金色劍氣橫掃
    EMKILL_PIERCE_FATAL     = 12002,    // 穿刺致命 - 銀色利刃穿透
    EMKILL_EXPLOSION_BLAST  = 12003,    // 爆炸轟殺 - 橙紅爆炸衝擊波
    EMKILL_ICE_SHATTER      = 12004,    // 冰凍粉碎 - 藍白冰晶碎裂
    EMKILL_FLAME_BURN       = 12005,    // 烈焰焚燒 - 赤紅烈焰燃燒
    EMKILL_LIGHTNING_STRIKE = 12006     // 雷電轟擊 - 紫藍雷電轟鳴
};

// Kill Animation Data Structure
struct SKILL_ANIMATION_DATA
{
    EMKILL_ANIMATION_TYPE   emType;
    std::string             strTexturePath;
    std::string             strSoundPath;
    float                   fDuration;
    D3DCOLOR                dwColor;
    float                   fScale;
    BOOL                    bUseParticle;
    
    SKILL_ANIMATION_DATA()
        : emType(EMKILL_SWORD_SLASH)
        , fDuration(2.5f)
        , dwColor(D3DCOLOR_XRGB(255, 255, 255))
        , fScale(1.0f)
        , bUseParticle(TRUE)
    {
    }
};

// Kill Animation Instance
class DxKillAnimation : public sc::PoolAllocMgr
{
private:
    EMKILL_ANIMATION_TYPE   m_emType;
    D3DXMATRIX              m_matWorld;
    float                   m_fAge;
    float                   m_fLifeTime;
    BOOL                    m_bActive;
    D3DXVECTOR3             m_vPosition;
    LPDIRECT3DTEXTUREQ      m_pTexture;
    SMovSound               m_MovSound;
    
public:
    DxKillAnimation();
    virtual ~DxKillAnimation();
    
    void SetAnimation(EMKILL_ANIMATION_TYPE emType, const D3DXVECTOR3& vPos);
    void FrameMove(float fElapsedTime);
    void Render(LPDIRECT3DDEVICEQ pd3dDevice);
    BOOL IsAlive() const { return m_bActive && m_fAge < m_fLifeTime; }
    void SetDead() { m_bActive = FALSE; }
    
private:
    void LoadResources();
    void PlaySound();
};

// Kill Animation Manager
class DxKillAnimationManager
{
private:
    static DxKillAnimationManager* s_pInstance;
    
    std::map<EMKILL_ANIMATION_TYPE, SKILL_ANIMATION_DATA> m_mapAnimData;
    std::list<DxKillAnimation*> m_listAnimation;
    LPDIRECT3DDEVICEQ m_pd3dDevice;
    
public:
    static DxKillAnimationManager& GetInstance();
    static void ReleaseInstance();
    
    HRESULT InitDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice);
    HRESULT RestoreDeviceObjects();
    HRESULT InvalidateDeviceObjects();
    HRESULT DeleteDeviceObjects();
    
    void FrameMove(float fElapsedTime);
    void Render();
    
    void PlayKillAnimation(EMKILL_ANIMATION_TYPE emType, const D3DXVECTOR3& vPos);
    void ClearAllAnimations();
    
private:
    DxKillAnimationManager();
    ~DxKillAnimationManager();
    
    void InitializeAnimationData();
    void CleanupDeadAnimations();
};