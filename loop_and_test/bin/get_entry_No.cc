/********************************************************************
 *
 *	file name : get_entry_No.cc 
 *	Execute command : GetEntries {file_name} {TTree_name or TChain_name in this file}
 *					  GetEntries {file1_name} {TChian_name for file1} {file2_name} {TChain_name for file2}
 *	data : 2019 May 10th
 *	Author : Chen-Yu Chaung
 *
 * *******************************************************************/

#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"

using namespace std;

int main( int argc , char* argv[] )
{
	if( argc != 3 && argc != 5 )
	{
		printf("We need \"3\" or \"5\" arguments here (including execute argument)");
	}
	else if( argc == 3)
	{
		//TChain* ch = new TChain( "root" );	//for pre_sel_file
		//TChain* ch = new TChain( "bprimeKit/root" );	//for original file
		TChain* ch = new TChain( argv[2] );
		ch->Add( argv[1] );

		cout << endl << "The Entries of file [" << std::string(argv[1]) << "] is : " << ch->GetEntries();
		delete ch;
	}
	else if( argc == 5)
	{
		//TChain* ch1 = new TChain( "root" );
		TChain* ch1 = new TChain( argv[2] );
		ch1->Add( argv[1] );
		int entries1 = ch1->GetEntries();
		delete ch1;

		//TChain* ch2 = new TChain( "root" );
		TChain* ch2 = new TChain( argv[4] );
		ch2->Add( argv[3] );
		int entries2 = ch2->GetEntries();
		delete ch2;

		
		cout << endl << "The Entries of file [" << std::string(argv[1]) << "] is : " << entries1;
		cout << endl << "The Entries of file [" << std::string(argv[2]) << "] is : " << entries2;
		cout << endl << "The entries' difference of these two file is " << entries1 - entries2 << endl;
	}
}
