#pragma	once

#include "../../../EngineLib/GUInterface/UIGroup.h"
#include "../../../EngineLib/GUInterface/BasicTextBox.h"
//#include "../../../RanLogicClient/InterfaceBaseDefine.h"
#include "../../../EngineLib/GUInterface/InterfaceBaseDefine.h"

#include <vector>

class EngineDeviceMan;
class CInnerInterface;
class GLGaeaClient;
class CBasicLineBoxSmart;
class CBasicTextButton;
class CBasicTextBox;
class CBasicScrollBarEx;
class CItemImage;
class CD3DFontPar;

class CBattePassPage : public CUIGroup
{
protected:
	enum
	{
		BATTLE_PASS_NONE = NO_ID + 1,
		BATTLE_PASS_SCROLLBAR,
	};

protected:
	struct BATTLEITEMSLOT : public CUIGroup
	{
		BATTLEITEMSLOT (GLGaeaClient* pGaeaClient, EngineDeviceMan* pEngineDevice)
			: CUIGroup(pEngineDevice)
			, pGAEACLIENT(pGaeaClient)
			, pItemImage(NULL)
			, pItemBlock(NULL)
			, pTEXTITEMNAME(NULL)
			, pTEXTACQLVL(NULL)
			, pTEXTACTION(NULL)
			, pAcquireButton(NULL)
			, ppBase (NULL)
			, bClaimed(false)
			, bENABLE(false)
		{
		}

		GLGaeaClient*  pGAEACLIENT;
		std::string	   strNAME;
		CItemImage*	   pItemImage;
		CUIControl*	   pItemBlock;
		CBasicTextBox* pTEXTITEMNAME;
		CBasicTextBox* pTEXTACQLVL;
		CBasicTextBox* pTEXTACTION;
		CBasicTextButton* pAcquireButton;
		CBattePassPage* ppBase;
		bool		   bClaimed;
		bool		   bENABLE;

		void CreateSubControl( CBattePassPage* pBase );
		void Reset();

		void SetEnable( const bool bEnable );
		void SetData( const UINT nIndex, const SBATTLEPASSITEM* pBattleItem );

		void SetTextPosition( CBasicTextBox* pTextBox, const UIRECT& rcBase );
		void SetImagePosition( CUIControl* pImage, const UIRECT& rcBase, const UIRECT& rcParent );
		CBasicTextBox* CreateStaticControl(const char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID=NO_ID );

	};

	typedef std::vector<BATTLEITEMSLOT*> BATTLEITEMSLOTVEC;
	typedef BATTLEITEMSLOTVEC::iterator  BATTLEITEMSLOTVEC_ITER;

protected:
	static const UINT nMAX_SLOT = 10;

public:
	CBattePassPage ( GLGaeaClient* pGaeaClient, CInnerInterface* pInterface, EngineDeviceMan* pEngineDevice );
	virtual	~CBattePassPage ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	void CreateSubControl ();
	void Update();

public:
	void ResetScrollBar();

public:
	void Update_BP();

protected:
	CBasicTextBox* CreateStaticControl (const char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID=NO_ID );
	CUIControl*	CreateColorBackgroundControl ( char* szControlKeyword, const D3DCOLOR& cColor, int nAlign=UI_FLAG_DEFAULT );

public:
	const UIRECT& GetBaseRectITEM()			{ return m_pTextBase_ITEM->GetLocalPos(); }
	const UIRECT& GetBaseRectItemName()		{ return m_pTextBase_ITEMNAME->GetLocalPos(); }
	const UIRECT& GetBaseRectAcquire()		{ return m_pTextBase_ACQ_LVL->GetLocalPos(); }
	const UIRECT& GetBaseRectAction()			{ return m_pTextBase_ACTION->GetLocalPos(); }
	const UIRECT& GetBaseRectDescription()		{ return m_pTextBase_DESCRIPTION->GetLocalPos(); }
	const UIRECT& GetBaseRectHeal()			{ return m_pTextBase_Heal->GetLocalPos(); }
	const UIRECT& GetBaseRectKill()			{ return m_pTextBase_Kill->GetLocalPos(); }
	const UIRECT& GetBaseRectDeath()		{ return m_pTextBase_Death->GetLocalPos(); }
	const UIRECT& GetBaseRectRebirth()		{ return m_pTextBase_Rebirth->GetLocalPos(); }
	const UIRECT& GetBaseRectContribution() { return m_pTextBase_Contribution->GetLocalPos(); }

private:
	CBasicTextBox* m_pTextBase_ITEM;
	CBasicTextBox* m_pTextBase_ITEMNAME;
	CBasicTextBox* m_pTextBase_ACQ_LVL;
	CBasicTextBox* m_pTextBase_ACTION;
	CBasicTextBox* m_pTextBase_DESCRIPTION;

	BATTLEITEMSLOTVEC m_vecSlot;

	CBasicScrollBarEx* m_pScroll;

private:
	UINT m_nScrollPos;

protected:
	GLGaeaClient*	 m_pGaeaClient;
	CInnerInterface* m_pInterface;

};
