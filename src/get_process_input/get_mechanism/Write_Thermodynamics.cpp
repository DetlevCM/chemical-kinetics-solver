/*
 * C_Write_Thermodynamics.cpp
 *
 *  Created on: 20 Nov 2012
 *      Author: pmdcm
 */

#include "./Species.h"
// Ofstream reference: http://www.cplusplus.com/reference/fstream/ofstream/

void Species::ThermodynamicData::Write_Thermodynamic_Data(
    string filename, const vector<Species> &species) {
  size_t i;
  ofstream ThermodynamicsOutput(filename.c_str(), ios::out);

  if (ThermodynamicsOutput.is_open()) {
    size_t Number_Species = species.size();
    ThermodynamicsOutput << "ThermData \n";
    for (i = 0; i < Number_Species; i++) {
      ThermodynamicsOutput << species[i].Name << "\n";

      ThermodynamicsOutput << species[i].thermodynamicdata.nasa.TLow << "	"
                           << species[i].thermodynamicdata.nasa.THigh << "	"
                           << species[i].thermodynamicdata.nasa.TChange

                           << "\n";
      for (size_t j = 0; j < 7; j++) {
        ThermodynamicsOutput << species[i].thermodynamicdata.nasa.low[j]
                             << "	";
      }
      ThermodynamicsOutput << "\n";
      for (size_t j = 0; j < 7; j++) {
        ThermodynamicsOutput << species[i].thermodynamicdata.nasa.high[j]
                             << "	";
      }
      ThermodynamicsOutput << "\n";

      // One more new line to separate species
      ThermodynamicsOutput << "\n"; // new line
    }
    ThermodynamicsOutput << "End \n";
    ThermodynamicsOutput.close();
  } else
    cout << "Unable to open file";
  cout << "File " << filename << " written. \n";
}
