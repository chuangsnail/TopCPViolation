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

#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"

#include <string>
#include <iostream>
#include "string.h"

using namespace std;


int main(int argc, char* argv[])
{	
	TFile* f = new TFile(argv[1]);

	TH1F* h_TT_mu;
	TH1F* h_DY_mu;
	TH1F* h_WJets_mu;
	TH1F* h_VV_mu;
	TH1F* h_ST_mu;
	TH1F* h_QCD_mu;
	TH1F* h_Data_mu;

	TH1F* h_TT_el;
	TH1F* h_DY_el;
	TH1F* h_WJets_el;
	TH1F* h_VV_el;
	TH1F* h_ST_el;
	TH1F* h_QCD_el;
	TH1F* h_Data_el;

	f->GetObject("h_TT_mu",h_TT_mu);
	f->GetObject("h_DY_mu",h_DY_mu);
	f->GetObject("h_WJets_mu",h_WJets_mu);
	f->GetObject("h_VV_mu",h_VV_mu);
	f->GetObject("h_ST_mu",h_ST_mu);
	f->GetObject("h_QCD_mu",h_QCD_mu);
	f->GetObject("h_Data_mu",h_Data_mu);

	f->GetObject("h_TT_el",h_TT_el);
	f->GetObject("h_DY_el",h_DY_el);
	f->GetObject("h_WJets_el",h_WJets_el);
	f->GetObject("h_VV_el",h_VV_el);
	f->GetObject("h_ST_el",h_ST_el);
	f->GetObject("h_QCD_el",h_QCD_el);
	f->GetObject("h_Data_el",h_Data_el);

	vector<TH1F*> h_mu;
	h_mu.push_back(h_TT_mu);		h_mu.push_back(h_DY_mu);
	h_mu.push_back(h_WJets_mu);		h_mu.push_back(h_VV_mu);
	h_mu.push_back(h_ST_mu);		h_mu.push_back(h_QCD_mu);
	h_mu.push_back(h_Data_mu);

	vector<TH1F*> h_el;
	h_el.push_back(h_TT_el);		h_el.push_back(h_DY_el);
	h_el.push_back(h_WJets_el);		h_el.push_back(h_VV_el);
	h_el.push_back(h_ST_el);		h_el.push_back(h_QCD_el);
	h_el.push_back(h_Data_el);

	/*
	if( !(std::string(argv[1]).find("had") != string::npos && std::string( argv[1] ).find("SR") != string::npos) ){
	hist_name.push_back("QCD");			//if we don't want to apply this data_set , just comment this line
	}
	*/

	string filename = string(argv[2]) + "_HadTop";	
	Plot_Stack( filename, h_mu, h_el  );
}
