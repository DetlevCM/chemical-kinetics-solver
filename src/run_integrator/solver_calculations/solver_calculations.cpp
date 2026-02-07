
#include "../../global_const.h"

#include "./solver_calculations.h"

void SolverCalculation::Evaluate_Thermodynamic_Parameters(
    const double Temperature) {
  Species::ThermodynamicData::ThermoT temperatures(Temperature);
/* Hf, Cp, Cv, S */
#pragma omp parallel for
  for (size_t i = 0; i < Number_Species; i++) {
    CalculatedThermo[i] =
        species[i].thermodynamicdata.calculate_thermodynamics(temperatures);
    // cout << "Hf: " <<  CalculatedThermo[i].Hf << "\n";
  }
#pragma omp barrier
}

double
SolverCalculation::Calculate_no_LOW_Troe(const SingleReactionData &ReactionData,
                                         const vector<double> &Concentration,
                                         double T, double third_body) {
  double inv_T = 1.0 / T;
  double k; // this is the normal forward K

  // values at "infinity" - standard Arrhenius values
  double a1 = ReactionData.forward.A;
  double n1 = ReactionData.forward.n;
  double e1 = ReactionData.forward.Ea;

  double mod_third_body;

  // not applicable to a special type with water

  k = third_body * a1 * exp(-e1 * inv_T);
  if (n1 != 0.0)
    k *= pow(T, n1);

  // basically, just a case of collision efficiency

  // else { /* collision efficiency corrections required */
  if (ReactionData.TB_param.size() > 0) {
    mod_third_body = third_body;

    // seems like a case of vector<value, speciesID for concentration> to
    // calculate the correction
    for (size_t i = 0; i < ReactionData.TB_param.size(); i++) {
      mod_third_body = mod_third_body +
                       ReactionData.TB_param[i].value *
                           Concentration[ReactionData.TB_param[i].SpeciesID];
    }

    k = mod_third_body * a1 * exp(-e1 * inv_T);
    if (n1 != 0.0)
      k *= pow(T, n1);
  }

  return k;
}

double SolverCalculation::Calculate_Lindeman_Hinshelwood_SRI(
    const SingleReactionData &ReactionData, const vector<double> &Concentration,
    double T, double third_body) {
  double inv_T = 1.0 / T;
  double kinf;
  double kzero;
  double k; // the result, Kf

  // values at "infinity" - standard Arrhenius values
  double a1 = ReactionData.forward.A;
  double n1 = ReactionData.forward.n;
  double e1 = ReactionData.forward.Ea;

  double a0 = ReactionData.TB_low.A;
  double n0 = ReactionData.TB_low.n;
  double e0 = ReactionData.TB_low.Ea;

  ThirdBody_SRI sri = ReactionData.TB_sri;

  double mod_third_body;

  /* LOW parameters but not TROE, ie simple Lindemann-Hinshelwood, unless it's
   * SRI type */
  kinf = a1 * exp(-e1 * inv_T);
  if (n1 != 0.0)
    kinf *= pow(T, n1);
  kzero = a0 * exp(-e0 * inv_T);
  if (n0 != 0.0)
    kzero *= pow(T, n0);

  // SRI flag which provides 3 calculation methods for three distinct cases
  // Special Water reactions that use the species concentration for H20, N2, H2,
  // Ar, CO2, CH4, C2H6, O2 calculations without collision efficiency
  // corrections that use a "third body" parameter calculations using a
  // corrected collision efficiency

  // needs the special treatment for the species concentration as an option for
  // mod_third_body
  mod_third_body = third_body;
  // if (ReactionData.collision_efficiency)  { /* collision efficiency
  // corrections */
  if (ReactionData.TB_param.size() > 0) {
    mod_third_body = third_body;

    // seems like a case of vector<value, speciesID for concentration> to
    // calculate the correction
    for (size_t i = 0; i < ReactionData.TB_param.size(); i++) {
      mod_third_body = mod_third_body +
                       ReactionData.TB_param[i].value *
                           Concentration[ReactionData.TB_param[i].SpeciesID];
    }
  }

  // double mod_third_body_molecules_cm3 = mod_third_body/1000.0*6.0221e23;
  double F = 1.0;

  if (ReactionData.sri_flag == 0) {
    F = 1.0;
  } else if (ReactionData.sri_flag == 1) /* its simple SRI */
  {
    // log is not possible for 0
    if (mod_third_body < 1.0e-30) {
      mod_third_body = 1.0e-30;
    }
    // F = T*pow((sri.a*exp(-sri.b/T)+exp(-T/sri.c)),
    // ((1.0/(1.0+pow((log10(kzero*mod_third_body/kinf)), 2)))));
    F = T *
        pow((sri.a * exp(-sri.b / T) + exp(-T / sri.c)),
            ((1.0 / (1.0 + pow((log10(kzero * mod_third_body / kinf)), 2)))));
  } else if (ReactionData.sri_flag == 2) /* It's complex SRI */
  {
    // log is not possible for 0
    if (mod_third_body < 1.0e-30) {
      mod_third_body = 1.0e-30;
    }
    // F = sri.d*pow(T, sri.e)*pow((sri.a*exp(-sri.b/T)+exp(-T/sri.c)),
    // ((1.0/(1.0+pow((log10(kzero*mod_third_body/kinf)), 2)))));
    F = sri.d * pow(T, sri.e) *
        pow((sri.a * exp(-sri.b / T) + exp(-T / sri.c)),
            ((1.0 / (1.0 + pow((log10(kzero * mod_third_body / kinf)), 2)))));
  }

  /* it is plain Lindeman-Hinshelwood */
  k = kzero * kinf * mod_third_body * F / (kzero * mod_third_body + kinf);

  return k;
}

