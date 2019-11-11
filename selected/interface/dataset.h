#ifndef DATASET_H
#define DATASET_H

#include "TopCPViolation/prepare_info/interface/Reader.h"
#include "TopCPViolation/prepare_info/interface/Read_info.h"

#include <vector>
#include <map>
#include <iterator>
#include <iostream>
#include <string>
#include "string.h"
#include <fstream>
#include <stdlib.h>

using namespace std;

/*
void get_ori_data_path( map< string, vector<double>* >& files_map )
{
	char file_path[500] = "";
	Read_info( file_path );

	map< string, vector<string>* >::iterator data_sm, data_se;
	data_sm = files_map.find("Data_SM");	data_se = files_map.find("Data_SE");
		
	vector< map<string, vector<string>* >::iterator > its;
	its.push_back(data_sm);		its.push_back(data_se);

	vector<string> s_Data_SM{ "B", "C", "D", "E", "F", "G", "H_v2", "H_v3" };
	vector<string> s_Data_SE{ "B", "C", "D", "E", "F", "G", "H_v2", "H_v3" };
	
	vector< vector<string>* > sets;
	sets.push_back(&s_Data_SM);		sets.push_back(&s_Data_SE);

	for(int i=0;i<(int)sets.size();i++)
	{
		for(int j=0;j<(int)sets.at(i)->size();j++)
		{
			string key_path = sets_name.at(i) + "_" + sets.at(i)->at(j) + "_" + "path";

			string path = r.second( key_path );
			its.at(i)->second->push_back( path );
		}
	}    	

}
*/


void get_lumi_weight( map< string ,vector<double>* >& Weights_map , string file_path = "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/selected/info./pre_sel_data.txt" )
{
	//char file_path[500] = "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/selected/info./data_sets.txt";
	
	//Read_info r( file_path );
	
	Read_info r( (char*)file_path.c_str() );

	map< string, vector<double>* >::iterator tt, dy, wjets, vv, st, qcd;
	tt = Weights_map.find("TT");		dy = Weights_map.find("DY");
	wjets = Weights_map.find("WJets");	vv = Weights_map.find("VV");
	st = Weights_map.find("ST");		qcd = Weights_map.find("QCD");

	vector< map<string, vector<double>* >::iterator > its;
	its.push_back(tt);	its.push_back(dy);	its.push_back(wjets);
	its.push_back(vv);	its.push_back(st);	its.push_back(qcd);
	
	vector<string> sets_name{ "TT", "DY", "WJets", "VV", "ST", "QCD" };

    vector<string> s_TT{ "001" };
    vector<string> s_DY{ "70-100", "100-200", "200-400", "400-600", "600-800", "800-1200", "1200-2500", "2500-Inf" };
    vector<string> s_WJets{  "100-200", "200-400", "400-600", "600-800", "800-1200", "1200-2500", "2500-Inf" };	//"70-100" have no Xsec
    vector<string> s_VV{ "WW", "WZ", "ZZ" };
    vector<string> s_ST{ "s", "t_antitop", "t_top", "tW_antitop", "tW_top" };
    vector<string> s_QCD{ "100-200", "200-300", "300-500", "500-700", "700-1000", "1000-1500", "1500-2000", "2000-Inf" }; //"50-100" have no Xsec

	vector< vector<string>* > sets;
	sets.push_back(&s_TT);	sets.push_back(&s_DY);	sets.push_back(&s_WJets);
	sets.push_back(&s_VV);	sets.push_back(&s_ST);	sets.push_back(&s_QCD);

	vector<string> option;
	option.push_back("luminosity");		option.push_back("path");
	option.push_back("Xsec");		option.push_back("k-fac");
	option.push_back("gen_no");
	
	for(int i=0;i<(int)sets.size();i++)
	{
		for(int j=0;j<(int)sets.at(i)->size();j++)
		{
			string key_lumi = sets_name[i] + "_" + sets.at(i)->at(j) + "_" + option.at(0);
			string key_Xsec = sets_name[i] + "_" + sets.at(i)->at(j) + "_" + option.at(2);
			string key_k_fac = sets_name[i] + "_" + sets.at(i)->at(j) + "_" + option.at(3);
			string key_gen_no = sets_name[i] + "_" + sets.at(i)->at(j) + "_" + option.at(4);
			
			double weight = std::stod( r.second(key_lumi) ) * std::stod( r.second(key_Xsec) )\
						   	* std::stod( r.second(key_k_fac) ) / std::stod( r.second(key_gen_no) ) ;

			its.at(i)->second->push_back((double)weight);
		}
	}    	
}

