/**********************************************************************************/
//	This is updated on 190502 by cychuang 
//	It's modified from last time is to update Plotter and Gen-Minus reweighting
/**********************************************************************************/

#include <iostream>
#include <vector>
#include "stdlib.h"
#include "stdio.h"
#include <string>
#include "string.h"
#include <ctime>
#include <map>
#include <iterator>

#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"		//which is in the library "roofit"
#include "TPad.h"			//for gPad
#include "THStack.h"
#include "TColor.h"
#include "TLegend.h"
#include "TLine.h"
#include "TGaxis.h"
#include "TAxis.h"
#include "TLatex.h"			//for "ttbar"

#include "TopCPViolation/selected/interface/BranchInfo.h"
#include "TopCPViolation/selected/interface/lep_sel.h"
#include "TopCPViolation/selected/interface/jet_sel.h"
#include "TopCPViolation/selected/interface/selection_PUIDBtag_weight.h"	//The update version of "selection.h"
#include "Selected/selected/interface/information.h"		//store any information to help MC weight to data
#include "Selected/selected/interface/File.h"			//an object
#include "Selected/selected/interface/Plotter.h"			//how to File
#include "Selected/selected/interface/GetInfo.h"			//Get some scale factor or something

#include "CondFormats/BTauObjects/interface/BTagCalibration.h"
#include "CondTools/BTau/interface/BTagCalibrationReader.h"
#include "CondFormats/BTauObjects/interface/BTagEntry.h"

using namespace std;

