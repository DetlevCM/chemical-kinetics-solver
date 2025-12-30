

#ifndef _SPECIES
#define _SPECIES

#include "../../global_const.h"
#include "../../helpers/Helpers.h"

// File Streams and IO
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>

#include <vector>
using std::vector ;

#include <string>
using std::string;

using std::ofstream;
using std::ifstream;
using std::ios;


//// TODO: Make the class contain the whole vector of species 

class Species {



public:
string Name;

static vector< string > Get_Species(string filename);

static vector<Species> VectorClass(vector<string> species);

static void Get_Thermodynamic_Data(
		string filename,
		vector< Species >& species
);

public:

class ThermodynamicData {

//private:
// want to make these private in the future
public: 
	double TLow;
	double THigh;
	double TChange;

    vector<double> NasaLow;
//	vector<double> nasalow(7);
//	NasaLow.reserve(7);
    //NasaLow.resize(7);

//*
	double NasaLow1;
	double NasaLow2;
	double NasaLow3;
	double NasaLow4;
	double NasaLow5;
	double NasaLow6;
	double NasaLow7;//*/

    vector<double> NasaHigh;
//	vector<double> nasahigh(7);
//	NasaHigh.reserve(7);
    //NasaHigh.resize(7);

	//*
    double NasaHigh1;
	double NasaHigh2;
	double NasaHigh3;
	double NasaHigh4;
	double NasaHigh5;
	double NasaHigh6;
	double NasaHigh7;
    //*/



//ThermodynamicData(
//	double tlow, double thigh, double tchange, 
//	vector<double> nasalow, vector<double> nasahigh
//){


    

//};

public:

void SetNasa(double tlow,double thigh,double tchange,vector<double> nasalow, vector<double> nasahigh);

double calculate_Hf_at_T(const double temperature);
double calculate_Cp_at_T(const double temperature);
double calculate_Cv_at_T(const double temperature);
double calculate_S_at_T(const double temperature);

static void Write_Thermodynamic_Data(
		string filename,
		const vector< Species >& species//,
		//const vector< ThermodynamicData >& Thermodynamics
);

};

ThermodynamicData thermodynamicdata;

void WriteSpecies(
		string ,
		const vector< string >&
);


};

#endif