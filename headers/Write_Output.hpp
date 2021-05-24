/*
 * Write_Output_Functions.h
 *
 *  Created on: 20 Dec 2012
 *      Author: pmdcm
 */

#ifndef WRITE_OUTPUT_FUNCTIONS_H_
#define WRITE_OUTPUT_FUNCTIONS_H_

/*|-------------------------------|*/
/*|-- Functions to write output --|*/
/*|-------------------------------|*/

// Function to write the labels


// Write labels for concentrations, then stream output
void Write_Header_Species_Temperature_Pressure(
		string filename,
		string separator,
		size_t Number_Species,
		vector< string > Names,
		bool GasPhasePressure
);

void StreamConcentrations(
		ofstream& Concentration_OFStream,
		const string separator,
		bool GasPhasePressure,
		size_t Number_Species,
		double CurrentTime,
		double Pressure,
		const vector<double >& Concentration
);

void StreamConcentrationsV2(
		ofstream& Concentration_OFStream,
		const string separator,
		double CurrentTime,
		size_t Number_Species,
		double* Concentration
);

// write labels for reactions then stream output
void WriteLabelsReactionRates(
		string ,
		string ,
		size_t
);


void StreamReactionRates(
		ofstream&,
		const string,
		double,
		vector< double> &
);



// output of input files
void Write_Thermodynamic_Data(
		string ,
		const vector< string >& ,
		const vector< ThermodynamicData >&
);


void WriteSpecies(
		string ,
		const vector< string >&
);

void WriteReactions(
		string ,
		const vector< string >& ,
		const vector< SingleReactionData >&
);


void Write_Stoichiometric_Matrix_For_Opt(
		string ,
		//		const vector< string >& ,
		const vector< SingleReactionData >&
);
void Input_File_For_Ehsan_Opt(
		string ,
		const vector< SingleReactionData >&
);

string Prepare_Single_Reaction_Output(
		size_t ,
		const vector< string >& ,
		const SingleReactionData&
);

void PetroOxyOutputHeader(
		string
);


void PetroOxyOutputStream(
		string,
		const PressureVesselCalc &,
		double
);

#endif /* WRITE_OUTPUT_FUNCTIONS_H_ */
