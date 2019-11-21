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
#include <string.h>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "TFile.h"

using namespace std;

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

	//for fill and use new things
	void NoCutModeON();
	void OneCutModeON();
	void TwoCutModeON();
	void InitNoCutVector();
	void InitOneCutVector();
	void InitTwoCutVector();

	void TH2ModeON( const int& algo_bins_No, const double&, const double&);

	void WriteIn( const string& option );
	
	//for plots or some applicatin
	void GetObjs( string& file_name, string& option );

};

class Hists_bb
{
public:
};

/*
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
*/

#endif
