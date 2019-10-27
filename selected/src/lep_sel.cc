#include "TopCPViolation/selected/interface/lep_sel.h"

//Muon selection

bool Pass_sel_Mu_ISO( LeptonInfo& lep, const int& i )
{
	//RelIsoR04
    double rel = lep.ChargedHadronIsoR04[i] + max(float(0.), \
    lep.NeutralHadronIsoR04[i] + lep.PhotonIsoR04[i] - float(0.5) * lep.sumPUPtR04[i] );
    rel /= lep.Pt[i];
	if( rel < 0.15 )
	{	return true;	}
	return false;
}

bool Pass_veto_Mu_ISO( LeptonInfo& lep, const int& i )
{
	//RelIsoR04
    double rel = lep.ChargedHadronIsoR04[i] + max(float(0.), \
    lep.NeutralHadronIsoR04[i] + lep.PhotonIsoR04[i] - float(0.5) * lep.sumPUPtR04[i] );
    rel /= lep.Pt[i];
	if( rel < 0.25 )
	{	return true;	}
	return false;	
}

bool is_Global_Mu( LeptonInfo& lep, const int& i )
{
	return (lep.MuType[i]) & 0x02;
}

bool is_Tracker_Mu( LeptonInfo& lep, const int& i )
{
	return (lep.MuType[i]) & 0x04;
}

bool Pass_tightMu_ID( LeptonInfo& lep, const int& i )
{
	//IsGlobalMuon
	if(
		is_Global_Mu( lep, i )				&&
		lep.isPFMuon[i] 					&&
    	lep.MuGlobalNormalizedChi2[i] < 10. &&
    	lep.MuNMuonhits[i] > 0				&&
    	lep.MuNMatchedStations[i] > 1		&&
    	fabs(lep.MuInnerTrackDxy[i]) < 0.2	&&
    	fabs(lep.MuInnerTrackDz[i]) < 0.5	&&
    	lep.MuNPixelLayers[i] > 0			&&
    	lep.MuNTrackLayersWMeasurement[i] > 5
		)
	{	return true;	}
	return false;
}

bool Pass_looseMu_ID( LeptonInfo& lep, const int& i )
{
	if(
		( is_Global_Mu( lep, i ) || is_Tracker_Mu( lep, i ) )	&&
		lep.isPFMuon[i]
	)
	{	return true;	}
	return false;
}

bool Pass_sel_Mu_Kinematic( LeptonInfo& lep, const int& i )
{
	if( 
		lep.Pt[i] > 30.	&&
		fabs( lep.Eta[i] ) < 2.4
	  )
	{	return true;	}
	return false;
}

bool Pass_veto_Mu_Kinematic( LeptonInfo& lep, const int& i )
{
	if(
		lep.Pt[i] > 15.	&&
		fabs(lep.Eta[i]) < 2.4
			)
	{	return true;	}
	return false;
}

bool Pass_SR_Selected_Muon( LeptonInfo& leps,int& index )
{
    int pass_no = 0;
    for(int i=0;i<(int)leps.Size;i++)
    { 
    	if( Pass_tightMu_ID( leps, i )		&&
			Pass_sel_Mu_ISO( leps, i )		&&
		   	Pass_sel_Mu_Kinematic( leps, i )
          )
        {	pass_no++;	index = i;	}
    }    
    if(pass_no == 1) 
	{	return true;	}
	return false;
}

bool Pass_SR_Veto_Muon(LeptonInfo& leps,int sel_mu_idx)
{
    int pass_no = 0;
    for(int i=0;i<(int)leps.Size;i++)
    {
        if(i == sel_mu_idx) { continue; }
        
        if( Pass_looseMu_ID( leps, i )			&&
			Pass_veto_Mu_ISO( leps, i )			&&
           	Pass_veto_Mu_Kinematic( leps, i )
			)
        {	pass_no++;	};
    }
    if(pass_no == 0)
	{ return true; }
    return false;
}

