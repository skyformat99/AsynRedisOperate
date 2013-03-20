
#include "LogFile.h"
#include "TimeSystem.h"
#include "Utility.h"
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <stdarg.h>
#include <sys/stat.h>
#include "LogSystem.h"
#include <algorithm>

CLogFile::CLogFile(const CHAR* szFileName, const CHAR* szPath)
{
    //CTimeSystem* pTimeSystem = CTimeSystem::Instance();

	m_eFileLogLevel = LOG_LEVEL_DEBUG;
	m_eConsoleLogLevel = LOG_LEVEL_DEBUG;

	if (szPath == NULL || strlen(szPath) <= 0)
	{
		strncpy(m_szFilePath, "./Log/", MAX_FILE_NAME);
	}
	else
	{
		_snprintf(m_szFilePath, MAX_FILE_NAME, "%s", szPath);
	}

	if (!CheckDirExsist(m_szFilePath))
	{
		DSCreateDirectory(m_szFilePath);
	}

    _snprintf(m_szFileName, MAX_FILE_NAME, "%s", szFileName);

	memset(m_pBuf, 0, 2*LOG_BUF_LEN);
	m_pFrontBuf = m_pBuf;
	m_pBackBuf = m_pBuf + LOG_BUF_LEN;
	m_pWriteBuf = m_pFrontBuf;

	m_bBackBufAvailable = FALSE;

	//����512M���¿�һ���ļ�
	m_nMaxLogFileLen = 512*1024*1024;
	m_nDate = DateTime().GetDay();

    MakeLogfilename();
}

CLogFile::~CLogFile()
{

}

VOID CLogFile::CacheLog(enum_LOG_LEVEL	logLevel, const CHAR* pLogString , ... )
{
	//�ȼ�̫�ͣ������ӡ����̨���ļ�
	if (logLevel < m_eConsoleLogLevel && logLevel < m_eFileLogLevel)
	{
		return ;
	}

	CHAR szLine[MAX_LOG_LINE] = {0};
	va_list	va;
	va_start(va,pLogString);
#ifdef __LINUX
	vsprintf(szLine,pLogString,va);
#else
	vsprintf_s(szLine,MAX_LOG_LINE,pLogString,va);
#endif
	va_end(va);
	
	DateTime now;
	CHAR szLogStr[MAX_LOG_LINE] = {'\0'};
	_snprintf(szLogStr, MAX_LOG_LINE-1, "%2d/%02d/%02d %02d:%02d:%02d.%03d	tId:%ld	%s\n",
		now.GetYear(),
		now.GetMonth(),
		now.GetDay(),
		now.GetHour(),
		now.GetMinute(),
		now.GetSecond(),
		now.GetMilliSecond(),
		GetThreadID(),
		szLine);
	//�ߵ������̨�ȼ������ӡ������̨
	if (logLevel >= m_eConsoleLogLevel)
	{
		printf("%s", szLogStr);
	}

	//�ߵ����ļ��ȼ������ӡ���ļ�
	if (logLevel >= m_eFileLogLevel)
	{
		CSmartLock sLock(m_lock);

		int strLength = strlen(szLogStr);
		if (m_pWriteBuf + strLength - m_pFrontBuf >=  LOG_BUF_LEN)
		{
			//front ��������
			if (m_bBackBufAvailable)
			{
				//����ȫ���ˣ�����Ҫ����
				printf("LOG LOST!!, %s", szLogStr);
				return;
			}
			//back���ţ���front������back
			SwitchBuf();
		}

		//д��front
		strcpy( m_pWriteBuf, szLogStr );
		m_pWriteBuf += strLength;
	}
}

VOID CLogFile::DiskLog(enum_LOG_LEVEL	logLevel, const CHAR* pLogString , ...)
{
	//�ȼ�̫�ͣ������ӡ����̨���ļ�
	if (logLevel < m_eConsoleLogLevel && logLevel < m_eFileLogLevel)
	{
		return ;
	}

	CHAR szLine[MAX_LOG_LINE] = {0};
	va_list	va;
	va_start(va,pLogString);
#ifdef __LINUX
	vsprintf(szLine,pLogString,va);
#else
	vsprintf_s(szLine,MAX_LOG_LINE,pLogString,va);
#endif
	va_end(va);

	DateTime now;
	CHAR szLogStr[MAX_LOG_LINE] = {'\0'};
	_snprintf(szLogStr, MAX_LOG_LINE, "%2d/%02d/%02d %02d:%02d:%02d.%03d	tId:%ld	%s\n", 
		now.GetYear(),
		now.GetMonth(),
		now.GetDay(),
		now.GetHour(),
		now.GetMinute(),
		now.GetSecond(),
		now.GetMilliSecond(),
		GetThreadID(),
		szLine);

	//�ߵ������̨�ȼ������ӡ������̨
	if (logLevel >= m_eConsoleLogLevel)
	{
		printf("%s", szLogStr);
	}

	//�ߵ����ļ��ȼ������ӡ���ļ�
	if (logLevel >= m_eFileLogLevel)
	{
		CSmartLock sLock(m_lock);

        INT nLogStrLen = strlen(szLogStr);

		if ((m_bBackBufAvailable) && 
			(m_pWriteBuf + nLogStrLen - m_pFrontBuf >=  LOG_BUF_LEN))
		{
			//���back�ж����� front��д���£���Ҫд2���ļ��ˣ������������Ӧ�ú���
			WriteBackBufToFile();
		}

		if (m_pWriteBuf + nLogStrLen - m_pFrontBuf >=  LOG_BUF_LEN)
		{
			//frontд���£���ôback�϶�����
			SwitchBuf();
		}

		//д��front
		strcpy( m_pWriteBuf, szLogStr );
		m_pWriteBuf += nLogStrLen;
		
		//ȫд���ļ���
		WriteAllBufToFile();
	}
}

