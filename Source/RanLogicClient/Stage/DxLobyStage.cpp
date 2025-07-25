#include "pch.h"
#include "./DxLobyStage.h"
#include "./DxGlobalStage.h"

#include "../../SigmaCore/Loading/NSLoadingDirect.h"
#include "../../SigmaCore/Math/Math.h"

#include "../../EngineLib/DxEffect/Frame/DxEffectMan.h"
#include "../../EngineLib/DxTools/PostProcessing/NSLightShafts.h"
#include "../../EngineLib/DxTools/PostProcessing/NSGlow.h"
#include "../../EngineLib/DxTools/DxEnvironment.h"
#include "../../EngineLib/DxTools/DxSurfaceTex.h"
#include "../../EngineLib/DxTools/DxGlowMan.h"
#include "../../EngineLib/DxTools/DxPostProcess.h"
#include "../../EngineLib/DxTools/DxCubeMap.h"
#include "../../EngineLib/DxTools/DxSkyMan.h"
#include "../../EngineLib/DxTools/RENDERPARAM.h"
#include "../../EngineLib/DxTools/DxBackUpRendTarget.h"
#include "../../EngineLib/DxTools/DxShadowMap.h"
#include "../../EngineLib/DxTools/DebugSet.h"
#include "../../EngineLib/DxEffect/Single/DxEffGroupPlayer.h"
#include "../../EngineLib/Common/profile.h"
#include "../../EngineLib/DxTools/DxDynamicVB.h"	// »èÁ¦ ÇØ¾ßÇÑ´Ù.
#include "../../EngineLib/GUInterface/GameTextControl.h"
#include "../../EngineLib/DxTools/DxRenderStates.h"
#include "../../EngineLib/Water/NSWaterMain.h"
#include "../../EngineLib/DxMeshs/SkinMesh/NSSkinAniThread.h"
#include "../../EngineLib/DxLand/DxLandMan.h"

#include "../../EngineLib/G-Logic/GLPeriod.h"
#include "../../EngineLib/DxTools/DxViewPort.h"
#include "../../EngineLib/DxTools/DxFontMan.h"
#include "../../EngineLib/DxTools/DxInputDevice.h"
#include "../../EngineLib/DxTools/Light/DxLightMan.h"
#include "../../EngineLib/DxTools/Light/NSShadowLight.h"
#include "../../EngineLib/DxTools/CubeMap/NSCubeMapCulling.h"
#include "../../EngineLib/DxTools/CubeMap/NSCubeMapUtil.h"
#include "../../EngineLib/DxTools/Deffered/NSDeffered.h"
#include "../../EngineLib/DxTools/PostProcessing/NSBloomHDR.h"

#include "../../EngineLib/DxSound/BgmSound.h"

#include "../../RanLogic/Msg/ServerMsg.h"
#include "../../RanLogic/RANPARAM.h"

#include "../../EngineLib/GUInterface/InterfaceBaseDefine.h"
//#include "../InterfaceBaseDefine.h"
#include "../Tool/DxParamSet.h"
#include "../GLGaeaClient.h"

//#include "../GfxInterfaceBase.h"
#include "../../RanGfxUI/OuterInterface.h"

// ----------------------------------------------------------------------------
#include "../../SigmaCore/DebugInclude.h"
// ----------------------------------------------------------------------------

namespace SEXUAL
{
	enum SEX 
	{
		SEX_MAN   = 1,
		SEX_WOMAN = 0
	};

	SEX GetSex ( EMCHARINDEX emIndex ) 
	{
		if( (WORD)emIndex == GLCI_FIGHTER_M || 
			(WORD)emIndex == GLCI_ARMS_M || 
			(WORD)emIndex == GLCI_ARCHER_M || 
			(WORD)emIndex == GLCI_SPIRIT_M || 
			(WORD)emIndex == GLCI_EXTREME_M ||
			(WORD)emIndex == GLCI_SCIENTIST_M ||
            (WORD)emIndex == GLCI_ASSASSIN_M ||
			(WORD)emIndex == GLCI_TRICKER_M ||
            (WORD)emIndex == GLCI_ETC_M ||
			(WORD)emIndex == GLCI_ACTOR_M )
			return SEX_MAN;
		
		return SEX_WOMAN;
	}
}

DxLobyStage::DxLobyStage(DxGlobalStage* pGlobalStage, GLGaeaClient* pGaeaClient, EngineDeviceMan* pEngineDevice)
    : m_pGlobalStage(pGlobalStage)
    , m_pGaeaClient(pGaeaClient)
	, m_pEngineDevice(pEngineDevice)
	, m_nStartCharNum(0)
    , m_nStartCharLoad(0)
    , m_nChaSNum(0)
    , m_dwCharSelect(MAX_SELECTCHAR)
    , m_bGameJoin(FALSE)
    , m_nCharStart(0)
    , m_fShiftTime(0)
    , m_bShiftButton(FALSE)
    , m_bShiftLBDown(FALSE)
    , m_bShiftRBDown(FALSE)
    , m_bUseInterface(TRUE)
    , m_bRenderSkip(FALSE)
    , m_fRenderSkipTime(0.0f)
    , m_bStyle( false )
    , m_vDir ( D3DXVECTOR3( -1.0f,-10.0f,-0.35f) )
	//, m_nStageBG( EMSTAGE_BG_NONE )
	, m_bChangedStage( true )
    , m_strTextureName( "GUI_001.dds")
	, m_fCheckGpuTimeADD(0.f)
	, m_nCheckGpuCountADD(0)
	, m_fElapsedTime(0.f)
	, m_dwCharIDForWorldBattle(0)
	, m_spLandCharSlt(new DxLandMan)
	, m_spLandLogin(new DxLandMan)
{	
	m_pOuterInterface = new COuterInterface(m_pGaeaClient,m_pEngineDevice);
}

DxLobyStage::~DxLobyStage(void)
{
}

HRESULT DxLobyStage::ReSizeWindow ( WORD wWidth, WORD wHeight )
{
	
	if ( m_pOuterInterface ) m_pOuterInterface->ISetResolution (MAKELONG(wHeight,wWidth));

	InvalidateDeviceObjects();
	RestoreDeviceObjects();

	return S_OK;
}

void DxLobyStage::OnInitLogin ()
{
	m_nStartCharLoad = 0;
	m_nChaSNum = 0;
	//m_bGameJoin = FALSE;
	m_dwCharSelect = MAX_SELECTCHAR;
	m_nCharStart = 0;
	m_bGameJoin = FALSE;
	m_fShiftTime = 0;
	m_bShiftButton = FALSE;
	m_bShiftLBDown = FALSE;
	m_bShiftRBDown = FALSE;
	m_bUseInterface = TRUE;

	for ( int i=0; i<MAX_SERVERCHAR; i++ ) m_CharInfo[i] = SCHARINFO_LOBBY();
	for ( int i=0; i<MAX_SELECTCHAR; i++ ) m_CharSham[i].ResetCharInfo ();

	m_NewCharSham.ResetCharInfo();
}

SCHARINFO_LOBBY* DxLobyStage::GetSelectCharInfo()
{
	if (m_dwCharSelect >= MAX_SELECTCHAR)	
		return NULL;
    else
	    return &(m_CharInfo[m_dwCharSelect+m_nCharStart]);
}

SCHARINFO_LOBBY* DxLobyStage::GetCharInfo(INT nIndex)
{
	if (nIndex >= MAX_SERVERCHAR)
		return NULL;

	if (nIndex >= m_nChaSNum)
		return NULL;

	return &(m_CharInfo[nIndex]);
}

//DxLandMan* DxLobyStage::GetCharNewLand ()
//{
//	switch ( m_sCharNewInfo.m_wSchool )
//	{
//	case 0: m_LandCharSlt_s01.ActiveMap ( m_LandCharSlt_s01 );	return &m_LandCharSlt_s01;
//	case 1:	m_LandCharSlt_s02.ActiveMap ( m_LandCharSlt_s02 );	return &m_LandCharSlt_s02;
//	case 2:	m_LandCharSlt_s03.ActiveMap ( m_LandCharSlt_s03 );	return &m_LandCharSlt_s03;
//
//	default:
//		GASSERT(0&&"»õ·Î »ý¼ºÇÒ ÄÉ¸¯ÅÍÀÇ ÇÐ±³Á¤º¸°¡ 0~2 ¿µ¿ª ÀÌ³»¿¡ ÀÖÁö ¾Ê½À´Ï´Ù." );
//		break;
//	};
//
//	return NULL;
//}

WORD DxLobyStage::ShiftCharClass ( EMSHIFT emShift, WORD wClassMax )
{
	WORD nRet(wClassMax);

	switch ( emShift )
	{
	case EM_PREV:
		{
			WORD wIndex = (WORD)m_sCharNewInfo.m_emIndex;
			if ( (WORD)m_sCharNewInfo.m_emIndex > 0 )			wIndex--;
			else												wIndex = wClassMax;
			SelCharClass ( wIndex );

			nRet = wIndex;
		}
		break;

	case EM_NEXT:
		{
			WORD wIndex = (WORD)m_sCharNewInfo.m_emIndex;
			if ( (WORD)m_sCharNewInfo.m_emIndex < wClassMax )	wIndex++;
			else												wIndex = 0;
			SelCharClass ( wIndex );

			nRet = wIndex;
		}
		break;
	};

	return nRet;
}

void DxLobyStage::ShiftCharSchool ( EMSHIFT emShift )
{
	switch ( emShift )
	{
	case EM_PREV:
		{
			WORD wIndex = m_sCharNewInfo.m_wSchool;
			if ( wIndex>0 )										wIndex--;
			else												wIndex = (GLCONST_CHAR::wSCHOOLNUM-1);
			SelCharSchool ( wIndex );
		}
		break;

	case EM_NEXT:
		{
			WORD wIndex = m_sCharNewInfo.m_wSchool;
			if ( (WORD)wIndex < (GLCONST_CHAR::wSCHOOLNUM-1) )	wIndex++;
			else												wIndex = 0;
			SelCharSchool ( wIndex );
		}
		break;
	};
}

void DxLobyStage::ShiftCharFace ( EMSHIFT emShift )
{
	const GLCONST_CHARCLASS &sCONST = GLCONST_CHAR::cCONSTCLASS[m_sCharNewInfo.m_emIndex];

	switch ( emShift )
	{
	case EM_PREV:
		{
			WORD wIndex = (WORD)m_sCharNewInfo.m_wFace;
			if ( wIndex > 0 )					wIndex--;
			else								wIndex = (WORD) (sCONST.dwHEADNUM_SELECT-1);
			
			SelCharFace ( wIndex );
		}
		break;

	case EM_NEXT:
		{
			WORD wIndex = (WORD)m_sCharNewInfo.m_wFace;
			if ( wIndex < WORD(sCONST.dwHEADNUM_SELECT-1) )	wIndex++;
			else											wIndex = 0;

			SelCharFace ( wIndex );
		}
		break;
	};
}

void DxLobyStage::ShiftCharHair ( EMSHIFT emShift )
{
	const GLCONST_CHARCLASS &sCONST = GLCONST_CHAR::cCONSTCLASS[m_sCharNewInfo.m_emIndex];

	switch ( emShift )
	{
	case EM_PREV:
		{
			WORD wIndex = (WORD)m_sCharNewInfo.m_wHair;
			if ( wIndex > 0 )					wIndex--;
			else								wIndex = (WORD) (sCONST.dwHAIRNUM_SELECT-1);

			WORD wHairColor = HAIRCOLOR::GetHairColor ( m_sCharNewInfo.m_emIndex, wIndex );
			
			SelCharHair ( wIndex );
			SelCharHairColor ( wHairColor );
		}
		break;

	case EM_NEXT:
		{
			WORD wIndex = (WORD)m_sCharNewInfo.m_wHair;
			if ( wIndex < WORD(sCONST.dwHAIRNUM_SELECT-1) )	wIndex++;
			else											wIndex = 0;

			WORD wHairColor = HAIRCOLOR::GetHairColor ( m_sCharNewInfo.m_emIndex, wIndex );

			SelCharHair ( wIndex );
			SelCharHairColor ( wHairColor );
		}
		break;
	};
}

//void DxLobyStage::ToCameraPos ( int n )
//{
//	ShiftCharClass ( (EMSHIFT)n );
//}

void DxLobyStage::SelCharClass ( WORD wIndex )
{
	//	ÄÉ¸¯ÅÍ ¼±ÅÃ.
	m_sCharNewInfo.m_emIndex = (EMCHARINDEX) wIndex;

	m_sCharNewInfo.m_wHair = 0;
	m_sCharNewInfo.m_wFace = 0;
	m_sCharNewInfo.m_wHairColor = HAIRCOLOR::GetHairColor ( m_sCharNewInfo.m_emIndex, 0 );

	// ¼ºº° ¼³Á¤
	m_sCharNewInfo.m_wSex = SEXUAL::GetSex ( m_sCharNewInfo.m_emIndex );
}

void DxLobyStage::SelCharSchool ( WORD wIndex )
{
	//	ÇÐ±³ ¼±ÅÃ.
	GASSERT(wIndex<GLCONST_CHAR::wSCHOOLNUM);
	m_sCharNewInfo.m_wSchool = (WORD)wIndex;

	//int nClassMax = GLCI_NUM_ETC;

	////	¼±ÅÃµÈ ÄÉ¸¯ÅÍ Å¬·¹½º ¸®¼Â.
	//SelCharClass( (WORD)nClassMax);

	//m_sCharNewInfo.m_wHair = 0;
	//m_sCharNewInfo.m_wFace = 0;
}

void DxLobyStage::DelChar ( int nCharID )
{
	//	Note : ±âÁ¸ Ä³¸¯ÅÍ Á¤º¸ ¼ö½Å½Ã.
	int nIndex = -1;
	for ( int i=0; i<m_nChaSNum; i++ )
	{
		if ( m_nChaIDs[i]== nCharID )	nIndex = i;
	}
	if ( nIndex == -1 )		return;

	//	Note : Ä³¸¯ÅÍ ¼±ÅÃÀ» Ãë¼Ò.
	m_dwCharSelect = MAX_SELECTCHAR;

	//	Note : »èÁ¦µÈ°É Áö¿ì°í ÀçÁ¤·Ä.
	for ( int i=nIndex+1; i<m_nChaSNum; i++ )
	{
		m_nChaIDs[i-1] = m_nChaIDs[i];
		m_CharInfo[i-1] = m_CharInfo[i];
	}
	m_nChaSNum--;

	//	Note : µð½ºÇÃ·¹ÀÌ ÄÉ¸¯ÅÍ Çü»ó °»½Å.
	for( int i=0; i<MAX_SELECTCHAR; i++ )
	{
		m_CharSham[i].ResetCharInfo ();
		m_CharSham[i].SetSelect ( FALSE );

		InitRenderSkip();
	}

	if( m_nCharStart >= m_nChaSNum )
	{
		ShiftLBDown();
		return;
	}

	for( int i=m_nCharStart, j=0; i<m_nChaSNum; i++, j++ )
	{
		if( j >= MAX_SELECTCHAR ) 
			break;

		m_CharSham[j].SetCharInfo( m_CharInfo[i] );
	}
}

BOOL DxLobyStage::ShiftRightChar()
{
	if( (m_nCharStart+MAX_SELECTCHAR) < m_nChaSNum )
	{
		m_nCharStart += MAX_SELECTCHAR;
		return TRUE;
	}

	return FALSE;
}

BOOL DxLobyStage::ShiftLeftChar()
{
	if( (m_nCharStart-MAX_SELECTCHAR) >= 0 )
	{
		m_nCharStart -= MAX_SELECTCHAR;
		return TRUE;
	}

	return FALSE;
}

BOOL DxLobyStage::ShiftCharIndex(INT nIndex )
{
	if( 0 <= nIndex * MAX_SELECTCHAR && nIndex * MAX_SELECTCHAR < m_nChaSNum )
	{
		if( nIndex * MAX_SELECTCHAR == m_nCharStart )
			return FALSE;

		m_nCharStart = nIndex * MAX_SELECTCHAR;
		m_bShiftButton = TRUE;
		return TRUE;
	}
	return FALSE;
}

void DxLobyStage::ShiftCharInfo()
{
	for(int i = 0; i < MAX_SELECTCHAR; i++ )
	{
		m_CharSham[ i ].ResetCharInfo();
	}

	for ( int i=m_nCharStart, j=0; i<m_nChaSNum; i++, j++ )
	{
		if( j >= MAX_SELECTCHAR ) 
			break;

		m_CharSham[j].SetCharInfo( m_CharInfo[i] );
	}
}

