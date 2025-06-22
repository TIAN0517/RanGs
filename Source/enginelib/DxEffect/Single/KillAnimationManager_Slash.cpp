#include "StdAfx.h"
#include "KillAnimationManager.h"
#include "DxEffectParticleSys.h"
#include "DxEffSingleMan.h"
#include "../../DxTools/DxMethods.h"
#include "../../../SigmaCore/DebugInclude.h"

// Slash Effect Implementation - 劍氣砍殺
// Creates a sword slash effect with particle trails and screen flash
DxEffSingle* KillAnimationManager::CreateSlashEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return nullptr;
	
	// Create particle system for slash trail
	PARTICLESYS_PROPERTY property;
	
	// Set basic particle properties
	property.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	property.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, -9.8f, 0.0f);
	property.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, -19.6f, 0.0f);
	
	property.m_Property.m_vPlayTime = D3DXVECTOR2(0.0f, pInstance->sConfig.fDuration);
	property.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.1f);
	
	// Set slash-specific flags
	property.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER | PARTICLE_FLAG_MESH;
	property.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD;
	
	// Range and rotation for slash arc
	property.m_Property.m_vRange = D3DXVECTOR3(2.0f, 0.5f, 0.1f) * pInstance->sConfig.fScale;
	property.m_Property.m_fRangeRate = 0.8f;
	property.m_Property.m_fRotateAngel = D3DX_PI * 0.25f; // 45 degree slash
	
	// Particle count and emission
	property.m_Property.m_dwParticles = static_cast<DWORD>(30 * pInstance->sConfig.fIntensity);
	property.m_Property.m_fEmissionRate = 50.0f * pInstance->sConfig.fIntensity;
	
	// Size properties for slash particles
	property.m_Property.m_vSize = D3DXVECTOR2(0.1f, 0.3f) * pInstance->sConfig.fScale;
	property.m_Property.m_vSizeRate = D3DXVECTOR2(2.0f, 1.5f);
	property.m_Property.m_vSizeStart = D3DXVECTOR2(0.05f, 0.1f);
	property.m_Property.m_vSizeEnd = D3DXVECTOR2(0.5f, 0.8f);
	
	// Life time for particles
	property.m_Property.m_vLife = D3DXVECTOR2(0.3f, 0.8f);
	property.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.2f);
	
	// Velocity for slash motion
	property.m_Property.m_vVelocity = D3DXVECTOR3(8.0f, 2.0f, 0.0f) * pInstance->sConfig.fIntensity;
	property.m_Property.m_vVelocityRate = D3DXVECTOR3(1.5f, 0.5f, 0.2f);
	
	// Color settings - golden sword gleam
	property.m_Property.m_cColorStart = D3DCOLOR_ARGB(255, 
		static_cast<int>(255 * pInstance->sConfig.vColor.x),
		static_cast<int>(255 * pInstance->sConfig.vColor.y * 0.9f),
		static_cast<int>(255 * pInstance->sConfig.vColor.z * 0.7f));
	
	property.m_Property.m_cColorVar = D3DCOLOR_ARGB(100, 50, 50, 30);
	property.m_Property.m_cColorEnd = D3DCOLOR_ARGB(0, 255, 200, 100);
	
	// Rotation for dynamic slash effect
	property.m_Property.m_vRotation = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 2.0f);
	property.m_Property.m_vRotationRate = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	
	// Texture settings
	property.m_Property.m_szTexture = "particle\\slash_trail.dds";
	property.m_Property.m_nBlend = 2; // Additive blending
	property.m_Property.m_nPower = static_cast<int>(100 * pInstance->sConfig.fIntensity);
	
	// Sequence settings for animated texture
	property.m_Property.m_dwSequenceFlag = SEQUENCE_FLAG_RANDOM;
	property.m_Property.m_nCol = 4;
	property.m_Property.m_nRow = 4;
	property.m_Property.m_fSequenceSpeed = 15.0f;
	
	// Create the effect
	DxEffSingle* pEffect = DxEffSingleMan::GetInstance().CreateEffSingle(
		PARTICLESYS_PROPERTY::TYPEID,
		&property,
		&pInstance->vPosition,
		&pInstance->vDirection,
		nullptr
	);
	
	if (pEffect)
	{
		// Set initial transformation matrix
		D3DXMATRIX matSlash;
		D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
		D3DXVECTOR3 vRight;
		D3DXVec3Cross(&vRight, &pInstance->vDirection, &vUp);
		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Cross(&vUp, &vRight, &pInstance->vDirection);
		
		// Create rotation matrix for slash direction
		matSlash._11 = vRight.x;    matSlash._12 = vRight.y;    matSlash._13 = vRight.z;    matSlash._14 = 0.0f;
		matSlash._21 = vUp.x;       matSlash._22 = vUp.y;       matSlash._23 = vUp.z;       matSlash._24 = 0.0f;
		matSlash._31 = pInstance->vDirection.x; matSlash._32 = pInstance->vDirection.y; matSlash._33 = pInstance->vDirection.z; matSlash._34 = 0.0f;
		matSlash._41 = pInstance->vPosition.x;  matSlash._42 = pInstance->vPosition.y;  matSlash._43 = pInstance->vPosition.z;  matSlash._44 = 1.0f;
		
		pEffect->SetMatrix(matSlash);
		
		// Create secondary spark effects for impact
		CreateSlashSparkEffect(pInstance);
		
		// Create screen flash effect if enabled
		if (pInstance->sConfig.bUseScreenEffect)
		{
			CreateSlashScreenEffect(pInstance);
		}
	}
	
	return pEffect;
}

