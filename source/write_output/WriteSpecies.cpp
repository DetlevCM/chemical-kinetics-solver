/*
 * WriteSpecies.cpp
 *
 *  Created on: 9 Jan 2013
 *      Author: DetlevCM
 */
#include <MyHeaders.h>


void WriteSpecies(string filename ,const vector< string >& Species)
{

	int i;
	// Stream to output file, output mode
	ofstream SpeciesOutput (filename.c_str(),ios::out);

	if (SpeciesOutput.is_open())
	{

		int Number_Species = 0;
		Number_Species = Species.size();

		//printf("%i",Number_Species);
		SpeciesOutput << "SPECIES\n";

		for(i=0;i<Number_Species;i++)
		{

			SpeciesOutput << "\t" << Species[i] << "\n";
		}

		SpeciesOutput << "END\n";

		SpeciesOutput.close();//*/
	}
	else cout << "Unable to open file";

	printf("File %s written. \n",filename.c_str());

}

