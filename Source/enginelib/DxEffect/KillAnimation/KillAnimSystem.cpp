#include "pch.h"
#include "KillAnimSystem.h"
#include "../../../DxTools/DxViewPort.h"
#include "../../../DxTools/TextureManager.h"
#include "../../../DxSound/DxSoundMan.h"

// Static instance
DxKillAnimationManager* DxKillAnimationManager::s_pInstance = nullptr;

//-----------------------------------------------------------------------------
// DxKillAnimation Implementation
//-----------------------------------------------------------------------------
DxKillAnimation::DxKillAnimation()
    : m_emType(EMKILL_SWORD_SLASH)
    , m_fAge(0.0f)
    , m_fLifeTime(2.5f)
    , m_bActive(FALSE)
    , m_vPosition(0.0f, 0.0f, 0.0f)
    , m_pTexture(nullptr)
{
    D3DXMatrixIdentity(&m_matWorld);
}

DxKillAnimation::~DxKillAnimation()
{
    if (m_pTexture)
    {
        m_pTexture->Release();
        m_pTexture = nullptr;
    }
}

void DxKillAnimation::SetAnimation(EMKILL_ANIMATION_TYPE emType, const D3DXVECTOR3& vPos)
{
    m_emType = emType;
    m_vPosition = vPos;
    m_fAge = 0.0f;
    m_bActive = TRUE;
    
    // Set life time based on animation type
    switch (emType)
    {
    case EMKILL_SWORD_SLASH:
        m_fLifeTime = 2.5f;
        break;
    case EMKILL_PIERCE_FATAL:
        m_fLifeTime = 1.8f;
        break;
    case EMKILL_EXPLOSION_BLAST:
        m_fLifeTime = 3.0f;
        break;
    case EMKILL_ICE_SHATTER:
        m_fLifeTime = 2.2f;
        break;
    case EMKILL_FLAME_BURN:
        m_fLifeTime = 2.8f;
        break;
    case EMKILL_LIGHTNING_STRIKE:
        m_fLifeTime = 2.0f;
        break;
    default:
        m_fLifeTime = 2.5f;
        break;
    }
    
    // Set world matrix
    D3DXMatrixTranslation(&m_matWorld, vPos.x, vPos.y, vPos.z);
    
    LoadResources();
    PlaySound();
}

void DxKillAnimation::FrameMove(float fElapsedTime)
{
    if (!m_bActive) return;
    
    m_fAge += fElapsedTime;
    
    if (m_fAge >= m_fLifeTime)
    {
        m_bActive = FALSE;
    }
    
    // Update animation properties based on age
    float fLifeRatio = m_fAge / m_fLifeTime;
    
    // Apply some simple scaling animation
    float fScale = 1.0f + sinf(fLifeRatio * D3DX_PI * 2.0f) * 0.3f;
    D3DXMATRIX matScale;
    D3DXMatrixScaling(&matScale, fScale, fScale, fScale);
    
    D3DXMATRIX matTrans;
    D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);
    
    m_matWorld = matScale * matTrans;
}

