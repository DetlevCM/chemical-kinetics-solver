/*
 * C_RenameSpecies.cpp
 *
 *  Created on: 21.11.2012
 *      Author: DetlevCM
 */

#include "../headers/Headers.hpp"


vector< string > RenameSpecies(
		vector< string > Species,
		vector<ClassNaming> UserDefinedNames,
		const vector< size_t > & SpeciesClassMapping
		)
{
	size_t i;
	size_t Number_Species = Species.size();


	size_t Number_Species_Classes = 0;
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
		size_t ClassID = SpeciesClassMapping[i];

		if(temp_species_names[ClassID].empty()) // retain name of species if class empty
		{
			temp_species_names[ClassID] = Species[i];
		}
		else
		{
			stringstream ClassName;
			if(UserDefinedNames[ClassID].IsNamed) // has user defined name
			{
				ClassName << UserDefinedNames[ClassID].Name;
				temp_species_names[ClassID] = ClassName.str(); // otherwise assign Class Name
			}
			else
			{
			ClassName << "Class(" << ClassID+1 << ")"; // uses generic name
			temp_species_names[ClassID] = ClassName.str(); // otherwise assign Class Name
			}
		}
	}

	return temp_species_names;
}