void DxLobyStage::ShiftChar( float fElapsedTime )
{
	DxInputDevice &dxInputDev = DxInputDevice::GetInstance();

	if( !m_bShiftButton )
	{
		//if( false == GfxInterfaceBase::GetInstance()->IsMouseHitUI() && 
		//	false == GfxInterfaceBase::GetInstance()->IsDragging() )
		{
			if( (dxInputDev.GetKeyState( DIK_LEFT )&DXKEY_DOWN) || m_bShiftLBDown )
			{
				if( ShiftLeftChar() )
				{
					m_bShiftButton = TRUE;
				}
			}
			else if( (dxInputDev.GetKeyState( DIK_RIGHT )&DXKEY_DOWN) || m_bShiftRBDown )
			{
				if( ShiftRightChar() )
				{
					m_bShiftButton = TRUE;
				}
			}

			if( m_bShiftButton )
			{
				//	Note : Ä³¸¯ÅÍ ¼±ÅÃÀ» Ãë¼Ò.
				m_dwCharSelect = MAX_SELECTCHAR;

				for ( int i=0; i<MAX_SELECTCHAR; i++ )
				{
					m_CharSham[i].ResetCharInfo ();
					m_CharSham[i].SetSelect ( FALSE );

					InitRenderSkip();
				}
			}
		}
	}

	if( m_bShiftButton )
	{
		m_fShiftTime += fElapsedTime;

		if( m_fShiftTime > 0.5f )
		{
			ShiftCharInfo();

			m_fShiftTime = 0;
			m_bShiftButton = FALSE;
			m_bShiftLBDown = FALSE;
			m_bShiftRBDown = FALSE;
		}
	}
}

//***********************************************
// ÃÊ±â Ä³¸¯ÅÍ ·»´õ¸µ½Ã ¾Ö´Ï¸ÞÀÌ¼ÇÀ» 1ÃÊÁ¤µµ ½ºÅµ
//***********************************************
void DxLobyStage::InitRenderSkip()
{
	m_bRenderSkip = TRUE;
	m_fRenderSkipTime = 0.0f;
}

HRESULT DxLobyStage::OneTimeSceneInit ( HWND hWnd, WORD wWidth, WORD wHeight, const char* szAppPath )
{
	GASSERT(hWnd);

	StringCchCopy ( m_szAppPath, MAX_PATH, szAppPath );
	m_hWnd = hWnd;

	m_dwCharSelect = MAX_SELECTCHAR;
	m_nCharStart = 0;

	return S_OK;
}

HRESULT DxLobyStage::InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	HRESULT hr;

	m_pd3dDevice = pd3dDevice;

	// ·Îºñ¿¡¼­´Â Texture Quality ¸¦ ÃÖ°í·Î ÇØÁÖÀÚ.
	// °­Á¦·Î ¿øÇÏ´Â ¼ÂÆÃÀ» ³Ö´Â´Ù.
	RENDERPARAM::SetTextureQuality_FORCE( TEXTURE_QUALITY_HIGH );

	hr = m_pd3dDevice->GetDeviceCaps ( &m_d3dCaps );
	if ( FAILED(hr) )	return hr;

	WORD wWidth = 800;
	WORD wHeight = 600;

	//	Note : ÇöÀçÇØ»óµµ ¹Þ¾Æ¿À±â.
	//
	LPDIRECT3DSURFACEQ pBackBuffer=NULL;
	m_pd3dDevice->GetBackBuffer ( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );

	if ( pBackBuffer )
	{
		pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
		pBackBuffer->Release();

		wWidth = static_cast<WORD> ( m_d3dsdBackBuffer.Width );
		wHeight = static_cast<WORD> ( m_d3dsdBackBuffer.Height );
	}

	if ( m_pOuterInterface ) 
	{
		m_pOuterInterface->ISetResolution ( MAKELONG ( wHeight, wWidth ) );
		m_pOuterInterface->SetResolutionBack ( MAKELONG ( 768, 1024 ) );
		m_pOuterInterface->IOneTimeSceneInit();
	}

	hr = m_pOuterInterface->IInitDeviceObjects ( m_pd3dDevice );
	if ( FAILED ( hr ) ) return hr;

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( FALSE );
    //---------------------------------------------------//

	//	Note : ºäÆ÷Æ®.
	//
	D3DXVECTOR3 vFromPt		= D3DXVECTOR3( 0.0f, 70.0f, -70.0f );
	D3DXVECTOR3 vLookatPt	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	DxViewPort::GetInstance().SetViewTrans ( vFromPt, vLookatPt, vUpVec );

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( FALSE );
    //---------------------------------------------------//

	//	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¹è°æ ÁöÇü ÆÄÀÏ ÃÊ±âÈ­.
	//
	m_spLandCharSlt->InitDeviceObjects ( m_pd3dDevice );
	m_spLandCharSlt->LoadFile ( "cha_select.wld", m_pd3dDevice, FALSE );

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( FALSE );
    //---------------------------------------------------//

	//m_LandCharSlt_s01.InitDeviceObjects ( m_pd3dDevice );
	//m_LandCharSlt_s01.LoadFile ( "character_slt_s01.wld", m_pd3dDevice, FALSE );

 //   //---------------------------------------------------//
 //   NSLoadingDirect::FrameMoveRender( FALSE );
 //   //---------------------------------------------------//

	//m_LandCharSlt_s02.InitDeviceObjects ( m_pd3dDevice );
	//m_LandCharSlt_s02.LoadFile ( "character_slt_s02.wld", m_pd3dDevice, FALSE );

 //   //---------------------------------------------------//
 //   NSLoadingDirect::FrameMoveRender( FALSE );
 //   //---------------------------------------------------//

	//m_LandCharSlt_s03.InitDeviceObjects ( m_pd3dDevice );
	//m_LandCharSlt_s03.LoadFile ( "character_slt_s03.wld", m_pd3dDevice, FALSE );

 //   //---------------------------------------------------//
 //   NSLoadingDirect::FrameMoveRender( FALSE );
 //   //---------------------------------------------------//

	m_spLandLogin->InitDeviceObjects ( m_pd3dDevice );
	m_spLandLogin->LoadFile ( "log_in.wld", m_pd3dDevice, FALSE );

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( FALSE );
    //---------------------------------------------------//

	// Note : ¹Ì¸® ·Îµù..
	D3DXVECTOR3 vMax = D3DXVECTOR3(100000,100000,100000);
	D3DXVECTOR3 vMin = D3DXVECTOR3(-100000,-100000,-100000);
	m_spLandCharSlt->LoadBasicPos( m_pd3dDevice, vMax, vMin );
	//m_LandCharSlt_s01.LoadBasicPos( m_pd3dDevice, vMax, vMin );
	//m_LandCharSlt_s02.LoadBasicPos( m_pd3dDevice, vMax, vMin );
	//m_LandCharSlt_s03.LoadBasicPos( m_pd3dDevice, vMax, vMin );
	m_spLandLogin->LoadBasicPos( m_pd3dDevice, vMax, vMin );

	//	NOTE
	//		BGM ¼¼ÆÃ, ·Îºñ´Â °íÁ¤
	m_spLandLogin->SetBgmFile( ID2GAMEWORD("LOBY_BGM") );		// °­Á¦ ¼ÂÆÃ.
	m_spLandCharSlt->SetBgmFile( ID2GAMEWORD("LOBY_BGM") );	// °­Á¦ ¼ÂÆÃ.

	m_spLandLogin->ActiveMap ( m_spLandLogin );

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( FALSE );
    //---------------------------------------------------//

	//	NOTE
	//		BGM ¼¼ÆÃ, ·Îºñ´Â °íÁ¤
	//DxBgmSound::GetInstance().SetFile ( ID2GAMEWORD("LOBY_BGM") );
	//DxBgmSound::GetInstance().Play ();

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( FALSE );
    //---------------------------------------------------//

	//	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¿ÀºêÁ§Æ® ÃÊ±âÈ­.
	//
	char szName[MAX_PATH] = {0};
	for ( int i=0; i<MAX_SELECTCHAR; i++ )
	{
		StringCchPrintf ( szName, MAX_PATH, "character[%02d]", i+1 );
		DxLandGateMan* pLandGateMan = m_spLandCharSlt->GetLandGateMan();
		PDXLANDGATE pLandGate = pLandGateMan->FindLandGate ( szName );
		GASSERT(pLandGate&&"character[%02d]¸¦ Ã£Áö ¸øÇÏ¿´½À´Ï´Ù." );
		if ( pLandGate )
		{
            //---------------------------------------------------//
            NSLoadingDirect::FrameMoveRender( FALSE );
            //---------------------------------------------------//

			m_vCharPos[ i ] = (pLandGate->GetMax()+pLandGate->GetMin())/2.0f;
			m_CharSham[i].Create( &m_vCharPos[ i ], m_pd3dDevice, TRUE, i );
		}
	}

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( FALSE );
    //---------------------------------------------------//

	DxSkinCharDataContainer::GetInstance().LoadData( "chs_kt.chf", pd3dDevice );
	DxSkinCharDataContainer::GetInstance().LoadData( "chs_ac.chf", pd3dDevice );
	DxSkinCharDataContainer::GetInstance().LoadData( "chs_mg.chf", pd3dDevice );

    //---------------------------------------------------//
    NSLoadingDirect::FrameMoveRender( FALSE );
    //---------------------------------------------------//

	StringCchPrintf ( szName, MAX_PATH, "Create_Char" );
	DxLandGateMan* pLandGateMan = m_spLandCharSlt->GetLandGateMan();
	PDXLANDGATE pLandGate = pLandGateMan->FindLandGate ( szName );
	GASSERT(pLandGate&&"Create_Char¸¦ Ã£Áö ¸øÇÏ¿´½À´Ï´Ù." );
	if ( pLandGate )
	{
		D3DXVECTOR3 vPos = (pLandGate->GetMax()+pLandGate->GetMin())/2.0f;
		m_NewCharSham.Create( &vPos, m_pd3dDevice, TRUE, MAX_SELECTCHAR );
	}

	// wld ·Îµù½Ã ZoomOut °ªÀÌ º¯ÇÏ±â ¶§¹®¿¡ Lobby¿ë ZoomOut °ªÀ¸·Î Àç ¼ÂÆÃÇÔ.
	DxViewPort::GetInstance().SetLobyCamera ();
	return S_OK;
}

HRESULT DxLobyStage::RestoreDeviceObjects ()
{
	HRESULT hr;
	LPDIRECT3DSURFACEQ pBackBuffer;
	m_pd3dDevice->GetBackBuffer ( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );

    pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
    pBackBuffer->Release();

	//	Note : ºäÆ÷Æ®
	//
	DxViewPort::GetInstance().SetProjection ( D3DX_PI/4, (float)m_d3dsdBackBuffer.Width, 
		(float)m_d3dsdBackBuffer.Height, 5.0f, 3000.0f );
	
	hr = DxViewPort::GetInstance().SetViewPort ();
	if ( FAILED(hr) )	return hr;

	if ( m_pOuterInterface ) m_pOuterInterface->IRestoreDeviceObjects(m_pd3dDevice);

	//	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¹è°æ ÁöÇü ÆÄÀÏ ÃÊ±âÈ­.
	//
	m_spLandCharSlt->RestoreDeviceObjects ( m_pd3dDevice );
	//m_LandCharSlt_s01.RestoreDeviceObjects ( m_pd3dDevice );
	//m_LandCharSlt_s02.RestoreDeviceObjects ( m_pd3dDevice );
	//m_LandCharSlt_s03.RestoreDeviceObjects ( m_pd3dDevice );

	m_spLandLogin->RestoreDeviceObjects ( m_pd3dDevice );

	//	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¿ÀºêÁ§Æ® ÃÊ±âÈ­.
	//
	for ( int i=0; i<MAX_SELECTCHAR; i++ )
	{
		m_CharSham[i].RestoreDeviceObjects ( m_pd3dDevice );
	}

	m_NewCharSham.RestoreDeviceObjects( m_pd3dDevice );

	m_setCheckGpuElapsedTime.clear();
	m_fCheckGpuTimeADD = 0.f;
	m_nCheckGpuCountADD = 0;

	// [shhan][2015.04.03] ¿ø·¡´Â FrameMove ÂÊ¿¡ ÀÖ¾ú´Âµ¥ ±×·² ÇÊ¿ä´Â ¾ø¾î¼­ ÀÌ°÷À¸·Î ¿Å°å´Ù.
	//						¶ÇÇÑ ¸»µµ ¾ÈµÇ´Â Crash °¡ ¹ß»ýÇÏ¿© È®ÀÎÇÏ±â À§ÇÔµµ ÀÖ´Ù.
	GLPeriod::GetInstance().SetPeriod ( 2003, 180, 1, 0 );
	return S_OK;
}

HRESULT DxLobyStage::InvalidateDeviceObjects ()
{
	//	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¹è°æ ÁöÇü ÆÄÀÏ ÃÊ±âÈ­.
	//
	if ( m_pOuterInterface ) m_pOuterInterface->IInvalidateDeviceObjects();
	m_spLandCharSlt->InvalidateDeviceObjects ();
	//m_LandCharSlt_s01.InvalidateDeviceObjects ();
	//m_LandCharSlt_s02.InvalidateDeviceObjects ();
	//m_LandCharSlt_s03.InvalidateDeviceObjects ();

	m_spLandLogin->InvalidateDeviceObjects ();

	//	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¿ÀºêÁ§Æ® ÃÊ±âÈ­.
	//
	for ( int i=0; i<MAX_SELECTCHAR; i++ )
	{
		m_CharSham[i].InvalidateDeviceObjects ();
	}

	m_NewCharSham.InvalidateDeviceObjects();
    return S_OK;
}

HRESULT DxLobyStage::DeleteDeviceObjects ()
{
	//	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¹è°æ ÁöÇü ÆÄÀÏ ÃÊ±âÈ­.
	//
	if ( m_pOuterInterface ) m_pOuterInterface->IDeleteDeviceObjects();

	m_spLandCharSlt->DeleteDeviceObjects ();
	//m_LandCharSlt_s01.DeleteDeviceObjects ();
	//m_LandCharSlt_s02.DeleteDeviceObjects ();
	//m_LandCharSlt_s03.DeleteDeviceObjects ();

	m_spLandLogin->DeleteDeviceObjects ();

	DxSkinCharDataContainer::GetInstance().ReleaseData ( "select01.chf" );
	DxSkinCharDataContainer::GetInstance().ReleaseData ( "select02.chf" );

	//DxBgmSound::GetInstance().ForceStop ();

	m_textureRes.Clear();

	// ¿ÀÅä ¿É¼Ç È®ÀÎ ´Ù ¸øÇß´Âµ¥ ·ÎºñÈ­¸éÀÌ ³¡³µ´Ù¸é, LOW ¿É¼ÇÀ¸·Î °­Á¦ ¼ÂÆÃÇÑ´Ù.
	//if ( !RANPARAM::IsAutoOptionSame() )
	//{
	//	RENDERPARAM::g_emTnLMode = TnL_PS_1_1;
	//	DxShadowMap::GetInstance().SetShadowDetail( LAND_QT_LOW );

	//	//RANPARAM::SetAutoOptionVer();
	//}

	return S_OK;
}

HRESULT DxLobyStage::FinalCleanup ()
{
	//	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¹è°æ ÁöÇü ÆÄÀÏ ÃÊ±âÈ­.
	//
	if ( m_pOuterInterface ) m_pOuterInterface->IFinalCleanup();

	m_spLandCharSlt->CleanUp ();
 //   m_LandCharSlt_s01.CleanUp ();
	//m_LandCharSlt_s02.CleanUp ();
	//m_LandCharSlt_s03.CleanUp ();

    m_spLandLogin->CleanUp ();

	m_bGameJoin = FALSE;

	//	Note : ¾È¾²´Â ÅØ½ºÃÄµéÀÌ³ª Á¤º¸µéÀ» µé°íÀÖ±â ¶§¹®¿¡ Á¤¸®¸¦ ÇØÁØ´Ù.
	//
	for ( int i=0; i<MAX_SELECTCHAR; i++ )
	{
		m_CharSham[i].DeleteSkin();
	}

	m_NewCharSham.DeleteSkin();

	return S_OK;
}

