/*
 * Run_Integrator.cpp
 *
 *  Created on: 20.10.2017
 *      Author: DetlevCM
 */

#include "./run_integrator.h"

/*
 * The more solvers and models are implemented, the messier the code becomes...
 * To keep things tidy, a dedicated function chooses the integrator
 */

void RunIntegrator::Choose_Integrator(
    Filenames OutputFilenames, ReactionMechanism reaction_mechanism,
    Initial_Data InitialParameters, vector<double> &KeyRates,
    PressureVesselCalc PetroOxyDataInitial,
    vector<vector<str_RatesAnalysis>> &RatesAnalysisData) {

  if (InitialParameters.Solver_Parameters.SolverType == 0) {
    cout << "Using Intel ODE\n" << std::flush;
  } else if (InitialParameters.Solver_Parameters.SolverType == 1) {
    cout << "Using odepack\n" << std::flush;
  } else {
    cout << "Error, solver undefined or not available.\n";
    exit(1); // exit with error
  }

  // if we don' exit, send ot the integrator:
  Integrate_Liquid_Phase(OutputFilenames,
                         InitialParameters.InitialSpeciesConcentration,
                         reaction_mechanism, InitialParameters, KeyRates,
                         PetroOxyDataInitial, RatesAnalysisData);
}
