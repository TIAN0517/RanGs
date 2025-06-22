#pragma once

#include <boost/function.hpp>
#include <boost/bind.hpp>

typedef std::vector< std::string >	VEC_STRING;
typedef VEC_STRING::iterator		VEC_STRING_ITER;
typedef VEC_STRING::const_iterator	VEC_STRING_CITER;

class GLCommandFunc
{
public :
	GLCommandFunc(void)
	{
		CallbackFunc = NULL;
	}

public :
	boost::function< std::string( const VEC_STRING& value ) > CallbackFunc;
};
typedef std::map< std::string, GLCommandFunc >	MAP_COMMAND;
typedef MAP_COMMAND::iterator					MAP_COMMAND_ITER;

class GLCommand
{
private :
	GLCommand(void);
	GLCommand( const GLCommand& value );
	~GLCommand(void);

public :
	static GLCommand& GetInstance();

private :
	MAP_COMMAND m_mapCommand;

private :
	// ���� ������ ���ɾ�� �Ķ���� �и� (���ɾ� ����)
	std::string ParseCommand( const char* strCommand, VEC_STRING& vecParam );

public :
	// ���� ���� (�ֿܼ� ��� �α� ����)
	std::string DoCommand( const char* strCommand );

	// ��� ���� ����
	std::string Excute( const VEC_STRING& vecParam );
	// ��ũ��Ʈ, �������� ���ε�
	std::string ReloadWidget( const VEC_STRING& vecParam );
	// UI ����
	std::string OpenWidget( const VEC_STRING& vecParam );
	// UI �ݱ�
	std::string CloseWidget( const VEC_STRING& vecParam );
	// UI ��ε�
	std::string UnloadWidget( const VEC_STRING& vecParam );
	// �̸� ���÷��� Ȱ��/��Ȱ��
	std::string DisplayName( const VEC_STRING& vecParam );
	// UI ������Ƽ ����/�ε�
	std::string SaveProp( const VEC_STRING& vecParam );

	// Complete Chinese GM Commands System - Jy技術團隊
	void InitializeChineseCommands();  // 初始化完整中文指令系統
	
	// Job Unlocking Commands
	std::string UnlockGunner( const VEC_STRING& vecParam );      // 開啟槍手
	std::string UnlockNinja( const VEC_STRING& vecParam );       // 開啟忍者
	std::string UnlockAllJobs( const VEC_STRING& vecParam );     // 解鎖所有職業
	
	// Kill Animation Commands
	std::string AddKillCard( const VEC_STRING& vecParam );       // 添加擊殺卡片
	
	// Item System Commands
	std::string AddItem( const VEC_STRING& vecParam );           // 添加道具
	std::string DelItem( const VEC_STRING& vecParam );           // 刪除道具
	std::string GiveItem( const VEC_STRING& vecParam );          // 贈送道具
	
	// Character Attribute Commands
	std::string GiveExp( const VEC_STRING& vecParam );           // 給經驗
	std::string SetLevel( const VEC_STRING& vecParam );          // 設定等級
	std::string SetHP( const VEC_STRING& vecParam );             // 設定血量
	std::string SetMP( const VEC_STRING& vecParam );             // 設定魔力
	
	// Teleport Commands
	std::string Goto( const VEC_STRING& vecParam );              // 傳送
	std::string Summon( const VEC_STRING& vecParam );            // 召喚
	std::string Teleport( const VEC_STRING& vecParam );          // 傳送玩家
	
	// Management Commands
	std::string Kick( const VEC_STRING& vecParam );              // 踢出
	std::string Ban( const VEC_STRING& vecParam );               // 封號
	std::string Mute( const VEC_STRING& vecParam );              // 禁言
	
	// System Commands
	std::string Notice( const VEC_STRING& vecParam );            // 公告
	std::string Broadcast( const VEC_STRING& vecParam );         // 廣播
	std::string Shutdown( const VEC_STRING& vecParam );          // 關機
	
	// Monster Commands
	std::string SummonMob( const VEC_STRING& vecParam );         // 召喚怪物
	std::string KillAll( const VEC_STRING& vecParam );           // 清怪
	
	// Information Commands
	std::string ViewUser( const VEC_STRING& vecParam );          // 查看玩家
	std::string Online( const VEC_STRING& vecParam );            // 線上人數
	std::string Help( const VEC_STRING& vecParam );              // 幫助
};
