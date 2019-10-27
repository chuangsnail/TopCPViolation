/*************************************************************************
*
*	Filename : store_mva.cc 
*	Description :
*		made date : about 2019 January
*		modified date : 2019 Sep 30	( apply Golden Json on real data )
*		for Legacy version data/MC
*		
*		Maybe next time I'll add the trigger on this pre-seletion step 
*	Author : Chen-Yu Chuang
*
*************************************************************************/
#include "TopCPViolation/selected/interface/Comp_DataMC.h"
#include "TopCPViolation/selected/interface/prepare_mva.h"

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

	vector<int> HLT_MC_mu, HLT_MC_el, HLT_Data_mu, HLT_Data_el;
	
	Get_HLT_Info( string("MC_mu"), HLT_MC_mu );			Get_HLT_Info( string("MC_el"), HLT_MC_el );
	
	//*******To prepare the things about MC reweigh********//

	//insert pileup re-weighting information
	vector<double> pileupinfo;
	pileupinfo.assign(76,0.);
	int PUI_size = 76;
	char path_PU_file[200] = "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/pileupweights_69200.csv"; 
	GetPileUpInfo( path_PU_file , pileupinfo , &PUI_size);

	//insert tight-Muon ID Scale Factors' TH2F*
	TFile* f1 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/muIDSF.root");
	TH2F* h_tightMuIDSF;		f1->GetObject("abseta_pt_ratio",h_tightMuIDSF);

	TFile* f2 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/muISOSF.root");
	TH2F* h_MuISOSF;			f2->GetObject("abseta_pt_ratio",h_MuISOSF);

	TFile* f3 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/muTrgSF.root");
	TH2F* h_MuTrgSF;			f3->GetObject("abseta_pt_ratio",h_MuTrgSF);

	//insert tight-Electron ID Scale Factor TH2F*
	TFile* f4 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/elTightIDSF.root");
	TH2F* h_tightElIDSF;		f4->GetObject("EGamma_SF2D",h_tightElIDSF);

	TFile* f5 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/elRECOSF.root");
	TH2F* h_ElRECOSF;			f5->GetObject("EGamma_SF2D",h_ElRECOSF);

	TFile* f6 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/elTrgSF.root");
	TH2D* h_ElTrgSF;			f6->GetObject("scale_ele32",h_ElTrgSF);
	//we need to delete the f1~f7 after finising using the objects from them		//so we delete them after selecting
	
	TH2F* eff_b;		TH2F* eff_c;		TH2F* eff_l;
	TFile* f7 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/beffPlot_TTbar_0pt6321.root");
	f7->GetObject( "eff_b", eff_b );	f7->GetObject( "eff_c", eff_c );	f7->GetObject( "eff_l", eff_l );



	//prepare for files' names

	TChain* root = new TChain( "root" );
	char pre_name[700] = "/wk_cms2/cychuang/2016legacy_full_sel_file/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8/full_sel_bpk_ntuple_*.root";
	root->Add( pre_name );

	//To make the output files' name

	char path_filename[500] = "/wk_cms2/cychuang/mva_sample/";
	char temp_path_filename[500];
	strcpy( temp_path_filename, path_filename );

	EvtInfo evt ;
	evt.Register( root );
	JetInfo jets ;
	jets.Register( root , "JetInfo" );
	LeptonInfo leps;
	leps.Register( root , "LepInfo" );
	TrgInfo trgInfo;
	trgInfo.Register( root , "TrgInfo" );	
	GenInfo gen;
	gen.Register( root );

	/*
	PhotonInfo photonInfo;
	photonInfo.Register( root , "PhotonInfo" );
	TrgInfo trgInfo;
	trgInfo.Register( root , "TrgInfo" );
	VertexInfo vertexInfo;
	vertexInfo.Register( root , "VertexInfo" );
	RunInfo runInfo;
	runInfo.Register( root , "RunInfo" );
	*/


	int pre_sel_No = 0;
    int total_entries = root->GetEntries();
    int k = 1;
	int r = 1;
    int entry = 0;
    int temp = 0;
    bool not_end = true;
    
    //TFile* f_new;
    TTree* root_new;
   
	cout << endl << "At the start of the pre-seletion , and the total entries number before full-selection is " << total_entries << endl;

    while( not_end )
    {

		strcpy( path_filename, temp_path_filename );
		char filename_no[50] = "";
		sprintf( filename_no, "bpk_ntuple_%d.root", k );
		strcat( path_filename, filename_no );

        TFile* f_new = new TFile( path_filename , "recreate" );
        root_new = root->CloneTree( 0 );
      
		//store info in an event to the selection file
		
		double evt_weight;
		int sel_lep_idx;
		vector<int> sel_jets_idx;
		vector<int> sel_b_jets_idx;
		int had_b, had_j1, had_j2;	

		root_new->Branch("evt_weight",&evt_weight,"evt_weight/D");
		root_new->Branch("sel_lep_idx",&sel_lep_idx,"sel_lep_idx/I");
		root_new->Branch("sel_jets_idx",&sel_jets_idx,"sel_jets_idx[]/I");
		root_new->Branch("sel_b_jets_idx",&sel_b_jets_idx,"sel_b_jets_idx[]/I");
		root_new->Branch("had_b",&had_b,"had_b/I");
		root_new->Branch("had_j1",&had_j1,"had_j1/I");
		root_new->Branch("had_j2",&had_j2,"had_j2/I");


		//prepare for some objs
		string channel = "";
			
		//prepare btag object
		BtagManager bmgr( &jets );
		bmgr.Register_Init_Maps();
		bmgr.Register_Init_TH2( eff_b, eff_c, eff_l );
    
    	//prepare gen object
    	MVAMgr mvamgr( &gen, &jets, &leps );

	    for(entry=temp;entry<total_entries;entry++)
   		{
        	root->GetEntry(entry);

			double weight = 1.;

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
			weight = weight * GetPileUpWeight( &evt, pileupinfo, &pass_PU );
		
			if(!pass_PU)
			{	continue;	}

			//Start selstion
        
        	int idx_Selected_Mu = -1;
        	int idx_Selected_El = -1;
        
			bool pass_sel_mu = Pass_SR_Selected_Muon(leps,idx_Selected_Mu);
			bool pass_sel_el = Pass_SR_Selected_Electron(leps,idx_Selected_El);
			bool pass_veto_el = false;		bool pass_veto_mu = false;

			if( (pass_sel_mu||pass_sel_el) && !(pass_sel_mu&&pass_sel_el) )
			{
				if(pass_sel_mu)
				{
					pass_veto_el = Pass_SR_Veto_Electron( leps, idx_Selected_Mu );
					pass_veto_mu = Pass_SR_Veto_Muon( leps, idx_Selected_Mu );
				}
				if(pass_sel_el)
				{
					pass_veto_el = Pass_SR_Veto_Electron( leps, idx_Selected_El );
					pass_veto_mu = Pass_SR_Veto_Muon( leps, idx_Selected_El );
				}
			}
			else
			{	continue;	}
		
			bool is_mu_channel = pass_sel_mu && pass_veto_mu && pass_veto_el ;
			bool is_el_channel = pass_sel_el && pass_veto_el && pass_veto_mu ;
        
			vector<int> sel_jets;		sel_jets.assign(128,-1);
			vector<int> sel_b_jets;		sel_b_jets.assign(2,-1);
		
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
			
			//Apply HLT trigger		
				
			bool is_pass_HLT = false;
			
			if(channel == "mu")
			{	is_pass_HLT = Pass_HLT( trgInfo, HLT_MC_mu );	}
			else if(channel == "el")
			{	is_pass_HLT = Pass_HLT( trgInfo, HLT_MC_el );	}
			
			if( !is_pass_HLT )
			{	continue;	}
				
			//luminosity cali after trigger

			if(channel == "mu")
			{	weight *= lumi_cali_trg(35.811/35.9);	}
			if(channel == "el")
			{	weight *= lumi_cali_trg(35.615/35.9);	}


        	bool pass_sel_jet = Pass_SR_Selected_Jets_Case( jets, sel_jets );
			if( !pass_sel_jet )
			{	continue;	}
        
			bool pass_delR = Pass_delR_Jet_Lep( jets, leps, sel_jets, idx_Selected_Lep );
            if( !pass_delR )
            {	continue;	}
					
			bool pass_sel_b = Pass_SR_bjets( jets, sel_jets, sel_b_jets);
			if( !pass_sel_b )	
			{	continue;	}
				
			//do the b-tag reweighting
			bmgr.Reset_idx_capacity();
			bmgr.Set_b_tagged_jets_idx( sel_b_jets );
			bmgr.Set_b_ntagged_jets_idx( sel_jets );
			double btag_weight = bmgr.Get_Btag_Weight();
			weight *= btag_weight;

			//do the lepton scale factor
			if(idx_Selected_Lep == idx_Selected_Mu){
				weight = weight * GetLepSF_TH2F( &leps, h_tightMuIDSF, idx_Selected_Lep );
				weight = weight * GetLepSF_TH2F( &leps, h_MuISOSF, idx_Selected_Lep );
				weight = weight * GetLepSF_TH2F( &leps, h_MuTrgSF, idx_Selected_Lep );
				}
			else if(idx_Selected_Lep == idx_Selected_El){
				weight = weight * GetLepSF_TH2F( &leps, h_tightElIDSF, idx_Selected_Lep );
				weight = weight * GetLepSF_TH2F( &leps, h_ElRECOSF, idx_Selected_Lep );	
				weight = weight * GetLepSF_TH2D( &leps, h_ElTrgSF, idx_Selected_Lep );
			}
					
			//do the gen-weight (second linear order calibration)
			weight *= Get_GenMinus_Weight( gen );

			//do the selection for mvamgr
        
			mvamgr.clean();
        	mvamgr.Get_lep_tagged_flavor( channel );
        	mvamgr.Get_selected_info( sel_jets, sel_b_jets, idx_Selected_Lep );
        	int cor_b = -1, cor_j1 = -1, cor_j2 = -1;
        	bool is_good_trained_evt = mvamgr.Find_Correct_HadronicTop( cor_b, cor_j1, cor_j2 );

			if( !is_good_trained_evt )
			{	continue;	}
            
			//assign info to new branch

			for(int i=0;i<(int)sel_jets.size();i++)
			{
				sel_jets_idx.push_back( sel_jets.at(i) );
			}

			for(int i=0;i<(int)sel_b_jets.size();i++)
			{
				sel_b_jets_idx.push_back( sel_b_jets.at(i) );
			}

			sel_lep_idx = idx_Selected_Lep;
			had_b = cor_b;	had_j1 = cor_j1;	had_j2 = cor_j2;
			evt_weight = weight;

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
        }       //end of for-loop
		//f_new->Map();
		f_new->Close();
    }   //end of while loop
    


	printf("\nThe original total events No.: %d",total_entries);
	printf("\nThe survived events No. after selection : %d",pre_sel_No);

}
