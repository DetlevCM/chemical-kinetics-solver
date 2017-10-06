


#ifndef _MY_OTHER_FUNCTIONS_
#define _MY_OTHER_FUNCTIONS_
//*** Define some of my random hard to classify functions ***//


vector< double > Get_Delta_N(
		const vector< SingleReactionData >&
);


void Prepare_Jacobian_Matrix (
		vector< JacobianData > &,
		const vector< SingleReactionData > &,
		vector< string >
);

/*
 * Making Scheme Irreversible *
 */


vector< SingleReactionData > Make_Irreversible(
		vector< SingleReactionData > ,
		vector< ThermodynamicData >
);

void Integrate_Liquid_Phase_Intel(
		/*string,
		string,
		string,
		string,//*/
		Filenames,
		vector< double >,
		vector< string > ,
		vector< ThermodynamicData > ,
		vector< SingleReactionData > &,
		InitParam,
		vector< double >&,
		PressureVesselCalc,
		vector< vector < str_RatesAnalysis > > &
);

void Integrate_PetroOxy_Liquid_Phase_Intel(
		/*string,
		string,
		string,
		string,//*/
		Filenames,
		vector< double >,
		vector< string > ,
		vector< ThermodynamicData > ,
		vector< SingleReactionData > &,
		InitParam,
		vector< double >&,
		PressureVesselCalc,
		vector< vector < str_RatesAnalysis > > &
);

void Integrate_Liquid_Phase_Odepack(
		/*string,
		string,
		string,
		string,//*/
		Filenames,
		vector< double >,
		vector< string > ,
		vector< ThermodynamicData > ,
		vector< SingleReactionData > &,
		InitParam,
		vector< double >&,
		PressureVesselCalc,
		vector< vector < str_RatesAnalysis > > &
);

#endif /* _MY_OTHER_FUNCTIONS_ */
