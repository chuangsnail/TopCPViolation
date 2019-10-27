/**************************************************************************************
 *
 *	File Name : CR_190923.cc
 *	Description : use new BtagManager ~!
 *	Date : 190902~
 *	Author : Chen-Yu Chuang
 *
 ****************************************************************************************/
#include "TopCPViolation/selected/interface/Comp_DataMC.h"

using namespace std;

int main(int argc,char* argv[])
{
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

	cout << "Weights_map.size()" << (int)Weights_map.size() << endl;
	cout << "w_TT.size()  " << (int)w_TT.size() << endl;
	cout << "w_DY.size()  " << (int)w_DY.size() << endl;
	cout << "w_WJets.size()  " << (int)w_WJets.size() << endl;
	cout << "w_VV.size()  " << (int)w_VV.size() << endl;
	cout << "w_ST.size()  " << (int)w_ST.size() << endl;
	cout << "w_QCD.size()  " << (int)w_QCD.size() << endl;

	//*********************About path***********************//
	
	//vector<char*> TT, DY, WJets, VV, ST, QCD, Data_SM, Data_SE;
	vector<string> TT, DY, WJets, VV, ST, QCD, Data_SM, Data_SE;
	//map< string, vector<char*>* > Data_Set_Path;
	map< string, vector<string>* > Data_Set_Path;
	Data_Set_Path[data_sets_name[0]] = &TT;				Data_Set_Path[data_sets_name[1]] = &DY;
	Data_Set_Path[data_sets_name[2]] = &WJets;			Data_Set_Path[data_sets_name[3]] = &VV;
	Data_Set_Path[data_sets_name[4]] = &ST;				Data_Set_Path[data_sets_name[5]] = &QCD;
	Data_Set_Path[d6] = &Data_SM;				Data_Set_Path[d7] = &Data_SE;
	get_path( Data_Set_Path );

	cout << "Data_Set_Path.size()" << (int)Data_Set_Path.size() << endl;
	cout << "TT.size()  " << (int)TT.size() << endl;
	cout << "DY.size()  " << (int)DY.size() << endl;
	cout << "WJets.size()  " << (int)WJets.size() << endl;
	cout << "VV.size()  " << (int)VV.size() << endl;
	cout << "ST.size()  " << (int)ST.size() << endl;
	cout << "QCD.size()  " << (int)QCD.size() << endl;
	
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

	int bins_No = 50;
	double hist_min = 0.;
	double hist_max = 500.;
	string x_axis_name = "";
	string histogram_cons = " ;" + x_axis_name + ";Events(No.)";
	//double bins_weight = 1./( (hist_max-hist_min)/(double)bins_No );

	TH1F* h_TT_mu = new TH1F("h_TT_mu","",bins_No,hist_min,hist_max);
	TH1F* h_TT_el = new TH1F("h_TT_el","",bins_No,hist_min,hist_max);
	
	TH1F* h_DY_mu = new TH1F("h_DY_mu","",bins_No,hist_min,hist_max);
	TH1F* h_DY_el = new TH1F("h_DY_el","",bins_No,hist_min,hist_max);

	TH1F* h_WJets_mu = new TH1F("h_WJets_mu","",bins_No,hist_min,hist_max);
	TH1F* h_WJets_el = new TH1F("h_WJets_el","",bins_No,hist_min,hist_max);
	
	TH1F* h_VV_mu = new TH1F("h_VV_mu","",bins_No,hist_min,hist_max);
	TH1F* h_VV_el = new TH1F("h_VV_el","",bins_No,hist_min,hist_max);

	TH1F* h_ST_mu = new TH1F("h_ST_mu","",bins_No,hist_min,hist_max);
	TH1F* h_ST_el = new TH1F("h_ST_el","",bins_No,hist_min,hist_max);
	
	TH1F* h_QCD_mu = new TH1F("h_QCD_mu","",bins_No,hist_min,hist_max);
	TH1F* h_QCD_el = new TH1F("h_QCD_el","",bins_No,hist_min,hist_max);
	
	TH1F* h_Data_mu = new TH1F("h_Data_mu"," ;Hadronic Top Mass(GeV);Events(No.)",bins_No,hist_min,hist_max);	
	//"(primary title);(X-Axis title);(Y-Axis Title)"
	TH1F* h_Data_el = new TH1F("h_Data_el"," ;Hadronic Top Mass(GeV);Events(No.)",bins_No,hist_min,hist_max);

	TH1F* h_btag_w = new TH1F("h_btag_w", "",45,0.45,1.35);

	vector<TH1F*> h_mu;
	h_mu.push_back(h_TT_mu);		h_mu.push_back(h_DY_mu);
	h_mu.push_back(h_WJets_mu);		h_mu.push_back(h_VV_mu);
	h_mu.push_back(h_ST_mu);		h_mu.push_back(h_QCD_mu);
	h_mu.push_back(h_Data_mu);

	vector<TH1F*> h_el;
	h_el.push_back(h_TT_el);		h_el.push_back(h_DY_el);
	h_el.push_back(h_WJets_el);		h_el.push_back(h_VV_el);
	h_el.push_back(h_ST_el);		h_el.push_back(h_QCD_el);
	h_el.push_back(h_Data_el);
	
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


	//**************Real Data Analysis**************//

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
			//if( Set_name != "TT" )
			//{	break;	}
			////////////////////////

			cout << "At Set " << Set_name << " " << r+1 << "'s start, (h_mu[0], h_TT_mu) = " << h_mu[0] << ", " << h_TT_mu << endl;
			
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
		
			int u = 1;	
			for(int entry=0;entry<(int)t_entries;++entry)
			{
				//test!!!
				//if(entry>=2000.)
				//{	break;	}
				
				if( (double)entry/t_entries >= 0.2*u )
				{	
					cout << "At percentage : " << (double)0.2 * u * 100 << " %" << endl;
					u++;
				}

				( files_map[ Set_name ]->at(r).ch )->GetEntry(entry);
				//check
				if( h_TT_mu != h_mu.at(0) )
				{
					cout << endl << "h_TT_mu != h_mu(0)  at entry-" << entry << " of set " << Set_name << "'s " << r+1 << " one!" << endl;
					break;
				}
				
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

				if(!is_data)
				{
					if(channel == "mu")
					{	weight *= lumi_cali_trg(35.811/35.9);	}
					if(channel == "el")
					{	weight *= lumi_cali_trg(35.615/35.9);	}
				}

				//Then ,do the jet-selection here
				bool pass_sel_jet = Pass_SR_Selected_Jets_Case(jetInfo,sel_jets);

				double chi_square_value = -1.,	had_t_mass = -1.;
				double lep_t_mass = -1.;
		
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
						bmgr.Set_b_tagged_jets_idx( sel_b_jets );
						bmgr.Set_b_ntagged_jets_idx( sel_jets );
						btag_weight = bmgr.Get_Btag_Weight();
						weight *= btag_weight;

						//double SF1 = bmgr.Get_Btag_Scale_Factor( sel_b_jets.at(0) );
						//double SF2 = bmgr.Get_Btag_Scale_Factor( sel_b_jets.at(1) );
						//cout << "Btag ( SF1, SF2, SF1*SF2 ) of tagged 2 b-jets are ( " << SF1 << ", " << SF2 << ", " << SF1*SF2 << " )" << endl;

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
					//use chi2-Sorting to choose best choice of reconstruct top quark's mass 
					chi_square_value = Chi2_Sorting(jetInfo,sel_jets,sel_b_jets);	
				}
				else
				{	continue;	}

				//check if chi-2 sorting is sucessful

				if( chi_square_value < 0. )
				{	printf("\n-----FAULT------\nThere is something wrong in selection!\n");		continue;	}

				//Apply Chi2 Cut here
				
				if( !(chi_square_value < 20.))
				{	continue;	}	

				TLorentzVector b1, j1, j2, lepton, b2;
                j1.SetPxPyPzE(jetInfo.Px[sel_jets[0]],jetInfo.Py[sel_jets[0]],jetInfo.Pz[sel_jets[0]],jetInfo.Energy[sel_jets[0]]);
                j2.SetPxPyPzE(jetInfo.Px[sel_jets[1]],jetInfo.Py[sel_jets[1]],jetInfo.Pz[sel_jets[1]],jetInfo.Energy[sel_jets[1]]);
                b1.SetPxPyPzE(jetInfo.Px[sel_b_jets[0]],jetInfo.Py[sel_b_jets[0]],jetInfo.Pz[sel_b_jets[0]],jetInfo.Energy[sel_b_jets[0]]);
                //b2.SetPxPyPzE(jetInfo.Px[sel_b_jets[1]],jetInfo.Py[sel_b_jets[1]],jetInfo.Pz[sel_b_jets[1]],jetInfo.Energy[sel_b_jets[1]]);
				//lepton.SetPxPyPzE(leptonInfo.Px[idx_Selected_Lep],leptonInfo.Py[idx_Selected_Lep],leptonInfo.Pz[idx_Selected_Lep],leptonInfo.Energy[idx_Selected_Lep]);
					
				had_t_mass = ( b1 + j1 + j2 ).M();
				
				//Apply lep_t_mass Cut here	

				//lep_t_mass = (lepton + b2).M();
					
				//if( !(lep_t_mass < 150.))
				//{	continue;	}
				
				if( Set_name == "TT" ){
					h_btag_w->Fill( btag_weight );
				}

				//cout << endl << "had_t_mass : " << had_t_mass ;

				if(channel == "mu")
				{	h_mu.at(k)->Fill(had_t_mass,weight);	}
				if(channel == "el")
				{	h_el.at(k)->Fill(had_t_mass,weight);	}

			}	//end of entry for-loop	
			cout << endl << "At Set " << Set_name << " " << r+1 << "'s end, (h_mu[0], h_TT_mu) = " << h_mu[0] << ", " << h_TT_mu << endl;
			cout << endl << "The end of the file-sets " << Set_name << " " << r+1 << " " << endl;
			
		}	//end of r for-loop
	}		//end of k for-loop

	//*****Drawing Plotting or Outputting files*****//

	//normalize		
	
	//Save these hists to be a root file
	
	string time = "";
	time = get_time_str( minute );
	string new_file_name = "CR190924_hadtop_" + time + ".root";

	TFile* f_out = new TFile( new_file_name.c_str() , "recreate" );

	h_TT_mu->Write();
	h_DY_mu->Write();
	h_WJets_mu->Write();
	h_VV_mu->Write();
	h_ST_mu->Write();
	h_QCD_mu->Write();

	h_TT_el->Write();
	h_DY_el->Write();
	h_WJets_el->Write();
	h_VV_el->Write();
	h_ST_el->Write();
	h_QCD_el->Write();
	
	h_Data_mu->Write();
	h_Data_el->Write();

	h_btag_w->Write();

	f_out->Close();
	delete f_out;

	//*****make space free*****//
	
	delete h_TT_mu;
	delete h_DY_mu;
	delete h_WJets_mu;
	delete h_VV_mu;
	delete h_ST_mu;
	delete h_QCD_mu;
	
	delete h_TT_el;
	delete h_DY_el;
	delete h_WJets_el;
	delete h_VV_el;
	delete h_ST_el;
	delete h_QCD_el;
	
	delete h_Data_mu;
	delete h_Data_el;

	delete h_btag_w;

	delete f7;
	delete f6;
	delete f5;
	delete f4;
	delete f3;
	delete f2;
	delete f1;
}

