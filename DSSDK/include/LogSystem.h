#ifndef	_LOG_SYSTEM_H_
#define	_LOG_SYSTEM_H_

#include "BaseType.h"
#include "LogFile.h"

#define MAX_LOG_FILE_NUM 16

enum enum_LOG_FILE_ID
{
	LOG_DEBUG,
	LOG_ASSERT,
	LOG_ERROR,
	LOG_FUNCTION,
	LOG_SEND,
	LOG_LUA_ERROR,
    LOG_DSSDK,
	LOG_MONEY,
	LOG_ITEM,
	LOG_AUDIT,
};


class CLogSystem
{
public:
	CLogSystem();
	~CLogSystem();

public:
	static CLogSystem* Instance();
private:
	static CLogSystem*	m_pInstance;

public:
    // ��ʼ����־ϵͳ,ָ���Ƿ񴴽�������logĿ¼���Ƿ�log�ļ������ʱ�䡢���ڵȵĺ�׺
    bool Initialize(bool bCreateLogDir = TRUE, bool bLogFilenameAddPostfix = TRUE);

	//��ʼ��һ����־�ļ���ָ��һ���ļ�id������־ʱ�ṩ����ļ�id��������ļ�
	bool InitLogFile(INT logFileId, const CHAR* szFileName, const CHAR* szLogPath );

	//�����ļ�id������־�ļ�
	CLogFile* GetLogFlieById(INT logFileId);

	//������־�ļ�����־�ȼ����ߵ�������ȼ�����־�Żᱻ�����ļ�
	VOID SetFileLogLevel(enum_LOG_LEVEL	logLevel) { m_eFileLogLevel = logLevel; }

	//���ÿ���̨�������־�ȼ����ߵ�������ȼ�����־�Żᱻ�������̨
	VOID SetConsoleLogLevel(enum_LOG_LEVEL logLevel) { m_eConsoleLogLevel = logLevel; }

    bool GetFileNameAddPostfix() { return m_bFileNameAddPostfix; }

	//ˢ��������־��backbuf���ļ�
	VOID Flush();

	VOID Exit();

private:
	enum_LOG_LEVEL		m_eFileLogLevel;
	enum_LOG_LEVEL		m_eConsoleLogLevel;
    bool                m_bFileNameAddPostfix; //��־�ļ����Ƿ����Ӻ�׺
    CLogFile*			m_pLogFiles[MAX_LOG_FILE_NUM];

	//Ĭ����־�ļ�����ָ�����ļ�id�д��Ҳ�����Ӧ�ļ�ʱ��������ļ�
	CLogFile*			m_pDefaultFile;

};

#define DiskLog_WARNNING(fileId, format, ...) CLogSystem::Instance()->GetLogFlieById(fileId)->DiskLog(LOG_LEVEL_WARNNING, format, ##__VA_ARGS__)
#define DiskLog_DEBUG(fileId, format, ...) CLogSystem::Instance()->GetLogFlieById(fileId)->DiskLog(LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define DiskLog_ERROR(fileId, format, ...) CLogSystem::Instance()->GetLogFlieById(fileId)->DiskLog(LOG_LEVEL_ERROR, format, ##__VA_ARGS__)

#define CacheLog_WARNNING(fileId, format, ...) CLogSystem::Instance()->GetLogFlieById(fileId)->CacheLog(LOG_LEVEL_WARNNING, format, ##__VA_ARGS__)
#define CacheLog_DEBUG(fileId, format, ...) CLogSystem::Instance()->GetLogFlieById(fileId)->CacheLog(LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define CacheLog_ERROR(fileId, format, ...) CLogSystem::Instance()->GetLogFlieById(fileId)->CacheLog(LOG_LEVEL_ERROR, format, ##__VA_ARGS__)

#endif
