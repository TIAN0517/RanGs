#ifndef _GL_LOGIN_MSG_H_
#define _GL_LOGIN_MSG_H_

#pragma once

#include "../s_NetGlobal.h"
#include "../WorldBattle/WorldBattleDefine.h"
#include "../Country/GLCountryDefine.h"

#define RSA_ADD	4

//
//Taiwan, Hongkong, Internal
//
struct NET_REGISTER_DATA : public NET_MSG_GENERIC
{
    int  m_nChannel;
    CHAR m_szRandomPassword[USR_RAND_PASS_LENGTH+1];
    CHAR m_szPassword[USR_PASS_LENGTH+1];
    CHAR m_szUserid  [USR_ID_LENGTH+1];
    CHAR m_szUserEmail  [USR_EMAIL_LENGTH+1];
    CHAR m_szPassword2[USR_PASS_LENGTH+1];


    NET_REGISTER_DATA(
        int  nChannel,
        const CHAR* szRandomPassword,
        const CHAR* szPassword,
        const CHAR* szPassword2,
        const CHAR* szUserid,
        const CHAR* szUserEmail)
        : m_nChannel(nChannel)
    {		
        dwSize = sizeof(NET_REGISTER_DATA);
        nType  = NET_MSG_REGISTER;

        StringCchCopyA(m_szRandomPassword, USR_RAND_PASS_LENGTH+1, szRandomPassword);
        StringCchCopyA(m_szPassword, USR_PASS_LENGTH+1, szPassword);
        StringCchCopyA(m_szPassword2, USR_PASS_LENGTH+1, szPassword2);
        StringCchCopyA(m_szUserid, USR_ID_LENGTH+1, szUserid);
        StringCchCopyA(m_szUserEmail, USR_ID_LENGTH+1, szUserEmail);

        MIN_STATIC_ASSERT(sizeof(NET_REGISTER_DATA)<=NET_DATA_BUFSIZE);
    }
};
typedef NET_REGISTER_DATA* LPNET_REGISTER_DATA;

/*	NOT USED
struct NET_REGISTER_DATA2 : public NET_MSG_GENERIC
{    
    CHAR			szUserid[USR_ID_LENGTH+1];
    CHAR			szPassword[USR_PASS_LENGTH+1];
    CHAR			szIp[MAX_IP_LENGTH+1];
    INT				nClient;

    NET_REGISTER_DATA2()
    {
        nType = EMNET_MSG(0);
        dwSize = sizeof(NET_REGISTER_DATA2);
        memset(szUserid, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
        memset(szPassword, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
        memset(szIp, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));
        nClient = 0;
    }
};
typedef  NET_REGISTER_DATA2* LPNET_REGISTER_DATA2;
*/

struct NET_REGISTER_FEEDBACK_DATA : public NET_MSG_GENERIC
{    
    EM_REGISTER_FB_SUB m_Result;    
	BOOL	m_bJumping;

    NET_REGISTER_FEEDBACK_DATA()
    {
        dwSize = sizeof(NET_REGISTER_FEEDBACK_DATA);
        nType  = EMNET_MSG(0);
        m_Result       = EM_REGISTER_FB_SUB_FAIL;
		m_bJumping = FALSE;
        MIN_STATIC_ASSERT(sizeof(NET_REGISTER_FEEDBACK_DATA)<=NET_DATA_BUFSIZE);
    }
};
typedef NET_REGISTER_FEEDBACK_DATA* LPNET_REGISTER_FEEDBACK_DATA;


struct NET_REGISTER_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{    
    CHAR			szUserid[USR_ID_LENGTH+1];
    CHAR			szIp[MAX_IP_LENGTH+1];
    INT				m_ClientSlot;
    EM_REGISTER_FB_SUB	m_emResult;
    INT				nUserNum;

    NET_REGISTER_FEEDBACK_DATA2()
        : m_ClientSlot(0)
        , m_emResult(EM_REGISTER_FB_SUB_OK)
        , nUserNum(0)
    {
        dwSize = sizeof(NET_REGISTER_FEEDBACK_DATA2);
        nType = NET_MSG_REGISTER_FEEDBACK_DATA2;
        memset(szUserid, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
        memset(szIp, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));        
        MIN_STATIC_ASSERT(sizeof(NET_REGISTER_FEEDBACK_DATA2)<=NET_DATA_BUFSIZE);
    }

    void SetUserId(const char* _UserID)
    {
        if (_UserID)
            StringCchCopy(szUserid, USR_ID_LENGTH+1, _UserID);
    }

    void SetIp(const char* _Ip)
    {
        if (_Ip)
            StringCchCopy(szIp, MAX_IP_LENGTH+1, _Ip);
    }
};
typedef NET_REGISTER_FEEDBACK_DATA2* LPNET_REGISTER_FEEDBACK_DATA2;


// Agent Server -> Field Server 로그인 성공후 결과값 전송
// Field Server 에서 세팅함
struct NET_REGISTER_FEEDBACK_DATA3 : public NET_MSG_GENERIC
{
    char szUserid[USR_ID_LENGTH+1];
    INT nUserNum;

    NET_REGISTER_FEEDBACK_DATA3(DWORD UserDbNum, const char* UserId)
        : nUserNum(UserDbNum)
    {
        nType = NET_MSG_REGISTER_FB_SUB_OK_FIELD;
        dwSize = sizeof(NET_REGISTER_FEEDBACK_DATA3);		
        if (UserId)
            StringCchCopy(szUserid, USR_ID_LENGTH+1, UserId);
        else
            memset(szUserid, 0, USR_ID_LENGTH+1);
        MIN_STATIC_ASSERT(sizeof(NET_REGISTER_FEEDBACK_DATA3)<=NET_DATA_BUFSIZE);
    }
};
//typedef NET_REGISTER_FEEDBACK_DATA3* LPNET_REGISTER_FEEDBACK_DATA3;


//
//Taiwan, Hongkong, Internal
//
struct NET_LOGIN_DATA : public NET_MSG_GENERIC
{
    int  m_nChannel;
    CHAR m_szRandomPassword[USR_RAND_PASS_LENGTH+1];
    CHAR m_szPassword[USR_PASS_LENGTH+1];
    CHAR m_szUserid  [USR_ID_LENGTH+1];
    CHAR m_szEnCrypt[ENCRYPT_KEY+1];
    int m_LauncherVersion;
    int m_GameVersion;

    NET_LOGIN_DATA(
        int  nChannel,
        const CHAR* szRandomPassword,
        const CHAR* szPassword,
        const CHAR* szUserid,
        const CHAR* szEnCrypt,
        int LauncherVersion,
        int GameVersion)
        : m_nChannel(nChannel)
        , m_LauncherVersion(LauncherVersion)
        , m_GameVersion(GameVersion)
    {		
        dwSize = sizeof(NET_LOGIN_DATA);
        nType  = NET_MSG_LOGIN_2;

        StringCchCopyA(m_szRandomPassword, USR_RAND_PASS_LENGTH+1, szRandomPassword);
        StringCchCopyA(m_szPassword, USR_PASS_LENGTH+1, szPassword);
        StringCchCopyA(m_szUserid, USR_ID_LENGTH+1, szUserid);
        StringCchCopyA(m_szEnCrypt, ENCRYPT_KEY+1, szEnCrypt);

        MIN_STATIC_ASSERT(sizeof(NET_LOGIN_DATA)<=NET_DATA_BUFSIZE);
    }
};
typedef NET_LOGIN_DATA* LPNET_LOGIN_DATA;

/*	NOT USED
struct NET_LOGIN_DATA2 : public NET_MSG_GENERIC
{    
    CHAR			szUserid[USR_ID_LENGTH+1];
    CHAR			szPassword[USR_PASS_LENGTH+1];
    CHAR			szIp[MAX_IP_LENGTH+1];
    INT				nClient;

    NET_LOGIN_DATA2()
    {
        nType = EMNET_MSG(0);
        dwSize = sizeof(NET_LOGIN_DATA2);
        memset(szUserid, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
        memset(szPassword, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
        memset(szIp, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));
        nClient = 0;
    }
};
typedef  NET_LOGIN_DATA2* LPNET_LOGIN_DATA2;
*/


//
//Indonesia
//  mjeon.indonesia
//
struct IDN_NET_LOGIN_DATA : public NET_MSG_GENERIC
{	
    int		m_nChannel;
    char	m_szUserid[USR_ID_LENGTH+1];
	char	m_szPassword[MD5_MAX_LENGTH+1];
    

    IDN_NET_LOGIN_DATA(int  nChannel, const char* szPassword, const char* szUserid)		
		:m_nChannel(nChannel)		
    {
        dwSize = sizeof(IDN_NET_LOGIN_DATA);
        nType  = IDN_NET_MSG_LOGIN;

        StringCchCopyA(m_szUserid, USR_ID_LENGTH+1, szUserid);
		StringCchCopyA(m_szPassword, MD5_MAX_LENGTH+1, szPassword);

        MIN_STATIC_ASSERT(sizeof(IDN_NET_LOGIN_DATA)<=NET_DATA_BUFSIZE);
    }
};

//
// Daum Game
//
struct DAUM_NET_LOGIN_DATA : public NET_MSG_GENERIC
{
    int nChannel;
    TCHAR szUUID[sc::string::UUID_LENGTH];	

    DAUM_NET_LOGIN_DATA()
    {
        dwSize = sizeof(DAUM_NET_LOGIN_DATA);
        nType = DAUM_NET_MSG_LOGIN;
        nChannel = 0;
        memset(szUUID, 0, sizeof(TCHAR) * (sc::string::UUID_LENGTH));
        MIN_STATIC_ASSERT(sizeof(DAUM_NET_LOGIN_DATA)<=NET_DATA_BUFSIZE);
    }
};
typedef DAUM_NET_LOGIN_DATA* LPDAUM_NET_LOGIN_DATA;

