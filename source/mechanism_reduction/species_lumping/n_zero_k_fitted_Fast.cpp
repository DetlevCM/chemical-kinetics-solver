/*
 * n_zero_k.fitted_Fast.cpp
 *
 *  Created on: 17.01.2016
 *      Author: DetlevCM
 */

#include "../headers/Headers.hpp"


ReactionParameter n_zero_k_fitted_Fast(
		vector< SingleReactionData >& Reactions,
		double temperature,
		size_t Reaction_Group_Size
)
{
	ReactionParameter ParameterOutput;
	size_t j;


	vector <double> Group_k(3);
	double temperature_endpoints = 20;

	for(j=0;j<Reaction_Group_Size;j++)
	{
		// lower temperature point
		Group_k[0] =
				Group_k[0] +
				Reactions[j].paramA *
				pow((temperature - temperature_endpoints),Reactions[j].paramN) *
				exp(-Reactions[j].paramEa/(temperature - temperature_endpoints));

		// middle temperature point
		Group_k[1] =
				Group_k[1] +
				Reactions[j].paramA *
				pow((temperature),Reactions[j].paramN) *
				exp(-Reactions[j].paramEa/(temperature ));

		// upper temperature point
		Group_k[2] =
				Group_k[2] +
				Reactions[j].paramA *
				pow((temperature + temperature_endpoints),Reactions[j].paramN) *
				exp(-Reactions[j].paramEa/(temperature + temperature_endpoints));
	}

	// Fit new A and Ea with n = 0 - I have k over a temperature range of 40K

	Group_k[0] = log(Group_k[0]);
	Group_k[1] = log(Group_k[1]);
	Group_k[2] = log(Group_k[2]);

	// average gradient of k vs 1/T
	double gradient, intercept;

	// we have 3 k, so we can use the average of the two gradients to get the middle point
	gradient = 0.5 * (
			(Group_k[1] - Group_k[0])/(temperature_endpoints) +
			(Group_k[2] - Group_k[1])/(temperature_endpoints) );

	// now need the intercept, use the middled point (average of end points)
	intercept  = Group_k[1] + (1/temperature)*gradient;

	ParameterOutput.Reversible = false;
	ParameterOutput.paramA = exp(intercept);
	ParameterOutput.paramN = 0;
	ParameterOutput.paramEa = gradient;


	return ParameterOutput;

}
