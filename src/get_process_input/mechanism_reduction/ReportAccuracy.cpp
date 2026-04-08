/*
 * ReportAccuracy.cpp
 *
 *  Created on: 9 Oct 2014
 *      Author: DetlevCM
 */

#include "./Mechanism_Reduction.h"

void MechanismReduction::ReportAccuracy(string separator, size_t Number_Species,
                                        vector<Species> species,
                                        string output_filename,
                                        string base_concentrations,
                                        string reduced_concentrations) {

  ifstream Input_Data_Full;
  Input_Data_Full.open(base_concentrations.c_str());

  ifstream Input_Data_Reduced;
  Input_Data_Reduced.open(reduced_concentrations.c_str());

  // position {0} is time, [Number_Species + 1] is temperature

  vector<double> OutputReview(Number_Species + 2);
  vector<double> MaxDiff(Number_Species + 2);
  vector<double> MinDiff(Number_Species + 2);

  double number_lines = 0;

  if (Input_Data_Full.is_open() && Input_Data_Reduced.is_open()) {
    string Line1, Line2;

    getline(Input_Data_Full, Line1);
    getline(Input_Data_Reduced, Line2);

    while (Input_Data_Full.good() && Input_Data_Reduced.good()) {

      getline(Input_Data_Full, Line1);
      getline(Input_Data_Reduced, Line2);

      vector<double> DataFull, DataReduced;
      DataFull = Tokenise_String_To_Double(Line1, separator);
      DataReduced = Tokenise_String_To_Double(Line2, separator);

      if (DataFull.size() != 0 && DataReduced.size() != 0) {
        for (size_t i = 1; i <= Number_Species; i++) {
          double difference;

          if (DataFull[i] > 1.0e-23) {
            difference = ((fabs(DataFull[i]) - fabs(DataReduced[i])) /
                          fabs(DataFull[i]));

            OutputReview[i] = OutputReview[i] + difference;

            if (MaxDiff[i] < difference) {
              MaxDiff[i] = difference;
            }
            if (MinDiff[i] > difference) {
              MinDiff[i] = difference;
            }
          }
        }
        number_lines = number_lines + 1;
      }
    }

    Input_Data_Full.close();
    Input_Data_Reduced.clear();

    // average and turn into %
    for (size_t i = 1; i <= Number_Species; i++) {
      OutputReview[i] = 100 * OutputReview[i] / number_lines;
      MaxDiff[i] = 100 * MaxDiff[i];
      MinDiff[i] = 100 * MinDiff[i];
    }

    ofstream OutputFile;

    OutputFile.open(output_filename.c_str());

    OutputFile << "Species Name 	Average Difference in % 	Min "
                  "Diff in % 	Max Diff in % \n";

    for (size_t i = 1; i <= Number_Species; i++) {
      OutputFile << species[i - 1].Name << " 	" << OutputReview[i]
                 << " 	" << MinDiff[i] << " 	" << MaxDiff[i] << " 	"
                 << " \n";
    }
  }
}
