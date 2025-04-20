#include "../headers/Headers.hpp"

vector< ReactionParameter > Process_Reaction_Parameters(
		const vector< SingleReactionData >& Reactions
		)
{
	size_t i;
	size_t Number_Reactions = Reactions.size();

	ReactionParameter temp_one_reaction;
	vector< ReactionParameter > temp_output;

	// Output per reaction
	for(i=0;i<Number_Reactions;i++)
	{
		temp_one_reaction.paramA = Reactions[i].paramA; // A
		temp_one_reaction.paramN = Reactions[i].paramN; // n
		temp_one_reaction.paramEa = Reactions[i].paramEa; // Ea
		temp_one_reaction.Reversible = Reactions[i].Reversible;

		temp_output.push_back(temp_one_reaction);
	}

	return temp_output;
}
