/**************************************************************************************
 *
 *	File Name : corbb_t20_10_MLP.cc
 *	Description : for train_20_10
 *	Date : 191016~
 *	Author : Chen-Yu Chuang
 *
 ****************************************************************************************/
#include "TopCPViolation/selected/interface/Comp_DataMC.h"
#include "TopCPViolation/selected/interface/kinematic_tool.h"
#include "/wk_cms2/cychuang/CMSSW_9_4_2/src/wk_mva/train_20_10/weights/TMVAClassification_MLP.class.C"

#include "TopCPViolation/selected/interface/GenMgr.h"	//for cor

using namespace std;

int main(int argc,char* argv[])
{

	string pre_file_name = string( argv[0] );

	string data_sets_name[7] = {"TT","DY","WJets","VV","ST","QCD","Data"};
	string d6 = data_sets_name[6] + "_SM";
	string d7 = data_sets_name[6] + "_SE";
	
	//***********To finish something about weight***********//

	vector<double> w_TT, w_DY, w_WJets, w_VV, w_ST, w_QCD;
	map< string, vector<double>* > Weights_map;
	Weights_map[data_sets_name[0]] = &w_TT;				Weights_map[data_sets_name[1]] = &w_DY;
	Weights_map[data_sets_name[2]] = &w_WJets;			Weights_map[data_sets_name[3]] = &w_VV;
	Weights_map[data_sets_name[4]] = &w_ST;				Weights_map[data_sets_name[5]] = &w_QCD;
	get_lumi_weight( Weights_map, "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/selected/info./data_sets_full_sel.txt" );

	//*********************About path***********************//
	
	//vector<char*> TT, DY, WJets, VV, ST, QCD, Data_SM, Data_SE;
	vector<string> TT, DY, WJets, VV, ST, QCD, Data_SM, Data_SE;
	//map< string, vector<char*>* > Data_Set_Path;
	map< string, vector<string>* > Data_Set_Path;
	Data_Set_Path[data_sets_name[0]] = &TT;				Data_Set_Path[data_sets_name[1]] = &DY;
	Data_Set_Path[data_sets_name[2]] = &WJets;			Data_Set_Path[data_sets_name[3]] = &VV;
	Data_Set_Path[data_sets_name[4]] = &ST;				Data_Set_Path[data_sets_name[5]] = &QCD;
	Data_Set_Path[d6] = &Data_SM;				Data_Set_Path[d7] = &Data_SE;
	get_path( Data_Set_Path, "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/selected/info./data_sets_full_sel.txt" );

	//*********Offline High level Trigger Dealing**********//
	
	vector<int> HLT_MC_mu, HLT_MC_el, HLT_Data_mu, HLT_Data_el;
	
	Get_HLT_Info( string("MC_mu"), HLT_MC_mu );			Get_HLT_Info( string("MC_el"), HLT_MC_el );
	Get_HLT_Info( string("Data_mu"), HLT_Data_mu );		Get_HLT_Info( string("Data_el"), HLT_Data_el );
	
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

	
	//*****************declare/make some object ( histograms or vector ......etc.)******************//

	Hists_bb histbb;
	histbb.SetAlgoBin(50,0.,1.);
	histbb.Init();

	Hists_cor histcor;
	histcor.SetAlgoBin(50,0.,1.);
	histcor.Init();

	//**********initial the files and TChain and make the file map and weight map***********//

	map< string , vector<File>* > files_map;
	
	vector<File> files_TT;			vector<File> files_DY;
	vector<File> files_WJets;		vector<File> files_VV;
	vector<File> files_ST;			vector<File> files_QCD;

	File f_TT_001((char*)TT[0].c_str());
	files_TT.push_back(f_TT_001);
	for(int i=0;i<(int)DY.size();i++){
		File f_DY((char*)DY[i].c_str());		files_DY.push_back(f_DY);
	}
	for(int i=0;i<(int)WJets.size();i++){
		File f_WJets((char*)WJets[i].c_str());	files_WJets.push_back(f_WJets);
	}
	for(int i=0;i<(int)VV.size();i++){
		File f_VV((char*)VV[i].c_str());		files_VV.push_back(f_VV);
	}
	for(int i=0;i<(int)ST.size();i++){
		File f_ST((char*)ST[i].c_str());		files_ST.push_back(f_ST);
	}	
	for(int i=0;i<(int)QCD.size();i++){
		File f_QCD((char*)QCD[i].c_str());		files_QCD.push_back(f_QCD);
	}

	files_map[data_sets_name[0]] = &files_TT;		files_map[data_sets_name[1]] = &files_DY;
	files_map[data_sets_name[2]] = &files_WJets;	files_map[data_sets_name[3]] = &files_VV;
	files_map[data_sets_name[4]] = &files_ST;		files_map[data_sets_name[5]] = &files_QCD;

	//**************Data Part**************//

	vector<File> files_Data;
	
	File f_Data_1((char*)Data_SM[0].c_str());
	for(int i=1;i<(int)Data_SM.size();i++){
		f_Data_1.AddFile((char*)Data_SM[i].c_str());
	}
	File f_Data_2((char*)Data_SE[0].c_str());
	for(int i=1;i<(int)Data_SE.size();i++){
		f_Data_2.AddFile((char*)Data_SE[i].c_str());
	}
	
	files_Data.push_back(f_Data_1);		files_Data.push_back(f_Data_2);

	files_map[ data_sets_name[6] ] = &files_Data;

	//*****prepare cor objects*****//

	double not_tag_t = 0.;
	double not_tag_mu = 0.;
	double not_tag_el = 0.;
	
	//*****prepare mva objects*****//
	vector<string> inputVars;
	inputVars.push_back("top_mass");
	inputVars.push_back("w_mass");

	inputVars.push_back("j1j2_sumPt");
	inputVars.push_back("j1j2_absdelPt");
	inputVars.push_back("j1j2_delR");
    
	inputVars.push_back("whadb_sumPt");
	inputVars.push_back("whadb_delPt");
	inputVars.push_back("whadb_delR");

	inputVars.push_back("hadblepton_sumPt");
	inputVars.push_back("hadblepton_delPt");
	inputVars.push_back("hadblepton_delR");

	inputVars.push_back("hadwlepton_sumPt");
	inputVars.push_back("hadwlepton_delPt");
	inputVars.push_back("hadwlepton_delR");

	inputVars.push_back("hadbmet_delPhi");
	inputVars.push_back("hadbmet_sumPt");
	inputVars.push_back("hadbmet_delPt");

	inputVars.push_back("hadwmet_delPhi");
	inputVars.push_back("hadwmet_sumPt");
	inputVars.push_back("hadwmet_delPt");

	ReadMLP MyMVA( inputVars );

	//**********************Start Analysis***********************//

	for(int k=0;k<(int)files_map.size();k++)
	{
		double weight = 1.;
		bool is_data = false;

		string Set_name = data_sets_name[k];

		if( Set_name == "Data" )
		{ is_data = true; }
		
		//loop around in a kind of data set that is files_map[ data_sets_name[k] ] (r-loop)
		for(int r=0;r<(int)files_map[ Set_name ]->size();r++)
		{
			//just apply TTMC/Data//
			//if( Set_name != "TT" && Set_name != "Data" )
			if( Set_name != "TT" )
			{	break;	}

			if(!is_data){
				if( r >= (int)Data_Set_Path[ Set_name ]->size() )
				{	break;	}
			}
			//Get lumi_weight
			double lumi_weight = 0.;
			if(!is_data)
			{	
				lumi_weight = Weights_map[ Set_name ]->at(r);
			}
			else
			{	lumi_weight = 1.;	}

			//Register branches

			printf("\n The file now dealing with is under the data sets %s, the %d one.",Set_name.c_str(),r+1);

			EvtInfo evtInfo ;
			evtInfo.Register( files_map[ Set_name ]->at(r).ch );
			JetInfo jetInfo ;
			jetInfo.Register( files_map[ Set_name ]->at(r).ch , "JetInfo" );
			LeptonInfo leptonInfo;
			leptonInfo.Register( files_map[ Set_name ]->at(r).ch , "LepInfo" );
			GenInfo genInfo;
			genInfo.Register( files_map[ Set_name ]->at(r).ch );		
			TrgInfo trgInfo;
			trgInfo.Register( files_map[ Set_name ]->at(r).ch , "TrgInfo" );
			
			/*
			VertexInfo vertexInfo;
			vertexInfo.Register( files_map[ Set_name ]->at(r).ch , "VertexInfo" );
			RunInfo runInfo;
			runInfo.Register( files_map[ Set_name ]->at(r).ch , "RunInfo" );
			PhotonInfo photonInfo;
			photonInfo.Register( files_map[ Set_name ]->at(r).ch , "PhotonInfo" );
			*/
			
			int t_entries = (files_map[ Set_name ]->at(r).ch)->GetEntries();
			printf("\nAnd the Entries of this data files are : %d\n",t_entries);
			string channel = "";	//lep is muon or electron

			//prepare btag object
			BtagManager bmgr( &jetInfo );
			bmgr.Register_Init_Maps();
			bmgr.Register_Init_TH2( eff_b, eff_c, eff_l );

			GenMgr genmgr( &genInfo, &jetInfo, &leptonInfo );
			int u = 1;	
			for(int entry=0;entry<(int)t_entries;++entry)
			{
				//test!!!
				//if(entry>=3000.)
				//{	break;	}

				//cout << "[ Entry " << entry << " ]" << endl;
				
				if( (double)entry/t_entries >= 0.1*u )
				{	
					cout << "At percentage : " << (double)0.1 * u * 100 << " %" << endl;
					u++;
				}

				( files_map[ Set_name ]->at(r).ch )->GetEntry(entry);
				//check
				
				//Set lumi_Weight first

				double btag_weight = 1.;

				weight = 1.;
				if( !is_data )
				{
					weight = lumi_weight; 		//first input lumi-weight
				}

				//pile-up reweigh MC

				bool pass_PU = true;
				if(!is_data){
					weight = weight * GetPileUpWeight( &evtInfo, pileupinfo, &pass_PU );
				}
				else{	weight = 1.;	}
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
				if( is_data )
				{
						
					//if(channel == "mu")
					//{	is_pass_HLT = Pass_HLT( trgInfo, HLT_Data_mu );	}
					//else if(channel == "el")
					//{	is_pass_HLT = Pass_HLT( trgInfo, HLT_Data_el );	}
					
					if( r == 0 )
					{	is_pass_HLT = Pass_HLT( trgInfo, HLT_Data_mu );	}
					else if( r == 1 )
					{	is_pass_HLT = Pass_HLT( trgInfo, HLT_Data_el );	}
				}
				else
				{	
					if(channel == "mu")
					{	is_pass_HLT = Pass_HLT( trgInfo, HLT_MC_mu );	}
					else if(channel == "el")
					{	is_pass_HLT = Pass_HLT( trgInfo, HLT_MC_el );	}
				}
				if( !is_pass_HLT )
				{	continue;	}
				

				//luminosity cali after trigger

				if(!is_data)
				{
					if(channel == "mu")
					{	weight *= lumi_cali_trg(35.811/35.9);	}
					if(channel == "el")
					{	weight *= lumi_cali_trg(35.615/35.9);	}
				}

				//Then ,do the jet-selection here
				bool pass_sel_jet = Pass_SR_Selected_Jets_Case(jetInfo,sel_jets);


				if( (is_mu_channel && !is_el_channel) || (is_el_channel && !is_mu_channel) )
				{
					if( !pass_sel_jet )
					{	continue;	}

					//selected-jets and selected-lepton distinguishable (delta R)
					bool pass_delR = Pass_delR_Jet_Lep(jetInfo, leptonInfo,sel_jets, idx_Selected_Lep );
					if( !pass_delR )
					{	continue;	}

					//Do the b-jets-selected
					//in Control region , the selected b jets number is 0 ,and even by loose b-jet criteria
					bool pass_sel_b = Pass_SR_bjets(jetInfo,sel_jets, sel_b_jets);
					if( !pass_sel_b )	
					{	continue;	}

					//Do other reweight to MC
					
					if(!is_data)
					{
						//do the b-tag reweighting
						
						bmgr.Reset_idx_capacity();
						bmgr.Set_b_tagged_jets_idx( sel_b_jets );
						bmgr.Set_b_ntagged_jets_idx( sel_jets );
						btag_weight = bmgr.Get_Btag_Weight();
						weight *= btag_weight;

						//do the lepton scale factor
						if(idx_Selected_Lep == idx_Selected_Mu){
							weight = weight * GetLepSF_TH2F(&leptonInfo,h_tightMuIDSF,idx_Selected_Lep);
							weight = weight * GetLepSF_TH2F(&leptonInfo,h_MuISOSF,idx_Selected_Lep);
							weight = weight * GetLepSF_TH2F(&leptonInfo,h_MuTrgSF,idx_Selected_Lep);
						}
						else if(idx_Selected_Lep == idx_Selected_El){
							weight = weight * GetLepSF_TH2F(&leptonInfo,h_tightElIDSF,idx_Selected_Lep);
							weight = weight * GetLepSF_TH2F(&leptonInfo,h_ElRECOSF,idx_Selected_Lep);
							weight = weight * GetLepSF_TH2D(&leptonInfo,h_ElTrgSF,idx_Selected_Lep);
						}
					
						//do the gen-weight (second linear order calibration)
						weight *= Get_GenMinus_Weight(genInfo);
					}
				}
				else
				{	continue;	}

				//count the events No. which pass full-sel
				
				histcor.h_chosen[channel]->Fill(1.,weight);
				histcor.h_chosen_t->Fill(1.,weight);

				//do the mva algorithm to pick up the best combination
				
				//mva method

				int mva_hadb = -1, mva_j1 = -1, mva_j2 = -1;
				int mva_lepb = -1 ;
				int var_num = (int)inputVars.size();
				double* var = new double[ var_num ];
				double max_mva_value = -1.;				//ANN's value is 0~1
				
				for(int B=0;B<(int)sel_b_jets.size();B++)
				{
					int lB = (B == 0) ? 1 : 0 ;
					for(int J1=0;J1<(int)sel_jets.size();J1++)
					{
						for(int J2=J1+1;J2<(int)sel_jets.size();J2++)
						{
							int tmp_mva_lepb = sel_b_jets.at(lB);
							int tmp_mva_hadb = sel_b_jets.at(B);
							int tmp_mva_j1 = sel_jets.at(J1);
							int tmp_mva_j2 = sel_jets.at(J2);

							TLorentzVector p_mva_j1, p_mva_j2, p_mva_hadb, p_mva_lepb, p_mva_lepton;
							
							p_mva_j1 = GetJetP4(jetInfo,tmp_mva_j1);	
							p_mva_j2 = GetJetP4(jetInfo,tmp_mva_j2);	
							p_mva_hadb = GetJetP4(jetInfo,tmp_mva_hadb);	
							p_mva_lepb = GetJetP4(jetInfo,tmp_mva_lepb);	
							
							TLorentzVector p_mva_hadw = ( p_mva_j1 + p_mva_j2 );
							
							var[0] = ( p_mva_j1 + p_mva_j2 + p_mva_hadb ).M();
							var[1] = ( p_mva_j1 + p_mva_j2 ).M();

							var[2] = ( jetInfo.Pt[tmp_mva_j1] + jetInfo.Pt[tmp_mva_j2] );
							var[3] = fabs( jetInfo.Pt[ tmp_mva_j1 ] - jetInfo.Pt[ tmp_mva_j2 ] );
							var[4] = delta_R( jetInfo.Eta[ tmp_mva_j1 ], jetInfo.Eta[ tmp_mva_j2 ], jetInfo.Phi[ tmp_mva_j1 ], jetInfo.Phi[ tmp_mva_j2 ]);
							
							var[5] = p_mva_hadw.Pt() + p_mva_hadb.Pt();
							var[6] = p_mva_hadw.Pt() - p_mva_hadb.Pt();							
							var[7] = delta_R( p_mva_hadw.Eta(), p_mva_hadb.Eta(), p_mva_hadw.Phi(), p_mva_hadb.Phi() );

							var[8] = jetInfo.Pt[ tmp_mva_hadb ] + leptonInfo.Pt[ idx_Selected_Lep ];
							var[9] = jetInfo.Pt[ tmp_mva_hadb ] - leptonInfo.Pt[ idx_Selected_Lep ];
							var[10] = delta_R( jetInfo.Eta[ tmp_mva_hadb ], leptonInfo.Eta[ idx_Selected_Lep ], jetInfo.Phi[ tmp_mva_hadb ], leptonInfo.Phi[ idx_Selected_Lep ]);

							var[11] = p_mva_hadw.Pt() + leptonInfo.Pt[ idx_Selected_Lep ];
							var[12] = p_mva_hadw.Pt() - leptonInfo.Pt[ idx_Selected_Lep ];
							var[13] = delta_R( p_mva_hadw.Eta(), leptonInfo.Eta[ idx_Selected_Lep ], p_mva_hadw.Phi(), leptonInfo.Phi[ idx_Selected_Lep ]);
							
							var[14] = TVector2::Phi_mpi_pi( jetInfo.Phi[ tmp_mva_hadb ] - evtInfo.PFMETPhi );
							var[15] = jetInfo.Pt[ tmp_mva_hadb ] + evtInfo.PFMET;
							var[16] = jetInfo.Pt[ tmp_mva_hadb ] - evtInfo.PFMET;
							
							var[17] = TVector2::Phi_mpi_pi( p_mva_hadw.Phi() - evtInfo.PFMETPhi );
							var[18] = p_mva_hadw.Pt() + evtInfo.PFMET;
							var[19] = p_mva_hadw.Pt() - evtInfo.PFMET;


							vector<double> inputValues;
							for(int in=0;in<var_num;in++ )
							{	inputValues.push_back( var[in] );	}
								
							double tmp_mva_value = MyMVA.GetMvaValue( inputValues );

							//cout << tmp_mva_value << " ";
							if( tmp_mva_value >= max_mva_value )
							{
								max_mva_value = tmp_mva_value;
								mva_hadb = tmp_mva_hadb;
								mva_j1 = tmp_mva_j1;
								mva_j2 = tmp_mva_j2;
								mva_lepb = tmp_mva_lepb;
								//cout << endl;
							}
						}
					}
				}

				delete [] var;

				//calculate the correctness of mva method

				TLorentzVector p_mva_j1, p_mva_j2, p_mva_hadb, p_mva_lepb, p_mva_lepton;

                p_mva_j1.SetPxPyPzE(jetInfo.Px[mva_j1],jetInfo.Py[mva_j1],jetInfo.Pz[mva_j1],jetInfo.Energy[mva_j1]);
                p_mva_j2.SetPxPyPzE(jetInfo.Px[mva_j2],jetInfo.Py[mva_j2],jetInfo.Pz[mva_j2],jetInfo.Energy[mva_j2]);
                p_mva_hadb.SetPxPyPzE(jetInfo.Px[mva_hadb],jetInfo.Py[mva_hadb],jetInfo.Pz[mva_hadb],jetInfo.Energy[mva_hadb]);
                p_mva_lepb.SetPxPyPzE(jetInfo.Px[mva_lepb],jetInfo.Py[mva_lepb],jetInfo.Pz[mva_lepb],jetInfo.Energy[mva_lepb]);
                p_mva_lepton.SetPxPyPzE(leptonInfo.Px[idx_Selected_Lep],leptonInfo.Py[idx_Selected_Lep],leptonInfo.Pz[idx_Selected_Lep],leptonInfo.Energy[idx_Selected_Lep]);

				
				double mva_tmass = ( p_mva_j1 + p_mva_j2 + p_mva_hadb ).M();
				double mva_lep_t_mass = ( p_mva_lepb + p_mva_lepton ).M();
				
				histbb.h_mvamax_mass[channel]->Fill( mva_tmass, max_mva_value, weight );
				histbb.h_mvamax_mass_t->Fill( mva_tmass, max_mva_value, weight );


				//To see the part of bbbar separetion of mva part
				//
				//
				int had_b = mva_hadb;	//which we chose in jjb (reconstructed top quark)	
				int lep_b = mva_lepb;
				int b = -1;
				int bbar = -1;
				int lep = idx_Selected_Lep;	

				//***To decide the b and bbar which is had_b and which is lep_b***//
				/*selected lepton's charge sign is same as the hadronic b's charge sign*/

				double lep_charge = leptonInfo.Charge[ lep ];
				if( lep_charge > 0 )
				{
					b = lep_b;
					bbar = had_b;
				}
				else if( lep_charge < 0 )
				{
					b = had_b;
					bbar = lep_b;
				}
				else
				{	continue;	}

				//***To decide the b and bbar matching type with GenInfo (generation level's information)***//
				bb_matching_type bbType = no_match;			//default
				bbType = Get_bb_Option( b , bbar , lep , jetInfo , leptonInfo , genInfo );
				
				//***To fill the histogram***//
	
				//Fill the histograms
				switch(bbType)
				{
					case correct:
						histbb.h_correct[channel]->Fill( max_mva_value, weight );
						histbb.h_correct_t->Fill( max_mva_value, weight );
						histbb.h_lt_correct[channel]->Fill( mva_lep_t_mass, weight );
						histbb.h_lt_correct_t->Fill( mva_lep_t_mass, weight );
						break;
					case charge_mis:
						histbb.h_charge_mis[channel]->Fill(max_mva_value,weight);
						histbb.h_charge_mis_t->Fill(max_mva_value,weight);
						histbb.h_lt_charge_mis[channel]->Fill( mva_lep_t_mass ,weight);
						histbb.h_lt_charge_mis_t->Fill( mva_lep_t_mass ,weight);
						break;
					case mistag:
						histbb.h_mistag[channel]->Fill(max_mva_value,weight);
						histbb.h_mistag_t->Fill(max_mva_value,weight);
						histbb.h_lt_mistag[channel]->Fill( mva_lep_t_mass ,weight);
						histbb.h_lt_mistag_t->Fill( mva_lep_t_mass ,weight);
						break;
					case no_match:
						histbb.no_match_number++;
						break;
					default:
						histbb.no_match_number++;
				}

				//***** do calculate the cor *****//

				int cor_b = -1, cor_j1 = -1, cor_j2 = -1;
        		bool is_good_trained_evt = genmgr.Find_Correct_HadronicTop( cor_b, cor_j1, cor_j2 );
				if( !is_good_trained_evt )
				{	
					not_tag_t += weight;
					if( channel == "mu" )
					{ not_tag_mu += weight; }
					else if( channel == "el" )
					{ not_tag_el += weight; }
				}
				else
				{
					if( (mva_hadb == cor_b && mva_j1 == cor_j1 && mva_j2 == cor_j2) \
						|| (mva_hadb == cor_b && mva_j1 == cor_j2 && mva_j2 == cor_j1) )
					{
						histcor.h_max_mva_cor[channel]->Fill( max_mva_value, weight );
						histcor.h_max_mva_cor_t->Fill( max_mva_value, weight );
						histcor.h_cor[channel]->Fill(1.,weight);
						histcor.h_cor_t->Fill(1.,weight);
					}
					else
					{
						histcor.h_max_mva_incor[channel]->Fill( max_mva_value, weight );
						histcor.h_max_mva_incor_t->Fill( max_mva_value, weight );
					}
				}
			}	//end of entry for-loop	
		}	//end of r for-loop
	}		//end of k for-loop

	//*****Drawing Plotting or Outputting files*****//

	//normalize		
	
	//Save these hists to be a root file
	
	string time_str = "";
	time_str = get_time_str( minute );
	string new_file_name = string("cor_bbsep_") + pre_file_name + time_str + string(".root");

	TFile* f_out = new TFile( new_file_name.c_str() , "recreate" );	
	
	histbb.WriteIn();
	histcor.WriteIn();

	f_out->Close();
	//delete f_out;
	
	double t_weight_t = t_IntegralTH1( histcor.h_chosen_t );
	double t_weight_mu = t_IntegralTH1( histcor.h_chosen_mu );
	double t_weight_el = t_IntegralTH1( histcor.h_chosen_el );

	double cor_weight_t = t_IntegralTH1( histcor.h_cor_t );
	double cor_weight_mu = t_IntegralTH1( histcor.h_cor_mu );
	double cor_weight_el = t_IntegralTH1( histcor.h_cor_el );

	histcor.h_cor_t->Divide( histcor.h_chosen_t );
	histcor.h_cor_mu->Divide( histcor.h_chosen_mu );
	histcor.h_cor_el->Divide( histcor.h_chosen_el );

	ofstream f;
	f.open( "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/selected/info./cor_result.txt",fstream::app );		

	f << "File name " << pre_file_name << endl;
	f << "Finish time is : " << time_str << endl;

	f << "mis_sel ratio: " << (double)not_tag_t/t_weight_t  << endl;
   	
	f << "cor_mva ratio : " << (double)cor_weight_t/t_weight_t << endl; 
	f << "Error : +-" << (double) histcor.h_cor_t->GetBinError(2) << endl; 	
   	f << "cor_mva_mu ratio : " << (double)cor_weight_mu/t_weight_mu << endl; 	
	f << "Error : +-" << (double) histcor.h_cor_mu->GetBinError(2) << endl; 	
   	f << "cor_mva_el ratio : " << (double)cor_weight_el/t_weight_el << endl; 	
	f << "Error : +-" << (double) histcor.h_cor_el->GetBinError(2) << endl; 	
   	
	f << "no-match number : " <<  histbb.no_match_number << endl;
	f << "==============================================================" << endl; 
	//*****make space free*****//
	
	delete f7;
	delete f6;
	delete f5;
	delete f4;
	delete f3;
	delete f2;
	delete f1;
}

