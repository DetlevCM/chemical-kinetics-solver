/*
 * WriteMechanism.cpp
 *
 *  Created on: 21.11.2012
 *      Author: DetlevCM
 */
#include <MyHeaders.h>


void Write_Stoichiometric_Matrix_For_Opt
(
		string filename ,
		//const vector< string >& Species,
		const vector< SingleReactionData >& Reactions
)
{
	int i, j;
	// Stream to output file, output mode
	ofstream Output (filename.c_str(),ios::out);

	if (Output.is_open())
	{

		int Number_Reactions = 0;
		Number_Reactions = Reactions.size();
		int Number_Species = 0;
		Number_Species = Reactions[0].Reactants.size();

		Output << "   ";

		for(i=0;i<Number_Species;i++)
		{
			//Output << Species[i] << " ";
			Output << i+1 << "  ";
		}
		Output << "\n";

		for(i=0;i<Number_Reactions;i++)
		{
			Output << i+1 << "  ";

			for(j=0;j<Number_Species;j++)
			{
				Output << Reactions[i].Products[j] - Reactions[i].Reactants[j] << "  ";

			}
			Output << "\n";
		}

		Output.close();
	}
	else cout << "Unable to open file";

	printf("File %s written. \n",filename.c_str());
}


