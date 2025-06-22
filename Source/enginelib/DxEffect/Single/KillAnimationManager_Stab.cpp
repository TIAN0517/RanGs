#include "StdAfx.h"
#include "KillAnimationManager.h"
#include "DxEffectParticleSys.h"
#include "DxEffSingleMan.h"
#include "../../DxTools/DxMethods.h"
#include "../../../SigmaCore/DebugInclude.h"

// Stab Effect Implementation - 穿刺致命
// Creates a piercing thrust effect with blood spray and impact
DxEffSingle* KillAnimationManager::CreateStabEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return nullptr;
	
	// Create particle system for stab thrust
	PARTICLESYS_PROPERTY property;
	
	// Set basic particle properties for pierce effect
	property.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	property.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
	property.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, -8.0f, 0.0f);
	
	property.m_Property.m_vPlayTime = D3DXVECTOR2(0.0f, pInstance->sConfig.fDuration);
	property.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.05f);
	
	// Concentrated linear effect flags
	property.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER | PARTICLE_FLAG_MESH;
	property.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD | PARTICLE_FLAG_EX_MOVE_PARENT;
	
	// Narrow range for focused thrust
	property.m_Property.m_vRange = D3DXVECTOR3(0.1f, 0.1f, 2.0f) * pInstance->sConfig.fScale;
	property.m_Property.m_fRangeRate = 0.9f;
	property.m_Property.m_fRotateAngel = 0.0f; // No rotation for straight thrust
	
	// Moderate particle count for clean thrust line
	property.m_Property.m_dwParticles = static_cast<DWORD>(20 * pInstance->sConfig.fIntensity);
	property.m_Property.m_fEmissionRate = 40.0f * pInstance->sConfig.fIntensity;
	
	// Elongated particles for thrust trail
	property.m_Property.m_vSize = D3DXVECTOR2(0.05f, 0.4f) * pInstance->sConfig.fScale;
	property.m_Property.m_vSizeRate = D3DXVECTOR2(1.0f, 2.0f);
	property.m_Property.m_vSizeStart = D3DXVECTOR2(0.02f, 0.1f);
	property.m_Property.m_vSizeEnd = D3DXVECTOR2(0.1f, 0.8f);
	
	// Quick life time for sharp effect
	property.m_Property.m_vLife = D3DXVECTOR2(0.2f, 0.5f);
	property.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.3f);
	
	// High forward velocity for thrust
	property.m_Property.m_vVelocity = D3DXVECTOR3(0.0f, 0.0f, 12.0f) * pInstance->sConfig.fIntensity;
	property.m_Property.m_vVelocityRate = D3DXVECTOR3(0.1f, 0.1f, 0.8f);
	
	// Steel blue color scheme
	property.m_Property.m_cColorStart = D3DCOLOR_ARGB(255, 
		static_cast<int>(255 * pInstance->sConfig.vColor.x * 0.8f),
		static_cast<int>(255 * pInstance->sConfig.vColor.y * 0.8f),
		static_cast<int>(255 * pInstance->sConfig.vColor.z));
	
	property.m_Property.m_cColorVar = D3DCOLOR_ARGB(80, 30, 30, 40);
	property.m_Property.m_cColorEnd = D3DCOLOR_ARGB(0, 100, 100, 200);
	
	// No rotation for clean thrust
	property.m_Property.m_vRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	property.m_Property.m_vRotationRate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	// Piercing blade texture
	property.m_Property.m_szTexture = "particle\\blade_trail.dds";
	property.m_Property.m_nBlend = 2; // Additive blending
	property.m_Property.m_nPower = static_cast<int>(120 * pInstance->sConfig.fIntensity);
	
	// Linear sequence for thrust effect
	property.m_Property.m_dwSequenceFlag = SEQUENCE_FLAG_LINEAR;
	property.m_Property.m_nCol = 2;
	property.m_Property.m_nRow = 4;
	property.m_Property.m_fSequenceSpeed = 20.0f;
	
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
		// Set transformation for thrust direction
		D3DXMATRIX matStab;
		D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
		D3DXVECTOR3 vRight;
		D3DXVec3Cross(&vRight, &pInstance->vDirection, &vUp);
		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Cross(&vUp, &vRight, &pInstance->vDirection);
		
		// Create precise rotation matrix for thrust
		matStab._11 = vRight.x;    matStab._12 = vRight.y;    matStab._13 = vRight.z;    matStab._14 = 0.0f;
		matStab._21 = vUp.x;       matStab._22 = vUp.y;       matStab._23 = vUp.z;       matStab._24 = 0.0f;
		matStab._31 = pInstance->vDirection.x; matStab._32 = pInstance->vDirection.y; matStab._33 = pInstance->vDirection.z; matStab._34 = 0.0f;
		matStab._41 = pInstance->vPosition.x;  matStab._42 = pInstance->vPosition.y;  matStab._43 = pInstance->vPosition.z;  matStab._44 = 1.0f;
		
		pEffect->SetMatrix(matStab);
		
		// Create blood spray effect at impact point
		CreateStabBloodEffect(pInstance);
		
		// Create impact shockwave
		CreateStabImpactEffect(pInstance);
	}
	
	return pEffect;
}

