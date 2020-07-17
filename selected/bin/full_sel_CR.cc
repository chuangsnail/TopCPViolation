/*************************************************************************
*
*	Filename : full_sel.cc 
*	Description :
*		made date : about 2019 January
*		modified date : 2019 August	( apply Golden Json on real data )
*		for Legacy version data/MC
*		
*		Maybe next time I'll add the trigger on this pre-seletion step 
*	Author : Chen-Yu Chuang
*
*************************************************************************/


#include "TopCPViolation/selected/interface/BranchInfo.h"
#include "TopCPViolation/selected/interface/lep_sel.h"
#include "TopCPViolation/selected/interface/jet_sel.h"
#include "TopCPViolation/selected/interface/checkEvtTool.h"		//for Golden Json file
#include "TopCPViolation/selected/interface/Comp_DataMC.h"

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1.h"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

//	arguments are { 0:(execute command), 1:(original files' path and name), 2:(The pre-sel files' directory's name), 3:(is_data?) }
//	1 need to be this form Ex : /wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_003/bpk_ntuple_ 
//	2 need to add '/' after the directory's name, Ex: BPK_80X_SingleElectron_Run2016H-03Feb2017_ver2-v1/
//	is_data -> [ 1 for real data ; 0 for MC data ]

int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		printf("\nThis is something Wrong! with the arguments' number!");
		return 0;
	}

	//check what type of this dataset (data or MC)
	if( std::stod( std::string( argv[3] ) ) == 1. )
	{	cout << endl << "This dataset is Data " << endl;	}
	else
	{	cout << endl << "This dataset is MC " << endl;	}

	//*********Offline High level Trigger Dealing**********//
	
	vector<int> HLT_MC_mu, HLT_MC_el, HLT_Data_mu, HLT_Data_el;	
	Get_HLT_Info( string("MC_mu"), HLT_MC_mu );			Get_HLT_Info( string("MC_el"), HLT_MC_el );
	Get_HLT_Info( string("Data_mu"), HLT_Data_mu );		Get_HLT_Info( string("Data_el"), HLT_Data_el );
	//*******To prepare the things about MC reweigh********//

	//insert pileup re-weighting information
	vector<double> pileupinfo;
	pileupinfo.assign(76,0.);		int PUI_size = 76;
	char path_PU_file[200] = "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/pileupweights_69200.csv"; 
	GetPileUpInfo( path_PU_file , pileupinfo , &PUI_size);

	TFile* f1 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/old_lep/muIDSF.root");
	TH2F* h_tightMuIDSF;		f1->GetObject("abseta_pt_ratio",h_tightMuIDSF);
	//TFile* f1 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/Lep2016/muid_Run16All.root");
	//TH2D* h_tightMuIDSF;		f1->GetObject("abseta_pt_ratio",h_tightMuIDSF);

	//TFile* f2 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/old_lep/muISOSF.root");
	//TH2F* h_MuISOSF;			f2->GetObject("abseta_pt_ratio",h_MuISOSF);
	TFile* f2 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/Lep2016/muiso_Run16All.root");
	TH2D* h_MuISOSF;			f2->GetObject("muiso",h_MuISOSF);

	//TFile* f3 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/old_lep/muTrgSF.root");
	//TH2F* h_MuTrgSF;			f3->GetObject("abseta_pt_ratio",h_MuTrgSF);
	TFile* f3 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/Lep2016/HLT16_mu_SF.root");
	TH2F* h_MuTrgSF;			f3->GetObject("abseta_pt_ratio",h_MuTrgSF);

	TFile* f4 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/old_lep/elTightIDSF.root");
	TH2F* h_tightElIDSF;		f4->GetObject("EGamma_SF2D",h_tightElIDSF);

	//TFile* f5 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/old_lep/elRECOSF.root");
	//TH2F* h_ElRECOSF;			f5->GetObject("EGamma_SF2D",h_ElRECOSF);
	TFile* f5 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/Lep2016/EGM2D_BtoH_GT20GeV_RecoSF_Legacy2016.root");
	TH2F* h_ElRECOSF;			f5->GetObject("EGamma_SF2D",h_ElRECOSF);

	//TFile* f6 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/old_lep/elTrgSF.root");
	//TH2D* h_ElTrgSF;			f6->GetObject("scale_ele32",h_ElTrgSF);
	TFile* f6 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/Lep2016/HLT_SF_16.root");
	TH2D* h_ElTrgSF;			f6->GetObject("abseta_pt_ratio",h_ElTrgSF);
	//we need to delete the f1~f7 after finising using the objects from them		//so we delete them after selecting
	
	TH2F* eff_b;		TH2F* eff_c;		TH2F* eff_l;
	TFile* f7 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/beffPlot_WJets_0pt2217.root");
	f7->GetObject( "eff_b", eff_b );	f7->GetObject( "eff_c", eff_c );	f7->GetObject( "eff_l", eff_l );

	//***** Open old files and read *****//

	TChain* root = new TChain( "root" );
    //root->Add("/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_001/bpk_ntuple_*.root");

	//To capture original files' name

	char star_point_root[10] = "*.root";						//There is a star!!!!!!!!!!!!!!!!!!
	char pre_name[500];
	strcpy(pre_name,argv[1]);
	//Ex. pre_name now is "/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_003/bpk_ntuple_"
	strcat(pre_name,star_point_root);
	//Ex.pre_name now is "/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_003/bpk_ntuple_*.root"
	
	root->Add( pre_name );

	//To make the output files' name

	char path_filename[500] = "/wk_cms2/cychuang/2016_full_CR/";
	strcat(path_filename,argv[2]);
	char temp_path_filename[500];
	strcpy( temp_path_filename, path_filename );

	EvtInfo evt ;
	evt.Register( root );
	JetInfo jets ;
	jets.Register( root , "JetInfo" );
	LeptonInfo leps;
	leps.Register( root , "LepInfo" );

	TrgInfo trg;
	trg.Register( root , "TrgInfo" );
	
	GenInfo gen;
	gen.Register( root );
	/*
	PhotonInfo photonInfo;
	photonInfo.Register( root , "PhotonInfo" );
	VertexInfo vertexInfo;
	vertexInfo.Register( root , "VertexInfo" );
	RunInfo runInfo;
	runInfo.Register( root , "RunInfo" );
	*/

	bool is_data = false;
	if( std::stod( std::string( argv[3] ) ) == 1. ) {	is_data = true;	}
    
	int pre_sel_No = 0;
	int not_pass_Golden = 0;
    int total_entries = root->GetEntries();
    int k = 1;
	int r = 1;
    int entry = 0;
    int temp = 0;
    bool not_end = true;
    
    //TFile* f_new;
    TTree* root_new;
   
	cout << endl << "At the start of the full-seletion , and the total entries number before full-selection is " << total_entries << endl;

	if(total_entries == 0)	{	not_end = false;	}

	double t_weight;
	double lep_weight;
	double btag_weight;
	double other_weight;

	int SelBJets[5];
	int SelJets[20];

	int JetsNo;

	for(int i=0;i<5;++i)
	{	SelBJets[i] = -1;	}
	for(int i=0;i<20;++i)
	{	SelJets[i] = -1;	}

	string channel;
	
	vector<int> sel_b_jets;
	vector<int> sel_jets;
	
	//prepare btag object
	BtagManager bmgr( &jets );
	bmgr.Register_Init_Maps();
	bmgr.Register_Init_TH2( eff_b, eff_c, eff_l );
	bmgr.Set_OP( BTagEntry::OP_LOOSE );

    while( not_end )
    {
		/*
		//if you wnt to just test few file, use this
		if( k >= 5 )
		{	not_end = false;	}
		*/

		strcpy( path_filename, temp_path_filename );
		char filename_no[50] = "";
		sprintf( filename_no, "full_sel_bpk_ntuple_%d.root", k );
		strcat( path_filename, filename_no );

        TFile* f_new = new TFile( path_filename , "recreate" );
        root_new = root->CloneTree( 0 );

		root_new->Branch( "t_Weight", &t_weight, "t_Weight/D" );
		root_new->Branch( "LepWeight", &lep_weight, "LepWeight/D" );
		root_new->Branch( "BtagWeight", &btag_weight, "BtagWeight/D" );
		root_new->Branch( "OtherWeight", &other_weight, "OtherWeight/D" );
		
		root_new->Branch( "JetsNo", &JetsNo, "JetsNo/I" );
		root_new->Branch( "SelJets", SelJets, "SelJets[JetsNo]/I" );
		root_new->Branch( "SelBJets", SelBJets, "SelBJets[2]/I" );
		
		root_new->Branch( "Channel", &channel );
        
	    for(entry=temp;entry<total_entries;entry++)
   		{
        	root->GetEntry(entry);

			//if(entry%1000000 == 0)
			//{	cout << "At entry : " << entry << endl;	}
		
			t_weight = 1.;	
			lep_weight = 1.;
			btag_weight = 1.;
			other_weight = 1.;

			for(int i=0;i<(int)sel_b_jets.size();++i)
			{
				SelBJets[i] = -1;
			}
			for(int i=0;i<(int)sel_jets.size();++i)
			{
				SelJets[i] = -1;
			}
			
			JetsNo = 0;
			channel = "";
			
			sel_b_jets.clear();
			sel_jets.clear();

            if(entry == total_entries - 1)
            {	
				root_new->AutoSave();
                not_end = false;
				break;
				//Here we directly abandon the last event
				//it is needed to be solve in the future
			}
			
			if( (double)entry/(double)total_entries >= 0.1*r )
			{
				cout << "*" ;
				r++;
			}
			//pile-up reweigh MC

			bool pass_PU = true;
			if(!is_data){
				other_weight *= GetPileUpWeight( &evt, pileupinfo, &pass_PU );
			}
			if( !pass_PU ) continue;

			//Start pre-selstion
        
        	int idx_Selected_Mu = -1;
        	int idx_Selected_El = -1;
        
			bool pass_sel_mu = Pass_CR_Selected_Muon(leps,idx_Selected_Mu);
			bool pass_sel_el = Pass_CR_Selected_Electron(leps,idx_Selected_El,evt);
			bool pass_veto_el = false;		bool pass_veto_mu = false;

			if( (pass_sel_mu||pass_sel_el) && !(pass_sel_mu&&pass_sel_el) )
			{
				if( pass_sel_mu )
				{
					pass_veto_el = Pass_CR_Veto_Electron(leps,idx_Selected_Mu,evt);
					pass_veto_mu = Pass_CR_Veto_Muon(leps,idx_Selected_Mu);
				}
				if( pass_sel_el )
				{
					pass_veto_el = Pass_CR_Veto_Electron(leps,idx_Selected_El,evt);
					pass_veto_mu = Pass_CR_Veto_Muon(leps,idx_Selected_El);
				}
			}
			else {	continue;	}
		
			bool is_mu_channel = pass_sel_mu && pass_veto_mu && pass_veto_el ;
			bool is_el_channel = pass_sel_el && pass_veto_el && pass_veto_mu ;
        
			int idx_Selected_Lep = -1;
			if( is_mu_channel&&!is_el_channel )
			{
				idx_Selected_Lep = idx_Selected_Mu;
				channel = "mu";
			}
			else if( is_el_channel&&!is_mu_channel )
			{
				idx_Selected_Lep = idx_Selected_El;
				channel = "el";
			}
			else
			{	continue;	}
				
			bool is_pass_HLT = false;
			if( is_data )
			{
				if( channel == "mu" )
				{	is_pass_HLT = Pass_HLT( trg, HLT_Data_mu );	}
				else if(channel == "el" )
				{	is_pass_HLT = Pass_HLT( trg, HLT_Data_el );	}
			}
			else
			{	
				if( channel == "mu" )
				{	is_pass_HLT = Pass_HLT( trg, HLT_MC_mu );	}
				else if( channel == "el" )
				{	is_pass_HLT = Pass_HLT( trg, HLT_MC_el );	}
			}
			if( !is_pass_HLT )
			{	continue;	}
			
        	bool pass_sel_jet = Pass_SR_Selected_Jets_Case( jets, sel_jets );
			if( !pass_sel_jet )
			{	continue;	}
        
		   	if( idx_Selected_Lep == -1 )
			{	continue;	}	
			
			bool pass_delR = Pass_delR_Jet_Lep( jets, leps, sel_jets, idx_Selected_Lep );
            if( !pass_delR )
            {	continue;	}
					
			bool pass_sel_b = Pass_CR_bjets( jets, sel_jets, sel_b_jets);
			if( !pass_sel_b )	
			{	continue;	}
            
			if(!is_data)
			{
				//do the b-tag reweighting
						
				bmgr.Reset_idx_capacity();
				vector<int> tmp_all_jets;

				for(int h=0;h<(int)sel_jets.size();h++) {
					tmp_all_jets.push_back( sel_jets.at(h) );
				}
				for(int h=0;h<(int)sel_b_jets.size();h++) {
					tmp_all_jets.push_back( sel_b_jets.at(h) );
				}

				bmgr.Set_b_ntagged_jets_idx( tmp_all_jets );
				vector<int> tmp_empty_vec;
				bmgr.Set_b_tagged_jets_idx( tmp_empty_vec );
				btag_weight = bmgr.Get_Btag_Weight();

				//do the lepton scale factor
				if(idx_Selected_Lep == idx_Selected_Mu){
					lep_weight *= GetLepSF_TH2F(&leps,h_tightMuIDSF,idx_Selected_Lep);
					lep_weight *= GetLepSF_TH2D(&leps,h_MuISOSF,idx_Selected_Lep);
					lep_weight *= GetLepSF_TH2F(&leps,h_MuTrgSF,idx_Selected_Lep);
				}
				else if(idx_Selected_Lep == idx_Selected_El){
					lep_weight *= GetLepSF_TH2F(&leps,h_tightElIDSF,idx_Selected_Lep);
					lep_weight *= GetLepSF_TH2F(&leps,h_ElRECOSF,idx_Selected_Lep);
					lep_weight *= GetLepSF_TH2D(&leps,h_ElTrgSF,idx_Selected_Lep);
				}
					
				//do the gen-weight (second linear order calibration)
				other_weight *= Get_GenMinus_Weight(gen);
			}
			
			t_weight = lep_weight * btag_weight * other_weight;
			JetsNo = (int)sel_jets.size();

			//Now Fill in the pre-selection case		
			root_new->Fill();
			pre_sel_No++;
            
            if(root_new->GetEntries() == 100000)
            {
                temp = entry+1;
                root_new->AutoSave();
				//cout << "Now at " << entry/total_entries * 100 << "%"<< endl;
				k++;
                break;
            }
            
			/*
            if(entry == total_entries - 1)
            {
                root_new->AutoSave();
                not_end = false;
                break;
            }
			*/
            
        }       //end of for-loop
		//f_new->Map();
		f_new->Close();
    }   //end of while loop
    



	printf("\nThe original total events No.: %d",total_entries);
	printf("\nThe events No. which is not pass GoldenJson : %d",not_pass_Golden);
	printf("\nThe survived events No. after full-selection : %d",pre_sel_No);

}