void DxKillAnimation::Render(LPDIRECT3DDEVICEQ pd3dDevice)
{
    if (!m_bActive || !pd3dDevice) return;
    
    // Simple billboard rendering for now
    // In a full implementation, this would render the actual effect
    
    // Set world matrix
    pd3dDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
    
    // Enable alpha blending
    pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    
    // Set color based on animation type
    D3DCOLOR dwColor = D3DCOLOR_XRGB(255, 255, 255);
    switch (m_emType)
    {
    case EMKILL_SWORD_SLASH:
        dwColor = D3DCOLOR_XRGB(255, 215, 0); // Gold
        break;
    case EMKILL_PIERCE_FATAL:
        dwColor = D3DCOLOR_XRGB(192, 192, 192); // Silver
        break;
    case EMKILL_EXPLOSION_BLAST:
        dwColor = D3DCOLOR_XRGB(255, 69, 0); // Orange-red
        break;
    case EMKILL_ICE_SHATTER:
        dwColor = D3DCOLOR_XRGB(173, 216, 230); // Light blue
        break;
    case EMKILL_FLAME_BURN:
        dwColor = D3DCOLOR_XRGB(220, 20, 60); // Crimson
        break;
    case EMKILL_LIGHTNING_STRIKE:
        dwColor = D3DCOLOR_XRGB(138, 43, 226); // Blue violet
        break;
    }
    
    // Alpha fade based on life ratio
    float fAlpha = 1.0f - (m_fAge / m_fLifeTime);
    DWORD dwAlpha = (DWORD)(fAlpha * 255.0f);
    dwColor = (dwColor & 0x00FFFFFF) | (dwAlpha << 24);
    
    // TODO: Render actual particle effect or mesh here
    // For now, we just set the material color
    D3DMATERIAL9 material;
    ZeroMemory(&material, sizeof(material));
    material.Diffuse.r = material.Ambient.r = ((dwColor >> 16) & 0xFF) / 255.0f;
    material.Diffuse.g = material.Ambient.g = ((dwColor >> 8) & 0xFF) / 255.0f;
    material.Diffuse.b = material.Ambient.b = (dwColor & 0xFF) / 255.0f;
    material.Diffuse.a = material.Ambient.a = ((dwColor >> 24) & 0xFF) / 255.0f;
    
    pd3dDevice->SetMaterial(&material);
}

void DxKillAnimation::LoadResources()
{
    // TODO: Load textures and sounds based on animation type
    // For now, just placeholder paths
    std::string strTexture, strSound;
    
    switch (m_emType)
    {
    case EMKILL_SWORD_SLASH:
        strTexture = "effect/kill/sword_slash.dds";
        strSound = "effect/kill/sword_slash.wav";
        break;
    case EMKILL_PIERCE_FATAL:
        strTexture = "effect/kill/pierce_fatal.dds";
        strSound = "effect/kill/pierce_fatal.wav";
        break;
    case EMKILL_EXPLOSION_BLAST:
        strTexture = "effect/kill/explosion_blast.dds";
        strSound = "effect/kill/explosion_blast.wav";
        break;
    case EMKILL_ICE_SHATTER:
        strTexture = "effect/kill/ice_shatter.dds";
        strSound = "effect/kill/ice_shatter.wav";
        break;
    case EMKILL_FLAME_BURN:
        strTexture = "effect/kill/flame_burn.dds";
        strSound = "effect/kill/flame_burn.wav";
        break;
    case EMKILL_LIGHTNING_STRIKE:
        strTexture = "effect/kill/lightning_strike.dds";
        strSound = "effect/kill/lightning_strike.wav";
        break;
    }
    
    // Load texture (commented out as actual texture files don't exist yet)
    // m_pTexture = TextureManager::GetInstance().LoadTexture(strTexture, false);
}

void DxKillAnimation::PlaySound()
{
    // TODO: Play sound based on animation type
    // For now, just placeholder
}

//-----------------------------------------------------------------------------
// DxKillAnimationManager Implementation
//-----------------------------------------------------------------------------
DxKillAnimationManager& DxKillAnimationManager::GetInstance()
{
    if (!s_pInstance)
    {
        s_pInstance = new DxKillAnimationManager();
    }
    return *s_pInstance;
}

