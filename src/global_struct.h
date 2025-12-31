

/* Structures that are of benefit in the entire code */

#ifndef _GLOBAL_STRUCT
#define _GLOBAL_STRUCT

#include <cstddef>
#include <string>
using std::string;

//// TODO: See that these are kept to a minimum

struct Filenames{
	/*
	 * This strcut collects the file names for the
	 * input files required during a run.
	 *
	 * To collate data, output files are equally treated in the
	 * same strcut, together together potentially with output flags.
	 */

	string mechanism;
	string initial_data;
	string species_mapping;


	// this a mess, there is an overlap... 
	string Species;
	string Rates;
	string PetroOxy;
	string Prefix;
};

struct PressureVesselCalc {
	bool HenryLawDiffusionLimitSet;
	double SampleSize ; // Old 0
	double HeadSpaceGas ; // Old 4
	double HeadSpaceGasMol; // Old 6
	double HenryConstantk; // Old 10
	double HeadSpaceGasPressure; // Old 7
	double HeadSpaceSolventComponentPressure; // Old 8
	double HenryLawDiffusionLimit;
};

struct str_RatesAnalysis
{
	double productionrate;
	double consumptionrate;
	double prod_time;
	double cons_time;
};

struct TrackSpecies {
	size_t SpeciesID;
	size_t ReactionID;
	double coefficient;
};

#endif