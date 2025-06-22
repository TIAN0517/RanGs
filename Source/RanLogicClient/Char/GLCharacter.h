#ifndef _GL_CHARACTER_H_
#define _GL_CHARACTER_H_

#pragma once

#include "../../SigmaCore/String/MinGuid.h"

#include "../../EngineLib/NaviMesh/Actor.h"
#include "../../EngineLib/DxMeshs/SkinMesh/DxSkinChar.h"

#include "../../RanLogic/Character/GLCharDefine.h"
#include "../../RanLogic/Character/GLCharData.h"
#include "../../RanLogic/GLogicData.h"
#include "../../RanLogic/GLogicEx.h"
#include "../../RanLogic/NpcTalk/NpcTalkControl.h"
#include "../../RanLogic/MSG/GLContrlMsg.h"
#include "../../RanLogic/Market/GLPrivateMarket.h"
#include "../../RanLogic/GLChatLinkDef.h"
#include "../../RanLogic/Activity/TitleManager.h"
#include "../../RanLogic/FSM/GLCharStateDefine.h"
#include "../../RanLogic/Skill/GLSkillEffectAction.h"

#include "../InterfaceBaseType.h"
#include "../Trade/GLTradeClient.h"
#include "../Reservation/GLReservationAction.h"
#include "../ClientActor.h"
#include "./GLWorkingSkill.h"

//#include "../Widget/GLWidgetScript.h"

using namespace NSSKILL_EFFSET;

class GLClubClient;
struct SKILLID;
class CClientPKCardManager;

enum EMFIND_TAR
{
	ENFIND_TAR_NULL		= 0,
	EMFIND_TAR_ANY		= 1,
	EMFIND_TAR_ENEMY	= 2,
	EMFIND_TAR_OUR		= 3,
	EMFIND_TAR_OUR_DIE	= 4,
	EMFIND_TAR_ANY_DIE	= 5,
	ENFIND_TAR_LAND		= 6,
};

enum EMFIND_OPTION
{
	EMFIND_OPTION_NULL		= 0x0000,
	EMFIND_OPTION_ONLYITEM	= 0x0001,
	EMFIND_OPTION_P2PMENU	= 0x0002,
};

// Ȯ�� �� ���� �ص� �Ǵ� Data
enum DoActWait_TYPE
{
	DoActWait_Zero,
	DoActWait_Create,
	DoActWait_MoveActiveMap,
	DoActWait_MsgMoveActiveMap,
	DoActWait_MsgResurrect,
	DoActWait_ReLvMustLeaveFB,
	DoActWait_ReCallFB,
	DoActWait_TeleportFB,
};

// ������ ��� �� Ȧ�� ���� ����
enum ESlotHold
{
	Release = 0,
	Hold,
};

struct SACTIONTAR
{
	EMACTIONTAR		emACTAR;
	STARGETID		sTARID;

	SACTIONTAR() :
		emACTAR(EMACTAR_NULL)
	{
	}

	const bool operator==( SACTIONTAR& rhs ) const
	{
		if ( emACTAR != rhs.emACTAR )
		{
			return (sTARID == rhs.sTARID);
		}

		return true;
	}

	void RESET()
	{
		emACTAR = EMACTAR_NULL;
		sTARID.RESET();
	}
};

class GLCharClient;
class GLGaeaClient;
struct GLREACTION;
struct ActorWorldLight;
class GLWorkingSkill;
class GLTransformSet;
class SkinCharMulti;

typedef std::vector<USER_ATTEND_INFO>		VECATTEND;

typedef std::map< DWORD, DWORD >	MAPMARKETCLICK;
typedef MAPMARKETCLICK::iterator	MAPMARKETCLICK_ITER;

typedef std::map< DWORD, SENTRYLIMITINFO > MAPENTRYINFO;
typedef MAPENTRYINFO::iterator			   MAPENTRYINFO_ITER;

typedef std::vector<CString> MAPNAMEVECTOR;
typedef MAPNAMEVECTOR::const_iterator MAPNAMEVECTORCITER;
typedef MAPNAMEVECTOR::iterator MAPNAMEVECTORITER;

typedef std::vector< GLSKILL* >		VEC_GLSKILL;
typedef VEC_GLSKILL::iterator		VEC_GLSKILL_ITER;

class GLCharacter : public GLCHARLOGIC, public ClientActor
{
public:
    GLCharacter(GLGaeaClient* pGaeaClient, GLClubClient* pMyClub);
    virtual ~GLCharacter();

	static TARDISTMAP	ms_vecTAR_TEMP;

protected:
	const static float	m_fELAPS_MOVE;
    const static float  fLIMIT_TIME_ITEM_SORT;
    const static float	fMAX_ITEM_SORT_MSG_DELAY_TIME;
	
	enum { EMWAIT_COUNT = 10 };

protected:
    int FIRST_PRIORITY_ITEMSORT[ ITEM_NSIZE ];
    int SECOND_PRIORITY_ITEMSORT[ EMITEM_LEVEL_NSIZE ];
    int ADDTIONAL_PRIORITY_ITEMSORT[ SUIT_NSIZE ];

	SNATIVEID	m_sChangeQUICK[EMSKILLQUICK_SIZE];

	typedef std::vector<WORD>		vSortQuick;
	typedef vSortQuick::iterator	vSortQuick_iter;

	vSortQuick					   m_vQuickNum;

protected:
	LPDIRECT3DDEVICEQ	m_pd3dDevice;
	DxSkinChar*	m_pSkinChar;
	SkinCharMulti* m_pSkinCharMulti;
	GLTransformSet*	m_pTransformSet; // ȯ���� ��ų�� ���� ����;
	GLWorkingSkill* m_pWorkingSkill; //  �������� ���� ��ų

	CSkillSThrow	m_SkillSTrow;
	CSkillCameraHit m_SkillCHit;
	
	D3DXMATRIX			m_matBikeTrans;

	D3DXVECTOR3			m_vServerPos;

    EMSERVICE_PROVIDER m_ServiceProvider;

public:
	SUMMONIDLIST            m_dwSummonGUIDList; // ��ȯ�� GUID ����Ʈ
	
	// PK Effect Cards Manager
	CClientPKCardManager*	m_pPKCardManager;

	//! �ൿ.
	//
protected:
	EMACTIONTYPE		m_Action;				//	���� �׼�.
	
	STARGETID			m_sActionID;
	STARGETID			m_sSelectID;
	STARGETID			m_sCursorID;
	
	SKILLTARGETDATA		m_sSkillTargetData;

	TARIDRC_SET			m_setTAR_SELECTED;			// Ÿ�� ����Ʈ�� ���� ī����

	SNATIVEID			m_sRunSkill;				// ������ ��ų
	SNATIVEID			m_sActiveSkill;				// �ߵ����� ��ų
	SKILLID				m_sScopeSkill;				// ���� ���� ��ų

	DWORD				m_dwANISUBCOUNT;
	DWORD				m_dwANISUBSELECT;

	DWORD				m_dwANISUBGESTURE;

	EMITEM_QUESTION		m_emOldQuestionType;	//	ȿ�� ����/��������� ������ Ÿ���� �����ϰ� ����.

	float				m_fQUEST_TIMER;				//	����Ʈ ���� ����.

protected:
	float				m_fLastMsgMoveSend;
	GLMSG::SNETPC_GOTO	m_sLastMsgMove;
	GLMSG::SNETPC_GOTO	m_sLastMsgMoveSend;

	GLReservationAction	m_ReservationAction;

	bool				m_bMiniMapMove;			// �̴ϸ��� ���ؼ� �̵� ���̴�
	float				m_fMiniMapMoveTime;

protected:
	float				m_fIdleTime;
	float				m_fattTIMER;
	int					m_nattSTEP;

	DWORD				m_dwWAIT;

protected:
	DWORD				m_dwNumStorageItem[EMSTORAGE_CHANNEL];	//	â���� ����ִ� ������ ����.
	SCONFTING_CLT		m_sCONFTING;							//	���� ��� ����.

	// â���� �ִ�  Ư�� �����۸� �޾� ĳ���صδ°�
	DWORD				m_StorageSpecificItemCacheCount;
	std::vector<DWORD>  m_StorageSpecificItemCacheVec;
public:
	WORD				m_wPMPosX;								//	���λ��� ���� �㰡��.
	WORD				m_wPMPosY;
	GLPrivateMarket		m_sPMarket;								//	���� ����.

	MAPMARKETCLICK		m_mapMarketClick;						//	Ŭ���� ���λ��� ����Ʈ

	BOOL				m_bCLUB_CERTIFY;						//	���� Ŭ�� ������ ���� ����.
	float				m_fCLUB_CERTIFY_TIMER;					//	���� Ŭ�� ������ ���� �ð�.
    bool				m_bForcedPKMode;						//  ���� PK���

protected:
	GLClubClient*		m_pMyClub;								//! ���� Ŭ�� ����.

public:
	WORD				m_wInvenPosX1;				// Temp
	WORD				m_wInvenPosY1;
	WORD				m_wInvenPosX2;				// Temp
	WORD				m_wInvenPosY2;

	// �ӽ� ���� ������ �� ������ �־�� �ϴµ� ������ 3���� ������ �ֵ��� �Ѵ�.
	WORDWORD			m_wwInvenPos_FaceStyle;		// Temp
	WORDWORD			m_wwInvenPos_HairStyle;		// Temp
	WORDWORD			m_wwInvenPos_HairColor;		// Temp

	SITEMCUSTOM			m_sTempItemCustom;

	MAPENTRYINFO		m_mapEntryLimitInfo;

	// Note : ���޿��� �Ӹ��� �ٲ� ��� �ӽ÷� �����͸� ������ ����.
private:
	//BOOL			m_bEnableHairSytle;
	//BOOL			m_bEnableHairColor;
	//BOOL			m_bEnableFaceStyle;
	//WORD			m_wHairColorTEMP;
	//WORD			m_wHairStyleTEMP;
	//WORD			m_wFaceStyleTEMP;

	BOOL			m_bAttackable;							// ���ݰ�������
	BOOL			m_bMiniGameOpen;

	float			m_fPartyRecruitTime;

    float           m_fCheckActivityItem;       //  Ư��Ȱ�� ���� ������ ���� ������Ʈ

private:
	EMSLOT			m_emCostumSelectSlot;		// Note : �ڽ�Ƭ ������ �� ��� ������ ����.

    float           m_fLimitTimeItemSort;
    float           m_fItemSortMsgDelayTime;
    float           m_bSendItemSortMsg;

    CHARACTER_POINT m_PointShopPoint;			//! PointShop Point

    RebuyItem::REBUY_VEC m_vecRebuy;

	EMANI_MAINTYPE		m_eMotionMID;			//! ��� MID
	EMANI_SUBTYPE		m_eMotionSID;			//! ��� SID
	FLOAT				m_fMotionSec;			//! ��� �÷��� �ð� (��)

    CharState           m_bMovement;
	SNATIVEID			m_sGeneralEffItemID;

	// ���� ĳ���Ͱ� ������ ĳ���� ���� ���� ( Lock, Unlock ���� ��� ���� );
	int					m_nOpenCharSlot;

public:
    unsignedState       m_sResurrectAbleState;      // ��Ȱ ���� ��������;

public:
    TitleManager    m_TitleManager;
    LONGLONG        m_llActivityPoint[ACTIVITY_CLASS_SIZE];

protected:
	// Country
	Country::SCOUNTRY_INFO	m_sCountryInfo;

public:
	// Country;
	void SetCountryInfo ( const Country::SCOUNTRY_INFO& sCountryInfo ) { m_sCountryInfo = sCountryInfo; }
	const Country::SCOUNTRY_INFO& GetCountryInfo () const { return m_sCountryInfo; }

private:
	std::tr1::shared_ptr<ActorWorldLight>		m_spWorldLight;

	// RequestLandIn Call this function after receive view around message already. ���� ã�� ���� Data
	DoActWait_TYPE								m_emDoActWaitTypePrev;
	DoActWait_TYPE								m_emDoActWaitType;

private:
    void SetPriorityItemSort();

    void ResetLimitTimeItemSort();
    void SetEnableSendItemSortMsg();
    void SetDisableSendItemSortMsg();

public:
	std::tr1::shared_ptr<ActorWorldLight>	GetWorldLightForTool()	{ return m_spWorldLight; }

	inline SKILLTARGETDATA&	GetTargetData()			{ return m_sSkillTargetData; }
	BOOL					TryToUseSkill();
	ClientActor*			GetCrowOnLand( const EMCROW emCrow, const DWORD dwGaeaId );

	EMSLOT GetCostumSelectSlot() const { return m_emCostumSelectSlot; }
    bool IsEnalbeItemSort();

    //! PointShop Point
    inline void PointShopPointSet(const CHARACTER_POINT& Point) { m_PointShopPoint = Point; }
	inline __int64 PointShopPoint() const { return m_PointShopPoint.Total(); }
	inline __int64 PointShopBattlePoint() const { return m_PointShopPoint.BattlePoint(); }

    //! �����
    const RebuyItem::REBUY_VEC& RebuyList() const;

    unsigned int GetCountItemInInven( EMITEM_TYPE emType );
	unsigned int GetCountItemInInven( EMITEM_TYPE emType, ITEM_VALUE typeValue, int _value );
    void GetItemPosInInven( EMITEM_TYPE emType, WORD& wOutPosX, WORD& wOutPosY );
	void GetItemPosInInven( EMITEM_TYPE emType, ITEM_VALUE typeValue, int _value, WORD& wOutPosX, WORD& wOutPosY );

	//LuaTable GetItemPosInInvenByTable( DWORD dwType );
	//LuaTable FindInvenItemPos( WORD wMID, WORD wSID, int nCountIndex );
	bool IsExistReturnSchoolCard(void);

public:	
	void SetServerPos( const D3DXVECTOR3& vPos ) { m_vServerPos = vPos; }

	const SNATIVEID& GetChangeQuick( int iNum );

public:
	BOOL IsOpenMiniGame() const { return m_bMiniGameOpen; }
	VOID OpenMiniGame()			{ m_bMiniGameOpen = TRUE; }
	VOID CloseMiniGame()		{ m_bMiniGameOpen = FALSE; }

public:
    bool IsOpenNPCMarket ();

public:
	//void HairStyleInitData();
	void HairStyleChange( WORD wStyle );
	//void HairStyleEnd()	{ m_bEnableHairSytle = FALSE; }
	
	//void HairColorInitData();
	void HairColorChange( WORD wColor );
	//void HairColorEnd()	{ m_bEnableHairColor = FALSE; }

	//void FaceStyleInitData();
	void FaceStyleChange( WORD wStyle );
	//void FaceStyleEnd()	{ m_bEnableFaceStyle = FALSE; }

private:
	void HairStyleUpdate( WORD wStyle );
	void HairColorUpdate( WORD wColor );
	void FaceStyleUpdate( WORD wStyle );

public:
	BOOL IsMarketClick( DWORD dwGaeaID );
	void InsertMarketClick( DWORD dwGaeaID );

public:
	//! AABB
	//
	D3DXVECTOR3		m_vMaxOrg;
	D3DXVECTOR3		m_vMinOrg;

public:
	D3DXVECTOR3		m_vMax;
	D3DXVECTOR3		m_vMin;
	float			m_fHeight;

protected:
	bool  m_bAwayGesture;

public:
	DWORD m_NpcLastClick; //! ���� �������� Ŭ���� NPC ID
	float m_fKeyDownCheckTime;
	DWORD m_nAwayGestureLevel;

public:
	// PET
	//LONGLONG  m_llPetCardGenNum;	//! Ȱ��ȭ�� ��ī���� GenNum
	sc::MGUID m_llPetCardGenNum;
	SNATIVEID m_sPetCardNativeID;   //! Ȱ��ȭ�� ��ī���� Nativeid
	BYTE      m_cPetCardGenType;	//! Ȱ��ȭ�� ��ī���� GenType

