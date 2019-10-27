#ifndef INFORMATION_H
#define INFORMATION_H

#include <iostream>
#include <vector>
#include "stdlib.h"
#include "stdio.h"
#include <string.h>
#include <cstring>
#include <vector>

using namespace std;

//** Original files' path **//

char TT_001[300] = "/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_001/bpk_ntuple_*.root";
char TT_002[300] = "/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_002/bpk_ntuple_*.root";
char TT_003[300] = "/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_003/bpk_ntuple_*.root";

//Drew-Yen
char DY_100_200[300] = "/wk_cms2/yichen/bpk_ntuple/DYJetsToLL_M-50_HT/BPK_80X_DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";
char DY_200_400[300] = "/wk_cms2/yichen/bpk_ntuple/DYJetsToLL_M-50_HT/BPK_80X_DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";
char DY_400_600[300] = "/wk_cms2/yichen/bpk_ntuple/DYJetsToLL_M-50_HT/BPK_80X_DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";
char DY_600_800[300] = "/wk_cms2/yichen/bpk_ntuple/DYJetsToLL_M-50_HT/BPK_80X_DYJetsToLL_M-50_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";
char DY_800_1200[300] = "/wk_cms2/yichen/bpk_ntuple/DYJetsToLL_M-50_HT/BPK_80X_DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";
char DY_1200_2500[300] = "/wk_cms2/yichen/bpk_ntuple/DYJetsToLL_M-50_HT/BPK_80X_DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";
char DY_2500_Inf[300] = "/wk_cms2/yichen/bpk_ntuple/DYJetsToLL_M-50_HT/BPK_80X_DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";

//W+Jets
char WJets_100_200[300] = "/wk_cms2/sam7k9621/WJetsToLNu_HT/BPK_80X_WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";
char WJets_200_400[300] = "/wk_cms2/sam7k9621/WJetsToLNu_HT/BPK_80X_WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";
char WJets_400_600[300] = "/wk_cms2/sam7k9621/WJetsToLNu_HT/BPK_80X_WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";
char WJets_600_800[300] = "/wk_cms2/sam7k9621/WJetsToLNu_HT/BPK_80X_WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";
char WJets_800_1200[300] = "/wk_cms2/sam7k9621/WJetsToLNu_HT/BPK_80X_WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";
char WJets_1200_2500[300] = "/wk_cms2/sam7k9621/WJetsToLNu_HT/BPK_80X_WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";
char WJets_2500_Inf[300] = "/wk_cms2/sam7k9621/WJetsToLNu_HT/BPK_80X_WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";

//2 Vector Bosons
char VV_WW[300] = "/wk_cms2/yichen/bpk_ntuple/VV/BPK_80X_WW_TuneCUETP8M1_13TeV-pythia8/bpk_ntuple_*.root";
char VV_WZ[300] = "/wk_cms2/yichen/bpk_ntuple/VV/BPK_80X_WZ_TuneCUETP8M1_13TeV-pythia8/bpk_ntuple_*.root";
char VV_ZZ[300] = "/wk_cms2/yichen/bpk_ntuple/VV/BPK_80X_ZZ_TuneCUETP8M1_13TeV-pythia8/bpk_ntuple_*.root";

//Single Top
char ST_s[300] = "/wk_cms2/yichen/bpk_ntuple/SingleTop/BPK_80X_ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/bpk_ntuple_*.root";
char ST_t_antitop[300] = "/wk_cms2/yichen/bpk_ntuple/SingleTop/BPK_80X_ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/bpk_ntuple_*.root";
char ST_t_top[300] = "/wk_cms2/yichen/bpk_ntuple/SingleTop/BPK_80X_ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/bpk_ntuple_*.root";
char ST_tW_antitop[300] = "/wk_cms2/yichen/bpk_ntuple/SingleTop/BPK_80X_ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/bpk_ntuple_*.root";
char ST_tW_top[300] = "/wk_cms2/yichen/bpk_ntuple/SingleTop/BPK_80X_ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/bpk_ntuple_*.root";