HRESULT DxLobyStage::FrameMove( float fTime, float fElapsedTime, BOOL bNotRendering )
{
	m_fElapsedTime = fElapsedTime;

	DxInputDevice &dxInputDev = DxInputDevice::GetInstance();

	if( true == m_bChangedStage )
	{
	//	GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_EnterLoginStage );
		m_bChangedStage = false;
	}
	

	if( m_bUseInterface )
	{
		if ( m_pOuterInterface ) m_pOuterInterface->IFrameMove ( m_pd3dDevice, fElapsedTime );
	}

	DWORD MouseLKey = /*false == GfxInterfaceBase::GetInstance()->IsMouseHitUI() && 
					  false == GfxInterfaceBase::GetInstance()->IsDragging() ? */
						dxInputDev.GetMouseState( DXMOUSE_LEFT ) /*: 0*/;

	if( (dxInputDev.GetKeyState( DIK_F12 )&DXKEY_DOWN) )
	{
		m_bUseInterface = !m_bUseInterface;
	}

	NSCubeMapUtil::NSCubeMapBlend::OnFrameMove( fElapsedTime );

	UIGUID ControlID = m_pOuterInterface->GetRenderStage();

	static UIGUID ControlIDBack = NO_ID;
	if ( ControlIDBack != ControlID )
	{
		switch ( ControlIDBack )
		{
			case SELECT_CHINA_AREA_PAGE:
			case SELECT_SERVER_PAGE:
			case LOGIN_PAGE:
			case PASS_KEYBOARD:
			case SECPASS_SETPAGE:
			case SECPASS_CHECKPAGE:
			case REGISTER_PAGE:
				{

				}
				break;

			case SELECT_CHARACTER_PAGE:
			case SELECT_CHARACTER_LEFTPAGE:
			case SELECT_COUNTRY_UI:
			case SELECT_CONTINENT_LIST_UI:
			case SELECT_COUNTRY_LIST_UI:
			case LOCK_SELECT_CHARACTER_PREVIEW_PAGE:
			case SELECT_COUNTRY_WINDOW:
			case SELECT_CLUB_WINDOW:
			case SELECT_CHARACTER_USERPANEL:
			case CHANGEPASS_PAGE:
			case CHANGEPIN_PAGE:
			case CHANGEEMAIL_PAGE:
			case RESETPIN_PAGE:
			case TOPUP_PAGE:
			case GAMETIMECVT_PAGE:
			case PREMIUMPOINTCVT_PAGE:
				{
					for ( int i=0; i<MAX_SELECTCHAR; i++ )
					{
						m_CharSham[i].SetSelect ( FALSE );
						//m_CharSham[i].FrameMove ( 0, 0 );
					}
				}
				break;

			case CREATE_CHARACTER_PAGE:
			case CREATE_CHARACTER_BUTTON_PAGE:
			case CREATE_CHARACTER_WEB:
				{
					m_NewCharSham.SetSelect ( FALSE );				
					//#endif
				}
				break;
		};

		DxWeatherMan::GetInstance()->ReSetSound ();

		switch ( ControlID )
		{
		case SELECT_CHINA_AREA_PAGE:
		case SELECT_SERVER_PAGE:
		case LOGIN_PAGE:
		case PASS_KEYBOARD:
		case SECPASS_SETPAGE:
		case SECPASS_CHECKPAGE:
		case REGISTER_PAGE:
			{
				if(  ControlIDBack==LOGIN_PAGE ||
					ControlIDBack==PASS_KEYBOARD || 
					ControlIDBack==SELECT_CHINA_AREA_PAGE ||
					ControlIDBack==SELECT_SERVER_PAGE ||
					ControlIDBack==SECPASS_SETPAGE ||
					ControlIDBack==SECPASS_CHECKPAGE ||
					ControlIDBack==REGISTER_PAGE ) 
					break;

				/*m_spLandLogin->ActiveMap ( m_spLandLogin );
				m_spLandLogin->SetNSLandThread();
				DxCamAniMan* pCamAniMan = m_spLandLogin->GetCamAniMan();
				pCamAniMan->ActiveCameraPos ( "login" );*/

				DxEnvironmentRender( m_spLandLogin.get() );
				{
					m_spLandLogin->ActiveMap ( m_spLandLogin );
					m_spLandLogin->SetNSLandThread();
					DxCamAniMan* pCamAniMan = m_spLandLogin->GetCamAniMan();
					pCamAniMan->ActiveCameraPos ( "login" );

					// 속도 체크를 위한 렌더를 하도록 한다.
					DxRenderDefferedOpaque( m_spLandLogin.get() );
					DxRenderDefferedSoftAlpha( m_spLandLogin.get() );
					m_spLandLogin->RenderEff_Deffered( m_pd3dDevice, DxViewPort::GetInstance().GetClipVolume() );

					//			m_pd3dDevice->SetRenderTarget( 0, DxSurfaceTex::GetInstance().m_pColorBuffer );
					m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER, 0L, 1.0f, 0L );
				}
			
				if ( GLCONST_CHAR::wENABLE_NEW_CLASS == 7 )
				{
					D3DXVECTOR3 vFromPt		= D3DXVECTOR3( 350.94f, 0.09f, -5.45 );
					D3DXVECTOR3 vLookatPt	= D3DXVECTOR3( -0.64f, 0.0f, 0.03f );
					D3DXVECTOR3 vUpVec		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
					DxViewPort::GetInstance().SetViewTrans ( vFromPt, vLookatPt, vUpVec );
				}
			}
			break;

		case SELECT_CHARACTER_PAGE:
		case SELECT_CHARACTER_LEFTPAGE:
		case SELECT_COUNTRY_UI:
		case SELECT_CONTINENT_LIST_UI:
		case SELECT_COUNTRY_LIST_UI:
		case LOCK_SELECT_CHARACTER_PREVIEW_PAGE:
		case SELECT_COUNTRY_WINDOW:
		case SELECT_CLUB_WINDOW:

		case SELECT_CHARACTER_USERPANEL:
		case CHANGEPASS_PAGE:
		case CHANGEPIN_PAGE:
		case CHANGEEMAIL_PAGE:
		case RESETPIN_PAGE:
		case TOPUP_PAGE:
		case GAMETIMECVT_PAGE:
		case PREMIUMPOINTCVT_PAGE:
			{
				if(  ControlIDBack==SELECT_CHARACTER_PAGE ||
					ControlIDBack==SELECT_CHARACTER_LEFTPAGE || 
					ControlIDBack==SELECT_COUNTRY_UI ||
					ControlIDBack==SELECT_CONTINENT_LIST_UI ||
					ControlIDBack==SELECT_COUNTRY_LIST_UI ||
					ControlIDBack==LOCK_SELECT_CHARACTER_PREVIEW_PAGE ||
					ControlIDBack==SELECT_COUNTRY_WINDOW ||
					ControlIDBack==SELECT_CLUB_WINDOW ||
					ControlIDBack==SELECT_CHARACTER_USERPANEL ||
					ControlIDBack==CHANGEPASS_PAGE ||
					ControlIDBack==CHANGEPIN_PAGE ||
					ControlIDBack==CHANGEEMAIL_PAGE ||
					ControlIDBack==RESETPIN_PAGE ||
					ControlIDBack==TOPUP_PAGE ||
					ControlIDBack==GAMETIMECVT_PAGE ||
					ControlIDBack==PREMIUMPOINTCVT_PAGE ) 
					break;

				//	¼±ÅÃµÈ ÄÉ¸¯ÅÍ ÃÊ±âÈ­.
				m_dwCharSelect = MAX_SELECTCHAR;

				m_spLandCharSlt->ActiveMap ( m_spLandCharSlt );
				m_spLandCharSlt->SetNSLandThread();
				DxCamAniMan* pCamAniMan = m_spLandCharSlt->GetCamAniMan();
				pCamAniMan->ActiveCameraPos ( "select_character" );

				InitRenderSkip();

				DxLightMan::GetInstance()->EnableLighting( true );
			}
			break;

		case CREATE_CHARACTER_PAGE:
		case CREATE_CHARACTER_BUTTON_PAGE:
		case CREATE_CHARACTER_WEB:
			{
				if ( ControlIDBack == CREATE_CHARACTER_PAGE || ControlIDBack == CREATE_CHARACTER_WEB || ControlIDBack == CREATE_CHARACTER_BUTTON_PAGE ) break;
				m_spLandCharSlt->ActiveMap ( m_spLandCharSlt );
				m_spLandCharSlt->SetNSLandThread();
				DxCamAniMan* pCamAniMan = m_spLandCharSlt->GetCamAniMan();
				pCamAniMan->ActiveCameraPos ( "create_camera" );

				InitRenderSkip();

				m_sCharNewInfo.RESET();
				SelCharSchool( m_sCharNewInfo.m_wSchool );
				// Ä³¸¯ÅÍ ¼ºº° Test
				// Ä«¸Þ¶ó ¹× Á¶¸í ¼ÂÆÃ				
				D3DXVECTOR3 vFromPt		= DxViewPort::GetInstance().GetFromPt();
				D3DXVECTOR3 vLookatPt	= DxViewPort::GetInstance().GetLookatPt();
				D3DXVECTOR3 vUpVec		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
				vFromPt.y = 19.0f;

				//DxViewPort::GetInstance().SetViewTrans ( vFromPt, vLookatPt, vUpVec );
				DxLightMan::GetInstance()->EnableLighting( true );					
				//#endif
			}
			break;
		};

		ControlIDBack = ControlID;
	}

	switch ( ControlID )
	{
	case SELECT_CHINA_AREA_PAGE:
	case SELECT_SERVER_PAGE:
	case LOGIN_PAGE:
	case PASS_KEYBOARD:
	case SECPASS_SETPAGE:
	case SECPASS_CHECKPAGE:
	case REGISTER_PAGE:
		{
			//	Note : ºäÆ÷Æ®
			//
			DxCamAniMan* pCamAniMan = m_spLandLogin->GetCamAniMan();
			pCamAniMan->ActiveCameraAni ( "login" );

			if ( GLCONST_CHAR::wENABLE_NEW_CLASS == 7 )
			{
				D3DXVECTOR3 vFromPt		= D3DXVECTOR3( 350.94f, 0.09f, -5.45 );
				D3DXVECTOR3 vLookatPt	= D3DXVECTOR3( -0.64f, 0.0f, 0.03f );
				D3DXVECTOR3 vUpVec		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
				DxViewPort::GetInstance().SetViewTrans ( vFromPt, vLookatPt, vUpVec );
			}

			DxViewPort::GetInstance().SetProjection ( D3DX_PI/4, (float)m_d3dsdBackBuffer.Width, 
				(float)m_d3dsdBackBuffer.Height, 5.f, DxFogMan::GetInstance().GetFogEnd_RealTime() );

			//GLPeriod::GetInstance().SetPeriod ( 2003, 180, 1, 0 );
			m_spLandLogin->FrameMove ( fTime, fElapsedTime, DxViewPort::GetInstance().GetClipVolume(), bNotRendering );
		}
		break;


	case SELECT_CHARACTER_PAGE:
	case SELECT_CHARACTER_LEFTPAGE:
	case SELECT_COUNTRY_UI:
	case SELECT_CONTINENT_LIST_UI:
	case SELECT_COUNTRY_LIST_UI:
	case LOCK_SELECT_CHARACTER_PREVIEW_PAGE:
	case SELECT_COUNTRY_WINDOW:
	case SELECT_CLUB_WINDOW:

	case SELECT_CHARACTER_USERPANEL:
	case CHANGEPASS_PAGE:
	case CHANGEPIN_PAGE:
	case CHANGEEMAIL_PAGE:
	case RESETPIN_PAGE:
	case TOPUP_PAGE:
	case GAMETIMECVT_PAGE:
	case PREMIUMPOINTCVT_PAGE:
		{
			DxCamAniMan* pCamAniMan = m_spLandCharSlt->GetCamAniMan();
			pCamAniMan->ActiveCameraPos ( "select_character" );

			//	Note : ºäÆ÷Æ®
			//

			D3DXVECTOR3 vFromPt		= DxViewPort::GetInstance().GetFromPt();
			D3DXVECTOR3 vLookatPt	= DxViewPort::GetInstance().GetLookatPt();
			D3DXVECTOR3 vUpVec		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
			DxViewPort::GetInstance().SetViewTrans ( vFromPt, vLookatPt, vUpVec );
			DxLightMan::GetInstance()->EnableLighting( true );
			DxViewPort::GetInstance().SetProjection ( D3DX_PI/4, (float)m_d3dsdBackBuffer.Width, 
				(float)m_d3dsdBackBuffer.Height, 5.f, DxFogMan::GetInstance().GetFogEnd_RealTime() );

			//GLPeriod::GetInstance().SetPeriod ( 2003, 180, 1, 0 );

			// Å×½ºÆ®¿ë
			static int g_nFrameMoveCount(0);
			++g_nFrameMoveCount;
			int					_nStartCharNum(m_nStartCharNum);			//	¼­¹öÀÇ »ý¼ºµÈ ÄÉ¸¯ ¼ýÀÚ.
			int					_nStartCharLoad(m_nStartCharLoad);			//	Å¬¶óÀÌ¾ðÆ®¿¡ ·ÎµåµÈ ÄÉ¸¯ ¼ýÀÚ. ( Á¤º¸ ¼ö½ÅµÈ. )
			int					_nChaSNum(m_nChaSNum);					//	»ý¼ºµÈ ÄÉ¸¯ÅÍ ¼ýÀÚ. ( Á¾Àü ÄÉ¸¯ ¼ö + »õ·Î »ý¼ºµÈ ÄÉ¸¯ ¼ö )
			DWORD				_dwCharSelect(m_dwCharSelect);
			int					_nCharStart(m_nCharStart);
			int					_nFrameMoveCount(g_nFrameMoveCount);

			//	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¹è°æ ÁöÇü ÆÄÀÏ.
			//
			m_spLandCharSlt->FrameMove ( fTime, fElapsedTime, DxViewPort::GetInstance().GetClipVolume(), bNotRendering );

			DWORD dwCharSelect = MAX_SELECTCHAR;
			if ( MouseLKey&(DXKEY_UP|DXKEY_DUP) && m_bGameJoin == FALSE )
			{
				D3DXVECTOR3 vFrom, vLook;
				vFrom = DxViewPort::GetInstance().GetFromPt ();
				BOOL bOk = DxViewPort::GetInstance().GetMouseTargetPosWnd ( vLook );

				if ( bOk )
				{
					//for ( int i=0; i<m_nChaSNum; i++ )
					for ( int i=0; i<MAX_SELECTCHAR; i++ )
					{
						if ( !m_CharSham[i].IsValidData() )		continue;

						D3DXVECTOR3 vMax, vMin;
						m_CharSham[i].GetAABB ( vMax, vMin );

						BOOL bCollision = COLLISION::IsCollisionLineToAABB ( vFrom, vLook, vMax, vMin );
						if ( bCollision )
						{
							dwCharSelect = i;

							//GLWidgetScript::GetInstance().LuaCallFunc< void >(
							//	NSWIDGET_SCRIPT::g_strFunc_CharSelect_SetSelecrCharIndex, "-n, -n", dwCharSelect, m_nCharStart );

							m_pOuterInterface->SetSelectCharIndex ( dwCharSelect, m_nCharStart );

							m_pGlobalStage->GetGameStage()->SetSelectChar( &m_CharInfo[dwCharSelect+m_nCharStart] );

							break;
						}
					}
				}
			}
			//	Note : Å¬¸¯µÈ ÄÉ¸¯ÀÌ ÀÖÀ»¶§.
			//
			SelectCharacter( dwCharSelect );

			if ( dwCharSelect != MAX_SELECTCHAR )
			{
				if ( MouseLKey&DXKEY_DUP && !m_bGameJoin )
				{
					//	TODO : ¼±ÅÃµÈ ÄÉ¸¯À¸·Î °ÔÀÓ Á¢¼Ó.
					//
					//GLLobyCharClient::GetInstance().JoinGame( m_dwCharSelect + m_nCharStart );
					SCHARINFO_LOBBY* pCharInfo = GetCharInfo( m_dwCharSelect + m_nCharStart );
					if( NULL == pCharInfo )
						return false;

					m_pGlobalStage->GetGameStage()->SetSelectChar( pCharInfo );

					if( NULL == pCharInfo || true == pCharInfo->IsLock() )
					{
						m_pGlobalStage->GetLobyStage()->m_bGameJoin = FALSE;
						return false;
					}
					else
					{
						m_pGlobalStage->GetLobyStage()->m_bGameJoin = TRUE;
						GLMSG::SNETLOBBY_REQ_GAME_JOIN NetMsgReq;
						m_pGlobalStage->GetLobyStage()->GetOuterInterface()->MsgProcess ( &NetMsgReq );
					}
				}
			}

			ShiftChar( fElapsedTime ); // Ä³¸¯ÅÍ ½¬ÇÁÆ®


			// ShiftChar ¿¡¼­ Ä³¸¯ÅÍ°¡ º¯°æµÉ ¼öµµ ÀÖÀ¸¹Ç·Î ¼ø¼­¸¦ ¹Ù²Û´Ù.
			D3DXVECTOR3 vDir = DxViewPort::GetInstance().GetFromPt() - DxViewPort::GetInstance().GetLookatPt();

			if( m_bRenderSkip )
			{
				m_fRenderSkipTime += fElapsedTime;
				if ( m_fRenderSkipTime > 1.f )
				{
					m_bRenderSkip = FALSE;
				}
			}

			//	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¿ÀºêÁ§Æ®.
			//
			if ( !m_bRenderSkip )
			{
				for ( int i=0; i<MAX_SELECTCHAR; i++ )
				{
					m_CharSham[i].SetDirection ( vDir );
					m_CharSham[i].FrameMove ( fTime, fElapsedTime );
				}
			}
		}
		break;

		case CREATE_CHARACTER_PAGE:
		case CREATE_CHARACTER_BUTTON_PAGE:
		case CREATE_CHARACTER_WEB:
		{
			//if ( m_bStyle )
			{
				DxCamAniMan* pCamAniMan = m_spLandCharSlt->GetCamAniMan();
				pCamAniMan->ActiveCameraAni ( "create_camera" );

				//	Note : ºäÆ÷Æ®
				//

				D3DXVECTOR3 vFromPt		= DxViewPort::GetInstance().GetFromPt();
				D3DXVECTOR3 vLookatPt	= DxViewPort::GetInstance().GetLookatPt();
				D3DXVECTOR3 vUpVec		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
				DxViewPort::GetInstance().SetViewTrans ( vFromPt, vLookatPt, vUpVec );
				DxLightMan::GetInstance()->EnableLighting( true );
				DxViewPort::GetInstance().SetProjection ( D3DX_PI/4, (float)m_d3dsdBackBuffer.Width, 
					(float)m_d3dsdBackBuffer.Height, 5.f, DxFogMan::GetInstance().GetFogEnd_RealTime() );

				//GLPeriod::GetInstance().SetPeriod ( 2003, 180, 1, 0 );

				//	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¹è°æ ÁöÇü ÆÄÀÏ.
				//
				m_spLandCharSlt->FrameMove ( fTime, fElapsedTime, DxViewPort::GetInstance().GetClipVolume(), bNotRendering );

				m_NewCharSham.SetSelect( FALSE );
				m_NewCharSham.SetDirection( m_vDir );

				m_NewCharSham.m_wFace = m_sCharNewInfo.m_wFace;
				m_NewCharSham.m_wHair = m_sCharNewInfo.m_wHair;
				m_NewCharSham.m_wHairColor = m_sCharNewInfo.m_wHairColor;

				m_NewCharSham.UpdateFaceHair();

				if( m_bRenderSkip )
				{
					m_fRenderSkipTime += fElapsedTime;
					if ( m_fRenderSkipTime > 0.2f )
					{
						m_bRenderSkip = FALSE;
					}
				}

				//	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¿ÀºêÁ§Æ®.
				//
				if ( !m_bRenderSkip )
				{
					m_NewCharSham.FrameMove( fTime, fElapsedTime );
				}
			}
			//#endif
		}
		break;
	};


	/*int nCurStageBG = GfxInterfaceBase::GetInstance()->GetStageBG();
	if( m_nStageBG != nCurStageBG )
	{
		// [2012.01.18] shhan
		// ½ºÅ×ÀÌÁö°¡ º¯°æµÇ¸é ½Ã°£À» 0À¸·Î °¡µµ·ÏÇÑ´Ù.
		// ÀÌ·¸°Ô ¾È ÇØÁÙ °æ¿ì ·Î±×ÀÎ È­¸éÀÌ Ã³À½ ³ª¿Ã ½Ã±â¿¡ fElapsedTime °ªÀº ¾ó¸¶³ª Å« °ªÀÌ ³ª¿ÃÁö´Â ¾Æ¹«µµ ¸ð¸¥´Ù.
		{
			fElapsedTime = 0.f;
		}

		switch ( m_nStageBG )
		{
		case EMSTAGE_BG_LOGIN :
			break;

		case EMSTAGE_BG_SELECT_CHAR :
			for ( int i=0; i<MAX_SELECTCHAR; i++ )
			{
				m_CharSham[i].SetSelect ( FALSE );
				//m_CharSham[i].FrameMove ( 0, 0 );
			}
			break;

		case EMSTAGE_BG_CREATE_CHAR :
			{
				m_NewCharSham.SetSelect ( FALSE );
				//m_NewCharSham.FrameMove ( 0, 0 );
			}
			break;
		}

		DxWeatherMan::GetInstance()->ReSetSound ();
		//		DxBgmSound::GetInstance().Stop ();

		switch( nCurStageBG )
		{
		case EMSTAGE_BG_LOGIN :
			{
				m_spLandLogin->ActiveMap ( m_spLandLogin );
				m_spLandLogin->SetNSLandThread();
				DxCamAniMan* pCamAniMan = m_spLandLogin->GetCamAniMan();
				pCamAniMan->ActiveCameraPos ( "login" );
			}
			break;

		case EMSTAGE_BG_SELECT_CHAR :
			{
				//	¼±ÅÃµÈ ÄÉ¸¯ÅÍ ÃÊ±âÈ­.
				m_dwCharSelect = MAX_SELECTCHAR;

				m_spLandCharSlt->ActiveMap ( m_spLandCharSlt );
				m_spLandCharSlt->SetNSLandThread();
				DxCamAniMan* pCamAniMan = m_spLandCharSlt->GetCamAniMan();
				pCamAniMan->ActiveCameraPos ( "select_character" );

				InitRenderSkip();

				DxLightMan::GetInstance()->EnableLighting( true );
			}
			break;

		case EMSTAGE_BG_CREATE_CHAR :
			{
				m_spLandCharSlt->ActiveMap ( m_spLandCharSlt );
				m_spLandCharSlt->SetNSLandThread();
				DxCamAniMan* pCamAniMan = m_spLandCharSlt->GetCamAniMan();
				pCamAniMan->ActiveCameraPos ( "create_camera" );

				InitRenderSkip();

				m_sCharNewInfo.RESET();
				SelCharSchool( m_sCharNewInfo.m_wSchool );
				// Ä³¸¯ÅÍ ¼ºº° Test
				// Ä«¸Þ¶ó ¹× Á¶¸í ¼ÂÆÃ				
				D3DXVECTOR3 vFromPt		= DxViewPort::GetInstance().GetFromPt();
				D3DXVECTOR3 vLookatPt	= DxViewPort::GetInstance().GetLookatPt();
				D3DXVECTOR3 vUpVec		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
				vFromPt.y = 19.0f;

				//DxViewPort::GetInstance().SetViewTrans ( vFromPt, vLookatPt, vUpVec );
				DxLightMan::GetInstance()->EnableLighting( true );					
				//#endif

			}
			break;
		};

		m_nStageBG = nCurStageBG;
	//}*/

	//switch ( nCurStageBG )
	//{
	//case EMSTAGE_BG_LOGIN :
	//	{
	//		//	Note : ºäÆ÷Æ®
	//		//
	//		DxCamAniMan* pCamAniMan = m_spLandLogin->GetCamAniMan();
	//		pCamAniMan->ActiveCameraAni ( "login" );

	//		DxViewPort::GetInstance().SetProjection ( D3DX_PI/4, (float)m_d3dsdBackBuffer.Width, 
	//			(float)m_d3dsdBackBuffer.Height, 5.f, DxFogMan::GetInstance().GetFogEnd_RealTime() );

	//		//GLPeriod::GetInstance().SetPeriod ( 2003, 180, 1, 0 );
	//		m_spLandLogin->FrameMove ( fTime, fElapsedTime, DxViewPort::GetInstance().GetClipVolume(), bNotRendering );
	//	}
	//	break;

	//case EMSTAGE_BG_SELECT_CHAR :
	//	{
	//		DxCamAniMan* pCamAniMan = m_spLandCharSlt->GetCamAniMan();
	//		pCamAniMan->ActiveCameraPos ( "select_character" );

	//		//	Note : ºäÆ÷Æ®
	//		//

	//		D3DXVECTOR3 vFromPt		= DxViewPort::GetInstance().GetFromPt();
	//		D3DXVECTOR3 vLookatPt	= DxViewPort::GetInstance().GetLookatPt();
	//		D3DXVECTOR3 vUpVec		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	//		DxViewPort::GetInstance().SetViewTrans ( vFromPt, vLookatPt, vUpVec );
	//		DxLightMan::GetInstance()->EnableLighting( true );
	//		DxViewPort::GetInstance().SetProjection ( D3DX_PI/4, (float)m_d3dsdBackBuffer.Width, 
	//			(float)m_d3dsdBackBuffer.Height, 5.f, DxFogMan::GetInstance().GetFogEnd_RealTime() );

	//		//GLPeriod::GetInstance().SetPeriod ( 2003, 180, 1, 0 );

	//		// Å×½ºÆ®¿ë
	//		static int g_nFrameMoveCount(0);
	//		++g_nFrameMoveCount;
	//		int					_nStartCharNum(m_nStartCharNum);			//	¼­¹öÀÇ »ý¼ºµÈ ÄÉ¸¯ ¼ýÀÚ.
	//		int					_nStartCharLoad(m_nStartCharLoad);			//	Å¬¶óÀÌ¾ðÆ®¿¡ ·ÎµåµÈ ÄÉ¸¯ ¼ýÀÚ. ( Á¤º¸ ¼ö½ÅµÈ. )
	//		int					_nChaSNum(m_nChaSNum);					//	»ý¼ºµÈ ÄÉ¸¯ÅÍ ¼ýÀÚ. ( Á¾Àü ÄÉ¸¯ ¼ö + »õ·Î »ý¼ºµÈ ÄÉ¸¯ ¼ö )
	//		DWORD				_dwCharSelect(m_dwCharSelect);
	//		int					_nCharStart(m_nCharStart);
	//		int					_nFrameMoveCount(g_nFrameMoveCount);

	//		//	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¹è°æ ÁöÇü ÆÄÀÏ.
	//		//
	//		m_spLandCharSlt->FrameMove ( fTime, fElapsedTime, DxViewPort::GetInstance().GetClipVolume(), bNotRendering );

	//		DWORD dwCharSelect = MAX_SELECTCHAR;
	//		if ( MouseLKey&(DXKEY_UP|DXKEY_DUP) && m_bGameJoin == FALSE )
	//		{
	//			D3DXVECTOR3 vFrom, vLook;
	//			vFrom = DxViewPort::GetInstance().GetFromPt ();
	//			BOOL bOk = DxViewPort::GetInstance().GetMouseTargetPosWnd ( vLook );

	//			if ( bOk )
	//			{
	//				//for ( int i=0; i<m_nChaSNum; i++ )
	//				for ( int i=0; i<MAX_SELECTCHAR; i++ )
	//				{
	//					if ( !m_CharSham[i].IsValidData() )		continue;

	//					D3DXVECTOR3 vMax, vMin;
	//					m_CharSham[i].GetAABB ( vMax, vMin );

	//					BOOL bCollision = COLLISION::IsCollisionLineToAABB ( vFrom, vLook, vMax, vMin );
	//					if ( bCollision )
	//					{
	//						dwCharSelect = i;

	//						GLWidgetScript::GetInstance().LuaCallFunc< void >(
	//							NSWIDGET_SCRIPT::g_strFunc_CharSelect_SetSelecrCharIndex, "-n, -n", dwCharSelect, m_nCharStart );

	//						m_pGlobalStage->GetGameStage()->SetSelectChar( &m_CharInfo[dwCharSelect+m_nCharStart] );

	//						break;
	//					}
	//				}
	//			}
	//		}
	//		//	Note : Å¬¸¯µÈ ÄÉ¸¯ÀÌ ÀÖÀ»¶§.
	//		//
	//		SelectCharacter( dwCharSelect );

	//		if ( dwCharSelect != MAX_SELECTCHAR )
	//		{
	//			if ( MouseLKey&DXKEY_DUP && !m_bGameJoin )
	//			{
	//				//	TODO : ¼±ÅÃµÈ ÄÉ¸¯À¸·Î °ÔÀÓ Á¢¼Ó.
	//				//
	//				GLLobyCharClient::GetInstance().JoinGame( m_dwCharSelect + m_nCharStart );
	//			}
	//		}

	//		ShiftChar( fElapsedTime ); // Ä³¸¯ÅÍ ½¬ÇÁÆ®


	//		// ShiftChar ¿¡¼­ Ä³¸¯ÅÍ°¡ º¯°æµÉ ¼öµµ ÀÖÀ¸¹Ç·Î ¼ø¼­¸¦ ¹Ù²Û´Ù.
	//		D3DXVECTOR3 vDir = DxViewPort::GetInstance().GetFromPt() - DxViewPort::GetInstance().GetLookatPt();

	//		if( m_bRenderSkip )
	//		{
	//			m_fRenderSkipTime += fElapsedTime;
	//			if ( m_fRenderSkipTime > 1.f )
	//			{
	//				m_bRenderSkip = FALSE;
	//			}
	//		}

	//		//	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¿ÀºêÁ§Æ®.
	//		//
	//		if ( !m_bRenderSkip )
	//		{
	//			for ( int i=0; i<MAX_SELECTCHAR; i++ )
	//			{
	//				m_CharSham[i].SetDirection ( vDir );
	//				m_CharSham[i].FrameMove ( fTime, fElapsedTime );
	//			}
	//		}
	//	}
	//	break;

	//case EMSTAGE_BG_CREATE_CHAR :
	//	{
	//		//if ( m_bStyle )
	//		{
	//			DxCamAniMan* pCamAniMan = m_spLandCharSlt->GetCamAniMan();
	//			pCamAniMan->ActiveCameraAni ( "create_camera" );

	//			//	Note : ºäÆ÷Æ®
	//			//

	//			D3DXVECTOR3 vFromPt		= DxViewPort::GetInstance().GetFromPt();
	//			D3DXVECTOR3 vLookatPt	= DxViewPort::GetInstance().GetLookatPt();
	//			D3DXVECTOR3 vUpVec		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	//			DxViewPort::GetInstance().SetViewTrans ( vFromPt, vLookatPt, vUpVec );
	//			DxLightMan::GetInstance()->EnableLighting( true );
	//			DxViewPort::GetInstance().SetProjection ( D3DX_PI/4, (float)m_d3dsdBackBuffer.Width, 
	//				(float)m_d3dsdBackBuffer.Height, 5.f, DxFogMan::GetInstance().GetFogEnd_RealTime() );

	//			//GLPeriod::GetInstance().SetPeriod ( 2003, 180, 1, 0 );

	//			//	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¹è°æ ÁöÇü ÆÄÀÏ.
	//			//
	//			m_spLandCharSlt->FrameMove ( fTime, fElapsedTime, DxViewPort::GetInstance().GetClipVolume(), bNotRendering );

	//			m_NewCharSham.SetSelect( FALSE );
	//			m_NewCharSham.SetDirection( m_vDir );

	//			m_NewCharSham.m_wFace = m_sCharNewInfo.m_wFace;
	//			m_NewCharSham.m_wHair = m_sCharNewInfo.m_wHair;
	//			m_NewCharSham.m_wHairColor = m_sCharNewInfo.m_wHairColor;

	//			m_NewCharSham.UpdateFaceHair();

	//			if( m_bRenderSkip )
	//			{
	//				m_fRenderSkipTime += fElapsedTime;
	//				if ( m_fRenderSkipTime > 0.2f )
	//				{
	//					m_bRenderSkip = FALSE;
	//				}
	//			}

	//			//	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¿ÀºêÁ§Æ®.
	//			//
	//			if ( !m_bRenderSkip )
	//			{
	//				m_NewCharSham.FrameMove( fTime, fElapsedTime );
	//			}
	//		}
	//		//#endif
	//	}
	//	break;
	//};

	// Rendering ÀÌ º¯°æµÇ¾î¼­ ±âÁ¸ ¿É¼ÇÀ¸·Î ÇÃ·¹ÀÌ ÇÒ °æ¿ì Á¤»óÀûÀÎ ÇÃ·¹ÀÌ°¡ ºÒ°¡´ÉÇØÁ®¼­, 
	// ÇÁ·Î±×·¡¸Ó°¡ ¿øÇÒ ¶§, ¿É¼Ç È®ÀÎ ÈÄ ¾Ë¸Â´Â »ç¾çÀ¸·Î ¼ÂÆÃ ÇÒ ¼ö ÀÖµµ·Ï ÀÛ¾÷ÇØÁÜ.
