/*
	�ļ�	��DBCommand.h
	���	��DBCommand���࣬���崦��DB����Ļ���
	          ����DBCommand�������ȷ�Ϊ���࣬һ����CHAR��صģ���һ��ʱmanager���͵ģ������̵�������ȣ�
			  ����CHAR��ص����ݣ�����������ֻ�ᱻLoginServer���أ�SceneServerֻ�ᱣ�棬����ÿ�α��棬Ϊ�˽���ͨ������������SMUnit������һ���֡�
			  ����manager���͵ģ�����SceneServer��Sharemem����ʱ���󲿷�ʱ���ʱ������Ҫ���أ�֮��ֻ�ᱣ�棬�����ٴμ��ء�
			  ���ԣ����¹滮Command�����ࣺChar_Load���������loginserver���ļ��ؽ�ɫ��������ַ�Ϊ��ȡ�˻��µĽ�ɫ��Ϣ�Ͷ�ȡĳ����ɫ������Ϣ��
			  Char_Create���������LoginServer���Ĵ�����ɫ��� Char_Save_Table��������SceneServer����Ĵ洢��ɫĳ����ĵ����Unit_Load,Unit_Save
			  �ֱ���manager��ص����������save��Ҫ���ô洢���̣���ʹ������update����������insert���߼���
	����	����־��
	�������ڣ�2010/01/04

	��Ȩ����(C) 2009-2010 �������£��������Ƽ����޹�˾

	����޸�����	�޸���		�޸�����
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/

#ifndef __DB_COMMAND_H__
#define __DB_COMMAND_H__

#include "BaseType.h"

enum DBCOMMAND_TYPE
{
	DBCOMMAND_TYPE_PLAYER_LOAD = 0,                 // for playergameserver
	DBCOMMAND_TYPE_PLAYER_SAVE,                         // for playergameserver
	DBCOMMAND_TYPE_PERSONAL_LOAD,                  // for playergameserver
	DBCOMMAND_TYPE_PERSONAL_SAVE,                // for playergameserver

	DBCOMMAND_TYPE_PLAYER_LOGIN,
	DBCOMMAND_TYPE_PLAYER_REGISTER,

	DBCOMMAND_TYPE_SIZE,
};

enum DBCOMMAND_PRIORITY
{
	DBCOMMAND_PRIORITY_LEVEL_HIGH = 0,
	DBCOMMAND_PRIORITY_LEVEL_MID,
	DBCOMMAND_PRIORITY_LEVEL_LOW,
};

//״̬���ù����õģ��������������״̬���Ӷ����и��á�
enum DBCOMMAND_STATUS
{
	DBCOMMAND_STATUS_IDLE,
	DBCOMMAND_STATUS_INIT,
	DBCOMMAND_STATUS_OPERATE,
};

#define DEFAULT_BLANK_CONTENT   "~@blank@!"

class IMsgHandler;
class DBCommand
{
public:
	DBCommand()
	:m_Priority(DBCOMMAND_PRIORITY_LEVEL_MID),m_Status(DBCOMMAND_STATUS_IDLE),m_pMsgHandler(NULL),m_nDBCallbackID(-1){

	};

	virtual ~DBCommand(){};

//	virtual BOOL BuildCommand() = 0;
	virtual bool Execute() = 0;
	virtual void CleanUp(){ m_nDBCallbackID = -1; };       //����DBCommand�Ǹ��еģ����Ա����ṩCleanUp�������Թ�����ʱ��յ��á�
	virtual DBCOMMAND_TYPE GetType() = 0;
	virtual DBCOMMAND_PRIORITY  GetPriority() const {
		return m_Priority;
	}
	virtual void SetPriority(DBCOMMAND_PRIORITY priority){
		m_Priority = priority;
	}
	virtual DBCOMMAND_STATUS GetStatus() const {
		return m_Status;
	}
	virtual void SetStatus(DBCOMMAND_STATUS status){
		m_Status = status;
	}

	virtual unsigned int GetSaveSerail() const {
		return m_nSaveSerail;
	}

	void setCallbackID(const unsigned int nID){
		m_nDBCallbackID = nID; 
	}

	int getCallbackID() const{
		return m_nDBCallbackID;
	}

	//�·���m_pMsgHandler������Execute�н��д�����Ϣ��
	IMsgHandler* GetMsgHandler() const { return m_pMsgHandler; }
	VOID SetMsgHandler(IMsgHandler* nMsgHandler) { m_pMsgHandler = nMsgHandler; }

protected:
	DBCOMMAND_PRIORITY m_Priority;
	DBCOMMAND_STATUS m_Status;
	IMsgHandler*  m_pMsgHandler;
	unsigned int  m_nSaveSerail;
	int m_nDBCallbackID;
};
#endif
