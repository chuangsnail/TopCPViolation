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
#include <iterator>

#include "CondFormats/BTauObjects/interface/BTagCalibration.h"
#include "CondTools/BTau/interface/BTagCalibrationReader.h"
#include "CondFormats/BTauObjects/interface/BTagEntry.h"


using namespace std;

int main(int argc,char* argv[])
{

	Data_SE.push_back(SE_2016B);
	Data_SE.push_back(SE_2016C);
	Data_SE.push_back(SE_2016D);
	Data_SE.push_back(SE_2016E);
	Data_SE.push_back(SE_2016F);
	Data_SE.push_back(SE_2016G);
	Data_SE.push_back(SE_2016H_v2);
	Data_SE.push_back(SE_2016H_v3);

	Data_SM.push_back(SM_2016B);
	Data_SM.push_back(SM_2016C);
	Data_SM.push_back(SM_2016D);
	Data_SM.push_back(SM_2016E);
	Data_SM.push_back(SM_2016F);
	Data_SM.push_back(SM_2016G);
	Data_SM.push_back(SM_2016H_v2);
	Data_SM.push_back(SM_2016H_v3);

	TT.push_back(pre_TT_001);
	TT.push_back(pre_TT_002);
	TT.push_back(pre_TT_003);

	DY.push_back(pre_DY_100_200);
	DY.push_back(pre_DY_200_400);
	DY.push_back(pre_DY_400_600);
	DY.push_back(pre_DY_600_800);
	DY.push_back(pre_DY_800_1200);
	DY.push_back(pre_DY_1200_2500);
	DY.push_back(pre_DY_2500_Inf);

	WJets.push_back(pre_WJets_100_200);
	WJets.push_back(pre_WJets_200_400);
	WJets.push_back(pre_WJets_400_600);
	WJets.push_back(pre_WJets_600_800);
	WJets.push_back(pre_WJets_800_1200);
	WJets.push_back(pre_WJets_1200_2500);
	WJets.push_back(pre_WJets_2500_Inf);

	VV.push_back(pre_VV_WW);
	VV.push_back(pre_VV_WZ);
	VV.push_back(pre_VV_ZZ);

	ST.push_back(pre_ST_s);
	ST.push_back(pre_ST_t_antitop);
	ST.push_back(pre_ST_t_top);
	ST.push_back(pre_ST_tW_antitop);
	ST.push_back(pre_ST_tW_top);
	
	/*
	QCD.push_back(pre_QCD_100_200);
	QCD.push_back(pre_QCD_200_300);
	QCD.push_back(pre_QCD_300_500);
	QCD.push_back(pre_QCD_500_700);
	QCD.push_back(pre_QCD_700_1000);
	QCD.push_back(pre_QCD_1000_1500);
	QCD.push_back(pre_QCD_1500_2000);
	QCD.push_back(pre_QCD_2000_Inf);
	*/

	gen_no_TT.push_back(28143936);
	gen_no_TT.push_back(28195200);
	gen_no_TT.push_back(20018717);

	gen_no_DY.push_back(7145468);
	gen_no_DY.push_back(5093947);
	gen_no_DY.push_back(1265157);
	gen_no_DY.push_back(8292957);
	gen_no_DY.push_back(2668730);
	gen_no_DY.push_back(596079);
	gen_no_DY.push_back(399492);

	gen_no_VV.push_back(994012);
	gen_no_VV.push_back(1000000);
	gen_no_VV.push_back(843120);

	gen_no_ST.push_back(1000000);
	gen_no_ST.push_back(38780433);
	gen_no_ST.push_back(66437948);
	gen_no_ST.push_back(6710218);
	gen_no_ST.push_back(6924540);

	gen_no_WJets.push_back(10210158);
	gen_no_WJets.push_back(13804318);
	gen_no_WJets.push_back(1963464);
	gen_no_WJets.push_back(12420040);
	gen_no_WJets.push_back(6200954);
	gen_no_WJets.push_back(244532);
	gen_no_WJets.push_back(253561);
	
	/*
	gen_no_QCD.push_back(80365476);
	gen_no_QCD.push_back(38857977);
	gen_no_QCD.push_back(37502012);
	gen_no_QCD.push_back(42190760);
	gen_no_QCD.push_back(15629253);
	gen_no_QCD.push_back(4767100);
	gen_no_QCD.push_back(7855883);
	gen_no_QCD.push_back(3839969);
	*/

	//calculate and store the result of Cross Sections
	Xs_TT.assign(3,831.76);

	//Xs_DY.push_back(169.9*1.23);	//70-100
	Xs_DY.push_back(147.4*1.23);	//100-200
	Xs_DY.push_back(40.99*1.23);	//200-400
	Xs_DY.push_back(5.678*1.23);	//400-600
	Xs_DY.push_back(1.367*1.23);	//600-800
	Xs_DY.push_back(0.6304*1.23);	//800-1200
	Xs_DY.push_back(0.1514*1.23);	//1200-2500
	Xs_DY.push_back(0.003565*1.23);	//2500-Inf

	Xs_WJets.push_back(1345.7*1.21);
	Xs_WJets.push_back(359.7*1.21);
	Xs_WJets.push_back(48.91*1.21);
	Xs_WJets.push_back(12.05*1.21);
	Xs_WJets.push_back(5.501*1.21);
	Xs_WJets.push_back(1.329*1.21);
	Xs_WJets.push_back(0.03216*1.21);

	Xs_VV.push_back(118.7);
	Xs_VV.push_back(47.13);
	Xs_VV.push_back(16.52);

	Xs_ST.push_back(3.36);	//s
	Xs_ST.push_back(80.95);	//t anti-top
	Xs_ST.push_back(136.02);//t top
	Xs_ST.push_back(35.6);	//s anti-top
	Xs_ST.push_back(35.6);	//s top
	/*
	Xs_QCD.push_back(27990000);		//100-200
	Xs_QCD.push_back(1712000);		//200-300
	Xs_QCD.push_back(347700);		//300-500
	Xs_QCD.push_back(32100);		//500-700
	Xs_QCD.push_back(6831);			//700-1000
	Xs_QCD.push_back(1207);			//1000-1500
	Xs_QCD.push_back(119.9);		//1500-2000
	Xs_QCD.push_back(25.24);		//2000-Inf
	*/
	//1pb = 1000fb
	//35.9fb^-1 = 35900pb^-1
	
	//To calculate the lumi-weight of any simulated data
	
	double _lumi = 35900.;

	//w_TT.push_back( _lumi*Xs_TT[0]/(double)gen_no_TT[0] );
	w_TT.push_back( _lumi*Xs_TT[1]/(double)gen_no_TT[1] );
	//w_TT.push_back( _lumi*Xs_TT[2]/(double)gen_no_TT[2] );

	for(int i=0;i<(int)DY.size();i++){
		w_DY.push_back( _lumi*Xs_DY[i]/(double)gen_no_DY[i] );
	}
	for(int i=0;i<(int)WJets.size();i++){
		w_WJets.push_back( _lumi*Xs_WJets[i]/(double)gen_no_WJets[i] );
	}	
	for(int i=0;i<(int)VV.size();i++){
		w_VV.push_back( _lumi*Xs_VV[i]/(double)gen_no_VV[i] );
	}
	for(int i=0;i<(int)ST.size();i++){
		w_ST.push_back( _lumi*Xs_ST[i]/(double)gen_no_ST[i] );
	}
	
	/*
	w_QCD.push_back( 35900.*Xs_QCD[0]/(double)gen_no_QCD[0] );
	w_QCD.push_back( 35900.*Xs_QCD[1]/(double)gen_no_QCD[1] );
	w_QCD.push_back( 35900.*Xs_QCD[2]/(double)gen_no_QCD[2] );
	w_QCD.push_back( 35900.*Xs_QCD[3]/(double)gen_no_QCD[3] );
	w_QCD.push_back( 35900.*Xs_QCD[4]/(double)gen_no_QCD[4] );
	w_QCD.push_back( 35900.*Xs_QCD[5]/(double)gen_no_QCD[5] );
	w_QCD.push_back( 35900.*Xs_QCD[6]/(double)gen_no_QCD[6] );
	w_QCD.push_back( 35900.*Xs_QCD[7]/(double)gen_no_QCD[7] );
	*/



	//make flags
	//bool is_data = true;
	//bool is_MC = false;

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
	

	//**************Make some object (histograms or vector ......etc.)***************//

	int bins_No = 45;
	double hist_min = 0.;
	double hist_max = 600.;
	//double bins_weight = 1./( (hist_max-hist_min)/(double)bins_no );

	TH1F* h_TT_mu = new TH1F("h_TT_mu","",bins_No,hist_min,hist_max);
	TH1F* h_TT_el = new TH1F("h_TT_el","",bins_No,hist_min,hist_max);
	
	TH1F* h_DY_mu = new TH1F("h_DY_mu","",bins_No,hist_min,hist_max);
	TH1F* h_DY_el = new TH1F("h_DY_el","",bins_No,hist_min,hist_max);

	TH1F* h_WJets_mu = new TH1F("h_WJets_mu","",bins_No,hist_min,hist_max);
	TH1F* h_WJets_el = new TH1F("h_WJets_el","",bins_No,hist_min,hist_max);
	
	TH1F* h_VV_mu = new TH1F("h_VV_mu","",bins_No,hist_min,hist_max);
	TH1F* h_VV_el = new TH1F("h_VV_el","",bins_No,hist_min,hist_max);

	TH1F* h_ST_mu = new TH1F("h_ST_mu","",bins_No,hist_min,hist_max);
	TH1F* h_ST_el = new TH1F("h_ST_el","",bins_No,hist_min,hist_max);
	
	//TH1F* h_QCD_mu = new TH1F("h_QCD_mu","",bins_No,hist_min,hist_max);
	//TH1F* h_QCD_el = new TH1F("h_QCD_el","",bins_No,hist_min,hist_max);
	
	TH1F* h_Data_mu = new TH1F("h_Data_mu"," ;hadronic top mass(GeV);Events(No.)",bins_No,hist_min,hist_max);	
	//"(primary title);(X-Axis title);(Y-Axis Title)"
	TH1F* h_Data_el = new TH1F("h_Data_el"," ;hadronic top mass(GeV);Events(No.)",bins_No,hist_min,hist_max);
	
	vector<TH1F*> h_mu;
	h_mu.push_back(h_TT_mu);		h_mu.push_back(h_DY_mu);
	h_mu.push_back(h_WJets_mu);		h_mu.push_back(h_VV_mu);
	h_mu.push_back(h_ST_mu);		//h_mu.push_back(h_QCD_mu);
	h_mu.push_back(h_Data_mu);

	vector<TH1F*> h_el;
	h_el.push_back(h_TT_el);		h_el.push_back(h_DY_el);
	h_el.push_back(h_WJets_el);		h_el.push_back(h_VV_el);
	h_el.push_back(h_ST_el);		//h_el.push_back(h_QCD_el);
	h_el.push_back(h_Data_el);
	

	map< string , vector<File>* > files_map;
	
	vector<File> files_TT;			vector<File> files_DY;
	vector<File> files_WJets;		vector<File> files_VV;
	vector<File> files_ST;			//vector<File> files_QCD;

	File f_TT_002(pre_TT_002);
	files_TT.push_back(f_TT_002);

	for(int i=0;i<(int)DY.size();i++){
		File f_DY(DY[i]);
		files_DY.push_back(f_DY);
	}

	for(int i=0;i<(int)WJets.size();i++){
		File f_WJets(WJets[i]);
		files_WJets.push_back(f_WJets);
	}

	for(int i=0;i<(int)VV.size();i++){
		File f_VV(VV[i]);
		files_VV.push_back(f_VV);
	}

	for(int i=0;i<(int)ST.size();i++){
		File f_ST(ST[i]);
		files_ST.push_back(f_ST);
	}	
/*
	for(int i=0;i<(int)QCD.size();i++){
		File f_QCD(QCD[i]);
		files_QCD.push_back(f_QCD);
	}
*/
	//string data_sets_name[7] = {"TT","DY","WJets","VV","ST","QCD","Data"};
	string data_sets_name[6] = {"TT","DY","WJets","VV","ST","Data"};
	
	files_map[data_sets_name[0]] = &files_TT;		files_map[data_sets_name[1]] = &files_DY;
	files_map[data_sets_name[2]] = &files_WJets;	files_map[data_sets_name[3]] = &files_VV;
	files_map[data_sets_name[4]] = &files_ST;		//files_map[data_sets_name[5]] = &files_QCD;
	

	map< string, vector<double>* > Weights_map;
	Weights_map[data_sets_name[0]] = &w_TT;			Weights_map[data_sets_name[1]] = &w_DY;	
	Weights_map[data_sets_name[2]] = &w_WJets;		Weights_map[data_sets_name[3]] = &w_VV;
	Weights_map[data_sets_name[4]] = &w_ST;			//Weights_map[data_sets_name[5]] = &w_QCD;


	//**************Real Data Analysis!**************//

	vector<File> files_Data;
	
	File f_Data_1(Data_SM[0]);
	for(int i=1;i<(int)Data_SM.size();i++){
		f_Data_1.AddFile(Data_SM[i]);
	}
	File f_Data_2(Data_SE[0]);
	for(int i=1;i<(int)Data_SE.size();i++){
		f_Data_2.AddFile(Data_SE[i]);
	}
	
	files_Data.push_back(f_Data_1);		files_Data.push_back(f_Data_2);

	//files_map[data_sets_name[6]] = &files_Data;
	files_map[data_sets_name[5]] = &files_Data;
	
	//loop the kinds of data set(k-loop)	
	//int data_sets_name_len = sizeof(data_sets_name)/sizeof(data_sets_name[0]);
	for(int k=0;k<(int)files_map.size();k++)
	{
		double weight = 1.;
		bool is_data = false;

		string Set_name = data_sets_name[k];

		if(Set_name == "Data")
		{ is_data = true; }

		
		//loop around in a kind of data set that is files_map[ data_sets_name[k] ] (r-loop)
		for(int r=0;r<(int)files_map[ Set_name ]->size();r++)
		{
			//Get lumi_weight
			printf("\ntest1");
			double lumi_weight = 1.;
			if(!is_data)
			{
				printf("\ntest2-1 MC");
				lumi_weight = Weights_map[ Set_name ]->at(r);
			}
			else
			{
				printf("\ntest2-2 real data");
				lumi_weight = 1.;
			}

			//Register branches

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

				//just for check
				if(entry >= 1)
				{	break;	}
				
				printf("\ntest3");
				( files_map[ Set_name ]->at(r).ch )->GetEntry(entry);
				
				printf("\ntest3-1");
				//Set lumi_Weight first
				weight = 1.;
				if( Set_name != "Data" )
				{
					weight = lumi_weight; 		//first input lumi-weight
				}

				printf("\ntest3-2");
				//pile-up reweigh MC

				
				bool pass_PU = true;
				if(!is_data){
					weight = weight * GetPileUpWeight(&evtInfo,pileupinfo,&pass_PU);
				}
				else{	weight = 1.;	}
				if(!pass_PU)
				{	continue;	}
        		
				printf("\ntest3-3");

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
					
					if(idx_Selected_Lep == idx_Selected_Mu)
					{	h_mu.at(k)->Fill(sel_t_mass,weight);	}
					else if(idx_Selected_Lep == idx_Selected_El)
					{	h_el.at(k)->Fill(sel_t_mass,weight);	}
				
				}
				else
				{	continue;	}
				//Do something with the two b-jets and seperate them
				

			}	//end of entry for-loop
	

		}	//end of r for-loop
	}		//end of k for-loop



	//int v_size = (int)Weights_map.size();

	
	vector<TH1F*> h_MCs_mu;
	h_MCs_mu.push_back(h_TT_mu);		h_MCs_mu.push_back(h_DY_mu);
	h_MCs_mu.push_back(h_WJets_mu);		h_MCs_mu.push_back(h_VV_mu);
	h_MCs_mu.push_back(h_ST_mu);		//h_MCs_mu.push_back(h_QCD_mu);

	vector<TH1F*> h_MCs_el;
	h_MCs_el.push_back(h_TT_el);		h_MCs_el.push_back(h_DY_el);
	h_MCs_el.push_back(h_WJets_el);		h_MCs_el.push_back(h_VV_el);
	h_MCs_el.push_back(h_ST_el);		//h_MCs_el.push_back(h_QCD_el);

	vector<string> hist_name;
	hist_name.push_back("Data");		hist_name.push_back("t#bar{t}");
	hist_name.push_back("Drell-Yan");	hist_name.push_back("W+Jets");
	hist_name.push_back("VV(WW,WZ,ZZ)");hist_name.push_back("Single top");

	
	//Plot!!
	vector<string> text_mu;
	text_mu.push_back( "Muon channel" );
	text_mu.push_back( "Luminosity : 35.9fb^{-1}" );

	vector<string> text_el;
	text_el.push_back( "Electron channel" );
	text_el.push_back( "Luminosity : 35.9fb^{-1}" );


	string file_name_ps = "SEL2";
	
	/*
	if ( argc == 2 ){	
		file_name_ps = std::string( argv[1] );
	}
	*/

	string file_name_mu = file_name_ps + "Muon_Channel";
	string file_name_el = file_name_ps + "Electron_Channel";

	Plot_MCData(h_mu.at(5),h_MCs_mu,file_name_mu,hist_name,text_mu);

	Plot_MCData(h_el.at(5),h_MCs_el,file_name_el,hist_name,text_el);

	

	delete h_TT_mu;
	delete h_DY_mu;
	delete h_WJets_mu;
	delete h_VV_mu;
	delete h_ST_mu;
	//delete h_QCD_mu;
	
	delete h_TT_el;
	delete h_DY_el;
	delete h_WJets_el;
	delete h_VV_el;
	delete h_ST_el;
	//delete h_QCD_el;
	
	delete h_Data_mu;
	delete h_Data_el;

	delete f1;
	delete f2;
}

