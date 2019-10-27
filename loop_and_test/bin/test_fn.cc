#include <iostream>
#include "TopCPViolation/selected/interface/test_fn.h"
#include "TLegend.h"
#include "TCanvas.h"
#include <vector>
#include <string>
#include "string.h"

#include "TopCPViolation/selected/interface/GetInfo.h"

using namespace std;

int main()
{
	/*
	TCanvas* c = new TCanvas("c"," ");

	TLegend* l2 = GetLegend2();
	cout << "GL2" << endl;
	
	auto l1 = GetLegend1();
	cout << "GL1";	

	l2->Draw();
	l1->Draw("same");

	c->SaveAs("test_fn.png");
	*/
	
	vector<double> pileupinfo;
	pileupinfo.assign(76,0.);
	int size = 76;
	char path_file[200] = "/wk_cms2/cychuang/CMSSW_9_2_3/src/Selected/data/pileupweights_69200.csv"; 

	GetPileUpInfo( path_file , pileupinfo , &size);

	for(int i=0;i<(int)pileupinfo.size();i++)
	{
		printf("%.10f  ",pileupinfo.at(i));
	}
}

