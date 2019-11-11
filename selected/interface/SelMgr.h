/******************************************************************
 *
 *	File Name : SelMgr.h
 *	Description : 
 *
 *
 *
 * ****************************************************************/

#ifndef SELMGR_H
#define SELMGR_H

#include "TopCPViolation/selected/interface/BranchInfo.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <cmath>
#include <cassert>

#include "TLorentzVector.h"
#include "TVector2.h"
#include "TVector3.h"

using namespace std;

class JetMgr
{
private:
	JetInfo* jets;
	//int now_idx;		
	//we can Call any member for example,Pt() default in now_idx		

friend class SelMgr;

public:
	JetMgr(){};
	//Register in
	JetMgr( JetInfo* );	

	void RegIn( JetInfo* );


	//void Set_Idx( const int& );
	
	double Pt(const int&) const;
	//double Eta(const int& idx);
	//double Phi(const int& idx);
};

JetMgr::JetMgr( JetInfo* j )	//it can call JetMgr::JetMgr(){};
{
	RegIn(j);
}

void JetMgr::RegIn( JetInfo* j )
{
	jets = j;
}

double JetMgr::Pt(const int& idx) const
{
	return jets->Pt[idx];
}

class SelMgr
{
protected:
	JetMgr Jets;
		
public:
	SelMgr(){};			//it can call JetMgr::JetMgr(){};
	SelMgr( JetInfo* );
	void test_sel_print();

};

SelMgr::SelMgr( JetInfo* j )
{
	Jets.RegIn( j );
}

void SelMgr::test_sel_print()
{
	//root->GetEntry(i) outside the class at main()
	int size = Jets.jets->Size;
	cout << "size : " << size << endl;
	for(int i=0;i<size;i++)
	{
		if(Jets.Pt(i) > 50.){
			cout << "Jet's Idx[" << i << "] : " << Jets.Pt(i) << endl;	
		}
	}
}

#endif
