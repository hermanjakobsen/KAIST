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

//namespace hkh {				// hkh namespace�� ����!

class HStopwatch {

	timeb	m_start;		// Ÿ�̸� ���� �ð�
	timeb	m_stop;			// Ÿ�̸� ���� �ð�
	bool	m_flag;			// Ÿ�̸� ���� ����

	double dt(const timeb &s, const timeb &e) const;		// Ÿ�̸� �ð����� ���Ѵ�.

public:

	HStopwatch();					// ������

	void start() {
		if (!m_flag) {
			ftime(&m_start);	// Ÿ�̸� ON
			m_flag = true;
		}
	}
	
	void stop() {
		if (m_flag) {
			ftime(&m_stop);		// Ÿ�̸� OFF
			m_flag = false;		
		}
	}

	// Ÿ�̸Ӹ� stop �ߴ��� resume�ϸ� ���� �ð��� ������ä �ٽ� �����Ѵ�.
	void resume() { m_flag = true; }		// Ÿ�̸� RESUME

	// Ÿ�̸� �ð��� �б�
	double getDT() const;

	// ��� ������
	friend std::ostream& operator << (std::ostream& os, const HStopwatch& TM);
};


//}				// hkh namespace�� ����!

#endif
// !_HSTOPWATCH_H_
