/***************************************************************************
 *
 *	FileName : Hists.cc
 *	Description : Make a class to wrap histagrams we want to use 
 *	Date : 2019 Nov 8 ~
 *	Author : Chen-Yu Chuang
 *
 * ************************************************************************/
#include "TopCPViolation/selected/interface/Hists.h"

void Hists::GetObjs( string& file_name, string& option )
{
	TFile* f = new TFile( (char*)file_name.c_str() );
	
	if( option.find("NC") != string::npos )
	{
		h_TT_mu = new TH1F();		h_TT_el = new TH1F();
		h_DY_mu = new TH1F();		h_DY_el = new TH1F();
		h_WJets_mu = new TH1F();	h_WJets_el = new TH1F();
		h_VV_mu = new TH1F();		h_VV_el = new TH1F();
		h_ST_mu = new TH1F();		h_ST_el = new TH1F();
		h_QCD_mu = new TH1F();		h_QCD_el = new TH1F();
		h_Data_mu = new TH1F();		h_Data_el = new TH1F();
		h_l_TT_mu = new TH1F();		h_l_TT_el = new TH1F();
		h_l_DY_mu = new TH1F();		h_l_DY_el = new TH1F();
		h_l_WJets_mu = new TH1F();	h_l_WJets_el = new TH1F();
		h_l_VV_mu = new TH1F();		h_l_VV_el = new TH1F();
		h_l_ST_mu = new TH1F();		h_l_ST_el = new TH1F();
		h_l_QCD_mu = new TH1F();	h_l_QCD_el = new TH1F();
		h_l_Data_mu = new TH1F();	h_l_Data_el = new TH1F();

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

		f->GetObject("h_l_TT_mu",h_l_TT_mu);
		f->GetObject("h_l_DY_mu",h_l_DY_mu);
		f->GetObject("h_l_WJets_mu",h_l_WJets_mu);
		f->GetObject("h_l_VV_mu",h_l_VV_mu);
		f->GetObject("h_l_ST_mu",h_l_ST_mu);
		f->GetObject("h_l_QCD_mu",h_l_QCD_mu);
		f->GetObject("h_l_Data_mu",h_l_Data_mu);

		f->GetObject("h_l_TT_el",h_l_TT_el);
		f->GetObject("h_l_DY_el",h_l_DY_el);
		f->GetObject("h_l_WJets_el",h_l_WJets_el);
		f->GetObject("h_l_VV_el",h_l_VV_el);
		f->GetObject("h_l_ST_el",h_l_ST_el);
		f->GetObject("h_l_QCD_el",h_l_QCD_el);
		f->GetObject("h_l_Data_el",h_l_Data_el);

		InitNoCutVector();
	
	}
	
	if( option.find("1C") != string::npos )
	{
		
		h_TT_mu_c = new TH1F();		h_TT_el_c = new TH1F();
		h_DY_mu_c = new TH1F();		h_DY_el_c = new TH1F();
		h_WJets_mu_c = new TH1F();	h_WJets_el_c = new TH1F();
		h_VV_mu_c = new TH1F();		h_VV_el_c = new TH1F();
		h_ST_mu_c = new TH1F();		h_ST_el_c = new TH1F();
		h_QCD_mu_c = new TH1F();		h_QCD_el_c = new TH1F();
		h_Data_mu_c = new TH1F();		h_Data_el_c = new TH1F();
		h_l_TT_mu_c = new TH1F();		h_l_TT_el_c = new TH1F();
		h_l_DY_mu_c = new TH1F();		h_l_DY_el_c = new TH1F();
		h_l_WJets_mu_c = new TH1F();	h_l_WJets_el_c = new TH1F();
		h_l_VV_mu_c = new TH1F();		h_l_VV_el_c = new TH1F();
		h_l_ST_mu_c = new TH1F();		h_l_ST_el_c = new TH1F();
		h_l_QCD_mu_c = new TH1F();	h_l_QCD_el_c = new TH1F();
		h_l_Data_mu_c = new TH1F();	h_l_Data_el_c = new TH1F();

		f->GetObject("h_TT_mu_c",h_TT_mu_c);
		f->GetObject("h_DY_mu_c",h_DY_mu_c);
		f->GetObject("h_WJets_mu_c",h_WJets_mu_c);
		f->GetObject("h_VV_mu_c",h_VV_mu_c);
		f->GetObject("h_ST_mu_c",h_ST_mu_c);
		f->GetObject("h_QCD_mu_c",h_QCD_mu_c);
		f->GetObject("h_Data_mu_c",h_Data_mu_c);

		f->GetObject("h_TT_el_c",h_TT_el_c);
		f->GetObject("h_DY_el_c",h_DY_el_c);
		f->GetObject("h_WJets_el_c",h_WJets_el_c);
		f->GetObject("h_VV_el_c",h_VV_el_c);
		f->GetObject("h_ST_el_c",h_ST_el_c);
		f->GetObject("h_QCD_el_c",h_QCD_el_c);
		f->GetObject("h_Data_el_c",h_Data_el_c);

		f->GetObject("h_l_TT_mu_c",h_l_TT_mu_c);
		f->GetObject("h_l_DY_mu_c",h_l_DY_mu_c);
		f->GetObject("h_l_WJets_mu_c",h_l_WJets_mu_c);
		f->GetObject("h_l_VV_mu_c",h_l_VV_mu_c);
		f->GetObject("h_l_ST_mu_c",h_l_ST_mu_c);
		f->GetObject("h_l_QCD_mu_c",h_l_QCD_mu_c);
		f->GetObject("h_l_Data_mu_c",h_l_Data_mu_c);

		f->GetObject("h_l_TT_el_c",h_l_TT_el_c);
		f->GetObject("h_l_DY_el_c",h_l_DY_el_c);
		f->GetObject("h_l_WJets_el_c",h_l_WJets_el_c);
		f->GetObject("h_l_VV_el_c",h_l_VV_el_c);
		f->GetObject("h_l_ST_el_c",h_l_ST_el_c);
		f->GetObject("h_l_QCD_el_c",h_l_QCD_el_c);
		f->GetObject("h_l_Data_el_c",h_l_Data_el_c);
		
		InitOneCutVector();
	}
	
	if( option.find("2C") != string::npos )
	{
		h_TT_mu_cc = new TH1F();		h_TT_el_cc = new TH1F();
		h_DY_mu_cc = new TH1F();		h_DY_el_cc = new TH1F();
		h_WJets_mu_cc = new TH1F();	h_WJets_el_cc = new TH1F();
		h_VV_mu_cc = new TH1F();		h_VV_el_cc = new TH1F();
		h_ST_mu_cc = new TH1F();		h_ST_el_cc = new TH1F();
		h_QCD_mu_cc = new TH1F();		h_QCD_el_cc = new TH1F();
		h_Data_mu_cc = new TH1F();		h_Data_el_cc = new TH1F();
		h_l_TT_mu_cc = new TH1F();		h_l_TT_el_cc = new TH1F();
		h_l_DY_mu_cc = new TH1F();		h_l_DY_el_cc = new TH1F();
		h_l_WJets_mu_cc = new TH1F();	h_l_WJets_el_cc = new TH1F();
		h_l_VV_mu_cc = new TH1F();		h_l_VV_el_cc = new TH1F();
		h_l_ST_mu_cc = new TH1F();		h_l_ST_el_cc = new TH1F();
		h_l_QCD_mu_cc = new TH1F();	h_l_QCD_el_cc = new TH1F();
		h_l_Data_mu_cc = new TH1F();	h_l_Data_el_cc = new TH1F();

		f->GetObject("h_TT_mu_cc",h_TT_mu_cc);
		f->GetObject("h_DY_mu_cc",h_DY_mu_cc);
		f->GetObject("h_WJets_mu_cc",h_WJets_mu_cc);
		f->GetObject("h_VV_mu_cc",h_VV_mu_cc);
		f->GetObject("h_ST_mu_cc",h_ST_mu_cc);
		f->GetObject("h_QCD_mu_cc",h_QCD_mu_cc);
		f->GetObject("h_Data_mu_cc",h_Data_mu_cc);

		f->GetObject("h_TT_el_cc",h_TT_el_cc);
		f->GetObject("h_DY_el_cc",h_DY_el_cc);
		f->GetObject("h_WJets_el_cc",h_WJets_el_cc);
		f->GetObject("h_VV_el_cc",h_VV_el_cc);
		f->GetObject("h_ST_el_cc",h_ST_el_cc);
		f->GetObject("h_QCD_el_cc",h_QCD_el_cc);
		f->GetObject("h_Data_el_cc",h_Data_el_cc);

		f->GetObject("h_l_TT_mu_cc",h_l_TT_mu_cc);
		f->GetObject("h_l_DY_mu_cc",h_l_DY_mu_cc);
		f->GetObject("h_l_WJets_mu_cc",h_l_WJets_mu_cc);
		f->GetObject("h_l_VV_mu_cc",h_l_VV_mu_cc);
		f->GetObject("h_l_ST_mu_cc",h_l_ST_mu_cc);
		f->GetObject("h_l_QCD_mu_cc",h_l_QCD_mu_cc);
		f->GetObject("h_l_Data_mu_cc",h_l_Data_mu_cc);

		f->GetObject("h_l_TT_el_cc",h_l_TT_el_cc);
		f->GetObject("h_l_DY_el_cc",h_l_DY_el_cc);
		f->GetObject("h_l_WJets_el_cc",h_l_WJets_el_cc);
		f->GetObject("h_l_VV_el_cc",h_l_VV_el_cc);
		f->GetObject("h_l_ST_el_cc",h_l_ST_el_cc);
		f->GetObject("h_l_QCD_el_cc",h_l_QCD_el_cc);
		f->GetObject("h_l_Data_el_cc",h_l_Data_el_cc);

		InitTwoCutVector();
	}
	if( (option.find("NC") == string::npos) && (option.find("1C") == string::npos) && (option.find("2C") == string::npos) )
	{
		cerr << "Illegal option of Hists::GetObjs(\"file_name\",\"option\") ," << endl \
			 << "option should include NC/1C/2C " << endl;
	}
}

