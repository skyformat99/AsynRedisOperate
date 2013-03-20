/*
	�ļ�	��DBThreadPool.h
	���	������ά������DB���ӵĴ����߳�
	����	����־��
	�������ڣ�2010/01/20

	��Ȩ����(C) 2009-2010 �������£��������Ƽ����޹�˾

	����޸�����	�޸���		�޸�����
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/

#ifndef __DB_THREAD_POOL_H__
#define __DB_THREAD_POOL_H__

#include "Singleton.h"
#include "DBConnectThread.h"

#define MAX_DB_CONNECT_THREAD_SIZE  1

class DBThreadPool : public Singleton<DBThreadPool>
{
public:
	DBThreadPool();
	virtual ~DBThreadPool();

	bool Instance();
	bool Begin();
	void Exit();

	int  GetRunningThreadSize();

	DBConnectThread* GetDBConnectThread() const ;

private:
	DBConnectThread* _db_thread;

	friend class Singleton<DBThreadPool>;
};

#endif
