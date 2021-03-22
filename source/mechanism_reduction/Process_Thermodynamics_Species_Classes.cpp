/*
 * C_Process_Thermodynamics_Species_Classes.cpp
 *
 *  Created on: 20 Nov 2012
 *      Author: pmdcm
 */
#include "../headers/Headers.hpp"


vector< ThermodynamicData > Process_Thermodynamics_Species_Classes(
		const vector< int >& SpeciesMapping,
		vector< ThermodynamicData > Thermodynamics
)
{

	/* For a start, do something idiotic, i.e. average all parameters
	 * This will invariably lead to dodgy thermodynamic data, but I cannot think
	 * of a better method right now...
	 * And anyway, upgrade can be made to just this function.
	 */

	vector< vector< double > > new_thermo;
	vector< double > SingleClassEntry;


	int i,j;

	int Number_Species = (int)Thermodynamics.size();


	// Convert Thermodynamics for Mechanims Modification
	vector< vector< double > > ThermodynamicsOld;

	//*
	for(i=0;i<Number_Species;i++){

		vector< double > temp(17);

		temp[0] = Thermodynamics[i].TLow;
		temp[1] = Thermodynamics[i].THigh;
		temp[2] = Thermodynamics[i].TChange;

		temp[3] = Thermodynamics[i].NasaLow1;
		temp[4] = Thermodynamics[i].NasaLow2;
		temp[5] = Thermodynamics[i].NasaLow3;
		temp[6] = Thermodynamics[i].NasaLow4;
		temp[7] = Thermodynamics[i].NasaLow5;
		temp[8] = Thermodynamics[i].NasaLow6;
		temp[9] = Thermodynamics[i].NasaLow7;


		temp[10] = Thermodynamics[i].NasaHigh1;
		temp[11] = Thermodynamics[i].NasaHigh2;
		temp[12] = Thermodynamics[i].NasaHigh3;
		temp[13] = Thermodynamics[i].NasaHigh4;
		temp[14] = Thermodynamics[i].NasaHigh5;
		temp[15] = Thermodynamics[i].NasaHigh6;
		temp[16] = Thermodynamics[i].NasaHigh7;

		ThermodynamicsOld.push_back(temp);
	}//*/


	int Number_Species_Classes = 0;
	// use abs() to get the absolute value
	for(i=0;i<Number_Species;i++)
	{
		// The biggest number will be the last class (might be an individual species or group)
		if(Number_Species_Classes < SpeciesMapping[i])
		{
			Number_Species_Classes = SpeciesMapping[i];
		}
		//printf("Number Classes %i  Mapping: %i \n",Number_Species_Classes,SpeciesMapping[i]);
	}
	Number_Species_Classes = Number_Species_Classes + 1; // even with class zero, there needs to be 1 space
	//printf("The system contain %u species classes! \n",Number_Species_Classes);

	//cout << "checkpoint 2 \n";

	/* we now know how many positions we require in our new output.
	 * so we can now loop through the thermodynamic data, assigning the relevant information as required
	 * In a final step we MUST remember to divide the thermodynamic data after summing it all up.
	 */

	/* remember that we have lost class 0 and species class 1 is in position 1 not 0
	 * which adds one array position. Then for some reason we need yet one extra position,
	 * therefore + 2 */
	int Number_New_Thermo_Positions = Number_Species_Classes;// + 2;

	new_thermo.resize(Number_New_Thermo_Positions);
	SingleClassEntry.resize(18);

	// Clumsy method of initialising the new thermodynamics array with zeroes
	for(i=0;i<Number_New_Thermo_Positions;i++)
	{
		for(j=0;j<17;j++) // make sure the matrix is initialized
		{
			SingleClassEntry[j] = 0;
		}
		new_thermo[i] = SingleClassEntry;
	}
	SingleClassEntry.clear();

	//cout << "checkpoint 3 \n";

	for(i=0;i<Number_Species;i++)
	{
		int ClassID = SpeciesMapping[i]; // abs() is important, includes negative values...
		//printf("%i %i \n",i,ClassID);
		SingleClassEntry.resize(18); // 1 extra position for number of species

		SingleClassEntry = new_thermo[ClassID]; // obtain any existing thermodynamic data

		// Add value for species
		for(j=0;j<17;j++)
		{
			SingleClassEntry[j] = SingleClassEntry[j] + ThermodynamicsOld[i][j];
		}
		SingleClassEntry[17] = SingleClassEntry[17] + 1; // 1 species added to class

		new_thermo[ClassID] = SingleClassEntry;

		SingleClassEntry.clear(); // clear just to make sure
	}

	//cout << "checkpoint 4 \n";

	// now average values by class count:
	for(i=0;i<Number_New_Thermo_Positions;i++)
	{
		for(j=0;j<17;j++)
		{
			new_thermo[i][j] = new_thermo[i][j] / new_thermo[i][17];
		}
	}

	//cout << "checkpoint 5 \n";


	/*
	 * Write Thermodynamic Data into new format again
	 */

	// Convert Thermodynamics for Mechanims Modification
	Thermodynamics.clear(); // emtpy first, then populate again
	//*
	for(i=0;i<Number_Species_Classes;i++){

		ThermodynamicData temp;

		temp.TLow = new_thermo[i][0];
		temp.THigh = new_thermo[i][1];
		temp.TChange = new_thermo[i][2];

		temp.NasaLow1 = new_thermo[i][3];
		temp.NasaLow2 = new_thermo[i][4];
		temp.NasaLow3 = new_thermo[i][5];
		temp.NasaLow4 = new_thermo[i][6];
		temp.NasaLow5 = new_thermo[i][7];
		temp.NasaLow6 = new_thermo[i][8];
		temp.NasaLow7 = new_thermo[i][9];

		temp.NasaHigh1 = new_thermo[i][10];
		temp.NasaHigh2 = new_thermo[i][11];
		temp.NasaHigh3 = new_thermo[i][12];
		temp.NasaHigh4 = new_thermo[i][13];
		temp.NasaHigh5 = new_thermo[i][14];
		temp.NasaHigh6 = new_thermo[i][15];
		temp.NasaHigh7 = new_thermo[i][16];

		Thermodynamics.push_back(temp);
	}//*/


	return Thermodynamics;
}
