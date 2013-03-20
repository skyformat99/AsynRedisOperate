/*
�ļ�	��Utility.h
���	��
����	����־��
�������ڣ�2009/12/23

����޸�����	�޸���		�޸�����
---------------------------------------------------------------------------------------------

---------------------------------------------------------------------------------------------
*/

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "BaseType.h"

#if defined(__LINUX)
# include <unistd.h>
#include <pthread.h>
#elif defined(__WINDOWS)
//#include <Windows.h>
#endif

typedef void (*SigHandleCallbackFun)();
typedef VOID (*LPDSSDK_LOGFUNCTION)(const CHAR *);

LPDSSDK_LOGFUNCTION SetSdkLogFunction(LPDSSDK_LOGFUNCTION pFun);

void registSignalHandler(const char* szDumpFileName,  SigHandleCallbackFun pAppExitFun);

#ifdef __WINDOWS
# define MySleep Sleep
#else
# define MySleep(x) usleep((x)*1000)
#endif


//�ļ���·����صĹ��ߺ���
VOID ParseFilePath(char* szPath);
VOID GetWorkingDir(const int nBuffSize, char* szBuff);
bool CheckDirExsist(const CHAR* szPath);
bool DSCreateDirectory(const CHAR* szPath);
bool CheckFileExsist(const CHAR* szPath);
VOID DSDeleteFile(const CHAR* szPath);

// [start, end)
INT RangeRandom(INT start, INT end);
FLOAT RangeRandom(FLOAT start, FLOAT end);

// ��ȡһ�����У���n��������ѡ����m��,����array�У�array�е�����Ϊ0��n-1����
VOID GetP( INT m, INT n, INT* array );

//�ڴ�ת����������Ҫ���ڴ洢��
BYTE  HexToByte(CHAR cChar);


/***************************************************************************************************************/
/*  ��������BinaryToString                                                                                     */
/*  ����  ����һ���ڴ�ת��Ϊ�ַ�����ʽ���ַ����ı�����ʽ�����ֱ�ӿ�����ڴ������                             */
/*  ����  ��pBuf ���ڴ�ָ�룬nBufLen ���ڴ�ĳ��ȣ���byte������pszBuf����ַ�����ָ�룬nSzBuffLen�����ַ�������*/
/*  ����ֵ���ɹ�����TRUE,ʧ�ܷ���FALSE                                                                         */
/***************************************************************************************************************/
bool  BinaryToString(const VOID* pBuf, const UINT nBufLen, CHAR* pszBuf, const UINT nSzBuffLen);


/***************************************************************************************************************/
/*  ��������StringToBinary                                                                                     */
/*  ����  ����һ����BinaryToString����ת�����ַ�����ת���ڴ�                                                   */
/*  ����  ��pszBuf Ҫת�����ַ�����pBuf �������ڴ��ָ�룬nLen �������ڴ���ڴ泤�ȣ���Byte����                */
/*  ����ֵ���ɹ�����TRUE,ʧ�ܷ���FALSE                                                                         */
/***************************************************************************************************************/
bool  StringToBinary(const CHAR* pszBuf, VOID* pBuf, UINT nLen);
bool  StringToBinary(const CHAR* pszBufBegin, const CHAR* pszBufEnd, VOID* pBuf, UINT nLen);

/*
BinaryToString��StringToBinary��ʹ�õ����ӣ�
INT nn[10] = {11, 12, 13, 14, 88, 85, -58, 51, 99, 852};
CHAR szBuff[100] = {0};
BinaryToString((PVOID)nn, sizeof(nn), szBuff, 100);
INT output[10] = {0};
StringToBinary(szBuff, (PVOID)output, sizeof(output));

*/

//�߳���صĹ��ߺ���
unsigned long GetThreadID();

// SDK�ڲ�����־����
VOID SDK_Log(const CHAR* pLogString , ...);

// ���ݽ�ɫ������һ��guid������debugģʽ�µ�¼ʹ��
INT CharName2Guid(const CHAR* pCharName, INT nMaxSize);

// �жϸ������Ƿ���Ч
bool IsNaN( FLOAT f );
bool IsNan( DOUBLE d );

// ��ú������е�ջ
#include <string>
const std::string GetStackString();

#endif // __UTILITY_H__
