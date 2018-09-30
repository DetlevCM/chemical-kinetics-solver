


#ifndef _MY_OTHER_FUNCTIONS_
#define _MY_OTHER_FUNCTIONS_
//*** Define some of my random hard to classify functions ***//

class FileNames {
public:
	/*
	 * This class collects the file names for the
	 * input files required during a run.
	 *
	 * To collate data, output files are equally treated in the
	 * same class, together together potentially with output flags.
	 */

	string mechanism;
	string initial_data;
	string species_mapping;

};

void Process_User_Input(
		FileNames& ,
		vector<string> // User_Inputs
);

vector< double > Get_Delta_N(
		const vector< SingleReactionData >&
);

// Making Scheme Irreversible
vector< SingleReactionData > Make_Irreversible(
		vector< SingleReactionData > ,
		vector< ThermodynamicData > ,
		double,
		double
);

void Synchronize_Gas_Liquid_Model(
		int ,
		int , int ,
		double* ,
		double ,
		vector< double >
);

#endif /* _MY_OTHER_FUNCTIONS_ */
