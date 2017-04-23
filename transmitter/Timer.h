// Timer.h

#ifndef _TIMER_h
#define _TIMER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

///
/// Проверяет истечение интервала времени
/// В конструкторе указывается отсчитываемый интервал в миллисекундах.
/// Таймер запускается вызовом start(). Истечение интервала проверяется
/// вызовом elapsed(). В любой момент таймер можно перезапустить
/// повторным вызовом start(). Если elapsed() вызвана до первого start(),
/// возвращает true.
///
class Timer
{
	public:
	Timer(unsigned long period):
	m_started(false),
	m_start(0),
	m_period(period)
	{
	}
	
	bool elapsed()
	{
		if (!m_started) return true;
		if (millis() - m_start > m_period)
		{
			m_started = false;
			return true;
		}
		return false;
	}
	
	void start()
	{
		m_started = true;
		m_start = millis();
	}
	
	private:
	bool m_started;
	unsigned long m_start;
	unsigned long m_period;
};

#endif

