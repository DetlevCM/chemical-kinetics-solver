#include <Headers.hpp>

vector< TrackSpecies > PrepareSpecies_ForSpeciesLoss(
		const vector< SingleReactionData >& Reactions
		)
{
	int i,j;
	int Number_Reactions = (int)Reactions.size();
	int Number_Species = (int)Reactions[0].Reactants.size();
	vector< TrackSpecies > SpeciesLossAll;

	TrackSpecies temp;

	for(i=0;i<Number_Species;i++)
	{
		for(j=0;j<Number_Reactions;j++)
		{
			if(Reactions[j].Reactants[i] != 0) // Reactants
			{
				temp.SpeciesID = (int) i;
				temp.ReactionID = (int) j;
				temp.coefficient = Reactions[j].Reactants[i];
				SpeciesLossAll.push_back(temp);
			}
		}
		// not sure if this may not be more efficient - first just negative, then just positive
		for(j=0;j<Number_Reactions;j++)
		{
			if(Reactions[j].Products[i] != 0) // Products
			{
				temp.SpeciesID = (int) i;
				temp.ReactionID = (int) j;
				temp.coefficient = Reactions[j].Products[i];
				SpeciesLossAll.push_back(temp);
			}
		}
	}

	return SpeciesLossAll;
}
