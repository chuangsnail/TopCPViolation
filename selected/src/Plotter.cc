#include <iostream>
#include <vector>
#include "stdlib.h"
#include "stdio.h"
#include <string>
#include "string.h"

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
#include "TObject.h"

#include "TopCPViolation/selected/interface/Plotter.h"

using namespace std;
//std::to_string()

//Drawing
	
							    
TPad* GetBotPad()
{
    TPad* bot = new TPad( "botpad", "", 0, 0.00, 1, 0.3 );
	bot->SetTicks( 1, 1 );
	bot->SetTopMargin( 0.025 );
	bot->SetLeftMargin( 0.13 );
	bot->SetRightMargin( 0.05 );
	bot->SetBottomMargin( 0.105/0.3 );

	return bot;
}

TPad* GetTopPad()
{
	TPad* top = new TPad( "toppad", "" , 0, 0.3 , 1., 1.0 );//(x_low,y_low,x_up,y_up)
	top->SetTicks( 1, 1 );
	top->SetBottomMargin( 0.05 );
	top->SetLeftMargin( 0.13 );
	top->SetRightMargin( 0.05 );
	top->SetTopMargin( 0.1/0.7 );

	return top;
}

auto GetLegend()
{
	//auto legend = new TLegend(0.54,0.4,0.95,0.6/0.7);		//the (0.95,0.6/0.7) is to match the toppad's margin (0.05,0.1/0.7)
	auto legend = new TLegend(0.54,0.4,0.95-0.05,0.6/0.7-0.05);		//the (0.95,0.6/0.7) is to match the toppad's margin (0.05,0.1/0.7)
	legend->SetTextFont( 43 );
	legend->SetTextSize( 16 );
	legend->SetBorderSize( 0 );
	//legend->SetFillColorAlpha( 0, 0 );
	
	return legend;
}

void Plot_MCData(TH1F* h_data ,vector<TH1F*>& h_mcs ,const string& title ,const vector<string>& histo_names,vector<string>& text)
//histo_names need to be {data_name , MC1_name , MC2_name , ......}
{
	//Setting and declaring
	//string stack_cons = title + ";" + " " + ";Events(No.)";	//if we want title, use this!
	
	string stack_cons = " ; ;Events(No.)";
		
	THStack* stack_MC = new THStack(" ",stack_cons.c_str());
	
	for(int i=0;i<(int)h_mcs.size();i++){
		stack_MC->Add(h_mcs[i]);
	}
	
	h_data->SetMarkerColor(1);
	h_data->SetMarkerSize(1);
	h_data->SetMarkerStyle(21);

	vector<int> color_set = { kRed-4 , kYellow-9 , kBlue-7 , kCyan-3 , kGray+2 , kMagenta  };
	
	for(int i=0;i<(int)h_mcs.size();i++){
		h_mcs[i]->SetMarkerColor(color_set[i]);
		h_mcs[i]->SetMarkerStyle(21);
		h_mcs[i]->SetFillColor(color_set[i]);
	}


	auto legend = GetLegend();
	
	//string lumi_info = "Luminosity : 35.9fb^{-1}" ;
	//legend->AddEntry( (TObject*)0, lumi_info.c_str() , "" );
	
	for(int i=0;i<(int)text.size();i++)
	{
		legend->AddEntry( (TObject*)0, text[i].c_str() , "" );
	}
	legend->AddEntry( (TObject*)0, " " , "" );	
	legend->AddEntry( h_data , histo_names[0].c_str() );
	//legend->AddEntry( h_data , Data_name.c_str() );
	for(int i=0;i<(int)h_mcs.size();i++){
		//legend->AddEntry( h_mcs[i] , MC_names[i].c_str() , "F" );
		legend->AddEntry( h_mcs[i] , histo_names[i+1].c_str() , "F" );
	}


	//Start Drawing
	
	TCanvas* c = new TCanvas("c","",700,600);
	
	TPad* top = GetTopPad();
	top->Draw();
	top->cd();

	stack_MC->SetMaximum( 1.5 * stack_MC->GetMaximum() );

	h_data->SetMarkerSize( 0.75 );

	stack_MC->Draw("HIST");
	h_data->Draw("P E same");
	legend->Draw("same");
	
	
	stack_MC->GetYaxis()->SetTitleOffset( 1.7 );	//to put yaxis title left a little
	//stack_MC->GetYaxis()->TGaxis::SetMaxDigits(4);

	c->cd();	//c->cd(0);

	TPad* bot = GetBotPad();
	bot->Draw();
	bot->cd();

	double xmin = h_data->GetXaxis()->GetXmin();
	double xmax = h_data->GetXaxis()->GetXmax();										    
	TLine* line       = new TLine( xmin, 1.0, xmax, 1.0 );
	TLine* upper_line = new TLine( xmin, 1.5, xmax, 1.5 );
	TLine* lower_line = new TLine( xmin, 0.5, xmax, 0.5 );

	
	//Make the TH1 to store the stacked MC information (because the THStack cannot be divide by TH1,so change it to TH1)
	int XbinsNo = h_data->GetXaxis()->GetNbins();
	TH1F* h_stack_MC= new TH1F("h_stack_MC","hstack in type TH1F",XbinsNo,xmin,xmax);	//h_result_ch_el (MC hist)
	
	for(int i=0;i<(int)h_mcs.size();i++){
		h_stack_MC->Add( h_mcs[i] );
	}

	TH1F* result = (TH1F*)( h_data->Clone() );
	result->Divide( h_stack_MC );

	for( int i = 0; i < h_data->GetNcells(); ++i ){
		if( h_data->GetBinContent( i ) == 0 && h_stack_MC->GetBinContent( i ) == 0 ){
		result->SetBinContent( i, 1. );
		}
	}

	result->Draw( "EP" );
	upper_line->Draw( "same" );
	lower_line->Draw( "same" );
	line->Draw( "same" );
	
	line->SetLineColor( kRed );
	upper_line->SetLineStyle( 3 );
	lower_line->SetLineStyle( 3 );
			    
	result->SetMaximum( 1.6 );
	result->SetMinimum( 0.4 );
	result->GetYaxis()->SetTitle( "Data/MC" );
	result->GetXaxis()->SetTitle( h_data->GetXaxis()->GetTitle() );

	result->GetXaxis()->SetLabelFont( 43 );
	result->GetXaxis()->SetTitleFont( 43 );
	result->GetYaxis()->SetTitleFont( 43 );
	result->GetYaxis()->SetLabelFont( 43 );
	result->GetXaxis()->SetLabelSize( 18 );
	result->GetXaxis()->SetTitleSize( 22 );
	result->GetYaxis()->SetLabelSize( 18 );
	result->GetYaxis()->SetTitleSize( 22 );
	
	result->GetYaxis()->SetNdivisions( 503 );
	result->GetXaxis()->SetTitleOffset( 4.0 );
	result->GetYaxis()->SetTitleOffset( 1.2 );

	result->SetStats( kFALSE );		//make default right upper block off

	c->cd();

	string save_time = "";
	save_time = get_time_str( minute );

	string file_name = title + "_" + save_time + ".pdf";
	SaveToPDF(c,file_name);

	delete h_stack_MC;

	delete top;
	delete bot;

	delete c;

	delete stack_MC;
}

