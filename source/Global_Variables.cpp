/*
 * Global_Variables.cpp
 *
 *  Created on: 6 Oct 2017
 *      Author: detlevcm
 */

#include <MyHeaders.h>


/* It seems a limited number of global variables cannot be avoided.
 * The reaction scheme as well as species concentrations will need
 * to be provided as a global variable as the ODEs need access to
 * them without passing any variables to the relevant void
 *
 * By making the following global, I avoid vector re-declarations
 * This has a positive impact on performance by sticking them in a
 * namespace I make them more manageable.
 * It also allows me to size the array beforehand
 */

namespace Jacobian
{
vector< JacobianData > JacobianMatrix;
}

namespace Jacobian_ODE_RHS
{
//// constant (i.e. set once) ////
int Number_Species;
vector< double > Delta_N;
vector< ThermodynamicData > Thermodynamics;
vector< ReactionParameter > ReactionParameters; // tidier than reactions vector
vector< TrackSpecies > SpeciesLossAll; // vector for recording species loss

//// variable (values change during calculation ////
vector< double > Concentration;
vector< CalculatedThermodynamics > CalculatedThermo;
vector< double > Kf;
vector< double > Kr;
}


namespace ODE_RHS
{
//// constant (i.e. set once) ////
int Number_Reactions;
ConstantInitRHSODE InitialDataConstants;
vector< TrackSpecies > ReactantsForReactions;
vector< TrackSpecies > ProductsForReactions;

//// variable (values change during calculation ////
vector< double > Rates;
vector< double > SpeciesConcentrationChange;
}

// Split into own namespace for efficiency
namespace ODE_RHS_Pressure_Vessel_Variables
{
int OxyGasSpeciesID;
PressureVesselCalc PetroOxyData;
//for limited Oxy
double time_previous;
}


