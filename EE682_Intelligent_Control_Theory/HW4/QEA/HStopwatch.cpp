//////////////////////////////////////////////////////////////////////////////
//
//	file: HStopwatch.cpp
//	Date: 2002.3.2
//	Author: Kuk-Hyun Han
//	Description: stopwatch class
//
//////////////////////////////////////////////////////////////////////////////
	

#include <time.h>
#include "HStopwatch.h"


//namespace hkh {				// hkh namespace로 묶자!

//using namespace hkh;		// hkh namespace를 사용하자. 


// 생성자
HStopwatch::HStopwatch()
{
	m_flag = false;
	ftime(&m_start);
	m_stop = m_start;
}


// 타이머 시간차를 구한다. 단위는 msec
double HStopwatch::dt(const timeb &s, const timeb &e) const
{
	double delta;
	delta = 1.0e3 * difftime(e.time, s.time);
	delta += (double)(e.millitm - s.millitm);
	return delta;
}


// 타이머 시간차 읽기
// 현재 타이머가 동작중이면: m_start부터 지금까지 시간 리턴
// 타이머가 정지중이면: m_start와 m_stop 시간차 리턴
double HStopwatch::getDT() const
{ 
	if (m_flag) {						// 동작중이면...
		timeb tmp;
		ftime(&tmp);
		return dt(m_start, tmp);
	}
	else								// 정지상태면...
		return dt(m_start, m_stop);
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// friend 출력 연산자

std::ostream& operator << (std::ostream& os, const HStopwatch& TM)
{
	os << TM.dt(TM.m_start, TM.m_stop) << "msec" << std::endl;
	return os;
}


//};						// hkh namespace로 묶자!
