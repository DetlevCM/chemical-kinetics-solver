/*
 * C_Combine_Species.cpp
 *
 *  Created on: 19.11.2012
 *  Revised on: 20.10.2017
 *      Author: DetlevCM
 */

#include <MyHeaders.h>

vector< vector< string > > Get_Combine_Species_Mapping(string filename)
{
	ifstream Input_Data;
	Input_Data.open (filename.c_str());

	string line;
	vector< vector < string > > temp_mapping;

	if (Input_Data.is_open())
	{
		while (Input_Data.good())
		{
			getline (Input_Data,line);
			if(LineNotCommentOrEmpty(line)) // only handle line if not a comment and not empty
			{
				vector< string > Remove_Comments;
				vector< string > Species_Mapping;

				Remove_Comments = Tokenise_String_To_String(line,"!"); // cut away comments
				Species_Mapping = Tokenise_String_To_String(Remove_Comments[0],"\t "); // split entry

				if(Species_Mapping.size() == 1) // species with no mapping have only one position
				{
					Species_Mapping.push_back("0"); // assign 0 to unmapped species...
				}
				temp_mapping.push_back(Species_Mapping);

				Remove_Comments.clear();
				Species_Mapping.clear();
			}
		}
		Input_Data.close();
	}
	return temp_mapping;
}


