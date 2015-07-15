/*
 * Namespaces.hpp
 *
 *  Created on: 02.07.2015
 *      Author: DetlevCM
 */

#ifndef SOURCE_NAMESPACES_HPP_
#define SOURCE_NAMESPACES_HPP_

/*
namespace GlobalArrays{
// Key parameters that define the whole reaction scheme - used globally via namespaces
// Not sure if this is a good place to put it...
extern vector< vector < str_RatesAnalysis > > RatesAnalysisData;
extern vector< TrackSpecies > ProductsForRatesAnalysis;

}
//*/


// For the integrator
// Needed to be set once for speed, especially re-initialising vectors is slow



namespace Jacobian_ODE_RHS
{
//// constant (i.e. set once) ////
extern int Number_Species;
extern vector< double > Delta_N;
extern vector< ThermodynamicData > Thermodynamics;
extern vector< ReactionParameter > ReactionParameters; // tidier than reactions vector
extern vector< TrackSpecies > SpeciesLossAll; // vector for recording species loss

//// variable (values change during calculation ////
extern vector< double > Concentration;
extern vector< CalculatedThermodynamics > CalculatedThermo;
extern vector< double > Kf;
extern vector< double > Kr;
}


namespace ODE_RHS
{
//// constant (i.e. set once) ////
extern int Number_Reactions;
extern ConstantInitRHSODE InitialDataConstants;
extern vector< TrackSpecies > ReactantsForReactions;
extern vector< TrackSpecies > ProductsForReactions;
extern int OxyGasSpeciesID;

//// variable (values change during calculation ////
extern vector< double > Rates;
extern vector< double > SpeciesConcentrationChange;

extern PetroOxyCalculation PetroOxyData;
//for limited Oxy
extern double time_previous;
}

namespace Jacobian
{
extern vector< JacobianData > JacobianMatrix;
}



#endif /* SOURCE_NAMESPACES_HPP_ */