/*	NOT USED

struct DAUM_NET_LOGIN_DATA2 : public NET_MSG_GENERIC
{    
    CHAR			szDaumGID[DAUM_MAX_GID_LENGTH+1];
    CHAR			szDaumUID[DAUM_MAX_UID_LENGTH+1];
    CHAR			szDaumSSNHEAD[DAUM_MAX_SSNHEAD_LENGTH+1];
    CHAR			szDaumSEX[DAUM_MAX_SEX_LENGTH+1];
    CHAR			szIp[MAX_IP_LENGTH+1];
    INT				nClient;

    DAUM_NET_LOGIN_DATA2()
    {
        nType = EMNET_MSG(0);
        dwSize = sizeof(DAUM_NET_LOGIN_DATA2);
        memset(szDaumGID, 0, sizeof(CHAR) * (DAUM_MAX_GID_LENGTH+1));
        memset(szDaumUID, 0, sizeof(CHAR) * (DAUM_MAX_UID_LENGTH+1));
        memset(szDaumSSNHEAD, 0, sizeof(CHAR) * (DAUM_MAX_SSNHEAD_LENGTH+1));
        memset(szDaumSEX, 0, sizeof(CHAR) * (DAUM_MAX_SEX_LENGTH+1));
        memset(szIp, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));
        nClient = 0;
    }
};
typedef  DAUM_NET_LOGIN_DATA2* LPDAUMNET_LOGIN_DATA2;
*/

struct DAUM_NET_PASSCHECK_DATA : public NET_MSG_GENERIC
{
    TCHAR	szDaumGID[DAUM_MAX_GID_LENGTH+1];
    TCHAR	szUserPass[DAUM_USERPASS+1];
    INT		nCheckFlag; // 패스워드 갱신을 위해서 추가

    DAUM_NET_PASSCHECK_DATA()
        : nCheckFlag(0)
    {
        nType = DAUM_NET_MSG_PASSCHECK;
        dwSize = sizeof(DAUM_NET_PASSCHECK_DATA);
        memset(szDaumGID, 0, sizeof(szDaumGID));
        memset(szUserPass, 0, sizeof(szUserPass));
        MIN_STATIC_ASSERT(sizeof(DAUM_NET_PASSCHECK_DATA)<=NET_DATA_BUFSIZE);
    }
};
typedef DAUM_NET_PASSCHECK_DATA* LPDAUM_NET_PASSCHECK_DATA;


struct DAUM_NET_PASSCHECK_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{    
    TCHAR			szDaumGID[DAUM_MAX_GID_LENGTH+1];
    INT				nClient;
    USHORT			nResult;

    DAUM_NET_PASSCHECK_FEEDBACK_DATA2()
        : nClient(0)
        , nResult(0)
    {
        //memset(this, 0, sizeof( DAUM_NET_PASSCHECK_FEEDBACK_DATA2 ));
        nType  = EMNET_MSG(0);
        dwSize = sizeof(DAUM_NET_PASSCHECK_FEEDBACK_DATA2);
        memset(szDaumGID, 0, sizeof(szDaumGID));
        MIN_STATIC_ASSERT(sizeof(DAUM_NET_PASSCHECK_FEEDBACK_DATA2)<=NET_DATA_BUFSIZE);
    }
};
typedef DAUM_NET_PASSCHECK_FEEDBACK_DATA2* LPDAUM_NET_PASSCHECK_FEEDBACK_DATA2;


struct DAUM_NET_LOGIN_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{    
    char			szDaumGID[DAUM_MAX_GID_LENGTH+1];
    //	TCHAR			szDaumUID[DAUM_MAX_UID_LENGTH+1];
    TCHAR			szUUID[sc::string::UUID_LENGTH]; 
    TCHAR			szIp[MAX_IP_LENGTH+1];
    INT				nClient;
    EM_LOGIN_FB_SUB	m_emResult;
    INT				nUserNum;
    INT				nUserType;
	BOOL			bExtreme;	// 극강부 생성 가능 여부;
	BOOL			bJumping;
	BOOL			bActor;		// 변술부 생성 가능 여부;
    INT				nCheckFlag; // Daum 패스워드를 받기위한 Check Flag 값
    USHORT			uChaRemain;
    USHORT			uChaTestRemain;
    __time64_t      tPremiumTime;
    __time64_t      tChatBlockTime;
    WORD            m_wAge; //! 나이 (주민등록번호 기준)

	Country::SCOUNTRY_INFO sCountryInfo;

	INT ItemTransferResult;

    DAUM_NET_LOGIN_FEEDBACK_DATA2()
    {	
        dwSize = sizeof(DAUM_NET_LOGIN_FEEDBACK_DATA2);
        nType  = EMNET_MSG(0);
        memset(szDaumGID, 0, sizeof(szDaumGID));
        memset(szUUID, 0, sizeof(TCHAR) * (sc::string::UUID_LENGTH));
        // memset(szDaumUID, 0, sizeof(szDaumUID));
        memset(szIp, 0, sizeof(szIp));		
        nClient        = 0;
        m_emResult     = EM_LOGIN_FB_SUB_OK;
        nUserNum       = 0;
        nUserType      = 0;
		bExtreme	   = FALSE;
		bJumping = FALSE;
		bActor = FALSE;
        uChaRemain     = 0;
        uChaTestRemain = 0;
        tPremiumTime   = 0;
        tChatBlockTime = 0;
        nCheckFlag	   = 0;
        m_wAge         = 0;
		ItemTransferResult = -1;
        MIN_STATIC_ASSERT(sizeof(DAUM_NET_LOGIN_FEEDBACK_DATA2)<=NET_DATA_BUFSIZE);
    }

    //! 나이 (주민등록번호 기준)
    void SetAge(WORD Age) { m_wAge = Age; }    
    WORD GetAge() const { return m_wAge; }

    void SetDaumGid(const char* szName)
    {
        if (szName)
            StringCchCopy(szDaumGID, DAUM_MAX_GID_LENGTH+1, szName);
    }

    void SetIp(const char* IpAddress)
    {
        if (IpAddress)
            StringCchCopy(szIp, MAX_IP_LENGTH+1, IpAddress);
    }

    void SetUUID(const char* Uuid)
    {
        if (Uuid)
            StringCchCopy(szUUID, sc::string::UUID_LENGTH, Uuid); 
    }
};
typedef DAUM_NET_LOGIN_FEEDBACK_DATA2* LPDAUM_NET_LOGIN_FEEDBACK_DATA2;

//
//GS
//
struct GS_NET_LOGIN_DATA : public NET_MSG_GENERIC
{
    int             nChannel;
    CHAR			szPassword[GS_USER_ID+1];
    CHAR			szUserid  [GS_USER_PASS+1];
	CHAR			szEnCrypt[ENCRYPT_KEY+1];
	USHORT			uUserPoints; //userpanel
	USHORT			uBattlePoints;
	DWORD			dwGameTime;
	TCHAR			szUserEmail[USR_EMAIL_LENGTH+RSA_ADD+1];	

    GS_NET_LOGIN_DATA()
    {		
        dwSize  = sizeof(GS_NET_LOGIN_DATA);
        nType  = GS_NET_MSG_LOGIN;
        nChannel   = 0;
        SecureZeroMemory(szUserid, sizeof(CHAR) * (GS_USER_ID+1));
        SecureZeroMemory(szPassword, sizeof(CHAR) * (GS_USER_PASS+1));
        SecureZeroMemory(szEnCrypt, sizeof(CHAR) * (ENCRYPT_KEY+1));
		uUserPoints = 0; //userpanel
		uBattlePoints = 0;
		dwGameTime = 0;
		memset(szUserEmail, 0, sizeof(char)*(USR_EMAIL_LENGTH+RSA_ADD+1));
		MIN_STATIC_ASSERT(sizeof(GS_NET_LOGIN_DATA)<=NET_DATA_BUFSIZE);
    }
};
typedef GS_NET_LOGIN_DATA* LPGS_NET_LOGIN_DATA;

struct GS_NET_WB_LOGIN_DATA : public NET_MSG_GENERIC
{
    int             nChannel;
    CHAR			szUserid  [GS_USER_PASS+1];

    GS_NET_WB_LOGIN_DATA()
    {		
        dwSize  = sizeof(GS_NET_WB_LOGIN_DATA);
        nType  = GS_NET_MSG_WB_LOGIN;
        nChannel   = 0;
        SecureZeroMemory(szUserid, sizeof(CHAR) * (GS_USER_ID+1));
        MIN_STATIC_ASSERT(sizeof(GS_NET_WB_LOGIN_DATA)<=NET_DATA_BUFSIZE);
    }
};

struct GS_NET_WB_LOGIN_DATA_AW : public NET_MSG_GENERIC
{
    int             nChannel;
    CHAR			szUserid  [GS_USER_PASS+1];
	DWORD			nClientID;

    GS_NET_WB_LOGIN_DATA_AW()
    {		
        dwSize  = sizeof(GS_NET_WB_LOGIN_DATA_AW);
        nType  = GS_NET_MSG_WB_LOGIN_AW;
        nChannel   = 0;
		nClientID = 0;
        SecureZeroMemory(szUserid, sizeof(CHAR) * (GS_USER_ID+1));
        MIN_STATIC_ASSERT(sizeof(GS_NET_WB_LOGIN_DATA_AW)<=NET_DATA_BUFSIZE);
    }
};

