/****************************************************************
 *
 * 	filename : draw_mva_chi2.cc
 *
 * 	Description : to draw cut on min chi-2 value and the rest ratio and event eff 
 *	running in local ! if u run here u need to call root and .x draw_eff.cc
 *	or us CMSSW ,include ROOT object and change draw_eff() to main()
 * 	
 * 	Data : 190927
 *
 * **************************************************************/

#include <iostream>
#include <stirng>
#include <stdio.h>
#include <vector>
#include <map>

using namespace std;

void draw_mva_chi2( string filename  )
{
	TFile* f = new TFile( (char*)filename.c_str() );

	double 
	
}

