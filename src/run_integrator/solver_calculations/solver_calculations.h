/*
 * Calculation_Functions.h
 *
 *  Created on: 20 Dec 2012
 *      Author: pmdcm
 */

#ifndef SOLVER_CALCULATION
#define SOLVER_CALCULATION

#include <omp.h>

#include "../../get_process_input/get_mechanism/ReactionMechanism.h"
#include "../../get_process_input/get_mechanism/Species.h"
#include "../../global_struct.h"

class SolverCalculation {

  // private member variables, needed for the calculations:
private:
  // vectors to provide performant calculation structures:
  vector<TrackSpecies> ReactantsForReactions;
  vector<TrackSpecies> ProductsForReactions;
  vector<TrackSpecies> SpeciesLossAll; // vector for recording species loss
  vector<ReactionParameters> reaction_parameters;

  // vector of data needed for the calculation, really just set once
  vector<double> delta_n;

  // variables used actively in the calculation
  vector<Species::ThermodynamicData::CalculatedThermodynamics> CalculatedThermo;

  vector<double> Kf;
  vector<double> Kr;
  vector<double> Rates;

  // functions needed for access to the private variables:
public:
  const vector<TrackSpecies> &Get_ReactantsForReactions() {
    return ReactantsForReactions;
  };
  const vector<double> &Get_Rates() { return Rates; };
  vector<double> Get_Kf() { return Kf; };
  vector<double> Get_Kr() { return Kr; };

public:
  // cannot make this private just yet
  vector<double> Concentration;
  vector<double> ConcentrationChange;

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

  size_t Number_Species;   // old variable
  size_t Number_Reactions; // old variable

  vector<Species> species; // quickest and easiest way right now...

public:
  // initialize the vectors that are needed for efficient calculation
  void init(vector<Species> vec_species,
            const vector<SingleReactionData> &mechanism_reactions) {

    reaction_parameters = Process_Reaction_Parameters(mechanism_reactions);
    ReactantsForReactions = Reactants_ForReactionRate(mechanism_reactions);
    ProductsForReactions = Products_ForReactionRate(mechanism_reactions, false);
    SpeciesLossAll = PrepareSpecies_ForSpeciesLoss(mechanism_reactions);
    delta_n = Get_Delta_N(mechanism_reactions);

    species = vec_species;

    size_t number_species = species.size();
    size_t number_reactions = reaction_parameters.size();

    Number_Species = number_species;
    Number_Reactions = number_reactions;

    //// variable (values change during calculation ////
    CalculatedThermo.resize(number_species);

    Kf.resize(number_reactions);
    Kr.resize(number_reactions);
    Rates.resize(number_reactions);

    // 1 extra position for temperature
    Concentration.resize(number_species + 1);
    ConcentrationChange.resize(number_species + 1);
  };

  static vector<double> Get_Delta_N(const vector<SingleReactionData> Reactions);

  // pre-processing functions
  static vector<ReactionParameters>
  Process_Reaction_Parameters(const vector<SingleReactionData> &);
  static vector<TrackSpecies>
  Reactants_ForReactionRate(const vector<SingleReactionData> &);
  static vector<TrackSpecies>
  Products_ForReactionRate(const vector<SingleReactionData> &, bool);
  static vector<TrackSpecies>
  PrepareSpecies_ForSpeciesLoss(const vector<SingleReactionData> &);

  // The ODE RHS functions are split - regular initial value & pressure vessel
  // (reservoir)
  void ODE_RHS(int *n, double *t, double *y, double *f);

  /*
   * Calculating Rate Constants
   */

  void Evaluate_Thermodynamic_Parameters(const double Temperature);

  //// BEGIN third body preparation
  //// needs a unit check

  double Calculate_no_LOW_Troe(const ReactionParameters &ReactionData,
                               const vector<double> &Concentration, double T,
                               double third_body);

  double
  Calculate_Lindeman_Hinshelwood_SRI(const ReactionParameters &ReactionData,
                                     const vector<double> &Concentration,
                                     double T, double third_body);

  double Calculate_Lindeman_Hinshelwood_Low_Troe(
      const ReactionParameters &ReactionData,
      const vector<double> &Concentration,
      double T,         // current temperature
      double third_body // sum of third bodies, but which units, original
                        // molecules per cm3
  );

  //// END third body preparation

  void Calculate_Rate_Constant(const double Temperature);

  void CalculateReactionRates(const vector<double> &Concentration,
                              vector<double> Forward_Rates,
                              vector<double> Reverse_Rates //,
  );

  vector<double> SpeciesLossRate();

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
