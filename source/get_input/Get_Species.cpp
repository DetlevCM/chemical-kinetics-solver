#include <MyHeaders.h>

vector< string > Get_Species(string filename)
{

	ifstream Mechanism_Data;
	Mechanism_Data.open (filename.c_str());

	vector<string> temp_species;

	int spec_flag = 0, end_flag = 0; // treat as Boolean, true/false
	string line1;
	size_t found;

	if (Mechanism_Data.is_open())
	{
		while (Mechanism_Data.good())
		{

			getline (Mechanism_Data,line1);

			if(spec_flag && !end_flag)
			{

				found = line1.find("END");
				if (found!=string::npos && spec_flag)
				{
					//printf("End found\n\r");
					end_flag = 1;
				}

				if(!end_flag){

					char *cstr, *p;
					vector< string > SplitLine;

					string str = line1;

					cstr = new char [str.size()+1];
					strcpy (cstr, str.c_str());

					//*
					found = line1.find("!");
					if (found!=string::npos && !end_flag)
					{
						p=strtok (cstr,"!");
						SplitLine.push_back(p);
						str = SplitLine[0];
						delete[] cstr;
						SplitLine.clear();
					}//*/

					cstr = new char [str.size()+1];
					strcpy (cstr, str.c_str());


					p=strtok (cstr," 	"); // split by space and tab
					while (p!=NULL)
					{
						temp_species.push_back(p);
						p=strtok(NULL," 	");
					}

					delete[] cstr;
					delete[] p;
				}
			}

			// Moving species check to end of function avoids "SPECIES" being read in as a name
			found = line1.find("SPEC");
			if (found!=string::npos && !end_flag)
			{
				spec_flag=1;
			}

		}
		Mechanism_Data.close();
	}

	return temp_species;
}