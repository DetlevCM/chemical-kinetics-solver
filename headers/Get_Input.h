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
		string ,
		string ,
		MechanismData& ,
		Initial_Data& ,
		vector< double >&,
		PressureVesselCalc &
);

vector<int> Set_Mechanism_Units(string );


vector< string > Get_Species(
		string
);


vector< ThermodynamicData > Get_Thermodynamic_Data(
		string ,
		const vector< string >&
);


vector< SingleReactionData > Get_Reactions(
		string ,
		const vector< string >&
);


void Get_Initial_Conditions(
		string,
		vector< string > ,
		Initial_Data&
);


#endif /* INPUT_FUNCTIONS_ */


