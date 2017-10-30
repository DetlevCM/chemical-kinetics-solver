/*
 * Run_Integrator.cpp
 *
 *  Created on: 20.10.2017
 *      Author: DetlevCM
 */

#include "MyHeaders.h"

/*
 * The more solvers and models are implemented, the messier the code becomes...
 * To keep things tidy, a dedicated function chooses the integrator
 */



void Choose_Integrator(
		Filenames OutputFilenames,
		vector< double > InitialSpeciesConcentration,
		vector< string > Species,
		vector< ThermodynamicData > Thermodynamics,
		vector< SingleReactionData >& Reactions,
		InitParam InitialParameters,
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
							InitialSpeciesConcentration,
							Species,Thermodynamics,Reactions,
							InitialParameters,
							KeyRates,
							PetroOxyDataInitial,
							RatesAnalysisData
					);
				}
				else // it isn't a pressure vessel, i.e. PetroOxy
				{
					Integrate_Liquid_Phase_Intel(
							OutputFilenames,
							InitialSpeciesConcentration,
							Species,Thermodynamics,Reactions,
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
							InitialSpeciesConcentration,
							Species,Thermodynamics,Reactions,
							InitialParameters,
							KeyRates,
							PetroOxyDataInitial,
							RatesAnalysisData
					);
				}
				else // it isn't a pressure vessel, i.e. PetroOxy
				{
					Integrate_Liquid_Phase_Odepack_LSODA(
							OutputFilenames,
							InitialSpeciesConcentration,
							Species,Thermodynamics,Reactions,
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
