/**************************************************************************************
 *
 *	File Name :	sel2.cc 
 *	Description : to test the new type to do selection
 *		Date : 
 *
 *	Author : Chen-Yu Chuang
 *
 *
 ****************************************************************************************/
#include "TopCPViolation/selected/interface/selection_PUIDBtag_weight.h"
#include "TopCPViolation/selected/interface/BranchInfo.h"
//#include "TopCPViolation/loop_and_test/interface/lep_sel_test.h"
//#include "TopCPViolation/loop_and_test/interface/jet_sel_test.h"
#include "TopCPViolation/selected/interface/jet_sel.h"
#include "TopCPViolation/selected/interface/lep_sel.h"
#include "TopCPViolation/selected/interface/GetInfo.h"
#include "TopCPViolation/selected/interface/reweightMC.h"
#include "TopCPViolation/selected/interface/information.h"
#include "TopCPViolation/selected/interface/File.h"
#include "TopCPViolation/selected/interface/Plotter.h"
#include <iostream>
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include <vector>
#include <map>
#include <string>
#include "string.h"

#include "CondFormats/BTauObjects/interface/BTagCalibration.h"
#include "CondTools/BTau/interface/BTagCalibrationReader.h"
#include "CondFormats/BTauObjects/interface/BTagEntry.h"


using namespace std;