	PETCARDINFO_MAP   m_mapPETCardInfo;     //! ��ī�� ����(���� ���°� ����ɶ����� ���� �����������)
	PETCARDINFO_MAP   m_mapPETCardInfoTemp; //! ��ī�� ����(�ŷ��� ���)
	PETREVIVEINFO_MAP m_mapPETReviveInfo;   //! ��Ȱ��Ű�� ���� ���� ����

	BOOL m_bReceivedPetCardInfo; // ��ī�� ���� ���ſ���
	BOOL m_bIsPetActive;		 // ���� �̵��Ҷ� ���� Ȱ�����̾����� ����

public:
	VEHICLEITEMINFO_MAP			m_mapVEHICLEItemInfo;					// Ż�� �������� ����( Ż�� ���� ���� )
	VEHICLEITEMINFO_MAP			m_mapVEHICLEItemInfoTemp;				// Ż�� �������� ����( �ŷ��� ��� )

protected:
	GLVEHICLE		m_sVehicle;				//! Ż�� ����ü
	DWORD			m_dwVehiclePassengerID;	// Vehicle ���� �����ġ�� �ɾ� �ִ°�~?
											// �� �ٸ� Vehicle �� Ż ���� �����Ƿ� GLVEHICLE �ȿ� ���� �ȵȴ�.

public:
	BOOL			m_bReqSwapArms;			// �ذ��� ���� ��ü ���̶��

	BOOL			m_bIsVehicleActive;		// �� �̵��� Ż�� Ÿ�� �־��ٸ�...
	BOOL			m_bRecivedVehicleItemInfo;  // Ż�� ���� ���� ����

	bool			m_bOldVisibleTracingUI[8];
	bool			m_bNewVisibleTracingUI[8];

	float			m_fPrintProcessInfoMsgGap;

	bool			m_bGarbageOpen;
	SINVEN_POS		m_sGarbageItem;			// �����뿡 ��ϵ� ������

	bool			m_bItemShopOpen;		// ItemShopOpen( Japan ) 
	
	bool			m_bReqAttendList;		// �⼮ ����Ʈ
	VECATTEND		m_vecAttend;			// �⼮ ����Ʈ
	DWORD			m_dwComboAttend;		// ���� �⼮ �ϼ�
	__time64_t		m_tAttendLogin;			// �α��νð�( �⼮�ο��� ��� ) 
	DWORD			m_dwAttendTime;			// �⼮ ������ ���� ���� �ð�

	CString			m_strRecordChat;
    bool			m_bRecordChat;
    //bool            m_bUseRemodelCard;
    bool            m_bUseStorageCard;
	CTime			m_recordStartTime;

	SINVEN_POS		m_sProductMaterialPos[ITEMMIX_ITEMNUM];

	bool			m_bCostumeStats;

private:
	EMTYPE_ITEM_REMODEL	m_emTypeItemRemodelState;

	float			m_fMacro_RecordUINotifyTime;// UI�� �˸��ð� ���, �и��� �����Ѵ�;

// ��� ��û�� ���̴� ������
private:
	DWORD			m_PtoPTargetID;
	DWORD			m_ConflictReqID; // = pNetMsg->dwID;
	EMCONFT_TYPE	m_emConflictReqType;	// = pNetMsg->emTYPE;
	SCONFT_OPTION	m_sReqConflictOption;	// = pNetMsg->sOption;

public:
    //! -----------------------------------------------------------------------
    //! Ż��

	// Vehicle. ���� ��� ��ġ�� �ɾ� �ִ°�~?
	void SetVehiclePassengerID( DWORD dwID );
	DWORD GetVehiclePassengerID() const;

    VEHICLE_TYPE VehicleType() const { return m_sVehicle.Type(); }

    //! ��ȯ�� ��û�� Ż�� �������� Mid/Sid
    SNATIVEID VehicleId() const { return m_sVehicle.Id(); }

    void VehicleActiveValueSet(bool bSet) { m_sVehicle.SetActiveValue(bSet); }
	bool VehicleActiveValue() const { return TRUE == m_sVehicle.IsActiveValue() ? true : false; }
    void VehicleReset() { m_sVehicle.RESET(); }

    inline BOOL VehicleUseBoost() const { return m_sVehicle.UseBoost(); }    
    
    inline BOOL VehicleOnceBoost() const { return m_sVehicle.OnceBoost(); }    
    inline BOOL* VehicleOnceBoostRef() { return m_sVehicle.OnceBoostRef(); }
    inline void VehicleOnceBoostSet(BOOL bSet) { m_sVehicle.OnceBoostSet(bSet); }
    
    DxVehicle* VehicleSkinChar() { return m_sVehicle.SkinChar(); }

    DWORD VehiclePassenger(size_t Index) const { return m_sVehicle.PassengerId(Index); }
    void  VehiclePassengerSet(size_t Index, DWORD GaeaId);

    void VehiclePutOnItemSet(VEHICLE_ACCESSORYTYPE Type, SNATIVEID ItemId) 
    { 
        m_sVehicle.PutOnItemSet(Type, ItemId);
    }

    SNATIVEID VehiclePutOnItem(VEHICLE_ACCESSORYTYPE Type) const
    {
        return m_sVehicle.PutOnItemId(Type);
    }

    const SITEMCUSTOM& VehiclePutOnItemRef(VEHICLE_ACCESSORYTYPE Type) const
    {
        return m_sVehicle.PutOnItemRef(Type);
    }

    BOOL VehicleCheckSlotItem(SNATIVEID sNativeID, EMSUIT emSUIT)
    {
        return m_sVehicle.CheckSlotItem(sNativeID, emSUIT);
    }
    
    int VehicleFull() const { return m_sVehicle.Full(); }

    float VehiclePutOnItemMoveSpeedTotal() const { return m_sVehicle.PutOnItemMoveSpeedTotal(); }

    DWORD VehicleBoosterId() const { return m_sVehicle.BoosterId(); }

    float VehicleBoostTime() const { return m_sVehicle.BoostTime(); }

    BOOL VehicleBoostUseDelay() const { return m_sVehicle.BoostUseDelay(); }
    float VehicleBoostDelay() const { return m_sVehicle.BoostDelay(); }

    DWORD VehicleColorMain(VEHICLE_ACCESSORYTYPE Type) const { return m_sVehicle.ColorMain(Type); }
    DWORD VehicleColorSub(VEHICLE_ACCESSORYTYPE Type) const { return m_sVehicle.ColorSub(Type); }

    int VehicleePassengerNum() const { return m_sVehicle.GetPassengerNum(); }    

	//LuaTable GetPutOnVehicleInfo();
	float GetVehicleBoosterGauge();

	void UpdateSpecialSkill( float fElapsedTime );
	void UpdateLunchBox   ( float fElapsedTime );
	void StartDealyAction( const SCHARSKILL& sSkill, GLCharacter* pOwnerChar, const SKILLTARGETDATA* pTargetData, bool bSubSkill );
	void UpdateSkillEff(float fTime, float fElapsedTime );

public:
	BOOL 			ISCONFRONT_TAR( const STARGETID &sTargetID );
    BOOL 			ISCONFRONT_ALLIANCE(const DWORD dwGaeaID);
	void 			DELCONFRONT_MEMBER( DWORD dwGaeaID );

public:
	bool 			IsSchoolConfting();
	UINT 			GetConftMemberCount();

public:
	const bool		IsEmptyReservationAction() const { return m_ReservationAction.IsEmpty(); }	
	const bool		IsExistReservationAction(EMREACTION emFind);
	void			ClearReservationAction(void);
	void 			ResetReservationAction(bool bForced=false);
	void 			ResetSelectID();
	void 			ResetCursorID() { m_sCursorID.RESET(); }
	void			ResetActionID() { m_sActionID.RESET(); }
	void 			SetSelectID( const STARGETID& sTarget, const bool bResetSelected=true );
	void 			SetCursorID( const STARGETID& sTarget ) { m_sCursorID = sTarget; }
	STARGETID& 		GetSelectID() { return m_sSelectID; }
	STARGETID& 		GetActionID() { return m_sActionID; }
	STARGETID& 		GetCursorID() { return m_sCursorID; }	

	BOOL			SelectForcedTarget();

/**********************��ý�û UI�� �Լ�********************/
public:
	void				SetAnsData(GLMSG::SNETPC_REQ_CONFRONT_TAR *_pNetMsg)
	{
		m_ConflictReqID = _pNetMsg->dwID;
		m_PtoPTargetID = _pNetMsg->dwID;
		m_emConflictReqType = _pNetMsg->emTYPE;
		m_sReqConflictOption = _pNetMsg->sOption;
	}
	void				SetReqConftID_ans(DWORD _ID)
	{
		m_ConflictReqID = _ID;
	}
	void				SetConftType_ans(EMCONFT_TYPE _Type)
	{
		m_emConflictReqType = _Type;
	}
	void				SetConftOption_ans(SCONFT_OPTION _Option)
	{
		m_sReqConflictOption = _Option;
	}

	DWORD			GetTarPtoP() { return m_PtoPTargetID; }
	std::string			GetConftMessage();
	//LuaTable	GetHPRevoverNum();
	//LuaTable	GetHPRate_Mine();
	//LuaTable	GetHPRate_Target();	
//	void				RequestConflict(LuaTable _lua_table); //������ ���̾�ID�� �Է�
	void				AnswerConflict(bool _Ans);

	bool				HaveClubJoinAutority();
	bool				HaveClubAllianceAuthority();
	bool				HaveClubBattleAuthority();
	bool				HaveClubStorageAuthority();
	bool				IsAllianceMaster();
	bool				IsRideAVehicleNow();

	float GetCriticalRate() { return GETCRITICAL_TOTAL(); }
	float GetCrushingBlow() { return m_sSUMITEM.fIncR_CrushingBlow; }
	float GetMoveSpeed() { return GETMOVE_TOTAL(); }
	float GetAtkSpeed() { return GETATT_TOTAL(); }
	float GetExpRate() { return GETEXPVELO(); }
	float GetIncrHpTotal() { return m_fVariationRateHP; }
	float GetIncrMpTotal() { return m_fVariationRateMP; }
	float GetIncrSpTotal() { return m_fVariationRateSP; }
	int	GetReceiveCP() { return m_sSUMITEM.nReceive_CP_Inc; }
	int	GetPotionHpInc() { return m_sSUMITEM.nPotion_HP_Inc + m_nPotion_HP_Inc_SKILL; }
	int	GetPotionMpInc() { return m_sSUMITEM.nPotion_MP_Inc + m_nPotion_MP_Inc_SKILL; }
	int	GetPotionSpInc(){ return m_sSUMITEM.nPotion_SP_Inc + m_nPotion_SP_Inc_SKILL; }

	float	GetIncR_Ignore_BossDamage()			{ return m_sSUMITEM.fIncR_Ignore_BossDamage; }
	INT		GetInc_BossFixedDamage()			{ return m_sSUMITEM.nInc_BossFixedDamage; }
	float	GetIncR_Critical_EmergencyMob()		{ return m_sSUMITEM.fIncR_Critical_EmergencyMob; }
	float	GetIncR_Critical_EmergencyBoss()	{ return m_sSUMITEM.fIncR_Critical_EmergencyBoss; }
	float	GetDec_Damage_EmergencyState()		{ return m_sSUMITEM.fDecR_Damage_EmergencyState; }
	float	GetIncR_ExpAddEffect()			{ return m_sSUMITEM.fIncR_ExpAddEffect; }
	float	GetIncR_DropGenMoney()			{ return m_sSUMITEM.fIncR_DropGenMoney; }
	float	GetDecR_Damage_MeleeSkill()		{ return m_sSUMITEM.fDecR_Damage_MeleeSkill; }
	float	GetDecR_Damage_RangeSkill()		{ return m_sSUMITEM.fDecR_Damage_RangeSkill; }
	float	GetDecR_Damage_MagicSkill()		{ return m_sSUMITEM.fDecR_Damage_MagicSkill; }
	INT		GetInc_HP_SoloPlay()			{ return m_sSUMITEM.nInc_HP_SoloPlay; }
	INT		GetInc_HP_PartyPlay()			{ return m_sSUMITEM.nInc_HP_PartyPlay; }
	float	GetIncR_AtkSpeed_SoloPlay ()	{ return m_sSUMITEM.fIncR_AtkSpeed_SoloPlay ; }
	float	GetIncR_AtkSpeed_PartyPlay()	{ return m_sSUMITEM.fIncR_AtkSpeed_PartyPlay; }
	float	GetInc_SkillDurationTAdd(WORD wGrade)	{ return GLCHARLOGIC::GetInc_SkillDurationTAdd(wGrade); }
	float	GetIncR_SkillDamageAdd(WORD wGrade)	{ return GLCHARLOGIC::GetIncR_SkillDamageAdd(wGrade); }
	float	GetDecR_SkillDelayT(WORD wGrade)	{ return GLCHARLOGIC::GetDecR_SkillDelayT(wGrade); }
	float	GetIncR_CriticalDamage() { return GLCHARLOGIC::GetIncR_CriticalDamage(); }

	// ĳ���� ���� ������ ���� �ɼ� ������ ������;
	//LuaTable GetCharInfoDetailAddonInfo();
	//LuaTable GetCharInfoDetailAddonLimitInfo();
	//LuaTable GetCharTempPuonInfoDetailAddonInfo(WORD wPosX, WORD wPosY, WORD wVirtualPow, WORD wVirtualDex,
												//WORD wVirtualSpi, WORD wVirtualStr, WORD wVirtualSta);

public:
	bool 			IsReqSwapArms() 			{ return m_bReqSwapArms ? true : false; }
	bool 			IsMoveAble();
	
    // EMIMPACT_SIDE�� ���� �ൿ�� �Ҽ� �ִ��� �˻�;
    const BOOL      IsReActionable( const STARGETID& _sTargetID, const EMIMPACT_SIDE emTargetSide);
    const BOOL      IsReActionable( const STARGETID& _sTargetID, const EMIMPACT_SIDE emTargetSide, const bool _bForceAction );
	const BOOL		IsReActionableOver( const STARGETID& _sTargetID, const EMIMPACT_SIDE emTargetSide, const bool _bForceAction );

	BOOL			IsSelectTargetLock();

private:
	BOOL			IsReActionableEnemyCrowPC( const ClientActor* pTARGET, const bool bEnableForcedPkMode = true );
	BOOL			IsReActionableEnemy( const ClientActor* pTARGET );
	BOOL			IsReActionableNoEnemyCrowPC( const ClientActor* pTARGET );
	BOOL			IsReActionableNoEnemy( const ClientActor* pTARGET );
    const BOOL      IsReActionableNonFaction( const STARGETID &sTargetID, const EMIMPACT_SIDE emTargetSide, const bool _bForceAction );    

public:
	SDOMINATE&	GetDominate()	{ return m_sDOMINATE; }

public:
	bool IsProducting();
	BOOL IsCtrlBlockBody();
	BOOL IsDie();

	const bool IsIdleCharacter();

    /**
        ������(��ų����) ���� ������ ���� �Ѵ�.        

        �̰��� ����������� �۵� �ǹǷ� Ŭ�󿡼��� ���� ������ �� ������ ���ȵǾ���.
        ���� �������� ���� �ʹٸ� ������ �������� ������ ��û�Ͽ� ������ ���� ó�� �Ǿ�� �Ѵ�.
        Ŭ�� ��ü������ ���� �ʹٸ�(�� �� ������ ���� �ʾƾ� �Ѵ� ������ ����ȭ�� �� �����Ƿ�)
        SetLockMovement �� ��� �ϵ��� �Ѵ�.

     */
    void SetMovement(const CharState state);
    const CharState IsMovement() const { return m_bMovement; } 

