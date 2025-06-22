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

	// Chinese GM Commands - Jy技術團隊
	std::string UnlockGunner( const VEC_STRING& vecParam );      // 開啟槍手
	std::string UnlockNinja( const VEC_STRING& vecParam );       // 開啟忍者
	std::string AddKillCard( const VEC_STRING& vecParam );       // 添加擊殺卡片
	std::string UnlockAllJobs( const VEC_STRING& vecParam );     // 解鎖所有職業
};
