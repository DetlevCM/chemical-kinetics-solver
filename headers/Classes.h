/*
 * Classes.h
 *
 *  Created on: 4 Dec 2017
 *      Author: detlevcm
 */

#ifndef HEADERS_CLASSES_H_
#define HEADERS_CLASSES_H_

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


	// Old mess below...


	bool PrintReacRates;
	bool EnforceStability;
	/* Extra Functionality */
	bool StoichiometryMatrixForOpt;
	/* Pre-Processing of the Mechanism */
	bool irrev;
};



// a class to store the mechanism
class MechanismData {
public:
	vector< string > Species;
	vector< ThermodynamicData > Thermodynamics;
	vector< SingleReactionData > Reactions;
};




#endif /* HEADERS_CLASSES_H_ */
