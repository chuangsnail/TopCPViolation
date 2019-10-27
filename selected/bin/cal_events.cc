//*****************************************************
//
// Filename : cal_events.cc
// Description : cal real data entry after selection
//
//
//

#include "TopCPViolation/selected/interface/Plotter.h"
#include "TopCPViolation/prepare_info/interface/Read_info.h"
#include "TopCPViolation/prepare_info/interface/about_time.h"

#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"

#include <string>
#include <iostream>
#include <iomanip>


int main(int argc, char* argv[])
{	
	TFile* f = new TFile(argv[1]);

	TH1F* h_Data_mu;
	TH1F* h_Data_el;

	f->GetObject("h_Data_mu",h_Data_mu);
	f->GetObject("h_Data_el",h_Data_el);

	double y_Data_el = h_Data_el->Integral(1,51);
	double y_Data_mu = h_Data_mu->Integral(1,51);

	cout << endl << setw(25) << "Muon Channel " << "|" << setw(25) << dec << left << y_Data_mu << endl;
	cout << endl << setw(25) << "Electron Channel " << "|" << setw(25) << dec << left << y_Data_el << endl;
	cout << endl << setw(25) << "Total " << "|" << setw(25) << dec << left << y_Data_mu + y_Data_el << endl;
}