// Helper function to create blood spray effect
void KillAnimationManager::CreateStabBloodEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return;
	
	// Create blood spray particles
	PARTICLESYS_PROPERTY bloodProperty;
	
	// Blood particle physics
	bloodProperty.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bloodProperty.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, -9.8f, 0.0f);
	bloodProperty.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, -15.0f, 0.0f);
	
	bloodProperty.m_Property.m_vPlayTime = D3DXVECTOR2(0.1f, 0.3f);
	bloodProperty.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.02f);
	
	bloodProperty.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER;
	bloodProperty.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD;
	
	// Spray cone area
	bloodProperty.m_Property.m_vRange = D3DXVECTOR3(0.5f, 0.5f, 0.3f) * pInstance->sConfig.fScale;
	bloodProperty.m_Property.m_fRangeRate = 0.7f;
	
	// More particles for dense blood spray
	bloodProperty.m_Property.m_dwParticles = static_cast<DWORD>(25 * pInstance->sConfig.fIntensity);
	bloodProperty.m_Property.m_fEmissionRate = 60.0f * pInstance->sConfig.fIntensity;
	
	// Variable blood drop sizes
	bloodProperty.m_Property.m_vSize = D3DXVECTOR2(0.03f, 0.08f) * pInstance->sConfig.fScale;
	bloodProperty.m_Property.m_vSizeRate = D3DXVECTOR2(1.2f, 1.5f);
	bloodProperty.m_Property.m_vSizeStart = D3DXVECTOR2(0.01f, 0.03f);
	bloodProperty.m_Property.m_vSizeEnd = D3DXVECTOR2(0.12f, 0.18f);
	
	// Moderate life for blood spray
	bloodProperty.m_Property.m_vLife = D3DXVECTOR2(0.3f, 0.8f);
	bloodProperty.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.4f);
	
	// Spray velocity in opposite direction of thrust
	D3DXVECTOR3 vSprayDir = -pInstance->vDirection;
	bloodProperty.m_Property.m_vVelocity = vSprayDir * 4.0f * pInstance->sConfig.fIntensity;
	bloodProperty.m_Property.m_vVelocityRate = D3DXVECTOR3(1.5f, 1.2f, 1.5f);
	
	// Dark red blood color
	bloodProperty.m_Property.m_cColorStart = D3DCOLOR_ARGB(200, 150, 20, 20);
	bloodProperty.m_Property.m_cColorVar = D3DCOLOR_ARGB(80, 50, 10, 10);
	bloodProperty.m_Property.m_cColorEnd = D3DCOLOR_ARGB(0, 80, 10, 10);
	
	// Blood droplet texture
	bloodProperty.m_Property.m_szTexture = "particle\\blood_drop.dds";
	bloodProperty.m_Property.m_nBlend = 1; // Alpha blending for blood
	bloodProperty.m_Property.m_nPower = static_cast<int>(80 * pInstance->sConfig.fIntensity);
	
	// Calculate impact position (end of thrust)
	D3DXVECTOR3 impactPos = pInstance->vPosition + pInstance->vDirection * 1.8f * pInstance->sConfig.fScale;
	
	DxEffSingle* pBloodEffect = DxEffSingleMan::GetInstance().CreateEffSingle(
		PARTICLESYS_PROPERTY::TYPEID,
		&bloodProperty,
		&impactPos,
		&vSprayDir,
		nullptr
	);
}

