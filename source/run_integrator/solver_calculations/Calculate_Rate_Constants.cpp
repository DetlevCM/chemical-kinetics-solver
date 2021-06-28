/*
 * Calculate_Rate_Constants.cpp
 *
 *  Created on: 19.12.2012
 *      Author: DetlevCM
 */

#include "../headers/Headers.hpp"


void Calculate_Rate_Constant(
		vector< double >& Kf,
		vector< double >& Kr,
		const double Temperature,
		const vector<double>& Concentrations,
		const vector< ReactionParameter >& ReactionParameters,
		const vector< CalculatedThermodynamics >& CalculatedThermo,
		const vector< TrackSpecies >& SpeciesAll,
		const vector< double >& Delta_N
)

{

	/* 2002 CODATA values */
	double R = 8.314472e0; // J mol-1 K-1
	//double Na = 6.0221415e23;

	size_t Number_Reactions = ReactionParameters.size();

	// Pressure Independent Reactions Only
	/*
	k[i] = a1[i]*exp(-e1[i]*inv_T);  // kinf calculation
	if (n1[i] != 0.0)
	k[i] *= pow(T, n1[i]);
	//*/

	size_t i;

	//Define vectors in the right size right away
	vector< double > delta_H(Number_Reactions);
	vector< double > delta_S(Number_Reactions);
	vector< double > delta_G(Number_Reactions);





	// Worked out per reaction
	// as we have the value of the calculated thermodynamics, we just need to put them together
	// per reaction, going through every species
	double third_body_sum = 0.0;
	for(i = 0; i<Concentrations.size() - 1; i++) // last position is temperature
	{
		third_body_sum = third_body_sum + Concentrations[i];
	}

	for(i = 0;i< SpeciesAll.size();i++){ // loop over every reaction
		delta_H[SpeciesAll[i].ReactionID] =
				delta_H[SpeciesAll[i].ReactionID] +
				(CalculatedThermo[SpeciesAll[i].SpeciesID].Hf * SpeciesAll[i].coefficient);

		delta_S[SpeciesAll[i].ReactionID] =
				delta_S[SpeciesAll[i].ReactionID] +
				(CalculatedThermo[SpeciesAll[i].SpeciesID].S * SpeciesAll[i].coefficient);

	}




	for(i=0;i<Number_Reactions;i++) // Straightforward Arrhenius Expression/Equation
	{
		Kf[i] = ReactionParameters[i].paramA *
				exp(-ReactionParameters[i].paramEa/Temperature); // do NOT forget the - !!!

		//* Speedup by only raising temperature to power where needed: improvement is large :)
		if(ReactionParameters[i].paramN != 0) // raising to power 0 has no effect, so only if not 0
		{
			// unsure if this check really gives a performance improvement...
			// maybe it used to and no longer does with a modern compiler/processor/kernel
			//if(ReactionParameters[i].paramN != 1)
			//{
			Kf[i] = Kf[i] * pow(Temperature,ReactionParameters[i].paramN);
			/*}
			else
			{
				Kf[i] = Kf[i] * Temperature; // raise temp^1 = temp
			}//*/
		}

		/*
		cout <<
		//		Temperature << " , " <<
		//		exp(-ReactionParameters[i].paramEa/Temperature) << " , " <<
				ReactionParameters[i].paramA << " , " <<
				ReactionParameters[i].paramN << " , " <<
				ReactionParameters[i].paramEa << " , " <<
				Kf[i] << "\n";//*/



		// all previous logic in the code applies if there is no third body
		//bool ThirdBody = false;

		//*

		// Note: third_body_sum is in mol/L at this point

		//Calculate_Lindeman_Hinshelwood_SRI_Low();
		if(ReactionParameters[i].ThirdBodyType == 1)
		{
			Kf[i] = Calculate_no_LOW_Troe(
					ReactionParameters[i],
					Concentrations,
					Temperature,
					third_body_sum
			);
		}
		else if(ReactionParameters[i].ThirdBodyType == 2 && ReactionParameters[i].troeThirdBody.has_troe == false)
		{
			Kf[i] = Calculate_Lindeman_Hinshelwood_SRI(
					ReactionParameters[i],
										Concentrations,
										Temperature,
										third_body_sum
								);
		}
		else if(ReactionParameters[i].ThirdBodyType == 2 && ReactionParameters[i].troeThirdBody.has_troe)
				{
			Kf[i] = Calculate_Lindeman_Hinshelwood_Low_Troe(
					ReactionParameters[i],
					Concentrations,
					Temperature,
					third_body_sum
			);
		}
		//*/




		// default, change if reversible - seems a little bit faster...
		Kr[i] = 0;
		// then calculate and set the reverse if required
		if(ReactionParameters[i].Reversible && ReactionParameters[i].explicit_rev == false)
		{
			double  temp_kp, temp_kc;
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
		else if(ReactionParameters[i].Reversible && ReactionParameters[i].explicit_rev == true)
		{
			// can explicit reversible reactions also be suject to third body terms? Not supported here...

			Kr[i] = ReactionParameters[i].rev_paramA *
							exp(-ReactionParameters[i].rev_paramEa/Temperature); // do NOT forget the - !!!

					//* Speedup by only raising temperature to power where needed: improvement is large :)
					if(ReactionParameters[i].rev_paramN != 0) // raising to power 0 has no effect, so only if not 0
					{
						// unsure if this check really gives a performance improvement...
						// maybe it used to and no longer does with a modern compiler/processor/kernel
						Kr[i] = Kr[i] * pow(Temperature,ReactionParameters[i].rev_paramN);
					}
		}

		// if it is set to zero at the start and not touched for irreversible reactions thus this is redundant
		// Fewer memory allocations should speed things up - or not?
		/*
				else
				{
					Kr[i] = 0;
				}//*/
	}


}
