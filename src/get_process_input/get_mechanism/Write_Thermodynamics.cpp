/*
 * C_Write_Thermodynamics.cpp
 *
 *  Created on: 20 Nov 2012
 *      Author: pmdcm
 */

#include "./Species.h"
// Ofstream reference: http://www.cplusplus.com/reference/fstream/ofstream/

void Species::ThermodynamicData::Write_Thermodynamic_Data(
    string filename, const vector<Species> &species
    // const vector< ThermodynamicData >& Thermodynamics
) {
  size_t i;
  ofstream ThermodynamicsOutput(filename.c_str(), ios::out);

  if (ThermodynamicsOutput.is_open()) {
    size_t Number_Species = species.size();
    ThermodynamicsOutput << "ThermData \n";
    for (i = 0; i < Number_Species; i++) {
      ThermodynamicsOutput << species[i].Name << "\n";

      ThermodynamicsOutput << species[i].thermodynamicdata.TLow << "	"
                           << species[i].thermodynamicdata.THigh << "	"
                           << species[i].thermodynamicdata.TChange

                           << "\n"
                           << species[i].thermodynamicdata.NasaLow1 << "	"
                           << species[i].thermodynamicdata.NasaLow2 << "	"
                           << species[i].thermodynamicdata.NasaLow3 << "	"
                           << species[i].thermodynamicdata.NasaLow4 << "	"
                           << species[i].thermodynamicdata.NasaLow5 << "	"
                           << species[i].thermodynamicdata.NasaLow6 << "	"
                           << species[i].thermodynamicdata.NasaLow7

                           << "\n"
                           << species[i].thermodynamicdata.NasaHigh1 << "	"
                           << species[i].thermodynamicdata.NasaHigh2 << "	"
                           << species[i].thermodynamicdata.NasaHigh3 << "	"
                           << species[i].thermodynamicdata.NasaHigh4 << "	"
                           << species[i].thermodynamicdata.NasaHigh5 << "	"
                           << species[i].thermodynamicdata.NasaHigh6 << "	"
                           << species[i].thermodynamicdata.NasaHigh7 << "\n";

      // One more new line to separate species
      ThermodynamicsOutput << "\n"; // new line
    }
    ThermodynamicsOutput << "End \n";
    ThermodynamicsOutput.close();
  } else
    cout << "Unable to open file";
  cout << "File " << filename << " written. \n";
}