// Helper function to create impact shockwave effect
void KillAnimationManager::CreateStabImpactEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return;
	
	// Create impact ring effect
	PARTICLESYS_PROPERTY impactProperty;
	
	// Impact wave physics
	impactProperty.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	impactProperty.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	impactProperty.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	impactProperty.m_Property.m_vPlayTime = D3DXVECTOR2(0.0f, 0.2f);
	impactProperty.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.01f);
	
	impactProperty.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER;
	impactProperty.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD | PARTICLE_FLAG_EX_GROUND;
	
	// Circular impact area
	impactProperty.m_Property.m_vRange = D3DXVECTOR3(0.8f, 0.1f, 0.8f) * pInstance->sConfig.fScale;
	impactProperty.m_Property.m_fRangeRate = 1.0f;
	
	// Few particles for clean ring effect
	impactProperty.m_Property.m_dwParticles = static_cast<DWORD>(8 * pInstance->sConfig.fIntensity);
	impactProperty.m_Property.m_fEmissionRate = 20.0f * pInstance->sConfig.fIntensity;
	
	// Large ring particles
	impactProperty.m_Property.m_vSize = D3DXVECTOR2(0.5f, 0.5f) * pInstance->sConfig.fScale;
	impactProperty.m_Property.m_vSizeRate = D3DXVECTOR2(3.0f, 3.0f);
	impactProperty.m_Property.m_vSizeStart = D3DXVECTOR2(0.1f, 0.1f);
	impactProperty.m_Property.m_vSizeEnd = D3DXVECTOR2(2.0f, 2.0f);
	
	// Quick expansion
	impactProperty.m_Property.m_vLife = D3DXVECTOR2(0.15f, 0.25f);
	impactProperty.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.0f);
	
	// Radial velocity
	impactProperty.m_Property.m_vVelocity = D3DXVECTOR3(6.0f, 0.0f, 6.0f) * pInstance->sConfig.fIntensity;
	impactProperty.m_Property.m_vVelocityRate = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
	
	// Bright impact flash
	impactProperty.m_Property.m_cColorStart = D3DCOLOR_ARGB(180, 200, 200, 255);
	impactProperty.m_Property.m_cColorVar = D3DCOLOR_ARGB(50, 30, 30, 50);
	impactProperty.m_Property.m_cColorEnd = D3DCOLOR_ARGB(0, 100, 100, 150);
	
	// Impact ring texture
	impactProperty.m_Property.m_szTexture = "particle\\impact_ring.dds";
	impactProperty.m_Property.m_nBlend = 2; // Additive
	impactProperty.m_Property.m_nPower = static_cast<int>(140 * pInstance->sConfig.fIntensity);
	
	// Calculate impact position
	D3DXVECTOR3 impactPos = pInstance->vPosition + pInstance->vDirection * 1.8f * pInstance->sConfig.fScale;
	D3DXVECTOR3 upDir(0.0f, 1.0f, 0.0f);
	
	DxEffSingle* pImpactEffect = DxEffSingleMan::GetInstance().CreateEffSingle(
		PARTICLESYS_PROPERTY::TYPEID,
		&impactProperty,
		&impactPos,
		&upDir,
		nullptr
	);
}