void Hists::InitNoCutVector()
{
	h_mu.push_back(h_TT_mu);		h_mu.push_back(h_DY_mu);
	h_mu.push_back(h_WJets_mu);		h_mu.push_back(h_VV_mu);
	h_mu.push_back(h_ST_mu);		h_mu.push_back(h_QCD_mu);
	h_mu.push_back(h_Data_mu);

	h_el.push_back(h_TT_el);		h_el.push_back(h_DY_el);
	h_el.push_back(h_WJets_el);		h_el.push_back(h_VV_el);
	h_el.push_back(h_ST_el);		h_el.push_back(h_QCD_el);
	h_el.push_back(h_Data_el);
	
	h_l_mu.push_back(h_l_TT_mu);		h_l_mu.push_back(h_l_DY_mu);
	h_l_mu.push_back(h_l_WJets_mu);		h_l_mu.push_back(h_l_VV_mu);
	h_l_mu.push_back(h_l_ST_mu);		h_l_mu.push_back(h_l_QCD_mu);
	h_l_mu.push_back(h_l_Data_mu);
	
	h_l_el.push_back(h_l_TT_el);		h_l_el.push_back(h_l_DY_el);
	h_l_el.push_back(h_l_WJets_el);		h_l_el.push_back(h_l_VV_el);
	h_l_el.push_back(h_l_ST_el);		h_l_el.push_back(h_l_QCD_el);
	h_l_el.push_back(h_l_Data_el);
}

