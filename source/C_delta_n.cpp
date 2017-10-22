#include <MyHeaders.h>

vector< double > Get_Delta_N(
		const vector< SingleReactionData >& Reactions
		)
{
	int i,j;
	int number_species = (int)Reactions[0].Reactants.size();
	int number_reactions = (int)Reactions.size();

	vector< double > temp_delta_n;

	for(i=0;i<number_reactions;i++)
	{
		double delta_n = 0;
		for(j=0;j<number_species;j++){
			delta_n = delta_n + Reactions[i].Reactants[j] + Reactions[i].Products[j]; // reactants & products
		}
		temp_delta_n.push_back(delta_n);
	}
	return temp_delta_n;
}
