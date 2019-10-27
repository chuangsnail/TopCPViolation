#include "TopCPViolation/selected/interface/BranchInfo.h"
#include "TopCPViolation/selected/interface/Comp_DataMC.h"

using namespace std;

int main()
{
int count = 0;
cout << "test " << count++ << endl;
	TH1F* h_mu = new TH1F("h_mu","mass",100,0.,300.);
	TH1F* h_el = new TH1F("h_el","mass",100,0.,300.);

	TChain* root = new TChain( "bprimeKit/root" );

	root->Add( "/wk_cms2/cychuang/CMSSW_8_0_19/src/TopCPViolation/bpk_ntuple_1.root" );
	root->Add( "/wk_cms2/cychuang/CMSSW_8_0_19/src/TopCPViolation/bpk_ntuple_2.root" );
	root->Add( "/wk_cms2/cychuang/CMSSW_8_0_19/src/TopCPViolation/bpk_ntuple_3.root" );
	root->Add( "/wk_cms2/cychuang/CMSSW_8_0_19/src/TopCPViolation/bpk_ntuple_4.root" );
	root->Add( "/wk_cms2/cychuang/CMSSW_8_0_19/src/TopCPViolation/bpk_ntuple_5.root" );

	LeptonInfo leps;
	leps.Register( root );
	JetInfo jets;
	jets.Register( root );
	
	int count_lep = 0;
	int count_jet = 0;
	int count_delR = 0;
	int count_b = 0;
	int count_reco = 0;

	int t_entries = root->GetEntries();

	cout << "total entries : " << t_entries << endl;

	for(int i=0;i<t_entries;++i)
	{
		bool info = false;
		if( i%50000 == 0 )
		{	info = true;	}

		root->GetEntry(i);
			
		if( info )
		{
			cout << "now at entry : " << (double)i/(double)t_entries << endl;	
		}
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
		
		count_lep++;
		if( info )
		{	cout << "After lepton seletion" << endl;	}

        bool pass_sel_jet = Pass_SR_Selected_Jets_Case( jets, sel_jets );
		if( !pass_sel_jet )
		{	continue;	}

		count_jet++;
		
		bool pass_delR = Pass_delR_Jet_Lep( jets, leps, sel_jets, idx_Selected_Lep );
			
		if( !pass_delR )
		{	continue;	}

		count_delR++;
		if( info )
		{	cout << "After jet selection" << endl;	}
			
		bool pass_sel_b = Pass_SR_bjets( jets, sel_jets, sel_b_jets );
			
		if( !pass_sel_b )	
		{	continue;	}

		count_b++;
			
		double chi_square_value = Chi2_Sorting( jets, sel_jets, sel_b_jets );
		

		TLorentzVector b1, j1, j2, lepton, b2;
        j1.SetPxPyPzE(jets.Px[sel_jets[0]],jets.Py[sel_jets[0]],jets.Pz[sel_jets[0]],jets.Energy[sel_jets[0]]);
        j2.SetPxPyPzE(jets.Px[sel_jets[1]],jets.Py[sel_jets[1]],jets.Pz[sel_jets[1]],jets.Energy[sel_jets[1]]);
        b1.SetPxPyPzE(jets.Px[sel_b_jets[0]],jets.Py[sel_b_jets[0]],jets.Pz[sel_b_jets[0]],jets.Energy[sel_b_jets[0]]);
  		//b2.SetPxPyPzE(jets.Px[sel_b_jets[1]],jets.Py[sel_b_jets[1]],jets.Pz[sel_b_jets[1]],jets.Energy[sel_b_jets[1]]);
		//lepton.SetPxPyPzE(leps.Px[idx_Selected_Lep],leps.Py[idx_Selected_Lep],leps.Pz[idx_Selected_Lep],leps.Energy[idx_Selected_Lep]);
				
		had_t_mass = ( b1 + j1 + j2 ).M();

		count_reco++;

		if(channel == "mu")
		{	h_mu->Fill(had_t_mass);	}
		if(channel == "el")
		{	h_el->Fill(had_t_mass);	}

		cout << "had_t_mass : " << had_t_mass << endl;
	}

	cout << "count_lep : " << count_lep << endl;
	cout << "count_jet : " << count_jet << endl;
	cout << "count_delR : " << count_delR << endl;
	cout << "count_b : " << count_b << endl;
	cout << "count_reco : " << count_reco << endl;

	TCanvas* c = new TCanvas();
	h_mu->Draw();

	c->Print("test_sel.pdf");
}
