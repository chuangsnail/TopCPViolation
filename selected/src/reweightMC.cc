#include "TopCPViolation/selected/interface/reweightMC.h"
#include "TopCPViolation/selected/interface/BranchInfo.h"
#include "TopCPViolation/selected/interface/GetInfo.h"
#include <iostream>
#include <string>
#include <string.h>
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TObject.h"	//SetName() GetName() ...  

#include "CondFormats/BTauObjects/interface/BTagCalibration.h"
#include "CondTools/BTau/interface/BTagCalibrationReader.h"



double GetPileUpWeight(EvtInfo* event,const vector<double>& pileupweights,bool* pass_PU)
{
	*pass_PU = true;
	double PU_factor = 0.;
	int _nPU = 0;
	
	//double* PU_factor = new double;
	//int* _nPU = new int;

	_nPU = -1;
	for(int i=0;i<(int)event->nBX;i++)
	{
		if(event->BXPU[i] == 0)
		{
			_nPU = event->TrueIT[i];
			break;
		}
	}
	
	if( _nPU <= 74 && _nPU >= 0 )
	{
		PU_factor = (double)(pileupweights.at( _nPU ));
	}
	else
	{
		*pass_PU = false;
	}
	//delete _nPU;
	return PU_factor;
}

double GetLepSF_TH2F(LeptonInfo* lep,TH2F* h_SF,const int idx_sel_lep)
{
	SF_2D SF( h_SF );
	double eta = -1. , pt = -1.;
	eta = lep->Eta[ idx_sel_lep ];
	pt = lep->Pt[ idx_sel_lep ];
	
	return SF.GetTH2FBinContent( eta , pt );
}

double GetLepSF_TH2D(LeptonInfo* lep,TH2D* h_SF,const int idx_sel_lep)
{
	SF_2D SF( h_SF );
	double eta = -1. , pt = -1.;
	eta = lep->Eta[ idx_sel_lep ];
	pt = lep->Pt[ idx_sel_lep ];
	
	return SF.GetTH2DBinContent( eta , pt );
}

/*
double GetLepSF_TH2(LeptonInfo* lep,TH2* h_SF,const int idx_sel_lep)
{
	SF_2D_t SF( h_SF );
	double eta = -1. , pt = -1.;
	eta = lep->Eta[ idx_sel_lep ];
	pt = lep->Pt[ idx_sel_lep ];
	
	return SF.GetTH2BinContent( eta , pt );
}
*/


double Get_BtagWeight_Factor(std::map<BTagEntry::OperatingPoint, BTagCalibrationReader>& map,JetInfo& jet,BTagEntry::OperatingPoint op , const int& b_idx)
{
	double eta = jet.Eta[ b_idx ];
	double pt = jet.Pt[ b_idx ];
	double Btag_Factor = map.at(op).eval_auto_bounds("central",BTagEntry::FLAV_B,eta,pt);

	return Btag_Factor;
}


double Get_GenMinus_Weight(GenInfo& gen)
{
	return (gen.Weight > 0) ? 1. : -1.;
}


BtagManager::BtagManager( JetInfo* jetInfo )
{
    jets = jetInfo;

	tagged_b.assign( 10, -1 );
	ntagged_b.assign( 15, -1 );

	op = BTagEntry::OP_MEDIUM;		//csv == 0.8484
}

void BtagManager::Set_OP( BTagEntry::OperatingPoint o )
{
	op = o;
}

/*
double BtagManager::Get_Btag_Scale_Factor( const int idx )
{
	BTagEntry::JetFlavor flavor;
	map<BTagEntry::OperatingPoint, BTagCalibrationReader>* map;

	if( fabs( jets->GenFlavor[idx] ) == 5 )
	{	flavor = BTagEntry::FLAV_B;	map = &map_b;	}
	else if( fabs( jets->GenFlavor[idx] ) == 4 )
	{	flavor = BTagEntry::FLAV_C;	map = &map_c;	}
	else
	{	flavor = BTagEntry::FLAV_UDSG;	map = &map_l;	}

	return map->at( op ).eval_auto_bounds( "central", flavor, jets->Eta[idx], jets->Pt[idx] );
}
*/