double SolverCalculation::Calculate_Lindeman_Hinshelwood_Low_Troe(
    const SingleReactionData &ReactionData, const vector<double> &Concentration,
    double T,         // current temperature
    double third_body // sum of third bodies, but which units, original
                      // molecules per cm3
) {
  double inv_T = 1.0 / T;
  double kinf;
  double kzero;
  double kappa;
  double Fc, F;
  double k; // the result, Kf

  // values at "infinity" - standard Arrhenius values
  double a1 = ReactionData.forward.A;
  double n1 = ReactionData.forward.n;
  double e1 = ReactionData.forward.Ea;

  double a0 = ReactionData.TB_low.A;
  double n0 = ReactionData.TB_low.n;
  double e0 = ReactionData.TB_low.Ea;

  ThirdBody_troe troe = ReactionData.TB_troe;
  double inv_T1 = 1.0 / troe.T1;
  double inv_T3 = 1.0 / troe.T3;

  double mod_third_body;

  /* LOW & Troe */
  kinf = a1 * exp(-e1 * inv_T);
  if (n1 != 0.0)
    kinf *= pow(T, n1);
  kzero = a0 * exp(-e0 * inv_T);
  if (n0 != 0.0)
    kzero *= pow(T, n0);

  // the more common form could be first to have a quicker code...
  if (ReactionData.TB_troe.is_4_param == false) {
    // 3 parameter TROE takes a, T3, T1
    Fc = (1.0 - troe.a) * exp(-T * inv_T3) + troe.a * exp(-T * inv_T1);
  } else // (ReactionData.troeThirdBody.is_4_param == true)
  {
    // 4 parameter TROE takes a, T3, T1, T2
    Fc = (1.0 - troe.a) * exp((-T * inv_T3)) + troe.a * exp((-T * inv_T1)) +
         exp((-troe.T2 * inv_T));
  }

  // needs the special treatment for the species concentration as an option for
  // mod_third_body
  mod_third_body = third_body;

  // if (ReactionData.collision_efficiency) { /* collision efficiency
  // corrections */
  if (ReactionData.TB_param.size() > 0) {
    mod_third_body = third_body;

    // seems like a case of vector<value, speciesID for concentration> to
    // calculate the correction
    for (size_t i = 0; i < ReactionData.TB_param.size(); i++) {
      mod_third_body = mod_third_body +
                       ReactionData.TB_param[i].value *
                           Concentration[ReactionData.TB_param[i].SpeciesID];
    }
  }

  // kappa fails if third body is 0, due to log10 of 0 ...
  if (mod_third_body < 1.0e-30) {
    mod_third_body = 1.0e-30;
  }

  // double mod_third_body_molecules_cm3 = mod_third_body/1000.0*6.0221e23;

  // kappa = log10(kzero*mod_third_body/kinf) - 0.4 -0.67*log10(Fc);
  kappa = log10(kzero * mod_third_body / kinf) - 0.4 - 0.67 * log10(Fc);
  F = pow(10,
          (log10(Fc) /
           (1 + pow((kappa / (0.75 - 1.27 * log10(Fc) - 0.14 * kappa)), 2))));
  k = kzero * kinf * mod_third_body * F / (kzero * mod_third_body + kinf);

  return k;
}

void SolverCalculation::Calculate_Rate_Constant(const double Temperature) {
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

#pragma omp parallel for
  for (size_t i = 0; i < Number_Reactions;
       i++) // Straightforward Arrhenius Expression/Equation
  {
    Kf[i] =
        ReactionParameters[i].A *
        exp(-ReactionParameters[i].Ea / Temperature); // do NOT forget the - !!!

    //* Speedup by only raising temperature to power where needed: improvement
    // is large :)
    if (ReactionParameters[i].n !=
        0.0) // raising to power 0 has no effect, so only if not 0
    {
      // unsure if this check really gives a performance improvement...
      // maybe it used to and no longer does with a modern
      // compiler/processor/kernel -> seems to be ever so slightly faster
      // if (ReactionParameters[i].paramN != 1.0) {
      Kf[i] = Kf[i] * pow(Temperature, ReactionParameters[i].n);
      /*} else {
        Kf[i] = Kf[i] * Temperature; // raise temp^1 = temp
      }//*/
    }

    /*
    cout <<
    //		Temperature << " , " <<
    //		exp(-ReactionParameters[i].paramEa/Temperature) << " , " <<
                    ReactionParameters[i].forward.A << " , " <<
                    ReactionParameters[i].forward.n << " , " <<
                    ReactionParameters[i].forward.Ea << " , " <<
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
#pragma omp barrier
}

void SolverCalculation::CalculateReactionRates(
    const vector<double> &Concentration, vector<double> Forward_Rates,
    vector<double> Reverse_Rates) {

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
