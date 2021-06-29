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
		// the three cases in which Arrhenius parameters are provided:
		temp_one_reaction.param_forward = Reactions[i].param_forward;
		temp_one_reaction.param_reverse = Reactions[i].param_reverse;

		temp_one_reaction.Reversible = Reactions[i].Reversible;

		// for cases where an explicit reverse reaction is specified
		temp_one_reaction.explicit_rev = Reactions[i].explicit_rev;

		// add in third body support...
		temp_one_reaction.ThirdBodyType = Reactions[i].ThirdBodyType;
		temp_one_reaction.param_TB_low = Reactions[i].param_TB_low;
		temp_one_reaction.ThBd_param = Reactions[i].ThBd_param;
		temp_one_reaction.sri_flag = Reactions[i].sri_flag;
		temp_one_reaction.collision_efficiency = Reactions[i].collision_efficiency;

		temp_one_reaction.sriThirdBody = Reactions[i].sriThirdBody;
		temp_one_reaction.troeThirdBody = Reactions[i].troeThirdBody;

		temp_output.push_back(temp_one_reaction);
	}

	return temp_output;
}
