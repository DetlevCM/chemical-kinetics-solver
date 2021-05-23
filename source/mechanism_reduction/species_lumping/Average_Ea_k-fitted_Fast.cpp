/*
 * Average_Ea_CalculateNewParametersFast.cpp
 *
 *  Created on: 17.01.2016
 *      Author: DetlevCM
 */

#include "../headers/Headers.hpp"


ReactionParameter Average_Ea_CalculateNewParametersFast(
		vector< SingleReactionData >& Reactions,
		double temperature,
		size_t Reaction_Group_Size
)
{

	ReactionParameter ParameterOutput;
	size_t j;

	vector <double> Group_k(3);
	double total_Ea = 0.0;

	double temperature_endpoints = 20.0;

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
				exp(-Reactions[j].paramEa/(temperature));

		// upper temperature point
		Group_k[2] =
				Group_k[2] +
				Reactions[j].paramA *
				pow((temperature + temperature_endpoints),Reactions[j].paramN) *
				exp(-Reactions[j].paramEa/(temperature + temperature_endpoints));

		// Total Ea to get an average Ea
		total_Ea = total_Ea + Reactions[j].paramEa;
	}


	/*
	 * Our Ea is an average of the existing Ea to obtain a realistic value
	 */
	double fitted_Ea;
	fitted_Ea = total_Ea/( (double) Reaction_Group_Size);

	/*
	 * We have an -Ea/T component and an n*ln(T) component in the gradient
	 * With Ea in Kelvins there is no R, else it would be -Ea/(RT)
	 */

	vector <double> Ea_for_gradient(3);
	vector <double> lnT_for_gradient(3);

	// lower temperature point
	Ea_for_gradient[0] = -fitted_Ea/(temperature - temperature_endpoints);
	lnT_for_gradient[0] = log(temperature - temperature_endpoints);
	Group_k[0] = log(Group_k[0]);

	// middle temperature point
	Ea_for_gradient[1] = -fitted_Ea/(temperature);
	lnT_for_gradient[1] = log(temperature);
	Group_k[1] = log(Group_k[1]);

	// upper temperature point
	Ea_for_gradient[2] = -fitted_Ea/(temperature + temperature_endpoints);
	lnT_for_gradient[2] = log(temperature + temperature_endpoints);
	Group_k[2] = log(Group_k[2]);


	// Work out Ea component of gradient
	double gradient_in_Group_k, gradient_in_Ea, gradient_in_lnT ;

	gradient_in_Group_k = 0.5 *(
			(Group_k[1] - Group_k[0]) / (temperature_endpoints) +
			(Group_k[2] - Group_k[1]) / (temperature_endpoints) );

	gradient_in_Ea = 0.5 * (
			(Ea_for_gradient[1] - Ea_for_gradient[0]) / (temperature_endpoints) +
			(Ea_for_gradient[2] - Ea_for_gradient[1]) / (temperature_endpoints) );

	gradient_in_lnT = 0.5 * (
			(lnT_for_gradient[1] - lnT_for_gradient[0]) / (temperature_endpoints) +
			(lnT_for_gradient[2] - lnT_for_gradient[1]) / (temperature_endpoints) );


	double difference_in_gradient, fitted_n;
	difference_in_gradient = (gradient_in_Group_k - gradient_in_Ea) / gradient_in_lnT ;


	// We can calculate our fitted n now:
	fitted_n = exp(difference_in_gradient);

	cout << fitted_Ea * 1.987192 / 1000 << " " << gradient_in_Group_k << " " << gradient_in_Ea << " " << difference_in_gradient << " " << fitted_n << "\n";

	// now for the fitted A, using the middle point
	//double ln_fitted_A, fitted_A;
	double fitted_A;

	//ln_fitted_A = Group_k[1] - fitted_n*log(temperature) - (-fitted_Ea/(temperature));
	//fitted_A = exp(ln_fitted_A);

	// calculate A on the centerpoint
	//cout << fitted_Ea * 1.987192 / 1000 << "\n";
	double denominator = pow(temperature,fitted_n)*exp(-fitted_Ea/temperature);
	fitted_A = Group_k[1] / denominator;
	//cout << fitted_A << "\n";

	ParameterOutput.Reversible = false; // reactions must be irreversible for lumping, they will come out irreversible
	ParameterOutput.paramA = fitted_A;
	ParameterOutput.paramN = fitted_n;
	ParameterOutput.paramEa = fitted_Ea;

	return ParameterOutput;
}




