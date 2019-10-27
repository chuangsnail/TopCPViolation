/****************************************************************
 *
 * 	filename : draw_TH2.cc
 *
 * 	Description : to draw chi2-mass-eventsNo TH2 diagram 
 *	running in local ! if u run here u need to call root and .x draw_TH2.cc
 *	or us CMSSW ,include ROOT object and change draw_TH2() to main()
 * 	Data : 190602
 *
 *
 * **************************************************************/




#include <iostream>
#include <string>
#include <vector>

void draw_TH2()
{
	TFile* f = new TFile("bbsep_tt.root");

	TH2D* h_mu;
	TH2D* h_el;
	f->GetObject("h_chi2min_mass_mu",h_mu);
	f->GetObject("h_chi2min_mass_el",h_el);

	h_mu->GetYaxis()->SetTitle("#chi^{2}_{min}");
	h_mu->GetXaxis()->SetTitle("M_{jjb}(Invariant Mass)");
	h_mu->GetZaxis()->SetTitle("events No.");
	h_mu->GetZaxis()->SetLabelSize(0.02);
	h_mu->GetZaxis()->SetTitleOffset(1.2);
	h_mu->GetYaxis()->SetTitleOffset(1.2);
	h_el->GetYaxis()->SetTitle("#chi^{2}_{min}");
	h_el->GetXaxis()->SetTitle("M_{jjb}(Invariant Mass)");
	h_el->GetZaxis()->SetTitle("events No.");
	h_el->GetZaxis()->SetLabelSize(0.02);
	h_el->GetZaxis()->SetTitleOffset(1.2);
	h_el->GetYaxis()->SetTitleOffset(1.2);
	h_el->SetStats(kFALSE);
	TCanvas* c = new TCanvas("c");
	gStyle->SetPalette(kBird);
	h_mu->SetStats(kFALSE);
	h_mu->Draw("COLZ");
	c->Print("chi2_mass.pdf(","Title:Muon Channel");


	h_el->Draw("COLZ");
	c->Print("chi2_mass.pdf)","Title:Electron Channel");

}

