#include "TopCPViolation/selected/interface/GetInfo.h"
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


using namespace std;	//std::stod()	//map	//string	//vector

void GetPileUpInfo(const char* filename,vector<double>& pileup_factors,int* size_of_vec)
{
	char file_name[ BUFFER_SIZE ];			//just make a buffer to store file name temporally
	strcpy(file_name,filename);
	
	int size = 0;
	ifstream fin;
	fin.open( file_name );
	//fstream fin;
	//fin.open( file_name , ios::in );
	
	if(!fin)
	{ 
		printf("\nthere is something wrong in opening the file ");
		printf( file_name );	printf("\n");
   	}

	string input_str;
	while( getline(fin,input_str) )
	{
		//getline( fin , input_str );
		if(size >= *size_of_vec)
		{
			printf("The input vector size is not enough!");
			pileup_factors.push_back( stod( input_str ) );
			//stod() make string to double
			*size_of_vec = size;
			size++;
		}
		else
		{
			pileup_factors.at(size) = stod( input_str );
			size++;
		}
	}
	
	fin.close();
}


double SF_2D::GetTH2FBinContent(double _x,double _y)
{
	int _xLabel = 1, _yLabel = 1;
	for(int i=1;i<=h->GetXaxis()->GetNbins();i++)
	{
		double low_edge = h->GetXaxis()->GetBinLowEdge(i);
		double up_edge = h->GetXaxis()->GetBinUpEdge(i);
		if( ( _x > low_edge  ) && ( _x <= up_edge ) )
		{
			_xLabel = i;
			break;
		}
	}	
	for(int j=1;j<=h->GetYaxis()->GetNbins();j++)
	{
		double low_edge = h->GetYaxis()->GetBinLowEdge(j);
		double up_edge = h->GetYaxis()->GetBinUpEdge(j);
		if( ( _y > low_edge  ) && ( _y <= up_edge ) )
		{
			_yLabel = j;
			break;
		}
	}
	double result = 0.;
	result = h->GetBinContent( _xLabel , _yLabel );
	
	return result;
}

double SF_2D::GetTH2DBinContent(double _x,double _y)
{
	int _xLabel = 1, _yLabel = 1;
	for(int i=1;i<=hd->GetXaxis()->GetNbins();i++)
	{
		double low_edge = hd->GetXaxis()->GetBinLowEdge(i);
		double up_edge = hd->GetXaxis()->GetBinUpEdge(i);
		if( ( _x >= low_edge  ) && ( _x < up_edge ) )
		{
			_xLabel = i;
			break;
		}
	}	
	for(int j=1;j<=hd->GetYaxis()->GetNbins();j++)
	{
		double low_edge = hd->GetYaxis()->GetBinLowEdge(j);
		double up_edge = hd->GetYaxis()->GetBinUpEdge(j);
		if( ( _y >= low_edge  ) && ( _y < up_edge ) )
		{
			_yLabel = j;
			break;
		}
	}
	double result = 0.;
	result = hd->GetBinContent( _xLabel , _yLabel );
	
	return result;
}

/*
template <class T>
double SF_2D_t<T>::GetTH2BinContent( double _x, double _y )
{
	int _xLabel = 1, _yLabel = 1;
	for(int i=1;i<=h->GetXaxis()->GetNbins();i++)
	{
		double low_edge = h->GetXaxis()->GetBinLowEdge(i);
		double up_edge = h->GetXaxis()->GetBinUpEdge(i);
		if( ( _x >= low_edge  ) && ( _x < up_edge ) )
		{
			_xLabel = i;
			break;
		}
	}	
	for(int j=1;j<=h->GetYaxis()->GetNbins();j++)
	{
		double low_edge = h->GetYaxis()->GetBinLowEdge(j);
		double up_edge = h->GetYaxis()->GetBinUpEdge(j);
		if( ( _y >= low_edge  ) && ( _y < up_edge ) )
		{
			_yLabel = j;
			break;
		}
	}
	double result = 0.;
	result = h->GetBinContent( _xLabel , _yLabel );
	
	return result;
}
*/


void Initial_Btag_Weight(std::map<BTagEntry::OperatingPoint, BTagCalibrationReader>& map,const string& tagger,const string& filename)
{
	BTagCalibration* _calib = new BTagCalibration( tagger , filename );

	for(int i=BTagEntry::OP_LOOSE;i!=BTagEntry::OP_RESHAPING;++i)
	{
		map[ BTagEntry::OperatingPoint( i ) ] = BTagCalibrationReader( BTagEntry::OperatingPoint( i ), "central", {"up","down"} );
		map[ BTagEntry::OperatingPoint( i ) ].load(*_calib,BTagEntry::FLAV_B,"comb");
	}
}

