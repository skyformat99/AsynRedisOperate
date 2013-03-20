/********************************************************************
	created:	2013/02/19
	created:	19:2:2013   16:21
	filename: 	c:\work\code\Server\PlayerDataServer\Server\DBCommandCallback_Player.h
	file path:	c:\work\code\Server\PlayerDataServer\Server
	file base:	DBCommandCallback_Player
	file ext:	h
	author:		houzhifang
	
	purpose:	
*********************************************************************/

#ifndef __DBCommandCallback_Player_h__
#define __DBCommandCallback_Player_h__
#pragma once

#include "ServerDefine.h"
#include "redisdb/DBCommandCallBack.h"
#include "redisdb/DBCommandCallbackFactory.h"

class DBCommandCallback_AskPlayerInfo : public DBCommandCallBack
{
public:
	DBCommandCallback_AskPlayerInfo():m_nAskGUID(invalid_guid),m_nTargetGUID(invalid_guid),m_bSendPersonalBaseInfo(false){
	};
	virtual ~DBCommandCallback_AskPlayerInfo(){};

	virtual DBCOMMAND_CALLBACK_TYPE getType(){
		return DBCOMMAND_CALLBACK_ASKPLAYERINFO;
	}
	virtual void CleanUp(){
		DBCommandCallBack::CleanUp();
		m_bSendPersonalBaseInfo = false;
		m_nAskGUID = invalid_guid;
		m_nTargetGUID = invalid_guid;
	}

	// ���������������ɺ���Ҫ�����߼��ı�Ҫ�������Ҫ��ָ�룬��Ϊ���̻߳���ʱҲ��ָ���Ѿ�����
	void Init(const Guid_t nAskGuid, const Guid_t nTargetGUID, ID_t nClientConnectionID, bool bSendPersonalBaseInfo = false);
	// db�߳̽����󣬽�DB���ݷ�������
	bool SetCallbackData(const PlayerInfo& playerinfo);
	virtual void ExcuteCallBack();

protected:
	Guid_t m_nAskGUID;
	Guid_t m_nTargetGUID;
	ID_t m_nClientConnectionID;
	PlayerInfo m_PlayerInfo;
	bool m_bSendPersonalBaseInfo;
};


#endif // __DBCommandCallback_Player_h__

