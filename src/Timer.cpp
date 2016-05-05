// Timer.cpp: implementation of the Timer class.
//
//////////////////////////////////////////////////////////////////////

#include "Timer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Timer::Timer():myWriter(0) {}

Timer::Timer(const char* filename)
{
	fopen_s(&myWriter,filename, "a");
}

Timer::~Timer()
{
	if (myWriter)
		fclose(myWriter);
}

void Timer::Start() {
	myTimer.Reset();
	myTimer.Start();
}

void Timer::Stop() {
	myTimer.Stop();
}

void Timer::Continue() {
	myTimer.Start();
}

void Timer::Reset() {
	myTimer.Reset();
}

float Timer::Seconds() {
  Standard_Real sec, cpu;
  Standard_Integer minutes, hours;
  myTimer.Show(sec, minutes, hours, cpu);
  return (float) (sec + minutes * 60 + hours * 60 * 60);
}

int Timer::Minutes() {
  Standard_Real sec, cpu;
  Standard_Integer minutes, hours;
  myTimer.Show(sec, minutes, hours, cpu);
  return minutes;
}

void Timer::Print(char* label) {
	Standard_Real seconds, cpu;
	Standard_Integer minutes, hours;
	myTimer.Show(seconds, minutes, hours, cpu);
	if (myWriter)
	{
		fprintf(myWriter, "%s took %d minutes %g seconds\n", label, minutes, seconds);
	}
	else
	{
		cout<<label<<" took "<<minutes<<" minutes, "<<seconds<<" seconds"<<endl;
	}
}
std::string Timer::WhatTime(){
	std::string time;
	Standard_Real seconds, cpu;
	Standard_Integer minutes, hours;
	myTimer.Show(seconds, minutes, hours, cpu);

	time += std::to_string(minutes) + ":" + std::to_string(seconds);
	return time;
}