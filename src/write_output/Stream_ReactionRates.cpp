
#include "./write_output.h"

/* This function write out the time, temperature and concentrations in the
 * species concentration file. It does NOT clear the file should it exist
 * and appends data to any existing file. See WriteLabelsSpecies for
 * the initial output which also clears the file. */

// output function for species labels, temperature at back
void WriteOutput::WriteLabelsReactionRates(string filename, string separator,
                                           size_t Number_Reactions) {
  size_t i;
  ofstream OutputFile(filename.c_str(), ios::out);

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

void WriteOutput::StreamReactionRates(ofstream &OutputFile,
                                      const string separator,
                                      double CurrentTime,
                                      vector<double> &ReactionRates) {
  size_t i;
  OutputFile << CurrentTime; // time

  for (i = 0; i < ReactionRates.size(); i++) {
    OutputFile << separator << ReactionRates[i];
  }
  OutputFile << "\n" << std::flush;
}
