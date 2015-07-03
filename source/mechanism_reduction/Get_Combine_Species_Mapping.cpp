/*
 * C_Combine_Species.cpp
 *
 *  Created on: 19.11.2012
 *      Author: DetlevCM
 */


#include <MyHeaders.h>

vector< vector< string > > Get_Combine_Species_Mapping(string filename)
{

	ifstream Species_Mapping;
	Species_Mapping.open (filename.c_str());

	vector< string > species_mapping;
	vector< vector < string > > temp_mapping;

	int continue_flag = 0, end_flag = 0; // treat as Boolean, true/false
	string line;
	size_t found;

	if (Species_Mapping.is_open())
	{

		while (Species_Mapping.good())
		{
			getline (Species_Mapping,line);

			if(continue_flag && !end_flag)
			{

				found = line.find("END");
				if (found!=string::npos && continue_flag)
				{
					printf("End found\n\r");
					end_flag = 1;
				}

				if(!end_flag && !line.empty()) // check end hasn't been reached and string is not empty
				{

					char * cstr, *p;
					string str = line;

					cstr = new char [str.size()+1];
					strcpy (cstr, str.c_str());

					//cout << "Pre processing " << line << "  Post Processing " ;

					vector< string > RemoveComments;
					//*
					found = line.find("!");
					if (found!=string::npos && !end_flag)
					{
						p=strtok (cstr,"!");
						RemoveComments.push_back(p);
						str = RemoveComments[0];
						//printf("%s \n",RemoveComments[0].c_str());
						line = RemoveComments[0];
						delete[] cstr;
						RemoveComments.clear();
					}//*/


					str = line;//aString;
					cstr = new char [str.size()+1];
					strcpy (cstr, str.c_str());

					p=strtok (cstr," 	"); // split line by space or tab

					// Species name in position 0, 1 left blank if no group/class
					while (p!=NULL)
					{
						species_mapping.push_back(p);
						p=strtok(NULL," 	");
					}

					if(species_mapping.size() == 1) // species with no mapping have only one position
					{
						species_mapping.push_back("0"); // assign 0 to unmapped species...
					}

					temp_mapping.push_back(species_mapping);
					species_mapping.clear(); // important!

				}
			}

			// Moving species check to end of function avoids "MAPPING" being read in as a name
			found = line.find("MAPPING");
			if (found!=string::npos)
			{
				printf("Mapping found!\n\r");
				continue_flag=1;
			}

		}
		Species_Mapping.close();
	}

	return temp_mapping;
}
