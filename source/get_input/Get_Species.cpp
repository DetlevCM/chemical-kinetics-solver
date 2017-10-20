#include <MyHeaders.h>

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
					vector< string > RemoveComments;
					RemoveComments = Tokenise_String_To_String(line , "!" );
					Tokenise_String_To_String_Append(temp_species , RemoveComments[0] , " 	" );
					RemoveComments.clear();
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
	return temp_species;
}
