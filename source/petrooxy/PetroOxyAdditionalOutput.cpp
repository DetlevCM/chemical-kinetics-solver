/*
 * PetroOxyAdditionalOutput.cpp
 *
 *  Created on: 23 May 2014
 *      Author: DetlevCM
 */

#include <MyHeaders.h>


// we know the volume of the PetroOxy Dish
// We should let the user enter all other information


/* Data is:
 * 0) Sample Size <- m^3
 * p - 1) Initial Pressure
 * T - 2) Target Temperature
 * pmax - 3) Maximum Pressure
 * 4) O2 space in PetroOxy <- m^3
 * 5) Gas Species
 * 6) mol of gas species at 25 degree celsius
 * 7) O2 derived pressure
 * 8) Vapour Pressure solvent component
 * 9) solubility of gas at 298K, 1atm mol/L
 * 10) "k" as Henry's Law Constant, M / L
 */

void PetroOxyOutputHeader(string filename)
{
	// Stream to output file, output mode (Should clear old content)
	ofstream ConcentrationOutput (filename.c_str(),ios::out);

	if (ConcentrationOutput.is_open())
	{
		ConcentrationOutput << "Time	";
		ConcentrationOutput << "volume_O2" << "	";
		ConcentrationOutput << "gas_mol_25C" << "	";
		ConcentrationOutput << "p_O2" << "	";
		ConcentrationOutput << "p_vapour_solvent" << "	";
		ConcentrationOutput << "k_henry_const" << "	";
		ConcentrationOutput << "\n"; // new line
		ConcentrationOutput.close();
	}

}




void PetroOxyOutputStream(
		string filename,
		const PressureVesselCalc & PetroOxyData,
		double CurrentTime
		)
{
	ofstream PetroOxyOutput (filename.c_str(),ios::app);

	PetroOxyOutput << CurrentTime << "	" ; // time
	PetroOxyOutput << PetroOxyData.HeadSpaceGas << "	" ; // concentration
	PetroOxyOutput << PetroOxyData.HeadSpaceGasMol << "	" ; // concentration
	PetroOxyOutput << PetroOxyData.HeadSpaceGasPressure << "	" ; // concentration
	PetroOxyOutput << PetroOxyData.HeadSpaceSolventComponentPressure << "	" ; // concentration
	PetroOxyOutput << PetroOxyData.HenryConstantk << "\n" ; // concentration

	//PetroOxyOutput.flush(); // To get the whole line
}