/*	if ( !RANPARAM::IsAutoOptionSame() )
	{
		// Á¤»óÀûÀÎ fElapsedTime ÀÎÁö È®ÀÎÇÑ´Ù.
		// ÃÖ¼ÒÇÑ 10fps ÀÌÇÏ´Â µÇ¾ßÁö ¾Ê°Ú³ª? ¾ÈµÇ¸é º¼ ÇÊ¿äµµ ¾ø´Ù. DeleteDeviceObjects ¶§ TnL_PS_1_1 ·Î ¼ÂÆÃµÈ´Ù.
		// 0.f ´Â ¸»ÀÌ ¾ÈµÇ´Â °ªÀÌ´Ù.
		if ( fElapsedTime < 0.1f && fElapsedTime > 0.f )
		{
			m_fCheckGpuTimeADD += fElapsedTime;
			if ( m_fCheckGpuTimeADD > 1.f )	// ÃÊ¹Ý ÆÄÀÏ ·Îµù °°Àº º¯µ¿ »ç¾ç ¶§¹®¿¡, 1 ÃÊ Á¤µµÀÇ ½Ã°£Àº °è»ê¿¡¼­ Á¦¿ÜÇÑ´Ù.
			{
				m_setCheckGpuElapsedTime.insert( fElapsedTime );
				++m_nCheckGpuCountADD;

				// 50°³ÀÇ sample À» ¾ò¾î¼­ Áß¾Ó¿¡ °ªÀ» »ç¿ëÇÏµµ·Ï ÇÑ´Ù.
				if ( m_nCheckGpuCountADD >= 50 )
				{
					float fCheckGpuElapsedTimeAVG(1.f);
					DWORD dwCount(0);
					std::set<float>::iterator iter = m_setCheckGpuElapsedTime.begin();
					for ( ; iter!=m_setCheckGpuElapsedTime.end(); ++iter, ++dwCount )
					{
						if ( dwCount >= m_setCheckGpuElapsedTime.size()/2 )
						{
							fCheckGpuElapsedTimeAVG = (*iter);
							break;
						}
					}

//#define VISIBLE_MESSAGE_FPS
#ifdef VISIBLE_MESSAGE_FPS
					static TSTRINGSTREAM sstrName;
#endif
					//RENDERPARAM::SetTextureFiltering( TEXTURE_FILTER_ANISOTROPIC_8X );

					// ReleaseD·Î Ã¼Å©µÊ.
					//
					// ÆÄµ¿ ³ª¿ÔÀ» ¶§ÀÇ ·Î±×ÀÎ È­¸é
					//
					// CPU : i5-2500(3.3G)				GPU : Geforce GTX 460		ElapsedTime/fps : 0.00213xx, 469.0fps		TnL_PS_3_0_CSM
					//
					// CPU : Dual-Core E5200(2.5G)		GPU : Geforce 8400 GS		ElapsedTime/fps : 0.0378xxx, 26.3fps		TnL_PS_3_0_CSM
					// CPU : Dual-Core E5200(2.5G)		GPU : Geforce 8400 GS		ElapsedTime/fps : 0.0349xxx, 28.6fps		TnL_PS_3_0_NM
					// CPU : Dual-Core E5200(2.5G)		GPU : Geforce 8400 GS		ElapsedTime/fps : 0.0207xxx, 48.2fps		TnL_PS_3_0_DF
					// CPU : Dual-Core E5200(2.5G)		GPU : Geforce 8400 GS		ElapsedTime/fps : 0.0039xxx, 251.6fps		TnL_PS_1_1
					//
					// CPU : Dual-Core E5200(2.5G)		GPU : Intel Q45/Q43			ElapsedTime/fps : 0.0235xxx, 42fps			TnL_PS_3_0_DF
					// CPU : Dual-Core E5200(2.5G)		GPU : Intel Q45/Q43			ElapsedTime/fps : 0.00605xx, 165.0fps		TnL_PS_1_1
					//
					// CPU : Dual-Core E5200(2.5G)		GPU : Geforce 7600			ElapsedTime/fps : 0.00210xx, 475.9fps		TnL_PS_1_1	( D3DPMISCCAPS_MRTINDEPENDENTBITDEPTHS : FALSE )
					//
					// CPU : Dual-Core E8500(3.16G)		GPU : AMD HD 2600			ElapsedTime/fps : 0.0209xxx, 47.fps			TnL_PS_3_0_CSM
					// CPU : Dual-Core E8500(3.16G)		GPU : AMD HD 2600			ElapsedTime/fps : 0.0204xxx, 48.fps			TnL_PS_3_0_NM
					// CPU : Dual-Core E8500(3.16G)		GPU : AMD HD 2600			ElapsedTime/fps : 0.0120xxx, 83.fps			TnL_PS_3_0_DF
					// CPU : Dual-Core E8500(3.16G)		GPU : AMD HD 2600			ElapsedTime/fps : 0.00293xx, 340.fps		TnL_PS_1_1
					//
					// CPU : Core2 Duo E8500(3.16G)		GPU : Intel Q45/Q43			ElapsedTime/fps : 0.0195xxx, 51.1fps		TnL_PS_3_0_DF
					// CPU : Core2 Duo E8500(3.16G)		GPU : Intel Q45/Q43			ElapsedTime/fps : 0.00539xx, 185.0fps		TnL_PS_1_1
					//
					//
					// ±Ø°­ºÎ ¸®´º¾ó ³ª¿ÔÀ» ¶§ÀÇ ·Î±×ÀÎ È­¸é
					//
					// CPU : i5-2500(3.3G)				GPU : Geforce GTX 460		ElapsedTime/fps : 0.00266xx, 383.4fps		TnL_PS_3_0_CSM
					//
					// CPU : Dual-Core E5200(2.5G)		GPU : Intel Q45/Q43			ElapsedTime/fps : 0.0349xxx, 28.6fps		TnL_PS_3_0_DF
					// CPU : Dual-Core E5200(2.5G)		GPU : Intel Q45/Q43			ElapsedTime/fps : 0.0156xx,  64.0fps		TnL_PS_1_1		TnL_CHAR_PS_1_1
					//
					//
					if ( fCheckGpuElapsedTimeAVG > 0.01f && ( RENDERPARAM::g_emTnLMode == TnL_PS_3_0_CSM ||
															RENDERPARAM::g_emTnLMode == TnL_PS_3_0_NM ||
															RENDERPARAM::g_emTnLMode == TnL_PS_3_0_DF ) )		// 100fps ÀÌ»óÀº µÇ¾î¾ß ÇÑ´Ù. ( Test Rendering °ú ½ÇÁ¦ Rendering Áßº¹ÀÌ¶ó¼­ 100fps ·Î ÀâÀ½ )
					{
						// ÀÌÂÊÀº ÁöÇü¸¸ °Ë»çÇÑ´Ù.

						// ´Ü°è¸¦ ³·Ãá´Ù.
						if ( RENDERPARAM::g_emTnLMode == TnL_PS_3_0_CSM )
						{
							//RENDERPARAM::g_emTnLMode = TnL_PS_3_0_NM;						// DXPARAMSET::GetInstance().SetShadowChar ¿¡¼­ ¼ÂÆÃµÊ.
							//DxShadowMap::GetInstance().SetShadowDetail( LAND_QT_HIGH );	// DXPARAMSET::GetInstance().SetShadowChar ¿¡¼­ ¼ÂÆÃµÊ.
							DXPARAMSET::GetInstance().SetShadowChar( 3 );
							RENDERPARAM::SetTextureFiltering( TEXTURE_FILTER_ANISOTROPIC_2X );

#ifdef VISIBLE_MESSAGE_FPS
							sstrName << _T("TnL_PS_3_0_CSM : ");
							sstrName << fCheckGpuElapsedTimeAVG;
							sstrName << _T(" / ");
							sstrName << 1.f/fCheckGpuElapsedTimeAVG;
							sstrName << _T("\r\n");
#endif
						}
						else if ( RENDERPARAM::g_emTnLMode == TnL_PS_3_0_NM )
						{
							//RENDERPARAM::g_emTnLMode = TnL_PS_3_0_DF;						// DXPARAMSET::GetInstance().SetShadowChar ¿¡¼­ ¼ÂÆÃµÊ.
							//DxShadowMap::GetInstance().SetShadowDetail( LAND_QT_MIDDLE );	// DXPARAMSET::GetInstance().SetShadowChar ¿¡¼­ ¼ÂÆÃµÊ.
							DXPARAMSET::GetInstance().SetShadowChar( 2 );
							RENDERPARAM::SetTextureFiltering( TEXTURE_FILTER_TRILINEAR );

#ifdef VISIBLE_MESSAGE_FPS
							sstrName << _T(" TnL_PS_3_0_NM : ");
							sstrName << fCheckGpuElapsedTimeAVG;
							sstrName << _T(" / ");
							sstrName << 1.f/fCheckGpuElapsedTimeAVG;
							sstrName << _T("\r\n");
#endif
						}
						else if ( RENDERPARAM::g_emTnLMode == TnL_PS_3_0_DF )
						{
							//RENDERPARAM::g_emTnLMode = TnL_PS_1_1;						// DXPARAMSET::GetInstance().SetShadowChar ¿¡¼­ ¼ÂÆÃµÊ.
							//DxShadowMap::GetInstance().SetShadowDetail( LAND_QT_LOW );	// DXPARAMSET::GetInstance().SetShadowChar ¿¡¼­ ¼ÂÆÃµÊ.
							DXPARAMSET::GetInstance().SetShadowChar( 1 );
							RENDERPARAM::SetTextureFiltering( TEXTURE_FILTER_TRILINEAR );

#ifdef VISIBLE_MESSAGE_FPS
							sstrName << _T(" TnL_PS_3_0_DF : ");
							sstrName << fCheckGpuElapsedTimeAVG;
							sstrName << _T(" / ");
							sstrName << 1.f/fCheckGpuElapsedTimeAVG;
							sstrName << _T("\r\n");
#endif
						}
					}
					else
					{
						// 300fps ÀÌ»óÀº µÇ¾î¾ß ÇÑ´Ù. ¼öÄ¡°¡ ¹Ì´ÞÀÌ¶ó¸é ¿É¼ÇÀ» ³·Ãá´Ù. TnL_CHAR_PS_3_0 ´Â 300 fps
						if ( fCheckGpuElapsedTimeAVG > 0.0033f && RENDERPARAM::g_emCharacterQulity == TnL_CHAR_PS_3_0 )
						{
#ifdef VISIBLE_MESSAGE_FPS
							if ( RENDERPARAM::g_emTnLMode == TnL_PS_3_0_CSM )
							{
								sstrName << _T(" TnL_PS_3_0_CSM : ");
							}
							else if ( RENDERPARAM::g_emTnLMode == TnL_PS_3_0_NM )
							{
								sstrName << _T(" TnL_PS_3_0_NM : ");
							}
							else if ( RENDERPARAM::g_emTnLMode == TnL_PS_3_0_DF )
							{
								sstrName << _T(" TnL_PS_3_0_DF : ");
							}
							else if ( RENDERPARAM::g_emTnLMode == TnL_PS_1_1 )
							{
								sstrName << _T(" TnL_PS_1_1 : ");
							}
							else
							{
								sstrName << _T(" TnL_FIXED : ");
							}
#endif

							if ( RENDERPARAM::g_emCharacterQulity == TnL_CHAR_PS_3_0 )
							{
								//RENDERPARAM::g_emCharacterQulity = TnL_CHAR_PS_2_0;			// DXPARAMSET::GetInstance().SetSkinDetail ¿¡¼­ ¼ÂÆÃµÊ.
								//DxShadowMap::GetInstance().SetShadowDetail( LAND_QT_HIGH );	// DXPARAMSET::GetInstance().SetSkinDetail ¿¡¼­ ¼ÂÆÃµÊ.
								DXPARAMSET::GetInstance().SetSkinDetail( 1 );

#ifdef VISIBLE_MESSAGE_FPS
								sstrName << _T(" TnL_CHAR_PS_3_0 : ");
								sstrName << fCheckGpuElapsedTimeAVG;
								sstrName << _T(" / ");
								sstrName << 1.f/fCheckGpuElapsedTimeAVG;
								sstrName << _T("\r\n");
#endif
							}
						}
						else
						{
							// ¾Ë¸Â´Â ¼ÂÆÃÀÌ µÇ¾ú´Ù°í ¾Ë·ÁÁÜ.
							//RANPARAM::SetAutoOptionVer();
							//DXPARAMSET::GetInstance().SetVideoLevel( 0 );

#ifdef VISIBLE_MESSAGE_FPS
							if ( RENDERPARAM::g_emTnLMode == TnL_PS_3_0_CSM )
							{
								sstrName << _T(" TnL_PS_3_0_CSM : ");
							}
							else if ( RENDERPARAM::g_emTnLMode == TnL_PS_3_0_NM )
							{
								sstrName << _T(" TnL_PS_3_0_NM : ");
							}
							else if ( RENDERPARAM::g_emTnLMode == TnL_PS_3_0_DF )
							{
								sstrName << _T(" TnL_PS_3_0_DF : ");
							}
							else if ( RENDERPARAM::g_emTnLMode == TnL_PS_1_1 )
							{
								sstrName << _T(" TnL_PS_1_1 : ");
							}
							else
							{
								sstrName << _T(" TnL_FIXED : ");
							}

							if ( RENDERPARAM::g_emCharacterQulity == TnL_CHAR_PS_3_0 )
							{
								sstrName << _T(" TnL_CHAR_PS_3_0 : ");
							}
							else if ( RENDERPARAM::g_emCharacterQulity == TnL_CHAR_PS_2_0 )
							{
								sstrName << _T(" TnL_CHAR_PS_2_0 : ");
							}
							else
							{
								sstrName << _T(" TnL_CHAR_PS_1_1 : ");
							}

							sstrName << fCheckGpuElapsedTimeAVG;
							sstrName << _T(" / ");
							sstrName << 1.f/fCheckGpuElapsedTimeAVG;
							AfxMessageBox( sstrName.str().c_str() );
#endif
						}
					}

					m_setCheckGpuElapsedTime.clear();
					m_nCheckGpuCountADD = 0;
				}
			}
		}
	}*/

	return S_OK;
}

