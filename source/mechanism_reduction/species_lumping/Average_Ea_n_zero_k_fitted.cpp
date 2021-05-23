/*
 * Average_Ea_CalculateNewParametersSlow.cpp
 *
 *  Created on: 17.01.2016
 *      Author: DetlevCM
 */

#include "../headers/Headers.hpp"


ReactionParameter Average_Ea_n_zero_k_fitted(
		vector< SingleReactionData >& temp_reactions3,
		double temperature,
		size_t Reaction_Group_Size
)
{

	/*
	 * a "special" mapping...
	 * we take the average Ea from RMG and set n to zero
	 * we then fit the A value to match k
	 * this will NOT give a correct temperature dependence
	 */


	//cout << "Slow Parameter Calculation Function \n";
	ReactionParameter ParameterOutput;
	size_t j;

	vector <double> Group_k(41);
	double fitted_Ea = 0.0;

	//for(j=0;j<Reaction_Grouping[i];j++)
	for(j=0;j<Reaction_Group_Size;j++)
	{
		// work out k over all reactions
		for(size_t k=0;k<41;k++){
			double temp_mod = (double)k - 20.0;

			Group_k[k] =
					Group_k[k] +
					temp_reactions3[j].paramA *
					pow((temperature + temp_mod),temp_reactions3[j].paramN) *
					exp(-temp_reactions3[j].paramEa/(temperature + temp_mod));
		}

		// get average Ea
		fitted_Ea = fitted_Ea + temp_reactions3[j].paramEa;
	}

	// this is the new Ea for our fitted reaction
	//fitted_Ea = fitted_Ea/( (double) Reaction_Grouping[i]);
	fitted_Ea = fitted_Ea/( (double) Reaction_Group_Size);
	cout << fitted_Ea << " \n";

	// we need the equivlaent of an ln(k) vs T plot for a straight line with a gradient
	// now we need an Ea gradient as well as ln(T) gradient
	vector <double> Ea_for_gradient(41);
	vector <double> lnT_for_gradient(41);
	for(size_t k=0;k<41;k++){
		double temp_mod = (double)k - 20.0;
		// we do not need R as the internal Ea is in Kelvins
		Ea_for_gradient[k] = -fitted_Ea/((temperature + temp_mod));
		lnT_for_gradient[k] = log(temperature + temp_mod);
		Group_k[k] = log(Group_k[k]);
	}


	// ln(k) = ln(A) + ln(T^n) + (-Ea/T) || Ea in Kelvin (else -Ea/RT)


	// average gradient of k vs 1/T


	// Individual step would only be needed if the function is no monotonous...
	/*
				double gradient = 0;
				for(int k=1;k<41;k++){
					gradient = gradient +
							(
									(Group_k[k-1]-Group_k[k])/
									(1/(temperature-1))-(1/temperature)
							); // /1 -> steps of 1K but inverse must be accounted for
				}
				gradient = gradient/40; //40 points I think...
				//*/

	// Work out Ea component of gradient

	//double gradient_in_Group_k = (Group_k[41] - Group_k[0])/40;
	//double gradient_in_Ea = (Ea_for_gradient[41] - Ea_for_gradient[0])/40;
	//double gradient_in_lnT = (lnT_for_gradient[41] - lnT_for_gradient[0])/40;

	double fitted_n;
	//double difference_in_gradient = gradient_in_Group_k - gradient_in_Ea;

	//fitted_n = difference_in_gradient / gradient_in_lnT;
	fitted_n = 0;

	// now for the fitted A - just use the middle, the target temperature
	double ln_fitted_a;
	//ln_fitted_a = Group_k[20]- fitted_n*log(temperature) - (-fitted_Ea/temperature);
	ln_fitted_a = Group_k[20]- (-fitted_Ea/temperature);

	// now need the intercept
	//double intercept  = Group_k[20]+(1/temperature)*gradient;

	ParameterOutput.Reversible = false;
	ParameterOutput.paramA = exp(ln_fitted_a);
	ParameterOutput.paramN = fitted_n;
	ParameterOutput.paramEa = fitted_Ea;

	return ParameterOutput;
}
