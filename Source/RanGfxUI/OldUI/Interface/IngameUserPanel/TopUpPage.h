#pragma	once

#include "../../Util/UIOuterWindow.h"

class GLGaeaClient;
class COuterInterface;
class CUIEditBoxMan;

class CTopUpPage : public CUIOuterWindow
{
	enum
	{
		TOPUP_OK = ET_CONTROL_NEXT,
		TOPUP_CANCEL,
		TOPUP_EDIT_MAN,
		TOPUP_EDIT_CD,
		TOPUP_EDIT_PN,
		TOPUP_EDIT_CP
	};

public:
	static int nLIMIT_CODE;
	static int nLIMIT_PIN;
	static int nLIMIT_CP;

public:
	CTopUpPage(GLGaeaClient* pGaeaClient, COuterInterface* pInterface, EngineDeviceMan* pEngineDevice);
	virtual	~CTopUpPage();

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