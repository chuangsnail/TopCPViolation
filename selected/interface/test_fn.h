#include "TLegend.h"
#include "TCanvas.h"

#include <vector>


auto GetLegend1()
{
	auto legend = new TLegend(0.8,0.5,0.99,0.99);
	legend->SetBorderSize( 0 );
	legend->SetTextFont( 43 );
	legend->SetTextSize( 18 );
	legend->SetFillColorAlpha( 0, 0 );

	return legend;
}


TLegend* GetLegend2()
{
	TLegend* legend = new TLegend(0.8,0.5,0.99,0.99);
	legend->SetBorderSize( 0 );
	legend->SetTextFont( 43 );
	legend->SetTextSize( 18 );
	legend->SetFillColorAlpha( 0, 0 );

	return legend;
}
