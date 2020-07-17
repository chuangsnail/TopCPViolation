#ifndef MCTRUTH_H
#define MCTRUTH_H

#include "TopCPViolation/selected/interface/BranchInfo.h"

#include <iostream>
#include <limits.h>		//INT_MAX
#include <float.h>		//DBL_MAX
#include <cmath>

#include "TLorentzVector.h"
#include "TVector3.h"
#include "TVector2.h"

//gen_eta and gen_phi is the gen information stored in JetInfo and LeptonInfo classes
//The return back idx is the corresponding idx in Geninfo's array
int Match_Gen(double gen_eta,double gen_phi,GenInfo& gen)
{
	int idx = -1;
	double delta_R = DBL_MAX;

	for(int i=0; i<(int)gen.Size ;i++)
	{
		double d_eta = gen_eta - gen.Eta[i];
		double d_phi  = TVector2::Phi_mpi_pi(gen_phi - gen.Phi[i]);
		double delta_R_tmp = TMath::Sqrt( d_eta * d_eta + d_phi * d_phi);

		if( delta_R_tmp < delta_R && delta_R_tmp < 0.4 )
		{
			delta_R = delta_R_tmp;
			idx = i;
		}
	}

	return idx;
}

//Input the idx of JetInfo and return the idx of genInfo
int GenJetIdx( int idx, JetInfo& jets, GenInfo& gen )
{
	return Match_Gen( jets.GenEta[idx], jets.GenPhi[idx], gen );
}

//Input the idx of LeptonInfo and return the idx of genInfo
int GenLepIdx( int idx, LeptonInfo& leps, GenInfo& gen )
{
	return Match_Gen( leps.GenEta[idx], leps.GenPhi[idx], gen );
}

//input pdgid and get the index of geninfo, if there is no this particle in the event,return index -1
int FindGenJet( const int& id_pdg , GenInfo& gen )
{
	int idx = -1;
	for(int i=0;i<gen.Size;i++)
	{
		if( gen.PdgID[i] == id_pdg )
		{
			if( fabs(gen.Mo1PdgID[i]) == 6 || fabs(gen.Mo2PdgID[i]) == 6 )
			{
				idx = i;
				break;	//break out of the for loop
			}
		}
	}
	return idx;
}

int Find_GenParticle( const int& id_pdg, const int& mo_id, GenInfo& gen )
{
	int idx = -1;
	for(int i=0;i<gen.Size;i++)
	{
		if( gen.PdgID[i] == id_pdg )
		{
			if( gen.Mo1PdgID[i] == mo_id )
			{
				idx = i;
				break;	//break out of the for loop
			}
		}
	}
	return idx;
}


void dumpinfo(GenInfo& gen)
{
	for(int i=0;i<gen.Size;i++)
	{
		cout << " idx:" << i ;
		cout << " pdgid:" << gen.PdgID[i];
		cout << " Mo1:" << gen.Mo1[i] << " Mo2:" << gen.Mo2[i] << endl;
	}
}


#endif
