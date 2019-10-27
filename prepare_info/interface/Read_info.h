#ifndef READ_INFO_H
#define READ_INFO_H

#include "Reader.h"
#include <iostream>
#include <string>
#include "string.h"
#include <vector>
#include <map>
#include <fstream>

using namespace std;

class Read_info : public Reader
{
public:
    Read_info(char* f):Reader(f)
    {
    }
    string second(string&);
};

string Read_info::second(string& key)
{
    
    string line;
    while(!infile->eof())
    {
        line = this->get_1_line();
        if(line.find(key) != string::npos)
        {   break;  }
    }
    if( infile->eof() || line == "NAN" )
    {
		this->reset_line_pin();	
        return "NAN";
    }
    
    string gotten_str;
    
    int line_len = line.size() + 1;
    char* c_in_str = new char[line_len];
    strcpy( c_in_str, line.c_str() );
    char* c_gotten_str = new char[line_len];
    
    int k = 0;      bool get_it = false;
    for(int i=0;i<line_len;i++)
    {
        if( c_in_str[i] == ' ' || c_in_str[i] == ':' )
        {
            if( c_in_str[i+1] == ' ' || c_in_str[i+1] == ':' )
            {
                continue;
            }
            get_it = true;
            continue;      //break out of the for-loop
        }
        if(get_it)
        {
            c_gotten_str[k] = c_in_str[i];
            k++;
        }
    }
    if(c_gotten_str == NULL)
    //if(*c_gotten_str == "")
    {
		this->reset_line_pin();	
        return "NAN";
    }

    gotten_str = string( c_gotten_str );
	this->reset_line_pin();	
    return gotten_str;
}


#endif
