#include "StdAfx.h"
#include "KillAnimationManager.h"
#include "DxEffectParticleSys.h"
#include "DxEffSingleMan.h"
#include "../../DxTools/DxMethods.h"
#include "../../../SigmaCore/DebugInclude.h"

// Explosion Effect Implementation - 爆炸轟殺
// Creates a devastating explosion with multiple particle stages
DxEffSingle* KillAnimationManager::CreateExplosionEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return nullptr;
	
	// Create main explosion particle system
	PARTICLESYS_PROPERTY property;
	
	// Set explosive particle properties
	property.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	property.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
	property.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, -12.0f, 0.0f);
	
	property.m_Property.m_vPlayTime = D3DXVECTOR2(0.0f, pInstance->sConfig.fDuration * 0.6f);
	property.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.1f);
	
	// High-intensity explosion flags
	property.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER | PARTICLE_FLAG_MESH;
	property.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD | PARTICLE_FLAG_EX_REMAIN;
	
	// Large spherical explosion area
	property.m_Property.m_vRange = D3DXVECTOR3(1.5f, 1.5f, 1.5f) * pInstance->sConfig.fScale;
	property.m_Property.m_fRangeRate = 1.0f;
	property.m_Property.m_fRotateAngel = D3DX_PI * 2.0f; // Full rotation
	
	// High particle count for dense explosion
	property.m_Property.m_dwParticles = static_cast<DWORD>(60 * pInstance->sConfig.fIntensity);
	property.m_Property.m_fEmissionRate = 120.0f * pInstance->sConfig.fIntensity;
	
	// Large, varying particle sizes
	property.m_Property.m_vSize = D3DXVECTOR2(0.2f, 0.6f) * pInstance->sConfig.fScale;
	property.m_Property.m_vSizeRate = D3DXVECTOR2(3.0f, 2.5f);
	property.m_Property.m_vSizeStart = D3DXVECTOR2(0.05f, 0.1f);
	property.m_Property.m_vSizeEnd = D3DXVECTOR2(1.8f, 2.2f);
	
	// Extended life for lingering explosion
	property.m_Property.m_vLife = D3DXVECTOR2(0.8f, 1.5f);
	property.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.3f);
	
	// High radial velocity for explosive burst
	property.m_Property.m_vVelocity = D3DXVECTOR3(8.0f, 6.0f, 8.0f) * pInstance->sConfig.fIntensity;
	property.m_Property.m_vVelocityRate = D3DXVECTOR3(2.0f, 1.5f, 2.0f);
	
	// Fiery orange-red color scheme
	property.m_Property.m_cColorStart = D3DCOLOR_ARGB(255, 
		static_cast<int>(255 * pInstance->sConfig.vColor.x),
		static_cast<int>(255 * pInstance->sConfig.vColor.y * 0.5f),
		static_cast<int>(255 * pInstance->sConfig.vColor.z * 0.2f));
	
	property.m_Property.m_cColorVar = D3DCOLOR_ARGB(120, 80, 50, 20);
	property.m_Property.m_cColorEnd = D3DCOLOR_ARGB(0, 120, 80, 30);
	
	// Random rotation for chaotic explosion
	property.m_Property.m_vRotation = D3DXVECTOR3(D3DX_PI, D3DX_PI, D3DX_PI);
	property.m_Property.m_vRotationRate = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
	
	// Explosion fireball texture
	property.m_Property.m_szTexture = "particle\\explosion_fire.dds";
	property.m_Property.m_nBlend = 2; // Additive blending
	property.m_Property.m_nPower = static_cast<int>(180 * pInstance->sConfig.fIntensity);
	
	// Random sequence for varied explosion appearance
	property.m_Property.m_dwSequenceFlag = SEQUENCE_FLAG_RANDOM;
	property.m_Property.m_nCol = 4;
	property.m_Property.m_nRow = 4;
	property.m_Property.m_fSequenceSpeed = 12.0f;
	
	// Create the main explosion effect
	DxEffSingle* pEffect = DxEffSingleMan::GetInstance().CreateEffSingle(
		PARTICLESYS_PROPERTY::TYPEID,
		&property,
		&pInstance->vPosition,
		&pInstance->vDirection,
		nullptr
	);
	
	if (pEffect)
	{
		// Set identity matrix for omnidirectional explosion
		D3DXMATRIX matExplosion;
		D3DXMatrixTranslation(&matExplosion, pInstance->vPosition.x, pInstance->vPosition.y, pInstance->vPosition.z);
		pEffect->SetMatrix(matExplosion);
		
		// Create additional explosion effects
		CreateExplosionShockwaveEffect(pInstance);
		CreateExplosionSmokeEffect(pInstance);
		CreateExplosionDebrisEffect(pInstance);
		
		// Create screen shake effect if enabled
		if (pInstance->sConfig.bUseScreenEffect)
		{
			CreateExplosionScreenEffect(pInstance);
		}
	}
	
	return pEffect;
}

