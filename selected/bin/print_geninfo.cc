
/*******************************************************
 *
 *	File Name : print_geninfo.cc 
 *	Description : 
 *	
 *
 * ****************************************************/

#include "TopCPViolation/selected/interface/BranchInfo.h"
#include "TopCPViolation/selected/interface/Comp_DataMC.h"

using namespace std;

int main()
{
int count = 0;
cout << "test " << count++ << endl;
	TH1F* h_mu = new TH1F("h_mu","mass",100,0.,300.);
	TH1F* h_el = new TH1F("h_el","mass",100,0.,300.);

	TChain* root = new TChain( "root" );

	root->Add( "/wk_cms2/cychuang/2016legacy_full_sel_file/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8/full_sel_bpk_ntuple_1.root" );

	LeptonInfo leps;
	leps.Register( root );
	JetInfo jets;
	jets.Register( root );
	GenInfo gen;
	gen.Register( root );
	
	int t_entries = root->GetEntries();

	cout << "total entries : " << t_entries << endl;

	for(int entry=0;entry<t_entries;++entry)
	{
		root->GetEntry(entry);
			
		//initialize some storage of selected-particles' indices
		int idx_Selected_Mu = -1;
		int idx_Selected_El = -1;
		
		vector<int> sel_jets;		sel_jets.assign(128,-1);
		vector<int> sel_b_jets;		sel_b_jets.assign(2,-1);

		double had_t_mass = -1.;
		//double chi_square_value = -1.;
			
		string channel = "";

		//Do the lepton Selected first here
				
		bool pass_sel_mu = Pass_SR_Selected_Muon( leps, idx_Selected_Mu );
		bool pass_sel_el = Pass_SR_Selected_Electron( leps, idx_Selected_El );
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
		
        bool pass_sel_jet = Pass_SR_Selected_Jets_Case( jets, sel_jets );
		if( !pass_sel_jet )
		{	continue;	}

		bool pass_delR = Pass_delR_Jet_Lep( jets, leps, sel_jets, idx_Selected_Lep );
			
		if( !pass_delR )
		{	continue;	}

		bool pass_sel_b = Pass_SR_bjets( jets, sel_jets, sel_b_jets );
			
		if( !pass_sel_b )	
		{	continue;	}

		for(int i=0;i<gen.Size;i++)
		{
			cout << "[Entry:" << entry << "][The " << i << "-th particle] ";
			cout << "PdgID:" << gen.PdgID[i] << ",";
			cout << "nMo:" << gen.nMo[i] << ",";
			cout << "Mo1:" << gen.Mo1[i] << ",";
			cout << "Mo2:" << gen.Mo2[i] << ",";
			cout << "Mo1PdgID:" << gen.Mo1PdgID[i] << ",";
			cout << "Mo2PdgID:" << gen.Mo2PdgID[i];

			cout << endl;
			//print out the jet information

			cout << endl;
		}
		
		cout << "Sel-Jets : " << endl << "{ ";
		for(int i=0;i<(int)sel_jets.size();i++)
		{
			cout << " " << sel_jets.at(i)+1 << "," ;
		}	
		cout << " }" << endl;

		cout << "Sel-b-jets : " << endl << "{ ";
		for(int i=0;i<(int)sel_b_jets.size();i++)
		{
			cout << " " << sel_b_jets.at(i)+1 << "," ;
		}	
		cout << " }" << endl;

		cout << "Sel_lep : " <<  idx_Selected_Lep << endl;


		for(int j=0;j<jets.Size;j++ )
		{
			cout << "jets-" << j+1 << " GenPdgID :" << jets.GenPdgID[j] << endl;
			int gen_jet_idx = GenJetIdx( j, jets, gen );
			cout << "       " << " corresponding gen idx: " << gen_jet_idx << endl;
		}

		cout << endl;
		for(int j=0;j<leps.Size;j++ )
		{
			cout << "leps-" << j+1 << " GenPdgID :" << leps.GenPdgID[j] << endl;
			int gen_lep_idx = GenLepIdx( j, leps, gen );
			cout << "       " << " corresponding gen idx: " << gen_lep_idx << endl;
		}

		cout << "End of this event " << endl;

	}
}
