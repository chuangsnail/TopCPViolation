/*************************************************************************
*
*	Filename : pre_sel_args.cc 
*	Description :
*		made date : about 2019 January
*		modified date : 2019 May 10th	( apply Golden Json on real data )
*	Author : Chen-Yu Chuang
*
*************************************************************************/


#include "TopCPViolation/selected/interface/BranchInfo.h"
#include "TopCPViolation/selected/interface/lep_sel.h"
#include "TopCPViolation/selected/interface/jet_sel.h"
#include "TopCPViolation/selected/interface/checkEvtTool.h"		//for Golden Json file

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1.h"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>

using namespace std;

//	arguments are { 0:(execute command), 1:(original files' path and name), 2:(The datas' directory's name), 3:(is_data?) }
//	1 need to be this form Ex : /wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_003/bpk_ntuple_ 
//	2 need to add '/' after the directory's name, Ex: BPK_80X_SingleElectron_Run2016H-03Feb2017_ver2-v1/
//	is_data -> [ 1 for real data ; 0 for MC data ]

int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		printf("\nThis is something Wrong! with the arguments' number!");
		return 0;
	}

	TChain* root = new TChain("bprimeKit/root");
    //root->Add("/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_001/bpk_ntuple_*.root");
    //root->Add("/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_002/bpk_ntuple_*.root");
    //root->Add("/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_003/bpk_ntuple_*.root");

	//To capture original files' name

	char star_point_root[10] = "*.root";
	char pre_name[300];
	strcpy(pre_name,argv[1]);
	//Ex. pre_name now is "/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_003/bpk_ntuple_"
	strcat(pre_name,star_point_root);
	//Ex.pre_name now is "/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_003/bpk_ntuple_*.root"
	
	root->Add( pre_name );

	//To make the output files' name

	char path_filename[300] = "/wk_cms2/cychuang/pre_sel_file/";
	strcat(path_filename,argv[2]);
	char temp_path_filename[300];
	strcpy(temp_path_filename,path_filename);

    EvtInfo evts;
    evts.Register( root );
    JetInfo jets;
    jets.Register( root, "JetInfo"  );
    LeptonInfo leps;
    leps.Register( root , "LepInfo" );
    GenInfo gens;
    gens.Register( root );

	checkEvtTool checkEvt_all;
	checkEvt_all.addJson( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/data/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt" );
	checkEvt_all.makeJsonMap();

	checkEvtTool checkEvt_el_data;
	checkEvt_el_data.addJson( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/data/StripLumisForPath_HLT_Ele32_eta2p1_WPTight_Gsf_withLowestSeed_L1_SingleIsoEG22er_OR_L1_SingleIsoEG24er_OR_L1_SingleIsoEG26er_OR_L1_SingleIsoEG28er_OR_L1_SingleIsoEG30er.json" );
	checkEvt_el_data.makeJsonMap();

	//apply extra json file on the SingleElectron data
	
    
	int pre_sel_No = 0;
	int not_pass_Golden = 0;
    int total_entries = root->GetEntries();
    int k = 1;
    int entry = 0;
    int temp = 0;
    bool not_end = true;
    
    TFile* f_new;
    TTree* root_new;
    
    while(not_end)
    {
		strcpy(path_filename,temp_path_filename);
		char filename_no[50] = "";
		sprintf(filename_no,"pre_sel_bpk_ntuple_%d.root",k);
		strcat(path_filename,filename_no);

        f_new = new TFile( path_filename , "recreate" );
        root_new = root->CloneTree( 0 );
        
	    for(entry=temp;entry<total_entries;entry++)
   		{
        	root->GetEntry(entry);

			//apply Golden Jsonfile
			
			if( std::stod( std::string( argv[3] ) ) == 1. )	//is_data
			{
				if( std::string( argv[2] ).find("SingleElectron") != string::npos )
				{
					if( !checkEvt_el_data.isGoodEvt(evts.RunNo,evts.LumiNo) )
					{
						not_pass_Golden++;
						continue;				//continue the entry-forloop	//to throw out the bad events
					}
				}
				else
				{
					if( !checkEvt_all.isGoodEvt(evts.RunNo,evts.LumiNo) )
					{
						not_pass_Golden++;
						continue;				//continue the entry-forloop	//to throw out the bad events
					}
				}
			}
			else {}	//usually I input argument with 0 for MC files
			
			//Start pre-selstion
        
        	int idx_Selected_Mu = -1;
        	int idx_Selected_El = -1;
        
        	bool pass_sel_mu = Pass_Selected_Muon(leps,&idx_Selected_Mu);
        	bool pass_veto_mu = Pass_Veto_Muon(leps,idx_Selected_Mu);
        	bool pass_sel_el = Pass_Selected_Electron(leps,&idx_Selected_El);
        	bool pass_veto_el = Pass_Veto_Electron(leps,idx_Selected_El);
        
        	bool is_mu_channel = pass_sel_mu && pass_veto_mu && pass_veto_el ;
        	bool is_el_channel = pass_sel_el && pass_veto_el && pass_veto_mu ;
        
        	int sel_jets_no = 128;
        	int sel_jets[sel_jets_no];
        	for(int i=0;i<sel_jets_no;i++)
        	{ sel_jets[i] = 0; }
        
        	bool pass_sel_jet = Pass_Selected_Jet(jets,sel_jets,&sel_jets_no);
        
        	if(is_mu_channel || is_el_channel)
        	{
                if(is_mu_channel && !is_el_channel)
                {
                    if(pass_sel_jet)
                    {
                        bool pass_delR = Pass_delta_R(leps,idx_Selected_Mu,jets,sel_jets,&sel_jets_no);
                        if(pass_delR)
                        {
                            root_new->Fill();
							pre_sel_No++;
                        }
                    }
                }
                else if(!is_mu_channel && is_el_channel)
                {
                    if(pass_sel_jet)
                    {
                        bool pass_delR = Pass_delta_R(leps,idx_Selected_El,jets,sel_jets,&sel_jets_no);
                        if(pass_delR)
                        {
                            root_new->Fill();
							pre_sel_No++;
                        }
                    }
                }
            }
            
            if(root_new->GetEntries() == 25000)
            {
                k++;
                temp = entry+1;
                root_new->AutoSave();
                break;
            }
            
            if(entry == total_entries - 1)
            {
                root_new->AutoSave();
                not_end = false;
                break;
            }
        }       //end of for-loop
    }   //end of while loop
    
    delete f_new;

	printf("\nThe original total events No.: %d",total_entries);
	printf("\nThe events No. which is not pass GoldenJson : %d",not_pass_Golden);
	printf("\nThe survived events No. after pre-selection : %d",pre_sel_No);

}
