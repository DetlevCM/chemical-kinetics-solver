/*
 * Mechanism_Reduction.h
 *
 *  Created on: 19 Dec 2014
 *      Author: DetlevCM
 */

#ifndef HEADERS_MECHANISM_REDUCTION_HPP_
#define HEADERS_MECHANISM_REDUCTION_HPP_


void Reduce_Species_Thermo_Mechanism(
		vector< bool > ,
		vector<string>& ,
		vector< ThermodynamicData >& ,
		vector< SingleReactionData >&
);


void mechanism_picking(
		string filename,
		vector< string >& Species,
		vector< ThermodynamicData >& Thermodynamics,
		vector< SingleReactionData >& Reactions
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
		size_t,
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
		vector< double >& KeyRates,
		const vector< double >& Rates,
		double parameter
);

vector< SingleReactionData > ReduceReactionsNew(
		const vector< string >& ,
		const vector< SingleReactionData >& ,
		const vector< double >&
);

vector< double > Compare_Concentrations(
		//const vector< double >& OldTimePoints,
		const vector< vector< double > >& OldConcentrations,
		//const vector< double >& NewTimePoints,
		const vector< vector< double > >& NewConcentrations
);



///* ***************************************** */
//*  species lumping and reaction redcution   *//
/* ***************************************** *///

vector< vector< string > > Get_Combine_Species_Mapping(
		string filename
);

vector<ClassNaming> GetSpeciesClassesNames(
		vector< vector< string > >
);

vector< string > RenameSpecies(
		vector< string > Species,
		vector<ClassNaming> UserDefinedNames,
		const vector< size_t > & SpeciesClassMapping
);


vector< size_t > Map_Species_Classes(
		const vector< vector< string > >& ,
		const vector< string >&
);


vector< ThermodynamicData > Process_Thermodynamics_Species_Classes(
		const vector< size_t >& SpeciesMapping,
		vector< ThermodynamicData > Thermodynamics
);

vector< SingleReactionData > Process_Reactions_For_Species_Lumping(
		size_t ,
		const vector< size_t >& ,
		vector< SingleReactionData >,
		double ,
		//bool,
		bool,
		int
);

// Some helpers for the reduction:
ReactionParameter Average_Ea_CalculateNewParametersFast(
		vector< SingleReactionData >& ,
		double ,
		size_t
);

ReactionParameter Average_Ea_CalculateNewParametersSlow(
		vector< SingleReactionData >& ,
		double ,
		size_t
);

// Some helpers for the reduction:
ReactionParameter n_zero_k_fitted_Fast(
		vector< SingleReactionData >& ,
		double ,
		size_t
);

ReactionParameter n_zero_k_fitted_Slow(
		vector< SingleReactionData >& ,
		double ,
		size_t
);

ReactionParameter Average_Ea_n_zero_k_fitted(
		vector< SingleReactionData >& ,
		double ,
		size_t
);

#endif /* HEADERS_MECHANISM_REDUCTION_HPP_ */
