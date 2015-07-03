/*
 * Calculate_Rate_Constants.cpp
 *
 *  Created on: 11 Mar 2014
 *      Author: DetlevCM
 */




/*
 * Calculate_Rate_Constants.cpp
 *
 *  Created on: 19.12.2012
 *      Author: DetlevCM
 */

#include <MyHeaders.h>


void Calculate_Rate_Constant(
		vector< double >& Kf,
		vector< double >& Kr,
		const double Temperature,
		const vector< ReactionParameter >& ReactionParameters,
		const vector< vector< double > >& CalculatedThermo,
		const vector< TrackSpecies >& SpeciesAll,
		const vector< double >& Delta_N
)

{

	/* 2002 CODATA values */
	double R = 8.314472e0;
	//double Na = 6.0221415e23;

	int Number_Reactions = ReactionParameters.size();

	// Pressure Independent Reactions Only
	/*
	k[i] = a1[i]*exp(-e1[i]*inv_T);  // kinf calculation
	if (n1[i] != 0.0)
	k[i] *= pow(T, n1[i]);
	//*/

	int i;

	//Define vectors in the right size right away
	vector< double > delta_H(Number_Reactions);
	vector< double > delta_S(Number_Reactions);
	vector< double > delta_G(Number_Reactions);

	double  temp_kp, temp_kc;



	// Worked out per reaction
	// as we have the value of the calculated thermodynamics, we just need to put them together
	// per reaction, going through every species

	for(i = 0;i<(int) SpeciesAll.size();i++){ // loop over every reaction

		delta_H[SpeciesAll[i].ReactionID] =
				delta_H[SpeciesAll[i].ReactionID] +
				CalculatedThermo[SpeciesAll[i].SpeciesID][0] *
				SpeciesAll[i].coefficient;

		delta_S[SpeciesAll[i].ReactionID] =
				delta_S[SpeciesAll[i].ReactionID] +
				CalculatedThermo[SpeciesAll[i].SpeciesID][3] *
				SpeciesAll[i].coefficient;
	}




	for(i=0;i<Number_Reactions;i++) // Straightforward Arrhenius Expression/Equation
	{
		Kf[i] = ReactionParameters[i].paramA *
				exp(-ReactionParameters[i].paramEa/Temperature); // do NOT forget the - !!!

		//* Speedup by only raising temperature to power where needed: improvement is large :)
		if(ReactionParameters[i].paramN != 0) // raising to power 0 has no effect, so only if not 0
		{
			if(ReactionParameters[i].paramN != 1)
			{
				Kf[i] = Kf[i] * pow(Temperature,ReactionParameters[i].paramN);
			}
			else
			{
				Kf[i] = Kf[i] * Temperature; // raise temp^1 = temp
			}
		}
		//cout << ReactionParameter[i].A << " , " << ReactionParameter[i].N << " , " << ReactionParameter[i].Ea << " , " << Kf[i] << "\n";


		// unbundle into two if statements
		if(ReactionParameters[i].Reversible)
		{
			delta_G[i] = delta_H[i] - Temperature*delta_S[i];
			temp_kp = exp(-delta_G[i]/(R*Temperature));

			if(Delta_N[i] == 0) // an if check is less expensive than a pow(,)
			{
			Kr[i] = Kf[i]/temp_kp;
			}
			else
			{
				temp_kc = temp_kp*pow((0.0820578*Temperature),(-Delta_N[i])); // L atm K^(-1) mol^(-1)
				//temp_kc = temp_kp*pow((1.3624659e-22*Temperature),(-Delta_N[i])); // for molecules cm^(-3)
				Kr[i] = Kf[i]/temp_kc;
			}

			// IEEE standards hack to avoid Nan Rate, shouldn't exist in the first place...
			/*if(Kr[i] != Kr[i])
			{
				Kr[i] = 0;
			}//*/
		}
		// if it is set to zero at the start and not touched for irreversible reactions thus this is redundant
		// Fewer memory allocations should speed things up
		//*
		else
		{
			Kr[i] = 0;
		}//*/
	}


}
