#pragma once

#include "GLContrlBaseMsg.h"
#include "../InstanceSystem/GLInstanceSystem.h"
#include "../Party/GLPartyDefine.h"

namespace InstanceSystem
{
    enum EMMESSAGE_TYPE
    {
        EMMESSAGE_ERROR,					// error!;
        EMMESSAGE_GM_COMMAND,			    // GM ���ɾ�;
		EMMESSAGE_RELOAD_AF,				// do script Reload agent -> field;
		EMMESSAGE_RELOAD_FA,				// result script Reload field -> agent;
        EMMESSAGE_NOTIFY_PLAYER_POSITION,	// player�� ��ġ �˸�;
        EMMESSAGE_NOTIFY_INSTANCE_LOG,		// instance ��Ȳ;
        EMMESSAGE_NOTIFY_INSTANCE_LIST,		// instance ���;
        EMMESSAGE_NOTIFY_CHILD_LAND,		// instance �� ���Ե� land ID;
        EMMESSAGE_NOTIFY_PLAYER_LIST,		// instance �� player ���;
		EMMESSAGE_NOTIFY_GLOBAL_FLOAT,		// field instance script �� float global ���� ��;
		EMMESSAGE_NOTIFY_GLOBAL_INTEGER,	// field instance script �� integer global ���� ��;
		EMMESSAGE_NOTIFY_GLOBAL_BOOLEAN,	// field instance script �� boolean global ���� ��;
		EMMESSAGE_NOTIFY_GLOBAL_STRING,		// field instance script �� string global ���� ��;
        EMMESSAGE_MOVE_MAP,				    // player ���� �̵�;
        EMMESSAGE_CREATE,					// instance ����;
        EMMESSAGE_CREATE_COMPLETE,	        // instance ���� �Ϸ�;
        EMMESSAGE_DESTROY,					// instance ����;
        EMMESSAGE_JOIN,						// (��)����;
        EMMESSAGE_REQUEST_JOIN_AF,		    // field server instance�� ���� ���� ���� ���;
        EMMESSAGE_ANSWER_JOIN_FA,		    // EMMESSAGE_REQUEST_JOIN_AF�� ���� ����;
        EMMESSAGE_UI_CHATMSG,			    // ä��â�� �޽������(������);
		EMMESSAGE_UI_CHATXMLMSG,		    // ä��â�� XML �޼��� ���;
        EMMESSAGE_UI_SIMPLE_MSG,		    // ȭ�� �߾ӿ� �޽������(������);
        EMMESSAGE_UI_SIMPLE_XMLMSG,		    // ȭ�� �߾ӿ� XML �޼��� ���;
        EMMESSAGE_UI_TEXT_DLG_MSG,		    // ȭ�� �߻�ܿ� �ؽ�Ʈ ���̾�α� �޼��� ���(������);
        EMMESSAGE_UI_TEXT_DLG_XMLMSG,		    // ȭ�� �߻�ܿ� �ؽ�Ʈ ���̾�α� XML �޼��� ���;
        EMMESSAGE_UI_TIMER_MSGBOX,          // ȭ�� �߾ӿ� Ÿ�̸Ӱ� �ִ� Ok,Cancel �޽��� �ڽ� ���;
		EMMESSAGE_EFFECT,				    // ȭ�鿡 ����Ʈ�� ���;
		EMMESSAGE_MATCHING_MAIL_AF,		    // ���� ������
        EMMESSAGE_SET_POSITION,			    // Ư�� ��ġ�� �̵�(��� �̵�);
        EMMESSAGE_SET_POSITION_FA,		    // ���;
        EMMESSAGE_DO_MOVE_TO,			    // Ư�� ��ġ�� �̵�(�ɾ �̵�);
        EMMESSAGE_JOIN_LOCK_AF,			    // ���� ���(agent -> field);
        EMMESSAGE_JOIN_LOCK_FA,			    // ���� ���(field -> agent);
		EMMESSAGE_JOIN_SUSPEND_AF,		    // ���� ����(agent -> field);
		EMMESSAGE_JOIN_SUSPEND_FA,		    // ���� ����(field -> agent);
		EMMESSAGE_REQ_FACTOIN_SET_FACTION,	// Faction ����.				
        EMMESSAGE_INFORMATION_CHANGE,	    // �δ��� ���� ���� �������;
		EMMESSAGE_REQUEST_JOIN,			    // ���� ��û ���;		
		EMMESSAGE_REQUEST_OUT,			    // �δ����� �ѾƳ�;
        EMMESSAGE_CLICK_TRIGGER,		    // �δ� ��ũ��Ʈ�� Ŀ���� NPC Ŭ���̺�Ʈ �߻��� �ʵ忡 ��û;
        EMMESSAGE_SET_MOTION_BRD,		    // ĳ���� ���¸� Ư�� ��ǻ��·� ���� (field -> �ش� pc�� pc�ֺ����� BroadCast);
        EMMESSAGE_SET_ACT_STATE_BRD,	    // �ɸ��� ActState�� Ư�� ���·� ���� (field -> Client);
        EMMESSAGE_NEW_CUSTOM_MESSAGE,	    // �δ��� �� �δ��� �������� ����� ���� Ŀ���� �޽���;
		EMMESSAGE_MOVE_DONE,			    // �̵��� �Ϸ� �� ���Ŀ� field -> agent�� ������ �޼���;
        EMMESSAGE_JOIN_PARTY_FA,            // ��Ƽ ���� �� ��Ƽ�� �߰�;
        EMMESSAGE_OUT_PARTY_FA,             // ��Ƽ Ż��;
        EMMESSAGE_CLEAR_PARTY_FA,           // ��Ƽ �ػ�;
		EMMESSAGE_REQUEST_OBSERVE,		// ���� ��û;
		EMMESSAGE_SET_AUTHORITY,			// ���� ����(field -> agent);
		EMMESSAGE_NOTIFY_COUNT_PACKET,	// subType�� ���� ���ŵ� ��Ŷ ���� �˸�;
        EMMESSAGE_NSIZE,
    };

    enum EMCOMMAND_TYPE
    { // GM ���ɾ� ���;
        EMCOMMAND_WHO,					// player ��� ����;
        EMCOMMAND_WHERE,				// player ��ġ ����;
        EMCOMMAND_LAND,					// instance�� ���� land ��� ����;
		EMCOMMAND_DEBUG,				// agent/field ������ mapID �� Ȯ��;
//        EMCOMMAND_FACTION,				// player�� ���� ID ����;
        EMCOMMAND_LIST,					// instance ��� ����;
        EMCOMMAND_LOG,					// instance log ����;
        EMCOMMAND_DESTROY,			// Ư�� instance ����;
        EMCOMMAND_DESTROY_TYPE,	// �ش� type�� instance ��� ����;
        EMCOMMAND_CREATE,				// instance ����;
		EMCOMMAND_CREATE_TEST,		// instance ���� x n(���� �׽�Ʈ);
		EMCOMMAND_SET_GLOBAL,		// script ���� �� ����;
		EMCOMMAND_GET_GLOBAL,		// script ���� �� ��� ��;		
		EMCOMMAND_MOVE_TO_INSTANCE,	// instance -> instance �� Ư�� ��ҷ� �̵�;
		EMCOMMAND_MOVE_TO_GATE,			// instance -> instance �� Ư�� Gate�� �̵�;
		EMCOMMAND_MOVE_TO_FIELD,			// instance -> field �� Ư�� ��ҷ� �̵�;		
		EMCOMMAND_ENTER_TO_INSTANCE,	// field -> instance �� Ư�� ��ҷ� �̵�;
		EMCOMMAND_MOVE_TO,					// instance ���ο��� �������� Ư�� ��ġ�� �̵�;
		EMCOMMAND_DO_JOIN,				// Ư�� player�� �δ����� ���� ����;
		EMCOMMAND_DO_OUT,				// Ư�� player�� �δ����� �ѾƳ�;
        EMCOMMAND_OFF,					// instance ��;
        EMCOMMAND_ON,						// instance ��;
		EMCOMMAND_JOIN_SUSPEND,				// instance ���� ����;
		EMCOMMAND_JOIN_SUSPEND_TYPE,	// instance ���� ����;
        EMCOMMAND_JOIN_LOCK,			// instance ���� ���;
		EMCOMMAND_JOIN_LOCK_TYPE,	// �ش� ���� instnace ���� ���;
        EMCOMMAND_RELOAD,				// script �ٽ� �ҷ�����;
		EMCOMMAND_COUNT_PACKET,	// ���ŵ� ��Ŷ�� ����;
		
		// Kill Animation Commands - TIAN0517 Implementation
		EMCOMMAND_KILL_ANIM_SWORD,		// Play sword slash kill animation (12001);
		EMCOMMAND_KILL_ANIM_PIERCE,		// Play pierce fatal kill animation (12002);
		EMCOMMAND_KILL_ANIM_EXPLODE,	// Play explosion blast kill animation (12003);
		EMCOMMAND_KILL_ANIM_ICE,		// Play ice shatter kill animation (12004);
		EMCOMMAND_KILL_ANIM_FLAME,		// Play flame burn kill animation (12005);
		EMCOMMAND_KILL_ANIM_LIGHTNING,	// Play lightning strike kill animation (12006);
		EMCOMMAND_KILL_ANIM_TEST,		// Test all kill animations;
		EMCOMMAND_KILL_ANIM_RANDOM,		// Play random kill animation;
		
