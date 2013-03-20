/*
	�ļ�	��DBConnectThread.h
	���	��db�����̣߳�ÿ���̱߳���һ��DB�ó����ӣ�ÿ֡���������ȡ�������Ҫ�����������д���
	����	����־��
	�������ڣ�2010/01/20

	��Ȩ����(C) 2009-2010 �������£��������Ƽ����޹�˾

	����޸�����	�޸���		�޸�����
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/

#ifndef __DB_CONNECT_THREAD_H__
#define __DB_CONNECT_THREAD_H__
#include "IThread.h"
#include "RedisConnector.h"
#include "DBCommandOperate.h"

class DBConnectThread : public IThread
{
public:
	DBConnectThread();
	virtual ~DBConnectThread();

	bool Instance();
	virtual void OnExit();

	RedisConnector* GetDBInterface() {
		return &m_Interface;
	}
	const ULONG GetConnectThreadID() const {
		return m_ThreadID;
	}

protected:
	virtual void _Run(void);

	// ��������ʣ������ĸ����õ�����Ƶ�ʣ��Ӷ���̬����DBѹ��
	int  GetSleepTime(int nLeaveCommandSize);

private:
	RedisConnector m_Interface;
	DBCommandOperate m_Operator;
	ULONG m_ThreadID;
	int   m_nLastDBHoldTime;
};

#endif
