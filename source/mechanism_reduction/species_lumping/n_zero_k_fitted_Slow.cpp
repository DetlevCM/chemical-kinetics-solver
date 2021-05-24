/*
 * n_zero_k.fitted_Slow.cpp
 *
 *  Created on: 17.01.2016
 *      Author: DetlevCM
 */

#include "../headers/Headers.hpp"


ReactionParameter n_zero_k_fitted_Slow(
		vector< SingleReactionData >& Reactions,
		double temperature,
		size_t Reaction_Group_Size
)
{
	ReactionParameter ParameterOutput;
	size_t j, k;

	vector <double> Group_k(41);

	for(j=0;j<Reaction_Group_Size;j++)
	{
		// work out k over all reactions
		for(k=0;k<41;k++){
			double temp_mod = (double)k - 20.0;

			Group_k[k] =
					Group_k[k] +
					Reactions[j].paramA *
					pow((temperature + temp_mod),Reactions[j].paramN) *
					exp(-Reactions[j].paramEa/(temperature + temp_mod));
		}
	}

	// Fit new A and Ea with n = 0 - I have k over a temperature range of 40K

	for(k=0;k<41;k++){
		Group_k[k] = log(Group_k[k]);
	}

	// average gradient of k vs 1/T
	double gradient, intercept;
	gradient = 0; // initialise to zero

	/* Individual steps are only really interesting if the function is not
	 * monotonous... it isn't really necessary, its the original code and
	 * will continue to exist in here for now.
	 */

	for(k=1;k<41;k++){
		gradient = gradient +
				(
						(Group_k[k-1]-Group_k[k])/
						(1/(temperature-1))-(1/temperature)
				); // /1 -> steps of 1K but inverse must be accounted for
	}
	gradient = gradient/40; //40 points I think...

	// now need the intercept
	intercept  = Group_k[20]+(1/temperature)*gradient;


	ParameterOutput.Reversible = false;
	ParameterOutput.paramA = exp(intercept);
	ParameterOutput.paramN = 0;
	ParameterOutput.paramEa = gradient;


	return ParameterOutput;
}
