/*
 * Global_Variables.hpp
 *
 *  Created on: 02.07.2015
 *      Author: DetlevCM
 */

#ifndef SOURCE_GLOBAL_VARIABLES_HPP_
#define SOURCE_GLOBAL_VARIABLES_HPP_


namespace Jacobian
{
extern vector< JacobianData > JacobianMatrix;
}

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

//// variable (values change during calculation ////
extern vector< double > Rates;
extern vector< double > SpeciesConcentrationChange;
}

// Split into own namespace for efficiency
namespace ODE_RHS_Pressure_Vessel_Variables
{
extern int OxyGasSpeciesID;
extern PressureVesselCalc PetroOxyData;
//for limited Oxy
extern double time_previous;
}



#endif /* SOURCE_GLOBAL_VARIABLES_HPP_ */
