/*
	�ļ�	��DBCommandFactory.h
	���	��DB������ඨ�壬DB����ع�����
	          ͨ���أ����Ծ�����֤����ǰ����ִ������ʱ��Ƶ�������ڴ�
			  ͨ�����������Ը���ʵ�����������ͬ������
			  ͨ��ģ��ʵ���˹�����ľ���ʵ�֣�ֻ���ڳ�ʼ��ʱnew����������
	����	����־��
	�������ڣ�2010/01/04

	��Ȩ����(C) 2009-2010 �������£��������Ƽ����޹�˾

	����޸�����	�޸���		�޸�����
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/

#ifndef __DBCOMMAND_FACTORY_H__
#define __DBCOMMAND_FACTORY_H__

#include "DBCommand.h"
#include "lock.h"
#include "ServerDefine.h"
#include <stdlib.h>

#define DBCOMMAND_CACHE_SIZE   20
#define DBCOMMAND_CACHE_ADD_SIZE    10

//���DBCommand��Ӧ�������̰߳�ȫ�ģ���Ҫ�����̴߳���ʱ����������
template<class T>
class DBCommandCache
{
public:
	DBCommandCache():_DBCommandCache(NULL),/*_Flag(NULL),*/_nextIdle(-1),_nowPoolSize(0),_nowUsedSize(0){};
	~DBCommandCache(){};

	int InitPool(){
		int nRet = 0;
		if (_DBCommandCache == NULL)
		{
			_DBCommandCache = (T**)malloc(DBCOMMAND_CACHE_SIZE*sizeof(T*));
			//_Flag = (BOOL*)malloc(DBCOMMAND_CACHE_SIZE*sizeof(BOOL));
			if (_DBCommandCache == NULL/* || _Flag == NULL*/)
			{
				return 0;
			}

			for (int i = 0; i < DBCOMMAND_CACHE_SIZE; i++)
			{
				_DBCommandCache[i] = new T;
				//_Flag[i] = FALSE;
				_DBCommandCache[i]->SetStatus(DBCOMMAND_STATUS_IDLE);
			}
			_nowPoolSize = DBCOMMAND_CACHE_SIZE;
		}
		_nextIdle = 0;
		nRet = _nowPoolSize;
		return nRet;
	}

	void UnInitPool(){
		if (_DBCommandCache != NULL)
		{
			for (INT i = 0; i < _nowPoolSize; i++)
			{
				delete _DBCommandCache[i];
				_DBCommandCache[i] = NULL;
			}
			free(_DBCommandCache);
			_DBCommandCache = NULL;
			/*free(_Flag);
			_Flag = NULL;*/
		}
		_DBCommandCache = NULL;
		//_Flag = NULL;
		_nextIdle = -1;
		_nowPoolSize = 0;
		_nowUsedSize = 0;
	}

	T* GetIdleDBCommand(){
		if (_nowPoolSize <= 0)
		{
			return NULL;
		}
		T* pRet = NULL;
		//Ԥ��ռ�
		CSmartLock lock(m_lock);
		if (_nowUsedSize >= _nowPoolSize-1)
		{
            T** pNewCache = (T**)realloc(_DBCommandCache, (_nowPoolSize+DBCOMMAND_CACHE_ADD_SIZE)*sizeof(T*));
			
			if (pNewCache == NULL)
			{
				return NULL;
			}

            _DBCommandCache = pNewCache;
			
			for (INT i = _nowPoolSize; i<_nowPoolSize+DBCOMMAND_CACHE_ADD_SIZE;i++)
			{
				_DBCommandCache[i] = new T;
				//_Flag[i] = FALSE;
			}

			_nowPoolSize += DBCOMMAND_CACHE_ADD_SIZE;
		}

		//�õ����ʹ�õĿռ�
		if (_nextIdle < 0 || _nextIdle >= _nowPoolSize)
		{
			return NULL;
		}
		pRet = _DBCommandCache[_nextIdle];
		if (pRet == NULL)
		{
			CacheLog_DEBUG(LOG_ERROR, "_DBCommandCache get a NULL: %d", _nextIdle);
			return NULL;
		}
		_DBCommandCache[_nextIdle]->SetStatus(DBCOMMAND_STATUS_INIT);
		//_Flag[_nextIdle] = TRUE;
		++_nowUsedSize;
		//assert(pRet != NULL && _nowUsedSize < _nowPoolSize);

		//�����´�ʹ��λ��
		bool bFind = false;
		for (INT i = _nextIdle+1; i< _nowPoolSize; i++)
		{
			if (_DBCommandCache[i]->GetStatus() == DBCOMMAND_STATUS_IDLE)
			{
				_nextIdle = i;
				bFind = true;
				break;
			}
		}

		if (!bFind)
		{
			for (INT i = 0; i < _nextIdle; i++)
			{
				if (_DBCommandCache[i]->GetStatus() == DBCOMMAND_STATUS_IDLE)
				{
					_nextIdle = i;
					bFind = true;
					break;
				}
			}
		}

		if (!bFind)
		{
			_nextIdle = -1;
			//�ǳ����ص���־���������Ҳ����ġ�
		}

		return pRet;
	}

	bool RecycleDBCommand(DBCommand* pCommand){
		if (pCommand == NULL)
		{
			return false;
		}

		CSmartLock lock(m_lock);
		for (INT i = 0; i < _nowPoolSize; i++)
		{
			if (/*_Flag[i] && */pCommand == _DBCommandCache[i])
			{
				_DBCommandCache[i]->CleanUp();
				_DBCommandCache[i]->SetStatus(DBCOMMAND_STATUS_IDLE);
				_nowUsedSize--;
				//_Flag[i] = FALSE;
				break;
			}
		}
		return true;
	}

