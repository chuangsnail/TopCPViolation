#include "TopCPViolation/selected/interface/BranchInfo.h"

#include <iostream>
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"

using namespace std;

int main()
{
	TChain* cor = new TChain( "correct" );
	TChain* incor = new TChain( "incorrect" );
	
	cor->Add( "/wk_cms2/cychuang/comb_*.root" );
	incor->Add( "/wk_cms2/cychuang/comb_*.root" );

	double c_hadb_deepcsv_v;
	double c_hadb_Pt;
	double i_hadb_deepcsv_v;
	
	cor->SetBranchAddress( "hadb_deepcsv_v", &c_hadb_deepcsv_v );
	cor->SetBranchAddress( "hadb_Pt", &c_hadb_Pt );
	incor->SetBranchAddress( "hadb_deepcsv_v", &i_hadb_deepcsv_v );

	int c_t_entries = cor->GetEntries();
	int i_t_entries = incor->GetEntries();

	/*
	for(int entry=0;entry<c_t_entries;entry++)
	{
		cor->GetEntry(entry);
		cout << c_hadb_Pt << endl;	
	}
	*/

	/*
	for(int entry=0;entry<c_t_entries;entry++)
	{
		cor->GetEntry(entry);

		cout << c_hadb_deepcsv_v << endl; 
	}
	*/
	
	int minus_num = 0;
	int one_num = 0;

	for(int entry=0;entry<c_t_entries;entry++)
	{
		cor->GetEntry(entry);
		if( c_hadb_deepcsv_v < 0 )
		{
			cout << "In entry " << entry << "minus num of deep csv : " << c_hadb_deepcsv_v << endl;
			minus_num++;
		}
		if( c_hadb_deepcsv_v > 1 )
		{
			cout << "In entry " << entry << ">1 num of deep csv : " << c_hadb_deepcsv_v << endl;
			one_num++;
		}
	}

	cout << "t_entries : " << c_t_entries << endl;
	cout << "minus_num : " << minus_num << endl;
	cout << "one_num : " << one_num << endl;	

	/*
	
	for(int entry=0;entry<i_t_entries;entry++)
	{
		incor->GetEntry(entry);
		if( i_hadb_deepcsv_v < 0 )
		{
			cout << "In entry " << entry << "minus num of deep csv : " << i_hadb_deepcsv_v << endl;
		}
	}
*/

	

	
}
