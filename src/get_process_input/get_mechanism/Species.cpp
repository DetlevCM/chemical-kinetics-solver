

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

  TLow = tlow;
  THigh = thigh;
  TChange = tchange;

  NasaLow = nasalow;
  NasaLow1 = nasalow[0];
  NasaLow2 = nasalow[1];
  NasaLow3 = nasalow[2];
  NasaLow4 = nasalow[3];
  NasaLow5 = nasalow[4];
  NasaLow6 = nasalow[5];
  NasaLow7 = nasalow[6];

  NasaHigh = nasahigh;
  NasaHigh1 = nasahigh[0];
  NasaHigh2 = nasahigh[1];
  NasaHigh3 = nasahigh[2];
  NasaHigh4 = nasahigh[3];
  NasaHigh5 = nasahigh[4];
  NasaHigh6 = nasahigh[5];
  NasaHigh7 = nasahigh[6];
}

double Species::ThermodynamicData::calculate_Hf_at_T(const ThermoT T) {
  if (T.T1 <= TChange) {
    return R * T.T1 *
           (NasaLow1 + NasaLow2 * T.T1 * 0.5 + NasaLow3 * T.T2 / 3.0 +
            NasaLow4 * T.T3 * 0.25 + NasaLow5 * T.T4 * 0.2 + NasaLow6 * T.InvT);
  } else {
    return R * T.T1 *
           (NasaHigh1 + NasaHigh2 * T.T1 * 0.5 + NasaHigh3 * T.T2 / 3.0 +
            NasaHigh4 * T.T3 * 0.25 + NasaHigh5 * T.T4 * 0.2 +
            NasaHigh6 * T.InvT);
  }
}

double Species::ThermodynamicData::calculate_Cp_at_T(const ThermoT T) {
  if (T.T1 <= TChange) {
    return R * (NasaLow1 + NasaLow2 * T.T1 + NasaLow3 * T.T2 + NasaLow4 * T.T3 +
                NasaLow5 * T.T4);
  } else {
    return R * (NasaHigh1 + NasaHigh2 * T.T1 + NasaHigh3 * T.T2 +
                NasaHigh4 * T.T3 + NasaHigh5 * T.T4);
  };
}

double Species::ThermodynamicData::calculate_Cv_at_T(const ThermoT T) {
  // skip ThermoT T(temperature); // temperatures -> T.T1
  return ThermodynamicData::calculate_Cp_at_T(T) - R;
}

double Species::ThermodynamicData::calculate_S_at_T(const ThermoT T) {
  if (T.T1 <= TChange) {
    return R * (NasaLow1 * T.logT + NasaLow2 * T.T1 + NasaLow3 * T.T2 * 0.5 +
                NasaLow4 * T.T3 / 3.0 + NasaLow5 * T.T4 * 0.25 + NasaLow7);
  } else {
    return R * (NasaHigh1 * T.logT + NasaHigh2 * T.T1 + NasaHigh3 * T.T2 * 0.5 +
                NasaHigh4 * T.T3 / 3.0 + NasaHigh5 * T.T4 * 0.25 + NasaHigh7);
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
