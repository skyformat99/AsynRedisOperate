#ifndef	_LOG_FILE_H_
#define	_LOG_FILE_H_

#include "BaseType.h"
#include "lock.h"

#define MAX_FILE_NAME	256
#define MAX_LOG_LINE	8192
#define LOG_BUF_LEN		1024*1024

enum enum_LOG_LEVEL
{
    LOG_LEVEL_DEBUG = 0,
	LOG_LEVEL_WARNNING,
	LOG_LEVEL_ERROR,
};

class CLogFile
{
public:
	CLogFile(const CHAR* szFileName, const CHAR* szPath );
	~CLogFile();

public:
	//��־д���ڴ滺�������Ȼ�����ʱ����һ����ʱ����ʱ��ͳһд���ļ�
	VOID CacheLog(enum_LOG_LEVEL logLevel, const CHAR* pLogString , ...);

	//ֱ��д���ļ��У�����������δд��
	VOID DiskLog(enum_LOG_LEVEL	logLevel, const CHAR* pLogString , ...);

	//���back�������������ݾ�д���ļ�
	VOID WriteBackBufToFile();

	//front �� back ����������д���ļ�
	VOID WriteAllBufToFile();

	//����ļ���С�������Ҫ�Ļ������½�һ���ļ�
	VOID CheckLogFileStat();

	VOID SetFileLogLevel(enum_LOG_LEVEL	logLevel) { m_eFileLogLevel = logLevel; }
	VOID SetConsoleLogLevel(enum_LOG_LEVEL logLevel) { m_eConsoleLogLevel = logLevel; }
private:
	VOID SwitchBuf();
    VOID MakeLogfilename();
    VOID MakeLogfilenameWithPostfix();
private:
	enum_LOG_LEVEL		m_eFileLogLevel;
	enum_LOG_LEVEL		m_eConsoleLogLevel;

	CHAR		m_szLogFileName[MAX_FILE_NAME];
	CHAR		m_szFileName[MAX_FILE_NAME];
	CHAR        m_szFilePath[MAX_FILE_NAME];
	CHAR		m_pBuf[2*LOG_BUF_LEN];
	CHAR*		m_pFrontBuf;
	CHAR*		m_pBackBuf;
	CHAR*		m_pWriteBuf;
	
	bool		m_bBackBufAvailable;
	CLock		m_lock;

	//��־�ļ����������С�����¿�һ���ļ�
	INT			m_nMaxLogFileLen;
	//��¼��ǰ�����ڣ�������ֿ��һ�죬���¿�һ���ļ�
	INT			m_nDate;
};

#endif
