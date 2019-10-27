#ifndef ABOUT_TIME_H
#define ABOUT_TIME_H

#include <string>
#include "string.h"
#include <ctime>

using namespace std;

enum time_option { day , hour , minute };	//{0,1,2}

string get_time_str(time_option opt)
{
	time_t now = time(0);
    tm* ltm = localtime(&now);
    string today_year = to_string( 1900+ltm->tm_year-2000 );
    string today_month = to_string( 1+ltm->tm_mon );
    string today_day = to_string( ltm->tm_mday );
    string today_hour = to_string( ltm->tm_hour );
    string today_min = to_string( 1+ltm->tm_min );
                                                                   
    if(ltm->tm_mon + 1 < 10){
    	today_month = "0" + today_month;
    }
    if(ltm->tm_mday  < 10){
    	today_day = "0" + today_day;
    }
    if(ltm->tm_hour < 10){
    	today_hour = "0" + today_hour;
    }
    if(ltm->tm_min + 1 < 10){
    	today_min = "0" + today_min;
    }
    
	string save_time = "";
	switch(opt)
	{
		case day:
    		save_time = today_year + today_month + today_day;
			break;
		case hour:
    		save_time = today_year + today_month + today_day + "_"\
    		+ today_hour;
			break;
		case minute:
    		save_time = today_year + today_month + today_day + "_"\
    		+ today_hour + today_min;
			break;
		default:
    		save_time = today_year + today_month + today_day + "_"\
    		+ today_hour + today_min;
	}	
	return save_time;                     
}

#endif
