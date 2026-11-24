/*
 * Class_Initial_Data.hpp
 *
 *  Created on: Sep 30, 2018
 *      Author: detlevcm
 */

#ifndef _INITIAL_DATA_
#define _INITIAL_DATA_

// File Streams and IO

#include "../../helpers/Helpers.h"
#include "../get_mechanism/Species.h"

class Initial_Data {

public:
  struct InitSpecies {
    size_t SpecID;
    double SpecConc;
  };

  struct MechanismAnalysisConfig {
    bool MaximumRates;
    bool RatesSpeciesAllAnalysis;
    bool StreamRatesAnalysis;
    bool RatesAnalysisAtTime;
    vector<double> RatesAnalysisAtTimeData;

    // integer array to identify the species or which we want rate of production
    // analysis
    bool RatesOfSpecies;
    vector<size_t> SpeciesSelectedForRates;
    vector<vector<size_t>> ReactionsForSpeciesConsAnalysis;
    vector<vector<size_t>> ReactionsForSpeciesProdAnalysis;
  };

  // Planing to split this into its own struct
  class PressureVesselConfig {
  public:
    bool IsSet;
    size_t GasSpecies;
    double SampleSize;
    double InitPressure;
    double MaxPressure;
    double GasSolubility;
    double TemperatureRise;
    bool HenryLawDiffusionLimitSet;
    double HenryLawDiffusionLimit;
    // new addition to allow different vessel sizes
    double VesselSize;
  };

  class PressureVessel {
  public:
    double Liquid_Sample_Volume;
    double Gas_Sample_Volume;
  };

  class mechanism_reduction {
  public:
    bool UseFastLumping;
    // bool UseNewLumping;
    int LumpingType;
    double ReduceReactions;
  };

  class solver_parameters { // structure to store the solver parameters
  public:
    int SolverType;        // 0 = IntelODE, 1 = Odepack
    bool Use_Stiff_Solver; // for IntelODE
    bool Use_Analytical_Jacobian;
    double rtol;
    double atol;
    double minimum_stepsize; // hm
    double initial_stepsize; // h
    string separator;        // separator in text output, e.g. comma delimited
    int threads; // allow the user to use threading, not useful for small
                 // mechanisms
  };

  // to allow modifying some solver behaviour during the run, for IntelODE only
  class solver_type {
  public:
    bool Use_Stiff_Solver;
    bool Use_Analytical_Jacobian;
  };

  /* The new data storage organisation, using a class rather than a struct */

  bool NoIntegration;

  // initial concentrations are part of the initial conditions
  vector<double> InitialSpeciesConcentration;

  // Analysis:
  MechanismAnalysisConfig MechanismAnalysis;

  // Initial Conditions:
  vector<double> TimeEnd;
  vector<double> TimeStep;
  double temperature;
  /* Experimental Gas Phase Code */
  bool GasPhase; // i.e. is gas phase, default false
  double GasPhaseVolume;
  double GasPhasePressure;

  // Mechanism Reduction:
  mechanism_reduction MechanismReduction;

  // PetroOxy
  PressureVesselConfig PetroOxy;

  // Pressure Vessel
  PressureVessel Pressure_Vessel;

  // Solver Parameters:
  solver_parameters Solver_Parameters;
  vector<solver_type> Solver_Type; // for specifying a mix of solvers, no
                                   // individual tolerances though

  // Species:
  vector<InitSpecies> InitialLiquidSpecies;
  vector<InitSpecies> InitialGasSpecies;
  /* Constant concentration option */
  /* Single vector, to switch rates of specified species to zero */
  bool ConstantConcentration;
  vector<size_t> ConstantSpecies;

  // extra parameters for a TGA case
  bool TGA_used;
  double TGA_rate;
  double TGA_target;

  // Old mess below...

  bool PrintReacRates;
  bool EnforceStability;
  /* Extra Functionality */
  bool StoichiometryMatrixForOpt;
  /* Pre-Processing of the Mechanism */
  bool irrev;

  static void Get_Initial_Conditions(string, vector<Species>, Initial_Data &);

  static void Handle_Initial_Conditions(Initial_Data &InitialParameters,
                                        vector<string> Input,
                                        solver_type Global_Solver_Settings);

  static vector<InitSpecies> Handle_Species(Initial_Data &InitialParameters,
                                            vector<string> Input,
                                            vector<Species> Species);

  static void Handle_Solver_Parameters(Initial_Data &InitialParameters,
                                       vector<string> Input);

  static void Handle_Analysis(Initial_Data &InitialParameters,
                              vector<string> Input, vector<Species> species);

  static void Handle_Pressure_Vessel(Initial_Data &InitialParameters,
                                     vector<string> Input);

  static void Handle_PetroOxy(Initial_Data &InitialParameters,
                              vector<string> Input, vector<Species> Species);

  static mechanism_reduction Handle_Mechanism_Reduction(vector<string> Input);
};

#endif