	BOOL IsRunMode() { return IsActState(EM_ACT_RUN); };

	BOOL IsActionLimit( DWORD dwLimit ) { return m_dwActionLimit&dwLimit; } 

	virtual const GLPartyID& GetPartyID(void) const;
	virtual BOOL IsPartyMaster();
	virtual const BOOL IsFeignDie() const;

	virtual BOOL IsPartyMem();

	virtual bool IsClubMaster();
	virtual bool IsHaveAllianceDisAuthority();
	virtual BOOL IsCDCertify();

	std::string GetClubName();

    bool IsAwayGesturing()  { return m_bAwayGesture; }
    void SetAwayGesture( bool _bAwayGesture ) { m_bAwayGesture = _bAwayGesture; }

public:
	void		BeginAction( EMACTIONTYPE emAction );
	void		EndAction( EMACTIONTYPE emAction );
	void		TurnAction( EMACTIONTYPE toAction );
	void		ResetAction();

public:
	virtual float GetMoveVelo();
	virtual void SetLockMovement(const bool bLock) override;
	BOOL ActionMoveTo( float fTime, D3DXVECTOR3 vFromPt, D3DXVECTOR3 vTargetPt, BOOL bContinue=FALSE, BOOL bREACT=FALSE, BOOL bMOBCOLL=TRUE );
	BOOL LargeMapMoveTo( D3DXVECTOR3* pPos, BOOL bContinue );
	BOOL	DominationMoveTo(float fTime, D3DXVECTOR3 vFromPt, D3DXVECTOR3 vTargetPt, BOOL bContinue=FALSE);

public:
	DxSkinChar* GetSkinChar()	{ return m_pSkinChar; }

public:
    virtual void FactEffectProcessing ( const EMFACTEFFPROCESSING emFactEffactCase );

	CNpcTalkControl::NPCMAP* FindDefaultTalk( const std::string& strTalkFile );

public:
	virtual const D3DXVECTOR3 &GetMaxOrg()     { return m_vMaxOrg; }
	virtual const D3DXVECTOR3 &GetMinOrg()     { return m_vMinOrg; }

	float GetDirection();

public:
	SCHARDATA2& GetCharData()			{ return(*this); }
	GLCHARLOGIC& GetCharLogic()		{ return(*this); }
	SNATIVEID GetskillRunSlot()		{ return m_sRunSkill; }
	
	void SetActiveSkillRun( SNATIVEID sSkillID ) { m_sActiveSkill = sSkillID; }
	SNATIVEID GetActiveSkillRun()				 { return m_sActiveSkill; }

	void SetScopeSkillRun( const SKILLID& sSkillID ) { m_sScopeSkill = sSkillID; }
	const SKILLID& GetScopeSkillRun()				 { return m_sScopeSkill; }
	
    //! ���
    void EndConfting();
    bool IsConfting() const { return m_sCONFTING.IsCONFRONTING(); }
    EMCONFT_TYPE GetConftingType() const { return m_sCONFTING.emTYPE; }
    void ResetConfting() { m_sCONFTING.RESET(); }

	const WORD FindQuickSkillSlot( const SKILLID& sSkillID );

	float GetBuyRate();
	float GetSaleRate();

	void SetMotion( EMANI_MAINTYPE eMID, EMANI_SUBTYPE eSID, FLOAT fSec );

	void QuickChangeSkill();
	void QuickChangeReset();	

public:
	BOOL GET_PRETRADE_ITEM( SITEMCUSTOM& sItemCustom );
	BOOL IsVALID_STORAGE( DWORD dwChannel );

public:
	BOOL IsPostBoxWindowOpen();
    
    //! ���λ����˻� �����찡 �����ִ°�?
    BOOL IsPrivateMarketSerchWindowOpen();

	BOOL IsPrivateMarketWindowOpen();

	void DoPrivateMarketClose();

public:
	DWORD DetectGate();
	void DetectGateToMapName(MAPNAMEVECTOR& mapNameVector);

	HRESULT UpdateSuit( BOOL bChangeHair = TRUE, BOOL bChangeFace = TRUE );
	HRESULT UpdateSuit_Weapon();

	void ResetPiece( int i );

	HRESULT SetPiece( 
		const TCHAR* szFile,
		LPDIRECT3DDEVICEQ pd3dDevice,
		DWORD dwFlag, 
		int nLevel, 
		EMSCD_FLAGS emFlags,
		BOOL bAttachSub=FALSE,
		EMPIECECHAR* pOutPiecetype=NULL,
		const char* pSelfBodyEffect=NULL );

	HRESULT SetPiece(
		EMPIECECHAR emPieceChar,
		const std::string& strFileOrigin,
		const std::string& strFileChange,
		LPDIRECT3DDEVICEQ pd3dDevice,
		DWORD dwFlag, 
		int nLevel, 
		EMSCD_FLAGS emFlags,
		BOOL bAttachSub=FALSE,
		EMPIECECHAR* pOutPiecetype=NULL,
		const char* pSelfBodyEffect=NULL );

	void SELECTANI( DxSkinChar* pRenderChar, const TCHAR* szAnim, DWORD dwFlags=NULL);
	void SELECTANI( DxSkinChar* pRenderChar, EMANI_MAINTYPE MType, EMANI_SUBTYPE SType=AN_SUB_NONE, DWORD dwFlags=NULL, DWORD dwSelect=0);
	void DEFAULTANI( DxSkinChar* pRenderChar );
	void TOSTARTTIME( DxSkinChar* pRenderChar );
	void TOENDTIME( DxSkinChar* pRenderChar );
	void SetPartRend( DxSkinChar* pRenderChar, EMPIECECHAR emPIECE, BOOL bRender );
	void SetHairColor( WORD wColor );
	BOOL PutPassiveEffect ( DxSkinChar *pSkinChar, const char* szEffFile, const D3DXVECTOR3 *pDir, float fTarDir=0.0f );
	void OutEffect ( DxSkinChar *pSkinChar, std::string strEffFile );
	//void CalculateAmbientColor( LPDIRECT3DDEVICEQ pd3dDevice, DxLandMan* pLandMan, const D3DXVECTOR3& vPos );	// Ambient Color�� ����Ѵ�.
	void SELECTSKILLANI (  DxSkinChar *pSkinChar, EMANI_MAINTYPE MType, EMANI_SUBTYPE SType=AN_SUB_NONE );
	// None const
	BOOL IsANI_SELECTANI ( DxSkinChar *pSkinChar, const TCHAR* szAnim, DWORD dwFlags=NULL );
	BOOL IsANI_SELECTANI( DxSkinChar *pSkinChar, EMANI_MAINTYPE MType, EMANI_SUBTYPE SType=AN_SUB_NONE, DWORD dwFlags=NULL, DWORD dwSelect=0 );
	void SetPositionSkin(  DxSkinChar *pSkinChar, const D3DXVECTOR3& vPos );
	HRESULT FrameMoveSkin( DxSkinChar *pSkinChar, 
		const float fTime, 
		const float fElapsedTime,
		BOOL bNoneOverFrame,
		const D3DXMATRIX &matRot, 
		const BOOL bContinue=TRUE, 
		BOOL const bFreeze=FALSE, 
		const BOOL bAttackMode=TRUE, 
		const BOOL bBoardRender = FALSE );


private:
	void UpdateCharPos_RenderChar( LPDIRECT3DDEVICEQ pd3dDevice, BOOL bHALF_VISIBLE );
	void UpdateCharPos_RenderChar_SoftAlpha( LPDIRECT3DDEVICEQ pd3dDevice, BOOL bHALF_VISIBLE );
	void UpdateCharPos_RenderChar_MaterialEffect( LPDIRECT3DDEVICEQ pd3dDevice, BOOL bHALF_VISIBLE );
	void RenderMyChar( LPDIRECT3DDEVICEQ pd3dDevice, DxSkinCharBase* pRenderChar, BOOL bOpaqueState, BOOL bHALF_VISIBLE );
	void RenderMyCharMaterialEffect( LPDIRECT3DDEVICEQ pd3dDevice, DxSkinCharBase* pRenderChar, BOOL bHALF_VISIBLE );
	void CalculateMatrix( const D3DXVECTOR3& vPosition );
	void UpdateSkillRandomBuild(float fTime, float fElapsedTime );
	void SetActiveAttack( float fTime );
	BOOL BuildRandomQuick();
	
public:

	virtual void Play_ScreenThrow( DWORD dwIndex, const STARGETID& sID );
	virtual void Stop_ScreenThrow( DWORD dwIndex );

	virtual void Play_CameraHit( WORD wEffNum );
	virtual void Stop_CameraHit( WORD wEffNum );

	virtual void End_FeignDeath( BOOL bForce );

	virtual void Play_Disguise( int iIdx, BOOL bOn );
	virtual void Stop_Disguise(  );

	virtual void SetGaeaID(const DWORD dwGaeaID) override
	{
		m_dwGlobalID = dwGaeaID;
		m_dwGaeaID = dwGaeaID;
	}
	virtual const DWORD GetCharID() const	override    { return m_CharDbNum; }
	virtual const char* GetName() const	{ return m_szName; }
	virtual WORD GetLevel() const		{ return m_wLevel; }
	virtual DWORD GetClubID() const;
	virtual bool CheckIsValidClub( int dbClubID );
	virtual bool IsValidClub() const;
	virtual GLDWDATA GetHp() const		{ return m_sHP; }
	virtual const EMCHARINDEX GetCharIndex() const { return m_CHARINDEX; }
	virtual const WORD GetCharSchool() const	{ return m_wSchool; }
	virtual const WORD GetChaRebornNum() const	{ return m_wRebornNum; }

	virtual WORD GetBodyRadius();
	virtual float GetBodyHeight()			{ return m_fHeight; }
	virtual D3DXVECTOR3 GetPosBodyHeight();

	virtual bool IsSafeZone() const { return m_bSafeZone; }
	virtual bool IsCDMSafeTime() const		{ return m_fCDMSafeTime > 0.0f; }

	virtual const BOOL IsNonTargetable(void) const override;// { return m_sHALLUCINATE.bNonTargetable; }

	void RestoreActStateByInvisible(); // ���� ��ȯ �� ��ų ���� ������ �Բ� ���� �ؾ� ��;

    /**
        �� ���� �� ó���� ���� �Ѵ�.
        ���� �� ������ ������ ��û �� �ʿ䰡 ���� �� ����� �� �ִ�.
        ������� �� �̵� �� 1�� PVE �δ� ������ ��û�ϰ� �޾� ������ �����ϰ� �ִ�.

     */
     void PostEnterMap();

protected:
	virtual void UPDATE_DATA_FINISH(const DWORD dwSkillFactIndex){ UPDATE_DATA_END(dwSkillFactIndex); }
	virtual void UPDATE_DATA_END(const DWORD dwSkillfactIndex, BOOL bTimeDel = FALSE, BOOL bForceEnd = FALSE, BOOL bOverLabEnd = FALSE);	
	virtual void UPDATE_DATA_Additional(const float fElapsedTime);

	void AddSkillFact_SpecialAddon_Client(const EMSPEC_ADDON& emAddon, const SKILL::SSPEC& sSPEC, SSKILLFACT& sSKILLEF);

    void RECEIVE_SKILLFACT_Taunt( const STARGETID& sTargetID, const float fDistance );
	void RECEIVE_SKILLFACT_Invisible(const float fVAR2);
	void RECEIVE_SKILLFACT_Dominate(const SKILL::SSPEC& sSPEC);
	void RECEIVE_SKILLFACT_RandomExpRate(const SKILL::SSPEC& sSPEC, const SNATIVEID& sSkillID, const float fLife, const WORD wLevel, SSKILLFACT& sSKILLEF, const float* const pLaunchTime, const float* const pLaunchValue);
	const BOOL RECEIVE_SKILLFACT_AIRBORNE(const SKILL::SSPEC& sSPEC, const SKILL::CDATA_LVL& sSKILL_DATA);
	const bool RECEIVE_SKILLFACT_Special(PGLSKILL pSkill, const WORD wlevel, const DWORD dwCount, const STARGETID& sID, SSKILLFACT& sSKILLEF, const SNATIVEID& skill_id, const float* const pLaunchTime = 0, const float* const pLaunchValue = 0);
	const bool RECEIVE_SKILLFACT_Addon( PGLSKILL pSkill, const WORD wlevel, SSKILLFACT& sSKILLEF );
	const bool RECEIVE_SKILLFACT_Basic( PGLSKILL pSkill, const WORD wlevel, SSKILLFACT& sSKILLEF);
	const bool RECEIVE_SKILLFACT_EntranceExit( const SNATIVEID& sSkillID, const SKILL::SSPEC& sSPEC, WORD wLevel, const STARGETID sID );
	const bool RECEIVE_SKILLFACT_Distinguish( const SNATIVEID& sSkillID, const SKILL::SSPEC& sSPEC, WORD wLevel, const STARGETID sID );
	const bool RECEIVE_SKILLFACT_Provoke( const SNATIVEID& sSkillID, const SKILL::SSPEC& sSPEC, WORD wLevel, const STARGETID sID );
	
	const BOOL RECEIVE_SKILLFACT( const GLMSG::SNETPC_SKILLHOLD_BRD* pMsg );
	const BOOL RECEIVE_SKILLFACT( 
		const SNATIVEID& skill_id, 
		const WORD wlevel, 
		const DWORD dwSELECT, 
        const STARGETID& sID,
		const SSKILLFACT::STIME sTime=SSKILLFACT::STIME(_SKILLFACT_DEFAULTTIME), 
		const DWORD dwCount=_SKILLFACT_DEFAULTCUSTOMCOUNT,
		const float* const pLaunchTime = 0, const float* const pLaunchValue = 0);

	const BOOL RECEIVE_SKILLCA(
		const GLMSG::SNETPC_SKILLHOLD_BRD* pMsg );

	virtual void ReceiveDamage(const int nDamage, DWORD dwDamageFlag, WORD wSchool, const STARGETID& sACTOR) override;
	virtual void ReceiveHeal(const int nHeal, DWORD dwHealFlag, WORD wSchool, const STARGETID& sACTOR) override;
	virtual void ReceiveBlocking(const STARGETID& sACTOR) override;
	virtual void ReceiveAVoid( const STARGETID& sACTOR ) override;
	virtual void ReceiveSwing() override;
	void ReceiveShock();

	void PrintTracingUserWindowInfo();
	void PrintTracingUserProcessInfo( float fElapsedTime );

public:	
    bool IsInsertToInven(std::tr1::shared_ptr<CItemClientDrop> spItemDrop);
	bool IsInsertToInven(const SITEM* pItem);
    //! �κ��丮 ��������
    DWORD InvenAmountEmpty(bool bAllLine);
	DWORD GetAmountActionQ(WORD wSLOT);

public:
	bool IsInvenSplitItem( WORD wPosX, WORD wPosY, bool bVietnamInven = FALSE );
	bool IsInvenDecomposeItem( WORD wPosX, WORD wPosY );
	bool IsStorageSplitItem( DWORD dwChannel, WORD wPosX, WORD wPosY );
	bool IsNpcPileItem( SNATIVEID sNID_NPC, DWORD dwChannel, WORD wPosX, WORD wPosY );
	bool IsRestartPossible(); // �� �ʿ��� ������� �Ǵ���
	BOOL IsVaildTradeInvenSpace();

public:
	void UpdateMacroState( float fElapsedTime );
	void FillMacroTime(float fTime);
	bool ReqMacroModeOnoff(bool bOn);
	bool PlayerGotoLocation(D3DXVECTOR3& vPosition);

