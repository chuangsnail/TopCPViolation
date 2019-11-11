#include "TopCPViolation/selected/interface/SelMgr.h"
#include "TChain.h"
#include "TTree.h"


using namespace std;

int main()
{
	TChain* root = new TChain("bprimeKit/root");
	root->Add("/wk_cms2/youying/public/bprimekit_2016_Legacy/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8/bpk_ntuple_1.root");

	JetInfo jetInfo;
	jetInfo.Register( root, "JetInfo" );

	SelMgr sel( &jetInfo );

	int total_entries = root->GetEntries();
	for(int i=0;i<total_entries;i++)
	{
		root->GetEntry(i);
		cout << "In entry : " << i << " {" << endl;
		sel.test_sel_print();
		cout << "}" << endl;
	}
}