//QCD (gluons or any quarks)
///wk_cms2/sam7k9621
char QCD_100_200[300] = "/wk_cms2/sam7k9621/QCD/BPK_80X_QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";
char QCD_200_300[300] = "/wk_cms2/sam7k9621/QCD/BPK_80X_QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";
char QCD_300_500[300] = "/wk_cms2/sam7k9621/QCD/BPK_80X_QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";
char QCD_500_700[300] = "/wk_cms2/sam7k9621/QCD/BPK_80X_QCD_HT500t700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";
char QCD_700_1000[300] = "/wk_cms2/sam7k9621/QCD/BPK_80X_QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";
char QCD_1000_1500[300] = "/wk_cms2/sam7k9621/QCD/BPK_80X_QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";
char QCD_1500_2000[300] = "/wk_cms2/sam7k9621/QCD/BPK_80X_QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";
char QCD_2000_Inf[300] = "/wk_cms2/sam7k9621/QCD/BPK_80X_QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/bpk_ntuple_*.root";

//make capacity to store pre_sel path

vector<char*> Data_SE;
vector<char*> Data_SM;
vector<char*> TT;
vector<char*> DY;
vector<char*> WJets;
vector<char*> VV;
vector<char*> ST;
vector<char*> QCD;

//**Data pre-selected files' path in abbreviated way!**//


char SE_2016B[300] = "/wk_cms2/cychuang/pre_sel_file/BPK_80X_SingleElectron_Run2016B-03Feb2017_ver2-v2/pre_sel_bpk_ntuple_*.root";
char SE_2016C[300] = "/wk_cms2/cychuang/pre_sel_file/BPK_80X_SingleElectron_Run2016C-03Feb2017-v1/pre_sel_bpk_ntuple_*.root";
char SE_2016D[300] = "/wk_cms2/cychuang/pre_sel_file/BPK_80X_SingleElectron_Run2016D-03Feb2017-v1/pre_sel_bpk_ntuple_*.root";
char SE_2016E[300] = "/wk_cms2/cychuang/pre_sel_file/BPK_80X_SingleElectron_Run2016E-03Feb2017-v1/pre_sel_bpk_ntuple_*.root";
char SE_2016F[300] = "/wk_cms2/cychuang/pre_sel_file/BPK_80X_SingleElectron_Run2016F-03Feb2017-v1/pre_sel_bpk_ntuple_*.root";
char SE_2016G[300] = "/wk_cms2/cychuang/pre_sel_file/BPK_80X_SingleElectron_Run2016G-03Feb2017-v1/pre_sel_bpk_ntuple_*.root";
char SE_2016H_v2[300] = "/wk_cms2/cychuang/pre_sel_file/BPK_80X_SingleElectron_Run2016H-03Feb2017_ver2-v1/pre_sel_bpk_ntuple_*.root";
char SE_2016H_v3[300] = "/wk_cms2/cychuang/pre_sel_file/BPK_80X_SingleElectron_Run2016H-03Feb2017_ver3-v1/pre_sel_bpk_ntuple_*.root";

/*
Data_SE.push_back(SE_2016B);
Data_SE.push_back(SE_2016C);
Data_SE.push_back(SE_2016D);
Data_SE.push_back(SE_2016E);
Data_SE.push_back(SE_2016F);
Data_SE.push_back(SE_2016G);
Data_SE.push_back(SE_2016H_v2);
Data_SE.push_back(SE_2016H_v3);
*/

char SM_2016B[300] = "/wk_cms2/cychuang/pre_sel_file/BPK_80X_SingleMuon_Run2016B-03Feb2017_ver2-v2/pre_sel_bpk_ntuple_*.root";
char SM_2016C[300] = "/wk_cms2/cychuang/pre_sel_file/BPK_80X_SingleMuon_Run2016C-03Feb2017-v1/pre_sel_bpk_ntuple_*.root";
char SM_2016D[300] = "/wk_cms2/cychuang/pre_sel_file/BPK_80X_SingleMuon_Run2016D-03Feb2017-v1/pre_sel_bpk_ntuple_*.root";
char SM_2016E[300] = "/wk_cms2/cychuang/pre_sel_file/BPK_80X_SingleMuon_Run2016E-03Feb2017-v1/pre_sel_bpk_ntuple_*.root";
char SM_2016F[300] = "/wk_cms2/cychuang/pre_sel_file/BPK_80X_SingleMuon_Run2016F-03Feb2017-v1/pre_sel_bpk_ntuple_*.root";
char SM_2016G[300] = "/wk_cms2/cychuang/pre_sel_file/BPK_80X_SingleMuon_Run2016G-03Feb2017-v1/pre_sel_bpk_ntuple_*.root";
char SM_2016H_v2[300] = "/wk_cms2/cychuang/pre_sel_file/BPK_80X_SingleMuon_Run2016H-03Feb2017_ver2-v1/pre_sel_bpk_ntuple_*.root";
char SM_2016H_v3[300] = "/wk_cms2/cychuang/pre_sel_file/BPK_80X_SingleMuon_Run2016H-03Feb2017_ver3-v1/pre_sel_bpk_ntuple_*.root";

