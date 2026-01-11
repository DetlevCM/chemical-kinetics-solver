/*
 * Calculation_Functions.h
 *
 *  Created on: 20 Dec 2012
 *      Author: pmdcm
 */

#ifndef SOLVER_CALCULATION
#define SOLVER_CALCULATION

#include "../../global_struct.h"
// #include "../../get_process_input/get_initial_data/Initial_Data.h"
#include "../../get_process_input/get_mechanism/Reaction.h"
#include "../../get_process_input/get_mechanism/ReactionMechanism.h"
#include "../../get_process_input/get_mechanism/Species.h"

class SolverCalculation {

public:
  /// PetroOxy Additional variables

  size_t OxyGasSpeciesID;
  PressureVesselCalc PetroOxyData;
  // for limited Oxy
  double time_previous;

  // end PetroOxy variables

  struct ConstantInitRHSODE {
    bool EnforceStability;
    bool PetroOxy;
    double temperature;
    double PetroOxyTemperatureRise;

    bool TGA_used;
    double TGA_rate;
    double TGA_target;

    // for constant concentration
    bool ConstantConcentration;
    vector<double> ConstantSpecies;
  };

  ConstantInitRHSODE InitialDataConstants;

  vector<TrackSpecies> ReactantsForReactions;
  vector<TrackSpecies> ProductsForReactions;

  vector<TrackSpecies> SpeciesLossAll; // vector for recording species loss

  // vector< ReactionParameter > ReactionParameters; // tidier than reactions
  // vector
  vector<Reaction::ReactionParameter>
      ReactionParameters; // tidier than reactions vector

  size_t Number_Species;   // old variable
  size_t Number_Reactions; // old variable

  vector<Species::ThermodynamicData::CalculatedThermodynamics> CalculatedThermo;
  vector<double> Kf;
  vector<double> Kr;
  vector<double> delta_n;

  vector<Species> species; // quickest and easiest way right now...

  vector<double> Rates;
  vector<double> Concentration;
  vector<double> Calc_Concentration;
  vector<double> SpeciesConcentrationChange;

public:
  // cannot use the object in the solver with a member function
  // so use a global object and then init & use helper function?
  void init(vector<Species> vec_species, // quick and ugly...
            size_t number_species, size_t number_reactions,
            vector<TrackSpecies> reactantsForReactions,
            vector<TrackSpecies> productsForReactions,
            vector<TrackSpecies> speciesLossAll, vector<double> prep_delta_n) {
    //// constant (i.e. set once) ////

    ConstantInitRHSODE InitialDataConstants;
    ReactantsForReactions = reactantsForReactions;
    ProductsForReactions = productsForReactions;
    SpeciesLossAll = speciesLossAll;
    delta_n = prep_delta_n;

    species = vec_species;

    Number_Species = species.size();
    Number_Reactions = number_reactions;

    //// variable (values change during calculation ////
    CalculatedThermo.resize(number_species);

    ReactionParameters.resize(number_reactions);
    Kf.resize(number_reactions);
    Kr.resize(number_reactions);
    Rates.resize(number_reactions);

    Concentration.resize(number_species +
                         1); // 1 extra position for temperature
    SpeciesConcentrationChange.resize(number_species +
                                      2); // temperature + one extra position?
  };

  // The ODE RHS functions are split - regular initial value & pressure vessel
  // (reservoir)
  void ODE_RHS(int *n, double *t, double *y, double *f);

  /*
   * Calculating Rate Constants
   */

  void Evaluate_Thermodynamic_Parameters(const double Temperature);

  void Calculate_Rate_Constant(const double Temperature,
                               const vector<TrackSpecies> &SpeciesAll //,
  );

  void CalculateReactionRates(const vector<double> &Concentration,
                              vector<double> Forward_Rates,
                              vector<double> Reverse_Rates //,
  );

  vector<double> SpeciesLossRate(const vector<double> &Combined_Rates,
                                 const vector<TrackSpecies> &SpeciesLossList);

  //// NOTE: included in solver calculations for ease of implementation

  struct JacobianSpecies {
    size_t SpeciesID;
    double power;
  };

  struct JacobianData {
    bool IsForward;
    bool IsProduction;
    size_t ColumnWiseArrayPosition;
    size_t ReactionID;
    double coefficient;
    vector<JacobianSpecies> Species;
  };

  vector<JacobianData> JacobianMatrix;

  // Jacobian Matrix for plain Arrhenius expressions only

  void Prepare_Jacobian_Matrix(
      vector<JacobianData> &JacobianColumnWise,
      const vector<Reaction::SingleReactionData> &Reactions);

  void Jacobian_Matrix_Intel(int *, double *, double *, double *);

  void Jacobian_Matrix_Odepack_LSODA(int *, double *, double *, double *,
                                     double *, double *, int *);

  //// NOTE: included in solver calculation for ease of implementation

  void AdjustGasConcentration(double &, double, double, PressureVesselCalc &);

  void Adjust_Gas_Concentration_Initial(double &, double, PressureVesselCalc &);

  //// TODO: should the PetroOxy output be here or in the output class
  static void PetroOxyOutputHeader(string);

  static void PetroOxyOutputStream(string, const PressureVesselCalc &, double);
};

/* wrappers to allow calling the member functions from Fortran */
// https://isocpp.org/wiki/faq/pointers-to-members

#endif
