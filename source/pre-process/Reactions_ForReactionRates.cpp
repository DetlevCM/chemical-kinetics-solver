#include <MyHeaders.h>

/*
struct ReactionRate{
	int ReactionID;
	int SpeciesID;
	int ReactantProduct;
	double coefficient;
};//*/


vector< TrackSpecies > Reactants_ForReactionRate(
		const vector< SingleReactionData >& Reactions
)
{
	vector< TrackSpecies > temp_proc_reac;
	// Bassically go through the reactions and accumulate the Relevant species

	TrackSpecies temp;
	int Number_Reactions = Reactions.size();
	int Number_Species = Reactions[0].Reactants.size();
	int i,j;

	// Output per reaction
	for(i=0;i<Number_Reactions;i++)
	{
		for(j=0;j<Number_Species;j++)
		{
			if(Reactions[i].Reactants[j] != 0)
			{
				temp.ReactionID = i;
				temp.SpeciesID = j;
				temp.coefficient = - Reactions[i].Reactants[j];
				temp_proc_reac.push_back(temp);
			}
		}
	}

	return temp_proc_reac;
}



vector< TrackSpecies > Products_ForReactionRate(
		const vector< SingleReactionData >& Reactions,
		bool SwitchType
)
{
	vector< TrackSpecies > temp_proc_reac;
	// Bassically go through the reactions and accumulate the Relevant species 
	int i,j;
	TrackSpecies temp;
	int Number_Reactions = Reactions.size();
	int Number_Species = Reactions[0].Reactants.size();

	if(!SwitchType) // false treat as rates function
	{
		// Output per reaction
		for(i=0;i<Number_Reactions;i++)
		{
			if(Reactions[i].Reversible) // only include if the reaction is reversible
			{
				for(j=0;j<Number_Species;j++)
				{
					if(Reactions[i].Products[j] != 0)
					{
						temp.ReactionID = i;
						temp.SpeciesID = j;
						temp.coefficient = Reactions[i].Products[j];
						temp_proc_reac.push_back(temp);
					}
				}
			}
		}
	}
	else // true, for Rates Analysis
	{
		for(i=0;i<Number_Reactions;i++)
		{
			for(j=0;j<Number_Species;j++)
			{
				if(Reactions[i].Products[j] != 0)
				{
					temp.ReactionID = i;
					temp.SpeciesID = j;
					temp.coefficient = Reactions[i].Products[j];
					temp_proc_reac.push_back(temp);
				}
			}
		}
	}


	return temp_proc_reac;
}