bool Pass_CR_Selected_Muon( LeptonInfo& leps,int& index )
{
    int pass_no = 0;
    for(int i=0;i<leps.Size;i++)
    { 
    	if( Pass_tightMu_ID( leps, i )		&&
			Pass_sel_Mu_ISO( leps, i )		&&		//191014 doubt?
		   	Pass_sel_Mu_Kinematic( leps, i )
          )
        {	pass_no++;	index = i;	}
    }    
    if(pass_no == 1) 
	{	return true;	}
	return false;
}

bool Pass_CR_Veto_Muon(LeptonInfo& leps,int sel_mu_idx)
{
    int pass_no = 0;
    for(int i=0;i<(int)leps.Size;i++)
    {
        if(i == sel_mu_idx) { continue; }
        
        if( Pass_looseMu_ID( leps, i )			&&
			//Pass_veto_Mu_ISO( leps, i )			&&		//comment out to tight the CR case(because it's veto)
           	Pass_veto_Mu_Kinematic( leps, i )
			)
        {	pass_no++;	};
    }
    if(pass_no == 0)
	{ return true; }
    return false;
}

//Electron selection

//without ISO
bool Pass_CR_tightEl_ID( LeptonInfo& lep, const int& i, EvtInfo& evt )
{
    const float ecal_energy_inverse = 1.0 / lep.ElEcalE[i];
    const float eSCoverP = lep.ElEoverP[i];
    float GsfEleEInverseMinusPInverseCut = fabs(1.0 - eSCoverP)*ecal_energy_inverse;
	if( fabs( lep.Eta[i] ) <= 1.479 )
	{	
		if(
			lep.SigmaIetaIeta[i] < 0.0104 	&&
			fabs( lep.EldeltaPhiSeed[i] ) < 0.00255 &&	//EldEtaInSeed
			fabs( lep.EldeltaPhi[i] ) < 0.022 		&&	//EldPhiIn
			lep.ElHadoverEm[i] < 0.026 + 1.15 / lep.EnergySC[i] + 0.0324 * evt.Rho / lep.EnergySC[i] &&		
			//ElGsfEleHadronicOverEMCut() < 0.026 + 1.15 / EnergySC() + 0.0324 * EvtRho() / EnergySC()
			fabs(GsfEleEInverseMinusPInverseCut) < 0.159 &&
			lep.NumberOfExpectedInnerHits[i] <= 1 &&
			!lep.ElhasConv[i]
	  	)
		{	return true;	}
		else
		{	return false;	}
	}
	else
	{
		if(
			lep.SigmaIetaIeta[i] < 0.0353 	&&
			fabs( lep.EldeltaPhiSeed[i] ) < 0.00501 &&	//EldEtaInSeed
			fabs( lep.EldeltaPhi[i] ) < 0.0236 		&&	//EldPhiIn
			lep.ElHadoverEm[i] < 0.0188 + 2.06 / lep.EnergySC[i] + 0.183 * evt.Rho / lep.EnergySC[i] &&		
			//ElGsfEleHadronicOverEMCut() < 0.0188 + 2.06 / EnergySC() + 0.183 * EvtRho() / EnergySC()
			fabs(GsfEleEInverseMinusPInverseCut) < 0.197 &&
			lep.NumberOfExpectedInnerHits[i] <= 1 &&
			!lep.ElhasConv[i]
	  	)
		{	return true;	}
		else
		{	return false;	}
	}
}