        EMCOMMAND_NSIZE,
    };

	// �ν��Ͻ� �ý��ۿ� ���� �� ��� ���� �ڵ�� �̰���;
	enum EMERROR_TYPE
	{
		EMERROR_CANNOT_JOIN_INSTANCE,
		EMERROR_CANNOT_JOIN_INSTANCE_IN_INSTANCE,
		EMERROR_MISMATCH_INFORMATION,
		EMERROR_ACCESS_DENIED_INSTANCE,
		EMERROR_CANNOT_CREATE_INSTANCE,
		EMERROR_CANNOT_CREATE_INSTANCE_IN_INSTANCE,
		EMERROR_RELOAD_SCRIPT,
		EMERROR_FAILED_TO_CREATE_INSTANCE,
		EMERROR_NOT_EXIST_INSTANCE,
		EMERROR_ALREADY_IN_INSTANCE,
		EMERROR_NOT_EXIST_TYPE_OF_INSTANCE,
		EMERROR_REJECT_JOIN,
        EMERROR_DESTROY_INSTANCE,
		EMERROR_FIELD_SERVER_OUT,
		EMERROR_INVALID_REQUEST_INFORMATION,
		EMERROR_NSIZE,
	};
}


namespace GLMSG
{
	// �ν��Ͻ� ���̽� �޼���;
	struct NET_INSTANCE_MESSAGE : public NET_MSG_GENERIC
	{
		const InstanceSystem::EMMESSAGE_TYPE emType;

		NET_INSTANCE_MESSAGE(const DWORD nSize, const InstanceSystem::EMMESSAGE_TYPE _emType)
			: NET_MSG_GENERIC(NET_MSG_GCTRL_INSTANCE_SYSTEM, nSize)
			, emType(_emType)
		{
		}
	};

    //! ���� : NET_INSTANCE_MSGPACK_MESSAGE�� ��ӹ��� ��� ����ü�� ���
    // MSGPACK_DEFINE ����� emType�� �� ù��°�� ���� ���Ŀ� �ڽ��� �ڷ����� �־�����Ѵ�.
    struct NET_INSTANCE_MSGPACK_MESSAGE // : public NET_MSG_GENERIC
    {
        DWORD emType;

        MSGPACK_DEFINE(emType);

        NET_INSTANCE_MSGPACK_MESSAGE()
            : emType(0)
        {
        }

        NET_INSTANCE_MSGPACK_MESSAGE(const InstanceSystem::EMMESSAGE_TYPE _emType)
            : emType(_emType)
        {
        }
    };

	//////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
    struct NET_INSTANCE_ERROR : public NET_INSTANCE_MESSAGE
    {
        enum
        {
            NSIZE = (NET_DATA_BUFSIZE - sizeof(NET_INSTANCE_MESSAGE) - sizeof(InstanceSystem::InstanceMapID) - sizeof(SNATIVEID) - sizeof(InstanceSystem::EMERROR_TYPE) - sizeof(DWORD)) / sizeof(DWORD),
            HEADER_NSIZE = sizeof(NET_INSTANCE_MESSAGE) + sizeof(InstanceSystem::InstanceMapID) + sizeof(SNATIVEID) + sizeof(InstanceSystem::EMERROR_TYPE) + sizeof(DWORD),
        };

        const InstanceSystem::InstanceMapID instanceMapID;
        const SNATIVEID keyMapID;
        InstanceSystem::EMERROR_TYPE emError;
        DWORD nPlayer;
        InstanceSystem::DBNUM nDbNum[NSIZE];

        const bool addPlayer(const InstanceSystem::DBNUM _nDbNum)
        {
            if ( this->nPlayer == NSIZE )
                return false;

            this->nDbNum[this->nPlayer] = _nDbNum;
            this->dwSize += sizeof(InstanceSystem::DBNUM);
            ++this->nPlayer;

            if ( this->nPlayer == NSIZE )
                return false;

            return true;
        }

        void reset(void)
        {
            this->nPlayer = 0;
            this->dwSize = HEADER_NSIZE;
        }

        const bool isValid(void) const
        {
            return this->nPlayer != 0;
        }

