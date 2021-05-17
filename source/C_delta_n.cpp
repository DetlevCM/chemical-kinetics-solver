#include "../headers/Headers.hpp"

vector< double > Get_Delta_N(
		const vector< SingleReactionData >& Reactions
		)
{
	size_t i,j;
	size_t number_species = Reactions[0].Reactants.size();
	size_t number_reactions = Reactions.size();

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
