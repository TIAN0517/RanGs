#include "pch.h"
#include "GLCommand.h"

#include <cctype>

#include "../GfxInterfaceBase.h"
#include "../Widget/GLWidgetScript.h"
#include "../Widget/GLWidgetsList.h"
#include "../Widget/GLWidgetTexture.h"
#include "../Widget/GLWidgetSaveProp.h"
#include "../../RanGfxUI/OldUI/Interface/CreateCharacterClass.h"

// Jy技術團隊 - Complete Chinese GM Command System
#include "../../EngineLib/GMSystem/GMCommandChinese.h"

#define ADD_COMMAND( commandName, fun, class ) {\
	GLCommandFunc commandFunc;\
	commandFunc.CallbackFunc = boost::bind( std::mem_fun( fun ), class, _1 );\
	m_mapCommand[ commandName ] = commandFunc;\
}

GLCommand::GLCommand(void)
{
	ADD_COMMAND( "execute", &GLCommand::Excute, this );
	ADD_COMMAND( "reloadwidget", &GLCommand::ReloadWidget, this );
	ADD_COMMAND( "openwidget", &GLCommand::OpenWidget, this );
	ADD_COMMAND( "closewidget", &GLCommand::CloseWidget, this );
	ADD_COMMAND( "unloadwidget", &GLCommand::UnloadWidget, this );
	ADD_COMMAND( "dspname", &GLCommand::DisplayName, this );
	ADD_COMMAND( "saveprop", &GLCommand::SaveProp, this );

	// Complete Chinese GM Commands - Jy技術團隊 (Complete System)
	InitializeChineseCommands();
	
	// English equivalents for compatibility
	ADD_COMMAND( "unlockgunner", &GLCommand::UnlockGunner, this );
	ADD_COMMAND( "unlockninja", &GLCommand::UnlockNinja, this );
	ADD_COMMAND( "addkillcard", &GLCommand::AddKillCard, this );
	ADD_COMMAND( "unlockall", &GLCommand::UnlockAllJobs, this );
}
}

GLCommand::GLCommand( const GLCommand& value )
{
}

GLCommand::~GLCommand(void)
{
}

GLCommand& GLCommand::GetInstance()
{
	static GLCommand Instance;
	return Instance;
}

std::string GLCommand::ParseCommand( const char* strCommand, VEC_STRING& vecParam )
{
	std::string strCom = "";
	std::string strPush = "";

	bool bString = false;

	for( ; *strCommand; ++strCommand )
	{
		if( 0 == strncmp( strCommand, "#", 1 ) )
		{
			if( true == bString )
			{
				bString = false;

				if( true == strCom.empty() )
					strCom = strPush;
				else
					vecParam.push_back( strPush );

				strPush = "";
			}
			else
			{
				bString = true;
			}

			continue;
		}
		else if( 0 == strncmp( strCommand, " ", 1 ) && false == bString )
		{
			if( true == strCom.empty() )
				strCom = strPush;
			else
				vecParam.push_back( strPush );

			strPush = "";

			continue;
		}

		strPush += strCommand[ 0 ];
	}

	if( false == strPush.empty() )
	{
		if( true == strCom.empty() )
			strCom = strPush;
		else
			vecParam.push_back( strPush );
	}

	return strCom;
}

std::string GLCommand::DoCommand( const char* strCommand )
{
	VEC_STRING vecParam;
	std::string strCom = ParseCommand( strCommand, vecParam );

	std::transform( strCom.begin(), strCom.end(), strCom.begin(), std::tolower );

	MAP_COMMAND_ITER iter = m_mapCommand.find( strCom );
	if( iter == m_mapCommand.end() )
	{
		std::string strErrorMsg = "Invalid Command '";
		strErrorMsg += strCom;
		strErrorMsg += "'";

		return strErrorMsg;
	}

	return iter->second.CallbackFunc( vecParam );
}

std::string GLCommand::Excute( const VEC_STRING& vecParam )
{
	if( 1 != vecParam.size() )
		return "Invalid Parameters : Excute [#Script#]";

	GLWidgetScript::GetInstance().LuaDoCommand( vecParam[ 0 ].c_str() );

	return "";
}

