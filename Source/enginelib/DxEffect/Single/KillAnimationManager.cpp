#include "StdAfx.h"
#include "KillAnimationManager.h"
#include "DxEffSingleMan.h"
#include "../../DxTools/DxViewPort.h"
#include "../../DxSound/DxSoundMan.h"
#include "../../../SigmaCore/DebugInclude.h"

// Static instance
KillAnimationManager* KillAnimationManager::s_pInstance = nullptr;

KillAnimationManager::KillAnimationManager()
	: m_poolAnimations(50) // Pool size of 50 animation instances
	, m_pEffectGroup(nullptr)
	, m_pd3dDevice(nullptr)
{
	InitializeDefaultConfigs();
	LoadDefaultResources();
}

KillAnimationManager::~KillAnimationManager()
{
	Cleanup();
}

KillAnimationManager& KillAnimationManager::GetInstance()
{
	if (!s_pInstance)
	{
		s_pInstance = new KillAnimationManager();
	}
	return *s_pInstance;
}

void KillAnimationManager::DestroyInstance()
{
	SAFE_DELETE(s_pInstance);
}

HRESULT KillAnimationManager::Initialize(LPDIRECT3DDEVICEQ pd3dDevice, DxEffSingleGroup* pEffectGroup)
{
	if (!pd3dDevice)
		return E_INVALIDARG;
	
	m_pd3dDevice = pd3dDevice;
	m_pEffectGroup = pEffectGroup;
	
	// Initialize object pool
	m_poolAnimations.Initialize();
	
	return S_OK;
}

void KillAnimationManager::Cleanup()
{
	// Stop all active animations
	StopAllAnimations();
	
	// Clear pools
	m_poolAnimations.FinalCleanup();
	
	m_pd3dDevice = nullptr;
	m_pEffectGroup = nullptr;
}

bool KillAnimationManager::PlayKillAnimation(KILL_ANIMATION_TYPE eType, 
											 const D3DXVECTOR3& vPosition, 
											 const D3DXVECTOR3& vDirection,
											 const KILL_ANIMATION_CONFIG* pConfig)
{
	if (eType >= KILL_ANIM_MAX)
		return false;
	
	if (!m_pd3dDevice || !m_pEffectGroup)
		return false;
	
	// Create new animation instance
	KILL_ANIMATION_INSTANCE* pInstance = CreateAnimationInstance();
	if (!pInstance)
		return false;
	
	// Set up animation data
	pInstance->eType = eType;
	pInstance->vPosition = vPosition;
	pInstance->vDirection = vDirection;
	pInstance->sConfig = pConfig ? *pConfig : m_arrConfigs[eType];
	pInstance->fElapsedTime = 0.0f;
	pInstance->bActive = true;
	
	// Create the appropriate effect based on animation type
	switch (eType)
	{
	case KILL_ANIM_SLASH:
		pInstance->pEffect = CreateSlashEffect(pInstance);
		break;
	case KILL_ANIM_STAB:
		pInstance->pEffect = CreateStabEffect(pInstance);
		break;
	case KILL_ANIM_EXPLOSION:
		pInstance->pEffect = CreateExplosionEffect(pInstance);
		break;
	case KILL_ANIM_FREEZE:
		pInstance->pEffect = CreateFreezeEffect(pInstance);
		break;
	case KILL_ANIM_FLAME:
		pInstance->pEffect = CreateFlameEffect(pInstance);
		break;
	case KILL_ANIM_LIGHTNING:
		pInstance->pEffect = CreateLightningEffect(pInstance);
		break;
	default:
		ReleaseAnimationInstance(pInstance);
		return false;
	}
	
	if (!pInstance->pEffect)
	{
		ReleaseAnimationInstance(pInstance);
		return false;
	}
	
	// Add to active animations list
	m_vecActiveAnimations.push_back(pInstance);
	
	// Play sound effect if enabled
	if (pInstance->sConfig.bUseSound && !m_arrSoundPaths[eType].empty())
	{
		DxSoundMan::GetInstance().PlaySound(m_arrSoundPaths[eType].c_str());
	}
	
	return true;
}

