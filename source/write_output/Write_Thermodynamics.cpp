/*
 * C_Write_Thermodynamics.cpp
 *
 *  Created on: 20 Nov 2012
 *      Author: pmdcm
 */
#include <MyHeaders.h>
// Ofstream reference: http://www.cplusplus.com/reference/fstream/ofstream/


void Write_Thermodynamic_Data(
		string filename,
		const vector< string >& Species,
		const vector< ThermodynamicData >& Thermodynamics
)
{
	int i;
	ofstream ThermodynamicsOutput (filename.c_str(),ios::out);

	if (ThermodynamicsOutput.is_open())
	{
		int Number_Species = (int)Thermodynamics.size();
		ThermodynamicsOutput << "ThermData \n";
		for(i=0;i<Number_Species;i++)
		{
			ThermodynamicsOutput << Species[i] << "\n";

			ThermodynamicsOutput
			<< Thermodynamics[i].TLow
			<< "	" << Thermodynamics[i].THigh
			<< "	" <<  Thermodynamics[i].TChange

			<< "\n" <<  Thermodynamics[i].NasaLow1
			<< "	" <<  Thermodynamics[i].NasaLow2
			<< "	" <<  Thermodynamics[i].NasaLow3
			<< "	" << Thermodynamics[i].NasaLow4
			<< "	" << Thermodynamics[i].NasaLow5
			<< "	" <<  Thermodynamics[i].NasaLow6
			<< "	" << Thermodynamics[i].NasaLow7

			<< "\n" << Thermodynamics[i].NasaHigh1
			<< "	" << Thermodynamics[i].NasaHigh2
			<< "	" << Thermodynamics[i].NasaHigh3
			<< "	" << Thermodynamics[i].NasaHigh4
			<< "	" << Thermodynamics[i].NasaHigh5
			<< "	" << Thermodynamics[i].NasaHigh6
			<< "	" << Thermodynamics[i].NasaHigh7
			<< "\n";

			// One more new line to separate species
			ThermodynamicsOutput << "\n"; // new line
		}
		ThermodynamicsOutput << "End \n";
		ThermodynamicsOutput.close();
	}
	else cout << "Unable to open file";
	cout << "File " << filename << " written. \n";
}

