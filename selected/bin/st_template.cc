/**************************************************************************************
 *
 *	File Name : st_template.cc
 *	Description : add new variable for mva
 *				  use new selected sample (with the GenMgr.h file instead of prepare_mva.h)! 
 *		{deepCSV}
 *	Date : 191105~
 *	Author : Chen-Yu Chuang
 *
 *	exe {old_file_name with path without .root} {just new_file_name with path without .root } {option}
 *	{option} == 0 -> test
 *	{option} == 1 -> Run all
 *
 ****************************************************************************************/
#include "TopCPViolation/selected/interface/Comp_DataMC.h"
#include "TopCPViolation/selected/interface/GenMgr.h"
#include "TopCPViolation/selected/interface/kinematic_tool.h"
#include "TopCPViolation/selected/interface/MVAvar.h"

#include <cmath>
#include "TLorentzVector.h"

using namespace std;
using namespace mvatool;

int main(int argc,char* argv[])
{
	//*********Offline High level Trigger Dealing**********//
	
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

	//**********************Start Analysis***********************//

	//Register branches
	
	TChain* root_old = new TChain( "root" );

	char point_root[10] = ".root";
	char pre_name[500] = "";
	strcpy( pre_name, argv[1] );
	strcat( pre_name, point_root );

	root_old->Add( pre_name );

	EvtInfo evtInfo ;
	evtInfo.Register( root_old );
	JetInfo jetInfo ;
	jetInfo.Register( root_old , "JetInfo" );
	LeptonInfo leptonInfo;
	leptonInfo.Register( root_old , "LepInfo" );
	GenInfo genInfo;
	genInfo.Register( root_old );		
	TrgInfo trgInfo;
	trgInfo.Register( root_old , "TrgInfo" );
			
	/*
	VertexInfo vertexInfo;
	vertexInfo.Register( files_map[ Set_name ]->at(r).ch , "VertexInfo" );
	RunInfo runInfo;
	runInfo.Register( files_map[ Set_name ]->at(r).ch , "RunInfo" );
	PhotonInfo photonInfo;
	photonInfo.Register( files_map[ Set_name ]->at(r).ch , "PhotonInfo" );
	*/
			
	int t_entries = root_old->GetEntries();
	printf("\nAnd the Entries of this data files are : %d\n",t_entries);


	//open new file and store new things

	string new_file_name = string( argv[2] ) + ".root";
	TFile* f_new = new TFile( new_file_name.c_str(), "recreate" );

    //allocate the memory space for branch
	//Initial the new trees and their branches
    
    TTree* correct = new TTree("correct","");           //for correct combination's info
    TTree* incorrect = new TTree("incorrect","");

	mvatool::StoreMVATree mvatree( correct, incorrect );
	mvatree.InitBranch();

    //
    string channel = "";	//lep is muon or electron

	//prepare btag object
	BtagManager bmgr( &jetInfo );
	bmgr.Register_Init_Maps();
	bmgr.Register_Init_TH2( eff_b, eff_c, eff_l );

    //prepare gen object
    GenMgr genmgr( &genInfo, &jetInfo, &leptonInfo );
		
	double pass_sel = 0.;
	double pass_seen = 0.;
	double pass_sample_sel = 0.;
	
	for(int entry=0;entry<(int)t_entries;++entry)
	{
		//if( entry >= 3000 )
		//{	break;	}

		//cout << "Entry : " << entry << endl;
		root_old->GetEntry(entry);

		double btag_weight = 1.;
		double weight = 1.;

		//pile-up reweigh MC

		bool pass_PU = true;
		weight = weight * GetPileUpWeight( &evtInfo, pileupinfo, &pass_PU );
		
		if(!pass_PU)
		{	continue;	}
        		
		//initialize some storage of selected-particles' indices
		int idx_Selected_Mu = -1;
		int idx_Selected_El = -1;
				
		vector<int> sel_jets;		sel_jets.assign(128,-1);
		vector<int> sel_b_jets;		sel_b_jets.assign(2,-1);
			
		//Do the lepton-selection first here
				
		bool pass_sel_mu = Pass_SR_Selected_Muon(leptonInfo,idx_Selected_Mu);
		bool pass_sel_el = Pass_SR_Selected_Electron(leptonInfo,idx_Selected_El);
		bool pass_veto_el = false;		bool pass_veto_mu = false;

		if( (pass_sel_mu||pass_sel_el) && !(pass_sel_mu&&pass_sel_el) )
		{
			if(pass_sel_mu)
			{
				pass_veto_el = Pass_SR_Veto_Electron(leptonInfo,idx_Selected_Mu);
				pass_veto_mu = Pass_SR_Veto_Muon(leptonInfo,idx_Selected_Mu);
			}
			if(pass_sel_el)
			{
				pass_veto_el = Pass_SR_Veto_Electron(leptonInfo,idx_Selected_El);
				pass_veto_mu = Pass_SR_Veto_Muon(leptonInfo,idx_Selected_El);
			}
		}
		else
		{	continue;	}
				
		bool is_mu_channel = pass_sel_mu && pass_veto_mu && pass_veto_el ;
		bool is_el_channel = pass_sel_el && pass_veto_el && pass_veto_mu ;
				
		//To assign Muon/Electron channel
				
		int idx_Selected_Lep = -1;
		if(is_mu_channel&&!is_el_channel)
		{
			idx_Selected_Lep = idx_Selected_Mu;
			channel = "mu";
		}
		else if(is_el_channel&&!is_mu_channel)
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
				

		//Then ,do the jet-selection here
		bool pass_sel_jet = Pass_SR_Selected_Jets_Case(jetInfo,sel_jets);
		
		if( (is_mu_channel && !is_el_channel) || (is_el_channel && !is_mu_channel) )
		{
			if( !pass_sel_jet )
			{	continue;	}

			//selected-jets and selected-lepton distinguishable (delta R)
			bool pass_delR = Pass_delR_Jet_Lep( jetInfo, leptonInfo, sel_jets, idx_Selected_Lep );
			if( !pass_delR )
			{	continue;	}

			//Do the b-jets-selected
			//in Control region , the selected b jets number is 0 ,and even by loose b-jet criteria
			bool pass_sel_b = Pass_SR_bjets( jetInfo, sel_jets, sel_b_jets );
			if( !pass_sel_b )	
			{	continue;	}

			//Do other reweight to MC
					
			//if(!is_data)
			{
				//do the b-tag reweighting
						
				bmgr.Reset_idx_capacity();
				bmgr.Set_b_tagged_jets_idx( sel_b_jets );
				bmgr.Set_b_ntagged_jets_idx( sel_jets );
				btag_weight = bmgr.Get_Btag_Weight();
				weight *= btag_weight;

				//do the lepton scale factor
				if(idx_Selected_Lep == idx_Selected_Mu){
					weight = weight * GetLepSF_TH2F( &leptonInfo, h_tightMuIDSF, idx_Selected_Lep );
					weight = weight * GetLepSF_TH2F( &leptonInfo, h_MuISOSF, idx_Selected_Lep );
					weight = weight * GetLepSF_TH2F( &leptonInfo, h_MuTrgSF, idx_Selected_Lep );
				}
				else if(idx_Selected_Lep == idx_Selected_El){
					weight = weight * GetLepSF_TH2F( &leptonInfo, h_tightElIDSF, idx_Selected_Lep );
					weight = weight * GetLepSF_TH2F( &leptonInfo, h_ElRECOSF, idx_Selected_Lep );
					weight = weight * GetLepSF_TH2D( &leptonInfo, h_ElTrgSF, idx_Selected_Lep );
				}
					
				//do the gen-weight (second linear order calibration)
				weight *= Get_GenMinus_Weight( genInfo );
			}
		}
		else
		{	continue;	}

		pass_sel += weight ;
       
        //use the sel_jets, sel_b_jets, idx_Selected_Lep to find the correct and incorrect combination
        
        int cor_b = -1, cor_j1 = -1, cor_j2 = -1;
        bool is_good_trained_evt = genmgr.Find_Correct_HadronicTop( cor_b, cor_j1, cor_j2 );

		//for check selected objects

		for(int l=0;l<(int)sel_jets.size();l++)
		{
			if( sel_jets.at(l) == -1 )
			{
				cout << "There is something wrong in jets selection! in entry :" << entry << endl;
				break;
			}
		}
		
		for(int l=0;l<(int)sel_b_jets.size();l++)
		{
			if( sel_b_jets.at(l) == -1 )
			{
				cout << "There is something wrong in b-jets selection! in entry :" << entry << endl;
				break;
			}
		}

		/*
		cout << "cor_b : " << cor_b << endl;
		cout << "cor_j1 : " << cor_j1 << endl;
		cout << "cor_j2 : " << cor_j2 << endl;
		*/

		//if( cor_b == -1 || cor_j1 == -1 || cor_j2 == -1 )
		//{	continue;	}
		//use the events which all pass full selection 

		//pass_seen += weight;

            for(int B=0;B<(int)sel_b_jets.size();++B)
            {
                for(int J1=0;J1<(int)sel_jets.size();++J1)
                {
                    for(int J2=J1+1;J2<(int)sel_jets.size();++J2)
                    {
						int lB = (B == 0) ? 1 : 0 ;
						int tmp_mva_lepb = sel_b_jets.at(lB);
						int tmp_mva_hadb = sel_b_jets.at(B);
						int tmp_mva_j1 = sel_jets.at(J1);
						int tmp_mva_j2 = sel_jets.at(J2);

                        if( ( sel_b_jets.at(B) == cor_b && sel_jets.at(J1) == cor_j1 && sel_jets.at(J2) == cor_j2 ) || ( sel_b_jets.at(B) == cor_b && sel_jets.at(J1) == cor_j2 && sel_jets.at(J2) == cor_j1) )
                        {
							mvatree.c_evt_weight = weight;
							mvatree.SetCorVar( jetInfo, leptonInfo, evtInfo, sel_jets, sel_b_jets, idx_Seleted_Lep, tmp_mva_hadb, tmp_mva_lepb, tmp_mva_j1, tmp_mva_j2 );

							mvatree.correct->Fill();
                        }
                        else	//incorrect combination
                        {
							mvatree.i_evt_weight = weight;
							mvatree.SetIncorVar( jetInfo, leptonInfo, evtInfo, sel_jets, sel_b_jets, idx_Seleted_Lep, tmp_mva_hadb, tmp_mva_lepb, tmp_mva_j1, tmp_mva_j2 );

							mvatree.incorrect->Fill();
                        }
                    }
                }               
            }               
        				
	}	//end of entry for-loop	
			
	(mvatree.correct)->Write();
	(mvatree.incorrect)->Write();

	f_new->Close();

	//*****Drawing Plotting or Outputting files*****//

	string time_str = "";
	time_str = get_time_str( minute );

	cout << "finish time is : " << time_str << endl;
	//cout << "pass_seen/pass_sel : " << (double)pass_seen/pass_sel << endl;

	//*****make space free*****//
	

	delete f7;
	delete f6;
	delete f5;
	delete f4;
	delete f3;
	delete f2;
	delete f1;
}

