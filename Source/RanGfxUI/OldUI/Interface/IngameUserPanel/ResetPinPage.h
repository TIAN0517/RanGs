#pragma	once

#include "../../Util/UIOuterWindow.h"

class GLGaeaClient;
class COuterInterface;
class CUIEditBoxMan;

class CResetPinPage : public CUIOuterWindow
{
	enum
	{
		RESETPIN_OK = ET_CONTROL_NEXT,
		RESETPIN_CANCEL,
		RESETPIN_EDIT_MAN,
		RESETPIN_EDIT_EM,
		RESETPIN_EDIT_PW,
		RESETPIN_EDIT_CP
	};

public:
	static int nLIMIT_EM;
	static int nLIMIT_PW;
	static int nLIMIT_CP;

public:
	CResetPinPage(GLGaeaClient* pGaeaClient, COuterInterface* pInterface, EngineDeviceMan* pEngineDevice);
	virtual	~CResetPinPage();

public:
	virtual void Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );

	virtual	void SetVisibleSingle( BOOL bVisible );

public:
	void CreateSubControl();
	CBasicButton * CreateFlipButton( char* szButton, char* szButtonFlip, UIGUID ControlID );

	void ResetAll();
	void SetCharToEditBox( TCHAR cKey );
	void DelCharToEditBox();
	void GoNextTab();

private:
	BOOL CheckString( CString strTemp );
	BOOL CheckEmail( CString strTemp );
private:
	GLGaeaClient* m_pGaeaClient;
	COuterInterface*	m_pInterface;
	CUIEditBoxMan * m_pEditBoxMan;
	CBasicButton * m_pIDSaveButton;
	CBasicTextBox * m_pRandTextBox;

	INT m_nRandPassNumber;
	TCHAR m_szRandomString[7];
	INT m_nRPUpdateCnt;
};