void Hists::InitOneCutVector()
{
	h_mu_c.push_back(h_TT_mu_c);		h_mu_c.push_back(h_DY_mu_c);
	h_mu_c.push_back(h_WJets_mu_c);		h_mu_c.push_back(h_VV_mu_c);
	h_mu_c.push_back(h_ST_mu_c);		h_mu_c.push_back(h_QCD_mu_c);
	h_mu_c.push_back(h_Data_mu_c);

	h_el_c.push_back(h_TT_el_c);		h_el_c.push_back(h_DY_el_c);
	h_el_c.push_back(h_WJets_el_c);		h_el_c.push_back(h_VV_el_c);
	h_el_c.push_back(h_ST_el_c);		h_el_c.push_back(h_QCD_el_c);
	h_el_c.push_back(h_Data_el_c);
	
	h_l_mu_c.push_back(h_l_TT_mu_c);		h_l_mu_c.push_back(h_l_DY_mu_c);
	h_l_mu_c.push_back(h_l_WJets_mu_c);		h_l_mu_c.push_back(h_l_VV_mu_c);
	h_l_mu_c.push_back(h_l_ST_mu_c);		h_l_mu_c.push_back(h_l_QCD_mu_c);
	h_l_mu_c.push_back(h_l_Data_mu_c);
	
	h_l_el_c.push_back(h_l_TT_el_c);		h_l_el_c.push_back(h_l_DY_el_c);
	h_l_el_c.push_back(h_l_WJets_el_c);		h_l_el_c.push_back(h_l_VV_el_c);
	h_l_el_c.push_back(h_l_ST_el_c);		h_l_el_c.push_back(h_l_QCD_el_c);
	h_l_el_c.push_back(h_l_Data_el_c);
}


