/*
 * Run_Integrator.h
 *
 *  Created on: 20 Oct 2017
 *      Author: detlevcm
 */

#ifndef RUN_INTEGRATOR
#define RUN_INTEGRATOR

#include <vector>
using std::vector;

#include "../global_struct.h"

#include "../main.h"

#include "../get_process_input/get_initial_data/Initial_Data.h"

#include "../get_process_input/get_mechanism/ReactionMechanism.h"
#include "../get_process_input/get_mechanism/Species.h"

#include "../get_process_input/mechanism_reduction/Mechanism_Reduction.h"

#include "../rates_analysis/Rates-Analysis.h"

#include "../write_output/write_output.h"

#include "../../lib/headers/lib_Intel_ODE.hpp"
#include "../../lib/headers/lib_odepack.hpp"
#include "./solver_calculations/solver_calculations.h"

class RunIntegrator {

public:
  struct Settings_LSODA {
    int solver_subsettings;

    int JT;
    // LSODA specific settings
    int LRW, LIW;
    int ITOL = 1;
    int ITASK = 1;
    int ISTATE = 1;
    int IOPT = 0;

    // general solver settings
    double RTOL, ATOL;

    // some vectors for LSODA
    vector<int> vector_IWORK;
    vector<double> vector_RWORK;
  };

  struct Settings_Intel {
    int ierr, dpar_size;
    double h, hm, ep, tr;

    int solver_subsettings;

    vector<int> vector_ipar;
    vector<int> vector_kd;
    vector<double> vector_dpar;
    // set the solver:
    Initial_Data::solver_type Solver_Type;
  };

  // Function to pick the right integration routine
  static void Choose_Integrator(Filenames, ReactionMechanism, Initial_Data,
                                vector<double> &, PressureVesselCalc,
                                vector<vector<str_RatesAnalysis>> &);

  static int Prepare_Integrator_Settings(Initial_Data, size_t, Settings_LSODA &,
                                         Settings_Intel &);

  static vector<double> Get_Delta_N(const vector<SingleReactionData> Reactions);

  // solve based on concentrations in the liquid phase
  static void Integrate_Liquid_Phase(
      Filenames OutputFilenames, vector<double> SpeciesConcentration,
      ReactionMechanism reaction_mechanism, Initial_Data InitialParameters,
      vector<double> &KeyRates, PressureVesselCalc PetroOxyDataInput,
      vector<vector<str_RatesAnalysis>> &RatesAnalysisData);

  //// Pre-Processing:

  static vector<ReactionParameter>
  Process_Reaction_Parameters(const vector<SingleReactionData> &);

  static vector<TrackSpecies>
  Reactants_ForReactionRate(const vector<SingleReactionData> &);

  static vector<TrackSpecies>
  Products_ForReactionRate(const vector<SingleReactionData> &, bool);

  static vector<TrackSpecies>
  PrepareSpecies_ForSpeciesLoss(const vector<SingleReactionData> &);
};

#endif
