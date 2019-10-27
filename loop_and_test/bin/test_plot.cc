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

using namespace std;

int main()
{

	TH1F* h_TT_mu = new TH1F("h_TT_mu","top mass",50,0.,1500.);
	TH1F* h_TT_el = new TH1F("h_TT_el","top mass",50,0.,1500.);
	TH1F* h_TT_mu2 = new TH1F("h_TT_mu2","top mass",50,0.,1500.);
	TH1F* h_TT_el2 = new TH1F("h_TT_el2","top mass",50,0.,1500.);
	char name[300] = "/wk_cms2/cychuang/pre_sel_file/pre_sel_TT/dir_001/pre_sel_TT_1.root";
	char name2[300] = "/wk_cms2/cychuang/pre_sel_file/pre_sel_TT/dir_001/pre_sel_TT_2.root";

	vector<char*> files_name;
	files_name.push_back(name);
	files_name.push_back(name2);
	/*
	for(int i=0;i<3;i++)
	{
		File* plot = new File(name);
		do_selection((plot)->ch,h_TT_mu,h_TT_el);

		delete plot;
	}
	*/
	//the above can be compiled but there're some memorial troubles in the executed process

	vector<File> Files;
	for(int i=0;i<(int)files_name.size();i++)
	{
		File f_TT(files_name[i]);
		Files.push_back(f_TT);
	}

	//File p_TT_test(name);
	do_selection((Files[0]).ch,h_TT_mu,h_TT_el);
	//File p_TT_test2(name2);
	do_selection((Files[1]).ch,h_TT_mu2,h_TT_el2);

	h_TT_mu->SetFillColor(50);
	h_TT_mu2->SetMarkerColor(1);
	h_TT_mu2->SetMarkerStyle(21);
	h_TT_mu2->SetMarkerSize(1);

	TCanvas* c = new TCanvas();
	c->Divide(2,2);

	c->cd(1);
	h_TT_mu->Draw("Hist");
	h_TT_mu2->Draw("p E1 same");

	c->cd(2);
	h_TT_mu->Draw("B");
	h_TT_mu2->Draw("pe1same");

	c->cd(3);
	h_TT_mu->Draw("AH");
	h_TT_mu2->Draw("p e1 same");

	c->cd(4);
	h_TT_mu->Draw();
	h_TT_mu2->Draw("p");

	c->SaveAs("test_File.png");

}

