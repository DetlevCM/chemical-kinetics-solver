/*
 * Mechanism_Reduction.h
 *
 *  Created on: 19 Dec 2014
 *      Author: DetlevCM
 */

#ifndef MECHANISM_REDUCTION
#define MECHANISM_REDUCTION

#include "../get_initial_data/Initial_Data.h"
#include "../get_mechanism/Species.h"
#include "../get_mechanism/Reaction.h"
using namespace Reaction; 
#include "../get_mechanism/ReactionMechanism.h"



class MechanismReduction
{
	public:

struct SpeciesPicking {
	bool Choice;
	size_t SpcClass;
};

// For Naming Species Classes in Lumping:
struct ClassNaming
{
	bool IsNamed;
	size_t ClassID;
	string Name;
};

void Reduce_Species_Thermo_Mechanism(
		vector< bool > ,
		vector<string>& ,
		vector< Species::ThermodynamicData >& ,
		vector< SingleReactionData >&
);


void mechanism_picking(
		string filename,
		vector< string >& Species,
		vector< Species::ThermodynamicData >& ,
		vector< SingleReactionData >&
);


void handle_reactions_with_chosen_species(
		string ,
		vector< SpeciesPicking >& ,
		vector< string >& ,
		vector< Species::ThermodynamicData >& ,
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
//*  species lumping and reaction reduction   *//
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


vector< Species > Process_Thermodynamics_Species_Classes(
		const vector< size_t >& SpeciesMapping,
		vector< Species > species
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
ReactionMechanism::ReactionParameter Average_Ea_k_fitted_Fast(
		vector< SingleReactionData >& ,
		double ,
		size_t
);

ReactionMechanism::ReactionParameter Average_Ea_k_fitted_Slow(
		vector< SingleReactionData >& ,
		double ,
		size_t
);

// Some helpers for the reduction:
ReactionMechanism::ReactionParameter n_zero_k_fitted_Fast(
		vector< SingleReactionData >& ,
		double ,
		size_t
);

ReactionMechanism::ReactionParameter n_zero_k_fitted_Slow(
		vector< SingleReactionData >& ,
		double ,
		size_t
);

ReactionMechanism::ReactionParameter Average_Ea_n_zero_k_fitted(
		vector< SingleReactionData >& ,
		double ,
		size_t
);

};

#endif
