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
	
	enum flav
	{
		B_Quark,
		C_Quark,
		Light_Quark		//for UDSG
	};
	
	/*
	 * analogues with the enum in BTagEntry.h's BTagEntry class
	 *
	 *	enum JetFlavor {
	 *		FLAV_B = 0,
	 *		FLAV_C = 1,
	 *	    FLAV_UDSG = 2,
	 *	   	};
	 */


	int size;
	//vector<int> b_jets_idx(10);
	//vector<int> c_jets_idx(10);
	//vector<int> l_jets_idx(10);

	map<BTagEntry::OperatingPoint, BTagCalibrationReader> map_b, map_c, map_l;
	
	vector<double> pt;	//(128,-1.);		//for all jets in an event
	vector<double> eta;	//(128,-1.);		//for all jets in an event
	vector<BtagManager::flav> flavors;		//(128,-1.);
	vector<BTagEntry::OperatingPoint> op;	//(128,BTagEntry::OP_MEDIUM);

	vector<int> tagged_b;	//(10,-1);
	vector<int> ntagged_b;	//(10,-1);

	TH2F* eff_b;
	TH2F* eff_c;
	TH2F* eff_l;

	//TFile* f;

	BtagManager( JetInfo& jets );		//Constructor

	void Set_All_op( BTagEntry::OperatingPoint );
	void Set_one_op( const int idx, BTagEntry::OperatingPoint point);
	void Register_Init_Maps();
	//void Register_Init_TH2();
	void Register_Init_TH2( TH2F* , TH2F* ,TH2F* );
	
	double Get_Btag_Scale_Factor( const int );
	double Get_Btag_Efficiency( const int );		//just for op = Medium
	
	double Get_Btag_Weight();

	void Set_b_tagged_jets_idx( vector<int>& sel_b_jets );
	void Set_b_ntagged_jets_idx( vector<int>& sel_jets );

	~BtagManager() 
	{
		//b_jets_idx.clear();
		//c_jets_idx.clear();
		//l_jets_idx.clear();
		pt.clear();
		eta.clear();
		flavors.clear();
		op.clear();
		
		/*
		delete eff_b;
		delete eff_c;
		delete eff_l;
		delete f;
		*/
	}
	
		
};

BtagManager::BtagManager( JetInfo& jets )
{
	size = (int)jets.Size;

	//b_jets_idx.clear();
	//c_jets_idx.clear();
	//l_jets_idx.clear();
	
	/*
	pt.resize( jets.Size );
	eta.resize( jets.Size );
	flavors.resize( jets.Size );
	op.resize( jets.Size );
	*/

	pt.assign( jets.Size, -1. );
	eta.assign( jets.Size, -5. );
	flavors.assign( jets.Size, BtagManager::B_Quark );
	op.assign( jets.Size, BTagEntry::OP_MEDIUM );

	tagged_b.assign( 10, -1 );
	ntagged_b.assign( 10, -1 );

	for(int i=0;i<(int)jets.Size;i++)
	{
		pt.at(i) = jets.Pt[i];
		eta.at(i) = jets.Eta[i];
		//op.at(i) = BTagEntry::OP_MEDIUM;		
		//default are all the jets on the Medium working point of btag
	}
		
	for(int i=0;i<(int)jets.Size;i++)
	{
		if( fabs( jets.GenFlavor[i] ) == 5 )
		{	
			//b_jets_idx.push_back(i);
			flavors.at(i) = BtagManager::B_Quark;
		}
		else if( fabs( jets.GenFlavor[i] ) == 4 )
		{	
			//c_jets_idx.push_back(i);
			flavors.at(i) = BtagManager::C_Quark;
		}
		else
		{	
			//l_jets_idx.push_back(i);
			flavors.at(i) = BtagManager::Light_Quark;
		}
	}

	/*
	eff_b = new TH2F();
	eff_c = new TH2F();
	eff_l = new TH2F(); 
	f = new TFile("/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/data/beffPlot_TT_0pt8484.root");
	*/

}


void BtagManager::Set_All_op( BTagEntry::OperatingPoint o )
{
	op.clear();
	op.assign( size, o );
}

void BtagManager::Set_one_op( const int idx, BTagEntry::OperatingPoint point)
{
	op.at(idx) = point;
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

	if( flavors.at(idx) == BtagManager::B_Quark )
	{	flavor = BTagEntry::FLAV_B;	map = &map_b;	}
	else if( flavors.at(idx) == BtagManager::C_Quark )
	{	flavor = BTagEntry::FLAV_C;	map = &map_c;	}
	else if( flavors.at(idx) == BtagManager::Light_Quark )
	{	flavor = BTagEntry::FLAV_UDSG;	map = &map_l;	}
	else
	{
		cout << endl <<  "-----FAULT----- The Input flavor in Get_Btag_Scale_Factor() fn. is wrong!" << endl;
		return -1.;
	}
	
	return map->at( op.at(idx) ).eval_auto_bounds( "central", flavor, eta.at(idx), pt.at(idx) );	
}

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

void BtagManager::Register_Init_TH2( TH2F* b, TH2F* c, TH2F* l )
{
	eff_b = b;
	eff_c = c;
	eff_l = l;
}



double BtagManager::Get_Btag_Efficiency(  const int idx )
{
	double j_pt = pt.at(idx);
	double j_eta = eta.at(idx);
	//cout << "j_pt: " << j_pt << "j_eta: " << j_eta << endl;
	if( flavors.at(idx) == BtagManager::B_Quark )
	{	
		SF_2D jet_eff(eff_b);
		return jet_eff.GetTH2FBinContent( j_pt, j_eta );	
	}
	else if( flavors.at(idx) == BtagManager::C_Quark )
	{	
		SF_2D jet_eff(eff_c);
		return jet_eff.GetTH2FBinContent( j_pt, j_eta );	
	}
	else if( flavors.at(idx) == BtagManager::Light_Quark )
	{	
		SF_2D jet_eff(eff_l);
		return jet_eff.GetTH2FBinContent( j_pt, j_eta );	
	}
	else
	{
		cout << endl <<  "-----FAULT----- The Input flavor in Get_Btag_Efficiency() fn. is wrong!" << endl;
		return -1.;
	}
	
cout << "End of Get_Btag_Efficiency(  const int idx )" << endl;
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
	//double prob_data = 1.;
	//double prob_mc = 1.;
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