struct GS_NET_WB_LOGIN_DATA_WA : public NET_MSG_GENERIC
{
    int             nChannel;
    CHAR			szUserid  [GS_USER_PASS+1];
	DWORD			nClientID;
	bool			bLoginWB;

    GS_NET_WB_LOGIN_DATA_WA()
    {		
        dwSize  = sizeof(GS_NET_WB_LOGIN_DATA_WA);
        nType  = GS_NET_MSG_WB_LOGIN_WA;
        nChannel   = 0;
		nClientID = 0;
		bLoginWB = false;
        SecureZeroMemory(szUserid, sizeof(CHAR) * (GS_USER_ID+1));
        MIN_STATIC_ASSERT(sizeof(GS_NET_WB_LOGIN_DATA_WA)<=NET_DATA_BUFSIZE);
    }
};

struct GS_NET_WB_LOGIN_DATA_FB : public NET_MSG_GENERIC
{
    int             nChannel;
    CHAR			szUserid  [GS_USER_PASS+1];
	bool			bLoginWB;
	bool			bWBOff;

    GS_NET_WB_LOGIN_DATA_FB()
    {		
        dwSize  = sizeof(GS_NET_WB_LOGIN_DATA_FB);
        nType  = GS_NET_MSG_WB_LOGIN_FB;
        nChannel   = 0;
		bLoginWB = false;
		bWBOff = false;
        SecureZeroMemory(szUserid, sizeof(CHAR) * (GS_USER_ID+1));
        MIN_STATIC_ASSERT(sizeof(GS_NET_WB_LOGIN_DATA_FB)<=NET_DATA_BUFSIZE);
    }
};


struct GS_NET_LOGIN_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{    
    CHAR			szUserid[GS_USER_ID+1];
    CHAR			szIp[MAX_IP_LENGTH+1];
    INT				nClient;
    EM_LOGIN_FB_SUB	m_emResult;
    INT				nUserNum;
    INT				nUserType;
	BOOL			bExtreme;
	BOOL			bJumping;
	BOOL			bActor;
    USHORT			uChaRemain;
    USHORT			uChaTestRemain;
    __time64_t      tPremiumTime;
	__time64_t      tChatBlockTime;
	USHORT			uUserPoints; //userpanel
	USHORT			uBattlePoints;
	DWORD			dwGameTime;
	TCHAR			szUserEmail[USR_EMAIL_LENGTH+RSA_ADD+1];	
	EMIP_BONUS_CLASS emIPBonus;			// 말레이시아 PC방 이벤트

	Country::SCOUNTRY_INFO sCountryInfo;

	INT ItemTransferResult;

    GS_NET_LOGIN_FEEDBACK_DATA2()
    {
        dwSize = sizeof(GS_NET_LOGIN_FEEDBACK_DATA2);
        memset (szUserid, 0, sizeof(CHAR) * (GS_USER_ID+1));
        memset (szIp, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));
        nClient        = 0;
        m_emResult     = EM_LOGIN_FB_SUB_OK;
        nUserNum       = 0;
        nUserType      = 0;
		bExtreme	   = FALSE;
		bJumping = FALSE;
		bActor = FALSE;
        uChaRemain     = 0;
        uChaTestRemain = 0;
        tPremiumTime   = 0;
        tChatBlockTime = 0;
		ItemTransferResult = -1;

		uUserPoints = 0; //userpanel
		uBattlePoints = 0;
		dwGameTime = 0;
		emIPBonus	   = EMIP_BONUS_NONE;
		memset(szUserEmail, 0, sizeof(char)*(USR_EMAIL_LENGTH+RSA_ADD+1));
        MIN_STATIC_ASSERT(sizeof(GS_NET_LOGIN_FEEDBACK_DATA2)<=NET_DATA_BUFSIZE);
    }
};
typedef GS_NET_LOGIN_FEEDBACK_DATA2* LPGSNET_LOGIN_FEEDBACK_DATA2;


