#ifndef __DATETIME_H__
#define __DATETIME_H__

#include "BaseType.h"
#include <time.h>

#if defined(__WINDOWS)
# include <sys/timeb.h>
#else
# include <sys/time.h>
#endif

class DateTime
{
public:
	DateTime();
	DateTime( int second, int milliSecond );

	//����ʱ�䣺
	INT GetYear();
	INT GetMonth();
	INT GetDay();
	// 0-6 Sunday-Saturday
	INT GetWeekDay();
	INT GetHour();
	INT GetMinute();
	INT GetSecond();
	INT GetMilliSecond();

	//1970 ���������
	LONGLONG GetBigSecond();
    //1970 ����ĺ�����
	LONGLONG GetBigMs();

private:

#ifdef __LINUX
	struct timeval m_tv;
#else
	time_t		m_time;
	int			m_milliSecond;
#endif
};

#endif // __DATETIME_H__ 
