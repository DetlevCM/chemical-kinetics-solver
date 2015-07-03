/*
 * Calculation_Functions.h
 *
 *  Created on: 20 Dec 2012
 *      Author: pmdcm
 */

#ifndef CALCULATION_FUNCTIONS_
#define CALCULATION_FUNCTIONS_

/*
 * Calculating Rate Constants
 */


void Calculate_Rate_Constant(
		vector< double >& ,
		vector< double >& ,
		const double ,
		const vector< ReactionParameter >& ,
		const vector< vector< double > >& ,
		const vector< TrackSpecies >& ,
		const vector< double >&
);


//vector< double >
void CalculateReactionRates(
		vector< double >& ,
		const vector< double >& ,
		vector< double > ,
		vector< double > ,
		const vector< TrackSpecies >& ,
		const vector< TrackSpecies >&
);



void Calculate_Thermodynamics(
		vector< vector< double > >& ,
		const double& ,
		const vector< ThermodynamicData >&
);


vector< double > SpeciesLossRate(
		const vector< TrackSpecies > &,
		int,
		const vector< double > &
);


#endif /* CALCULATION_FUNCTIONS_ */