VOID CLogFile::SwitchBuf()
{
	if (m_pFrontBuf != m_pWriteBuf && m_bBackBufAvailable == FALSE)
	{
		//�ж���������2ҳ��front -> back.
		std::swap( m_pFrontBuf, m_pBackBuf );
		m_pWriteBuf = m_pFrontBuf;

		*m_pFrontBuf = '\0';

		m_bBackBufAvailable = TRUE;
	}
}

	//��backbuf������д���ļ�����������ݵĻ�
VOID CLogFile::WriteBackBufToFile()
{
	CSmartLock sLock(m_lock);

	CheckLogFileStat();

	if (m_bBackBufAvailable)
	{
		std::ofstream logFile;
		logFile.open(m_szLogFileName, std::ios::app);
		INT outLen = strlen(m_pBackBuf);
		logFile.write(m_pBackBuf, (outLen>LOG_BUF_LEN)?LOG_BUF_LEN:outLen);
		logFile.close();

		*m_pBackBuf = '\0';
		m_bBackBufAvailable = FALSE;
	}
}


//front �� back ����������д���ļ�
VOID CLogFile::WriteAllBufToFile()
{
	CSmartLock sLock(m_lock);	

	CheckLogFileStat();

	std::ofstream logFile;
	if (m_bBackBufAvailable)
	{
		logFile.open(m_szLogFileName, std::ios::app);
		INT outLen = strlen(m_pBackBuf);
		logFile.write(m_pBackBuf, (outLen>LOG_BUF_LEN)?LOG_BUF_LEN:outLen);
		*m_pBackBuf = '\0';
		
		if (*m_pFrontBuf != '\0' && m_pWriteBuf != m_pFrontBuf)
		{
			outLen = strlen(m_pFrontBuf);
			logFile.write(m_pFrontBuf, (outLen>LOG_BUF_LEN)?LOG_BUF_LEN:outLen);
			*m_pFrontBuf = '\0';
			m_pWriteBuf = m_pFrontBuf;
		}
		logFile.close();

		m_bBackBufAvailable = FALSE;
	}	
	else
	{
		if (*m_pFrontBuf != '\0' && m_pWriteBuf != m_pFrontBuf)
		{
			logFile.open(m_szLogFileName, std::ios::app);
			INT outLen = strlen(m_pFrontBuf);
			logFile.write(m_pFrontBuf, (outLen>LOG_BUF_LEN)?LOG_BUF_LEN:outLen);
			logFile.close();
			*m_pFrontBuf = '\0';
			m_pWriteBuf = m_pFrontBuf;
		}
	}
}

//����ļ���С�������Ҫ�Ļ������½�һ���ļ�
VOID CLogFile::CheckLogFileStat()
{
	bool bNeedNewFile = FALSE;
	DateTime now;
	
	if ( now.GetDay() != m_nDate)
	{
		//����Ƿ����һ��
		m_nDate = now.GetDay();
		bNeedNewFile = TRUE;
	}
	else
	{
		//����ļ���С
		std::fstream logFile;
		logFile.open(m_szLogFileName, std::ios::in);
		logFile.seekg(0, std::ios::end);
		INT length = logFile.tellg();
		logFile.close();

		if (length > m_nMaxLogFileLen)
		{
			bNeedNewFile = TRUE;
		}
	}

	if (bNeedNewFile)
	{
		//��Ҫ�¿�һ���ļ���
		MakeLogfilename();
	}
}

VOID CLogFile::MakeLogfilenameWithPostfix()
{
   DateTime now;

    _snprintf(m_szLogFileName,MAX_FILE_NAME, "%s/%s_%.4d%.2d%.2d_%.2d%.2d%.2d_%.2d.log", m_szFilePath, m_szFileName, now.GetYear(), now.GetMonth(), 
        now.GetDay(), now.GetHour(), now.GetMinute(), now.GetSecond(), now.GetMilliSecond());
}

VOID CLogFile::MakeLogfilename()
{
    if (CLogSystem::Instance()->GetFileNameAddPostfix())
    {
        MakeLogfilenameWithPostfix();
    }
    else
    {
        _snprintf(m_szLogFileName, MAX_FILE_NAME, "%s.log", m_szFileName);
    }
}
