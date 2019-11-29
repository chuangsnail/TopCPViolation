#ifndef MVAVAR_H
#define MVAVAR_H

#include "TopCPViolation/selected/interface/BranchInfo.h"
#include "TopCPViolation/selected/interface/jet_sel.h"
#include "TopCPViolation/selected/interface/kinematic_tool.h"
namespace mvatool
{
	string pre_file_name = "train20_10";
	//used in making the mva trained tree

	class StoreMVATree
	{
	public:

		StoreMVATree() {}
		StoreMVATree( TTree* co, TTree* inco ) : correct(co), incorrect(inco) {}
		~StoreMVATree() {}

		TTree* correct;
		TTree* incorrect;

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

		//member function~

		void InitBranch()
		{
			//***** for correct *****//

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

			//***** for incorrect *****//

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
		}

		void SetCorVar( JetInfo& jetInfo, LeptonInfo& leptonInfo, EvtInfo& evtInfo,\
			const vector<int>& sel_jets, const vector<int>& sel_b_jets, \
		   	int& idx_Selected_Lep, int& tmp_mva_hadb, int& tmp_mva_lepb, \
			int& tmp_mva_j1, int& tmp_mva_j2 )
		{
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
		}
		
		void SetIncorVar( JetInfo& jetInfo, LeptonInfo& leptonInfo, EvtInfo& evtInfo,\
			const vector<int>& sel_jets, const vector<int>& sel_b_jets, \
		   	int& idx_Selected_Lep, int& tmp_mva_hadb, int& tmp_mva_lepb, \
			int& tmp_mva_j1, int& tmp_mva_j2 )
		{
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
		}
	};
	
	//used in using this mva
	

	void AddVarName( vector<string>& inputVars )
	{
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
	}

	void InputVar( double* var, JetInfo& jetInfo, LeptonInfo& leptonInfo, \
			EvtInfo& evtInfo, const vector<int>& sel_jets, const vector<int>& sel_b_jets, \
		   	int& idx_Selected_Lep, int& tmp_mva_hadb, int& tmp_mva_lepb, \
			int& tmp_mva_j1, int& tmp_mva_j2 )
	{
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
	}
}


#endif
