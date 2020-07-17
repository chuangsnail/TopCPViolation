#ifndef GENMGR_H
#define GENMGR_H

#include "TopCPViolation/selected/interface/BranchInfo.h"
#include "TopCPViolation/selected/interface/MCTruth.h"

#include <limits.h>        //INT_MAX
#include <float.h>        //DBL_MAX
#include <cmath>
#include <iostream>
#include <vector>
#include <string>

#include "TLorentzVector.h"
#include "TVector3.h"
#include "TVector2.h"

class GenMgr
{
public:
    
    //members
    GenInfo* gen;
    JetInfo* jets;
    LeptonInfo* leps;
    
    //Constructors
	/*
    GenMgr( GenInfo* genInfo )
    {   gen = genInfo;  }
    */

    GenMgr( GenInfo* genInfo, JetInfo* jetInfo, LeptonInfo* leptonInfo )
    {
        gen = genInfo;
        jets = jetInfo;
        leps = leptonInfo;
    }
    
    //Member fns
    
    void SetGenBranch( GenInfo* );
    void SetJetBranch( JetInfo* );
    void SetLepBranch( LeptonInfo* );
    int Find_GenParticle( const int& id_pdg, const int& mo_id );
    int Match_Gen( double gen_eta, double gen_phi );
    int GenJetIdx( int idx );       //input the idx in JetInfo, get the index in GenInfo, If there is no match , return -1
    int GenLepIdx( int idx );       //input the idx in LeptonInfo, get the index in GenInfo, If there is no match , return -1
	bool Check_Source_Mo( const int& gen_idx, const int& sourc_mo_pdgid );		//find the mother after checking its propagation
	void PrintInfo();
	bool is_Gen_Jet_Seen( const int& idx );
	bool is_Gen_Lep_Seen( const int& idx );
    
    //member & member function for MVA combination choosing
	bool Find_Correct_HadronicTop( int& cor_b, int& cor_j1, int& cor_j2 );

};

bool GenMgr::is_Gen_Jet_Seen( const int& idx )
{
    double delta_R = DBL_MAX;
	for(int i=0;i<jets->Size;i++)
	{
		double gen_eta = jets->GenEta[i];
		double gen_phi = jets->GenPhi[i];
		
        double d_eta = gen_eta - gen->Eta[idx];
        double d_phi  = TVector2::Phi_mpi_pi(gen_phi - gen->Phi[idx]);
        double delta_R_tmp = TMath::Sqrt( d_eta * d_eta + d_phi * d_phi);
        
        if( delta_R_tmp < delta_R && delta_R_tmp < 0.4 )
        {
            delta_R = delta_R_tmp;
			return true;
        }
	}
	return false;
}

bool GenMgr::is_Gen_Lep_Seen( const int& idx )
{
    double delta_R = DBL_MAX;
	for(int i=0;i<leps->Size;i++)
	{
		double gen_eta = leps->GenEta[i];
		double gen_phi = leps->GenPhi[i];
		
        double d_eta = gen_eta - gen->Eta[idx];
        double d_phi  = TVector2::Phi_mpi_pi(gen_phi - gen->Phi[idx]);
        double delta_R_tmp = TMath::Sqrt( d_eta * d_eta + d_phi * d_phi);
        
        if( delta_R_tmp < delta_R && delta_R_tmp < 0.4 )
        {
            delta_R = delta_R_tmp;
			return true;
        }
	}
	return false;
}


int GenMgr::Match_Gen( double gen_eta,double gen_phi )
{
    int idx = -1;
    double delta_R = DBL_MAX;
    
    for(int i=0; i<(int)gen->Size ;i++)
    {
        double d_eta = gen_eta - gen->Eta[i];
        double d_phi  = TVector2::Phi_mpi_pi(gen_phi - gen->Phi[i]);
        double delta_R_tmp = TMath::Sqrt( d_eta * d_eta + d_phi * d_phi);
        
        if( delta_R_tmp < delta_R && delta_R_tmp < 0.4 )
        {
            delta_R = delta_R_tmp;
            idx = i;
        }
    }
    
    return idx;
}


int GenMgr::Find_GenParticle( const int& id_pdg, const int& mo_id )
{
    int idx = -1;
    for(int i=0;i<(int)gen->Size;i++)
    {
        if( gen->PdgID[i] == id_pdg )
        {
			//just find the first one in an event
			//the later others are its propagation
            if( gen->Mo1PdgID[i] == mo_id || gen->Mo2PdgID[i] == mo_id )
            {
                idx = i;
                break;    //break out of the for loop
            }
        }
    }
    return idx;
}

