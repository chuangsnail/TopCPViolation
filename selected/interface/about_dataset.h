#ifndef ABOUT_DATASET_H
#define ABOUT_DATASET_H

#include "TopCPViolation/prepare_info/interface/json.hpp"
//#include "TopCPViolation/selected/interface/BranchInfo.h"
//#include "TopCPViolation/selected/interface/"

#include <vector>
#include <map>
#include <iostream>
#include <string>
#include "string.h"
#include <fstream>
#include <stdlib.h>

using namespace std;
using json = nlohmann::json; 

void get_lumi_weight( map< string ,vector<double>* >& Weights_map )
{
    ifstream fin;
    fin.open("/wk_cms2/cychuang/CMSSW_9_2_3/src/TopCPViolation/selected/info./lumi_weight.json");
    json j;
    fin >> j;
    fin.close();
    Weights_map["TT"]->assign(3,0.);
    Weights_map["DY"]->assign(7,0.);
    Weights_map["WJets"]->assign(7,0.);
    Weights_map["VV"]->assign(3,0.);
    Weights_map["ST"]->assign(5,0.);
    Weights_map["QCD"]->assign(8,0.);
    string TT[3] = { "001", "002", "003" };
    string DY[7] = { "100-200", "200-400", "400-600", "600-800", "800-1200", "1200-2500", "2500-Inf" };
    string WJets[7] = { "100-200", "200-400", "400-600", "600-800", "800-1200", "1200-2500", "2500-Inf" };
    string VV[3] = { "WW", "WZ", "ZZ" };
    string ST[5] = { "s", "t_antitop", "t_top", "tW_antitop", "tW_top" };
    string QCD[8] = { "100-200", "200-300", "300-500", "500-700", "700-1000", "1000-1500", "1500-2000", "2000-Inf" };
    
    for(int i=0;i<3;i++)
    {
        double weight = 0.;
        weight = j["TT"][ TT[i] ]["luminosity"] * j["TT"][ TT[i] ]["cross_section"] * j["TT"][ TT[i] ]["xsec_scale_factor"] / j["TT"][ TT[i] ]["gen_number"] ;
        Weights_map["TT"]->at(i) = weight;
    }
    for(int i=0;i<7;i++)
    {
        double weight = 0.;
        weight = j["DY"][ DY[i] ]["luminosity"] * j["DY"][ DY[i] ]["cross_section"] * j["DY"][ DY[i] ]["xsec_scale_factor"] / j["DY"][ DY[i] ]["gen_number"] ;
        Weights_map["DY"]->at(i) = weight;
    }
    for(int i=0;i<7;i++)
    {
        double weight = 0.;
        weight = j["WJets"][ WJets[i] ]["luminosity"] * j["WJets"][ WJets[i] ]["cross_section"] * j["WJets"][ WJets[i] ]["xsec_scale_factor"] / j["WJets"][ WJets[i] ]["gen_number"] ;
        Weights_map["WJets"]->at(i) = weight;
    }
    for(int i=0;i<3;i++)
    {
        double weight = 0.;
        weight = j["VV"][ VV[i] ]["luminosity"] * j["VV"][ VV[i] ]["cross_section"] * j["VV"][ VV[i] ]["xsec_scale_factor"] / j["VV"][ VV[i] ]["gen_number"] ;
        Weights_map["VV"]->at(i) = weight;
    }
    for(int i=0;i<5;i++)
    {
        double weight = 0.;
        weight = j["ST"][ ST[i] ]["luminosity"] * j["ST"][ ST[i] ]["cross_section"] * j["ST"][ ST[i] ]["xsec_scale_factor"] / j["ST"][ ST[i] ]["gen_number"] ;
        Weights_map["ST"]->at(i) = weight;
    }
    for(int i=0;i<8;i++)
    {
        double weight = 0.;
        weight = j["QCD"][ QCD[i] ]["luminosity"] * j["QCD"][ QCD[i] ]["cross_section"] * j["QCD"][ QCD[i] ]["xsec_scale_factor"] / j["QCD"][ QCD[i] ]["gen_number"] ;
        Weights_map["QCD"]->at(i) = weight;
    }
}