	void ReqToggleRun();
	void ReqTogglePeaceMode();
	void ReqGateOut(const DWORD dwIndex = 0);

	void ReqVisibleNone();
	void ReqVisibleOff();
	void ReqVisibleOn();

	bool ReqGESTURE( int nMOTION, bool bCOMMAND=false );

	void SET_RECORD_CHAT();
	void UPDATE_RECORD_CHAT( bool bCloseClient = FALSE );


	bool GetLinkData( const SLINK_DATA_BASIC& sLinkDataBasic, CString& strLinkName, DWORD& dwColor ); 
	bool GetLinkDataName( const SLINK_TYPE& sLinkType, DWORD dwData, CString& strLinkName, DWORD& dwColor );

public:
	EMCONFT_TYPE GetConftType( DWORD dwID );

public:
	void InitAllSkillFactByMoveMap();

public:
	LONGLONG GetCalcTaxiCharge( int nSelectMap, int nSelectStop );

public:
	bool IsPOSTBOXUSEABLE( const EMPOSTBOXID_TYPE emIDType, const SNATIVEID& sPostBoxID );

public:
	bool IsCoolTime(const SNATIVEID& sNativeID);
	bool CheckCoolTime(const SNATIVEID& sNativeID);
	bool SetCoolTime( ITEM_COOLTIME& sCoolTime, EMCOOL_TYPE emCoolType );
	__time64_t GetMaxCoolTime(const SNATIVEID& sNativeID);

	const ITEM_COOLTIME& GetCoolTime(DWORD dwCoolID, EMCOOL_TYPE emCoolType) const;
	const ITEM_COOLTIME& GetCoolTime(const SNATIVEID& sNativeID) const;

public:
	inline const int GetOpenCharSlot ( void ) const { return m_nOpenCharSlot; }
	inline const EMTYPE_ITEM_REMODEL GetTypeItemRemodel ( void ) const { return m_emTypeItemRemodelState; }

public:
	virtual BOOL CHECK_ACTIVITY_POINT( const SITEM& sItem );
	virtual BOOL CHECK_CONTRIBUTION_POINT( const SITEM& sItem );
	
    // �ν��Ͻ� ���� ��ũ���� NPC Ŭ�� �̺�Ʈ�� �޽��� ����;
    HRESULT ReqClickTrigger( const STARGETID& _target );

public:
	//! ä�ó��� ����
	HRESULT ReqSendChatMsg( DWORD dwChatType, const char* szName, const char* szMsg, const VEC_LINK_DATA_BASIC& vecLinkDataBasic );	

	//! ��Ƽ���� ��ũ ���� ����
	HRESULT ReqPartyLinkJoin( DWORD dwMasterID, int nLinkIndex = -1);

	//! ��Ƽ���� ��ũ ���� �� ����
	HRESULT ReqPartyLinkLure( bool bLure, const DWORD dwHashKey, const DWORD dwTimeKey, const DWORD dwGaeaID );

    //! �κ� ������ ���� ��û
    HRESULT ReqInvenToSlot(WORD wPosX, WORD wPosY);

    //! �κ� ������ ���� �������� ���� ��û
    HRESULT ReqInvenToSpecificSlot( EMSLOT emSlot, WORD wPosX, WORD wPosY );

    //! ���� ��� ��û.( �κ� ����������. );
    HRESULT ReqInvenCure(WORD wPosX, WORD wPosY);

	//! ��ų ���� ��û.( �κ� ����������. )
	HRESULT ReqInvenSkill(WORD wPosX, WORD wPosY);

	//! ���� ��û;
	HRESULT ReqInvenItemDecompose(WORD wPosX, WORD wPosY);

	//! ��ų ���� ��û.( â�� ����������. )
	HRESULT ReqStorageSkill( SNATIVEID sNPCID, DWORD dwNPCGlobalID, DWORD dwChannel, WORD wPosX, WORD wPosY );

	//! ��ȯ ��û.
	HRESULT ReqReCall(WORD wPosX, WORD wPosY);

	//! ��ȯ�� ��� ��û.
	HRESULT ReqTeleport(WORD wPosX, WORD wPosY);

	//! �������� ����.
	HRESULT ReqBoxOpen(WORD wPosX, WORD wPosY);

	//! ���������� �ڽ� ����.
	HRESULT ReqRandumBoxOpen(WORD wPosX, WORD wPosY);

	//! ���������� �ڽ� ����.
	HRESULT ReqDisJunction(WORD wPosX, WORD wPosY);

	//! �� ���� ��Ų ������ ����.
	HRESULT ReqPetSkinPackOpen(WORD wPosX, WORD wPosY);

	//! �Ⱓ �����ϱ�;
	HRESULT ReqPeriodExtend(WORD wPosX, WORD wPosY);

	//! �����ϱ�.
	HRESULT ReqGrinding(WORD wPosX, WORD wPosY);

	//! �Ͻð�ȭ
	HRESULT ReqTLGrinding(WORD wPosX, WORD wPosY);
	void    ReqTLGrinding();

	//! �ڽ��� ����� �Ϲ� ����� ����.
	HRESULT ReqDisguise(WORD wPosX, WORD wPosY);

	//! �ڽ��� ����� �Ϲ� ����� �и�.
	HRESULT ReqCleanser(WORD wPosX, WORD wPosY);

    //! �������� �ڽ��������� ����.
    HRESULT ReqBoxWrapping(WORD wPosX, WORD wPosY);

    //! ����� �ڽ��� ǯ;
    HRESULT ReqBoxUnwrapping(WORD wPosX, WORD wPosY);

	//! �ɸ��� ī�� ���.
	HRESULT ReqCharCard(WORD wPosX, WORD wPosY);

	//! â�� ī�� ���.
	HRESULT ReqStorageCard( WORD wPosX, WORD wPosY, WORD wSTORAGE );

	//! â�� ī�� ���.
	HRESULT ReqInvenLineCard(WORD wPosX, WORD wPosY);

	//! â�� ���� ī�� ��� ��û
	HRESULT ReqStorageOpenCard(WORD wPosX, WORD wPosY);

	//! �κ��丮���� â�� ���� ��û
	bool ReqStorageOpenFromInven();

	//! ��� ���� ���� ��û;
	HRESULT ReqLockBoxUnlock(WORD wPosX, WORD wPosY);
	//! ���������� �ڽ� ����;
	HRESULT ReqLockBoxOpen(WORD wPosX, WORD wPosY);

	//! â�� ���� ī�� ��� �Ϸ�
	HRESULT ReqStorageCloseCard();

    HRESULT ReqPMarketSearchOpen(WORD wPosX, WORD wPosY);
    HRESULT ReqPMarketSearchClose();

	//! �����̾��� ���.
	HRESULT ReqPremiumSet(WORD wPosX, WORD wPosY);
	
	//! ��ų�� ���� ����.
	HRESULT ResetSkillStats(WORD wPosX, WORD wPosY);
	HRESULT ReqResetSkillStats( WORD* pStats, const WORD wMaxAmount, const EMITEM_TYPE emItemType );	
	//HRESULT RequestResetSkillStats( LuaTable tbStats, const WORD wMaxAmount, const EMITEM_TYPE emItemType );
	HRESULT RequestOpenResetSkillStats( const WORD wPosX, const WORD wPosY );
	HRESULT RequestResetSkillStatsBySkillUI();

	//! NPC ��ȭâ ����;
	HRESULT ReqDialogueWindowOpen(const SNATIVEID& sNPCID, const DWORD dwGaeaID, const EMCROW emCrow = CROW_NPC );

	// Note : ������ ���� ī�� �� ��ü�� ����
	HRESULT ReqPostBoxOpenCard(WORD wPosX, WORD wPosY);
	HRESULT ReqPostBoxOpen	  ( const SNATIVEID& sTargetID );
	HRESULT	ReqPostBoxClose	  ();

	// ������ ��������� ������;
	HRESULT	ReqPostBoxSendPageAttachItem();

	//! ��Ÿ�� ����.
	HRESULT ReqInvenHairChange(WORD wPosX, WORD wPosY);

	HRESULT ReqInvenHairStyleChange( WORD wHairStyle );
	HRESULT InvenHairStyleChange(WORD wPosX, WORD wPosY);

	HRESULT ReqInvenHairColorChange( WORD wHairColor );
	HRESULT InvenHairColorChange(WORD wPosX, WORD wPosY);

	//! �ڽ�Ƭ ���� ����. by luxes.
	HRESULT ReqInvenCostumColorChange( DWORD dwMainColor, DWORD dwSubColor );
	HRESULT InvenCostumColorChange( EMSLOT emSlot, bool bRestrict = true ); // bRestrict ��� ���� ����. default�� ���ѵ�.

	//Ż�� ���� ����
	HRESULT ReqInvenVehicleColorChange( DWORD dwMainColor[], DWORD dwSubColor[] );
	HRESULT InvenVehicleColorChange(WORD wPosX, WORD wPosY);

	//����ũ �ν��� �߰�
	HRESULT ReqInvenBikeBoost(WORD wPosX, WORD wPosY);	

	//! �󱼽�Ÿ�� ����.
	HRESULT ReqInvenFaceChange(WORD wPosX, WORD wPosY);

	HRESULT ReqInvenFaceStyleChange( WORD wHairStyle );
	HRESULT InvenFaceStyleChange(WORD wPosX, WORD wPosY);

	//! �ý�ī�� ���
	HRESULT InvenUseTaxiCard(WORD wPosX, WORD wPosY);

	//! NPC ��ȯ
	HRESULT InvenUseNpcRecall(WORD wPosX, WORD wPosY);

	// Note : ���� ����
	HRESULT InvenGenderChange(WORD wPosX, WORD wPosY);
	HRESULT	ReqInvenGenderChange( WORD wFace, WORD wHair );

	HRESULT	ReqInvenSchoolChange( WORD wPosX, WORD wPosY );
	HRESULT	ReqInvenSchoolChange( WORD wSchool );

	// Note : �̸� ����.
	HRESULT ReqInvenRename( const char* szCharName );

	HRESULT InvenRename(WORD wPosX, WORD wPosY);

	//	Memo : ���� ���
	HRESULT ReqRemodelOpenCard(WORD wPosX, WORD wPosY);

	//	Memo : ������ ���
	HRESULT ReqGabargeOpenCard(WORD wPosX, WORD wPosY);

	//! ��Ʈ�� ������, ����ġ ȹ�� ī�� ���
	HRESULT ReqInvenVietnamGet( WORD wPosX, WORD wPosY, bool bGetExp );

	// Note : ���ö�(���Ĺ���) ���
	HRESULT ReqInvenLunchBox(WORD wPosX, WORD wPosY);

	// Note : �ֻ��� ���� ��ȣ ��û;
	HRESULT ReqDiceNumber(const DWORD _nDice);

	//! �������� �н���
	HRESULT ReqStagePass(WORD wPosX, WORD wPosY);

	//! �δ� ����Ƚ�� ������
	HRESULT ReqEnterNumCharge(WORD wPosX, WORD wPosY);

    //! �Ǹ� ��� Ȯ�� ī��
    HRESULT ReqConsignmentSlotExpiration(WORD wPosX, WORD wPosY);

	//! ���� ���� ������ ���;
	HRESULT ReqInvenChangeCountry ( WORD wPosX, WORD wPosY );

	HRESULT	ReqJumpingLevelUp( int nExp );
	HRESULT	ReqJumpingLevelUp( WORD wPosX, WORD wPosY );

	// ����ġ ���� / ĸ��
	HRESULT	ReqInvenExpCompressor( WORD wPosX, WORD wPosY );
	HRESULT	ReqInvenExpCapsule( WORD wPosX, WORD wPosY );

	// ������ ��ų ����
	HRESULT ReqInvenChangeItemSkill ( WORD _wPosX, WORD _wPosY );

	// ������ �⺻ �ɷ�ġ ����;
	HRESULT ReqInvenChangeBasicStats ( WORD _wPosX, WORD _wPosY );

	// ������ ���� �ɷ�ġ ����;
	HRESULT ReqInvenChangeAddStats ( WORD _wPosX, WORD _wPosY );

	// ĳ���� ���� �� ����;
	HRESULT ReqInvenUnlockCharacterSlot ( WORD _wPosX, WORD _wPosY );

	// Custom Message ( Instance System );
	void SendCustomMessage ( const DWORD _dwDestType, const DWORD _dwInstanceDungeonID, const DWORD _dwParam1, const DWORD _dwParam2, const DWORD _dwParam3, const DWORD _dwParam4 );

public:
	//! �ŷ��� �ݾ� �ֱ�.
	//HRESULT ReqTradeMoney( LONGLONG lnMoney );

	//! �ŷ��� ������ �ֱ�/����/��ȯ.
	HRESULT ReqTradeBoxTo(WORD wPosX, WORD wPosY);

	// Note : �ŷ� ���.
	HRESULT ReqTradeLock(); // by luxes.

	//! �ŷ� ����.
	HRESULT ReqTradeAgree();

	//! �ŷ� ���.
	HRESULT ReqTradeCancel();

	//! ��� ��û.
	HRESULT ReqConflict( DWORD dwID, const SCONFT_OPTION &sOption );

public:
	//! â�� ������ ������ ��û.
	HRESULT ReqGetStorage( DWORD dwChannel, DWORD dwNpcGlobID );
	bool ReqGetStorageForWidget( DWORD dwChannel );

	//! Ư�� â�� ������ ������ ������ ��û.
	HRESULT ReqGetStorageSpecificItem(DWORD itemType);
	
	//! â�� ������ �鶧, ������, ��ȯ�Ҷ�, ��ĥ��.
	HRESULT ReqStorageTo( SNATIVEID sNPCID, DWORD dwNPCGlobalID, DWORD dwChannel, WORD wPosX, WORD wPosY );

	//! �κ��丮 ��ħ ������ �и�.
	HRESULT ReqStorageSplit( SNATIVEID sNPCID, DWORD dwNPCGlobalID, DWORD dwChannel, WORD wPosX, WORD wPosY, WORD wSplitNum );

	//! â�� ������ ����Ҷ�( ���ñ�, ��ų���� �� ).
	HRESULT ReqStorageDrug( SNATIVEID sNPCID, DWORD dwNPCGlobalID, DWORD dwChannel, WORD wPosX, WORD wPosY );

	//! â�� �� �ֱ�.
	HRESULT ReqStorageSaveMoney( SNATIVEID sNPCID, DWORD dwNPCGlobalID, LONGLONG lnMoney );
	void ReqStorageSaveMoneyForWidget( LONGLONG lnMoney );

	//! â�� �� ������.
	HRESULT ReqStorageDrawMoney( SNATIVEID sNPCID, DWORD dwNPCGlobalID, LONGLONG lnMoney );
	void ReqStorageDrawMoneyForWidget( LONGLONG lnMoney );

	//! ���� Npc���� ��ǰ�� �����ϰų� �ǸŽ�.
	HRESULT ReqNpcTo( SNATIVEID sNPCID, DWORD dwNPCGlobalID, DWORD dwChannel, WORD wPosX, WORD wPosY, WORD wBuyNum=1, EMCROW emCrow = CROW_NPC );

    //! ���� Npc���� ��ǰ�� �ǸŽ�.
    HRESULT ReqNpcSaleTo( SNATIVEID sNPCID, DWORD dwNPCGlobalID, WORD wPosX, WORD wPosY, EMCROW emCrow = CROW_NPC );

    //! ���� Npc���� ��ǰ�� ��Ŭ������ �ٷ� �ǸŽ�.
    HRESULT ReqNpcDirectSaleTo( SNATIVEID sNPCID, DWORD dwNPCGlobalID, WORD wPosX, WORD wPosY, EMCROW emCrow = CROW_NPC );

