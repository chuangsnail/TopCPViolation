#include "TopCPViolation/selected/interface/jet_sel.h"

//the 3 slash comment means the old CSV version
//the new one is deepCSV version


using namespace std;

bool Pass_Loose_Jet( JetInfo& jet, const int& idx )
{
	//Jet Loose ID
	if( jet.NHF[idx] <= 0.99 \
	&& jet.NEF[idx] <= 0.99 \
	&& jet.NConstituents[idx] > 1 \
	&& jet.CHF[idx] > 0. \
	&& jet.NCH[idx] > 0 \
	&& jet.CEF[idx] <= 0.99 \
	)
	{	return true;	}

	return false;
}

bool Pass_Selected_Jet( JetInfo& jet, const int& idx)
{
	if(jet.Pt[idx] > 30. \
	&& fabs(jet.Eta[idx]) < 2.4 \
	&& Pass_Loose_Jet( jet, idx ) \
	)
	{
		return true;
	}
	return false;
}


bool Pass_SR_Selected_Jets_Case( JetInfo& jets, vector<int>& sel_jets_idx )
{
	sel_jets_idx.clear();
	int pass_no = 0;
	//jets_idx.assign(jets.Size,-1);
	for(int i=0;i<(int)jets.Size;i++)
	{
		if( Pass_Selected_Jet( jets, i ) )
		{
			//sel_jets_idx.at( pass_no ) = i;
			sel_jets_idx.push_back(i);
			pass_no++;
		}
	}
	if( pass_no >= 4 )
	{
		return true;
	}
	return false;
}

//For generally 2 direction of any 2 things 
double delta_R( const double& eta1, const double& eta2, const double& phi1,const double& phi2 )
{
	double delta_eta = eta1 - eta2;
	double delta_phi = TVector2::Phi_mpi_pi(phi1 - phi2);		//TLorentzVector.h
	double del_R = sqrt( pow(delta_eta,2) + pow(delta_phi,2) );
	return del_R;
}

bool Pass_delR_Jet_Lep( JetInfo& jets, LeptonInfo& lep, vector<int>& sel_jets_idx, const int& sel_lep_idx )
{
	int pass_no = 0;
	vector<int> tmp_sel_jets_idx;
	//tmp_sel_jets_idx.assign((int)sel_jets_idx.size(),-1);
	for(int i=0;i<(int)sel_jets_idx.size();i++)
	{
		int sel_jet_idx = sel_jets_idx[i];
		if( delta_R( jets.Eta[sel_jet_idx], lep.Eta[sel_lep_idx], jets.Phi[sel_jet_idx], lep.Phi[sel_lep_idx] ) > 0.5 )
		{
			tmp_sel_jets_idx.push_back( sel_jet_idx );
			pass_no++;
		}
	}
	if( pass_no >= 4 )
	{
        sel_jets_idx.resize( pass_no );
		for(int i=0;i<(int)sel_jets_idx.size();i++)
		{
            if(i<(int)tmp_sel_jets_idx.size())
            {   sel_jets_idx.at(i) = tmp_sel_jets_idx.at(i);    }
            else
            {   sel_jets_idx.at(i) = -1;    }
		} 
		return true;
	}
	return false;
}

double CSV_value( JetInfo& jets , const int& idx )
{   return jets.pfCombinedInclusiveSecondaryVertexV2BJetTags[idx];  }

double deepCSV_value( JetInfo& jets, const int& idx )
{	return jets.pfDeepCSVJetTags_probb[idx] + jets.pfDeepCSVJetTags_probbb[idx];	}

bool is_CSV_Medium_bjet( JetInfo& jets, const int& idx )
{
	if(jets.pfCombinedInclusiveSecondaryVertexV2BJetTags[idx] > 0.8484)
	{   return true;    }
	return false;
}

bool is_CSV_Loose_bjet( JetInfo& jets, const int& idx )
{
	
	if(jets.pfCombinedInclusiveSecondaryVertexV2BJetTags[idx] > 0.5426)
	{   return true;    }
	return false;
}

//The btag for 2016 legacy data is from here
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation2016Legacy

bool is_deepCSV_Tight_bjet( JetInfo& jets, const int& idx )
{
	if( deepCSV_value( jets, idx ) > 0.8953 )
	{	return true;	}
	return false;
}

bool is_deepCSV_Medium_bjet( JetInfo& jets, const int& idx )
{
	if( deepCSV_value( jets, idx ) > 0.6321 )
	{	return true;	}
	return false;
}

bool is_deepCSV_Loose_bjet( JetInfo& jets, const int& idx )
{
	if( deepCSV_value( jets, idx ) > 0.2217 )
	{	return true;	}
	return false;
}