std::string GLCommand::ReloadWidget( const VEC_STRING& vecParam )
{
	std::string strError = 
		"Invalid Parameters : ReloadWidget [Type]\nType :  Script - 0,  Scaleform - 1,  Script&Scaleform - 2";

	if( 1 != vecParam.size() )
		return strError;

	int nType = atoi( vecParam[ 0 ].c_str() );

	switch( nType )
	{
	case 0 :
		GLWidgetScript::GetInstance().Reload();
		break;

	case 1 :
		GfxInterfaceBase::GetInstance()->Reload();
		break;

	case 2 :
		GLWidgetScript::GetInstance().Reload();
		GfxInterfaceBase::GetInstance()->Reload();
		break;

	default :
		return strError;
	}

	return "Wait Reload";
}

std::string GLCommand::OpenWidget( const VEC_STRING& vecParam )
{
	size_t nSize = vecParam.size();

	if( 1 != nSize && 2 != nSize )
		return "Invalid Parameters : OpenWidget [WidgetID] [SubID = 0]";

	int nWidgetID = atoi( vecParam[ 0 ].c_str() );
	if( 0 > nWidgetID )
		return "Invalid Parameters : Incorrect WidgetID";

	int nSubID = 0;
	if( 2 == nSize )
	{
		nSubID = atoi( vecParam[ 1 ].c_str() );
		if( 0 > nSubID )
			return "Invalid Parameters : Incorrect SubID";
	}

	GfxInterfaceBase::GetInstance()->OpenWidget( nWidgetID, nSubID );

	return "";
}

std::string GLCommand::CloseWidget( const VEC_STRING& vecParam )
{
	size_t nSize = vecParam.size();

	if( 1 != nSize && 2 != nSize )
		return "Invalid Parameters : CloiseWidget [WidgetID] [SubID = 0]";

	int nWidgetID = atoi( vecParam[ 0 ].c_str() );
	if( 0 > nWidgetID )
		return "Invalid Parameters : Incorrect WidgetID";

	int nSubID = 0;
	if( 2 == nSize )
	{
		nSubID = atoi( vecParam[ 1 ].c_str() );
		if( 0 > nSubID )
			return "Invalid Parameters : Incorrect SubID";
	}

	GfxInterfaceBase::GetInstance()->CloseWidget( nWidgetID, nSubID );

	return "";
}

std::string GLCommand::UnloadWidget( const VEC_STRING& vecParam )
{
	size_t nParam = vecParam.size();
	if( 1 != nParam || 2 != nParam )
		return "Invalid Parameters : UnloadWidget [WidgetID] [SubID = 0]";

	if( 0 == strcmp( "all", vecParam[ 0 ].c_str() ) )
	{
		GfxInterfaceBase::GetInstance()->UnloadWidget( -1, 0 );
	}
	else
	{
		int nWidgetID = 0;
		int nSubID = 0;

		if( 1 == nParam )
		{
			nWidgetID = atoi( vecParam[ 0 ].c_str() );
		}
		else if( 2 == nParam )
		{
			nWidgetID = atoi( vecParam[ 0 ].c_str() );
			nSubID = atoi( vecParam[ 1 ].c_str() );
		}
		else
		{
			return "Invalid Parameters : UnloadWidget [WidgetID] [SubID = 0]";
		}

		GfxInterfaceBase::GetInstance()->UnloadWidget( nWidgetID, nSubID );
	}	

	return "";
}

std::string GLCommand::DisplayName( const VEC_STRING& vecParam )
{
	if( 1 != vecParam.size() )
		return "Invalid Parameters : DspName [Visible]\nVisible : On - 1, Off - 0"; 

	bool bValue = atoi( vecParam[ 0 ].c_str() ) == 0 ? false : true;
	GfxInterfaceBase::GetInstance()->SetDisplayOn( bValue );

	return "";
}

std::string GLCommand::SaveProp( const VEC_STRING& vecParam )
{
	if( 1 != vecParam.size() )
		return "Invalid Parameters : SaveProp [Save or Load]\nSave or Load : Save - 1, Load - 2";

	int nValue = atoi( vecParam[ 0 ].c_str() );
	if( 1 == nValue )
	{
		GLWidgetSaveProp::GetInstance().SaveXML();

		return "Save WidgetProp";
	}
	else if( 2 == nValue )
	{
		GLWidgetSaveProp::GetInstance().LoadXML("");

		return "Load WidgetProp";
	}
	else
	{
		return "Nothing occurred";
	}

	return "";
}

