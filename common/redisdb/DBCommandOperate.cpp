#include "DBCommandOperate.h"
#include "DBCommandPool.h"
#include "DBCommandFactoryManager.h"
#include "LogSystem.h"

DBCommandOperate::DBCommandOperate()
{
}

DBCommandOperate::~DBCommandOperate()
{

}

bool DBCommandOperate::Instance()
{
	return true;
}

int DBCommandOperate::HeartBeat(INT nNowTime)
{
	DBCommand* pCmd = DBCommandPool::GetInstancePtr()->GetTopPriorityCommand();
	if (pCmd == NULL)
	{
		pCmd = DBCommandPool::GetInstancePtr()->GetTopCommonCommand();
	}

	if (pCmd != NULL)
	{
		bool bRet = pCmd->Execute();
		if (bRet)
		{
			DBCommandPool::GetInstancePtr()->ExcureEnd(pCmd);  //��������������ɫ���ݵ��ظ��洢���⣬��ֹ�������ǣ�Ŀǰֻ�н�ɫ�洢���õ���
			DBCommandFactoryManager::GetInstancePtr()->GetDBCommandFactory(pCmd->GetType())->DestoryDBCommand(pCmd);
		}
		else
		{
			CacheLog_DEBUG(LOG_ERROR, "DBCommandPool Excure Fial");
		}
	}

	return DBCommandPool::GetInstancePtr()->GetTotalCommandSize();
}

bool DBCommandOperate::Exit()
{
	return true;
}
