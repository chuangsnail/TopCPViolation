#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TopCPViolation/selected/interface/BranchInfo.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		printf("\nThis is something wrong with the arguments' number!");
		return 0;
	}

	TChain* root = new TChain( argv[2] );
    
	//char star_point_root[50] = "bpk_ntuple_*.root";
	char star_point_root[50] = "*.root";
	char pre_name[300];
	strcpy( pre_name,argv[1] );
	strcat( pre_name,star_point_root );

	root->Add( pre_name );

	GenInfo gen;
	gen.Register( root );
	int positive_number = 0;
	int negative_number = 0;

	for( int i=0;i<(int)root->GetEntries();i++ )
	{
		if(i%1000000 == 0)
		{	cout << endl << "---Have run " << i << " entries---";	}

		root->GetEntry( i );

		//cout << "gen.Weight :  " << gen.Weight << endl;
		//cout << "gen.Size : " << gen.Size << endl;

		if( gen.Weight > 0 )
		{	positive_number++;	}
		else
		{	negative_number++;	}
	}

	int total_gen_no = positive_number - negative_number;

	
	ofstream f;
	string file_name(pre_name);		//use c++ string's constructor to convert c-string 'pre_name' to c++ string 'file_name'
	f.open("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/MC_GenNumber_2016legacy.txt",fstream::app);		
	//'ios::out' means just open the file	//"ios::out" used by fstream object?
	//'fstream::app' means we just append something on this file
	f << "\nFile_name:" << endl << file_name << " gen number (with secondary correction) : " << total_gen_no \
	<< endl << "GetEntries() : " << root->GetEntries() ;
	//make it convenient to be taken
	f.close();

}