int GenMgr::GenJetIdx( int idx )
{
    return Match_Gen( jets->GenEta[idx], jets->GenPhi[idx] );
}

//Input the idx of LeptonInfo and return the idx of genInfo
int GenMgr::GenLepIdx( int idx )
{
    return Match_Gen( leps->GenEta[idx], leps->GenPhi[idx] );
}

bool GenMgr::Check_Source_Mo( const int& gen_idx, const int& sourc_mo_pdgid )
{
	int self_pdg_id = gen->PdgID[ gen_idx ];
	int find_pin = gen_idx;
	for(int i=0;i<gen->Size;i++)
	{		
		if( gen->Mo1PdgID[find_pin] == sourc_mo_pdgid || gen->Mo2PdgID[find_pin] == sourc_mo_pdgid )
		{	
			return true;
		}
		if( gen->Mo1PdgID[find_pin] == self_pdg_id || gen->Mo2PdgID[find_pin] == self_pdg_id )
		{
			if( gen->nMo[find_pin] == 1 )
			{
				int next_find_pin = gen->Mo1[ find_pin ];
				find_pin = next_find_pin;
			}
			else if( gen->nMo[find_pin] >= 2 )
			{
				if( gen->Mo1PdgID[find_pin] == self_pdg_id )
				{
					int next_find_pin = gen->Mo1[ find_pin ];
					find_pin = next_find_pin;
					continue;
				}
				if( gen->Mo2PdgID[find_pin] == self_pdg_id )
				{
					int next_find_pin = gen->Mo2[ find_pin ];
					find_pin = next_find_pin;
					continue;
				}
			}
		}
		return false;
	}
	return false;
}


void GenMgr::SetGenBranch( GenInfo* g ){    gen = g;    }
void GenMgr::SetJetBranch( JetInfo* j ){    jets = j;   }
void GenMgr::SetLepBranch( LeptonInfo* l ){ leps = l;   }

void GenMgr::PrintInfo()
{
}



class MVAMgr : public GenMgr
{
public:
    
    vector<int> sel_jets;
    vector<int> sel_b_jets;
    int sel_lep;
    string lep_flav ;

   /*	
    MVAMgr( GenInfo* g ) : GenMgr( g )
    {
        sel_jets.assign(20,-1);
        sel_b_jets.assign(5,-1);
        lep = -1;
        lep_flav = "mu";
    }
	*/
    
    MVAMgr( GenInfo* g, JetInfo* j, LeptonInfo* l ) : GenMgr( g, j, l )
    {
        sel_jets.assign(20,-1);
        sel_b_jets.assign(5,-1);
        sel_lep = -1;
        lep_flav = "mu";

    }
    
    ~MVAMgr()
    {
        sel_jets.clear();
        sel_b_jets.clear();
    }
    
    void clean();
    void Get_selected_info( const vector<int>& sel_j, const vector<int>& sel_b, const int& l );
    void Get_lep_tagged_flavor( string flav ){  lep_flav = flav;    }
    
    //bool Find_Correct_HadronicTop( int& cor_b, int& cor_j1, int& cor_j2 );

};

void MVAMgr::clean()
{
    sel_jets.clear();
    sel_b_jets.clear();
    sel_jets.assign(20,-1);
    sel_b_jets.assign(5,-1);
    sel_lep = -1;
}


void MVAMgr::Get_selected_info( const vector<int>& sel_j, const vector<int>& sel_b, const int& l )
{
    sel_jets.resize( (int)sel_j.size() );
    sel_b_jets.resize( (int)sel_b.size() );
    
    for(int i=0;i<(int)sel_j.size();i++)
    {
        sel_jets.at(i) = sel_j.at(i);
    }
    for(int i=0;i<(int)sel_b.size();i++)
    {
        sel_b_jets.at(i) = sel_b.at(i);
    }
    sel_lep = l;
}


