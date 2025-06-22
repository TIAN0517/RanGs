#include "stdafx.h"
#include "./TechPanelRenderer.h"
#include "./TechKillPanelManager.h"

#include "../../enginelib/DxTools/DxFontMan.h"
#include "../../enginelib/DxTools/TextureManager.h"
#include "../../enginelib/DxMeshs/SkinMesh/DxSkinMeshMan.h"

#include <random>

CTechPanelRenderer::CTechPanelRenderer(EngineDeviceMan* pEngineDevice)
    : m_pEngineDevice(pEngineDevice)
    , m_pPanelTexture(NULL)
    , m_pAvatarTexture(NULL)
    , m_pGearTexture(NULL)
    , m_pNoiseTexture(NULL)
    , m_pGradientTexture(NULL)
    , m_pPanelVB(NULL)
    , m_pAvatarVB(NULL)
    , m_pGearVB(NULL)
    , m_pTextQuadVB(NULL)
    , m_pFont(NULL)
    , m_pFontLarge(NULL)
    , m_fMatrixScrollSpeed(2.0f)
    , m_dwOldBlendOp(0)
    , m_dwOldSrcBlend(0)
    , m_dwOldDestBlend(0)
    , m_dwOldAlphaBlendEnable(0)
    , m_dwOldAlphaTestEnable(0)
    , m_fDPIScale(1.0f)
    , m_nRenderWidth(TECH_PANEL_WIDTH)
    , m_nRenderHeight(TECH_PANEL_HEIGHT)
{
    // Initialize Matrix code rain characters
    const char matrixChars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZアイウエオカキクケコサシスセソタチツテトナニヌネノハヒフヘホマミムメモヤユヨラリルレロワヲン";
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> posDis(0.0f, TECH_PANEL_WIDTH);
    std::uniform_real_distribution<> speedDis(10.0f, 30.0f);
    std::uniform_real_distribution<> intensityDis(0.3f, 1.0f);
    std::uniform_int_distribution<> charDis(0, strlen(matrixChars) - 1);
    
    // Create 50 matrix characters for background effect
    for (int i = 0; i < 50; ++i)
    {
        MATRIX_CHAR matrixChar;
        matrixChar.x = posDis(gen);
        matrixChar.y = posDis(gen) - TECH_PANEL_HEIGHT; // Start above the panel
        matrixChar.character = matrixChars[charDis(gen)];
        matrixChar.intensity = intensityDis(gen);
        matrixChar.speed = speedDis(gen);
        
        m_vecMatrixChars.push_back(matrixChar);
    }
}

CTechPanelRenderer::~CTechPanelRenderer()
{
    SAFE_RELEASE(m_pPanelTexture);
    SAFE_RELEASE(m_pAvatarTexture);
    SAFE_RELEASE(m_pGearTexture);
    SAFE_RELEASE(m_pNoiseTexture);
    SAFE_RELEASE(m_pGradientTexture);
    
    SAFE_RELEASE(m_pPanelVB);
    SAFE_RELEASE(m_pAvatarVB);
    SAFE_RELEASE(m_pGearVB);
    SAFE_RELEASE(m_pTextQuadVB);
}

