/*
 * WriteMechanism.cpp
 *
 *  Created on: 21.11.2012
 *      Author: DetlevCM
 */
#include <MyHeaders.h>


void Input_File_For_Ehsan_Opt
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

		for(i=0;i<Number_Reactions;i++)
		{
			Output << i+1 << "\t";
			int counter = 0;
			for(j=0;j<Number_Species;j++)
			{
				if(Reactions[i].Reactants[j] != 0)
				{
					Output << -(j+1) << "\t";
					counter = counter + 1;
				}
			}

			// ensure columns are zero filled if required
			if(counter == 0 )
			{
				Output << "0\t" << "0\t";
			}
			if(counter == 1 )
			{
				Output << "0\t";
			}

			for(j=0;j<Number_Species;j++)
			{
				int counter = 0;
				for(j=0;j<Number_Species;j++)
				{
					if(Reactions[i].Products[j] != 0)
					{
						Output << (j+1) << "\t";
						counter = counter + 1;
					}
				}

				// ensure columns are zero filled if required
				if(counter == 0 )
				{
					Output << "0\t" << "0\t" << "0\t";
				}
				if(counter == 1 )
				{
					Output << "0\t" << "0\t";
				}
				if(counter == 2 )
				{
					Output << "0\t";
				}
			}

			// followed by reaction parameters in the right units
			Output << Reactions[i].paramA*1000  << "\t"
					<< Reactions[i].paramN << "\t"
					<< Reactions[i].paramEa/1000*1.98709;

			Output << "\n";
		}
		Output.close();
	}
	else cout << "Unable to open file";

	cout << "File " << filename << " written.\n";
}


