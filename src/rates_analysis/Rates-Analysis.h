/*
 * Rates_Analysis.h
 *
 *  Created on: 19 Dec 2014
 *      Author: DetlevCM
 */

#ifndef RATES_ANALYSIS
#define RATES_ANALYSIS

#include <iostream>
using std::ofstream;
// using std::ifstream;
using std::ios;

#include <vector>
using std::vector;

#include <string>
using std::string;

#include "../global_struct.h"

#include "../get_process_input/get_mechanism/Reaction.h"
#include "../get_process_input/get_mechanism/Species.h"

//// TODO:
using Reaction::SingleReactionData; // remove in future or optimize

class RatesAnalysis {

public:
  static void MaxRatesAnalysis(vector<vector<str_RatesAnalysis>> &,
                               const vector<TrackSpecies> &,
                               const vector<TrackSpecies> &,
                               const vector<double>, const double);

  static void RatesAnalysisAtTimes(const vector<TrackSpecies> &,
                                   const vector<TrackSpecies> &,
                                   const vector<double> &, const double,
                                   const vector<Species> species,
                                   const vector<SingleReactionData> &);

  static void WriteMaxRatesAnalysis(vector<vector<str_RatesAnalysis>> &,
                                    const vector<Species> &species,
                                    const vector<SingleReactionData> &, string);

  static void PrepareStreamRatesAnalysis(const vector<Species> &species,
                                         string);

  static void StreamRatesAnalysis(const string, const vector<TrackSpecies> &,
                                  const vector<TrackSpecies> &,
                                  const vector<double> &, const double,
                                  const size_t);

  // New function for rates of production

  static void Prepare_Print_Rates_Per_Species(
      // vector< TrackSpecies > & ,
      // vector< TrackSpecies > & ,
      string,
      // vector< double > & ,
      vector<Species> species, vector<size_t>,
      vector<vector<size_t>> //,
      // vector< SingleReactionData >&
  );

  static void Print_Rates_Per_Species(vector<TrackSpecies> &,
                                      vector<TrackSpecies> &, const string,
                                      vector<double> &, double,
                                      vector<Species> species, vector<size_t>,
                                      vector<vector<size_t>> //,
                                      // vector< SingleReactionData >&
  );
};

#endif
