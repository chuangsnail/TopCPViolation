#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <ctime>
#include <string>

using namespace std;


class Timer
{
private:
	// Type aliases to make accessing nested type easier
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;
		
	std::chrono::time_point<clock_t> m_beg; 				 
public:
	Timer() : m_beg(clock_t::now())
	{
	}

	void reset()
	{
		m_beg = clock_t::now();
	}

	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};

enum Time_Option { date = 0 , date_time = 1 , date_time_to_second = 2 };

string GetTime(Time_Option option)
{
	time_t now = time(0);
	tm* ltm = localtime(&now);
	string today_year = to_string( 1900+ltm->tm_year-2000 );
	string today_month = to_string( 1+ltm->tm_mon );
	string today_day = to_string( ltm->tm_mday );
	//string today_hour = to_string( 1+ltm->tm_hour );		//for winter time
	string today_hour = to_string( ltm->tm_hour );			//for summer time
	string today_min = to_string( 1+ltm->tm_min );

	if(ltm->tm_mon + 1 < 10){
		today_month = "0" + today_month;
	}
	if(ltm->tm_mday  < 10){
		today_day = "0" + today_day;
	}
	if(ltm->tm_hour + 1 < 10){
		today_hour = "0" + today_hour;
	}
	if(ltm->tm_min + 1 < 10){
		today_min = "0" + today_min;
	}

	return ;
}

#endif