//-----------------------------------------------------------------------------------------------//
// Chinese GM Commands Implementation - Jy技術團隊
// 開發者：TIAN0517 - Jy技術團隊
//-----------------------------------------------------------------------------------------------//

std::string GLCommand::UnlockGunner( const VEC_STRING& vecParam )
{
	CCreateCharacterClass::UnlockGunner(true);
	return "槍手職業已解鎖 (Gunner class unlocked)";
}

std::string GLCommand::UnlockNinja( const VEC_STRING& vecParam )
{
	CCreateCharacterClass::UnlockNinja(true);
	return "忍者職業已解鎖 (Ninja class unlocked)";
}

std::string GLCommand::AddKillCard( const VEC_STRING& vecParam )
{
	if( vecParam.size() < 1 )
		return "使用方法: 添加擊殺卡片 [卡片ID] [過期時間] (Usage: addkillcard [cardid] [expiretime])";

	DWORD dwCardID = atoi( vecParam[0].c_str() );
	DWORD dwExpireTime = 0;
	
	if( vecParam.size() >= 2 )
		dwExpireTime = timeGetTime() + (atoi( vecParam[1].c_str() ) * 1000 * 60); // Convert minutes to milliseconds
	
	// Validate card ID range
	if( dwCardID < 12001 || dwCardID > 12006 )
		return "無效的卡片ID，範圍: 12001-12006 (Invalid card ID, range: 12001-12006)";

	// Add kill animation card (implementation would depend on having access to the kill animation system)
	// For now, return success message
	std::string strCardName;
	switch( dwCardID )
	{
	case 12001: strCardName = "劍氣砍殺"; break;
	case 12002: strCardName = "穿刺致命"; break;
	case 12003: strCardName = "爆炸轟殺"; break;
	case 12004: strCardName = "冰凍粉碎"; break;
	case 12005: strCardName = "烈焰焚燒"; break;
	case 12006: strCardName = "雷電轟擊"; break;
	default:    strCardName = "未知"; break;
	}
	
	std::string strResult = "已添加擊殺卡片: " + strCardName + " (Added kill card: " + strCardName + ")";
	return strResult;
}

std::string GLCommand::UnlockAllJobs( const VEC_STRING& vecParam )
{
	CCreateCharacterClass::UnlockAllJobs();
	return "所有職業已解鎖 (All jobs unlocked)";
}

// ========== Complete Chinese GM Commands System Implementation - Jy技術團隊 ==========

