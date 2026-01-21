/*
 * Write_Output_Functions.h
 *
 *  Created on: 20 Dec 2012
 *      Author: pmdcm
 */

#ifndef WRITE_OUTPUT
#define WRITE_OUTPUT

#include <iostream>
using std::ofstream;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "../global_struct.h"

#include "../get_process_input/get_mechanism/ReactionMechanism.h"

class WriteOutput {

private:
  string separator; // character to use as separator

  string prefix = "";

  string filename_concentrations;
  string filename_rates;
  string filename_petrooxy;

  ofstream stream_concentrations;
  ofstream stream_rates;

public:
  /*|-------------------------------|*/
  /*|-- Functions to write output --|*/
  /*|-------------------------------|*/

  void set_output(string sep, string name_concentrations, string name_rates,
                  string name_petrooxy) {
    separator = sep;
    filename_concentrations = name_concentrations;
    filename_rates = name_rates;
    filename_petrooxy = name_petrooxy;
  };

  void set_prefix(string pref) { prefix = pref; }
  string get_prefix() { return prefix; }

  string get_name_concentration() { return filename_concentrations; };
  string get_name_rates() { return filename_rates; };
  string get_name_petrooxy() { return filename_petrooxy; };

  void open_stream_concentrations() {
    stream_concentrations = ofstream(filename_concentrations.c_str(), ios::app);
  };
  void open_stream_rates() {
    stream_rates = ofstream(filename_rates.c_str(), ios::app);
  };

  void close_stream_concentrations() { stream_concentrations.close(); };
  void close_stream_rates() { stream_rates.close(); };

  // Function to write the labels

  // Write labels for concentrations, then stream output
  void Write_Header_Species_Temperature_Pressure(
      // string filename,
      // string separator,
      size_t Number_Species, vector<Species> species, bool GasPhasePressure);

  void StreamConcentrations( // ofstream &Concentration_OFStream,
                             //       const string separator,
      bool GasPhasePressure, size_t Number_Species, double CurrentTime,
      double Pressure, const vector<double> &Concentration);

  void StreamConcentrationsV2( // ofstream &Concentration_OFStream,
                               //       const string separator,
      double CurrentTime, size_t Number_Species, double *Concentration);

  // write labels for reactions then stream output
  void WriteLabelsReactionRates(
      // string, string,
      size_t);

  void StreamReactionRates( // ofstream &,
      const string, double, const vector<double> &Rates);

  // output of input files

  static void Write_Stoichiometric_Matrix_For_Opt(
      string,
      //		const vector< string >& ,
      const vector<SingleReactionData> &);
  static void Input_File_For_Ehsan_Opt(string,
                                       const vector<SingleReactionData> &);
};

#endif