void Hists::InitTwoCutVector()
{
	h_mu_cc.push_back(h_TT_mu_cc);		h_mu_cc.push_back(h_DY_mu_cc);
	h_mu_cc.push_back(h_WJets_mu_cc);		h_mu_cc.push_back(h_VV_mu_cc);
	h_mu_cc.push_back(h_ST_mu_cc);		h_mu_cc.push_back(h_QCD_mu_cc);
	h_mu_cc.push_back(h_Data_mu_cc);

	h_el_cc.push_back(h_TT_el_cc);		h_el_cc.push_back(h_DY_el_cc);
	h_el_cc.push_back(h_WJets_el_cc);		h_el_cc.push_back(h_VV_el_cc);
	h_el_cc.push_back(h_ST_el_cc);		h_el_cc.push_back(h_QCD_el_cc);
	h_el_cc.push_back(h_Data_el_cc);
	
	h_l_mu_cc.push_back(h_l_TT_mu_cc);		h_l_mu_cc.push_back(h_l_DY_mu_cc);
	h_l_mu_cc.push_back(h_l_WJets_mu_cc);		h_l_mu_cc.push_back(h_l_VV_mu_cc);
	h_l_mu_cc.push_back(h_l_ST_mu_cc);		h_l_mu_cc.push_back(h_l_QCD_mu_cc);
	h_l_mu_cc.push_back(h_l_Data_mu_cc);
	
	h_l_el_cc.push_back(h_l_TT_el_cc);		h_l_el_cc.push_back(h_l_DY_el_cc);
	h_l_el_cc.push_back(h_l_WJets_el_cc);		h_l_el_cc.push_back(h_l_VV_el_cc);
	h_l_el_cc.push_back(h_l_ST_el_cc);		h_l_el_cc.push_back(h_l_QCD_el_cc);
	h_l_el_cc.push_back(h_l_Data_el_cc);
}

void Hists::TH2ModeON( const int& algo_bins_No, const double& m, const double& M )
{
	double algo_v_min = m;
	double algo_v_max = M;
	h_mvamax_mass_mu = new TH2D("h_mvamax_mass_mu","",bins_No,hist_min,hist_max,algo_bins_No,algo_v_min,algo_v_max);
	h_mvamax_mass_el = new TH2D("h_mvamax_mass_el","",bins_No,hist_min,hist_max,algo_bins_No,algo_v_min,algo_v_max);
	h_mvamax_mass[ "mu" ] = h_mvamax_mass_mu;			
	h_mvamax_mass[ "el" ] = h_mvamax_mass_el;
}
void Hists::NoCutModeON()
{
	h_TT_mu = new TH1F("h_TT_mu","",bins_No,hist_min,hist_max);
	h_TT_el = new TH1F("h_TT_el","",bins_No,hist_min,hist_max);
	
	h_DY_mu = new TH1F("h_DY_mu","",bins_No,hist_min,hist_max);
	h_DY_el = new TH1F("h_DY_el","",bins_No,hist_min,hist_max);

	h_WJets_mu = new TH1F("h_WJets_mu","",bins_No,hist_min,hist_max);
	h_WJets_el = new TH1F("h_WJets_el","",bins_No,hist_min,hist_max);
	
	h_VV_mu = new TH1F("h_VV_mu","",bins_No,hist_min,hist_max);
	h_VV_el = new TH1F("h_VV_el","",bins_No,hist_min,hist_max);

	h_ST_mu = new TH1F("h_ST_mu","",bins_No,hist_min,hist_max);
	h_ST_el = new TH1F("h_ST_el","",bins_No,hist_min,hist_max);
	
	h_QCD_mu = new TH1F("h_QCD_mu","",bins_No,hist_min,hist_max);
	h_QCD_el = new TH1F("h_QCD_el","",bins_No,hist_min,hist_max);
	
	h_Data_mu = new TH1F("h_Data_mu"," ;Hadronic Top Mass(GeV);Events(No.)",bins_No,hist_min,hist_max);	
	h_Data_el = new TH1F("h_Data_el"," ;Hadronic Top Mass(GeV);Events(No.)",bins_No,hist_min,hist_max);

	h_l_TT_mu = new TH1F("h_l_TT_mu","",bins_No,hist_min,hist_max);
	h_l_TT_el = new TH1F("h_l_TT_el","",bins_No,hist_min,hist_max);
	
	h_l_DY_mu = new TH1F("h_l_DY_mu","",bins_No,hist_min,hist_max);
	h_l_DY_el = new TH1F("h_l_DY_el","",bins_No,hist_min,hist_max);

	h_l_WJets_mu = new TH1F("h_l_WJets_mu","",bins_No,hist_min,hist_max);
	h_l_WJets_el = new TH1F("h_l_WJets_el","",bins_No,hist_min,hist_max);
	
	h_l_VV_mu = new TH1F("h_l_VV_mu","",bins_No,hist_min,hist_max);
	h_l_VV_el = new TH1F("h_l_VV_el","",bins_No,hist_min,hist_max);

	h_l_ST_mu = new TH1F("h_l_ST_mu","",bins_No,hist_min,hist_max);
	h_l_ST_el = new TH1F("h_l_ST_el","",bins_No,hist_min,hist_max);
	
	h_l_QCD_mu = new TH1F("h_l_QCD_mu","",bins_No,hist_min,hist_max);
	h_l_QCD_el = new TH1F("h_l_QCD_el","",bins_No,hist_min,hist_max);
	
	h_l_Data_mu = new TH1F("h_l_Data_mu"," ;Leptonic Top Mass(GeV);Events(No.)",bins_No,hist_min,hist_max);
	h_l_Data_el = new TH1F("h_l_Data_el"," ;Leptonic Top Mass(GeV);Events(No.)",bins_No,hist_min,hist_max);

	InitNoCutVector();
}

