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
void WriteNewLabelsSpecies(
		string,
		string,
		size_t ,
		vector< string >,
		bool
);

void StreamConcentrations(
		ofstream&,
		const string,
		bool,
		size_t,
		double,
		double,
		vector< double>&
);

void StreamConcentrationsV2(
		ofstream&,
		const string,
		double,
		size_t,
		double*
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
