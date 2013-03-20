/*
	�ļ�	��AssertDefine.h
	���	���������쳣����ĺ궨�壬��ע����ÿ�������Ŀ�ͷ����__ENTER_FUNCTION__���ڽ�βʱ����__LEAVE_FUNCTION__������ÿ����Assertʱ����ӿ�׷����ٶȡ�
	����	����־��
	�������ڣ�2010/05/06

	��Ȩ����(C) 2009-2010 �������£��������Ƽ����޹�˾

	����޸�����	�޸���		�޸�����
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/

#ifndef __ASSERTDEFINE_H__
#define __ASSERTDEFINE_H__

#include "GlobalDefine.h"
#include "LogSystem.h"
#include <stdio.h>
#include "Utility.h"

#ifdef __LINUX
#define Assert(expr)				if(!(expr)) { DiskLog_ERROR(LOG_ASSERT, " Assert in:"#expr"; File:%s:%d; Function:%s\n%s", __FILE__, __LINE__, __FUNCTION__, GetStackString().c_str() );throw 0;}
#define AssertEx(expr, assertMsg)	if(!(expr)) { DiskLog_ERROR(LOG_ASSERT, " Assert in:"#expr"; File:%s:%d; Function:%s; Reason:%s\n%s", __FILE__, __LINE__, __FUNCTION__, assertMsg, GetStackString().c_str());throw 0;}
#define AssertExcept(expr)			if(!(expr)) { DiskLog_ERROR(LOG_ASSERT, "Stack: File:%s:%d; Function:%s", __FILE__, __LINE__, __FUNCTION__ );throw;}
#elif defined (__WINDOWS)
#include "Windows.h"
//#define AssertMessageBox(expr)     {MessageBox(NULL, expr, "Assert", MB_OKCANCEL);}

#include <signal.h>
#if defined (_M_IX86)
#define _DbgBreak() __asm { int 3 }
#elif defined (_M_IA64)
void __break(int);
#pragma intrinsic (__break)
#define _DbgBreak() __break(0x80016)
#else  /* defined (_M_IA64) */
#define _DbgBreak() DebugBreak()
#endif  /* defined (_M_IA64) */

extern void __cdecl AssertMessageBox( const char* expr );


#define Assert(expr)               if(!(expr)) {                                 \
                                      CHAR szBuff[MAX_LOG_LINE] = {0};        \
									  _snprintf(szBuff, MAX_LOG_LINE-1, "Assert in:%s; File:%s:%d; Function:%s", #expr, __FILE__, __LINE__, __FUNCTION__);    \
                                      AssertMessageBox(szBuff);                \
                                      DiskLog_ERROR(LOG_ASSERT, szBuff );throw 0;}

#define AssertEx(expr, assertMsg)		if(!(expr)) {                           \
										CHAR szBuff[MAX_LOG_LINE] = {0};        \
										_snprintf(szBuff, MAX_LOG_LINE-1, "Assert in:%s; File:%s:%d; Function:%s; Reason:%s", #expr, __FILE__, __LINE__, __FUNCTION__, assertMsg);    \
										AssertMessageBox(szBuff);                \
										DiskLog_ERROR(LOG_ASSERT, szBuff );throw 0;}	

#define AssertExcept(expr)         if(!(expr)) { DiskLog_ERROR(LOG_ASSERT, "Stack: File:%s:%d; Function:%s", __FILE__, __LINE__, __FUNCTION__ );throw;}
#endif

#define __ENTER_FUNCTION__         try {
#define __LEAVE_FUNCTION__         } catch(...){ AssertExcept(0);}

#endif // __ASSERTDEFINE_H__
