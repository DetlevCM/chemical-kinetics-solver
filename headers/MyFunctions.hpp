


#ifndef _MY_OTHER_FUNCTIONS_
#define _MY_OTHER_FUNCTIONS_
//*** Define some of my random hard to classify functions ***//


void Process_User_Input(
		FileNames& filenames,
		vector<string> User_Inputs
);

void Get_Mechanism(
		string filename ,
		Reaction_Mechanism& reaction_mechanism
);

vector< double > Get_Delta_N(
		const vector< SingleReactionData >& Reactions
);

// Making Scheme Irreversible
vector< SingleReactionData > Make_Irreversible(
		vector< SingleReactionData > Reactions,
		const vector< ThermodynamicData > Thermodynamics,
		double Initial_Temperature, /// use initial temperature from initial data
		double Range // specify +/- range around initial temperature
);

void Synchronize_Gas_Liquid_Model(
		int number_synchronized_species,
		int liquid_species_count, int gas_species_count, // gas and liquid counts so I know where concentration entries belong to
		double *y, // concentrations (&temperature) from the ODE solver
		double Vliq_div_Vgas,
		vector< double > Henry_Constants // need to line up with species IDs
);

#endif /* _MY_OTHER_FUNCTIONS_ */
