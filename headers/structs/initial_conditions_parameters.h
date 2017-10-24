/*
 * initial_conditions.h
 *
 *  Created on: 25 Oct 2017
 *      Author: detlevcm
 */

#ifndef HEADERS_STRUCTS_INITIAL_CONDITIONS_PARAMETERS_H_
#define HEADERS_STRUCTS_INITIAL_CONDITIONS_PARAMETERS_H_


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


struct InitParam {

	MechanismAnalysisConfig MechanismAnalysis;

	// a block for solver parameters
	solver_parameters Solver_Parameters;
	vector<solver_type> Solver_Type; // for specifying a mix of solvers, no individual tolerances though


	/* Needed when Solving */

	bool NoIntegration;

	bool PrintReacRates;

	bool EnforceStability;
	int nrspec;
	vector< double > TimeEnd;
	vector< double > TimeStep;
	double temperature;


	double ReduceReactions;

	/* Constant concentration option */
	/* Single vector, to switch rates of specified species to zero */
	//
	bool ConstantConcentration;
	vector< int > ConstantSpecies;
	// END

	/* Extra Functionality */
	bool StoichiometryMatrixForOpt;
	// END


	/* Pre-Processing of the Mechanism */
	bool irrev;
	bool UseFastLumping;
	bool UseNewLumping;
	// END

	// END

	/* Experimental Gas Phase Code */
	bool GasPhase; // i.e. is gas phase, default false
	double GasPhaseVolume;
	double GasPhasePressure;

	// END

	// New option for storing the pressure vessel
	PressureVesselConfig PressureVessel;
};




#endif /* HEADERS_STRUCTS_INITIAL_CONDITIONS_PARAMETERS_H_ */