// Helper function to create shockwave effect
void KillAnimationManager::CreateExplosionShockwaveEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return;
	
	// Create expanding shockwave ring
	PARTICLESYS_PROPERTY shockProperty;
	
	// Shockwave physics
	shockProperty.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	shockProperty.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	shockProperty.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	shockProperty.m_Property.m_vPlayTime = D3DXVECTOR2(0.0f, 0.4f);
	shockProperty.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.02f);
	
	shockProperty.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER;
	shockProperty.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD | PARTICLE_FLAG_EX_GROUND;
	
	// Large radial shockwave
	shockProperty.m_Property.m_vRange = D3DXVECTOR3(2.5f, 0.2f, 2.5f) * pInstance->sConfig.fScale;
	shockProperty.m_Property.m_fRangeRate = 1.0f;
	
	// Few particles for clean ring effect
	shockProperty.m_Property.m_dwParticles = static_cast<DWORD>(12 * pInstance->sConfig.fIntensity);
	shockProperty.m_Property.m_fEmissionRate = 30.0f * pInstance->sConfig.fIntensity;
	
	// Very large expanding rings
	shockProperty.m_Property.m_vSize = D3DXVECTOR2(1.0f, 1.0f) * pInstance->sConfig.fScale;
	shockProperty.m_Property.m_vSizeRate = D3DXVECTOR2(5.0f, 5.0f);
	shockProperty.m_Property.m_vSizeStart = D3DXVECTOR2(0.5f, 0.5f);
	shockProperty.m_Property.m_vSizeEnd = D3DXVECTOR2(8.0f, 8.0f);
	
	// Quick expansion time
	shockProperty.m_Property.m_vLife = D3DXVECTOR2(0.3f, 0.5f);
	shockProperty.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.0f);
	
	// Rapid radial expansion
	shockProperty.m_Property.m_vVelocity = D3DXVECTOR3(15.0f, 0.0f, 15.0f) * pInstance->sConfig.fIntensity;
	shockProperty.m_Property.m_vVelocityRate = D3DXVECTOR3(1.2f, 0.0f, 1.2f);
	
	// Bright orange shockwave
	shockProperty.m_Property.m_cColorStart = D3DCOLOR_ARGB(200, 255, 180, 80);
	shockProperty.m_Property.m_cColorVar = D3DCOLOR_ARGB(60, 50, 40, 20);
	shockProperty.m_Property.m_cColorEnd = D3DCOLOR_ARGB(0, 150, 100, 40);
	
	// Shockwave ring texture
	shockProperty.m_Property.m_szTexture = "particle\\shockwave_ring.dds";
	shockProperty.m_Property.m_nBlend = 2; // Additive
	shockProperty.m_Property.m_nPower = static_cast<int>(160 * pInstance->sConfig.fIntensity);
	
	D3DXVECTOR3 upDir(0.0f, 1.0f, 0.0f);
	DxEffSingle* pShockEffect = DxEffSingleMan::GetInstance().CreateEffSingle(
		PARTICLESYS_PROPERTY::TYPEID,
		&shockProperty,
		&pInstance->vPosition,
		&upDir,
		nullptr
	);
}