void DxLobyStage::SelectCharacter( DWORD dwCharSelect )
{
	if ( dwCharSelect != MAX_SELECTCHAR )
	{
		//	Note : ÄÉ¸¯ÅÍ ¼±ÅÃ È¿°ú Ç¥½Ã.
		//
		if( m_dwCharSelect != dwCharSelect )
		{
			m_dwCharSelect = dwCharSelect;

			for ( int i=0; i<MAX_SELECTCHAR; i++ )	
				m_CharSham[i].SetSelect ( FALSE, TRUE );

			m_CharSham[m_dwCharSelect].SetSelect ( TRUE );
		}
	}
}

void DxLobyStage::UpdateSelSkinChar ( DxSkinChar* pSkinObj, EMCHARINDEX emIndex, WORD wFace, WORD wHair, WORD wHairColor )
{
	if ( !pSkinObj )	return;

	const GLCONST_CHARCLASS &sCONST = GLCONST_CHAR::cCONSTCLASS[emIndex];

	if ( sCONST.dwHEADNUM > wFace )
	{
		std::string strHEAD_CPS = sCONST.strHEAD_CPS[wFace];
	
		PDXCHARPART pCharPart = NULL;
		pCharPart = pSkinObj->GetPiece(PIECE_HEAD);		//	ÇöÁ¦ ÀåÂø ½ºÅ².

		if ( !pCharPart || !pCharPart->GetFileName() || strcmp(strHEAD_CPS.c_str(),pCharPart->GetFileName()) )
		{
			pSkinObj->SetPiece( strHEAD_CPS.c_str(), m_pd3dDevice, 0, 0, EMSCD_ZERO );
		}
	}

	if ( sCONST.dwHAIRNUM > wHair )
	{
		std::string strHAIR_CPS = sCONST.strHAIR_CPS[wHair];
	
		PDXCHARPART pCharPart = NULL;
		pCharPart = pSkinObj->GetPiece(PIECE_HAIR);		//	ÇöÁ¦ ÀåÂø ½ºÅ².

		if ( !pCharPart || !pCharPart->GetFileName() || strcmp(strHAIR_CPS.c_str(),pCharPart->GetFileName()) )
		{
			pSkinObj->SetPiece( strHAIR_CPS.c_str(), m_pd3dDevice, 0, 0, EMSCD_ZERO );
		}
	}

	// Çì¾îÄÃ·¯ Àû¿ë
	pSkinObj->SetHairColor( wHairColor );
}

HRESULT DxLobyStage::DxEnvironmentRender( DxLandMan* pWLD )
{
	DxEnvironment::GetInstance().ResetReflectionCV();
	if ( DxSurfaceTex::GetInstance().m_pColorSuf_1st && RENDERPARAM::g_emGroundQulity >= TnL_PS_3_0_DF && pWLD && pWLD->GetUsedMaterialSystem() )
	{
		DxEnvironment::GetInstance().RenderReflectionMaterial( m_pd3dDevice, pWLD );
	}
	else
	{
		PROFILE_BEGIN2("Environment Render");
		DxEnvironment::GetInstance().Render( m_pd3dDevice, NULL, pWLD );
		PROFILE_END2("Environment Render");
	}

	// Note : ±×¸²ÀÚ Clear
	PROFILE_BEGIN2("DxShadowMap::Render");
	DxShadowMap::GetInstance().ClearShadow( m_pd3dDevice, pWLD->GetUsedMaterialSystem() );
	PROFILE_END2("DxShadowMap::Render");

	// ÁöÇü ±×¸²ÀÚ¸¦ »ý¼ºÇÏµµ·Ï ÇÑ´Ù. 
	// PS_3_0 ÀÌ»óÀÏ °æ¿ì¸¸ µ¿ÀÛÇÑ´Ù.
	if ( RENDERPARAM::g_emGroundQulity >= TnL_PS_3_0_CSM )
	{
		// ±×¸²ÀÚ ·»´õÀü¿¡ NSShadowLight::FrameMove ¸¦ È£ÃâÇØÁà¾ßÇÑ´Ù.
		NSShadowLight::FrameMove( m_fElapsedTime );
	}

	DxShadowMap::GetInstance().RenderShadowForLobbyStage( pWLD->GetUsedMaterialSystem() );
	pWLD->RenderShadow( m_pd3dDevice );

	return S_OK;
}

