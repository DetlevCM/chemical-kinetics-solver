#include "../headers/Headers.hpp"

vector< string > Get_Species(string filename)
{

	// should follow the pattern used for thermodynamic data and reactions

	vector<string> Species_List = Read_Chemkin_Block(filename, "SPEC");

	// assuming the list has been read in correctly:

	vector<string> All_Species;

	for(size_t i=1;i<Species_List.size();i++)
	{
		string line = Species_List[i];
		vector< string > Remove_Comments;
		if(line.find("!")!=string::npos) // contains comments, so remove comments
		{
			Remove_Comments = Tokenise_String_To_String(line , "!" );
			line = Remove_Comments[0];
		}
		Remove_Comments.clear();

		if(!line.empty())
		{
			if(line.find(" ")!=string::npos || line.find("\t")!=string::npos) // contains spaces or tabs, so tokenize
			{
				vector<string> tmp = Tokenise_String_To_String(line , " \t" );
				for(size_t j=0;j<tmp.size();j++)
				{
					if(!tmp[j].empty())
					{
						All_Species.push_back(tmp[j]);
					}
				}
			}
			else
			{
				All_Species.push_back(line);
			}
		}
	}

	return All_Species;
}
