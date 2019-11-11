#include "TopCPViolation/selected/interface/Plotter.h"
#include "TopCPViolation/prepare_info/interface/Read_info.h"
#include "TopCPViolation/prepare_info/interface/about_time.h"

#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"

#include <string>
#include <iostream>


int main(int argc, char* argv[])
{	
	TFile* f = new TFile(argv[1]);

	TH1F* h_TT_mu;
	TH1F* h_DY_mu;
	TH1F* h_WJets_mu;
	TH1F* h_VV_mu;
	TH1F* h_ST_mu;
	TH1F* h_QCD_mu;
	TH1F* h_Data_mu;

	TH1F* h_TT_el;
	TH1F* h_DY_el;
	TH1F* h_WJets_el;
	TH1F* h_VV_el;
	TH1F* h_ST_el;
	TH1F* h_QCD_el;
	TH1F* h_Data_el;

	f->GetObject("h_TT_mu",h_TT_mu);
	f->GetObject("h_DY_mu",h_DY_mu);
	f->GetObject("h_WJets_mu",h_WJets_mu);
	f->GetObject("h_VV_mu",h_VV_mu);
	f->GetObject("h_ST_mu",h_ST_mu);
	f->GetObject("h_QCD_mu",h_QCD_mu);
	f->GetObject("h_Data_mu",h_Data_mu);

	f->GetObject("h_TT_el",h_TT_el);
	f->GetObject("h_DY_el",h_DY_el);
	f->GetObject("h_WJets_el",h_WJets_el);
	f->GetObject("h_VV_el",h_VV_el);
	f->GetObject("h_ST_el",h_ST_el);
	f->GetObject("h_QCD_el",h_QCD_el);
	f->GetObject("h_Data_el",h_Data_el);


	double y_TT_el = h_TT_el->Integral(1,51);
	double y_DY_el = h_DY_el->Integral(1,51);
	double y_WJets_el = h_WJets_el->Integral(1,51);
	double y_VV_el = h_VV_el->Integral(1,51);
	double y_ST_el = h_ST_el->Integral(1,51);
	double y_QCD_el = h_QCD_el->Integral(1,51);
	double y_Data_el = h_Data_el->Integral(1,51);
	
	double y_TT_mu = h_TT_mu->Integral(1,51);
	double y_DY_mu = h_DY_mu->Integral(1,51);
	double y_WJets_mu = h_WJets_mu->Integral(1,51);
	double y_VV_mu = h_VV_mu->Integral(1,51);
	double y_ST_mu = h_ST_mu->Integral(1,51);
	double y_QCD_mu = h_QCD_mu->Integral(1,51);
	double y_Data_mu = h_Data_mu->Integral(1,51);

	double y_total_el = y_TT_el + y_DY_el + y_WJets_el + y_VV_el + y_ST_el + y_QCD_el;
	double y_total_mu = y_TT_mu + y_DY_mu + y_WJets_mu + y_VV_mu + y_ST_mu + y_QCD_mu;

	cout << "\n-----Muon Channel-----" << endl;
	cout << "total Data yields : " << y_Data_mu << endl;
	cout << "total MC yields : " << y_total_mu << endl;
	cout << "MC signal : " << y_TT_mu << endl;
	cout << "MC background : " << y_total_mu - y_TT_mu << endl;

	cout << "\n-----Electron Channel-----" << endl;
	cout << "total Data yields : " << y_Data_el << endl;
	cout << "total MC yields : " << y_total_el << endl;
	cout << "MC signal : " << y_TT_el << endl;
	cout << "MC background : " << y_total_el - y_TT_el << endl;

	cout << "\n-----Total-----" << endl;
	cout << "Total MC yields : " << y_total_el + y_total_mu << endl;
	cout << "MC Signal yields : " << y_TT_el + y_TT_mu << endl;
	cout << "MC background yields : " << y_total_el + y_total_mu - y_TT_el - y_TT_mu << endl;
	cout << "Total Data yields : " << y_Data_el + y_Data_mu << endl << endl;
	

	cout << endl;
	cout << "ch      TT   " << "    DY    " << "   WJets   " << "    VV    " << "    ST    " << "   QCD   " << endl;
	cout << "Mu  " << y_TT_el/y_total_el << " " << y_DY_el/y_total_el << " " << y_WJets_el/y_total_el << " " << y_VV_el/y_total_el \
		<< " " << y_ST_el/y_total_el << " " << y_QCD_el/y_total_el << endl;
	cout << "El  " << y_TT_mu/y_total_mu << " " << y_DY_mu/y_total_mu << " " << y_WJets_mu/y_total_mu << " " << y_VV_mu/y_total_mu \
		<< " " << y_ST_mu/y_total_mu << " " << y_QCD_mu/y_total_mu << endl;
}
