#pragma once

#include <string>
#include <vector>
#include <map>
#include "../../../SigmaCore/Memory/ObjectPool.h"
#include "../../DxTools/Collision.h"
#include "../../DxSound/MovableSound.h"
#include "./DxEffSingle.h"
#include "./DxEffectParticleSys.h"

// Forward declarations
class DxEffSingleGroup;
struct D3DXVECTOR3;
struct D3DXMATRIX;

// Kill animation types
enum KILL_ANIMATION_TYPE
{
	KILL_ANIM_SLASH = 0,		// 劍氣砍殺
	KILL_ANIM_STAB,				// 穿刺致命
	KILL_ANIM_EXPLOSION,		// 爆炸轟殺
	KILL_ANIM_FREEZE,			// 冰凍粉碎
	KILL_ANIM_FLAME,			// 烈焰焚燒
	KILL_ANIM_LIGHTNING,		// 雷電轟擊
	KILL_ANIM_MAX
};

// Animation configuration parameters
struct KILL_ANIMATION_CONFIG
{
	float fIntensity;			// 動畫強度 (0.1f - 2.0f)
	D3DXVECTOR3 vColor;			// 顏色 (RGB 0.0f - 1.0f)
	float fDuration;			// 持續時間 (秒)
	float fScale;				// 縮放比例
	bool bUseSound;				// 是否播放音效
	bool bUseScreenEffect;		// 是否使用屏幕效果
	
	KILL_ANIMATION_CONFIG() 
		: fIntensity(1.0f)
		, vColor(1.0f, 1.0f, 1.0f)
		, fDuration(2.0f)
		, fScale(1.0f)
		, bUseSound(true)
		, bUseScreenEffect(true)
	{}
};

// Animation instance data
struct KILL_ANIMATION_INSTANCE
{
	KILL_ANIMATION_TYPE eType;
	D3DXVECTOR3 vPosition;
	D3DXVECTOR3 vDirection;
	KILL_ANIMATION_CONFIG sConfig;
	float fElapsedTime;
	bool bActive;
	DxEffSingle* pEffect;
	
	KILL_ANIMATION_INSTANCE()
		: eType(KILL_ANIM_SLASH)
		, vPosition(0.0f, 0.0f, 0.0f)
		, vDirection(0.0f, 0.0f, 1.0f)
		, fElapsedTime(0.0f)
		, bActive(false)
		, pEffect(nullptr)
	{}
};

// Kill Animation Manager Class
// 負責管理六大卡片動畫系統的核心類
class KillAnimationManager
{
private:
	static KillAnimationManager* s_pInstance;
	
	// Object pool for performance optimization
	sc::ObjectPool<KILL_ANIMATION_INSTANCE> m_poolAnimations;
	
	// Active animation instances
	std::vector<KILL_ANIMATION_INSTANCE*> m_vecActiveAnimations;
	
	// Effect manager reference
	DxEffSingleGroup* m_pEffectGroup;
	
	// Device reference
	LPDIRECT3DDEVICEQ m_pd3dDevice;
	
	// Animation configurations for each type
	KILL_ANIMATION_CONFIG m_arrConfigs[KILL_ANIM_MAX];
	
	// Sound file paths
	std::string m_arrSoundPaths[KILL_ANIM_MAX];
	
	// Effect file paths  
	std::string m_arrEffectPaths[KILL_ANIM_MAX];
	
	// Constructor/Destructor
	KillAnimationManager();
	~KillAnimationManager();
	
public:
	// Singleton access
	static KillAnimationManager& GetInstance();
	static void DestroyInstance();
	
	// Initialization
	HRESULT Initialize(LPDIRECT3DDEVICEQ pd3dDevice, DxEffSingleGroup* pEffectGroup);
	void Cleanup();
	