void Hists::OneCutModeON()
{
	h_TT_mu_c = new TH1F("h_TT_mu_c","",bins_No,hist_min,hist_max);
	h_TT_el_c = new TH1F("h_TT_el_c","",bins_No,hist_min,hist_max);
	
	h_DY_mu_c = new TH1F("h_DY_mu_c","",bins_No,hist_min,hist_max);
	h_DY_el_c = new TH1F("h_DY_el_c","",bins_No,hist_min,hist_max);

	h_WJets_mu_c = new TH1F("h_WJets_mu_c","",bins_No,hist_min,hist_max);
	h_WJets_el_c = new TH1F("h_WJets_el_c","",bins_No,hist_min,hist_max);
	
	h_VV_mu_c = new TH1F("h_VV_mu_c","",bins_No,hist_min,hist_max);
	h_VV_el_c = new TH1F("h_VV_el_c","",bins_No,hist_min,hist_max);

	h_ST_mu_c = new TH1F("h_ST_mu_c","",bins_No,hist_min,hist_max);
	h_ST_el_c = new TH1F("h_ST_el_c","",bins_No,hist_min,hist_max);
	
	h_QCD_mu_c = new TH1F("h_QCD_mu_c","",bins_No,hist_min,hist_max);
	h_QCD_el_c = new TH1F("h_QCD_el_c","",bins_No,hist_min,hist_max);
	
	h_Data_mu_c = new TH1F("h_Data_mu_c"," ;Hadronic Top Mass(GeV);Events(No.)",bins_No,hist_min,hist_max);	
	h_Data_el_c = new TH1F("h_Data_el_c"," ;Hadronic Top Mass(GeV);Events(No.)",bins_No,hist_min,hist_max);

	h_l_TT_mu_c = new TH1F("h_l_TT_mu_c","",bins_No,hist_min,hist_max);
	h_l_TT_el_c = new TH1F("h_l_TT_el_c","",bins_No,hist_min,hist_max);
	
	h_l_DY_mu_c = new TH1F("h_l_DY_mu_c","",bins_No,hist_min,hist_max);
	h_l_DY_el_c = new TH1F("h_l_DY_el_c","",bins_No,hist_min,hist_max);

	h_l_WJets_mu_c = new TH1F("h_l_WJets_mu_c","",bins_No,hist_min,hist_max);
	h_l_WJets_el_c = new TH1F("h_l_WJets_el_c","",bins_No,hist_min,hist_max);
	
	h_l_VV_mu_c = new TH1F("h_l_VV_mu_c","",bins_No,hist_min,hist_max);
	h_l_VV_el_c = new TH1F("h_l_VV_el_c","",bins_No,hist_min,hist_max);

	h_l_ST_mu_c = new TH1F("h_l_ST_mu_c","",bins_No,hist_min,hist_max);
	h_l_ST_el_c = new TH1F("h_l_ST_el_c","",bins_No,hist_min,hist_max);
	
	h_l_QCD_mu_c = new TH1F("h_l_QCD_mu_c","",bins_No,hist_min,hist_max);
	h_l_QCD_el_c = new TH1F("h_l_QCD_el_c","",bins_No,hist_min,hist_max);
	
	h_l_Data_mu_c = new TH1F("h_l_Data_mu_c"," ;Leptonic Top Mass(GeV);Events(No.)",bins_No,hist_min,hist_max);
	h_l_Data_el_c = new TH1F("h_l_Data_el_c"," ;Leptonic Top Mass(GeV);Events(No.)",bins_No,hist_min,hist_max);
	
	InitOneCutVector();
}

