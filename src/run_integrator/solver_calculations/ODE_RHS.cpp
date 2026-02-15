/*
 * ODE_RHS.cpp
 *
 *  Created on: 06.07.2015
 *      Author: DetlevCM
 */

#include "./solver_calculations.h"

// n = number equations/differentiated variables
// t = time step
// y = current value
// f = dy/dt

void SolverCalculation::ODE_RHS(int *n, double *t, double *y, double *f) {
  // stability hack - but has a performance impact...
  if (InitialDataConstants.EnforceStability) {
    for (size_t i = 0; i <= Number_Species; i++) {
      if (y[i] < 0) {
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

  // if PetroOxy case
  if (InitialDataConstants.PetroOxy) {
    double time_difference;

    time_difference = fabs(*t - time_previous);
    AdjustGasConcentration(y[OxyGasSpeciesID], Concentration[Number_Species],
                           time_difference, PetroOxyData);
  }

  Evaluate_Thermodynamic_Parameters(Concentration[Number_Species]);
  Calculate_Rate_Constant(Concentration[Number_Species]);
  CalculateReactionRates(Concentration, Kf, Kr);

  ConcentrationChange = SpeciesLossRate();

  double ctot = 0.0;
  double qint = 0.0;
  double qtot = 0.0;

  for (size_t i = 0; i < Number_Species; i++) {
    ctot = ctot + CalculatedThermo[i].Cv * Concentration[i];
    // reduce number of loops
    f[i] = ConcentrationChange[i];
  }
  // ctot = ctot / 1000; // working in moles/l so no Na;

  for (size_t i = 0; i < Number_Reactions; i++) {
    qint = qint + delta_n[i] * Rates[i];
  }
  qtot = -qint / (ctot); //*1000); // scale l to ml and Na not needed for
                         // moles/l * Na); //*/

  f[Number_Species] = qtot; // Temperature equation
  // cout << ctot << " " << qint << " " << qtot << "\n";

  if (InitialDataConstants.PetroOxy) {
    if (Concentration[Number_Species] >= InitialDataConstants.temperature) {
      y[Number_Species] =
          InitialDataConstants
              .temperature; // ensure temperature is not exceeded
      InitialDataConstants.PetroOxyTemperatureRise = 0.0;

      f[Number_Species] = 0.0;
    }

    if (InitialDataConstants.PetroOxyTemperatureRise !=
        0) // fix temperature for Oxy, rise desired
    {
      // 298K starting temp, GlobalArrays::InitialParameters.temperature is
      // target rise time given in minutes
      f[Number_Species] = (InitialDataConstants.temperature - 298.15) /
                          (InitialDataConstants.PetroOxyTemperatureRise);

      // std::cout << f[Number_Species] << "\n";
    }
  }

  // Settings relevant rates to zero
  if (InitialDataConstants.ConstantConcentration) {
    // cout << "preparing constant species \n";
    for (size_t i = 0; i < Number_Species; i++) {
      if (InitialDataConstants.ConstantSpecies[i] != 0.0) {
        f[i] = 0.0; // concentration reset
                    // cout << "f[" << i << "] = " << f[i];
      }
    }
  }

  if (InitialDataConstants.TGA_used) {
    if (Concentration[Number_Species] < InitialDataConstants.TGA_target) {
      f[Number_Species] = InitialDataConstants.TGA_rate;
    } else if (Concentration[Number_Species] >=
               InitialDataConstants.TGA_target) {
      f[Number_Species] = 0.0;
    }
  }

  // IEEE standard hack to check for NaN
  // if temperature blows up, freeze it
  //*
  if (qtot != qtot) {
    f[Number_Species] = 0.0;
  }
  //*/

  // For Oxy limiting - easier to just always calculate it
  time_previous = *t;
}
