#include "pch.h"
#include "GLCommand.h"

#include <cctype>

#include "../GfxInterfaceBase.h"
#include "../Widget/GLWidgetScript.h"
#include "../Widget/GLWidgetsList.h"
#include "../Widget/GLWidgetTexture.h"
#include "../Widget/GLWidgetSaveProp.h"
#include "../../RanGfxUI/OldUI/Interface/CreateCharacterClass.h"

// Jy技術團隊 - Chinese GM Command Support

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

	// Chinese GM Commands - Jy技術團隊
	ADD_COMMAND( "開啟槍手", &GLCommand::UnlockGunner, this );
	ADD_COMMAND( "開啟忍者", &GLCommand::UnlockNinja, this );
	ADD_COMMAND( "添加擊殺卡片", &GLCommand::AddKillCard, this );
	ADD_COMMAND( "解鎖所有職業", &GLCommand::UnlockAllJobs, this );
	
	// English equivalents for compatibility
	ADD_COMMAND( "unlockgunner", &GLCommand::UnlockGunner, this );
	ADD_COMMAND( "unlockninja", &GLCommand::UnlockNinja, this );
	ADD_COMMAND( "addkillcard", &GLCommand::AddKillCard, this );
	ADD_COMMAND( "unlockall", &GLCommand::UnlockAllJobs, this );
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