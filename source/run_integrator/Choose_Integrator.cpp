/*
 * Run_Integrator.cpp
 *
 *  Created on: 20.10.2017
 *      Author: DetlevCM
 */

#include <Headers.hpp>

/*
 * The more solvers and models are implemented, the messier the code becomes...
 * To keep things tidy, a dedicated function chooses the integrator
 */



void Choose_Integrator(
		Filenames OutputFilenames,
		Reaction_Mechanism reaction_mechanism,
		Initial_Data InitialParameters,
		vector< double >& KeyRates,
		PressureVesselCalc PetroOxyDataInitial,
		vector< vector < str_RatesAnalysis > >& RatesAnalysisData
)
{
	if(InitialParameters.Solver_Parameters.SolverType == 0)
	{
		cout << "Using Intel ODE\n" << std::flush;
		// it is the pressure vessel case
		if(InitialParameters.PetroOxy.IsSet)
		{
			Integrate_Pressure_Vessel_Liquid_Phase_Intel(
					OutputFilenames,
					InitialParameters.InitialSpeciesConcentration,
					reaction_mechanism,
					InitialParameters,
					KeyRates,
					PetroOxyDataInitial,
					RatesAnalysisData
			);
		}
		else // it isn't a pressure vessel, i.e. PetroOxy
		{
			//Integrate_Liquid_Phase_Intel(
			Integrate_Liquid_Phase(
					OutputFilenames,
					InitialParameters.InitialSpeciesConcentration,
					reaction_mechanism,
					InitialParameters,
					KeyRates,
					RatesAnalysisData
			);
		}
	}
	else if(InitialParameters.Solver_Parameters.SolverType == 1)
	{
		cout << "Using odepack\n" << std::flush;
		// it is the pressure vessel case
		if(InitialParameters.PetroOxy.IsSet)
		{
			Integrate_Pressure_Vessel_Liquid_Phase_Odepack_LSODA(
					OutputFilenames,
					InitialParameters.InitialSpeciesConcentration,
					reaction_mechanism,
					InitialParameters,
					KeyRates,
					PetroOxyDataInitial,
					RatesAnalysisData
			);
		}
		else // it isn't a pressure vessel, i.e. PetroOxy
		{
			//Integrate_Liquid_Phase_Odepack_LSODA(
			Integrate_Liquid_Phase(
					OutputFilenames,
					InitialParameters.InitialSpeciesConcentration,
					reaction_mechanism,
					InitialParameters,
					KeyRates,
					RatesAnalysisData
			);
		}
	}
	else
	{
		cout << "Error, solver undefined or not available.\n";
	}


}
