/**************************************************************************************
 *
 *	File Name : store_candidate_tree10.cc
 *	Description : add new variable for mva
 *				  use new selected sample (with the GenMgr.h file instead of prepare_mva.h)! 
 *		{deepCSV}
 *	Date : 191105~
 *	Author : Chen-Yu Chuang
 *
 *	exe {old_file_name with path without .root} {just new_file_name with path without .root } {option}
 *	{option} == 0 -> test
 *	{option} == 1 -> Run all
 *
 ****************************************************************************************/
#include "TopCPViolation/selected/interface/Comp_DataMC.h"
#include "TopCPViolation/selected/interface/GenMgr.h"
#include "TopCPViolation/selected/interface/kinematic_tool.h"

#include <cmath>
#include "TLorentzVector.h"

using namespace std;

int main(int argc,char* argv[])
{
	//*********Offline High level Trigger Dealing**********//
	
	vector<int> HLT_MC_mu, HLT_MC_el, HLT_Data_mu, HLT_Data_el;
	
	Get_HLT_Info( string("MC_mu"), HLT_MC_mu );			Get_HLT_Info( string("MC_el"), HLT_MC_el );
	
	//*******To prepare the things about MC reweigh********//

	//insert pileup re-weighting information
	vector<double> pileupinfo;
	pileupinfo.assign(76,0.);
	int PUI_size = 76;
	char path_PU_file[200] = "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/pileupweights_69200.csv"; 
	GetPileUpInfo( path_PU_file , pileupinfo , &PUI_size);

	//insert tight-Muon ID Scale Factors' TH2F*
	TFile* f1 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/muIDSF.root");
	TH2F* h_tightMuIDSF;		f1->GetObject("abseta_pt_ratio",h_tightMuIDSF);

	TFile* f2 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/muISOSF.root");
	TH2F* h_MuISOSF;			f2->GetObject("abseta_pt_ratio",h_MuISOSF);

	TFile* f3 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/muTrgSF.root");
	TH2F* h_MuTrgSF;			f3->GetObject("abseta_pt_ratio",h_MuTrgSF);

	//insert tight-Electron ID Scale Factor TH2F*
	TFile* f4 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/elTightIDSF.root");
	TH2F* h_tightElIDSF;		f4->GetObject("EGamma_SF2D",h_tightElIDSF);

	TFile* f5 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/elRECOSF.root");
	TH2F* h_ElRECOSF;			f5->GetObject("EGamma_SF2D",h_ElRECOSF);

	TFile* f6 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/elTrgSF.root");
	TH2D* h_ElTrgSF;			f6->GetObject("scale_ele32",h_ElTrgSF);
	//we need to delete the f1~f7 after finising using the objects from them		//so we delete them after selecting
	
	TH2F* eff_b;		TH2F* eff_c;		TH2F* eff_l;
	TFile* f7 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/beffPlot_TTbar_0pt6321.root");
	f7->GetObject( "eff_b", eff_b );	f7->GetObject( "eff_c", eff_c );	f7->GetObject( "eff_l", eff_l );

	//**********************Start Analysis***********************//

	//Register branches
	
	TChain* root_old = new TChain( "root" );

	char point_root[10] = ".root";
	char pre_name[500] = "";
	strcpy( pre_name, argv[1] );
	strcat( pre_name, point_root );

	root_old->Add( pre_name );


	EvtInfo evtInfo ;
	evtInfo.Register( root_old );
	JetInfo jetInfo ;
	jetInfo.Register( root_old , "JetInfo" );
	LeptonInfo leptonInfo;
	leptonInfo.Register( root_old , "LepInfo" );
	GenInfo genInfo;
	genInfo.Register( root_old );		
	TrgInfo trgInfo;
	trgInfo.Register( root_old , "TrgInfo" );
			
	/*
	VertexInfo vertexInfo;
	vertexInfo.Register( files_map[ Set_name ]->at(r).ch , "VertexInfo" );
	RunInfo runInfo;
	runInfo.Register( files_map[ Set_name ]->at(r).ch , "RunInfo" );
	PhotonInfo photonInfo;
	photonInfo.Register( files_map[ Set_name ]->at(r).ch , "PhotonInfo" );
	*/
			
	int t_entries = root_old->GetEntries();
	printf("\nAnd the Entries of this data files are : %d\n",t_entries);


	//open new file and store new things

	string new_file_name = string( argv[2] ) + ".root";
	TFile* f_new = new TFile( new_file_name.c_str(), "recreate" );

    //allocate the memory space for branch
    
    double c_evt_weight;                                  //combine all the scale factor in this event

	double c_top_mass;
	double c_w_mass;
	double c_top_mass_dev;
	double c_w_mass_dev;

	double c_j1j2_absdelEta;
	double c_j1j2_delPhi;
	double c_j1j2_sumPt;
	double c_j1j2_absdelPt;
	double c_j1j2_delR;

	double c_lepblep_absdelEta;
	double c_lepblep_delPhi;
	double c_lepblep_sumPt;
	double c_lepblep_delR;
	
    double c_hadb_deepcsv_v, c_lepb_deepcsv_v;
	double c_hadblepb_delPt;	
	double c_whadb_absdelEta;
	double c_whadb_delPhi;
    double c_whadb_sumPt;
	double c_whadb_delPt;
	double c_whadb_delR;

	double c_hadblepton_absdelEta;
	double c_hadblepton_delPhi;
	double c_hadblepton_delPt;
	double c_hadblepton_sumPt;
	double c_hadblepton_delR;
	
	double c_hadwlepton_absdelEta;
	double c_hadwlepton_delPhi;
	double c_hadwlepton_delR;
	double c_hadwlepton_delPt;
	double c_hadwlepton_sumPt;

	double c_hadbmet_delPhi;
	double c_hadbmet_delPt;
	double c_hadbmet_sumPt;

	double c_hadwmet_delPhi;
	double c_hadwmet_delPt;
	double c_hadwmet_sumPt;
    
	//The incoreect combination's memory need to be used many times in one events, and be stored in many events
    double i_evt_weight;

	double i_top_mass;
	double i_w_mass;
	double i_top_mass_dev;
	double i_w_mass_dev;
    
	double i_j1j2_absdelEta;
	double i_j1j2_delPhi;
	double i_j1j2_sumPt;
	double i_j1j2_absdelPt;
	double i_j1j2_delR;
	
	double i_lepblep_absdelEta;
	double i_lepblep_delPhi;
	double i_lepblep_sumPt;
	double i_lepblep_delR;
    
    double i_hadb_deepcsv_v, i_lepb_deepcsv_v;
	double i_hadblepb_delPt;

	double i_whadb_absdelEta;
	double i_whadb_delPhi;
    double i_whadb_sumPt;
	double i_whadb_delPt;
	double i_whadb_delR;
    
	double i_hadblepton_absdelEta;
	double i_hadblepton_delPhi;
	double i_hadblepton_delPt;
	double i_hadblepton_sumPt;
	double i_hadblepton_delR;
	
	double i_hadwlepton_absdelEta;
	double i_hadwlepton_delPhi;
	double i_hadwlepton_delR;
	double i_hadwlepton_delPt;
	double i_hadwlepton_sumPt;

	double i_hadbmet_delPhi;
	double i_hadbmet_delPt;
	double i_hadbmet_sumPt;

	double i_hadwmet_delPhi;
	double i_hadwmet_delPt;
	double i_hadwmet_sumPt;
	
	//Initial the new trees and their branches
    
    TTree* correct = new TTree("correct","");           //for correct combination's info
    TTree* incorrect = new TTree("incorrect","");

    correct->Branch("evt_weight",&c_evt_weight,"evt_weight/D");
	
	correct->Branch("top_mass",&c_top_mass,"top_mass/D");
	correct->Branch("w_mass",&c_w_mass,"w_mass/D");
	correct->Branch("top_mass_dev",&c_top_mass_dev,"top_mass_dev/D");
	correct->Branch("w_mass_dev",&c_w_mass_dev,"w_mass_dev/D");

	correct->Branch("j1j2_sumPt",&c_j1j2_sumPt,"j1j2_sumPt/D");
	correct->Branch("j1j2_delPhi",&c_j1j2_delPhi,"j1j2_delPhi/D");
	correct->Branch("j1j2_absdelEta",&c_j1j2_absdelEta,"j1j2_absdelEta/D");
	correct->Branch("j1j2_absdelPt",&c_j1j2_absdelPt,"j1j2_absdelPt/D");
	correct->Branch("j1j2_delR",&c_j1j2_delR,"j1j2_delR/D");
	
	correct->Branch("lepblep_sumPt",&c_lepblep_sumPt,"lepblep_sumPt/D");
	correct->Branch("lepblep_delPhi",&c_lepblep_delPhi,"lepblep_delPhi/D");
	correct->Branch("lepblep_absdelEta",&c_lepblep_absdelEta,"lepblep_absdelEta/D");
	correct->Branch("lepblep_delR",&c_lepblep_delR,"lepblep_delR/D");
    
	correct->Branch("hadb_deepcsv_v",&c_hadb_deepcsv_v,"hadb_deepcsv_v/D");
	correct->Branch("hadblepb_delPt",&c_hadblepb_delPt,"hadblepb_delPt/D");

	correct->Branch("whadb_delPhi",&c_whadb_delPhi,"whadb_delPhi/D");
	correct->Branch("whadb_absdelEta",&c_whadb_absdelEta,"whadb_absdelEta/D");
	correct->Branch("whadb_sumPt",&c_whadb_sumPt,"whadb_sumPt/D");
	correct->Branch("whadb_delPt",&c_whadb_delPt,"whadb_delPt/D");
	correct->Branch("whadb_delR",&c_whadb_delR,"whadb_delR/D");

	correct->Branch("hadblepton_absdelEta",&c_hadblepton_absdelEta,"hadblepton_absdelEta/D");
	correct->Branch("hadblepton_delPhi",&c_hadblepton_delPhi,"hadblepton_delPhi/D");
	correct->Branch("hadblepton_delPt",&c_hadblepton_delPt,"hadblepton_delPt/D");
	correct->Branch("hadblepton_sumPt",&c_hadblepton_sumPt,"hadblepton_sumPt/D");
	correct->Branch("hadblepton_delR",&c_hadblepton_delR,"hadblepton_delR/D");

	correct->Branch("hadwlepton_absdelEta",&c_hadwlepton_absdelEta,"hadwlepton_absdelEta/D");
	correct->Branch("hadwlepton_delPhi",&c_hadwlepton_delPhi,"hadwlepton_delPhi/D");
	correct->Branch("hadwlepton_delPt",&c_hadwlepton_delPt,"hadwlepton_delPt/D");
	correct->Branch("hadwlepton_sumPt",&c_hadwlepton_sumPt,"hadwlepton_sumPt/D");
	correct->Branch("hadwlepton_delR",&c_hadwlepton_delR,"hadwlepton_delR/D");

	correct->Branch("hadbmet_delPhi",&c_hadbmet_delPhi,"hadbmet_delPhi/D");
	correct->Branch("hadbmet_delPt",&c_hadbmet_delPt,"hadbmet_delPt/D");
	correct->Branch("hadbmet_sumPt",&c_hadbmet_sumPt,"hadbmet_sumPt/D");
	
	correct->Branch("hadwmet_delPhi",&c_hadwmet_delPhi,"hadwmet_delPhi/D");
	correct->Branch("hadwmet_delPt",&c_hadwmet_delPt,"hadwmet_delPt/D");
	correct->Branch("hadwmet_sumPt",&c_hadwmet_sumPt,"hadwmet_sumPt/D");

    incorrect->Branch("evt_weight",&i_evt_weight,"evt_weight/D"); 

	incorrect->Branch("top_mass",&i_top_mass,"top_mass/D");
	incorrect->Branch("w_mass",&i_w_mass,"w_mass/D");
	incorrect->Branch("top_mass_dev",&i_top_mass_dev,"top_mass_dev/D");
	incorrect->Branch("w_mass_dev",&i_w_mass_dev,"w_mass_dev/D");
	
	incorrect->Branch("j1j2_sumPt",&i_j1j2_sumPt,"j1j2_sumPt/D");
	incorrect->Branch("j1j2_delPhi",&i_j1j2_delPhi,"j1j2_delPhi/D");
	incorrect->Branch("j1j2_absdelEta",&i_j1j2_absdelEta,"j1j2_absdelEta/D");
	incorrect->Branch("j1j2_absdelPt",&i_j1j2_absdelPt,"j1j2_absdelPt/D");
	incorrect->Branch("j1j2_delR",&i_j1j2_delR,"j1j2_delR/D");
	
	incorrect->Branch("lepblep_sumPt",&i_lepblep_sumPt,"lepblep_sumPt/D");
	incorrect->Branch("lepblep_delPhi",&i_lepblep_delPhi,"lepblep_delPhi/D");
	incorrect->Branch("lepblep_absdelEta",&i_lepblep_absdelEta,"lepblep_absdelEta/D");
	incorrect->Branch("lepblep_delR",&i_lepblep_delR,"lepblep_delR/D");
    
	incorrect->Branch("hadb_deepcsv_v",&i_hadb_deepcsv_v,"hadb_deepcsv_v/D");
	incorrect->Branch("hadblepb_delPt",&i_hadblepb_delPt,"hadblepb_delPt/D");

	incorrect->Branch("whadb_delPhi",&i_whadb_delPhi,"whadb_delPhi/D");
	incorrect->Branch("whadb_absdelEta",&i_whadb_absdelEta,"whadb_absdelEta/D");
	incorrect->Branch("whadb_sumPt",&i_whadb_sumPt,"whadb_sumPt/D");
	incorrect->Branch("whadb_delPt",&i_whadb_delPt,"whadb_delPt/D");
	incorrect->Branch("whadb_delR",&i_whadb_delR,"whadb_delR/D");

	incorrect->Branch("hadblepton_absdelEta",&i_hadblepton_absdelEta,"hadblepton_absdelEta/D");
	incorrect->Branch("hadblepton_delPhi",&i_hadblepton_delPhi,"hadblepton_delPhi/D");
	incorrect->Branch("hadblepton_delPt",&i_hadblepton_delPt,"hadblepton_delPt/D");
	incorrect->Branch("hadblepton_sumPt",&i_hadblepton_sumPt,"hadblepton_sumPt/D");
	incorrect->Branch("hadblepton_delR",&i_hadblepton_delR,"hadblepton_delR/D");

	incorrect->Branch("hadwlepton_absdelEta",&i_hadwlepton_absdelEta,"hadwlepton_absdelEta/D");
	incorrect->Branch("hadwlepton_delPhi",&i_hadwlepton_delPhi,"hadwlepton_delPhi/D");
	incorrect->Branch("hadwlepton_delPt",&i_hadwlepton_delPt,"hadwlepton_delPt/D");
	incorrect->Branch("hadwlepton_sumPt",&i_hadwlepton_sumPt,"hadwlepton_sumPt/D");
	incorrect->Branch("hadwlepton_delR",&i_hadwlepton_delR,"hadwlepton_delR/D");

	incorrect->Branch("hadbmet_delPhi",&i_hadbmet_delPhi,"hadbmet_delPhi/D");
	incorrect->Branch("hadbmet_delPt",&i_hadbmet_delPt,"hadbmet_delPt/D");
	incorrect->Branch("hadbmet_sumPt",&i_hadbmet_sumPt,"hadbmet_sumPt/D");
	
	incorrect->Branch("hadwmet_delPhi",&i_hadwmet_delPhi,"hadwmet_delPhi/D");
	incorrect->Branch("hadwmet_delPt",&i_hadwmet_delPt,"hadwmet_delPt/D");
	incorrect->Branch("hadwmet_sumPt",&i_hadwmet_sumPt,"hadwmet_sumPt/D");

    //
    string channel = "";	//lep is muon or electron

	//prepare btag object
	BtagManager bmgr( &jetInfo );
	bmgr.Register_Init_Maps();
	bmgr.Register_Init_TH2( eff_b, eff_c, eff_l );

cout << "test_2" << endl;	
    //prepare gen object
    GenMgr genmgr( &genInfo, &jetInfo, &leptonInfo );
		
	double pass_sel = 0.;
	double pass_seen = 0.;
	double pass_sample_sel = 0.;
	
	for(int entry=0;entry<(int)t_entries;++entry)
	{
		//if( entry >= 3000 )
		//{	break;	}

		//cout << "Entry : " << entry << endl;
		root_old->GetEntry(entry);

		double btag_weight = 1.;
		double weight = 1.;

		//pile-up reweigh MC

		bool pass_PU = true;
		weight = weight * GetPileUpWeight( &evtInfo, pileupinfo, &pass_PU );
		
		if(!pass_PU)
		{	continue;	}
        		
		//initialize some storage of selected-particles' indices
		int idx_Selected_Mu = -1;
		int idx_Selected_El = -1;
				
		vector<int> sel_jets;		sel_jets.assign(128,-1);
		vector<int> sel_b_jets;		sel_b_jets.assign(2,-1);
			
		//Do the lepton-selection first here
				
		bool pass_sel_mu = Pass_SR_Selected_Muon(leptonInfo,idx_Selected_Mu);
		bool pass_sel_el = Pass_SR_Selected_Electron(leptonInfo,idx_Selected_El);
		bool pass_veto_el = false;		bool pass_veto_mu = false;

		if( (pass_sel_mu||pass_sel_el) && !(pass_sel_mu&&pass_sel_el) )
		{
			if(pass_sel_mu)
			{
				pass_veto_el = Pass_SR_Veto_Electron(leptonInfo,idx_Selected_Mu);
				pass_veto_mu = Pass_SR_Veto_Muon(leptonInfo,idx_Selected_Mu);
			}
			if(pass_sel_el)
			{
				pass_veto_el = Pass_SR_Veto_Electron(leptonInfo,idx_Selected_El);
				pass_veto_mu = Pass_SR_Veto_Muon(leptonInfo,idx_Selected_El);
			}
		}
		else
		{	continue;	}
				
		bool is_mu_channel = pass_sel_mu && pass_veto_mu && pass_veto_el ;
		bool is_el_channel = pass_sel_el && pass_veto_el && pass_veto_mu ;
				
		//To assign Muon/Electron channel
				
		int idx_Selected_Lep = -1;
		if(is_mu_channel&&!is_el_channel)
		{
			idx_Selected_Lep = idx_Selected_Mu;
			channel = "mu";
		}
		else if(is_el_channel&&!is_mu_channel)
		{
			idx_Selected_Lep = idx_Selected_El;
			channel = "el";
		}
		else
		{	continue;	}
		
		//Apply HLT trigger		
				
		bool is_pass_HLT = false;
			
		if(channel == "mu")
		{	is_pass_HLT = Pass_HLT( trgInfo, HLT_MC_mu );	}
		else if(channel == "el")
		{	is_pass_HLT = Pass_HLT( trgInfo, HLT_MC_el );	}
		
		if( !is_pass_HLT )
		{	continue;	}
				

		//Then ,do the jet-selection here
		bool pass_sel_jet = Pass_SR_Selected_Jets_Case(jetInfo,sel_jets);
		
		if( (is_mu_channel && !is_el_channel) || (is_el_channel && !is_mu_channel) )
		{
			if( !pass_sel_jet )
			{	continue;	}

			//selected-jets and selected-lepton distinguishable (delta R)
			bool pass_delR = Pass_delR_Jet_Lep( jetInfo, leptonInfo, sel_jets, idx_Selected_Lep );
			if( !pass_delR )
			{	continue;	}

			//Do the b-jets-selected
			//in Control region , the selected b jets number is 0 ,and even by loose b-jet criteria
			bool pass_sel_b = Pass_SR_bjets( jetInfo, sel_jets, sel_b_jets );
			if( !pass_sel_b )	
			{	continue;	}

			//Do other reweight to MC
					
			//if(!is_data)
			{
				//do the b-tag reweighting
						
				bmgr.Reset_idx_capacity();
				bmgr.Set_b_tagged_jets_idx( sel_b_jets );
				bmgr.Set_b_ntagged_jets_idx( sel_jets );
				btag_weight = bmgr.Get_Btag_Weight();
				weight *= btag_weight;

				//do the lepton scale factor
				if(idx_Selected_Lep == idx_Selected_Mu){
					weight = weight * GetLepSF_TH2F( &leptonInfo, h_tightMuIDSF, idx_Selected_Lep );
					weight = weight * GetLepSF_TH2F( &leptonInfo, h_MuISOSF, idx_Selected_Lep );
					weight = weight * GetLepSF_TH2F( &leptonInfo, h_MuTrgSF, idx_Selected_Lep );
				}
				else if(idx_Selected_Lep == idx_Selected_El){
					weight = weight * GetLepSF_TH2F( &leptonInfo, h_tightElIDSF, idx_Selected_Lep );
					weight = weight * GetLepSF_TH2F( &leptonInfo, h_ElRECOSF, idx_Selected_Lep );
					weight = weight * GetLepSF_TH2D( &leptonInfo, h_ElTrgSF, idx_Selected_Lep );
				}
					
				//do the gen-weight (second linear order calibration)
				weight *= Get_GenMinus_Weight( genInfo );
			}
		}
		else
		{	continue;	}

		pass_sel += weight ;
       
        //use the sel_jets, sel_b_jets, idx_Selected_Lep to find the correct and incorrect combination
        
        int cor_b = -1, cor_j1 = -1, cor_j2 = -1;
        bool is_good_trained_evt = genmgr.Find_Correct_HadronicTop( cor_b, cor_j1, cor_j2 );


		//for check selected objects

		for(int l=0;l<(int)sel_jets.size();l++)
		{
			if( sel_jets.at(l) == -1 )
			{
				cout << "There is something wrong in jets selection! in entry :" << entry << endl;
				break;
			}
		}
		
		for(int l=0;l<(int)sel_b_jets.size();l++)
		{
			if( sel_b_jets.at(l) == -1 )
			{
				cout << "There is something wrong in b-jets selection! in entry :" << entry << endl;
				break;
			}
		}

		/*
		cout << "cor_b : " << cor_b << endl;
		cout << "cor_j1 : " << cor_j1 << endl;
		cout << "cor_j2 : " << cor_j2 << endl;
		*/

		//if( cor_b == -1 || cor_j1 == -1 || cor_j2 == -1 )
		//{	continue;	}
		//use the events which all pass full selection 

		//pass_seen += weight;

            for(int B=0;B<(int)sel_b_jets.size();++B)
            {
                for(int J1=0;J1<(int)sel_jets.size();++J1)
                {
                    for(int J2=J1+1;J2<(int)sel_jets.size();++J2)
                    {
                        if( ( sel_b_jets.at(B) == cor_b && sel_jets.at(J1) == cor_j1 && sel_jets.at(J2) == cor_j2 ) || ( sel_b_jets.at(B) == cor_b && sel_jets.at(J1) == cor_j2 && sel_jets.at(J2) == cor_j1) )
                        {
							int lB = (B == 0) ? 1 : 0 ;
							int tmp_mva_lepb = sel_b_jets.at(lB);
							int tmp_mva_hadb = sel_b_jets.at(B);
							int tmp_mva_j1 = sel_jets.at(J1);
							int tmp_mva_j2 = sel_jets.at(J2);
							
							c_evt_weight = weight;

							TLorentzVector p_mva_j1, p_mva_j2, p_mva_hadb, p_mva_lepb, p_mva_lepton;
							
							p_mva_j1 = GetJetP4(jetInfo,tmp_mva_j1);	
							p_mva_j2 = GetJetP4(jetInfo,tmp_mva_j2);	
							p_mva_hadb = GetJetP4(jetInfo,tmp_mva_hadb);	
							p_mva_lepb = GetJetP4(jetInfo,tmp_mva_lepb);	
							//p_mva_lepton = GetLepP4(leptonInfo,idx_Selected_Lep);

							TLorentzVector p_mva_hadw = ( p_mva_j1 + p_mva_j2 );

							double t_mass = ( p_mva_j1 + p_mva_j2 + p_mva_hadb ).M();
							double w_mass = p_mva_hadw.M();
							c_top_mass = t_mass;
							c_w_mass = w_mass;
							
							c_top_mass_dev = (t_mass - 172.5)/16.3;
							c_w_mass_dev = (w_mass - 82.9)/9.5;

                            c_j1j2_sumPt = jetInfo.Pt[ tmp_mva_j1 ] + jetInfo.Pt[ tmp_mva_j2 ];
                            c_j1j2_delPhi = TVector2::Phi_mpi_pi( jetInfo.Phi[ tmp_mva_j1 ] - jetInfo.Phi[ tmp_mva_j2 ] );
                            c_j1j2_absdelEta = fabs( jetInfo.Eta[ tmp_mva_j1 ] - jetInfo.Eta[ tmp_mva_j2 ] );
							c_j1j2_absdelPt = fabs( jetInfo.Pt[ tmp_mva_j1 ] - jetInfo.Pt[ tmp_mva_j2 ] );
							c_j1j2_delR = delta_R( jetInfo.Eta[ tmp_mva_j1 ], jetInfo.Eta[ tmp_mva_j2 ], jetInfo.Phi[ tmp_mva_j1 ], jetInfo.Phi[ tmp_mva_j2 ]);

                            c_lepblep_sumPt = leptonInfo.Pt[ idx_Selected_Lep ] + jetInfo.Pt[ tmp_mva_lepb ];
                            c_lepblep_absdelEta = fabs( leptonInfo.Eta[ idx_Selected_Lep ] - jetInfo.Eta[ tmp_mva_lepb ] );
                            c_lepblep_delPhi = TVector2::Phi_mpi_pi( jetInfo.Phi[ tmp_mva_lepb ] - leptonInfo.Phi[ idx_Selected_Lep ] );
							c_lepblep_delR = delta_R( jetInfo.Eta[ tmp_mva_lepb ], leptonInfo.Eta[ idx_Selected_Lep ], jetInfo.Phi[ tmp_mva_lepb ], leptonInfo.Phi[ idx_Selected_Lep ] );

							c_hadblepb_delPt = jetInfo.Pt[ tmp_mva_hadb ] - jetInfo.Pt[ tmp_mva_lepb ];
                            c_hadb_deepcsv_v = jetInfo.pfDeepCSVJetTags_probb[ tmp_mva_hadb ] + jetInfo.pfDeepCSVJetTags_probbb[ tmp_mva_lepb ];

							c_whadb_absdelEta = fabs( p_mva_hadw.Eta() - p_mva_hadb.Eta() );
							c_whadb_delPhi = TVector2::Phi_mpi_pi( p_mva_hadw.Phi() - p_mva_hadb.Phi() );
                            
							c_whadb_sumPt = p_mva_hadw.Pt() + p_mva_hadb.Pt();
                            c_whadb_delPt = p_mva_hadw.Pt() - p_mva_hadb.Pt();
							c_whadb_delR = delta_R( p_mva_hadw.Eta(), p_mva_hadb.Eta(), p_mva_hadw.Phi(), p_mva_hadb.Phi() );

							c_hadblepton_absdelEta = fabs( jetInfo.Eta[ tmp_mva_hadb ] - leptonInfo.Eta[ idx_Selected_Lep ] );
							c_hadblepton_delPhi = TVector2::Phi_mpi_pi( jetInfo.Phi[ tmp_mva_hadb ] - leptonInfo.Phi[ idx_Selected_Lep ] );
							c_hadblepton_delPt = jetInfo.Pt[ tmp_mva_hadb ] - leptonInfo.Pt[ idx_Selected_Lep ];
							c_hadblepton_sumPt = jetInfo.Pt[ tmp_mva_hadb ] + leptonInfo.Pt[ idx_Selected_Lep ];
							c_hadblepton_delR = delta_R( jetInfo.Eta[ tmp_mva_hadb ], leptonInfo.Eta[ idx_Selected_Lep ], jetInfo.Phi[ tmp_mva_hadb ], leptonInfo.Phi[ idx_Selected_Lep ]);
	
							c_hadwlepton_absdelEta = fabs( p_mva_hadw.Eta() - leptonInfo.Eta[ idx_Selected_Lep ] );
							c_hadwlepton_delPhi = TVector2::Phi_mpi_pi( p_mva_hadw.Phi() - leptonInfo.Phi[ idx_Selected_Lep ] );
							c_hadwlepton_delPt = p_mva_hadw.Pt() - leptonInfo.Pt[ idx_Selected_Lep ];
							c_hadwlepton_sumPt = p_mva_hadw.Pt() + leptonInfo.Pt[ idx_Selected_Lep ];
							c_hadwlepton_delR = delta_R( p_mva_hadw.Eta(), leptonInfo.Eta[ idx_Selected_Lep ], p_mva_hadw.Phi(), leptonInfo.Phi[ idx_Selected_Lep ]);
							
							c_hadbmet_delPhi = TVector2::Phi_mpi_pi( jetInfo.Phi[ tmp_mva_hadb ] - evtInfo.PFMETPhi );
							c_hadbmet_delPt = jetInfo.Pt[ tmp_mva_hadb ] - evtInfo.PFMET;
							c_hadbmet_sumPt = jetInfo.Pt[ tmp_mva_hadb ] + evtInfo.PFMET;

							c_hadwmet_delPhi = TVector2::Phi_mpi_pi( p_mva_hadw.Phi() - evtInfo.PFMETPhi );
							c_hadwmet_delPt = p_mva_hadw.Pt() - evtInfo.PFMET;
							c_hadwmet_sumPt = p_mva_hadw.Pt() + evtInfo.PFMET;

							correct->Fill();
                        }
                        else	//incorrect combination
                        {
							int lB = (B == 0) ? 1 : 0 ;
							int tmp_mva_lepb = sel_b_jets.at(lB);
							int tmp_mva_hadb = sel_b_jets.at(B);
							int tmp_mva_j1 = sel_jets.at(J1);
							int tmp_mva_j2 = sel_jets.at(J2);
							
							i_evt_weight = weight;

							TLorentzVector p_mva_j1, p_mva_j2, p_mva_hadb, p_mva_lepb, p_mva_lepton;

							p_mva_j1 = GetJetP4(jetInfo,tmp_mva_j1);	
							p_mva_j2 = GetJetP4(jetInfo,tmp_mva_j2);	
							p_mva_hadb = GetJetP4(jetInfo,tmp_mva_hadb);	
							p_mva_lepb = GetJetP4(jetInfo,tmp_mva_lepb);	
							//p_mva_lepton = GetLepP4(leptonInfo,idx_Selected_Lep);	
			
							TLorentzVector p_mva_hadw = ( p_mva_j1 + p_mva_j2 );
							
							double t_mass = ( p_mva_j1 + p_mva_j2 + p_mva_hadb ).M();
							double w_mass = p_mva_hadw.M();
							i_top_mass = t_mass;
							i_w_mass = w_mass;
							
							i_top_mass_dev = (t_mass - 172.5)/16.3;
							i_w_mass_dev = (w_mass - 82.9)/9.5;

                            i_j1j2_sumPt = jetInfo.Pt[ tmp_mva_j1 ] + jetInfo.Pt[ tmp_mva_j2 ];
                            i_j1j2_delPhi = TVector2::Phi_mpi_pi( jetInfo.Phi[ tmp_mva_j1 ] - jetInfo.Phi[ tmp_mva_j2 ] );
                            i_j1j2_absdelEta = fabs( jetInfo.Eta[ tmp_mva_j1 ] - jetInfo.Eta[ tmp_mva_j2 ] );
							i_j1j2_absdelPt = fabs( jetInfo.Pt[ tmp_mva_j1 ] - jetInfo.Pt[ tmp_mva_j2 ] );
							i_j1j2_delR = delta_R( jetInfo.Eta[ tmp_mva_j1 ], jetInfo.Eta[ tmp_mva_j2 ], jetInfo.Phi[ tmp_mva_j1 ], jetInfo.Phi[ tmp_mva_j2 ]);

                            i_lepblep_sumPt = leptonInfo.Pt[ idx_Selected_Lep ] + jetInfo.Pt[ tmp_mva_lepb ];
                            i_lepblep_absdelEta = fabs( leptonInfo.Eta[ idx_Selected_Lep ] - jetInfo.Eta[ tmp_mva_lepb ] );
                            i_lepblep_delPhi = TVector2::Phi_mpi_pi( jetInfo.Phi[ tmp_mva_lepb ] - leptonInfo.Phi[ idx_Selected_Lep ] );
							i_lepblep_delR = delta_R( jetInfo.Eta[ tmp_mva_lepb ], leptonInfo.Eta[ idx_Selected_Lep ], jetInfo.Phi[ tmp_mva_lepb ], leptonInfo.Phi[ idx_Selected_Lep ] );

							i_hadblepb_delPt = jetInfo.Pt[ tmp_mva_hadb ] - jetInfo.Pt[ tmp_mva_lepb ];
                            i_hadb_deepcsv_v = jetInfo.pfDeepCSVJetTags_probb[ tmp_mva_hadb ] + jetInfo.pfDeepCSVJetTags_probbb[ tmp_mva_lepb ];

							i_whadb_absdelEta = fabs( p_mva_hadw.Eta() - p_mva_hadb.Eta() );
							i_whadb_delPhi = TVector2::Phi_mpi_pi( p_mva_hadw.Phi() - p_mva_hadb.Phi() );
                            
							i_whadb_sumPt = p_mva_hadw.Pt() + p_mva_hadb.Pt();
                            i_whadb_delPt = p_mva_hadw.Pt() - p_mva_hadb.Pt();
							i_whadb_delR = delta_R( p_mva_hadw.Eta(), p_mva_hadb.Eta(), p_mva_hadw.Phi(), p_mva_hadb.Phi() );

							i_hadblepton_absdelEta = fabs( jetInfo.Eta[ tmp_mva_hadb ] - leptonInfo.Eta[ idx_Selected_Lep ] );
							i_hadblepton_delPhi = TVector2::Phi_mpi_pi( jetInfo.Phi[ tmp_mva_hadb ] - leptonInfo.Phi[ idx_Selected_Lep ] );
							i_hadblepton_delPt = jetInfo.Pt[ tmp_mva_hadb ] - leptonInfo.Pt[ idx_Selected_Lep ];
							i_hadblepton_sumPt = jetInfo.Pt[ tmp_mva_hadb ] + leptonInfo.Pt[ idx_Selected_Lep ];
							i_hadblepton_delR = delta_R( jetInfo.Eta[ tmp_mva_hadb ], leptonInfo.Eta[ idx_Selected_Lep ], jetInfo.Phi[ tmp_mva_hadb ], leptonInfo.Phi[ idx_Selected_Lep ]);
	
							i_hadwlepton_absdelEta = fabs( p_mva_hadw.Eta() - leptonInfo.Eta[ idx_Selected_Lep ] );
							i_hadwlepton_delPhi = TVector2::Phi_mpi_pi( p_mva_hadw.Phi() - leptonInfo.Phi[ idx_Selected_Lep ] );
							i_hadwlepton_delPt = p_mva_hadw.Pt() - leptonInfo.Pt[ idx_Selected_Lep ];
							i_hadwlepton_sumPt = p_mva_hadw.Pt() + leptonInfo.Pt[ idx_Selected_Lep ];
							i_hadwlepton_delR = delta_R( p_mva_hadw.Eta(), leptonInfo.Eta[ idx_Selected_Lep ], p_mva_hadw.Phi(), leptonInfo.Phi[ idx_Selected_Lep ]);
							
							i_hadbmet_delPhi = TVector2::Phi_mpi_pi( jetInfo.Phi[ tmp_mva_hadb ] - evtInfo.PFMETPhi );
							i_hadbmet_delPt = jetInfo.Pt[ tmp_mva_hadb ] - evtInfo.PFMET;
							i_hadbmet_sumPt = jetInfo.Pt[ tmp_mva_hadb ] + evtInfo.PFMET;

							i_hadwmet_delPhi = TVector2::Phi_mpi_pi( p_mva_hadw.Phi() - evtInfo.PFMETPhi );
							i_hadwmet_delPt = p_mva_hadw.Pt() - evtInfo.PFMET;
							i_hadwmet_sumPt = p_mva_hadw.Pt() + evtInfo.PFMET;

							incorrect->Fill();
                        }
                    }
                }               
            }               
        				
	}	//end of entry for-loop	
			
	correct->Write();
	incorrect->Write();

	f_new->Close();

	//*****Drawing Plotting or Outputting files*****//

	string time = "";
	time = get_time_str( minute );

	cout << "finish time is : " << time << endl;
	//cout << "pass_seen/pass_sel : " << (double)pass_seen/pass_sel << endl;

	//*****make space free*****//
	

	delete f7;
	delete f6;
	delete f5;
	delete f4;
	delete f3;
	delete f2;
	delete f1;
}

