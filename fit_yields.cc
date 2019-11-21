/**************************************************************
 *
 *	File Name : fit_yields.h
 *	Description : 
 *	Data : 191112
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


//Global obj

TH1F* h_sig;
TH1F* h_bkg;
TH1F* h_data;

TH1F** h_mc_sig;
TH1F** h_mc_bkg;


	//used to be fit by templates below
TH1F* h_SR_Data_mu = 0;
TH1F* h_SR_Data_el = 0;
	//used to be signal template
TH1F* h_SR_TT_mu = 0;
TH1F* h_SR_TT_el = 0;
	//used to be bkg template
TH1F* h_CR_Data_mu = 0;
TH1F* h_CR_Data_el = 0;


TH1F* h_SR_bkg_mu = 0;
TH1F* h_SR_bkg_el = 0;
/*
TH1F* h_CR_MC_mu = 0;
TH1F* h_CR_MC_el = 0;
*/

void Fitting( const string& pdf1_filename, const string& pdf2_filename, string pdf_name );

double model(int i, double* par)
{
	double sig_n = par[0];
	double bkg_n = par[1];

	double sig_term = h_sig->GetBinContent(i);
	double bkg_term = h_bkg->GetBinContent(i);
		
	return sig_n * sig_term + bkg_n * bkg_term; 		//don't times binwidth because it's an unbinned fit
}


void fcn(int &npar, double *gin, double &f, double *par, int iflag)
{
    f = 0.;
	    
    double sig_n = par[0];
    double bkg_n = par[1];
	f = 2. * (sig_n + bkg_n);     //extended ML
	    
    for(int i=1;i<=h_data->GetNbinsX();i++)
    {
        double weight = h_data->GetBinContent(i);		//to realize binned ML to be unbinned ML 
        double L = model(i,par);
        if(L>0.){ f -= 2.*log(L)*weight;}
        else {f = HUGE; return;}
   	}
}


void exeFit()
{
	TMinuit* gMinuit = new TMinuit(5);
	gMinuit->SetFCN(fcn);
	gMinuit->DefineParameter(0,"n_sig",15000.,1.,1.,1000000.);
    gMinuit->DefineParameter(1,"n_bkg",1000.,1.,1.,1000000.);

    gMinuit->Command("MIGRAD");
    gMinuit->Command("MIGRAD");
    gMinuit->Command("MINOS");
}

void exe()
{
	Fitting("/Users/chen-yuchuang/Desktop/ttbar/new chi2/SR_chi2_all_191112_0246.root","/Users/chen-yuchuang/Desktop/ttbar/new chi2/CR_chi2_all_191111_1206.root","[mu]chi2");
	Fitting("/Users/chen-yuchuang/Desktop/ttbar/new chi2/SR_chi2_all_191112_0246.root","/Users/chen-yuchuang/Desktop/ttbar/new chi2/CR_chi2_all_191111_1206.root","[el]chi2");
	Fitting("/Users/chen-yuchuang/Desktop/ttbar/t11_MLP/SR_t11_MLP_all_191108_1612.root","/Users/chen-yuchuang/Desktop/ttbar/t11_MLP/CR_t11_MLP_all_191110_1825.root","[mu]t11_MLP");
	Fitting("/Users/chen-yuchuang/Desktop/ttbar/t11_MLP/SR_t11_MLP_all_191108_1612.root","/Users/chen-yuchuang/Desktop/ttbar/t11_MLP/CR_t11_MLP_all_191110_1825.root","[el]t11_MLP");
	Fitting("/Users/chen-yuchuang/Desktop/ttbar/t13_MLP/SR_t13_MLP_all_191108_1612.root","/Users/chen-yuchuang/Desktop/ttbar/t13_MLP/CR_t13_MLP_all_191110_2350.root","[mu]t13_MLP");
	Fitting("/Users/chen-yuchuang/Desktop/ttbar/t13_MLP/SR_t13_MLP_all_191108_1612.root","/Users/chen-yuchuang/Desktop/ttbar/t13_MLP/CR_t13_MLP_all_191110_2350.root","[el]t13_MLP");

}


