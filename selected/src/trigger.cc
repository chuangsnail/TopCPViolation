#include "TopCPViolation/selected/interface/trigger.h"

using namespace std;

void Get_HLT_Info( string key, vector<int>& HLT_info )
{
	vector<int> info;	
	if(key == "MC_mu")
	{

		info.push_back( 1319 );
		info.push_back( 2415 );
/*		
		info.push_back( 2412 );
		info.push_back( 2413 );
		info.push_back( 2414 );
		info.push_back( 2415 );
*/
	}
	else if(key == "Data_mu")
	{
		info.push_back( 1315 );	
		info.push_back( 1316 );	
		info.push_back( 1317 );	
		info.push_back( 1318 );	
		info.push_back( 1319 );	
		info.push_back( 1320 );	
		info.push_back( 1321 );	
		info.push_back( 1322 );	
		info.push_back( 1323 );	
		info.push_back( 1324 );
///*
		info.push_back( 2412 );			
		info.push_back( 2413 );			
		info.push_back( 2414 );			
		info.push_back( 2415 );	
//*/		
	}
	else if(key == "MC_el")
	{

		info.push_back( 923 );	
		
/*		
		info.push_back( 2412 );
		info.push_back( 2413 );
		info.push_back( 2414 );
		info.push_back( 2415 );
*/
	}
	else if(key == "Data_el")
	{
		info.push_back( 917 );		
		info.push_back( 918 );		
		info.push_back( 919 );		
		info.push_back( 920 );		
		info.push_back( 921 );		
		info.push_back( 922 );		
		info.push_back( 923 );		
	}
	else
	{
		cout << "There is no option of this HLT trigger!" << endl;
	}

	for(int i=0;i<(int)info.size();i++)
	{
		HLT_info.push_back( info.at(i) );
	}	
}

bool Pass_HLT( TrgInfo& trg, const vector<int>& HLT_info )
{
	for(int i=0;i<(int)HLT_info.size();i++)
	{
		if( trg.TrgBook[ HLT_info.at(i) ] == 1 )
		{
			return true;
		}
	}	
	return false;
}

double lumi_cali_trg(double frac)
{
	return frac;
}


//if there is a calibration on luminosity when apply trigger

double trigger_calib( const string& channel )
{
	double calib = 1.;
	if( channel == "el" )
	{
		//calib *= 35.615/35.9;
	}
	else if( channel == "mu" )
	{
		//calib *= 35.811/35.9;
	}
	return calib;
}

