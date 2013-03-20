/*
	�ļ�	��DBCommandFactoryManager.h
	���	��DB������Ĺ����࣬��ͨ������õ�������Ҫ�Ĺ�����
	����	����־��
	�������ڣ�2013/02/19

	����޸�����	�޸���		�޸�����
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/
#ifndef __DBCOMMAND_CALLBACK_FACTORY_MANAGER_H__
#define __DBCOMMAND_CALLBACK_FACTORY_MANAGER_H__
#include <list>
#include "BaseType.h"
#include "Singleton.h"
#include "DBCommandCallBack.h"
#include "DBCommandCallbackFactory.h"

class DBCommandCallbackFactoryManager : public Singleton<DBCommandCallbackFactoryManager>
{
public:
	DBCommandCallbackFactoryManager();
	virtual ~DBCommandCallbackFactoryManager();

	bool       Instance();
	void       Exit();
	bool       AddDBCommandCallbackFactory(DBCommandCallBackFactoryBase* pFactory);
	DBCommandCallBackFactoryBase* GetDBCommandCallbackFactory(const DBCOMMAND_CALLBACK_TYPE type);

private:
	//DBCommandFactoryBase* m_FactoryCache[MAX_DBCOMMAND_FACTORY_SIZE];  //�滻Ϊ�·��Լ�ʵ�ֵ�list�ˣ�������������С���Լ����Կ��ơ�
	std::list<DBCommandCallBackFactoryBase*> m_FactoryCache;
	INT m_FactoryPos;

	friend class Singleton<DBCommandCallbackFactoryManager>;
};

#endif  // __DBCOMMAND_CALLBACK_FACTORY_MANAGER_H__
