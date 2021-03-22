/*
 * C_Species_Class_Mapping.cpp
 *
 *  Created on: 20 Nov 2012
 *      Author: pmdcm
 */

#include "../headers/Headers.hpp"


vector< int > Map_Species_Classes(
		const vector< vector< string > >& SpeciesMapping,
		const vector< string >& Species
		)
{
	int Number_Species = (int)Species.size();
	/* The Species Mapping is right now chaotic, so maybe sort that out */
	int Species_Mapping_Source_Vector = (int)SpeciesMapping.size();
	vector< int > OrderedSpeciesMapping;
	OrderedSpeciesMapping.resize(Number_Species); // Needs enough space for all species

	int i,j;

	for(i=0;i<Species_Mapping_Source_Vector;i++)
	{
		string Species_Name = SpeciesMapping[i][0];
		/* Now need to go through the mapping vector, step by step... */
		for(j=0;j<Number_Species;j++)
		{
			string Comparator = Species[j];
			if(strcmp(Species_Name.c_str(),Comparator.c_str()) == 0){
				// http://ubuntuforums.org/showthread.php?t=396479

				stringstream MappingParameter(SpeciesMapping[i][1]);
				if( (MappingParameter >> OrderedSpeciesMapping[j]).fail() )
				{
					cout << "Failure in Species Ordering... \n";
				}
				//OrderedSpeciesMapping[j] = strtod(SpeciesMapping[i][j].c_str(),NULL); // compare class ID to int

			}
		}
	}
	//printf("Species Ordered Successfully! \n"); // works

	/* We also need a count of the number of species classes assigned by the user.
	 * Species in class 0 are considered unclassed */
	int NumberOfClasses = 0;
	int CountUnmappedSpecies = 0;
	int BiggestClass = 0; // assume no assigned classes

	for(i=0;i<Number_Species;i++)
	{
		if(OrderedSpeciesMapping[i]==0) // every unassigned species is its own species class
		{
			//printf("Species not in class: %u \n",i);
			CountUnmappedSpecies = CountUnmappedSpecies + 1;
		}
		// Assuming Classes are numbered consecutively, the largest class is the number of classes
		if(BiggestClass < OrderedSpeciesMapping[i])
		{
			BiggestClass = OrderedSpeciesMapping[i];
		}
	}
	NumberOfClasses = CountUnmappedSpecies + BiggestClass; // sum of unmapped species and the number of classes

	cout << "The Mapping consists of " << CountUnmappedSpecies << " unmapped Species, and "
		 << BiggestClass << " dedicated class mappings, resulting in " << NumberOfClasses
		 << " Species Classes. \n";

	// Classed species have a number, unclassed species get identified via a negative ID:
	// This will only work if I coded the rest well as position 0 drops out
	int UnclassedSpeciesGrouping = BiggestClass + 1;

	for(i=0;i<Number_Species;i++)
	{
		if(OrderedSpeciesMapping[i]==0) // every unassigned species is its own species class
		{
			OrderedSpeciesMapping[i] = -UnclassedSpeciesGrouping;
			UnclassedSpeciesGrouping = UnclassedSpeciesGrouping + 1;
		}
	}

	for(i=0;i<Number_Species;i++)
	{
		OrderedSpeciesMapping[i]= abs(OrderedSpeciesMapping[i])-1; // correct to use zero based array and turn negative to positive
	}
	return OrderedSpeciesMapping;
}
