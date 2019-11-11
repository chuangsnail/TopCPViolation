/**************************************************************************************
 *
 *	File Name : cor_mva_t13_BDTG.cc
 *	Description : for train_13
 *	Date : 191016~
 *	Author : Chen-Yu Chuang
 *
 ****************************************************************************************/
#include "TopCPViolation/selected/interface/Comp_DataMC.h"
#include "TopCPViolation/selected/interface/GenMgr.h"
#include "/wk_cms2/cychuang/CMSSW_9_4_2/src/wk_mva/train_13/weights/TMVAClassification_BDTG.class.C"

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
	get_lumi_weight( Weights_map, "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/selected/info./data_sets_full_sel.txt" );

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
	get_path( Data_Set_Path, "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/selected/info./data_sets_full_sel.txt" );

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
	int bins_No = 100;
	double hist_min = 0.;
	double hist_max = 500.;

	TH1F* h_max_mva_mu = new TH1F("h_max_mva_mu","",100,0.,1.);
	TH1F* h_max_mva_cor_mu = new TH1F("h_max_mva_cor_mu","",100,0.,1.);
	TH1F* h_max_mva_incor_mu = new TH1F("h_max_mva_incor_mu","",100,0.,1.);
	
	TH1F* h_min_chi2_mu = new TH1F("h_min_chi2_mu","",100,0.,500.);
	TH1F* h_min_chi2_cor_mu = new TH1F("h_min_chi2_cor_mu","",100,0.,500.);
	TH1F* h_min_chi2_incor_mu = new TH1F("h_min_chi2_incor_mu","",100,0.,500.);

	TH1F* h_max_mva_el = new TH1F("h_max_mva_el","",100,0.,1.);
	TH1F* h_max_mva_cor_el = new TH1F("h_max_mva_cor_el","",100,0.,1.);
	TH1F* h_max_mva_incor_el = new TH1F("h_max_mva_incor_el","",100,0.,1.);
	
	TH1F* h_min_chi2_el = new TH1F("h_min_chi2_el","",100,0.,500.);
	TH1F* h_min_chi2_cor_el = new TH1F("h_min_chi2_cor_el","",100,0.,500.);
	TH1F* h_min_chi2_incor_el = new TH1F("h_min_chi2_incor_el","",100,0.,500.);
	
	TH1F* h_max_mva_cor_t = new TH1F( "h_max_mva_cor_t","",100,0.,1. );
	TH1F* h_max_mva_incor_t = new TH1F( "h_max_mva_incor_t","",100,0.,1. );

	TH1F* h_min_chi2_cor_t = new TH1F( "h_min_chi2_cor_t","",100,0.,500. );
	TH1F* h_min_chi2_incor_t = new TH1F( "h_min_chi2_incor_t","",100,0.,500. );

	TH2D* h_mvamax_mass_mu = new TH2D("h_mvamax_mass_mu","",50,0.,500.,50,0.,1.);
	TH2D* h_mvamax_mass_el = new TH2D("h_mvamax_mass_el","",50,0.,500.,50,0.,1.);
	TH2D* h_mvamax_mass_t = new TH2D("h_mvamax_mass_t","",50,0.,500.,50,0.,1.);

	TH2D* h_chi2min_mass_mu = new TH2D("h_chi2min_mass_mu","",50,0.,500.,40,0.,200.);
	TH2D* h_chi2min_mass_el = new TH2D("h_chi2min_mass_el","",50,0.,500.,40,0.,200.);

	//for store to calculate Poisson error
	TH1F* h_chosen_mu = new TH1F( "h_chosen_mu","",2,0.,2. );
	TH1F* h_cor_mu = new TH1F( "h_cor_mu","",2,0.,2. );
		
	TH1F* h_chosen_el = new TH1F( "h_chosen_el","",2,0.,2. );
	TH1F* h_cor_el = new TH1F( "h_cor_el","",2,0.,2. );
	
	TH1F* h_chosen_t = new TH1F( "h_chosen","",2,0.,2. );
	TH1F* h_cor_t = new TH1F( "h_cor","",2,0.,2. );

	map<string, TH1F*> h_chosen, h_cor;
	h_chosen[ "mu" ] = h_chosen_mu;		h_chosen[ "el" ] = h_chosen_el;
	h_cor[ "mu" ] = h_cor_mu;			h_cor[ "el" ] = h_cor_el;
	
	
	map<string, TH1F*> h_max_mva, h_max_mva_cor, h_max_mva_incor;
	map<string, TH1F*> h_min_chi2, h_min_chi2_cor, h_min_chi2_incor;
	map<string, TH2D*> h_chi2min_mass, h_mvamax_mass;

	h_max_mva[ "mu" ] = h_max_mva_mu;					h_max_mva[ "el" ] = h_max_mva_el;
	h_max_mva_cor[ "mu" ] = h_max_mva_cor_mu;			h_max_mva_cor[ "el" ] = h_max_mva_cor_el;
	h_max_mva_incor[ "mu" ] = h_max_mva_incor_mu;		h_max_mva_incor[ "el" ] = h_max_mva_incor_el;

	h_min_chi2[ "mu" ] = h_min_chi2_mu;					h_min_chi2[ "el" ] = h_min_chi2_el;
	h_min_chi2_cor[ "mu" ] = h_min_chi2_cor_mu;			h_min_chi2_cor[ "el" ] = h_min_chi2_cor_el;
	h_min_chi2_incor[ "mu" ] = h_min_chi2_incor_mu;		h_min_chi2_incor[ "el" ] = h_min_chi2_incor_el;

	h_chi2min_mass[ "mu" ] = h_chi2min_mass_mu;			h_chi2min_mass[ "el" ] = h_chi2min_mass_el;
	h_mvamax_mass[ "mu" ] = h_mvamax_mass_mu;			h_mvamax_mass[ "el" ] = h_mvamax_mass_el;


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

	//prepare count object
	
	double t_weight_el = 0.;
	double t_weight_mu = 0.;
	double t_weight = 0.;

	double s_weight_el = 0.;
	double s_weight_mu = 0.;
	double s_weight = 0.;
	
	double mis_sel = 0.;
	
	double cor_mva = 0.;
	double cor_mva_mu = 0.;
	double cor_mva_el = 0.;

	double cor_chi2 = 0.;
	double cor_chi2_mu = 0.;
	double cor_chi2_el = 0.;

	//prepare mva object
	vector<string> inputVars;
	inputVars.push_back("top_mass");
	inputVars.push_back("w_mass");

	inputVars.push_back("j1j2_sumPt");
	inputVars.push_back("j1j2_absdelEta");
	inputVars.push_back("j1j2_delPhi");
	inputVars.push_back("lepblep_sumPt");
	inputVars.push_back("lepblep_absdelEta");
	inputVars.push_back("lepblep_delPhi");

	ReadBDTG MyMVA( inputVars );

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

			//prepare gen object
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

