#ifndef REWEIGHTMC_H
#define REWEIGHTMC_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TopCPViolation/selected/interface/BranchInfo.h"
#include "TopCPViolation/selected/interface/GetInfo.h"

#include "CondFormats/BTauObjects/interface/BTagCalibration.h"
#include "CondTools/BTau/interface/BTagCalibrationReader.h"

using namespace std;

double GetPileUpWeight(EvtInfo* event, const vector<double>&, bool* pass_PU);

double GetLepSF_TH2F(LeptonInfo* ,TH2F* ,const int );

double GetLepSF_TH2D(LeptonInfo* ,TH2D* ,const int );

//template <class T> double GetLepSF_TH2(LeptonInfo* ,T* ,const int );

class BtagManager
{
public:
	
	int size;
    JetInfo* jets;

	map<BTagEntry::OperatingPoint, BTagCalibrationReader> map_b, map_c, map_l;
	
    vector<int> tagged_b;	//(10,-1);
	vector<int> ntagged_b;	//(10,-1);

	TH2F* eff_b;
	TH2F* eff_c;
	TH2F* eff_l;

	//TFile* f;

	BtagManager( JetInfo& jets );		//Constructor

	//void Set_All_op( BTagEntry::OperatingPoint );
	//void Set_one_op( const int idx, BTagEntry::OperatingPoint point );
	void Register_Init_Maps();
	void Register_Init_TH2( TH2F* , TH2F* , TH2F* );
	
	double Get_Btag_Scale_Factor( const int );
	double Get_Btag_Efficiency( const int );		//just for op = Medium
	
	double Get_Btag_Weight();

	void Set_b_tagged_jets_idx( vector<int>& sel_b_jets );
	void Set_b_ntagged_jets_idx( vector<int>& sel_jets );

	~BtagManager() 
	{
        tagged_b.clear();
        ntagged_b.clear();
	}
	
		
};

BtagManager::BtagManager( JetInfo* jetInfo )
{
    jets = jetInfo;

	tagged_b.assign( 10, -1 );
	ntagged_b.assign( 10, -1 );
}


void BtagManager::Register_Init_Maps()
{
	string tagger = "bcheck";
	string filename = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/data/CSVv2_Moriond17_B_H.csv";

	BTagCalibration* _calib = new BTagCalibration( tagger , filename );

	for(int i=BTagEntry::OP_LOOSE;i!=BTagEntry::OP_RESHAPING;++i)
	{
		map_b[ BTagEntry::OperatingPoint( i ) ] = BTagCalibrationReader( BTagEntry::OperatingPoint( i ), "central", {"up","down"} );
		map_b[ BTagEntry::OperatingPoint( i ) ].load( *_calib, BTagEntry::FLAV_B, "comb" );
		map_c[ BTagEntry::OperatingPoint( i ) ] = BTagCalibrationReader( BTagEntry::OperatingPoint( i ), "central", {"up","down"} );
		map_c[ BTagEntry::OperatingPoint( i ) ].load( *_calib, BTagEntry::FLAV_C, "incl" );
		map_l[ BTagEntry::OperatingPoint( i ) ] = BTagCalibrationReader( BTagEntry::OperatingPoint( i ), "central", {"up","down"} );
		map_l[ BTagEntry::OperatingPoint( i ) ].load( *_calib, BTagEntry::FLAV_UDSG, "incl" );
	}
}

double BtagManager::Get_Btag_Scale_Factor( const int idx )
{
	BTagEntry::JetFlavor flavor;
	map<BTagEntry::OperatingPoint, BTagCalibrationReader>* map;
    BtagEntry::OperatingPoint op = BTagEntry::OP_MEDIUM;

	if( fabs( jets->GenFlavor[idx] ) == 5 )
	{	flavor = BTagEntry::FLAV_B;	map = &map_b;	}
	else if( fabs( jets->GenFlavor[idx] ) == 4 )
	{	flavor = BTagEntry::FLAV_C;	map = &map_c;	}
	else
	{	flavor = BTagEntry::FLAV_UDSG;	map = &map_l;	}

	return map->at( op ).eval_auto_bounds( "central", flavor, jets->Eta[idx], jets->Pt[idx] );
}

void BtagManager::Register_Init_TH2( TH2F* b, TH2F* c, TH2F* l )
{
	eff_b = b;
	eff_c = c;
	eff_l = l;
}

double BtagManager::Get_Btag_Efficiency(  const int idx )
{
	double j_pt = jets->Pt[idx];
	double j_eta = jets->Eta[idx];
	//cout << "j_pt: " << j_pt << "j_eta: " << j_eta << endl;
	if( fabs( jets->GenFlavor[i] ) == 5)
	{	
		SF_2D jet_eff(eff_b);
		return jet_eff.GetTH2FBinContent( j_pt, j_eta );	
	}
	else if( fabs( jets->GenFlavor[i] ) == 4 )
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

void BtagManager::Set_b_tagged_jets_idx( vector<int>& sel_b_jets )
{
	tagged_b.resize( (int)sel_b_jets.size() );
	for(int i=0;i<(int)sel_b_jets.size();i++)
	{
		tagged_b.at(i) = sel_b_jets.at(i);
	}
}
void BtagManager::Set_b_ntagged_jets_idx( vector<int>& sel_jets )
{
	ntagged_b.resize( (int)sel_jets.size() );
	for(int i=0;i<(int)sel_jets.size();i++)
	{
		ntagged_b.at(i) = sel_jets.at(i);
	}
}

double BtagManager::Get_Btag_Weight()
{
	double prob_div = 1.;
	for(int i=0;i<(int)tagged_b.size();i++)
	{
		prob_div *= this->Get_Btag_Scale_Factor( tagged_b.at(i) );
	}
	for(int i=0;i<(int)ntagged_b.size();i++)
	{
		prob_div *= ( 1. - ( this->Get_Btag_Scale_Factor( this->ntagged_b.at(i) ) * this->Get_Btag_Efficiency( this->ntagged_b.at(i) ) ) )/( 1.- this->Get_Btag_Efficiency( this->ntagged_b.at(i) ) );
	}
	
	return prob_div;
}






double Get_BtagWeight_Factor(std::map<BTagEntry::OperatingPoint,BTagCalibrationReader>& ,JetInfo& ,BTagEntry::OperatingPoint ,const int& );

double Get_GenMinus_Weight(GenInfo& );

#endif

