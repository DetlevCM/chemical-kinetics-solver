/*
 * C_Process_Thermodynamics_Species_Classes.cpp
 *
 *  Created on: 20 Nov 2012
 *      Author: pmdcm
 */

#include "Mechanism_Reduction.h"

vector<Species> MechanismReduction::Process_Thermodynamics_Species_Classes(
    const vector<size_t> &SpeciesMapping, vector<Species> species) {

  /* For a start, do something idiotic, i.e. average all parameters
   * This will invariably lead to dodgy thermodynamic data, but I cannot think
   * of a better method right now...
   * And anyway, upgrade can be made to just this function.
   */

  vector<vector<double>> new_thermo;
  vector<double> SingleClassEntry;

  size_t i, j;

  size_t Number_Species = species.size();

  // Convert Thermodynamics for Mechanims Modification
  vector<vector<double>> ThermodynamicsOld;

  //*
  for (i = 0; i < Number_Species; i++) {

    vector<double> temp(17);

    temp[0] = species[i].thermodynamicdata.nasa.TLow;
    temp[1] = species[i].thermodynamicdata.nasa.THigh;
    temp[2] = species[i].thermodynamicdata.nasa.TChange;

    // temp[3] = species[i].thermodynamicdata.NasaLow1;
    for (size_t j = 0; j < 7; j++) {
      temp[j + 3] = species[i].thermodynamicdata.nasa.low[j];
    }
    for (size_t j = 0; j < 7; j++) {
      temp[j + 10] = species[i].thermodynamicdata.nasa.high[j];
    }

    ThermodynamicsOld.push_back(temp);
  } //*/

  size_t Number_Species_Classes = 0;
  // use abs() to get the absolute value
  for (i = 0; i < Number_Species; i++) {
    // The biggest number will be the last class (might be an individual species
    // or group)
    if (Number_Species_Classes < SpeciesMapping[i]) {
      Number_Species_Classes = SpeciesMapping[i];
    }
    // printf("Number Classes %i  Mapping: %i
    // \n",Number_Species_Classes,SpeciesMapping[i]);
  }
  Number_Species_Classes = Number_Species_Classes +
                           1; // even with class zero, there needs to be 1 space
  // printf("The system contain %u species classes! \n",Number_Species_Classes);

  // cout << "checkpoint 2 \n";

  /* we now know how many positions we require in our new output.
   * so we can now loop through the thermodynamic data, assigning the relevant
   * information as required In a final step we MUST remember to divide the
   * thermodynamic data after summing it all up.
   */

  /* remember that we have lost class 0 and species class 1 is in position 1 not
   * 0 which adds one array position. Then for some reason we need yet one extra
   * position, therefore + 2 */
  size_t Number_New_Thermo_Positions = Number_Species_Classes; // + 2;

  new_thermo.resize(Number_New_Thermo_Positions);
  SingleClassEntry.resize(18);

  // Clumsy method of initialising the new thermodynamics array with zeroes
  for (i = 0; i < Number_New_Thermo_Positions; i++) {
    for (j = 0; j < 17; j++) // make sure the matrix is initialized
    {
      SingleClassEntry[j] = 0;
    }
    new_thermo[i] = SingleClassEntry;
  }
  SingleClassEntry.clear();

  // cout << "checkpoint 3 \n";

  for (i = 0; i < Number_Species; i++) {
    size_t ClassID =
        SpeciesMapping[i]; // abs() is important, includes negative values...
    // printf("%i %i \n",i,ClassID);
    SingleClassEntry.resize(18); // 1 extra position for number of species

    SingleClassEntry =
        new_thermo[ClassID]; // obtain any existing thermodynamic data

    // Add value for species
    for (j = 0; j < 17; j++) {
      SingleClassEntry[j] = SingleClassEntry[j] + ThermodynamicsOld[i][j];
    }
    SingleClassEntry[17] = SingleClassEntry[17] + 1; // 1 species added to class

    new_thermo[ClassID] = SingleClassEntry;

    SingleClassEntry.clear(); // clear just to make sure
  }

  // cout << "checkpoint 4 \n";

  // now average values by class count:
  for (i = 0; i < Number_New_Thermo_Positions; i++) {
    for (j = 0; j < 17; j++) {
      new_thermo[i][j] = new_thermo[i][j] / new_thermo[i][17];
    }
  }

  // cout << "checkpoint 5 \n";

  /*
   * Write Thermodynamic Data into new format again
   */

  /// TODO: I am sure this has now become a mess...

  // Convert Thermodynamics for Mechanims Modification
  // Thermodynamics.clear(); // emtpy first, then populate again
  //*
  for (i = 0; i < Number_Species_Classes; i++) {

    // ThermodynamicData temp;

    Species temp;

    temp.Name = "";

    temp.thermodynamicdata.nasa.TLow = new_thermo[i][0];
    temp.thermodynamicdata.nasa.THigh = new_thermo[i][1];
    temp.thermodynamicdata.nasa.TChange = new_thermo[i][2];

    // temp.thermodynamicdata.NasaLow1 = new_thermo[i][3];
    for (size_t j = 0; j < 7; j++) {
      temp.thermodynamicdata.nasa.low[j] = new_thermo[i][3 + j];
    }
    //  temp.thermodynamicdata.NasaHigh1 = new_thermo[i][10];
    for (size_t j = 0; j < 7; j++) {
      temp.thermodynamicdata.nasa.high[j] = new_thermo[i][10 + j];
    }

    // Thermodynamics.push_back(temp);
    species.push_back(temp);
  } //*/

  // return Thermodynamics;
  return species;
}
