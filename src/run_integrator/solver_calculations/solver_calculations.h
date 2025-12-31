/*
 * Calculation_Functions.h
 *
 *  Created on: 20 Dec 2012
 *      Author: pmdcm
 */

#ifndef SOLVER_CALCULATION
#define SOLVER_CALCULATION

#include "../../global_struct.h"
//#include "../../get_process_input/get_initial_data/Initial_Data.h"
#include "../../get_process_input/get_mechanism/Species.h"
#include "../../get_process_input/get_mechanism/Reaction.h"
#include "../../get_process_input/get_mechanism/ReactionMechanism.h"

class SolverCalculation {

public:




//// TODO: These were the old global variables, now become member variables:

/*
namespace ODE_RHS
{
//// constant (i.e. set once) ////
size_t Number_Reactions;
ConstantInitRHSODE InitialDataConstants;
vector< TrackSpecies > ReactantsForReactions;
vector< TrackSpecies > ProductsForReactions;

//// variable (values change during calculation ////
vector< double > Rates;
vector< double > SpeciesConcentrationChange;
}
//*/

struct ConstantInitRHSODE {
	bool EnforceStability;
	bool PetroOxy;
	double temperature;
	double PetroOxyTemperatureRise;

	bool TGA_used;
	double TGA_rate;
	double TGA_target;

	// for constant concentration
	bool ConstantConcentration;
	vector< double > ConstantSpecies;
};

ConstantInitRHSODE InitialDataConstants;

vector< TrackSpecies > ReactantsForReactions;
vector< TrackSpecies > ProductsForReactions;

vector< TrackSpecies > SpeciesLossAll; // vector for recording species loss

//vector< ReactionParameter > ReactionParameters; // tidier than reactions vector
vector< Reaction::ReactionParameter > ReactionParameters; // tidier than reactions vector

static size_t Number_Species; // old variable
static size_t Number_Reactions; //old variable

static vector< Species::ThermodynamicData::CalculatedThermodynamics > CalculatedThermo;
static vector< double > Kf;
static vector< double > Kr;
static vector< double > Rates;
static vector< double > Concentration;
static vector< double > SpeciesConcentrationChange;
static vector< double > delta_n;
vector< Species > species; // quickest and easiest way right now... 

double time_previous = 0.0;

public:

SolverCalculation(
	vector< Species > vec_species, // quick and ugly... 
	size_t number_species,
	size_t number_reactions, 
	vector< TrackSpecies > reactantsForReactions,
	vector< TrackSpecies > productsForReactions,
	vector< TrackSpecies > speciesLossAll,
    vector< double > prep_delta_n)
{
//// constant (i.e. set once) ////

ConstantInitRHSODE InitialDataConstants;
ReactantsForReactions = reactantsForReactions;
ProductsForReactions = productsForReactions;
vector< TrackSpecies > speciesLossAll;
delta_n = prep_delta_n;

species = vec_species;

Number_Species = species.size();
Number_Reactions = number_reactions;

//// variable (values change during calculation ////
CalculatedThermo.resize(number_species);

ReactionParameters.resize(number_reactions);
Kf.resize(number_reactions);
Kr.resize(number_reactions);
Rates.resize(number_reactions);

Concentration.resize(number_species + 1); // 1 extra position for temperature
SpeciesConcentrationChange.resize(number_species);
};





// The ODE RHS functions are split - regular initial value & pressure vessel (reservoir)
void ODE_RHS_Liquid_Phase(int*, double*, double*, double*);
void ODE_RHS_Gas_Phase(int*, double*, double*, double*);
void ODE_RHS_Gas_Liquid_Phase(int*, double*, double*, double*);
void ODE_RHS_Pressure_Vessel(int*, double*, double*, double*);



static vector< double > Get_Delta_N(const vector< SingleReactionData >& Reactions);


/*
 * Calculating Rate Constants
 */

static void Calculate_Rate_Constant(
		vector< double >& Kf,
		vector< double >& Kr,
		const double Temperature,
		const vector< Reaction::ReactionParameter >& ReactionParameters,
		const vector< Species::ThermodynamicData::CalculatedThermodynamics >& CalculatedThermo,
		const vector< TrackSpecies >& SpeciesAll,
		const vector< double >& Delta_N
);


//vector< double >
static void CalculateReactionRates(
		vector< double >& Rates,
		const vector< double >& SpeciesConcentration,
		vector< double > Forward_Rates,
		vector< double > Reverse_Rates,
		const vector< TrackSpecies >& ReactantsForRates,
		const vector< TrackSpecies >& ProductsForRates
);


void SolverCalculation::Evaluate_Thermodynamic_Parameters(
		vector< Species::ThermodynamicData::CalculatedThermodynamics > &CalculatedThermo,
		vector< Species > & species, //Thermodynamics,
		const double Temperature
);


static vector< double > SpeciesLossRate(
		size_t Number_Species,
		const vector< double >& Combined_Rates,
		const vector< TrackSpecies >& SpeciesLossList
);

static void Synchronize_Gas_Liquid_Model(
		size_t number_synchronized_species,
		size_t liquid_species_count, size_t gas_species_count, // gas and liquid counts so I know where concentration entries belong to
		double *y, // concentrations (&temperature) from the ODE solver
		double Vliq_div_Vgas,
		vector< double > Henry_Constants // need to line up with species IDs
);

};

#endif
