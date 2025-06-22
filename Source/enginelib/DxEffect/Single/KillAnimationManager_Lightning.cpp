#include "StdAfx.h"
#include "KillAnimationManager.h"
#include "DxEffectParticleSys.h"
#include "DxEffSingleMan.h"
#include "../../DxTools/DxMethods.h"
#include "../../../SigmaCore/DebugInclude.h"

// Lightning Effect Implementation - 雷電轟擊
// Creates electric lightning bolt with branching arcs and electric discharge
DxEffSingle* KillAnimationManager::CreateLightningEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return nullptr;
	
	// Create main lightning bolt particle system
	PARTICLESYS_PROPERTY property;
	
	// Set lightning physics properties
	property.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	property.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	property.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	property.m_Property.m_vPlayTime = D3DXVECTOR2(0.0f, pInstance->sConfig.fDuration * 0.3f); // Quick flash
	property.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.02f);
	
	// Electric discharge flags
	property.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER | PARTICLE_FLAG_MESH;
	property.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD | PARTICLE_FLAG_EX_MOVE_PARENT;
	
	// Vertical lightning bolt area
	property.m_Property.m_vRange = D3DXVECTOR3(0.2f, 3.0f, 0.2f) * pInstance->sConfig.fScale;
	property.m_Property.m_fRangeRate = 0.95f;
	property.m_Property.m_fRotateAngel = 0.0f; // Straight bolt
	
	// Moderate particle count for clean bolt
	property.m_Property.m_dwParticles = static_cast<DWORD>(25 * pInstance->sConfig.fIntensity);
	property.m_Property.m_fEmissionRate = 100.0f * pInstance->sConfig.fIntensity;
	
	// Thin, bright lightning segments
	property.m_Property.m_vSize = D3DXVECTOR2(0.08f, 0.6f) * pInstance->sConfig.fScale;
	property.m_Property.m_vSizeRate = D3DXVECTOR2(1.5f, 1.2f);
	property.m_Property.m_vSizeStart = D3DXVECTOR2(0.05f, 0.3f);
	property.m_Property.m_vSizeEnd = D3DXVECTOR2(0.15f, 0.8f);
	
	// Very short duration for electric flash
	property.m_Property.m_vLife = D3DXVECTOR2(0.05f, 0.15f);
	property.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.2f);
	
	// Instantaneous electric velocity
	property.m_Property.m_vVelocity = D3DXVECTOR3(0.0f, -20.0f, 0.0f) * pInstance->sConfig.fIntensity;
	property.m_Property.m_vVelocityRate = D3DXVECTOR3(0.2f, 0.8f, 0.2f);
	
	// Brilliant electric white-blue color
	property.m_Property.m_cColorStart = D3DCOLOR_ARGB(255, 
		static_cast<int>(255 * pInstance->sConfig.vColor.x * 0.9f),
		static_cast<int>(255 * pInstance->sConfig.vColor.y * 0.9f),
		static_cast<int>(255 * pInstance->sConfig.vColor.z));
	
	property.m_Property.m_cColorVar = D3DCOLOR_ARGB(100, 30, 30, 50);
	property.m_Property.m_cColorEnd = D3DCOLOR_ARGB(0, 200, 200, 255);
	
	// No rotation for straight bolt
	property.m_Property.m_vRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	property.m_Property.m_vRotationRate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	// Lightning bolt texture
	property.m_Property.m_szTexture = "particle\\lightning_bolt.dds";
	property.m_Property.m_nBlend = 2; // Additive for bright electric effect
	property.m_Property.m_nPower = static_cast<int>(250 * pInstance->sConfig.fIntensity);
	
	// Linear sequence for bolt animation
	property.m_Property.m_dwSequenceFlag = SEQUENCE_FLAG_LINEAR;
	property.m_Property.m_nCol = 2;
	property.m_Property.m_nRow = 8;
	property.m_Property.m_fSequenceSpeed = 30.0f;
	
	// Create the main lightning effect
	DxEffSingle* pEffect = DxEffSingleMan::GetInstance().CreateEffSingle(
		PARTICLESYS_PROPERTY::TYPEID,
		&property,
		&pInstance->vPosition,
		&pInstance->vDirection,
		nullptr
	);
	
	if (pEffect)
	{
		// Set transformation for downward lightning bolt
		D3DXMATRIX matLightning;
		D3DXMatrixTranslation(&matLightning, pInstance->vPosition.x, pInstance->vPosition.y, pInstance->vPosition.z);
		pEffect->SetMatrix(matLightning);
		
		// Create additional lightning effects
		CreateLightningBranchEffect(pInstance);
		CreateLightningDischargeEffect(pInstance);
		CreateLightningImpactEffect(pInstance);
		
		// Create screen flash for dramatic effect
		if (pInstance->sConfig.bUseScreenEffect)
		{
			CreateLightningScreenEffect(pInstance);
		}
	}
	
	return pEffect;
}

