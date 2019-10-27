#ifndef OLD_LEP_SEL_H
#define OLD_LEP_SEL_H

#include <stdio.h>
#include <cmath>
#include <algorithm>	//for std::max
#include <vector>

using namespace std;

bool Pass_Selected_Muon(LeptonInfo& leptonInfo,int* index_ptr = NULL)
{
	int pass_no = 0;
	for(int i=0;i<leptonInfo.Size;i++)
	{
		//RelIsoR04
		double rel = leptonInfo.ChargedHadronIsoR04[i] + max(float(0.), \
		leptonInfo.NeutralHadronIsoR04[i] + leptonInfo.PhotonIsoR04[i] \
		- float(0.5) * leptonInfo.sumPUPtR04[i] );
		rel /= leptonInfo.Pt[i];
		
		if((leptonInfo.MuType[i] & 0x02)	//IsGlobalMuon
		&& leptonInfo.isPFMuon[i] \
		&& leptonInfo.MuGlobalNormalizedChi2[i] < 10. \
		&& leptonInfo.MuNMuonhits[i] > 0 \
		&& leptonInfo.MuNMatchedStations[i] > 1 \
		&& fabs(leptonInfo.MuInnerTrackDxy_PV[i]) < 0.2 \
		&& fabs(leptonInfo.MuInnerTrackDz[i]) < 0.5 \
		&& leptonInfo.MuNPixelLayers[i] > 0 \
		&& leptonInfo.MuNTrackLayersWMeasurement[i] > 5 \
		&& leptonInfo.Pt[i] > 30. \
		&& fabs(leptonInfo.Eta[i]) < 2.4 \
		&& rel < 0.15)
		{ pass_no++; *index_ptr = i;}
	}
	
	if(pass_no == 1) { return true; }
	else { return false;}
}

bool Pass_Veto_Muon(LeptonInfo& leptonInfo,int sel_mu_idx)
{
	int pass_no = 0;
	for(int i=0;i<leptonInfo.Size;i++)
	{	
		if(i == sel_mu_idx) { continue; }		
			
		double rel = leptonInfo.ChargedHadronIsoR04[i] + max(float(0.), \
		leptonInfo.NeutralHadronIsoR04[i] + leptonInfo.PhotonIsoR04[i] \
		- float(0.5) * leptonInfo.sumPUPtR04[i] );
		rel /= leptonInfo.Pt[i];
		
		if(leptonInfo.isPFMuon[i]
		&& ((leptonInfo.MuType[i] & 0x02) || (leptonInfo.MuType[i] & 0x04))  //IsGlobalMuon or IsTrackerMuon
		&& leptonInfo.Pt[i] > 15. \
		&& fabs(leptonInfo.Eta[i]) <2.4 \
		&& rel < 0.25)
		{ pass_no++; };
	}
	
	if(pass_no == 0) { return true; }
	else { return false; }	
}

