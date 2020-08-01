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


//namespace hkh {				// hkh namespace�� ����!

//using namespace hkh;		// hkh namespace�� �������. 


// ������
HStopwatch::HStopwatch()
{
	m_flag = false;
	ftime(&m_start);
	m_stop = m_start;
}


// Ÿ�̸� �ð����� ���Ѵ�. ������ msec
double HStopwatch::dt(const timeb &s, const timeb &e) const
{
	double delta;
	delta = 1.0e3 * difftime(e.time, s.time);
	delta += (double)(e.millitm - s.millitm);
	return delta;
}


// Ÿ�̸� �ð��� �б�
// ���� Ÿ�̸Ӱ� �������̸�: m_start���� ���ݱ��� �ð� ����
// Ÿ�̸Ӱ� �������̸�: m_start�� m_stop �ð��� ����
double HStopwatch::getDT() const
{ 
	if (m_flag) {						// �������̸�...
		timeb tmp;
		ftime(&tmp);
		return dt(m_start, tmp);
	}
	else								// �������¸�...
		return dt(m_start, m_stop);
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// friend ��� ������

std::ostream& operator << (std::ostream& os, const HStopwatch& TM)
{
	os << TM.dt(TM.m_start, TM.m_stop) << "msec" << std::endl;
	return os;
}


//};						// hkh namespace�� ����!
