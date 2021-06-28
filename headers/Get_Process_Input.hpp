/*
 * Input_Functions.h
 *
 *  Created on: 20 Dec 2012
 *      Author: pmdcm
 */

/*
 * Functions that deal with reading in user input
 */

#ifndef INPUT_FUNCTIONS_
#define INPUT_FUNCTIONS_


//void Handle_Mechanism_Input(
bool Handle_Mechanism_Input(
		FileNames , //filenames,
		Reaction_Mechanism& ,
		Initial_Data& ,
		//vector< double >&,
		PressureVesselCalc&
);

vector<int> Set_Mechanism_Units(string );


vector< string > Get_Species(
		string filename
);


vector< ThermodynamicData > Get_Thermodynamic_Data(
		string filename,
		const vector< string >& Species
);

void Process_Chemkin_Thermo_Format(
		vector< ThermodynamicData >& read_in_thermodynamics,
		const vector<string>& Species,
		const vector<string>& Thermodynamics_List
);

void Process_Internal_Thermo_Format(
		vector< ThermodynamicData >& read_in_thermodynamics,
		const vector<string>& Species,
		const vector<string>& Thermodynamics_List
);

SingleReactionData Parse_Chemking_Reaction_String(const vector< int > SchemeUnits, const vector<string> SpeciesNames, string line);

vector< SingleReactionData > Get_Reactions(
		string filename,
		const vector< string >& Species
);
double Scale_Ea(
		double Ea_read_in,
		int scaling_type
		);

double Scale_A(
		double A_read_in,
		vector<double> ReactantData,
		int scaling_type
		);

void Get_Initial_Conditions(
		string,
		vector< string > ,
		Initial_Data&
);


#endif /* INPUT_FUNCTIONS_ */


