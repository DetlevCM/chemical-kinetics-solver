/*
 * Species_Class_New_Name.cpp
 *
 *  Created on: 09.01.2016
 *      Author: DetlevCM
 */

#include "../headers/Headers.hpp"


vector<ClassNaming> GetSpeciesClassesNames(vector< vector< string > > SpeciesMapping)
{
	int i;
	int MaxClass = 0;

	vector< ClassNaming > temp1;

	for(i=0;i<(int)SpeciesMapping.size();i++)
	{
		ClassNaming ClassNamingInput;

		// http://stackoverflow.com/questions/7663709/convert-string-to-int-c
		int CurrentClass = (int) strtol(SpeciesMapping[i][1].c_str(),NULL,10); // converting String to int

		if((int) SpeciesMapping[i].size() == 3) // only these have a user defined label
		{
			ClassNamingInput.IsNamed = true;
			ClassNamingInput.ClassID = CurrentClass;
			ClassNamingInput.Name = SpeciesMapping[i][2];
			//cout << ClassNamingInput.Name << " \n"; // works
			temp1.push_back(ClassNamingInput);
		}

		// ensure renaming vector will cover all classes to avoid stumbling blocks
		if( CurrentClass > MaxClass)
		{
			MaxClass = CurrentClass;
		}
	}

	// Now sort them - I am being lazy and assume good input

	vector<ClassNaming> temp2(MaxClass);

	// Insurance - set IsNamed to false first
	for(i=0;i<(int)temp2.size();i++)
	{
		temp2[i].IsNamed = false;
	}

	for(i=0;i<(int)temp1.size();i++){
		temp2[temp1[i].ClassID-1] = temp1[i]; // beware, visible ID is one higher than class ID
		//cout << temp2[temp1[i].ClassID].Name << " \n"; // works
	}

	// Testing Entry
	/*
	for(i=0;i<(int)temp2.size();i++){
			cout << temp2[i].IsNamed << " \n"; // works
	}//*/

	return temp2;
}

