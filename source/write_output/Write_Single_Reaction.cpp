/*
 * Write_Single_Reaction.cpp
 *
 *  Created on: 15.02.2015
 *      Author: DetlevCM
 */

#include "../headers/Headers.hpp"


string Prepare_Single_Reaction_Output(
		size_t Number_Species,
		const vector< string >& Species,
		const SingleReactionData& Reaction
)
{
	size_t i;
	bool check;

	ostringstream convert;

	double Reaction_Order = 0;

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
				Reaction_Order = Reaction_Order + 1; // stoichiometry 1
			}
			else
			{
				if(check)
				{
					convert << " + ";
				}
				convert << fabs(Reaction.Reactants[i]) << Species[i];
				check = true;
				Reaction_Order = Reaction_Order + fabs(Reaction.Reactants[i]); // stoichiometry not 1
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
	/*convert << "\t\t" << Reaction.paramA*1000 << "\t" << Reaction.paramN << "\t" << Reaction.paramEa << "\n"; //*/

	// ensure that A is scaled correctly to per cm^3 units depending on the reaction order
	double prexponential_A = Reaction.paramA / pow(1000.0,(Reaction_Order-1.0));

	convert << "\t\t" <<
			prexponential_A << "\t" <<
			//Reaction.paramA*1000 << "\t" << // mol / cm^3
			Reaction.paramN << "\t" <<
			(Reaction.paramEa/1000*1.98709) << "\n"; // correct for kcal/mol

	return convert.str();
}