/*
Data_SM.push_back(SM_2016B);
Data_SM.push_back(SM_2016C);
Data_SM.push_back(SM_2016D);
Data_SM.push_back(SM_2016E);
Data_SM.push_back(SM_2016F);
Data_SM.push_back(SM_2016G);
Data_SM.push_back(SM_2016H_v2);
Data_SM.push_back(SM_2016H_v3);
*/

//**Monte Carlo pre-selected files' path in abbreviated way!**//

//TTbar
char pre_TT_001[300] = "/wk_cms2/cychuang/pre_sel_file/pre_sel_TT/dir_001/pre_sel_TT_*.root";
char pre_TT_002[300] = "/wk_cms2/cychuang/pre_sel_file/pre_sel_TT/dir_002/pre_sel_TT_*.root";
char pre_TT_003[300] = "/wk_cms2/cychuang/pre_sel_file/pre_sel_TT/dir_003/pre_sel_TT_*.root";

/*
TT.push_back(pre_TT_001);
TT.push_back(pre_TT_002);
TT.push_back(pre_TT_003);
*/

//Drew-Yen
char pre_DY_100_200[300] = "/wk_cms2/cychuang/pre_sel_file/DYJetsToLL_M-50_HT/BPK_80X_DYJetsToLL_M-50_HT-100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_DY_200_400[300] = "/wk_cms2/cychuang/pre_sel_file/DYJetsToLL_M-50_HT/BPK_80X_DYJetsToLL_M-50_HT-200to400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_DY_400_600[300] = "/wk_cms2/cychuang/pre_sel_file/DYJetsToLL_M-50_HT/BPK_80X_DYJetsToLL_M-50_HT-400to600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_DY_600_800[300] = "/wk_cms2/cychuang/pre_sel_file/DYJetsToLL_M-50_HT/BPK_80X_DYJetsToLL_M-50_HT-600to800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_DY_800_1200[300] = "/wk_cms2/cychuang/pre_sel_file/DYJetsToLL_M-50_HT/BPK_80X_DYJetsToLL_M-50_HT-800to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_DY_1200_2500[300] = "/wk_cms2/cychuang/pre_sel_file/DYJetsToLL_M-50_HT/BPK_80X_DYJetsToLL_M-50_HT-1200to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_DY_2500_Inf[300] = "/wk_cms2/cychuang/pre_sel_file/DYJetsToLL_M-50_HT/BPK_80X_DYJetsToLL_M-50_HT-2500toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";

/*
DY.push_back(pre_DY_100_200);
DY.push_back(pre_DY_200_400);
DY.push_back(pre_DY_400_600);
DY.push_back(pre_DY_600_800);
DY.push_back(pre_DY_800_1200);
DY.push_back(pre_DY_1200_2500);
DY.push_back(pre_DY_2500_Inf);
*/

//W+Jets
char pre_WJets_100_200[300] = "/wk_cms2/cychuang/pre_sel_file/WJetsToLNu_HT/BPK_80X_WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_WJets_200_400[300] = "/wk_cms2/cychuang/pre_sel_file/WJetsToLNu_HT/BPK_80X_WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_WJets_400_600[300] = "/wk_cms2/cychuang/pre_sel_file/WJetsToLNu_HT/BPK_80X_WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_WJets_600_800[300] = "/wk_cms2/cychuang/pre_sel_file/WJetsToLNu_HT/BPK_80X_WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_WJets_800_1200[300] = "/wk_cms2/cychuang/pre_sel_file/WJetsToLNu_HT/BPK_80X_WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_WJets_1200_2500[300] = "/wk_cms2/cychuang/pre_sel_file/WJetsToLNu_HT/BPK_80X_WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_WJets_2500_Inf[300] = "/wk_cms2/cychuang/pre_sel_file/WJetsToLNu_HT/BPK_80X_WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";

/*
WJets.push_back(pre_Wjets_100_200);
WJets.push_back(pre_Wjets_200_400);
WJets.push_back(pre_Wjets_400_600);
WJets.push_back(pre_Wjets_600_800);
WJets.push_back(pre_Wjets_800_1200);
WJets.push_back(pre_Wjets_1200_2500);
WJets.push_back(pre_Wjets_2500_Inf);
*/