void get_path( map< string, vector<char*>* >& Data_Set_Path)
{
    ifstream fin;
    fin.open("/wk_cms2/cychuang/CMSSW_9_2_3/src/TopCPViolation/selected/info./lumi_weight.json");
    json j;
    fin >> j;
    fin.close();
	
    string s_TT[3] = { "001", "002", "003" };
    string s_DY[7] = { "100-200", "200-400", "400-600", "600-800", "800-1200", "1200-2500", "2500-Inf" };
    string s_WJets[7] = { "100-200", "200-400", "400-600", "600-800", "800-1200", "1200-2500", "2500-Inf" };
    string s_VV[3] = { "WW", "WZ", "ZZ" };
    string s_ST[5] = { "s", "t_antitop", "t_top", "tW_antitop", "tW_top" };
    string s_QCD[8] = { "100-200", "200-300", "300-500", "500-700", "700-1000", "1000-1500", "1500-2000", "2000-Inf" };
	
	string s_Data_SM[8] = { "B", "C", "D", "E", "F", "G", "H_v2", "H_v3" };
	string s_Data_SE[8] = { "B", "C", "D", "E", "F", "G", "H_v2", "H_v3" };
	

    for(int i=0;i<3;i++)
    {
        string path;
        path = j["TT"][ s_TT[i] ]["path"];
		char* writable = new char[path.size()+1];
		std::copy(path.begin(),path.end(),writable);
		writable[path.size()] = '\0';
		char c_path[500] = "";
		strcpy(c_path,writable);
		Data_Set_Path["TT"]->push_back( c_path );
		delete [] writable;
    }
    for(int i=0;i<7;i++)
    {
        string path;
        path = j["DY"][ s_DY[i] ]["path"];
		char* writable = new char[path.size()+1];
		std::copy(path.begin(),path.end(),writable);
		writable[path.size()] = '\0';
		char c_path[500] = "";
		strcpy(c_path,writable);
        Data_Set_Path["DY"]->push_back( c_path );
		delete [] writable;
    }
    for(int i=0;i<7;i++)
    {
        string path;
        path = j["WJets"][ s_WJets[i] ]["path"];
		char* writable = new char[path.size()+1];
		std::copy(path.begin(),path.end(),writable);
		writable[path.size()] = '\0';
		char c_path[500] = "";
		strcpy(c_path,writable);
        Data_Set_Path["WJets"]->push_back( c_path );
		delete [] writable;
    }
    for(int i=0;i<3;i++)
    {
        string path;
        path = j["VV"][ s_VV[i] ]["path"];
		char* writable = new char[path.size()+1];
		std::copy(path.begin(),path.end(),writable);
		writable[path.size()] = '\0';
		char c_path[500] = "";
		strcpy(c_path,writable);
        Data_Set_Path["VV"]->push_back( c_path );
		delete [] writable;
    }
    for(int i=0;i<5;i++)
    {
        string path;
        path = j["ST"][ s_ST[i] ]["path"];
		char* writable = new char[path.size()+1];
		std::copy(path.begin(),path.end(),writable);
		writable[path.size()] = '\0';
		char c_path[500] = "";
		strcpy(c_path,writable);
        Data_Set_Path["ST"]->push_back( c_path );
		delete [] writable;
    }
    for(int i=0;i<8;i++)
    {
        string path;
        path = j["QCD"][ s_QCD[i] ]["path"];
		char* writable = new char[path.size()+1];
		std::copy(path.begin(),path.end(),writable);
		writable[path.size()] = '\0';
		char c_path[500] = "";
		strcpy(c_path,writable);
        Data_Set_Path["QCD"]->push_back( c_path );
		delete [] writable;
    }
    for(int i=0;i<8;i++)
    {
        string path = "";
        path = j["Data"]["Single_Muon"][ s_Data_SM[i] ]["path"];
		char* writable = new char[path.size()+1];
		std::copy(path.begin(),path.end(),writable);
		writable[path.size()] = '\0';
		char c_path[500] = "";
		strcpy(c_path,writable);
        Data_Set_Path["Data_SM"]->push_back( c_path );
		delete [] writable;
    }
    for(int i=0;i<8;i++)
    {
        string path;
        path = j["Data"]["Single_Electron"][ s_Data_SE[i] ]["path"];
		char* writable = new char[path.size()+1];
		std::copy(path.begin(),path.end(),writable);
		writable[path.size()] = '\0';
		char c_path[500] = "";
		strcpy(c_path,writable);
        Data_Set_Path["Data_SE"]->push_back( c_path );
		delete [] writable;
    }
}

#endif