// Helper function to create branching lightning arcs
void KillAnimationManager::CreateLightningBranchEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return;
	
	// Create smaller branching arcs
	PARTICLESYS_PROPERTY branchProperty;
	
	// Branch arc physics
	branchProperty.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	branchProperty.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	branchProperty.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	branchProperty.m_Property.m_vPlayTime = D3DXVECTOR2(0.02f, pInstance->sConfig.fDuration * 0.4f);
	branchProperty.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.03f);
	
	branchProperty.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER;
	branchProperty.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD;
	
	// Radiating branch area
	branchProperty.m_Property.m_vRange = D3DXVECTOR3(2.0f, 1.5f, 2.0f) * pInstance->sConfig.fScale;
	branchProperty.m_Property.m_fRangeRate = 1.0f;
	
	// Fewer branch particles
	branchProperty.m_Property.m_dwParticles = static_cast<DWORD>(15 * pInstance->sConfig.fIntensity);
	branchProperty.m_Property.m_fEmissionRate = 50.0f * pInstance->sConfig.fIntensity;
	
	// Thinner, shorter branch arcs
	branchProperty.m_Property.m_vSize = D3DXVECTOR2(0.03f, 0.3f) * pInstance->sConfig.fScale;
	branchProperty.m_Property.m_vSizeRate = D3DXVECTOR2(1.2f, 1.5f);
	branchProperty.m_Property.m_vSizeStart = D3DXVECTOR2(0.02f, 0.1f);
	branchProperty.m_Property.m_vSizeEnd = D3DXVECTOR2(0.08f, 0.5f);
	
	// Quick branch flash
	branchProperty.m_Property.m_vLife = D3DXVECTOR2(0.03f, 0.1f);
	branchProperty.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.3f);
	
	// Radiating branch velocity
	branchProperty.m_Property.m_vVelocity = D3DXVECTOR3(15.0f, 8.0f, 15.0f) * pInstance->sConfig.fIntensity;
	branchProperty.m_Property.m_vVelocityRate = D3DXVECTOR3(2.0f, 1.0f, 2.0f);
	
	// Bright electric blue branches
	branchProperty.m_Property.m_cColorStart = D3DCOLOR_ARGB(200, 180, 180, 255);
	branchProperty.m_Property.m_cColorVar = D3DCOLOR_ARGB(80, 40, 40, 50);
	branchProperty.m_Property.m_cColorEnd = D3DCOLOR_ARGB(0, 100, 100, 200);
	
	// Random branch angles
	branchProperty.m_Property.m_vRotation = D3DXVECTOR3(D3DX_PI, D3DX_PI, D3DX_PI);
	branchProperty.m_Property.m_vRotationRate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	// Lightning arc texture
	branchProperty.m_Property.m_szTexture = "particle\\lightning_arc.dds";
	branchProperty.m_Property.m_nBlend = 2; // Additive
	branchProperty.m_Property.m_nPower = static_cast<int>(180 * pInstance->sConfig.fIntensity);
	
	// Calculate branch position along main bolt
	D3DXVECTOR3 branchPos = pInstance->vPosition + D3DXVECTOR3(0.0f, -0.8f * pInstance->sConfig.fScale, 0.0f);
	D3DXVECTOR3 radialDir(1.0f, 0.0f, 0.0f);
	
	DxEffSingle* pBranchEffect = DxEffSingleMan::GetInstance().CreateEffSingle(
		PARTICLESYS_PROPERTY::TYPEID,
		&branchProperty,
		&branchPos,
		&radialDir,
		nullptr
	);
}

