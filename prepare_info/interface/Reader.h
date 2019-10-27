#ifndef READER_H
#define READER_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <iterator>
#include <string>
#include <string.h>			//strlen(char*)
#include <stdlib.h>
#include <fstream>
//#include <ifstream>

using namespace std;

class Reader
{
public:
	char* file_name;
	int file_name_len;
	int line_pin;
	ifstream* infile;

	//Constructor
	Reader(char* f)
	{
		infile = new ifstream;
		file_name_len = strlen(f) + 1;
		file_name = new char[ file_name_len ];
		for(int i=0;i<(int)file_name_len;i++)
		{
			file_name[i] = f[i];	
		}
		file_name[file_name_len-1] = '\0';
		line_pin = 0;
	}

	~Reader()
	{
		delete infile;
		delete [] file_name;
	}

	string get_1_line();
	void reset_line_pin();
	void set_line_pin( int );
    int get_line_pin();
	Reader& operator=( const Reader& reader );

};

//Need to make a Null string and call it by reference to store the read information in it
string Reader::get_1_line()
{
    line_pin++;
	infile->open( file_name );

	string in_str = "";
	string gotten_str = "";
	for(int i=0;i<(int)this->line_pin;i++)
    {  getline( *infile , in_str );  }
	if( !infile->eof() ) 
	{
        char* c_in_str;
        int c_in_str_len = in_str.size() + 1;//Get length from a string
        c_in_str = new char[c_in_str_len];
        strcpy( c_in_str ,(in_str).c_str());
        /*
        for(int i=0;i<(int)c_in_str_len;i++)
        {
            c_in_str[i] = in_str.c_str()[i];
        }
        */
		char* c_gotten_str;
        c_gotten_str = new char[c_in_str_len];
		for(int i=0;i<(int)c_in_str_len;i++)
		{
			if( c_in_str[i] == '#')
			{
                if(c_gotten_str == NULL)
                //if(*c_gotten_str == "")
                {   return "NAN";   }
                //gotten_str = string( c_gotten_str );
                break;      //break out of the for-loop
            }
            c_gotten_str[i] = c_in_str[i];
		}
        if( c_gotten_str == NULL)      //e.g. beginning of the line is '#'
    	//if(*c_gotten_str == "")
        {   gotten_str = "NAN";   }
		gotten_str = string( c_gotten_str );
        
        delete [] c_in_str;
        delete [] c_gotten_str;
        
        infile->close();
        
        return gotten_str;
	}
	else
	{
		this->line_pin = 0;
        infile->close();
        cout << "\nThere is at the end of this file!" << endl;
        return NULL;
	}
}

void Reader::reset_line_pin()
{	line_pin = 0;	}

void Reader::set_line_pin(int l)
{	line_pin = l;	}

int Reader::get_line_pin()
{
    return line_pin;
}


//Deep Copy in C/C++
Reader& Reader::operator=( const Reader& reader )
{
	//if they have been the same yet.
	if(this == &reader)
	{
		return *this;
	}

	if( this->file_name )			//if(this->file_name != NULL)
	{	
		delete [] file_name;
		delete infile;
	}
	
	int new_file_name_len = reader.file_name_len;
	this->file_name = new char[ new_file_name_len ];
	for(int i=0;i<new_file_name_len;i++)
	{
		this->file_name[i] = reader.file_name[i];
		this->infile = reader.infile;
	}
	this->file_name[ file_name_len-1 ] = '\0';

	this->line_pin = 0;

	return *this;
}	

/*
void Reader::read1Line(  )
{
	ifstream f;
	f.open( file_name , std::ifstream::in );
	//f.open( file_name );
	//
	//ifstream f( file_name );
	

}
*/

#endif