double BtagManager::Get_Btag_Scale_Factor( const int idx )
{
	BTagEntry::JetFlavor flavor;

	if( fabs( jets->GenFlavor[idx] ) == 5 )
	{	flavor = BTagEntry::FLAV_B;	}
	else if( fabs( jets->GenFlavor[idx] ) == 4 )
	{	flavor = BTagEntry::FLAV_C;	}
	else
	{	flavor = BTagEntry::FLAV_UDSG; }

	return map_.at( op ).eval_auto_bounds( "central", flavor, jets->Eta[idx], jets->Pt[idx] );
}

double BtagManager::Get_Btag_Efficiency(  const int idx )
{
	double j_pt = jets->Pt[idx];
	double j_eta = jets->Eta[idx];
	if( fabs( jets->GenFlavor[idx] ) == 5)
	{	
		SF_2D jet_eff(eff_b);
		return jet_eff.GetTH2FBinContent( j_pt, j_eta );	
	}
	else if( fabs( jets->GenFlavor[idx] ) == 4 )
	{	
		SF_2D jet_eff(eff_c);
		return jet_eff.GetTH2FBinContent( j_pt, j_eta );	
	}
	else
	{	
		SF_2D jet_eff(eff_l);
		return jet_eff.GetTH2FBinContent( j_pt, j_eta );	
	}	
}

/*
void BtagManager::Register_Init_Maps()
{
	string tagger = "bcheck";
	string filename = "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/DeepCSV_2016LegacySF_V1.csv";

	BTagCalibration* _calib1 = new BTagCalibration( tagger , filename );
	BTagCalibration* _calib2 = new BTagCalibration( tagger , filename );
	BTagCalibration* _calib3 = new BTagCalibration( tagger , filename );

	for(int i=BTagEntry::OP_LOOSE;i!=BTagEntry::OP_RESHAPING;++i)
	{
		map_b[ BTagEntry::OperatingPoint( i ) ] = BTagCalibrationReader( BTagEntry::OperatingPoint( i ), "central", {"up","down"} );
		map_b[ BTagEntry::OperatingPoint( i ) ].load( *_calib1, BTagEntry::FLAV_B, "comb" );
		map_c[ BTagEntry::OperatingPoint( i ) ] = BTagCalibrationReader( BTagEntry::OperatingPoint( i ), "central", {"up","down"} );
		map_c[ BTagEntry::OperatingPoint( i ) ].load( *_calib2, BTagEntry::FLAV_C, "comb" );
		map_l[ BTagEntry::OperatingPoint( i ) ] = BTagCalibrationReader( BTagEntry::OperatingPoint( i ), "central", {"up","down"} );
		map_l[ BTagEntry::OperatingPoint( i ) ].load( *_calib3, BTagEntry::FLAV_UDSG, "incl" );
	}
}
*/

void BtagManager::Register_Init_Maps()
{
	string tagger = "bcheck";
	string filename = "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/DeepCSV_2016LegacySF_V1.csv";

	BTagCalibration* _calib = new BTagCalibration( tagger , filename );

	for(int i=BTagEntry::OP_LOOSE;i!=BTagEntry::OP_RESHAPING;++i)
	{
		map_[ BTagEntry::OperatingPoint( i ) ] = BTagCalibrationReader( BTagEntry::OperatingPoint( i ), "central", {"up","down"} );
		map_[ BTagEntry::OperatingPoint( i ) ].load( *_calib, BTagEntry::FLAV_B, "comb" );
		map_[ BTagEntry::OperatingPoint( i ) ].load( *_calib, BTagEntry::FLAV_C, "comb" );
		map_[ BTagEntry::OperatingPoint( i ) ].load( *_calib, BTagEntry::FLAV_UDSG, "incl" );
	}
}

