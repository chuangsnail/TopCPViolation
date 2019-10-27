/**************************************************************************************
 *
 *	File Name :	bbsep_2D.cc 
 *	Description : 
 *	Making Date : 190602~
 *
 *	Author : Chen-Yu Chuang
 *
 *
 ****************************************************************************************/
#include "TopCPViolation/selected/interface/selection_PUIDBtag_weight.h"
#include "TopCPViolation/selected/interface/BranchInfo.h"
#include "TopCPViolation/selected/interface/jet_sel.h"		//updated! for double Chi2_Sorting(...)
#include "TopCPViolation/selected/interface/lep_sel.h"
#include "TopCPViolation/selected/interface/GetInfo.h"
#include "TopCPViolation/selected/interface/reweightMC.h"
#include "TopCPViolation/selected/interface/information.h"
#include "TopCPViolation/selected/interface/File.h"
#include "TopCPViolation/selected/interface/Plotter.h"
#include "TopCPViolation/selected/interface/MCTruth.h"
#include "TopCPViolation/prepare_info/interface/bbbar.h"

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "string.h"
#include <iterator>

#include "TLorentzVector.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLegend.h"

#include "CondFormats/BTauObjects/interface/BTagCalibration.h"
#include "CondTools/BTau/interface/BTagCalibrationReader.h"
#include "CondFormats/BTauObjects/interface/BTagEntry.h"


using namespace std;

