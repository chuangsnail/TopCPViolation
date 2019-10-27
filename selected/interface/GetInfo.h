#ifndef GETINFO_H
#define GETINFO_H

#include "TopCPViolation/selected/interface/BranchInfo.h"
#include <string>		//for string and getline( , )
#include <fstream>		//for ifstream to input and read file 
#include "string.h"
#include <vector>
#include <iostream>
#include <map>
#include <iterator>

#include "TObject.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TAxis.h"

#include "CondFormats/BTauObjects/interface/BTagEntry.h"
#include "CondFormats/BTauObjects/interface/BTagCalibration.h"
#include "CondTools/BTau/interface/BTagCalibrationReader.h"


#define BUFFER_SIZE 300


using namespace std;	//std::stod()

void GetPileUpInfo(const char* filename,vector<double>& pileup_factors,int* size_of_vec);

class SF_2D
{
public:
	TH2F* h;
	TH2D* hd;

	SF_2D(TH2F* h_input)
	{
		h = h_input;
	}
	
	SF_2D(TH2D* h_input_d)
	{
		hd = h_input_d;
	}
	

	double GetTH2FBinContent(double _x,double _y);
	double GetTH2DBinContent(double _x,double _y);
	
	~SF_2D() { }
};

/*
class SF_2D_t
{
public:
	TH2* h;

	SF_2D_t(TH2* h_input)
	{
		h = h_input;
	}

	double GetTH2BinContent( double _x, double _y );

	~SF_2D_t() { }
};
*/





void Initial_Btag_Weight(std::map<BTagEntry::OperatingPoint, BTagCalibrationReader>& map,const string& tagger,const string& filename);


#endif