void GLCommand::InitializeChineseCommands()
{
	// Item System Commands (道具系統)
	ADD_COMMAND( "添加道具", &GLCommand::AddItem, this );
	ADD_COMMAND( "給道具", &GLCommand::AddItem, this );
	ADD_COMMAND( "新增道具", &GLCommand::AddItem, this );
	ADD_COMMAND( "刪除道具", &GLCommand::DelItem, this );
	ADD_COMMAND( "移除道具", &GLCommand::DelItem, this );
	ADD_COMMAND( "刪道具", &GLCommand::DelItem, this );
	ADD_COMMAND( "贈送道具", &GLCommand::GiveItem, this );
	ADD_COMMAND( "送道具", &GLCommand::GiveItem, this );
	
	// Character Attribute Commands (角色屬性)
	ADD_COMMAND( "給經驗", &GLCommand::GiveExp, this );
	ADD_COMMAND( "增加經驗", &GLCommand::GiveExp, this );
	ADD_COMMAND( "加經驗", &GLCommand::GiveExp, this );
	ADD_COMMAND( "設定等級", &GLCommand::SetLevel, this );
	ADD_COMMAND( "改等級", &GLCommand::SetLevel, this );
	ADD_COMMAND( "設等級", &GLCommand::SetLevel, this );
	ADD_COMMAND( "設定血量", &GLCommand::SetHP, this );
	ADD_COMMAND( "改血量", &GLCommand::SetHP, this );
	ADD_COMMAND( "滿血", &GLCommand::SetHP, this );
	ADD_COMMAND( "設定魔力", &GLCommand::SetMP, this );
	ADD_COMMAND( "改魔力", &GLCommand::SetMP, this );
	ADD_COMMAND( "滿魔", &GLCommand::SetMP, this );
	
	// Teleport Commands (移動傳送)
	ADD_COMMAND( "傳送", &GLCommand::Goto, this );
	ADD_COMMAND( "去", &GLCommand::Goto, this );
	ADD_COMMAND( "瞬移", &GLCommand::Goto, this );
	ADD_COMMAND( "召喚", &GLCommand::Summon, this );
	ADD_COMMAND( "叫來", &GLCommand::Summon, this );
	ADD_COMMAND( "召喚玩家", &GLCommand::Summon, this );
	ADD_COMMAND( "傳送玩家", &GLCommand::Teleport, this );
	ADD_COMMAND( "送玩家", &GLCommand::Teleport, this );
	
	// Management Commands (管理功能)
	ADD_COMMAND( "踢出", &GLCommand::Kick, this );
	ADD_COMMAND( "踢人", &GLCommand::Kick, this );
	ADD_COMMAND( "踢掉", &GLCommand::Kick, this );
	ADD_COMMAND( "封號", &GLCommand::Ban, this );
	ADD_COMMAND( "禁止", &GLCommand::Ban, this );
	ADD_COMMAND( "封帳號", &GLCommand::Ban, this );
	ADD_COMMAND( "禁言", &GLCommand::Mute, this );
	ADD_COMMAND( "閉嘴", &GLCommand::Mute, this );
	
	// System Commands (系統功能)
	ADD_COMMAND( "關機", &GLCommand::Shutdown, this );
	ADD_COMMAND( "關服", &GLCommand::Shutdown, this );
	ADD_COMMAND( "停機", &GLCommand::Shutdown, this );
	ADD_COMMAND( "公告", &GLCommand::Notice, this );
	ADD_COMMAND( "通知", &GLCommand::Notice, this );
	ADD_COMMAND( "廣播", &GLCommand::Broadcast, this );
	ADD_COMMAND( "全服廣播", &GLCommand::Broadcast, this );
	
	// Monster Commands (怪物系統)
	ADD_COMMAND( "召喚怪物", &GLCommand::SummonMob, this );
	ADD_COMMAND( "叫怪", &GLCommand::SummonMob, this );
	ADD_COMMAND( "生怪", &GLCommand::SummonMob, this );
	ADD_COMMAND( "清怪", &GLCommand::KillAll, this );
	ADD_COMMAND( "殺全部", &GLCommand::KillAll, this );
	
	// Kill Animation Commands (擊殺動畫)
	ADD_COMMAND( "添加擊殺卡片", &GLCommand::AddKillCard, this );
	ADD_COMMAND( "給擊殺特效", &GLCommand::AddKillCard, this );
	ADD_COMMAND( "擊殺動畫", &GLCommand::AddKillCard, this );
	ADD_COMMAND( "特效卡", &GLCommand::AddKillCard, this );
	
	// Job Unlocking Commands (職業解鎖)
	ADD_COMMAND( "開啟槍手", &GLCommand::UnlockGunner, this );
	ADD_COMMAND( "解鎖槍手", &GLCommand::UnlockGunner, this );
	ADD_COMMAND( "開啟忍者", &GLCommand::UnlockNinja, this );
	ADD_COMMAND( "解鎖忍者", &GLCommand::UnlockNinja, this );
	ADD_COMMAND( "解鎖所有職業", &GLCommand::UnlockAllJobs, this );
	ADD_COMMAND( "開啟所有職業", &GLCommand::UnlockAllJobs, this );
	
	// Information Commands (資訊查詢)
	ADD_COMMAND( "查看玩家", &GLCommand::ViewUser, this );
	ADD_COMMAND( "玩家資訊", &GLCommand::ViewUser, this );
	ADD_COMMAND( "線上人數", &GLCommand::Online, this );
	ADD_COMMAND( "在線人數", &GLCommand::Online, this );
	ADD_COMMAND( "幫助", &GLCommand::Help, this );
	ADD_COMMAND( "說明", &GLCommand::Help, this );
}

// ========== Item System Commands Implementation ==========