bool Pass_SR_bjets( JetInfo& jets, vector<int>& sel_jets_idx, vector<int>& b_jets_idx)
{
    int pass_b_no = 0;
	b_jets_idx.clear();
    for(int i=0;i<(int)sel_jets_idx.size();i++)
    {
        //int k = sel_jets_idx.at(i);
        ///if( is_CSV_Medium_bjet( jets, sel_jets_idx.at(i) ) )
        if( is_deepCSV_Medium_bjet( jets, sel_jets_idx.at(i) ) )
        {
			b_jets_idx.push_back( sel_jets_idx.at(i) );
			//b_jets_idx.at(pass_b_no) = sel_jets_idx.at(i);
			pass_b_no++;
        }
    }
	
    if( pass_b_no == 2 )
    {
		//b_jets_idx.erase( b_jets_idx.begin()+2,b_jets_idx.end() );

        vector<int> tmp_sel_jets_idx;
        
        for(int i=0;i<(int)sel_jets_idx.size();i++)
        {
            if( sel_jets_idx.at(i) == b_jets_idx.at(0) || sel_jets_idx.at(i) == b_jets_idx.at(1) )
            {   continue;   }
            tmp_sel_jets_idx.push_back( sel_jets_idx.at(i) );
        }
        
        sel_jets_idx.resize( (int)tmp_sel_jets_idx.size() );
        for(int i=0;i<(int)sel_jets_idx.size();i++)
        {
            sel_jets_idx.at(i) = tmp_sel_jets_idx.at(i);
        }
        return true;
    }
    return false;
    
}

