#pragma once

#include "../../enginelib/DxEffect/KillAnimation/KillAnimSystem.h"

class GLChar;
class GLLandMan;

//! Kill Animation Handler for Server
class GLKillAnimation
{
private:
    static GLKillAnimation* s_pInstance;
    
public:
    static GLKillAnimation& GetInstance();
    static void ReleaseInstance();
    
    // Server methods
    void InitKillAnimation();
    void ProcessKillAnimation(GLChar* pKiller, GLChar* pTarget, EMKILL_ANIMATION_TYPE emType);
    void SendKillAnimationToClients(GLLandMan* pLandMan, const D3DXVECTOR3& vPos, EMKILL_ANIMATION_TYPE emType);
    
    // GM Command handlers
    void GMCommand_PlayKillAnimation(GLChar* pGM, EMKILL_ANIMATION_TYPE emType);
    void GMCommand_TestAllAnimations(GLChar* pGM);
    
    // Utility methods
    EMKILL_ANIMATION_TYPE GetRandomKillAnimation();
    BOOL IsValidKillAnimationType(DWORD dwType);
    const char* GetKillAnimationName(EMKILL_ANIMATION_TYPE emType);
    
private:
    GLKillAnimation();
    ~GLKillAnimation();
};