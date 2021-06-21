/*
 * Structs.h
 *
 *  Created on: 19 Dec 2014
 *      Author: DetlevCM
 */

#ifndef HEADERS_STRUCTS_H_
#define HEADERS_STRUCTS_H_

struct SpeciesWithCoefficient
{
	size_t SpeciesID;
	double coefficient;
	bool ismatched;// = false;
};

struct Filenames{
	string Species;
	string Rates;
	string PetroOxy;
	string Prefix;
};

struct ThirdBodyParameters{
	size_t SpeciesID;
	double value;
};


struct JacobianSpecies{
	size_t SpeciesID;
	double power;
};

struct JacobianData {
	bool IsForward;
	bool IsProduction;
	size_t ColumnWiseArrayPosition;
	size_t ReactionID;
	double coefficient;
	vector< JacobianSpecies > Species;
};


struct TrackSpecies {
	size_t SpeciesID;
	size_t ReactionID;
	double coefficient;
};


struct SpeciesPicking {
	bool Choice;
	size_t SpcClass;
};

struct InitSpecies {
	size_t SpecID;
	double SpecConc;
};


struct ConstantInitRHSODE {
	bool EnforceStability;
	bool PetroOxy;
	double temperature;
	double PetroOxyTemperatureRise;

	bool TGA_used;
	double TGA_rate;
	double TGA_target;

	// for constant concentration
	bool ConstantConcentration;
	vector< double > ConstantSpecies;
};

struct CalculatedThermodynamics {
	double Hf;
	double Cp;
	double Cv;
	double S;
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
	vector< size_t > SpeciesSelectedForRates;
	vector< vector< size_t > > ReactionsForSpeciesConsAnalysis;
	vector< vector< size_t > > ReactionsForSpeciesProdAnalysis;

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
	size_t ClassID;
	string Name;
};


#endif /* HEADERS_STRUCTS_H_ */
