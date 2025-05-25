#pragma	once

#include "../../Util/TapSelectorEx.h"
#include "../../Util/UIWindowEx.h"

// "P" 키를 누르면 나오는 파티/원정대창;

class PartyInfoTap;
class ExpeditionInfoTap;

class CInnerInterface;
class GLGaeaClient;
class CInnerInterface;
class PartyWindowRenewal : public CUIWindowEx, private CTapSelectorEx<DEFAULT_TAPBUTTON>
{
	enum
	{
		PARTY_RENEWAL_INFO_TAP = ET_CONTROL_NEXT + 1,
		EXPEDITION_RENEWAL_INFO_TAP,

		PARTY_RENEWAL_INFO_TAP_WINDOW,
		EXPEDITION_RENEWAL_INFO_TAP_WINDOW,

		PARTY_INFO_HELP_BUTTON, // 도움말 버튼;
	};
public:
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl) override;
	virtual void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg) override;
	virtual void SetVisibleSingle(BOOL bVisible) override;

	void SetPartyOption(const SPartyOption& PartyOption);
	void SetPartyInfo(const DWORD dwPartyID, const DWORD dwHashKey, const DWORD dwMasterID, const DWORD dwTimeKey);
	void CreateSubControl(void);

	const DWORD GetPartyID(void) const;
	const DWORD GetHashKey(void) const;
	const DWORD GetMasterID(void) const;
	const DWORD GetTimeKey(void) const;

private:
	virtual void EventSelectedTap(UIGUID controlID) override;

public:
	PartyWindowRenewal(GLGaeaClient* pGaeaClient, CInnerInterface* pInterface, EngineDeviceMan* pEngineDevice);
	virtual ~PartyWindowRenewal(void);

protected:
	GLGaeaClient* m_pGaeaClient;

	CUIGroup* m_pCurrentTapWindow;
	PartyInfoTap* m_pPartyInfoTapWindow;
	ExpeditionInfoTap* m_pExpeditionInfoTapWindow;	

private:
	DWORD	m_dwPartyID;
	DWORD	m_dwHashKey;
	DWORD	m_dwMasterID;
	DWORD	m_dwTimeKey;
};

class MyPartyWindowRenewal : public IPartyWindowRenewal, public PartyWindowRenewal
{
public:
    MyPartyWindowRenewal( GLGaeaClient* pGaeaCleint, CInnerInterface* pInterface, EngineDeviceMan* pEngineDevice );
    virtual ~MyPartyWindowRenewal(void){};

public:
	virtual void SetPartyOption(const SPartyOption& PartyOption) override;
	virtual void CreateUIWindowAndRegisterOwnership(void) override;

	virtual const DWORD GetPartyID(void) const override;
	virtual const DWORD GetHashKey(void) const override;
	virtual const DWORD GetMasterID(void) const override;
	virtual const DWORD GetTimeKey(void) const override;
	virtual void SetPartyInfo(const DWORD dwPartyID, const DWORD dwHashKey, const DWORD dwMasterID, const DWORD dwTimeKey) override;  
};