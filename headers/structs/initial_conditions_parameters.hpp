/*
 * initial_conditions.h
 *
 *  Created on: 25 Oct 2017
 *      Author: detlevcm
 */

#ifndef HEADERS_STRUCTS_INITIAL_CONDITIONS_PARAMETERS_HPP_
#define HEADERS_STRUCTS_INITIAL_CONDITIONS_PARAMETERS_HPP_


struct solver_parameters { // structure to store the solver parameters
	int SolverType; // 0 = IntelODE, 1 = Odepack
	bool Use_Stiff_Solver; // for IntelODE
	bool Use_Analytical_Jacobian;
	double rtol;
	double atol;
	double minimum_stepsize; // hm
	double initial_stepsize; // h
	string separator; // separator in text output, e.g. comma delimited
};


// to allow modifying some solver behaviour during the run, for IntelODE only
struct solver_type {
	bool Use_Stiff_Solver;
	bool Use_Analytical_Jacobian;
};


// Planing to split this into its own struct
struct PressureVesselConfig {
	bool IsSet;
	int GasSpecies;
	double SampleSize;
	double InitPressure;
	double MaxPressure;
	double GasSolubility;
	double TemperatureRise;
	bool HenryLawDiffusionLimitSet;
	double HenryLawDiffusionLimit;
	// new addition to allow different vessel sizes
	double VesselSize;
};

struct PressureVessel {
	double Liquid_Sample_Volume;
	double Gas_Sample_Volume;
};


struct mechanism_reduction {
	bool UseFastLumping;
	//bool UseNewLumping;
	int LumpingType;
	double ReduceReactions;
};


struct Initial_Conditions {

};






#endif /* HEADERS_STRUCTS_INITIAL_CONDITIONS_PARAMETERS_HPP_ */
