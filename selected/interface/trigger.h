#ifndef TRIGGER_H
#define TRIGGER_H

#include "TopCPViolation/selected/interface/BranchInfo.h"

#include <vector>
#include <string>
#include "string.h"
#include <map>
#include <iostream>

using namespace std;

void Get_HLT_Info( string key, vector<int>& HLT_info );

bool Pass_HLT( TrgInfo& evt, const vector<int>& HLT_info );

double lumi_cali_trg(double frac);

double trigger_calib( const string& channel = "no_calib" );

#endif
