/*
	�ļ�	��DBCommandFactoryManager.h
	���	��DB������Ĺ����࣬��ͨ������õ�������Ҫ�Ĺ�����
	����	����־��
	�������ڣ�2010/01/04

	��Ȩ����(C) 2009-2010 �������£��������Ƽ����޹�˾

	����޸�����	�޸���		�޸�����
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/
#ifndef __DBCOMMAND_FACTORY_MANAGER_H__
#define __DBCOMMAND_FACTORY_MANAGER_H__
#include <list>
#include "BaseType.h"
#include "Singleton.h"
#include "DBCommand.h"
#include "DBCommandFactory.h"

class DBCommandFactoryManager : public Singleton<DBCommandFactoryManager>
{
public:
	DBCommandFactoryManager();
	virtual ~DBCommandFactoryManager();

	bool       Instance();
	void       Exit();
	bool       AddDBCommandFactory(DBCommandFactoryBase* pFactory);
	DBCommandFactoryBase* GetDBCommandFactory(const DBCOMMAND_TYPE type);

private:
	//DBCommandFactoryBase* m_FactoryCache[MAX_DBCOMMAND_FACTORY_SIZE];  //�滻Ϊ�·��Լ�ʵ�ֵ�list�ˣ�������������С���Լ����Կ��ơ�
	std::list<DBCommandFactoryBase*> m_FactoryCache;
	INT m_FactoryPos;

	friend class Singleton<DBCommandFactoryManager>;
};

#endif
