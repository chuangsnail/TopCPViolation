/*
 *
 *	File_name : reco_method_workframe.h
 *	Description : to make a env to work for reco top method
 *
 *
 *
 */

#ifndef RECO_METHOD_WORKFRAME_H
#define RECO_METHOD_WORKFRAME_H

#include "TopCPViolation/selected/interface/BranchInfo.h"
#include "TopCPViolation/prepare_info/interface/about_time.h"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <map>
#include <iterator>

class RecoMgr
{
public:
	
	GenInfo& gen;
	JetInfo& jets;
	LeptonInfo& leps;

	RecoMgr( GenInfo& g, JetInfo& j, LeptonInfo& l );
};


RecoMgr::RecoMgr( GenInfo& g, JetInfo& j, LeptonInfo& l )
{
	gen = g;	jets = j;	leps = l;
}

RecoMgr::AddBranch( string branch_name, const double& value, string file_name = "" )
{
	string branch_des = branch_name + "\D";

	TTree* cor = (TTree*)f_old->Get("correct");
	TTree* incor = (TTree*)f_old->Get("incorrect");

	double tmp_space;
	TFile* f_old = new TFile( (char*)file_name.c_str() ,"update" );
	

}


#endif