struct CHINA_NET_RESETPASS_DATA : public NET_MSG_GENERIC
{
	int             nChannel;
	TCHAR           szRandomPassword[USR_RAND_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szPassword[USR_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szUserid[USR_ID_LENGTH+RSA_ADD+1];

	CHINA_NET_RESETPASS_DATA()
	{
		dwSize  = sizeof(CHINA_NET_RESETPASS_DATA);
		nType  = CHINA_NET_MSG_RESETPASS;
		nChannel   = 0;
		SecureZeroMemory( szUserid,         sizeof(TCHAR) * (USR_ID_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szPassword,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szRandomPassword, sizeof(TCHAR) * (USR_RAND_PASS_LENGTH+RSA_ADD+1) );
	}
};
typedef CHINA_NET_RESETPASS_DATA* LPCHINA_NET_RESETPASS_DATA;

struct NET_RESETPASS_FEEDBACK_DATA : public NET_MSG_GENERIC
{
	CHAR			szUserid[USR_ID_LENGTH+1];
	CHAR			szIp[MAX_IP_LENGTH+1];
	INT				nClient;
	USHORT			nResult;
	INT				nUserNum;
	
	NET_RESETPASS_FEEDBACK_DATA()
	{
		dwSize = sizeof(NET_RESETPASS_FEEDBACK_DATA);
		memset (szUserid, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
		memset (szIp, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));
		nClient        = 0;
		nResult        = 0;
		nUserNum       = 0;
	}
};
typedef NET_RESETPASS_FEEDBACK_DATA* LPNET_RESET_FEEDBACK_DATA;

struct CHINA_NET_CP_DATA : public NET_MSG_GENERIC
{
	TCHAR           szCaptcha[USR_RAND_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szOPassword[USR_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szPassword[USR_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szPassword2[USR_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szUserid[USR_ID_LENGTH+RSA_ADD+1];
	TCHAR			szUserEmail[USR_EMAIL_LENGTH+RSA_ADD+1];

	CHINA_NET_CP_DATA()
	{
		dwSize  = sizeof(CHINA_NET_CP_DATA);
		nType  = CHINA_NET_MSG_PASS;
		SecureZeroMemory( szUserid,         sizeof(TCHAR) * (USR_ID_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szOPassword,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szPassword,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szPassword2,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szUserEmail,       sizeof(TCHAR) * (USR_EMAIL_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szCaptcha, sizeof(TCHAR) * (USR_RAND_PASS_LENGTH+RSA_ADD+1) );
	}
};
typedef CHINA_NET_CP_DATA* LPCHINA_NET_CP_DATA;

struct NET_CP_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{
	CHAR			szUserpass[USR_PASS_LENGTH+1];
	CHAR			szUseropass[USR_PASS_LENGTH+1];
	CHAR			szUserpass2[USR_PASS_LENGTH+1];
	CHAR			szUserEmail[USR_EMAIL_LENGTH+1];
	CHAR			szCaptcha[USR_RAND_PASS_LENGTH+1];
	CHAR			szUserid[USR_ID_LENGTH+1];
	INT				nClient;
	USHORT			nResult;
	
	NET_CP_FEEDBACK_DATA2()
	{
		dwSize = sizeof(NET_CP_FEEDBACK_DATA2);
		memset (szUserid, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
		memset (szUserpass, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset (szUseropass, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset (szUserpass2, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset (szUserEmail, 0, sizeof(CHAR) * (USR_EMAIL_LENGTH+1));
		memset (szCaptcha, 0, sizeof(CHAR) * (USR_RAND_PASS_LENGTH+1));
		nClient        = 0;
		nResult        = 0;
	}
};

typedef NET_CP_FEEDBACK_DATA2* LPNET_CP_FEEDBACK_DATA;

struct CHINA_NET_CPN_DATA : public NET_MSG_GENERIC
{
	TCHAR           szCaptcha[USR_RAND_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szPassword[USR_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szPassword2[USR_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szOPassword2[USR_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szUserid[USR_ID_LENGTH+RSA_ADD+1];
	TCHAR			szUserEmail[USR_EMAIL_LENGTH+RSA_ADD+1];

	CHINA_NET_CPN_DATA()
	{
		dwSize  = sizeof(CHINA_NET_CPN_DATA);
		nType  = CHINA_NET_MSG_PIN;
		SecureZeroMemory( szUserid,         sizeof(TCHAR) * (USR_ID_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szPassword,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szPassword2,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szOPassword2,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szUserEmail,       sizeof(TCHAR) * (USR_EMAIL_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szCaptcha, sizeof(TCHAR) * (USR_RAND_PASS_LENGTH+RSA_ADD+1) );
	}
};
typedef CHINA_NET_CPN_DATA* LPCHINA_NET_CPN_DATA;

struct NET_CPN_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{
	CHAR			szUserid[USR_ID_LENGTH+1];
	CHAR			szUserpass[USR_PASS_LENGTH+1];
	CHAR			szUserpass2[USR_PASS_LENGTH+1];
	CHAR			szUserOpass2[USR_PASS_LENGTH+1];
	CHAR			szUserEmail[USR_EMAIL_LENGTH+1];
	CHAR			szCaptcha[USR_RAND_PASS_LENGTH+1];
	INT				nClient;
	USHORT			nResult;
	
	NET_CPN_FEEDBACK_DATA2()
	{
		dwSize = sizeof(NET_CPN_FEEDBACK_DATA2);
		memset (szUserid, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
		memset (szUserpass, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset (szUserpass2, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset (szUserOpass2, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset (szUserEmail, 0, sizeof(CHAR) * (USR_EMAIL_LENGTH+1));
		memset (szCaptcha, 0, sizeof(CHAR) * (USR_RAND_PASS_LENGTH+1));
		nClient        = 0;
		nResult        = 0;
	}
};

typedef NET_CPN_FEEDBACK_DATA2* LPNET_CPN_FEEDBACK_DATA;

struct CHINA_NET_RPS_DATA : public NET_MSG_GENERIC
{
	TCHAR           szCaptcha[USR_RAND_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szPassword2[USR_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szUserid[USR_ID_LENGTH+RSA_ADD+1];

	CHINA_NET_RPS_DATA()
	{
		dwSize  = sizeof(CHINA_NET_RPS_DATA);
		nType  = CHINA_NET_MSG_RESETPASS;
		SecureZeroMemory( szUserid,         sizeof(TCHAR) * (USR_ID_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szPassword2,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szCaptcha, sizeof(TCHAR) * (USR_RAND_PASS_LENGTH+RSA_ADD+1) );
	}
};
typedef CHINA_NET_RPS_DATA* LPCHINA_NET_RPS_DATA;

struct NET_RPS_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{
	CHAR			szUserid[USR_ID_LENGTH+1];
	CHAR			szUserpass2[USR_PASS_LENGTH+1];
	CHAR			szUserEmail[USR_EMAIL_LENGTH+1];
	CHAR			szCaptcha[USR_RAND_PASS_LENGTH+1];
	INT				nClient;
	USHORT			nResult;
	
	NET_RPS_FEEDBACK_DATA2()
	{
		dwSize = sizeof(NET_RPS_FEEDBACK_DATA2);
		memset (szUserid, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
		memset (szUserpass2, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset (szCaptcha, 0, sizeof(CHAR) * (USR_RAND_PASS_LENGTH+1));
		nClient        = 0;
		nResult        = 0;
	}
};

typedef NET_RPS_FEEDBACK_DATA2* LPNET_RPS_FEEDBACK_DATA;


//TODO


struct CHINA_NET_RPS2_DATA : public NET_MSG_GENERIC
{
	TCHAR           szCaptcha[USR_RAND_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szPassword[USR_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szPassword2[USR_PASS_LENGTH+RSA_ADD+1];

	CHINA_NET_RPS2_DATA()
	{
		dwSize  = sizeof(CHINA_NET_RPS2_DATA);
		nType  = CHINA_NET_MSG_RESETPASS2;
		SecureZeroMemory( szPassword,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szPassword2,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szCaptcha, sizeof(TCHAR) * (USR_RAND_PASS_LENGTH+RSA_ADD+1) );
	}
};
typedef CHINA_NET_RPS2_DATA* LPCHINA_NET_RPS2_DATA;

struct NET_RPS2_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{
	CHAR			szUserid[USR_ID_LENGTH+1];
	CHAR			szUserpass[USR_PASS_LENGTH+1];
	CHAR			szUserpass2[USR_PASS_LENGTH+1];
	CHAR			szCaptcha[USR_RAND_PASS_LENGTH+1];
	INT				nClient;
	USHORT			nResult;
	
	NET_RPS2_FEEDBACK_DATA2()
	{
		dwSize = sizeof(NET_RPS2_FEEDBACK_DATA2);
		memset (szUserid, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
		memset (szUserpass, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset (szUserpass2, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset (szCaptcha, 0, sizeof(CHAR) * (USR_RAND_PASS_LENGTH+1));
		nClient        = 0;
		nResult        = 0;
	}
};

typedef NET_RPS2_FEEDBACK_DATA2* LPNET_RPS2_FEEDBACK_DATA;

struct CHINA_NET_RPN_DATA : public NET_MSG_GENERIC
{
	TCHAR           szCaptcha[USR_RAND_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szUserid[USR_ID_LENGTH+RSA_ADD+1];
	TCHAR			szUserEmail[USR_EMAIL_LENGTH+RSA_ADD+1];
	TCHAR			szPassword[USR_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szPassword2[USR_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szPassword2Hold[USR_PASS_LENGTH+RSA_ADD+1];

	CHINA_NET_RPN_DATA()
	{
		dwSize  = sizeof(CHINA_NET_CPN_DATA);
		nType  = CHINA_NET_MSG_RESETPIN;
		SecureZeroMemory( szPassword,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szPassword2,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szPassword2Hold,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szUserid,         sizeof(TCHAR) * (USR_ID_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szUserEmail,       sizeof(TCHAR) * (USR_EMAIL_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szCaptcha, sizeof(TCHAR) * (USR_RAND_PASS_LENGTH+RSA_ADD+1) );
	}
};
typedef CHINA_NET_RPN_DATA* LPCHINA_NET_RPN_DATA;

struct NET_RPN_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{
	CHAR			szUserid[USR_ID_LENGTH+1];
	CHAR			szUserEmail[USR_EMAIL_LENGTH+1];
	CHAR			szUserpass[USR_PASS_LENGTH+1];
	CHAR			szUserpass2[USR_PASS_LENGTH+1];
	CHAR			szUserpass2Hold[USR_PASS_LENGTH+1];
	CHAR			szCaptcha[USR_RAND_PASS_LENGTH+1];
	INT				nClient;
	USHORT			nResult;
	
	NET_RPN_FEEDBACK_DATA2()
	{
		dwSize = sizeof(NET_RPN_FEEDBACK_DATA2);
		memset (szUserid, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
		memset (szUserEmail, 0, sizeof(CHAR) * (USR_EMAIL_LENGTH+1));
		memset (szUserpass, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset (szUserpass2, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset (szUserpass2Hold, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset (szCaptcha, 0, sizeof(CHAR) * (USR_RAND_PASS_LENGTH+1));
		nClient        = 0;
		nResult        = 0;
	}
};

typedef NET_RPN_FEEDBACK_DATA2* LPNET_RPN_FEEDBACK_DATA;


struct CHINA_NET_CE_DATA : public NET_MSG_GENERIC
{
	TCHAR           szCaptcha[USR_RAND_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szPassword[USR_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szPassword2[USR_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szUserid[USR_ID_LENGTH+RSA_ADD+1];
	TCHAR			szUserEmail[USR_EMAIL_LENGTH+RSA_ADD+1];

	CHINA_NET_CE_DATA()
	{
		dwSize  = sizeof(CHINA_NET_CE_DATA);
		nType  = CHINA_NET_MSG_EMAIL;
		SecureZeroMemory( szUserid,         sizeof(TCHAR) * (USR_ID_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szPassword,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szPassword2,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szUserEmail,       sizeof(TCHAR) * (USR_EMAIL_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szCaptcha, sizeof(TCHAR) * (USR_RAND_PASS_LENGTH+RSA_ADD+1) );
	}
};
typedef CHINA_NET_CE_DATA* LPCHINA_NET_CE_DATA;

struct NET_CE_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{
	CHAR			szUserid[USR_ID_LENGTH+1];
	CHAR			szUserpass[USR_PASS_LENGTH+1];
	CHAR			szUserpass2[USR_PASS_LENGTH+1];
	CHAR			szUserEmail[USR_EMAIL_LENGTH+1];
	CHAR			szCaptcha[USR_RAND_PASS_LENGTH+1];
	INT				nClient;
	USHORT			nResult;
	
	NET_CE_FEEDBACK_DATA2()
	{
		dwSize = sizeof(NET_CE_FEEDBACK_DATA2);
		memset (szUserid, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
		memset (szUserpass, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset (szUserpass2, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset (szUserEmail, 0, sizeof(CHAR) * (USR_EMAIL_LENGTH+1));
		memset (szCaptcha, 0, sizeof(CHAR) * (USR_RAND_PASS_LENGTH+1));
		nClient        = 0;
		nResult        = 0;
	}
};

typedef NET_CE_FEEDBACK_DATA2* LPNET_CE_FEEDBACK_DATA;

struct CHINA_NET_TOPUP_DATA : public NET_MSG_GENERIC
{
	TCHAR           szCaptcha[USR_RAND_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szCode[USR_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szPin[USR_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szUserid[USR_ID_LENGTH+RSA_ADD+1];

	CHINA_NET_TOPUP_DATA()
	{
		dwSize  = sizeof(CHINA_NET_TOPUP_DATA);
		nType  = CHINA_NET_MSG_TOPUP;
		SecureZeroMemory( szUserid,         sizeof(TCHAR) * (USR_ID_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szCode,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szPin,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szCaptcha, sizeof(TCHAR) * (USR_RAND_PASS_LENGTH+RSA_ADD+1) );
	}
};
typedef CHINA_NET_TOPUP_DATA* LPCHINA_NET_TOPUP_DATA;

struct NET_TOPUP_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{
	CHAR			szUserid[USR_ID_LENGTH+1];
	CHAR			szCode[USR_PASS_LENGTH+1];
	CHAR			szPin[USR_PASS_LENGTH+1];
	CHAR			szCaptcha[USR_RAND_PASS_LENGTH+1];
	INT				nClient;
	USHORT			nResult;
	
	NET_TOPUP_FEEDBACK_DATA2()
	{
		dwSize = sizeof(NET_TOPUP_FEEDBACK_DATA2);
		memset (szUserid, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
		memset (szCode, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset (szPin, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset (szCaptcha, 0, sizeof(CHAR) * (USR_RAND_PASS_LENGTH+1));
		nClient        = 0;
		nResult        = 0;
	}
};

typedef NET_TOPUP_FEEDBACK_DATA2* LPNET_TOPUP_FEEDBACK_DATA;

struct CHINA_NET_GTCVT_DATA : public NET_MSG_GENERIC
{
	TCHAR           szCaptcha[USR_RAND_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szPassword[USR_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szPassword2[USR_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szUserid[USR_ID_LENGTH+RSA_ADD+1];
	TCHAR			szUserEmail[USR_EMAIL_LENGTH+RSA_ADD+1];

	CHINA_NET_GTCVT_DATA()
	{
		dwSize  = sizeof(CHINA_NET_GTCVT_DATA);
		nType  = CHINA_NET_MSG_GT2VP;
		SecureZeroMemory( szUserid,         sizeof(TCHAR) * (USR_ID_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szPassword,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szPassword2,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szUserEmail,       sizeof(TCHAR) * (USR_EMAIL_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szCaptcha, sizeof(TCHAR) * (USR_RAND_PASS_LENGTH+RSA_ADD+1) );
	}
};
typedef CHINA_NET_GTCVT_DATA* LPCHINA_NET_GTCVT_DATA;

struct NET_GTCVT_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{
	CHAR			szUserid[USR_ID_LENGTH+1];
	CHAR			szUserpass[USR_PASS_LENGTH+1];
	CHAR			szUserpass2[USR_PASS_LENGTH+1];
	CHAR			szUserEmail[USR_EMAIL_LENGTH+1];
	CHAR			szCaptcha[USR_RAND_PASS_LENGTH+1];
	INT				nClient;
	USHORT			nResult;
	
	NET_GTCVT_FEEDBACK_DATA2()
	{
		dwSize = sizeof(NET_GTCVT_FEEDBACK_DATA2);
		memset (szUserid, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
		memset (szUserpass, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset (szUserpass2, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
		memset (szUserEmail, 0, sizeof(CHAR) * (USR_EMAIL_LENGTH+1));
		memset (szCaptcha, 0, sizeof(CHAR) * (USR_RAND_PASS_LENGTH+1));
		nClient        = 0;
		nResult        = 0;
	}
};

typedef NET_GTCVT_FEEDBACK_DATA2* LPNET_GTCVT_FEEDBACK_DATA;

struct CHINA_NET_PPCVT_DATA : public NET_MSG_GENERIC
{
	TCHAR           szCaptcha[USR_RAND_PASS_LENGTH+RSA_ADD+1];
	TCHAR			szValue[USR_RAND_PASS_LENGTH+RSA_ADD+1];

	CHINA_NET_PPCVT_DATA()
	{
		dwSize  = sizeof(CHINA_NET_PPCVT_DATA);
		nType  = CHINA_NET_MSG_PP2VP;
		SecureZeroMemory( szValue, sizeof(TCHAR) * (USR_RAND_PASS_LENGTH+RSA_ADD+1) );
		SecureZeroMemory( szCaptcha, sizeof(TCHAR) * (USR_RAND_PASS_LENGTH+RSA_ADD+1) );
	}
};
typedef CHINA_NET_PPCVT_DATA* LPCHINA_NET_PPCVT_DATA;

struct NET_PPCVT_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{
	INT				nClient;
	USHORT			nResult;
	
	NET_PPCVT_FEEDBACK_DATA2()
	{
		dwSize = sizeof(NET_PPCVT_FEEDBACK_DATA2);
		nClient        = 0;
		nResult        = 0;
	}
};

typedef NET_PPCVT_FEEDBACK_DATA2* LPNET_PPCVT_FEEDBACK_DATA;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//
//GM
//
struct GM_NET_LOGIN_DATA : public NET_MSG_GENERIC
{
	int             nChannel;
	CHAR			szPassword[GM_USER_ID+1];
	CHAR			szUserid  [GM_USER_PASS+1];
	CHAR			szEnCrypt[ENCRYPT_KEY+1];

	GM_NET_LOGIN_DATA()
	{		
		dwSize  = sizeof(GM_NET_LOGIN_DATA);
		nType  = GM_NET_MSG_LOGIN;
		nChannel   = 0;
		SecureZeroMemory(szUserid, sizeof(CHAR) * (GM_USER_ID+1));
		SecureZeroMemory(szPassword, sizeof(CHAR) * (GM_USER_PASS+1));
		SecureZeroMemory(szEnCrypt, sizeof(CHAR) * (ENCRYPT_KEY+1));
		MIN_STATIC_ASSERT(sizeof(GM_NET_LOGIN_DATA)<=NET_DATA_BUFSIZE);
	}
};
typedef GM_NET_LOGIN_DATA* LPGM_NET_LOGIN_DATA;


struct GM_NET_LOGIN_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{    
	CHAR			szUserid[GM_USER_ID+1];
	CHAR			szIp[MAX_IP_LENGTH+1];
	INT				nClient;
	EM_LOGIN_FB_SUB	m_emResult;
	INT				nUserNum;
	INT				nUserType;
	BOOL			bExtreme;
	BOOL			bJumping;
	BOOL			bActor;
	USHORT			uChaRemain;
	USHORT			uChaTestRemain;
	__time64_t      tPremiumTime;
	__time64_t      tChatBlockTime;

	GM_NET_LOGIN_FEEDBACK_DATA2()
	{
		dwSize = sizeof(GM_NET_LOGIN_FEEDBACK_DATA2);
		memset (szUserid, 0, sizeof(CHAR) * (GM_USER_ID+1));
		memset (szIp, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));
		nClient        = 0;
		m_emResult     = EM_LOGIN_FB_SUB_OK;
		nUserNum       = 0;
		nUserType      = 0;
		bExtreme	   = FALSE;
		bJumping	= FALSE;
		bActor = FALSE;
		uChaRemain     = 0;
		uChaTestRemain = 0;
		tPremiumTime   = 0;
		tChatBlockTime = 0;
		MIN_STATIC_ASSERT(sizeof(GM_NET_LOGIN_FEEDBACK_DATA2)<=NET_DATA_BUFSIZE);
	}
};
typedef GM_NET_LOGIN_FEEDBACK_DATA2* LPGMNET_LOGIN_FEEDBACK_DATA2;

//
//US
//
struct US_NET_LOGIN_DATA : public NET_MSG_GENERIC
{
	int             nChannel;
	CHAR			szPassword[GS_USER_ID+1];
	CHAR			szUserid  [GS_USER_PASS+1];
	CHAR			szEnCrypt[ENCRYPT_KEY+1];

	US_NET_LOGIN_DATA()
	{		
		dwSize  = sizeof(US_NET_LOGIN_DATA);
		nType  = US_NET_MSG_LOGIN;
		nChannel   = 0;
		SecureZeroMemory(szUserid, sizeof(CHAR) * (GS_USER_ID+1));
		SecureZeroMemory(szPassword, sizeof(CHAR) * (GS_USER_PASS+1));
		SecureZeroMemory(szEnCrypt, sizeof(CHAR) * (ENCRYPT_KEY+1));
		MIN_STATIC_ASSERT(sizeof(US_NET_LOGIN_DATA)<=NET_DATA_BUFSIZE);
	}
};
typedef US_NET_LOGIN_DATA* LPUS_NET_LOGIN_DATA;


struct US_NET_LOGIN_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{    
	CHAR			szUserid[GS_USER_ID+1];
	CHAR			szIp[MAX_IP_LENGTH+1];
	INT				nClient;
	EM_LOGIN_FB_SUB	m_emResult;
	INT				nUserNum;
	INT				nUserType;
	BOOL			bExtreme;	// 극강부 생성 가능 여부;
	BOOL			bJumping;
	BOOL			bActor;
	USHORT			uChaRemain;
	USHORT			uChaTestRemain;
	__time64_t      tPremiumTime;
	__time64_t      tChatBlockTime;

	INT ItemTransferResult;

	US_NET_LOGIN_FEEDBACK_DATA2()
	{
		dwSize = sizeof(US_NET_LOGIN_FEEDBACK_DATA2);
		memset (szUserid, 0, sizeof(CHAR) * (GS_USER_ID+1));
		memset (szIp, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));
		nClient        = 0;
		m_emResult     = EM_LOGIN_FB_SUB_OK;
		nUserNum       = 0;
		nUserType      = 0;
		bExtreme	   = FALSE;
		bJumping = FALSE;
		bActor = FALSE;
		uChaRemain     = 0;
		uChaTestRemain = 0;
		tPremiumTime   = 0;
		tChatBlockTime = 0;
		ItemTransferResult = -1;
		MIN_STATIC_ASSERT(sizeof(US_NET_LOGIN_FEEDBACK_DATA2)<=NET_DATA_BUFSIZE);
	}
};
typedef US_NET_LOGIN_FEEDBACK_DATA2* LPUSNET_LOGIN_FEEDBACK_DATA2;

// Thailand (태국)
struct THAI_NET_LOGIN_DATA : public NET_MSG_GENERIC
{
    int             nChannel;
    CHAR			szPassword[USR_PASS_LENGTH+1];
    CHAR			szUserid  [USR_ID_LENGTH+1];	

    THAI_NET_LOGIN_DATA()
    {		
        dwSize  = sizeof(THAI_NET_LOGIN_DATA);
        nType  = THAI_NET_MSG_LOGIN;
        nChannel   = 0;
        memset(szUserid,   0, sizeof(CHAR) * (USR_ID_LENGTH+1));
        memset(szPassword, 0, sizeof(CHAR) * (USR_PASS_LENGTH+1));
        MIN_STATIC_ASSERT(sizeof(THAI_NET_LOGIN_DATA)<=NET_DATA_BUFSIZE);
    }
};
typedef THAI_NET_LOGIN_DATA* LPTHAI_NET_LOGIN_DATA;

// Terra
struct TERRA_NET_LOGIN_DATA : public NET_MSG_GENERIC
{
    int nChannel;
    TCHAR szTID[TERRA_TID_ENCODE+1];

    TERRA_NET_LOGIN_DATA()
    {		
        dwSize = sizeof(TERRA_NET_LOGIN_DATA);
        nType = TERRA_NET_MSG_LOGIN;
        nChannel  = 0;
        memset(szTID, 0, sizeof(TCHAR) * (TERRA_TID_ENCODE+1));
        MIN_STATIC_ASSERT(sizeof(TERRA_NET_LOGIN_DATA)<=NET_DATA_BUFSIZE);
    }
};
typedef TERRA_NET_LOGIN_DATA* LPTERRA_NET_LOGIN_DATA;


struct TERRA_NET_PASSCHECK_DATA : public NET_MSG_GENERIC
{
    TCHAR szTLoginName[TERRA_TLOGIN_NAME+1];
    TCHAR szUserPass[TERRA_USERPASS+1];
    INT		nCheckFlag; // 패스워드 갱신을 위해서 추가

    TERRA_NET_PASSCHECK_DATA()
        : nCheckFlag(0)
    {		
        dwSize = sizeof(TERRA_NET_PASSCHECK_DATA);
        nType = TERRA_NET_MSG_PASSCHECK;
        memset(szTLoginName, 0, sizeof(TCHAR) * (TERRA_TLOGIN_NAME+1));
        memset(szUserPass, 0, sizeof(TCHAR) * (TERRA_USERPASS+1));
        MIN_STATIC_ASSERT(sizeof(TERRA_NET_PASSCHECK_DATA)<=NET_DATA_BUFSIZE);
    }
};
typedef TERRA_NET_PASSCHECK_DATA* LPTERRA_NET_PASSCHECK_DATA;

struct TERRA_NET_PASSCHECK_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{    
    CHAR            szTerraTLoginName[TERRA_TLOGIN_NAME+1];
    INT				nClient;
    USHORT			nResult;

    TERRA_NET_PASSCHECK_FEEDBACK_DATA2()
    {
        memset(szTerraTLoginName, 0, sizeof(CHAR) * (TERRA_TLOGIN_NAME+1));
        //memset(this, 0, sizeof( TERRA_NET_PASSCHECK_FEEDBACK_DATA2 ));
        nType   = EMNET_MSG(0);
        dwSize  = sizeof(TERRA_NET_PASSCHECK_FEEDBACK_DATA2);
        nClient	= 0;
        nResult	= 0;
        MIN_STATIC_ASSERT(sizeof(TERRA_NET_PASSCHECK_FEEDBACK_DATA2)<=NET_DATA_BUFSIZE);
    }
};
typedef TERRA_NET_PASSCHECK_FEEDBACK_DATA2* LPTERRA_NET_PASSCHECK_FEEDBACK_DATA2;

struct TERRA_NET_LOGIN_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{    
    CHAR            szTerraTLoginName[TERRA_TLOGIN_NAME+1];
    CHAR            szTerraDecodedTID[TERRA_TID_DECODE+1];
    CHAR            szIp[MAX_IP_LENGTH+1];
    INT				nClient;
    EM_LOGIN_FB_SUB	m_emResult;
    INT				nUserNum;
    INT				nUserType;
	BOOL			bExtreme;				// 극강부 생성 가능 여부;
	BOOL			bJumping;
	BOOL			bActor;					// 변술부 생성 가능 여부;
    INT				nCheckFlag;				// 말레이지아 패스워드를 받기위한 Check Flag 값
    USHORT			uChaRemain;
    USHORT			uChaTestRemain;
    EMIP_BONUS_CLASS emIPBonus;			// 말레이시아 PC방 이벤트
    INT				nVTGameTime;			//  베트남 GameTime 저장
    __time64_t		tLastLoginTime;			// 중국, 베트남 LastLoginTime 저장
    __time64_t      tPremiumTime;
    __time64_t      tChatBlockTime;

    TERRA_NET_LOGIN_FEEDBACK_DATA2()
    {		
        dwSize = sizeof(TERRA_NET_LOGIN_FEEDBACK_DATA2);
        nType  = EMNET_MSG(0);
        memset(szTerraTLoginName, 0, sizeof(CHAR) * (TERRA_TLOGIN_NAME+1));
        memset(szIp, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));
        memset(szTerraDecodedTID, 0, sizeof(CHAR) * (TERRA_TID_DECODE+1));
        nClient        = 0;
        m_emResult     = EM_LOGIN_FB_SUB_OK;
        nUserNum       = 0;
        nUserType      = 0;
		bExtreme	   = FALSE;
		bJumping = FALSE;
		bActor = FALSE;
        nCheckFlag	   = 0;
        uChaRemain     = 0;		
        uChaTestRemain = 0;
        tPremiumTime   = 0;
        tChatBlockTime = 0;
        nVTGameTime	   = 0;
        tLastLoginTime = 0;
        emIPBonus	   = EMIP_BONUS_NONE;
        MIN_STATIC_ASSERT(sizeof(TERRA_NET_LOGIN_FEEDBACK_DATA2)<=NET_DATA_BUFSIZE);
    }
};
typedef TERRA_NET_LOGIN_FEEDBACK_DATA2* LPTERRA_NET_LOGIN_FEEDBACK_DATA2;

// Japan
struct JAPAN_NET_LOGIN_DATA : public NET_MSG_GENERIC
{
    int				nChannel;		
    CHAR			szUserid  [JAPAN_USER_ID+1];
    CHAR			szPassword[JAPAN_USER_PASS+1];
    CHAR			szEnCrypt[ENCRYPT_KEY+1];

    JAPAN_NET_LOGIN_DATA()
    {		
        dwSize  = sizeof(JAPAN_NET_LOGIN_DATA);
        nType  = JAPAN_NET_MSG_LOGIN;
        nChannel   = 0;
        SecureZeroMemory(szUserid,         sizeof(CHAR) * (JAPAN_USER_ID+1));
        SecureZeroMemory(szPassword,       sizeof(CHAR) * (JAPAN_USER_PASS+1));
        SecureZeroMemory(szEnCrypt, sizeof(CHAR) * (ENCRYPT_KEY+1));
        MIN_STATIC_ASSERT(sizeof(JAPAN_NET_LOGIN_DATA)<=NET_DATA_BUFSIZE);
    }
};
typedef JAPAN_NET_LOGIN_DATA* LPJAPAN_NET_LOGIN_DATA;

// Japan Gonzo로 회사 변경되면서 구조체추가
struct JAPAN_NET_LOGIN_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{    
    CHAR            JapanUserID[JAPAN_USER_ID+1];
    CHAR            szIp[MAX_IP_LENGTH+1];
    INT				nClient;
    EM_LOGIN_FB_SUB	m_emResult;
    INT				nUserNum;
    INT				nUserType;
	BOOL			bExtreme;	// 극강부 생성 가능 여부;
	BOOL			bJumping;
	BOOL			bActor;
    USHORT			uChaRemain;
    USHORT			uChaTestRemain;
    __time64_t      tPremiumTime;
    __time64_t      tChatBlockTime;

    JAPAN_NET_LOGIN_FEEDBACK_DATA2()
    {		
        dwSize = sizeof(JAPAN_NET_LOGIN_FEEDBACK_DATA2);
        memset(JapanUserID, 0, sizeof(CHAR) * (JAPAN_USER_ID+1));
        memset(szIp, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));
        nClient        = 0;
        m_emResult     = EM_LOGIN_FB_SUB_OK;
        nUserNum       = 0;
        nUserType      = 0;
		bExtreme	   = FALSE;
		bJumping = FALSE;
		bActor = FALSE;
        uChaRemain     = 0;
        uChaTestRemain = 0;
        tPremiumTime   = 0;
        tChatBlockTime = 0;
        MIN_STATIC_ASSERT(sizeof(JAPAN_NET_LOGIN_FEEDBACK_DATA2)<=NET_DATA_BUFSIZE);
    }
};
typedef JAPAN_NET_LOGIN_FEEDBACK_DATA2* LPJAPAN_NET_LOGIN_FEEDBACK_DATA2;

struct JAPAN_NET_LOGIN_UUID : public NET_MSG_GENERIC
{    
    CHAR	szUUID[sc::string::UUID_LENGTH];

    JAPAN_NET_LOGIN_UUID()
    {		
        nType	= JAPAN_NET_MSG_UUID;
        dwSize	= sizeof(JAPAN_NET_LOGIN_UUID);
        memset( szUUID, 0, sizeof(CHAR) * sc::string::UUID_LENGTH );
        MIN_STATIC_ASSERT(sizeof(JAPAN_NET_LOGIN_UUID)<=NET_DATA_BUFSIZE);
    }
};

// Excite Japan
struct EXCITE_NET_LOGIN_DATA : public NET_MSG_GENERIC
{
    int				nChannel;
    CHAR			szUID[EXCITE_USER_ID+1];
    CHAR			szTDATE[EXCITE_TDATE+1];
    CHAR            szMD5[EXCITE_MD5+1];

    EXCITE_NET_LOGIN_DATA()
    {
        dwSize = sizeof(EXCITE_NET_LOGIN_DATA);
        nType = EXCITE_NET_MSG_LOGIN;
        nChannel  = 0;
        memset(szUID,   0, sizeof(CHAR) * (EXCITE_USER_ID+1));
        memset(szTDATE, 0, sizeof(CHAR) * (EXCITE_TDATE+1));
        memset(szMD5,   0, sizeof(CHAR) * (EXCITE_MD5+1));
        MIN_STATIC_ASSERT(sizeof(EXCITE_NET_LOGIN_DATA)<=NET_DATA_BUFSIZE);
    }
};
typedef EXCITE_NET_LOGIN_DATA* LPEXCITE_NET_LOGIN_DATA;

struct EXCITE_NET_PASSCHECK_DATA : public NET_MSG_GENERIC
{
    TCHAR	szExciteUserID[EXCITE_SEC_ID+1];
    TCHAR	szUserPass[EXCITE_SEC_PASS+1];
    INT		nCheckFlag; // 패스워드 갱신을 위해서 추가

    EXCITE_NET_PASSCHECK_DATA()
        : nCheckFlag(0)
    {
        nType = EXCITE_NET_MSG_PASSCHECK;
        dwSize = sizeof(EXCITE_NET_PASSCHECK_DATA);
        memset(szExciteUserID, 0, sizeof(szExciteUserID));
        memset(szUserPass, 0, sizeof(szUserPass));
        MIN_STATIC_ASSERT(sizeof(EXCITE_NET_PASSCHECK_DATA)<=NET_DATA_BUFSIZE);
    }
};
typedef EXCITE_NET_PASSCHECK_DATA* LPEXCITE_NET_PASSCHECK_DATA;

struct EXCITE_NET_PASSCHECK_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{    
    TCHAR			szExciteUserID[EXCITE_USER_ID+1];
    INT				nClient;
    USHORT			nResult;

    EXCITE_NET_PASSCHECK_FEEDBACK_DATA2()
        : nClient(0)
        , nResult(0)
    {
        //memset(this, 0, sizeof( EXCITE_NET_PASSCHECK_FEEDBACK_DATA2 ));
        dwSize = sizeof(EXCITE_NET_PASSCHECK_FEEDBACK_DATA2);
        nType = EMNET_MSG(0);
        memset(szExciteUserID, 0, sizeof( szExciteUserID ));
        MIN_STATIC_ASSERT(sizeof(EXCITE_NET_PASSCHECK_FEEDBACK_DATA2)<=NET_DATA_BUFSIZE);
    }
};
typedef EXCITE_NET_PASSCHECK_FEEDBACK_DATA2* LPEXCITE_NET_PASSCHECK_FEEDBACK_DATA2;

struct EXCITE_NET_LOGIN_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{    
    CHAR            szExciteUserID[EXCITE_USER_ID+1];
    CHAR            szIp[MAX_IP_LENGTH+1];
    INT				nClient;
    EM_LOGIN_FB_SUB	m_emResult;
    INT				nUserNum;
    INT				nUserType;
    INT				nCheckFlag; // 일본 패스워드를 받기위한 Check Flag 값
    USHORT			uChaRemain;
    USHORT			uChaTestRemain;
    __time64_t      tPremiumTime;
    __time64_t      tChatBlockTime;

    EXCITE_NET_LOGIN_FEEDBACK_DATA2()
    {
        dwSize = sizeof(EXCITE_NET_LOGIN_FEEDBACK_DATA2);
        memset(szExciteUserID, 0, sizeof(CHAR) * (EXCITE_USER_ID+1));
        memset(szIp, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));
        nClient        = 0;
        m_emResult     = EM_LOGIN_FB_SUB_OK;
        nUserNum       = 0;
        nUserType      = 0;
        nCheckFlag	   = 0;
        uChaRemain     = 0;
        uChaTestRemain = 0;
        tPremiumTime   = 0;
        tChatBlockTime = 0;
        MIN_STATIC_ASSERT(sizeof(EXCITE_NET_LOGIN_FEEDBACK_DATA2)<=NET_DATA_BUFSIZE);
    }
};
typedef EXCITE_NET_LOGIN_FEEDBACK_DATA2* LPEXCITE_NET_LOGIN_FEEDBACK_DATA2;

// China
struct CHINA_NET_LOGIN_DATA : public NET_MSG_GENERIC
{
    int             nChannel;
    TCHAR           szRandomPassword[USR_RAND_PASS_LENGTH+RSA_ADD+1];
    TCHAR			szPassword[USR_PASS_LENGTH+RSA_ADD+1];
    TCHAR			szUserid[USR_ID_LENGTH+RSA_ADD+1];

    CHINA_NET_LOGIN_DATA()
    {
        dwSize  = sizeof(CHINA_NET_LOGIN_DATA);
        nType  = CHINA_NET_MSG_LOGIN;
        nChannel   = 0;
        SecureZeroMemory( szUserid,         sizeof(TCHAR) * (USR_ID_LENGTH+RSA_ADD+1) );
        SecureZeroMemory( szPassword,       sizeof(TCHAR) * (USR_PASS_LENGTH+RSA_ADD+1) );
        SecureZeroMemory( szRandomPassword, sizeof(TCHAR) * (USR_RAND_PASS_LENGTH+RSA_ADD+1) );
        MIN_STATIC_ASSERT(sizeof(CHINA_NET_LOGIN_DATA)<=NET_DATA_BUFSIZE);
    }
};
typedef CHINA_NET_LOGIN_DATA* LPCHINA_NET_LOGIN_DATA;

// GSP
struct GSP_NET_LOGIN_DATA : public NET_MSG_GENERIC
{
    int nChannel;
    TCHAR szUUID[sc::string::UUID_LENGTH];

    GSP_NET_LOGIN_DATA()
    {
        dwSize = sizeof(GSP_NET_LOGIN_DATA);
        nType = GSP_NET_MSG_LOGIN;
        nChannel  = 0;
        memset( szUUID, 0, sizeof(TCHAR) * (sc::string::UUID_LENGTH) );
        MIN_STATIC_ASSERT(sizeof(GSP_NET_LOGIN_DATA)<=NET_DATA_BUFSIZE);
    }
};
typedef GSP_NET_LOGIN_DATA* LPGSP_NET_LOGIN_DATA;

struct GSP_NET_LOGIN_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{    
    TCHAR           szGspUserID[GSP_USERID];	
    TCHAR           szIp[GSP_USERIP];
    TCHAR			szUUID[sc::string::UUID_LENGTH]; 
    INT				nClient;
    EM_LOGIN_FB_SUB	m_emResult;
    INT				nUserNum;
    INT				nUserType;
    USHORT			uChaRemain;
    USHORT			uChaTestRemain;
    __time64_t      tPremiumTime;
    __time64_t      tChatBlockTime;

    GSP_NET_LOGIN_FEEDBACK_DATA2()
    {		
        dwSize = sizeof(GSP_NET_LOGIN_FEEDBACK_DATA2);
        nType = EMNET_MSG(0);
        memset( szGspUserID, 0, sizeof(TCHAR) * (GSP_USERID) );
        memset( szIp, 0, sizeof(TCHAR) * (GSP_USERIP) );
        memset( szUUID, 0, sizeof(TCHAR) * (sc::string::UUID_LENGTH) );
        nClient        = 0;
        m_emResult     = EM_LOGIN_FB_SUB_OK;
        nUserNum       = 0;
        nUserType      = 0;
        uChaRemain     = 0;
        uChaTestRemain = 0;
        tPremiumTime   = 0;
        tChatBlockTime = 0;
        MIN_STATIC_ASSERT(sizeof(GSP_NET_LOGIN_FEEDBACK_DATA2)<=NET_DATA_BUFSIZE);
    }
};
typedef GSP_NET_LOGIN_FEEDBACK_DATA2* LPGSP_NET_LOGIN_FEEDBACK_DATA2;

// Common
struct NET_PASSCHECK_FEEDBACK_DATA : public NET_MSG_GENERIC
{    
    INT				nClient;
    USHORT			nResult;

    NET_PASSCHECK_FEEDBACK_DATA()
    {		
        //memset(this, 0, sizeof( NET_PASSCHECK_FEEDBACK_DATA ));
        nType		= NET_MSG_PASSCHECK_FB;
        dwSize		= sizeof( NET_PASSCHECK_FEEDBACK_DATA );
        nClient			= 0;
        nResult			= 0;
        MIN_STATIC_ASSERT(sizeof(NET_PASSCHECK_FEEDBACK_DATA)<=NET_DATA_BUFSIZE);
    }
};
typedef NET_PASSCHECK_FEEDBACK_DATA* LPNET_PASSCHECK_FEEDBACK_DATA;

struct NET_LOGIN_FEEDBACK_DATA : public NET_MSG_GENERIC
{    
    EM_LOGIN_FB_SUB m_Result;    
	BOOL				m_Extreme;	// 극강부 생성 가능 여부;
	BOOL				m_bActor;	// 변술부 생성 가능 여부;
    INT				m_CheckFlag; // 2차 패스워드를 받기위한 Check Flag 값
    INT				m_LauncherVersion;
    INT				m_GameVersion;
    USHORT          m_ChaRemain; // 만들수 있는 캐릭터 갯수
    char			m_szDaumGID[DAUM_MAX_GID_LENGTH+1];
    wb::EM_COUNTRY  m_Country;
	Country::SCOUNTRY_INFO m_sCountryInfo;
	BOOL	m_bJumping;
	USHORT			uUserPoints; //userpanel
	USHORT			uBattlePoints;
	DWORD			dwGameTime;
	TCHAR			szUserEmail[USR_EMAIL_LENGTH+RSA_ADD+1];	

    NET_LOGIN_FEEDBACK_DATA()
        : m_Country(wb::UNKNOWN_COUNTRY)
    {
        //memset(this, 0, sizeof(NET_LOGIN_FEEDBACK_DATA));
        dwSize = sizeof(NET_LOGIN_FEEDBACK_DATA);
        nType  = EMNET_MSG(0);
        memset(m_szDaumGID, 0, sizeof(char)*(DAUM_MAX_GID_LENGTH+1));
        m_Result       = EM_LOGIN_FB_SUB_FAIL;
        m_LauncherVersion = 0;
        m_GameVersion  = 0;
        m_ChaRemain    = 0;
		m_Extreme	   = FALSE;
		m_bActor = FALSE;
        m_CheckFlag	   = 0;    
		m_bJumping = FALSE;

		uUserPoints = 0; //userpanel
		uBattlePoints = 0;
		dwGameTime = 0;
		memset(szUserEmail, 0, sizeof(char)*(USR_EMAIL_LENGTH+RSA_ADD+1));
        MIN_STATIC_ASSERT(sizeof(NET_LOGIN_FEEDBACK_DATA)<=NET_DATA_BUFSIZE);
    }

    void SetDaumGid(const char* szGid)
    {
        if (szGid)
            StringCchCopy(m_szDaumGID, DAUM_MAX_GID_LENGTH+1, szGid);
    }
};
typedef NET_LOGIN_FEEDBACK_DATA* LPNET_LOGIN_FEEDBACK_DATA;


struct NET_LOGIN_FEEDBACK_DATA2 : public NET_MSG_GENERIC
{    
    CHAR			szUserid[USR_ID_LENGTH+1];
    CHAR			szIp[MAX_IP_LENGTH+1];
    INT				m_ClientSlot;
    EM_LOGIN_FB_SUB	m_emResult;
    INT				nUserNum;
    INT				nUserType;
	BOOL			bExtreme;				//! 극강부 생성 가능 여부;
	BOOL			bJumping;
	BOOL			bActor;					//! 변술부 생성 가능 여부;
    INT				nCheckFlag;				//! 패스워드를 받기 위한 Check Flag 값
    EMIP_BONUS_CLASS emIPBonus;				//! IP보너스
    INT				nChinaGameTime;			//! 중국 GameTime 저장
    INT				nChinaOfflineTime;		//! 중국 OfflienTiem 저장
    INT				nChinaUserAge;			//! 중국 UserAge 저장
    INT				nVTGameTime;			//! 베트남 GameTime 저장
    USHORT			uChaRemain;     //! 생성가능 캐릭터 갯수
    USHORT			uChaTestRemain; //! 생성가능 캐릭터 갯수(Test 서버)
    __time64_t      tPremiumTime;   //! Premium User
    __time64_t      tChatBlockTime; //! Chat block time
    __time64_t		tLastLoginTime; //! 중국, 베트남 LastLoginTime 저장
    WORD            wThaiPlayTime;  //! Thailand (오늘 플레이한 시간 단위:분)
    WORD            wThaiFlag;      //! Thailand (사용자 구분 플래그)
    wb::EM_COUNTRY  m_Country; //! 국가
	Country::SCOUNTRY_INFO sCountryInfo;	//! 대륙 및 국가 정보;
	INT ItemTransferResult;
	USHORT			uUserPoints; //userpanel
	USHORT			uBattlePoints;
	TCHAR			szUserEmail[USR_EMAIL_LENGTH+RSA_ADD+1];

    NET_LOGIN_FEEDBACK_DATA2()
        : m_ClientSlot(0)
        , m_emResult(EM_LOGIN_FB_SUB_OK)
        , nUserNum(0)
        , nUserType(0)
		, bExtreme(false)
		, bJumping(false)
		, bActor(FALSE)
        , uChaRemain(0)
        , uChaTestRemain(0)
        , tPremiumTime(0)
        , tChatBlockTime(0)
        , wThaiPlayTime(0)
        , wThaiFlag(0)
        , nCheckFlag(0)        
        , emIPBonus(EMIP_BONUS_NONE)
        , nChinaGameTime(0)
        , nChinaOfflineTime(0)
        , tLastLoginTime(0)
        , nChinaUserAge(0)
        , nVTGameTime(0)
        , m_Country(wb::UNKNOWN_COUNTRY)
		, ItemTransferResult(-1)
    {
        dwSize = sizeof(NET_LOGIN_FEEDBACK_DATA2);
        nType = NET_MSG_LOGIN_FEEDBACK_DATA2;
        memset(szUserid, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
		memset(szIp, 0, sizeof(CHAR) * (MAX_IP_LENGTH+1));        

		uUserPoints = 0; //userpanel
		uBattlePoints = 0;
		memset(szUserEmail, 0, sizeof(char)*(USR_EMAIL_LENGTH+RSA_ADD+1));
		MIN_STATIC_ASSERT(sizeof(NET_LOGIN_FEEDBACK_DATA2)<=NET_DATA_BUFSIZE);
    }

    void SetUserId(const char* _UserID)
    {
        if (_UserID)
            StringCchCopy(szUserid, USR_ID_LENGTH+1, _UserID);
    }

    void SetIp(const char* _Ip)
    {
        if (_Ip)
            StringCchCopy(szIp, MAX_IP_LENGTH+1, _Ip);
    }
};
typedef NET_LOGIN_FEEDBACK_DATA2* LPNET_LOGIN_FEEDBACK_DATA2;


// Agent Server -> Field Server 로그인 성공후 결과값 전송
// Field Server 에서 세팅함
struct NET_LOGIN_FEEDBACK_DATA3 : public NET_MSG_GENERIC
{
    char szUserid[USR_ID_LENGTH+1];
    INT nUserNum;
    wb::EM_COUNTRY m_Country;

    NET_LOGIN_FEEDBACK_DATA3(DWORD UserDbNum, const char* UserId, wb::EM_COUNTRY Country)
        : nUserNum(UserDbNum)
        , m_Country(Country)
    {
        nType = NET_MSG_LOGIN_FB_SUB_OK_FIELD;
        dwSize = sizeof(NET_LOGIN_FEEDBACK_DATA3);		
        if (UserId)
            StringCchCopy(szUserid, USR_ID_LENGTH+1, UserId);
        else
            memset(szUserid, 0, USR_ID_LENGTH+1);
        MIN_STATIC_ASSERT(sizeof(NET_LOGIN_FEEDBACK_DATA3)<=NET_DATA_BUFSIZE);
    }
};
//typedef NET_LOGIN_FEEDBACK_DATA3* LPNET_LOGIN_FEEDBACK_DATA3;

struct NET_LOGOUT_DATA : public NET_MSG_GENERIC
{    
    CHAR			szUserID[USR_ID_LENGTH+1];// 로그아웃 하는 사용자
    INT				nGameTime;				// 플레이시간
    INT				nUserNum;				// 사용자 번호
    INT				nChaNum;

    NET_LOGOUT_DATA()
    {
        nType = EMNET_MSG(0);
        dwSize = sizeof(NET_LOGOUT_DATA);
        memset(szUserID, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
        nGameTime = 0;
        nUserNum  = -1;
        nChaNum   = 0;
        MIN_STATIC_ASSERT(sizeof(NET_LOGOUT_DATA)<=NET_DATA_BUFSIZE);
    }

    void SetUserId(const char* UserId)
    {
        if (UserId)
            StringCchCopy(szUserID, USR_ID_LENGTH+1, UserId);
    }
};
typedef NET_LOGOUT_DATA* LPNET_LOGOUT_DATA;

struct DAUM_NET_LOGOUT_DATA : public NET_MSG_GENERIC
{   
    CHAR			szUID[DAUM_MAX_UID_LENGTH+1];// 로그아웃 하는 사용자
    INT				nGameTime;				// 플레이시간
    INT				nUserNum;				// 사용자 번호
    INT				nChaNum;

    DAUM_NET_LOGOUT_DATA()
    {
        nType = EMNET_MSG(0);
        dwSize = sizeof(DAUM_NET_LOGOUT_DATA);
        memset(szUID, 0, sizeof(CHAR) * (DAUM_MAX_UID_LENGTH+1));
        nGameTime = 0;
        nUserNum  = -1;
        nChaNum   = 0;
        MIN_STATIC_ASSERT(sizeof(DAUM_NET_LOGOUT_DATA)<=NET_DATA_BUFSIZE);
    }
};
typedef DAUM_NET_LOGOUT_DATA* LPDAUM_NET_LOGOUT_DATA;

struct NET_LOGOUT_DATA_SIMPLE : public NET_MSG_GENERIC
{    
    CHAR szUserID[USR_ID_LENGTH+1];// 로그아웃 하는 사용자

    NET_LOGOUT_DATA_SIMPLE()
    {
        nType = EMNET_MSG(0);
        dwSize = sizeof(NET_LOGOUT_DATA_SIMPLE);
        memset(szUserID, 0, sizeof(CHAR) * (USR_ID_LENGTH+1));
        MIN_STATIC_ASSERT(sizeof(NET_LOGOUT_DATA_SIMPLE)<=NET_DATA_BUFSIZE);
    }

    void SetUserId(const char* UserId)
    {
        if (UserId)
            StringCchCopy(szUserID, USR_ID_LENGTH+1, UserId);
    }
};
typedef NET_LOGOUT_DATA_SIMPLE* LPNET_LOGOUT_DATA_SIMPLE;

struct NET_LOGIN_WB_CA
{
    int m_Channel;
    std::string m_Key;
    std::string m_Password;
    std::string m_UserId;
    
    MSGPACK_DEFINE(m_Channel, m_Key, m_UserId, m_Password);

    NET_LOGIN_WB_CA()
        : m_Channel(0)
    {
    }

    NET_LOGIN_WB_CA(
        int Channel,
        const std::string& Key,
        const std::string& Password,
        const std::string& UserId)
        : m_Channel(Channel)
        , m_Key(Key)
        , m_Password(Password)
        , m_UserId(UserId)
    {
    }
};

struct NET_CLOSE_CLIENT : public NET_MSG_GENERIC
{
    DWORD dwCloseCharNum;

    NET_CLOSE_CLIENT(const DWORD dwCharNum)
        : dwCloseCharNum(dwCharNum)
    {
        dwSize = sizeof(NET_CLOSE_CLIENT);
        nType  = NET_MSG_CLOSE_CLIENT;
    }
};

#endif