HRESULT DxLobyStage::MaterialSystemStretchRect( DxLandMan* pWLD )
{
	//if ( DxSurfaceTex::GetInstance().m_pColorSuf_1st && RENDERPARAM::g_emGroundQulity >= TnL_PS_3_0_DF && pWLD && pWLD->GetUsedMaterialSystem() )
	//{
	//	// Note : 
	//	D3DSURFACE_DESC	sDesc;
	//	LPDIRECT3DSURFACEQ pBackBuffer;
	//	m_pd3dDevice->GetBackBuffer ( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
	//	pBackBuffer->GetDesc ( &sDesc );
	//	pBackBuffer->Release();

	//	m_pd3dDevice->SetRenderTarget( 0, DxSurfaceTex::GetInstance().m_pColorBuffer );
	//}

	return S_OK;
}

void DxLobyStage::DxRenderDefferedOpaque( DxLandMan* pWLD )
{
	//////////////////////////////////////////////////////////////////////////
	//					RenderDefferedOpaqueForPSF
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );

	LPDIRECT3DSURFACEQ	pColorBuffer(NULL);
	if ( RENDERPARAM::g_bEnable_HDR )
	{
		pColorBuffer = DxSurfaceTex::GetInstance().m_pColorSuf16F_HDR;
	}
	else
	{
		pColorBuffer = DxSurfaceTex::GetInstance().m_pColorBuffer;
	}

	if ( RENDERPARAM::g_emGroundQulity == TnL_PS_3_0_DF )
	{
		NSDeffered::RenderDefferedForSM30DF
		(
			m_pd3dDevice,
			DxFogMan::GetInstance().GetFogColor(),
			pWLD,
			DxSurfaceTex::GetInstance().GetWidth(),
			DxSurfaceTex::GetInstance().GetHeight(),
			pColorBuffer,
			DxSurfaceTex::GetInstance().m_pColorSuf_1st,
			DxSurfaceTex::GetInstance().m_pColorSuf_2nd,
			DxSurfaceTex::GetInstance().m_pColorSuf_4th,
			DxSurfaceTex::GetInstance().m_pColorTex_1st,
			DxSurfaceTex::GetInstance().m_pColorTex_2nd, 
			DxSurfaceTex::GetInstance().m_pColorTex_4th, 
			FALSE,
			0.f,
			matWorld,
			DxViewPort::GetInstance().GetMatView(),
			DxViewPort::GetInstance().GetMatProj(),
			DxViewPort::GetInstance().GetClipVolume(),
			FALSE,
			TRUE,
			TRUE,
			TRUE
		);
	}
	else
	{
		NSDeffered::RenderDefferedForSM30NM
		(
			m_pd3dDevice,
			DxFogMan::GetInstance().GetFogColor(),
			pWLD,
			DxSurfaceTex::GetInstance().GetWidth(),
			DxSurfaceTex::GetInstance().GetHeight(),
			pColorBuffer,
			DxSurfaceTex::GetInstance().m_pColorSuf_1st,
			DxSurfaceTex::GetInstance().m_pColorSuf_4th,
			DxSurfaceTex::GetInstance().m_pColorSuf_2nd,
			DxSurfaceTex::GetInstance().m_pColorSuf_3rd,	// Color´Â unsign64 ¸¦ ¾²°Ú´Ù.
			DxSurfaceTex::GetInstance().m_pColorSuf_LBuffer,
			DxSurfaceTex::GetInstance().m_pSufFourCC_INTZ,
			DxSurfaceTex::GetInstance().m_pColorTex_1st,
			DxSurfaceTex::GetInstance().m_pColorTex_4th,
			DxSurfaceTex::GetInstance().m_pColorTex_2nd,
			DxSurfaceTex::GetInstance().m_pColorTex_3rd, 	// Color´Â unsign64 ¸¦ ¾²°Ú´Ù.
			DxSurfaceTex::GetInstance().m_pColorTex_LBuffer,
			DxSurfaceTex::GetInstance().m_pTexFourCC_INTZ, 
			matWorld,
			DxViewPort::GetInstance().GetMatView(),
			DxViewPort::GetInstance().GetMatProj(),
			DxViewPort::GetInstance().GetClipVolume(),
			FALSE
		);
	}

	// ÁöÇü ¹° ·»´õ¸µ
	NSWaterMain::Render( m_pd3dDevice );
}

void DxLobyStage::DxRenderDefferedSoftAlpha( DxLandMan* pWLD )
{
	LPDIRECT3DSURFACEQ	pColorBuffer(NULL);
	if ( RENDERPARAM::g_bEnable_HDR )
	{
		pColorBuffer = DxSurfaceTex::GetInstance().m_pColorSuf16F_HDR;
	}
	else
	{
		pColorBuffer = DxSurfaceTex::GetInstance().m_pColorBuffer;
	}

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );
	D3DCOLOR colorClear = D3DCOLOR_XRGB(0,0,0);
	if ( RENDERPARAM::g_emGroundQulity == TnL_PS_3_0_DF )
	{
		NSDeffered::RenderDefferedForSM30DF
		(
			m_pd3dDevice,
			colorClear,
			pWLD,
			DxSurfaceTex::GetInstance().GetWidth(),
			DxSurfaceTex::GetInstance().GetHeight(),
			pColorBuffer,
			DxSurfaceTex::GetInstance().m_pColorSuf_1st,
			DxSurfaceTex::GetInstance().m_pColorSuf_2nd,
			DxSurfaceTex::GetInstance().m_pColorSuf_4th,
			DxSurfaceTex::GetInstance().m_pColorTex_1st,
			DxSurfaceTex::GetInstance().m_pColorTex_2nd, 
			DxSurfaceTex::GetInstance().m_pColorTex_4th, 
			FALSE,
			0.f,
			matWorld,
			DxViewPort::GetInstance().GetMatView(),
			DxViewPort::GetInstance().GetMatProj(),
			DxViewPort::GetInstance().GetClipVolume(),
			TRUE,
			TRUE,
			TRUE,
			FALSE
		);
	}
	else
	{
		NSDeffered::RenderDefferedForSM30NM
		(
			m_pd3dDevice,
			colorClear,
			pWLD,
			DxSurfaceTex::GetInstance().GetWidth(),
			DxSurfaceTex::GetInstance().GetHeight(),
			pColorBuffer,
			DxSurfaceTex::GetInstance().m_pColorSuf_1st,
			DxSurfaceTex::GetInstance().m_pColorSuf_4th,
			DxSurfaceTex::GetInstance().m_pColorSuf_2nd,
			DxSurfaceTex::GetInstance().m_pColorSuf_3rd,	// Color´Â unsign64 ¸¦ ¾²°Ú´Ù.
			DxSurfaceTex::GetInstance().m_pColorSuf_LBuffer,
			DxSurfaceTex::GetInstance().m_pSufFourCC_INTZ,
			DxSurfaceTex::GetInstance().m_pColorTex_1st,
			DxSurfaceTex::GetInstance().m_pColorTex_4th,
			DxSurfaceTex::GetInstance().m_pColorTex_2nd,
			DxSurfaceTex::GetInstance().m_pColorTex_3rd, 	// Color´Â unsign64 ¸¦ ¾²°Ú´Ù.
			DxSurfaceTex::GetInstance().m_pColorTex_LBuffer,
			DxSurfaceTex::GetInstance().m_pTexFourCC_INTZ, 
			matWorld,
			DxViewPort::GetInstance().GetMatView(),
			DxViewPort::GetInstance().GetMatProj(),
			DxViewPort::GetInstance().GetClipVolume(),
			TRUE
		);
	}
}

HRESULT DxLobyStage::Render ()
{
	if ( DxFogMan::GetInstance().GetFogRange() != EMFR_OFF )
	{
		RenderWorldCharEffect();
	}

	if( m_bUseInterface )
	{
		if ( m_pOuterInterface ) m_pOuterInterface->IRender ( m_pd3dDevice );
	}

	return S_OK;
}