	//! ������ ��ǰ ��������.
	HRESULT ReqChargedItemTo(WORD wPosX, WORD wPosY);

	//! ������ ������ ���� ��������
	HRESULT ReqItemBankInfo(bool bFirstJoin);

	HRESULT ReqRankingsInfo ();

    //! NPC ����� ui���� �Ҹ�ǰ ������ üũ.
    HRESULT ReqCheckRebuySupplies( bool bCheck );

	//! ���� Npc���� ��ȯ ���� ����
	HRESULT ReqExChangeNpcTo( const GLMSG::SNETPC_REQ_EXCHANGE_BUY_FROM_NPC* pData );

public: 
    void    MsgActivityUpdateComplate(NET_MSG_GENERIC* nmg);
    void    MsgActivityUpdateValue(NET_MSG_GENERIC* nmg);
    void    MsgActivityNotifyComplate(NET_MSG_GENERIC* nmg);
    void    MsgActivityNotifyClosed(NET_MSG_GENERIC* nmg);
    void    MsgActivitySync(NET_MSG_GENERIC* nmg);
    void    MsgActivityRewardPoint(NET_MSG_GENERIC* nmg);
    void    MsgActivityRewardBadge(NET_MSG_GENERIC* nmg);
    void    MsgActivityChangeBadgeFB(NET_MSG_GENERIC* nmg);
	void    MsgActivityLoadCompletedFF(NET_MSG_GENERIC* nmg);

	void	MsgGetChaExtraInfo(NET_MSG_GENERIC* nmg);



	//! �ʵ� ������(��) ������.
	HRESULT ReqFieldTo( const STARGETID &sTargetID, bool bPet = false );

	//! �κ��丮 ������ �鶧, ������, ��ȯ�Ҷ�, ��ĥ��.
	HRESULT ReqInvenTo(WORD wPosX, WORD wPosY);
	//! ��Ʈ�� �κ��丮 ������ �鶧, ������, ��ȯ�Ҷ�, ��ĥ��.
	HRESULT ReqVNInvenTo(WORD wPosX, WORD wPosY);
	//! ��Ʈ�� �κ��丮�� ������ ��ü ����
	HRESULT ReqVNInveReset();
	//! ��Ʈ�� �κ��丮���� ������ ��ư���� �������� �ű� ���
	HRESULT ReqVietemInvenTo(WORD wPosX, WORD wPosY);

	//! �κ��丮 ��ħ ������ �и�.
	HRESULT ReqInvenSplit( WORD wPosX, WORD wPosY, WORD wSplitNum );

	//! �κ��丮 ������ ����Ҷ�( ���ñ�, ��ų���� �� ).
	ESlotHold ReqInvenDrug(WORD wPosX, WORD wPosY);

    //! �κ��丮 ������ ���� Ȯ��
    EMITEM_TYPE GetInvenItemType( WORD wPosX, WORD wPosY );

    bool ReqInvenDrugDrug( EMITEM_DRUG DrugType );
    void ReqInvenDrugNotHoldItem( const SITEM* pItem, WORD wPosX, WORD wPosY, const SINVENITEM* pInvenItem );
    void ReqInvenDrugPrivateMarket( WORD wPosX, WORD wPosY );
    void ReqInvenDrugPetCard( const SINVENITEM* pInvenItem, WORD wPosX, WORD wPosY );
    void ReqInvenDrugLunchBox( WORD wPosX, WORD wPosY );
    ESlotHold ReqInvenDrugHoldItem( const SITEM* pHold, const SITEM* pItem, WORD wPosX, WORD wPosY );
    void ReqInvenDrugHoldPetRevive( const SITEM* pItem, WORD wPosX, WORD wPosY );
    //! �κ��丮 ������ ����.
    HRESULT ReqInvenItemSort();
    HRESULT MsgReqInvenItemSort(NET_MSG_GENERIC* nmg);
	HRESULT MsgReqInvenItemSortStart(NET_MSG_GENERIC* nmg);
	HRESULT MsgReqInvenItemSortData(NET_MSG_GENERIC* nmg);
	HRESULT MsgReqInvenItemSortEnd(NET_MSG_GENERIC* nmg);

	// ������ ������/�ν�Ʈ , ����;
	virtual void DurabilityDropHit( unsigned int nDamage, BOOL bPvp/* = FALSE  */);
	virtual void DurabilityDropUseSkill();
	virtual void DurabilityDropDeath( BOOL bPvp = FALSE );
	virtual void ItemRepairing( EMSLOT _slot );
	virtual void Durabilitiy_INIT_DATA( EMSLOT slot ); 
	
	// UI ��ε�� ����� ���̰��ϱ�����;
	//LuaTable GetWeaponBoostInitData(void);
	void SfSetWeaponBoostInfo(void);
private:
	bool GetWeaponDurabilityValue(float& fDurability, float& fMaxDurability);

public:
	//! ���� �����Կ� �ִ� ���� ����.( ��ǰ�� ��� ���ñ� ).
	void ReqActionQ(WORD wSLOT);

    void ReqActionQ_SlotDrug( SNATIVEID NativeId );
	//! ��ų ���� ��û.
	HRESULT ReqSkillUp(const SNATIVEID& skill_id);
    HRESULT ReqRnSkillUp( std::vector<SNATIVEID>& vecSkillID );

	//! ����� ��ų ����.
	HRESULT ReqSkillRunSet( const WORD wSLOT );
	HRESULT ReqSkillRunReSet();

	//! ������ ���Կ� ���/����.
	HRESULT ReqItemQuickSet( const WORD wSLOT );
	HRESULT ReqItemQuickReSet( const WORD wSLOT );

	//! ��ų ���Կ� ���/����.
	HRESULT ReqSkillQuickSet( const SNATIVEID& skill_id, const WORD wSLOT );
	HRESULT ReqSkillQuickReSet( const WORD wSLOT );

	//! ���� �κ��丮�� ������ ����Ҷ�. by luxes.
	HRESULT ReqSlotConsume( EMSLOT emSlot );

	//! �������� �����ϰų� ���ø�.
	HRESULT ReqSlotTo( EMSLOT emSlot );

	//! �ذ��� ���⸦ ���ΰ� ���� ��ü.
	HRESULT ReqSlotChange();

	//! �������� �����ϰų�/�Ⱦƹ���.
	//! ������ �ٴڿ� ����.
	HRESULT ReqHoldToField( const D3DXVECTOR3 &vPos );
	void ReqHoldToFieldForWidget( const char* szSlotID, int nHoldItemPlace, 
		WORD wSlotPosX, WORD wSlotPosY, float fPosX, float fPosY, float fPosZ );

	//! ������ �ٴڿ� ���� �� �ִ��� üũ. by luxes.
	HRESULT CheckHoldToField( const D3DXVECTOR3 &vPos );

	//! ������ �ٴڿ� ���.
	HRESULT ReqHoldToFieldFireCracker( const D3DXVECTOR3 &vPos );

	//! �� �ٴڿ� ����.
	HRESULT ReqMoneyToField( LONGLONG lnMoney );

	//! ��Ȱ ��ġ ���� ��û.
	HRESULT ReqReGenGate( DWORD dwNpcGlobID );

	//! ������ ����ġ ������ ��û�Ѵ�.(NPC)
	HRESULT ReqRecoveryExpNpc( DWORD dwNpcGlobID );
	
	//! ������ ����ġ�� ���´�.
	HRESULT ReqGetReExpNpc( DWORD dwNpcGlobID );	

	//! ġ�� ��û.
	HRESULT ReqCure( DWORD dwNpcGlobID, DWORD dwTalkID );
	HRESULT MsgCureFB(NET_MSG_GENERIC* nmg);

	//! �ɸ��� ���� ����.( stats, skill )
	HRESULT ReqCharReset( DWORD dwNpcGlobID );

	//! NPC�� item�� ��ȯ A:npc���� �ִ°�, b:npc���Լ� �޴°�.
	HRESULT ReqItemTrade(DWORD dwNpcGlobID, DWORD dwTalkID, std::tr1::shared_ptr<SNpcTalk> spNpcTalk);

	//! ��ȯ�� ���� ��û;
	HRESULT ReqHireSummon ( DWORD dwNpcGlobID, DWORD dwTalkID, std::tr1::shared_ptr<SNpcTalk> spNpcTalk );

	//! ģ������ ����.
	HRESULT	Req2Friend( const char *szNAME );

	HRESULT ReqNpcTalkBasic( DWORD dwNpcGlobID, DWORD nAction, DWORD dwTalkID );
	HRESULT MsgNpcTalkBasic( DWORD dwNpcGlobID, DWORD nAction, DWORD dwTalkID );

public:

	//! ���ο� ����Ʈ ����.
	HRESULT ReqQuestStart( DWORD dwNpcGlobID, DWORD dwTalkID, DWORD dwQUESTID );

	//! ����Ʈ ����.( npc�� ��ȭ )
	HRESULT ReqQuestStepNpcTalk( DWORD dwNpcGlobID, DWORD dwTalkID, DWORD dwQUESTID, DWORD dwQUESTSTEP );

	//! ����Ʈ ����.
	HRESULT ReqQuestGiveUp( DWORD dwQUESTID );

	//! ����Ʈ �о�� ��û( �о����� ����. )
	HRESULT ReqQuestREADINGReset( DWORD dwQUESTID );

	//! ����Ʈ �Ϸ��û.
	HRESULT ReqQuestComplete( DWORD dwQUESTID );

	//! ���� ���� ��û.
	HRESULT ReqBusStation( DWORD dwNpcGlobID, DWORD dwSTATION, EMCROW eCrow = CROW_NPC );

	//! �ý� ���� ��û.
	HRESULT ReqTaxiStation( WORD wPosX, WORD wPosY, int nSelectMap, int nSelectStop );
	
	//! Ȯ���� ��� ��û.
	HRESULT ReqLoudSpeaker( const char* szChat, const VEC_LINK_DATA_BASIC& vecLinkDataBasic );

public:
	//! ���λ����� Ÿ��Ʋ ����.
	HRESULT ReqPMarketTitle( const char* szTitle );

	//! ���λ����� �������� ���.
	HRESULT ReqPMarketRegItem( WORD wPosX, WORD wPosY, LONGLONG llMoney, DWORD dwNum );

	//! ���λ����� ��ϵ� �������� ����.
	HRESULT ReqPMarketDisItem(WORD wPosX, WORD wPosY);

	//! ���λ��� ����.
	HRESULT ReqPMarketOpen( BOOL bOnlyUI );

	//! ���λ��� �ݱ�.
	HRESULT ReqPMarketClose();

	//! �ٸ� ����� ���λ����� ���� ����.
	HRESULT ReqPMarketInfo( DWORD dwGaeaID );
	HRESULT ReqPMarketInfoRelease( DWORD dwGaeaID );

	//! �ٸ� ����� ���� �������� ��ǰ�� ����.
	HRESULT ReqPMarketBuy( DWORD dwGaeaID, WORD wPosX, WORD wPosY, DWORD dwNum );

public:    
	//! Ŭ�� ����.
	HRESULT ReqClubNew( DWORD dwNpcGlobID, const char* szClubName );
	void ReqClubNewForWidget( const char* szClubName );
	
	//! �������� ������ ����.
	HRESULT ReqGuidCommission( DWORD dwNpcGlobID, float fRATE );
	void ReqGuidCommissionForWidgt( float fRate );

	//! �⼮ ����Ʈ ��û
	HRESULT	ReqAttendList( bool bDay = false );

	//! �⼮ ��û
	HRESULT	ReqAttendance();

	HRESULT ReqGathering( const STARGETID& sTargetID );
	HRESULT ReqCancelGathering();

	HRESULT ReqCharacterLock ( EMCHARLOCK emReq, const TCHAR * szPassword, const TCHAR * szPIN );

public:
	//! ĳ������ ��ȭ��ȣ ���� / ������Ʈ
	HRESULT ReqSetPhoneNumber( const TCHAR * szPhoneNumber );
	HRESULT ReqSendSMS( int nCharID, const TCHAR * szPhoneNumber, const TCHAR * szSmsMsg );

public:
	//! ��Ƽ������ Qbox �ɼ� ���¸� ������.
	bool    ReqQBoxEnableState( bool bQboxEnable );

public :
	//! �ڽ�Ƭ �ɷ�ġ �ο� ���� ��Ŷ 
	void	ReqCostumeStatCToS();
	void    ReqCostumeStat( WORD wPosX, WORD wPosY );
	void    ReqCostumeStatRelease( WORD wPosX, WORD wPosY );
	void	ReqCostumeStatReleaseOK( WORD wPosX, WORD wPosY, WORD wHoldPosX, WORD wHoldPosY );

	// Ŭ�� �̸� ����
	HRESULT InvenClubRename( WORD wPosX, WORD wPosY );
	HRESULT ReqInvenClubRename( const char* szClubName );

public:
	// ĳ���� ���� ����;
	void	ReqLockCharSlot ( const WORD _wCharSlotNum );
	void	ReqUnlockCharSlot ( const WORD _wCharSlotNum );
	void	ReqChangeCharSlot ( const WORD _wCharSlotNum );

public:
	// PK Effect Cards
	CClientPKCardManager* GetPKCardManager() const { return m_pPKCardManager; }
	EMPK_EFFECT_CARD_TYPE GetActivePKCard() const;
	BOOL UsePKEffectCard(EMPK_EFFECT_CARD_TYPE emType, DWORD dwDays = 0);
	void UpdatePKCards();

	const bool IsChangeCharSlot ( const WORD _wCharSlotNum );
	// add stats cmd
	void ReqStatsUpCmd( EMSTATS emStats, DWORD value );
private :
	int m_nTitleSelectIndex;					// ������ Ÿ��Ʋ �ε���

	enum { SKILL_TAB_COUNT = 4 };
	VEC_GLSKILL m_vecSkill[ SKILL_TAB_COUNT ];	// ��ų ����Ʈ

public :
	// ��ų ����Ʈ ����
	void GenerateSkillList();

public :
	// Ÿ��Ʋ ����
	//LuaTable GetTitle();
	// ������ Ÿ��Ʋ �ε���
	int GetTitleSelectIndex() { return m_nTitleSelectIndex; }
	// ���� ����ġ
	int GetSumPowerAttribute( int nType );
	// ���׷�
	short GetResistFire() { return m_sSUMRESIST_SKILL.nFire; }
	short GetResistIce() { return m_sSUMRESIST_SKILL.nIce; }
	short GetResistElectric() { return m_sSUMRESIST_SKILL.nElectric; }
	short GetResistPoison() { return m_sSUMRESIST_SKILL.nPoison; }
	short GetResistSpirit() { return m_sSUMRESIST_SKILL.nSpirit; }
	// �ɷ�ġ ��� ��
	//LuaTable GetChangeUpAbillity( WORD wPow, WORD wDex, WORD wSpi, WORD wStr, WORD wSta );
	// ���� ����Ʈ ���
	void ApplyStatPoint( WORD wPow, WORD wDex, WORD wSpi, WORD wStr, WORD wSta );
	// �������� ������
	//LuaTable GetPutOnItem( EMSLOT emSlot );
	//LuaTable GetPutOnItems();
	// �������� ������ ����
	WORD GetPutOnItemCount( EMSLOT emSlot );

	//const SITEMCUSTOM* GetPutOnItemCustom( SNATIVEID nativId );
	// �������� ������ Ÿ��
	int GetPutOnItemType( EMSLOT emSlot );
	// �������� ���� �Ӽ�;
	int GetWeaponType(void);

