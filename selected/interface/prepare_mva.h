#ifndef PREPARE_MVA_H
#define PREPARE_MVA_H

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
    
    //member & member function for MVA combination choosing

};


int GenMgr::Match_Gen( double gen_eta,double gen_phi )
{
    int idx = -1;
    double delta_R = DBL_MAX;
    
    for(int i=0; i<(int)gen->Size ;i++)
    {
        double d_eta = gen_eta - gen->Eta[i];
        double d_phi  = TVector2::Phi_mpi_pi(gen_phi - gen->Phi[i]);
        double delta_R_tmp = TMath::Sqrt( d_eta * d_eta - d_phi * d_phi);
        
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

	//int nonbtag;
   
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

		//nonbtag = 0;
		//c_check_lep = 0;
		//c_check_bb = 0;
		//c_check_jj = 0;
    }
    
    ~MVAMgr()
    {
        sel_jets.clear();
        sel_b_jets.clear();
    }
    
    void clean();
    void Get_selected_info( const vector<int>& sel_j, const vector<int>& sel_b, const int& l );
    void Get_lep_tagged_flavor( string flav ){  lep_flav = flav;    }
    
    bool Find_Correct_HadronicTop( int& cor_b, int& cor_j1, int& cor_j2 );

	//for checking the Find_Correct_HadronicTop()
	//int c_check_lep;
	//int c_check_bb;
	//int c_check_jj;
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


bool MVAMgr::Find_Correct_HadronicTop( int& cor_b, int& cor_j1, int& cor_j2 )
{
    //first check if the lepton is choosen correctly
	
	if( sel_lep == -1 )
	{
		cerr << "----[Warning]---- You need to set up the lepton idx in MVAMgr object for calculation!" << endl;
		return false;
	}
    
    double lep_charge = leps->Charge[ sel_lep ];
    
    int tag_lep_in_gen = GenLepIdx( sel_lep );         //idx in GenInfo
	if( tag_lep_in_gen == -1 )
    {   return false;   }

   	//That is not good because the direct mother of it is usually its propagating
    if( lep_flav == "mu" ){
        if( lep_charge >= 0. ){
            if( !( Find_GenParticle( -13, 24 ) == tag_lep_in_gen ) ){      //anti lepton is from W+
                return false;
            }
        }
        else{       //charge < 0
            if( !( Find_GenParticle( 13, -24 ) == tag_lep_in_gen ) ){      //lepton is from W-
                return false;
            }
        }
    }
    else if( lep_flav == "el" ){
        if( lep_charge >= 0. ){
            if( !( Find_GenParticle( -11, 24 ) == tag_lep_in_gen ) ){
                return false;
            }
        }
        else{       //charge < 0
            if( !( Find_GenParticle( 11, -24 ) == tag_lep_in_gen ) ){
                return false;
            }
        }
    }
    else{
        cerr << "[---WARNING---] You didn't assign a lepton flavor info in the MVAMgr obj" << endl;
        return false;
    }

/*
	int corresponding_pdg = gen->PdgID[ tag_lep_in_gen ];
	if( lep_flav == "mu" )
	{	
		if( lep_charge >= 0. )
		{
			if( !(corresponding_pdg == -13) )
			{	return false;	}
			//if( !Check_Source_Mo( tag_lep_in_gen, 24 ) )
			//{	return false;	}	
		}
		else
		{
			if( !(corresponding_pdg == 13) )
			{	return false;	}	 
			//if( !Check_Source_Mo( tag_lep_in_gen, -24 ) )
			//{	return false;	}	
		}
	}
	else if( lep_flav == "el" )
	{	
		if( lep_charge >= 0. )
		{
			if( !(corresponding_pdg == -11) )
			{	return false;	}	
			//if( !Check_Source_Mo( tag_lep_in_gen, 24 ) )
			//{	return false;	}	
		}
		else
		{
			if( !(corresponding_pdg == 11) )
			{	return false;	}	 
			//if( !Check_Source_Mo( tag_lep_in_gen, -24 ) )
			//{	return false;	}	
		}
	}
	else
	{
		cerr << "[---WARNING---] You didn't assign a lepton flavor info in the MVAMgr obj" << endl;
        return false;	
	}
	*/

	//c_check_lep++;
    
    //because if the lepton's charge is determined, then the b and bbar are certainly assigned to leptonic or hadronic branch
    
    //if lepton is right
    //check the b-tag is correctly tagged ( b and bbar )
    
    int real_gen_b = Find_GenParticle( 5, 6 );            //idx in GenInfo
    int real_gen_bbar = Find_GenParticle( -5, -6 );
    
    if( real_gen_b == -1 || real_gen_bbar == -1 )       //that means there are no b or bbar in this selected event
    {   return false;   }
    
	//check btag
	if( fabs(jets->GenPdgID[ sel_b_jets.at(0) ]) != 5 || fabs(jets->GenPdgID[ sel_b_jets.at(1) ]) != 5 )
	{
		//cout << "In this event, b-tag is not good!" << endl;
		//nonbtag++;
		//return false;
	}
    
	int tag_b1 = GenJetIdx( sel_b_jets.at(0) );           //idx in GenInfo
    int tag_b2 = GenJetIdx( sel_b_jets.at(1) );
    
    if( (tag_b1 == -1) || (tag_b2 == -1) ){         //that means b-tag is not good
        return false;
    }
    
    if( real_gen_b == tag_b1 && real_gen_bbar == tag_b2 ){
        if( lep_charge >= 0. ){            //leptonic branch is W+ and b
            cor_b = sel_b_jets.at(1);
        }
        else{           //leptonic branch is W- and bbar
            cor_b = sel_b_jets.at(0);
        }
    }
    else if( real_gen_b == tag_b2 && real_gen_bbar == tag_b1 ){
        if( lep_charge >= 0. ){            //leptonic branch is W+ and b
            cor_b = sel_b_jets.at(0);
        }
        else{           //leptonic branch is W- and bbar
            cor_b = sel_b_jets.at(1);
        }
    }
    else{
        return false;
    }
    
	//c_check_bb++;

    //if lepton and 2 b are right
    //loop the combination and see which are j1, j2
    
    for(int i=0;i<(int)sel_jets.size();i++)
    {
        int tmp_j1 = sel_jets.at(i);
        for(int j=i+1;j<(int)sel_jets.size();j++)
        {
            int tmp_j2 = sel_jets.at(j);

            int gen_tmp_j1 = GenJetIdx( tmp_j1 );		//return idx in GenInfo
            int gen_tmp_j2 = GenJetIdx( tmp_j2 );
            
            if( gen_tmp_j1 == -1 || gen_tmp_j2 == -1 )
            {   continue;  }
            
            if( lep_charge >= 0. )
            {
                if( !(  gen->Mo1PdgID[ gen_tmp_j1 ] == -24 || gen->Mo2PdgID[ gen_tmp_j1 ] == -24 ) )
                {   continue;   }
                if( !(  gen->Mo1PdgID[ gen_tmp_j2 ] == -24 || gen->Mo2PdgID[ gen_tmp_j2 ] == -24 ) )
                {   continue;   }
            }
            else
            {
                if( !(  gen->Mo1PdgID[ gen_tmp_j1 ] == 24 || gen->Mo2PdgID[ gen_tmp_j1 ] == 24 ) )
                {   continue;   }
                if( !(  gen->Mo1PdgID[ gen_tmp_j2 ] == 24 || gen->Mo2PdgID[ gen_tmp_j2 ] == 24 ) )
                {   continue;   }
            }
            cor_j1 = sel_jets.at(i);
            cor_j2 = sel_jets.at(j);
			//c_check_jj++;
            return true;
        }
    }

    return false;
}

#endif
