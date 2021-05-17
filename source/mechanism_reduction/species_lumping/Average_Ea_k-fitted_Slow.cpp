/*
 * Average_Ea_CalculateNewParametersSlow.cpp
 *
 *  Created on: 17.01.2016
 *      Author: DetlevCM
 */

#include "../headers/Headers.hpp"


ReactionParameter Average_Ea_CalculateNewParametersSlow(
		vector< SingleReactionData >& temp_reactions3,
		double temperature,
		size_t Reaction_Group_Size
)
{

	/*
	 * Not functional as of now, not sure if it will be fully implemented
	 */


	//cout << "Slow Parameter Calculation Function \n";
	ReactionParameter ParameterOutput;
	size_t j;

	vector <double> Group_k(41);
	double fitted_Ea = 0;

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
	//cout << fitted_Ea << " \n";

	// we need the equivlaent of an ln(k) vs T plot for a straight line with a gradient
	// now we need an Ea gradient as well as ln(T) gradient
	vector <double> Ea_for_gradient(41);
	//vector <double> lnT_for_gradient(41);

	for(size_t k=0;k<41;k++){
		double temp_mod = (double)k - 20.0;
		// we do not need R as the internal Ea is in Kelvins
		Ea_for_gradient[k] = -fitted_Ea/((temperature + temp_mod));
		//lnT_for_gradient[k] = log(temperature + temp_mod);
		Group_k[k] = log(Group_k[k]);
	}


	// ln(k) = ln(A) + ln(T^n) + (-Ea/T) || Ea in Kelvin (else -Ea/RT)


	// Individual step would only be needed if the function is no monotonous...
	//*
	double gradient_k = 0;
	double gradient_Ea = 0;
	for(size_t k=1;k<41;k++){
		gradient_k = gradient_k +
				(
						(Group_k[k-1]-Group_k[k])
						// /(temperature-1-temperature) // 1K Steps, so we save us the effort
				);
		gradient_Ea = gradient_Ea +
				(
						(Ea_for_gradient[k-1]-Ea_for_gradient[k])
						// /(temperature-1-temperature) // 1K Steps, so we save us the effort
				);
	}
	gradient_k = gradient_k/40; //40 points I think...
	gradient_Ea = gradient_Ea/40; //40 points I think...
	//*/

	// Work out Ea component of gradient
	//double gradient_in_Group_k, gradient_in_Ea, gradient_in_lnT;

	//gradient_in_Group_k = gradient_k;//(Group_k[41] - Group_k[0])/40;
	//gradient_in_Ea = (Ea_for_gradient[41] - Ea_for_gradient[0])/40;
	//gradient_in_lnT = (lnT_for_gradient[41] - lnT_for_gradient[0])/40;

	/*
	double difference_in_gradient, fitted_n;
	difference_in_gradient = gradient_in_Group_k - gradient_in_Ea;
	fitted_n = difference_in_gradient / gradient_in_lnT;//*/

	double difference_in_gradient = 0;
	double fitted_n;
	//difference_in_gradient = gradient_in_Group_k - gradient_in_Ea;
	// We can calculate our fitted n now:
	fitted_n = exp(difference_in_gradient);

	// now for the fitted A, using the middle point
	//double ln_fitted_A, fitted_A;
	double fitted_A;

	//ln_fitted_A = Group_k[1] - fitted_n*log(temperature) - (-fitted_Ea/(temperature));
	//fitted_A = exp(ln_fitted_A);

	// calculate A on the centerpoint
	fitted_A = Group_k[20] / (pow(temperature,fitted_n)*exp(-fitted_Ea/temperature));


	ParameterOutput.Reversible = false;
	ParameterOutput.paramA = fitted_A;
	ParameterOutput.paramN = fitted_n;
	ParameterOutput.paramEa = fitted_Ea;

	return ParameterOutput;
}