//use 
void Fitting( const string& pdf1_filename, const string& pdf2_filename, string pdf_name )
{
	//for SR bkg objs
	TH1F* h_SR_DY_mu;
	TH1F* h_SR_WJets_mu;
	TH1F* h_SR_ST_mu;
	TH1F* h_SR_VV_mu;

	TH1F* h_SR_DY_el;
	TH1F* h_SR_WJets_el;
	TH1F* h_SR_ST_el;
	TH1F* h_SR_VV_el;

	TFile* f1 = new TFile( (char*)pdf1_filename.c_str() );
	TFile* f2 = new TFile( (char*)pdf2_filename.c_str() );

	//"_c" means after mva-cut/chi2-cut

	//SetName to avoid same name in SR between CR
	//for SR data
	f1->GetObject("h_l_Data_mu_c",h_SR_Data_mu);
	f1->GetObject("h_l_Data_el_c",h_SR_Data_el);
	h_SR_Data_mu->SetName("h_SR_l_Data_mu_c");
	h_SR_Data_el->SetName("h_SR_l_Data_el_c");

	//for SR sig
	f1->GetObject("h_l_TT_mu_c",h_SR_TT_mu);
	f1->GetObject("h_l_TT_el_c",h_SR_TT_el);
	h_SR_TT_mu->SetName("h_SR_l_TT_mu_c");
	h_SR_TT_el->SetName("h_SR_l_TT_el_c");
	
	//for SR bkg
	f1->GetObject("h_l_DY_mu_c",h_SR_DY_mu);
	f1->GetObject("h_l_WJets_mu_c",h_SR_WJets_mu);
	f1->GetObject("h_l_ST_mu_c",h_SR_ST_mu);
	f1->GetObject("h_l_VV_mu_c",h_SR_VV_mu);

	f1->GetObject("h_l_DY_el_c",h_SR_DY_el);
	f1->GetObject("h_l_WJets_el_c",h_SR_WJets_el);
	f1->GetObject("h_l_ST_el_c",h_SR_ST_el);
	f1->GetObject("h_l_VV_el_c",h_SR_VV_el);

	//for CR data
	f2->GetObject("h_l_Data_mu_c",h_CR_Data_mu);
	f2->GetObject("h_l_Data_el_c",h_CR_Data_el);

	int Nbins = h_SR_Data_mu->GetNbinsX();
	double xmax = h_SR_Data_mu->GetXaxis()->GetXmax();
	double xmin = h_SR_Data_mu->GetXaxis()->GetXmin();
	

	//for SR MC bkg
	//declareTH1(h_SR_bkg_mu);
	//declareTH1(h_SR_bkg_el);
	h_SR_bkg_mu = new TH1F("h_SR_bkg_mu","",Nbins,xmin,xmax);
	h_SR_bkg_el = new TH1F("h_SR_bkg_el","",Nbins,xmin,xmax);

	h_SR_DY_mu->SetName("h_SR_l_DY_mu_c");
	h_SR_WJets_mu->SetName("h_SR_l_WJets_mu_c");
	h_SR_ST_mu->SetName("h_SR_l_ST_mu_c");
	h_SR_VV_mu->SetName("h_SR_l_VV_mu_c");

	h_SR_DY_el->SetName("h_SR_l_DY_el_c");
	h_SR_WJets_el->SetName("h_SR_l_WJets_el_c");
	h_SR_ST_el->SetName("h_SR_l_ST_el_c");
	h_SR_VV_el->SetName("h_SR_l_VV_el_c");

	h_SR_bkg_mu->Add( h_SR_DY_mu );
	h_SR_bkg_mu->Add( h_SR_WJets_mu );
	h_SR_bkg_mu->Add( h_SR_ST_mu );
	h_SR_bkg_mu->Add( h_SR_VV_mu );

	h_SR_bkg_el->Add( h_SR_DY_el );
	h_SR_bkg_el->Add( h_SR_WJets_el );
	h_SR_bkg_el->Add( h_SR_ST_el );
	h_SR_bkg_el->Add( h_SR_VV_el );


	string str_flag = pdf_name.substr(0,4);
	if(str_flag == "[mu]")
	{
		h_sig = h_SR_TT_mu;
		h_bkg = h_CR_Data_mu;
		h_data = h_SR_Data_mu;

		h_mc_sig = &h_SR_TT_mu;
		h_mc_bkg = &h_SR_bkg_mu;
	}
	else if(str_flag == "[el]")
	{
		h_sig = h_SR_TT_el;
		h_bkg = h_CR_Data_el;
		h_data = h_SR_Data_el;

		h_mc_sig = &h_SR_TT_el;
		h_mc_bkg = &h_SR_bkg_el;
	}
	else
	{
		cerr << "Wrong Name tag !" << endl;	return;
	}
	
	//cal
	double no_mc_sig = (*h_mc_sig)->Integral(1.,(*h_mc_sig)->GetNbinsX()+1);
    double no_mc_bkg = (*h_mc_bkg)->Integral(1.,(*h_mc_bkg)->GetNbinsX()+1);

	Normalize(h_sig);
	Normalize(h_bkg);

	exeFit();

	double* yields = new double[2];
	double* error = new double[2];
    for(int i=0;i<2;i++)
    {	gMinuit->GetParameter(i,yields[i],error[i]);	}

	//Draw the comparison plot
	TH1F* curve = new TH1F("curve","",(h_data->GetNbinsX()),\
    h_data->GetXaxis()->GetXmin(),h_data->GetXaxis()->GetXmax());

    for(int i=1;i<curve->GetNbinsX();i++)
    {
        double f = model(i,yields);		//now the par is the parameter after fitting
        //double BinWidth = h_data->GetBinWidth(1);
    	//curve->SetBinContent(i,f*BinWidth);		//use in unbin case
    	curve->SetBinContent(i,f);
    }

    TH1F* curve_sig = new TH1F("curve_sig","",(h_sig->GetNbinsX()),\
    h_sig->GetXaxis()->GetXmin(),h_sig->GetXaxis()->GetXmax());

    for(int i=1;i<curve_sig->GetNbinsX();i++)
    {
    	double content = h_sig->GetBinContent(i) * yields[0];
    	curve_sig->SetBinContent(i,content);
    }
    curve_sig->SetLineWidth(3);

    TH1F* curve_bkg = new TH1F("curve_bkg","",(h_bkg->GetNbinsX()),\
    h_bkg->GetXaxis()->GetXmin(),h_bkg->GetXaxis()->GetXmax());

    for(int i=1;i<curve_bkg->GetNbinsX();i++)
    {
    	double content = h_bkg->GetBinContent(i) * yields[1];
    	curve_bkg->SetBinContent(i,content);
    }
    curve_bkg->SetLineWidth(3);

    h_data->SetMaximum(h_data->GetMaximum() * 1.3);
    h_data->SetTitle(";M_{lb} [GeV];Events No.");
    h_data->SetStats( false );
    h_data->GetYaxis()->SetTitleOffset(1.5);

    TCanvas* c = new TCanvas("c");
    h_data->Draw();
    curve->SetLineWidth(3);    
    curve->Draw("csame");
    curve_bkg->Draw("csame");
    c->Print( (pdf_name + string("_fitting_yields.pdf")).c_str() );

    //Print out the fitting result

    cout << "Fitting Result : " << endl;
    cout << "MC sig : " << yields[0] << " ( " << yields[0]/(yields[0]+yields[1]) << " )"<< endl;
    cout << "MC bkg : " << yields[1] << " ( " << yields[1]/(yields[0]+yields[1]) << " )"<< endl;


    //Print out the MC yields to be compared with fitting result

    cout << endl << "MC Yields : " << endl;
    cout << "MC sig : " << no_mc_sig << " ( " << no_mc_sig/(no_mc_sig+no_mc_bkg) << " )"<< endl;
    cout << "MC bkg : " << no_mc_bkg << " ( " << no_mc_bkg/(no_mc_sig+no_mc_bkg) << " )"<< endl;

    //Write the info above in a file 

    ofstream f;
	f.open("/Users/chen-yuchuang/Desktop/ttbar/fitting_yields.txt",fstream::app);
	f << "Fitting file name : " << pdf_name << endl;		
	f << "Fitting Result : " << endl;
    f << "MC sig : " << yields[0] << " ( " << yields[0]/(yields[0]+yields[1]) << " )"<< endl;
    f << "MC bkg : " << yields[1] << " ( " << yields[1]/(yields[0]+yields[1]) << " )"<< endl;
    f << "MC Yields : " << endl;
    f << "MC sig : " << no_mc_sig << " ( " << no_mc_sig/(no_mc_sig+no_mc_bkg) << " )"<< endl;
    f << "MC bkg : " << no_mc_bkg << " ( " << no_mc_bkg/(no_mc_sig+no_mc_bkg) << " )"<< endl << endl;

    f.close();
	


/*
    string text_mc_sig = to_string(no_mc_sig);
    string text_mc_bkg = to_string(no_mc_bkg);

    TText* t1 = new TText(0.5,0.7,(char*)text_mc_sig.c_str());
    TText* t2 = new TText(0.5,0.3,(char*)text_mc_bkg.c_str());

    t1->Draw();
    t2->Draw("SAME");
    c->Print( (pdf_name + string("_fitting_yields.pdf)")).c_str() , "Title:2" );
*/

}






