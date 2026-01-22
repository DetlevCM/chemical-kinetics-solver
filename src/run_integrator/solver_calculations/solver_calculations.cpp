
#include "../../global_const.h"

#include "./solver_calculations.h"

void SolverCalculation::Evaluate_Thermodynamic_Parameters(
    const double Temperature) {
  Species::ThermodynamicData::ThermoT temperatures(Temperature);
  /* Hf, Cp, Cv, S */
  for (size_t i = 0; i < Number_Species; i++) {
    CalculatedThermo[i] =
        species[i].thermodynamicdata.calculate_thermodynamics(temperatures);
    // cout << "Hf: " <<  CalculatedThermo[i].Hf << "\n";
  }
}

void SolverCalculation::Calculate_Rate_Constant(const double Temperature)

{
  // Pressure Independent Reactions Only
  /*
  k[i] = a1[i]*exp(-e1[i]*inv_T);  // kinf calculation
  if (n1[i] != 0.0)
  k[i] *= pow(T, n1[i]);
  //*/

  // Define vectors in the right size right away
  vector<double> delta_H(Number_Reactions);
  vector<double> delta_S(Number_Reactions);
  vector<double> delta_G(Number_Reactions);

  // Worked out per reaction
  // as we have the value of the calculated thermodynamics, we just need to put
  // them together per reaction, going through every species

  for (size_t i = 0; i < SpeciesLossAll.size();
       i++) { // loop over every reaction
    delta_H[SpeciesLossAll[i].ReactionID] =
        delta_H[SpeciesLossAll[i].ReactionID] +
        (CalculatedThermo[SpeciesLossAll[i].SpeciesID].Hf *
         SpeciesLossAll[i].coefficient);

    delta_S[SpeciesLossAll[i].ReactionID] =
        delta_S[SpeciesLossAll[i].ReactionID] +
        (CalculatedThermo[SpeciesLossAll[i].SpeciesID].S *
         SpeciesLossAll[i].coefficient);
  }

  for (size_t i = 0; i < Number_Reactions;
       i++) // Straightforward Arrhenius Expression/Equation
  {
    Kf[i] = ReactionParameters[i].paramA *
            exp(-ReactionParameters[i].paramEa /
                Temperature); // do NOT forget the - !!!

    //* Speedup by only raising temperature to power where needed: improvement
    // is large :)
    if (ReactionParameters[i].paramN !=
        0.0) // raising to power 0 has no effect, so only if not 0
    {
      // unsure if this check really gives a performance improvement...
      // maybe it used to and no longer does with a modern
      // compiler/processor/kernel -> seems to be ever so slightly faster
      // if (ReactionParameters[i].paramN != 1.0) {
      Kf[i] = Kf[i] * pow(Temperature, ReactionParameters[i].paramN);
      /*} else {
        Kf[i] = Kf[i] * Temperature; // raise temp^1 = temp
      }//*/
    }

    /*
    cout <<
    //		Temperature << " , " <<
    //		exp(-ReactionParameters[i].paramEa/Temperature) << " , " <<
                    ReactionParameters[i].paramA << " , " <<
                    ReactionParameters[i].paramN << " , " <<
                    ReactionParameters[i].paramEa << " , " <<
                    Kf[i] << "\n";//*/

    // default, change if reversible - seems a little bit faster...
    Kr[i] = 0;
    // then calculate and set the reverse if required
    if (ReactionParameters[i].Reversible) {
      double temp_kp, temp_kc;
      delta_G[i] = delta_H[i] - Temperature * delta_S[i];
      temp_kp = exp(-delta_G[i] / (R * Temperature));

      if (delta_n[i] == 0) // an if check is less expensive than a pow(,)
      {
        Kr[i] = Kf[i] / temp_kp;
      } else {
        temp_kc = temp_kp * pow((0.0820578 * Temperature),
                                (-delta_n[i])); // L atm K^(-1) mol^(-1)
        // temp_kc = temp_kp*pow((1.3624659e-22*Temperature),(-Delta_N[i])); //
        // for molecules cm^(-3)
        Kr[i] = Kf[i] / temp_kc;
      }

      // IEEE standards hack to avoid Nan Rate, shouldn't exist in the first
      // place...
      /*if(Kr[i] != Kr[i])
      {
              Kr[i] = 0;
      }//*/
    }
  }
}

void SolverCalculation::CalculateReactionRates(
    const vector<double> &Concentration, vector<double> Forward_Rates,
    vector<double> Reverse_Rates) {

// would this be useful?
#pragma omp parallel for

  for (size_t i = 0; i < ReactantsForReactions.size();
       i++) { // Forward Rates determined by the reactants

    if (ReactantsForReactions[i].coefficient ==
        1) // this is quicker than raising to the power of 1 - but check with
           // the struct
    {
      Forward_Rates[ReactantsForReactions[i].ReactionID] =
          Forward_Rates[ReactantsForReactions[i].ReactionID] *
          Concentration[ReactantsForReactions[i].SpeciesID];
    } else {
      Forward_Rates[ReactantsForReactions[i].ReactionID] =
          Forward_Rates[ReactantsForReactions[i].ReactionID] *
          pow(Concentration[ReactantsForReactions[i].SpeciesID],
              ReactantsForReactions[i].coefficient);
    }
  }

  for (size_t i = 0; i < ProductsForReactions.size();
       i++) { // Reverse Rates determined by the products

    if (ProductsForReactions[i].coefficient ==
        1) // this is quicker than raising to the power of 1 - but check with
           // the struct
    {
      Reverse_Rates[ProductsForReactions[i].ReactionID] =
          Reverse_Rates[ProductsForReactions[i].ReactionID] *
          Concentration[ProductsForReactions[i].SpeciesID];
    } else {
      Reverse_Rates[ProductsForReactions[i].ReactionID] =
          Reverse_Rates[ProductsForReactions[i].ReactionID] *
          pow(Concentration[ProductsForReactions[i].SpeciesID],
              ProductsForReactions[i].coefficient);
    }
  }
#pragma omp barrier

  for (size_t i = 0; i < Rates.size(); i++) {
    Rates[i] = Forward_Rates[i] - Reverse_Rates[i];
    /*
    cout << i << " " << Rates[i] << "   " << Forward_Rates[i] << "   "
         << Reverse_Rates[i] << "\r\n"
         << std::flush;
    //*/
  }
}

vector<double> SolverCalculation::SpeciesLossRate() {
  vector<double> temp_species_loss(Number_Species);

  for (size_t i = 0; i < SpeciesLossAll.size(); i++) {
    temp_species_loss[SpeciesLossAll[i].SpeciesID] =
        temp_species_loss[SpeciesLossAll[i].SpeciesID] +
        (Rates[SpeciesLossAll[i].ReactionID] * SpeciesLossAll[i].coefficient);
  }

  /*
  for (size_t i = 0; i < temp_species_loss.size(); i++) {
    cout << i << "  " << temp_species_loss[i] << "\n" << std::flush;
  }
  //*/

  return temp_species_loss;
}