void DxLobyStage::RenderWorldCharEffect()
{
	D3DXVECTOR3 vDiffuse(1.f,1.f,1.f);
	D3DXVECTOR3 vPointLight(0.f,0.f,0.f);

	CLIPVOLUME &CV = DxViewPort::GetInstance().GetClipVolume ();

	if ( m_d3dCaps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX )	
	{
		m_pd3dDevice->SetRenderState ( D3DRS_FOGENABLE, TRUE );
	}

	// [2013.07.01] °­Á¦·Î Fog¸¦ ²ôµµ·Ï ÇÑ´Ù.
	//				ÇöÀç ¸Ê»ó ÀÌ·¸°Ô ÇÒ ¼ö ¹Û¿¡ ¾ø´Ù. ¹èÄ¡¸¦ ³Ê¸Ó ¸Ö¸®ÇØ³õ¾Ò´Ù. Á¤»óÀûÀÎ ¸ÊÀÌ ¸¸µé¾îÁö¸é µÇµ¹¸®µµ·Ï ÇÏÀÚ.
	DxFogMan::GetInstance().RenderFogSB ( m_pd3dDevice, TRUE );

	DxLandMan* pWLD( NULL );

	// g_emGroundQulity BackUp
	TnL_MODE emTnL_BackUp = RENDERPARAM::g_emGroundQulity;

	// CSM ¹®Á¦·Î ÀÎÇØ ÀÌ»óÇÏ°Ô ³ª¿À´Â ¹®Á¦°¡ ÀÖ¾î¼­ ¿É¼ÇÀÌ ³ôÀ» °æ¿ì ÇÑ´Ü°è ³·Ãç¼­ ·»´õ¸µ ÇÏµµ·Ï ÇÑ´Ù.
	//if ( RENDERPARAM::g_emGroundQulity >= TnL_PS_3_0_CSM )
	//{
	//	RENDERPARAM::g_emGroundQulity = TnL_PS_3_0_DF;
	//}

	////////////////////////////////////////////////////////////////////////////
	////							±×¸²ÀÚ

	//// ¼Óµµ È®ÀÎ
	//// ´Ù ±×¸° ÈÄ ZBuffer ¸¦ ´Ù Áö¿öÁÖ±â ¶§¹®¿¡ ºüÁø ±¸¿ª ¾øÀÌ ´Ù ±×¸®¸é ¹®Á¦°¡ ¾ø´Ù.
	//if ( !RANPARAM::IsAutoOptionSame() && RENDERPARAM::g_emTnLMode >= TnL_PS_3_0_DF && m_spLandLogin->GetUsedMaterialSystem() )
	//{
	//	DxEnvironmentRender( m_spLandLogin.get() );

	//	// ´Ü°è¸¦ ³·Ãá´Ù.
	//	if ( RENDERPARAM::g_emTnLMode == TnL_PS_3_0_CSM )
	//	{
	//		RENDERPARAM::g_emGroundQulity = TnL_PS_3_0_CSM;
	//	}
	//	else if ( RENDERPARAM::g_emTnLMode == TnL_PS_3_0_NM )
	//	{
	//		RENDERPARAM::g_emGroundQulity = TnL_PS_3_0_NM;
	//	}
	//	else if ( RENDERPARAM::g_emTnLMode == TnL_PS_3_0_DF )
	//	{
	//		RENDERPARAM::g_emGroundQulity = TnL_PS_3_0_DF;
	//	}

	//	// Test Rendering
	//	// ¿øÇÏ´Â ¿É¼ÇÀ¸·Î ·»´õ¸µÀ» ½ÃÀÛÇÏµµ·Ï ÇÑ´Ù.
	//	// Àú»ç¾ç¿¡¼­´Â ÀÌºÎºÐÀ» ·»´õÇÏ¸é¼­ ¸¹Àº ¼Óµµ ÀúÇÏ°¡ ÀÖÀ» °ÍÀÌ´Ù.
	//	{
	//		DxCamAniMan* pCamAniMan = m_spLandLogin->GetCamAniMan();
	//		pCamAniMan->ActiveCameraPos ( "login" );

	//		// ¼Óµµ Ã¼Å©¸¦ À§ÇÑ ·»´õ¸¦ ÇÏµµ·Ï ÇÑ´Ù.
	//		DxRenderDefferedOpaque( m_spLandLogin.get() );
	//		DxRenderDefferedSoftAlpha( m_spLandLogin.get() );
	//		m_spLandLogin->RenderEff_Deffered( m_pd3dDevice, DxViewPort::GetInstance().GetClipVolume() );

	//		//			m_pd3dDevice->SetRenderTarget( 0, DxSurfaceTex::GetInstance().m_pColorBuffer );
	//		m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_ZBUFFER, 0L, 1.0f, 0L );
	//	}

	//	// ¼Óµµ È®ÀÎÀ» À§ÇÏ¿© ±âº» ·»´õ¸µÀº 1_1 ¹öÁ¯À¸·Î µ¹¸®µµ·Ï ÇÑ´Ù.
	//	// ½ÇÁ¦ ·»´õ´Â ³·Àº Option À¸·Î ·»´õ¸µ ÇÑ´Ù.
	//	// ¼Óµµ È®ÀÎ ÁßÀÌ´Ù.
	//	RENDERPARAM::g_emGroundQulity = TnL_PS_1_1;
	//}

	//	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¹è°æ ÁöÇü ÆÄÀÏ.
	//

	UIGUID ControlID = m_pOuterInterface->GetRenderStage();
	switch ( ControlID )
	{
	case SELECT_CHINA_AREA_PAGE:
	case SELECT_SERVER_PAGE:
	case LOGIN_PAGE:
	case PASS_KEYBOARD:
	case SECPASS_SETPAGE:
	case SECPASS_CHECKPAGE:
	case REGISTER_PAGE:
		{
			pWLD = m_spLandLogin.get();

			DxEnvironmentRender( pWLD );

			if ( DxSurfaceTex::GetInstance().m_pColorSuf_1st && RENDERPARAM::g_emGroundQulity >= TnL_PS_3_0_DF && GetUsedMaterialSystem() )
			{
				DxRenderDefferedOpaque( pWLD );

				// Note : 
				OPTMManager::GetInstance().Render_HardAlpha();

				DxRenderDefferedSoftAlpha( pWLD );
				pWLD->RenderEff_Deffered( m_pd3dDevice, DxViewPort::GetInstance().GetClipVolume() );
			}
			else
			{
				m_spLandLogin->Render ( m_pd3dDevice, CV );

				// Note : 
				OPTMManager::GetInstance().Render_HardAlpha();

				m_spLandLogin->Render_LOW_SoftAlpha( m_pd3dDevice, CV );
				m_spLandLogin->Render_LOW_SingleEff( m_pd3dDevice, CV );
			}

			//MaterialSystemStretchRect( pWLD );
			//m_spLandLogin->Render_2Pass( m_pd3dDevice, CV, TRUE );
			//m_spLandLogin->RenderPickAlpha( m_pd3dDevice );
			DxWeatherMan::GetInstance()->Render ( m_pd3dDevice );
		}
		break;

	case SELECT_CHARACTER_PAGE:
	case SELECT_CHARACTER_LEFTPAGE:
	case SELECT_COUNTRY_UI:
	case SELECT_CONTINENT_LIST_UI:
	case SELECT_COUNTRY_LIST_UI:
	case LOCK_SELECT_CHARACTER_PREVIEW_PAGE:
	case SELECT_COUNTRY_WINDOW:
	case SELECT_CLUB_WINDOW:

	case SELECT_CHARACTER_USERPANEL:
	case CHANGEPASS_PAGE:
	case CHANGEPIN_PAGE:
	case CHANGEEMAIL_PAGE:
	case RESETPIN_PAGE:
	case TOPUP_PAGE:
	case GAMETIMECVT_PAGE:
	case PREMIUMPOINTCVT_PAGE:
		{
			pWLD = m_spLandCharSlt.get();
			DxCamAniMan* pCamAniMan = m_spLandCharSlt->GetCamAniMan();
			pCamAniMan->ActiveCameraPos ( "select_character" );

			DxEnvironmentRender( pWLD );

			if ( DxSurfaceTex::GetInstance().m_pColorSuf_1st && RENDERPARAM::g_emGroundQulity >= TnL_PS_3_0_DF && GetUsedMaterialSystem() )
			{
				DxRenderDefferedOpaque( pWLD );
			}
			else
			{
				m_spLandCharSlt->Render ( m_pd3dDevice, CV );
			}

			//	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¿ÀºêÁ§Æ®.
			//
			if( !m_bRenderSkip )
			{
				for ( int i=0; i<MAX_SELECTCHAR; i++ )
				{
					pWLD->CalculateActorWorldLight_Thread( vDiffuse, vPointLight, m_vCharPos[ i ] );
					m_CharSham[i].Render ( m_pd3dDevice, CV, FALSE, vDiffuse, vPointLight );

					int nTempIdx = m_nCharStart + i;
					if ( nTempIdx < 0 || nTempIdx >= MAX_SERVERCHAR )
						continue;

					// Note : Ä³¸¯ÅÍ Àá±Ý ÀÌ¹ÌÁö ·»´õ.
					if ( m_CharInfo[ nTempIdx ].IsLock() )
					{
						//!! SF_TODO
						m_pOuterInterface->ISetLockImagePos( m_vCharPos[ i ], i );
						m_pOuterInterface->ISetVisibleLockImage( true, i );
					}
				}
			}

			// Note : 
			OPTMManager::GetInstance().Render_HardAlpha();

			if ( DxSurfaceTex::GetInstance().m_pColorSuf_1st && RENDERPARAM::g_emGroundQulity >= TnL_PS_3_0_DF && GetUsedMaterialSystem() )
			{
				DxRenderDefferedSoftAlpha( pWLD );
				pWLD->RenderEff_Deffered( m_pd3dDevice, DxViewPort::GetInstance().GetClipVolume() );
			}
			else
			{
				m_spLandCharSlt->Render_LOW_SoftAlpha( m_pd3dDevice, CV );
				m_spLandCharSlt->Render_LOW_SingleEff( m_pd3dDevice, CV );
			}			

			//MaterialSystemStretchRect( pWLD );

			//m_spLandCharSlt->Render_2Pass( m_pd3dDevice, CV, TRUE );
			//m_spLandCharSlt->Render_EFF ( m_pd3dDevice, CV );
			//m_spLandCharSlt->RenderPickAlpha( m_pd3dDevice );

			DxWeatherMan::GetInstance()->Render ( m_pd3dDevice );
		}
		break;

	case CREATE_CHARACTER_PAGE:
	case CREATE_CHARACTER_BUTTON_PAGE:
	case CREATE_CHARACTER_WEB:
		{
			//m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET,	D3DCOLOR_XRGB(0,0,0), 1.0f, 0L );

			//if ( m_bStyle )
			{
				//m_pd3dDevice->SetTexture ( 0, m_textureRes.GetTexture() );
				//m_pd3dDevice->SetFVF( TEXTUREVERTEXFVF );
				//m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_Vertices, sizeof(TEXTUREVERTEX) );

				//m_pd3dDevice->SetTexture ( 0, NULL );

				pWLD = m_spLandCharSlt.get();
				DxCamAniMan* pCamAniMan = m_spLandCharSlt->GetCamAniMan();
				pCamAniMan->ActiveCameraPos ( "create_camera" );

				DxEnvironmentRender( pWLD );

				//m_spLandCharSlt->Render ( m_pd3dDevice, CV );

				if ( DxSurfaceTex::GetInstance().m_pColorSuf_1st && RENDERPARAM::g_emGroundQulity >= TnL_PS_3_0_DF && GetUsedMaterialSystem() )
				{
					DxRenderDefferedOpaque( pWLD );
				}
				else
				{
					m_spLandCharSlt->Render ( m_pd3dDevice, CV );
				}

				if( !m_bRenderSkip )
				{
					pWLD->CalculateActorWorldLight_Thread( vDiffuse, vPointLight, m_NewCharSham.GetPosition() );
					m_NewCharSham.Render ( m_pd3dDevice, CV, FALSE, vDiffuse, vPointLight );
				}

				// Note : 
				OPTMManager::GetInstance().Render_HardAlpha();

				if ( DxSurfaceTex::GetInstance().m_pColorSuf_1st && RENDERPARAM::g_emGroundQulity >= TnL_PS_3_0_DF && GetUsedMaterialSystem() )
				{
					DxRenderDefferedSoftAlpha( pWLD );
					pWLD->RenderEff_Deffered( m_pd3dDevice, DxViewPort::GetInstance().GetClipVolume() );
				}
				else
				{
					m_spLandCharSlt->Render_LOW_SoftAlpha( m_pd3dDevice, CV );
					m_spLandCharSlt->Render_LOW_SingleEff( m_pd3dDevice, CV );
				}	

				//MaterialSystemStretchRect( pWLD );

				//m_spLandCharSlt->Render_2Pass( m_pd3dDevice, CV, TRUE );
				//m_spLandCharSlt->Render_EFF ( m_pd3dDevice, CV );
				//m_spLandCharSlt->RenderPickAlpha( m_pd3dDevice );

				DxWeatherMan::GetInstance()->Render ( m_pd3dDevice );
			}
			//#endif
		}
		break;
	};

	//switch( m_nStageBG )
	//{
	//case EMSTAGE_BG_LOGIN :
	//	{
	//		pWLD = m_spLandLogin.get();

	//		DxEnvironmentRender( pWLD );

	//		if ( DxSurfaceTex::GetInstance().m_pColorSuf_1st && RENDERPARAM::g_emGroundQulity >= TnL_PS_3_0_DF && GetUsedMaterialSystem() )
	//		{
	//			DxRenderDefferedOpaque( pWLD );

	//			// Note : 
	//			OPTMManager::GetInstance().Render_HardAlpha();

	//			DxRenderDefferedSoftAlpha( pWLD );
	//			pWLD->RenderEff_Deffered( m_pd3dDevice, DxViewPort::GetInstance().GetClipVolume() );
	//		}
	//		else
	//		{
	//			m_spLandLogin->Render ( m_pd3dDevice, CV );

	//			// Note : 
	//			OPTMManager::GetInstance().Render_HardAlpha();

	//			m_spLandLogin->Render_LOW_SoftAlpha( m_pd3dDevice, CV );
	//			m_spLandLogin->Render_LOW_SingleEff( m_pd3dDevice, CV );
	//		}

	//		//MaterialSystemStretchRect( pWLD );
	//		//m_spLandLogin->Render_2Pass( m_pd3dDevice, CV, TRUE );
	//		//m_spLandLogin->RenderPickAlpha( m_pd3dDevice );
	//		DxWeatherMan::GetInstance()->Render ( m_pd3dDevice );
	//	}
	//	break;

	//case EMSTAGE_BG_SELECT_CHAR :
	//	{
	//		pWLD = m_spLandCharSlt.get();
	//		DxCamAniMan* pCamAniMan = m_spLandCharSlt->GetCamAniMan();
	//		pCamAniMan->ActiveCameraPos ( "select_character" );

	//		DxEnvironmentRender( pWLD );

	//		if ( DxSurfaceTex::GetInstance().m_pColorSuf_1st && RENDERPARAM::g_emGroundQulity >= TnL_PS_3_0_DF && GetUsedMaterialSystem() )
	//		{
	//			DxRenderDefferedOpaque( pWLD );
	//		}
	//		else
	//		{
	//			m_spLandCharSlt->Render ( m_pd3dDevice, CV );
	//		}

	//		//	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¿ÀºêÁ§Æ®.
	//		//
	//		if( !m_bRenderSkip )
	//		{
	//			for ( int i=0; i<MAX_SELECTCHAR; i++ )
	//			{
	//				pWLD->CalculateActorWorldLight_Thread( vDiffuse, vPointLight, m_vCharPos[ i ] );
	//				m_CharSham[i].Render ( m_pd3dDevice, CV, FALSE, vDiffuse, vPointLight );

	//				int nTempIdx = m_nCharStart + i;
	//				if ( nTempIdx < 0 || nTempIdx >= MAX_SERVERCHAR )
	//					continue;

	//				// Note : Ä³¸¯ÅÍ Àá±Ý ÀÌ¹ÌÁö ·»´õ.
	//				if ( m_CharInfo[ nTempIdx ].IsLock() )
	//				{
	//					//!! SF_TODO
	//					//m_pOutInterface->ISetLockImagePos( m_vCharPos[ i ], i );
	//					//m_pOutInterface->ISetVisibleLockImage( true, i );
	//				}
	//			}
	//		}

	//		// Note : 
	//		OPTMManager::GetInstance().Render_HardAlpha();

	//		if ( DxSurfaceTex::GetInstance().m_pColorSuf_1st && RENDERPARAM::g_emGroundQulity >= TnL_PS_3_0_DF && GetUsedMaterialSystem() )
	//		{
	//			DxRenderDefferedSoftAlpha( pWLD );
	//			pWLD->RenderEff_Deffered( m_pd3dDevice, DxViewPort::GetInstance().GetClipVolume() );
	//		}
	//		else
	//		{
	//			m_spLandCharSlt->Render_LOW_SoftAlpha( m_pd3dDevice, CV );
	//			m_spLandCharSlt->Render_LOW_SingleEff( m_pd3dDevice, CV );
	//		}			

	//		//MaterialSystemStretchRect( pWLD );

	//		//m_spLandCharSlt->Render_2Pass( m_pd3dDevice, CV, TRUE );
	//		//m_spLandCharSlt->Render_EFF ( m_pd3dDevice, CV );
	//		//m_spLandCharSlt->RenderPickAlpha( m_pd3dDevice );

	//		DxWeatherMan::GetInstance()->Render ( m_pd3dDevice );
	//	}
	//	break;

	//case EMSTAGE_BG_CREATE_CHAR :
	//	{
	//		//m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET,	D3DCOLOR_XRGB(0,0,0), 1.0f, 0L );

	//		//if ( m_bStyle )
	//		{
	//			//m_pd3dDevice->SetTexture ( 0, m_textureRes.GetTexture() );
	//			//m_pd3dDevice->SetFVF( TEXTUREVERTEXFVF );
	//			//m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_Vertices, sizeof(TEXTUREVERTEX) );

	//			//m_pd3dDevice->SetTexture ( 0, NULL );

	//			pWLD = m_spLandCharSlt.get();
	//			DxCamAniMan* pCamAniMan = m_spLandCharSlt->GetCamAniMan();
	//			pCamAniMan->ActiveCameraPos ( "create_camera" );

	//			DxEnvironmentRender( pWLD );

	//			//m_spLandCharSlt->Render ( m_pd3dDevice, CV );

	//			if ( DxSurfaceTex::GetInstance().m_pColorSuf_1st && RENDERPARAM::g_emGroundQulity >= TnL_PS_3_0_DF && GetUsedMaterialSystem() )
	//			{
	//				DxRenderDefferedOpaque( pWLD );
	//			}
	//			else
	//			{
	//				m_spLandCharSlt->Render ( m_pd3dDevice, CV );
	//			}

	//			if( !m_bRenderSkip )
	//			{
	//				pWLD->CalculateActorWorldLight_Thread( vDiffuse, vPointLight, m_NewCharSham.GetPosition() );
	//				m_NewCharSham.Render ( m_pd3dDevice, CV, FALSE, vDiffuse, vPointLight );
	//			}

	//			// Note : 
	//			OPTMManager::GetInstance().Render_HardAlpha();

	//			if ( DxSurfaceTex::GetInstance().m_pColorSuf_1st && RENDERPARAM::g_emGroundQulity >= TnL_PS_3_0_DF && GetUsedMaterialSystem() )
	//			{
	//				DxRenderDefferedSoftAlpha( pWLD );
	//				pWLD->RenderEff_Deffered( m_pd3dDevice, DxViewPort::GetInstance().GetClipVolume() );
	//			}
	//			else
	//			{
	//				m_spLandCharSlt->Render_LOW_SoftAlpha( m_pd3dDevice, CV );
	//				m_spLandCharSlt->Render_LOW_SingleEff( m_pd3dDevice, CV );
	//			}	

	//			//MaterialSystemStretchRect( pWLD );

	//			//m_spLandCharSlt->Render_2Pass( m_pd3dDevice, CV, TRUE );
	//			//m_spLandCharSlt->Render_EFF ( m_pd3dDevice, CV );
	//			//m_spLandCharSlt->RenderPickAlpha( m_pd3dDevice );

	//			DxWeatherMan::GetInstance()->Render ( m_pd3dDevice );
	//		}
	//		//#endif
	//	}
	//	break;

	//};

	//////////////////////////////////////////////////////////////////////////
	// [2014.08.27][shhan] Lobby ´Â Bloom À» º¸¿©ÁÖÁö ¾Ê´Â´Ù.
	//////////////////////////////////////////////////////////////////////////
	//			HDR - Bloom Àû¿ë.
	if ( RENDERPARAM::g_bEnable_HDR )
	{
		m_pd3dDevice->StretchRect( DxSurfaceTex::GetInstance().m_pColorSuf16F_HDR, NULL, DxSurfaceTex::GetInstance().m_pColorBuffer, NULL, D3DTEXF_POINT );
		m_pd3dDevice->SetRenderTarget( 0, DxSurfaceTex::GetInstance().m_pColorBuffer );
	}
	//NSBloomHDR::OnRender( m_pd3dDevice );
	//m_pd3dDevice->SetRenderTarget( 0, DxSurfaceTex::GetInstance().m_pColorBuffer );




	RENDERPARAM::g_emGroundQulity = emTnL_BackUp;

	if ( m_d3dCaps.RasterCaps & D3DPRASTERCAPS_FOGVERTEX )		m_pd3dDevice->SetRenderState ( D3DRS_FOGENABLE, FALSE );

	PROFILE_BEGIN("DxEffGroupPlayer::Render");
	DxEffGroupPlayer::GetInstance().Render ( m_pd3dDevice );
	PROFILE_END("DxEffGroupPlayer::Render");

	// Note : ÃÖÀûÈ­ÇÑ Eff
	OPTMManager::GetInstance().Render();


	// Note : LightShafts ÀÛ¾÷À» ÇÑ´Ù.
	{
		DxBackupTarget sBackupTarget;
		sBackupTarget.Backup( m_pd3dDevice );

		if ( pWLD )
		{
			pWLD->Render_LightShafts(m_pd3dDevice);
		}
		NSLightShafts::OnRender(m_pd3dDevice);
	}

	//	Note : Å¥ºê ÅØ½ºÃÄ ·»´õ¸µ..!!
	//DxCubeMap::GetInstance().Render ( m_pd3dDevice );
	NSCubeMapUtil::NSCubeMapBlend::OnRender( m_pd3dDevice, NSCubeMapCulling::GetCubeMapTexture( DxViewPort::GetInstance().GetLookatPt() ) );

	//	Note : ±Û·Î¿ì Ã³¸®¸¦ ÇÏ±âÀ§ÇØ »ç¿ë ÇÑ´Ù.
	//
	DxGlowMan::GetInstance().RenderTex ( m_pd3dDevice );
	DxGlowMan::GetInstance().Render ( m_pd3dDevice );
	DxGlowMan::GetInstance().RenderBURN( m_pd3dDevice );


	// StarGlow ·»´õ¸µ
	if ( DxSurfaceTex::GetInstance().m_pGlowSuf_SRC )
	{
		DxBackupTarget sBackupTarget;
		sBackupTarget.Backup( m_pd3dDevice );

		m_pd3dDevice->SetRenderTarget ( 0, DxSurfaceTex::GetInstance().m_pGlowSuf_SRC );
		m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0L);

		// StarGlow ·»´õ¸µ
		m_pGaeaClient->RenderStarGlow();

		// Note : Glow ÀÛ¾÷À» ÇÑ´Ù.
		NSGlow::OnRender( m_pd3dDevice, DxSurfaceTex::GetInstance().m_pColorBuffer );
	}

	if ( pWLD )
	{
		// Áö±Ý±îÁö ±×¸°°ÍÀ» ±¼ÀýÀ» À§ÇØ º¹»çÇØ³õ´Â´Ù.
		PROFILE_BEGIN("DxEnvironment::UserToRefraction()");
		DxEnvironment::GetInstance().UserToRefraction ( m_pd3dDevice );
		PROFILE_END("DxEnvironment::UserToRefraction()");
	}

	//	Note : Wave µÇ¾ß ÇÏ´Â°ÍÀ» ÀúÀåÈÄ ¸¶Áö¸·À¸·Î »Ñ¸°´Ù.
	//

	PROFILE_BEGIN("DxEnvironment::RenderWave()");
	DxEnvironment::GetInstance().RenderWave ( m_pd3dDevice, DxEffectMan::GetInstance().GetBumpTexture() );
	PROFILE_END("DxEnvironment::RenderWave()");

	// Note : ÈÄÃ³¸®
	//DxPostProcess::GetInstance().Render( m_pd3dDevice );
}

