#include "../headers/Headers.hpp"

/* This function determines the species loss rate by combining the species
 * coefficients from the reactions with the reaction rate for each reaction.
 * The output from this function is the rate of change of the concentration for
 * every species in the system. */

vector< double > SpeciesLossRate(
		int Number_Species,
		const vector< double >& Combined_Rates,
		const vector< TrackSpecies >& SpeciesLossList
		)
{
	int i;

	vector< double > temp_species_loss(Number_Species);

	for(i=0;i< (int) SpeciesLossList.size();i++){
		temp_species_loss[SpeciesLossList[i].SpeciesID] =
				temp_species_loss[SpeciesLossList[i].SpeciesID] +
				(Combined_Rates[SpeciesLossList[i].ReactionID] * SpeciesLossList[i].coefficient);
	}

	return temp_species_loss;
}