// Helper function to create smoke clouds
void KillAnimationManager::CreateExplosionSmokeEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return;
	
	// Create rising smoke clouds
	PARTICLESYS_PROPERTY smokeProperty;
	
	// Smoke physics with upward drift
	smokeProperty.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	smokeProperty.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	smokeProperty.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, 0.5f, 0.0f);
	
	smokeProperty.m_Property.m_vPlayTime = D3DXVECTOR2(0.2f, pInstance->sConfig.fDuration);
	smokeProperty.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.15f);
	
	smokeProperty.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER;
	smokeProperty.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD;
	
	// Smoke expansion area
	smokeProperty.m_Property.m_vRange = D3DXVECTOR3(1.2f, 0.5f, 1.2f) * pInstance->sConfig.fScale;
	smokeProperty.m_Property.m_fRangeRate = 0.8f;
	
	// Dense smoke particle count
	smokeProperty.m_Property.m_dwParticles = static_cast<DWORD>(40 * pInstance->sConfig.fIntensity);
	smokeProperty.m_Property.m_fEmissionRate = 80.0f * pInstance->sConfig.fIntensity;
	
	// Large smoke billows
	smokeProperty.m_Property.m_vSize = D3DXVECTOR2(0.3f, 0.8f) * pInstance->sConfig.fScale;
	smokeProperty.m_Property.m_vSizeRate = D3DXVECTOR2(2.5f, 2.0f);
	smokeProperty.m_Property.m_vSizeStart = D3DXVECTOR2(0.1f, 0.2f);
	smokeProperty.m_Property.m_vSizeEnd = D3DXVECTOR2(2.0f, 3.0f);
	
	// Long-lasting smoke
	smokeProperty.m_Property.m_vLife = D3DXVECTOR2(2.0f, 4.0f);
	smokeProperty.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.2f);
	
	// Slow upward and outward velocity
	smokeProperty.m_Property.m_vVelocity = D3DXVECTOR3(2.0f, 4.0f, 2.0f) * pInstance->sConfig.fIntensity;
	smokeProperty.m_Property.m_vVelocityRate = D3DXVECTOR3(1.5f, 0.8f, 1.5f);
	
	// Dark smoke color
	smokeProperty.m_Property.m_cColorStart = D3DCOLOR_ARGB(150, 80, 70, 60);
	smokeProperty.m_Property.m_cColorVar = D3DCOLOR_ARGB(80, 40, 30, 30);
	smokeProperty.m_Property.m_cColorEnd = D3DCOLOR_ARGB(0, 40, 35, 30);
	
	// Slow rotation for natural smoke motion
	smokeProperty.m_Property.m_vRotation = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f);
	smokeProperty.m_Property.m_vRotationRate = D3DXVECTOR3(0.0f, 0.0f, 0.3f);
	
	// Smoke cloud texture
	smokeProperty.m_Property.m_szTexture = "particle\\smoke_cloud.dds";
	smokeProperty.m_Property.m_nBlend = 1; // Alpha blending for smoke
	smokeProperty.m_Property.m_nPower = static_cast<int>(70 * pInstance->sConfig.fIntensity);
	
	D3DXVECTOR3 upDir(0.0f, 1.0f, 0.0f);
	DxEffSingle* pSmokeEffect = DxEffSingleMan::GetInstance().CreateEffSingle(
		PARTICLESYS_PROPERTY::TYPEID,
		&smokeProperty,
		&pInstance->vPosition,
		&upDir,
		nullptr
	);
}

