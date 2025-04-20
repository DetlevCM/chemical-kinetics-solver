#include "../headers/Headers.hpp"

vector< string > Get_Species(string filename)
{
	ifstream Mechanism_Data;
	Mechanism_Data.open (filename.c_str());

	vector<string> temp_species;

	int spec_flag = 0, end_flag = 0; // treat as Boolean, true/false


	if (Mechanism_Data.is_open())
	{
		string line;
		while (Mechanism_Data.good())
		{
			getline (Mechanism_Data,line);
			if(spec_flag && !end_flag)
			{
				if (line.find("END")!=string::npos && spec_flag)
				{
					end_flag = 1;
				}
				if(!end_flag){

					vector< string > Remove_Comments;
					if(line.find("!")!=string::npos) // contains comments, so remove comments
					{
						Remove_Comments = Tokenise_String_To_String(line , "!" );
						line = Remove_Comments[0];
					}
					Remove_Comments.clear();
					if(line.find(" ")!=string::npos || line.find("\t")!=string::npos) // contains spaces or tabs, so tokenize
					{
						Tokenise_String_To_String_Append(temp_species , line , " \t" ); // this is really weird, but a " ! " string is required after a species name
					}
					else
					{
						temp_species.push_back(line);
					}
				}
			}
			// Moving species check to end of function avoids "SPECIES" being read in as a name
			if (line.find("SPEC")!=string::npos && !end_flag)
			{
				spec_flag=1;
			}
		}
		Mechanism_Data.close();
	}

	/*
	for(int i=0;i<(int)temp_species.size();i++)
	{
		cout << temp_species[i] << "\n";
	}//*/

	return temp_species;
}