int main( int argc, char* argv[] )
{
	//*****Do some tedious previous calculus*****//

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

	w_TT.push_back( _lumi*Xs_TT[0]/(double)gen_no_TT[0] );
	w_TT.push_back( _lumi*Xs_TT[1]/(double)gen_no_TT[1] );
	w_TT.push_back( _lumi*Xs_TT[2]/(double)gen_no_TT[2] );

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
	bool is_data = true;
	bool is_MC = false;

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
	
	//*********************************************************************************//
	//										Start!									   //
	//*********************************************************************************//

	//**************MC data Analysis***************//

	int bins_No = 45;
	double hist_min = 0.;
	double hist_max = 600.;

	TH1F* h_TT_mu = new TH1F("h_TT_mu","",bins_No,hist_min,hist_max);
	TH1F* h_TT_el = new TH1F("h_TT_el","",bins_No,hist_min,hist_max);
	
	File p_TT_002(pre_TT_002);

	do_selection(p_TT_002.ch,h_TT_mu,h_TT_el,w_TT[1],pileupinfo,h_tightMuIDSF,h_tightElIDSF,btag_map,is_MC);


	TH1F* h_DY_mu = new TH1F("h_DY_mu","",bins_No,hist_min,hist_max);
	TH1F* h_DY_el = new TH1F("h_DY_el","",bins_No,hist_min,hist_max);

	vector<File> p_DY;
	File p_DY_0(DY[0]);		p_DY.push_back(p_DY_0);
	File p_DY_1(DY[1]);		p_DY.push_back(p_DY_1);
	File p_DY_2(DY[2]);		p_DY.push_back(p_DY_2);
	File p_DY_3(DY[3]);		p_DY.push_back(p_DY_3);
	File p_DY_4(DY[4]);		p_DY.push_back(p_DY_4);
	File p_DY_5(DY[5]);		p_DY.push_back(p_DY_5);
	File p_DY_6(DY[6]);		p_DY.push_back(p_DY_6);

	for(int i=0;i<(int)DY.size();i++){
		do_selection(p_DY[i].ch,h_DY_mu,h_DY_el,w_DY[i],pileupinfo,h_tightMuIDSF,h_tightElIDSF,btag_map,is_MC);
	}


	TH1F* h_WJets_mu = new TH1F("h_WJets_mu","",bins_No,hist_min,hist_max);
	TH1F* h_WJets_el = new TH1F("h_WJets_el","",bins_No,hist_min,hist_max);
	
	vector<File> p_WJets;
	File p_WJets_0(WJets[0]);		p_WJets.push_back(p_WJets_0);
	File p_WJets_1(WJets[1]);		p_WJets.push_back(p_WJets_1);
	File p_WJets_2(WJets[2]);		p_WJets.push_back(p_WJets_2);
	File p_WJets_3(WJets[3]);		p_WJets.push_back(p_WJets_3);
	File p_WJets_4(WJets[4]);		p_WJets.push_back(p_WJets_4);
	File p_WJets_5(WJets[5]);		p_WJets.push_back(p_WJets_5);
	File p_WJets_6(WJets[6]);		p_WJets.push_back(p_WJets_6);

	for(int i=0;i<(int)WJets.size();i++){
		do_selection(p_WJets[i].ch,h_WJets_mu,h_WJets_el,w_WJets[i],pileupinfo,h_tightMuIDSF,h_tightElIDSF,btag_map,is_MC);
	}
	

	TH1F* h_VV_mu = new TH1F("h_VV_mu","",bins_No,hist_min,hist_max);
	TH1F* h_VV_el = new TH1F("h_VV_el","",bins_No,hist_min,hist_max);
	
	vector<File> p_VV;
	File p_VV_0(VV[0]);		p_VV.push_back(p_VV_0);
	File p_VV_1(VV[1]);		p_VV.push_back(p_VV_1);
	File p_VV_2(VV[2]);		p_VV.push_back(p_VV_2);

	for(int i=0;i<(int)VV.size();i++){
		do_selection(p_VV[i].ch,h_VV_mu,h_VV_el,w_VV[i],pileupinfo,h_tightMuIDSF,h_tightElIDSF,btag_map,is_MC);
	}
/*
	for (int k=0;k<(int)VV.sizw();i++)
	{
		File p_VV_k(VV[k]);
		do_selection(p_VV_k.ch,h_VV_mu,h_VV_el,w_VV[k],pileupinfo,h_tightMuIDSF,h_tightElIDSF,btag_map,is_MC);
	}
*/
	TH1F* h_ST_mu = new TH1F("h_ST_mu","",bins_No,hist_min,hist_max);
	TH1F* h_ST_el = new TH1F("h_ST_el","",bins_No,hist_min,hist_max);
	
	vector<File> p_ST;
	File p_ST_0(ST[0]);		p_ST.push_back(p_ST_0);
	File p_ST_1(ST[1]);		p_ST.push_back(p_ST_1);
	File p_ST_2(ST[2]);		p_ST.push_back(p_ST_2);
	File p_ST_3(ST[3]);		p_ST.push_back(p_ST_3);
	File p_ST_4(ST[4]);		p_ST.push_back(p_ST_4);

	for(int i=0;i<(int)ST.size();i++){
		do_selection(p_ST[i].ch,h_ST_mu,h_ST_el,w_ST[i],pileupinfo,h_tightMuIDSF,h_tightElIDSF,btag_map,is_MC);
	}

	/*
	TH1F* h_QCD_mu = new TH1F("h_QCD_mu","",bins_No,hist_min,hist_max);
	TH1F* h_QCD_el = new TH1F("h_QCD_el","",bins_No,hist_min,hist_max);
	
	vector<File> p_QCD;
	File p_QCD_0(QCD[0]);		p_QCD.push_back(p_QCD_0);
	File p_QCD_1(QCD[1]);		p_QCD.push_back(p_QCD_1);
	File p_QCD_2(QCD[2]);		p_QCD.push_back(p_QCD_2);
	File p_QCD_3(QCD[3]);		p_QCD.push_back(p_QCD_3);
	File p_QCD_4(QCD[4]);		p_QCD.push_back(p_QCD_4);
	File p_QCD_5(QCD[5]);		p_QCD.push_back(p_QCD_5);
	File p_QCD_6(QCD[6]);		p_QCD.push_back(p_QCD_6);
	File p_QCD_7(QCD[7]);		p_QCD.push_back(p_QCD_7);

	for(int i=0;i<(int)QCD.size();i++){
		do_selection(p_QCD[i].ch,h_QCD_mu,h_QCD_el,w_QCD[i],pileupinfo,h_tightMuIDSF,h_tightElIDSF,btag_map,is_MC);
	}
	*/
	//**************Real Data Analysis!**************//


	TH1F* h_Data_mu = new TH1F("h_Data_mu"," ;hadronic top mass(GeV);Events(No.)",bins_No,hist_min,hist_max);	//"(primary title);(X-Axis title);(Y-Axis Title)"
	TH1F* h_Data_el = new TH1F("h_Data_el"," ;hadronic top mass(GeV);Events(No.)",bins_No,hist_min,hist_max);

	TChain* ch_Data = new TChain("root");
	for(int i=0;i<(int)Data_SM.size();i++){
		ch_Data->Add(Data_SM[i]);
	}
	for(int i=0;i<(int)Data_SE.size();i++){
		ch_Data->Add(Data_SE[i]);
	}

	do_selection(ch_Data,h_Data_mu,h_Data_el,1.,pileupinfo,h_tightMuIDSF,h_tightElIDSF,btag_map,is_data);
	//after the upper do_selection code , the h_Data_mu(el) is Filled~

	//*********************************************************************************************//
	/******************************************Plot it!*********************************************/
	//*********************************************************************************************//
	//Make vector to store histo. to be Plotted

	vector<TH1F*> h_MCs_mu;
	h_MCs_mu.push_back(h_TT_mu);
	h_MCs_mu.push_back(h_DY_mu);
	h_MCs_mu.push_back(h_WJets_mu);
	h_MCs_mu.push_back(h_VV_mu);
	h_MCs_mu.push_back(h_ST_mu);
	//h_MCs_mu.push_back(h_QCD_mu);

	vector<TH1F*> h_MCs_el;
	h_MCs_el.push_back(h_TT_el);
	h_MCs_el.push_back(h_DY_el);
	h_MCs_el.push_back(h_WJets_el);
	h_MCs_el.push_back(h_VV_el);
	h_MCs_el.push_back(h_ST_el);
	//h_MCs_el.push_back(h_QCD_el);


	vector<string> hist_name;
	hist_name.push_back("Data");
	hist_name.push_back("t#bar{t}");
	hist_name.push_back("Drell-Yan");
	hist_name.push_back("W+Jets");
	hist_name.push_back("VV(WW,WZ,ZZ)");
	hist_name.push_back("Single top");
	//hist_name.push_back("QCD");

	//Plot!!
	vector<string> text_mu;
	text_mu.push_back( "Muon channel" );
	text_mu.push_back( "Luminosity : 35.9fb^{-1}" );

	vector<string> text_el;
	text_el.push_back( "Electron channel" );
	text_el.push_back( "Luminosity : 35.9fb^{-1}" );


	string file_name_ps = "";
	if ( argc == 2 ){	
		file_name_ps = std::string( argv[1] );
	}

	string file_name_mu = file_name_ps + "Muon_Channel";
	string file_name_el = file_name_ps + "Electron_Channel";

	Plot_MCData(h_Data_mu,h_MCs_mu,file_name_mu,hist_name,text_mu);

	Plot_MCData(h_Data_el,h_MCs_el,file_name_el,hist_name,text_el);


	//delete the allocated memories
	
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

	delete ch_Data;

	delete f1;
	delete f2;
	
}

