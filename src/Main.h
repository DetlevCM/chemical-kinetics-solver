
#ifndef _MAIN
#define _MAIN

#include "global_const.h"
#include "helpers/Helpers.h"

#include "./get_process_input/get_initial_data/Initial_Data.h"
#include "./get_process_input/get_mechanism/Species.h"
#include "./get_process_input/get_mechanism/ReactionMechanism.h"

// File Streams and IO
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>

// type conversion
#include <stdlib.h>

// maths
#include <math.h>

// timekeeping
#include <time.h>

// variable types
#include <vector>
#include <string>

// for the case insensitive search
#include <cctype>
#include <algorithm>

// Needed globally for the function definitions
//using namespace std;

// specific components of namespace std
using std::vector;
using std::cout;
using std::string;

using std::ofstream;
using std::ifstream;
using std::ios;

using std::ostringstream;
using std::stringstream;
using std::istringstream;

using std::stod; // for C++ string to double

class Main {

public:
struct Filenames{
	/*
	 * This strcut collects the file names for the
	 * input files required during a run.
	 *
	 * To collate data, output files are equally treated in the
	 * same strcut, together together potentially with output flags.
	 */

	string mechanism;
	string initial_data;
	string species_mapping;


	// this a mess, there is an overlap... 
	string Species;
	string Rates;
	string PetroOxy;
	string Prefix;
};


};

//void Handle_Mechanism_Input(
bool Handle_Mechanism_Input(
		Main::Filenames filenames,
		ReactionMechanism & reactions_mechanism ,
		Initial_Data& //,
		//vector< double >&,
//		PressureVesselCalc&
);

void Process_User_Input(
		Main::Filenames& filenames,
		vector<string> User_Inputs
);



// Making Scheme Irreversible
vector< SingleReactionData > Make_Irreversible(
		vector< SingleReactionData > Reactions,
		const vector< Species > species,
		double Initial_Temperature, /// use initial temperature from initial data
		double Range // specify +/- range around initial temperature
);

void Synchronize_Gas_Liquid_Model(
		size_t number_synchronized_species,
		size_t liquid_species_count, size_t gas_species_count, // gas and liquid counts so I know where concentration entries belong to
		double *y, // concentrations (&temperature) from the ODE solver
		double Vliq_div_Vgas,
		vector< double > Henry_Constants // need to line up with species IDs
);

#endif