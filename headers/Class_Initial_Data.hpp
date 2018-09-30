/*
 * Class_Initial_Data.hpp
 *
 *  Created on: Sep 30, 2018
 *      Author: detlevcm
 */

#ifndef HEADERS_CLASS_INITIAL_DATA_HPP_
#define HEADERS_CLASS_INITIAL_DATA_HPP_


//struct InitParam {
class Initial_Data{
public:

	/* The new data storage organisation, using a class rather than a struct */

	bool NoIntegration;


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

	// initial concentrations are part of the initial conditions
	vector< double > InitialSpeciesConcentration;

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
