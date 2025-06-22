#include "StdAfx.h"
#include "KillAnimationManager.h"
#include "DxEffectParticleSys.h"
#include "DxEffSingleMan.h"
#include "../../DxTools/DxMethods.h"
#include "../../../SigmaCore/DebugInclude.h"

// Freeze Effect Implementation - 冰凍粉碎
// Creates ice crystallization followed by shattering effect
DxEffSingle* KillAnimationManager::CreateFreezeEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return nullptr;
	
	// Create ice crystallization particle system
	PARTICLESYS_PROPERTY property;
	
	// Set ice crystal properties
	property.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	property.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	property.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, -5.0f, 0.0f);
	
	property.m_Property.m_vPlayTime = D3DXVECTOR2(0.0f, pInstance->sConfig.fDuration * 0.7f);
	property.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.12f);
	
	// Ice crystal formation flags
	property.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER | PARTICLE_FLAG_MESH;
	property.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD | PARTICLE_FLAG_EX_REMAIN;
	
	// Spherical freeze area
	property.m_Property.m_vRange = D3DXVECTOR3(1.2f, 1.2f, 1.2f) * pInstance->sConfig.fScale;
	property.m_Property.m_fRangeRate = 0.9f;
	property.m_Property.m_fRotateAngel = D3DX_PI * 0.5f;
	
	// Dense ice crystal count
	property.m_Property.m_dwParticles = static_cast<DWORD>(45 * pInstance->sConfig.fIntensity);
	property.m_Property.m_fEmissionRate = 90.0f * pInstance->sConfig.fIntensity;
	
	// Sharp, angular ice crystal sizes
	property.m_Property.m_vSize = D3DXVECTOR2(0.1f, 0.3f) * pInstance->sConfig.fScale;
	property.m_Property.m_vSizeRate = D3DXVECTOR2(2.0f, 1.8f);
	property.m_Property.m_vSizeStart = D3DXVECTOR2(0.02f, 0.05f);
	property.m_Property.m_vSizeEnd = D3DXVECTOR2(0.8f, 1.2f);
	
	// Extended life for ice formation and melting
	property.m_Property.m_vLife = D3DXVECTOR2(1.2f, 2.0f);
	property.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.2f);
	
	// Slow, crystallizing velocity
	property.m_Property.m_vVelocity = D3DXVECTOR3(3.0f, 2.0f, 3.0f) * pInstance->sConfig.fIntensity;
	property.m_Property.m_vVelocityRate = D3DXVECTOR3(1.2f, 0.8f, 1.2f);
	
	// Ice blue color scheme
	property.m_Property.m_cColorStart = D3DCOLOR_ARGB(255, 
		static_cast<int>(255 * pInstance->sConfig.vColor.x * 0.7f),
		static_cast<int>(255 * pInstance->sConfig.vColor.y * 0.9f),
		static_cast<int>(255 * pInstance->sConfig.vColor.z));
	
	property.m_Property.m_cColorVar = D3DCOLOR_ARGB(100, 30, 40, 50);
	property.m_Property.m_cColorEnd = D3DCOLOR_ARGB(0, 150, 200, 255);
	
	// Crystalline rotation pattern
	property.m_Property.m_vRotation = D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.5f, D3DX_PI * 0.5f);
	property.m_Property.m_vRotationRate = D3DXVECTOR3(0.8f, 0.8f, 0.8f);
	
	// Ice crystal texture
	property.m_Property.m_szTexture = "particle\\ice_crystal.dds";
	property.m_Property.m_nBlend = 2; // Additive blending for ice gleam
	property.m_Property.m_nPower = static_cast<int>(140 * pInstance->sConfig.fIntensity);
	
	// Sequential crystallization effect
	property.m_Property.m_dwSequenceFlag = SEQUENCE_FLAG_LINEAR;
	property.m_Property.m_nCol = 3;
	property.m_Property.m_nRow = 3;
	property.m_Property.m_fSequenceSpeed = 8.0f;
	
	// Create the main freeze effect
	DxEffSingle* pEffect = DxEffSingleMan::GetInstance().CreateEffSingle(
		PARTICLESYS_PROPERTY::TYPEID,
		&property,
		&pInstance->vPosition,
		&pInstance->vDirection,
		nullptr
	);
	
	if (pEffect)
	{
		// Set transformation matrix
		D3DXMATRIX matFreeze;
		D3DXMatrixTranslation(&matFreeze, pInstance->vPosition.x, pInstance->vPosition.y, pInstance->vPosition.z);
		pEffect->SetMatrix(matFreeze);
		
		// Create ice shatter effect (delayed)
		CreateFreezeShatterEffect(pInstance);
		
		// Create frost mist effect
		CreateFreezeMistEffect(pInstance);
	}
	
	return pEffect;
}

