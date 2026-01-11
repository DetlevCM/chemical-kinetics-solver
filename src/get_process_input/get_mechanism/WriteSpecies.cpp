/*
 * WriteSpecies.cpp
 *
 *  Created on: 9 Jan 2013
 *      Author: DetlevCM
 */

#include "./Species.h"

void Species::WriteSpecies(string filename, const vector<Species> &species) {
  size_t i;
  ofstream SpeciesOutput(filename.c_str(), ios::out);

  if (SpeciesOutput.is_open()) {
    size_t Number_Species = 0;
    Number_Species = species.size();
    SpeciesOutput << "SPECIES\n";
    for (i = 0; i < Number_Species; i++) {

      SpeciesOutput << "\t" << species[i].Name << "\n";
    }
    SpeciesOutput << "END\n";
    SpeciesOutput.close();
  } else
    cout << "Unable to open file";
  cout << "File " << filename << " written.\n";
}