//void get_path( map< string, vector<char*>* >& Data_Set_Path)
void get_path( map< string, vector<string>* >& Data_Set_Path , string file_path = "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/selected/info./pre_sel_data.txt")
{
	//char file_path[500] = "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/selected/info./data_sets.txt";
	//Read_info r( file_path );
	
	Read_info r( (char*)file_path.c_str() );
	
	//map< string, vector<char*>* >::iterator tt, dy, wjets, vv, st, qcd, data_sm, data_se;
	map< string, vector<string>* >::iterator tt, dy, wjets, vv, st, qcd, data_sm, data_se;
	tt = Data_Set_Path.find("TT");			dy = Data_Set_Path.find("DY");
	wjets = Data_Set_Path.find("WJets");	vv = Data_Set_Path.find("VV");
	st = Data_Set_Path.find("ST");			qcd = Data_Set_Path.find("QCD");
	data_sm = Data_Set_Path.find("Data_SM");data_se = Data_Set_Path.find("Data_SE");


	//vector< map<string, vector<char*>* >::iterator > its;
	vector< map<string, vector<string>* >::iterator > its;
	its.push_back(tt);	its.push_back(dy);	its.push_back(wjets);
	its.push_back(vv);	its.push_back(st);	its.push_back(qcd);
	its.push_back(data_sm);		its.push_back(data_se);
	
	vector<string> sets_name{ "TT", "DY", "WJets", "VV", "ST", "QCD", "Data_SM", "Data_SE" };

    vector<string> s_TT{ "001" };
    vector<string> s_DY{ "70-100", "100-200", "200-400", "400-600", "600-800", "800-1200", "1200-2500", "2500-Inf" };
    vector<string> s_WJets{  "100-200", "200-400", "400-600", "600-800", "800-1200", "1200-2500", "2500-Inf" };
    vector<string> s_VV{ "WW", "WZ", "ZZ" };
    vector<string> s_ST{ "s", "t_antitop", "t_top", "tW_antitop", "tW_top" };
    vector<string> s_QCD{  "100-200", "200-300", "300-500", "500-700", "700-1000", "1000-1500", "1500-2000", "2000-Inf" };

	vector<string> s_Data_SM{ "B", "C", "D", "E", "F", "G", "H" };
	vector<string> s_Data_SE{ "B", "C", "D", "E", "F", "G", "H" };

	vector< vector<string>* > sets;
	sets.push_back(&s_TT);	sets.push_back(&s_DY);	sets.push_back(&s_WJets);
	sets.push_back(&s_VV);	sets.push_back(&s_ST);	sets.push_back(&s_QCD);
	sets.push_back(&s_Data_SM);		sets.push_back(&s_Data_SE);

	vector<string> option;
	option.push_back("luminosity");		
	option.push_back("path");
	option.push_back("Xsec");		
	option.push_back("k-fac");
	option.push_back("gen_no");

	for(int i=0;i<(int)sets.size();i++)
	{
		for(int j=0;j<(int)sets.at(i)->size();j++)
		{
			string key_path = sets_name.at(i) + "_" + sets.at(i)->at(j) + "_" + option[1];

			string path = r.second( key_path );
			//char* c_path = new char[ (int)path.size()+10 ];
			//c_path = (char*)path.c_str();
			//its.at(i)->second->push_back( c_path );
			its.at(i)->second->push_back( path );
		}
	}    	
}

void get_weight_map( map<string, vector<double>*>& m )
{
	char file_path[500] = "/wk_cms2/cychuang/CMSSW_9_2_3/src/TopCPViolation/selected/info./test_data_sets.txt";
	Read_info r( file_path );
	
	map<string,vector<double>*>::iterator it1, it2;
	it1 = m.find("w01");	it2 = m.find("w02");

	//cout << *it1 << "  " << *it2 << endl << endl;
	
	vector<map<string,vector<double>*>::iterator> its;
	its.push_back(it1);	its.push_back(it2);
	
	vector<string> sets_name{"w01","w02"};

	vector<string> s_w01{"1-1","1-2"};
	vector<string> s_w02{"2-1","2-2","2-3"};
	
	vector< vector<string>* > sets;
	sets.push_back( &s_w01 );	sets.push_back( &s_w02 );

	vector<string> option;
	string aa = "path";
	option.push_back("name");	option.push_back(aa);

	for(int i=0;i<(int)sets.size();i++)
	{
		for(int j=0;j<(int)sets[i]->size();j++)
		{
			string dash = "_";
			string key_path = sets_name.at(i) + dash + sets[i]->at(j) + dash + option[1];
			cout << "key_path  " << key_path << endl;
			string path = r.second( key_path );
			cout << "path  " << path << endl;
			its.at(i)->second->push_back( std::stod(path) );
		}
	}    	

}

#endif
