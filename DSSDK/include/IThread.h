/*
�ļ�	��IThread.h
���	��
����	����־��
�������ڣ�2009/12/23

����޸�����	�޸���		�޸�����
---------------------------------------------------------------------------------------------

---------------------------------------------------------------------------------------------
*/


#ifndef __ITHREAD_H__
#define __ITHREAD_H__

#include "BaseType.h"

#ifdef __WINDOWS
#define THREADPROC DWORD WINAPI
#elif defined (__LINUX)
#define THREADPROC void*
#endif

class IThread
{
public:
	IThread(void);
	virtual ~IThread(void);
	// ��ʼ�߳�
	void Start(void);
    // �����߳�
    void Stop(void);
    virtual bool IsRunning();
    static int GetThreadCount(){ return s_ThreadCount; };
protected:
	virtual void _Run(void) = 0;
    virtual VOID OnExit();
private:
	static THREADPROC __ThreadProc(void* lParam);
private:
    volatile bool m_bRun;
    static volatile int s_ThreadCount;
public:
    static void __DecreaseCount(); //����sdk�ڲ�����
};
#endif
