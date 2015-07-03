/*
 * adjust_gas_concentration_PetroOxy.cpp
 *
 *  Created on: 23 May 2014
 *      Author: DetlevCM
 */


#include <MyHeaders.h>

// we know the volume of the PetroOxy Dish
// We should let the user enter all other information

/* Data is:
 * 0) Sample Size <- m^3
 * 1) p Initial Pressure Pa
 * 2) T Target Temperature
 * 3) pmax Maximum Pressure
 * 4) O2 space in PetroOxy <- m^3
 * 5) Gas Species
 * 6) mol of gas species at 25 degree celsius
 * 7) O2 derived pressure
 * 8) Vapour Pressure solvent component
 * 9) solubility of gas at 298K, mol/L (C)
 * 10) "k" as Henry's Law Constant, k = P_gas/C //  Pa mol / L
 *
 * k_H = P_gas/c
 * k Pa mol / L
 * C mol / L
 * P_gas
 */


void AdjustGasConcentration(
		double & GasSpeciesInLiquidConcentration,
		double temperature,
		double time_difference,
		PetroOxyCalculation & PetroOxyData
)
{
	/* 2002 CODATA values */
	double R = 8.314472e0; // J K^-1 mol^-1 (use with Pa and m^3)
	//double Na = 6.0221415e23;
	// Pa -> atm *9.86923267e-6


	// cout << time_difference << "\n"; // checked, it is non-zero

	//cout << PetroOxyData.HenryLawDiffusionLimitSet << "\n";

	if(PetroOxyData.HenryLawDiffusionLimitSet){ // arrangement for limited gas to liquid diffusion
		//cout << "Check Limit Set \n";
		//cout << time_difference << "\n";

		double targetconcentration =
				(
						(PetroOxyData.HeadSpaceGasMol +
								PetroOxyData.SampleSize*GasSpeciesInLiquidConcentration*1000)
								*R*temperature)
								/
								(PetroOxyData.HeadSpaceGas*PetroOxyData.HenryConstantk +
										R*temperature*PetroOxyData.SampleSize);


		// Now we need to compare the liquid concentration to the gas concentrations
		// volume is in m^3 -> I get mol/L m^3 - *1000 to get mol /L * L
		double transferMolGas = (targetconcentration - GasSpeciesInLiquidConcentration) * PetroOxyData.SampleSize*1000;


		if(transferMolGas > PetroOxyData.HenryLawDiffusionLimit*12.5*time_difference) // we assume 12.5cm^2 surface area for the liquid sample in the PetroOxy
		{
			transferMolGas = PetroOxyData.HenryLawDiffusionLimit*12.5*time_difference; // limiter
			//cout << "Transfer Gas Value " << transferMolGas << "\n";

			PetroOxyData.HeadSpaceGasMol = PetroOxyData.HeadSpaceGasMol - transferMolGas;
			PetroOxyData.HeadSpaceGasPressure = PetroOxyData.HeadSpaceGasMol*R*temperature/PetroOxyData.HeadSpaceGas;// + PetroOxyData[8];

			GasSpeciesInLiquidConcentration = GasSpeciesInLiquidConcentration + transferMolGas/(1000*PetroOxyData.SampleSize); // hope I got the correction right...
		}
		else
		{
			PetroOxyData.HeadSpaceGasMol = PetroOxyData.HeadSpaceGasMol- transferMolGas;

			// use ideal gas law to recompute pressure
			// assume the vapour pressure component from the solvent is constant
			PetroOxyData.HeadSpaceGasPressure = PetroOxyData.HeadSpaceGasMol*R*temperature/PetroOxyData.HeadSpaceGas;// + PetroOxyData[8];

			// this is a bit "dangerous", it assumes there is sufficient gas left to saturate the sample,
			// i.e. oxygen in the headspace does not reach 0
			GasSpeciesInLiquidConcentration = targetconcentration;
		}

	}
	else{ // setup for "fast" diffusion from gas to the liquid phase
		//cout << "Check Limit Not Set \n";

		double targetconcentration =
				(
						(PetroOxyData.HeadSpaceGasMol +
								PetroOxyData.SampleSize*GasSpeciesInLiquidConcentration*1000)
								*R*temperature)
								/
								(PetroOxyData.HeadSpaceGas*PetroOxyData.HenryConstantk +
										R*temperature*PetroOxyData.SampleSize);


		// Now we need to compare the liquid concentration to the gas concentrations
		// volume is in m^3 -> I get mol/L m^3 - *1000 to get mol /L * L
		double transferMolGas = (targetconcentration - GasSpeciesInLiquidConcentration) * PetroOxyData.SampleSize*1000;

		PetroOxyData.HeadSpaceGasMol = PetroOxyData.HeadSpaceGasMol- transferMolGas;

		// use ideal gas law to recompute pressure
		// assume the vapour pressure component from the solvent is constant
		PetroOxyData.HeadSpaceGasPressure = PetroOxyData.HeadSpaceGasMol*R*temperature/PetroOxyData.HeadSpaceGas;// + PetroOxyData[8];

		// this is a bit "dangerous", it assumes there is sufficient gas left to saturate the sample,
		// i.e. oxygen in the headspace does not reach 0
		GasSpeciesInLiquidConcentration = targetconcentration;
	}
}





// identical to regular PetroOxy function, but corrects concentration without change to reservoir
void Adjust_Gas_Concentration_Initial(
		double & GasSpeciesInLiquidConcentration,
		double temperature,
		PetroOxyCalculation & PetroOxyData
)
{
	/* 2002 CODATA values */
	double R = 8.314472e0; // J K^-1 mol^-1 (use with Pa and m^3)
	//double Na = 6.0221415e23;
	// Pa -> atm *9.86923267e-6

	double targetconcentration =
			(
					(PetroOxyData.HeadSpaceGasMol +
							PetroOxyData.SampleSize*GasSpeciesInLiquidConcentration*1000)
							*R*temperature)
							/
							(PetroOxyData.HeadSpaceGas*PetroOxyData.HenryConstantk +
									R*temperature*PetroOxyData.SampleSize);

	// this is a bit "dangerous", it assumes there is sufficient gas left to saturate the sample,
	// i.e. oxygen in the headspace does not reach 0
	GasSpeciesInLiquidConcentration = targetconcentration;
}