bool Pass_Selected_Electron(LeptonInfo& leptonInfo,int* index_ptr = NULL)
{
	int pass_no = 0;
	
	for(int i=0;i<leptonInfo.Size;i++)
	{
		
		//Get Electron tight ID
		bool PassElTightID;
		//GsfEleEInverseMinusPInverseCut
		const float ecal_energy_inverse = 1.0 / leptonInfo.ElEcalE[i];
		const float eSCoverP = leptonInfo.ElEoverP[i];
		float GsfEleEInverseMinusPInverseCut = fabs(1.0 - eSCoverP)*ecal_energy_inverse;
	
		if( fabs(leptonInfo.Eta[i]) <= 1.479 )
		{
			PassElTightID = ( leptonInfo.ElsigmaIetaIeta[i] < 0.00998 \
			&& fabs(leptonInfo.EldeltaEta[i] - leptonInfo.Eta[i] + leptonInfo.caloEta[i]) < 0.00308 //abs(EldEtaInSeed)
			&& fabs(leptonInfo.EldeltaPhi[i]) < 0.0816 \
			&& leptonInfo.ElHadoverEm[i] < 0.0414 \
			&& leptonInfo.IsoRhoCorrR03[i]/leptonInfo.Pt[i] < 0.0588 \
			&& fabs(GsfEleEInverseMinusPInverseCut) < 0.0129 \
			&& leptonInfo.NumberOfExpectedInnerHits[i] <=1 \
			&& !(leptonInfo.ElhasConv[i]) );
		}
		else
		{	 
			PassElTightID = ( leptonInfo.ElsigmaIetaIeta[i] < 0.0292 \
			&& fabs(leptonInfo.EldeltaEta[i] - leptonInfo.Eta[i] + leptonInfo.caloEta[i]) < 0.00605 //abs(EldEtaInSeed)
			&& fabs(leptonInfo.EldeltaPhi[i]) < 0.0394 \
			&& leptonInfo.ElHadoverEm[i] < 0.0641 \
			&& leptonInfo.IsoRhoCorrR03[i]/leptonInfo.Pt[i] < 0.0571 \
			&& fabs(GsfEleEInverseMinusPInverseCut) < 0.0129 \
			&& leptonInfo.NumberOfExpectedInnerHits[i] <=1 \
			&& !(leptonInfo.ElhasConv[i]) );
		}
		
		//Get PassImpactPar
		bool PassImpactParameter = true;
		if( fabs(leptonInfo.Eta[i]) < 1.45 )
		{
        		if( fabs(leptonInfo.ElTrackDxy_PV[i]) > 0.05 )
			{ PassImpactParameter = false; }
		        if( fabs(leptonInfo.ElTrackDz[i])  > 0.10 )
            		{ PassImpactParameter = false; }
		}
		else
		{
			if( fabs(leptonInfo.ElTrackDxy_PV[i]) > 0.10 )
            		{ PassImpactParameter = false; }
		        if( fabs(leptonInfo.ElTrackDz[i]) > 0.20 )
            		{ PassImpactParameter = false; }
		}

		if( PassElTightID \
		&& leptonInfo.Pt[i] > 35. \
		&& fabs(leptonInfo.Eta[i]) < 2.1 \
		&& !( fabs(leptonInfo.Eta[i]) > 1.44 && fabs(leptonInfo.Eta[i]) < 1.57) \
		&& PassImpactParameter)
		{
			pass_no++; *index_ptr = i;
		}
	}//end of an event's for-loop
	
	if(pass_no == 1){ return true; }
	else { return false; }
}

bool Pass_Veto_Electron( LeptonInfo& leptonInfo,int sel_el_idx )
{
		
	int pass_no = 0;
	
	for(int i=0;i<leptonInfo.Size;i++)
	{
		if(i == sel_el_idx ) { continue; }		

		//Get Electron Loose ID
		bool PassElLooseID;
		//GsfEleEInverseMinusPInverseCut
		const float ecal_energy_inverse = 1.0 / leptonInfo.ElEcalE[i];
		const float eSCoverP = leptonInfo.ElEoverP[i];
		float GsfEleEInverseMinusPInverseCut = fabs(1.0 - eSCoverP)*ecal_energy_inverse;
	
		if( fabs(leptonInfo.Eta[i]) <= 1.479 )
		{
			PassElLooseID = ( leptonInfo.ElsigmaIetaIeta[i] < 0.011 \
			&& fabs(leptonInfo.EldeltaEta[i] - leptonInfo.Eta[i] + leptonInfo.caloEta[i]) < 0.00447 //abs(EldEtaInSeed)
			&& fabs(leptonInfo.EldeltaPhi[i]) < 0.222 \
			&& leptonInfo.ElHadoverEm[i] < 0.298 \
			&& leptonInfo.IsoRhoCorrR03[i]/leptonInfo.Pt[i] < 0.0994 \
			&& fabs(GsfEleEInverseMinusPInverseCut) < 0.241 \
			&& leptonInfo.NumberOfExpectedInnerHits[i] <=1 \
			&& !(leptonInfo.ElhasConv[i]) );
		}
		else
		{	 
			PassElLooseID = ( leptonInfo.ElsigmaIetaIeta[i] < 0.0314 \
			&& fabs(leptonInfo.EldeltaEta[i] - leptonInfo.Eta[i] + leptonInfo.caloEta[i]) < 0.00868 //abs(EldEtaInSeed)
			&& fabs(leptonInfo.EldeltaPhi[i]) < 0.213 \
			&& leptonInfo.ElHadoverEm[i] < 0.101 \
			&& leptonInfo.IsoRhoCorrR03[i]/leptonInfo.Pt[i] < 0.107 \
			&& fabs(GsfEleEInverseMinusPInverseCut) < 0.14 \
			&& leptonInfo.NumberOfExpectedInnerHits[i] <=1 \
			&& !(leptonInfo.ElhasConv[i]) );
		}	

		if( PassElLooseID \
		&& leptonInfo.Pt[i] > 15. \
		&& fabs(leptonInfo.Eta[i]) < 2.4 \
		&& !( fabs(leptonInfo.Eta[i]) > 1.44 && fabs(leptonInfo.Eta[i]) < 1.57) )
		{
			pass_no++;
		}
	}//end of an event's for-loop
	
	if(pass_no == 0){ return true; }
	else { return false; }
}