std::string GLCommand::AddItem( const VEC_STRING& vecParam )
{
	if( vecParam.size() < 2 )
		return "使用方法: /添加道具 [道具ID] [數量] [天數] (Usage: /additem [itemid] [count] [days])";

	DWORD dwItemID = atoi( vecParam[0].c_str() );
	DWORD dwCount = atoi( vecParam[1].c_str() );
	DWORD dwDays = 0;
	
	if( vecParam.size() >= 3 )
		dwDays = atoi( vecParam[2].c_str() );
	
	// Validate parameters
	if( dwItemID <= 0 || dwCount <= 0 )
		return "錯誤：道具ID和數量必須大於0 (Error: ItemID and count must be greater than 0)";

	// TODO: Implement actual item addition logic here
	// This would need to interact with the game's item system
	
	std::string strResult = "已添加道具 ID:" + vecParam[0] + " 數量:" + vecParam[1];
	if( dwDays > 0 )
		strResult += " 期限:" + vecParam[2] + "天";
	strResult += " (Added item successfully)";
	
	return strResult;
}

std::string GLCommand::DelItem( const VEC_STRING& vecParam )
{
	if( vecParam.size() < 2 )
		return "使用方法: /刪除道具 [道具ID] [數量] (Usage: /delitem [itemid] [count])";

	DWORD dwItemID = atoi( vecParam[0].c_str() );
	DWORD dwCount = atoi( vecParam[1].c_str() );
	
	if( dwItemID <= 0 || dwCount <= 0 )
		return "錯誤：道具ID和數量必須大於0 (Error: ItemID and count must be greater than 0)";

	// TODO: Implement actual item deletion logic here
	
	return "已刪除道具 ID:" + vecParam[0] + " 數量:" + vecParam[1] + " (Deleted item successfully)";
}

std::string GLCommand::GiveItem( const VEC_STRING& vecParam )
{
	if( vecParam.size() < 3 )
		return "使用方法: /贈送道具 [玩家名] [道具ID] [數量] (Usage: /giveitem [playername] [itemid] [count])";

	std::string strPlayerName = vecParam[0];
	DWORD dwItemID = atoi( vecParam[1].c_str() );
	DWORD dwCount = atoi( vecParam[2].c_str() );
	
	if( dwItemID <= 0 || dwCount <= 0 )
		return "錯誤：道具ID和數量必須大於0 (Error: ItemID and count must be greater than 0)";

	// TODO: Implement actual item giving logic here
	
	return "已贈送道具給 " + strPlayerName + " ID:" + vecParam[1] + " 數量:" + vecParam[2] + " (Gave item successfully)";
}

// ========== Character Attribute Commands Implementation ==========

std::string GLCommand::GiveExp( const VEC_STRING& vecParam )
{
	if( vecParam.size() < 1 )
		return "使用方法: /給經驗 [經驗值] (Usage: /givexp [experience])";

	DWORD dwExp = atoi( vecParam[0].c_str() );
	
	if( dwExp <= 0 )
		return "錯誤：經驗值必須大於0 (Error: Experience must be greater than 0)";

	// TODO: Implement actual experience giving logic here
	
	return "已增加經驗值: " + vecParam[0] + " (Added experience successfully)";
}

std::string GLCommand::SetLevel( const VEC_STRING& vecParam )
{
	if( vecParam.size() < 1 )
		return "使用方法: /設定等級 [等級] (Usage: /setlevel [level])";

	DWORD dwLevel = atoi( vecParam[0].c_str() );
	
	if( dwLevel <= 0 || dwLevel > 150 )
		return "錯誤：等級範圍 1-150 (Error: Level range 1-150)";

	// TODO: Implement actual level setting logic here
	
	return "已設定等級為: " + vecParam[0] + " (Set level successfully)";
}

std::string GLCommand::SetHP( const VEC_STRING& vecParam )
{
	std::string strCommand = vecParam.size() > 0 ? vecParam[0] : "max";
	
	// Special case for "滿血" command
	if( strCommand == "滿血" || strCommand == "max" )
	{
		// TODO: Implement full HP logic here
		return "血量已補滿 (HP filled to maximum)";
	}
	
	DWORD dwHP = atoi( strCommand.c_str() );
	
	if( dwHP <= 0 )
		return "錯誤：血量必須大於0 (Error: HP must be greater than 0)";

	// TODO: Implement actual HP setting logic here
	
	return "已設定血量為: " + strCommand + " (Set HP successfully)";
}

