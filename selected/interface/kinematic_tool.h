#ifndef KINEMATIC_TOOL_H
#define KINEMATIC_TOOL_H

#include <cmath>
#include <iostream>

#include "TopCPViolation/selected/interface/BranchInfo.h"

#include "TLorentzVector.h"
#include "TVector3.h"
#include "TVector2.h"

using namespace std;

TLorentzVector GetJetP4( JetInfo& jets, const int& idx )
{
	TLorentzVector v;
	v.SetPxPyPzE( jets.Px[idx], jets.Py[idx], jets.Pz[idx], jets.Energy[idx] );
	return v;

}	

TLorentzVector GetLepP4( LeptonInfo& leps, const int& idx )
{
	TLorentzVector v;
	v.SetPxPyPzE( leps.Px[idx], leps.Py[idx], leps.Pz[idx], leps.Energy[idx] );
	return v;
}




#endif
