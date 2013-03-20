/*
	�ļ�	��ServerBase.h
	���	������������Ļ��� app
	����	��cuiyinjie
	�������ڣ�2010/01/06

	��Ȩ����(C) 2009-2010 �������£��������Ƽ����޹�˾

	����޸�����	�޸���		�޸�����
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/

#ifndef __SERVERBASE_H__
#define __SERVERBASE_H__

#include "BaseType.h"
#include "Utility.h"

#include <time.h>
#include <stdlib.h>

// ʵ�����绷���ĳ�ʼ�����������������ʼ��������ʱ����
class CServerBase
{
public:
    CServerBase(){ m_nSleepTimeMs = 1000;m_nLastPerSecondsProcessTime=0; m_bNetWorkStarted = FALSE;};
    virtual ~CServerBase(){ CleanUp(); };
    bool Init();
    VOID Start();
    VOID Stop();
    VOID SetSleepTimeMs(INT nMilliseconds);;
    VOID CleanUp();

protected:
    virtual VOID DoStart() = 0;
	virtual VOID DoTick() = 0;
	VOID DoLogFlush();
    virtual bool DoInitGameData() = 0;
    virtual const CHAR* ToString(){ return "ServerBase"; };
    virtual VOID OnExit(){}; // �˳��Ĵ���
	static	VOID sdkLogCallBack(const CHAR* szLogText);

private:
    bool	InitNetWork(); 
	void	ProcessPerSeconds();
	void	StopCheck();

private:
    volatile bool m_bRun;
    INT m_nSleepTimeMs;
	bool m_bNetWorkStarted;
	INT m_nLastPerSecondsProcessTime;
};

#endif // __SERVERBASE_H__