void Bigger_Order( double& x1, double& x2 )
{
	if( x2 > x1 )
	{
		double tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
}

int CSV_to_idx( JetInfo& jets, double csv_value )
{
	for(int i=0;i<(int)jets.Size;i++)
	{
		if( CSV_value( jets, i ) == csv_value )
		{
			return i;
		}
	}
	return -1;
}

int deepCSV_to_idx( JetInfo& jets, double& csv_value )
{
	for(int i=0;i<(int)jets.Size;i++)
	{
		if( deepCSV_value( jets, i ) == csv_value )
		{
			return i;
		}
	}
	return -1;
}

//b_idx here means that the 2 highest CSV value jets in the pass case(with no selected b-jets in CR)
bool Pass_CR_bjets( JetInfo& jets, vector<int>& sel_jets_idx, vector<int>& b_jets_idx)
{
    int pass_b_no = 0;
    for(int i=0;i<(int)sel_jets_idx.size();i++)
    {
        int k = sel_jets_idx.at(i);
        ///if( is_CSV_Loose_bjet( jets, k ) )
        if( is_deepCSV_Loose_bjet( jets, k ) )
        {
            pass_b_no++;
            return false;
        }
    }
    if( pass_b_no == 0 )
    {
        //To order them with the CSV value
        ///double MAX_CSV_value = CSV_value( jets, sel_jets_idx.at(0) );
        ///double MAX2_CSV_value = CSV_value( jets, sel_jets_idx.at(1) );
        double MAX_CSV_value = deepCSV_value( jets, sel_jets_idx.at(0) );
        double MAX2_CSV_value = deepCSV_value( jets, sel_jets_idx.at(1) );
        Bigger_Order( MAX_CSV_value, MAX2_CSV_value );
        for(int i=2;i<(int)sel_jets_idx.size();i++)
        {
            ///double tmp_CSV_value = CSV_value( jets, sel_jets_idx[i] );
            double tmp_CSV_value = deepCSV_value( jets, sel_jets_idx[i] );
            Bigger_Order( MAX_CSV_value, tmp_CSV_value );
            Bigger_Order( MAX2_CSV_value, tmp_CSV_value );
        }
       
	   	b_jets_idx.clear();	
        ///b_jets_idx.push_back( CSV_to_idx( jets, MAX_CSV_value ) );
        ///b_jets_idx.push_back( CSV_to_idx( jets, MAX2_CSV_value ) );
        b_jets_idx.push_back( deepCSV_to_idx( jets, MAX_CSV_value ) );
        b_jets_idx.push_back( deepCSV_to_idx( jets, MAX2_CSV_value ) );

		//cout << "Highest CSV b-like 2 jets indices are : ( " << b_jets_idx.at(0) << " " << b_jets_idx.at(1) << " )" << endl;
		//cout << "Value : (" << CSV_value( jets, b_jets_idx.at(0) ) << " " << CSV_value( jets, b_jets_idx.at(1) ) << " )" << endl;
        
        vector<int> tmp_sel_jets_idx;

		//cout << "The rest jets idices are : ( ";
        int k = 0;
        for(int i=0;i<(int)sel_jets_idx.size();i++)
        {
            if( sel_jets_idx[i] == b_jets_idx.at(0) || sel_jets_idx[i] == b_jets_idx.at(1) )
            {    continue;    }
			//cout << sel_jets_idx.at(i) << " : " << CSV_value( jets, sel_jets_idx.at(i) ) << "  ";
            tmp_sel_jets_idx.push_back( sel_jets_idx.at(i) );
            k++;
        }

		//cout << ")" << endl;
        
        sel_jets_idx.resize( (int)tmp_sel_jets_idx.size() );
        for(int i=0;i<(int)tmp_sel_jets_idx.size();i++)
        {
            sel_jets_idx.at(i) = tmp_sel_jets_idx.at(i);
        }
        
        return true;
    }
    return false;
    
}


//Factorial (used in chi2 sorting)
double factorial(int x)
{
    if(x == 1)
    {return 1;}
    if(x == 0)
    {return 1;}
    
    return x * factorial(x-1);
}

//I think that it would be better to use while loop?!?

//input jets_idx

double chi_2_value( JetInfo& jet,const vector<int>& sel_jets_idx,const vector<int>& b_jets_idx, const int& idx_j1, const int& idx_j2, const int& idx_b  )
{
	TLorentzVector j1, j2, b;
	
    j1.SetPxPyPzE(jet.Px[sel_jets_idx[idx_j1]],jet.Py[sel_jets_idx[idx_j1]],jet.Pz[sel_jets_idx[idx_j1]],jet.Energy[sel_jets_idx[idx_j1]]);
    j2.SetPxPyPzE(jet.Px[sel_jets_idx[idx_j2]],jet.Py[sel_jets_idx[idx_j2]],jet.Pz[sel_jets_idx[idx_j2]],jet.Energy[sel_jets_idx[idx_j2]]);
    b.SetPxPyPzE(jet.Px[b_jets_idx[idx_b]],jet.Py[b_jets_idx[idx_b]],jet.Pz[b_jets_idx[idx_b]],jet.Energy[b_jets_idx[idx_b]]);
	
	double pdg_t_mass = 172.5;
	double pdg_w_mass = 82.9;
	double pdg_t_sigma = 16.3;
	double pdg_w_sigma = 9.5;

	double t_mass = ( j1 + j2 + b ).M();
	double w_mass = ( j1 + j2 ).M();

	double c2 = pow( (t_mass - pdg_t_mass)/pdg_t_sigma , 2 ) + pow( (w_mass - pdg_w_mass)/pdg_w_sigma , 2 );
	return c2;

}


//use chi-square sorting to choose good jj choice to be W and good jjb choice to be top
double Chi2_Sorting(JetInfo& jet,vector<int>& sel_jets_idx,vector<int>& b_jets_idx)
//now the jets_idx is jets without b-tagged jet
{
    if( (int)b_jets_idx.size() != 2 )
    {
        cout << endl << "-----FAULT----- There is no 2 b-jets in the b_jets_idx " << endl;
		return -1.;
    }
    
    int no_of_choice = 0;
    no_of_choice = 2 * factorial( (int)sel_jets_idx.size() ) / (int)b_jets_idx.size() / factorial( (int)sel_jets_idx.size() - (int)b_jets_idx.size() );
	double chi2_sorting[no_of_choice];
	for(int i=0;i<no_of_choice;i++)
	{
		chi2_sorting[i] = -1.;	
	}
	
    //we can use vector and .pushback() to be more convenient
    int chi2_idx = 0;
    int selected_j1_idx = 0, selected_j2_idx = 1, selected_b_idx = 0;    //**index in the array of indices**//
    //double selected_w_mass = 0., selected_t_mass = 0.;
        
    TLorentzVector init_smallest_t , init_smallest_w;
    init_smallest_t.SetPxPyPzE( jet.Px[sel_jets_idx[0]] + jet.Px[sel_jets_idx[1]] + jet.Px[b_jets_idx[0]] ,\
                                jet.Py[sel_jets_idx[0]] + jet.Py[sel_jets_idx[1]] + jet.Py[b_jets_idx[0]] ,\
                                jet.Pz[sel_jets_idx[0]] + jet.Pz[sel_jets_idx[1]] + jet.Pz[b_jets_idx[0]] ,\
                                jet.Energy[sel_jets_idx[0]] + jet.Energy[sel_jets_idx[1]] + jet.Energy[b_jets_idx[0]]\
                                );
        
    init_smallest_w.SetPxPyPzE( jet.Px[sel_jets_idx[0]] + jet.Px[sel_jets_idx[1]] ,\
                                jet.Py[sel_jets_idx[0]] + jet.Py[sel_jets_idx[1]] ,\
                                jet.Pz[sel_jets_idx[0]] + jet.Pz[sel_jets_idx[1]] ,\
                                jet.Energy[sel_jets_idx[0]] + jet.Energy[sel_jets_idx[1]]\
                                   );
        
    double smallest = pow( (init_smallest_t.M() - 172.5)/16.3 , 2 ) + pow( (init_smallest_w.M() - 82.9)/9.5 , 2 ) ;
    //double t_final_mass = init_smallest_t.M();
        
    //i is b_idx's index , j & k are jets_idx's index
    for(int i=0;i<(int)b_jets_idx.size();i++)
    {
        for(int j=0;j<(int)sel_jets_idx.size();j++)
        {
            for(int k=j+1;k<(int)sel_jets_idx.size();k++)
            {
                TLorentzVector j1 , j2 , b ;
                j1.SetPxPyPzE(jet.Px[sel_jets_idx[j]],jet.Py[sel_jets_idx[j]],jet.Pz[sel_jets_idx[j]],jet.Energy[sel_jets_idx[j]]);
                j2.SetPxPyPzE(jet.Px[sel_jets_idx[k]],jet.Py[sel_jets_idx[k]],jet.Pz[sel_jets_idx[k]],jet.Energy[sel_jets_idx[k]]);
                b.SetPxPyPzE(jet.Px[b_jets_idx[i]],jet.Py[b_jets_idx[i]],jet.Pz[b_jets_idx[i]],jet.Energy[b_jets_idx[i]]);
                double w_mass = (j1 + j2).M() ;
                double t_mass = (j1 + j2 + b).M() ;
                chi2_sorting[chi2_idx] = pow( (t_mass - 172.5)/16.3 , 2 ) + pow( (w_mass - 82.9)/9.5 , 2 );
                
                //choose the smallest chi2
                if(chi2_sorting[chi2_idx] < smallest)
                {
                    smallest = chi2_sorting[chi2_idx];
                    selected_j1_idx = j;    selected_j2_idx = k;    selected_b_idx = i;
                    //t_final_mass = t_mass;
                }
                
                chi2_idx++;
            }
        }
    }
        
    int j1_idx_evt = sel_jets_idx[selected_j1_idx];
    int j2_idx_evt = sel_jets_idx[selected_j2_idx];
    int b_idx_evt = b_jets_idx[selected_b_idx];
    
    //make the jets_idx[0] and jets_idx[1] to be two selected jets' indices
    //make the b_idx[0] to be the selected b-jet index
       
   /*	
    vector<int> tmp_sel_jets_idx;
    for(int i=0;i<(int)sel_jets_idx.size();i++)
    {
        if( sel_jets_idx.at(i) == j1_idx_evt || sel_jets_idx.at(i) == j2_idx_evt )
        {   continue;   }
        tmp_sel_jets_idx.push_back( sel_jets_idx.at(i) );
    }
    
    sel_jets_idx.at(0) = j1_idx_evt;        sel_jets_idx.at(1) = j2_idx_evt;
    
    for(int i=2;i<(int)sel_jets_idx.size();i++)
    {
        sel_jets_idx.at(i) = tmp_sel_jets_idx.at(i-2);
    }
	*/

	sel_jets_idx.resize(2);
	sel_jets_idx.at(0) = j1_idx_evt;
	sel_jets_idx.at(1) = j2_idx_evt;
        
    if(b_idx_evt != b_jets_idx[0])    //b_idx_evt == b_idx[1]
    {
        b_jets_idx[1] = b_jets_idx[0];
        b_jets_idx[0] = b_idx_evt;
    }
        
    return smallest;
}
//'evt' means the index which is in an event

/*
bool Leading_2_Pt_Jet( JetInfo& jets, vector<int> sel_jets_idx, vector<int> sel_b_jets )
{
	int bb = -1;

	if( jets.Pt[sel_b_jets.at(0)] >= jets.Pt[sel_b_jets.at(1)])
	{
		bb = 0;
	}
	else
	{
		bb = 1;
	}

	TLorentzVector j1 , j2 , b ;
	j1.SetPxPyPzE(jet.Px[sel_jets_idx[0]],jet.Py[sel_jets_idx[0]],jet.Pz[sel_jets_idx[0]],jet.Energy[sel_jets_idx[0]]);
	j2.SetPxPyPzE(jet.Px[sel_jets_idx[1]],jet.Py[sel_jets_idx[1]],jet.Pz[sel_jets_idx[1]],jet.Energy[sel_jets_idx[1]]);
	b.SetPxPyPzE(jet.Px[b_jets_idx[bb]],jet.Py[b_jets_idx[bb]],jet.Pz[b_jets_idx[bb]],jet.Energy[b_jets_idx[bb]]);	

}

*/

