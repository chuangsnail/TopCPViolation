/**************************************************************************************
 *
 *	File Name : CR_t20_10_MLP_all.cc
 *	Description : use new hists object to store histagrams.
 *	Date : 191112~
 *	Author : Chen-Yu Chuang
 *
 ****************************************************************************************/
#include "TopCPViolation/selected/interface/Comp_DataMC.h"
#include "/wk_cms2/cychuang/CMSSW_9_4_2/src/wk_mva/p03_all/weights/TMVAClassification_MLP.class.C"
#include "TopCPViolation/selected/interface/Hists.h"
#include "TopCPViolation/selected/interface/MVAvar.h"

using namespace std;
using namespace mvatool;

int main(int argc,char* argv[])
{
	double mva_cut = stod( string(argv[1]) );
	string training_name = string("p03") + string("_MLP");
	string inv_name = string( argv[2] ) + string(".root");				//invIso used to Data-driven QCD,need to do the inv first

	string data_sets_name[7] = {"TT","DY","WJets","VV","ST","QCD","Data"};
	string d6 = data_sets_name[6] + "_SM";
	string d7 = data_sets_name[6] + "_SE";
	
	//***********To finish something about weight***********//

	vector<double> w_TT, w_DY, w_WJets, w_VV, w_ST, w_QCD;
	map< string, vector<double>* > Weights_map;
	Weights_map[data_sets_name[0]] = &w_TT;				Weights_map[data_sets_name[1]] = &w_DY;
	Weights_map[data_sets_name[2]] = &w_WJets;			Weights_map[data_sets_name[3]] = &w_VV;
	Weights_map[data_sets_name[4]] = &w_ST;				Weights_map[data_sets_name[5]] = &w_QCD;
	get_lumi_weight( Weights_map );
	cout << "Finish getting lumi-weight" << endl;

	//*********************About path***********************//
	
	//vector<char*> TT, DY, WJets, VV, ST, QCD, Data_SM, Data_SE;
	vector<string> TT, DY, WJets, VV, ST, QCD, Data_SM, Data_SE;
	//map< string, vector<char*>* > Data_Set_Path;
	map< string, vector<string>* > Data_Set_Path;
	Data_Set_Path[data_sets_name[0]] = &TT;				Data_Set_Path[data_sets_name[1]] = &DY;
	Data_Set_Path[data_sets_name[2]] = &WJets;			Data_Set_Path[data_sets_name[3]] = &VV;
	Data_Set_Path[data_sets_name[4]] = &ST;				Data_Set_Path[data_sets_name[5]] = &QCD;
	Data_Set_Path[d6] = &Data_SM;						Data_Set_Path[d7] = &Data_SE;
	get_path( Data_Set_Path );
	cout << "Finish getting Path info." << endl;

	//*********Offline High level Trigger Dealing**********//
	
	vector<int> HLT_MC_mu, HLT_MC_el, HLT_Data_mu, HLT_Data_el;	
	Get_HLT_Info( string("MC_mu"), HLT_MC_mu );			Get_HLT_Info( string("MC_el"), HLT_MC_el );
	Get_HLT_Info( string("Data_mu"), HLT_Data_mu );		Get_HLT_Info( string("Data_el"), HLT_Data_el );
	
	//*******To prepare the things about MC reweigh********//

	//insert pileup re-weighting information
	vector<double> pileupinfo;
	pileupinfo.assign(76,0.);		int PUI_size = 76;
	char path_PU_file[200] = "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/pileupweights_69200.csv"; 
	GetPileUpInfo( path_PU_file , pileupinfo , &PUI_size);
/*
	TFile* f1 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/muIDSF.root");
	TH2F* h_tightMuIDSF;		f1->GetObject("abseta_pt_ratio",h_tightMuIDSF);

	TFile* f2 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/muISOSF.root");
	TH2F* h_MuISOSF;			f2->GetObject("abseta_pt_ratio",h_MuISOSF);

	TFile* f3 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/muTrgSF.root");
	TH2F* h_MuTrgSF;			f3->GetObject("abseta_pt_ratio",h_MuTrgSF);

	TFile* f4 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/elTightIDSF.root");
	TH2F* h_tightElIDSF;		f4->GetObject("EGamma_SF2D",h_tightElIDSF);

	TFile* f5 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/elRECOSF.root");
	TH2F* h_ElRECOSF;			f5->GetObject("EGamma_SF2D",h_ElRECOSF);

	TFile* f6 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/elTrgSF.root");
	TH2D* h_ElTrgSF;			f6->GetObject("scale_ele32",h_ElTrgSF);
	//we need to delete the f1~f7 after finising using the objects from them		//so we delete them after selecting
*/	
	TFile* f1 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/old_lep/muIDSF.root");
	TH2F* h_tightMuIDSF;		f1->GetObject("abseta_pt_ratio",h_tightMuIDSF);

	TFile* f2 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/Lep2016/muiso_Run16All.root");
	TH2D* h_MuISOSF;			f2->GetObject("muiso",h_MuISOSF);

	TFile* f3 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/Lep2016/HLT16_mu_SF.root");
	TH2F* h_MuTrgSF;			f3->GetObject("abseta_pt_ratio",h_MuTrgSF);

	TFile* f4 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/old_lep/elTightIDSF.root");
	TH2F* h_tightElIDSF;		f4->GetObject("EGamma_SF2D",h_tightElIDSF);

	TFile* f5 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/Lep2016/EGM2D_BtoH_GT20GeV_RecoSF_Legacy2016.root");
	TH2F* h_ElRECOSF;			f5->GetObject("EGamma_SF2D",h_ElRECOSF);

	TFile* f6 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/Lep2016/HLT_SF_16.root");
	TH2D* h_ElTrgSF;			f6->GetObject("abseta_pt_ratio",h_ElTrgSF);

	TH2F* eff_b;		TH2F* eff_c;		TH2F* eff_l;
	TFile* f7 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/beffPlot_WJets_0pt2217.root");
	f7->GetObject( "eff_b", eff_b );	f7->GetObject( "eff_c", eff_c );	f7->GetObject( "eff_l", eff_l );

	TH1F* h_data_dd_mu;		TH1F* h_data_dd_el;
	TH1F* h_l_data_dd_mu;	TH1F* h_l_data_dd_el;
	string inv_path_name = "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/" + inv_name;
	TFile* f_dd = new TFile( (char*)inv_path_name.c_str() );
	f_dd->GetObject( "h_Data_mu", h_data_dd_mu );
	f_dd->GetObject( "h_Data_el", h_data_dd_el );
	f_dd->GetObject( "h_l_Data_mu", h_l_data_dd_mu );
	f_dd->GetObject( "h_l_Data_el", h_l_data_dd_el );

	//**********initial the files and TChain and make the file map and weight map***********//

	map< string , vector<File>* > files_map;
	
	vector<File> files_TT;			vector<File> files_DY;
	vector<File> files_WJets;		vector<File> files_VV;
	vector<File> files_ST;			vector<File> files_QCD;

	File f_TT_001((char*)TT[0].c_str());
	files_TT.push_back(f_TT_001);
	for(int i=0;i<(int)DY.size();i++){
		File f_DY((char*)DY[i].c_str());			files_DY.push_back(f_DY);
	}
	for(int i=0;i<(int)WJets.size();i++){
		File f_WJets((char*)WJets[i].c_str());		files_WJets.push_back(f_WJets);
	}
	for(int i=0;i<(int)VV.size();i++){
		File f_VV((char*)VV[i].c_str());			files_VV.push_back(f_VV);
	}
	for(int i=0;i<(int)ST.size();i++){
		File f_ST((char*)ST[i].c_str());			files_ST.push_back(f_ST);
	}	
	for(int i=0;i<(int)QCD.size();i++){
		File f_QCD((char*)QCD[i].c_str());			files_QCD.push_back(f_QCD);
	}

	files_map[data_sets_name[0]] = &files_TT;		files_map[data_sets_name[1]] = &files_DY;
	files_map[data_sets_name[2]] = &files_WJets;	files_map[data_sets_name[3]] = &files_VV;
	files_map[data_sets_name[4]] = &files_ST;		files_map[data_sets_name[5]] = &files_QCD;

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

	//******prepare mva object******//
	
	vector<string> inputVars;
	
	AddVarName( inputVars );

	ReadMLP MyMVA( inputVars );

	//**********************Start Analysis***********************//

	//**************** open the file we want to store the result **********************************//

	string time_str = "";
	time_str = get_time_str( minute );
	string new_file_name = string("CR_") + training_name +"_" + time_str + ".root";

	TFile* f_out = new TFile( new_file_name.c_str() , "recreate" );
	//initialize ntuple must be after the initialization of the TFile
	
	//*****************declare/make some object ( histograms or vector ......etc.)******************//
	//make histograms
	Hists hists;
	hists.NoCutModeON();	
	hists.OneCutModeON();	
	hists.TwoCutModeON();	

	double* ntvar;		//for temporally store the ntuple vars
	hists.NtupleModeON();

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
			//if( Set_name != "TT" )
			//{	break;	}

			
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
			bmgr.Set_OP( BTagEntry::OP_LOOSE );
		
			int u = 1;	
			for(int entry=0;entry<(int)t_entries;++entry)
			{
				//test!!!
				//if(entry>=3000.)
				//{	break;	}
				
				if( (double)entry/t_entries >= 0.1*u )
				{	
					cout << "At percentage : " << (double)0.1 * u * 100 << " %" << endl;
					u++;
				}

				( files_map[ Set_name ]->at(r).ch )->GetEntry(entry);
				
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
				
				bool pass_sel_mu = Pass_CR_Selected_Muon(leptonInfo,idx_Selected_Mu);
				bool pass_sel_el = Pass_CR_Selected_Electron(leptonInfo,idx_Selected_El,evtInfo);
				bool pass_veto_el = false;		bool pass_veto_mu = false;

				if( (pass_sel_mu||pass_sel_el) && !(pass_sel_mu&&pass_sel_el) )
				{
					if(pass_sel_mu)
					{
						pass_veto_el = Pass_CR_Veto_Electron(leptonInfo,idx_Selected_Mu,evtInfo);
						pass_veto_mu = Pass_CR_Veto_Muon(leptonInfo,idx_Selected_Mu);
					}
					if(pass_sel_el)
					{
						pass_veto_el = Pass_CR_Veto_Electron(leptonInfo,idx_Selected_El,evtInfo);
						pass_veto_mu = Pass_CR_Veto_Muon(leptonInfo,idx_Selected_El);
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
				//no~

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
					bool pass_sel_b = Pass_CR_bjets(jetInfo,sel_jets, sel_b_jets);
					if( !pass_sel_b )	
					{	continue;	}

					//Do other reweight to MC
					
					if(!is_data)
					{
						//do the b-tag reweighting
						
						bmgr.Reset_idx_capacity();
						vector<int> tmp_all_jets;

						for(int h=0;h<(int)sel_jets.size();h++)
						{
							tmp_all_jets.push_back( sel_jets.at(h) );
						}
						for(int h=0;h<(int)sel_b_jets.size();h++)
						{
							tmp_all_jets.push_back( sel_b_jets.at(h) );
						}

						bmgr.Set_b_ntagged_jets_idx( tmp_all_jets );
						vector<int> tmp_empty_vec;
						bmgr.Set_b_tagged_jets_idx( tmp_empty_vec );
						btag_weight = bmgr.Get_Btag_Weight();
						weight *= btag_weight;

						//do the lepton scale factor
						if(idx_Selected_Lep == idx_Selected_Mu){
							weight = weight * GetLepSF_TH2F(&leptonInfo,h_tightMuIDSF,idx_Selected_Lep);
							weight = weight * GetLepSF_TH2D(&leptonInfo,h_MuISOSF,idx_Selected_Lep);
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
					//use mva to choose best choice of reconstruct top quark's mass 
				}
				else
				{	continue;	}

				double max_mva_value = -10.;
				double had_t_mass = -1.;
				double lep_t_mass = -1.;
				int mva_hadb = -1, mva_j1 = -1, mva_j2 = -1;
				int mva_lepb = -1 ;
				int var_num = (int)inputVars.size();
				double* var = new double[ var_num ];
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
							
							mvatool::InputVar( var, jetInfo, leptonInfo, evtInfo, sel_jets, sel_b_jets, idx_Selected_Lep, tmp_mva_hadb, tmp_mva_lepb, tmp_mva_j1, tmp_mva_j2 );

							vector<double> inputValues;
							for(int in=0;in<var_num;in++ )
							{	inputValues.push_back( var[in] );	}
								
							double tmp_mva_value = MyMVA.GetMvaValue( inputValues );
							if( tmp_mva_value >= max_mva_value )
							{
								max_mva_value = tmp_mva_value;
								mva_hadb = tmp_mva_hadb;
								mva_j1 = tmp_mva_j1;
								mva_j2 = tmp_mva_j2;
								mva_lepb = tmp_mva_lepb;
							}
						}
					}
				}

				delete [] var;
				
				//check mva-reco
				if( max_mva_value == -10. || mva_lepb == -1 )
				{
					cerr << "There is something wrong in the process of mva-reco " << endl;
					continue;	
				}


				TLorentzVector b1, j1, j2, b2, lepton;
                b1.SetPxPyPzE(jetInfo.Px[mva_hadb],jetInfo.Py[mva_hadb],jetInfo.Pz[mva_hadb],jetInfo.Energy[mva_hadb]);
                j1.SetPxPyPzE(jetInfo.Px[mva_j1],jetInfo.Py[mva_j1],jetInfo.Pz[mva_j1],jetInfo.Energy[mva_j1]);
                j2.SetPxPyPzE(jetInfo.Px[mva_j2],jetInfo.Py[mva_j2],jetInfo.Pz[mva_j2],jetInfo.Energy[mva_j2]);
                b2.SetPxPyPzE(jetInfo.Px[mva_lepb],jetInfo.Py[mva_lepb],jetInfo.Pz[mva_lepb],jetInfo.Energy[mva_lepb]);
				lepton.SetPxPyPzE( leptonInfo.Px[idx_Selected_Lep] ,leptonInfo.Py[idx_Selected_Lep], leptonInfo.Pz[idx_Selected_Lep], leptonInfo.Energy[idx_Selected_Lep]);
					
				had_t_mass = ( b1 + j1 + j2 ).M();
				lep_t_mass = (lepton + b2).M();

				ntvar = new double[5];
				ntvar[0] = max_mva_value;
				ntvar[1] = had_t_mass;
				ntvar[2] = lep_t_mass;
				ntvar[3] = weight;
				ntvar[4] = (double)k;
				hists.mvav_mass[channel]->Fill( ntvar );
				delete [] ntvar;
				
				if(channel == "mu")
				{	
					hists.h_l_mu.at(k)->Fill(lep_t_mass,weight);
					hists.h_mu.at(k)->Fill(had_t_mass,weight);
				}
				if(channel == "el")
				{	
					hists.h_l_el.at(k)->Fill(lep_t_mass,weight);
					hists.h_el.at(k)->Fill(had_t_mass,weight);
				}

				//mva_value cut
				if( max_mva_value <= mva_cut )
				{	continue;	}

				if(channel == "mu")
				{	
					hists.h_l_mu_c.at(k)->Fill(lep_t_mass,weight);
					hists.h_mu_c.at(k)->Fill(had_t_mass,weight);
				}
				if(channel == "el")
				{	
					hists.h_l_el_c.at(k)->Fill(lep_t_mass,weight);
					hists.h_el_c.at(k)->Fill(had_t_mass,weight);
				}

				//leptonic top mass cut 
				if( !(lep_t_mass < 150.))
				{	continue;	}

				if(channel == "mu")
				{	
					hists.h_l_mu_cc.at(k)->Fill(lep_t_mass,weight);
					hists.h_mu_cc.at(k)->Fill(had_t_mass,weight);
				}
				if(channel == "el")
				{	
					hists.h_l_el_cc.at(k)->Fill(lep_t_mass,weight);
					hists.h_el_cc.at(k)->Fill(had_t_mass,weight);
				}

			}	//end of entry for-loop	
			cout << endl << "The end of the file-sets " << Set_name << " " << r+1 << " " << endl;
			
		}		//end of r for-loop
	}			//end of k for-loop

	//Data-Driven for QCD

	hists.h_QCD_mu_c = Data_Driven(h_data_dd_mu,hists.h_QCD_mu_c);
	hists.h_QCD_el_c = Data_Driven(h_data_dd_el,hists.h_QCD_el_c);
	hists.h_l_QCD_mu_c = Data_Driven(h_l_data_dd_mu,hists.h_l_QCD_mu_c);
	hists.h_l_QCD_el_c = Data_Driven(h_l_data_dd_el,hists.h_l_QCD_el_c);

	hists.h_QCD_mu_c->SetName("h_QCD_mu_c");
	hists.h_QCD_el_c->SetName("h_QCD_el_c");
	hists.h_l_QCD_mu_c->SetName("h_l_QCD_mu_c");
	hists.h_l_QCD_el_c->SetName("h_l_QCD_el_c");
	
	//*****Drawing Plotting or Outputting files*****//

	//Save these hists to be a root file
	
	hists.WriteIn("NC 1C 2C NT");

	f_out->Close();
	delete f_out;

	//*****make space free*****//
	
	delete f7;
	delete f6;
	delete f5;
	delete f4;
	delete f3;
	delete f2;
	delete f1;
}

