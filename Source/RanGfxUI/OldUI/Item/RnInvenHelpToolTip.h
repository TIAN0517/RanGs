#pragma once
#include "../../../EngineLib/GUInterface/UIGroup.h"

class CInnerInterface;
class CBasicLineBoxSmart;
class CBasicTextBox;

class RnInvenHelpToolTip : public CUIGroup
{
private:
    enum
    {
        ID_TOOLTIP_BACKGROUND = NO_ID + 1,
        ID_TOOLTIP_TEXT,
    };
public:
    RnInvenHelpToolTip( CInnerInterface* pInterface, EngineDeviceMan* pEngineDevice );
    void                CreateSubControl();
	void AddText ( std::string strText );

protected:
    virtual	void Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
    virtual	void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
	

private:
    CBasicLineBoxSmart* m_pToolTipBackground;
    CBasicTextBox*      m_pToolTipTextBox;
};