//2 Vector Bosons
char pre_VV_WW[300] = "/wk_cms2/cychuang/pre_sel_file/VV/BPK_80X_WW_TuneCUETP8M1_13TeV-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_VV_WZ[300] = "/wk_cms2/cychuang/pre_sel_file/VV/BPK_80X_WZ_TuneCUETP8M1_13TeV-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_VV_ZZ[300] = "/wk_cms2/cychuang/pre_sel_file/VV/BPK_80X_ZZ_TuneCUETP8M1_13TeV-pythia8/pre_sel_bpk_ntuple_*.root";

/*
VV.push_back(pre_VV_WW);
VV.push_back(pre_VV_WZ);
VV.push_back(pre_VV_ZZ);
*/

//Single Top
char pre_ST_s[300] = "/wk_cms2/cychuang/pre_sel_file/SingleTop/BPK_80X_ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/pre_sel_bpk_ntuple_*.root";
char pre_ST_t_antitop[300] = "/wk_cms2/cychuang/pre_sel_file/SingleTop/BPK_80X_ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/pre_sel_bpk_ntuple_*.root";
char pre_ST_t_top[300] = "/wk_cms2/cychuang/pre_sel_file/SingleTop/BPK_80X_ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/pre_sel_bpk_ntuple_*.root";
char pre_ST_tW_antitop[300] = "/wk_cms2/cychuang/pre_sel_file/SingleTop/BPK_80X_ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/pre_sel_bpk_ntuple_*.root";
char pre_ST_tW_top[300] = "/wk_cms2/cychuang/pre_sel_file/SingleTop/BPK_80X_ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/pre_sel_bpk_ntuple_*.root";

/*
ST.push_back(pre_ST_s);
ST.push_back(pre_ST_t_antitop);
ST.push_back(pre_ST_t_top;
ST.push_back(pre_ST_tW_antitop);
ST.push_back(pre_ST_tW_top);
*/

//QCD (gluons or any quarks)
char pre_QCD_100_200[300] = "/wk_cms2/cychuang/pre_sel_file/QCD/BPK_80X_QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_QCD_200_300[300] = "/wk_cms2/cychuang/pre_sel_file/QCD/BPK_80X_QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_QCD_300_500[300] = "/wk_cms2/cychuang/pre_sel_file/QCD/BPK_80X_QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_QCD_500_700[300] = "/wk_cms2/cychuang/pre_sel_file/QCD/BPK_80X_QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_QCD_700_1000[300] = "/wk_cms2/cychuang/pre_sel_file/QCD/BPK_80X_QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_QCD_1000_1500[300] = "/wk_cms2/cychuang/pre_sel_file/QCD/BPK_80X_QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_QCD_1500_2000[300] = "/wk_cms2/cychuang/pre_sel_file/QCD/BPK_80X_QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";
char pre_QCD_2000_Inf[300] = "/wk_cms2/cychuang/pre_sel_file/QCD/BPK_80X_QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/pre_sel_bpk_ntuple_*.root";

/*
QCD.push_back(pre_QCD_100_200);
QCD.push_back(pre_QCD_200_300);
QCD.push_back(pre_QCD_300_500);
QCD.push_back(pre_QCD_500_700);
QCD.push_back(pre_QCD_700_1000);
QCD.push_back(pre_QCD_1000_1500);
QCD.push_back(pre_QCD_1500_2000);
QCD.push_back(pre_QCD_2000_Inf);
*/

//gen_number

vector<int> gen_no_TT;
vector<int> gen_no_DY;
vector<int> gen_no_WJets;
vector<int> gen_no_VV;
vector<int> gen_no_ST;
vector<int> gen_no_QCD;
/*
gen_no_TT.push_back(28143936);
gen_no_TT.push_back(28195200);
gen_no_TT.push_back(20018717);

gen_no_DY.puch_back(7145468);
gen_no_DY.puch_back(5093947);
gen_no_DY.puch_back(1265157);
gen_no_DY.puch_back(8292957);
gen_no_DY.puch_back(2668730);
gen_no_DY.puch_back(596079);
gen_no_DY.puch_back(399492);

gen_no_VV.push_back(994012);
gen_no_VV.push_back(1000000);
gen_no_VV.push_back(843120);

gen_no_ST.push_back(1000000);
gen_no_ST.push_back(38780433);
gen_no_ST.push_back(66437948);
gen_no_ST.push_back(6710218);
gen_no_ST.push_back(6924540);

gen_no_WJets.push_back(10210158);
gen_no_WJets.push_back(13804318);
gen_no_WJets.push_back(1963464);
gen_no_WJets.push_back(12420040);
gen_no_WJets.push_back(6200954);
gen_no_WJets.push_back(244532);
gen_no_WJets.push_back(253561);

gen_no_QCD.push_back(80365476);
gen_no_QCD.push_back(38857977);
gen_no_QCD.push_back(37502012);
gen_no_QCD.push_back(42190760);
gen_no_QCD.push_back(15629253);
gen_no_QCD.push_back(4767100);
gen_no_QCD.push_back(7855883);
gen_no_QCD.push_back(3839969);
*/
//Cross Section of MC files