void DxLobyStage::MsgProcess ( NET_MSG_GENERIC* nmg )
{
	CString str;

	switch ( nmg->nType )
	{
	case NET_MSG_LOGIN_FB :
	case GS_NET_MSG_WB_LOGIN_FB : // GSÀÇ °æ¿ì ¿ùµå¹èÆ² Á¢¼Ó ¿©ºÎ È®ÀÎ
	case NET_MSG_REGISTER_FB:
	case CHINA_NET_MSG_PASS_FB: //userpanel
	case CHINA_NET_MSG_PIN_FB:
	case CHINA_NET_MSG_EMAIL_FB:
	case CHINA_NET_MSG_GT2VP_FB:
	case CHINA_NET_MSG_TOPUP_FB:
	case CHINA_NET_MSG_PP2VP_FB:
	case CHINA_NET_MSG_RESETPASS_FB:
	case CHINA_NET_MSG_RESETPASS2_FB:
	case CHINA_NET_MSG_RESETPIN_FB:
		{
			m_pOuterInterface->MsgProcess( nmg );
		}
		break;

	case NET_MSG_CHA_DEL_FB_OK :
		{
			NET_CHA_DEL_FB* pCharDelFb = (NET_CHA_DEL_FB*) nmg;
			//GLLoginClient::GetInstance().SetCharRemain( pCharDelFb->wChaRemain );

			m_pOuterInterface->SetCharRemain( pCharDelFb->wChaRemain );
		}
		// break ºüÁø°Ç ÀÇµµÀûÀÎ°ÅÀÓ;
	case NET_MSG_LOBBY_CHAR_JOIN_FB : 
	case NET_MSG_LOBBY_CHARJOIN_2NDPASS :
	case NET_MSG_CHA_DEL_FB_CLUB:
	case NET_MSG_CHA_DEL_FB_ERROR:
	case NET_MSG_LOBBY_CHINA_ERROR:
		{
			m_bGameJoin = FALSE;

	//		GLLobyCharClient::GetInstance().MsgProcess( nmg );
			m_pOuterInterface->MsgProcess( nmg );

			// ¿¡·¯ÀÏ °æ¿ì ÀçÁ¢¼Ó ¹æÁö
			if( NET_MSG_LOBBY_CHAR_JOIN_FB == nmg->nType )
			{
				GLMSG::SNETLOBBY_CHARJOIN_FB* pNetMsg = (GLMSG::SNETLOBBY_CHARJOIN_FB *) nmg;
				if( pNetMsg->emCharJoinFB == EMCJOIN_FB_ERROR )
				{
					m_bGameJoin = TRUE;
					// ·Î±×ÀÎ ÇÁ·Î¼¼½º È®ÀÎÀ» À§ÇÑ ·Î±×
					if( m_pGlobalStage->m_bLoginProcessLog) sc::writeLogError(std::string("LOGIN ERROR. JOIN ERROR CODE 0"));
				}
			}
		}
		break;

	case NET_MSG_PASSCHECK_FB:
	case JAPAN_NET_MSG_UUID:
		{
			//GLLobyCharClient::GetInstance().MsgProcess( nmg );
			m_pOuterInterface->MsgProcess( nmg );
		}
		break;

	case NET_MSG_CHA_NEW_FB :
		{
			//GLLobyCharCreateClient::GetInstance().MsgProcess( nmg );
			m_pOuterInterface->MsgProcess( nmg );
		}
		break;

	case NET_MSG_CHA_BAINFO_AC:
		{
			// ·Î±×ÀÎ ÇÁ·Î¼¼½º È®ÀÎÀ» À§ÇÑ ·Î±×
			if( m_pGlobalStage->m_bLoginProcessLog) sc::writeLogError(std::string("LOGIN PROCESS. Answer Basic Char Info End"));

			NET_CHA_BBA_INFO_AC* pPacket = (NET_CHA_BBA_INFO_AC*) nmg;

			m_nStartCharNum = m_nChaSNum = pPacket->m_ChaServerTotalNum;

            std::vector<DWORD> vecChaDbNum;
            for ( int i=0; i<m_nChaSNum; i++ )
            {
                m_nChaIDs[i] = pPacket->m_ChaDbNum[i];
                vecChaDbNum.push_back(m_nChaIDs[i]);				
            }
            m_pGlobalStage->GetNetClient()->SndChaBasicInfoPack(vecChaDbNum);
			// ·Î±×ÀÎ ÇÁ·Î¼¼½º È®ÀÎÀ» À§ÇÑ ·Î±×
			if( m_pGlobalStage->m_bLoginProcessLog) sc::writeLogError(std::string("LOGIN PROCESS. Request Detail Char Info"));


			//GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_CharSelect_InitList );

			// À¯ÀúµéÀÇ System Á¤º¸¸¦ ¼öÁýÇÑ´Ù.
			if ( m_pGaeaClient )
			{
				GLMSG::SNET_USER_SYSTEM_INFO_CA NetMsg
					(
					sc::getProcessorNumber(), 
					sc::getCpuSpeed(), 
					sc::getMemoryPhysicalTotal(), 
					RENDERPARAM::m_dwResolutionX,
					RENDERPARAM::m_dwResolutionY,
					RENDERPARAM::m_dwGpuVendor,
					RENDERPARAM::m_dwGpuModel,
					RENDERPARAM::m_dwShaderModel, 
					static_cast< int >( RANPARAM::emNation )
					); 

				m_pGaeaClient->NETSENDTOAGENT( &NetMsg );
			}

			// ·Îºñ ½ºÅ×ÀÌÁö¿¡¼­ ¸Þ¼¼Áö¹Ú½º´Â ¿ÀÁ÷ ÇÏ³ª¸¸ È°¼ºÈ­ µÉ ¼ö ÀÖ´Ù;
			// ¸¸¾à ´õÇÊ¿äÇÏ´Ù¸é ¾Æ·¡ ÄÝµÇ´Â ÇÔ¼ö³»¿ë ¼öÁ¤ÀÌ ÇÊ¿äÇÔ;
	//		GLWidgetScript::GetInstance().LuaCallFunc< void >(NSWIDGET_SCRIPT::g_strFunc_LobbyWaitMsgBoxClose);
		}
		break;

	case NET_MSG_LOBBY_CHARINFO_AC:
		MsgLobbyCharInfoAC(nmg);
		break;

	case NET_MSG_LOBBY_CHARINFO_AC_END:
		MsgLobbyCharInfoACEnd(nmg);
		break;

	case NET_MSG_DEFAULT_GAME_OPTION:
		{
			NET_DEFAULT_GAME_OPTION* pPacket = ( NET_DEFAULT_GAME_OPTION* ) nmg;
			GLCONST_CHAR::bPARTY_2OTHERSCHOOL = pPacket->bPARTY_2OTHERSCHOOL;
			GLCONST_CHAR::bCLUB_2OTHERSCHOOL = pPacket->bCLUB_2OTHERSCHOOL;
			GLCONST_CHAR::fITEM_DROP_SCALE = pPacket->fITEM_DROP_SCALE;
			GLCONST_CHAR::fMONEY_DROP_SCALE = pPacket->fMONEY_DROP_SCALE;
			GLCONST_CHAR::fITEM_DROP_LOWER_SCALE = pPacket->fITEM_DROP_LOWER_SCALE;
			GLCONST_CHAR::dwCRITICAL_MAX = pPacket->dwCRITICAL_MAX;
			GLCONST_CHAR::fNONPK_ITEM_DROP = pPacket->fNONPK_ITEM_DROP;
			GLCONST_CHAR::fPK_ITEM_DROP = pPacket->fPK_ITEM_DROP;
		}
		break;
	default:
		CDebugSet::ToListView ( "DxLobyStage::MsgProcess unknown message type %d", nmg->nType );
		break;
	};
}

void  DxLobyStage::SetStyleStep( bool bStyle )
{
	m_bStyle = bStyle;
	InitRenderSkip();

	if ( m_bStyle )
	{
		SCHARINFO_LOBBY sCharInfo;
		sCharInfo.m_emClass = CharIndexToClass( m_sCharNewInfo.m_emIndex ); // Á÷¾÷
	    sCharInfo.m_wSchool = m_sCharNewInfo.m_wSchool; // ÇÐ¿ø.
	    sCharInfo.m_wHair = m_sCharNewInfo.m_wHair; // Çì¾î½ºÅ¸ÀÏ
		sCharInfo.m_wFace = m_sCharNewInfo.m_wFace;
		sCharInfo.m_wHairColor = m_sCharNewInfo.m_wHairColor;

		SCHARDATA2& sCharData = GLCONST_CHAR::GET_CHAR_DATA2(m_sCharNewInfo.m_wSchool,m_sCharNewInfo.m_emIndex);

		sCharInfo.m_PutOnItems[SLOT_UPPER].Assign( sCharData.m_PutOnItems[SLOT_UPPER] );
		sCharInfo.m_PutOnItems[SLOT_LOWER].Assign( sCharData.m_PutOnItems[SLOT_LOWER] );		

		m_NewCharSham.SetCharInfo( sCharInfo );
		m_vDir = D3DXVECTOR3( 0.0f, 0.0f, 1.0f);

		//m_vDir = D3DXVECTOR3( -1.0f,-1.0f,-0.35f);
		
	}
	else
	{
		m_NewCharSham.ResetCharInfo();
	}
}

void  DxLobyStage::RotateChar( bool bLeft, float fDegree )
{
	/*if ( bLeft )
	{
		m_vDir.x += 0.05f;
		m_vDir.z -= 0.05f;		

		if ( m_vDir.x > 0.5f ) m_vDir.x = 0.5f;
		if ( m_vDir.z < 0.5f ) m_vDir.z = 0.5f;
	}
	else
	{
		m_vDir.x -= 0.05f;
		m_vDir.z += 0.05f;

		if ( m_vDir.x < -1.5f ) m_vDir.x = -1.5f;
		if ( m_vDir.z > 1.5f ) m_vDir.z = 1.5f;
	}*/

	if( false == bLeft )
		fDegree *= -1.0f;

	// yÃà ±âÁØÀ¸·Î È¸Àü½ÃÅ²´Ù.
	m_vDir = scmath::RotateVectorFromAxis( m_vDir, fDegree, D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
}

void DxLobyStage::MsgLobbyCharInfoAC( NET_MSG_GENERIC* nmg )
{
	// ·Î±×ÀÎ ÇÁ·Î¼¼½º È®ÀÎÀ» À§ÇÑ ·Î±×
	if( m_pGlobalStage->m_bLoginProcessLog) sc::writeLogError(std::string("LOGIN PROCESS. Answer Detail Char Info"));

    // NET_MSG_LOBBY_CHARINFO_AC
    // GLMSG::NET_LOBBY_CHARINFO_AC* pPacket = (GLMSG::NET_LOBBY_CHARINFO_AC*) nmg;
    NET_MSG_PACK* pPacket = (NET_MSG_PACK*) nmg;

    GLMSG::NET_LOBBY_CHARINFO_AC MsgData;
    if (!msgpack::BufferToObject(pPacket->DataBuffer(), pPacket->DataSize(), MsgData))
    {
        sc::writeLogError("DxLobyStage::MsgLobbyCharInfoAC");
        return;
    }

    BOOL bInserted = FALSE;

    // ±âÁ¸ Ä³¸¯ÅÍ Á¤º¸ ¼ö½Å½Ã.
    for (int i=0; i<m_nChaSNum; ++i)
    {
        if (m_nChaIDs[i] == MsgData.Data.m_dwCharID)
        {
            bInserted = TRUE;
            m_CharInfo[i] = MsgData.Data;

            if (i < MAX_SELECTCHAR)
                m_CharSham[i].SetCharInfo(m_CharInfo[i]);

            ++m_nStartCharLoad;
			
			// ÀüÀå¼­¹ö Á¢¼Ó Ä³¸¯ÅÍ ¾ÆÀÌµð ¼ÂÆÃ
			m_dwCharIDForWorldBattle = MsgData.Data.m_dwCharID;

            break;
        }
    }

    //	Note : »õ·ÎÀÌ »ý¼ºµÈ Ä³¸¯ÅÍ Á¤º¸ ¼ö½Å½Ã.
    if ( !bInserted )
    {
        if ( m_nChaSNum < MAX_SERVERCHAR )
        {
            m_nChaIDs[m_nChaSNum] = MsgData.Data.m_dwCharID;
            m_CharInfo[m_nChaSNum] = MsgData.Data;
            m_CharSham[m_nChaSNum%MAX_SELECTCHAR].SetCharInfo( m_CharInfo[m_nChaSNum] );

            ++m_nChaSNum;

			// ÀüÀå¼­¹ö Á¢¼Ó Ä³¸¯ÅÍ ¾ÆÀÌµð ¼ÂÆÃ
			m_dwCharIDForWorldBattle = MsgData.Data.m_dwCharID;

            ShiftRBDown();
        }
    }

	// Ä³¸¯ÅÍ ¸®½ºÆ® ÃÊ±âÈ­
	//GLWidgetScript::GetInstance().LuaCallFunc< void >( NSWIDGET_SCRIPT::g_strFunc_CharSelect_InitList );
}

// Ä³¸¯ÅÍ Á¤º¸ Àü¼Û
// ÀÚµ¿ ·Î±×ÀÎ ±â´ÉÀ» À§ÇØ¼­ Ãß°¡
void DxLobyStage::MsgLobbyCharInfoACEnd( NET_MSG_GENERIC* nmg )
{
	// ·Î±×ÀÎ ÇÁ·Î¼¼½º È®ÀÎÀ» À§ÇÑ ·Î±×
	if( m_pGlobalStage->m_bLoginProcessLog) sc::writeLogError(std::string("LOGIN PROCESS. Answer Detail Char Info End"));

	if( m_pGlobalStage->GetNetClient()->IsWorldBattleCharSelectEnd() )
	{
		// ·Î±×ÀÎ ÇÁ·Î¼¼½º È®ÀÎÀ» À§ÇÑ ·Î±×
		if( m_pGlobalStage->m_bLoginProcessLog) sc::writeLogError(std::string("LOGIN PROCESS. Answer Detail Char Info End ( Check Battle Server )"));

		// ÅëÇÕÀüÀå¼­¹ö¿¡´Â ÇÏ³ªÀÇ Ä³¸¯¹Û¿¡ ¾ø¾î¾ßÇÑ´Ù. µû¶ó¼­ ¹«Á¶°Ç ÀÎµ¦½º 0ÀÌ´Ù.
		// ³ªÁß¿¡ ¿©·¯ Ä³¸¯ÀÌ ÀÖ´Â°æ¿ì index¸¦ »ç¿ëÇØ¾ßÇÔ.
		SCHARINFO_LOBBY* pCharInfo = m_pGlobalStage->GetLobyStage()->GetCharInfo( 0 );
		if( NULL == pCharInfo )
			return;

		m_pGlobalStage->GetGameStage()->SetSelectChar( pCharInfo );
	
		if( NULL == pCharInfo || true == pCharInfo->IsLock() )
		{
			m_pGlobalStage->GetLobyStage()->m_bGameJoin = FALSE;

			// ·Î±×ÀÎ ÇÁ·Î¼¼½º È®ÀÎÀ» À§ÇÑ ·Î±×
			if( m_pGlobalStage->m_bLoginProcessLog) sc::writeLogError(std::string("LOGIN ERROR. Cha Info error"));
			return;
		}
		else
		{
			m_pGlobalStage->GetLobyStage()->m_bGameJoin = TRUE;
			m_pGlobalStage->GetNetClient()->SetWorldBattleCharSelectEnd(FALSE);
			m_pGlobalStage->GetNetClient()->SndGameJoin(m_dwCharIDForWorldBattle, NULL , FALSE);	
		}
	}
}


BOOL DxLobyStage::GetUsedMaterialSystem()
{
	//int nCurStageBG = GfxInterfaceBase::GetInstance()->GetStageBG();

	////	Note : Ä³¸¯ÅÍ ¼±ÅÃ ¹è°æ ÁöÇü ÆÄÀÏ.
	////
	//switch( nCurStageBG )
	//{
	//case EMSTAGE_BG_LOGIN :
	//	{
	//		return m_spLandLogin->GetUsedMaterialSystem();
	//	}
	//	break;

	//case EMSTAGE_BG_SELECT_CHAR :
	//case EMSTAGE_BG_CREATE_CHAR :
	//	{
	//		return m_spLandCharSlt->GetUsedMaterialSystem();
	//	}
	//	break;

	//default :
	//	return FALSE;
	//};

	UIGUID ControlID = m_pOuterInterface->GetRenderStage();
	switch ( ControlID )
	{
	case SELECT_CHINA_AREA_PAGE:
	case SELECT_SERVER_PAGE:
	case LOGIN_PAGE:
	case PASS_KEYBOARD:
	case SECPASS_SETPAGE:
	case SECPASS_CHECKPAGE:
	case REGISTER_PAGE:
		{
			return m_spLandLogin->GetUsedMaterialSystem();
		}
		break;

	case SELECT_CHARACTER_PAGE:
	case SELECT_CHARACTER_LEFTPAGE:
	case SELECT_COUNTRY_UI:
	case SELECT_CONTINENT_LIST_UI:
	case SELECT_COUNTRY_LIST_UI:
	case LOCK_SELECT_CHARACTER_PREVIEW_PAGE:
	case SELECT_COUNTRY_WINDOW:
	case SELECT_CLUB_WINDOW:
	case CREATE_CHARACTER_PAGE:
	case CREATE_CHARACTER_WEB:
	case SELECT_CHARACTER_USERPANEL:
	case CHANGEPASS_PAGE:
	case CHANGEPIN_PAGE:
	case CHANGEEMAIL_PAGE:
	case RESETPIN_PAGE:
	case TOPUP_PAGE:
	case GAMETIMECVT_PAGE:
	case PREMIUMPOINTCVT_PAGE:
		{
			return m_spLandCharSlt->GetUsedMaterialSystem();
		}
		break;
	};
}

void DxLobyStage::GetNewCharShanScreenRect(RECT& rc)
{
	D3DXVECTOR3 vMin, vMax, vScreenMin, vScreenMax;
	m_NewCharSham.GetAABB(vMin, vMax);
	vScreenMin = DxViewPort::GetInstance().ComputeVec3Project(&vMin, NULL);
	vScreenMax = DxViewPort::GetInstance().ComputeVec3Project(&vMax, NULL);
	rc.left = vScreenMin.x;
	rc.top = vScreenMin.y;
	rc.right = vScreenMax.x;
	rc.bottom = vScreenMax.y;
}