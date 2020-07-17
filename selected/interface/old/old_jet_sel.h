#ifndef OLD_JET_SEL_H
#define OLD_JET_SEL_H

#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <vector>
#include "TLorentzVector.h"		//for deltaR
#include "TVector2.h"
#include "TVector3.h"

using namespace std;

bool Pass_Selected_Jet(JetInfo& jet,int* jets_idx,int* jets_no)
{
	int pass_no = 0;
	*jets_no = jet.Size;
	for(int i=0;i<jet.Size;i++)
	{
		if(jet.Pt[i] > 30. \
		&& fabs(jet.Eta[i]) < 2.4 \
		//Jet Loose ID
		&& jet.NHF[i] <= 0.99 \
		&& jet.NEF[i] <= 0.99 \
		&& jet.NConstituents[i] > 1 \
		&& jet.CHF[i] > 0. \
		&& jet.NCH[i] > 0 \
		&& jet.CEF[i] <= 0.99 \
		)
		{
			jets_idx[pass_no] = i; pass_no++;
		}
	}
	if( pass_no >= 4 )
	{
		*jets_no = pass_no;	
		return true;
	}
	else
	{ return false; }
}
//to check the Jet is distingushed with lepton we see
bool Pass_delta_R(LeptonInfo& lep,int lep_idx,JetInfo& jet,int* jets_idx,int* jets_no)
//ref:TLorentzVector,TVector3,TVector2
//(lepInfo,jetInfo,input jets' indeces,input jets' no.,output jets' indices,output jets' no.)
{	
	int pass_no = 0;	//pass_no = jets_no_out
	vector<int> jets_idx_out;	//temporary vector to store selected jet's index
	jets_idx_out.assign(*jets_no,0);

	vector<int> jets_idx_not_pass;
	
	for(int i=0;i<*jets_no;i++)	//now jets_no is the jets_no before deltaR selection
	{
		int k = jets_idx[i];
		double delta_eta = lep.Eta[lep_idx] - jet.Eta[k];
		double delta_phi = TVector2::Phi_mpi_pi(lep.Phi[lep_idx] - jet.Phi[k]);		//TLorentzVector.h
		double delta_R = sqrt( pow(delta_eta,2) + pow(delta_phi,2) );
		if(delta_R > 0.5)
		{
			jets_idx_out[pass_no] = k; pass_no++;
		}
		else
		{
			jets_idx_not_pass.push_back(k);
		}
	}
	if(pass_no >= 4)
	{
			int original_jets_no = *jets_no;
			*jets_no = pass_no;
  			for(int i=0;i<*jets_no;i++)	//now jets_no is the jets_no after deltaR selection
			{
				jets_idx[i] = jets_idx_out[i];		//assign the temporary vector to origin input array of jets_idx 
			}
			for(int i=*jets_no;i<original_jets_no;i++)
			{
				jets_idx[i] = jets_idx_not_pass[ i-*jets_no ];
			}
		return true;
	}
	else
	{ return false; }
}


