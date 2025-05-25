//-----------------------------------------------------------------------------
// File: Pch.h
//
// Desc: Header file to precompile
//
// Copyright (C) 1998-2000 Microsoft Corporation. All Rights Reserved.
//
//	Note : Max 4.x 버전에 들어 있는 플러그인 SDK의 디랙토리가 지정되어 있어야한다.
//			또한 케릭터 스트디오의 SDK의 디랙토리 또한 지정되어야 한다.
//
//-----------------------------------------------------------------------------
#ifndef __PCH__H
#define __PCH__H

#ifdef _MAX_2012
    #include "../../RanThirdParty/Max12_sdk/include/Max.h"
    #include "../../RanThirdParty/Max12_sdk/include/istdplug.h"
    #include "../../RanThirdParty/Max12_sdk/include/iparamb2.h"
    #include "../../RanThirdParty/Max12_sdk/include/iparamm2.h"
    #include "../../RanThirdParty/Max12_sdk/include/cs/phyexp.h"
    #include "../../RanThirdParty/Max12_sdk/include/stdmat.h"
    #include "../../RanThirdParty/Max12_sdk/include/iskin.h"
#else
    #include "../../RanThirdParty/Max11_sdk/include/Max.h"
    #include "../../RanThirdParty/Max11_sdk/include/istdplug.h"
    #include "../../RanThirdParty/Max11_sdk/include/iparamb2.h"
    #include "../../RanThirdParty/Max11_sdk/include/iparamm2.h"
    #include "../../RanThirdParty/Max11_sdk/include/cs/phyexp.h"
    #include "../../RanThirdParty/Max11_sdk/include/stdmat.h"
    #include "../../RanThirdParty/Max11_sdk/include/iskin.h"
#endif

#include "resource.h"
#include <assert.h>
#include <io.h>
#include <fcntl.h>
#include <direct.h>
#include <commdlg.h>

#include "d3dx9.h"

inline BOOL _streql ( char *str1, char *str2, int nComp )
{
	while ( (*str1==*str2) && (*str1) && (nComp>=0) )
	{
		++str1;
		++str2;
		--nComp;
	}

	return (nComp==0);
}

#endif