// Helper function to create shattering ice effect
void KillAnimationManager::CreateFreezeShatterEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return;
	
	// Create ice shard particles (triggered after initial freeze)
	PARTICLESYS_PROPERTY shatterProperty;
	
	// Shattering ice physics
	shatterProperty.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	shatterProperty.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, -9.8f, 0.0f);
	shatterProperty.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, -15.0f, 0.0f);
	
	// Delayed shatter start (after crystallization)
	shatterProperty.m_Property.m_vPlayTime = D3DXVECTOR2(pInstance->sConfig.fDuration * 0.6f, pInstance->sConfig.fDuration);
	shatterProperty.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.05f);
	
	shatterProperty.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER | PARTICLE_FLAG_MESH;
	shatterProperty.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD;
	
	// Explosive shatter area
	shatterProperty.m_Property.m_vRange = D3DXVECTOR3(1.5f, 1.0f, 1.5f) * pInstance->sConfig.fScale;
	shatterProperty.m_Property.m_fRangeRate = 1.0f;
	
	// Many small ice shards
	shatterProperty.m_Property.m_dwParticles = static_cast<DWORD>(35 * pInstance->sConfig.fIntensity);
	shatterProperty.m_Property.m_fEmissionRate = 70.0f * pInstance->sConfig.fIntensity;
	
	// Small, sharp ice shard sizes
	shatterProperty.m_Property.m_vSize = D3DXVECTOR2(0.03f, 0.08f) * pInstance->sConfig.fScale;
	shatterProperty.m_Property.m_vSizeRate = D3DXVECTOR2(1.0f, 1.2f);
	shatterProperty.m_Property.m_vSizeStart = D3DXVECTOR2(0.02f, 0.04f);
	shatterProperty.m_Property.m_vSizeEnd = D3DXVECTOR2(0.12f, 0.18f);
	
	// Quick shattering life
	shatterProperty.m_Property.m_vLife = D3DXVECTOR2(0.5f, 1.2f);
	shatterProperty.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.3f);
	
	// High explosive velocity
	shatterProperty.m_Property.m_vVelocity = D3DXVECTOR3(8.0f, 6.0f, 8.0f) * pInstance->sConfig.fIntensity;
	shatterProperty.m_Property.m_vVelocityRate = D3DXVECTOR3(2.0f, 1.5f, 2.0f);
	
	// Bright crystalline shards
	shatterProperty.m_Property.m_cColorStart = D3DCOLOR_ARGB(255, 200, 230, 255);
	shatterProperty.m_Property.m_cColorVar = D3DCOLOR_ARGB(80, 40, 30, 50);
	shatterProperty.m_Property.m_cColorEnd = D3DCOLOR_ARGB(0, 100, 150, 200);
	
	// Tumbling shard rotation
	shatterProperty.m_Property.m_vRotation = D3DXVECTOR3(D3DX_PI, D3DX_PI, D3DX_PI);
	shatterProperty.m_Property.m_vRotationRate = D3DXVECTOR3(4.0f, 4.0f, 4.0f);
	
	// Ice shard texture
	shatterProperty.m_Property.m_szTexture = "particle\\ice_shard.dds";
	shatterProperty.m_Property.m_nBlend = 2; // Additive for bright shards
	shatterProperty.m_Property.m_nPower = static_cast<int>(110 * pInstance->sConfig.fIntensity);
	
	D3DXVECTOR3 upDir(0.0f, 1.0f, 0.0f);
	DxEffSingle* pShatterEffect = DxEffSingleMan::GetInstance().CreateEffSingle(
		PARTICLESYS_PROPERTY::TYPEID,
		&shatterProperty,
		&pInstance->vPosition,
		&upDir,
		nullptr
	);
}

