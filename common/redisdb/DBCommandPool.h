/*
	�ļ�	��DBCommandPool.h
	���	��DBCommand������أ����ﱣ�����ָ�룬ָ��ָ�򹤳���������Щ���
	          �����̴߳ӱ�����ȡ����Ҫ�������������̸߳���Զ��������Ҫ�������������
	����	����־��
	�������ڣ�2010/01/20

	��Ȩ����(C) 2009-2010 �������£��������Ƽ����޹�˾

	����޸�����	�޸���		�޸�����
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/

#ifndef __DB_COMMAND_POOL_H__
#define __DB_COMMAND_POOL_H__

#include "ServerDefine.h"
#include "DBCommand.h"

class DBCommandPool : public Singleton<DBCommandPool>{
public:
	bool Init();
	bool UnInit();
	bool AddPriorityCommand(DBCommand* cmd);
	bool AddCommonCommand(DBCommand* cmd);
	DBCommand* GetTopPriorityCommand();
	DBCommand* GetTopCommonCommand();
	int  GetTotalCommandSize();

	//���ڴ洢��ҽ�ɫ���ݣ�������һ�����Ʋ��ԣ����ڴ洢�е���ң���������˴洢������Ҫ�ȴ���һ���洢��ɺ���ܽ��еģ�������ֽ�ɫ�洢�����������ǵ����⡣
	VOID ExcureEnd(DBCommand* pCommand);

private:
	//�����ƻ�ʹ���̰߳�ȫ�ĵģ��������ֻ����Լ��������ȽϺá������ڶ���������������з��ʺ��޸�ʱһ��Ҫע���̰߳�ȫ���⡣
	std::list<DBCommand*> _commandPriority;
	std::list<DBCommand*> _commandCommon;

	CLock m_Lock;

	DBCommandPool();
	virtual ~DBCommandPool();
	friend class Singleton<DBCommandPool>;
};

#endif
