/***
 *	
 *	file name : Plot_HadTop_DataMC.cc
 *	Data : 191026
 *	Description : Modification of Plotter.h and Plotter.cc
 *	Author : Chen-Yu Chuang
 *
 * **/

#include "TopCPViolation/selected/interface/Plotter.h"
#include "TopCPViolation/prepare_info/interface/Read_info.h"
#include "TopCPViolation/prepare_info/interface/about_time.h"
#include "TopCPViolation/selected/interface/Hists.h"

#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"

#include <string>
#include <iostream>
#include "string.h"

using namespace std;


int main(int argc, char* argv[])
{	
	string option = string( argv[3] );
	string root_file = string( argv[1] );
	Hists hists;
	hists.GetObjs( root_file , option );

	if(option.find("NC") != string::npos)
	{
		string filename = string(argv[2]) + "_HadTop";	
		Plot_Stack( filename, hists.h_mu, hists.h_el  );
	
		string filename2 = string(argv[2]) + "_LepTop";	
		Plot_Stack( filename2, hists.h_l_mu, hists.h_l_el  );
	}
	
	if(option.find("1C") != string::npos)
	{
		string filename = string(argv[2]) + "_HadTop";	
		Plot_Stack( filename, hists.h_mu_c, hists.h_el_c  );
	
		string filename2 = string(argv[2]) + "_LepTop";	
		Plot_Stack( filename2, hists.h_l_mu_c, hists.h_l_el_c  );
	}
	
	if(option.find("2C") != string::npos)
	{
		string filename = string(argv[2]) + "_HadTop";	
		Plot_Stack( filename, hists.h_mu_cc, hists.h_el_cc  );
	
		string filename2 = string(argv[2]) + "_LepTop";	
		Plot_Stack( filename2, hists.h_l_mu_cc, hists.h_l_el_cc  );
	}
	
	if( (option.find("NC") == string::npos) && (option.find("1C") == string::npos) && (option.find("2C") == string::npos) )
	{
		cerr << "Illegal option of plot_Mass ," << endl \
			 << "option should include NC/1C/2C " << endl;
	}
}