void Hists::TwoCutModeON()
{
	h_TT_mu_cc = new TH1F("h_TT_mu_cc","",bins_No,hist_min,hist_max);
	h_TT_el_cc = new TH1F("h_TT_el_cc","",bins_No,hist_min,hist_max);
	
	h_DY_mu_cc = new TH1F("h_DY_mu_cc","",bins_No,hist_min,hist_max);
	h_DY_el_cc = new TH1F("h_DY_el_cc","",bins_No,hist_min,hist_max);

	h_WJets_mu_cc = new TH1F("h_WJets_mu_cc","",bins_No,hist_min,hist_max);
	h_WJets_el_cc = new TH1F("h_WJets_el_cc","",bins_No,hist_min,hist_max);
	
	h_VV_mu_cc = new TH1F("h_VV_mu_cc","",bins_No,hist_min,hist_max);
	h_VV_el_cc = new TH1F("h_VV_el_cc","",bins_No,hist_min,hist_max);

	h_ST_mu_cc = new TH1F("h_ST_mu_cc","",bins_No,hist_min,hist_max);
	h_ST_el_cc = new TH1F("h_ST_el_cc","",bins_No,hist_min,hist_max);
	
	h_QCD_mu_cc = new TH1F("h_QCD_mu_cc","",bins_No,hist_min,hist_max);
	h_QCD_el_cc = new TH1F("h_QCD_el_cc","",bins_No,hist_min,hist_max);
	
	h_Data_mu_cc = new TH1F("h_Data_mu_cc"," ;Hadronic Top Mass(GeV);Events(No.)",bins_No,hist_min,hist_max);	
	h_Data_el_cc = new TH1F("h_Data_el_cc"," ;Hadronic Top Mass(GeV);Events(No.)",bins_No,hist_min,hist_max);

	h_l_TT_mu_cc = new TH1F("h_l_TT_mu_cc","",bins_No,hist_min,hist_max);
	h_l_TT_el_cc = new TH1F("h_l_TT_el_cc","",bins_No,hist_min,hist_max);
	
	h_l_DY_mu_cc = new TH1F("h_l_DY_mu_cc","",bins_No,hist_min,hist_max);
	h_l_DY_el_cc = new TH1F("h_l_DY_el_cc","",bins_No,hist_min,hist_max);

	h_l_WJets_mu_cc = new TH1F("h_l_WJets_mu_cc","",bins_No,hist_min,hist_max);
	h_l_WJets_el_cc = new TH1F("h_l_WJets_el_cc","",bins_No,hist_min,hist_max);
	
	h_l_VV_mu_cc = new TH1F("h_l_VV_mu_cc","",bins_No,hist_min,hist_max);
	h_l_VV_el_cc = new TH1F("h_l_VV_el_cc","",bins_No,hist_min,hist_max);

	h_l_ST_mu_cc = new TH1F("h_l_ST_mu_cc","",bins_No,hist_min,hist_max);
	h_l_ST_el_cc = new TH1F("h_l_ST_el_cc","",bins_No,hist_min,hist_max);
	
	h_l_QCD_mu_cc = new TH1F("h_l_QCD_mu_cc","",bins_No,hist_min,hist_max);
	h_l_QCD_el_cc = new TH1F("h_l_QCD_el_cc","",bins_No,hist_min,hist_max);
	
	h_l_Data_mu_cc = new TH1F("h_l_Data_mu_cc"," ;Leptonic Top Mass(GeV);Events(No.)",bins_No,hist_min,hist_max);
	h_l_Data_el_cc = new TH1F("h_l_Data_el_cc"," ;Leptonic Top Mass(GeV);Events(No.)",bins_No,hist_min,hist_max);

	InitTwoCutVector();
}

