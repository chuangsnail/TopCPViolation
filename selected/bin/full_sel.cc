/*************************************************************************
*
*	Filename : full_sel.cc 
*	Description :
*		made date : about 2019 January
*		modified date : 2019 August	( apply Golden Json on real data )
*		for Legacy version data/MC
*		
*		Maybe next time I'll add the trigger on this pre-seletion step 
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
#include <vector>

using namespace std;

//	arguments are { 0:(execute command), 1:(original files' path and name), 2:(The pre-sel files' directory's name), 3:(is_data?) }
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

	//check what type of this dataset (data or MC)
	if( std::stod( std::string( argv[3] ) ) == 1. )
	{	cout << endl << "This dataset is Data " << endl;	}
	else
	{	cout << endl << "This dataset is MC " << endl;	}


	TChain* root = new TChain( "root" );
    //root->Add("/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_001/bpk_ntuple_*.root");

	//To capture original files' name

	char star_point_root[10] = "*.root";						//There is no star!!!!!!!!!!!!!!!!!!
	char pre_name[500];
	strcpy(pre_name,argv[1]);
	//Ex. pre_name now is "/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_003/bpk_ntuple_"
	strcat(pre_name,star_point_root);
	//Ex.pre_name now is "/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_003/bpk_ntuple_*.root"
	
	root->Add( pre_name );

	//To make the output files' name

	char path_filename[500] = "/wk_cms2/cychuang/2016legacy_full_sel_file/";
	strcat(path_filename,argv[2]);
	char temp_path_filename[500];
	strcpy( temp_path_filename, path_filename );

	EvtInfo evts ;
	evts.Register( root );
	JetInfo jets ;
	jets.Register( root , "JetInfo" );
	LeptonInfo leps;
	leps.Register( root , "LepInfo" );

	/*
	GenInfo genInfo;
	genInfo.Register( root );
	PhotonInfo photonInfo;
	photonInfo.Register( root , "PhotonInfo" );
	TrgInfo trgInfo;
	trgInfo.Register( root , "TrgInfo" );
	VertexInfo vertexInfo;
	vertexInfo.Register( root , "VertexInfo" );
	RunInfo runInfo;
	runInfo.Register( root , "RunInfo" );
	*/

	//This is the official Golden json file
	checkEvtTool checkEvt_all;
	checkEvt_all.addJson( "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/Cert_271036-284044_13TeV_ReReco_07Aug2017_Collisions16_JSON.txt" );
	checkEvt_all.makeJsonMap();


	bool is_data = false;
	if( std::stod( std::string( argv[3] ) ) == 1. )
	{	is_data = true;	}
    
	int pre_sel_No = 0;
	int not_pass_Golden = 0;
    int total_entries = root->GetEntries();
    int k = 1;
	int r = 1;
    int entry = 0;
    int temp = 0;
    bool not_end = true;
    
    //TFile* f_new;
    TTree* root_new;
   
	cout << endl << "At the start of the pre-seletion , and the total entries number before full-selection is " << total_entries << endl;

    while( not_end )
    {
		/*
		//if you wnt to just test few file, use this
		if( k >= 5 )
		{	not_end = false;	}
		*/

		strcpy( path_filename, temp_path_filename );
		char filename_no[50] = "";
		sprintf( filename_no, "full_sel_bpk_ntuple_%d.root", k );
		strcat( path_filename, filename_no );

        TFile* f_new = new TFile( path_filename , "recreate" );
        root_new = root->CloneTree( 0 );
        
	    for(entry=temp;entry<total_entries;entry++)
   		{
        	root->GetEntry(entry);

			//if(entry%1000000 == 0)
			//{	cout << "At entry : " << entry << endl;	}

            if(entry == total_entries - 1)
            {	
				root_new->AutoSave();
                not_end = false;
				break;
				//Here we directly abandon the last event
				//it is needed to be solve in the future
			}
			
			if( (double)entry/(double)total_entries >= 0.1*r )
			{
				cout << "*" ;
				r++;
			}


			//apply Golden Jsonfile ( just for data )

			if( is_data )
			{
				if( !checkEvt_all.isGoodEvt(evts.RunNo,evts.LumiNo) )
				{
					not_pass_Golden++;
					continue;	
				}
			}
			else {}	//usually I input argument with 0 for MC files

			//Start pre-selstion
        
        	int idx_Selected_Mu = -1;
        	int idx_Selected_El = -1;
        
			bool pass_sel_mu = Pass_SR_Selected_Muon(leps,idx_Selected_Mu);
			bool pass_sel_el = Pass_SR_Selected_Electron(leps,idx_Selected_El);
			bool pass_veto_el = false;		bool pass_veto_mu = false;

			if( (pass_sel_mu||pass_sel_el) && !(pass_sel_mu&&pass_sel_el) )
			{
				if(pass_sel_mu)
				{
					pass_veto_el = Pass_SR_Veto_Electron( leps, idx_Selected_Mu );
					pass_veto_mu = Pass_SR_Veto_Muon( leps, idx_Selected_Mu );
				}
				if(pass_sel_el)
				{
					pass_veto_el = Pass_SR_Veto_Electron( leps, idx_Selected_El );
					pass_veto_mu = Pass_SR_Veto_Muon( leps, idx_Selected_El );
				}
			}
			else
			{	continue;	}
		
			bool is_mu_channel = pass_sel_mu && pass_veto_mu && pass_veto_el ;
			bool is_el_channel = pass_sel_el && pass_veto_el && pass_veto_mu ;
        
			vector<int> sel_jets;		sel_jets.assign(128,-1);
			vector<int> sel_b_jets;		sel_b_jets.assign(2,-1);
		
			int idx_Selected_Lep = -1;
			if( is_mu_channel&&!is_el_channel )
			{
				idx_Selected_Lep = idx_Selected_Mu;
			}
			else if( is_el_channel&&!is_mu_channel )
			{
				idx_Selected_Lep = idx_Selected_El;
			}
			else
			{	continue;	}
			
        	bool pass_sel_jet = Pass_SR_Selected_Jets_Case( jets, sel_jets );
			if( !pass_sel_jet )
			{	continue;	}
        
		   	if( idx_Selected_Lep == -1 )
			{	continue;	}	
			
			bool pass_delR = Pass_delR_Jet_Lep( jets, leps, sel_jets, idx_Selected_Lep );
            if( !pass_delR )
            {	continue;	}
					
			bool pass_sel_b = Pass_SR_bjets( jets, sel_jets, sel_b_jets);
			if( !pass_sel_b )	
			{	continue;	}
            
			//Now Fill in the pre-selection case		
			root_new->Fill();
			pre_sel_No++;
            
            if(root_new->GetEntries() == 100000)
            {
                temp = entry+1;
                root_new->AutoSave();
				//cout << "Now at " << entry/total_entries * 100 << "%"<< endl;
				k++;
                break;
            }
            
			/*
            if(entry == total_entries - 1)
            {
                root_new->AutoSave();
                not_end = false;
                break;
            }
			*/
            
        }       //end of for-loop
		//f_new->Map();
		f_new->Close();
    }   //end of while loop
    



	printf("\nThe original total events No.: %d",total_entries);
	printf("\nThe events No. which is not pass GoldenJson : %d",not_pass_Golden);
	printf("\nThe survived events No. after full-selection : %d",pre_sel_No);

}
