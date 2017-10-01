 #include <MyHeaders.h>

/* This function determines the species loss rate by combining the species
 * coefficients from the reactions with the reaction rate for each reaction.
 * The output from this function is the rate of change of the concentration for
 * every species in the system. */

vector< double > SpeciesLossRate(
		const vector< TrackSpecies >& SpeciesLossList,
		int Number_Species,
		const vector< double >& Combined_Rates
		)
{

	int i;

	vector< double > temp_species_loss;
	temp_species_loss.resize(Number_Species);

	for(i=0;i< (int) SpeciesLossList.size();i++){

		temp_species_loss[SpeciesLossList[i].SpeciesID]=
				temp_species_loss[SpeciesLossList[i].SpeciesID] +
				SpeciesLossList[i].coefficient * Combined_Rates[SpeciesLossList[i].ReactionID];
	}

	return temp_species_loss;
}