//if idx.Size() >= 4 ,then we call this function to select out which event have 2 b-Jets
bool Pass_Selected_b_Jet(JetInfo& jet,int* jets_idx,int* jets_no,int* b_idx)
{	
	int pass_no = 0; //b-Jets no.
	
	vector<int> jets_not_b_idx;
	jets_not_b_idx.assign(*jets_no,0);	
	
	for(int i=0;i<*jets_no;i++)
	{
		int k = jets_idx[i];
		if(jet.pfCombinedInclusiveSecondaryVertexV2BJetTags[k] > 0.8484)
		{
			b_idx[pass_no] = k; pass_no++;
			if(pass_no > 2)
			{
				return false;
			}
		}
		else	//if it's not b,put it in not_b_idx
		{
			jets_not_b_idx[i-pass_no] = k;
		}	
	}

	if( pass_no == 2 )
	{
		//output jets do not include b-jets
		*jets_no -= 2;
		for(int i=0;i<*jets_no;i++)
		{
			jets_idx[i] = jets_not_b_idx[i];
		}
		for(int i=*jets_no;i<*jets_no+2;i++)
		{
			jets_idx[i] = -1;
		}
		return true;
	}
	else
	{ return false; }
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


//use chi-square sorting to choose good jj choice to be W and good jjb choice to be top  
double Chi_Square_Sorting(JetInfo& jet,int* jets_idx,int* jets_no,int* b_idx,int b_jets_no)
//now the jets_idx is jets without b jet
{
	if(b_jets_no != 2)
	{ 
		printf("\nThere are not 2 b-jets in this event!");
		return -1;
	}
	else
	{
		int no_of_choice = 0;
		no_of_choice = 2 * factorial(*jets_no) / 2 / factorial(*jets_no - 2);	
		double* chi2_sorting = new double[no_of_choice];
		//we can use vector and .pushback() to be more convenient
		int chi2_idx = 0;
		//int selected_j1_idx = 0, selected_j2_idx = 1, selected_b_idx = 0;	//index in the array of indices
		//double selected_w_mass = 0., selected_t_mass = 0.;
		
		TLorentzVector init_smallest_t , init_smallest_w;
		init_smallest_t.SetPxPyPzE( jet.Px[jets_idx[0]] + jet.Px[jets_idx[1]] + jet.Px[b_idx[0]] ,\
		jet.Py[jets_idx[0]] + jet.Py[jets_idx[1]] + jet.Py[b_idx[0]] ,\
		jet.Pz[jets_idx[0]] + jet.Pz[jets_idx[1]] + jet.Pz[b_idx[0]] ,\
		jet.Energy[jets_idx[0]] + jet.Energy[jets_idx[1]] + jet.Energy[b_idx[0]]\
		);
		
		init_smallest_w.SetPxPyPzE( jet.Px[jets_idx[0]] + jet.Px[jets_idx[1]] ,\
		jet.Py[jets_idx[0]] + jet.Py[jets_idx[1]] ,\
		jet.Pz[jets_idx[0]] + jet.Pz[jets_idx[1]] ,\
		jet.Energy[jets_idx[0]] + jet.Energy[jets_idx[1]]\
		);

		double smallest = pow( (init_smallest_t.M() - 172.5)/16.3 , 2 ) + pow( (init_smallest_w.M() - 82.9)/9.5 , 2 ) ;
		double t_final_mass = init_smallest_t.M();		

		//i is b_idx's index , j & k are jets_idx's index
		for(int i=0;i<b_jets_no;i++)
		{
			for(int j=0;j<*jets_no;j++)
			{
				for(int k=j+1;k<*jets_no;k++)
				{
					TLorentzVector j1 , j2 , b ;
					j1.SetPxPyPzE(jet.Px[jets_idx[j]],jet.Py[jets_idx[j]],jet.Pz[jets_idx[j]],jet.Energy[jets_idx[j]]);
					j2.SetPxPyPzE(jet.Px[jets_idx[k]],jet.Py[jets_idx[k]],jet.Pz[jets_idx[k]],jet.Energy[jets_idx[k]]);
					b.SetPxPyPzE(jet.Px[b_idx[i]],jet.Py[b_idx[i]],jet.Pz[b_idx[i]],jet.Energy[b_idx[i]]);	
					double w_mass = (j1 + j2).M() ;
					double t_mass = (j1 + j2 + b).M() ;
					chi2_sorting[chi2_idx] = pow( (t_mass - 172.5)/16.3 , 2 ) + pow( (w_mass - 82.9)/9.5 , 2 );
					
					//choose the smallest chi2
					if(chi2_sorting[chi2_idx] < smallest)
					{
						smallest = chi2_sorting[chi2_idx];
						//selected_j1_idx = j; selected_j2_idx = k; selected_b_idx = i;
						t_final_mass = t_mass;
					}	
					
					chi2_idx++;
				}
			}
		}
		
		//int j1_idx_evt = jets_idx[selected_j1_idx];
		//int j2_idx_evt = jets_idx[selected_j2_idx];
		//int b_idx_evt = b_idx[selected_b_idx];
		return t_final_mass;
	}
	
}
//I think that it would be better to use while loop?!?

//input jets_idx


//use chi-square sorting to choose good jj choice to be W and good jjb choice to be top  
double Chi2_Sorting(JetInfo& jet,int* jets_idx,int* jets_no,int* b_idx,int b_jets_no)
//now the jets_idx is jets without b-tagged jet
{
	if(b_jets_no != 2)
	{ 
		printf("\nThere are not 2 b-jets in this event!");
		return -1.;
	}
	else
	{
		int no_of_choice = 0;
		no_of_choice = 2 * factorial(*jets_no) / 2 / factorial(*jets_no - 2);	
		double chi2_sorting[no_of_choice];
		for(int i=0;i<no_of_choice;i++)
		{
			chi2_sorting[i] = -1.;	
		}
		
		//we can use vector and .pushback() to be more convenient
		int chi2_idx = 0;
		int selected_j1_idx = 0, selected_j2_idx = 1, selected_b_idx = 0;	//**index in the array of indices**//
		//double selected_w_mass = 0., selected_t_mass = 0.;
		
		TLorentzVector init_smallest_t , init_smallest_w;
		init_smallest_t.SetPxPyPzE( jet.Px[jets_idx[0]] + jet.Px[jets_idx[1]] + jet.Px[b_idx[0]] ,\
		jet.Py[jets_idx[0]] + jet.Py[jets_idx[1]] + jet.Py[b_idx[0]] ,\
		jet.Pz[jets_idx[0]] + jet.Pz[jets_idx[1]] + jet.Pz[b_idx[0]] ,\
		jet.Energy[jets_idx[0]] + jet.Energy[jets_idx[1]] + jet.Energy[b_idx[0]]\
		);
		
		init_smallest_w.SetPxPyPzE( jet.Px[jets_idx[0]] + jet.Px[jets_idx[1]] ,\
		jet.Py[jets_idx[0]] + jet.Py[jets_idx[1]] ,\
		jet.Pz[jets_idx[0]] + jet.Pz[jets_idx[1]] ,\
		jet.Energy[jets_idx[0]] + jet.Energy[jets_idx[1]]\
		);

		double smallest = pow( (init_smallest_t.M() - 172.5)/16.3 , 2 ) + pow( (init_smallest_w.M() - 82.9)/9.5 , 2 ) ;
		//double t_final_mass = init_smallest_t.M();		

		//i is b_idx's index , j & k are jets_idx's index
		for(int i=0;i<b_jets_no;i++)
		{
			for(int j=0;j<*jets_no;j++)
			{
				for(int k=j+1;k<*jets_no;k++)
				{
					TLorentzVector j1 , j2 , b ;
					j1.SetPxPyPzE(jet.Px[jets_idx[j]],jet.Py[jets_idx[j]],jet.Pz[jets_idx[j]],jet.Energy[jets_idx[j]]);
					j2.SetPxPyPzE(jet.Px[jets_idx[k]],jet.Py[jets_idx[k]],jet.Pz[jets_idx[k]],jet.Energy[jets_idx[k]]);
					b.SetPxPyPzE(jet.Px[b_idx[i]],jet.Py[b_idx[i]],jet.Pz[b_idx[i]],jet.Energy[b_idx[i]]);	
					double w_mass = (j1 + j2).M() ;
					double t_mass = (j1 + j2 + b).M() ;
					chi2_sorting[chi2_idx] = pow( (t_mass - 172.5)/16.3 , 2 ) + pow( (w_mass - 82.9)/9.5 , 2 );
					
					//choose the smallest chi2
					if(chi2_sorting[chi2_idx] < smallest)
					{
						smallest = chi2_sorting[chi2_idx];
						selected_j1_idx = j;	selected_j2_idx = k;	selected_b_idx = i;
						//t_final_mass = t_mass;
					}	
					
					chi2_idx++;
				}
			}
		}
		
		int j1_idx_evt = jets_idx[selected_j1_idx];
		int j2_idx_evt = jets_idx[selected_j2_idx];
		int b_idx_evt = b_idx[selected_b_idx];
		
		//make the jets_idx[0] and jets_idx[1] to be two selected jets' indices
		//make the b_idx[0] to be the selected b-jet index

		jets_idx[0] = j1_idx_evt;
		jets_idx[1] = j2_idx_evt;
		
		if(b_idx_evt != b_idx[0])	//b_idx_evt == b_idx[1]
		{
			b_idx[1] = b_idx[0];
			b_idx[0] = b_idx_evt;
		}

		return smallest;
	}
}
	//'evt' means the index which is in an event
	
#endif
