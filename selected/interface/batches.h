#ifndef BATCHES_H
#define BATCHES_H

#include "TChain.h"
#include "TTree.h"

#include <string>
#include "string.h"
#include <iostream>

void Add_in_batches( TChain* ch , string option )
{
	

	ch->Add();

}



#endif
