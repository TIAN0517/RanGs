// KillCard Integration Example for GLCharClient.cpp
// 
// This example shows how to integrate the Kill Card system with the character combat system.
// Add these code snippets to the appropriate places in GLCharClient.cpp

// 1. Add include at the top of GLCharClient.cpp:
#include "../../RanGfxUI/KillCard/KillCardManager.h"

// 2. Add this method to determine kill type based on combat conditions:
EKILL_TYPE GLCharClient::DetermineKillType(const STARGETID& sACTOR, DWORD dwDamageFlag, BOOL bSkillAttack)
{
    EKILL_TYPE killType = 0;
    
    // Check for critical hit
    if (dwDamageFlag & DAMAGE_TYPE_CRUSHING_BLOW)
    {
        killType |= KILL_TYPE_CRITICAL;
    }
    
    // Check for skill attack
    if (bSkillAttack)
    {
        killType |= KILL_TYPE_SKILL;
    }
    
    // Check for headshot (would need additional data from combat system)
    // if (bHeadshot) killType |= KILL_TYPE_HEADSHOT;
    
    // Check for weapon type to determine ranged/melee
    SITEMCUSTOM* pRightHand = &m_CharData.m_PutOnItems[SLOT_RHAND];
    if (pRightHand && pRightHand->GetNativeID() != NATIVEID_NULL())
    {
        const SITEM* pItem = GLogicData::GetInstance().GetItem(pRightHand->GetNativeID());
        if (pItem)
        {
            // Check weapon type
            if (pItem->sSuitOp.emAttack == ITEMATT_BOW || 
                pItem->sSuitOp.emAttack == ITEMATT_GUN ||
                pItem->sSuitOp.emAttack == ITEMATT_RIFLE)
            {
                killType |= KILL_TYPE_RANGED;
            }
            else if (pItem->sSuitOp.emAttack == ITEMATT_SWORD ||
                     pItem->sSuitOp.emAttack == ITEMATT_DAGGER ||
                     pItem->sSuitOp.emAttack == ITEMATT_SPEAR)
            {
                killType |= KILL_TYPE_MELEE;
            }
        }
    }
    
    return killType;
}

// 3. Add this method to trigger kill cards when an enemy is defeated:
void GLCharClient::OnEnemyKilled(const STARGETID& sVictim, DWORD dwDamageFlag, BOOL bSkillAttack)
{
    // Determine the kill type based on combat conditions
    EKILL_TYPE killType = DetermineKillType(sVictim, dwDamageFlag, bSkillAttack);
    
    // Check for consecutive kills (this would need to be tracked globally)
    static int s_nConsecutiveKills = 0;
    static DWORD s_dwLastKillTime = 0;
    DWORD dwCurrentTime = GetTickCount();
    
    if ((dwCurrentTime - s_dwLastKillTime) <= 3000) // 3 seconds for consecutive
    {
        s_nConsecutiveKills++;
        if (s_nConsecutiveKills >= 2)
        {
            killType |= KILL_TYPE_CONSECUTIVE;
        }
    }
    else
    {
        s_nConsecutiveKills = 1;
    }
    s_dwLastKillTime = dwCurrentTime;
    
    // Trigger the kill card through the interface
    if (m_pGaeaClient && m_pGaeaClient->GetInterface())
    {
        m_pGaeaClient->GetInterface()->TriggerKillCard((int)killType, sVictim.dwID);
    }
}

// 4. Example of how to call this from existing damage/combat methods:
// In ReceiveDamage method, when HP reaches 0, you could add:
/*
if (m_CharData.m_Base.sHP.nNow <= 0)
{
    // Character is defeated, trigger kill card for the attacker
    // This would need to be called on the attacker's client, not the victim
    OnEnemyKilled(m_sTargetID, dwDamageFlag, bSkillAttack);
}
*/

// 5. Alternative integration through message system:
// You could also integrate this through the existing message system
// by adding a new message type for kill notifications and processing
// them in the appropriate message handlers.

// Usage Example:
// The system can be triggered programmatically for testing:
/*
// Trigger a quantum fission card (critical kill)
m_pGaeaClient->GetInterface()->TriggerKillCard(KILL_TYPE_CRITICAL);

// Trigger a hologram disruption card (skill kill)  
m_pGaeaClient->GetInterface()->TriggerKillCard(KILL_TYPE_SKILL);

// Trigger a space-time distortion card (consecutive kills)
m_pGaeaClient->GetInterface()->TriggerKillCard(KILL_TYPE_CONSECUTIVE);

// Trigger a plasma explosion card (ranged kill)
m_pGaeaClient->GetInterface()->TriggerKillCard(KILL_TYPE_RANGED);

// Trigger a nano dissolution card (melee kill)
m_pGaeaClient->GetInterface()->TriggerKillCard(KILL_TYPE_MELEE);

// Trigger a virtual reconstruction card (headshot kill)
m_pGaeaClient->GetInterface()->TriggerKillCard(KILL_TYPE_HEADSHOT);

// Combine multiple conditions (critical + ranged kill)
m_pGaeaClient->GetInterface()->TriggerKillCard(KILL_TYPE_CRITICAL | KILL_TYPE_RANGED);
*/