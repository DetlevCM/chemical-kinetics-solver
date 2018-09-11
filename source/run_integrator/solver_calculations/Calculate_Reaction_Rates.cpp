/*
 * Calculate_Reaction_Rates_New.cpp
 *
 *  Created on: 01.01.2013
 *      Author: DetlevCM
 */


#include <Headers.hpp>

// NOTE: This function modifies Kf and Kr - input as Forward_Rates and Reverse_Rates

//vector< double > CalculateReactionRates(
void CalculateReactionRates(
		vector< double >& Rates,
		const vector< double >& SpeciesConcentration,
		vector< double > Forward_Rates,
		vector< double > Reverse_Rates,
		const vector< TrackSpecies >& ReactantsForRates,
		const vector< TrackSpecies >& ProductsForRates
)
{

	int i;



	for(i = 0;i< (int) ReactantsForRates.size();i++){ // Forward Rates determined by the reactants

		if(ReactantsForRates[i].coefficient == 1) // this is quicker than raising to the power of 1 - but check with the struct
		{
			Forward_Rates[ReactantsForRates[i].ReactionID] =
					Forward_Rates[ReactantsForRates[i].ReactionID]*
					SpeciesConcentration[ReactantsForRates[i].SpeciesID];
		}
		else
		{
			Forward_Rates[ReactantsForRates[i].ReactionID] =
					Forward_Rates[ReactantsForRates[i].ReactionID]*
					pow(SpeciesConcentration[ReactantsForRates[i].SpeciesID],ReactantsForRates[i].coefficient);
		}
	}


	for(i = 0;i< (int) ProductsForRates.size();i++){ // Reverse Rates determined by the products

		if(ProductsForRates[i].coefficient == 1) // this is quicker than raising to the power of 1 - but check with the struct
		{
			Reverse_Rates[ProductsForRates[i].ReactionID] =
					Reverse_Rates[ProductsForRates[i].ReactionID]*
					SpeciesConcentration[ProductsForRates[i].SpeciesID];
		}
		else
		{
			Reverse_Rates[ProductsForRates[i].ReactionID] =
					Reverse_Rates[ProductsForRates[i].ReactionID]*
					pow(SpeciesConcentration[ProductsForRates[i].SpeciesID],ProductsForRates[i].coefficient);
		}
	}


	for(i=0;i<(int) Rates.size();i++){
		Rates[i] = Forward_Rates[i] - Reverse_Rates[i];
		//	cout << Rates[i] << "   " << Forward_Rates[i] << "   " << Reverse_Rates[i] << "\r\n";
	}

}