void CTechPanelRenderer::CreateSubControl()
{
    if (!m_pEngineDevice) return;
    
    LPDIRECT3DDEVICE9 pd3dDevice = m_pEngineDevice->GetDevice();
    if (!pd3dDevice) return;
    
    // Get DPI scale for high resolution displays
    HDC hdc = GetDC(NULL);
    if (hdc)
    {
        int dpi = GetDeviceCaps(hdc, LOGPIXELSX);
        m_fDPIScale = dpi / 96.0f;
        ReleaseDC(NULL, hdc);
    }
    
    // Adjust render dimensions based on DPI
    m_nRenderWidth = (int)(TECH_PANEL_WIDTH * m_fDPIScale);
    m_nRenderHeight = (int)(TECH_PANEL_HEIGHT * m_fDPIScale);
    
    // Load textures
    TextureManager::GetInstance().LoadTexture(
        _T("UI\\TechPanel\\panel_background.dds"),
        TextureManager::TEXTURE_ADD_DATA::TEXUTRE_TYPE_2D,
        &m_pPanelTexture,
        0,
        0);
        
    TextureManager::GetInstance().LoadTexture(
        _T("UI\\TechPanel\\avatar_frame.dds"),
        TextureManager::TEXTURE_ADD_DATA::TEXUTRE_TYPE_2D,
        &m_pAvatarTexture,
        0,
        0);
        
    TextureManager::GetInstance().LoadTexture(
        _T("UI\\TechPanel\\gear_icon.dds"),
        TextureManager::TEXTURE_ADD_DATA::TEXUTRE_TYPE_2D,
        &m_pGearTexture,
        0,
        0);
    
    // Create procedural noise texture for effects
    HRESULT hr = pd3dDevice->CreateTexture(256, 256, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pNoiseTexture, NULL);
    if (SUCCEEDED(hr))
    {
        D3DLOCKED_RECT lr;
        if (SUCCEEDED(m_pNoiseTexture->LockRect(0, &lr, NULL, 0)))
        {
            DWORD* pPixels = (DWORD*)lr.pBits;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 255);
            
            for (int y = 0; y < 256; ++y)
            {
                for (int x = 0; x < 256; ++x)
                {
                    BYTE noise = dis(gen);
                    pPixels[y * (lr.Pitch / 4) + x] = D3DCOLOR_ARGB(255, noise, noise, noise);
                }
            }
            m_pNoiseTexture->UnlockRect(0);
        }
    }
    
    // Create gradient texture for neon effects
    hr = pd3dDevice->CreateTexture(256, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pGradientTexture, NULL);
    if (SUCCEEDED(hr))
    {
        D3DLOCKED_RECT lr;
        if (SUCCEEDED(m_pGradientTexture->LockRect(0, &lr, NULL, 0)))
        {
            DWORD* pPixels = (DWORD*)lr.pBits;
            for (int x = 0; x < 256; ++x)
            {
                float t = x / 255.0f;
                BYTE alpha = (BYTE)(255 * (1.0f - t));
                pPixels[x] = D3DCOLOR_ARGB(alpha, 0, 255, 255); // Cyan gradient
            }
            m_pGradientTexture->UnlockRect(0);
        }
    }
    
    // Create vertex buffers
    CreateQuad(m_pPanelVB, m_nRenderWidth, m_nRenderHeight);
    CreateQuad(m_pAvatarVB, 48 * m_fDPIScale, 48 * m_fDPIScale);
    CreateQuad(m_pGearVB, 32 * m_fDPIScale, 32 * m_fDPIScale);
    CreateQuad(m_pTextQuadVB, 200 * m_fDPIScale, 32 * m_fDPIScale);
    
    // Create fonts
    m_pFont = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 12, _DEFAULT_FONT_FLAG);
    m_pFontLarge = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 16, _DEFAULT_FONT_FLAG);
}