private:
	T** _DBCommandCache;
	//BOOL* _Flag;
	int _nextIdle;
	int _nowPoolSize;
	int _nowUsedSize;
	CLock m_lock;
};

class DBCommandFactoryBase
{
public:
	DBCommandFactoryBase(DBCOMMAND_TYPE type)
		:m_FactoryType(type){};
	virtual ~DBCommandFactoryBase(){};

	DBCOMMAND_TYPE GetCommandType(){
		return m_FactoryType;
	}

	virtual DBCommand* CreateDBCommand() = 0;

	virtual bool DestoryDBCommand(DBCommand* command) = 0;

protected:
	DBCOMMAND_TYPE m_FactoryType;

	//��������ֻ����Ԫ��DBCommandFactoryManager����
	virtual bool InitFactory() = 0;
	virtual void UnInitFactory() = 0;

	friend class DBCommandFactoryManager;
};

template<class DBCommandClass>
class DBCommandFactory : public DBCommandFactoryBase
{
public:
	DBCommandFactory(DBCOMMAND_TYPE type)
		:DBCommandFactoryBase(type){
	};
	virtual ~DBCommandFactory(){};

	virtual DBCommand* CreateDBCommand(){
		DBCommandClass* pCommand = _CommandCache.GetIdleDBCommand();   //_CommandCache�ڲ���֤��֧���̰߳�ȫ�ԡ�
		if (pCommand)
		{
			pCommand->CleanUp();
		}
		else
		{
			CacheLog_DEBUG(LOG_ERROR, "_CommandCache.GetIdleDBCommand() error");
			Assert(false);
		}
		//���س�ȥ�󣬼��������������
		return pCommand;
	}

	virtual bool DestoryDBCommand(DBCommand* command){
		bool bRet = true;
		bRet = _CommandCache.RecycleDBCommand(command);
		return bRet;
	}

protected:
	DBCommandCache<DBCommandClass> _CommandCache;

	virtual bool InitFactory(){
		bool bRet = TRUE;
		int nPoolSize = _CommandCache.InitPool();
		if (nPoolSize <= 0)
		{
			bRet = false;
		}

		return bRet;
	}

	virtual void UnInitFactory(){
		_CommandCache.UnInitPool();
	}
};

#endif
