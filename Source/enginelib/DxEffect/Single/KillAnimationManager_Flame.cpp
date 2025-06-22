#include "StdAfx.h"
#include "KillAnimationManager.h"
#include "DxEffectParticleSys.h"
#include "DxEffSingleMan.h"
#include "../../DxTools/DxMethods.h"
#include "../../../SigmaCore/DebugInclude.h"

// Flame Effect Implementation - 烈焰焚燒
// Creates intense fire burst with burning embers and heat waves
DxEffSingle* KillAnimationManager::CreateFlameEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return nullptr;
	
	// Create main flame particle system
	PARTICLESYS_PROPERTY property;
	
	// Set flame physics properties
	property.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	property.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	property.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	
	property.m_Property.m_vPlayTime = D3DXVECTOR2(0.0f, pInstance->sConfig.fDuration);
	property.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.08f);
	
	// Intense fire flags
	property.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER | PARTICLE_FLAG_MESH;
	property.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD | PARTICLE_FLAG_EX_REMAIN;
	
	// Tall flame column area
	property.m_Property.m_vRange = D3DXVECTOR3(1.0f, 2.0f, 1.0f) * pInstance->sConfig.fScale;
	property.m_Property.m_fRangeRate = 0.8f;
	property.m_Property.m_fRotateAngel = D3DX_PI * 0.3f;
	
	// Dense flame particle count
	property.m_Property.m_dwParticles = static_cast<DWORD>(70 * pInstance->sConfig.fIntensity);
	property.m_Property.m_fEmissionRate = 140.0f * pInstance->sConfig.fIntensity;
	
	// Large, dancing flame sizes
	property.m_Property.m_vSize = D3DXVECTOR2(0.2f, 0.5f) * pInstance->sConfig.fScale;
	property.m_Property.m_vSizeRate = D3DXVECTOR2(2.5f, 2.0f);
	property.m_Property.m_vSizeStart = D3DXVECTOR2(0.1f, 0.15f);
	property.m_Property.m_vSizeEnd = D3DXVECTOR2(1.2f, 1.8f);
	
	// Extended burning duration
	property.m_Property.m_vLife = D3DXVECTOR2(1.0f, 2.5f);
	property.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.4f);
	
	// Upward flame velocity with turbulence
	property.m_Property.m_vVelocity = D3DXVECTOR3(3.0f, 8.0f, 3.0f) * pInstance->sConfig.fIntensity;
	property.m_Property.m_vVelocityRate = D3DXVECTOR3(2.0f, 1.2f, 2.0f);
	
	// Fiery red-orange color scheme
	property.m_Property.m_cColorStart = D3DCOLOR_ARGB(255, 
		static_cast<int>(255 * pInstance->sConfig.vColor.x),
		static_cast<int>(255 * pInstance->sConfig.vColor.y * 0.3f),
		static_cast<int>(255 * pInstance->sConfig.vColor.z * 0.1f));
	
	property.m_Property.m_cColorVar = D3DCOLOR_ARGB(120, 80, 40, 10);
	property.m_Property.m_cColorEnd = D3DCOLOR_ARGB(0, 200, 100, 20);
	
	// Flickering flame rotation
	property.m_Property.m_vRotation = D3DXVECTOR3(D3DX_PI * 0.2f, D3DX_PI * 0.2f, D3DX_PI);
	property.m_Property.m_vRotationRate = D3DXVECTOR3(1.5f, 1.5f, 2.0f);
	
	// Fire texture
	property.m_Property.m_szTexture = "particle\\fire_flame.dds";
	property.m_Property.m_nBlend = 2; // Additive blending for bright flames
	property.m_Property.m_nPower = static_cast<int>(200 * pInstance->sConfig.fIntensity);
	
	// Animated flame sequence
	property.m_Property.m_dwSequenceFlag = SEQUENCE_FLAG_RANDOM;
	property.m_Property.m_nCol = 4;
	property.m_Property.m_nRow = 4;
	property.m_Property.m_fSequenceSpeed = 18.0f;
	
	// Create the main flame effect
	DxEffSingle* pEffect = DxEffSingleMan::GetInstance().CreateEffSingle(
		PARTICLESYS_PROPERTY::TYPEID,
		&property,
		&pInstance->vPosition,
		&pInstance->vDirection,
		nullptr
	);
	
	if (pEffect)
	{
		// Set transformation matrix for upward flame
		D3DXMATRIX matFlame;
		D3DXMatrixTranslation(&matFlame, pInstance->vPosition.x, pInstance->vPosition.y, pInstance->vPosition.z);
		pEffect->SetMatrix(matFlame);
		
		// Create additional flame effects
		CreateFlameEmberEffect(pInstance);
		CreateFlameHeatWaveEffect(pInstance);
		CreateFlameSmokeEffect(pInstance);
	}
	
	return pEffect;
}