// Helper function to create frost mist effect
void KillAnimationManager::CreateFreezeMistEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return;
	
	// Create cold mist particles
	PARTICLESYS_PROPERTY mistProperty;
	
	// Cold mist physics
	mistProperty.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	mistProperty.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, 0.5f, 0.0f);
	mistProperty.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	mistProperty.m_Property.m_vPlayTime = D3DXVECTOR2(0.3f, pInstance->sConfig.fDuration);
	mistProperty.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.2f);
	
	mistProperty.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER;
	mistProperty.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD;
	
	// Wide mist coverage
	mistProperty.m_Property.m_vRange = D3DXVECTOR3(2.0f, 0.8f, 2.0f) * pInstance->sConfig.fScale;
	mistProperty.m_Property.m_fRangeRate = 0.7f;
	
	// Dense mist particle count
	mistProperty.m_Property.m_dwParticles = static_cast<DWORD>(30 * pInstance->sConfig.fIntensity);
	mistProperty.m_Property.m_fEmissionRate = 60.0f * pInstance->sConfig.fIntensity;
	
	// Large, soft mist clouds
	mistProperty.m_Property.m_vSize = D3DXVECTOR2(0.4f, 0.8f) * pInstance->sConfig.fScale;
	mistProperty.m_Property.m_vSizeRate = D3DXVECTOR2(2.0f, 1.8f);
	mistProperty.m_Property.m_vSizeStart = D3DXVECTOR2(0.2f, 0.3f);
	mistProperty.m_Property.m_vSizeEnd = D3DXVECTOR2(1.8f, 2.5f);
	
	// Long-lasting mist
	mistProperty.m_Property.m_vLife = D3DXVECTOR2(3.0f, 5.0f);
	mistProperty.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.2f);
	
	// Slow drifting velocity
	mistProperty.m_Property.m_vVelocity = D3DXVECTOR3(1.5f, 2.0f, 1.5f) * pInstance->sConfig.fIntensity;
	mistProperty.m_Property.m_vVelocityRate = D3DXVECTOR3(1.0f, 0.5f, 1.0f);
	
	// Pale blue-white mist
	mistProperty.m_Property.m_cColorStart = D3DCOLOR_ARGB(100, 220, 240, 255);
	mistProperty.m_Property.m_cColorVar = D3DCOLOR_ARGB(60, 30, 20, 30);
	mistProperty.m_Property.m_cColorEnd = D3DCOLOR_ARGB(0, 180, 200, 220);
	
	// Gentle mist rotation
	mistProperty.m_Property.m_vRotation = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.2f);
	mistProperty.m_Property.m_vRotationRate = D3DXVECTOR3(0.0f, 0.0f, 0.2f);
	
	// Mist cloud texture
	mistProperty.m_Property.m_szTexture = "particle\\cold_mist.dds";
	mistProperty.m_Property.m_nBlend = 1; // Alpha blending for soft mist
	mistProperty.m_Property.m_nPower = static_cast<int>(60 * pInstance->sConfig.fIntensity);
	
	D3DXVECTOR3 upDir(0.0f, 1.0f, 0.0f);
	DxEffSingle* pMistEffect = DxEffSingleMan::GetInstance().CreateEffSingle(
		PARTICLESYS_PROPERTY::TYPEID,
		&mistProperty,
		&pInstance->vPosition,
		&upDir,
		nullptr
	);
}