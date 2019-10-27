/**************************************************************************************
 *
 *	File Name : store_candidate_tree7.cc
 *	Description : add new variable for mva
 *				  use new selected sample (with the GenMgr.h file instead of prepare_mva.h)! 
 *		{deepCSV}
 *	Date : 191003~
 *	Author : Chen-Yu Chuang
 *
 *	exe {old_file_name with path without .root} {just new_file_name with path without .root } {option}
 *	{option} == 0 -> test
 *	{option} == 1 -> Run all
 *
 ****************************************************************************************/
#include "TopCPViolation/selected/interface/Comp_DataMC.h"
#include "TopCPViolation/selected/interface/GenMgr.h"

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

	double c_lepblep_absdelEta;
	double c_lepblep_delPhi;
	double c_lepblep_sumPt;
	
    
    //The incoreect combination's memory need to be used many times in one events, and be stored in many events
    double i_evt_weight;

	double i_top_mass;
	double i_w_mass;
	double i_top_mass_dev;
	double i_w_mass_dev;
    
	double i_j1j2_absdelEta;
	double i_j1j2_delPhi;
	double i_j1j2_sumPt;
	
	double i_lepblep_absdelEta;
	double i_lepblep_delPhi;
	double i_lepblep_sumPt;
    
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
	
	correct->Branch("lepblep_sumPt",&c_lepblep_sumPt,"lepblep_sumPt/D");
	correct->Branch("lepblep_delPhi",&c_lepblep_delPhi,"lepblep_delPhi/D");
	correct->Branch("lepblep_absdelEta",&c_lepblep_absdelEta,"lepblep_absdelEta/D");


    incorrect->Branch("evt_weight",&i_evt_weight,"evt_weight/D"); 

	incorrect->Branch("top_mass",&i_top_mass,"top_mass/D");
	incorrect->Branch("w_mass",&i_w_mass,"w_mass/D");
	incorrect->Branch("top_mass_dev",&i_top_mass_dev,"c_top_mass_dev/D");
	incorrect->Branch("w_mass_dev",&i_w_mass_dev,"c_w_mass_dev/D");
	
	incorrect->Branch("j1j2_sumPt",&i_j1j2_sumPt,"j1j2_sumPt/D");
	incorrect->Branch("j1j2_delPhi",&i_j1j2_delPhi,"j1j2_delPhi/D");
	incorrect->Branch("j1j2_absdelEta",&i_j1j2_absdelEta,"j1j2_absdelEta/D");
	
	incorrect->Branch("lepblep_sumPt",&i_lepblep_sumPt,"lepblep_sumPt/D");
	incorrect->Branch("lepblep_delPhi",&i_lepblep_delPhi,"lepblep_delPhi/D");
	incorrect->Branch("lepblep_absdelEta",&i_lepblep_absdelEta,"lepblep_absdelEta/D");

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
				

		//luminosity cali after trigger

		
		if(channel == "mu")
		{	weight *= lumi_cali_trg(35.811/35.9);	}
		if(channel == "el")
		{	weight *= lumi_cali_trg(35.615/35.9);	}

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

		/*
		cout << "cor_b : " << cor_b << endl;
		cout << "cor_j1 : " << cor_j1 << endl;
		cout << "cor_j2 : " << cor_j2 << endl;
		*/

		//if( cor_b == -1 || cor_j1 == -1 || cor_j2 == -1 )
		//{	continue;	}
		//use the events which all pass full selection 

		//pass_seen += weight;

            for(int i=0;i<(int)sel_b_jets.size();i++)
            {
                for(int j=0;j<(int)sel_jets.size();j++)
                {
                    for(int k=j+1;k<(int)sel_jets.size();k++)
                    {
                        if( ( sel_b_jets.at(i) == cor_b && sel_jets.at(j) == cor_j1 && sel_jets.at(k) == cor_j2 ) || ( sel_b_jets.at(i) == cor_b && sel_jets.at(j) == cor_j2 && sel_jets.at(k) == cor_j1) )
                        {
                        	int leptonic_b = -1;
                        	if(i == 0){ leptonic_b = sel_b_jets.at(1); }
                        	else{ leptonic_b = sel_b_jets.at(0); }			//i == 1
                            
							c_evt_weight = weight;

							TLorentzVector p_mva_j1, p_mva_j2, p_mva_hadb;

                			p_mva_j1.SetPxPyPzE(jetInfo.Px[cor_j1],jetInfo.Py[cor_j1],jetInfo.Pz[cor_j1],jetInfo.Energy[cor_j1]);
                			p_mva_j2.SetPxPyPzE(jetInfo.Px[cor_j2],jetInfo.Py[cor_j2],jetInfo.Pz[cor_j2],jetInfo.Energy[cor_j2]);
                			p_mva_hadb.SetPxPyPzE(jetInfo.Px[cor_b],jetInfo.Py[cor_b],jetInfo.Pz[cor_b],jetInfo.Energy[cor_b]);
			
							double t_mass = ( p_mva_j1 + p_mva_j2 + p_mva_hadb ).M();
							double w_mass = ( p_mva_j1 + p_mva_j2 ).M()	;
							c_top_mass = t_mass;
							c_w_mass = w_mass;
							
							c_top_mass_dev = (t_mass - 172.5)/16.3;
							c_w_mass_dev = (w_mass - 82.9)/9.5;

                            c_j1j2_sumPt = jetInfo.Pt[ cor_j1 ] + jetInfo.Pt[ cor_j2 ];
                            c_j1j2_delPhi = TVector2::Phi_mpi_pi( jetInfo.Phi[ cor_j1 ] - jetInfo.Phi[ cor_j2 ] );
                            c_j1j2_absdelEta = fabs( jetInfo.Eta[ cor_j1 ] - jetInfo.Eta[ cor_j2 ] );
                            c_lepblep_sumPt = leptonInfo.Pt[ idx_Selected_Lep ] + jetInfo.Pt[ leptonic_b ];
                            c_lepblep_absdelEta = fabs( leptonInfo.Eta[ idx_Selected_Lep ] - jetInfo.Eta[ leptonic_b ] );
                            c_lepblep_delPhi = TVector2::Phi_mpi_pi( leptonInfo.Phi[ idx_Selected_Lep ] - jetInfo.Phi[ leptonic_b ] );


                            correct->Fill();
                        }
                        else	//incorrect combination
                        {
                        	int leptonic_b = -1;
                        	if(i == 0){ leptonic_b = sel_b_jets.at(1); }
                        	else{ leptonic_b = sel_b_jets.at(0); }			//i == 1
							
							i_evt_weight = weight;

							TLorentzVector p_mva_j1, p_mva_j2, p_mva_hadb;

                			p_mva_j1.SetPxPyPzE(jetInfo.Px[sel_jets.at(j)],jetInfo.Py[sel_jets.at(j)],jetInfo.Pz[sel_jets.at(j)],jetInfo.Energy[sel_jets.at(j)]);
                			p_mva_j2.SetPxPyPzE(jetInfo.Px[sel_jets.at(k)],jetInfo.Py[sel_jets.at(k)],jetInfo.Pz[sel_jets.at(k)],jetInfo.Energy[sel_jets.at(k)]);
                			p_mva_hadb.SetPxPyPzE(jetInfo.Px[sel_b_jets.at(i)],jetInfo.Py[sel_b_jets.at(i)],jetInfo.Pz[sel_b_jets.at(i)],jetInfo.Energy[sel_b_jets.at(i)]);
			
							double t_mass = ( p_mva_j1 + p_mva_j2 + p_mva_hadb ).M();
							double w_mass = ( p_mva_j1 + p_mva_j2 ).M();
								
							i_top_mass = t_mass;
							i_w_mass = w_mass;
													
							i_top_mass_dev = (t_mass - 172.5)/16.3;
							i_w_mass_dev = (w_mass - 82.9)/9.5;


                            i_j1j2_sumPt = jetInfo.Pt[ sel_jets.at(j) ] + jetInfo.Pt[ sel_jets.at(k) ];
                            i_j1j2_delPhi = TVector2::Phi_mpi_pi( jetInfo.Phi[ sel_jets.at(j) ] - jetInfo.Phi[ sel_jets.at(k) ] );
                            i_j1j2_absdelEta = fabs( jetInfo.Eta[ sel_jets.at(j) ] - jetInfo.Eta[ sel_jets.at(k) ] );
                            i_lepblep_sumPt = leptonInfo.Pt[ idx_Selected_Lep ] + jetInfo.Pt[ leptonic_b ];
                            i_lepblep_absdelEta = fabs( leptonInfo.Eta[ idx_Selected_Lep ] - jetInfo.Eta[ leptonic_b ] );
                            i_lepblep_delPhi = TVector2::Phi_mpi_pi( leptonInfo.Phi[ idx_Selected_Lep ] - jetInfo.Phi[ leptonic_b ] );

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

