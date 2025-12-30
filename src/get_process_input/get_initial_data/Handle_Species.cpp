/*
 * Handle_Species.cpp
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */


#include "Initial_Data.h"

vector< Initial_Data::InitSpecies > Initial_Data::Handle_Species(
		Initial_Data&  InitialParameters,
		vector<string> Input,
		vector< Species > species
		)
{
	size_t i,j;
	vector< InitSpecies > SetupSpecies;

	for(i=0;i<Input.size();i++)
	{
		vector< string > line_content;
		InitSpecies SingleSpeciesInput;

		// first position: species name
		// second position: species concentration
		// third (& fourth) position: constant concentration keyword if present

		// strip comments
		vector< string > strip_comment = Tokenise_String_To_String(Input[i],"!/*#"); // split
		//line_content = Tokenise_String_To_String(Input[i]," \t");
		line_content = Tokenise_String_To_String(strip_comment[0]," \t");
		strip_comment.clear();

		// Case 1: No Constant concentration, line will contain two entries
		//if(!Test_If_Word_Found(Input[i],"ConstantConcentration") || !Test_If_Word_Found(Input[i],"Constant Concentration"))
		if(line_content.size()>=2)
		{
			for(j=0;j<species.size();j++){
				if(strcmp(line_content[0].c_str(),species[j].Name.c_str()) == 0)
				{
					SingleSpeciesInput.SpecID = j;
					SingleSpeciesInput.SpecConc = stod(line_content[1],NULL);
					SetupSpecies.push_back(SingleSpeciesInput);

					if(Test_If_Word_Found(Input[i],"ConstantConcentration") || Test_If_Word_Found(Input[i],"Constant Concentration"))
					{
						InitialParameters.ConstantSpecies.push_back(j); // set one species constant
						InitialParameters.ConstantConcentration = true;
					}
				}
			}
		}
		line_content.clear(); // clean up
	}

	return SetupSpecies;
}



