/*
 * Handle_Species.cpp
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#include <MyHeaders.h>

void Handle_Species(InitParam&  InitialParameters, vector< InitSpecies >& SetupSpecies, vector<string> Input, vector< string > Species)
{
	int Number_Species = (int) Species.size();
	int i,j;

	//cout << "The species block contains " << Input.size() << " lines\n";
	for(i=0;i<(int)Input.size();i++)
	{
		vector< string > line_content;
		InitSpecies SingleSpeciesInput;

		// first position: species name
		// second position: species concentration
		// third (& fourth) position: contant concentration keyword if present
		line_content = Tokenise_String_To_String(Input[i]," \t");

		// Case 1: No Constant concentration, line will contain two entries
		if(!Test_If_Word_Found(Input[i],"ConstantConcentration") || !Test_If_Word_Found(Input[i],"Constant Concentration"))
		{
			for(j=0;j<Number_Species;j++){
				if(strcmp(line_content[0].c_str(),Species[j].c_str()) == 0)
				{
					SingleSpeciesInput.SpecID = j;
					SingleSpeciesInput.SpecConc = strtod(line_content[1].c_str(),NULL);
					SetupSpecies.push_back(SingleSpeciesInput);
				}
			}
		}

		// Case 2: Constant concentration, line will contain three or four entries (concentration is in the middle)
		if(Test_If_Word_Found(Input[i],"ConstantConcentration") || !Test_If_Word_Found(Input[i],"Constant Concentration"))
		{
			for(j=0;j<Number_Species;j++){
				if(strcmp(line_content[0].c_str(),Species[j].c_str()) == 0)
				{
					SingleSpeciesInput.SpecID = j;
					SingleSpeciesInput.SpecConc = strtod(line_content[1].c_str(),NULL);
					SetupSpecies.push_back(SingleSpeciesInput);
				}
			}
			InitialParameters.ConstantConcentration = true;
		}

		line_content.clear(); // clean up
	}
}