        NET_INSTANCE_ERROR(const SNATIVEID& _keyMapID, const InstanceSystem::InstanceMapID _instanceMapID = GAEAID_NULL)
            : NET_INSTANCE_MESSAGE(HEADER_NSIZE, InstanceSystem::EMMESSAGE_ERROR)
            , instanceMapID(_instanceMapID)
            , keyMapID(_keyMapID)
            , nPlayer(0)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_ERROR) <= NET_DATA_BUFSIZE);
        }
		NET_INSTANCE_ERROR(const InstanceSystem::EMERROR_TYPE _emError, const SNATIVEID& _keyMapID = SNATIVEID(false), const InstanceSystem::InstanceMapID _instanceMapID = GAEAID_NULL)
            : NET_INSTANCE_MESSAGE(HEADER_NSIZE, InstanceSystem::EMMESSAGE_ERROR)
            , instanceMapID(_instanceMapID)
            , keyMapID(_keyMapID)
            , emError(_emError)
            , nPlayer(0)			
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_ERROR) <= NET_DATA_BUFSIZE);
        }		
    };

    struct NET_INSTANCE_INFORMATION_CHANGE : public NET_INSTANCE_MESSAGE
    {
        InstanceSystem::InstanceInformationClient instanceInformation;

        NET_INSTANCE_INFORMATION_CHANGE(const InstanceSystem::InstanceInformationClient& _instanceInformation)
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_INFORMATION_CHANGE), InstanceSystem::EMMESSAGE_INFORMATION_CHANGE)
            , instanceInformation(_instanceInformation)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_INFORMATION_CHANGE) <= NET_DATA_BUFSIZE);
        }
    };

	struct NET_INSTANCE_RELOAD_AF : public NET_INSTANCE_MESSAGE
	{
		const DWORD dwGaeaID;

		NET_INSTANCE_RELOAD_AF(const DWORD _dwGaeaID)
			: NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_RELOAD_AF), InstanceSystem::EMMESSAGE_RELOAD_AF)
			, dwGaeaID(_dwGaeaID)
		{
			MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_RELOAD_AF) <= NET_DATA_BUFSIZE);
		}
	};

	struct NET_INSTANCE_RELOAD_FA : public NET_INSTANCE_MESSAGE
	{
		const unsigned int nFieldServer;
		const unsigned int nFieldChannel;
		const bool bSuccess;
		const DWORD dwGaeaID; // �˸��� gaeaID, ��� �����ϴµ� ���� ����;

		NET_INSTANCE_RELOAD_FA(const unsigned int _nFieldServer, const unsigned int _nFieldChannel, const bool _bSuccess, const DWORD _dwGaeaID = GAEAID_NULL)
			: NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_RELOAD_FA), InstanceSystem::EMMESSAGE_RELOAD_FA)
			, nFieldServer(_nFieldServer)
			, nFieldChannel(_nFieldChannel)
			, bSuccess(_bSuccess)
			, dwGaeaID(_dwGaeaID)
		{
			MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_RELOAD_FA) <= NET_DATA_BUFSIZE);
		}
	};

    struct NET_INSTANCE_GM_COMMAND : public NET_INSTANCE_MESSAGE
    {
		enum
		{
			EMPARAMETER_LENGTH = 128,
		};
        InstanceSystem::EMCOMMAND_TYPE emCommandType;
        union
        {
            struct
            {
                unsigned int nParameter0;
                unsigned int nParameter1;
				unsigned int nParameter2;
				unsigned int nParameter3;
            };	
            struct
            {
                float fParameter0;
                float fParameter1;
				float fParameter2;
				float fParameter3;
            };
        };
        char szParameter0[EMPARAMETER_LENGTH];
        char szParameter1[EMPARAMETER_LENGTH];

        NET_INSTANCE_GM_COMMAND(void)
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_GM_COMMAND), InstanceSystem::EMMESSAGE_GM_COMMAND)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_GM_COMMAND) <= NET_DATA_BUFSIZE);
        }
    };

    struct NET_INSTANCE_NOTIFY_PLAYER_POSITION : public NET_INSTANCE_MESSAGE
    {
        const InstanceSystem::DBNUM dbNum;
        const SNATIVEID baseMapID; // mainMapID/subMapID;		
        const SNATIVEID instanceMapID; // memoryIndex/stage;
        const SNATIVEID keyMapID; // -1 = not in Instance Dungeon;
        const float fPositionX;
        const float fPositionZ;
        char targetPlayerName[CHAR_SZNAME];

        NET_INSTANCE_NOTIFY_PLAYER_POSITION(
            const InstanceSystem::DBNUM _dbNum, const SNATIVEID& _baseMapID, const D3DXVECTOR3& _position,
            const SNATIVEID& _instanceMapID = SNATIVEID(false), const SNATIVEID _keyMapID = SNATIVEID(false))
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_NOTIFY_PLAYER_POSITION), InstanceSystem::EMMESSAGE_NOTIFY_PLAYER_POSITION)
            , dbNum(_dbNum)
            , baseMapID(_baseMapID)
            , instanceMapID(_instanceMapID)
            , keyMapID(_keyMapID)
            , fPositionX(_position.x)
            , fPositionZ(_position.z)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_NOTIFY_PLAYER_POSITION) <= NET_DATA_BUFSIZE);
        }
    };

    struct NET_INSTANCE_NOTIFY_INSTANCE_LOG : public NET_INSTANCE_MESSAGE
    {		
        typedef InstanceSystem::InstanceLog CustomArray;

        enum
        {
            NSIZE = (NET_DATA_BUFSIZE - sizeof(NET_INSTANCE_MESSAGE) - sizeof(DWORD)) / sizeof(CustomArray),
            HEADER_NSIZE = sizeof(NET_INSTANCE_MESSAGE) + sizeof(DWORD),
        };
        DWORD nInstance;
        CustomArray instanceLog[NSIZE];

        const bool addInstance(const CustomArray& _instanceLog)
        {
            if ( this->nInstance == NSIZE )
                return false;

            this->instanceLog[this->nInstance] = _instanceLog;
            this->dwSize += sizeof(CustomArray);
            ++this->nInstance;

            if ( this->nInstance == NSIZE )
                return false;

            return true;
        }

        void reset(void)
        {
            this->nInstance = 0;
            this->dwSize = HEADER_NSIZE;
        }

        const bool isValid(void) const
        {
            return this->nInstance != 0;
        }

        NET_INSTANCE_NOTIFY_INSTANCE_LOG(void)
            : NET_INSTANCE_MESSAGE(HEADER_NSIZE, InstanceSystem::EMMESSAGE_NOTIFY_INSTANCE_LOG)
            , nInstance(0)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_NOTIFY_INSTANCE_LOG) <= NET_DATA_BUFSIZE);
        }
    };

    struct NET_INSTANCE_NOTIFY_INSTANCE_LIST : public NET_INSTANCE_MESSAGE
    {
        typedef InstanceSystem::InstanceList CustomArray;

        enum
        {
            NSIZE = (NET_DATA_BUFSIZE - sizeof(NET_INSTANCE_MESSAGE) - sizeof(DWORD) - sizeof(DWORD) - sizeof(DWORD) - sizeof(DWORD)) / sizeof(CustomArray),
            HEADER_NSIZE = sizeof(NET_INSTANCE_MESSAGE) + sizeof(DWORD) + sizeof(DWORD) + sizeof(DWORD) + sizeof(DWORD),
        };
        const DWORD nViewPage;
        const DWORD nMaxPage;
        const DWORD nMaxInstance;
        DWORD nInstance;
        CustomArray instanceList[NSIZE];

        const bool addInstance(const CustomArray& _instanceLog)
        {
            if ( this->nInstance == NSIZE )
                return false;

            this->instanceList[this->nInstance] = _instanceLog;
            this->dwSize += sizeof(CustomArray);
            ++this->nInstance;

            if ( this->nInstance == NSIZE )
                return false;

            return true;
        }

        void reset(void)
        {
            this->nInstance = 0;
            this->dwSize = HEADER_NSIZE;
        }

        const bool isValid(void) const
        {
            return this->nInstance != 0;
        }

        NET_INSTANCE_NOTIFY_INSTANCE_LIST(const DWORD _nViewPage, const DWORD _nMaxPage, const DWORD _nMaxInstance)
            : NET_INSTANCE_MESSAGE(HEADER_NSIZE, InstanceSystem::EMMESSAGE_NOTIFY_INSTANCE_LIST)
            , nViewPage(_nViewPage)
            , nMaxPage(_nMaxPage)
            , nMaxInstance(_nMaxInstance)
            , nInstance(0)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_NOTIFY_INSTANCE_LIST) <= NET_DATA_BUFSIZE);
        }
    };

    struct NET_INSTANCE_NOTIFY_CHILD_LAND : public NET_INSTANCE_MESSAGE
    {
        const SNATIVEID keyMapID;
        const InstanceSystem::InstanceMapID instanceMapID;		
        SNATIVEID childMapID[MAX_LAND_SID];	
        DWORD nChildMap;

        void addChildMapID(const SNATIVEID& _childMapID)
        {
            if ( this->nChildMap == MAX_LAND_SID )
                return;

            this->childMapID[this->nChildMap] = _childMapID;
            ++this->nChildMap;
        }

        NET_INSTANCE_NOTIFY_CHILD_LAND(const SNATIVEID& _keyMapID, const InstanceSystem::InstanceMapID& _instanceMapID)
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_NOTIFY_CHILD_LAND), InstanceSystem::EMMESSAGE_NOTIFY_CHILD_LAND)
            , keyMapID(_keyMapID)
            , instanceMapID(_instanceMapID)
            , nChildMap(0)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_NOTIFY_CHILD_LAND) <= NET_DATA_BUFSIZE);
        }
    };

    struct NET_INSTANCE_NOTIFY_PLAYER_LIST : public NET_INSTANCE_MESSAGE
    {
        struct PlayerElement
        {
            InstanceSystem::DBNUM dbNum;
            char playerName[CHAR_SZNAME];
			InstanceSystem::EMAUTHORITY typeAuthority;
        };
        typedef PlayerElement CustomArray;

        enum
        {
            NSIZE = (NET_DATA_BUFSIZE - sizeof(NET_INSTANCE_MESSAGE) - sizeof(SNATIVEID) - sizeof(InstanceSystem::InstanceMapID) - sizeof(DWORD)) / sizeof(CustomArray),
            HEADER_NSIZE = sizeof(NET_INSTANCE_MESSAGE) + sizeof(SNATIVEID) + sizeof(InstanceSystem::InstanceMapID) + sizeof(DWORD),
        };

        const bool addPlayer(const InstanceSystem::DBNUM _dbNum, const char* const _playerName, InstanceSystem::EMAUTHORITY _typeAuthority)
        {
            if ( this->nPlayer == NSIZE )
                return false;

            const unsigned int _nIndex(this->nPlayer);
            this->playerList[_nIndex].dbNum = _dbNum;
			this->playerList[_nIndex].typeAuthority = _typeAuthority;

            if ( _playerName != 0 )
                ::StringCchCopy(this->playerList[_nIndex].playerName, CHAR_SZNAME, _playerName);
            this->dwSize += sizeof(CustomArray);
            ++this->nPlayer;

            if ( this->nPlayer == NSIZE )
                return false;

            return true;
        }

        void reset(void)
        {
            this->nPlayer = 0;
            this->dwSize = HEADER_NSIZE;
        }

        const bool isValid(void) const
        {
            return this->nPlayer != 0;
        }

        const SNATIVEID keyMapID;
        const InstanceSystem::InstanceMapID instanceMapID;
        DWORD nPlayer;
        CustomArray playerList[NSIZE];

        NET_INSTANCE_NOTIFY_PLAYER_LIST(const SNATIVEID& _keyMapID, const InstanceSystem::InstanceMapID& _instanceMapID)
            : NET_INSTANCE_MESSAGE(HEADER_NSIZE, InstanceSystem::EMMESSAGE_NOTIFY_PLAYER_LIST)
            , keyMapID(_keyMapID)
            , instanceMapID(_instanceMapID)
            , nPlayer(0)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_NOTIFY_PLAYER_LIST) <= NET_DATA_BUFSIZE);
        }
    };
		
	struct NET_INSTANCE_NOTIFY_GLOBAL_FLOAT : public NET_INSTANCE_MESSAGE
	{
		struct CustomArray
		{
			DWORD nKey;
			float value;
		};
		enum
		{
			NSIZE = (NET_DATA_BUFSIZE - sizeof(NET_INSTANCE_MESSAGE) - sizeof(bool) - sizeof(InstanceSystem::InstanceMapID) - sizeof(SNATIVEID) - (sizeof(char) * 64) - sizeof(DWORD)) / sizeof(CustomArray),
			HEADER_NSIZE = sizeof(NET_INSTANCE_MESSAGE) + sizeof(bool) + sizeof(InstanceSystem::InstanceMapID) + sizeof(SNATIVEID) + (sizeof(char) * 64) + sizeof(DWORD),
		};
		const bool bTableValue;
		const InstanceSystem::InstanceMapID instanceMapID;
		const SNATIVEID keyMapID;

		char globalName[64];
		DWORD nVariable;
		CustomArray globalVariable[NSIZE];

		const bool addGlobal(const DWORD _nKey, const float _value)
		{
			if ( this->nVariable == NSIZE )
				return false;

			const unsigned int _nIndex(this->nVariable);
			this->globalVariable[_nIndex].nKey = _nKey;
			this->globalVariable[_nIndex].value = _value;
			
			this->dwSize += sizeof(CustomArray);
			++this->nVariable;

			if ( this->nVariable == NSIZE )
				return false;

			return true;
		}

		void reset(void)
		{
			this->nVariable = 0;
			this->dwSize = HEADER_NSIZE;
		}

		const bool isValid(void) const
		{
			return this->nVariable != 0;
		}

		NET_INSTANCE_NOTIFY_GLOBAL_FLOAT(const char* const _globalName, const InstanceSystem::InstanceMapID& _instanceMapID, const SNATIVEID& _keyMapID, const bool _bTableValue = false)
			: NET_INSTANCE_MESSAGE(HEADER_NSIZE, InstanceSystem::EMMESSAGE_NOTIFY_GLOBAL_FLOAT)
			, nVariable(0)
			, bTableValue(_bTableValue)
			, instanceMapID(_instanceMapID)
			, keyMapID(_keyMapID)
		{
			::StringCchCopy(this->globalName, 63, _globalName);
			this->globalName[63] = 0;
			MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_NOTIFY_GLOBAL_FLOAT) <= NET_DATA_BUFSIZE);
		}
	};

	struct NET_INSTANCE_NOTIFY_GLOBAL_INTEGER : public NET_INSTANCE_MESSAGE
	{
		struct CustomArray
		{
			DWORD nKey;
			int value;
		};
		enum
		{
			NSIZE = (NET_DATA_BUFSIZE - sizeof(NET_INSTANCE_MESSAGE) - sizeof(InstanceSystem::InstanceMapID) - sizeof(SNATIVEID) - sizeof(DWORD) - (sizeof(char) * 64) - sizeof(DWORD)) / sizeof(CustomArray),
			HEADER_NSIZE = sizeof(NET_INSTANCE_MESSAGE) + sizeof(InstanceSystem::InstanceMapID) + sizeof(SNATIVEID) + sizeof(DWORD) + (sizeof(char) * 64) + sizeof(DWORD),
		};
		const bool bTableValue;
		const InstanceSystem::InstanceMapID instanceMapID;
		const SNATIVEID keyMapID;

		char globalName[64];
		DWORD nVariable;
		CustomArray globalVariable[NSIZE];

		const bool addGlobal(const DWORD _nKey, const int _value)
		{
			if ( this->nVariable == NSIZE )
				return false;

			const unsigned int _nIndex(this->nVariable);
			this->globalVariable[_nIndex].nKey = _nKey;
			this->globalVariable[_nIndex].value = _value;

			this->dwSize += sizeof(CustomArray);
			++this->nVariable;

			if ( this->nVariable == NSIZE )
				return false;

			return true;
		}

		void reset(void)
		{
			this->nVariable = 0;
			this->dwSize = HEADER_NSIZE;
		}

		const bool isValid(void) const
		{
			return this->nVariable != 0;
		}

		NET_INSTANCE_NOTIFY_GLOBAL_INTEGER(const char* const _globalName, const InstanceSystem::InstanceMapID& _instanceMapID, const SNATIVEID& _keyMapID, const bool _bTableValue = false)
			: NET_INSTANCE_MESSAGE(HEADER_NSIZE, InstanceSystem::EMMESSAGE_NOTIFY_GLOBAL_INTEGER)
			, nVariable(0)
			, bTableValue(_bTableValue)
			, instanceMapID(_instanceMapID)
			, keyMapID(_keyMapID)
		{
			::StringCchCopy(this->globalName, 63, _globalName);
			this->globalName[63] = 0;
			MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_NOTIFY_GLOBAL_INTEGER) <= NET_DATA_BUFSIZE);
		}
	};

	struct NET_INSTANCE_WINNER : public NET_INSTANCE_MESSAGE
	{		
		const SNATIVEID instanceMapID;

		NET_INSTANCE_WINNER(const SNATIVEID& _instanceMapID)
			: NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_WINNER), InstanceSystem::EMMESSAGE_DESTROY)
			, instanceMapID(_instanceMapID)
		{
			MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_WINNER) <= NET_DATA_BUFSIZE);
		}
	};


	struct NET_FACTION_RELATION_SET_FACTOIN: public NET_INSTANCE_MESSAGE
	{
		const DWORD dwDbNum;
		const DWORD dwFaction;
		const DWORD instanceMapID;
		Faction::EMFACTION_TYPE factiontype;

		NET_FACTION_RELATION_SET_FACTOIN(const DWORD _dwDbNum,
			const DWORD _dwFaction,const DWORD _mapID,
			Faction::EMFACTION_TYPE _factionType)
			: NET_INSTANCE_MESSAGE(sizeof(NET_FACTION_RELATION_SET_FACTOIN), InstanceSystem::EMMESSAGE_REQ_FACTOIN_SET_FACTION)
			, dwDbNum(_dwDbNum) , dwFaction(_dwFaction) , instanceMapID(_mapID) , factiontype(_factionType)
		{
			MIN_STATIC_ASSERT(sizeof(NET_FACTION_RELATION_SET_FACTOIN) <= NET_DATA_BUFSIZE);
		}
	};


	struct NET_INSTANCE_NOTIFY_GLOBAL_BOOLEAN : public NET_INSTANCE_MESSAGE
	{
		struct CustomArray
		{
			DWORD nKey;
			bool value;
		};
		enum
		{
			NSIZE = (NET_DATA_BUFSIZE - sizeof(NET_INSTANCE_MESSAGE) - sizeof(InstanceSystem::InstanceMapID) - sizeof(SNATIVEID) - sizeof(bool) - (sizeof(char) * 64) - sizeof(DWORD)) / sizeof(CustomArray),
			HEADER_NSIZE = sizeof(NET_INSTANCE_MESSAGE) + sizeof(InstanceSystem::InstanceMapID) + sizeof(SNATIVEID) + sizeof(bool) + (sizeof(char) * 64) + sizeof(DWORD),
		};

		const bool bTableValue;
		const InstanceSystem::InstanceMapID instanceMapID;
		const SNATIVEID keyMapID;

		char globalName[64];
		DWORD nVariable;
		CustomArray globalVariable[NSIZE];

		const bool addGlobal(const DWORD _nKey, const bool _value)
		{
			if ( this->nVariable == NSIZE )
				return false;

			const unsigned int _nIndex(this->nVariable);
			this->globalVariable[_nIndex].nKey = _nKey;
			this->globalVariable[_nIndex].value = _value;

			this->dwSize += sizeof(CustomArray);
			++this->nVariable;

			if ( this->nVariable == NSIZE )
				return false;

			return true;
		}

		void reset(void)
		{
			this->nVariable = 0;
			this->dwSize = HEADER_NSIZE;
		}

		const bool isValid(void) const
		{
			return this->nVariable != 0;
		}

		NET_INSTANCE_NOTIFY_GLOBAL_BOOLEAN(const char* const _globalName, const InstanceSystem::InstanceMapID& _instanceMapID, const SNATIVEID& _keyMapID, const bool _bTableValue = false)
			: NET_INSTANCE_MESSAGE(HEADER_NSIZE, InstanceSystem::EMMESSAGE_NOTIFY_GLOBAL_BOOLEAN)
			, nVariable(0)
			, bTableValue(_bTableValue)
			, instanceMapID(_instanceMapID)
			, keyMapID(_keyMapID)
		{
			::StringCchCopy(this->globalName, 63, _globalName);
			this->globalName[63] = 0;
			MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_NOTIFY_GLOBAL_BOOLEAN) <= NET_DATA_BUFSIZE);
		}
	};

	struct NET_INSTANCE_NOTIFY_GLOBAL_STRING : public NET_INSTANCE_MESSAGE
	{
		struct CustomArray
		{
			DWORD nKey;
			char value[64];
		};
		enum
		{
			NSIZE = (NET_DATA_BUFSIZE - sizeof(NET_INSTANCE_MESSAGE) - sizeof(InstanceSystem::InstanceMapID) - sizeof(SNATIVEID) - sizeof(bool) - (sizeof(char) * 64) - sizeof(DWORD)) / sizeof(CustomArray),
			HEADER_NSIZE = sizeof(NET_INSTANCE_MESSAGE) + sizeof(InstanceSystem::InstanceMapID) + sizeof(SNATIVEID) + sizeof(bool) + (sizeof(char) * 64) + sizeof(DWORD),
		};
		const bool bTableValue;
		const InstanceSystem::InstanceMapID instanceMapID;
		const SNATIVEID keyMapID;

		char globalName[64];
		DWORD nVariable;
		CustomArray globalVariable[NSIZE];

		const bool addGlobal(const DWORD _nKey, const char* const _value)
		{
			if ( this->nVariable == NSIZE )
				return false;

			const unsigned int _nIndex(this->nVariable);
			this->globalVariable[_nIndex].nKey = _nKey;
			::StringCchCopy(this->globalVariable[_nIndex].value, 63, _value);
			this->globalVariable[_nIndex].value[63] = 0;

			this->dwSize += sizeof(CustomArray);
			++this->nVariable;

			if ( this->nVariable == NSIZE )
				return false;

			return true;
		}

		void reset(void)
		{
			this->nVariable = 0;
			this->dwSize = HEADER_NSIZE;
		}

		const bool isValid(void) const
		{
			return this->nVariable != 0;
		}

		NET_INSTANCE_NOTIFY_GLOBAL_STRING(const char* const _globalName, const InstanceSystem::InstanceMapID& _instanceMapID, const SNATIVEID& _keyMapID, const bool _bTableValue = false)
			: NET_INSTANCE_MESSAGE(HEADER_NSIZE, InstanceSystem::EMMESSAGE_NOTIFY_GLOBAL_BOOLEAN)
			, nVariable(0)
			, bTableValue(_bTableValue)
			, instanceMapID(_instanceMapID)
			, keyMapID(_keyMapID)
		{
			::StringCchCopy(this->globalName, 63, _globalName);
			this->globalName[63] = 0;
			MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_NOTIFY_GLOBAL_STRING) <= NET_DATA_BUFSIZE);
		}
	};

	struct NET_INSTANCE_MOVE_MAP : public NET_INSTANCE_MESSAGE
	{ // �̵� �뺸 �޼���;		
		const InstanceSystem::DBNUM dbNum;
		const MapID fromMapID;
		const D3DXVECTOR3 fromMapPosition;
		//const DWORD fromFieldServer;
		const MapID toMapID;

		// 
		const bool bAfterMove;

		NET_INSTANCE_MOVE_MAP(const InstanceSystem::DBNUM _dbNum,
			const MapID& _fromMapID, const D3DXVECTOR3& _fromMapPosition,// const DWORD _fromFieldServer,
			const MapID& _toMapID, const bool _bAfterMove = true )
			: NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_MOVE_MAP), InstanceSystem::EMMESSAGE_MOVE_MAP)
			, dbNum(_dbNum)
			, fromMapID(_fromMapID)
			, fromMapPosition(_fromMapPosition)
			//, fromFieldServer(_fromFieldServer)
			, toMapID(_toMapID)
			, bAfterMove(_bAfterMove)
		{
			MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_MOVE_MAP) <= NET_DATA_BUFSIZE);
		}
		NET_INSTANCE_MOVE_MAP(const NET_INSTANCE_MOVE_MAP& _rhs)
			: NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_MOVE_MAP), InstanceSystem::EMMESSAGE_MOVE_MAP)
			, dbNum(_rhs.dbNum)
			, fromMapID(_rhs.fromMapID)
			, fromMapPosition(_rhs.fromMapPosition)
			//, fromFieldServer(_rhs.fromFieldServer)
			, toMapID(_rhs.toMapID)
			, bAfterMove(_rhs.bAfterMove)
		{
			MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_MOVE_MAP) <= NET_DATA_BUFSIZE);
		}
	};

    struct NET_INSTANCE_CREATE : public NET_INSTANCE_MESSAGE
    {        
        const InstanceSystem::InstanceMapID instanceMapID;
		const SNATIVEID keyMapID;
        const SNATIVEID baseMapID;
		const InstanceSystem::EMINSTANCE_TYPE emInstanceType;


		NET_INSTANCE_CREATE(const SNATIVEID& _keyMapID, const InstanceSystem::InstanceMapID _instanceMapID, const SNATIVEID& _baseMapID, const InstanceSystem::EMINSTANCE_TYPE _emInstanceType)
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_CREATE), InstanceSystem::EMMESSAGE_CREATE)
            , keyMapID(_keyMapID)
            , instanceMapID(_instanceMapID)
            , baseMapID(_baseMapID)
			, emInstanceType(_emInstanceType)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_CREATE) <= NET_DATA_BUFSIZE);
        }
    };

    struct NET_INSTANCE_CREATE_COMPLETE : public NET_INSTANCE_MESSAGE
    {
        const InstanceSystem::InstanceMapID instanceMapID;
		const bool bFirstMake;
        const bool bSuccess;

        NET_INSTANCE_CREATE_COMPLETE(const InstanceSystem::InstanceMapID _instanceMapID, const bool _bFirstMake, const bool _bSuccess = true)
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_CREATE_COMPLETE), InstanceSystem::EMMESSAGE_CREATE_COMPLETE)
            , instanceMapID(_instanceMapID)
			, bFirstMake(_bFirstMake)
            , bSuccess(_bSuccess)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_CREATE_COMPLETE) <= NET_DATA_BUFSIZE);
        }
    };	

    struct NET_INSTANCE_DESTROY : public NET_INSTANCE_MESSAGE
    {		
        const InstanceSystem::InstanceMapID instanceMapID;
		const DWORD dwFieldChannel;
		const DWORD dwFieldID;

        NET_INSTANCE_DESTROY(const InstanceSystem::InstanceMapID _instanceMapID, const DWORD _dwFieldChannel = net::INVALID_SLOT, const DWORD _dwFieldID = net::INVALID_SLOT)
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_DESTROY), InstanceSystem::EMMESSAGE_DESTROY)
            , instanceMapID(_instanceMapID)
			, dwFieldChannel(_dwFieldChannel)
			, dwFieldID(_dwFieldID)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_DESTROY) <= NET_DATA_BUFSIZE);
        }
    };
    
    // agent�� ���� ���� ����� �ƴٰ� �ص�,
    // ���� �δ��� ������ ���� ���� �Ұ� Ȥ�� ��� ���¿��� �� �� �����Ƿ�,
    // �δ� ������ Ȥ�� �������̶� �ϴ��� �ٷ� ���� ��Ű�� �ʰ� field server�Բ� ���޾� ����;
    struct NET_INSTANCE_REQUEST_JOIN_AF : public NET_INSTANCE_MESSAGE
    {	
        const InstanceSystem::InstanceMapID instanceMapID;
        const SNATIVEID keyMapID;
		const InstanceSystem::DBNUM dbNum;
		const InstanceSystem::EMAUTHORITY emAuthority;
		

		NET_INSTANCE_REQUEST_JOIN_AF(const InstanceSystem::InstanceMapID _instanceMapID, const SNATIVEID& _keyMapID, const InstanceSystem::DBNUM _dbNum, const InstanceSystem::EMAUTHORITY _emAuthority)
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_REQUEST_JOIN_AF), InstanceSystem::EMMESSAGE_REQUEST_JOIN_AF)
            , instanceMapID(_instanceMapID)
            , keyMapID(_keyMapID)
			, dbNum(_dbNum)
			, emAuthority(_emAuthority)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_REQUEST_JOIN_AF) <= NET_DATA_BUFSIZE);
        }
    };
    struct NET_INSTANCE_ANSWER_JOIN_FA : public NET_INSTANCE_MESSAGE
    {
        const InstanceSystem::InstanceMapID instanceMapID;
        const SNATIVEID keyMapID;
		const InstanceSystem::DBNUM dbNum;
		const bool bReject;

		NET_INSTANCE_ANSWER_JOIN_FA(const InstanceSystem::InstanceMapID _instanceMapID, const SNATIVEID& _keyMapID, const InstanceSystem::DBNUM _dbNum, const bool _bReject)
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_ANSWER_JOIN_FA), InstanceSystem::EMMESSAGE_ANSWER_JOIN_FA)
            , instanceMapID(_instanceMapID)
            , keyMapID(_keyMapID)
            , dbNum(_dbNum)
			, bReject(_bReject)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_ANSWER_JOIN_FA) <= NET_DATA_BUFSIZE);
        }
    };

    struct NET_INSTANCE_UI_CHATMSG : public NET_INSTANCE_MSGPACK_MESSAGE
    {
        std::string strText;
        DWORD dwColor;

        MSGPACK_DEFINE(emType, strText, dwColor);

        NET_INSTANCE_UI_CHATMSG()
            : NET_INSTANCE_MSGPACK_MESSAGE(InstanceSystem::EMMESSAGE_UI_CHATMSG)
        {
        }
        NET_INSTANCE_UI_CHATMSG(const char* _pText, const DWORD _dwColor)
            : NET_INSTANCE_MSGPACK_MESSAGE(InstanceSystem::EMMESSAGE_UI_CHATMSG)
            , strText(_pText)
            , dwColor(_dwColor)
        {
        }
    };

    struct NET_INSTANCE_UI_CHATXMLMSG : public NET_INSTANCE_MSGPACK_MESSAGE
    {
        DWORD dwColor;
        InstanceSystem::InstanceXmlString xmlString;

        MSGPACK_DEFINE(emType, dwColor, xmlString);

        // EMMESSAGE_UI_CHATXMLMSG;

        NET_INSTANCE_UI_CHATXMLMSG()
            : NET_INSTANCE_MSGPACK_MESSAGE(InstanceSystem::EMMESSAGE_UI_CHATXMLMSG)
        {
        }
    };

    struct NET_INSTANCE_UI_SIMPLE_MSG : public NET_INSTANCE_MSGPACK_MESSAGE
    {
        float fShowTime;
        std::string strText;
        DWORD dwColor;
        DWORD dwFontSize;

        MSGPACK_DEFINE(emType, fShowTime, strText, dwColor);

        NET_INSTANCE_UI_SIMPLE_MSG()
            : NET_INSTANCE_MSGPACK_MESSAGE(InstanceSystem::EMMESSAGE_UI_SIMPLE_MSG)
        {
        }
        NET_INSTANCE_UI_SIMPLE_MSG(const float _fShowTime, const char* const _pText, const DWORD _dwColor, const DWORD _dwFontSize)
            : NET_INSTANCE_MSGPACK_MESSAGE(InstanceSystem::EMMESSAGE_UI_SIMPLE_MSG)
            , fShowTime(_fShowTime)
            , strText(_pText)
            , dwColor(_dwColor)
            , dwFontSize(_dwFontSize)
        {
        }
    };

    struct NET_INSTANCE_UI_SIMPLE_XMLMSG : public NET_INSTANCE_MSGPACK_MESSAGE
    {
        InstanceSystem::InstanceXmlString xmlString;
        float fShowTime;
        DWORD dwColor;
        DWORD dwFontSize;

        MSGPACK_DEFINE(emType, xmlString, fShowTime, dwColor, dwFontSize);

        NET_INSTANCE_UI_SIMPLE_XMLMSG()
            : NET_INSTANCE_MSGPACK_MESSAGE(InstanceSystem::EMMESSAGE_UI_SIMPLE_XMLMSG)
        {
        }
    };

    struct NET_INSTANCE_UI_TEXT_DLG_MSG : public NET_INSTANCE_MSGPACK_MESSAGE
    {
        std::string strText;
        DWORD dwColor;

        MSGPACK_DEFINE(emType, strText, dwColor);

        NET_INSTANCE_UI_TEXT_DLG_MSG()
            : NET_INSTANCE_MSGPACK_MESSAGE(InstanceSystem::EMMESSAGE_UI_TEXT_DLG_MSG)
        {
        }
    };

    struct NET_INSTANCE_UI_TEXT_DLG_XMLMSG : public NET_INSTANCE_MSGPACK_MESSAGE
    {
        InstanceSystem::InstanceXmlString xmlString;
        DWORD dwColor;

        MSGPACK_DEFINE(emType, xmlString, dwColor);

        NET_INSTANCE_UI_TEXT_DLG_XMLMSG()
            : NET_INSTANCE_MSGPACK_MESSAGE(InstanceSystem::EMMESSAGE_UI_TEXT_DLG_XMLMSG)
        {
        }
    };

    struct NET_INSTANCE_UI_TIMER_MSGBOX : public NET_INSTANCE_MSGPACK_MESSAGE
    {
        DWORD dwDbNum;  // UI�� ����� User�� CharDbnum;

        std::string _strContentText;
        std::string _strOkText;
        std::string _strCancelText;

        DWORD emResponseType;
        DWORD dwInstanceID;
        DWORD dwResponseMsgParam1;
        DWORD dwResponseMsgParam2;

        float fTimer;  // Ÿ�̸� �ð� ( 0�ϰ�� Ÿ�̸� ������ );

        MSGPACK_DEFINE(emType, dwDbNum, _strContentText, _strOkText, _strCancelText, emResponseType, dwInstanceID, dwResponseMsgParam1, dwResponseMsgParam2, fTimer);

        NET_INSTANCE_UI_TIMER_MSGBOX()
            : NET_INSTANCE_MSGPACK_MESSAGE(InstanceSystem::EMMESSAGE_UI_TIMER_MSGBOX)
            , dwDbNum(0)
            , emResponseType(InstanceSystem::EM_TO_MY_INSTANCE)
            , dwInstanceID(0)
            , dwResponseMsgParam1(0)
            , dwResponseMsgParam2(0)
            , fTimer(0.0f)
        {
        }
    };

	// ����Ʈ ��� �޽���
	struct NET_INSTANCE_EFFECT: public NET_INSTANCE_MSGPACK_MESSAGE
	{
		std::string _strEffectName;  // ����Ʈ ���� �̸�;
        float _fEffectZonePosX;     // ����Ʈ ����;
        float _fEffectZonePosY;
        float _fEffectZonePosZ;

        DWORD _targetActorType;     // ����Ʈ ���;
        DWORD _targetActorID;
        //DWORD _targetPosX;          // ����Ʈ ��� ��ġ;
        //DWORD _targetPosY;
        //DWORD _targetPosZ;

		MSGPACK_DEFINE(emType, _strEffectName, _fEffectZonePosX, _fEffectZonePosY, _fEffectZonePosZ, _targetActorType, _targetActorID);

		NET_INSTANCE_EFFECT()
			:NET_INSTANCE_MSGPACK_MESSAGE(InstanceSystem::EMMESSAGE_EFFECT)
            , _fEffectZonePosX(0.0f)
            , _fEffectZonePosY(0.0f)
            , _fEffectZonePosZ(0.0f)
            , _targetActorType(0)
            , _targetActorID(0)
		{
		}
	};

	// ��Ī ���� �޽���
	struct NET_INSTANCE_MAIL: public NET_INSTANCE_MSGPACK_MESSAGE
	{
		InstanceSystem::SystemMail Mail;

		MSGPACK_DEFINE(emType,Mail);

		NET_INSTANCE_MAIL()
			:NET_INSTANCE_MSGPACK_MESSAGE(InstanceSystem::EMMESSAGE_MATCHING_MAIL_AF)
		{
		}

	};


    struct NET_INSTANCE_SET_POSITION : public NET_INSTANCE_MESSAGE
    {	
        const InstanceSystem::InstanceMapID instanceMapID;
        const InstanceSystem::InformationPosition informationSetPosition;	

        NET_INSTANCE_SET_POSITION(const InstanceSystem::InstanceMapID& _instanceMapID, const InstanceSystem::InformationPosition& _reserveSetPosition)
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_SET_POSITION), InstanceSystem::EMMESSAGE_SET_POSITION)
            , instanceMapID(_instanceMapID)
            , informationSetPosition(_reserveSetPosition)
        {
            // targetActorType/targetActorID �� GAEAID_NULL�̳� �ƴϳĿ� ���� �̵� ���°� �޶���;
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_SET_POSITION) <= NET_DATA_BUFSIZE);
        }
    };

    struct NET_INSTANCE_SET_POSITION_FA : public NET_INSTANCE_MESSAGE
    {
        const Faction::GLActorID actorID;
		const MapID targetMapID;
        const bool bFail;

        NET_INSTANCE_SET_POSITION_FA(const Faction::GLActorID& _actorID, const MapID& _targetMapID = MapID(), const bool _bFail = true)
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_SET_POSITION_FA), InstanceSystem::EMMESSAGE_SET_POSITION_FA)
            , actorID(_actorID)
            , targetMapID(_targetMapID)
            , bFail(_bFail)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_SET_POSITION_FA) <= NET_DATA_BUFSIZE);
        }
    };	

    struct NET_INSTANCE_DO_MOVE_TO : public NET_INSTANCE_MESSAGE
    {
        const InstanceSystem::InstanceMapID instanceMapID;
        const Faction::GLActorID actorID;
        const Faction::GLActorID targetActorID;
        const D3DXVECTOR3 targetPosition;

        NET_INSTANCE_DO_MOVE_TO(const InstanceSystem::InstanceMapID& _instanceMapID, const Faction::GLActorID& _actorID, const Faction::GLActorID& _targetID, const D3DXVECTOR3& _targetPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f))
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_DO_MOVE_TO), InstanceSystem::EMMESSAGE_DO_MOVE_TO)
            , instanceMapID(_instanceMapID)
            , actorID(_actorID)
            , targetActorID(_targetID)
            , targetPosition(_targetPosition)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_DO_MOVE_TO) <= NET_DATA_BUFSIZE);

        }
    };

    struct NET_INSTANCE_JOIN_LOCK_AF : public NET_INSTANCE_MESSAGE
    {
        const InstanceSystem::InstanceMapID instanceMapID;
		const SNATIVEID keyMapID;
        const bool bLock;

		NET_INSTANCE_JOIN_LOCK_AF(const InstanceSystem::InstanceMapID& _instanceMapID, const SNATIVEID& _keyMapID, const bool _bLock)
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_JOIN_LOCK_AF), InstanceSystem::EMMESSAGE_JOIN_LOCK_AF)
            , instanceMapID(_instanceMapID)
			, keyMapID(_keyMapID)
            , bLock(_bLock)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_JOIN_LOCK_AF) <= NET_DATA_BUFSIZE);
        }
		NET_INSTANCE_JOIN_LOCK_AF(const NET_INSTANCE_JOIN_LOCK_AF& _rhs)
			: NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_JOIN_LOCK_AF), InstanceSystem::EMMESSAGE_JOIN_LOCK_AF)
			, instanceMapID(_rhs.instanceMapID)
			, keyMapID(_rhs.keyMapID)
			, bLock(_rhs.bLock)
		{
			MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_JOIN_LOCK_AF) <= NET_DATA_BUFSIZE);
		}
    };

    struct NET_INSTANCE_JOIN_LOCK_FA : public NET_INSTANCE_MESSAGE
    {
        const InstanceSystem::InstanceMapID instanceMapID;
		const SNATIVEID keyMapID;
        const bool bLock;

        NET_INSTANCE_JOIN_LOCK_FA(const InstanceSystem::InstanceMapID& _instanceMapID, const SNATIVEID& _keyMapID, const bool _bLock)
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_JOIN_LOCK_FA), InstanceSystem::EMMESSAGE_JOIN_LOCK_FA)
            , instanceMapID(_instanceMapID)
			, keyMapID(_keyMapID)
            , bLock(_bLock)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_JOIN_LOCK_FA) <= NET_DATA_BUFSIZE);
        }
		NET_INSTANCE_JOIN_LOCK_FA(const NET_INSTANCE_JOIN_LOCK_FA& _rhs)
			: NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_JOIN_LOCK_FA), InstanceSystem::EMMESSAGE_JOIN_LOCK_FA)
			, instanceMapID(_rhs.instanceMapID)
			, keyMapID(_rhs.keyMapID)
			, bLock(_rhs.bLock)
		{
			MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_JOIN_LOCK_FA) <= NET_DATA_BUFSIZE);
		}
    };

	struct NET_INSTANCE_JOIN_SUSPEND_AF : public NET_INSTANCE_MESSAGE
	{
		const InstanceSystem::InstanceMapID instanceMapID;
		const SNATIVEID keyMapID; 
		const bool bSuspend;

		NET_INSTANCE_JOIN_SUSPEND_AF(const InstanceSystem::InstanceMapID& _instanceMapID, const SNATIVEID& _keyMapID, const bool _bSuspend)
			: NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_JOIN_SUSPEND_AF), InstanceSystem::EMMESSAGE_JOIN_SUSPEND_AF)
			, instanceMapID(_instanceMapID)
			, keyMapID(_keyMapID)
			, bSuspend(_bSuspend)
		{
			MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_JOIN_SUSPEND_AF) <= NET_DATA_BUFSIZE);
		}
		NET_INSTANCE_JOIN_SUSPEND_AF(const NET_INSTANCE_JOIN_SUSPEND_AF& _rhs)
			: NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_JOIN_SUSPEND_AF), InstanceSystem::EMMESSAGE_JOIN_SUSPEND_AF)
			, instanceMapID(_rhs.instanceMapID)
			, keyMapID(_rhs.keyMapID)
			, bSuspend(_rhs.bSuspend)
		{
			MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_JOIN_SUSPEND_AF) <= NET_DATA_BUFSIZE);
		}
	};

	struct NET_INSTANCE_JOIN_SUSPEND_FA : public NET_INSTANCE_MESSAGE
	{
		const InstanceSystem::InstanceMapID instanceMapID;
		const SNATIVEID keyMapID; 
		const bool bSuspend;

		NET_INSTANCE_JOIN_SUSPEND_FA(const InstanceSystem::InstanceMapID& _instanceMapID, const SNATIVEID& _keyMapID, const bool _bSuspend)
			: NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_JOIN_SUSPEND_FA), InstanceSystem::EMMESSAGE_JOIN_SUSPEND_FA)
			, instanceMapID(_instanceMapID)
			, keyMapID(_keyMapID)
			, bSuspend(_bSuspend)
		{
			MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_JOIN_SUSPEND_FA) <= NET_DATA_BUFSIZE);
		}
		NET_INSTANCE_JOIN_SUSPEND_FA(const NET_INSTANCE_JOIN_SUSPEND_FA& _rhs)
			: NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_JOIN_LOCK_FA), InstanceSystem::EMMESSAGE_JOIN_SUSPEND_FA)
			, instanceMapID(_rhs.instanceMapID)
			, keyMapID(_rhs.keyMapID)
			, bSuspend(_rhs.bSuspend)
		{
			MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_JOIN_SUSPEND_FA) <= NET_DATA_BUFSIZE);
		}
	};

	struct NET_INSTANCE_REQUEST_JOIN : public NET_INSTANCE_MESSAGE
	{
		const InstanceSystem::InformationPosition informationSetPosition;	
		const InstanceSystem::EMAUTHORITY emAuthority;

		NET_INSTANCE_REQUEST_JOIN(const InstanceSystem::InformationPosition& _informationPosition, InstanceSystem::EMAUTHORITY _emAuthority)
			: NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_REQUEST_JOIN), InstanceSystem::EMMESSAGE_REQUEST_JOIN)
			, informationSetPosition(_informationPosition)
			, emAuthority(_emAuthority)
		{
			MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_REQUEST_JOIN) <= NET_DATA_BUFSIZE);
		}
	};

	struct NET_INSTANCE_REQUEST_OUT : public NET_INSTANCE_MESSAGE
	{
		const InstanceSystem::InstanceMapID instanceMapID;
		const InstanceSystem::DBNUM dbNum;

		NET_INSTANCE_REQUEST_OUT(const InstanceSystem::InstanceMapID _instanceMapID, const InstanceSystem::DBNUM _dbNum)
			: NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_REQUEST_OUT), InstanceSystem::EMMESSAGE_REQUEST_OUT)
			, instanceMapID(_instanceMapID)
			, dbNum(_dbNum)
		{
			MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_REQUEST_OUT) <= NET_DATA_BUFSIZE);
		}
	};


    // �δ� ��ũ��Ʈ�� Ŀ���� NPC Ŭ���� Client -> Field�� �̺�Ʈ �޽��� ����;
    struct NET_INSTANCE_CLICK_TRIGGER : public NET_INSTANCE_MESSAGE
    {
        STARGETID sTarget;

        NET_INSTANCE_CLICK_TRIGGER( const STARGETID& _target )
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_CLICK_TRIGGER), InstanceSystem::EMMESSAGE_CLICK_TRIGGER)
            , sTarget(_target)
        {
            MIN_STATIC_ASSERT( sizeof(NET_INSTANCE_CLICK_TRIGGER)<= NET_DATA_BUFSIZE );
        }
    };

    // �δ�<->�δ� Ȥ�� �δ�<->Ŭ���̾�Ʈ�� �� Ŀ���� �޽����� �ְ� ����;
    struct NET_INSTANCE_NEW_CUSTOM_MESSAGE : public NET_INSTANCE_MESSAGE
    {
        // ID���� �ǹ̴� Type�� ���� �ٸ��� �Ʒ��� ����;
        // EM_USER : CharDbNum, EM_TO_MY_INSTANCE : CharDbNum,  Base�� : �δ� ������ KeyMapID,  Level�� : �δ� ��ü�� InstanceMapID;

        DWORD emDestType;   // ���� ������ Ÿ��;
        DWORD dwDestID;     // ���� ������ ����;
        DWORD emSrcType;    // ���� ����� Ÿ��;
        DWORD dwSrcID;      // ���� ����� ����;

        // Ŀ���� �޽����� ��;
        // ���� String�̳� float���� �� �߰� �ʿ�� ���⿡ �߰�;
        DWORD param1;
        DWORD param2;
        DWORD param3;
        DWORD param4;

        NET_INSTANCE_NEW_CUSTOM_MESSAGE(DWORD _param1 , DWORD _param2 = 0, DWORD _param3 = 0, DWORD _param4 = 0 )
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_NEW_CUSTOM_MESSAGE), InstanceSystem::EMMESSAGE_NEW_CUSTOM_MESSAGE)
            , emDestType(InstanceSystem::EM_MESSAGE_USER)
            , dwDestID(0)
            , emSrcType(InstanceSystem::EM_MESSAGE_USER)
            , dwSrcID(0)
            , param1(_param1)
            , param2(_param2)
            , param3(_param3)
            , param4(_param4)
        {
            MIN_STATIC_ASSERT( sizeof(NET_INSTANCE_NEW_CUSTOM_MESSAGE)<= NET_DATA_BUFSIZE );
        }
    };

    // ĳ���� ���¸� Ư�� ��ǻ��·� ���� (field -> �ش� pc�� pc�ֺ����� BroadCast);
    struct NET_INSTANCE_SET_MOTION_BRD : public NET_INSTANCE_MESSAGE
    {
        DWORD dwGaeaID;
        DWORD emMainType;
        DWORD emSubType;
        float fAniSec;

        NET_INSTANCE_SET_MOTION_BRD(const DWORD GaeaId)
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_SET_MOTION_BRD), InstanceSystem::EMMESSAGE_SET_MOTION_BRD)
            , dwGaeaID(GaeaId)
            , emMainType(UINT_MAX)
            , emSubType(UINT_MAX)
            , fAniSec(0.0f)
        {
            MIN_STATIC_ASSERT( sizeof(NET_INSTANCE_SET_MOTION_BRD)<= NET_DATA_BUFSIZE );
        }
        NET_INSTANCE_SET_MOTION_BRD(const DWORD GaeaId, const DWORD _mainID, const DWORD _subID, const float _aniSec)
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_SET_MOTION_BRD), InstanceSystem::EMMESSAGE_SET_MOTION_BRD)
            , dwGaeaID(GaeaId)
            , emMainType(_mainID)
            , emSubType(_subID)
            , fAniSec(_aniSec)
        {
            MIN_STATIC_ASSERT( sizeof(NET_INSTANCE_SET_MOTION_BRD)<= NET_DATA_BUFSIZE );
        }
    };

    struct NET_INSTANCE_SET_ACT_STATE_BRD : public NET_INSTANCE_MESSAGE
    {
        DWORD dwGaeaID;
        DWORD dwActStateType;
        bool bState;

        NET_INSTANCE_SET_ACT_STATE_BRD(const DWORD GaeaID, const DWORD _actState, const bool _state)
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_SET_ACT_STATE_BRD), InstanceSystem::EMMESSAGE_SET_ACT_STATE_BRD)
            , dwGaeaID(GaeaID)
            , dwActStateType(_actState)
            , bState(_state)
        {
            MIN_STATIC_ASSERT( sizeof(NET_INSTANCE_SET_ACT_STATE_BRD)<= NET_DATA_BUFSIZE );
        }
    };

	struct NET_INSTANCE_MOVE_DONE : public NET_INSTANCE_MESSAGE
	{
		const MapID mapIDCurrent;
		const DWORD dbNum;

		NET_INSTANCE_MOVE_DONE(const MapID& _mapIDCurrent, const DWORD _dbNum)
			: NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_MOVE_DONE), InstanceSystem::EMMESSAGE_MOVE_DONE)
			, mapIDCurrent(_mapIDCurrent)
			, dbNum(_dbNum)
		{
			MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_MOVE_DONE) <= NET_DATA_BUFSIZE);
		}
	};

    struct NET_INSTANCE_JOIN_PARTY_FA : public NET_INSTANCE_MESSAGE
    {
        const DWORD dwMasterGaeaID;
        const DWORD dwMemberGaeaID;
        const SPartyOption partyOption;

        NET_INSTANCE_JOIN_PARTY_FA(const DWORD _dwMasterDbNum, const DWORD _dwMemberDbNum, const SPartyOption& _partyOption)
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_JOIN_PARTY_FA), InstanceSystem::EMMESSAGE_JOIN_PARTY_FA)
            , dwMasterGaeaID(_dwMasterDbNum)
            , dwMemberGaeaID(_dwMemberDbNum)
            , partyOption(_partyOption)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_JOIN_PARTY_FA) <= NET_DATA_BUFSIZE);
        }
    };

    struct NET_INSTANCE_OUT_PARTY_FA : public NET_INSTANCE_MESSAGE
    {
        const DWORD dwGaeaID;

        NET_INSTANCE_OUT_PARTY_FA(const DWORD _dbNum)
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_OUT_PARTY_FA), InstanceSystem::EMMESSAGE_OUT_PARTY_FA)
            , dwGaeaID(_dbNum)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_OUT_PARTY_FA) <= NET_DATA_BUFSIZE);
        }
    };

    struct NET_INSTANCE_CLEAR_PARTY_FA : public NET_INSTANCE_MESSAGE
    {
        const DWORD dwPartyID;

        NET_INSTANCE_CLEAR_PARTY_FA(const DWORD _dwPartyID)
            : NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_CLEAR_PARTY_FA), InstanceSystem::EMMESSAGE_CLEAR_PARTY_FA)
            , dwPartyID(_dwPartyID)
        {
            MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_CLEAR_PARTY_FA) <= NET_DATA_BUFSIZE);
        }
    };

	struct NET_INSTANCE_REQUEST_OBSERVE : public NET_INSTANCE_MESSAGE
	{
		NET_INSTANCE_REQUEST_OBSERVE(void)
			: NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_REQUEST_OBSERVE), InstanceSystem::EMMESSAGE_REQUEST_OBSERVE)
			, instanceMapIDTargetObserve(InstanceSystem::InstanceMapID(-1))
			, dbNumTargetObserve(InstanceSystem::DBNUM(-1))
		{			
		}

		// �Ʒ� �� �߿� �ϳ��� �����ϸ� �ش� �δ��� ���� ��û�� ������;
		// ó�� �켱 ������ : instanceMapID > dbNum > name �̴�;
		InstanceSystem::InstanceMapID instanceMapIDTargetObserve;
		InstanceSystem::DBNUM dbNumTargetObserve;
		char namePlayerTargetObserve[CHAR_SZNAME];
	};		

	struct NET_INSTANCE_NOTIFY_COUNT_PACKET : public NET_INSTANCE_MESSAGE
	{		
		struct PACKET_COUNT
		{
			union
			{
				struct
				{
					unsigned short indexPacket;
					unsigned short countPacket;
				};
				unsigned int informationPacket;
			};
		};

		enum
		{
			ARRAY_NSIZE = (NET_DATA_BUFSIZE - sizeof(NET_INSTANCE_MESSAGE) - sizeof(unsigned int)) / sizeof(NET_INSTANCE_NOTIFY_COUNT_PACKET::PACKET_COUNT),
		};


		const bool isValid(void) const { return this->sizePacket == 0 ? false : true; }
		const bool push_back(const unsigned int _indexPacket, const unsigned int _countPacket)
		{
			if ( this->sizePacket >= NET_INSTANCE_NOTIFY_COUNT_PACKET::ARRAY_NSIZE )
				return false;

			this->countPacket[this->sizePacket].indexPacket = unsigned short(_indexPacket);
			this->countPacket[this->sizePacket].countPacket = unsigned short(_countPacket);

			this->sizePacket += 1;
			dwSize += sizeof(NET_INSTANCE_NOTIFY_COUNT_PACKET::PACKET_COUNT);
			if ( this->sizePacket >= NET_INSTANCE_NOTIFY_COUNT_PACKET::ARRAY_NSIZE )
				return false;

			return true;
		}
		void clear(void)
		{
			this->sizePacket = 0;
			dwSize = sizeof(NET_INSTANCE_NOTIFY_COUNT_PACKET) - sizeof(NET_INSTANCE_NOTIFY_COUNT_PACKET::PACKET_COUNT) * NET_INSTANCE_NOTIFY_COUNT_PACKET::ARRAY_NSIZE;
		}


		NET_INSTANCE_NOTIFY_COUNT_PACKET(void)
			: NET_INSTANCE_MESSAGE(sizeof(NET_INSTANCE_NOTIFY_COUNT_PACKET), InstanceSystem::EMMESSAGE_NOTIFY_COUNT_PACKET)
			, sizePacket(0)
		{
			dwSize = sizeof(NET_INSTANCE_NOTIFY_COUNT_PACKET) - sizeof(NET_INSTANCE_NOTIFY_COUNT_PACKET::PACKET_COUNT) * NET_INSTANCE_NOTIFY_COUNT_PACKET::ARRAY_NSIZE;
			MIN_STATIC_ASSERT(sizeof(NET_INSTANCE_NOTIFY_COUNT_PACKET) <= NET_DATA_BUFSIZE);
		}

		unsigned int sizePacket;
		PACKET_COUNT countPacket[NET_INSTANCE_NOTIFY_COUNT_PACKET::ARRAY_NSIZE];
	};

	/// ������ �� �ý��ۿ� ���缭 ��Ŷ����͸��� �����Ű�� ���ؼ�;
	/// ���ο� ����� �����Ѵ�;

	// Instance System �� ��� ��Ŷ�� ���;
	// ����� �� �ִ� ��Ŷ�� �ƴϴ�;
	struct NET_INSTANCE_MESSAGE_HEADER : public NET_MSG_GENERIC
	{
		InstanceSystem::InstanceMapID nInstanceID;
		SNATIVEID sKeyMapID;

		NET_INSTANCE_MESSAGE_HEADER ( const DWORD _nSize
			, const EMNET_MSG _msg
			, InstanceSystem::InstanceMapID _nInstanceID
			, SNATIVEID _sKeyMapID )
			: NET_MSG_GENERIC( _msg, _nSize )
			, nInstanceID ( _nInstanceID )
			, sKeyMapID ( _sKeyMapID )
		{

		}
	};

	struct NET_RETRY_FACTION_FB : public NET_INSTANCE_MESSAGE_HEADER
	{
		float fTime;				// ������ �ð�;
		DWORD dwMoney;				// ���Ǵ� ��;
		bool bShow;

		NET_RETRY_FACTION_FB ( InstanceSystem::InstanceMapID _nInstanceID
			, SNATIVEID _sKeyMapID )
			: NET_INSTANCE_MESSAGE_HEADER ( sizeof( NET_RETRY_FACTION_FB )
			, NET_MSG_GCTRL_INSTANCE_RETRY_FACTION_FB
			, _nInstanceID
			, _sKeyMapID )
			, fTime ( 0.f )
			, dwMoney ( 0 )
			, bShow ( true )
		{

		}
	};

	struct NET_RETRY_FACTION_CF : public NET_INSTANCE_MESSAGE_HEADER
	{
		bool bOK;

		NET_RETRY_FACTION_CF ( InstanceSystem::InstanceMapID _nInstanceID
			, SNATIVEID _sKeyMapID )
			: NET_INSTANCE_MESSAGE_HEADER ( sizeof( NET_RETRY_FACTION_CF )
			, NET_MSG_GCTRL_INSTANCE_RETRY_FACTION_CF
			, _nInstanceID
			, _sKeyMapID )
			, bOK ( false )
		{

		}
	};

	struct NET_SHOW_TIME_UI_FB : public NET_INSTANCE_MESSAGE_HEADER
	{
		bool bShow;
		float fSec;					// ������ �ð�;

		NET_SHOW_TIME_UI_FB ( InstanceSystem::InstanceMapID _nInstanceID
			, SNATIVEID _sKeyMapID )
			: NET_INSTANCE_MESSAGE_HEADER ( sizeof( NET_SHOW_TIME_UI_FB )
			, NET_MSG_GCTRL_INSTANCE_SHOW_TIME_UI_FB
			, _nInstanceID
			, _sKeyMapID )
			, bShow ( false )
			, fSec ( 0.f )
		{

		}
	};

	struct NET_SHOW_ENTRANCE_STATE_UI_FB : public NET_INSTANCE_MESSAGE_HEADER
	{
		DWORD dwCurCount;
		DWORD dwMaxCount;
		bool bShow;
		float fSec;					// ������ �ð�;

		NET_SHOW_ENTRANCE_STATE_UI_FB ( InstanceSystem::InstanceMapID _nInstanceID
			, SNATIVEID _sKeyMapID )
			: NET_INSTANCE_MESSAGE_HEADER ( sizeof( NET_SHOW_ENTRANCE_STATE_UI_FB )
			, NET_MSG_GCTRL_INSTANCE_SHOW_ENTRANCE_STATE_UI_FB
			, _nInstanceID
			, _sKeyMapID )
			, dwCurCount ( 0 )
			, dwMaxCount ( 0 )
			, bShow ( false )
			, fSec ( 0.f )
		{

		}
	};

	struct NET_SET_RETURNMAP_INFORMATION_FA : public NET_INSTANCE_MESSAGE_HEADER
	{
		DWORD dwChaDBNum;
		InstanceSystem::MapInformation sReturnMapInformation;

		NET_SET_RETURNMAP_INFORMATION_FA ( InstanceSystem::InstanceMapID _nInstanceID
			, SNATIVEID _sKeyMapID )
			: NET_INSTANCE_MESSAGE_HEADER ( sizeof( NET_SET_RETURNMAP_INFORMATION_FA )
			, NET_MSG_GCTRL_INSTANCE_SET_RETURNMAP_INFORMATION_FA
			, _nInstanceID
			, _sKeyMapID )
			, dwChaDBNum ( 0 )
		{

		}
	};
}
