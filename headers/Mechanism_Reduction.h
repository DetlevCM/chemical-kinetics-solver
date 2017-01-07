/*
 * Mechanism_Reduction.h
 *
 *  Created on: 19 Dec 2014
 *      Author: DetlevCM
 */

#ifndef HEADERS_MECHANISM_REDUCTION_H_
#define HEADERS_MECHANISM_REDUCTION_H_


void Reduce_Species_Thermo_Mechanism(
		vector< bool > ,
		vector<string>& ,
		vector< ThermodynamicData >& ,
		vector< SingleReactionData >&
);


void mechanism_picking(
		string filename,
		vector< string >& ,
		vector< ThermodynamicData >& ,
		vector< SingleReactionData >&
);


void handle_reactions_with_chosen_species(
		string ,
		vector< SpeciesPicking >& ,
		vector< string >& ,
		vector< ThermodynamicData >& ,
		vector< SingleReactionData >&
);



/* ******************************************************* */
/*  various functions associated with mechanism reduction  */
/* ******************************************************* */


void ReportAccuracy(
		string,
		int,
		vector< string > ,
		string,
		string,
		string
);

vector< bool > Read_Kill_List(
		string filename,
		vector< string > Species
);

void ReactionRateImportance(
		vector< double>& ,
		const vector< double >& ,
		double
);

vector< SingleReactionData > ReduceReactionsNew(
		const vector< string >& ,
		const vector< SingleReactionData >& ,
		const vector< double >&
);

vector< double > Compare_Concentrations(
		const vector< double >& ,
		const vector< vector< double > >& ,
		const vector< double >& ,
		const vector< vector< double > >&
);



///* ***************************************** */
//*  species lumping and reaction redcution   *//
/* ***************************************** *///

vector< vector< string > > Get_Combine_Species_Mapping(
		string
);

vector<ClassNaming> GetSpeciesClassesNames(
		vector< vector< string > >
);

vector< string > RenameSpecies(
		vector< string > ,
		vector<ClassNaming> ,
		const vector< int > &
);


vector< int > Map_Species_Classes(
		const vector< vector< string > >& ,
		const vector< string >&
);


vector< ThermodynamicData > Process_Thermodynamics_Species_Classes(
		const vector< int >& ,
		vector< ThermodynamicData >
);

vector< SingleReactionData > Process_Reactions_For_Species_Lumping(
		int ,
		const vector< int >& ,
		vector< SingleReactionData >,
		double ,
		bool,
		bool
);

// Some helpers for the reduction:
ReactionParameter Average_Ea_CalculateNewParametersFast(
		vector< SingleReactionData >& ,
		double ,
		int
);

ReactionParameter Average_Ea_CalculateNewParametersSlow(
		vector< SingleReactionData >& ,
		double ,
		int
);

// Some helpers for the reduction:
ReactionParameter n_zero_CalculateNewParametersFast(
		vector< SingleReactionData >& ,
		double ,
		int
);

ReactionParameter n_zero_CalculateNewParametersSlow(
		vector< SingleReactionData >& ,
		double ,
		int
);

#endif /* HEADERS_MECHANISM_REDUCTION_H_ */
