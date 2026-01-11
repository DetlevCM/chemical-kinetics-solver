/*
 * ODE_RHS_Liquid_Phase.cpp
 *
 *  Created on: 06.07.2015
 *      Author: DetlevCM
 */

#include "./solver_calculations.h"

/*
 * Customized ODE function for the pressure Vessel case
 * should make long term maintenance better AND will
 * improve performance through fewer if statements
 * (fewer ifs = good)
 */

void SolverCalculation::ODE_RHS_Pressure_Vessel(int *n, double *time_current,
                                                double *y, double *f) {
  // stability hack
  if (InitialDataConstants.EnforceStability) {
    for (size_t i = 0; i <= Number_Species; i++) {
      if (y[i] < 0) {
        // if(y[i]<1.e-24){
        Concentration[i] = 0;
      } else {
        Concentration[i] = y[i];
      }
    }
  } else {
    for (size_t i = 0; i <= Number_Species; i++) {
      Concentration[i] = y[i];
    }
  }

  double time_difference;

  time_difference = fabs(*time_current - time_previous);
  AdjustGasConcentration(y[OxyGasSpeciesID], Concentration[Number_Species],
                         time_difference, PetroOxyData);

  // retain previous thermo if temperature change is effectively zero
  if (f[Number_Species] > 1e-6) {
    // Thermodynamic data, Rate Constant, Rates, new Concentrations
    // Evaluate_Thermodynamic_Parameters(CalculatedThermo, species,
    // Concentration[Number_Species]);
    Evaluate_Thermodynamic_Parameters(Concentration[Number_Species]);
    // Calculate_Rate_Constant(Kf, Kr,
    // Concentration[Number_Species],ReactionParameters, CalculatedThermo,
    // SpeciesLossAll, delta_n);
    Calculate_Rate_Constant(Concentration[Number_Species], SpeciesLossAll);
  }
  // CalculateReactionRates(Rates, Concentration, Kf, Kr, ReactantsForReactions,
  // ProductsForReactions);
  CalculateReactionRates(Concentration, Kf, Kr);
  SpeciesConcentrationChange = SpeciesLossRate(Rates, SpeciesLossAll);

  double ctot = 0;
  double qint = 0;
  double qtot = 0;

  for (size_t i = 0; i < Number_Species; i++) {
    ctot = ctot + CalculatedThermo[i].Cv * Concentration[i];
    // cout << CalculatedThermo[i].Hf << " ";

    // reduce number of loops
    f[i] = SpeciesConcentrationChange[i];
  }
  // cout << "\n";
  //  ctot = ctot / 1000; // working in moles/l so no Na;

  for (size_t i = 0; i < Number_Reactions; i++) {
    qint = qint + delta_n[i] * Rates[i];
  }
  qtot = -qint / (ctot); //*1000); // scale l to ml and Na not needed for
                         // moles/l * Na); //*/

  // Checked f[i] -> no unexpected rates of change for "inert gases", all 0.
  /*
  for (i = 0; i < Number_Species; i++)
  {
          f[i] = SpeciesConcentrationChange[i]; // Species equations //
  }//*/
  f[Number_Species] = qtot; // Temperature equation //
  // cout << ctot << " " << qint << " " << qtot << "\n";

  if (Concentration[Number_Species] >= InitialDataConstants.temperature) {
    y[Number_Species] =
        InitialDataConstants.temperature; // ensure temperature is not exceeded
    InitialDataConstants.PetroOxyTemperatureRise = 0;

    f[Number_Species] = 0;
  }

  if (InitialDataConstants.PetroOxyTemperatureRise !=
      0) // fix temperature for Oxy, rise desired
  {
    // 298K starting temp, GlobalArrays::InitialParameters.temperature is target
    // rise time given in minutes
    f[Number_Species] = (InitialDataConstants.temperature - 298) /
                        (InitialDataConstants.PetroOxyTemperatureRise);

    // std::cout << f[Number_Species] << "\n";
  } //*/

  // Settings relevant rates to zero
  if (InitialDataConstants.ConstantConcentration) {
    // cout << "preparing constant species \n";
    for (size_t i = 0; i < Number_Species; i++) {
      if (InitialDataConstants.ConstantSpecies[i] != 0) {
        f[i] = 0; // concentration reset
                  // cout << "f[" << i << "] = " << f[i];
      }
    }
  }

  // IEEE standard hack to check for NaN
  // if temperature blows up, freeze it
  if (qtot != qtot) {
    f[Number_Species] = 0;
  }

  // For Oxy limiting
  time_previous = *time_current;
}