	// ���� ������ ���������� üũ
	int CheckTrashItem();
	// �κ��丮 ������ ���
	ESlotHold UseInvenItem( WORD wPosX, WORD wPosY );
	// ���� ������ ���� ��û �õ�
	bool TryRequestBankItemInfo();
	// �ֱ� ��� ��ų ID
	DWORD GetSkillRunSlotID() { return m_sRunSkill.dwID; }
	// ������ ��ϵ� ��ų
	DWORD GetSkillQuick( WORD wIndex );
	// ������ ��ϵ� ������
	DWORD GetItemQuick( WORD wIndex );

	// ä�� �޽���
	//void SendChatMsg( DWORD dwType, const char* szName, const char* szMsg, LuaTable tbLinkData );

	// ��ų �ε��� ����
	void GetSkillIndexRange( int& nBegin, int& nEnd );
	// ��ų ����Ʈ
	//LuaTable GetSkillList( int nPage );
	// �Ҹ��� ��ų ����Ʈ
	//DWORD GetUsingSkillPoint( LuaTable tbSkillList );
	// ��ų ����
	//LuaTable GetSkillInvest( DWORD dwID, WORD wLevel );
	//  ��� �� �ִ� �ִ� ��ų ����
	//LuaTable GetLearnSkillMaxLevel(DWORD dwID, DWORD dwLevel);

	//LuaTable GetLearnSkillMinLevel(DWORD dwID, DWORD dwLevel);

	// �ʿ� ��ų ����Ʈ
	DWORD GetNeedSkillPoint( DWORD dwID, WORD wLevel );
	// ��ų ���� �ø���
	//void SkillLevelUp( LuaTable tbSkillUpData );
	// ��ų ��� ��ȯ �ݾ�
	std::string GetLearnedSkillCost();
	// �� ��ų ������ �ε��� �˻�
	WORD GetEmptySkillQuick();
	// �������� Ż�� ID ��
	//LuaTable GetHaveVehicleAll();
	// �� ID
	DWORD GetCurrentMapID();
	// ĳ���� ��ġ
	//LuaTable GetPositionForLua();
	// ĳ���� �� ��ġ
//	LuaTable GetMapPosition();
	// �̴ϸ� ��ġ ������
	//LuaTable GetMapPositionOffset();
	// �ش� �̴ϸ� ��ġ�� �̵�
	bool MoveToMap( DWORD dwMapID, int nPosX, int nPosY, bool bVehicle, bool bGMMove );

	void ReqBikePutOnWith();
	// Ż�� �÷� ����
//	void SetVehicleColor( LuaTable tbColorList );
	// Ż�� �÷�
//	LuaTable GetVehicleColor();
	// Ż�� �÷� ���� Ȯ��
//	void CommitVehicleColor( LuaTable tbColorList );

	// �ڽ�Ƭ �÷� ����
//	void SetCostumeColor( LuaTable tbColor );
	// �ڽ�Ƭ �÷�
	//LuaTable GetCostumeColor();
	// �ڽ�Ƭ �÷� ���� Ȯ��
//	void CommitCostumeColor( LuaTable tbColor );

	// ĳ���� ���� ��ġ ��ID
	DWORD GetStartMapID();
	// ��Ȱ Ÿ�� ������ ���� ���� Ȯ��
//	LuaTable GetResurrectionCondition();
	// ����ġ ȸ�� ��Ȱ�� �Ҹ�Ǵ� ����ġ, ��
//	LuaTable GetRsurrectionExpRecovery();
	// üũ ����Ʈ ��Ȱ ��û
	void ResurrectCheckPoint();
	// ����ġ ȸ�� ��Ȱ ��û
	void ResurrectExp();
	// ��ȥ�� ��Ȱ ��û
	void ResurrectCurPos();
	// ��ų ��Ȱ ��û
	void ResurrectSkill();
//����Ʈ ���̺� ���� ��ȯ�� �Լ� ������ �ʿ��� �͵��� �����ܴ� ��� ��� ���⿡ ��ġ�ص�(����̳� ����Ʈ ����� �����ʿ�, Quest)
//	LuaTable GetQuestListProc(); //���� �� �������� ����Ʈ ���� ���̺� ��ȯ(������)
//	LuaTable GetQuestDetailListProc(std::string _AreaName); // ���ڷ� ���� �̸��� �ش��ϴ� �������� ����� ��ȯ(������)
//	LuaTable GetQuestListEnd(); //���� �� �������� ����Ʈ ���� ���̺� ��ȯ(�Ϸ���)
//	LuaTable GetQuestDetailListEnd(std::string _AreaName); // ���ڷ� ���� �̸��� �ش��ϴ� �������� ����� ��ȯ(�Ϸ���)
//	LuaTable GettbQuestProc( DWORD dwQuestID); // ����Ʈ ���뿡 �ش��ϴ� �����͵��� ��ȯ(������)
//	LuaTable GettbQuestEnd( DWORD dwQuestID);	// ����Ʈ ���뿡 �ش��ϴ� �����͵��� ��ȯ(�Ϸ���)
//	LuaTable GettbQuestProcInfo( DWORD dwQuestID);	// ����Ʈ ������׿� �ش��ϴ� �����͵��� ��ȯ(������)

	bool GetQuestComplete(DWORD _dwID, bool ProcOrEnd); //����Ʈ �Ϸ� ���� ��ȯ
	DWORD GetQuestStep(DWORD _dwID); //���� �ܰ� ��ȯ
	bool GetQuestCompleteCheck(DWORD _dwID); // �κ��丮�� ������� �Ϸ��ư���� ȹ���ϴµ� �׶� �Ϸ��޳� ���޳� ����
	bool GetQuestGiveUp(DWORD _dwID); //����Ʈ ���Ⱑ�� ����
	void SetQuestHelpCheck(DWORD _dwID, bool _bCheck);

	// ��Ŀ ���� �ð�
//	LuaTable GetStorageTimeForWidget( DWORD dwChannel );
	// �����̾� ���� �ð�
//	LuaTable GetPremium();

	// �κ��丮 ������ �ӽ� ���� �ɷ�ġ
//	LuaTable TempPutOnStatInvenItem( WORD wPosX, WORD wPosY, WORD wVirtualPow, WORD wVirtualDex, 
//		WORD wVirtualSpi, WORD wVirtualStr, WORD wVirtualSta );

	// ���� ���� �ʱ�ȭ ��û
	void ReqResetCountry( WORD wPosX, WORD wPosY );

	// ����ġ ĸ�� ��� ��û;
	void ReqUseExpCapsuleItem ( WORD wInvenPositionX, WORD wInvenPositionY );

	// ����ġ ���� ��� ��û;
	void ReqUseExpCompressorItem ( WORD wInvenPositionX, WORD wInvenPositionY );

	// ĳ���� ���� Ȯ�� ������ ��� ��û;
	void ReqUseUnlockCharacterSlotItem ( WORD wInvenPositionX, WORD wInvenPositionY );

	// �κ��丮 ������ ���� �������� Ȯ��
	bool CheckInvenItemDecompose( WORD wPosX, WORD wPosY );

	// �������� �Ҹ�ǰ
//	LuaTable GetEquipConsume();

	// ���� ������ ������
//	LuaTable GetRepairableItem();
	// ������ ���� ��û
//	void ReqItemRepair( LuaTable tbSlot );
	// ������ ���� ����ī�� ���
	void ReqItemRepairCardUse( WORD wPosX, WORD wPosY );

	// �κ� ������ ����
	DWORD GetInvenItemCount( WORD wMID, WORD wSID );
	// �κ� ������ ���� Ÿ��
	WORD GetInvenItemSuitType( WORD wPosX, WORD wPosY );
	// ���� ���� üũ;
	bool WearableCheck( WORD wPosX, WORD wPosY );

	// �κ� ������ TurnNum (����.. ?) �� ��������
	DWORD GetSlotTurnNumCountItem(WORD wMID, WORD wSID);

	// ���ö� ���� ������ ���ΰ�?
	bool IsEnableLunchBox();

	// �ڽ�Ƭ ������ ���
	void UseDisguiseInvenItem( WORD wPosX, WORD wPosY, WORD wHoldPosX, WORD wHoldPosY );
	bool IsDisguiseInvenItem( WORD wPosX, WORD wPosY );

	// ������ �������� Ȯ���Ѵ�;
	const bool IsRebuildItem ( const DWORD _dwNPCGlobalID = MAXDWORD ) const;

public:
	HRESULT 				ReqRebuildOpen( SNATIVEID sNPCID, DWORD dwNPCGlobalID ); // ����â ����	// ITEMREBUILD_MARK
	HRESULT 				ReqRebuildMoveItem(const bool bMsg = true);	// ����â�� �� ������ ���� ��� ������ �̵� ��û;
	HRESULT 				ReqRebuildMoveStamp(void);			// ����â�� �� ������ ���� ������ �̵� ��û;
	HRESULT 				ReqRebuildInputMoney( LONGLONG i64InputMoney );	// ����â�� �Է��� �ݾ� ������ ����

	HRESULT					ReqRebuild_SelectCache(void);
	HRESULT 				ReqChangeRebuildType( DWORD dwNPCGlobalID, bool bKeepOrigin );
	HRESULT					ReqRebuildTry( SNATIVEID sNPCID, DWORD dwNPCGlobalID );				// ���� �õ�;
	HRESULT 				ReqRebuildResult( SNATIVEID sNPCID, DWORD dwNPCGlobalID );			// ���� ��û;
	HRESULT 				ReqRebuildCancel( SNATIVEID sNPCID, DWORD dwNPCGlobalID );			// ���� ���;
	HRESULT					ReqRebuildDirectResult( SNATIVEID sNPCID, DWORD dwNPCGlobalID );	// ���� �ٷ� ��û;
	HRESULT 				ReqRebuildClose();								// ����â �ݱ�

public:
	HRESULT 				ReqGarbageOpen();								// ������ ����
	HRESULT 				ReqGarbageMoveItem();							// �����뿡 ������ �̵� ��û
	HRESULT 				ReqGarbageResult();								// ������ �ļ� ��û
	HRESULT 				ReqGarbageClose();								// ������ �ݱ�

    HRESULT 				ReqChangeTitle( DWORD dwIndex, bool bSync = false );

	BOOL					GET_GARBAGE_ITEM( SITEMCUSTOM& sItemCustom );

	VOID					InitGarbageData();
	VOID					OpenGarbage()	   				{ m_bGarbageOpen = true; }
	VOID					CloseGarbage()	   				{ m_bGarbageOpen = false; }
	bool					ValidGarbageOpen() const		{ return m_bGarbageOpen; }

	bool					ValidItemMixOpen();
	const SITEMCUSTOM		GET_ITEM_MIX( int nIndex );
	void					SetItemMixMoveItem( int nIndex );
	void 					ReSetItemMixItem( int nIndex );
	void 					ResetItemMix();
	DWORD					GetItemMixMoney();
	HRESULT					ReqItemMix( DWORD dwNpcGlobID );

	bool					ValidWindowOpen();

	VOID					OpenCostumeStat()	   				{ m_bCostumeStats = true; }
	VOID					CloseCostumeStat();
	bool					ValidCostumeStatOpen();

public:
	// PET
	HRESULT 				ReqUsePetCard(WORD wPosX, WORD wPosY);
	HRESULT 				ReqReGenPet();
	HRESULT					ReqPetCardInfo();
	HRESULT 				ReqPetReviveInfo();
	HRESULT 				ReqPetRevive( DWORD dwPetID );

public:
    HRESULT                 ReqResurrect(const EMRESURRECTION_TYPE _resurrectType);  // ��Ȱ ��û;
	void 					ReqReBirth();						//! ��Ȱ ��û.
	void 					ReqLevelUp();						//! ���� ��û.
	void 					ReqStatsUp( EMSTATS emStats );	//! ���� �й� ��û.
	void					ReqStatsUp(WORD* emStatsArray); 
public:
	HRESULT 				SetVehicle( bool bActive );
	HRESULT 				ReqSetVehicle( bool bActive );	// Ż�� Ȱ��ȭ/��Ȱ��ȭ
	HRESULT 				ReqVehicleUpdate(); // Ż�� ������ ���� ��û
	void					ReqVehicleChangeAccessory( EMSUIT emSUIT ); // Ż�� ���� ������ ��ü
	void					ReqVehicleRemoveSlotItem( EMSUIT emSUIT );	 // Ż�� ���� ������ ����
	void					ReqVehicleGiveBattery(WORD wPosX, WORD wPosY);	// Ż�� ���� ä���
	void					ReqVehicleInvenUpdate();	// Ż�� ���� ��û

	void					UseBikeBoost();

public:
	void					ReqItemShopOpen( bool bOpen );		// ItemShop Open/Close �뺸

public:
	HRESULT 				ReqSkillHoldReset( SNATIVEID& sSkillID, WORD wSlot ); // ����/���ö� ���� ��û
	void					ReqSkillHoldResetForWidget( DWORD dwID, WORD wSlot );

	// ���û��� ���� ;
private:
	INT		m_nSelFormBoxPosX;
	INT		m_nSelFormBoxPosY;
public:
	//! ���û��� ����;
	HRESULT SelectiveformBoxOpen(WORD wPosX, WORD wPosY);
	//! ���û��� ������ ����;
	HRESULT ReqSelformBoxSelect(WORD wSelItemPos);
	void	MsgSelformBoxSelectOpenFb(NET_MSG_GENERIC* pMsg);

public:
	HRESULT 				DoActWait( DoActWait_TYPE emDoActWaitType );
	HRESULT 				Resurrect( const int nHP, const int nMP, const int nSP, D3DXVECTOR3 vPos, bool bWait );
	HRESULT 				MoveActiveMap( boost::shared_ptr<NavigationMesh> spNavi, const D3DXVECTOR3 &vPos, bool bWait=true );
	HRESULT 				PlayerUpdate( float fTime, float fElapsedTime );
	HRESULT 				SelectTargetUpdate( const bool bUsingSelectInfo );
	HRESULT 				ScopeSkillUpdate();
		
public:
	STARGETID*				FindForcedTarget( VEC_SK_TAR* pDetectMap );
	const STARGETID			GetSelectForcedTargetAround( const UINT nDegree = 360, const float fRange = 100.0f );
	const STARGETID 		GetSelectTargetAround( const EMFIND_TAR emFindTar, const D3DXVECTOR3& vPosition, const float fRange, const UINT nDegree );
	const STARGETID 		GetSelectTargetBodyAround( const EMFIND_TAR emFindTar, const float fRange, const UINT nDegree );
	const STARGETID			GetSelectTargetMouseAround( const EMFIND_TAR emFindTar, const float fRange );

public :
	virtual void ACCOUNTSKILL( const SCHARSKILL& sSkill, WORD wStrikeNum, bool bServer=false );

	void GetSkillCoolDownTime( const SNATIVEID& sID, float& fCoolDownTime, float& fCoolDownRatio );
	float GetSkillFactAge( DWORD dwSkillID );

	void GetItemCoolDownTime( const SNATIVEID& sID, float& fCoolDownTime, float& fCoolDownRatio );

	//LuaTable GetBuffList();
	void RecvExperience( const SNATIVEID& sID, WORD wLevel, float fAge = 0.0f );

protected:
	void				ReSelectAnimation();
	HRESULT 		UpdateAnimation( float fTime, float fElapsedTime, const EMACTIONTYPE Action );
	//HRESULT 		UpdateMultiAnimation( float fTime, float fElapsedTime, const EMACTIONTYPE Action );
	HRESULT			UpdateAction( float fTime, float fElapsedTime );	
	void			UpdateActState(void);
	HRESULT 		UpdateClientState( GLMSG::SNETPC_UPDATE_STATE *pNetMsg );

