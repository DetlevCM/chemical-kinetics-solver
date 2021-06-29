/*
 * Read_Chemkin_Block.cpp
 *
 *  Created on: 29 Jun 2021
 *      Author: detlevcm
 */

#include "../headers/Headers.hpp"


// reads the text in a "Chemkin block", i.e. the text between for example SPECIES and END
// the first line will always contain the header line - e.g. REACTIONS as this contains the units

vector<string> Read_Chemkin_Block(string filename, string blockname)
{
	// most mechanisms only have megabyte sizes, thus it is possible to
	// store the mechanism in memory for easier string processing

	vector<string> Source_File_Lines;

	// we open the file that contains the data
	ifstream File_Data;
	File_Data.open (filename.c_str());

	bool begin_flag = false;
	bool end_flag = false;

	if (File_Data.is_open())
	{
		string line;
		while (File_Data.good())
		{
			getline (File_Data,line);

			if(begin_flag && !end_flag)
			{
				if(line.compare(0,3,"END") == 0 || line.compare(0,3,"end") == 0)
				{
					//cout << "END found\n";
					end_flag = true;
				}

				if(end_flag == false && !line.empty() && line.compare(0,1,"!") != 0) // dont't keep an empty line or comment
					//if(end_flag == 0 && Line_Not_Comment_Or_Empty(line))
				{
					Source_File_Lines.push_back(line);
				}
			}

			// Moving reaction data check to end of function avoids "REAC" being read in as a name
			if(Test_If_Word_Found(line,blockname) && line.compare(0,1,"!") != 0 && begin_flag == false) // only test once... once we found the reactions, its done
			{
				//cout << "REAC found\n";
				begin_flag=true;
				Source_File_Lines.push_back(line);
			}
		}
		File_Data.close();
	}

	return Source_File_Lines;
}

