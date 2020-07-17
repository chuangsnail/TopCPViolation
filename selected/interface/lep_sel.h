#ifndef LEP_SEL_H
#define LEP_SEL_H

#include "TopCPViolation/selected/interface/BranchInfo.h"
#include <iostream>

using namespace std;

//Muon selection

bool Pass_sel_Mu_ISO( LeptonInfo& lep, const int& i );

bool Pass_veto_Mu_ISO( LeptonInfo& lep, const int& i );

bool is_Global_Mu( LeptonInfo& lep, const int& i );

bool is_Tracker_Mu( LeptonInfo& lep, const int& i );

bool Pass_tightMu_ID( LeptonInfo& lep, const int& i );

bool Pass_looseMu_ID( LeptonInfo& lep, const int& i );

bool Pass_sel_Mu_Kinematic( LeptonInfo& lep, const int& i );

bool Pass_veto_Mu_Kinematic( LeptonInfo& lep, const int& i );

bool Pass_SR_Selected_Muon( LeptonInfo& leps,int& index );

bool Pass_SR_Veto_Muon(LeptonInfo& leps,int sel_mu_idx);

bool Pass_CR_Selected_Muon( LeptonInfo& leps,int& index );

bool Pass_CR_Veto_Muon(LeptonInfo& leps,int sel_mu_idx);

//Electron selection

bool Pass_CR_tightEl_ID( LeptonInfo& lep, const int& i, EvtInfo& evt );

bool Pass_CR_looseEl_ID( LeptonInfo& lep, const int& i, EvtInfo& evt );

double El_PFISO( LeptonInfo& lep, const int& i );

bool Pass_tight_El_ISO( LeptonInfo& lep, const int& i );

bool Pass_SR_tightEl_ID( LeptonInfo& lep, const int& i );

bool Pass_SR_looseEl_ID( LeptonInfo& lep, const int& i );

bool Pass_sel_El_Kinematic( LeptonInfo& lep, const int& i );

bool Pass_veto_El_Kinematic( LeptonInfo& lep, const int& i );

//just used in tight El
bool Pass_ImpactParameter( LeptonInfo& lep, const int& i );

bool Pass_SR_Selected_Electron(LeptonInfo& leps,int& index );

bool Pass_CR_Selected_Electron( LeptonInfo& leps, int& index, EvtInfo& evt );

bool Pass_SR_Veto_Electron( LeptonInfo& leps,int sel_el_idx );

bool Pass_CR_Veto_Electron( LeptonInfo& leps, int sel_el_idx, EvtInfo& evt );

//for CR datadriven QCD

bool Pass_CR_InvSelected_Muon( LeptonInfo& leps,int& index );

bool Pass_CR_InvSelected_Electron( LeptonInfo& leps, int& index, EvtInfo& evt );


//for pre pre selection

bool Pass_presel_Selected_Muon( LeptonInfo& leps,int& index );

bool Pass_presel_Selected_Electron( LeptonInfo& leps, int& index, EvtInfo& evt );

void ElEnergy_Cor( LeptonInfo* leps );

#endif
