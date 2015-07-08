/*
 * ODE_RHS_Liquid_Phase.cpp
 *
 *  Created on: 06.07.2015
 *      Author: DetlevCM
 */

#include <MyHeaders.h>


void ODE_RHS_Liquid_Phase(int*n, double*time_current, double*y, double*f)
{
	// A namespace allows global variables without causing a mess, should be quicker than redefining too
	using namespace ODESolverConstant;
	using namespace ODESolverVariable;

	int i;


	/* 2002 CODATA values */
	//double R = 8.314472e0;
	//double Na = 6.0221415e23;

	// stability hack
	//if(GlobalArrays::InitialParameters.EnforceStability)
	if(InitialDataConstants.EnforceStability)
	{
		for (i = 0; i <= Number_Species; i++)
		{
			if(y[i]<0){
				//if(y[i]<1.e-24){
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

	double time_difference;


	//if(GlobalArrays::InitialParameters.PetroOxy)
	if(InitialDataConstants.PetroOxy)
	{
		time_difference = fabs(*time_current - time_previous);
		AdjustGasConcentration(
				y[OxyGasSpeciesID],
				Concentration[Number_Species],
				time_difference,
				PetroOxyData);
	}

	// Thermodynamic data, Rate Constant, Rates, new Concentrations
	Calculate_Thermodynamics(CalculatedThermo, Concentration[Number_Species], Thermodynamics);
	Calculate_Rate_Constant(Kf, Kr, Concentration[Number_Species],ReactionParameters, CalculatedThermo, SpeciesLossAll, Delta_N);
	CalculateReactionRates(Rates, Concentration, Kf, Kr, ReactantsForReactions, ProductsForReactions);
	SpeciesConcentrationChange = SpeciesLossRate(SpeciesLossAll,Number_Species, Rates);


	double ctot=0;
	double qint=0;
	double qtot=0;


	for (i = 0; i < Number_Species; i++)
	{
		ctot = ctot + CalculatedThermo[i].Cv * Concentration[i];
	}
	// ctot = ctot / 1000; // working in moles/l so no Na;

	for (i = 0; i < Number_Reactions; i++)
	{
		qint = qint + Delta_N[i] * Rates[i];
	}
	qtot = -qint / (ctot);//*1000); // scale l to ml and Na not needed for moles/l * Na); //*/

	// Checked f[i] -> no unexpected rates of change for "inert gases", all 0.
	for (i = 0; i < Number_Species; i++)
	{
		f[i] = SpeciesConcentrationChange[i]; // Species equations //
	}
	f[Number_Species] = qtot; // Temperature equation //

	//*
	if(
			Concentration[Number_Species] >= InitialDataConstants.temperature
			&&
			InitialDataConstants.PetroOxy) // fix temperature for Oxy
			{
			y[Number_Species] = InitialDataConstants.temperature; // ensure temperature is not exceeded
			InitialDataConstants.PetroOxyTemperatureRise = 0;

			f[Number_Species] = 0;
	}//*/

	if(
			InitialDataConstants.PetroOxy
			&&
			InitialDataConstants.PetroOxyTemperatureRise != 0) // fix temperature for Oxy, rise desired
	{
		// 298K starting temp, GlobalArrays::InitialParameters.temperature is target
		// rise time given in minutes
		f[Number_Species] =
				(InitialDataConstants.temperature - 298)
				/
				(InitialDataConstants.PetroOxyTemperatureRise);

		//std::cout << f[Number_Species] << "\n";
	}//*/

	// IEEE standard hack to check for NaN
	// if temperature blows up, freeze it
	if(qtot != qtot)
	{
		f[Number_Species] = 0;
	}


	// For Oxy limiting
	time_previous = *time_current;
}






