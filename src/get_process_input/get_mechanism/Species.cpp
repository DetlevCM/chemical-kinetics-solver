

#include <math.h>

#include "Species.h"

// function to cconvert vectorstring> of species into vector<class> for new
// design
vector<Species> Species::VectorClass(vector<string> species) {

  vector<Species> new_species_vec;
  new_species_vec.reserve(species.size());

  for (size_t i = 0; i < species.size(); i++) {
    Species new_species;
    new_species.Name = species[i];

    new_species_vec.push_back(new_species);
  }
  return new_species_vec;
}

void Species::ThermodynamicData::SetNasa(double tlow, double thigh,
                                         double tchange, vector<double> nasalow,
                                         vector<double> nasahigh) {

  nasa.TLow = tlow;
  nasa.THigh = thigh;
  nasa.TChange = tchange;

  // should never exceed 7 - there are always 7 entries
  for (size_t i = 0; i < 7; i++) {
    nasa.low[i] = nasalow[i];
    nasa.high[i] = nasahigh[i];
  }
}

double Species::ThermodynamicData::calculate_Hf_at_T(const ThermoT T) {
  if (T.T1 <= nasa.TChange) {
    return R * T.T1 *
           (nasa.low[0] + nasa.low[1] * T.T1 * 0.5 + nasa.low[2] * T.T2 / 3.0 +
            nasa.low[3] * T.T3 * 0.25 + nasa.low[4] * T.T4 * 0.2 +
            nasa.low[5] * T.InvT);
  } else {
    return R * T.T1 *
           (nasa.high[0] + nasa.high[1] * T.T1 * 0.5 +
            nasa.high[2] * T.T2 / 3.0 + nasa.high[3] * T.T3 * 0.25 +
            nasa.high[4] * T.T4 * 0.2 + nasa.high[5] * T.InvT);
  }
}

double Species::ThermodynamicData::calculate_Cp_at_T(const ThermoT T) {
  if (T.T1 <= nasa.TChange) {
    return R * (nasa.low[0] + nasa.low[1] * T.T1 + nasa.low[2] * T.T2 +
                nasa.low[3] * T.T3 + nasa.low[4] * T.T4);
  } else {
    return R * (nasa.high[0] + nasa.high[1] * T.T1 + nasa.high[2] * T.T2 +
                nasa.high[3] * T.T3 + nasa.high[4] * T.T4);
  };
}

double Species::ThermodynamicData::calculate_Cv_at_T(const ThermoT T) {
  // skip ThermoT T(temperature); // temperatures -> T.T1
  return calculate_Cp_at_T(T) - R;
}

double Species::ThermodynamicData::calculate_S_at_T(const ThermoT T) {
  if (T.T1 <= nasa.TChange) {
    return R * (nasa.low[0] * T.logT + nasa.low[1] * T.T1 +
                nasa.low[2] * T.T2 * 0.5 + nasa.low[3] * T.T3 / 3.0 +
                nasa.low[4] * T.T4 * 0.25 + nasa.low[6]);
  } else {
    return R * (nasa.high[0] * T.logT + nasa.high[1] * T.T1 +
                nasa.high[2] * T.T2 * 0.5 + nasa.high[3] * T.T3 / 3.0 +
                nasa.high[4] * T.T4 * 0.25 + nasa.high[6]);
  };
}

Species::ThermodynamicData::CalculatedThermodynamics
Species::ThermodynamicData::calculate_thermodynamics(const ThermoT T) {

  CalculatedThermodynamics output;

  output.Hf = calculate_Hf_at_T(T);
  output.Cp = calculate_Cp_at_T(T);
  output.Cv = calculate_Cv_at_T(T);
  output.S = calculate_S_at_T(T);

  return output;
}
