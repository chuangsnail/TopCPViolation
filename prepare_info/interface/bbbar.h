#ifndef BBBAR_H
#define BBBAR_H

#include "TopCPViolation/selected/interface/MCTruth.h"

enum bb_matching_type { correct , charge_mis , mistag , no_match };
//{ 0 , 1 , 2 , 3 }
//input the decter-level-simulated-selected b index and bbar index and lep index
bb_matching_type Get_bb_Option( int b, int bbar, int lep, JetInfo& jets, LeptonInfo& leps, GenInfo& gen )
{
	int GEN_b = -1;
	int GEN_bbar = -1;
	
	GEN_b = FindGenJet( 5, gen );			//5 is the pdgID of b
	GEN_bbar = FindGenJet( -5, gen );		//-5 is the pdgID of bbar

	//there is exactly no this channel(semileptonic decay) in the simualated event
	if( (GEN_b == -1) || (GEN_bbar == -1) )
	{
		return no_match;
	}

	//To match b and bbar to Geninfo
	
	int gen_b = GenJetIdx( b, jets, gen );			//Get the b-quark we determined before the fn by sign of charge
	int gen_bbar = GenJetIdx( bbar, jets, gen );	//Get the anti-b-quark we determined before the fn by sign of charge

	//cout << "start!" << endl;
	//cout << "GEN_b " << GEN_b << " gen_b " << gen_b <<endl;
	//cout << "GEN_bbar " << GEN_bbar << " gen_bbar " << gen_bbar << endl;
	//dumpinfo(gen);

	if( (gen_b == -1) || (gen_bbar == -1) )
	{
		return no_match;
	}

	if(	(gen_b == GEN_b) && (gen_bbar == GEN_bbar) )
	{
		return correct;
	}
	else if( (gen_b == GEN_bbar) && (gen_bbar == GEN_b) )
	{
		return charge_mis;
	}
	else
	{
		return mistag;
	}

}

#endif
