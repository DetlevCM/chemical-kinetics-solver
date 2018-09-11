/*
 * Synchronize_Gas_Liquid_Model.cpp
 *
 *  Created on: 26 Oct 2017
 *      Author: detlevcm
 */

// I should ensure I have a synch list from the pre-processing


#include <Headers.hpp>

void Synchronize_Gas_Liquid_Model(
		int number_synchronized_species,
		int liquid_species_count, int gas_species_count, // gas and liquid counts so I know where concentration entries belong to
		double *y, // concentrations (&temperature) from the ODE solver
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

	// now in vector format:
	int i;

	double gas_temperature = y[liquid_species_count + gas_species_count + 1];

	vector< double > gas_concentration ;
	vector< double > liquid_concentration ;
	vector< double > new_liquid_concentration(number_synchronized_species);
	vector< double > new_gas_concentration(number_synchronized_species);


	for(i=0;i<number_synchronized_species;i++) // this only needs to be done for the synchronised species
	{
		new_liquid_concentration[i] =
				((y[i + liquid_species_count + 1] + y[i]*Vliq_div_Vgas)*R*gas_temperature)
				/ (Henry_Constants[i] + Vliq_div_Vgas*R*gas_temperature);
		// this calculation then gives the new gas and liquid concentrations
		// pressures, etc. can be calculated if desired/necessary during the output writing phase
		new_gas_concentration[i] =
				y[i + liquid_species_count + 1] - (new_liquid_concentration[i] - y[i]) * Vliq_div_Vgas;
	}

	// feeding the modified concentrations back into the array...
	for(i=0;i<number_synchronized_species;i++) // this only needs to be done for the synchronised species
	{
		y[i] = new_liquid_concentration[i];
		y[i+liquid_species_count+1] = new_gas_concentration[i];
	}
}