int main(int argc,char* argv[])
{
	gen_no_TT.push_back(28195200);

	gen_no_WJets.push_back(10210158);
	gen_no_WJets.push_back(13804318);
	gen_no_WJets.push_back(1963464);
	gen_no_WJets.push_back(12420040);
	
	Xs_TT.assign(1,831.76);
	Xs_WJets.push_back(1345.7*1.21);
	Xs_WJets.push_back(359.7*1.21);
	Xs_WJets.push_back(48.91*1.21);

	double _lumi = 35900.;

	w_TT.push_back( _lumi*Xs_TT[0]/(double)gen_no_TT[0] );

	for(int i=0;i<(int)WJets.size();i++){
		w_WJets.push_back( _lumi*Xs_WJets[i]/(double)gen_no_WJets[i] );
	}

	char tt1[500] = "/wk_cms2/cychuang/pre_sel_file/pre_sel_TT/dir_002/pre_sel_TT_1.root";
	TT.push_back(tt1);

	char prewj1[500] = "/wk_cms2/cychuang/pre_sel_file/WJetsToLNu_HT/BPK_80X_WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_1.root";
	char prewj2[500] = "/wk_cms2/cychuang/pre_sel_file/WJetsToLNu_HT/BPK_80X_WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_1.root";
	char prewj3[500] = "/wk_cms2/cychuang/pre_sel_file/WJetsToLNu_HT/BPK_80X_WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_1.root";
	char prewj4[500] = "/wk_cms2/cychuang/pre_sel_file/WJetsToLNu_HT/BPK_80X_WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_1.root";
	
	WJets.push_back(prewj1);
	WJets.push_back(prewj2);
	WJets.push_back(prewj3);
	WJets.push_back(prewj4);
	
	char d1[500] = "/wk_cms2/cychuang/pre_sel_file/pre_sel_TT/dir_002/pre_sel_TT_2.root" ;	
	
	
	vector<File> files_TT;
	File f_TT(TT[0]);
	files_TT.push_back(f_TT);

	vector<File> files_WJets;
	for(int i=0;i<(int)WJets.size();i++){
		File f_WJets(WJets[i]);
		files_WJets.push_back(f_WJets);
	}
	vector<File> files_Data;
	File f_Data(d1);
	files_Data.push_back(f_Data);



	//insert pileup re-weighting information
	vector<double> pileupinfo;
	pileupinfo.assign(76,0.);
	int PUI_size = 76;
	char path_PU_file[200] = "/wk_cms2/cychuang/CMSSW_9_2_3/src/Selected/data/pileupweights_69200.csv"; 
	GetPileUpInfo( path_PU_file , pileupinfo , &PUI_size);

	//insert tight-Muon ID Scale Factors' TH2F*
	TFile* f1 = new TFile("/wk_cms2/cychuang/CMSSW_9_2_3/src/Selected/data/muIDSF.root");
	TH2F* h_tightMuIDSF;
	f1->GetObject("abseta_pt_ratio",h_tightMuIDSF);

	//insert tight-Electron ID Scale Factor TH2F*
	TFile* f2 = new TFile("/wk_cms2/cychuang/CMSSW_9_2_3/src/Selected/data/elTightIDSF.root");
	TH2F* h_tightElIDSF;
	f2->GetObject("EGamma_SF2D",h_tightElIDSF);

	//we need to delete the f1 and f2 after finising using the objects from them
	//so we delete them after selecting
	
	//initial a map to doing the Btagging-reweighting
	std::map<BTagEntry::OperatingPoint,BTagCalibrationReader> btag_map;
	Initial_Btag_Weight(btag_map,"bcheck","/wk_cms2/cychuang/CMSSW_9_2_3/src/Selected/data/CSVv2_Moriond17_B_H.csv");






	//string data_sets_name[7] = {"TT","DY","WJets","VV","ST","QCD","Data"};
	string data_sets_name[3] = {"TT","WJets","Data"};
	
	map< string, vector<File>* > files_map{
		{data_sets_name[0],&files_TT},
		{data_sets_name[1],&files_WJets},
		{data_sets_name[2],&files_Data}
	};

	//files_map[data_sets_name[0]] = &files_TT;		
	//files_map[data_sets_name[1]] = &files_WJets;
	//files_map[data_sets_name[2]] = &files_Data;
	

	map< string, vector<double>* > Weights_map;
	Weights_map[data_sets_name[0]] = &w_TT;				
	Weights_map[data_sets_name[1]] = &w_WJets;		

	
	int bins_No = 45;
	double hist_min = 0.;
	double hist_max = 600.;
	
	TH1F* h_TT_mu = new TH1F("h_TT_mu","",bins_No,hist_min,hist_max);
	TH1F* h_TT_el = new TH1F("h_TT_el","",bins_No,hist_min,hist_max);
	
	TH1F* h_WJets_mu = new TH1F("h_WJets_mu","",bins_No,hist_min,hist_max);
	TH1F* h_WJets_el = new TH1F("h_WJets_el","",bins_No,hist_min,hist_max);
	
	TH1F* h_Data_mu = new TH1F("h_Data_mu"," ;hadronic top mass(GeV);Events(No.)",bins_No,hist_min,hist_max);	
	//"(primary title);(X-Axis title);(Y-Axis Title)"
	TH1F* h_Data_el = new TH1F("h_Data_el"," ;hadronic top mass(GeV);Events(No.)",bins_No,hist_min,hist_max);


	vector<TH1F*> h_mu;
	h_mu.push_back(h_TT_mu);		
	h_mu.push_back(h_WJets_mu);	
	h_mu.push_back(h_Data_mu);

	vector<TH1F*> h_el;
	h_el.push_back(h_TT_el);		
	h_el.push_back(h_WJets_el);	
	h_el.push_back(h_Data_el);

	//loop the kinds of data set(k-loop)	
	//int data_sets_name_len = sizeof(data_sets_name)/sizeof(data_sets_name[0]);

	//for(int k=0;k<(int)files_map.size();k++)
	for(int k=0;k<3;k++)
	{
		double weight = 1.;
		bool is_data = false;

		string Set_name = data_sets_name[k];

		if(Set_name == "Data")
		{ is_data = true; }

		printf("\nBefore selection, the file now dealing with is under the sets %s .",Set_name.c_str());
		
		//loop around in a kind of data set that is files_map[ data_sets_name[k] ] (r-loop)
		for(int r=0;r<(int)files_map[ Set_name ]->size();r++)
		{
			double lumi_weight = Weights_map[ Set_name ]->at(r);

			printf("\nBefore register, the file now dealing with is under the sets %s the %d one.",Set_name.c_str(),r+1);
			EvtInfo evtInfo ;
			evtInfo.Register( files_map[ Set_name ]->at(r).ch );
			JetInfo jetInfo ;
			jetInfo.Register( files_map[ Set_name ]->at(r).ch , "JetInfo" );
			LeptonInfo leptonInfo;
			leptonInfo.Register( files_map[ Set_name ]->at(r).ch , "LepInfo" );
			GenInfo genInfo;
			genInfo.Register( files_map[ Set_name ]->at(r).ch );	

			int t_entries = (files_map[ Set_name ]->at(r).ch)->GetEntries();
			printf("\nAfter register, the file now dealing with is under the sets %s the %d one.",Set_name.c_str(),r+1);
			printf("\nAnd the Entries of this data files are : %d",t_entries);

			for(int entry=0;entry<(int)t_entries;++entry)
			{
				( files_map[ Set_name ]->at(r).ch )->GetEntry(entry);

				//Set lumi_Weight first
				weight = 1.;
				if( Set_name != "Data" )
				{
					weight = lumi_weight; 		//first input lumi-weight
				}

				//pile-up reweigh MC

				
				bool pass_PU = true;
				if(!is_data){
					weight = weight * GetPileUpWeight(&evtInfo,pileupinfo,&pass_PU);
				}
				else{	weight = 1.;	}
				if(!pass_PU)
				{	continue;	}
        		

				//initialize some storage of selected-particles' indices
				int idx_Selected_Mu = -1;
				int idx_Selected_El = -1;
				
				int sel_jets_no = 128;	
				int sel_jets[sel_jets_no];
				for(int i=0;i<sel_jets_no;i++)
				{ sel_jets[i] = 0; }
			
				int sel_b_jets[2] = {0,0};		//in this channel we just need 2 b-jets
			

				//Do the lepton Selected first here
				bool pass_sel_mu = Pass_Selected_Muon(leptonInfo,&idx_Selected_Mu);
				bool pass_veto_mu = Pass_Veto_Muon(leptonInfo,idx_Selected_Mu);
				bool pass_sel_el = Pass_Selected_Electron(leptonInfo,&idx_Selected_El);
				bool pass_veto_el = Pass_Veto_Electron(leptonInfo,idx_Selected_El);
		
				bool is_mu_channel = pass_sel_mu && pass_veto_mu && pass_veto_el ;
				bool is_el_channel = pass_sel_el && pass_veto_el && pass_veto_mu ;

				//To assign Muon/Electron channel
				
				int idx_Selected_Lep = -1;
				if(is_mu_channel&&!is_el_channel)
				{
					idx_Selected_Lep = idx_Selected_Mu;
				}
				else if(is_el_channel&&!is_mu_channel)
				{
					idx_Selected_Lep = idx_Selected_El;
				}
				else
				{	continue;	}
		
				//Then ,do the jet-selected here
				bool pass_sel_jet = Pass_Selected_Jet(jetInfo,sel_jets,&sel_jets_no);
		
		
				if( (is_mu_channel && !is_el_channel) || (is_el_channel && !is_mu_channel) )
				{
				
					if( !pass_sel_jet )
					{	continue;	}

					//selected-jets and selected-lepton distinguishable (delta R)
					bool pass_delR = Pass_delta_R(leptonInfo,idx_Selected_Lep,jetInfo,sel_jets,&sel_jets_no);
					if( !pass_delR )
					{	continue;	}

					//Do the b-jets-selected
					bool pass_sel_b = Pass_Selected_b_Jet(jetInfo,sel_jets,&sel_jets_no,sel_b_jets);
			
					if( !pass_sel_b )	//2 b-jets
					{	continue;	}
				
					//Do other reweight to MC
					
					if(!is_data)
					{
						//do the b-tag reweighting scale factor
						int b_1_idx = sel_b_jets[0];	int b_2_idx = sel_b_jets[1];
	
						double b_reweight_factor_1 = Get_BtagWeight_Factor(btag_map,jetInfo,BTagEntry::OP_MEDIUM ,b_1_idx);
						double b_reweight_factor_2 = Get_BtagWeight_Factor(btag_map,jetInfo,BTagEntry::OP_MEDIUM ,b_2_idx);

						weight = weight * b_reweight_factor_1 * b_reweight_factor_2;

						//do the tight-Muon ID scale factor to make MC more match to data
						if(idx_Selected_Lep == idx_Selected_Mu){
							weight = weight * GetLepSF(&leptonInfo,h_tightMuIDSF,idx_Selected_Lep);
						}
						else if(idx_Selected_Lep == idx_Selected_El){
							weight = weight * GetLepSF(&leptonInfo,h_tightElIDSF,idx_Selected_Lep);
						}
					
						//do the gen-weight (second linear order calibration)
						weight *= Get_GenMinus_Weight(genInfo);
					}
					

					//use chi2-Sorting to choose best choice of reconstruct top quark's mass 
					double sel_t_mass = Chi_Square_Sorting(jetInfo,sel_jets,&sel_jets_no,sel_b_jets,2);	
					
					//printf("\n%.10f",sel_t_mass);

					if(idx_Selected_Lep == idx_Selected_Mu)
					{	h_mu.at(k)->Fill(sel_t_mass,weight);	}
					else if(idx_Selected_Lep == idx_Selected_El)
					{	h_el.at(k)->Fill(sel_t_mass,weight);	}	

				}
				else
				{	continue;	}
				//Do something with the two b-jets and seperate them
				
				//just for check
				if(entry == 50)
				{	break;	}

			}	//end of entry for-loop
	
			printf("\nAfter selection, the file now dealing with is under the sets %s the %d one.",Set_name.c_str(),r+1);

		}	//end of r for-loop
	}		//end of k for-loop


	delete f1;
	delete f2;
/*
	int v_size = (int)Weights_map.size();

	vector<TH1F*> h_MCs_mu(v_size);
	h_MCs_mu.push_back(h_TT_mu);	
	h_MCs_mu.push_back(h_WJets_mu);	

	vector<TH1F*> h_MCs_el(v_size);
	h_MCs_el.push_back(h_TT_el);	
	h_MCs_el.push_back(h_WJets_el);		

	vector<string> hist_name((int)files_map.size());
	hist_name.push_back("Data");		hist_name.push_back("t#bar{t}");
	hist_name.push_back("W+Jets");

	
	//Plot!!
	vector<string> text_mu(2);
	text_mu.push_back( "Muon channel" );
	text_mu.push_back( "Luminosity : 35.9fb^{-1}" );

	vector<string> text_el(2);
	text_el.push_back( "Electron channel" );
	text_el.push_back( "Luminosity : 35.9fb^{-1}" );


	string file_name_ps = "SEL2";
	if ( argc == 2 ){	
		file_name_ps = std::string( argv[1] );
	}

	string file_name_mu = file_name_ps + "Muon_Channel";
	string file_name_el = file_name_ps + "Electron_Channel";

	Plot_MCData(h_mu.at(5),h_MCs_mu,file_name_mu,hist_name,text_mu);

	Plot_MCData(h_el.at(5),h_MCs_el,file_name_el,hist_name,text_el);
	*/

	delete h_TT_mu;
	delete h_WJets_mu;
	
	delete h_TT_el;
	delete h_WJets_el;
	
	delete h_Data_mu;
	delete h_Data_el;

}

