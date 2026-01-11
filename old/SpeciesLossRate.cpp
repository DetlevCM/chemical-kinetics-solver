

#include "./solver_calculations.h"

/* This function determines the species loss rate by combining the species
 * coefficients from the reactions with the reaction rate for each reaction.
 * The output from this function is the rate of change of the concentration for
 * every species in the system. */

vector< double > SolverCalculation::SpeciesLossRate(
		size_t Number_Species,
		const vector< double >& Combined_Rates,
		const vector< TrackSpecies >& SpeciesLossList
		)
{
	vector< double > temp_species_loss(Number_Species);

	for(size_t i=0;i< SpeciesLossList.size();i++){
		temp_species_loss[SpeciesLossList[i].SpeciesID] =
				temp_species_loss[SpeciesLossList[i].SpeciesID] +
				(Combined_Rates[SpeciesLossList[i].ReactionID] * SpeciesLossList[i].coefficient);
	}


    /*
	for(size_t i = 0; i< temp_species_loss.size();i++)
	{
		cout << i << "  " << temp_species_loss[i] << std::flush; 
	}
	//*/

	return temp_species_loss;
}
