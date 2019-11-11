#ifndef HISTS_H
#define HISTS_H

#define bins_No 50
#define hist_min 0
#define hist_max 500

/***************************************************************************
 *
 *	FileName : Hists.h
 *	Description : Make a class to wrap histagrams we want to use 
 *	Date : 2019 Nov 8 ~
 *	Author : Chen-Yu Chuang
 *
 * ************************************************************************/

#include <string>
#include <iostream>
#include <cassert>

#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "TFile.h"

class Hists
{
public:

	vector<TH1F*> h_mu, h_el, h_l_mu, h_l_el;

	TH1F* h_TT_mu;		TH1F* h_TT_el;	
	TH1F* h_DY_mu;		TH1F* h_DY_el;
	TH1F* h_WJets_mu;	TH1F* h_WJets_el;
	TH1F* h_VV_mu;		TH1F* h_VV_el;
	TH1F* h_ST_mu;		TH1F* h_ST_el;
	TH1F* h_QCD_mu;		TH1F* h_QCD_el;
	TH1F* h_Data_mu;	TH1F* h_Data_el;

	TH1F* h_l_TT_mu;	TH1F* h_l_TT_el;
	TH1F* h_l_DY_mu;	TH1F* h_l_DY_el;
	TH1F* h_l_WJets_mu;	TH1F* h_l_WJets_el;
	TH1F* h_l_VV_mu;	TH1F* h_l_VV_el;
	TH1F* h_l_ST_mu;	TH1F* h_l_ST_el;
	TH1F* h_l_QCD_mu;	TH1F* h_l_QCD_el;
	TH1F* h_l_Data_mu;	TH1F* h_l_Data_el;

	vector<TH1F*> h_mu_c, h_el_c, h_l_mu_c, h_l_el_c;

	TH1F* h_TT_mu_c;		TH1F* h_TT_el_c;	
	TH1F* h_DY_mu_c;		TH1F* h_DY_el_c;
	TH1F* h_WJets_mu_c;		TH1F* h_WJets_el_c;
	TH1F* h_VV_mu_c;		TH1F* h_VV_el_c;
	TH1F* h_ST_mu_c;		TH1F* h_ST_el_c;
	TH1F* h_QCD_mu_c;		TH1F* h_QCD_el_c;
	TH1F* h_Data_mu_c;		TH1F* h_Data_el_c;

	TH1F* h_l_TT_mu_c;		TH1F* h_l_TT_el_c;
	TH1F* h_l_DY_mu_c;		TH1F* h_l_DY_el_c;
	TH1F* h_l_WJets_mu_c;	TH1F* h_l_WJets_el_c;
	TH1F* h_l_VV_mu_c;		TH1F* h_l_VV_el_c;
	TH1F* h_l_ST_mu_c;		TH1F* h_l_ST_el_c;
	TH1F* h_l_QCD_mu_c;		TH1F* h_l_QCD_el_c;
	TH1F* h_l_Data_mu_c;	TH1F* h_l_Data_el_c;


	vector<TH1F*> h_mu_cc, h_el_cc, h_l_mu_cc, h_l_el_cc;

	TH1F* h_TT_mu_cc;		TH1F* h_TT_el_cc;	
	TH1F* h_DY_mu_cc;		TH1F* h_DY_el_cc;
	TH1F* h_WJets_mu_cc;	TH1F* h_WJets_el_cc;
	TH1F* h_VV_mu_cc;		TH1F* h_VV_el_cc;
	TH1F* h_ST_mu_cc;		TH1F* h_ST_el_cc;
	TH1F* h_QCD_mu_cc;		TH1F* h_QCD_el_cc;
	TH1F* h_Data_mu_cc;		TH1F* h_Data_el_cc;

	TH1F* h_l_TT_mu_cc;		TH1F* h_l_TT_el_cc;
	TH1F* h_l_DY_mu_cc;		TH1F* h_l_DY_el_cc;
	TH1F* h_l_WJets_mu_cc;	TH1F* h_l_WJets_el_cc;
	TH1F* h_l_VV_mu_cc;		TH1F* h_l_VV_el_cc;
	TH1F* h_l_ST_mu_cc;		TH1F* h_l_ST_el_cc;
	TH1F* h_l_QCD_mu_cc;	TH1F* h_l_QCD_el_cc;
	TH1F* h_l_Data_mu_cc;	TH1F* h_l_Data_el_cc;

	TH2D* h_mvamax_mass_mu;
	TH2D* h_mvamax_mass_el;
	map<string, TH2D*> h_mvamax_mass;
	//double algo_v_min;
	//double algo_v_max;

	void NoCutModeON();
	void OneCutModeON();
	void TwoCutModeON();
	void TH2ModeON( const double&, const double&);

	void WriteIn( const string& option );

};

class Hists_bb
{
public:
};

//use to see the discripency 
class Hists_test
{
public:
	int len;
	int now_idx;
	
	TH1F* h_MC;
	TH1F* h_Data;
	
	Hists_test( int& space ) 
	{
		len = space;
		h_MC = new TH1F[space];
		h_Data = new TH1F[space];
		now_idx = 0;
		len = 0;
	};

	~Hists_test()
	{
		if(len == 1){
			delete h_MC;
			delete h_Data;
		}
		else if(len > 1){
			delete [] h_MC;
			delete [] h_Data;
		}
	}

	void Set_idx(const int& i){
		assert(i <= len);
		now_idx = i;
	}

	void FillIn(const bool& is_data ,const double& value, const double& weight = 1. )
	{
		if(now_idx > len){
			len = now_idx;
		}

		if(is_data)
			h_Data[now_idx]->Fill(value, weight);
		else
			h_MC[now_idx]->Fill(value, weight);
		++now_idx;
	}

	void Store_Discrepancy(string& file_name)
	{
		TH1F* result = new TH1F[len];
		for(int i=0;i<len;++i)
		{
			result[i] = (TH1F*)( h_Data[i]->Clone() );
			string name = "discrepancy_" + i;
			result[i]->Divide( h_MC[i] );
			result[i]->SetName( name );
		}

		TFile* f = new TFile((char*)file_name.c_str(),"recreate");
		for(int i=0;i<len;++i){
			result[i]->Write();
		}
		f->Close();
		delete f;
	}

};


void Hists::TH2ModeON( const double& m, const double& M )
{
	double algo_v_min = m;
	double algo_v_max = M;
	h_mvamax_mass_mu = new TH2D("h_mvamax_mass_mu","",bins_No,hist_min,hist_max,bins_No,algo_v_min,algo_v_max);
	h_mvamax_mass_el = new TH2D("h_mvamax_mass_el","",bins_No,hist_min,hist_max,bins_No,algo_v_min,algo_v_max);
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

void Hists::WriteIn( const string& option = "NoCut" )
{
	if( option == "NoCut" )
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
	
	if( option == "OneCut" )
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

	if( option == "TwoCut" )
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
	
	if( !(option == "NoCut" || option == "OneCut" || option == "TwoCut") )
	{
		cerr << "-----FAULT----- Illegal option of Hists::WriteIn([(string)option])" << endl;
	}

}
#endif
