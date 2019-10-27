#ifndef PLOTTER_H
#define PLOTTER_H

#include <iostream>
#include <vector>
#include "stdlib.h"
#include "stdio.h"
#include <string>
#include "string.h"
#include <ctime>
#include <map>
#include <iterator>
#include <cassert>

#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"		//which is in the library "roofit"
#include "TPad.h"			//for gPad
#include "THStack.h"
#include "TColor.h"
#include "TLegend.h"
#include "TLine.h"
#include "TGaxis.h"
#include "TAxis.h"

#include "TopCPViolation/selected/interface/BranchInfo.h"
#include "TopCPViolation/prepare_info/interface/about_time.h"

using namespace std;


//Drawing
							    
TPad* GetBotPad();

TPad* GetTopPad();

auto GetLegend( TH1F* h_data ,const string& Data_name,vector<TH1F*>& MC_histograms , vector<string>& MC_names );

void Plot_MCData(TH1F* h_data ,vector<TH1F*>& h_mcs ,const string& title ,const vector<string>& histo_names,vector<string>& text);

void SaveToPDF(TCanvas* c,const string& filename);

TCanvas* Plot_DataMC(TCanvas* c, TH1F* h_data ,vector<TH1F*>& h_mcs ,const vector<string>& histo_names,vector<string>& text);

void Plot_Stack( const string& fname, vector<TH1F*>& h_mu, vector<TH1F*>& h_el );



/*
class Plotter
{
private:
	int Size;
	vector<TH1F*> hists;
	vector<string> hist_names;
	bool is_data;
	
public:
	void SetHists( vector<TH1F*>& );
	void PrintInfo();


};

void Plotter::SetHists( vector<TH1F*>& hists )
{
	assert( Size == 0 );
	if( Size != 0 )
	{
		cerr << "There is hists in this Plotter, please use <AddHist()> to add some hist." << endl;
	}
	Size = (int)hists.size();

}
*/


#endif

