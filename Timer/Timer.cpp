#include "../stdafx.h"
#include "Timer.h"
#include <time.h>
#include <Windows.h>

Timer::Timer()
{
	lastTime = 0;
}

Timer::~Timer()
{
}

Timer *Timer::m_Timer = NULL;

Timer* Timer::GetInstance()
{
	if (m_Timer == NULL)
		m_Timer = new Timer();

	return m_Timer;
}

void Timer::CleanUp()
{
	delete m_Timer;
}

float Timer::GetElapsedTime()
{
	double nowTime = clock();
	float elapsedTime = ClockDiff(nowTime, lastTime);
	lastTime = nowTime;



	return elapsedTime;
}

void Timer::ResetTimer()
{
	lastTime = 0;
}

float Timer::ClockDiff(clock_t clock1, clock_t clock2)
{
	double diffticks=clock1-clock2;
	double diffms=(diffticks)/(CLOCKS_PER_SEC*100);

	return (float)diffms;
}