// Helper function to create flying debris
void KillAnimationManager::CreateExplosionDebrisEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return;
	
	// Create flying debris particles
	PARTICLESYS_PROPERTY debrisProperty;
	
	// Debris physics with gravity
	debrisProperty.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	debrisProperty.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, -9.8f, 0.0f);
	debrisProperty.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, -19.6f, 0.0f);
	
	debrisProperty.m_Property.m_vPlayTime = D3DXVECTOR2(0.0f, 0.6f);
	debrisProperty.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.08f);
	
	debrisProperty.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER | PARTICLE_FLAG_MESH;
	debrisProperty.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD;
	
	// Wide debris scatter area
	debrisProperty.m_Property.m_vRange = D3DXVECTOR3(1.8f, 1.0f, 1.8f) * pInstance->sConfig.fScale;
	debrisProperty.m_Property.m_fRangeRate = 1.0f;
	
	// Moderate debris count
	debrisProperty.m_Property.m_dwParticles = static_cast<DWORD>(20 * pInstance->sConfig.fIntensity);
	debrisProperty.m_Property.m_fEmissionRate = 40.0f * pInstance->sConfig.fIntensity;
	
	// Small to medium debris pieces
	debrisProperty.m_Property.m_vSize = D3DXVECTOR2(0.05f, 0.15f) * pInstance->sConfig.fScale;
	debrisProperty.m_Property.m_vSizeRate = D3DXVECTOR2(1.0f, 1.2f);
	debrisProperty.m_Property.m_vSizeStart = D3DXVECTOR2(0.03f, 0.08f);
	debrisProperty.m_Property.m_vSizeEnd = D3DXVECTOR2(0.15f, 0.25f);
	
	// Medium duration for debris flight
	debrisProperty.m_Property.m_vLife = D3DXVECTOR2(1.0f, 2.5f);
	debrisProperty.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.4f);
	
	// High initial velocity in all directions
	debrisProperty.m_Property.m_vVelocity = D3DXVECTOR3(10.0f, 8.0f, 10.0f) * pInstance->sConfig.fIntensity;
	debrisProperty.m_Property.m_vVelocityRate = D3DXVECTOR3(2.0f, 1.5f, 2.0f);
	
	// Dark debris color
	debrisProperty.m_Property.m_cColorStart = D3DCOLOR_ARGB(255, 100, 80, 60);
	debrisProperty.m_Property.m_cColorVar = D3DCOLOR_ARGB(100, 50, 40, 30);
	debrisProperty.m_Property.m_cColorEnd = D3DCOLOR_ARGB(200, 80, 60, 40);
	
	// Tumbling rotation
	debrisProperty.m_Property.m_vRotation = D3DXVECTOR3(D3DX_PI, D3DX_PI, D3DX_PI);
	debrisProperty.m_Property.m_vRotationRate = D3DXVECTOR3(3.0f, 3.0f, 3.0f);
	
	// Debris chunk texture
	debrisProperty.m_Property.m_szTexture = "particle\\debris_chunk.dds";
	debrisProperty.m_Property.m_nBlend = 1; // Alpha blending
	debrisProperty.m_Property.m_nPower = static_cast<int>(90 * pInstance->sConfig.fIntensity);
	
	D3DXVECTOR3 upDir(0.0f, 1.0f, 0.0f);
	DxEffSingle* pDebrisEffect = DxEffSingleMan::GetInstance().CreateEffSingle(
		PARTICLESYS_PROPERTY::TYPEID,
		&debrisProperty,
		&pInstance->vPosition,
		&upDir,
		nullptr
	);
}

// Helper function to create screen shake effect
void KillAnimationManager::CreateExplosionScreenEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	// This would integrate with the screen effect system for camera shake
	// Implementation would depend on the camera/viewport system
	
	/*
	// Example screen shake implementation:
	CameraManager::GetInstance().AddScreenShake(
		pInstance->sConfig.fIntensity * 0.5f,  // Shake intensity
		0.3f,                                   // Shake duration
		CameraManager::SHAKE_TYPE_RANDOM       // Shake pattern
	);
	
	// Example screen flash for explosion
	ScreenEffectManager::GetInstance().CreateFlash(
		D3DCOLOR_ARGB(120, 255, 200, 100),    // Flash color
		0.15f,                                  // Flash duration
		ScreenEffectManager::FLASH_TYPE_ADDITIVE
	);
	*/
}