void DxKillAnimationManager::ReleaseInstance()
{
    if (s_pInstance)
    {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

DxKillAnimationManager::DxKillAnimationManager()
    : m_pd3dDevice(nullptr)
{
    InitializeAnimationData();
}

DxKillAnimationManager::~DxKillAnimationManager()
{
    ClearAllAnimations();
}

HRESULT DxKillAnimationManager::InitDeviceObjects(LPDIRECT3DDEVICEQ pd3dDevice)
{
    m_pd3dDevice = pd3dDevice;
    return S_OK;
}

HRESULT DxKillAnimationManager::RestoreDeviceObjects()
{
    return S_OK;
}

HRESULT DxKillAnimationManager::InvalidateDeviceObjects()
{
    return S_OK;
}

HRESULT DxKillAnimationManager::DeleteDeviceObjects()
{
    ClearAllAnimations();
    m_pd3dDevice = nullptr;
    return S_OK;
}

void DxKillAnimationManager::FrameMove(float fElapsedTime)
{
    // Update all active animations
    for (auto it = m_listAnimation.begin(); it != m_listAnimation.end(); ++it)
    {
        (*it)->FrameMove(fElapsedTime);
    }
    
    // Clean up dead animations
    CleanupDeadAnimations();
}

void DxKillAnimationManager::Render()
{
    if (!m_pd3dDevice) return;
    
    // Render all active animations
    for (auto it = m_listAnimation.begin(); it != m_listAnimation.end(); ++it)
    {
        (*it)->Render(m_pd3dDevice);
    }
}

void DxKillAnimationManager::PlayKillAnimation(EMKILL_ANIMATION_TYPE emType, const D3DXVECTOR3& vPos)
{
    // Create new animation instance
    DxKillAnimation* pAnim = new DxKillAnimation();
    pAnim->SetAnimation(emType, vPos);
    
    m_listAnimation.push_back(pAnim);
}

void DxKillAnimationManager::ClearAllAnimations()
{
    for (auto it = m_listAnimation.begin(); it != m_listAnimation.end(); ++it)
    {
        delete *it;
    }
    m_listAnimation.clear();
}

void DxKillAnimationManager::InitializeAnimationData()
{
    // Initialize animation data for each type
    SKILL_ANIMATION_DATA data;
    
    // 劍氣砍殺 - Gold sword slash
    data.emType = EMKILL_SWORD_SLASH;
    data.strTexturePath = "effect/kill/sword_slash.dds";
    data.strSoundPath = "effect/kill/sword_slash.wav";
    data.fDuration = 2.5f;
    data.dwColor = D3DCOLOR_XRGB(255, 215, 0);
    m_mapAnimData[EMKILL_SWORD_SLASH] = data;
    
    // 穿刺致命 - Silver pierce
    data.emType = EMKILL_PIERCE_FATAL;
    data.strTexturePath = "effect/kill/pierce_fatal.dds";
    data.strSoundPath = "effect/kill/pierce_fatal.wav";
    data.fDuration = 1.8f;
    data.dwColor = D3DCOLOR_XRGB(192, 192, 192);
    m_mapAnimData[EMKILL_PIERCE_FATAL] = data;
    
    // 爆炸轟殺 - Orange explosion
    data.emType = EMKILL_EXPLOSION_BLAST;
    data.strTexturePath = "effect/kill/explosion_blast.dds";
    data.strSoundPath = "effect/kill/explosion_blast.wav";
    data.fDuration = 3.0f;
    data.dwColor = D3DCOLOR_XRGB(255, 69, 0);
    m_mapAnimData[EMKILL_EXPLOSION_BLAST] = data;
    
    // 冰凍粉碎 - Blue ice
    data.emType = EMKILL_ICE_SHATTER;
    data.strTexturePath = "effect/kill/ice_shatter.dds";
    data.strSoundPath = "effect/kill/ice_shatter.wav";
    data.fDuration = 2.2f;
    data.dwColor = D3DCOLOR_XRGB(173, 216, 230);
    m_mapAnimData[EMKILL_ICE_SHATTER] = data;
    
    // 烈焰焚燒 - Red flame
    data.emType = EMKILL_FLAME_BURN;
    data.strTexturePath = "effect/kill/flame_burn.dds";
    data.strSoundPath = "effect/kill/flame_burn.wav";
    data.fDuration = 2.8f;
    data.dwColor = D3DCOLOR_XRGB(220, 20, 60);
    m_mapAnimData[EMKILL_FLAME_BURN] = data;
    
    // 雷電轟擊 - Purple lightning
    data.emType = EMKILL_LIGHTNING_STRIKE;
    data.strTexturePath = "effect/kill/lightning_strike.dds";
    data.strSoundPath = "effect/kill/lightning_strike.wav";
    data.fDuration = 2.0f;
    data.dwColor = D3DCOLOR_XRGB(138, 43, 226);
    m_mapAnimData[EMKILL_LIGHTNING_STRIKE] = data;
}

void DxKillAnimationManager::CleanupDeadAnimations()
{
    auto it = m_listAnimation.begin();
    while (it != m_listAnimation.end())
    {
        if (!(*it)->IsAlive())
        {
            delete *it;
            it = m_listAnimation.erase(it);
        }
        else
        {
            ++it;
        }
    }
}