bool Pass_CR_Veto_Muon(LeptonInfo& leptonInfo,int sel_mu_idx)
{
	int pass_no = 0;
	for(int i=0;i<leptonInfo.Size;i++)
	{	
		if(i == sel_mu_idx) { continue; }		
			
		double rel = leptonInfo.ChargedHadronIsoR04[i] + max(float(0.), \
		leptonInfo.NeutralHadronIsoR04[i] + leptonInfo.PhotonIsoR04[i] \
		- float(0.5) * leptonInfo.sumPUPtR04[i] );
		rel /= leptonInfo.Pt[i];
		
		if(leptonInfo.isPFMuon[i]
		&& ((leptonInfo.MuType[i] & 0x02) || (leptonInfo.MuType[i] & 0x04))  //IsGlobalMuon or IsTrackerMuon
		&& leptonInfo.Pt[i] > 15. \
		&& fabs(leptonInfo.Eta[i]) <2.4 \
		//&& rel < 0.25)
			)
		{ pass_no++; };
	}
	
	if(pass_no == 0) { return true; }
	else { return false; }	
}

bool Pass_CR_Veto_Electron( LeptonInfo& leptonInfo,int sel_el_idx )
{
		
	int pass_no = 0;
	
	for(int i=0;i<leptonInfo.Size;i++)
	{
		if(i == sel_el_idx ) { continue; }		

		//Get Electron Loose ID
		bool PassElLooseID;
		//GsfEleEInverseMinusPInverseCut
		const float ecal_energy_inverse = 1.0 / leptonInfo.ElEcalE[i];
		const float eSCoverP = leptonInfo.ElEoverP[i];
		float GsfEleEInverseMinusPInverseCut = fabs(1.0 - eSCoverP) * ecal_energy_inverse;
	
		if( fabs(leptonInfo.Eta[i]) <= 1.479 )
		{
			PassElLooseID = ( leptonInfo.ElsigmaIetaIeta[i] < 0.011 \
			&& fabs(leptonInfo.EldeltaEta[i] - leptonInfo.Eta[i] + leptonInfo.caloEta[i]) < 0.00447 //abs(EldEtaInSeed)
			&& fabs(leptonInfo.EldeltaPhi[i]) < 0.222 \
			&& leptonInfo.ElHadoverEm[i] < 0.298 \
			//&& leptonInfo.IsoRhoCorrR03[i]/leptonInfo.Pt[i] < 0.0994
			&& fabs(GsfEleEInverseMinusPInverseCut) < 0.241 \
			&& leptonInfo.NumberOfExpectedInnerHits[i] <=1 \
			&& !(leptonInfo.ElhasConv[i]) );
		}
		else
		{	 
			PassElLooseID = ( leptonInfo.ElsigmaIetaIeta[i] < 0.0314 \
			&& fabs(leptonInfo.EldeltaEta[i] - leptonInfo.Eta[i] + leptonInfo.caloEta[i]) < 0.00868 //abs(EldEtaInSeed)
			&& fabs(leptonInfo.EldeltaPhi[i]) < 0.213 \
			&& leptonInfo.ElHadoverEm[i] < 0.101 \
			//&& leptonInfo.IsoRhoCorrR03[i]/leptonInfo.Pt[i] < 0.107 
			&& fabs(GsfEleEInverseMinusPInverseCut) < 0.14 \
			&& leptonInfo.NumberOfExpectedInnerHits[i] <=1 \
			&& !(leptonInfo.ElhasConv[i]) );
		}	

		if( PassElLooseID \
		&& leptonInfo.Pt[i] > 15. \
		&& fabs(leptonInfo.Eta[i]) < 2.4 \
		&& !( fabs(leptonInfo.Eta[i]) > 1.44 && fabs(leptonInfo.Eta[i]) < 1.57) )
		{
			pass_no++;
		}
	}//end of an event's for-loop
	
	if(pass_no == 0){ return true; }
	else { return false; }
}

#endif
