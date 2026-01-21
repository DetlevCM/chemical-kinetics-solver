
#include "./write_output.h"

/* This function write out the time, temperature and concentrations in the
 * species concentration file. It does NOT clear the file should it exist
 * and appends data to any existing file. See WriteLabelsSpecies for
 * the initial output which also clears the file. */

// output function for species labels, temperature and pressure at the back
void WriteOutput::WriteHeaders(vector<Species> species, bool GasPhasePressure,
                               size_t Number_Reactions) {

  // reason to open their own streams here is to open fresh output files
  ofstream conc_stream(filename_concentrations.c_str(), ios::out);

  if (conc_stream.is_open()) {
    conc_stream << "Time";
    for (size_t i = 0; i < species.size(); i++) {
      conc_stream << separator << species[i].Name;
    }
    conc_stream << separator << "Temperature";
    if (GasPhasePressure) {
      conc_stream << separator << "Pressure";
    }
    conc_stream << "\n";
    conc_stream.close();
  } else
    cout << "Unable to open file";

  if (print_rates) {
    ofstream rate_stream(filename_rates.c_str(), ios::out);

    if (rate_stream.is_open()) {
      rate_stream << "Time";
      for (size_t i = 0; i < Number_Reactions; i++) {
        rate_stream << separator << "Reaction(" << i << ")";
      }
      rate_stream << "\n";
      rate_stream.close();
    } else
      cout << "Unable to open file";
  }
}

void WriteOutput::StreamData(double CurrentTime, bool GasPhasePressure,
                             double Pressure,
                             const vector<double> &concentrations,
                             const vector<double> &rates) {

  stream_concentrations << CurrentTime;
  for (auto &concentration : concentrations) { // iterate over concentrations
    stream_concentrations << separator << concentration;
  }

  if (GasPhasePressure) {
    stream_concentrations << separator << Pressure;
  }
  stream_concentrations << "\n" << std::flush;

  if (print_rates) // reaction rates are options
  {
    stream_rates << CurrentTime; // time

    for (auto &rate : rates) { // iterate over rates
      stream_rates << separator << rate;
    }
    stream_rates << "\n" << std::flush;
  }

  // case petrooxy:
}
