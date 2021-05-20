/*
 * WriteSpecies.cpp
 *
 *  Created on: 9 Jan 2013
 *      Author: DetlevCM
 */
#include "../headers/Headers.hpp"


void WriteSpecies(string filename ,const vector< string >& Species)
{
	size_t i;
	ofstream SpeciesOutput (filename.c_str(),ios::out);

	if (SpeciesOutput.is_open())
	{
		size_t Number_Species = 0;
		Number_Species = Species.size();
		SpeciesOutput << "SPECIES\n";
		for(i=0;i<Number_Species;i++)
		{

			SpeciesOutput << "\t" << Species[i] << "\n";
		}
		SpeciesOutput << "END\n";
		SpeciesOutput.close();
	}
	else cout << "Unable to open file";
	cout << "File " << filename << " written.\n";
}

