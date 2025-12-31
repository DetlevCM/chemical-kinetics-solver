/*
 * Run_Integrator.h
 *
 *  Created on: 20 Oct 2017
 *      Author: detlevcm
 */

#ifndef RUN_INTEGRATOR
#define RUN_INTEGRATOR

#include <vector>
using std::vector;

#include "../Main.h"

#include "../get_process_input/get_initial_data/Initial_Data.h"

#include "../get_process_input/get_mechanism/Species.h"
#include "../get_process_input/get_mechanism/ReactionMechanism.h"

#include "../global_struct.h"


class RunIntegrator 
{

public:








/*
class Jacobian_ODE_RHS
{
	public:
//// constant (i.e. set once) ////
size_t Number_Species;
vector< double > Delta_N;
vector< Species::ThermodynamicData > Thermodynamics;
//vector< Reaction::ReactionParameter > ReactionParameters; // tidier than reactions vector
vector< TrackSpecies > SpeciesLossAll; // vector for recording species loss

//// variable (values change during calculation ////
vector< double > Concentration;
vector< Species::ThermodynamicData::CalculatedThermodynamics > CalculatedThermo;
vector< double > Kf;
vector< double > Kr;
};
//*/

/*
class ODE_RHS
{
	public: 
//// constant (i.e. set once) ////
size_t Number_Reactions;
//ConstantInitRHSODE InitialDataConstants;
vector< TrackSpecies > ReactantsForReactions;
vector< TrackSpecies > ProductsForReactions;

//// variable (values change during calculation ////
vector< double > Rates;
vector< double > SpeciesConcentrationChange;
};
//*/

// Split into own namespace for efficiency
/*
class ODE_RHS_Pressure_Vessel_Variables
{
	public:
size_t OxyGasSpeciesID;
PressureVesselCalc PetroOxyData;
//for limited Oxy
double time_previous;
};
//*/

//// end TODO: 

struct Settings_LSODA{
	int solver_subsettings;

	int JT;
	// LSODA specific settings
	int LRW, LIW;
	int ITOL = 1;
	int ITASK = 1;
	int ISTATE = 1;
	int IOPT = 0;

	// general solver settings
	double RTOL, ATOL;

	// some vectors for LSODA
	vector<int> vector_IWORK;
	vector<double> vector_RWORK;
};

struct Settings_Intel{
	int ierr, dpar_size;
	double h, hm, ep, tr;

	int solver_subsettings;

	vector<int> vector_ipar;
	vector<int> vector_kd;
	vector<double> vector_dpar;
	// set the solver:
	Initial_Data::solver_type Solver_Type;
};

// Function to pick the right integration routine
static void Choose_Integrator(
		Main::Filenames,
		ReactionMechanism ,
		Initial_Data,
		vector< double >&,
		PressureVesselCalc,
		vector< vector < str_RatesAnalysis > > &
);

static int Prepare_Integrator_Settings(
		Initial_Data ,
		size_t ,
		Settings_LSODA& ,
		Settings_Intel&
);

// solve based on concentrations in the liquid phase
static void Integrate_Liquid_Phase(
		Main::Filenames,
		vector< double >,
		ReactionMechanism ,
		Initial_Data,
		vector< double >&,
		vector< vector < str_RatesAnalysis > > &
);


static void Integrate_Gas_Phase_Odepack_LSODA(
		Main::Filenames,
		vector< double >,
		ReactionMechanism ,
		Initial_Data,
		vector< double >&,
		vector< vector < str_RatesAnalysis > > &
);


// solve based on concentrations in the liquid phase and consider headspace gas
static void Integrate_Pressure_Vessel_Liquid_Phase(
		Main::Filenames,
		vector< double >,
		ReactionMechanism ,
		Initial_Data,
		vector< double >&,
		PressureVesselCalc,
		vector< vector < str_RatesAnalysis > > &
);


//// Pre-Processing:

static vector< ReactionParameter > Process_Reaction_Parameters(
		const vector< SingleReactionData >&
);


static vector< TrackSpecies > Reactants_ForReactionRate(
		const vector< SingleReactionData >&
);


static vector< TrackSpecies > Products_ForReactionRate(
		const vector< SingleReactionData >&,
		bool
);


static vector< TrackSpecies > PrepareSpecies_ForSpeciesLoss(
		const vector< SingleReactionData >&
);


};

#endif