int main(int argc,char* argv[])
{
	//***********To finish something about weight***********//

	TT.push_back(pre_TT_001);
	TT.push_back(pre_TT_002);
	TT.push_back(pre_TT_003);
	
	gen_no_TT.push_back(28143936);
	gen_no_TT.push_back(28195200);
	gen_no_TT.push_back(20018717);

	//calculate and store the result of Cross Sections
	Xs_TT.assign(3,831.76);

	//1pb = 1000fb			//35.9fb^-1 = 35900pb^-1
	//calculate the lumi-weight of any simulated data
	
	double _lumi = 35900.;

	//w_TT.push_back( _lumi*Xs_TT[0]/(double)gen_no_TT[0] );
	w_TT.push_back( _lumi*Xs_TT[1]/(double)gen_no_TT[1] );
	//w_TT.push_back( _lumi*Xs_TT[2]/(double)gen_no_TT[2] );

	//*******To prepare the things about MC reweigh********//

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

	//we need to delete the f1 and f2 after finising using the objects from the		//so we delete them after selecting
	
	//initial a map to doing the Btagging-reweighting
	std::map<BTagEntry::OperatingPoint,BTagCalibrationReader> btag_map;
	Initial_Btag_Weight(btag_map,"bcheck","/wk_cms2/cychuang/CMSSW_9_2_3/src/Selected/data/CSVv2_Moriond17_B_H.csv");
	
	//*****************Make some object (histograms or vector ......etc.)******************//

	int bins_No = 1000;
	double hist_min = 0.;
	double hist_max = 1000.;
	double bins_weight = 1./( (hist_max-hist_min)/(double)bins_No );

	TH1F* h_correct_mu = new TH1F("h_correct_mu"," ;#chi^{2}_{min};PDF",bins_No,hist_min,hist_max);
	TH1F* h_mistag_mu = new TH1F("h_mistag_mu"," ;#chi^{2}_{min};PDF",bins_No,hist_min,hist_max);
	TH1F* h_charge_mis_mu = new TH1F("h_charge_mis_mu"," ;#chi^{2}_{min};PDF",bins_No,hist_min,hist_max); 
	TH1F* h_correct_el = new TH1F("h_correct_el"," ;#chi^{2}_{min};PDF",bins_No,hist_min,hist_max);
	TH1F* h_mistag_el = new TH1F("h_mistag_el"," ;#chi^{2}_{min};PDF",bins_No,hist_min,hist_max);
	TH1F* h_charge_mis_el = new TH1F("h_charge_mis_el"," ;#chi^{2}_{min};PDF",bins_No,hist_min,hist_max); 
	
	TH1F* h_chi2_data_mu = new TH1F("h_chi2_data_mu"," ;#chi^{2}_{min};PDF",bins_No,hist_min,hist_max);	
	TH1F* h_chi2_data_el = new TH1F("h_chi2_data_el"," ;#chi^{2}_{min};PDF",bins_No,hist_min,hist_max);	
	int no_match_number = 0;

	TH2D* h_chi2min_mass_mu = new TH2D("h_chi2min_mass_mu","",50,0.,500.,40,0.,200.);
	TH2D* h_chi2min_mass_el = new TH2D("h_chi2min_mass_el","",50,0.,500.,40,0.,200.);

	map<string,TH1F*> h_correct, h_charge_mis, h_mistag, h_chi2_data;
	map<string,TH2D*> h_chi2min_mass;
	h_correct[ "mu" ] = h_correct_mu;			h_correct[ "el" ] = h_correct_el;
	h_charge_mis[ "mu" ] = h_charge_mis_mu;		h_charge_mis[ "el" ] = h_charge_mis_el;
	h_mistag[ "mu" ] = h_mistag_mu;				h_mistag[ "el" ] = h_mistag_el;
	h_chi2_data[ "mu" ] = h_chi2_data_mu;		h_chi2_data[ "el" ] = h_chi2_data_el;
	h_chi2min_mass[ "mu" ] = h_chi2min_mass_mu;		h_chi2min_mass[ "el" ] = h_chi2min_mass_el;
	map<string,double> cor_t_weight, cmis_t_weight, mist_t_weight, data_t_weight;
	cor_t_weight[ "mu" ] = 0.;		cmis_t_weight[ "mu" ] = 0.;		mist_t_weight[ "mu" ] = 0.;
	cor_t_weight[ "el" ] = 0.;		cmis_t_weight[ "el" ] = 0.;		mist_t_weight[ "el" ] = 0.;
	data_t_weight[ "mu" ] = 0.;		data_t_weight[ "el" ] = 0.;

	//x-axis is mass , y-axis is minimum chi-square
	
	//**********initial the files and TChain and make the file map and weight map***********//

	map< string , vector<File>* > files_map;
	
	vector<File> files_TT;			
	File f_TT_002(pre_TT_002);
	files_TT.push_back(f_TT_002);

	string data_sets_name[6] = {"TT"};
	
	files_map[data_sets_name[0]] = &files_TT;		
	
	map< string, vector<double>* > Weights_map;
	Weights_map[data_sets_name[0]] = &w_TT;				

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
			double lumi_weight = 1.;
			if(!is_data)
			{	lumi_weight = Weights_map[ Set_name ]->at(r);	}
			else
			{	lumi_weight = 1.;	}

			//Register branches

			printf("\n The file now dealing with is under the data sets %s, the %d one.",Set_name.c_str(),r+1);
			EvtInfo evtInfo ;
			evtInfo.Register( files_map[ Set_name ]->at(r).ch );
			JetInfo jetInfo ;
			jetInfo.Register( files_map[ Set_name ]->at(r).ch , "JetInfo" );
			LeptonInfo leptonInfo;
			leptonInfo.Register( files_map[ Set_name ]->at(r).ch , "LepInfo" );
			GenInfo genInfo;
			genInfo.Register( files_map[ Set_name ]->at(r).ch );	

			int t_entries = (files_map[ Set_name ]->at(r).ch)->GetEntries();
			printf("\nAnd the Entries of this data files are : %d",t_entries);
			string channel = "";	//lep is muon or electron

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
					channel = "mu";
				}
				else if(is_el_channel&&!is_mu_channel)
				{
					idx_Selected_Lep = idx_Selected_El;
					channel = "el";
				}
				else
				{	continue;	}
		
				//Then ,do the jet-selected here
				bool pass_sel_jet = Pass_Selected_Jet(jetInfo,sel_jets,&sel_jets_no);

				double chi_square_value = -1.;
				double t_mass = -1., w_mass = -1.;
		
		
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
					chi_square_value = Chi2_Sorting(jetInfo,sel_jets,&sel_jets_no,sel_b_jets,2);	
					//printf("\n%.10f",chi_square_value);

					TLorentzVector j1, j2, b1;
					j1.SetPxPyPzE(jetInfo.Px[sel_jets[0]],jetInfo.Py[sel_jets[0]],jetInfo.Pz[sel_jets[0]],jetInfo.Energy[sel_jets[0]]);
					j2.SetPxPyPzE(jetInfo.Px[sel_jets[1]],jetInfo.Py[sel_jets[1]],jetInfo.Pz[sel_jets[1]],jetInfo.Energy[sel_jets[1]]);
					b1.SetPxPyPzE(jetInfo.Px[sel_b_jets[0]],jetInfo.Py[sel_b_jets[0]],jetInfo.Pz[sel_b_jets[0]],jetInfo.Energy[sel_b_jets[0]]);	
					w_mass = (j1 + j2).M() ;
					t_mass = (j1 + j2 + b1).M() ;
				
					if(!is_data){
						h_chi2min_mass[channel]->Fill( t_mass, chi_square_value , weight );
					}
				}
				else
				{	continue;	}

				//*****Do something with the two b-jets and seperate them*****//

				if(!is_data)
				{
					//***make the corresponding index for this channel's Feynmann diagram***//			
					//idx in JetInfo
					int had_b = sel_b_jets[0];	//which we chose in jjb (reconstructed top quark)	
					int lep_b = sel_b_jets[1];
					//int jet_1 = sel_jets[0];
					//int jet_2 = sel_jets[1];
					int b = -1;
					int bbar = -1;
					int lep = idx_Selected_Lep;	

					//***To decide the b and bbar which is had_b and which is lep_b***//
					/*selected lepton's charge sign is same as the hadronic b's charge sign*/
					/*for EX. if it's anti_muon(plus one charge), then the W in leptonic branch is is W-plus, 
					then the b in leptonic branch is b-quark, so the hadronic b is bbar(plus 1/3 charge)!*/

					double lep_charge = leptonInfo.Charge[ lep ];
					if( lep_charge > 0 )
					{
						b = lep_b;
						bbar = had_b;
					}
					else if( lep_charge < 0 )
					{
						b = had_b;
						bbar = lep_b;
					}
					else
					{	continue;	}

					//***To decide the b and bbar matching type with GenInfo (generation level's information)***//
					bb_matching_type bbType = no_match;			//default
					bbType = Get_bb_Option( b , bbar , lep , jetInfo , leptonInfo , genInfo );
				
					//***To fill the histogram***//
	
					//check
					if( (chi_square_value == -1.) || (t_mass == -1.) || (w_mass == -1.) )
					{	printf("\nThere is no chi2 value/t_mass/w_mass in this selected event! weird!");	continue;	}

					//Fill the histograms
					switch(bbType)
					{
						case correct:
							h_correct[channel]->Fill(chi_square_value,weight);
							cor_t_weight[channel] += weight;
							break;
						case charge_mis:
							h_charge_mis[channel]->Fill(chi_square_value,weight);
							cmis_t_weight[channel] += weight;
							break;
						case mistag:
							h_mistag[channel]->Fill(chi_square_value,weight);
							mist_t_weight[channel] += weight;
							break;
						case no_match:
							no_match_number++;
							break;
						default:
							no_match_number++;
					}
				}	//end of if(!is_data)
				else		//if(is_data)
				{
					h_chi2_data[channel]->Fill(chi_square_value);
					data_t_weight[channel] += 1.;
				}
			}	//end of entry for-loop	
		}	//end of r for-loop
	}		//end of k for-loop

	//*****Drawing Plotting or Outputting files*****//

	//normalize		
	h_correct_mu->Scale(bins_weight);
	h_charge_mis_mu->Scale(bins_weight);
	h_mistag_mu->Scale(bins_weight);
	h_chi2_data_mu->Scale(bins_weight);
	h_correct_el->Scale(bins_weight);
	h_charge_mis_el->Scale(bins_weight);
	h_mistag_el->Scale(bins_weight);
	h_chi2_data_el->Scale(bins_weight);

	h_correct_mu->Scale(1./cor_t_weight["mu"]);
	h_charge_mis_mu->Scale(1./cmis_t_weight["mu"]);
	h_mistag_mu->Scale(1./mist_t_weight["mu"]);
	h_chi2_data_mu->Scale(1./data_t_weight["mu"]);
	h_correct_el->Scale(1./cor_t_weight["el"]);
	h_charge_mis_el->Scale(1./cmis_t_weight["el"]);
	h_mistag_el->Scale(1./mist_t_weight["el"]);
	h_chi2_data_el->Scale(1./data_t_weight["el"]);

	printf("\nMu Correct %.2f",cor_t_weight["mu"]);
	printf("\nMu Charge MisIdetified %.2f",cmis_t_weight["mu"]);
	printf("\nMu Mistag %.2f",mist_t_weight["mu"]);
	//printf("\nMu No_match Case %.2f",);
	printf("\nMu Data %.2f",data_t_weight["mu"]);

	printf("\nEl Correct %.2f",cor_t_weight["el"]);
	printf("\nEl Charge MisIdetified %.2f",cmis_t_weight["el"]);
	printf("\nEl Mistag %.2f",mist_t_weight["el"]);
	printf("\nEl Data %.2f",data_t_weight["el"]);

	//Save these hists to be a root file
	
	TFile* f_out = new TFile("bbsep_tt_1000.root","recreate");

	h_correct_mu->Write();
	h_charge_mis_mu->Write();
	h_mistag_mu->Write();
	h_chi2_data_mu->Write();

	h_correct_el->Write();
	h_charge_mis_el->Write();
	h_mistag_el->Write();
	h_chi2_data_el->Write();

	h_chi2min_mass_mu->Write();
	h_chi2min_mass_el->Write();
	
	f_out->Close();
	delete f_out;

	//*****make space free*****//
	
	delete h_correct_mu;
	delete h_charge_mis_mu;
	delete h_mistag_mu;
	delete h_chi2_data_mu;
	delete h_correct_el;
	delete h_charge_mis_el;
	delete h_mistag_el;
	delete h_chi2_data_el;

	delete h_chi2min_mass_mu;
	delete h_chi2min_mass_el;

	delete f2;
	delete f1;
}