// Helper function to create burning ember effect
void KillAnimationManager::CreateFlameEmberEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return;
	
	// Create floating ember particles
	PARTICLESYS_PROPERTY emberProperty;
	
	// Ember physics with buoyancy
	emberProperty.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	emberProperty.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	emberProperty.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, -3.0f, 0.0f);
	
	emberProperty.m_Property.m_vPlayTime = D3DXVECTOR2(0.2f, pInstance->sConfig.fDuration);
	emberProperty.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.15f);
	
	emberProperty.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER;
	emberProperty.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD;
	
	// Wide ember distribution
	emberProperty.m_Property.m_vRange = D3DXVECTOR3(1.5f, 1.0f, 1.5f) * pInstance->sConfig.fScale;
	emberProperty.m_Property.m_fRangeRate = 0.9f;
	
	// Moderate ember count
	emberProperty.m_Property.m_dwParticles = static_cast<DWORD>(40 * pInstance->sConfig.fIntensity);
	emberProperty.m_Property.m_fEmissionRate = 80.0f * pInstance->sConfig.fIntensity;
	
	// Small, glowing ember sizes
	emberProperty.m_Property.m_vSize = D3DXVECTOR2(0.02f, 0.06f) * pInstance->sConfig.fScale;
	emberProperty.m_Property.m_vSizeRate = D3DXVECTOR2(1.5f, 1.2f);
	emberProperty.m_Property.m_vSizeStart = D3DXVECTOR2(0.01f, 0.02f);
	emberProperty.m_Property.m_vSizeEnd = D3DXVECTOR2(0.08f, 0.12f);
	
	// Long ember flight time
	emberProperty.m_Property.m_vLife = D3DXVECTOR2(2.0f, 4.0f);
	emberProperty.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.3f);
	
	// Floating upward velocity
	emberProperty.m_Property.m_vVelocity = D3DXVECTOR3(2.0f, 5.0f, 2.0f) * pInstance->sConfig.fIntensity;
	emberProperty.m_Property.m_vVelocityRate = D3DXVECTOR3(1.8f, 1.0f, 1.8f);
	
	// Bright orange-red embers
	emberProperty.m_Property.m_cColorStart = D3DCOLOR_ARGB(255, 255, 150, 50);
	emberProperty.m_Property.m_cColorVar = D3DCOLOR_ARGB(100, 50, 30, 20);
	emberProperty.m_Property.m_cColorEnd = D3DCOLOR_ARGB(0, 150, 80, 20);
	
	// Gentle tumbling rotation
	emberProperty.m_Property.m_vRotation = D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI * 0.5f, D3DX_PI * 0.5f);
	emberProperty.m_Property.m_vRotationRate = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	
	// Ember spark texture
	emberProperty.m_Property.m_szTexture = "particle\\fire_ember.dds";
	emberProperty.m_Property.m_nBlend = 2; // Additive for glowing effect
	emberProperty.m_Property.m_nPower = static_cast<int>(120 * pInstance->sConfig.fIntensity);
	
	D3DXVECTOR3 upDir(0.0f, 1.0f, 0.0f);
	DxEffSingle* pEmberEffect = DxEffSingleMan::GetInstance().CreateEffSingle(
		PARTICLESYS_PROPERTY::TYPEID,
		&emberProperty,
		&pInstance->vPosition,
		&upDir,
		nullptr
	);
}

// Helper function to create heat wave distortion effect
void KillAnimationManager::CreateFlameHeatWaveEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return;
	
	// Create heat distortion particles
	PARTICLESYS_PROPERTY heatProperty;
	
	// Heat wave physics
	heatProperty.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	heatProperty.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	heatProperty.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	
	heatProperty.m_Property.m_vPlayTime = D3DXVECTOR2(0.1f, pInstance->sConfig.fDuration * 0.8f);
	heatProperty.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.1f);
	
	heatProperty.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER;
	heatProperty.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD;
	
	// Tall heat column
	heatProperty.m_Property.m_vRange = D3DXVECTOR3(0.8f, 2.5f, 0.8f) * pInstance->sConfig.fScale;
	heatProperty.m_Property.m_fRangeRate = 0.6f;
	
	// Fewer heat distortion particles
	heatProperty.m_Property.m_dwParticles = static_cast<DWORD>(20 * pInstance->sConfig.fIntensity);
	heatProperty.m_Property.m_fEmissionRate = 40.0f * pInstance->sConfig.fIntensity;
	
	// Large, transparent heat waves
	heatProperty.m_Property.m_vSize = D3DXVECTOR2(0.5f, 1.0f) * pInstance->sConfig.fScale;
	heatProperty.m_Property.m_vSizeRate = D3DXVECTOR2(2.0f, 1.8f);
	heatProperty.m_Property.m_vSizeStart = D3DXVECTOR2(0.2f, 0.4f);
	heatProperty.m_Property.m_vSizeEnd = D3DXVECTOR2(1.5f, 2.0f);
	
	// Medium heat wave duration
	heatProperty.m_Property.m_vLife = D3DXVECTOR2(1.5f, 2.5f);
	heatProperty.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.2f);
	
	// Steady upward flow
	heatProperty.m_Property.m_vVelocity = D3DXVECTOR3(1.0f, 6.0f, 1.0f) * pInstance->sConfig.fIntensity;
	heatProperty.m_Property.m_vVelocityRate = D3DXVECTOR3(0.8f, 0.5f, 0.8f);
	
	// Semi-transparent heat shimmer
	heatProperty.m_Property.m_cColorStart = D3DCOLOR_ARGB(80, 255, 200, 150);
	heatProperty.m_Property.m_cColorVar = D3DCOLOR_ARGB(40, 30, 30, 20);
	heatProperty.m_Property.m_cColorEnd = D3DCOLOR_ARGB(0, 200, 150, 100);
	
	// Slight wavering motion
	heatProperty.m_Property.m_vRotation = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.1f);
	heatProperty.m_Property.m_vRotationRate = D3DXVECTOR3(0.0f, 0.0f, 0.5f);
	
	// Heat distortion texture
	heatProperty.m_Property.m_szTexture = "particle\\heat_wave.dds";
	heatProperty.m_Property.m_nBlend = 1; // Alpha blending for transparency
	heatProperty.m_Property.m_nPower = static_cast<int>(60 * pInstance->sConfig.fIntensity);
	
	D3DXVECTOR3 upDir(0.0f, 1.0f, 0.0f);
	DxEffSingle* pHeatEffect = DxEffSingleMan::GetInstance().CreateEffSingle(
		PARTICLESYS_PROPERTY::TYPEID,
		&heatProperty,
		&pInstance->vPosition,
		&upDir,
		nullptr
	);
}