	// Animation playback
	bool PlayKillAnimation(KILL_ANIMATION_TYPE eType, 
						   const D3DXVECTOR3& vPosition, 
						   const D3DXVECTOR3& vDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f),
						   const KILL_ANIMATION_CONFIG* pConfig = nullptr);
	
	// Update loop
	void FrameMove(float fElapsedTime);
	void Render();
	
	// Configuration
	void SetAnimationConfig(KILL_ANIMATION_TYPE eType, const KILL_ANIMATION_CONFIG& config);
	const KILL_ANIMATION_CONFIG& GetAnimationConfig(KILL_ANIMATION_TYPE eType) const;
	
	// Stop animations
	void StopAnimation(KILL_ANIMATION_INSTANCE* pInstance);
	void StopAllAnimations();
	void StopAnimationsByType(KILL_ANIMATION_TYPE eType);
	
	// Utility functions
	bool IsAnimationActive(KILL_ANIMATION_TYPE eType) const;
	int GetActiveAnimationCount() const { return static_cast<int>(m_vecActiveAnimations.size()); }
	
	// Configuration file I/O
	bool LoadConfigFromFile(const std::string& strFilePath);
	bool SaveConfigToFile(const std::string& strFilePath) const;
	
private:
	// Internal helper functions
	KILL_ANIMATION_INSTANCE* CreateAnimationInstance();
	void ReleaseAnimationInstance(KILL_ANIMATION_INSTANCE* pInstance);
	void UpdateAnimation(KILL_ANIMATION_INSTANCE* pInstance, float fElapsedTime);
	void InitializeDefaultConfigs();
	void LoadDefaultResources();
	
	// Effect creation functions (implemented in separate files)
	DxEffSingle* CreateSlashEffect(KILL_ANIMATION_INSTANCE* pInstance);
	DxEffSingle* CreateStabEffect(KILL_ANIMATION_INSTANCE* pInstance);
	DxEffSingle* CreateExplosionEffect(KILL_ANIMATION_INSTANCE* pInstance);
	DxEffSingle* CreateFreezeEffect(KILL_ANIMATION_INSTANCE* pInstance);
	DxEffSingle* CreateFlameEffect(KILL_ANIMATION_INSTANCE* pInstance);
	DxEffSingle* CreateLightningEffect(KILL_ANIMATION_INSTANCE* pInstance);
	
	// Helper functions for complex effects
	void CreateSlashSparkEffect(KILL_ANIMATION_INSTANCE* pInstance);
	void CreateSlashScreenEffect(KILL_ANIMATION_INSTANCE* pInstance);
	void CreateStabBloodEffect(KILL_ANIMATION_INSTANCE* pInstance);
	void CreateStabImpactEffect(KILL_ANIMATION_INSTANCE* pInstance);
	void CreateExplosionShockwaveEffect(KILL_ANIMATION_INSTANCE* pInstance);
	void CreateExplosionSmokeEffect(KILL_ANIMATION_INSTANCE* pInstance);
	void CreateExplosionDebrisEffect(KILL_ANIMATION_INSTANCE* pInstance);
	void CreateExplosionScreenEffect(KILL_ANIMATION_INSTANCE* pInstance);
	void CreateFreezeShatterEffect(KILL_ANIMATION_INSTANCE* pInstance);
	void CreateFreezeMistEffect(KILL_ANIMATION_INSTANCE* pInstance);
	void CreateFlameEmberEffect(KILL_ANIMATION_INSTANCE* pInstance);
	void CreateFlameHeatWaveEffect(KILL_ANIMATION_INSTANCE* pInstance);
	void CreateFlameSmokeEffect(KILL_ANIMATION_INSTANCE* pInstance);
	void CreateLightningBranchEffect(KILL_ANIMATION_INSTANCE* pInstance);
	void CreateLightningDischargeEffect(KILL_ANIMATION_INSTANCE* pInstance);
	void CreateLightningImpactEffect(KILL_ANIMATION_INSTANCE* pInstance);
	void CreateLightningScreenEffect(KILL_ANIMATION_INSTANCE* pInstance);
	
	// Specific animation implementations (forward declarations)
	friend class KillAnimationSlash;
	friend class KillAnimationStab;
	friend class KillAnimationExplosion;
	friend class KillAnimationFreeze;
	friend class KillAnimationFlame;
	friend class KillAnimationLightning;
};

// Convenience macros
#define KILL_ANIM_MGR KillAnimationManager::GetInstance()