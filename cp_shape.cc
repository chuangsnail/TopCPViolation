/**************************************************************
 *
 *	File Name : cp_shape.h
 *	Description : To compare (SR signal ,CR MC) or (SR signal ,CR Data) with Mlb mass plot pdf
 *	Data : 191111
 *	Author : Chen-Yu Chunag
 *
 * ************************************************************/

#include <iostream>
#include <string>
#include <iomanip>
#include <string.h>

//constant for legend
#define Xm 0.6
#define XM 0.89
#define Ym 0.6
#define YM 0.85
#define TEXT_FONT 43
#define TEXT_SIZE 16

#define declareTH1(T)						\
TH1F* T = new TH1F("T","",Nbins,xmin,xmax);

#define Normalize(h)						\
h->Scale( 1./(h->Integral(1,h->GetXaxis()->GetNbins()+1)) );


#define InitLegend(l)						\
TLegend* l = new TLegend(Xm,Ym,XM,YM);		\
l->SetTextFont(TEXT_FONT);					\
l->SetTextSize(TEXT_SIZE);					\
l->SetBorderSize(0);


using namespace std;

void plot_cp(const string&,const string&,string);

void exe()
{
    plot_cp("/Users/chen-yuchuang/Desktop/ttbar/t13_MLP/SR_t13_MLP_all_191108_1612.root","/Users/chen-yuchuang/Desktop/ttbar/t13_MLP/CR_t13_MLP_all_191110_2350.root","t13");
    plot_cp("/Users/chen-yuchuang/Desktop/ttbar/t11_MLP/SR_t11_MLP_all_191108_1612.root","/Users/chen-yuchuang/Desktop/ttbar/t11_MLP/CR_t11_MLP_all_191110_1825.root","t11");
    plot_cp("/Users/chen-yuchuang/Desktop/ttbar/new chi2/SR_chi2_all_191112_0246.root","/Users/chen-yuchuang/Desktop/ttbar/new chi2/CR_chi2_all_191111_1206.root","chi2");    
}



