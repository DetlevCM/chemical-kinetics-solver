


#ifndef _MY_OTHER_FUNCTIONS_
#define _MY_OTHER_FUNCTIONS_
//*** Define some of my random hard to classify functions ***//


void Process_User_Input(
		FileNames& ,
		vector<string> // User_Inputs
);

void Get_Mechanism(
		string , //filename ,
		Reaction_Mechanism& // reaction_mechanism
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
