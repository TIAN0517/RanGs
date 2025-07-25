#include "../../pch.h"
#include "../../../SigmaCore/gassert.h"
#include "../../../SigmaCore/Util/DateTime.h"
#include "../../../RanLogic/Msg/LoginMsg.h"
#include "../../RanLogic/Msg/RegisterMsg.h"
#include "./ADOManager.h"

// ----------------------------------------------------------------------------
#include "../../../SigmaCore/DebugInclude.h"
// ----------------------------------------------------------------------------

namespace db
{

int AdoManager::UserChaNumIncrease(int nUserNum)
{
    sc::writeLogError("Do not call this function. UserChaNumIncrease");
    return sc::db::DB_ERROR;
}

int AdoManager::SetPremiumTime(int nUserNum, __time64_t tPremiumTime)
{
    sc::writeLogError("Do not call this function. SetPremiumTime");
    return sc::db::DB_ERROR;
}

__time64_t AdoManager::GetChatBlockTime(int nUserNum)
{
    sc::writeLogError("Do not call this function. GetChatBlockTime");
    return sc::db::DB_ERROR;
}

int AdoManager::SetChatBlockTime(int nUserNum, __time64_t tBlockTime)
{
    sc::writeLogError("Do not call this function. SetChatBlockTime");
    return sc::db::DB_ERROR;
}

int AdoManager::TerraUserPassCheck(const TCHAR* szTID, const TCHAR* szPasswd, int nCheckFlag)
{
    sc::writeLogError("Do not call this function. TerraUserPassCheck");
    return sc::db::DB_ERROR;
}

int AdoManager::DaumUserPassCheck(const TCHAR* szDaumGID, const TCHAR* szDaumPasswd, int nCheckFlag)
{
    sc::writeLogError("Do not call this function. DaumUserPassCheck");
    return sc::db::DB_ERROR;
}

int AdoManager::ExciteUserPassCheck(
    const TCHAR* szUID, const TCHAR* szUserID, const TCHAR* szPasswd, int nCheckFlag)
{
    sc::writeLogError("Do not call this function. ExciteUserPassCheck");
    return sc::db::DB_ERROR;
}

int AdoManager::ThaiUserClassCheck(const TCHAR* szUsrIP)
{
    sc::writeLogError("AdoManager::ThaiUserClassCheck. Use ODBC");
    return sc::db::DB_ERROR;
}

int AdoManager::MyUserClassCheck(const TCHAR* szUsrIP)
{
    sc::writeLogError("Do not call this function. MyUserClassCheck");
    return sc::db::DB_ERROR;
}

int AdoManager::PhUserClassCheck(const TCHAR* szUsrIP)
{
    sc::writeLogError("Do not call this function. PhUserClassCheck");
    return sc::db::DB_ERROR;
}

// UserMoney Add
int AdoManager::UpdateUserMoneyAdd(DWORD dwUserID, LONGLONG lnUserMoney)
{
    sc::writeLogError("Do not call this function. UpdateUserMoneyAdd");
    return sc::db::DB_ERROR;
}

int AdoManager::UpdateUserLastInfoAdd(DWORD dwUserID, LONGLONG lnUserMoney)
{
    sc::writeLogError("Do not call this function. UpdateUserLastInfoAdd");
    return sc::db::DB_ERROR;
}
    
int AdoManager::AllServerUserLogout(int nSvrGrp)
{
    sc::writeLogError("Do not call this function. AllServerUserLogout");
    return sc::db::DB_ERROR;
}

int AdoManager::GetUserTempInfo(int nUserNum)
{
    sc::writeLogError("Do not call this function. GetUserTempInfo");
    return sc::db::DB_ERROR;
}

//! 0 일반사용자
//! 10 특별사용자 (게임방 업체 사장, 기자 등등)
//! 20 GM 3 급
//! 21 GM 2 급
//! 22 GM 1 급
//! 30 Master
int AdoManager::GetUserType(int nUserNum)
{
    if (nUserNum <= 0)
        return 0;	

    sc::db::AdoExt Ado(m_UserDBConnString);
    Ado.AppendRParamInteger();
    Ado.AppendIParamInteger("@nUserNum", nUserNum);
    Ado.AppendOParamInteger("@nReturn");

    std::string spName;

    switch (m_ServiceProvider)
    {
    case SP_KOREA:		
        spName = "dbo.daum_user_gettype";
        break;

    case SP_JAPAN:		
        // 일본 Excite에서 Gonzo로 서비스사 변경에 따른 변경
        spName = "dbo.Japan_user_gettype";
        break;

    case SP_PHILIPPINES:
        spName = "dbo.terra_user_gettype";
        break;

    case SP_MALAYSIA:		
        spName = "dbo.terra_user_gettype";
        break;

    case SP_THAILAND:		
        spName = "dbo.user_gettype";
        break;

    case SP_GLOBAL:		
        spName = "dbo.gsp_user_gettype";
        break;

    case SP_GS:		
        spName = "dbo.GS_user_gettype";
        break;

    case SP_INDONESIA:
        spName = "dbo.user_gettype";
        break;

	case SP_EU:		
		spName = "dbo.GM_user_gettype";
		break;

	case SP_US:		
		spName = "dbo.US_user_gettype";
		break;

    default:		
        spName = "dbo.user_gettype";
        break;
    }

    if (spName.empty())
        return 0;
    else
        return Ado.ExecuteStoredProcedureIntReturn(spName, "@nReturn");
}

int AdoManager::GetUserCountryInfo ( IN int nUserNum, OUT Country::SCOUNTRY_INFO& sCountryInfo )
{
	int nRet = sc::db::DB_OK;

	sc::db::AdoExt adoUserDB( m_UserDBConnString );
	APPEND_IPARAM_BIGINT( adoUserDB, "@UserID", nUserNum );

	if( !adoUserDB.Execute4Cmd( "dbo.sp_SelectUserCountry", adCmdStoredProc ) )
		return sc::db::DB_ERROR;

	if( adoUserDB.GetEOF() || adoUserDB.GetBOF() )
	{
		sCountryInfo.RESET();
	}
	else
	{
		adoUserDB.GetCollect( "Continent", sCountryInfo.wContinentID );
		adoUserDB.GetCollect( "Country", sCountryInfo.wCountryID );
	}

	return nRet;
}

int AdoManager::UserTestChaNumDecrease(int nUserNum)
{
    sc::writeLogError("Do not call this function. UserTestChaNumDecrease");
    return sc::db::DB_ERROR;
}

int AdoManager::UserChaNumDecrease(int nUserNum)
{
    sc::writeLogError("Do not call this function. UserChaNumDecrease");
    return sc::db::DB_ERROR;
}

int AdoManager::GetChaGuildNum(DWORD nChaNum)
{
    sc::writeLogError("Do not call this function. GetChaGuildNum");
    return sc::db::DB_ERROR;
}

int AdoManager::UserTestChaNumIncrease(int nUserNum)
{
    sc::writeLogError("Do not call this function. UserTestChaNumIncrease");
    return sc::db::DB_ERROR;
}

int AdoManager::UserUpdateCha(int nUsrNum, const TCHAR* szChaName)
{
    sc::writeLogError("Do not call this function. UserUpdateCha");
    return sc::db::DB_ERROR;
}

int AdoManager::UserLogoutSimple(const TCHAR* szUsrID)
{
    if (!szUsrID)
        return sc::db::DB_ERROR;

    std::string TempUserId(szUsrID);
    sc::string::replaceForDb(TempUserId);

    // dbo.UserLogoutSimple
    // @szUserID varchar(20)
    const long UserIdLength = 20;
    char szUserId[UserIdLength] = {0};
    StringCchCopy(szUserId, UserIdLength, TempUserId.c_str());

    sc::db::AdoExt Ado(m_UserDBConnString);    
    
    switch (m_ServiceProvider)
    {
    case SP_WORLD_BATTLE:
        APPEND_IPARAM_VARCHAR(Ado, "@szUserID", szUserId, UserIdLength-1);        
        break;

    default:
        sc::writeLogError(sc::string::format("AdoManager::UserLogoutSimple Unknown service provider %1%", m_ServiceProvider));
        break;
    }
    
    return ExecuteStoredProcedure("dbo.UserLogoutSimple" , Ado);
}

int AdoManager::UserLogoutSimple2(int nUserNum)		
{
    sc::writeLogError("Do not call this function. UserLogoutSimple2");
    return sc::db::DB_ERROR;
}

int AdoManager::UserLogout(
    const TCHAR* szUsrID, 
	const TCHAR* szUsrIP, 
    int nUsrNum, 
    int nGameTime,
    int nChaNum,
    int nSvrGrp,
    int nSvrNum, 
    int nTotalGameTime,
    int nOfflineTime)		
{
    sc::writeLogError("Do not call this function. UserLogout");
    return sc::db::DB_ERROR;
}

int AdoManager::ThaiUserCheck(
    const TCHAR* szUsrID,
    const TCHAR* szPasswd,
    const TCHAR* szUsrIP,
    int nSvrGrp,
    int nSvrNum)
{
    sc::writeLogError("Do not call this function. ThaiUserCheck");
    return sc::db::DB_ERROR;
}

int AdoManager::ThaiGetUserInfo(const TCHAR* szUserID, NET_LOGIN_FEEDBACK_DATA2* pMsg )
{
    sc::writeLogError("Do not call this function. ThaiGetUserInfo");
    return sc::db::DB_ERROR;
}

int AdoManager::UserCheck(
    const TCHAR* szUsrID, 
    const TCHAR* szPasswd, 
    const TCHAR* szUsrIP,
    const TCHAR* szRandomPasswd,
    int nRandomNum,
    int nSvrGrp,
    int nSvrNum)			
{
    sc::writeLogError("Do not call this function. UserCheck");
    return sc::db::DB_ERROR;
}

int	AdoManager::UserRegister(
                    const TCHAR* szUsrID, 
                    const TCHAR* szUsrEmail, 
                    const TCHAR* szPasswd, 
                    const TCHAR* szPasswd2)			
{
    sc::writeLogError("Do not call this function. UserCheck");
    return sc::db::DB_ERROR;
}


//userpanel
int AdoManager::UserChangeEmail(
					const TCHAR* szUsrID, 
					const TCHAR* szPasswd, 
					const TCHAR* szPasswd2,
					const TCHAR* szUsrEmail,
					const TCHAR* szCaptcha)
{
	sc::writeLogError("Do not call this function. IdnUserCheck");
	return sc::db::DB_ERROR;
}


int AdoManager::UserChangePin(
				  const TCHAR* szUsrID, 
				  const TCHAR* szOPasswd2,
				  const TCHAR* szPasswd, 
				  const TCHAR* szPasswd2,
				  const TCHAR* szCaptcha)
{
	sc::writeLogError("Do not call this function. IdnUserCheck");
	return sc::db::DB_ERROR;
}


int AdoManager::UserChangePass(
				   const TCHAR* szUsrID, 
				   const TCHAR* szOPasswd,
				   const TCHAR* szPasswd, 
				   const TCHAR* szPasswd2,
				   const TCHAR* szCaptcha)
{
	sc::writeLogError("Do not call this function. IdnUserCheck");
	return sc::db::DB_ERROR;
}


int AdoManager::UserResetPass(
				  const TCHAR* szUsrID, 
				  const TCHAR* szPasswd2,
				  const TCHAR* szCaptcha)
{
	sc::writeLogError("Do not call this function. IdnUserCheck");
	return sc::db::DB_ERROR;
}


int AdoManager::UserResetPass2(
				   const TCHAR* szUsrID, 
				   const TCHAR* szPasswd, 
				   const TCHAR* szPasswd2,
				   const TCHAR* szCaptcha)
{
	sc::writeLogError("Do not call this function. IdnUserCheck");
	return sc::db::DB_ERROR;
}


int AdoManager::UserResetPin(
				 const TCHAR* szUsrID, 
				 const TCHAR* szUsrEmail,
				 const TCHAR* szPasswd, 
				 const TCHAR* szPasswd2,
				 const TCHAR* szCaptcha)
{
	sc::writeLogError("Do not call this function. IdnUserCheck");
	return sc::db::DB_ERROR;
}



int AdoManager::UserGameTimeCVT(
					const TCHAR* szUsrID, 
					const DWORD dwGameTime,
					const TCHAR* szCaptcha)
{
	sc::writeLogError("Do not call this function. IdnUserCheck");
	return sc::db::DB_ERROR;
}


int AdoManager::UserPremiumPointCVT(
						const TCHAR* szUsrID, 
						const TCHAR* szValue,
						const TCHAR* szCaptcha)
{
	sc::writeLogError("Do not call this function. IdnUserCheck");
	return sc::db::DB_ERROR;
}


int AdoManager::UserTopUp(
			  const TCHAR* szUsrID, 
			  const TCHAR* szCode, 
			  const TCHAR* szPin,
			  const TCHAR* szCaptcha)
{
	sc::writeLogError("Do not call this function. IdnUserCheck");
	return sc::db::DB_ERROR;
}


int AdoManager::IdnUserCheck(int nUserNum, const std::string &strId, const std::string &strIP, int nSvrGrp, int nSvrNum)
{
	sc::writeLogError("Do not call this function. IdnUserCheck");
    return sc::db::DB_ERROR;
}


int AdoManager::GetUserInfo(const TCHAR* szUserID, NET_LOGIN_FEEDBACK_DATA2* pMsg)
{
    sc::writeLogError("Do not call this function. GetUserInfo");
    return sc::db::DB_ERROR;
}

USER_INFO_BASIC AdoManager::GetUserInfo(const TCHAR* szUsrId, const TCHAR* szPasswd )
{
    sc::writeLogError("Do not call this function. GetUserInfo");
    USER_INFO_BASIC uib;
    return uib;
}

int AdoManager::IdnGetUserInfo(const std::string &strId, NET_LOGIN_FEEDBACK_DATA2* pMsg)
{
	sc::writeLogError("Do not call this function. IdnGetUserInfo");
    return sc::db::DB_ERROR;
}

int AdoManager::DaumUserCheck(
    TCHAR* szDaumGID,
    const TCHAR* szUUID,
    const TCHAR* szUserIP,
    int nSvrGrp,
    int nSvrNum,
    int& Age OUT)
{
    sc::writeLogError("Do not call this function. DaumUserCheck");
    return sc::db::DB_ERROR;
}

int AdoManager::DaumGetUserInfo(const TCHAR* szGID, DAUM_NET_LOGIN_FEEDBACK_DATA2* pMsg)
{
    sc::writeLogError("Do not call this function. DaumGetUserInfo");
    return sc::db::DB_ERROR;
}

int AdoManager::GspUserCheck(
    TCHAR* szGspUserID,
    const TCHAR* szUUID,
    const TCHAR* szUserIP,
    int nSvrGrp,
    int nSvrNum)
{
    sc::writeLogError("Do not call this function. GspUserCheck");
    return sc::db::DB_ERROR;
}

int AdoManager::GspGetUserInfo(const TCHAR* szUserID, GSP_NET_LOGIN_FEEDBACK_DATA2* pMsg)
{
    sc::writeLogError("Do not call this function. GspGetUserInfo");
    return sc::db::DB_ERROR;
}

int AdoManager::TerraUserCheck(
    TCHAR* szTerraLoginName,
    const TCHAR* szTerraDecodedTID,
    const TCHAR* szUsrIP,
    int nSvrGrp,
    int nSvrNum )			
{
    sc::writeLogError("Do not call this function. TerraUserCheck");
    return sc::db::DB_ERROR;
}

int AdoManager::TerraGetUserInfo(const TCHAR* szTLoginName, TERRA_NET_LOGIN_FEEDBACK_DATA2* pMsg)
{
    sc::writeLogError("Do not call this function. TerraGetUserInfo");
    return sc::db::DB_ERROR;
}

int AdoManager::GsUserCheck(
    const TCHAR* szUsrID,
    const TCHAR* szPasswd,
    const TCHAR* szUsrIP,
    int nSvrGrp,
    int nSvrNum)			
{
    sc::writeLogError("Do not call this function. GsUserCheck");
    return sc::db::DB_ERROR;
}

int AdoManager::GsGetUserInfo(const TCHAR* szUserID, GS_NET_LOGIN_FEEDBACK_DATA2* pMsg)
{
    sc::writeLogError("Do not call this function. GsGetUserInfo");
    return sc::db::DB_ERROR;
}

int AdoManager::GmUserCheck(
							const TCHAR* szUsrID,
							const TCHAR* szPasswd,
							const TCHAR* szUsrIP,
							int nSvrGrp,
							int nSvrNum )			
{
	sc::writeLogError( "Do not call this function. GmUserCheck" );
	return sc::db::DB_ERROR;
}

int AdoManager::GmGetUserInfo( const TCHAR* szUserID, GM_NET_LOGIN_FEEDBACK_DATA2* pMsg )
{
	sc::writeLogError( "Do not call this function. GmGetUserInfo" );
	return sc::db::DB_ERROR;
}

int AdoManager::UsUserCheck(
							const TCHAR* szUsrID,
							const TCHAR* szPasswd,
							const TCHAR* szUsrIP,
							int nSvrGrp,
							int nSvrNum )			
{
	sc::writeLogError( "Do not call this function. UsUserCheck" );
	return sc::db::DB_ERROR;
}

int AdoManager::UsGetUserInfo( const TCHAR* szUserID, US_NET_LOGIN_FEEDBACK_DATA2* pMsg )
{
	sc::writeLogError( "Do not call this function. UsGetUserInfo" );
	return sc::db::DB_ERROR;
}

int AdoManager::ExciteUserCheck(
    const TCHAR* szUserID,
    const TCHAR* szUserIP, 
    int nSvrGrp, 
    int nSvrNum)			
{
    sc::writeLogError("Do not call this function. ExciteUserCheck");
    return sc::db::DB_ERROR;
}

int AdoManager::ExciteGetUserInfo(
    const TCHAR* szUserID,
    EXCITE_NET_LOGIN_FEEDBACK_DATA2* pMsg)
{
    sc::writeLogError("Do not call this function. ExciteGetUserInfo");
    return sc::db::DB_ERROR;
}

int AdoManager::JapanUserCheck(
    const TCHAR* szUserID,
    const TCHAR* szUserIP, 
    int nSvrGrp, 
    int nSvrNum,
    int nUserNum)			
{
    sc::writeLogError("Do not call this function. JapanUserCheck");
    return sc::db::DB_ERROR;
}

int AdoManager::JapanGetUserInfo(const TCHAR* szUserID, JAPAN_NET_LOGIN_FEEDBACK_DATA2* pMsg)
{
    sc::writeLogError("Do not call this function. JapanGetUserInfo");
    return sc::db::DB_ERROR;
}

int AdoManager::UserLogoutSvr(int nSvrGrp, int nSvrNum)
{
    sc::writeLogError("Do not call this function. UserLogoutSvr");
    return sc::db::DB_ERROR;
}

int AdoManager::UserBlock(int nUserNum, int nDay)	
{
    sc::writeLogError("Do not call this function. UserBlock");
    return sc::db::DB_ERROR;
}

int AdoManager::UserBlockDate(int nUserNum, __time64_t tBlockTime)
{
    sc::writeLogError("Do not call this function. UserBlockDate");
    return sc::db::DB_ERROR;
}

WORD AdoManager::ThaiGetTodayPlayTime(int nUserNum)
{
    sc::writeLogError("Do not call this function. ThaiGetTodayPlayTime");
    return sc::db::DB_ERROR;
}

int AdoManager::UserLogoutSimple3(const TCHAR* szUsrID, int nSvrGrp)
{
    sc::writeLogError("Do not call this function. UserLogoutSimple3");
    return sc::db::DB_ERROR;
}

int AdoManager::AllUserLogout()
{
    sc::writeLogError("Do not call this function. AllUserLogout");
    return sc::db::DB_ERROR;
}

int AdoManager::GetLogItem(std::vector<DWORD>& LogItemData)
{
    sc::db::AdoExt Ado(m_UserDBConnString);
    if (!Ado.Execute("SELECT ItemMain, ItemSub FROM dbo.LogItemData WHERE ItemLog=1"))
        return sc::db::DB_ERROR;

    if (Ado.GetEOF())
    {
        Ado.PRINT("ADO:GetLogItem There's no data. RecordSet is Empty.");
        return sc::db::DB_ERROR;
    }

    int ItemMid = 0;
    int ItemSid = 0;
    int ItemPrice  = 0;
    do
    {            
        Ado.GetCollect(0, ItemMid);
        Ado.GetCollect(1, ItemSid);        
        if (!Ado.IsCollectedAll())
            return sc::db::DB_ERROR;

        SNATIVEID ItemID(ItemMid, ItemSid);
        LogItemData.push_back(ItemID.Id());
    } while (Ado.Next());

    return sc::db::DB_OK;
}

int AdoManager::UserDbLogItemDataInsertUpdate(int ItemMid, int ItemSid, const std::string& ItemName)
{
    std::string ItemNameTemp(ItemName);
    sc::string::replaceForDb(ItemNameTemp);
    char szItemName[100] = {0};
    size_t NameLen = ItemNameTemp.length();
    StringCchCopy(szItemName, 100, ItemNameTemp.c_str());

    sc::db::AdoExt Ado(m_UserDBConnString);
    APPEND_IPARAM_INT    (Ado, "@ItemMain", ItemMid);
    APPEND_IPARAM_INT    (Ado, "@ItemSub",  ItemSid);
    APPEND_IPARAM_VARCHAR(Ado, "@ItemName", szItemName, static_cast<long> (NameLen));

    return ExecuteStoredProcedure("dbo.LogItemDataInsertUpdate", Ado);
}

int AdoManager::WorldBattleUserCheck(
	const std::string& UserId,
	const std::string& UserPass,
	const std::string& UserIp,
	int ServerGroup,
	int ServerNumber)
{
	std::string TempUserId(UserId);
    sc::string::replaceForDb(TempUserId);
	std::string TempUserPass(UserPass);
    sc::string::replaceForDb(TempUserPass);
	std::string TempUserIp(UserIp);
    sc::string::replaceForDb(TempUserIp);

	// dbo.WorldBattleUserVerify
	// @userId		varchar(20),
	// @userPass 	varchar(65),
	// @userIp		varchar(25),
	// @SvrGrpNum	int,
	// @SvrNum		int,	
	// @nReturn 	int	OUTPUT
    const long UserIdLength = 20;
    const long UserPassLength = 65;
    const long UserIpLength = 25;

    char szUserId[UserIdLength] = {0};
    char szUserPass[UserPassLength] = {0};
    char szUserIp[UserIpLength] = {0};

    StringCchCopy(szUserId, UserIdLength, TempUserId.c_str());
    StringCchCopy(szUserPass, UserPassLength, TempUserPass.c_str());
    StringCchCopy(szUserIp, UserIpLength, TempUserIp.c_str());
    
	sc::db::AdoExt Ado(m_UserDBConnString);
	APPEND_RPARAM_INT(Ado);
	
	APPEND_IPARAM_VARCHAR(Ado, "@userId",    szUserId,   UserIdLength-1);
	APPEND_IPARAM_VARCHAR(Ado, "@userPass",  szUserPass, UserPassLength-1);
	APPEND_IPARAM_VARCHAR(Ado, "@userIp",    szUserIp,   UserIpLength-1);
	APPEND_IPARAM_INT    (Ado, "@SvrGrpNum", ServerGroup);
	APPEND_IPARAM_INT    (Ado, "@SvrNum",    ServerNumber);

	APPEND_OPARAM_INT    (Ado, "@nReturn");

	return ExecuteStoredProcedureIntReturn("dbo.WorldBattleUserVerify", Ado, "@nReturn");
}

int AdoManager::WorldBattleGetUserInfo(const std::string& UserId, NET_LOGIN_FEEDBACK_DATA2& Msg)
{
    std::string TempUserId(UserId);
    sc::string::replaceForDb(TempUserId);
    
    // @userId		varchar(20),
    const long UserIdLength = 20;
    char szUserId[UserIdLength] = {0};
    StringCchCopy(szUserId, UserIdLength, TempUserId.c_str());

    sc::db::AdoExt Ado(m_UserDBConnString);
    APPEND_IPARAM_VARCHAR(Ado, "@userId", szUserId, UserIdLength-1);

    if (ExecuteStoredProcedure("dbo.WorldBattleGetUserInfo", Ado) != sc::db::DB_OK)
    {
        sc::writeLogError(std::string("AdoManager::WorldBattleGetUserInfo failure."));
        return sc::db::DB_ERROR;
    }

    int UserNum = 0;

    BEGIN_GETCOLLECT(Ado)    
    
    int UserType = 0;
    int UserCountry = 0;
    __time64_t PremiumTime = 0;
    USHORT ChaRemain = 0;
    USHORT ChaTestRemain = 0;
    __time64_t ChatBlockTime = 0;

    Ado.GetCollect("UserNum", UserNum);
    Ado.GetCollect("UserType", UserType);
    Ado.GetCollect("UserCountry", UserCountry);
    Ado.GetCollect("PremiumDate", PremiumTime);
    Ado.GetCollect("ChaRemain", ChaRemain);
    Ado.GetCollect("ChaTestRemain", ChaTestRemain);
    Ado.GetCollect("ChatBlockDate", ChatBlockTime);

    Msg.SetUserId(UserId.c_str());
    Msg.nUserNum = UserNum;
    Msg.nUserType = UserType;
    Msg.m_Country = static_cast<wb::EM_COUNTRY> (UserCountry);

    sc::time::AdjustTime(PremiumTime);
    Msg.tPremiumTime = PremiumTime;
    
    Msg.uChaRemain = ChaRemain;
    Msg.uChaTestRemain = ChaTestRemain;
    
    sc::time::AdjustTime(ChatBlockTime);
    Msg.tChatBlockTime = ChatBlockTime;
    
    END_GETCOLLECT(Ado)

    // 극강부 체크
    //ExtreamClassCheck(UserNum, Msg.nExtremeM, Msg.nExtremeW);

    return sc::db::DB_OK;
}

// 사용하지 않으므로 주석 처리 (2014.04.04)
//int AdoManager::ExtreamClassCheck(int UserDbNum, int& ExtreamM, int& ExtreamW)
//{
//    sc::db::AdoExt Ado(m_GameDBConnString);
//    APPEND_IPARAM_INT(Ado, "@nUserNum", UserDbNum);
//    
//    if (ExecuteStoredProcedure("dbo.sp_Extreme", Ado) != sc::db::DB_OK)
//    {
//        sc::writeLogError(std::string("AdoManager::ExtreamClassCheck failure."));
//        return sc::db::DB_ERROR;
//    }
//
//    BEGIN_GETCOLLECT(Ado)
//    
//    Ado.GetCollect(0, ExtreamM);
//    Ado.GetCollect(1, ExtreamW);
//
//    END_GETCOLLECT(Ado)
//
//    return sc::db::DB_OK;
//}

int AdoManager::UserCountryUpdate(int UserDbNum, wb::EM_COUNTRY Country)
{
    // dbo.WorldBattleUserCountryUpdate
    // @UserDbNum int
    // @UserCountry int    
    sc::db::AdoExt Ado(m_UserDBConnString);
    
    APPEND_IPARAM_INT(Ado, "@UserDbNum", UserDbNum);
    APPEND_IPARAM_INT(Ado, "@UserCountry", Country);

    if (ExecuteStoredProcedure("dbo.WorldBattleUserCountryUpdate", Ado) != sc::db::DB_OK)
    {
        sc::writeLogError(std::string("AdoManager::UserCountryUpdate failure."));
        return sc::db::DB_ERROR;
    }
    else
    {
        return sc::db::DB_OK;
    }
}

int	AdoManager::UserSystemInfoInsert( 
									 DWORD dwUserNum, 
									 DWORD dwCpuCoreNumber, 
									 DWORD dwCpuClock, 
									 DWORDLONG dwMemorySize,
									 DWORD dwResolutionX,
									 DWORD dwResolutionY,
									 DWORD dwGpuVendor,
									 DWORD dwGpuModel,
									 DWORD dwShaderModel,
									 int languageCode )
{
	/*
	@UserNum		int,
	@CpuCoreNum		int,
	@CpuCoreClock	int,
	@PhysicalMemory	bigint,
	@ResolutionX	int,
	@ResolutionY	int,
	@GpuVendor		int,
	@GpuModel		int,
	@ShaderModel	int,
	@LanguageCode	int
	*/

	sc::db::AdoExt Ado( m_UserDBConnString );

	APPEND_IPARAM_INT( Ado, "@UserNum", dwUserNum );
	APPEND_IPARAM_INT( Ado, "@CpuCoreNum", dwCpuCoreNumber );
	APPEND_IPARAM_INT( Ado, "@CpuCoreClock", dwCpuClock );
	APPEND_IPARAM_BIGINT( Ado, "@PhysicalMemory", dwMemorySize );
	APPEND_IPARAM_INT( Ado, "@ResolutionX", dwResolutionX );
	APPEND_IPARAM_INT( Ado, "@ResolutionY", dwResolutionY );
	APPEND_IPARAM_INT( Ado, "@GpuVendor", dwGpuVendor );
	APPEND_IPARAM_INT( Ado, "@GpuModel", dwGpuModel );
	APPEND_IPARAM_INT( Ado, "@ShaderModel", dwShaderModel );
	APPEND_IPARAM_INT( Ado, "@LanguageCode", languageCode );

	return ExecuteStoredProcedure( "dbo.sp_AddUserSystemInfo", Ado );
}

int AdoManager::WriteGMLog( int nGmNum, std::string& strCmd, std::string& strUserIP )
{
    /*
    PROCEDURE [dbo].[sp_gmEditLog]
    @nUserNum	int,
    @szGmCmd 	varChar(200),
    @userIP		varChar(50),
    @nReturn 	int	OUTPUT
    AS
    */
    sc::db::AdoExt Ado(m_UserDBConnString);


    APPEND_IPARAM_INT(Ado, "@nUserNum", nGmNum);
    APPEND_IPARAM_VARCHAR(Ado, "@szGmCmd", strCmd.c_str(), strCmd.length());
    APPEND_IPARAM_VARCHAR(Ado, "@userIP", strUserIP.c_str(), strUserIP.length());
    APPEND_OPARAM_INT(Ado, "@nReturn");

    return ExecuteStoredProcedure("dbo.sp_gmEditLog", Ado);
}


// 란모바일
int AdoManager::RanMobileRequest( DWORD dwUserDBNum, DWORD dwChaDBNum, int nServerNum, int nCommand, int nValue, const std::string& strChaName, int nChaClass )
{
	if( false == GLUseFeatures::GetInstance().IsUsingRanMobile() )
	{
		sc::writeLogError(std::string("RAN MOBILE option is closed.(ADODB::RanMobileRequest)"));	
		return sc::db::DB_OK;
	}

	sc::db::AdoExt Ado( m_RanMobileDBConnString );

	std::string TempChaName( strChaName );
	sc::string::replaceForDb( TempChaName );
	char szChaName[50] = { 0, };
	size_t nChaNameLen = TempChaName.length();
	StringCchCopy( szChaName, 50, TempChaName.c_str() );

	//std::vector<RAN_MOBILE_INFO> vecRanMobileChaInfo;
	GLMSG::SNET_RANMOBILE_COMMAND_RESPONSE MsgDS;
	MsgDS.dwUserDbNum = dwUserDBNum;
	MsgDS.dwChaDBNum = dwChaDBNum;
	MsgDS.nCommand = nCommand;
	MsgDS.nValue = nValue;

	int nCount = 0;
	_variant_t varStartData, varEndDate, varRewardDate;
	std::string strTempChaName;

	switch( nCommand )
	{
	case EM_RAN_MOBILE::EM_RAN_MOBILE_CREATE_GM:
	case EM_RAN_MOBILE::EM_RAN_MOBILE_CREATE_MONEY:
	case EM_RAN_MOBILE::EM_RAN_MOBILE_CREATE_CACHE:
		APPEND_IPARAM_INT	 ( Ado, "@nUserNum", static_cast<int>(dwUserDBNum) );
		APPEND_IPARAM_INT	 ( Ado, "@nChaNum", static_cast<int>(dwChaDBNum) );
		APPEND_IPARAM_INT	 ( Ado, "@nSvrNum", nServerNum );	
		APPEND_IPARAM_VARCHAR( Ado, "@szName", szChaName, static_cast<long>(nChaNameLen) );
		APPEND_IPARAM_INT	 ( Ado, "@nChaClass", nChaClass );
		APPEND_IPARAM_INT	 ( Ado, "@initExtendTime", nValue );
		APPEND_OPARAM_INT    ( Ado, "@nReturn");

		MsgDS.nResult = ExecuteStoredProcedureIntReturn( "dbo.sp_CreateRMCharacter", Ado, "@nReturn" );
		break;
	case EM_RAN_MOBILE::EM_RAN_MOBILE_DELETE_GM:
		APPEND_IPARAM_INT	 ( Ado, "@nUserNum", static_cast<int>(dwUserDBNum) );
		APPEND_IPARAM_INT	 ( Ado, "@nChaNum", static_cast<int>(dwChaDBNum) );
		APPEND_IPARAM_INT	 ( Ado, "@nSvrNum", nServerNum );	
		APPEND_OPARAM_INT    ( Ado, "@nReturn");

		MsgDS.nResult = ExecuteStoredProcedureIntReturn( "dbo.sp_DeleteRMCharacter", Ado, "@nReturn" );
		break;
	case EM_RAN_MOBILE::EM_RAN_MOBILE_INFO_GM: 
	case EM_RAN_MOBILE::EM_RAN_MOBILE_INFO: 
		{
			APPEND_IPARAM_INT	 ( Ado, "@nUserNum", static_cast<int>(dwUserDBNum) );
			APPEND_IPARAM_INT	 ( Ado, "@nChaNum", static_cast<int>(dwChaDBNum) );
			APPEND_IPARAM_INT	 ( Ado, "@nSvrNum", nServerNum );	
			//APPEND_OPARAM_INT    ( Ado, "@nReturn");

			//nRet = ExecuteStoredProcedureIntReturn( "dbo.sp_GetRMChaInfo", Ado, "@nReturn" );
			if (!Ado.ExecuteStoredProcedure("dbo.sp_GetRMChaInfo"))
			{
				sc::writeLogError(std::string("[RAN MOBILE DB JOB] DB_ERROR : sp_GetRMChaInfo"));
				return sc::db::DB_ERROR;
			}

			if (Ado.GetEOF())
			{
				sc::writeLogError(std::string("[RAN MOBILE DB JOB] DB NO DATA : sp_GetRMChaInfo"));
				return sc::db::DB_OK;
			}


			MsgDS.nResult = ERanMobileResult_OK;


			// Bind data
			Ado.GetCollect("ChaNum", MsgDS.sMobileInfo.dwChaNum);
			Ado.GetCollect("ChaName", strTempChaName);
			Ado.GetCollect("IsDelete", MsgDS.sMobileInfo.bDeleted);
			Ado.GetCollect("IfStartDate", varStartData );
			Ado.GetCollect("IfEndDate", varEndDate);
			Ado.GetCollect("RewardDate", varRewardDate );
			Ado.GetCollect("CurJewel", MsgDS.sMobileInfo.nRMJewel);
			Ado.GetCollect("CurPoint", MsgDS.sMobileInfo.nRMPoint);

			CTime cTemp1(sc::time::GetTime(varEndDate));
			MsgDS.sMobileInfo.tExpiredDate = cTemp1.GetTime();

			CTime cTemp2(sc::time::GetTime(varStartData));
			MsgDS.sMobileInfo.tStartData = cTemp2.GetTime();

			CTime cTemp3(sc::time::GetTime(varRewardDate));
			MsgDS.sMobileInfo.tRewardDate = cTemp3.GetTime();

			MsgDS.sMobileInfo.SetName(strTempChaName);
		}
		break;

	case EM_RAN_MOBILE::EM_RAN_MOBILE_INFO_ALL_GM: 
	case EM_RAN_MOBILE::EM_RAN_MOBILE_INFO_ALL: 
		{
			APPEND_IPARAM_INT	 ( Ado, "@nUserNum", static_cast<int>(dwUserDBNum) );
			APPEND_IPARAM_INT	 ( Ado, "@nChaNum", static_cast<int>(dwChaDBNum) );
			APPEND_IPARAM_INT	 ( Ado, "@nSvrNum", nServerNum );	
			//APPEND_OPARAM_INT    ( Ado, "@nReturn");

			//nRet = ExecuteStoredProcedureIntReturn( "dbo.sp_GetRMChaInfo", Ado, "@nReturn" );
			if (!Ado.ExecuteStoredProcedure("dbo.sp_GetRMChaInfoAll"))
			{
				sc::writeLogError(std::string("[RAN MOBILE DB JOB] DB_ERROR : sp_GetRMChaInfoAll"));
				return sc::db::DB_ERROR;
			}

			if (Ado.GetEOF())
			{
				sc::writeLogError(std::string("[RAN MOBILE DB JOB] DB NO DATA : sp_GetRMChaInfoAll"));
				return sc::db::DB_OK;
			}

			MsgDS.nResult = ERanMobileResult_OK;

			do
			{
				//!! 캐릭터 선택창에서 정보가 보여야하기 때문에 캐릭터 개수만큼 나눠서 보내주도록 수정함.
				// 연동가능 캐릭터 최대숫자
				/*if( MsgDS.nMobileCount >= 10 )
				{
					sc::writeLogError(std::string("[RAN MOBILE DB JOB] DB ERROR COUNT : sp_GetRMChaInfoAll"));
					MsgDS.nResult = sc::db::DB_ERROR;
					break;
				}*/

				// Bind data
				Ado.GetCollect("ChaNum", MsgDS.sMobileInfo.dwChaNum);
				Ado.GetCollect("ChaName", strTempChaName);
				Ado.GetCollect("IsDelete", MsgDS.sMobileInfo.bDeleted);
				Ado.GetCollect("IfStartDate", varStartData );
				Ado.GetCollect("IfEndDate", varEndDate);
				Ado.GetCollect("CurJewel", MsgDS.sMobileInfo.nRMJewel);
				Ado.GetCollect("CurPoint", MsgDS.sMobileInfo.nRMPoint);

				CTime cTemp1(sc::time::GetTime(varEndDate));
				MsgDS.sMobileInfo.tExpiredDate = cTemp1.GetTime();

				CTime cTemp2(sc::time::GetTime(varStartData));
				MsgDS.sMobileInfo.tStartData = cTemp2.GetTime();

				MsgDS.sMobileInfo.SetName(strTempChaName);

				if( NULL != m_pServer )
					m_pServer->InsertMsg(DBACTION_CLIENT_ID, &MsgDS);

			} while (Ado.Next());

			if( MsgDS.nResult == sc::db::DB_ERROR ) 
			{
				sc::writeLogError(std::string("[RAN MOBILE DB JOB] DB_ERROR"));
				return sc::db::DB_ERROR;
			}

			MsgDS.nCommand = EM_RAN_MOBILE::EM_RAN_MOBILE_INFO_ALL_END;
		}
		break;

	case EM_RAN_MOBILE::EM_RAN_MOBILE_GET_POINT_GM:
	case EM_RAN_MOBILE::EM_RAN_MOBILE_GET_POINT:
		APPEND_IPARAM_INT	 ( Ado, "@nChaNum", static_cast<int>(dwChaDBNum) );
		APPEND_IPARAM_INT	 ( Ado, "@nSvrNum", nServerNum );	
		APPEND_OPARAM_INT    ( Ado, "@nReturn");

		MsgDS.nResult = ExecuteStoredProcedureIntReturn( "dbo.sp_GetCurRMPoint", Ado, "@nReturn" );

		break;

	case EM_RAN_MOBILE::EM_RAN_MOBILE_GET_JEWEL_GM:
		APPEND_IPARAM_INT	 ( Ado, "@nUserNum", static_cast<int>(dwUserDBNum) );
		APPEND_OPARAM_INT    ( Ado, "@nReturn");

		MsgDS.nResult = ExecuteStoredProcedureIntReturn( "dbo.sp_GetCurRMJewel", Ado, "@nReturn" );
		break;

	case EM_RAN_MOBILE::EM_RAN_MOBILE_UPDATE_DATE_GM:
	case EM_RAN_MOBILE::EM_RAN_MOBILE_EXTEND_TIME:
		APPEND_IPARAM_INT	 ( Ado, "@nChaNum", static_cast<int>(dwChaDBNum) );
		APPEND_IPARAM_INT	 ( Ado, "@nSvrNum", nServerNum );	
		APPEND_IPARAM_INT	 ( Ado, "@nTime", nValue );
		APPEND_OPARAM_INT    ( Ado, "@nReturn");

		MsgDS.nResult = ExecuteStoredProcedureIntReturn( "dbo.sp_UpdateExpiredDate", Ado, "@nReturn" );
		break;

	case EM_RAN_MOBILE::EM_RAN_MOBILE_GET_DAILY_ITEM:
		APPEND_IPARAM_INT	 ( Ado, "@nUserNum", static_cast<int>(dwUserDBNum) );
		APPEND_OPARAM_INT    ( Ado, "@nReturn");

		MsgDS.nResult = ExecuteStoredProcedureIntReturn( "dbo.sp_UpdateRewardDate", Ado, "@nReturn" );
		if( sc::db::DB_OK == MsgDS.nResult )
		{
			MsgDS.nResult = ERanMobileResult_OK;
			Ado.GetCollect( "DailyRewardDate", varRewardDate );

			CTime cTemp1(sc::time::GetTime(varRewardDate));
			MsgDS.sMobileInfo.tRewardDate = cTemp1.GetTime();
		}

		break;

	//case EM_RAN_MOBILE::EM_RAN_MOBILE_GET_POINT:
	//	APPEND_IPARAM_INT	 ( Ado, "@nUserNum", static_cast<int>(dwUserDBNum) );
	//	APPEND_IPARAM_INT	 ( Ado, "@nChaNum", static_cast<int>(dwChaDBNum) );
	//	APPEND_IPARAM_INT	 ( Ado, "@nSvrNum", nServerNum );	
	//	APPEND_OPARAM_INT    ( Ado, "@nReturn");

	//	MsgDS.nResult = ExecuteStoredProcedureIntReturn( "dbo.sp_UpdateRMPointAll", Ado, "@nReturn" );
	//	break;

	case EM_RAN_MOBILE::EM_RAN_MOBILE_UPDATE_POINT_GM:
	case EM_RAN_MOBILE::EM_RAN_MOBILE_CHANGE_ITEM:
		{
			APPEND_IPARAM_INT	 ( Ado, "@nUserNum", static_cast<int>(dwUserDBNum) );
			APPEND_IPARAM_INT	 ( Ado, "@nChaNum", static_cast<int>(dwChaDBNum) );
			APPEND_IPARAM_INT	 ( Ado, "@nSvrNum", nServerNum );	
			APPEND_IPARAM_INT	 ( Ado, "@nMinusPoint", nValue );
			APPEND_OPARAM_INT    ( Ado, "@nReturn");

			int nResult = ExecuteStoredProcedureIntReturn( "dbo.sp_UpdateRMPoint", Ado, "@nReturn" );
			if( nValue == nResult )
				MsgDS.nResult = ERanMobileResult_OK;
			else
				MsgDS.nResult = nResult;
		}
		break;

	case EM_RAN_MOBILE::EM_RAN_MOBILE_UPDATE_JEWEL_GM:
	case EM_RAN_MOBILE::EM_RAN_MOBILE_SEND_JEWEL:
		APPEND_IPARAM_INT	 ( Ado, "@nUserNum", static_cast<int>(dwUserDBNum) );
		APPEND_IPARAM_INT	 ( Ado, "@nPlusJewel", nValue );
		APPEND_OPARAM_INT    ( Ado, "@nReturn");

		MsgDS.nResult = ExecuteStoredProcedureIntReturn( "dbo.sp_UpdateRMJewel", Ado, "@nReturn" );
		break;
	}

	sc::writeLogError(sc::string::format("[RAN MOBILE DB JOB] UserDBNum %1%, ChaDBNum %2%, SvrNum %3%, Command %4%, nValue %5%, ChaName %6%, ChaClass %7%, RET %8%",  dwUserDBNum, dwChaDBNum, nServerNum, nCommand, nValue, strChaName, nChaClass, MsgDS.nResult) );

	if( NULL != m_pServer )
	{
		m_pServer->InsertMsg(DBACTION_CLIENT_ID, &MsgDS);
	}

	return sc::db::DB_OK;
}

} // namespace db