void plot_cp( const string& pdf1_filename, const string& pdf2_filename , string output_filename )
{
	//SR objs
	TH1F* h_SR_TT_mu;
	TH1F* h_SR_TT_el;

	TH1F* h_SR_DY_mu;
	TH1F* h_SR_WJets_mu;
	TH1F* h_SR_ST_mu;
	TH1F* h_SR_VV_mu;
	TH1F* h_SR_QCD_mu;

	TH1F* h_SR_DY_el;
	TH1F* h_SR_WJets_el;
	TH1F* h_SR_ST_el;
	TH1F* h_SR_VV_el;
	TH1F* h_SR_QCD_el;

	//CR objs
	TH1F* h_CR_TT_mu;
	TH1F* h_CR_DY_mu;
	TH1F* h_CR_WJets_mu;
	TH1F* h_CR_ST_mu;
	TH1F* h_CR_VV_mu;
	TH1F* h_CR_QCD_mu;
	
	TH1F* h_CR_TT_el;
	TH1F* h_CR_DY_el;
	TH1F* h_CR_WJets_el;
	TH1F* h_CR_ST_el;
	TH1F* h_CR_VV_el;
	TH1F* h_CR_QCD_el;

	TH1F* h_CR_Data_mu;
	TH1F* h_CR_Data_el;

	TFile* f1 = new TFile( (char*)pdf1_filename.c_str() );
	TFile* f2 = new TFile( (char*)pdf2_filename.c_str() );

	//"_c" means after mva-cut/chi2-cut
	f1->GetObject("h_l_TT_mu_c",h_SR_TT_mu);
	f1->GetObject("h_l_TT_el_c",h_SR_TT_el);
	h_SR_TT_mu->SetName("h_SR_l_TT_mu_c");
	h_SR_TT_el->SetName("h_SR_l_TT_el_c");
	

	f1->GetObject("h_l_DY_mu_c",h_SR_DY_mu);
	f1->GetObject("h_l_WJets_mu_c",h_SR_WJets_mu);
	f1->GetObject("h_l_ST_mu_c",h_SR_ST_mu);
	f1->GetObject("h_l_VV_mu_c",h_SR_VV_mu);
	f1->GetObject("h_l_QCD_mu_c",h_SR_QCD_mu);

	f1->GetObject("h_l_DY_el_c",h_SR_DY_el);
	f1->GetObject("h_l_WJets_el_c",h_SR_WJets_el);
	f1->GetObject("h_l_ST_el_c",h_SR_ST_el);
	f1->GetObject("h_l_VV_el_c",h_SR_VV_el);
	f1->GetObject("h_l_QCD_el_c",h_SR_QCD_el);

	h_SR_DY_mu->SetName("h_SR_l_DY_mu_c");
	h_SR_WJets_mu->SetName("h_SR_l_WJets_mu_c");
	h_SR_ST_mu->SetName("h_SR_l_ST_mu_c");
	h_SR_VV_mu->SetName("h_SR_l_VV_mu_c");
	h_SR_QCD_mu->SetName("h_SR_l_QCD_mu_c");

	h_SR_DY_el->SetName("h_SR_l_DY_el_c");
	h_SR_WJets_el->SetName("h_SR_l_WJets_el_c");
	h_SR_ST_el->SetName("h_SR_l_ST_el_c");
	h_SR_VV_el->SetName("h_SR_l_VV_el_c");
	h_SR_QCD_el->SetName("h_SR_l_QCD_el_c");


	f2->GetObject("h_l_TT_mu_c",h_CR_TT_mu);
	f2->GetObject("h_l_DY_mu_c",h_CR_DY_mu);
	f2->GetObject("h_l_WJets_mu_c",h_CR_WJets_mu);
	f2->GetObject("h_l_ST_mu_c",h_CR_ST_mu);
	f2->GetObject("h_l_VV_mu_c",h_CR_VV_mu);
	f2->GetObject("h_l_QCD_mu_c",h_CR_QCD_mu);

	f2->GetObject("h_l_TT_el_c",h_CR_TT_el);
	f2->GetObject("h_l_DY_el_c",h_CR_DY_el);
	f2->GetObject("h_l_WJets_el_c",h_CR_WJets_el);
	f2->GetObject("h_l_ST_el_c",h_CR_ST_el);
	f2->GetObject("h_l_VV_el_c",h_CR_VV_el);
	f2->GetObject("h_l_QCD_el_c",h_CR_QCD_el);

	f2->GetObject("h_l_Data_mu_c",h_CR_Data_mu);
	f2->GetObject("h_l_Data_el_c",h_CR_Data_el);

	double Nbins = h_CR_TT_mu->GetXaxis()->GetNbins();
	double xmin = h_CR_TT_mu->GetXaxis()->GetXmin();
	double xmax = h_CR_TT_mu->GetXaxis()->GetXmax();										    

	//TH1F* h_SR_bkg_mu = new TH1F("h_SR_bkg_mu","",Nbins,xmin,xmax);
	//
	//TH1F* h_CR_MC_mu = new TH1F("h_CR_MC_mu","",Nbins,xmin,xmax);
	//TH1F* h_CR_MC_el = new TH1F("h_CR_MC_el","",Nbins,xmin,xmax);

	declareTH1(h_SR_bkg_mu);		
	declareTH1(h_SR_bkg_el);
	declareTH1(h_CR_MC_mu);			
	declareTH1(h_CR_MC_el);

	h_SR_bkg_mu->Add( h_SR_DY_mu );
	h_SR_bkg_mu->Add( h_SR_WJets_mu );
	h_SR_bkg_mu->Add( h_SR_ST_mu );
	h_SR_bkg_mu->Add( h_SR_VV_mu );
	//h_SR_bkg_mu->Add( h_SR_QCD_mu );

	h_SR_bkg_el->Add( h_SR_DY_el );
	h_SR_bkg_el->Add( h_SR_WJets_el );
	h_SR_bkg_el->Add( h_SR_ST_el );
	h_SR_bkg_el->Add( h_SR_VV_el );
	//h_SR_bkg_el->Add( h_SR_QCD_el );

	h_CR_MC_mu->Add( h_CR_TT_mu );
	h_CR_MC_mu->Add( h_CR_DY_mu );
	h_CR_MC_mu->Add( h_CR_WJets_mu );
	h_CR_MC_mu->Add( h_CR_ST_mu );
	h_CR_MC_mu->Add( h_CR_VV_mu );
	h_CR_MC_mu->Add( h_CR_QCD_mu );
	
	h_CR_MC_el->Add( h_CR_TT_el );
	h_CR_MC_el->Add( h_CR_DY_el );
	h_CR_MC_el->Add( h_CR_WJets_el );
	h_CR_MC_el->Add( h_CR_ST_el );
	h_CR_MC_el->Add( h_CR_VV_el );
	h_CR_MC_el->Add( h_CR_QCD_el );


	//Normalization

	/*
	h_SR_TT_mu->Scale( 1./(h_SR_TT_mu->Integral(1,h_SR_TT_mu->GetXaxis()->GetNbins()+1)) );
	h_SR_TT_el->Scale( 1./(h_SR_TT_el->Integral(1,h_SR_TT_el->GetXaxis()->GetNbins()+1)) );
	h_SR_bkg_mu->Scale( 1./(h_SR_bkg_mu->Integral(1,h_SR_bkg_mu->GetXaxis()->GetNbins()+1)) );
	h_SR_bkg_el->Scale( 1./(h_SR_bkg_el->Integral(1,h_SR_bkg_el->GetXaxis()->GetNbins()+1)) );

	h_CR_MC_mu->Scale( 1./(h_CR_MC_mu->Integral(1,h_CR_MC_mu->GetXaxis()->GetNbins()+1)) );
	h_CR_MC_el->Scale( 1./(h_CR_MC_el->Integral(1,h_CR_MC_el->GetXaxis()->GetNbins()+1)) );
	h_CR_Data_mu->Scale( 1./(h_CR_Data_mu->Integral(1,h_CR_Data_mu->GetXaxis()->GetNbins()+1)) );
	h_CR_Data_el->Scale( 1./(h_CR_Data_el->Integral(1,h_CR_Data_el->GetXaxis()->GetNbins()+1)) );
	*/
	Normalize(h_SR_TT_mu);		Normalize(h_SR_TT_el);
	Normalize(h_SR_bkg_mu);		Normalize(h_SR_bkg_el);
	Normalize(h_CR_MC_mu);		Normalize(h_CR_MC_el);
	Normalize(h_CR_Data_mu);	Normalize(h_CR_Data_el);

	TCanvas* c = new TCanvas("c");

	h_SR_TT_mu->SetTitle(";M_{lb};PDF");
	h_SR_TT_el->SetTitle(";M_{lb};PDF");
	h_SR_bkg_mu->SetTitle(";M_{lb};PDF");
	h_SR_bkg_el->SetTitle(";M_{lb};PDF");
	h_CR_MC_mu->SetTitle(";M_{lb};PDF");
	h_CR_MC_el->SetTitle(";M_{lb};PDF");
	h_CR_Data_mu->SetTitle(";M_{lb};PDF");
	h_CR_Data_el->SetTitle(";M_{lb};PDF");

	h_SR_TT_mu->SetMaximum( 1.5 * h_SR_TT_mu->GetMaximum() );
	h_SR_TT_el->SetMaximum( 1.5 * h_SR_TT_el->GetMaximum() );
	h_SR_bkg_mu->SetMaximum( 1.5 * h_SR_bkg_mu->GetMaximum() );
	h_SR_bkg_el->SetMaximum( 1.5 * h_SR_bkg_el->GetMaximum() );
	h_CR_TT_mu->SetMaximum( 1.5 * h_CR_TT_mu->GetMaximum() );
	h_CR_TT_el->SetMaximum( 1.5 * h_CR_TT_el->GetMaximum() );
	h_CR_Data_mu->SetMaximum( 1.5 * h_CR_Data_mu->GetMaximum() );
	h_CR_Data_el->SetMaximum( 1.5 * h_CR_Data_el->GetMaximum() );

	h_SR_TT_mu->SetLineColor(2);
	h_SR_TT_el->SetLineColor(2);
	h_SR_bkg_mu->SetLineColor(kRed-4);
	h_SR_bkg_el->SetLineColor(kRed-4);
	h_CR_MC_mu->SetLineColor(4);
	h_CR_MC_el->SetLineColor(4);
	h_CR_Data_mu->SetLineColor(1);
	h_CR_Data_el->SetLineColor(1);

	h_SR_TT_mu->SetLineWidth(2);
	h_SR_TT_el->SetLineWidth(2);
	h_SR_bkg_mu->SetLineWidth(2);
	h_SR_bkg_el->SetLineWidth(2);
	h_CR_MC_mu->SetLineWidth(2);
	h_CR_MC_el->SetLineWidth(2);
	h_CR_Data_mu->SetLineWidth(2);
	h_CR_Data_el->SetLineWidth(2);

	h_SR_TT_mu->SetStats( false );
	h_SR_TT_el->SetStats( false );
	h_SR_bkg_mu->SetStats( false );
	h_SR_bkg_el->SetStats( false );
	h_CR_MC_mu->SetStats( false );
	h_CR_MC_el->SetStats( false );
	h_CR_Data_mu->SetStats( false );
	h_CR_Data_el->SetStats( false );
	
	output_filename += string("_MlbPDF_Cmp");
	output_filename += string(".pdf"); 

	TLegend* l1 = new TLegend(Xm,Ym,XM,YM);
	l1->AddEntry((TObject*)0,"Muon channel","");
	l1->AddEntry(h_SR_TT_mu,"SR signal MC");
	l1->AddEntry(h_CR_MC_mu,"CR MC");
	l1->SetTextFont(TEXT_FONT);
	l1->SetTextSize(TEXT_SIZE);
	l1->SetBorderSize(0);

	h_SR_TT_mu->Draw("HIST E");
	h_CR_MC_mu->Draw("HIST E SAME");
	l1->Draw("SAME");
	c->Print( (char*)(output_filename + "(").c_str(), "Title:[mu]SR signal MC/CR MC" );

	TLegend* l2 = new TLegend(Xm,Ym,XM,YM);
	l2->AddEntry((TObject*)0,"Electron channel","");
	l2->AddEntry(h_SR_TT_el,"SR signal MC");
	l2->AddEntry(h_CR_MC_el,"CR MC");
	l2->SetTextFont(TEXT_FONT);
	l2->SetTextSize(TEXT_SIZE);
	l2->SetBorderSize(0);

	h_SR_TT_el->Draw("HIST E");
	h_CR_MC_el->Draw("HIST E SAME");
	l2->Draw("SAME");
	c->Print( (char*)(output_filename).c_str(), "Title:[el]SR signal MC/CR MC" );

	TLegend* l3 = new TLegend(Xm,Ym,XM,YM);
	l3->AddEntry((TObject*)0,"Muon channel","");
	l3->AddEntry(h_SR_TT_mu,"SR signal MC");
	l3->AddEntry(h_CR_Data_mu,"CR Data");
	l3->SetTextFont(TEXT_FONT);
	l3->SetTextSize(TEXT_SIZE);
	l3->SetBorderSize(0);

	h_SR_TT_mu->Draw("HIST E");
	h_CR_Data_mu->Draw("P E SAME");
	l3->Draw("SAME");
	c->Print( (char*)(output_filename).c_str(), "Title:[mu]SR signal MC/CR Data" );

	TLegend* l4 = new TLegend(Xm,Ym,XM,YM);
	l4->AddEntry((TObject*)0,"Electron channel","");
	l4->AddEntry(h_SR_TT_el,"SR signal MC");
	l4->AddEntry(h_CR_Data_el,"CR Data");
	l4->SetTextFont(TEXT_FONT);
	l4->SetTextSize(TEXT_SIZE);
	l4->SetBorderSize(0);

	h_SR_TT_el->Draw("HIST E");
	h_CR_Data_el->Draw("P E SAME");
	l4->Draw("SAME");
	c->Print( (char*)(output_filename).c_str(), "Title:[el]SR signal MC/CR Data" );


	InitLegend(l5);
	l5->AddEntry((TObject*)0,"Muon channel","");
	l5->AddEntry(h_SR_bkg_mu,"SR bkg. MC");
	l5->AddEntry(h_CR_Data_mu,"CR Data");

	h_SR_bkg_mu->Draw("HIST E");
	h_CR_Data_mu->Draw("P E SAME");
	l5->Draw("SAME");
	c->Print( (char*)(output_filename).c_str(), "Title:[mu]SR bkg. MC/CR Data" );

	InitLegend(l6);
	l6->AddEntry((TObject*)0,"Electron channel","");
	l6->AddEntry(h_SR_bkg_el,"SR bkg. MC");
	l6->AddEntry(h_CR_Data_el,"CR Data");

	h_SR_bkg_el->Draw("HIST E");
	h_CR_Data_el->Draw("P E SAME");
	l6->Draw("SAME");
	c->Print( (char*)(output_filename + ")").c_str(), "Title:[el]SR bkg. MC/CR Data" );
	

}