void KillAnimationManager::FrameMove(float fElapsedTime)
{
	// Update all active animations
	for (auto it = m_vecActiveAnimations.begin(); it != m_vecActiveAnimations.end();)
	{
		KILL_ANIMATION_INSTANCE* pInstance = *it;
		
		if (!pInstance || !pInstance->bActive)
		{
			if (pInstance)
				ReleaseAnimationInstance(pInstance);
			it = m_vecActiveAnimations.erase(it);
			continue;
		}
		
		// Update animation
		UpdateAnimation(pInstance, fElapsedTime);
		
		// Check if animation has finished
		if (pInstance->fElapsedTime >= pInstance->sConfig.fDuration)
		{
			StopAnimation(pInstance);
			it = m_vecActiveAnimations.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void KillAnimationManager::Render()
{
	if (!m_pd3dDevice)
		return;
	
	// Render all active animations
	for (KILL_ANIMATION_INSTANCE* pInstance : m_vecActiveAnimations)
	{
		if (pInstance && pInstance->bActive && pInstance->pEffect)
		{
			pInstance->pEffect->Render(m_pd3dDevice, DxViewPort::GetInstance().GetMatView(), DxViewPort::GetInstance().GetMatProj());
		}
	}
}

void KillAnimationManager::SetAnimationConfig(KILL_ANIMATION_TYPE eType, const KILL_ANIMATION_CONFIG& config)
{
	if (eType < KILL_ANIM_MAX)
	{
		m_arrConfigs[eType] = config;
	}
}

const KILL_ANIMATION_CONFIG& KillAnimationManager::GetAnimationConfig(KILL_ANIMATION_TYPE eType) const
{
	static KILL_ANIMATION_CONFIG defaultConfig;
	return (eType < KILL_ANIM_MAX) ? m_arrConfigs[eType] : defaultConfig;
}

void KillAnimationManager::StopAnimation(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance)
		return;
	
	pInstance->bActive = false;
	
	if (pInstance->pEffect)
	{
		// Effect cleanup will be handled by the effect system
		pInstance->pEffect = nullptr;
	}
	
	ReleaseAnimationInstance(pInstance);
}

void KillAnimationManager::StopAllAnimations()
{
	for (KILL_ANIMATION_INSTANCE* pInstance : m_vecActiveAnimations)
	{
		if (pInstance)
		{
			StopAnimation(pInstance);
		}
	}
	m_vecActiveAnimations.clear();
}

void KillAnimationManager::StopAnimationsByType(KILL_ANIMATION_TYPE eType)
{
	for (auto it = m_vecActiveAnimations.begin(); it != m_vecActiveAnimations.end();)
	{
		KILL_ANIMATION_INSTANCE* pInstance = *it;
		
		if (pInstance && pInstance->eType == eType)
		{
			StopAnimation(pInstance);
			it = m_vecActiveAnimations.erase(it);
		}
		else
		{
			++it;
		}
	}
}

bool KillAnimationManager::IsAnimationActive(KILL_ANIMATION_TYPE eType) const
{
	for (const KILL_ANIMATION_INSTANCE* pInstance : m_vecActiveAnimations)
	{
		if (pInstance && pInstance->eType == eType && pInstance->bActive)
		{
			return true;
		}
	}
	return false;
}

KILL_ANIMATION_INSTANCE* KillAnimationManager::CreateAnimationInstance()
{
	return m_poolAnimations.New();
}

void KillAnimationManager::ReleaseAnimationInstance(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (pInstance)
	{
		// Reset instance data
		*pInstance = KILL_ANIMATION_INSTANCE();
		m_poolAnimations.Delete(pInstance);
	}
}

void KillAnimationManager::UpdateAnimation(KILL_ANIMATION_INSTANCE* pInstance, float fElapsedTime)
{
	if (!pInstance || !pInstance->bActive)
		return;
	
	pInstance->fElapsedTime += fElapsedTime;
	
	// Update effect if it exists
	if (pInstance->pEffect)
	{
		D3DXMATRIX matWorld;
		D3DXMatrixTranslation(&matWorld, pInstance->vPosition.x, pInstance->vPosition.y, pInstance->vPosition.z);
		pInstance->pEffect->FrameMove(fElapsedTime, matWorld);
	}
}

void KillAnimationManager::InitializeDefaultConfigs()
{
	// Slash Animation - 劍氣砍殺
	m_arrConfigs[KILL_ANIM_SLASH].fIntensity = 1.2f;
	m_arrConfigs[KILL_ANIM_SLASH].vColor = D3DXVECTOR3(1.0f, 0.9f, 0.7f); // Golden
	m_arrConfigs[KILL_ANIM_SLASH].fDuration = 1.5f;
	m_arrConfigs[KILL_ANIM_SLASH].fScale = 1.0f;
	
	// Stab Animation - 穿刺致命
	m_arrConfigs[KILL_ANIM_STAB].fIntensity = 1.5f;
	m_arrConfigs[KILL_ANIM_STAB].vColor = D3DXVECTOR3(0.8f, 0.8f, 1.0f); // Steel blue
	m_arrConfigs[KILL_ANIM_STAB].fDuration = 1.0f;
	m_arrConfigs[KILL_ANIM_STAB].fScale = 0.8f;
	
	// Explosion Animation - 爆炸轟殺
	m_arrConfigs[KILL_ANIM_EXPLOSION].fIntensity = 2.0f;
	m_arrConfigs[KILL_ANIM_EXPLOSION].vColor = D3DXVECTOR3(1.0f, 0.5f, 0.2f); // Orange-red
	m_arrConfigs[KILL_ANIM_EXPLOSION].fDuration = 2.0f;
	m_arrConfigs[KILL_ANIM_EXPLOSION].fScale = 1.5f;
	
	// Freeze Animation - 冰凍粉碎
	m_arrConfigs[KILL_ANIM_FREEZE].fIntensity = 1.0f;
	m_arrConfigs[KILL_ANIM_FREEZE].vColor = D3DXVECTOR3(0.7f, 0.9f, 1.0f); // Ice blue
	m_arrConfigs[KILL_ANIM_FREEZE].fDuration = 2.5f;
	m_arrConfigs[KILL_ANIM_FREEZE].fScale = 1.2f;
	
	// Flame Animation - 烈焰焚燒
	m_arrConfigs[KILL_ANIM_FLAME].fIntensity = 1.8f;
	m_arrConfigs[KILL_ANIM_FLAME].vColor = D3DXVECTOR3(1.0f, 0.3f, 0.1f); // Fire red
	m_arrConfigs[KILL_ANIM_FLAME].fDuration = 3.0f;
	m_arrConfigs[KILL_ANIM_FLAME].fScale = 1.3f;
	
	// Lightning Animation - 雷電轟擊
	m_arrConfigs[KILL_ANIM_LIGHTNING].fIntensity = 1.7f;
	m_arrConfigs[KILL_ANIM_LIGHTNING].vColor = D3DXVECTOR3(0.9f, 0.9f, 1.0f); // Electric white
	m_arrConfigs[KILL_ANIM_LIGHTNING].fDuration = 0.8f;
	m_arrConfigs[KILL_ANIM_LIGHTNING].fScale = 1.1f;
}

void KillAnimationManager::LoadDefaultResources()
{
	// Default sound paths
	m_arrSoundPaths[KILL_ANIM_SLASH] = "sound/kill_slash.wav";
	m_arrSoundPaths[KILL_ANIM_STAB] = "sound/kill_stab.wav";
	m_arrSoundPaths[KILL_ANIM_EXPLOSION] = "sound/kill_explosion.wav";
	m_arrSoundPaths[KILL_ANIM_FREEZE] = "sound/kill_freeze.wav";
	m_arrSoundPaths[KILL_ANIM_FLAME] = "sound/kill_flame.wav";
	m_arrSoundPaths[KILL_ANIM_LIGHTNING] = "sound/kill_lightning.wav";
	
	// Default effect paths
	m_arrEffectPaths[KILL_ANIM_SLASH] = "effect/kill_slash.effsig";
	m_arrEffectPaths[KILL_ANIM_STAB] = "effect/kill_stab.effsig";
	m_arrEffectPaths[KILL_ANIM_EXPLOSION] = "effect/kill_explosion.effsig";
	m_arrEffectPaths[KILL_ANIM_FREEZE] = "effect/kill_freeze.effsig";
	m_arrEffectPaths[KILL_ANIM_FLAME] = "effect/kill_flame.effsig";
	m_arrEffectPaths[KILL_ANIM_LIGHTNING] = "effect/kill_lightning.effsig";
}

// Forward declarations for effect creation functions
// These will be implemented in separate files
DxEffSingle* KillAnimationManager::CreateSlashEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	// Implementation will be in KillAnimationManager_Slash.cpp
	return nullptr;
}

DxEffSingle* KillAnimationManager::CreateStabEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	// Implementation will be in KillAnimationManager_Stab.cpp
	return nullptr;
}

DxEffSingle* KillAnimationManager::CreateExplosionEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	// Implementation will be in KillAnimationManager_Explosion.cpp
	return nullptr;
}

DxEffSingle* KillAnimationManager::CreateFreezeEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	// Implementation will be in KillAnimationManager_Freeze.cpp
	return nullptr;
}

DxEffSingle* KillAnimationManager::CreateFlameEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	// Implementation will be in KillAnimationManager_Flame.cpp
	return nullptr;
}

DxEffSingle* KillAnimationManager::CreateLightningEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	// Implementation will be in KillAnimationManager_Lightning.cpp
	return nullptr;
}