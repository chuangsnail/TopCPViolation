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

	vector<TH1F*> h_MCs_mu;
	h_MCs_mu.push_back(h_TT_mu);		h_MCs_mu.push_back(h_DY_mu);
	h_MCs_mu.push_back(h_WJets_mu);		h_MCs_mu.push_back(h_VV_mu);
	h_MCs_mu.push_back(h_ST_mu);		h_MCs_mu.push_back(h_QCD_mu);

	vector<TH1F*> h_MCs_el;
	h_MCs_el.push_back(h_TT_el);		h_MCs_el.push_back(h_DY_el);
	h_MCs_el.push_back(h_WJets_el);		h_MCs_el.push_back(h_VV_el);
	h_MCs_el.push_back(h_ST_el);		h_MCs_el.push_back(h_QCD_el);

	vector<string> hist_name;
	hist_name.push_back("Data");		hist_name.push_back("t#bar{t}");
	hist_name.push_back("Drell-Yan");	hist_name.push_back("W+Jets");
	hist_name.push_back("VV(WW,WZ,ZZ)");hist_name.push_back("Single top");
	hist_name.push_back("QCD");

	/*
	if( !(std::string(argv[1]).find("had") != string::npos && std::string( argv[1] ).find("SR") != string::npos) ){
	hist_name.push_back("QCD");			//if we don't want to apply this data_set , just comment this line
	}
	*/

	//Plot!!
	vector<string> text_mu;
	text_mu.push_back( "Muon channel" );
	text_mu.push_back( "Luminosity : 35.811fb^{-1}" );

	vector<string> text_el;
	text_el.push_back( "Electron channel" );
	text_el.push_back( "Luminosity : 35.615fb^{-1}" );

	string _condi = "";

	string file_name_ps = "";
	if(argv[2] != NULL){
		file_name_ps =string( argv[2] );
	}

	if(argv[3] != NULL){
		_condi = string(argv[3]);	
	}

	if( _condi != "")
	{
		text_mu.push_back( _condi );
		text_el.push_back( _condi );
	}
	
	string file_name_mu = file_name_ps + "Muon_Channel";
	string file_name_el = file_name_ps + "Electron_Channel";

	Plot_MCData( h_Data_mu, h_MCs_mu, file_name_mu, hist_name, text_mu );

	Plot_MCData( h_Data_el, h_MCs_el, file_name_el, hist_name, text_el );

}
