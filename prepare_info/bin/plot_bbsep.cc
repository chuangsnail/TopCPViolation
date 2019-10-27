/**************************************************************************************
 *
 *	File Name :	plot_bbsep.cc 
 *	Description : to test the new type to do selection
 *	Making Date : 190525~
 *
 *	Author : Chen-Yu Chuang
 *	using : {exe's name} {rootfile's name} {new pdf's name}
 *
 ****************************************************************************************/
#include "TopCPViolation/selected/interface/BranchInfo.h"
#include "TopCPViolation/selected/interface/Plotter.h"

using namespace std;

int main(int argc,char* argv[])
{
	//double cor_t_weight_mu = 172204.66;
	//double cmis_t_weight_mu = 76779.62;
	//double mist_t_weight_mu = 50340.72;

	//double cor_t_weight_el = 119997.17;
	//double cmis_t_weight_el = 55786.79;
	//double mist_t_weight_el = 37049.75;

	TH1F* h_correct_mu;
	TH1F* h_charge_mis_mu;
	TH1F* h_mistag_mu;
	//TH1F* h_chi2_data_mu;
	TH1F* h_correct_el;
	TH1F* h_charge_mis_el;
	TH1F* h_mistag_el;
	//TH1F* h_chi2_data_el;

	TFile* f = new TFile(argv[1]);		//if we execute it in the directory TopCPViolation
	f->GetObject("h_correct_mu",h_correct_mu);
	f->GetObject("h_correct_el",h_correct_el);
	f->GetObject("h_charge_mis_mu",h_charge_mis_mu);
	f->GetObject("h_charge_mis_mu",h_charge_mis_el);
	f->GetObject("h_mistag_mu",h_mistag_mu);
	f->GetObject("h_mistag_el",h_mistag_el);
	//f->GetObject("h_chi2_data_mu",h_chi2_data_mu);
	//f->GetObject("h_chi2_data_el",h_chi2_data_el);


	TCanvas* c = new TCanvas("c");
	//c->SetLogy(true);

	h_correct_mu->SetStats( false );
	h_charge_mis_mu->SetStats( false );
	h_mistag_mu->SetStats( false );
	//h_chi2_data_mu->SetStats( false );
	h_correct_el->SetStats( false );
	h_charge_mis_el->SetStats( false );
	h_mistag_el->SetStats( false );
	//h_chi2_data_el->SetStats( false );

	h_correct_mu->SetFillColorAlpha(kBlue,0.1);
	h_charge_mis_mu->SetFillColorAlpha(kGreen+1,0.1);
	h_mistag_mu->SetFillColorAlpha(kRed,0.1);
	h_correct_el->SetFillColorAlpha(kBlue,0.1);
	h_charge_mis_el->SetFillColorAlpha(kGreen+1,0.1);
	h_mistag_el->SetFillColorAlpha(kRed,0.1);

	h_correct_mu->SetFillStyle(4050);
	h_charge_mis_mu->SetFillStyle(4050);
	h_mistag_mu->SetFillStyle(4050);
	h_correct_el->SetFillStyle(4050);
	h_charge_mis_el->SetFillStyle(4050);
	h_mistag_el->SetFillStyle(4050);

	h_correct_mu->SetLineColor(kBlue);
	h_charge_mis_mu->SetLineColor(kGreen+1);
	h_mistag_mu->SetLineColor(kRed);
	h_correct_el->SetLineColor(kBlue);
	h_charge_mis_el->SetLineColor(kGreen+1);
	h_mistag_el->SetLineColor(kRed);

	h_correct_mu->SetLineWidth(2);
	h_charge_mis_mu->SetLineWidth(2);
	h_mistag_mu->SetLineWidth(2);
	h_correct_el->SetLineWidth(2);
	h_charge_mis_el->SetLineWidth(2);
	h_mistag_el->SetLineWidth(2);

	/*
	h_correct->Draw("HIST");
	c->Print("chi2_value_tt.pdf(","Title:Correct Matching case");
	h_charge_mis->Draw("HIST");
	c->Print("chi2_value_tt.pdf","Title:Charge Misidentified case");
	h_mistag->Draw("HIST");
	c->Print("chi2_value_tt.pdf","Title:Mistag case");
	*/

	auto legend1 = new TLegend(0.54,0.69,0.95-0.05,0.6/0.7-0.05);	
	legend1->SetTextFont( 43 );
	legend1->SetTextSize( 13 );
	legend1->SetBorderSize( 0 );

	legend1->AddEntry( (TObject*)0, "Muon Channel" , "" );	
	legend1->AddEntry( h_correct_mu , "Correct Matching" );
	legend1->AddEntry( h_charge_mis_mu , "Charge Misidentified" );
	legend1->AddEntry( h_mistag_mu , "Mistag" );

	auto legend2 = new TLegend(0.54,0.69,0.95-0.05,0.6/0.7-0.05);	
	legend2->SetTextFont( 43 );
	legend2->SetTextSize( 13 );
	legend2->SetBorderSize( 0 );
	
	legend2->AddEntry( (TObject*)0, "Electron Channel" , "" );	
	legend2->AddEntry( h_correct_el , "Correct Matching" );
	legend2->AddEntry( h_charge_mis_el , "Charge Misidentified" );
	legend2->AddEntry( h_mistag_el , "Mistag" );
	
	h_correct_mu->GetYaxis()->SetTitleOffset(1.2);
	h_correct_mu->GetXaxis()->SetTitle( "Leptonic top Mass(GeV)" );
	
	h_correct_mu->Draw("HIST");
	h_charge_mis_mu->Draw("HIST SAME");
	h_mistag_mu->Draw("HIST SAME");
	legend1->Draw("SAME");
	string name1 = string(argv[2]) + "(";
	c->Print(name1.c_str(),"Title:Muon Channel");

	h_correct_el->GetYaxis()->SetTitleOffset(1.2);
	h_correct_el->GetXaxis()->SetTitle( "Leptonic top Mass(GeV)" );

	h_correct_el->Draw("HIST");
	h_charge_mis_el->Draw("HIST SAME");
	h_mistag_el->Draw("HIST SAME");
	legend2->Draw("SAME");
	string name2 = string(argv[2]) + ")";
	c->Print(name2.c_str(),"Title:Electron Channel");

	/*	
	double total_weight = cor_t_weight + cmis_t_weight + mist_t_weight;
	h_correct->Scale(cor_t_weight/total_weight);
	h_charge_mis->Scale(cmis_t_weight/total_weight);
	h_mistag->Scale(mist_t_weight/total_weight);

	vector<TH1F*> h_MCs;
	h_MCs.push_back(h_correct);
	h_MCs.push_back(h_charge_mis);
	h_MCs.push_back(h_mistag);

	vector<string> text;
	text.push_back("Cases");

	vector<string> hist_name;
	hist_name.push_back("Data");
	hist_name.push_back("Correct Matching");
	hist_name.push_back("Charge Misidentified");
	hist_name.push_back("Mistag");

	Plot_MCData(h_chi2_data,h_MCs,"BBSep",hist_name,text);
	
*/

	//Save these 4 hist to be a root file
	
	//*****make space free*****//
	
	//delete h_correct_mu;
	//delete h_charge_mis_mu;
	//delete h_mistag_mu;

	//delete h_chi2_data_mu;	

	//delete h_correct_el;
	//delete h_charge_mis_el;
	//delete h_mistag_el;

	//delete h_chi2_data_el;	
	
	delete c;

}