void SaveToPDF(TCanvas* c,const string& filename)
{
	c->SaveAs( filename.c_str() );
}

TCanvas* Plot_DataMC(TCanvas* c, TH1F* h_data ,vector<TH1F*>& h_mcs ,const vector<string>& histo_names,vector<string>& text)
//histo_names need to be {data_name , MC1_name , MC2_name , ......}
{
	//Setting and declaring
	//string stack_cons = title + ";" + " " + ";Events(No.)";	//if we want title, use this!
	
	string stack_cons = " ; ;Events(No.)";
		
	THStack* stack_MC = new THStack(" ",stack_cons.c_str());
	
	for(int i=0;i<(int)h_mcs.size();i++){
		stack_MC->Add(h_mcs[i]);
	}
	
	h_data->SetMarkerColor(1);
	h_data->SetMarkerSize(1);
	h_data->SetMarkerStyle(21);

	vector<int> color_set = { kRed-4 , kYellow-9 , kBlue-7 , kCyan-3 , kGray+2 , kMagenta  };
	
	for(int i=0;i<(int)h_mcs.size();i++){
		h_mcs[i]->SetMarkerColor(color_set[i]);
		h_mcs[i]->SetMarkerStyle(21);
		h_mcs[i]->SetFillColor(color_set[i]);
	}


	auto legend = GetLegend();
	
	//string lumi_info = "Luminosity : 35.9fb^{-1}" ;
	//legend->AddEntry( (TObject*)0, lumi_info.c_str() , "" );
	
	for(int i=0;i<(int)text.size();i++)
	{
		legend->AddEntry( (TObject*)0, text[i].c_str() , "" );
	}
	legend->AddEntry( (TObject*)0, " " , "" );	
	legend->AddEntry( h_data , histo_names[0].c_str() );
	//legend->AddEntry( h_data , Data_name.c_str() );
	for(int i=0;i<(int)h_mcs.size();i++){
		//legend->AddEntry( h_mcs[i] , MC_names[i].c_str() , "F" );
		legend->AddEntry( h_mcs[i] , histo_names[i+1].c_str() , "F" );
	}


	//Start Drawing
	
	c->cd();

	TPad* top = GetTopPad();
	top->Draw();
	top->cd();

	stack_MC->SetMaximum( 1.5 * stack_MC->GetMaximum() );

	h_data->SetMarkerSize( 0.75 );

	stack_MC->Draw("HIST");
	h_data->Draw("P E same");
	legend->Draw("same");
	
	
	stack_MC->GetYaxis()->SetTitleOffset( 1.7 );	//to put yaxis title left a little
	//stack_MC->GetYaxis()->TGaxis::SetMaxDigits(4);

	c->cd();	//c->cd(0);

	TPad* bot = GetBotPad();
	bot->Draw();
	bot->cd();

	double xmin = h_data->GetXaxis()->GetXmin();
	double xmax = h_data->GetXaxis()->GetXmax();										    
	TLine* line       = new TLine( xmin, 1.0, xmax, 1.0 );
	TLine* upper_line = new TLine( xmin, 1.5, xmax, 1.5 );
	TLine* lower_line = new TLine( xmin, 0.5, xmax, 0.5 );

	
	//Make the TH1 to store the stacked MC information (because the THStack cannot be divide by TH1,so change it to TH1)
	int XbinsNo = h_data->GetXaxis()->GetNbins();
	TH1F* h_stack_MC= new TH1F("h_stack_MC","hstack in type TH1F",XbinsNo,xmin,xmax);	//h_result_ch_el (MC hist)
	
	for(int i=0;i<(int)h_mcs.size();i++){
		h_stack_MC->Add( h_mcs[i] );
	}

	TH1F* result = (TH1F*)( h_data->Clone() );
	result->Divide( h_stack_MC );

	for( int i = 0; i < h_data->GetNcells(); ++i ){
		if( h_data->GetBinContent( i ) == 0 && h_stack_MC->GetBinContent( i ) == 0 ){
		result->SetBinContent( i, 1. );
		}
	}

	result->Draw( "EP" );
	upper_line->Draw( "same" );
	lower_line->Draw( "same" );
	line->Draw( "same" );
	
	line->SetLineColor( kRed );
	upper_line->SetLineStyle( 3 );
	lower_line->SetLineStyle( 3 );
			    
	result->SetMaximum( 1.6 );
	result->SetMinimum( 0.4 );
	result->GetYaxis()->SetTitle( "Data/MC" );
	result->GetXaxis()->SetTitle( h_data->GetXaxis()->GetTitle() );

	result->GetXaxis()->SetLabelFont( 43 );
	result->GetXaxis()->SetTitleFont( 43 );
	result->GetYaxis()->SetTitleFont( 43 );
	result->GetYaxis()->SetLabelFont( 43 );
	result->GetXaxis()->SetLabelSize( 18 );
	result->GetXaxis()->SetTitleSize( 22 );
	result->GetYaxis()->SetLabelSize( 18 );
	result->GetYaxis()->SetTitleSize( 22 );
	
	result->GetYaxis()->SetNdivisions( 503 );
	result->GetXaxis()->SetTitleOffset( 4.0 );
	result->GetYaxis()->SetTitleOffset( 1.2 );

	result->SetStats( kFALSE );		//make default right upper block off

	return c;
}

