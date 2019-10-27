#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <iostream>
#include <string>
#include "stdlib.h"

//can use static member to count the times it calls
class Debugger
{
	public:
		string info;
		int step_no;

		Debugger( string information )
		{
			info = information;
			step_no = 1;	
		}

		void print_info();
		void print_info( string information , bool change_step );

		~Debugger()
		{}
};

void Debugger::print_info()
{
	cout << info << 
}


void print_info( string information , bool change_step )
{
	
}


#endif