void CTechPanelRenderer::RenderTechPanel(LPDIRECT3DDEVICE9 pd3dDevice,
                                       const D3DXVECTOR2& position,
                                       const CString& attackerName,
                                       const CString& targetName,
                                       DWORD killType,
                                       float neonPhase,
                                       float textWavePhase,
                                       float gearRotation,
                                       float animationProgress,
                                       BOOL bFadingOut)
{
    if (!pd3dDevice) return;
    
    // Calculate alpha based on animation state
    float alpha = 1.0f;
    if (bFadingOut)
    {
        alpha = 1.0f - animationProgress;
    }
    
    // Setup render state for transparency
    SetupNeonRenderState(pd3dDevice);
    
    // Render semi-transparent panel background
    RenderSemiTransparentPanel(pd3dDevice, position, alpha * 0.8f);
    
    // Render Matrix-style data stream background
    UIRECT panelRect = { (int)position.x, (int)position.y, m_nRenderWidth, m_nRenderHeight };
    RenderMatrixCodeRain(pd3dDevice, panelRect, neonPhase * 50.0f, alpha * 0.6f);
    
    // Render attacker avatar with energy shield (top-left)
    D3DXVECTOR2 avatarPos = position;
    avatarPos.x += 12 * m_fDPIScale;
    avatarPos.y += 12 * m_fDPIScale;
    RenderAttackerAvatar(pd3dDevice, avatarPos, attackerName, alpha);
    
    // Render energy shield around avatar
    RenderEnergyShield(pd3dDevice, 
                      D3DXVECTOR2(avatarPos.x + 24 * m_fDPIScale, avatarPos.y + 24 * m_fDPIScale),
                      30 * m_fDPIScale,
                      neonPhase,
                      alpha);
    
    // Render dynamic text in center with wave motion
    D3DXVECTOR2 textPos = position;
    textPos.x += 80 * m_fDPIScale;
    textPos.y += 40 * m_fDPIScale;
    RenderDynamicText(pd3dDevice, textPos, attackerName, targetName, textWavePhase, neonPhase, alpha);
    
    // Render rotating gear kill icon (bottom-right)
    D3DXVECTOR2 gearPos = position;
    gearPos.x += m_nRenderWidth - 44 * m_fDPIScale;
    gearPos.y += m_nRenderHeight - 44 * m_fDPIScale;
    RenderKillIcon(pd3dDevice, gearPos, killType, gearRotation, alpha);
    
    // Restore render state
    RestoreRenderState(pd3dDevice);
}

void CTechPanelRenderer::RenderDataStreamBackground(LPDIRECT3DDEVICE9 pd3dDevice, float currentTime)
{
    if (!pd3dDevice) return;
    
    // Update matrix character positions
    for (auto& matrixChar : m_vecMatrixChars)
    {
        matrixChar.y += matrixChar.speed * (1.0f / 60.0f); // Assume 60 FPS for smooth scrolling
        
        // Reset position when character goes below panel
        if (matrixChar.y > TECH_PANEL_HEIGHT + 20)
        {
            matrixChar.y = -20;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> posDis(0.0f, TECH_PANEL_WIDTH);
            matrixChar.x = posDis(gen);
        }
    }
}

void CTechPanelRenderer::RenderSemiTransparentPanel(LPDIRECT3DDEVICE9 pd3dDevice, const D3DXVECTOR2& position, float alpha)
{
    if (!m_pPanelVB || !m_pPanelTexture) return;
    
    pd3dDevice->SetTexture(0, m_pPanelTexture);
    RenderQuadWithTexture(pd3dDevice, m_pPanelVB, m_pPanelTexture, position, 0.0f, alpha);
}

void CTechPanelRenderer::RenderAttackerAvatar(LPDIRECT3DDEVICE9 pd3dDevice, const D3DXVECTOR2& position, const CString& name, float alpha)
{
    if (!m_pAvatarVB || !m_pAvatarTexture) return;
    
    // Render circular avatar frame
    pd3dDevice->SetTexture(0, m_pAvatarTexture);
    RenderQuadWithTexture(pd3dDevice, m_pAvatarVB, m_pAvatarTexture, position, 0.0f, alpha);
    
    // Render name below avatar (optional, if space permits)
    if (m_pFont && name.GetLength() > 0)
    {
        D3DXVECTOR2 namePos = position;
        namePos.y += 52 * m_fDPIScale;
        
        D3DCOLOR textColor = D3DCOLOR_ARGB((BYTE)(255 * alpha), 0, 255, 255);
        m_pFont->DrawText(namePos.x, namePos.y, textColor, name, 0L);
    }
}

void CTechPanelRenderer::RenderDynamicText(LPDIRECT3DDEVICE9 pd3dDevice, const D3DXVECTOR2& position, const CString& attackerName, const CString& targetName, float textWavePhase, float neonPhase, float alpha)
{
    if (!m_pFontLarge) return;
    
    // Calculate wave offset for 3D floating effect
    float waveOffset = sinf(textWavePhase * D3DX_PI * 2.0f) * TEXT_WAVE_AMPLITUDE;
    
    // Get neon color with RGB dynamic variations
    D3DXCOLOR neonColor = GetNeonColor(neonPhase, alpha);
    DWORD textColor = D3DCOLOR_ARGB(
        (BYTE)(255 * alpha),
        (BYTE)(neonColor.r * 255),
        (BYTE)(neonColor.g * 255),
        (BYTE)(neonColor.b * 255)
    );
    
    // Render kill message with wave motion
    CString killMessage;
    killMessage.Format(_T("%s ► %s"), attackerName, targetName);
    
    D3DXVECTOR2 textPos = position;
    textPos.y += waveOffset;
    
    m_pFontLarge->DrawText(textPos.x, textPos.y, textColor, killMessage, 0L);
}

