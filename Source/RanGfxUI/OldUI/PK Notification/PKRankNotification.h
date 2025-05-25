#pragma	once

#include "../Util/UIWindowEx.h"
#include "../RanLogic/Msg/PKRankData.h"

class CInnerInterface;
class GLGaeaClient;

class CPKRankNotificationInfo;

class CPKRankNotification : public CUIWindowEx
{
public:
	enum
	{
	};

public:
	CPKRankNotification ( GLGaeaClient* pGaeaClient, CInnerInterface* pInterface, EngineDeviceMan* pEngineDevice );
	virtual ~CPKRankNotification();


public:
	void CreateSubControl();

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

public:
	CPKRankNotificationInfo*		m_pInfo[PKNOTIF_NUM];
	

protected:
	CInnerInterface* m_pInterface;
	GLGaeaClient*	 m_pGaeaClient;
};