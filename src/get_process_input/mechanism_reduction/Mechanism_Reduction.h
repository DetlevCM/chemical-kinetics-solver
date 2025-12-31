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

static void Reduce_Species_Thermo_Mechanism(
		vector< bool > ,
		vector<Species>& species ,
		//vector< Species::ThermodynamicData >& ,
		vector< SingleReactionData >&
);


static void mechanism_picking(
		string filename,
		vector< Species >& species,
		//vector< Species::ThermodynamicData >& ,
		vector< SingleReactionData >&
);


static void handle_reactions_with_chosen_species(
		string ,
		vector< SpeciesPicking >& ,
		vector< Species >& species,
		//vector< Species::ThermodynamicData >& ,
		vector< SingleReactionData >&
);



/* ******************************************************* */
/*  various functions associated with mechanism reduction  */
/* ******************************************************* */


static void ReportAccuracy(
		string,
		size_t,
		vector< Species > species,
		string,
		string,
		string
);

static vector< bool > Read_Kill_List(
		string filename,
		vector< Species > species
);

static void ReactionRateImportance(
		vector< double >& KeyRates,
		const vector< double >& Rates,
		double parameter
);

static vector< SingleReactionData > ReduceReactionsNew(
		const vector< Species >& species,
		const vector< SingleReactionData >& ,
		const vector< double >&
);

static vector< double > Compare_Concentrations(
		//const vector< double >& OldTimePoints,
		const vector< vector< double > >& OldConcentrations,
		//const vector< double >& NewTimePoints,
		const vector< vector< double > >& NewConcentrations
);



///* ***************************************** */
//*  species lumping and reaction reduction   *//
/* ***************************************** *///

static vector< vector< string > > Get_Combine_Species_Mapping(
		string filename
);

static vector<ClassNaming> GetSpeciesClassesNames(
		vector< vector< string > >
);

static vector< Species > RenameSpecies(
		vector< Species > species,
		vector<ClassNaming> UserDefinedNames,
		const vector< size_t > & SpeciesClassMapping
);


static vector< size_t > Map_Species_Classes(
		const vector< vector< string > >& ,
		const vector< Species >&
);


static vector< Species > Process_Thermodynamics_Species_Classes(
		const vector< size_t >& SpeciesMapping,
		vector< Species > species
);

static vector< SingleReactionData > Process_Reactions_For_Species_Lumping(
		size_t Number_Species_Classes,
		const vector< size_t > SpeciesClassMapping,
		vector< SingleReactionData >Reactions,
		double temperature,
		bool FastLumping, // this will calculated only 3 points, hence is fast
		int LumpingType
);

// Some helpers for the reduction:
static Reaction::ReactionParameter Average_Ea_k_fitted_Fast(
		vector< SingleReactionData >& ,
		double ,
		size_t
);

static Reaction::ReactionParameter Average_Ea_k_fitted_Slow(
		vector< SingleReactionData >& ,
		double ,
		size_t
);

// Some helpers for the reduction:
static Reaction::ReactionParameter n_zero_k_fitted_Fast(
		vector< SingleReactionData >& ,
		double ,
		size_t
);

static Reaction::ReactionParameter n_zero_k_fitted_Slow(
		vector< SingleReactionData >& ,
		double ,
		size_t
);

static Reaction::ReactionParameter Average_Ea_n_zero_k_fitted(
		vector< SingleReactionData >& ,
		double ,
		size_t
);

};

#endif