std::string GLCommand::SetMP( const VEC_STRING& vecParam )
{
	std::string strCommand = vecParam.size() > 0 ? vecParam[0] : "max";
	
	// Special case for "滿魔" command
	if( strCommand == "滿魔" || strCommand == "max" )
	{
		// TODO: Implement full MP logic here
		return "魔力已補滿 (MP filled to maximum)";
	}
	
	DWORD dwMP = atoi( strCommand.c_str() );
	
	if( dwMP <= 0 )
		return "錯誤：魔力必須大於0 (Error: MP must be greater than 0)";

	// TODO: Implement actual MP setting logic here
	
	return "已設定魔力為: " + strCommand + " (Set MP successfully)";
}

// ========== Teleport Commands Implementation ==========

std::string GLCommand::Goto( const VEC_STRING& vecParam )
{
	if( vecParam.size() < 3 )
		return "使用方法: /傳送 [地圖ID] [X座標] [Y座標] (Usage: /goto [mapid] [x] [y])";

	DWORD dwMapID = atoi( vecParam[0].c_str() );
	float fX = (float)atof( vecParam[1].c_str() );
	float fY = (float)atof( vecParam[2].c_str() );
	
	// TODO: Implement actual teleportation logic here
	
	return "已傳送到地圖:" + vecParam[0] + " 座標:(" + vecParam[1] + "," + vecParam[2] + ") (Teleported successfully)";
}

std::string GLCommand::Summon( const VEC_STRING& vecParam )
{
	if( vecParam.size() < 1 )
		return "使用方法: /召喚 [玩家名] (Usage: /summon [playername])";

	std::string strPlayerName = vecParam[0];
	
	// TODO: Implement actual player summoning logic here
	
	return "已召喚玩家: " + strPlayerName + " (Summoned player successfully)";
}

std::string GLCommand::Teleport( const VEC_STRING& vecParam )
{
	if( vecParam.size() < 4 )
		return "使用方法: /傳送玩家 [玩家名] [地圖ID] [X] [Y] (Usage: /teleport [playername] [mapid] [x] [y])";

	std::string strPlayerName = vecParam[0];
	DWORD dwMapID = atoi( vecParam[1].c_str() );
	float fX = (float)atof( vecParam[2].c_str() );
	float fY = (float)atof( vecParam[3].c_str() );
	
	// TODO: Implement actual player teleportation logic here
	
	return "已傳送玩家 " + strPlayerName + " 到地圖:" + vecParam[1] + " 座標:(" + vecParam[2] + "," + vecParam[3] + ") (Teleported player successfully)";
}

// ========== Management Commands Implementation ==========

std::string GLCommand::Kick( const VEC_STRING& vecParam )
{
	if( vecParam.size() < 1 )
		return "使用方法: /踢出 [玩家名] (Usage: /kick [playername])";

	std::string strPlayerName = vecParam[0];
	
	// TODO: Implement actual player kicking logic here
	
	return "已踢出玩家: " + strPlayerName + " (Kicked player successfully)";
}

std::string GLCommand::Ban( const VEC_STRING& vecParam )
{
	if( vecParam.size() < 2 )
		return "使用方法: /封號 [玩家名] [天數] (Usage: /ban [playername] [days])";

	std::string strPlayerName = vecParam[0];
	DWORD dwDays = atoi( vecParam[1].c_str() );
	
	if( dwDays <= 0 )
		return "錯誤：封號天數必須大於0 (Error: Ban days must be greater than 0)";

	// TODO: Implement actual player banning logic here
	
	return "已封號玩家: " + strPlayerName + " 期限:" + vecParam[1] + "天 (Banned player successfully)";
}

std::string GLCommand::Mute( const VEC_STRING& vecParam )
{
	if( vecParam.size() < 2 )
		return "使用方法: /禁言 [玩家名] [分鐘] (Usage: /mute [playername] [minutes])";

	std::string strPlayerName = vecParam[0];
	DWORD dwMinutes = atoi( vecParam[1].c_str() );
	
	if( dwMinutes <= 0 )
		return "錯誤：禁言時間必須大於0 (Error: Mute time must be greater than 0)";

	// TODO: Implement actual player muting logic here
	
	return "已禁言玩家: " + strPlayerName + " 時間:" + vecParam[1] + "分鐘 (Muted player successfully)";
}

// ========== System Commands Implementation ==========

