#include "../headers/Headers.hpp"

vector< TrackSpecies > PrepareSpecies_ForSpeciesLoss(
		const vector< SingleReactionData >& Reactions
		)
{
	size_t i,j;
	size_t Number_Reactions = Reactions.size();
	size_t Number_Species = Reactions[0].Reactants.size();
	vector< TrackSpecies > SpeciesLossAll;

	TrackSpecies temp;

	for(i=0;i<Number_Species;i++)
	{
		for(j=0;j<Number_Reactions;j++)
		{
			if(Reactions[j].Reactants[i] != 0) // Reactants
			{
				temp.SpeciesID = i;
				temp.ReactionID = j;
				temp.coefficient = Reactions[j].Reactants[i];
				SpeciesLossAll.push_back(temp);
			}
		}
		// not sure if this may not be more efficient - first just negative, then just positive
		for(j=0;j<Number_Reactions;j++)
		{
			if(Reactions[j].Products[i] != 0) // Products
			{
				temp.SpeciesID = i;
				temp.ReactionID = j;
				temp.coefficient = Reactions[j].Products[i];
				SpeciesLossAll.push_back(temp);
			}
		}
	}

	return SpeciesLossAll;
}
