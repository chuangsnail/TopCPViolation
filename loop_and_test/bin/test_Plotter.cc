#include <iostream>
#include <vector>
#include "stdlib.h"
#include "stdio.h"

#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"		//which is in the library "roofit"
#include "TPad.h"			//for gPad
#include "THStack.h"
#include "TColor.h"
#include "TLegend.h"

#include "TopCPViolation/selected/interface/BranchInfo.h"
#include "TopCPViolation/selected/interface/lep_sel.h"
#include "TopCPViolation/selected/interface/jet_sel.h"
#include "TopCPViolation/selected/interface/selection.h"
//#include "TopCPViolation/selected/interface/weight.h"
#include "TopCPViolation/selected/interface/File.h"
#include "TopCPViolation/selected/interface/Plotter.h"

using namespace std;

int main()
{

	TH1F* h_TT_mu = new TH1F("h_TT_mu","top mass",50,0.,1500.);
	TH1F* h_TT_el = new TH1F("h_TT_el","top mass",50,0.,1500.);
	TH1F* h_data_mu = new TH1F("h_TT_mu2","top mass;Mass(GeV);Events(No.)",50,0.,1500.);
	TH1F* h_data_el = new TH1F("h_TT_el2","top mass;Mass(GeV);Events(No.)",50,0.,1500.);
	char name[300] = "/wk_cms2/cychuang/pre_sel_file/pre_sel_TT/dir_001/pre_sel_TT_1.root";
	char name2[300] = "/wk_cms2/cychuang/pre_sel_file/pre_sel_TT/dir_001/pre_sel_TT_2.root";	//pretend to be data

	/*
	for(int i=0;i<3;i++)
	{
		File* plot = new File(name);
		do_selection((plot)->ch,h_TT_mu,h_TT_el);

		delete plot;
	}
	*/
	//the above can be compiled but there're some memorial troubles in the executed process

	File p_TT_test(name);
	do_selection((p_TT_test).ch,h_TT_mu,h_TT_el);
	File p_data_test(name2);
	do_selection((p_data_test).ch,h_data_mu,h_data_el);

	vector<TH1F*> MC_mu;
	MC_mu.push_back(h_TT_mu);
	vector<TH1F*> MC_el;
	MC_el.push_back(h_TT_el);


	vector<string> hist_name;
	hist_name.push_back("Data");	
	hist_name.push_back("t#bar{t}");

	vector<string> text;
	text.push_back( "" );
	
	Plot_MCData(h_data_mu,MC_mu,"test_Plotter",hist_name,text);
	//Plot_MCData(h_data_el,MC_el,"test_El_ch",hist_name);

}