//without ISO
bool Pass_CR_looseEl_ID( LeptonInfo& lep, const int& i, EvtInfo& evt )
{
    const float ecal_energy_inverse = 1.0 / lep.ElEcalE[i];
    const float eSCoverP = lep.ElEoverP[i];
    float GsfEleEInverseMinusPInverseCut = fabs(1.0 - eSCoverP)*ecal_energy_inverse;
	if( fabs( lep.Eta[i] ) <= 1.479 )
	{	
		if(
			lep.SigmaIetaIeta[i] < 0.0112 	&&
			fabs( lep.EldeltaPhiSeed[i] ) < 0.00377 &&	//EldEtaInSeed
			fabs( lep.EldeltaPhi[i] ) < 0.0884 		&&	//EldPhiIn
			lep.ElHadoverEm[i] < 0.005 + 1.16 / lep.EnergySC[i] + 0.0324 * evt.Rho / lep.EnergySC[i] &&		
			fabs(GsfEleEInverseMinusPInverseCut) < 0.193 &&
			lep.NumberOfExpectedInnerHits[i] <= 1 &&
			!lep.ElhasConv[i]
	  	)
		{	return true;	}
		else
		{	return false;	}
	}
	else
	{
		if(
			lep.SigmaIetaIeta[i] < 0.0425 	&&
			fabs( lep.EldeltaPhiSeed[i] ) < 0.00674 &&	//EldEtaInSeed
			fabs( lep.EldeltaPhi[i] ) < 0.169 		&&	//EldPhiIn
			lep.ElHadoverEm[i] < 0.0441 + 2.54 / lep.EnergySC[i] + 0.183 * evt.Rho / lep.EnergySC[i] &&		
			fabs(GsfEleEInverseMinusPInverseCut) < 0.111 &&
			lep.NumberOfExpectedInnerHits[i] <= 1 &&
			!lep.ElhasConv[i]
	  	)
		{	return true;	}
		else
		{	return false;	}
	}
}

double El_PFISO( LeptonInfo& lep, const int& i )
{
	return lep.PFIsoRhoCorrR03[i] / lep.Pt[i];
}

bool Pass_tight_El_ISO( LeptonInfo& lep, const int& i )
{
	if( fabs( lep.Eta[i] ) <= 1.479 ){
	   return El_PFISO( lep, i ) < ( 0.0287 + 0.506 / lep.Pt[i] );
	}
	else{  //absEta > 1.479
	return El_PFISO( lep, i ) < ( 0.0445 + 0.963 / lep.Pt[i] );
	}
}

//within ISO 
bool Pass_SR_tightEl_ID( LeptonInfo& lep, const int& i )
{
	return lep.EgammaCutBasedEleIdTIGHT[i];
}

//within ISO
bool Pass_SR_looseEl_ID( LeptonInfo& lep, const int& i )
{
	return lep.EgammaCutBasedEleIdLOOSE[i] ;
}

bool Pass_sel_El_Kinematic( LeptonInfo& lep, const int& i )
{
	if( lep.Pt[i] > 35. && fabs(lep.Eta[i]) < 2.1 
        && !( fabs(lep.Eta[i]) > 1.44 && fabs(lep.Eta[i]) < 1.57 ) )
	{	return true;	}
	return false;
}

bool Pass_veto_El_Kinematic( LeptonInfo& lep, const int& i )
{
	if( lep.Pt[i] > 15. && fabs(lep.Eta[i]) < 2.4 
        && !( fabs(lep.Eta[i]) > 1.44 && fabs(lep.Eta[i]) < 1.57 ) )
	{	return true;	}
	return false;
}

//just used in tight El
bool Pass_ImpactParameter( LeptonInfo& lep, const int& i )
{
	bool PassImpactParameter = true;
    if( fabs(lep.Eta[i]) < 1.45 )
    {
        if( fabs(lep.ElTrackDxy[i]) > 0.05 )
        { PassImpactParameter = false; }
        if( fabs(lep.ElTrackDz[i])  > 0.10 )
        { PassImpactParameter = false; }
    }
    else
    {
        if( fabs(lep.ElTrackDxy[i]) > 0.10 )
        { PassImpactParameter = false; }
        if( fabs(lep.ElTrackDz[i]) > 0.20 )
        { PassImpactParameter = false; }
	}
	return PassImpactParameter;
}

bool Pass_SR_Selected_Electron(LeptonInfo& leps,int& index )
{
    int pass_no = 0;
    
    for(int i=0;i<(int)leps.Size;i++)
    {
        if( Pass_SR_tightEl_ID( leps, i )
			&& Pass_sel_El_Kinematic( leps, i )
        	&& Pass_ImpactParameter( leps, i ) )
        {
            pass_no++; index = i;
        }
    }//end of an event's for-loop
    
    if(pass_no == 1){ return true; }
    else { return false; }
}