// Helper function to create spark effects at slash impact point
void KillAnimationManager::CreateSlashSparkEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return;
	
	// Create small spark particles at the end of slash
	PARTICLESYS_PROPERTY sparkProperty;
	
	// Basic spark settings
	sparkProperty.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	sparkProperty.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, -5.0f, 0.0f);
	sparkProperty.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
	
	sparkProperty.m_Property.m_vPlayTime = D3DXVECTOR2(0.2f, 0.4f);
	sparkProperty.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.05f);
	
	sparkProperty.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER;
	sparkProperty.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD;
	
	// Spark burst area
	sparkProperty.m_Property.m_vRange = D3DXVECTOR3(0.3f, 0.3f, 0.3f) * pInstance->sConfig.fScale;
	sparkProperty.m_Property.m_fRangeRate = 1.0f;
	
	// Fewer but brighter particles for sparks
	sparkProperty.m_Property.m_dwParticles = static_cast<DWORD>(15 * pInstance->sConfig.fIntensity);
	sparkProperty.m_Property.m_fEmissionRate = 30.0f * pInstance->sConfig.fIntensity;
	
	// Small, bright spark size
	sparkProperty.m_Property.m_vSize = D3DXVECTOR2(0.02f, 0.05f) * pInstance->sConfig.fScale;
	sparkProperty.m_Property.m_vSizeRate = D3DXVECTOR2(1.0f, 1.2f);
	sparkProperty.m_Property.m_vSizeStart = D3DXVECTOR2(0.01f, 0.02f);
	sparkProperty.m_Property.m_vSizeEnd = D3DXVECTOR2(0.08f, 0.12f);
	
	// Short life for quick flash
	sparkProperty.m_Property.m_vLife = D3DXVECTOR2(0.1f, 0.3f);
	sparkProperty.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.5f);
	
	// High velocity burst
	sparkProperty.m_Property.m_vVelocity = D3DXVECTOR3(5.0f, 5.0f, 5.0f) * pInstance->sConfig.fIntensity;
	sparkProperty.m_Property.m_vVelocityRate = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
	
	// Bright white-yellow sparks
	sparkProperty.m_Property.m_cColorStart = D3DCOLOR_ARGB(255, 255, 255, 200);
	sparkProperty.m_Property.m_cColorVar = D3DCOLOR_ARGB(50, 30, 30, 50);
	sparkProperty.m_Property.m_cColorEnd = D3DCOLOR_ARGB(0, 255, 150, 50);
	
	// Spark texture
	sparkProperty.m_Property.m_szTexture = "particle\\spark.dds";
	sparkProperty.m_Property.m_nBlend = 2; // Additive
	sparkProperty.m_Property.m_nPower = static_cast<int>(150 * pInstance->sConfig.fIntensity);
	
	// Calculate spark position at end of slash
	D3DXVECTOR3 sparkPos = pInstance->vPosition + pInstance->vDirection * 1.5f * pInstance->sConfig.fScale;
	
	DxEffSingle* pSparkEffect = DxEffSingleMan::GetInstance().CreateEffSingle(
		PARTICLESYS_PROPERTY::TYPEID,
		&sparkProperty,
		&sparkPos,
		&pInstance->vDirection,
		nullptr
	);
}

// Helper function to create screen flash effect
void KillAnimationManager::CreateSlashScreenEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	// This would integrate with the screen effect system
	// For now, we'll add a comment indicating where screen flash would be implemented
	// Implementation would depend on the specific screen effect system used by the engine
	
	/*
	// Example screen flash implementation:
	ScreenEffectManager::GetInstance().CreateFlash(
		D3DCOLOR_ARGB(100, 255, 255, 200), // Flash color
		0.1f,                               // Flash duration
		ScreenEffectManager::FLASH_TYPE_ADDITIVE
	);
	*/
}