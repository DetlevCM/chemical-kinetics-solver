#include "Species.h"

// http://stackoverflow.com/questions/7677007/passing-vector-to-function-c
// Pass vector by reference as input needs not be changed
void Species::Get_Thermodynamic_Data(string filename,
                                     vector<Species> &species) {

  /*
   * Design for new Thermodynamics Data Input
   * Species Name
   * 3 temperatures - may be followed by elemental parameters
   * 1 line low values
   * 1 line high values
   */

  ifstream Mechanism_Data;
  Mechanism_Data.open(filename.c_str());

  size_t Number_Species = species.size();
  size_t i;

  size_t ThermoCounter = 0;
  // Vector to store all data as a struct (more efficient than vector< vector> >
  // vector< Species::ThermodynamicData > read_in_thermodynamics;
  // read_in_thermodynamics.resize(Number_Species);

  if (Mechanism_Data.is_open()) {
    string line1;
    size_t found;
    bool begin_flag = false;
    bool end_flag = false;

    while (Mechanism_Data.good()) {
      getline(Mechanism_Data, line1);

      //  Check if the Thermodynamic Data is Over and terminate the scheme
      found =
          line1.find("End"); // need to check for end in loop for 4 line blocks

      if (found != string::npos && begin_flag) {
        end_flag = true;
      }

      if (begin_flag && !end_flag) {
        //  Check that the End has not been reached
        //  Check that the Line is not empty
        //  Check the initial character is not !, whitespace or tab
        //
        //  Does NOT filter blank lines with more than one white space...
        //
        if (!end_flag && !line1.empty() &&
            line1.compare(0, 1, "!") !=
                0 //&&
                  // line1.compare(0,1," ") != 0 && // normal whitespace
                  // line1.compare(0,1,"	") != 0 // tab
        ) {
          // One Species
          // Species::ThermodynamicData temp_read_in_single_species;

          // split the line by white spaces or tabs

          vector<string> temp_split_line;
          vector<double> temp_split_line_double1;
          vector<double> temp_split_line_double2;
          vector<double> temp_split_line_double3;

          temp_split_line = Tokenise_String_To_String(line1, " 	");

          // for now we just want the species name, that is the initial input
          // Species Name is now temp_split_line[0]
          // Now we need to identify the species

          size_t temp_species_id = 0;

          for (i = 0; i < Number_Species; i++) {
            string Comparator = species[i].Name;
            // Find the appropriate species to go with the thermodynamic data
            if (strcmp(temp_split_line[0].c_str(), Comparator.c_str()) == 0) {
              temp_species_id = i; // mapping.push_back(i); // retain a map of
                                   // the arrangement of the species
            }
          }
          temp_split_line.clear();

          // now the 3 temperature readings
          getline(Mechanism_Data, line1);
          temp_split_line_double1 = Tokenise_String_To_Double(line1, " 	");

          // now the low thermodynamics
          getline(Mechanism_Data, line1);
          temp_split_line_double2 = Tokenise_String_To_Double(line1, " 	");

          // now the high thermodynamics
          getline(Mechanism_Data, line1);
          temp_split_line_double3 = Tokenise_String_To_Double(line1, " 	");

          std::cout << line1 << "\n";
          std::cout << temp_split_line_double1[0] << "\n";
          species[temp_species_id].thermodynamicdata.SetNasa(
              temp_split_line_double1[0], temp_split_line_double1[1],
              temp_split_line_double1[2], temp_split_line_double2,
              temp_split_line_double3);

          ThermoCounter = ThermoCounter + 1;
        }
      }

      // Check if the Thermodynamics Data Begins, if yes set a start flag
      found = line1.find("ThermData");
      if (found != string::npos && !end_flag) {
        begin_flag = true;
      }
    }
    Mechanism_Data.close();
  }

  Mechanism_Data.open(filename.c_str());

  // standard data structure
  if (Mechanism_Data.is_open()) {

    string line1;
    size_t found;
    bool begin_flag = false;
    bool end_flag = false;

    while (Mechanism_Data.good()) {
      getline(Mechanism_Data, line1);

      if (begin_flag && !end_flag) {

        found = line1.find(
            "END"); // need to check for end in loop for 4 line blocks
        if (found != string::npos && begin_flag) {
          end_flag = true;
        }

        if (!end_flag && !line1.empty() &&
            line1.compare(0, 1, "!") !=
                0) { // Abort if end reACHED... or empty or comment
          // Species::ThermodynamicData temp_read_in_single_species;

          // http://www.cplusplus.com/reference/string/string/substr/
          // Substrings, exploit format
          /* First 17 characters are the species name
                          Next 31 characters are just clutter
                          Followed by 40 characters of temperatures
                          The next 3 lines are 15 character blocks
           */

          string line2, line3, line4;
          getline(Mechanism_Data, line2);
          getline(Mechanism_Data, line3);
          getline(Mechanism_Data, line4);

          string species_name, change_Temps;

          species_name = line1.substr(0, 17); // contains name of species
          change_Temps =
              line1.substr(46, 30); // contains low_T, high_T,change_T

          // Strip out whitespaces from species name
          istringstream iss1(species_name);
          string sub1;
          iss1 >> sub1;
          if (!sub1.empty()) { // avoid empty lines/entries
            species_name = sub1;
          }
          // End stripping out whitespaces

          size_t temp_species_id = 0;

          for (i = 0; i < Number_Species; i++) {
            string Comparator = species[i].Name;
            // Find the appropriate species to go with the thermodynamic data
            if (strcmp(species_name.c_str(), Comparator.c_str()) == 0) {
              temp_species_id = i; // mapping.push_back(i); // retain a map of
                                   // the arrangement of the species
            }
          }

          vector<double> SingleSpeciesData; // One row of data for one species

          // Process the 3 temperature entries low_T, high_T, change_T
          istringstream iss2(change_Temps);
          do {
            string sub;
            iss2 >> sub;
            if (!sub.empty()) { // avoid empty lines
              SingleSpeciesData.push_back(strtod(sub.c_str(), NULL));
            }
          } while (iss2);

          double TLow = SingleSpeciesData[0];
          double THigh = SingleSpeciesData[1];
          double TChange = SingleSpeciesData[2];

          vector<double> NasaHigh(7);
          vector<double> NasaLow(7);
          NasaHigh[0] = strtod(line2.substr(0, 15).c_str(), NULL);
          NasaHigh[1] = strtod(line2.substr(15, 15).c_str(), NULL);
          NasaHigh[2] = strtod(line2.substr(30, 15).c_str(), NULL);
          NasaHigh[3] = strtod(line2.substr(45, 15).c_str(), NULL);
          NasaHigh[4] = strtod(line2.substr(60, 15).c_str(), NULL);

          NasaHigh[5] = strtod(line3.substr(0, 15).c_str(), NULL);
          NasaHigh[6] = strtod(line3.substr(15, 15).c_str(), NULL);
          NasaLow[0] = strtod(line3.substr(30, 15).c_str(), NULL);
          NasaLow[1] = strtod(line3.substr(45, 15).c_str(), NULL);
          NasaLow[2] = strtod(line3.substr(60, 15).c_str(), NULL);

          NasaLow[3] = strtod(line4.substr(0, 15).c_str(), NULL);
          NasaLow[4] = strtod(line4.substr(15, 15).c_str(), NULL);
          NasaLow[5] = strtod(line4.substr(30, 15).c_str(), NULL);
          NasaLow[6] = strtod(line4.substr(45, 15).c_str(), NULL);

          species[temp_species_id].thermodynamicdata.SetNasa(
              TLow, THigh, TChange, NasaLow, NasaHigh);

          ThermoCounter = ThermoCounter + 1;
        }
      }

      // Moving thermo data check to end of function avoids "THERMO" being read
      // in as a name
      found = line1.find("THERM");
      if (found != string::npos && !end_flag) {
        // cout << "found begining of thermo \n";
        begin_flag = true;
        getline(Mechanism_Data, line1);
      }
    }

    Mechanism_Data.close();
  }
  cout << ThermoCounter << " Thermodynamic entries processed.\n";
}
