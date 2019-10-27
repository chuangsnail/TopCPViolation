/****************************************************************
 *
 * 	filename : draw_eff.cc
 *
 * 	Description : to draw cut on min chi-2 value and the rest ratio and event eff 
 *	running in local ! if u run here u need to call root and .x draw_eff.cc
 *	or us CMSSW ,include ROOT object and change draw_eff() to main()
 * 	Data : 190602
 *
 *
 * **************************************************************/
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
#include <map>
#include <iterator>

using namespace std;

void draw_eff()
{
	TFile* f = new TFile("bbsep_tt.root");
	
	double cor_evt_mu = 172204.66;
	double cmis_evt_mu = 76779.62;
	double mist_evt_mu = 50340.72;
    
    double cor_evt_el = 119997.17;
    double cmis_evt_el = 55786.79;
    double mist_evt_el = 37049.75;
    
    map<string,double> cor_evt, cmis_evt, mist_evt, total_evt;
    cor_evt["mu"] = cor_evt_mu;
    cmis_evt["mu"] = cmis_evt_mu;
    mist_evt["mu"] = mist_evt_mu;
    total_evt["mu"] = cor_evt_mu + cmis_evt_mu + mist_evt_mu;
    
    cor_evt["el"] = cor_evt_el;
    cmis_evt["el"] = cmis_evt_el;
    mist_evt["el"] = mist_evt_el;
    total_evt["el"] = cor_evt_el + cmis_evt_el + mist_evt_el;
 
    /*
    map<string,map<string,double>*> evt;
    evt["cor"] = &cor_evt;
    evt["cmis"] = &cmis_evt;
    evt["mist"] = &mist_evt;
     */
     
	TH1F* h_correct_mu;
	TH1F* h_charge_mis_mu;
	TH1F* h_mistag_mu;
	TH1F* h_chi2_data_mu;
	f->GetObject("h_correct_mu",h_correct_mu);
	f->GetObject("h_charge_mis_mu",h_charge_mis_mu);
	f->GetObject("h_mistag_mu",h_mistag_mu);
	f->GetObject("h_chi2_data_mu",h_chi2_data_mu);
    
    TH1F* h_correct_el;
    TH1F* h_charge_mis_el;
    TH1F* h_mistag_el;
    TH1F* h_chi2_data_el;
    f->GetObject("h_correct_el",h_correct_el);
    f->GetObject("h_charge_mis_el",h_charge_mis_el);
    f->GetObject("h_mistag_el",h_mistag_el);
    f->GetObject("h_chi2_data_el",h_chi2_data_el);
    
    TH1F* h_eff_cor_mu = new TH1F("h_eff_cor_mu","",(int)h_correct_mu->GetNbinsX(),0,200);
    TH1F* h_eff_cmis_mu = new TH1F("h_eff_cmis_mu","",(int)h_correct_mu->GetNbinsX(),0,200);
    TH1F* h_eff_mist_mu = new TH1F("h_eff_mist_mu","",(int)h_correct_mu->GetNbinsX(),0,200);
    TH1F* h_eff_events_mu = new TH1F("h_eff_events_mu","",(int)h_correct_mu->GetNbinsX(),0,200);
    
    TH1F* h_eff_cor_el = new TH1F("h_eff_cor_el","",(int)h_correct_el->GetNbinsX(),0,200);
    TH1F* h_eff_cmis_el = new TH1F("h_eff_cmis_el","",(int)h_correct_el->GetNbinsX(),0,200);
    TH1F* h_eff_mist_el = new TH1F("h_eff_mist_el","",(int)h_correct_el->GetNbinsX(),0,200);
    TH1F* h_eff_events_el = new TH1F("h_eff_events_el","",(int)h_correct_el->GetNbinsX(),0,200);

    map<string,TH1F*> h_correct, h_charge_mis, h_mistag, h_eff_cor, h_eff_cmis, h_eff_mist, h_eff_events;
    h_correct["mu"] = h_correct_mu;
    h_charge_mis["mu"] = h_charge_mis_mu;
    h_mistag["mu"] = h_mistag_mu;
    h_eff_cor["mu"] = h_eff_cor_mu;
    h_eff_cmis["mu"] = h_eff_cmis_mu;
    h_eff_mist["mu"] = h_eff_mist_mu;
    h_eff_events["mu"] = h_eff_events_mu;
    
    h_correct["el"] = h_correct_el;
    h_charge_mis["el"] = h_charge_mis_el;
    h_mistag["el"] = h_mistag_el;
    h_eff_cor["el"] = h_eff_cor_el;
    h_eff_cmis["el"] = h_eff_cmis_el;
    h_eff_mist["el"] = h_eff_mist_el;
    h_eff_events["el"] = h_eff_events_el;
    
    /*
    map<string,map<string,TH1F*>*> hist_map;
    hist_map["cor"] = &h_correct;
    hist_map["cmis"] = &h_charge_mis;
    hist_map["mist"] = &h_mistag;
    map<string,map<string,TH1F*>*> hist_eff_map;
    hist_eff_map["cor"] = &h_eff_cor;
    hist_eff_map["cmis"] = &h_eff_cmis;
    hist_eff_map["mist"] = &h_eff_mist;
    */
    
    vector<string> ch = { "mu" , "el" };
    vector<string> cases ={ "cor" , "cmis" , "mist" };
    
    
	double binW = h_correct_mu->GetBinWidth(1);

    for(int i=0;i<(int)ch.size();i++)
    {
        string channel = ch.at(i);
		//use in if binW is 1.
        for(int k=0;k<200;k++)
        {
            int cut = k;
            double no_cor = 0.,no_cmis = 0.,no_mist = 0.;
            double r_cor = 0.,r_cmis = 0.,r_mist = 0.;
            double no_total = 0.;
            for(int r=1;r<=cut;r++)
            {
                no_cor += cor_evt[channel] * h_correct[channel]->GetBinContent(r);
                no_cmis += cmis_evt[channel] * h_charge_mis[channel]->GetBinContent(r);
                no_mist += mist_evt[channel] * h_mistag[channel]->GetBinContent(r);
                no_total =  no_cor + no_cmis + no_mist;
                r_cor = no_cor/no_total;
                r_cmis = no_cmis/no_total;
                r_mist = no_mist/no_total;
                
                h_eff_cor[channel]->SetBinContent(r,r_cor);
                h_eff_cmis[channel]->SetBinContent(r,r_cmis);
                h_eff_mist[channel]->SetBinContent(r,r_mist);
                
                h_eff_events[channel]->SetBinContent(r,no_total/total_evt[channel]);
                
                /*
                for(int p=0;p<cases.size();p++)
                {
                    string Case = cases[p];
                    hist_eff_map[cases.at(Case)]->at(channel)->Fill()
                }
                */
            
                
            }
        }
    }
    
    for(int i=0;i<(int)ch.size();i++)
    {
        string channel = ch.at(i);
        
        h_eff_cor[channel]->SetFillStyle(4050);
        h_eff_cmis[channel]->SetFillStyle(4050);
        h_eff_mist[channel]->SetFillStyle(4050);
        
        h_eff_cor[channel]->SetLineColor(kBlue);
        h_eff_cmis[channel]->SetLineColor(kGreen+1);
        h_eff_mist[channel]->SetLineColor(kRed);
        h_eff_events[channel]->SetLineColor(1);
        
        h_eff_events[channel]->SetLineWidth(2);
        
        h_eff_cor[channel]->SetFillColorAlpha(kBlue,0.);
        h_eff_cmis[channel]->SetFillColorAlpha(kGreen+1,0.);
        h_eff_mist[channel]->SetFillColorAlpha(kRed,0.);
        
        h_eff_cor[channel]->SetStats(false);
        h_eff_cmis[channel]->SetStats(false);
        h_eff_mist[channel]->SetStats(false);
        h_eff_events[channel]->SetStats(false);
        
    }
    
    TLegend* l1 = new TLegend(0.7,0.6,0.9,0.8);
    l1->SetBorderSize(0);
    l1->AddEntry(h_eff_events_mu,"Event Efficiency");
    l1->AddEntry(h_eff_cor_mu,"Correct Matching");
    l1->AddEntry(h_eff_cmis_mu,"Charge Misidentified");
    l1->AddEntry(h_eff_mist_mu,"Mistag");
    
    TLegend* l2 = new TLegend(0.7,0.6,0.9,0.8);
    l2->SetBorderSize(0);
    l2->AddEntry(h_eff_events_el,"Event Efficiency");
    l2->AddEntry(h_eff_cor_el,"Correct Matching");
    l2->AddEntry(h_eff_cmis_el,"Charge Misidentified");
    l2->AddEntry(h_eff_mist_el,"Mistag");
    
    
    TCanvas* c = new TCanvas("c");
    
    c->SetGrid();
    
    h_eff_events_mu->Draw("C");
    h_eff_cor_mu->Draw("C SAME");
    h_eff_cmis_mu->Draw("C SAME");
    h_eff_mist_mu->Draw("C SAME");
    l1->Draw("SAME");
    //h_eff_events_mu->GetYaxis()->SetRange(0.,1.);
    h_eff_events_mu->SetMaximum(1.);
    h_eff_events_mu->SetMinimum(0.);
    
    c->Print("chi2_cut_eff_ratio.pdf(","Title:Muon Channel");
    
    h_eff_events_el->Draw("C");
    h_eff_cor_el->Draw("C SAME");
    h_eff_cmis_el->Draw("C SAME");
    h_eff_mist_el->Draw("C SAME");
    l2->Draw("SAME");
    h_eff_events_el->SetMaximum(1.);
    h_eff_events_el->SetMinimum(0.);

    
    c->Print("chi2_cut_eff_ratio.pdf)","Title:Electron Channel");
    
    
    f->Close();
    delete f;
}
