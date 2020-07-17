#ifndef JET_SEL_H
#define JET_SEL_H

#include "TopCPViolation/selected/interface/BranchInfo.h"
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <vector>
#include "TLorentzVector.h"		//for deltaR
#include "TVector2.h"
#include "TVector3.h"

/****
 *	
 *	CSV and deepCSV value is for  RERECO version data/MC
 *
 * **/


using namespace std;

bool Pass_Loose_Jet( JetInfo& jet, const int& idx );

bool Pass_Selected_Jet( JetInfo& jet, const int& idx);

bool Pass_SR_Selected_Jets_Case( JetInfo& jets, vector<int>& sel_jets_idx );

bool Pass_Selected_Jet_woE( JetInfo& jet, const int& idx);

bool Pass_SR_Selected_Jets_Case_woE( JetInfo& jets, vector<int>& sel_jets_idx );

//For generally 2 direction of any 2 things 
double delta_R( const double& eta1, const double& eta2, const double& phi1,const double& phi2 );

bool Pass_delR_Jet_Lep( JetInfo& jets, LeptonInfo& lep, vector<int>& sel_jets_idx, const int& sel_lep_idx );

double CSV_value( JetInfo& jets , const int& idx );

double deepCSV_value( JetInfo& jets, const int& idx );

bool is_CSV_Medium_bjet( JetInfo& jets, const int& idx );

bool is_CSV_Loose_bjet( JetInfo& jets, const int& idx );

bool is_deepCSV_Tight_bjet( JetInfo& jets, const int& idx );

bool is_deepCSV_Medium_bjet( JetInfo& jets, const int& idx );

bool is_deepCSV_Loose_bjet( JetInfo& jets, const int& idx );

bool Pass_SR_bjets( JetInfo& jets, vector<int>& sel_jets_idx, vector<int>& b_jets_idx);

void Bigger_Order( double& x1, double& x2 );

int CSV_to_idx( JetInfo& jets, double csv_value );

int deepCSV_to_idx( JetInfo& jets, double& csv_value );


//b_idx here means that the 2 highest CSV value jets in the pass case(with no selected b-jets in CR)
bool Pass_CR_bjets( JetInfo& jets, vector<int>& sel_jets_idx, vector<int>& b_jets_idx);

//Factorial (used in chi2 sorting)
double factorial(int x);
//I think that it would be better to use while loop?!?

//input jets_idx


double chi_2_value( JetInfo& jet,const vector<int>& sel_jets_idx,const vector<int>& b_jets_idx, const int& idx_j1, const int& idx_j2, const int& idx_b  );

//use chi-square sorting to choose good jj choice to be W and good jjb choice to be top
double Chi2_Sorting(JetInfo& jet,vector<int>& sel_jets_idx,vector<int>& b_jets_idx);
//now the jets_idx is jets without b-tagged jet
//'evt' means the index which is in an event

#endif
