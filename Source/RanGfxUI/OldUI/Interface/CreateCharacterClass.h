#pragma	once

#include "../../../EngineLib/GUInterface/UIGroup.h"
#include "NewCreateCharEnum.h"

class CBasicTextBox;

class CCreateCharacterClass : public CUIGroup
{
    enum
    {
        EXTREME,
        SWORDMAN,
        ARCHER,         // Gunner (槍手) - ClassID: 4
        BRAWLER,
        SHAMAN,
        SCIENTIST,
        ASSASSIN,       // Ninja (忍者) - ClassID: 5
		TRICKER,
        ETC,
    };

    // Job Unlocking System - Jy技術團隊
    enum EMJOB_UNLOCK_STATE
    {
        JOB_LOCKED = 0,
        JOB_UNLOCKED = 1,
    };

    static bool s_bGunnerUnlocked;  // 槍手解鎖狀態
    static bool s_bNinjaUnlocked;   // 忍者解鎖狀態

public:
    CCreateCharacterClass(EngineDeviceMan* pEngineDevice);
    virtual ~CCreateCharacterClass();

    void CreateSubControl ();

    CUIControl*	CreateControl( CString strControl, WORD wAlignFlag = UI_FLAG_DEFAULT, UIGUID WndID = NO_ID );
    CBasicTextBox* CreateStaticControl(const char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR D3DCOLOR, int nAlign );

    void ResetData();
    void SetClassData( INT nSex, INT nClass );

    // Job Unlocking Functions - Jy技術團隊
    static void UnlockGunner(bool bUnlock = true) { s_bGunnerUnlocked = bUnlock; }
    static void UnlockNinja(bool bUnlock = true) { s_bNinjaUnlocked = bUnlock; }
    static bool IsGunnerUnlocked() { return s_bGunnerUnlocked; }
    static bool IsNinjaUnlocked() { return s_bNinjaUnlocked; }
    static void UnlockAllJobs() { s_bGunnerUnlocked = true; s_bNinjaUnlocked = true; }
    bool IsJobAvailable(INT nClass);

public:
    virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
    virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
    virtual void SetVisibleSingle( BOOL bVisible );

private:

    CUIControl* m_pClass[GLCI_NUM_ETC];

    CBasicTextBox* m_pNameTextBox;
    CBasicTextBox* m_pInfoTextBox;
};