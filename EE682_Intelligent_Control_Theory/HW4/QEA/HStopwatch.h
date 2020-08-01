//////////////////////////////////////////////////////////////////////////////
//
//	file: HStopwatch.h
//	Date: 2002.3.2
//	Author: Kuk-Hyun Han
//	Description: stopwatch class
//
//////////////////////////////////////////////////////////////////////////////


#ifndef	_HSTOPWATCH_H_
#define	_HSTOPWATCH_H_

#include <iostream>
#include <sys/timeb.h>

//namespace hkh {				// hkh namespace로 묶자!

class HStopwatch {

	timeb	m_start;		// 타이머 시작 시간
	timeb	m_stop;			// 타이머 종료 시간
	bool	m_flag;			// 타이머 동작 여부

	double dt(const timeb &s, const timeb &e) const;		// 타이머 시간차를 구한다.

public:

	HStopwatch();					// 생성자

	void start() {
		if (!m_flag) {
			ftime(&m_start);	// 타이머 ON
			m_flag = true;
		}
	}
	
	void stop() {
		if (m_flag) {
			ftime(&m_stop);		// 타이머 OFF
			m_flag = false;		
		}
	}

	// 타이머를 stop 했더라도 resume하면 시작 시간을 유지한채 다시 동작한다.
	void resume() { m_flag = true; }		// 타이머 RESUME

	// 타이머 시간차 읽기
	double getDT() const;

	// 출력 연산자
	friend std::ostream& operator << (std::ostream& os, const HStopwatch& TM);
};


//}				// hkh namespace로 묶자!

#endif
// !_HSTOPWATCH_H_