	void 					NewConftBoundEffect( const EMCONFT_TYPE emCONFT );
	void 					DelConftBoundEffect();

protected:
	STARGETID				m_sOLD_TARMOB;

public:
	bool 					IsClubBattle_TAR(std::tr1::shared_ptr<GLCharClient> pCHAR);
	int						GetClubColorIndex(std::tr1::shared_ptr<GLCharClient> pCHAR);

protected:
	STARGETID* 				GetCONFT_TAR   ( VEC_SK_ITER_RANGE &pair );
	STARGETID* 				GetCONFT_SUMMON( VEC_SK_ITER_RANGE &pair );

public:
	SACTIONTAR 				FindActionTarget( const D3DXVECTOR3 &vTargetPt, const D3DXVECTOR3 &vFromPt, const DWORD dwFindOption, const EMFIND_TAR emFINDTAR );
	SACTIONTAR 				FindNearItem();

	void 					SK_EFF_TARG( const PGLSKILL pSkill, const SANIMSTRIKE &sStrikeEff, const STARGETID &sTarget, STARGETID *pResultTarget = NULL );
	void 					SK_EFF_SELFZONE( const SANIMSTRIKE &sStrikeEff, const SKILL::EMEFFECTPOS emPOS, const char* const szZONE_EFF, STARGETID *pTarget=NULL );
	void 					SK_EFF_TARZONE( const STARGETID &sTarget, const SKILL::EMEFFECTPOS emPOS, const char* const szZONE_EFF );
	EMELEMENT				GET_ITEM_ELMT();
	void 					ACCOUNTBIKESKILL( const SCHARSKILL& sSkill, WORD wStrikeNum, bool bServer=false );

protected:	
	void 					StartAttackProc(void);
	BOOL 				AttackProc( float fElapsedTime );
	BOOL 				AttackVehicleProc( float fElapsedTime );
	void 					AttackEffect( const SANIMSTRIKE &sStrikeEff );

protected:
	void					StartSkillWaitProc();
	void					StartSkillProc();
	BOOL					IsExceedSkillHitFrameOfLast();
	BOOL					SkillProc( float fElapsedTime );

	// ��ų ���� �� ��ų�� ������ ���Ⱑ ���� ��� ���⸦ �����ϰ�;
	// ��ų �ִϸ��̼��� ���� �� �����;
	// �ذ��ο����� ����;
	void					SetHiddenWeaponPiece();
	void					ResetHiddenWeaponPiece();

	void 					SKT_EFF_HOLDOUT( STARGETID sTarget, DWORD dwDamageFlag );

	void 					SkillEffect( const SANIMSTRIKE &sStrikeEff );
public:
	void 					SkillEffectDealyAction(  const SANIMSTRIKE &sStrikeEff, SNATIVEID sSkillId, SKILLTARGETDATA *sSkillTargetData  );

private:
	void 					PreLoadEffectFull();
	void 					PreLoadEffect( const PGLSKILL pSkill );

	virtual void			SetAction(const EMACTIONTYPE emAction);

protected:
	//! ���� ��ų���� ����
	const bool SkillActivationInfoFanwise( IN const SKILLID& sSkillID, 
		IN const D3DXVECTOR3& vFromPt,
		IN const D3DXVECTOR3& vTargetPt,
		OUT D3DXVECTOR3& vOutDirection,
		OUT float& fOutRadius, 
		OUT float& fOutDegree );

	const bool SkillActivationInfoCircle( IN const SKILLID& sSkillID, 
		IN const D3DXVECTOR3& vFromPt,
		IN const D3DXVECTOR3& vTargetPt,
		OUT bool& bOutEnable, 
		OUT D3DXVECTOR3& vOutPoint,
		OUT float& fOutRadius );

public:	
	const bool SkillReaction( IN OUT GLREACTION& InReaction );

	void P2PReaction( IN OUT GLREACTION& InOutReaction );
	void PvPReaction( IN OUT GLREACTION& InOutReaction );

	void MobReaction( IN OUT GLREACTION& InOutReaction );
	void NpcReaction( IN OUT GLREACTION& InOutReaction );
	void SummonNpcReaction( IN OUT GLREACTION& InOutReaction );

	void ItemReaction( IN OUT GLREACTION& InOutReaction );
	void GatheringReaction( IN OUT GLREACTION& InOutReaction );

	void ReservedActionUpdate( float fTime );
	void DoReaction( float fTime, IN OUT GLREACTION& sReaction );
	void PreservedReaction( IN const GLREACTION& InReaction );
	GLREACTION_ITER BeginReservedSkillReaction(); // ���ེų�� ù �ݺ���
	GLREACTION_ITER NextReservedSkillReaction( GLREACTION_ITER iterWhere ); // ���� ���ེų�� �ݺ���
	GLREACTION_ITER EndReactionIter(); // ���׼��� end

public:
	void ReqPickupNearItem();
	void ReqPickupItem( struct CROWREN* pDispName );
	void ReqSkillReaction( STARGETID sTID );
	void ReqPartyMemberSkillReaction( DWORD dwGaeaID );

public:
	SNATIVEID GetSkillIDQSlot(WORD wSlot) const;
	void AttackTargetAfterSkill( const STARGETID& sTargetID );
	void ReserveSkill( const SNATIVEID &sSkillID, GLREACTION_SKILL& sSkillReaction );
	bool SkillActiveSelfToSpec( const SNATIVEID &sSkillID, WORD wSkillQuickSlot = 0xFFFF );
	bool SkillActiveZone( const SNATIVEID &sSkillID, WORD wSkillQuickSlot );
	bool SkillAutoActiveZone( const SNATIVEID &sSkillID, WORD wSkillQuickSlot );

	bool SkillActive( const SNATIVEID &sSkillID, WORD wSkillQuickSlot = 0xFFFF, bool bAutoMatic = false );
	bool SkillActive( const WORD wSkillQuickSlot );
	void ScopeSkillActive();

	EMSKILLCHECK SkillCheck ( 
		const SNATIVEID& sSkillID, 
		const STARGETID& sTargetID,
		WORD wStrikeNum, 
		bool bPrintFail = false, 
		bool bCheckOnlyAvailability = false,  
		bool bNotLearn = false );

	EMSKILLCHECK RandomSkillCheck ( 
		const SNATIVEID& sSkillID, 
		const STARGETID& sTargetID,
		WORD wStrikeNum, 
		bool bCheckOnlyAvailability = false );

	void AutoArmSwap();

public:
	// �˻��� ��Ʈ ���̵�, ��Ʈ ȿ���� �ޱ����� �ּ� ����, �ش� ������ ���� ����;
	BOOL GetSetitemEquipInfo(const DWORD dwSetID, OUT DWORD& dwEffectMinTermCount, OUT DWORD& dwEquipCount);

public:
	HRESULT Create( boost::shared_ptr<NavigationMesh> spNavi, SCHARDATA2 *pCharData2, D3DXVECTOR3* pvPos, DWORD dwGaeaID, LPDIRECT3DDEVICEQ pd3dDevice, DxLandMan* pLandMan );
	HRESULT SkinLoad( LPDIRECT3DDEVICEQ pd3dDevice, DxLandMan* pLandMan );
	HRESULT SkinLoadForTool( LPDIRECT3DDEVICEQ pd3dDevice, const char* szFile );
	void	DeleteSkin();