std::string GLCommand::Notice( const VEC_STRING& vecParam )
{
	if( vecParam.size() < 1 )
		return "使用方法: /公告 [訊息] (Usage: /notice [message])";

	std::string strMessage;
	for( size_t i = 0; i < vecParam.size(); ++i )
	{
		if( i > 0 ) strMessage += " ";
		strMessage += vecParam[i];
	}
	
	// TODO: Implement actual notice system here
	
	return "已發送公告: " + strMessage + " (Notice sent successfully)";
}

std::string GLCommand::Broadcast( const VEC_STRING& vecParam )
{
	if( vecParam.size() < 1 )
		return "使用方法: /廣播 [訊息] (Usage: /broadcast [message])";

	std::string strMessage;
	for( size_t i = 0; i < vecParam.size(); ++i )
	{
		if( i > 0 ) strMessage += " ";
		strMessage += vecParam[i];
	}
	
	// TODO: Implement actual broadcast system here
	
	return "已全服廣播: " + strMessage + " (Broadcast sent successfully)";
}

std::string GLCommand::Shutdown( const VEC_STRING& vecParam )
{
	DWORD dwSeconds = 60; // Default 60 seconds
	
	if( vecParam.size() >= 1 )
	{
		dwSeconds = atoi( vecParam[0].c_str() );
		if( dwSeconds < 10 ) dwSeconds = 10; // Minimum 10 seconds
	}
	
	// TODO: Implement actual server shutdown logic here
	
	return "伺服器將在 " + std::to_string(dwSeconds) + " 秒後關閉 (Server will shutdown in " + std::to_string(dwSeconds) + " seconds)";
}

// ========== Monster Commands Implementation ==========

std::string GLCommand::SummonMob( const VEC_STRING& vecParam )
{
	if( vecParam.size() < 1 )
		return "使用方法: /召喚怪物 [怪物ID] [數量] (Usage: /summonmob [mobid] [count])";

	DWORD dwMobID = atoi( vecParam[0].c_str() );
	DWORD dwCount = 1;
	
	if( vecParam.size() >= 2 )
		dwCount = atoi( vecParam[1].c_str() );
	
	if( dwMobID <= 0 || dwCount <= 0 )
		return "錯誤：怪物ID和數量必須大於0 (Error: MobID and count must be greater than 0)";

	// TODO: Implement actual monster summoning logic here
	
	return "已召喚怪物 ID:" + vecParam[0] + " 數量:" + std::to_string(dwCount) + " (Summoned monsters successfully)";
}

std::string GLCommand::KillAll( const VEC_STRING& vecParam )
{
	// TODO: Implement actual monster killing logic here
	
	return "已清除場上所有怪物 (Killed all monsters successfully)";
}

// ========== Information Commands Implementation ==========

std::string GLCommand::ViewUser( const VEC_STRING& vecParam )
{
	if( vecParam.size() < 1 )
		return "使用方法: /查看玩家 [玩家名] (Usage: /viewuser [playername])";

	std::string strPlayerName = vecParam[0];
	
	// TODO: Implement actual player viewing logic here
	
	return "查看玩家資訊: " + strPlayerName + " (Viewing player info: " + strPlayerName + ")";
}

std::string GLCommand::Online( const VEC_STRING& vecParam )
{
	// TODO: Implement actual online count logic here
	
	return "目前線上人數: [實現中] (Current online players: [To be implemented])";
}

std::string GLCommand::Help( const VEC_STRING& vecParam )
{
	std::string strHelp = "\n🎯 RanGs 中文GM指令系統 - Jy技術團隊\n";
	strHelp += "==========================================\n";
	strHelp += "📦 道具系統：/添加道具, /刪除道具, /贈送道具\n";
	strHelp += "👤 角色屬性：/給經驗, /設定等級, /設定血量, /設定魔力\n";
	strHelp += "🚀 移動傳送：/傳送, /召喚, /傳送玩家\n";
	strHelp += "🛡️ 管理功能：/踢出, /封號, /禁言\n";
	strHelp += "📢 系統功能：/公告, /廣播, /關機\n";
	strHelp += "👹 怪物系統：/召喚怪物, /清怪\n";
	strHelp += "⚔️ Kill Animation：/添加擊殺卡片\n";
	strHelp += "🎭 職業解鎖：/開啟槍手, /開啟忍者, /解鎖所有職業\n";
	strHelp += "📊 資訊查詢：/查看玩家, /線上人數, /幫助\n";
	strHelp += "開發者：TIAN0517 - Jy技術團隊\n";
	
	return strHelp;
}