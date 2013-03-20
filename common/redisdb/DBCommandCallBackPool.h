/********************************************************************
created:	2013/02/19
created:	19:2:2013   15:21
filename: 	c:\work\code\Server\common\redisdb\DBCommandCallBackPool.cpp
file path:	c:\work\code\Server\common\redisdb
file base:	DBCommandCallBackPool
file ext:	cpp
author:		houzhifang

purpose:	
*********************************************************************/

#ifndef __DBCommandCallBackPool_h__
#define __DBCommandCallBackPool_h__
#pragma once

#include "ServerDefine.h"
#include "DBCommandCallBack.h"

class DBCommandCallBackPool : public Singleton<DBCommandCallBackPool>
{
public:
	DBCommandCallBackPool():m_CallbackID(100){}
	~DBCommandCallBackPool(){};
	unsigned int AddDBCommandCallBack(DBCommandCallBack* pDBCommandCallback);
	bool SignalCommandCallback(unsigned int nCallbackID);
	void HeartBeat(float dt, float nowTime);

	// ��������ӿڱȽ�Σ�գ��õ������callback�����ܱ�֤�̰߳�ȫ��ʹ��ʱ����
	DBCommandCallBack* GetCommandCallBack(unsigned int nCallBackID);

private:
	std::map<unsigned int , DBCommandCallBack*> m_ReadyingDBCommandCallback;
	unsigned int m_CallbackID;
	CLock m_Lock;
}; 

#endif // __DBCommandCallBackPool_h__