void Hists::WriteIn( const string& option = "NC" )
{
	if( option.find("NC") != string::npos )
	{
		h_TT_mu->Write();		
		h_TT_el->Write();	
		h_DY_mu->Write();		
		h_DY_el->Write();
		h_WJets_mu->Write();	
		h_WJets_el->Write();
		h_VV_mu->Write();		
		h_VV_el->Write();
		h_ST_mu->Write();		
		h_ST_el->Write();
		h_QCD_mu->Write();		
		h_QCD_el->Write();
		h_Data_mu->Write();	
		h_Data_el->Write();

		h_l_TT_mu->Write();	
		h_l_TT_el->Write();
		h_l_DY_mu->Write();	
		h_l_DY_el->Write();
		h_l_WJets_mu->Write();	
		h_l_WJets_el->Write();
		h_l_VV_mu->Write();	
		h_l_VV_el->Write();
		h_l_ST_mu->Write();	
		h_l_ST_el->Write();
		h_l_QCD_mu->Write();	
		h_l_QCD_el->Write();
		h_l_Data_mu->Write();	
		h_l_Data_el->Write();
	}	
	
	if( option.find("1C") != string::npos )
	{
		h_TT_mu_c->Write();		
		h_TT_el_c->Write();	
		h_DY_mu_c->Write();		
		h_DY_el_c->Write();
		h_WJets_mu_c->Write();	
		h_WJets_el_c->Write();
		h_VV_mu_c->Write();		
		h_VV_el_c->Write();
		h_ST_mu_c->Write();		
		h_ST_el_c->Write();
		h_QCD_mu_c->Write();		
		h_QCD_el_c->Write();
		h_Data_mu_c->Write();	
		h_Data_el_c->Write();

		h_l_TT_mu_c->Write();	
		h_l_TT_el_c->Write();
		h_l_DY_mu_c->Write();	
		h_l_DY_el_c->Write();
		h_l_WJets_mu_c->Write();	
		h_l_WJets_el_c->Write();
		h_l_VV_mu_c->Write();	
		h_l_VV_el_c->Write();
		h_l_ST_mu_c->Write();	
		h_l_ST_el_c->Write();
		h_l_QCD_mu_c->Write();	
		h_l_QCD_el_c->Write();
		h_l_Data_mu_c->Write();	
		h_l_Data_el_c->Write();
	}

	if( option.find("2C") != string::npos )
	{
		h_TT_mu_cc->Write();		
		h_TT_el_cc->Write();	
		h_DY_mu_cc->Write();		
		h_DY_el_cc->Write();
		h_WJets_mu_cc->Write();	
		h_WJets_el_cc->Write();
		h_VV_mu_cc->Write();		
		h_VV_el_cc->Write();
		h_ST_mu_cc->Write();		
		h_ST_el_cc->Write();
		h_QCD_mu_cc->Write();		
		h_QCD_el_cc->Write();
		h_Data_mu_cc->Write();	
		h_Data_el_cc->Write();

		h_l_TT_mu_cc->Write();	
		h_l_TT_el_cc->Write();
		h_l_DY_mu_cc->Write();	
		h_l_DY_el_cc->Write();
		h_l_WJets_mu_cc->Write();	
		h_l_WJets_el_cc->Write();
		h_l_VV_mu_cc->Write();	
		h_l_VV_el_cc->Write();
		h_l_ST_mu_cc->Write();	
		h_l_ST_el_cc->Write();
		h_l_QCD_mu_cc->Write();	
		h_l_QCD_el_cc->Write();
		h_l_Data_mu_cc->Write();	
		h_l_Data_el_cc->Write();
	}
	
	if( (option.find("NC") == string::npos) && (option.find("1C") == string::npos) && (option.find("2C") == string::npos) )
	{
		cerr << "Illegal option of Hists::WriteIn(\"file_name\",\"option\") ," << endl \
			 << "option should include NC/1C/2C " << endl;
	}

}