//h_mu is { h_MC1_mu, h_MC2_mu, ... , h_Data_mu }
void Plot_Stack( const string& fname, vector<TH1F*>& h_mu, vector<TH1F*>& h_el )
{
	TCanvas* c = new TCanvas("c","",700,600);

	string save_time = "";
	save_time = get_time_str( minute );
	string filename = fname + "_" + save_time + ".pdf";
	
	vector<string> hist_name;
	hist_name.push_back("Data");		hist_name.push_back("t#bar{t}");
	hist_name.push_back("Drell-Yan");	hist_name.push_back("W+Jets");
	hist_name.push_back("VV(WW,WZ,ZZ)");hist_name.push_back("Single top");
	hist_name.push_back("QCD");
	
	//Muon Channel
	
	vector<TH1F*> h_MCs_mu, h_MCs_el;
	for(int i=0;i<(int)h_mu.size()-1;++i)
	{
		h_MCs_mu.push_back( h_mu.at(i) );
	}
	for(int i=0;i<(int)h_el.size()-1;++i)
	{
		h_MCs_el.push_back( h_el.at(i) );
	}
	
	vector<string> text_mu;
	text_mu.push_back( "Muon channel" );
	text_mu.push_back( "Luminosity : 35.9fb^{-1}" );
	
	vector<string> text_el;
	text_el.push_back( "Electron channel" );
	text_el.push_back( "Luminosity : 35.9fb^{-1}" );
	
	c = Plot_DataMC( c, h_mu.at( (int)h_mu.size()-1 ), h_MCs_mu, hist_name, text_mu );
	
	string filename1 = filename + "(";
	c->Print( filename1.c_str() , "Title:Muon Channel" );

	c->cd();

	c = Plot_DataMC( c, h_el.at( (int)h_el.size()-1 ), h_MCs_el, hist_name, text_el );

	string filename2 = filename + ")";
	c->Print( filename2.c_str(), "Title:Electron Channel" );

	
}