// Helper function to create flame smoke effect
void KillAnimationManager::CreateFlameSmokeEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return;
	
	// Create dark smoke from combustion
	PARTICLESYS_PROPERTY smokeProperty;
	
	// Smoke physics
	smokeProperty.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	smokeProperty.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	smokeProperty.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, 0.5f, 0.0f);
	
	smokeProperty.m_Property.m_vPlayTime = D3DXVECTOR2(0.5f, pInstance->sConfig.fDuration);
	smokeProperty.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.2f);
	
	smokeProperty.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER;
	smokeProperty.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD;
	
	// Expanding smoke area
	smokeProperty.m_Property.m_vRange = D3DXVECTOR3(1.2f, 1.5f, 1.2f) * pInstance->sConfig.fScale;
	smokeProperty.m_Property.m_fRangeRate = 0.7f;
	
	// Dense smoke particles
	smokeProperty.m_Property.m_dwParticles = static_cast<DWORD>(35 * pInstance->sConfig.fIntensity);
	smokeProperty.m_Property.m_fEmissionRate = 70.0f * pInstance->sConfig.fIntensity;
	
	// Large smoke billows
	smokeProperty.m_Property.m_vSize = D3DXVECTOR2(0.4f, 0.8f) * pInstance->sConfig.fScale;
	smokeProperty.m_Property.m_vSizeRate = D3DXVECTOR2(3.0f, 2.5f);
	smokeProperty.m_Property.m_vSizeStart = D3DXVECTOR2(0.2f, 0.3f);
	smokeProperty.m_Property.m_vSizeEnd = D3DXVECTOR2(2.5f, 3.5f);
	
	// Long smoke duration
	smokeProperty.m_Property.m_vLife = D3DXVECTOR2(3.0f, 5.0f);
	smokeProperty.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.3f);
	
	// Upward drift with dispersion
	smokeProperty.m_Property.m_vVelocity = D3DXVECTOR3(2.5f, 4.0f, 2.5f) * pInstance->sConfig.fIntensity;
	smokeProperty.m_Property.m_vVelocityRate = D3DXVECTOR3(1.5f, 0.8f, 1.5f);
	
	// Dark smoke color
	smokeProperty.m_Property.m_cColorStart = D3DCOLOR_ARGB(140, 60, 50, 40);
	smokeProperty.m_Property.m_cColorVar = D3DCOLOR_ARGB(60, 30, 25, 20);
	smokeProperty.m_Property.m_cColorEnd = D3DCOLOR_ARGB(0, 40, 35, 25);
	
	// Natural smoke swirling
	smokeProperty.m_Property.m_vRotation = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.3f);
	smokeProperty.m_Property.m_vRotationRate = D3DXVECTOR3(0.0f, 0.0f, 0.4f);
	
	// Smoke texture
	smokeProperty.m_Property.m_szTexture = "particle\\dark_smoke.dds";
	smokeProperty.m_Property.m_nBlend = 1; // Alpha blending
	smokeProperty.m_Property.m_nPower = static_cast<int>(70 * pInstance->sConfig.fIntensity);
	
	// Offset smoke position slightly upward
	D3DXVECTOR3 smokePos = pInstance->vPosition + D3DXVECTOR3(0.0f, 0.5f * pInstance->sConfig.fScale, 0.0f);
	D3DXVECTOR3 upDir(0.0f, 1.0f, 0.0f);
	
	DxEffSingle* pSmokeEffect = DxEffSingleMan::GetInstance().CreateEffSingle(
		PARTICLESYS_PROPERTY::TYPEID,
		&smokeProperty,
		&smokePos,
		&upDir,
		nullptr
	);
}