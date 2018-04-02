/*
 * ODE_RHS_Liquid_Phase.cpp
 *
 *  Created on: 06.07.2015
 *      Author: DetlevCM
 */

#include <MyHeaders.h>


void ODE_RHS_Liquid_Phase(int*n, double*t, double*y, double*f)
{
	// A namespace allows global variables without causing a mess, should be quicker than redefining too
	using namespace ODE_RHS;
	using namespace Jacobian_ODE_RHS;

	int i;

	// stability hack - but has a performance impact...
	if(InitialDataConstants.EnforceStability)
	{
		for (i = 0; i <= Number_Species; i++)
		{
			if(y[i]<0){
				Concentration[i] = 0;
			}
			else
			{
				Concentration[i] = y[i];
			}
		}
	}
	else{
		for (i = 0; i <= Number_Species; i++)
		{
			Concentration[i] = y[i];
		}
	}

	Evaluate_Thermodynamic_Parameters(CalculatedThermo, Thermodynamics, Concentration[Number_Species]);
	Calculate_Rate_Constant(Kf, Kr, Concentration[Number_Species],ReactionParameters, CalculatedThermo, SpeciesLossAll, Delta_N);
	CalculateReactionRates(Rates, Concentration, Kf, Kr, ReactantsForReactions, ProductsForReactions);
	SpeciesConcentrationChange = SpeciesLossRate(Number_Species, Rates, SpeciesLossAll);


	double ctot=0;
	double qint=0;
	double qtot=0;


	for (i = 0; i < Number_Species; i++)
	{
		ctot = ctot + CalculatedThermo[i].Cv * Concentration[i];
		// reduce number of loops
		f[i] = SpeciesConcentrationChange[i];
	}
	// ctot = ctot / 1000; // working in moles/l so no Na;

	for (i = 0; i < Number_Reactions; i++)
	{
		qint = qint + Delta_N[i] * Rates[i];
	}
	qtot = -qint / (ctot);//*1000); // scale l to ml and Na not needed for moles/l * Na); //*/

	f[Number_Species] = qtot; // Temperature equation
	//cout << ctot << " " << qint << " " << qtot << "\n";


	// Settings relevant rates to zero
	if(InitialDataConstants.ConstantConcentration)
	{
		//cout << "preparing constant species \n";
		for(i=0;i<Number_Species;i++)
		{
			if(InitialDataConstants.ConstantSpecies[i] != 0){
				f[i] = 0 ; // concentration reset
				//cout << "f[" << i << "] = " << f[i];
			}
		}
	}

	if(InitialDataConstants.TGA_used)
	{
		if(Concentration[Number_Species] < InitialDataConstants.TGA_target)
		{
			f[Number_Species] = InitialDataConstants.TGA_rate;
		}
		else if(Concentration[Number_Species] >= InitialDataConstants.TGA_target)
		{
			f[Number_Species] = 0;
		}
	}

	// IEEE standard hack to check for NaN
	// if temperature blows up, freeze it
	//*
	if(qtot != qtot)
	{
		f[Number_Species] = 0;
	}
	//*/

}






