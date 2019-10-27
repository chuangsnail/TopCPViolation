#ifndef REWEIGHTMC_H
#define REWEIGHTMC_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <math.h> //for isnan()
#include <random>
#include <climits>

#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TLorentzVector.h"

#include "TopCPViolation/selected/interface/BranchInfo.h"
#include "TopCPViolation/selected/interface/GetInfo.h"

#include "CondFormats/BTauObjects/interface/BTagCalibration.h"
#include "CondTools/BTau/interface/BTagCalibrationReader.h"

using namespace std;

double GetPileUpWeight(EvtInfo* event, const vector<double>&, bool* pass_PU);

double GetLepSF_TH2F(LeptonInfo* ,TH2F* ,const int );

double GetLepSF_TH2D(LeptonInfo* ,TH2D* ,const int );


class BtagManager
{
public:
	
    JetInfo* jets;

	map<BTagEntry::OperatingPoint, BTagCalibrationReader> map_b, map_c, map_l;

	BTagEntry::OperatingPoint op;			//this is the case for all with the same operating point
	
    vector<int> tagged_b;	//(10,-1);
	vector<int> ntagged_b;	//(10,-1);

	TH2F* eff_b;
	TH2F* eff_c;
	TH2F* eff_l;

	BtagManager( JetInfo* jetInfo );		//Constructor

	void Set_OP( BTagEntry::OperatingPoint );

	void Register_Init_Maps();
	void Register_Init_TH2( TH2F* , TH2F* , TH2F* );

	void Reset_idx_capacity();
	void Reset_b_tagged_jets_idx();
	void Reset_b_ntagged_jets_idx();
	void Set_b_tagged_jets_idx( vector<int>& sel_b_jets );
	void Set_b_ntagged_jets_idx( vector<int>& sel_jets );

	double Get_Btag_Scale_Factor( const int );
	double Get_Btag_Efficiency( const int );		//just for op = Medium
	double Get_Btag_Weight();
	
	~BtagManager() 
	{
        tagged_b.clear();
        ntagged_b.clear();
	}		
};


//open file in a fn. and use its obj in the file outside the fn.and have somethiing wrong!!!!
/*
void BtagManager::Register_Init_TH2()
{
cout << "Start of Register_Init_TH2()" << endl;
	//Open the root file of the MC btag-eff of three real type of jets
	//TFile f("/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/data/beffPlot_TT_0pt8484.root");
	f->GetObject( "eff_b", eff_b );
	f->GetObject( "eff_c", eff_c );
	f->GetObject( "eff_l", eff_l );
	f->Close();
	//delete f;
cout << "End of Register_Init_TH2()" << endl;
}
*/


double Get_BtagWeight_Factor(std::map<BTagEntry::OperatingPoint,BTagCalibrationReader>& ,JetInfo& ,BTagEntry::OperatingPoint ,const int& );

double Get_GenMinus_Weight(GenInfo& );



bool Is_Well_Match( JetInfo& jets, const int& idx, const double& res )
{
	double deta = jets.Eta[idx] - jets.GenEta[idx];
	double dphi = TVector2::Phi_mpi_pi( (double)( jets.Phi[idx] - jets.GenPhi[idx] ) );
	double delR = TMath::Sqrt( deta * deta + dphi * dphi );

	if( delR >= 0.4 / 2. ){
		return false;
	}	
	if( fabs( jets.Pt[idx] - jets.GenPt[idx] ) >= 3 * res * jets.Pt[idx] ){
		return false;
	}

	return true;
}

double Scale_Case( JetInfo& jets, const int& idx, const double& ressf )
{
	const double newpt = std::max( 0.0, jets.GenPt[idx] + ressf * ( jets.Pt[idx] - jets.GenPt[idx] ) );
    const double scale = newpt / jets.Pt[idx];

	return scale;
}

double Smeared_Case( const double& res, const double& ressf )
{
	const double width = ressf > 1 ? sqrt( ressf * ressf - 1 ) * res : 0;

	std::normal_distribution<> myrand( 0, width );
    std::uint32_t seed              = 37428479;
	std::mt19937 m_random_generator = std::mt19937( seed );

	double scale = 1. + myrand( m_random_generator );
	if( scale <= 0 || isnan( scale ) ){
		return 1;
	}
	else{
		return scale;
	}
}

double Get_JERScale( JetInfo& jets, const int& i )
{
	double scale = 1.;

	double ressf = jets.JERScale[i];
	double res   = jets.JERPt[i];

	if( Is_Well_Match( jets, i, res ) ){
		scale = Scale_Case( jets, i, ressf );
	}
	else{
		scale = Smeared_Case( res, ressf );
	}
	return scale;
}

//it can change the jets' Pt/Eta/Phi/P4...(kinematic)
void JERCor( JetInfo& jets )
{
	for(int i=0;i<jets.Size;++i)
	{
		TLorentzVector tmp_jet;
		tmp_jet.SetPxPyPzE( jets.Px[i], jets.Py[i], jets.Pz[i], jets.Energy[i] );
		tmp_jet *= Get_JERScale( jets, i );
		jets.Px[i] = tmp_jet.Px();
		jets.Py[i] = tmp_jet.Py();
		jets.Pz[i] = tmp_jet.Pz();
		jets.Pt[i] = tmp_jet.Pt();
		jets.Energy[i] = tmp_jet.Energy();
	}
}

/*
class JetCorMgr
{
protected:
	JetInfo* jets;
public:
	
	enum JetMatch
	{
		IsNotWellMatch = 0,
		IsWellMatch = 1
	}
	
	JetCorMgr(){}

	JetCorMgr( JetInfo* j )
	{
		jets = j;
	}
	
	double Get_JERScale( JetMatch m ,int& idx );
	JetMatch IsWellMatch( int& idx );
}
*/

//to use Data driven on QCD

TH1F* DataDriven( const string& filename, const string& h_name, TH1F* h_re );

#endif

