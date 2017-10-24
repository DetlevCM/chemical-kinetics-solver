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

		// prepare char array with tokenization from the start
		char * cstr, *p;
		string str;
		str = Input[i];//a String - For some reason cannot pass line1 directly...
		cstr = new char [str.size()+1];
		strcpy (cstr, str.c_str());
		InitSpecies SingleSpeciesInput;

		//cout << i << " " << str << "\n";

		// Case 1: No Constant concentration, line will contain two entries
		if(Test_If_Word_Found(Input[i],"ConstantConcentration"))
		{
			p=strtok (cstr," \t"); // p contains the species name


			for(j=0;j<Number_Species;j++){
				string Comparator = Species[j];
				if(strcmp(p,Comparator.c_str()) == 0)
				{
					SingleSpeciesInput.SpecID = j;
					// Obviously assigning only makes sense if I have a species name
					p=strtok(NULL," \t"); // p contains the concentration
					SingleSpeciesInput.SpecConc = strtod(p,NULL);
					SetupSpecies.push_back(SingleSpeciesInput);
				}
			}
			delete[] cstr;
		}

		// Case 2: Constant concentration, line will contain three entries (concentration is in the middle)

		// This will work, but I don't think it is the best implementation
		if(Test_If_Word_Found(Input[i],"ConstantConcentration"))
		{
			// get species concentration the same way
			p=strtok (cstr," \t"); // p contains the species name

			for(j=0;j<Number_Species;j++){
				string Comparator = Species[j];
				if(strcmp(p,Comparator.c_str()) == 0)
				{
					SingleSpeciesInput.SpecID = j;
					// Obviously assigning only makes sense if I have a species name
					p=strtok(NULL," \t"); // p contains the concentration
					SingleSpeciesInput.SpecConc = strtod(p,NULL);
					SetupSpecies.push_back(SingleSpeciesInput);

					InitialParameters.ConstantSpecies.push_back(j); // set one species constant
				}
			}
			InitialParameters.ConstantConcentration = true;
			delete[] cstr;
		}
	}
}



