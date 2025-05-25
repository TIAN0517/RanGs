#include "stdafx.h"

#include "./OldUI/Display/ProgressDisplay.h"
#include "./InnerInterface.h"

//-----------------------------------------------------------------------------
#include "../SigmaCore/DebugInclude.h"
//-----------------------------------------------------------------------------

void CInnerInterface::ProgressDisplayShow ( const UI::EMPROGRESSDISPLAY_TYPE emType, const std::string& strText, const D3DCOLOR& TextColor )
{
    m_pProgressDisplay->Show( emType, strText, TextColor );
}


void CInnerInterface::ProgressDisplayShowPulblic( const std::string& strText, const D3DCOLOR& TextColor )
{
	m_pProgressDisplay->ShowPublic( strText, TextColor );
}