// Helper function to create electric discharge sparks
void KillAnimationManager::CreateLightningDischargeEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return;
	
	// Create electric discharge particles
	PARTICLESYS_PROPERTY dischargeProperty;
	
	// Discharge physics
	dischargeProperty.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	dischargeProperty.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	dischargeProperty.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	dischargeProperty.m_Property.m_vPlayTime = D3DXVECTOR2(0.0f, 0.2f);
	dischargeProperty.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.01f);
	
	dischargeProperty.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER;
	dischargeProperty.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD;
	
	// Wide discharge area
	dischargeProperty.m_Property.m_vRange = D3DXVECTOR3(1.5f, 0.5f, 1.5f) * pInstance->sConfig.fScale;
	dischargeProperty.m_Property.m_fRangeRate = 1.0f;
	
	// Many small discharge sparks
	dischargeProperty.m_Property.m_dwParticles = static_cast<DWORD>(30 * pInstance->sConfig.fIntensity);
	dischargeProperty.m_Property.m_fEmissionRate = 120.0f * pInstance->sConfig.fIntensity;
	
	// Small, intense discharge sparks
	dischargeProperty.m_Property.m_vSize = D3DXVECTOR2(0.01f, 0.03f) * pInstance->sConfig.fScale;
	dischargeProperty.m_Property.m_vSizeRate = D3DXVECTOR2(1.0f, 1.5f);
	dischargeProperty.m_Property.m_vSizeStart = D3DXVECTOR2(0.005f, 0.01f);
	dischargeProperty.m_Property.m_vSizeEnd = D3DXVECTOR2(0.05f, 0.08f);
	
	// Very quick discharge flash
	dischargeProperty.m_Property.m_vLife = D3DXVECTOR2(0.02f, 0.08f);
	dischargeProperty.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.2f);
	
	// High-speed radial discharge
	dischargeProperty.m_Property.m_vVelocity = D3DXVECTOR3(25.0f, 10.0f, 25.0f) * pInstance->sConfig.fIntensity;
	dischargeProperty.m_Property.m_vVelocityRate = D3DXVECTOR3(3.0f, 2.0f, 3.0f);
	
	// Brilliant white-electric sparks
	dischargeProperty.m_Property.m_cColorStart = D3DCOLOR_ARGB(255, 255, 255, 255);
	dischargeProperty.m_Property.m_cColorVar = D3DCOLOR_ARGB(100, 50, 50, 80);
	dischargeProperty.m_Property.m_cColorEnd = D3DCOLOR_ARGB(0, 150, 150, 255);
	
	// Electric spark texture
	dischargeProperty.m_Property.m_szTexture = "particle\\electric_spark.dds";
	dischargeProperty.m_Property.m_nBlend = 2; // Additive
	dischargeProperty.m_Property.m_nPower = static_cast<int>(200 * pInstance->sConfig.fIntensity);
	
	D3DXVECTOR3 upDir(0.0f, 1.0f, 0.0f);
	DxEffSingle* pDischargeEffect = DxEffSingleMan::GetInstance().CreateEffSingle(
		PARTICLESYS_PROPERTY::TYPEID,
		&dischargeProperty,
		&pInstance->vPosition,
		&upDir,
		nullptr
	);
}

