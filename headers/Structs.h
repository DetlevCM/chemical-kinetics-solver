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

struct JacobainSpecies{
	int SpeciesID;
	double coefficient;
};

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
	int ReactionID;
	int SpeciesID;
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
};

struct CalculatedThermodynamics {
	double Hf;
	double Cp;
	double Cv;
	double S;
};

struct InitParam {
	bool irrev; // shoud be bool
	bool PrintReacRates;
	bool RatesMaxAnalysis;
	bool RatesSpeciesAllAnalysis;
	bool StreamRatesAnalysis;
	bool EnforceStability;
	int nrspec;
	double rtol;
	double atol;
	double threshold;
	vector< double > TimeEnd;
	vector< double > TimeStep;
	double temperature;
	double hm;
	double h;

	bool UseStiffSolver;
	double ReduceReactions;

	bool RatesAnalysisAtTime;
	vector< double > RatesAnalysisAtTimeData;

	/* Code Extension for Gas Phase */
	bool GasPhase; // i.e. is gas phase, default false
	double GasPhaseVolume;
	double GasPhasePressure;

	/* Handling of PetroOxy in Main Input File */
	bool PetroOxy;
	int PetroOxyGasSpecies;
	double PetroOxySampleSize;
	double PetroOxyInitPressure;
	double PetroOxyMaxPressure;
	double PetroOxyGasSolubility;
	double PetroOxyTemperatureRise;
	bool HenryLawDiffusionLimitSet;
	double HenryLawDiffusionLimit;

	bool StoichimoetryMatrixForOpt;

	bool Jacobian;

};


struct PetroOxyCalculation {
	bool HenryLawDiffusionLimitSet;
	double SampleSize ; // Old 0
	double HeadSpaceGas ; // Old 4
	double HeadSpaceGasMol; // Old 6
	double HenryConstantk; // Old 10
	double HeadSpaceGasPressure; // Old 7
	double HeadSpaceSolventComponentPressure; // Old 8
	double HenryLawDiffusionLimit;
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


#endif /* HEADERS_STRUCTS_H_ */
