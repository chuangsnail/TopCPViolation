#ifndef TRIGGER_H
#define TRIGGER_H

#include "TopCPViolation/selected/interface/BranchInfo.h"

#include <vector>
#include <string>
#include "string.h"
#include <map>
#include <iostream>

using namespace std;

//trigger part

void Get_HLT_Info( string key, vector<int>& HLT_info );

bool Pass_HLT( TrgInfo& evt, const vector<int>& HLT_info );

double lumi_cali_trg(double frac);

double trigger_calib( const string& channel = "no_calib" );

//vertex part

bool IsGoodPVertex( VertexInfo* vtx, const int& idx )
{
	return ( !vtx->isFake[ idx ] 		&&
			vtx->Ndof[idx] > 4			&&
			fabs( vtx->z[idx] ) < 24	&&
			vtx->Rho[idx] < 2 );
}

bool PassGoodVtx( VertexInfo* vtx )
{
	for(int i=0;i<(int)vtx->Size;++i)
	{
		if( IsGoodPVertex( vtx, i ) ) return true;
	}
	return false;
}

#endif
