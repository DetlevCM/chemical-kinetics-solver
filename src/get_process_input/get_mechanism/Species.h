

#ifndef _SPECIES
#define _SPECIES

#include "../../global_const.h"
#include "../../helpers/Helpers.h"

// File Streams and IO
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <iomanip>
using std::ifstream;
using std::ios;
using std::ofstream;

//// TODO: Make the class contain the whole vector of species

class Species {

public:
  string Name;

  static vector<string> Get_Species(string filename);

  static vector<Species> VectorClass(vector<string> species);

  static void Get_Thermodynamic_Data(string filename, vector<Species> &species);

public:
  class ThermodynamicData {

    // private:
    //  want to make these private in the future
  public:
    struct NASA {
      double TLow;
      double THigh;
      double TChange;

      // why use a vector, when an array has a set size that will never change?
      double low[7];
      double high[7];
    };

    NASA nasa;

  public:
    class ThermoT {
    public:
      double T1;
      double T2;
      double T3;
      double T4;
      double logT;
      double InvT;

      ThermoT(double temperature) {
        T1 = temperature;
        T2 = temperature * temperature;
        T3 = T2 * temperature;
        T4 = T3 * temperature;
        logT = log(temperature);
        InvT = 1.0 / temperature;
      };
    };

    struct CalculatedThermodynamics {
      double Hf;
      double Cp;
      double Cv;
      double S;
    };

    void SetNasa(double tlow, double thigh, double tchange,
                 vector<double> nasalow, vector<double> nasahigh);

    double calculate_Hf_at_T(const ThermoT T);
    double calculate_Cp_at_T(const ThermoT T);
    double calculate_Cv_at_T(const ThermoT T);
    double calculate_S_at_T(const ThermoT T);

    CalculatedThermodynamics calculate_thermodynamics(const ThermoT T);

    static void Write_Thermodynamic_Data(string filename,
                                         const vector<Species> &species);
  };

  ThermodynamicData thermodynamicdata;

  static void WriteSpecies(string, const vector<Species> &);
};

#endif
