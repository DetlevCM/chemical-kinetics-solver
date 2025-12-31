
#include "./run_integrator.h"

vector< double > Get_Delta_N(
		const vector< SingleReactionData > Reactions
		)
{
	size_t i,j;
	size_t number_species = Reactions[0].Reactants.size();
	size_t number_reactions = Reactions.size();

	vector< double > delta_n(number_reactions);

	for(i=0;i<number_reactions;i++)
	{
		//double delta_n = 0;
		for(j=0;j<number_species;j++){
			delta_n[i] = delta_n[i] + Reactions[i].Reactants[j] + Reactions[i].Products[j]; // reactants & products
		}
		//temp_delta_n.push_back(delta_n);
	}
	return delta_n;
}
