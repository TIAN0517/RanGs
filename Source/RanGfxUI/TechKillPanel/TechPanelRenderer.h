#pragma once

#include "../../enginelib/GUInterface/UIGroup.h"

class CTechKillPanelManager;

/**
 * Tech Panel Renderer
 * Handles 4D visual effects including parallax particles, neon transitions, 
 * 3D floating text, and Matrix-style data stream background
 */
class CTechPanelRenderer
{
public:
    CTechPanelRenderer(EngineDeviceMan* pEngineDevice);
    virtual ~CTechPanelRenderer();

public:
    void CreateSubControl();
    
    // Main rendering functions
    void RenderTechPanel(LPDIRECT3DDEVICE9 pd3dDevice,
                        const D3DXVECTOR2& position,
                        const CString& attackerName,
                        const CString& targetName,
                        DWORD killType,
                        float neonPhase,
                        float textWavePhase,
                        float gearRotation,
                        float animationProgress,
                        BOOL bFadingOut);
                        
    void RenderDataStreamBackground(LPDIRECT3DDEVICE9 pd3dDevice, float currentTime);

private:
    // Component rendering functions
    void RenderSemiTransparentPanel(LPDIRECT3DDEVICE9 pd3dDevice, const D3DXVECTOR2& position, float alpha);
    void RenderAttackerAvatar(LPDIRECT3DDEVICE9 pd3dDevice, const D3DXVECTOR2& position, const CString& name, float alpha);
    void RenderDynamicText(LPDIRECT3DDEVICE9 pd3dDevice, const D3DXVECTOR2& position, const CString& attackerName, const CString& targetName, float textWavePhase, float neonPhase, float alpha);
    void RenderKillIcon(LPDIRECT3DDEVICE9 pd3dDevice, const D3DXVECTOR2& position, DWORD killType, float gearRotation, float alpha);
    void RenderEnergyShield(LPDIRECT3DDEVICE9 pd3dDevice, const D3DXVECTOR2& center, float radius, float intensity, float alpha);
    void RenderMatrixCodeRain(LPDIRECT3DDEVICE9 pd3dDevice, const UIRECT& rect, float scrollOffset, float alpha);
    
    // Neon effect functions
    D3DXCOLOR GetNeonColor(float phase, float intensity = 1.0f);
    void SetupNeonRenderState(LPDIRECT3DDEVICE9 pd3dDevice);
    void RestoreRenderState(LPDIRECT3DDEVICE9 pd3dDevice);
    
    // Helper functions
    void CreateQuad(LPDIRECT3DVERTEXBUFFER9& pVB, float width, float height);
    void RenderQuadWithTexture(LPDIRECT3DDEVICE9 pd3dDevice, LPDIRECT3DVERTEXBUFFER9 pVB, LPDIRECT3DTEXTURE9 pTexture, const D3DXVECTOR2& position, float rotation = 0.0f, float alpha = 1.0f);

private:
    struct TECH_VERTEX
    {
        float x, y, z, rhw;
        DWORD color;
        float tu, tv;
        
        static const DWORD FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
    };
    
    struct MATRIX_CHAR
    {
        float x, y;
        char character;
        float intensity;
        float speed;
    };

private:
    EngineDeviceMan* m_pEngineDevice;
    
    // Textures and resources
    LPDIRECT3DTEXTURE9 m_pPanelTexture;
    LPDIRECT3DTEXTURE9 m_pAvatarTexture;
    LPDIRECT3DTEXTURE9 m_pGearTexture;
    LPDIRECT3DTEXTURE9 m_pNoiseTexture;
    LPDIRECT3DTEXTURE9 m_pGradientTexture;
    
    // Vertex buffers for optimized rendering
    LPDIRECT3DVERTEXBUFFER9 m_pPanelVB;
    LPDIRECT3DVERTEXBUFFER9 m_pAvatarVB;
    LPDIRECT3DVERTEXBUFFER9 m_pGearVB;
    LPDIRECT3DVERTEXBUFFER9 m_pTextQuadVB;
    
    // Font for text rendering
    CD3DFontPar* m_pFont;
    CD3DFontPar* m_pFontLarge;
    
    // Matrix code rain data
    std::vector<MATRIX_CHAR> m_vecMatrixChars;
    float m_fMatrixScrollSpeed;
    
    // Render state backup
    DWORD m_dwOldBlendOp;
    DWORD m_dwOldSrcBlend;
    DWORD m_dwOldDestBlend;
    DWORD m_dwOldAlphaBlendEnable;
    DWORD m_dwOldAlphaTestEnable;
    
    // Dynamic resolution adaptation
    float m_fDPIScale;
    int m_nRenderWidth;
    int m_nRenderHeight;
};