bool Pass_CR_Selected_Electron( LeptonInfo& leps, int& index, EvtInfo& evt )
{
    int pass_no = 0;
    
    for(int i=0;i<(int)leps.Size;i++)
    {
        if( Pass_CR_tightEl_ID( leps, i, evt )
			&& Pass_tight_El_ISO( leps, i )
			&& Pass_sel_El_Kinematic( leps, i )
        	&& Pass_ImpactParameter( leps, i ) )
        {
            pass_no++; index = i;
        }
    }//end of an event's for-loop
    
    if(pass_no == 1){ return true; }
    else { return false; }
}

bool Pass_SR_Veto_Electron( LeptonInfo& leps,int sel_el_idx )
{
    int pass_no = 0;
    
    for(int i=0;i<(int)leps.Size;i++)
    {
        if( i == sel_el_idx ) { continue; }
        
        if( Pass_SR_looseEl_ID( leps, i ) &&
			Pass_veto_El_Kinematic( leps, i ) ) 
		{	pass_no++;	}
    }//end of an event's for-loop
    
    if( pass_no == 0 ){ return true; }
    else { return false; }
}

bool Pass_CR_Veto_Electron( LeptonInfo& leps, int sel_el_idx, EvtInfo& evt )
{
	int pass_no = 0;
	
	for(int i=0;i<(int)leps.Size;i++)
	{
		if(i == sel_el_idx ) { continue; }		

		if( Pass_CR_looseEl_ID( leps, i, evt ) &&
			Pass_veto_El_Kinematic( leps, i ) )
		{
			pass_no++;
		}
	}//end of an event's for-loop
	if(pass_no == 0){ return true; }
	else { return false; }
}


//for control region's QCD

bool Pass_CR_InvSelected_Muon( LeptonInfo& leps,int& index )
{
    int pass_no = 0;
    for(int i=0;i<leps.Size;i++)
    { 
    	if( Pass_tightMu_ID( leps, i )		&&
			!Pass_sel_Mu_ISO( leps, i )		&&
		   	Pass_sel_Mu_Kinematic( leps, i )
          )
        {	pass_no++;	index = i;	}
    }    
    if(pass_no == 1) 
	{	return true;	}
	return false;
}


bool Pass_CR_InvSelected_Electron( LeptonInfo& leps, int& index, EvtInfo& evt )
{
    int pass_no = 0;
    
    for(int i=0;i<(int)leps.Size;i++)
    {
        if( Pass_CR_tightEl_ID( leps, i, evt )
			&& !Pass_tight_El_ISO( leps, i )
			&& Pass_sel_El_Kinematic( leps, i )
        	&& Pass_ImpactParameter( leps, i ) )
        {
            pass_no++; index = i;
        }
    }//end of an event's for-loop
    
    if(pass_no == 1){ return true; }
    else { return false; }
}

//for pre-pre selection

bool Pass_presel_Selected_Muon( LeptonInfo& leps,int& index )
{
    int pass_no = 0;
    for(int i=0;i<(int)leps.Size;i++)
    { 
    	if( Pass_tightMu_ID( leps, i )		&&
		   	Pass_sel_Mu_Kinematic( leps, i )
          )
        {	pass_no++;	index = i;	}
    }    
    if(pass_no == 1) 
	{	return true;	}
	return false;
}

bool Pass_presel_Selected_Electron( LeptonInfo& leps, int& index, EvtInfo& evt )
{
    int pass_no = 0;
    
    for(int i=0;i<(int)leps.Size;i++)
    {
        if( Pass_CR_tightEl_ID( leps, i, evt )
			&& Pass_sel_El_Kinematic( leps, i )
        	&& Pass_ImpactParameter( leps, i ) )
        {
            pass_no++; index = i;
        }
    }//end of an event's for-loop
    
    if(pass_no == 1){ return true; }
    else { return false; }
}
