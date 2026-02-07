/*
 * liquid_phase_odepack.cpp
 *
 *  Created on: 02.07.2015
 *      Author: DetlevCM
 */

#include <chrono>

#include "./run_integrator.h"
#include "./write_output/write_output.h"

void RunIntegrator::Integrate(
    ReactionMechanism reaction_mechanism, Initial_Data InitialParameters,
    vector<double> &KeyRates, PressureVesselCalc PetroOxyDataInput,
    vector<vector<str_RatesAnalysis>> &RatesAnalysisData) {

  if (InitialParameters.Solver_Parameters.SolverType == 0) {
    cout << "Using Intel ODE\n" << std::flush;
  } else if (InitialParameters.Solver_Parameters.SolverType == 1) {
    cout << "Using odepack\n" << std::flush;
  } else {
    cout << "Error, solver undefined or not available.\n";
    exit(1); // exit with error
  }

  vector<TrackSpecies> ProductsForRatesAnalysis;

  size_t Number_Species = reaction_mechanism.species.size();
  size_t Number_Reactions = reaction_mechanism.reactions_size();

  // Set the number of threads to use:
  // Threading has limited use in small mechanisms.
  omp_set_num_threads(InitialParameters.Solver_Parameters.threads);

  // outputting mechanism size in integration routing so that it is printed
  // every time
  cout << "The mechanism to be integrated contains " << Number_Species
       << " species and " << Number_Reactions << " Reactions.\n"
       << std::flush;

  write_output.open_stream_concentrations();

  if (InitialParameters.PrintReacRates) {
    write_output.open_stream_rates();
  }

  Settings_LSODA LSODA;
  Settings_Intel Intel;

  // general variables for the ODE solver
  int n;
  // intel ODE wants int not size_t
  n = (int)Number_Species + 1;

  // this function will prepare the required settings. Only the required class
  // is updated.
  int solver_choice = 0;
  solver_choice = Prepare_Integrator_Settings(InitialParameters, Number_Species,
                                              LSODA, Intel);

  // just make sure the Delta_N is current
  vector<double> prep_delta_n = Get_Delta_N(reaction_mechanism.reactions);
  // Reduce the matrix from a sparse matrix to something more manageable and
  // quicker to use

  vector<TrackSpecies> ReactantsForReactions =
      Reactants_ForReactionRate(reaction_mechanism.reactions);

  vector<TrackSpecies> ProductsForReactions =
      Products_ForReactionRate(reaction_mechanism.reactions, false);

  if (InitialParameters.MechanismAnalysis.MaximumRates ||
      InitialParameters.MechanismAnalysis.StreamRatesAnalysis ||
      InitialParameters.MechanismAnalysis.RatesAnalysisAtTime ||
      InitialParameters.MechanismAnalysis.RatesOfSpecies) {
    ProductsForRatesAnalysis =
        Products_ForReactionRate(reaction_mechanism.reactions, true);
  }

  vector<TrackSpecies> SpeciesLossAll = PrepareSpecies_ForSpeciesLoss(
      reaction_mechanism.reactions); // New method of listing species

  //// NOTE: Here initialises the Solver Calc class, in which the members act as
  //// global variables / for and during the calculation
  solver_calculation.init(
      reaction_mechanism.species, reaction_mechanism.species.size(),
      reaction_mechanism.reactions_size(), ReactantsForReactions,
      ProductsForReactions, SpeciesLossAll, prep_delta_n);

  solver_calculation.ReactionParameters =
      Process_Reaction_Parameters(reaction_mechanism.reactions);

  solver_calculation.Concentration =
      InitialParameters.InitialSpeciesConcentration;

  double *y = solver_calculation.Concentration.data();

  double time_current, time_step, time_step1, time_end;

  time_current = 0; // -> Solver is designed for t_0 = 0
  time_step1 = InitialParameters.TimeStep[0];
  time_end = InitialParameters.TimeEnd[0];
  size_t TimeChanges = InitialParameters.TimeStep.size();
  size_t tracker = 0;

  // cout << "\nEnd Time: " << time_end << " Time Step: " << time_step1 << "\n";

  /* -- Initial values at t = 0 -- */

  solver_calculation.InitialDataConstants.EnforceStability =
      InitialParameters.EnforceStability;
  solver_calculation.InitialDataConstants.temperature =
      InitialParameters.temperature;

  // in case we want to simulate a TGA
  if (InitialParameters.TGA_used) {
    solver_calculation.InitialDataConstants.TGA_used =
        InitialParameters.TGA_used;
    solver_calculation.InitialDataConstants.TGA_rate =
        InitialParameters.TGA_rate;
    solver_calculation.InitialDataConstants.TGA_target =
        InitialParameters.TGA_target;
  }

  // Is the PetroOxy pressure vessel option requested?
  if (InitialParameters.PetroOxy.IsSet) {
    solver_calculation.InitialDataConstants.EnforceStability =
        InitialParameters.EnforceStability;
    solver_calculation.InitialDataConstants.PetroOxy =
        InitialParameters.PetroOxy.IsSet;
    solver_calculation.InitialDataConstants.PetroOxyTemperatureRise =
        InitialParameters.PetroOxy.TemperatureRise;
  }

  // set constant concentration if desired
  solver_calculation.InitialDataConstants.ConstantConcentration =
      InitialParameters.ConstantConcentration;
  if (InitialParameters.ConstantConcentration) {
    cout << "Constant Species desired\n";

    solver_calculation.InitialDataConstants.ConstantSpecies.clear();
    solver_calculation.InitialDataConstants.ConstantSpecies.resize(
        Number_Species);

    // zero just to make sure
    for (size_t i = 0; i < Number_Species; i++) {
      solver_calculation.InitialDataConstants.ConstantSpecies[i] = 0;
    }

    for (size_t i = 0; i < InitialParameters.ConstantSpecies.size();
         i++) { // fix initial concentrations
      solver_calculation.InitialDataConstants
          .ConstantSpecies[InitialParameters.ConstantSpecies[i]] =
          solver_calculation
              .Concentration[InitialParameters.ConstantSpecies[i]];
    }
  }

  if (InitialParameters.PetroOxy.IsSet) {
    SolverCalculation::PetroOxyOutputHeader(write_output.get_name_petrooxy());
    solver_calculation.OxyGasSpeciesID = InitialParameters.PetroOxy.GasSpecies;
    solver_calculation.PetroOxyData = PetroOxyDataInput;
  }
  // Oxy with temperature Rise
  if (InitialParameters.PetroOxy.IsSet &&
      InitialParameters.PetroOxy.TemperatureRise !=
          0) // fix temperature for Oxy, rise desired
  {
    solver_calculation.Concentration[Number_Species] = 298;
    // for Oxy diffusion limit, gets ignored if not required
    solver_calculation.time_previous = 0;
  } //*/

  solver_calculation.Evaluate_Thermodynamic_Parameters(
      solver_calculation.Concentration[Number_Species]);

  // prepare the Jacobian matrix
  solver_calculation.Prepare_Jacobian_Matrix(solver_calculation.JacobianMatrix,
                                             reaction_mechanism.reactions);

  // Get the rate Constants, forward and backwards
  solver_calculation.Calculate_Rate_Constant(
      solver_calculation.Concentration[Number_Species]);

  solver_calculation.CalculateReactionRates(solver_calculation.Concentration,
                                            solver_calculation.Get_Kf(),
                                            solver_calculation.Get_Kr());
  // Don't forget Rates Analysis for Mechanism Reduction at t=0 - or is this
  // nonsense?
  if (InitialParameters.MechanismReduction.ReduceReactions != 0) {
    MechanismReduction::ReactionRateImportance(
        KeyRates, solver_calculation.Get_Rates(),
        InitialParameters.MechanismReduction.ReduceReactions);
  }

  if (InitialParameters.PetroOxy.IsSet) {

    //  the PetroOxy will saturate the hydrocarbon with oxygen
    // at no loss to the reservoir
    solver_calculation.Adjust_Gas_Concentration_Initial(
        solver_calculation.Concentration[solver_calculation.OxyGasSpeciesID],
        solver_calculation.Concentration[Number_Species],
        solver_calculation.PetroOxyData);

    SolverCalculation::PetroOxyOutputStream(write_output.get_name_petrooxy(),
                                            solver_calculation.PetroOxyData,
                                            time_current);
  }

  // do not forget output at time = 0
  write_output.StreamData(time_current, InitialParameters.GasPhase,
                          InitialParameters.GasPhasePressure,
                          solver_calculation.Concentration,
                          solver_calculation.Get_Rates());

  // not happy with this more widely available, needs a cleanup...
  vector<vector<size_t>> ReactionsForSpeciesSelectedForRates;
  // Not the best place to put it, but OK for now:
  if (InitialParameters.MechanismAnalysis.RatesOfSpecies) {
    size_t tempi, tempj;
    vector<vector<size_t>> TempMatrix;
    vector<size_t> TempRow;
    size_t Temp_Number_Species = reaction_mechanism.species.size();
    for (tempi = 0; tempi < reaction_mechanism.reactions_size(); tempi++) {
      TempRow.resize(reaction_mechanism.species.size());
      for (tempj = 0; tempj < Temp_Number_Species; tempj++) {
        if (reaction_mechanism.reactions[tempi].Reactants[tempj] != 0) {
          TempRow[tempj] = 1;
        }
        if (reaction_mechanism.reactions[tempi].Products[tempj] != 0) {
          TempRow[tempj] = 1;
        }
      }
      TempMatrix.push_back(TempRow);
      TempRow.clear();
    }

    size_t Number_Of_Selected_Species_Temp =
        InitialParameters.MechanismAnalysis.SpeciesSelectedForRates.size();

    for (tempj = 0; tempj < Number_Of_Selected_Species_Temp; tempj++) {
      size_t SpeciesID =
          InitialParameters.MechanismAnalysis.SpeciesSelectedForRates[tempj];
      vector<size_t> temp;

      for (tempi = 0; tempi < reaction_mechanism.reactions_size(); tempi++) {
        if (TempMatrix[tempi][SpeciesID] != 0) {
          temp.push_back(tempi);
        }
      }
      ReactionsForSpeciesSelectedForRates.push_back(temp);
      temp.clear();
    }

    RatesAnalysis::Prepare_Print_Rates_Per_Species(
        InitialParameters.Solver_Parameters.separator,
        reaction_mechanism.species,
        InitialParameters.MechanismAnalysis.SpeciesSelectedForRates,
        ReactionsForSpeciesSelectedForRates);
  }

  solver_calculation.ConcentrationChange = solver_calculation.SpeciesLossRate();

  /* -- Got values at t = 0 -- */

  // enables reset of Rates Analysis
  size_t RatesAnalysisTimepoint = 0;

  // For performance assessment, use a clock:

  // start the clock:
  auto chrono_current = std::chrono::high_resolution_clock::now();
  auto chrono_begin = chrono_current;

  do {
    time_step = time_current + time_step1;

    // cout << "debug " << time_current << " " << time_step1 << "\n";

    // https://en.cppreference.com/w/cpp/language/switch
    switch (solver_choice) // begin ODE Solver switch
    {

    // case IntelODE
    case 1001:
      dodesol_rkm9mkn(&Intel.vector_ipar[0], &n, &time_current, &time_step, y,
                      (void *)&wrapper_ODE_RHS, &Intel.h, &Intel.hm, &Intel.ep,
                      &Intel.tr, &Intel.vector_dpar[0], &Intel.vector_kd[0],
                      &Intel.ierr);
      if (Intel.ierr != 0) {
        printf("\n dodesol_rkm9mkn routine exited with error code %4d\n",
               Intel.ierr);
        exit(1);
      }
      break;

    case 1002:
      dodesol_rkm9mka(&Intel.vector_ipar[0], &n, &time_current, &time_step, y,
                      (void *)&wrapper_ODE_RHS,
                      (void *)&wrapper_Jacobian_Matrix_Intel, &Intel.h,
                      &Intel.hm, &Intel.ep, &Intel.tr, &Intel.vector_dpar[0],
                      &Intel.vector_kd[0], &Intel.ierr);
      if (Intel.ierr != 0) {
        printf("\n dodesol_rkm9mkn routine exited with error code %4d\n",
               Intel.ierr);
        exit(1);
      }
      break;

    case 1003:
      dodesol_mk52lfn(&Intel.vector_ipar[0], &n, &time_current, &time_step, y,
                      (void *)&wrapper_ODE_RHS, &Intel.h, &Intel.hm, &Intel.ep,
                      &Intel.tr, &Intel.vector_dpar[0], &Intel.vector_kd[0],
                      &Intel.ierr);
      if (Intel.ierr != 0) {
        printf("\n dodesol_rkm9mkn routine exited with error code %4d\n",
               Intel.ierr);
        exit(1);
      }
      break;

    case 1004:
      dodesol_mk52lfa(&Intel.vector_ipar[0], &n, &time_current, &time_step, y,
                      (void *)&wrapper_ODE_RHS,
                      (void *)&wrapper_Jacobian_Matrix_Intel, &Intel.h,
                      &Intel.hm, &Intel.ep, &Intel.tr, &Intel.vector_dpar[0],
                      &Intel.vector_kd[0], &Intel.ierr);
      if (Intel.ierr != 0) {
        printf("\n dodesol_rkm9mkn routine exited with error code %4d\n",
               Intel.ierr);
        exit(1);
      }
      break;

      // use LSODA
    case 2001:
      dlsoda_((void *)wrapper_ODE_RHS, &n, y, &time_current, &time_step,
              &LSODA.ITOL, &LSODA.RTOL, &LSODA.ATOL, &LSODA.ITASK,
              &LSODA.ISTATE, &LSODA.IOPT, &LSODA.vector_RWORK[0], &LSODA.LRW,
              &LSODA.vector_IWORK[0], &LSODA.LIW,
              (void *)wrapper_Jacobian_Matrix_Odepack_LSODA, &LSODA.JT);
      if (LSODA.ISTATE < 0) {
        printf("\n LSODA routine exited with error code %4d\n", LSODA.ISTATE);
        exit(1);
      }
      break;
    case 2002:
      dlsoda_((void *)wrapper_ODE_RHS, &n, y, &time_current, &time_step,
              &LSODA.ITOL, &LSODA.RTOL, &LSODA.ATOL, &LSODA.ITASK,
              &LSODA.ISTATE, &LSODA.IOPT, &LSODA.vector_RWORK[0], &LSODA.LRW,
              &LSODA.vector_IWORK[0], &LSODA.LIW,
              (void *)wrapper_Jacobian_Matrix_Odepack_LSODA, &LSODA.JT);
      if (LSODA.ISTATE < 0) {
        printf("\n LSODA routine exited with error code %4d\n", LSODA.ISTATE);
        exit(1);
      }
      break;

    } // end of ODE solver switch

    if (InitialParameters.MechanismAnalysis.MaximumRates) {
      RatesAnalysis::MaxRatesAnalysis(
          RatesAnalysisData, ProductsForRatesAnalysis,
          solver_calculation.Get_ReactantsForReactions(),
          solver_calculation.Get_Rates(), time_current);
    }

    if (InitialParameters.MechanismAnalysis.RatesAnalysisAtTime &&
        InitialParameters.MechanismAnalysis
                .RatesAnalysisAtTimeData[RatesAnalysisTimepoint] ==
            time_current) {
      RatesAnalysis::RatesAnalysisAtTimes(
          ProductsForRatesAnalysis,
          solver_calculation.Get_ReactantsForReactions(),
          solver_calculation.Get_Rates(), time_current,
          reaction_mechanism.species, reaction_mechanism.reactions);
      RatesAnalysisTimepoint = RatesAnalysisTimepoint + 1;
    }

    // Function for new per species output
    if (InitialParameters.MechanismAnalysis.RatesOfSpecies) {
      RatesAnalysis::Print_Rates_Per_Species(
          ProductsForRatesAnalysis,
          solver_calculation.Get_ReactantsForReactions(),
          InitialParameters.Solver_Parameters.separator,
          solver_calculation.Get_Rates(), time_current,
          reaction_mechanism.species,
          InitialParameters.MechanismAnalysis.SpeciesSelectedForRates,
          ReactionsForSpeciesSelectedForRates);
    }

    if (InitialParameters.MechanismAnalysis.StreamRatesAnalysis) {
      RatesAnalysis::StreamRatesAnalysis(
          write_output.get_prefix(), ProductsForRatesAnalysis,
          solver_calculation.Get_ReactantsForReactions(),
          solver_calculation.Get_Rates(), time_current, Number_Species);
    }

    double pressure = 0;
    //* // this is used in the PetroOxy code
    if (InitialParameters.GasPhase) {
      //  Pressure Tracking for Gas Phase Kinetics
      double total_mol = 0;
      for (size_t i = 0; i < Number_Species; i++) {
        total_mol = total_mol + solver_calculation.Concentration[i];
      }
      pressure =
          (total_mol * R * solver_calculation.Concentration[Number_Species]) /
          InitialParameters.GasPhaseVolume;
    } //*/

    write_output.StreamData(time_current, InitialParameters.GasPhase, pressure,
                            solver_calculation.Concentration,
                            solver_calculation.Get_Rates());

    //// TODO: seems to have broken...
    /*
    WriteOutput::Input_File_For_Ehsan_Opt(
                    ReactionRates_OFStream,
                    InitialParameters.Solver_Parameters.separator,
                    time_current,
                    solver_calculation.Rates
    );
    //*/

    if (InitialParameters.PetroOxy.IsSet) {
      SolverCalculation::PetroOxyOutputStream(write_output.get_name_petrooxy(),
                                              solver_calculation.PetroOxyData,
                                              time_current);
    }

    if (InitialParameters.MechanismReduction.ReduceReactions != 0) {
      MechanismReduction::ReactionRateImportance(
          KeyRates, solver_calculation.Get_Rates(),
          InitialParameters.MechanismReduction.ReduceReactions);
    }

    if (tracker < (TimeChanges - 1) &&
        time_step >= InitialParameters.TimeEnd[tracker]) {
      std::chrono::duration<double> chrono_time_taken =
          std::chrono::high_resolution_clock::now() - chrono_current;
      cout << "CPU Time: " << chrono_time_taken.count() << " seconds\n";
      chrono_current = std::chrono::high_resolution_clock::now();

      tracker = tracker + 1;
      time_step1 = InitialParameters.TimeStep[tracker];
      time_end = InitialParameters.TimeEnd[tracker];
      cout << "End Time: " << time_end << " Time Step: " << time_step1 << "\n";
    }

  } while (time_step < time_end);
  // end of loop from t_zero to t_end

  std::chrono::duration<double> chrono_time_taken =
      std::chrono::high_resolution_clock::now() - chrono_current;

  cout << "CPU Time: " << chrono_time_taken.count() << " seconds\n";

  write_output.close_stream_concentrations();
  write_output.close_stream_rates();

  if (InitialParameters.MechanismAnalysis.MaximumRates) {
    RatesAnalysis::WriteMaxRatesAnalysis(
        RatesAnalysisData, reaction_mechanism.species,
        reaction_mechanism.reactions, write_output.get_prefix());
  }

  // stop the clock
  auto chrono_end = std::chrono::high_resolution_clock::now();
  chrono_time_taken = chrono_end - chrono_begin;

  cout << "\nTotal CPU time: " << chrono_time_taken.count() << " seconds\n\n";
}