// Helper function to create lightning ground impact
void KillAnimationManager::CreateLightningImpactEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	if (!pInstance || !m_pd3dDevice)
		return;
	
	// Create ground impact effect
	PARTICLESYS_PROPERTY impactProperty;
	
	// Impact physics
	impactProperty.m_Property.m_vGVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	impactProperty.m_Property.m_vGGravityStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	impactProperty.m_Property.m_vGGravityEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	impactProperty.m_Property.m_vPlayTime = D3DXVECTOR2(0.0f, 0.3f);
	impactProperty.m_Property.m_vSleepTime = D3DXVECTOR2(0.0f, 0.02f);
	
	impactProperty.m_Property.m_dwFlag = PARTICLE_FLAG_WEATHER;
	impactProperty.m_Property.m_dwFlagEX = PARTICLE_FLAG_EX_BILLBORAD | PARTICLE_FLAG_EX_GROUND;
	
	// Circular impact area
	impactProperty.m_Property.m_vRange = D3DXVECTOR3(1.5f, 0.2f, 1.5f) * pInstance->sConfig.fScale;
	impactProperty.m_Property.m_fRangeRate = 1.0f;
	
	// Impact ring particles
	impactProperty.m_Property.m_dwParticles = static_cast<DWORD>(12 * pInstance->sConfig.fIntensity);
	impactProperty.m_Property.m_fEmissionRate = 40.0f * pInstance->sConfig.fIntensity;
	
	// Large impact ring
	impactProperty.m_Property.m_vSize = D3DXVECTOR2(0.8f, 0.8f) * pInstance->sConfig.fScale;
	impactProperty.m_Property.m_vSizeRate = D3DXVECTOR2(4.0f, 4.0f);
	impactProperty.m_Property.m_vSizeStart = D3DXVECTOR2(0.2f, 0.2f);
	impactProperty.m_Property.m_vSizeEnd = D3DXVECTOR2(4.0f, 4.0f);
	
	// Quick expansion
	impactProperty.m_Property.m_vLife = D3DXVECTOR2(0.2f, 0.4f);
	impactProperty.m_Property.m_vLifeRate = D3DXVECTOR2(1.0f, 1.0f);
	
	// Radial expansion
	impactProperty.m_Property.m_vVelocity = D3DXVECTOR3(10.0f, 0.0f, 10.0f) * pInstance->sConfig.fIntensity;
	impactProperty.m_Property.m_vVelocityRate = D3DXVECTOR3(1.5f, 0.0f, 1.5f);
	
	// Electric blue impact ring
	impactProperty.m_Property.m_cColorStart = D3DCOLOR_ARGB(220, 150, 150, 255);
	impactProperty.m_Property.m_cColorVar = D3DCOLOR_ARGB(80, 50, 50, 60);
	impactProperty.m_Property.m_cColorEnd = D3DCOLOR_ARGB(0, 80, 80, 180);
	
	// Electric impact ring texture
	impactProperty.m_Property.m_szTexture = "particle\\electric_ring.dds";
	impactProperty.m_Property.m_nBlend = 2; // Additive
	impactProperty.m_Property.m_nPower = static_cast<int>(170 * pInstance->sConfig.fIntensity);
	
	// Calculate ground impact position
	D3DXVECTOR3 impactPos = pInstance->vPosition + D3DXVECTOR3(0.0f, -2.5f * pInstance->sConfig.fScale, 0.0f);
	D3DXVECTOR3 upDir(0.0f, 1.0f, 0.0f);
	
	DxEffSingle* pImpactEffect = DxEffSingleMan::GetInstance().CreateEffSingle(
		PARTICLESYS_PROPERTY::TYPEID,
		&impactProperty,
		&impactPos,
		&upDir,
		nullptr
	);
}

// Helper function to create screen flash effect
void KillAnimationManager::CreateLightningScreenEffect(KILL_ANIMATION_INSTANCE* pInstance)
{
	// This would integrate with the screen effect system for lightning flash
	// Implementation would depend on the screen effect system
	
	/*
	// Example lightning screen flash implementation:
	ScreenEffectManager::GetInstance().CreateFlash(
		D3DCOLOR_ARGB(150, 255, 255, 255),    // Bright white flash
		0.1f,                                  // Very quick flash
		ScreenEffectManager::FLASH_TYPE_ADDITIVE
	);
	
	// Example screen shake for thunder
	CameraManager::GetInstance().AddScreenShake(
		pInstance->sConfig.fIntensity * 0.3f,  // Moderate shake
		0.2f,                                   // Brief shake duration
		CameraManager::SHAKE_TYPE_QUICK        // Quick shake pattern
	);
	*/
}