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

#include "../get_process_input/get_mechanism/ReactionMechanism.h"

#include "../global_struct.h"

class WriteOutput
{

	public:


/*|-------------------------------|*/
/*|-- Functions to write output --|*/
/*|-------------------------------|*/

// Function to write the labels


// Write labels for concentrations, then stream output
static void Write_Header_Species_Temperature_Pressure(
		string filename,
		string separator,
		size_t Number_Species,
		vector< Species > species,
		bool GasPhasePressure
);

static void StreamConcentrations(
		ofstream& Concentration_OFStream,
		const string separator,
		bool GasPhasePressure,
		size_t Number_Species,
		double CurrentTime,
		double Pressure,
		const vector<double >& Concentration
);

static void StreamConcentrationsV2(
		ofstream& Concentration_OFStream,
		const string separator,
		double CurrentTime,
		size_t Number_Species,
		double* Concentration
);

// write labels for reactions then stream output
static void WriteLabelsReactionRates(
		string ,
		string ,
		size_t
);


static void StreamReactionRates(
		ofstream&,
		const string,
		double,
		vector< double> &
);



// output of input files


static void Write_Stoichiometric_Matrix_For_Opt(
		string ,
		//		const vector< string >& ,
		const vector< SingleReactionData >&
);
static void Input_File_For_Ehsan_Opt(
		string ,
		const vector< SingleReactionData >&
);




};

#endif