void CTechPanelRenderer::RenderKillIcon(LPDIRECT3DDEVICE9 pd3dDevice, const D3DXVECTOR2& position, DWORD killType, float gearRotation, float alpha)
{
    if (!m_pGearVB || !m_pGearTexture) return;
    
    // Render rotating gear effect
    pd3dDevice->SetTexture(0, m_pGearTexture);
    RenderQuadWithTexture(pd3dDevice, m_pGearVB, m_pGearTexture, position, gearRotation, alpha);
}

void CTechPanelRenderer::RenderEnergyShield(LPDIRECT3DDEVICE9 pd3dDevice, const D3DXVECTOR2& center, float radius, float intensity, float alpha)
{
    if (!m_pGradientTexture) return;
    
    // Create energy shield effect using animated gradient
    D3DXCOLOR shieldColor = GetNeonColor(intensity, alpha * 0.7f);
    
    // Simple implementation: render a colored circle with alpha blending
    // In a full implementation, this would use a custom shader for better effects
    
    const int segments = 16;
    TECH_VERTEX vertices[segments + 2];
    
    // Center vertex
    vertices[0].x = center.x;
    vertices[0].y = center.y;
    vertices[0].z = 0.0f;
    vertices[0].rhw = 1.0f;
    vertices[0].color = D3DCOLOR_ARGB(
        (BYTE)(shieldColor.a * 255),
        (BYTE)(shieldColor.r * 255),
        (BYTE)(shieldColor.g * 255),
        (BYTE)(shieldColor.b * 255)
    );
    vertices[0].tu = 0.5f;
    vertices[0].tv = 0.5f;
    
    // Circle vertices
    for (int i = 0; i <= segments; ++i)
    {
        float angle = (i * D3DX_PI * 2.0f) / segments;
        vertices[i + 1].x = center.x + cosf(angle) * radius;
        vertices[i + 1].y = center.y + sinf(angle) * radius;
        vertices[i + 1].z = 0.0f;
        vertices[i + 1].rhw = 1.0f;
        vertices[i + 1].color = D3DCOLOR_ARGB(0, 0, 255, 255); // Transparent edge
        vertices[i + 1].tu = 0.5f + cosf(angle) * 0.5f;
        vertices[i + 1].tv = 0.5f + sinf(angle) * 0.5f;
    }
    
    pd3dDevice->SetTexture(0, m_pGradientTexture);
    pd3dDevice->SetFVF(TECH_VERTEX::FVF);
    pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, segments, vertices, sizeof(TECH_VERTEX));
}

void CTechPanelRenderer::RenderMatrixCodeRain(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rect, float scrollOffset, float alpha)
{
    if (!m_pFont) return;
    
    // Render Matrix-style code rain
    for (const auto& matrixChar : m_vecMatrixChars)
    {
        if (matrixChar.x >= rect.left && matrixChar.x <= rect.right &&
            matrixChar.y >= rect.top && matrixChar.y <= rect.bottom)
        {
            D3DCOLOR charColor = D3DCOLOR_ARGB(
                (BYTE)(255 * alpha * matrixChar.intensity),
                0,
                (BYTE)(255 * matrixChar.intensity),
                0
            );
            
            CString charStr;
            charStr.Format(_T("%c"), matrixChar.character);
            
            m_pFont->DrawText(
                rect.left + matrixChar.x,
                rect.top + matrixChar.y,
                charColor,
                charStr,
                0L
            );
        }
    }
}

