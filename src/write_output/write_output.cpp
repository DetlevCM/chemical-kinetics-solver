
#include "./write_output.h"

/* This function write out the time, temperature and concentrations in the
 * species concentration file. It does NOT clear the file should it exist
 * and appends data to any existing file. See WriteLabelsSpecies for
 * the initial output which also clears the file. */

// output function for species labels, temperature and pressure at the back
void WriteOutput::WriteHeaders(vector<Species> species, bool GasPhasePressure,
                               size_t Number_Reactions) {

  // reason to open their own streams here is to open fresh output files
  ofstream Concentration_OFStream(filename_concentrations.c_str(), ios::out);

  if (Concentration_OFStream.is_open()) {
    Concentration_OFStream << "Time";
    for (size_t i = 0; i < species.size(); i++) {
      Concentration_OFStream << separator << species[i].Name;
    }
    Concentration_OFStream << separator << "Temperature";
    if (GasPhasePressure) {
      Concentration_OFStream << separator << "Pressure";
    }
    Concentration_OFStream << "\n";
    Concentration_OFStream.close();
  } else
    cout << "Unable to open file";

  if (print_rates) {
    ofstream OutputFile(filename_rates.c_str(), ios::out);

    if (OutputFile.is_open()) {
      OutputFile << "Time";
      for (size_t i = 0; i < Number_Reactions; i++) {
        OutputFile << separator << "Reaction(" << i << ")";
      }
      OutputFile << "\n";
      OutputFile.close();
    } else
      cout << "Unable to open file";
  }
}

void WriteOutput::StreamData(double CurrentTime, bool GasPhasePressure,
                             double Pressure,
                             const vector<double> &concentration,
                             const vector<double> &rates) {

  stream_concentrations << CurrentTime;
  for (size_t i = 0; i < concentration.size(); i++) {
    stream_concentrations << separator << concentration[i];
  }
  if (GasPhasePressure) {
    stream_concentrations << separator << Pressure;
  }
  stream_concentrations << "\n" << std::flush;

  if (print_rates) // reaction rates are options
  {
    stream_rates << CurrentTime; // time

    for (size_t i = 0; i < rates.size(); i++) {
      stream_rates << separator << rates[i];
    }
    stream_rates << "\n" << std::flush;
  }

  // case petrooxy:
}
