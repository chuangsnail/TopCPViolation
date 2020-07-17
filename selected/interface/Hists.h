#ifndef HISTS_H
#define HISTS_H

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
#include "TNtupleD.h"

using namespace std;


//Tool for Hist

double t_IntegralTH1( const TH1* h ) { return h->Integral(1,h->GetXaxis()->GetNbins()+1); }

void NormalizeTH1( TH1* h ) { h->Scale( (double)1./t_IntegralTH1( h ) ); }

//maybe we can use a mother class and inheritance, and the bins_No,hist_min/max are the mother member, and  Init(),WriteIn() are virtual functions! 

class Hists
{
public:
	
	Hists()
	{
		bins_No = 50;
		hist_min = 0.;
		hist_max = 500.;
	}
	~Hists() {}


	int bins_No;
	double hist_min;
	double hist_max;

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

	TH2D* h_mvamax_hadmass_mu;		//with hadmass
	TH2D* h_mvamax_hadmass_el;
	TH2D* h_mvamax_lepmass_mu;		//with lepmass
	TH2D* h_mvamax_lepmass_el;
	map<string, TH2D*> h_mvamax_hadmass, h_mvamax_lepmass;
	//double algo_v_min;
	//double algo_v_max;

	TNtupleD* mvav_mass_mu;
	TNtupleD* mvav_mass_el;
	map<string, TNtupleD*> mvav_mass;

	//Set
	void SetBinInfo( const int& n, const double& m, const double& M  ){
		bins_No = n;	
		hist_min = m;	
		hist_max = M;
	}

	//for fill and use new things
	void NoCutModeON();
	void OneCutModeON();
	void TwoCutModeON();
	void InitNoCutVector();
	void InitOneCutVector();
	void InitTwoCutVector();

	void TH2ModeON( const int& algo_bins_No, const double&, const double&);

	void NtupleModeON(){
		mvav_mass_mu = new TNtupleD( "mvav_mass_mu", "store event by event", "max_mva_value:hadt_mass:lept_mass:weight:dataset" );
		mvav_mass_el = new TNtupleD( "mvav_mass_el", "store event by event", "max_mva_value:hadt_mass:lept_mass:weight:dataset" );
		mvav_mass[ "mu" ] = mvav_mass_mu;
		mvav_mass[ "el" ] = mvav_mass_el;
	}

	void WriteIn( const string& option );
	
	//for plots or some applicatin
	void GetObjs( string& file_name, string& option );

};

class Hists_bb
{
public:
	
	Hists_bb()
	{
		bins_No1 = 50;
		hist_min1 = 0.;
		hist_max1 = 1.;
		
		bins_No2 = 100;
		hist_min2 = 0.;
		hist_max2 = 500.;

		no_match_number = 0;
	}
	~Hists_bb() {}
	
	map<string,TH1F*> h_correct, h_charge_mis, h_mistag, h_lt_correct, h_lt_charge_mis, h_lt_mistag;
	map<string, TH2D*> h_mvamax_mass;
	
	TH2D* h_mvamax_mass_mu;
	TH2D* h_mvamax_mass_el;
	TH2D* h_mvamax_mass_t;

	TH1F* h_correct_t;
	TH1F* h_mistag_t;
	TH1F* h_charge_mis_t;

	TH1F* h_correct_mu;
	TH1F* h_mistag_mu;
	TH1F* h_charge_mis_mu;

	TH1F* h_correct_el;
	TH1F* h_mistag_el;
	TH1F* h_charge_mis_el;

	TH1F* h_lt_correct_t;
	TH1F* h_lt_mistag_t;
	TH1F* h_lt_charge_mis_t;

	TH1F* h_lt_correct_mu;
	TH1F* h_lt_mistag_mu;
	TH1F* h_lt_charge_mis_mu;

	TH1F* h_lt_correct_el;
	TH1F* h_lt_mistag_el;
	TH1F* h_lt_charge_mis_el;

	int no_match_number;

	//for algo value
	int bins_No1;
	double hist_min1;
	double hist_max1;
	//for Mlb
	int bins_No2;
	double hist_min2;
	double hist_max2;

	void SetAlgoBin( const int& n, const double& m, const double& M ){
		bins_No1 = n;	
		hist_min1 = m;	
		hist_max1 = M;
	}
	void Init();
	void WriteIn();
};

class Hists_cor
{
public:

	Hists_cor()
	{
		bins_No1 = 100;	
		hist_min1 = 0.;	
		hist_max1 = 1.;
	}

	~Hists_cor(){}

	//for store	mva
	map<string, TH1F*> h_max_mva, h_max_mva_cor, h_max_mva_incor;
	
	TH1F* h_max_mva_mu;
	TH1F* h_max_mva_cor_mu;
	TH1F* h_max_mva_incor_mu;
		
	TH1F* h_max_mva_el;
	TH1F* h_max_mva_cor_el;
	TH1F* h_max_mva_incor_el;

	TH1F* h_max_mva_t;
	TH1F* h_max_mva_cor_t;
	TH1F* h_max_mva_incor_t;

	int bins_No1;
	double hist_min1;
	double hist_max1;
	
	//for store to calculate Poisson error
	//chosen -> 'all': pass full-sel / 'part': pass full-sel and pass gen-matching		
	//cor -> pass 'all'/'pass' and it's correct permutation(combination)
	
	map<string, TH1F*> h_chosen, h_cor;
	
	TH1F* h_chosen_mu;
	TH1F* h_cor_mu;
		
	TH1F* h_chosen_el;
	TH1F* h_cor_el;
	
	TH1F* h_chosen_t;
	TH1F* h_cor_t;
	
	void SetAlgoBin( const int& n, const double& m, const double& M ){
		bins_No1 = n;	
		hist_min1 = m;	
		hist_max1 = M;
	}
	void Init();
	void WriteIn();

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
