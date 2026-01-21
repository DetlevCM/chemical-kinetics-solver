
#include "./write_output.h"

/* This function write out the time, temperature and concentrations in the
 * species concentration file. It does NOT clear the file should it exist
 * and appends data to any existing file. See WriteLabelsSpecies for
 * the initial output which also clears the file. */

// output function for species labels, temperature at back
void WriteOutput::Write_Header_Species_Temperature_Pressure(
    size_t Number_Species, vector<Species> species, bool GasPhasePressure) {
  size_t i;
  ofstream Concentration_OFStream(filename_concentrations.c_str(), ios::out);

  if (Concentration_OFStream.is_open()) {
    Concentration_OFStream << "Time";
    for (i = 0; i < Number_Species; i++) {
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
}

/*
// Function to stream the concentrations
void WriteOutput::StreamConcentrations(bool GasPhasePressure,
                                       size_t Number_Species,
                                       double CurrentTime, double Pressure,
                                       const vector<double> &Concentration) {
  size_t i;
  stream_concentrations << CurrentTime;
  for (i = 0; i <= Number_Species; i++) {
    stream_concentrations << separator << Concentration[i];
  }
  if (GasPhasePressure) {
    stream_concentrations << separator << Pressure;
  }
  stream_concentrations << "\n" << std::flush;
}
//*/

/*
void WriteOutput::StreamReactionRates(
                                      double CurrentTime,
                                      const vector<double> &ReactionRates) {
  size_t i;
  stream_rates << CurrentTime; // time

  for (i = 0; i < ReactionRates.size(); i++) {
    stream_rates << separator << ReactionRates[i];
  }
  stream_rates << "\n" << std::flush;
}
//*/

void WriteOutput::StreamData(stream_type type, double CurrentTime,
                             bool GasPhasePressure, double Pressure,
                             const vector<double> &data) {
  switch (type) {
  case concentration:
    stream_concentrations << CurrentTime;
    for (size_t i = 0; i < data.size(); i++) {
      stream_concentrations << separator << data[i];
    }
    if (GasPhasePressure) {
      stream_concentrations << separator << Pressure;
    }
    stream_concentrations << "\n" << std::flush;
    break;
  case rates:
    stream_rates << CurrentTime; // time

    for (size_t i = 0; i < data.size(); i++) {
      stream_rates << separator << data[i];
    }
    stream_rates << "\n" << std::flush;
    break;
    // case petrooxy:
  }
}

/*
void WriteOutput::StreamConcentrationsV2(double CurrentTime,
                                         size_t Number_Species,
                                         double *Concentration) {
  size_t i;
  stream_concentrations << CurrentTime << separator;

  for (i = 0; i < Number_Species; i++) {
    stream_concentrations << Concentration[i] << separator;
  }
  stream_concentrations << Concentration[Number_Species] << separator;
  stream_concentrations << "\n" << std::flush;
}//*/

// output function for species labels, temperature at back
void WriteOutput::WriteLabelsReactionRates(size_t Number_Reactions) {
  size_t i;
  ofstream OutputFile(filename_rates.c_str(), ios::out);

  if (OutputFile.is_open()) {
    OutputFile << "Time";
    for (i = 0; i < Number_Reactions; i++) {
      OutputFile << separator << "Reaction(" << i << ")";
    }
    OutputFile << "\n";
    OutputFile.close();
  } else
    cout << "Unable to open file";
}