/*
				if(!is_data)
				{
					if(channel == "mu")
					{	weight *= lumi_cali_trg(35.811/35.9);	}
					if(channel == "el")
					{	weight *= lumi_cali_trg(35.615/35.9);	}
				}
*/

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

				//claculate channel's total weight
				if( channel == "mu" )
				{	t_weight_mu += weight;	}
				else if( channel == "el" )
				{	t_weight_el += weight;	}

				t_weight += weight;

				//do the mva algorithm to pick up the best combination
				
				//mva method

				int cor_b = -1, cor_j1 = -1, cor_j2 = -1;
        		bool is_good_trained_evt = genmgr.Find_Correct_HadronicTop( cor_b, cor_j1, cor_j2 );
				if( !is_good_trained_evt )
				{
					mis_sel += weight;
					//continue;
				}

				if( channel == "mu" )
				{	s_weight_mu += weight;	}
				else if( channel == "el" )
				{	s_weight_el += weight;	}

				s_weight += weight;

				h_chosen[channel]->Fill(1.,weight);
				h_chosen_t->Fill(1.,weight);


				int mva_hadb = -1, mva_j1 = -1, mva_j2 = -1;
				
				int var_num = 8;
				double* var = new double[var_num];
				double MAX_mva_value = -1.;				//ANN's value is 0~1
				
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

							TLorentzVector p_mva_j1, p_mva_j2, p_mva_hadb;

                			p_mva_j1.SetPxPyPzE(jetInfo.Px[tmp_mva_j1],jetInfo.Py[tmp_mva_j1],jetInfo.Pz[tmp_mva_j1],jetInfo.Energy[tmp_mva_j1]);
                			p_mva_j2.SetPxPyPzE(jetInfo.Px[tmp_mva_j2],jetInfo.Py[tmp_mva_j2],jetInfo.Pz[tmp_mva_j2],jetInfo.Energy[tmp_mva_j2]);
                			p_mva_hadb.SetPxPyPzE(jetInfo.Px[tmp_mva_hadb],jetInfo.Py[tmp_mva_hadb],jetInfo.Pz[tmp_mva_hadb],jetInfo.Energy[tmp_mva_hadb]);
				
							var[0] = ( p_mva_j1 + p_mva_j2 + p_mva_hadb ).M();
							var[1] = ( p_mva_j1 + p_mva_j2 ).M();

							var[2] = ( jetInfo.Pt[tmp_mva_j1] + jetInfo.Pt[tmp_mva_j2] );
							var[3] = fabs( jetInfo.Eta[tmp_mva_j1] - jetInfo.Eta[tmp_mva_j2] ) ;
							var[4] = TVector2::Phi_mpi_pi( jetInfo.Phi[tmp_mva_j1] - jetInfo.Phi[tmp_mva_j2] );
							
							var[5] = ( jetInfo.Pt[tmp_mva_lepb] + leptonInfo.Pt[idx_Selected_Lep] );
							var[6] = fabs( jetInfo.Eta[tmp_mva_lepb] - leptonInfo.Eta[idx_Selected_Lep] ) ;
							var[7] = TVector2::Phi_mpi_pi( jetInfo.Phi[tmp_mva_lepb] - leptonInfo.Phi[idx_Selected_Lep] );

							vector<double> inputValues;
							for(int in=0;in<var_num;in++ )
							{	inputValues.push_back( var[in] );	}
								
							double tmp_mva_value = MyMVA.GetMvaValue( inputValues );

							//cout << tmp_mva_value << " ";
							if( tmp_mva_value >= MAX_mva_value )
							{
								MAX_mva_value = tmp_mva_value;
								mva_hadb = tmp_mva_hadb;
								mva_j1 = tmp_mva_j1;
								mva_j2 = tmp_mva_j2;
								//cout << endl;
							}
						}
					}
				}

				if( mva_hadb == -1 || mva_j1 == -1 || mva_j2 == -1 )
				{
					cerr << "There is something wrong!" << endl;
					continue;
				}

				h_max_mva[channel]->Fill( MAX_mva_value, weight );
				
				delete [] var;

				//calculate the correctness of mva method

				if( (mva_hadb == cor_b && mva_j1 == cor_j1 && mva_j2 == cor_j2) || (mva_hadb == cor_b && mva_j1 == cor_j2 && mva_j2 == cor_j1) )
				{
					cor_mva += weight;

					if( channel == "mu" )
					{	cor_mva_mu += weight;	}
					else if( channel == "el" )
					{	cor_mva_el += weight;	}

					h_max_mva_cor[channel]->Fill( MAX_mva_value, weight );
					h_max_mva_cor_t->Fill( MAX_mva_value, weight );
					h_cor[channel]->Fill(1.,weight);
					h_cor_t->Fill(1.,weight);
				}
				else
				{
					h_max_mva_incor[channel]->Fill( MAX_mva_value, weight );
					h_max_mva_incor_t->Fill( MAX_mva_value, weight );
				}

				TLorentzVector p_mva_j1, p_mva_j2, p_mva_hadb;

                p_mva_j1.SetPxPyPzE(jetInfo.Px[mva_j1],jetInfo.Py[mva_j1],jetInfo.Pz[mva_j1],jetInfo.Energy[mva_j1]);
                p_mva_j2.SetPxPyPzE(jetInfo.Px[mva_j2],jetInfo.Py[mva_j2],jetInfo.Pz[mva_j2],jetInfo.Energy[mva_j2]);
                p_mva_hadb.SetPxPyPzE(jetInfo.Px[mva_hadb],jetInfo.Py[mva_hadb],jetInfo.Pz[mva_hadb],jetInfo.Energy[mva_hadb]);
				
				double mva_tmass = ( p_mva_j1 + p_mva_j2 + p_mva_hadb ).M();
				
				h_mvamax_mass[channel]->Fill( mva_tmass, MAX_mva_value, weight );
				h_mvamax_mass_t->Fill( mva_tmass, MAX_mva_value, weight );

				/*
				//chi2 method
				int chi2_hadb = -1, chi2_j1 = -1, chi2_j2 = -1;
				double chi_square_value = Chi2_Sorting( jetInfo, sel_jets, sel_b_jets );

				//calculate correctness of chi2 method
				
				chi2_hadb = sel_b_jets.at(0);
				chi2_j1 = sel_jets.at(0);
				chi2_j2 = sel_jets.at(1);
				
				h_min_chi2[channel]->Fill( chi_square_value, weight );

				if( (chi2_hadb == cor_b && chi2_j1 == cor_j1 && chi2_j2 == cor_j2) || (chi2_hadb == cor_b && chi2_j1 == cor_j2 && chi2_j2 == cor_j1) )
				{
					cor_chi2 += weight;
					if( channel == "mu" )
					{	cor_chi2_mu += weight;	}
					else if( channel == "el" )
					{	cor_chi2_el += weight;	}
					h_min_chi2_cor[channel]->Fill( chi_square_value, weight );
					h_min_chi2_cor_t->Fill( chi_square_value, weight );
				}
				else
				{
					h_min_chi2_incor[channel]->Fill( chi_square_value, weight );
					h_min_chi2_incor_t->Fill( chi_square_value, weight );
				}
                
				TLorentzVector p_chi2_j1, p_chi2_j2, p_chi2_hadb;

				p_chi2_j1.SetPxPyPzE(jetInfo.Px[chi2_j1],jetInfo.Py[chi2_j1],jetInfo.Pz[chi2_j1],jetInfo.Energy[chi2_j1]);
				p_chi2_j2.SetPxPyPzE(jetInfo.Px[chi2_j2],jetInfo.Py[chi2_j2],jetInfo.Pz[chi2_j2],jetInfo.Energy[chi2_j2]);
				p_chi2_hadb.SetPxPyPzE(jetInfo.Px[chi2_hadb],jetInfo.Py[chi2_hadb],jetInfo.Pz[chi2_hadb],jetInfo.Energy[chi2_hadb]);

				double chi2_tmass = ( p_chi2_j1 + p_chi2_j2 + p_chi2_hadb ).M();

				h_chi2min_mass[channel]->Fill( chi2_tmass, chi_square_value, weight );
				*/

			}	//end of entry for-loop	
		}	//end of r for-loop
	}		//end of k for-loop

	//*****Drawing Plotting or Outputting files*****//

	//normalize		
	
	//Save these hists to be a root file
	
	string time = "";
	time = get_time_str( minute );
	string new_file_name = "cor_mva_chi2_t13_BDTG_" + time + ".root";

	TFile* f_out = new TFile( new_file_name.c_str() , "recreate" );	

	h_max_mva_mu->Write();
	h_max_mva_cor_mu->Write();
	h_max_mva_incor_mu->Write();
	
	h_min_chi2_mu->Write();
	h_min_chi2_cor_mu->Write();
	h_min_chi2_incor_mu->Write();

	h_max_mva_el->Write();
	h_max_mva_cor_el->Write();
	h_max_mva_incor_el->Write();
	
	h_min_chi2_el->Write();
	h_min_chi2_cor_el->Write();
	h_min_chi2_incor_el->Write();
	
	h_max_mva_cor_t->Write();
	h_max_mva_incor_t->Write();

	h_min_chi2_cor_t->Write();
	h_min_chi2_incor_t->Write();
	
	h_mvamax_mass_mu->Write();
	h_mvamax_mass_el->Write();
	h_mvamax_mass_t->Write();

	h_chi2min_mass_mu->Write();
	h_chi2min_mass_el->Write();

	h_cor_t->Write();
	h_chosen_t->Write();
	h_cor_mu->Write();
	h_chosen_mu->Write();
	h_cor_el->Write();
	h_chosen_el->Write();
	

	f_out->Close();
	delete f_out;


	h_cor_t->Divide( h_chosen_t );
	h_cor_mu->Divide( h_chosen_mu );
	h_cor_el->Divide( h_chosen_el );


	cout << "mis_sel ratio: " << (double)mis_sel/t_weight  << endl;
	cout << "s_weight, s_weight_mu, s_weight_el = [ " << s_weight << ", " << s_weight_mu << ", " << s_weight_el << " ]" << endl << endl; 
   	
	cout << "cor_mva ratio : " << (double)cor_mva/s_weight << endl; 
	cout << "Error : " << (double) h_cor_t->GetBinError(2) << endl; 	
   	cout << "cor_mva_mu ratio : " << (double)cor_mva_mu/s_weight_mu << endl; 	
	cout << "Error : " << (double) h_cor_mu->GetBinError(2) << endl; 	
   	cout << "cor_mva_el ratio : " << (double)cor_mva_el/s_weight_el << endl; 	
	cout << "Error : " << (double) h_cor_el->GetBinError(2) << endl << endl; 	

	cout << "cor_chi2 ratio : " << (double)cor_chi2/s_weight << endl; 	
   	cout << "cor_chi2_mu ratio : " << (double)cor_chi2_mu/s_weight_mu << endl; 	
   	cout << "cor_chi2_el ratio : " << (double)cor_chi2_el/s_weight_el << endl; 	
	
	//*****make space free*****//
	
	delete f7;
	delete f6;
	delete f5;
	delete f4;
	delete f3;
	delete f2;
	delete f1;
}