bool GenMgr::Find_Correct_HadronicTop( int& cor_b, int& cor_j1, int& cor_j2 )
{
	//check if there is a lepton( muon or electron ), can be seen
	int lepl_no = 0;
	int lep_idx = -1;
	for(int i=0;i<gen->Size;i++)
	{
		if( fabs(gen->PdgID[i]) == 11 || fabs(gen->PdgID[i]) == 13 )
		{
			if( fabs(gen->Mo1PdgID[i]) == 24 || fabs(gen->Mo2PdgID[i]) == 24 )
			{	
				if( is_Gen_Lep_Seen( i ) )
				{
					lepl_no++;
					lep_idx = i;
				}
			}
		}
	}
	if( lepl_no != 1 )
	{	return false;	}	

	double lep_charge;
	if( gen->PdgID[lep_idx] > 0 )		//charge < 0
	{
		lep_charge = -1.;
	}
	else
	{
		lep_charge = 1.;
	}
   	
   	//check 2 b-quark can be seen

    int real_gen_b = Find_GenParticle( 5, 6 );            //idx in GenInfo
    int real_gen_bbar = Find_GenParticle( -5, -6 );
    
    if( real_gen_b == -1 || real_gen_bbar == -1 )       //that means there are no b or bbar in this selected event
    {   return false;   }

	if( !( is_Gen_Jet_Seen( real_gen_b ) && is_Gen_Jet_Seen( real_gen_bbar ) ) )
	{	return false;	}
    
	//check 2 jets from W can be seen

	vector<int> hadj;
	hadj.assign(10,0);
	int hadj_no = 0;
	hadj.clear();

	int w_pdg = 24;			//for hadronic branch W
	if( lep_charge > 0 )	//leptonic branch is W-plus
	{
		w_pdg = -24;	
	}

	for(int i=0;i<gen->Size;i++)
	{
		if( (  gen->Mo1PdgID[ i ] == w_pdg || gen->Mo2PdgID[ i ] == w_pdg ) )
		{
			if( is_Gen_Jet_Seen( i ) )
			{
				hadj_no++;
				hadj.push_back( i );
			}
		}
	}
	if( hadj_no != 2 )
	{	return false;	}	

	int hadb = -1;
	//to inspect which is hdronic b
	if( lep_charge > 0 )	//hadW is W-minus
	{
		hadb = real_gen_bbar;
	}
	else
	{
		hadb = real_gen_b;
	}

	//to find cor_b, cor_j1, cor_j2 , the idces in detector objects
	
    double delta_R = DBL_MAX;
	for(int i=0;i<jets->Size;i++)
	{
		double gen_eta = jets->GenEta[i];
		double gen_phi = jets->GenPhi[i];
		
        double d_eta = gen_eta - gen->Eta[ hadb ];
        double d_phi  = TVector2::Phi_mpi_pi(gen_phi - gen->Phi[ hadb ]);
        double delta_R_tmp = TMath::Sqrt( d_eta * d_eta + d_phi * d_phi);
        
        if( delta_R_tmp < delta_R && delta_R_tmp < 0.4 )
        {
            delta_R = delta_R_tmp;
            cor_b = i;
        }
	}

    delta_R = DBL_MAX;
	for(int i=0;i<jets->Size;i++)
	{
		double gen_eta = jets->GenEta[i];
		double gen_phi = jets->GenPhi[i];
		
        double d_eta = gen_eta - gen->Eta[ hadj.at(0) ];
        double d_phi  = TVector2::Phi_mpi_pi(gen_phi - gen->Phi[ hadj.at(0) ]);
        double delta_R_tmp = TMath::Sqrt( d_eta * d_eta + d_phi * d_phi);
        
        if( delta_R_tmp < delta_R && delta_R_tmp < 0.4 )
        {
            delta_R = delta_R_tmp;
            cor_j1 = i;
        }
	}

    delta_R = DBL_MAX;
	for(int i=0;i<jets->Size;i++)
	{
		double gen_eta = jets->GenEta[i];
		double gen_phi = jets->GenPhi[i];
		
        double d_eta = gen_eta - gen->Eta[ hadj.at(1) ];
        double d_phi  = TVector2::Phi_mpi_pi(gen_phi - gen->Phi[ hadj.at(1) ]);
        double delta_R_tmp = TMath::Sqrt( d_eta * d_eta + d_phi * d_phi);
        
        if( delta_R_tmp < delta_R && delta_R_tmp < 0.4 )
        {
            delta_R = delta_R_tmp;
            cor_j2 = i;
        }
	}

	if( cor_b == -1 || cor_j1 == -1 || cor_j2 == -1 )
	{
		cerr << "There is something wrong!" << endl;
		return false;
	}


    return true;
}

#endif
