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

  bool print_rates;

public:
  /*|-------------------------------|*/
  /*|-- Functions to write output --|*/
  /*|-------------------------------|*/

  void set_output(string sep, string name_concentrations, string name_rates,
                  string name_petrooxy, bool bool_print_rates) {
    separator = sep;
    filename_concentrations = name_concentrations;
    filename_rates = name_rates;
    filename_petrooxy = name_petrooxy;

    print_rates = bool_print_rates;
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

  void WriteHeaders(vector<Species> species, bool GasPhasePressure,
                    size_t Number_Reactions);

  void StreamData(double CurrentTime, bool GasPhasePressure, double Pressure,
                  const vector<double> &concentration,
                  const vector<double> &rates);

  // output of input files
  static void
  Write_Stoichiometric_Matrix_For_Opt(string,
                                      const vector<SingleReactionData> &);
  static void Input_File_For_Ehsan_Opt(string,
                                       const vector<SingleReactionData> &);
};

#endif
