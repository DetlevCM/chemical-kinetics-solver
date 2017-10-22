/*
 * WriteMechanism.cpp
 *
 *  Created on: 21.11.2012
 *      Author: DetlevCM
 */
#include <MyHeaders.h>


void WriteReactions(
		string filename ,
		const vector< string >& Species,
		const vector< SingleReactionData >& Reactions
)
{
	int i;
	// Stream to output file, output mode
	ofstream ReactionsOutput (filename.c_str(),ios::out);

	if (ReactionsOutput.is_open())
	{

		int Number_Reactions = 0;
		Number_Reactions = (int)Reactions.size();
		int Number_Species = 0;
		Number_Species = (int)Reactions[0].Reactants.size();

		//ReactionsOutput << "REACTIONS	KELVINS	MOLES 	\n";
		ReactionsOutput << "REACTIONS	KCAL/MOL	MOLES 	\n";

		for(i=0;i<Number_Reactions;i++)
		{
			ReactionsOutput << Prepare_Single_Reaction_Output(Number_Species, Species, Reactions[i]);
			//cout << Reactions[i].IsDuplicate << "\n";
			if(Reactions[i].IsDuplicate == true){
				ReactionsOutput << "DUPLICATE\n";
			}
		}

		ReactionsOutput << "END\n";

		ReactionsOutput.close();
	}
	else cout << "Unable to open file";

	cout << "File " << filename << " written.\n";
}


