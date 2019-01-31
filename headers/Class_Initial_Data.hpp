/*
 * Class_Initial_Data.hpp
 *
 *  Created on: Sep 30, 2018
 *      Author: detlevcm
 */

#ifndef HEADERS_CLASS_INITIAL_DATA_HPP_
#define HEADERS_CLASS_INITIAL_DATA_HPP_

// Planing to split this into its own struct
class PressureVesselConfig {
public:
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

class PressureVessel {
public:
	double Liquid_Sample_Volume;
	double Gas_Sample_Volume;
};


class mechanism_reduction {
public:
	bool UseFastLumping;
	//bool UseNewLumping;
	int LumpingType;
	double ReduceReactions;
};

class solver_parameters { // structure to store the solver parameters
public:
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
class solver_type {
public:
	bool Use_Stiff_Solver;
	bool Use_Analytical_Jacobian;
};

//struct InitParam {
class Initial_Data{
public:

	/* The new data storage organisation, using a class rather than a struct */

	bool NoIntegration;

	// initial concentrations are part of the initial conditions
	vector< double > InitialSpeciesConcentration;

	// Analysis:
	MechanismAnalysisConfig MechanismAnalysis;

	// Initial Conditions:
	vector< double > TimeEnd;
	vector< double > TimeStep;
	double temperature;
	/* Experimental Gas Phase Code */
	bool GasPhase; // i.e. is gas phase, default false
	double GasPhaseVolume;
	double GasPhasePressure;



	// Mechanism Reduction:
	mechanism_reduction MechanismReduction;


	// PetroOxy
	PressureVesselConfig PetroOxy;


	// Pressure Vessel
	PressureVessel Pressure_Vessel;


	// Solver Parameters:
	solver_parameters Solver_Parameters;
	vector<solver_type> Solver_Type; // for specifying a mix of solvers, no individual tolerances though


	// Species:
	vector< InitSpecies > InitialLiquidSpecies;
	vector< InitSpecies > InitialGasSpecies;
	/* Constant concentration option */
	/* Single vector, to switch rates of specified species to zero */
	bool ConstantConcentration;
	vector< int > ConstantSpecies;

	// extra parameters for a TGA case
	bool TGA_used;
	double TGA_rate;
	double TGA_target;

	// Old mess below...

	bool PrintReacRates;
	bool EnforceStability;
	/* Extra Functionality */
	bool StoichiometryMatrixForOpt;
	/* Pre-Processing of the Mechanism */
	bool irrev;
};


#endif /* HEADERS_CLASS_INITIAL_DATA_HPP_ */
