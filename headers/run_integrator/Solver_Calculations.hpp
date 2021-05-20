/*
 * Calculation_Functions.h
 *
 *  Created on: 20 Dec 2012
 *      Author: pmdcm
 */

#ifndef CALCULATION_FUNCTIONS_
#define CALCULATION_FUNCTIONS_



// The ODE RHS functions are split - regular initial value & pressure vessel (reservoir)
void ODE_RHS_Liquid_Phase(int*, double*, double*, double*);
void ODE_RHS_Gas_Phase(int*, double*, double*, double*);
void ODE_RHS_Gas_Liquid_Phase(int*, double*, double*, double*);
void ODE_RHS_Pressure_Vessel(int*, double*, double*, double*);






/*
 * Calculating Rate Constants
 */

void Calculate_Rate_Constant(
		vector< double >& Kf,
		vector< double >& Kr,
		const double Temperature,
		const vector< ReactionParameter >& ReactionParameters,
		const vector< CalculatedThermodynamics >& CalculatedThermo,
		const vector< TrackSpecies >& SpeciesAll,
		const vector< double >& Delta_N
);


//vector< double >
void CalculateReactionRates(
		vector< double >& Rates,
		const vector< double >& SpeciesConcentration,
		vector< double > Forward_Rates,
		vector< double > Reverse_Rates,
		const vector< TrackSpecies >& ReactantsForRates,
		const vector< TrackSpecies >& ProductsForRates
);



void Evaluate_Thermodynamic_Parameters(
		vector< CalculatedThermodynamics > &CalculatedThermo,
		const vector<ThermodynamicData> &Thermodynamics,
		const double Temperature
);


vector< double > SpeciesLossRate(
		size_t Number_Species,
		const vector< double >& Combined_Rates,
		const vector< TrackSpecies >& SpeciesLossList
);


#endif /* CALCULATION_FUNCTIONS_ */
