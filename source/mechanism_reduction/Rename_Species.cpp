/*
 * C_RenameSpecies.cpp
 *
 *  Created on: 21.11.2012
 *      Author: DetlevCM
 */

#include <MyHeaders.h>


vector< string > RenameSpecies(
		vector< string > Species,
		const vector< int > & SpeciesClassMapping
		)
{
	int i;
	int Number_Species = Species.size();


	int Number_Species_Classes = 0;
	for(i=0;i<Number_Species;i++)
	{
		if(Number_Species_Classes < SpeciesClassMapping[i])
		{
			Number_Species_Classes = SpeciesClassMapping[i];
		}
	}
	Number_Species_Classes = Number_Species_Classes + 1; // class 0 needs 1 position

	vector< string > temp_species_names;
	temp_species_names.resize(Number_Species_Classes);


	for(i=0;i<Number_Species;i++)
	{
		int ClassID = SpeciesClassMapping[i];

		if(temp_species_names[ClassID].empty()) // retain name of species if class empty
		{
			temp_species_names[ClassID] = Species[i];
		}
		else
		{
			stringstream ClassName;
			ClassName << "Class(" << ClassID+1 << ")";
			temp_species_names[ClassID] = ClassName.str(); // otherwise assign Class Name

		}
	}

	return temp_species_names;
}