void BtagManager::Register_Init_TH2( TH2F* b, TH2F* c, TH2F* l )
{
	eff_b = b;
	eff_c = c;
	eff_l = l;
}

void BtagManager::Reset_idx_capacity()
{
	tagged_b.clear();		
	ntagged_b.clear();
}

void BtagManager::Reset_b_tagged_jets_idx()
{
	tagged_b.clear();
}

void BtagManager::Reset_b_ntagged_jets_idx()
{
	ntagged_b.clear();
}

void BtagManager::Set_b_tagged_jets_idx( vector<int>& sel_b_jets )
{
	if( sel_b_jets.empty() )
	{
		tagged_b.clear();
	}
	else
	{
		tagged_b.resize( (int)sel_b_jets.size() );
		for(int i=0;i<(int)sel_b_jets.size();i++)
		{
			tagged_b.at(i) = sel_b_jets.at(i);
		}
	}
}


void BtagManager::Set_b_ntagged_jets_idx( vector<int>& sel_jets )
{
	if( sel_jets.empty() )
	{
		ntagged_b.clear();
	}
	else
	{
		ntagged_b.resize( (int)sel_jets.size() );
		for(int i=0;i<(int)sel_jets.size();i++)
		{
			ntagged_b.at(i) = sel_jets.at(i);
		}
	}
}

double BtagManager::Get_Btag_Weight()
{
	double prob_div = 1.;
	if( !tagged_b.empty() )
	{
		for(int i=0;i<(int)tagged_b.size();i++)
		{	
			double SF = Get_Btag_Scale_Factor( tagged_b.at(i) );
			//cout << "The " << i << " one's tagged b 's SF is : " << SF << endl;  
			prob_div *= SF;	
		}
	}
	
	if( !ntagged_b.empty() )
	{
		for(int i=0;i<(int)ntagged_b.size();i++)
		{
			double SF = Get_Btag_Scale_Factor( ntagged_b.at(i) );
			double EFF = Get_Btag_Efficiency( ntagged_b.at(i) );
			//cout << "The " << i << " one's non-tagged b 's SF and Eff are : " << SF << " , " << EFF << endl;  
			prob_div *= ( 1. -  SF * EFF )/( 1.- EFF );
		}
	}	

	//cout << "Btag Weight : " << prob_div << endl;
	
	return prob_div;
}

//GetName() is in TObject class

TH1F* DataDriven( const string& filename, const string& data_name, TH1F* h_re )
{
	double evt_no_re = h_re->Integral(1,(int)h_re->GetXaxis()->GetNbins()+1);
	TH1F* h_old = new TH1F(h_re->GetName(),"",(int)h_re->GetXaxis()->GetNbins(),(double)h_re->GetXaxis()->GetXmin(),(double)h_re->GetXaxis()->GetXmax());
	TFile* f = new TFile( filename.c_str() );
	f->GetObject( data_name.c_str() , h_old );
	TH1F* h_new = new TH1F( "h_new","", (int)h_re->GetXaxis()->GetNbins(),(double)h_re->GetXaxis()->GetXmin(),(double)h_re->GetXaxis()->GetXmax() );

	h_new = (TH1F*)h_old->Clone();

	f->Close();

	double evt_no_data = h_new->Integral(1,(int)h_new->GetXaxis()->GetNbins()+1);
	h_new->Scale( evt_no_re/evt_no_data );
	h_new->SetName( h_re->GetName() );
	
	return h_new;
}


TH1F* Data_Driven( TH1F* h_data, TH1F* h_re )
{
	double evt_no_re = h_re->Integral(1,(int)h_re->GetXaxis()->GetNbins()+1);
	double evt_no_data = h_data->Integral(1,h_data->GetXaxis()->GetNbins()+1);
	TH1F* h_new = new TH1F();
	*h_new = *h_data;
	
	h_new->Scale( evt_no_re/evt_no_data );
	h_new->SetName( h_re->GetName() );
	return h_new;
}



