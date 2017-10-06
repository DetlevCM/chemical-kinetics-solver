/*
 * Structs.h
 *
 *  Created on: 19 Dec 2014
 *      Author: DetlevCM
 */

#ifndef HEADERS_STRUCTS_H_
#define HEADERS_STRUCTS_H_


// Seems better to manually code in than to reproduce ReactionParameter may be wrong...
//*


struct Filenames{
	string Species;
	string Rates;
	string PetroOxy;
	//string RatesAnalysisStream;
	string Prefix;
};

/*
struct JacobianSpecies{
	int SpeciesID;
	double coefficient;
};//*/

struct JacobianSpecies{
	int SpeciesID;
	double power;
};

struct JacobianData {
	bool IsForward;
	bool IsProduction;
	int ColumnWiseArrayPosition;
	int ReactionID;
	double coefficient;
	vector< JacobianSpecies > Species;
};



struct SingleReactionData {
	bool Reversible;
	bool IsDuplicate;
	double paramA;
	double paramN;
	double paramEa;
	vector<double> Reactants;
	vector<double> Products;
};
//*/

//*
struct ReactionParameter {
	bool Reversible;
	double paramA;
	double paramN;
	double paramEa;
};
//*/

struct TrackSpecies {
	int SpeciesID;
	int ReactionID;
	double coefficient;
};


struct SpeciesPicking {
	bool Choice;
	int SpcClass;
};

struct InitSpecies {
	int SpecID;
	double SpecConc;
};


struct ConstantInitRHSODE {
	bool EnforceStability;
	bool PetroOxy;
	double temperature;
	double PetroOxyTemperatureRise;

	// for constant concentration
	//
	bool ConstantConcentration;
	vector< double > ConstantSpecies;
};

struct CalculatedThermodynamics {
	double Hf;
	double Cp;
	double Cv;
	double S;
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

/*
struct ProdConsAnalysis {
	int Consumption;
	int Production;
};//*/

struct MechanismAnalysisConfig {
	bool MaximumRates;
	bool RatesSpeciesAllAnalysis;
	bool StreamRatesAnalysis;
	bool RatesAnalysisAtTime;
	vector< double > RatesAnalysisAtTimeData;

	// integer array to identify the species or which we want rate of production analysis
	bool RatesOfSpecies;
	vector< int > SpeciesSelectedForRates;
	vector< vector< int > > ReactionsForSpeciesConsAnalysis;
	vector< vector< int > > ReactionsForSpeciesProdAnalysis;

};


// new structure for initial parameters

struct param_solver {

	// we need to pick the solver tpe
	int SolverType;
	// 0 = Intel, 1 = Odepack

	bool Use_Stiff_Solver;
	bool Use_Analytical_Jacobian;


	/* ODE Solver Parameters */
	double rtol;
	double threshold;
	double atol; // atol = rtol * threshhold
	double minimum_stepsize; // hm
	double initial_stepsize; // h

	// output control
	string separator;
};

// for the extra input choices with a mutli solver setup
struct solver_type {
	int SolverType;
	// 0 = Intel, 0 = Odepack
	bool Use_Stiff_Solver;
	bool Use_Analytical_Jacobian;
};

struct InitParam {


	MechanismAnalysisConfig MechanismAnalysis;

	// a block for solver parameters
	param_solver Param_Solver;
	vector<solver_type> Solver_Type; // for specifying a mix of solvers, no individual tolerances though


	/* Needed when Solving */



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





struct ThermodynamicData {
	double TLow;
	double THigh;
	double TChange;
	double NasaLow1;
	double NasaLow2;
	double NasaLow3;
	double NasaLow4;
	double NasaLow5;
	double NasaLow6;
	double NasaLow7;
	double NasaHigh1;
	double NasaHigh2;
	double NasaHigh3;
	double NasaHigh4;
	double NasaHigh5;
	double NasaHigh6;
	double NasaHigh7;
};


struct str_RatesAnalysis
{
	double productionrate;
	double consumptionrate;
	double prod_time;
	double cons_time;
};


// For Naming Species Classes in Lumping:
struct ClassNaming
{
	bool IsNamed;
	int ClassID;
	string Name;
};


#endif /* HEADERS_STRUCTS_H_ */
