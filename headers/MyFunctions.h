

//*** Define Intel ODE Solver ***//
#ifndef _INTEL_ODE_H_
#define _INTEL_ODE_H_
//*
extern "C" {

void dodesol_mk52lfa(
		int*,
		int*,
		double*,
		double*,
		double*,
		void*,
		void*,
		double*,
		double*,
		double*,
		double*,
		double*,
		int*,
		int*
);

void dodesol_rkm9mka(
		int*,
		int*,
		double*,
		double*,
		double*,
		void*,
		void*,
		double*,
		double*,
		double*,
		double*,
		double*,
		int*,
		int*
);

void dodesol_mk52lfn(
		int*,
		int*,
		double*,
		double*,
		double*,
		void*,
		double*,
		double*,
		double*,
		double*,
		double*,
		int*,
		int*
);

void dodesol_rkm9mkn(
		int*,
		int*,
		double*,
		double*,
		double*,
		void*,
		double*,
		double*,
		double*,
		double*,
		double*,
		int*,
		int*
);

}//*/
#endif /* _INTEL_ODE_H_ */



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


//*
void Integrate_Liquid_Phase(
		string,
		string,
		string,
		string,
		vector< double >,
		vector< string > ,
		vector< SingleReactionData > &,
		InitParam,
		vector< double >&,
		PetroOxyCalculation,
		vector< vector < str_RatesAnalysis > > &
);
//*/

//*
void ODE_RHS_Liquid_Phase(int*, double*, double*, double*);
void Jacobian_Matrix(int*, double*, double*, double*);
//*/

#endif /* _MY_OTHER_FUNCTIONS_ */