D3DXCOLOR CTechPanelRenderer::GetNeonColor(float phase, float intensity)
{
    // RGB dynamic variations with specified frequency
    float r = 0.5f + 0.5f * sinf(phase * D3DX_PI * 2.0f);
    float g = 0.5f + 0.5f * sinf(phase * D3DX_PI * 2.0f + D3DX_PI * 2.0f / 3.0f);
    float b = 0.5f + 0.5f * sinf(phase * D3DX_PI * 2.0f + D3DX_PI * 4.0f / 3.0f);
    
    return D3DXCOLOR(r * intensity, g * intensity, b * intensity, intensity);
}

void CTechPanelRenderer::SetupNeonRenderState(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // Store old render state
    pd3dDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &m_dwOldAlphaBlendEnable);
    pd3dDevice->GetRenderState(D3DRS_SRCBLEND, &m_dwOldSrcBlend);
    pd3dDevice->GetRenderState(D3DRS_DESTBLEND, &m_dwOldDestBlend);
    pd3dDevice->GetRenderState(D3DRS_BLENDOP, &m_dwOldBlendOp);
    pd3dDevice->GetRenderState(D3DRS_ALPHATESTENABLE, &m_dwOldAlphaTestEnable);
    
    // Setup for alpha blending
    pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    pd3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    
    // Setup texture stage states for proper blending
    pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
}

void CTechPanelRenderer::RestoreRenderState(LPDIRECT3DDEVICE9 pd3dDevice)
{
    // Restore old render state
    pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, m_dwOldAlphaBlendEnable);
    pd3dDevice->SetRenderState(D3DRS_SRCBLEND, m_dwOldSrcBlend);
    pd3dDevice->SetRenderState(D3DRS_DESTBLEND, m_dwOldDestBlend);
    pd3dDevice->SetRenderState(D3DRS_BLENDOP, m_dwOldBlendOp);
    pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, m_dwOldAlphaTestEnable);
}

void CTechPanelRenderer::CreateQuad(LPDIRECT3DVERTEXBUFFER9& pVB, float width, float height)
{
    if (!m_pEngineDevice) return;
    
    LPDIRECT3DDEVICE9 pd3dDevice = m_pEngineDevice->GetDevice();
    if (!pd3dDevice) return;
    
    HRESULT hr = pd3dDevice->CreateVertexBuffer(
        4 * sizeof(TECH_VERTEX),
        D3DUSAGE_WRITEONLY,
        TECH_VERTEX::FVF,
        D3DPOOL_MANAGED,
        &pVB,
        NULL
    );
    
    if (SUCCEEDED(hr) && pVB)
    {
        TECH_VERTEX* pVertices;
        if (SUCCEEDED(pVB->Lock(0, 0, (void**)&pVertices, 0)))
        {
            // Create quad vertices
            pVertices[0] = { 0.0f, 0.0f, 0.0f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f };
            pVertices[1] = { width, 0.0f, 0.0f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f };
            pVertices[2] = { 0.0f, height, 0.0f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f };
            pVertices[3] = { width, height, 0.0f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f };
            
            pVB->Unlock();
        }
    }
}

void CTechPanelRenderer::RenderQuadWithTexture(LPDIRECT3DDEVICE9 pd3dDevice, LPDIRECT3DVERTEXBUFFER9 pVB, LPDIRECT3DTEXTURE9 pTexture, const D3DXVECTOR2& position, float rotation, float alpha)
{
    if (!pd3dDevice || !pVB) return;
    
    // Setup transformation matrix for position and rotation
    D3DXMATRIX matWorld, matTrans, matRot;
    D3DXMatrixIdentity(&matWorld);
    
    if (rotation != 0.0f)
    {
        D3DXMatrixRotationZ(&matRot, D3DXToRadian(rotation));
        matWorld = matRot;
    }
    
    D3DXMatrixTranslation(&matTrans, position.x, position.y, 0.0f);
    matWorld *= matTrans;
    
    pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
    pd3dDevice->SetTexture(0, pTexture);
    pd3dDevice->SetStreamSource(0, pVB, 0, sizeof(TECH_VERTEX));
    pd3dDevice->SetFVF(TECH_VERTEX::FVF);
    
    pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}