	// ���� �̵��ϴ� ���̴� �� �������� ���̴� �������� ����.
	void LandShredPtrRefSub();

public:
	void ResetData();

public:
	HRESULT FrameMove( float fTime, float fElapsedTime );
	HRESULT FrameMoveForTool( float fTime, float fElapsedTime );
	HRESULT Render( LPDIRECT3DDEVICEQ pd3dDevice, DxLandMan* pLandMan );
	HRESULT RenderSoftAlpha( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT RenderMaterialSkinEffect( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT RenderShadow( LPDIRECT3DDEVICEQ pd3dDevice, BOOL bWorldSystemOLD );
	HRESULT RenderShadow_SpotLight ( LPDIRECT3DDEVICEQ pd3dDevice, BOOL bUsedMaterialSystem, DWORD dwSlot );
	HRESULT RenderReflect( LPDIRECT3DDEVICEQ pd3dDevice, float fWaterHeight );
	void	EventCalculate();
	void	VietnamCalculate();

	// Ŭ���̾�Ʈ���� ��ü������ ������ ���� ����;
    //void	DisableDebuffSkillFact();
	// ���� ȿ�� ������Ʈ
	void	UpdateLandEffect();
	void	UpdateMapEffect();

    void    UpdateActivityItem();

	//Rnattendance
	void	ResetRnAttendaceQuest();

	void UpdateGeneralEffect();

public:
	// ������ ��ȿ���� ���� ��� ���⼭ ó���� �ش�;
	// ���� ���� �����Ͽ� ������ ���� ���� ���;
	void OffLine();

public:
	HRESULT InitDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT RestoreDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

    // Interface
public:
    void PrintMsgText(D3DCOLOR dwColor, const std::string& strText);
    void PrintConsoleText(const std::string& strText);
    void PrintMsgTextDlg(D3DCOLOR dwColor, const std::string& strText );
    void PrintConsoleTextDlg(const std::string& strText);
    void PrintMsgDlg(D3DCOLOR dwColor, const std::string& strText);

    // Callback
public:
    void CallbackHairColorCreate  ( UI::SPANNELGROUPINFOARRAY&                 OutPannelGroupInfoArray );
    void CallbackHairColorInit    ( UI::SPANNELGROUPINFOARRAY&                 OutPannelGroupInfoArray );
    void CallbackHairColorUpdate  ( const UI::SPANNELGROUPINFOARRAY&           InPannelGroupInfoArray  );
    void CallbackHairColorCommit  ( const UI::SPANNELGROUPINFOARRAY&           InPannelGroupInfoArray  );

    void CallbackCostumColorCreate( std::vector<UI::SCOLORPANNELGROUP_INFO>&   OutPannelGroupInfoArray );
    void CallbackCostumColorInit  ( std::vector<UI::SCOLORPANNELGROUP_INFO>&   OutPannelGroupInfoArray );
    void CallbackCostumColorUpdate( const UI::SPANNELGROUPINFOARRAY&           InPannelGroupInfoArray  );
    void CallbackCostumColorCommit( const UI::SPANNELGROUPINFOARRAY&           InPannelGroupInfoArray  );

    void CallbackVehicleColorCreate  ( std::vector<UI::SCOLORPANNELGROUP_INFO>&   OutPannelGroupInfoArray );
    void CallbackVehicleColorInit    ( std::vector<UI::SCOLORPANNELGROUP_INFO>&   OutPannelGroupInfoArray );
    void CallbackVehicleColorUpdate  ( const UI::SPANNELGROUPINFOARRAY&           InPannelGroupInfoArray  );
    void CallbackVehicleColorCommit  ( const UI::SPANNELGROUPINFOARRAY&           InPannelGroupInfoArray  );

	// ���󺯰� ī�� ���� Ÿ�Կ� ���� �ؽ�Ʈ ����;
	bool VehicleColorChangeTextInit( std::vector<UI::SCOLORPANNELGROUP_INFO>& OutPannelGroupInfoArray );

protected:
	// ���� ��ȯ�� ���� ���Ž� �Ź� ������Ʈ���� üũ�Ѵ�;
	// ������Ʈ���� üũ�����ʰ� ���� ��ȭ->�нú�üũ ��;
	// ������ ���ŵǵ����ϱ�����;
	void SummonStateVisibleUpdate(void) override;

public:
	//LuaTable GetSummonHPInfo(void);
	const char* GetSummonName(void);

	//////////////////////////////////////////////////////////////////////////
	//						AnimationMove
private:
	BOOL m_bFirstAnimationMove;

private:
	void SkinAniControl_AnimationMove();
	//						AnimationMove
	//////////////////////////////////////////////////////////////////////////

public:
    struct GLCharacterPacketFunc
    {
       (void)(GLCharacter::*Msgfunc)(NET_MSG_GENERIC* pMsg);
        GLCharacterPacketFunc()
        {
            Msgfunc = NULL;
        }
    };
    GLCharacterPacketFunc m_GLCharacterMsgFunc[NET_MSG_GCTRL_MAX];

    void InitPacketFunc();

    virtual void MsgProcess(NET_MSG_GENERIC* pMsg);

	void MsgPKComboBrd ( NET_MSG_GENERIC* pMsg );
	void MsgPKDIncBrd ( NET_MSG_GENERIC* pMsg );

	void MsgPKKIncBrd ( NET_MSG_GENERIC* pMsg );
	void MsgPKComboEndBrd ( NET_MSG_GENERIC* pMsg );
	void MsgAddStatsCmd  ( NET_MSG_GENERIC* pMsg );

    void MsgGetExpRecoveryNpcFb(NET_MSG_GENERIC* pMsg);
    void MsgEventFactInfo(NET_MSG_GENERIC* pMsg);
    void MsgEventFactEndBrd(NET_MSG_GENERIC* pMsg);
    void MsgEventFactBrd(NET_MSG_GENERIC* pMsg);
    void MsgQuestionItemFactEndBrd(NET_MSG_GENERIC* pMsg);
    void MsgIPEventBrd(NET_MSG_GENERIC* pMsg);
    void MsgQuestionItemFactBrd(NET_MSG_GENERIC* pMsg);
    void MsgInvenPostBoxOpenCardFb(NET_MSG_GENERIC* pMsg);
    void MsgPostBoxOpenFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenRenameFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenGenderChangeFb(NET_MSG_GENERIC* pMsg);
	void MsgInvenSchoolChangeFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenFaceChangeFb(NET_MSG_GENERIC* pMsg);
    void MsgUseBikeBoostCancelFb(NET_MSG_GENERIC* pMsg);
    void MsgUseBikeBoostFb(NET_MSG_GENERIC* pMsg);
    void MsgLearnBikeBoostFb(NET_MSG_GENERIC* pMsg);
    void MsgVehicleColorChangeFb(NET_MSG_GENERIC* pMsg);
    void MsgCostumeColorChangeFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenHairColorChangeFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenHairChangeFb(NET_MSG_GENERIC* pMsg);
	void MsgInvenItemDecomposeFb(NET_MSG_GENERIC* pMsg);
    void MsgPlayerKillingDel(NET_MSG_GENERIC* pMsg);
    void MsgPlayerKillingAdd(NET_MSG_GENERIC* pMsg);
    void MsgPMarketSearchItemResult(NET_MSG_GENERIC* pMsg);
    void MsgPMarketCloseBrd(NET_MSG_GENERIC* pMsg);
    void MsgPMarketItemUpdateBrd(NET_MSG_GENERIC* pMsg);
    void MsgPMarketBuyFb(NET_MSG_GENERIC* pMsg);
    void MsgPMarketOpenFb(NET_MSG_GENERIC* pMsg);
    void MsgPMarketRegItemFb(NET_MSG_GENERIC* pMsg);
    void MsgReviveFb(NET_MSG_GENERIC* pMsg);
    void MsgChargedItemDel(NET_MSG_GENERIC* pMsg);
    void MsgChargedItemGetFb(NET_MSG_GENERIC* pMsg);
    void MsgGetCharedItemFromDbFb(NET_MSG_GENERIC* pMsg);
    void MsgNpcItemTradeFb(NET_MSG_GENERIC* pMsg);
    void MsgPremiumState(NET_MSG_GENERIC* pMsg);
    void MsgFriendToFb(NET_MSG_GENERIC* pMsg);
    void MsgRebuildResult(NET_MSG_GENERIC* pMsg);
    void MsgGarbageResultFb(NET_MSG_GENERIC* pMsg);
    void MsgRebuildMoveItem(NET_MSG_GENERIC* pMsg);
    void MsgReBuildCostMoeny(NET_MSG_GENERIC* pMsg);
    void MsgReBuildInputMoney(NET_MSG_GENERIC* pMsg);
    void MsgUpdateLastCall(NET_MSG_GENERIC* pMsg);
    void MsgUpdateStartCall(NET_MSG_GENERIC* pMsg);
    void MsgSmsSendFb(NET_MSG_GENERIC* pMsg);
    void MsgSmsPhoneNumberFb(NET_MSG_GENERIC* pMsg);    
    void MsgVietnamGainType(NET_MSG_GENERIC* pMsg);
    void MsgVietnamGainMoney(NET_MSG_GENERIC* pMsg);
    void MsgVietnamGainExp(NET_MSG_GENERIC* pMsg);
    void MsgGmLimitEventBeginFb(NET_MSG_GENERIC* pMsg);
    void MsgGmLimitEventTimeReqFb(NET_MSG_GENERIC* pMsg);
    void MsgTradeProcess(NET_MSG_GENERIC* pMsg);
    void MsgActiveVehicleFb(NET_MSG_GENERIC* pMsg);
    void MsgLandEffectUpdateBrd(NET_MSG_GENERIC* pMsg);
    void MsgMapEffectUpdateBrd(NET_MSG_GENERIC* pMsg);
    void MsgGetVehicleFb(NET_MSG_GENERIC* pMsg);
    void MsgUngetVehicleFb(NET_MSG_GENERIC* pMsg);
    void MsgVehicleReqSlotExHoldFb(NET_MSG_GENERIC* pMsg);
    void MsgVehicleReqHoldToSlotFb(NET_MSG_GENERIC* pMsg);
    void MsgVehicleReqSlotToHoldFb(NET_MSG_GENERIC* pMsg);
    void MsgVehicleRemoveSlotItemFb(NET_MSG_GENERIC* pMsg);
    void MsgVehicleAccessoryDelete(NET_MSG_GENERIC* pMsg);
    void MsgVehicleUpdateClientBattery(NET_MSG_GENERIC* pMsg);
    void MsgVehicleReqGiveBatteryFb(NET_MSG_GENERIC* pMsg);
    void MsgVehicleReqItemInfoFb(NET_MSG_GENERIC* pMsg);
    void MsgWithVehicleFb(NET_MSG_GENERIC* pMsg);
    void MsgActiveWithVehicleFb(NET_MSG_GENERIC* pMsg);
    void MsgWithVehicleMoveMap(NET_MSG_GENERIC* pMsg);
    void MsgVehicleBrd(NET_MSG_GENERIC* pMsg);
    void MsgQboxOptionMember(NET_MSG_GENERIC* pMsg);
    void MsgUpdateTracingCharacter(NET_MSG_GENERIC* pMsg);
    void MsgReqAttendListFb(NET_MSG_GENERIC* pMsg);
    void MsgReqAttendanceFb(NET_MSG_GENERIC* pMsg);
    void MsgReqAttendRewardClt(NET_MSG_GENERIC* pMsg);
    void MsgNpcRecallFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenItemMixFb(NET_MSG_GENERIC* pMsg);
    void MsgReqGatheringFb(NET_MSG_GENERIC* pMsg);
    void MsgReqGatheringResult(NET_MSG_GENERIC* pMsg);
    void MsgUpdateCdmSafeTimeBrd(NET_MSG_GENERIC* pMsg);
	void MsgReqDialogueWindowOpenFb(NET_MSG_GENERIC* pMsg);
    void MsgReqNpcTalkBasicFb(NET_MSG_GENERIC* pMsg);
    void MsgChatPartyRecruitFb(NET_MSG_GENERIC* pMsg);
	void MsgChatFactionFb(NET_MSG_GENERIC* pMsg);
    void MsgChatAreaFb(NET_MSG_GENERIC* pMsg);
    void MsgChatLoudSpeakerFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenPremiumSetFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenStorageOpenFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenGarbageOpenFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenReModelOpenFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenPMarketSearchOpenFb(NET_MSG_GENERIC* pMsg);
	void MsgDominateMoveToFb(NET_MSG_GENERIC* pMsg);
	void MsgDominateMoveToFc(NET_MSG_GENERIC* pMsg);
	void MsgGoto(NET_MSG_GENERIC* pMsg);
    void MsgInvenInvenLineFb(NET_MSG_GENERIC* pMsg);
    void MsgStorageState(NET_MSG_GENERIC* pMsg);
    void MsgInvenStorageCardFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenCharCardFb(NET_MSG_GENERIC* pMsg);
    void MsgCureFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenResetSkstFb(NET_MSG_GENERIC* pMsg);
    void MsgVietnamAllInitTime(NET_MSG_GENERIC* pMsg);
    void MsgInvenVietnamExpGetFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenVietnamItemGetFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenVietnamItemGetNumUpdate(NET_MSG_GENERIC* pMsg);
    void MsgFireCrackerBrd(NET_MSG_GENERIC* pMsg);
    void MsgFireCrackerFb(NET_MSG_GENERIC* pMsg);
    void MsgCharResetFb(NET_MSG_GENERIC* pMsg);
	void MsgSelectTarget(NET_MSG_GENERIC* pMsg);
    void MsgRegenGateFb(NET_MSG_GENERIC* pMsg);
    void MsgPushPullBrd(NET_MSG_GENERIC* pMsg);
    void MsgInvenDelItemTimeLimit(NET_MSG_GENERIC* pMsg);
    void MsgInvenCleanserFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenDisguiseFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenDisjunctionFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenRandomBoxOpenFb(NET_MSG_GENERIC* pMsg);
	void MsgInvenLockBoxFB(NET_MSG_GENERIC* pMsg);
    void MsgInvenBoxOpenFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenBoxWrappingFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenBoxUnwrappingFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenItemUpdate(NET_MSG_GENERIC* pMsg);
    void MsgReqActionQFb(NET_MSG_GENERIC* pMsg);
    void MsgReqSkillQFb(NET_MSG_GENERIC* pMsg);
    void MsgStorageUpdateMoney(NET_MSG_GENERIC* pMsg);
    void MsgStorageDrugUpdate(NET_MSG_GENERIC* pMsg);    
	void MsgDefenseSkillActive(NET_MSG_GENERIC* pMsg);
	void MsgBeginSkill(NET_MSG_GENERIC* pMsg);
	void MsgEndSkill(NET_MSG_GENERIC* pMsg);	
	void MsgEndSkillBrd(NET_MSG_GENERIC* pMsg);
    void MsgInvenGrindingFb(NET_MSG_GENERIC* pMsg);
	void MsgInvenPeriodExtensionFb(NET_MSG_GENERIC* pMsg);
	void MsgInvenTLGrinding(NET_MSG_GENERIC* pMsg);
    void MsgStorageDelInsert(NET_MSG_GENERIC* pMsg);
    void MsgStorageItemUpdate(NET_MSG_GENERIC* pMsg);
    void MsgStorageDelete(NET_MSG_GENERIC* pMsg);
    void MsgStorageInsert(NET_MSG_GENERIC* pMsg);
    void MsgReqGetStorageItem(NET_MSG_GENERIC* pMsg);
    void MsgReqGetStorageFb(NET_MSG_GENERIC* pMsg);
	
	void MsgStorageSpecificItemCount(NET_MSG_GENERIC* pMsg);
	void MsgStorageSpecificItem(NET_MSG_GENERIC* pMsg);
    
	void MsgPutonDrugUpdate(NET_MSG_GENERIC* pMsg);
    void MsgCureStateBlowBrd(NET_MSG_GENERIC* pMsg);
    void MsgStateBlowBrd(NET_MSG_GENERIC* pMsg);
    void MsgSkillCancelBrd(NET_MSG_GENERIC* pMsg);
    void MsgSaveSkillFactBrd(NET_MSG_GENERIC* pMsg);
    void MsgSkillHoldRsBrd(NET_MSG_GENERIC* pMsg);
    void MsgSkillHoldBrd(NET_MSG_GENERIC* pMsg);
    void MsgSkillConsumeFb(NET_MSG_GENERIC* pMsg);
    void MsgUpdateSkillRevive(NET_MSG_GENERIC* pMsg);
    void MsgReqInvenLunchBoxFb(NET_MSG_GENERIC* pMsg);
    void MsgReqSkillHoldRsFb(NET_MSG_GENERIC* pMsg);
    void MsgReqSkillFb(NET_MSG_GENERIC* pMsg);
	void MsgSkillDealyAction(NET_MSG_GENERIC* pMsg);
    void MsgReqSkillUpFb(NET_MSG_GENERIC* pMsg);
	void MsgReqRnSkillUpComplete(NET_MSG_GENERIC* pMsg);
    void MsgReqLearnSkillFb(NET_MSG_GENERIC* pMsg);
    void MsgReqStatsUpFb(NET_MSG_GENERIC* pMsg);
	void MsgReqStatsUpExFb(NET_MSG_GENERIC* pMsg);
    void MsgReqLevelUpFb(NET_MSG_GENERIC* pMsg);
    void MsgPutonChange(NET_MSG_GENERIC* pMsg);
    void MsgPutonUpdate(NET_MSG_GENERIC* pMsg);
    void MsgPutonRelease(NET_MSG_GENERIC* pMsg);
    void MsgItemCoolTimeError(NET_MSG_GENERIC* pMsg);
    void MsgItemCoolTimeUpdate(NET_MSG_GENERIC* pMsg);
    void MsgReqVNInvenToInvenFb(NET_MSG_GENERIC* pMsg);
    void MsgInvenInsert(NET_MSG_GENERIC* pMsg);
    void MsgItemSortFail(NET_MSG_GENERIC* pMsg);
    void MsgReqItemSort(NET_MSG_GENERIC* pMsg);
	void MsgReqItemSortStart(NET_MSG_GENERIC* pMsg);
	void MsgReqItemSortData(NET_MSG_GENERIC* pMsg);
	void MsgReqItemSortEnd(NET_MSG_GENERIC* pMsg);
    void MsgReqHoldFb(NET_MSG_GENERIC* pMsg);
    void MsgUpdatePickupItem(NET_MSG_GENERIC* pMsg);
    void MsgPickUpMoney(NET_MSG_GENERIC* pMsg);
    void MsgUpdateStats(NET_MSG_GENERIC* pMsg);
    void MsgUpdateBright(NET_MSG_GENERIC* pMsg);
    void MsgUpdateSkillPoint(NET_MSG_GENERIC* pMsg);
    void MsgUpdateLp(NET_MSG_GENERIC* pMsg);
    void MsgUpdateCp(NET_MSG_GENERIC* pMsg);
    void MsgUpdateSp(NET_MSG_GENERIC* pMsg);
    void MsgUpdateMoney(NET_MSG_GENERIC* pMsg);
    void MsgUpdateExp(NET_MSG_GENERIC* pMsg);
    void MsgUpdateState(NET_MSG_GENERIC* pMsg);
	void MsgUpdateHireSummonState(NET_MSG_GENERIC* pMsg);
	void MsgUpdateCharacterSlotState(NET_MSG_GENERIC* pMsg);
    void MsgActionBrd(NET_MSG_GENERIC* pMsg);
    void MsgAttackDamage(NET_MSG_GENERIC* pMsg);
	void MsgAttackHeal(NET_MSG_GENERIC* pMsg);
    void MsgAttackAvoid(NET_MSG_GENERIC* pMsg);
    void MsgAttackBlocking(NET_MSG_GENERIC* pMsg);

    void MsgProcessAlliance(NET_MSG_GENERIC* nmg);
    void MsgProcessClub(NET_MSG_GENERIC* nmg);
    void MsgProcessQuest(NET_MSG_GENERIC* nmg);
    void MsgProcessConfront(NET_MSG_GENERIC* nmg);
    void MsgProcessConfront_FB(const GLMSG::SNETPC_REQ_CONFRONT_FB* pNetMsg);
    void MsgProcessConfront_FB_ReEnableLunch(const EMCONFT_TYPE emCONFT_TYPE);
    
    void MsgInvenDelInsert(NET_MSG_GENERIC* pMsg);
    void MsgInvenDelete(NET_MSG_GENERIC* pMsg);

    void MsgInvenDrugUpdate(NET_MSG_GENERIC* pMsg);
    void MsgSkillFactBrd(NET_MSG_GENERIC* pMsg);
    void MsgGmLimitEventEndFb(NET_MSG_GENERIC* pMsg);
    void MsgGmLimitEventApplyEnd(NET_MSG_GENERIC* pMsg);
    void MsgGmLimitEventApplyStart(NET_MSG_GENERIC* pMsg);
    void MsgVietnamTimeReqFb(NET_MSG_GENERIC* pMsg);
    void MsgChinaGainType(NET_MSG_GENERIC* pMsg);
    void MsgGmShowMeTheMoney(NET_MSG_GENERIC* pMsg);
    void MsgGmMoveToGateFb(NET_MSG_GENERIC* pMsg);	
    void MsgPMarketTitleFb(NET_MSG_GENERIC* pMsg);
    void MsgPMarketDisItemFb(NET_MSG_GENERIC* pMsg);
    void MsgRecoveryNpcFb(NET_MSG_GENERIC* pMsg);

	// ������ ��� ���� ������
	void MsgReqEnterNumChargeFb(NET_MSG_GENERIC* pMsg);

    // �����
    void MsgAddRebuyItem(NET_MSG_GENERIC* pMsg);
    void MsgBuyRebuyItem(NET_MSG_GENERIC* pMsg);
    void MsgCheckRebuySupplies(NET_MSG_GENERIC* pMsg);
	
	//���λ����˻�
	void MsgPrivateMarketSearch(NET_MSG_GENERIC* pMsg);

	//Rnattendance Rn�⼮��
	void MsgProcessRnattendance(NET_MSG_GENERIC* nmg);
	
	//! �⼮�� ����Ʈ ����.
	HRESULT ReqAttendanceQuestStart( DWORD dwQUESTID );

	//!  ��ų ��Ÿ�� ( ����ȭ )
	void MsgSkillDelay(NET_MSG_GENERIC* pMsg);
	void MsgExchangeItemNpc(NET_MSG_GENERIC* pMsg);

	void MsgCostumeStatsFb(NET_MSG_GENERIC* pMsg);
	void MsgDrugFb(NET_MSG_GENERIC* pMsg);

	void MsgCostumeStatsReleaseFb(NET_MSG_GENERIC* pMsg);

	void MsgJumpingLevelUpFb(NET_MSG_GENERIC* pMsg);

	// ����ġ ���� / ĸ��
	void MsgExpCompressorAns(NET_MSG_GENERIC* pMsg);
	void MsgExpCapsuleAns(NET_MSG_GENERIC* pMsg);

	void MsgForceresetSkillStatFC ( NET_MSG_GENERIC* pMsg );

	// ������ ��ų ����;
	void MsgInvenChangeItemSkillFB ( NET_MSG_GENERIC* _pNetMsg );

	// ������ �⺻�ɷ�ġ ����;
	void MsgInvenChangeBasicStatsFB ( NET_MSG_GENERIC* _pNetMsg );

	// ������ ����ɷ�ġ ����;
	void MsgInvenChangeAddStatsFB ( NET_MSG_GENERIC* _pNetMsg );
	void MsgTutorialAck ( NET_MSG_GENERIC* _pNetMsg );
	void MsgCharLockFB ( NET_MSG_GENERIC* _pNetMsg );
	void MsgGetRankingsFB ( NET_MSG_GENERIC* _pNetMsg );
	void MsgCombatReport ( NET_MSG_GENERIC* _pNetMsg );
	void MsgCombatReportSkill ( NET_MSG_GENERIC* _pNetMsg );
};

BOOL SelectSkillTarget(GLGaeaClient* const pGaeaClient, const SNATIVEID& sSkillID, const WORD wSkillLevel, const STARGETID& sTargetID, GLCharacter* pChar, const D3DXVECTOR3& vPos, SKILLTARGETDATA& sSkillTarget);

#endif // _GL_CHARACTER_H_
