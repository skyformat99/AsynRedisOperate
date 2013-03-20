/*
�ļ�	��TimeSystem.h
���	��
����	����־��
�������ڣ�2009/12/23

����޸�����	�޸���		�޸�����
---------------------------------------------------------------------------------------------

---------------------------------------------------------------------------------------------
*/

#ifndef	_TIME_SYSTEM_H_
#define	_TIME_SYSTEM_H_

#include "BaseType.h"
#include <time.h>
#include "DateTime.h"

class CTimeSystem
{
public:
	CTimeSystem(){}
	~CTimeSystem(){}

public:
	static CTimeSystem* Instance();
	VOID InitialStartUp();
	
public:
	//1970 ����������ͺ�����
	LONGLONG GetBigSecond();
	LONGLONG GetBigMs();

	//���ʱ�䣬�Գ���������ʼ��
	//ϵͳ�������������
	INT GetSecondFromStart();
	//ϵͳ��������ĺ�����
	INT GetMsFromStart();
private:
	static CTimeSystem*	m_pInstance;	

private:
	DateTime		m_dateTime;
};


#endif
