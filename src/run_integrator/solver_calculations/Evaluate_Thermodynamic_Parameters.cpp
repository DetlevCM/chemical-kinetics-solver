
#include "../../global_const.h"

#include "./solver_calculations.h"



void SolverCalculation::Evaluate_Thermodynamic_Parameters(
		vector< Species::ThermodynamicData::CalculatedThermodynamics > &CalculatedThermo,
		vector< Species > & species, //Thermodynamics,
		const double Temperature
		)
{
	size_t i;

	size_t Number_Species = species.size() ; //Thermodynamics.size();

	Species::ThermodynamicData::ThermoT temperatures(Temperature);

	/* Hf, Cp, Cv, S */

	for(i=0;i<Number_Species;i++)
	{
		CalculatedThermo[i] = species[i].thermodynamicdata.calculate_thermodynamics(temperatures);
	}
}



