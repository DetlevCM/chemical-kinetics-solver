/*
 * Write_Single_Reaction.cpp
 *
 *  Created on: 15.02.2015
 *      Author: DetlevCM
 */

#include <MyHeaders.h>


string Prepare_Single_Reaction_Output(
		int Number_Species,
		const vector< string >& Species,
		const SingleReactionData& Reaction
		)
{

	int i;
	bool check;

	//string ReactionsOutput;
	ostringstream convert;

				check = false;
				for(i=0;i<Number_Species;i++) // Line 1
				{
					if(Reaction.Reactants[i] != 0)
					{
						if(Reaction.Reactants[i] == -1.0){
							if(check)
							{
								convert << " + ";
							}
							convert << Species[i];
							check = true;
						}
						else
						{
							if(check)
							{
								convert << " + ";
							}
							convert << fabs(Reaction.Reactants[i]) << Species[i];
							check = true;
						}
					}
				}


				// sign
				if(Reaction.Reversible)
				{
					convert << " =  ";
				}
				else
				{
					convert << " => ";
				}


				// product
				check = false;
				for(i=0;i<Number_Species;i++) // Line 1
				{
					if(Reaction.Products[i] != 0)
					{
						if(Reaction.Products[i] == 1.0){
							if(check)
							{
								convert << " + ";
							}
							convert << Species[i];
							check = true;
						}
						else
						{
							if(check)
							{
								convert << " + ";
							}
							convert << Reaction.Products[i] << Species[i];
							check = true;
						}
					}
				}


				// Arrhenius parameters correct moles/l to moles/cm^3
				/*
				convert << "\t\t" <<
						Reaction.paramA*1000 << "\t" <<
						Reaction.paramN << "\t" <<
						Reaction.paramEa << "\n";
				//*/

				convert << "\t\t" <<
						Reaction.paramA*1000 << "\t" << // mol / cm^3
						Reaction.paramN << "\t" <<
						(Reaction.paramEa/1000*1.98709) << "\n"; // correct for kcal/mol



		return convert.str();
}
