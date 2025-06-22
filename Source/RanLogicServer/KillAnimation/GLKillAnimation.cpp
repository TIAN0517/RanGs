#include "pch.h"
#include "GLKillAnimation.h"
#include "../Character/GLChar.h"
#include "../Land/GLLandMan.h"
#include "../FieldServer/GLGaeaServer.h"
#include "../../RanLogic/Msg/GLContrlMsg.h"

// Static instance
GLKillAnimation* GLKillAnimation::s_pInstance = nullptr;

GLKillAnimation& GLKillAnimation::GetInstance()
{
    if (!s_pInstance)
    {
        s_pInstance = new GLKillAnimation();
    }
    return *s_pInstance;
}

void GLKillAnimation::ReleaseInstance()
{
    if (s_pInstance)
    {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

GLKillAnimation::GLKillAnimation()
{
}

GLKillAnimation::~GLKillAnimation()
{
}

void GLKillAnimation::InitKillAnimation()
{
    // Initialize kill animation system
    // Could load configuration from database here
}

void GLKillAnimation::ProcessKillAnimation(GLChar* pKiller, GLChar* pTarget, EMKILL_ANIMATION_TYPE emType)
{
    if (!pKiller || !pTarget) return;
    
    GLLandMan* pLandMan = pTarget->GetLandMan();
    if (!pLandMan) return;
    
    // Get target position for animation
    D3DXVECTOR3 vPos = pTarget->GetPosition();
    
    // Send animation to all clients in the area
    SendKillAnimationToClients(pLandMan, vPos, emType);
}

void GLKillAnimation::SendKillAnimationToClients(GLLandMan* pLandMan, const D3DXVECTOR3& vPos, EMKILL_ANIMATION_TYPE emType)
{
    if (!pLandMan) return;
    
    // Create message to send to clients
    // For now, we'll use a simple approach with existing message system
    // In a full implementation, you'd create a specific message type for kill animations
    
    // TODO: Create GLMSG::NET_KILL_ANIMATION message
    // For now, use general effect message approach
    
    // Send to all players in visible range
    // This is a simplified version - in practice you'd want to optimize the range
    const float fRange = 100.0f; // Visible range for the effect
    
    // Get all characters in range
    VEC_SK_TAR vecTargets;
    pLandMan->DetectTarget(vPos, fRange, vecTargets);
    
    for (size_t i = 0; i < vecTargets.size(); ++i)
    {
        if (vecTargets[i].emCrow == CROW_PC)
        {
            GLChar* pChar = pLandMan->GetChar(vecTargets[i].GaeaId);
            if (pChar)
            {
                // TODO: Send kill animation message to client
                // For now, just notify via chat for testing
                std::string strMsg = sc::string::format("Kill Animation: %s at position (%.1f, %.1f, %.1f)", 
                    GetKillAnimationName(emType), vPos.x, vPos.y, vPos.z);
                
                GLMSG::SNET_SERVER_GENERALCHAT NetMsg;
                NetMsg.emType = CHAT_TYPE_SYSTEM;
                NetMsg.SetName("System");
                NetMsg.SetChatMsg(strMsg.c_str());
                
                gpGaeaServer->SENDTOCLIENT(pChar->ClientSlot(), &NetMsg);
            }
        }
    }
}

void GLKillAnimation::GMCommand_PlayKillAnimation(GLChar* pGM, EMKILL_ANIMATION_TYPE emType)
{
    if (!pGM) return;
    
    if (!IsValidKillAnimationType((DWORD)emType))
    {
        GLMSG::SNET_SERVER_GENERALCHAT NetMsg;
        NetMsg.emType = CHAT_TYPE_SYSTEM;
        NetMsg.SetName("GM");
        NetMsg.SetChatMsg("Invalid kill animation type!");
        gpGaeaServer->SENDTOCLIENT(pGM->ClientSlot(), &NetMsg);
        return;
    }
    
    D3DXVECTOR3 vPos = pGM->GetPosition();
    vPos.y += 2.0f; // Slightly above the GM
    
    GLLandMan* pLandMan = pGM->GetLandMan();
    if (pLandMan)
    {
        SendKillAnimationToClients(pLandMan, vPos, emType);
        
        // Notify GM
        std::string strMsg = sc::string::format("Playing %s animation at your position", 
            GetKillAnimationName(emType));
        
        GLMSG::SNET_SERVER_GENERALCHAT NetMsg;
        NetMsg.emType = CHAT_TYPE_SYSTEM;
        NetMsg.SetName("GM");
        NetMsg.SetChatMsg(strMsg.c_str());
        gpGaeaServer->SENDTOCLIENT(pGM->ClientSlot(), &NetMsg);
    }
}

void GLKillAnimation::GMCommand_TestAllAnimations(GLChar* pGM)
{
    if (!pGM) return;
    
    D3DXVECTOR3 vBasePos = pGM->GetPosition();
    GLLandMan* pLandMan = pGM->GetLandMan();
    
    if (!pLandMan) return;
    
    // Play all animations in a circle around the GM
    EMKILL_ANIMATION_TYPE aTypes[] = {
        EMKILL_SWORD_SLASH,
        EMKILL_PIERCE_FATAL,
        EMKILL_EXPLOSION_BLAST,
        EMKILL_ICE_SHATTER,
        EMKILL_FLAME_BURN,
        EMKILL_LIGHTNING_STRIKE
    };
    
    const int nCount = sizeof(aTypes) / sizeof(aTypes[0]);
    const float fRadius = 5.0f;
    
    for (int i = 0; i < nCount; ++i)
    {
        float fAngle = (2.0f * D3DX_PI * i) / nCount;
        D3DXVECTOR3 vPos = vBasePos;
        vPos.x += cosf(fAngle) * fRadius;
        vPos.z += sinf(fAngle) * fRadius;
        vPos.y += 2.0f;
        
        SendKillAnimationToClients(pLandMan, vPos, aTypes[i]);
    }
    
    // Notify GM
    GLMSG::SNET_SERVER_GENERALCHAT NetMsg;
    NetMsg.emType = CHAT_TYPE_SYSTEM;
    NetMsg.SetName("GM");
    NetMsg.SetChatMsg("Playing all kill animations around your position");
    gpGaeaServer->SENDTOCLIENT(pGM->ClientSlot(), &NetMsg);
}

EMKILL_ANIMATION_TYPE GLKillAnimation::GetRandomKillAnimation()
{
    EMKILL_ANIMATION_TYPE aTypes[] = {
        EMKILL_SWORD_SLASH,
        EMKILL_PIERCE_FATAL,
        EMKILL_EXPLOSION_BLAST,
        EMKILL_ICE_SHATTER,
        EMKILL_FLAME_BURN,
        EMKILL_LIGHTNING_STRIKE
    };
    
    int nIndex = rand() % (sizeof(aTypes) / sizeof(aTypes[0]));
    return aTypes[nIndex];
}

BOOL GLKillAnimation::IsValidKillAnimationType(DWORD dwType)
{
    switch (dwType)
    {
    case EMKILL_SWORD_SLASH:
    case EMKILL_PIERCE_FATAL:
    case EMKILL_EXPLOSION_BLAST:
    case EMKILL_ICE_SHATTER:
    case EMKILL_FLAME_BURN:
    case EMKILL_LIGHTNING_STRIKE:
        return TRUE;
    default:
        return FALSE;
    }
}

const char* GLKillAnimation::GetKillAnimationName(EMKILL_ANIMATION_TYPE emType)
{
    switch (emType)
    {
    case EMKILL_SWORD_SLASH:
        return "Sword Slash (劍氣砍殺)";
    case EMKILL_PIERCE_FATAL:
        return "Pierce Fatal (穿刺致命)";
    case EMKILL_EXPLOSION_BLAST:
        return "Explosion Blast (爆炸轟殺)";
    case EMKILL_ICE_SHATTER:
        return "Ice Shatter (冰凍粉碎)";
    case EMKILL_FLAME_BURN:
        return "Flame Burn (烈焰焚燒)";
    case EMKILL_LIGHTNING_STRIKE:
        return "Lightning Strike (雷電轟擊)";
    default:
        return "Unknown";
    }
}