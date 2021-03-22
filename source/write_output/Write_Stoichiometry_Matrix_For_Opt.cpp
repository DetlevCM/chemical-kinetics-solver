/*
 * WriteMechanism.cpp
 *
 *  Created on: 21.11.2012
 *      Author: DetlevCM
 */
#include "../headers/Headers.hpp"


void Write_Stoichiometric_Matrix_For_Opt
(
		string filename ,
		const vector< SingleReactionData >& Reactions
)
{
	int i, j;
	ofstream Output (filename.c_str(),ios::out);
	if (Output.is_open())
	{
		int Number_Reactions = 0;
		Number_Reactions = (int)Reactions.size();
		int Number_Species = 0;
		Number_Species = (int)Reactions[0].Reactants.size();

		Output << "   ";
		for(i=0;i<Number_Species;i++)
		{
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


