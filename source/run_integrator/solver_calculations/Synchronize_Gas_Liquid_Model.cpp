/*
 * Synchronize_Gas_Liquid_Model.cpp
 *
 *  Created on: 26 Oct 2017
 *      Author: detlevcm
 */


// I should ensure I have a synch list from the pre-processing

/*
struct Sync_ID {
	int liquid_id;
	int gas_id;
};
//*/

#include <MyHeaders.h>

void Synchronize_Gas_Liquid_Model(
		//vector< Sync_ID > Sync_IDs, // this vector will need to include a map: liquid species <-> gas species
		// alternatively I can re-arrange species at the beginning, this might be better for performance
		int number_synchronized_species,
		int liquid_species_count, int gas_species_count, // gas and liquid counts so I know where concentration entries belong to
		double* y, // concentrations (&temperature) from the ODE solver
		double Vliq_div_Vgas,
		vector< double > Henry_Constants // need to line up with species IDs
)
{

	/* 2002 CODATA values */
	double R = 8.314472e0; // J K^-1 mol^-1 (use with Pa and m^3)
	//double Na = 6.0221415e23;
	// Pa -> atm *9.86923267e-6


	/* From my PhD Thesis
	 *
	 * C_new = (N_current + C_current * V_sample) R * T / (kH * V_gas + V_sample * R * T)
	 *
	 * This can be rewritten for concentrations
	 *
	 * C_new = (gas_C_current + C_current * V_liq/V_gas) * R * T / ( kH + V_liq/V_gas * R * T )
	 *
	 * IF we assume no volume changes, V_liq/V_gas = constant
	 * carrying on:
	 * gas_C_new = gas_C_current - (C_new - C_current) * Vliq/V_gas
	 *
	 * p = n/V_gas * R * T
	 * -> So partial pressures are easy too if needed
	 */

	/*/ example maths for a single species
	double gas_temperature ;
	double liquid_temperature ;
	double gas_concentration ;
	double liquid_concentration ;
	double hK;
	double  new_liquid_concentration =
					((gas_concentration + liquid_concentration*Vliq_div_Vgas)*R*gas_temperature)
									/ (hK + Vliq_div_Vgas*R*gas_temperature);
	// this calculation then gives the new gas and liquid concentrations
	// pressures, etc. can be calculated if desired/necessary during the output writing phase
	double new_gas_concentration =
			gas_concentration - (new_liquid_concentration - liquid_concentration) * Vliq_div_Vgas;
	// end example maths //*/


	// now in vector format:
	int i;

	// 2 Liq, 2 Gas
	// 0  1  2  3  4  5
	// L1 L2 LT G1 G2 GT

	double gas_temperature = y[liquid_species_count + gas_species_count + 1];
	//double liquid_temperature = y[liquid_species_count];

	vector< double > gas_concentration ;
	vector< double > liquid_concentration ; //*/
	//vector< double > Henry_Constants;

	// int number_synchronized_species,
	// int liquid_species_count, int gas_species_count
	// assuming species 0 to n = liquid phase + temperature
	// then positions n + 1 are gas phase species...
	//for(i=0;i<liquid_species_count;i++)

	vector< double > new_liquid_concentration(number_synchronized_species);
	vector< double > new_gas_concentration(number_synchronized_species);

	for(i=0;i<number_synchronized_species;i++) // this only needs to be done for the synchronised species
	{
		new_liquid_concentration[i] =
				//((gas_concentration[i] + liquid_concentration[i]*Vliq_div_Vgas)*R*gas_temperature)
				((y[i + liquid_species_count + 1] + y[i]*Vliq_div_Vgas)*R*gas_temperature)
				/ (Henry_Constants[i] + Vliq_div_Vgas*R*gas_temperature);
		// this calculation then gives the new gas and liquid concentrations
		// pressures, etc. can be calculated if desired/necessary during the output writing phase
		new_gas_concentration[i] =
				//gas_concentration[i] - (new_liquid_concentration[i] - liquid_concentration[i]) * Vliq_div_Vgas;
				y[i + liquid_species_count + 1] - (new_liquid_concentration[i] - y[i]) * Vliq_div_Vgas;
	}

	// feeding the modified concentrations back into the array...
	for(i=0;i<number_synchronized_species;i++) // this only needs to be done for the synchronised species
	{
		y[i] = new_liquid_concentration[i];
		y[i+liquid_species_count+1] = new_gas_concentration[i];
	}
}




