

/* Structures that are of benefit in the entire code */

#ifndef _GLOBAL_STRUCT
#define _GLOBAL_STRUCT

//// TODO: See that these are kept to a minimum

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

#endif