vector<double> Xs_TT;
vector<double> Xs_DY;
vector<double> Xs_WJets;
vector<double> Xs_VV;
vector<double> Xs_ST;
vector<double> Xs_QCD;
/*
Xs_TT.assign(3,831.76);

//Xs_DY.push_back(169.9*1.23);	//70-100
Xs_DY.push_back(147.4*1.23);	//100-200
Xs_DY.push_back(40.99*1.23);	//200-400
Xs_DY.push_back(5.678*1.23);	//400-600
Xs_DY.push_back(1.367*1.23);	//600-800
Xs_DY.push_back(0.6304*1.23);	//800-1200
Xs_DY.push_back(0.1514*1.23);	//1200-2500
Xs_DY.push_back(0.003565*1.23);	//2500-Inf

Xs_WJets.push_back(1345.7*1.21);
Xs_WJets.push_back(359.7*1.21);
Xs_WJets.push_back(48.91*1.21);
Xs_WJets.push_back(12.05*1.21);
Xs_WJets.push_back(5.501*1.21);
Xs_WJets.push_back(1.329*1.21);
Xs_WJets.push_back(0.03216*1.21);

Xs_VV.push_back(118.7);
Xs_VV.push_back(47.13);
Xs_VV.push_back(16.52);

Xs_ST.push_back(3.36);	//s
Xs_ST.push_back(80.95);	//t anti-top
Xs_ST.push_back(136.02);//t top
Xs_ST.push_back(35.6);	//s anti-top
Xs_ST.push_back(35.6);	//s top

Xs_QCD.push_back(27990000);		//100-200
Xs_QCD.push_back(1712000);		//200-300
Xs_QCD.push_back(347700);		//300-500
Xs_QCD.push_back(32100);		//500-700
Xs_QCD.push_back(6831);			//700-1000
Xs_QCD.push_back(1207);			//1000-1500
Xs_QCD.push_back(119.9);		//1500-2000
Xs_QCD.push_back(25.24);		//2000-Inf
*/
//To calculate weight to lumi 35.9 (without trigger)
//

vector<double> w_TT;
vector<double> w_DY;
vector<double> w_WJets;
vector<double> w_VV;
vector<double> w_ST;
vector<double> w_QCD;
/*
w_TT.push_back( 35.9*Xs_TT[0]/(double)gen_no_TT[0] );
w_TT.push_back( 35.9*Xs_TT[1]/(double)gen_no_TT[1] );
w_TT.push_back( 35.9*Xs_TT[2]/(double)gen_no_TT[2] );

for(int i=0;i<DY.size();i++)
{
	w_DY.push_back( 35.9*Xs_DY[i]/(double)gen_no_DY[i] );
}

for(int i=0;i<WJets.size();i++)
{
	w_WJets.push_back( 35.9*Xs_WJets[i]/(double)gen_no_WJets[i] );
}

for(int i=0;i<VV.size();i++)
{
	w_VV.push_back( 35.9*Xs_VV[i]/(double)gen_no_VV[i] );
}

for(int i=0;i<ST.size();i++)
{
	w_ST.push_back( 35.9*Xs_ST[i]/(double)gen_no_ST[i] );
}


w_QCD.push_back( 35.9*Xs_QCD[0]/(double)gen_no_QCD[0] );
w_QCD.push_back( 35.9*Xs_QCD[1]/(double)gen_no_QCD[1] );
w_QCD.push_back( 35.9*Xs_QCD[2]/(double)gen_no_QCD[2] );
w_QCD.push_back( 35.9*Xs_QCD[3]/(double)gen_no_QCD[3] );
w_QCD.push_back( 35.9*Xs_QCD[4]/(double)gen_no_QCD[4] );
w_QCD.push_back( 35.9*Xs_QCD[5]/(double)gen_no_QCD[5] );
w_QCD.push_back( 35.9*Xs_QCD[6]/(double)gen_no_QCD[6] );
w_QCD.push_back( 35.9*Xs_QCD[7]/(double)gen_no_QCD[7] );
*/

#endif
