/*
 * Combine-Species-average-Ea-fitted-k.cpp
 *
 *  Created on: 16.01.2016
 *      Author: DetlevCM
 */


#include <MyHeaders.h>


/*
 * Function to produce lumped reactions. The code in this function builds on
 * the original k lumping which assumed n = 0.
 * In the new method, Ea is averaged across the reactions and A and n are
 * fitted to reproduce the desired mechanism behaviour.
 * This enables the user to more closely retain the activation energies
 * Ea as estimated by RMG (or as used in the supplied mechanism).
 */

vector< SingleReactionData > Process_Reactions_For_Species_Lumping(
		int Number_Species_Classes,
		const vector< int >& SpeciesClassMapping,
		vector< SingleReactionData >Reactions,
		double temperature,
		bool FastLumping, // this will calculated only 3 points, hence is fast
		int LumpingType
)
{

	int i,j,k;

	int Number_Species = (int)Reactions[0].Reactants.size();
	int Number_Reactions = (int)Reactions.size();


	/*
	 * As we now have an ordered map of Species classes as well as a count
	 * of unmapped species and species classes, we can begin to compact
	 * the reaction matrix
	 */


	vector< SingleReactionData > temp_reactions1;


	vector< double > ReactantData; // Reactant Information
	ReactantData.resize(Number_Species_Classes);
	vector< double > ProductData; // Product Information
	ProductData.resize(Number_Species_Classes);
	vector< double > ReactionData; // Reaction parameters such as Arrhenius parameters and whether irreversible or not


	vector< int > SpeciesClassesSize;
	SpeciesClassesSize.resize(Number_Species_Classes);

	for(i = 0;i<Number_Species;i++)
	{
		SpeciesClassesSize[SpeciesClassMapping[i]]++; // count number of species in each class
	}


	// Per reaction:
	cout << Number_Reactions << " reactions and " << Number_Species_Classes << " species classes to process.\n";

	for(i=0;i<Number_Reactions;i++)
	{
		SingleReactionData SingleReactionData;

		//printf("i: %i \n j:",i);
		ReactantData.resize(Number_Species_Classes);
		ProductData.resize(Number_Species_Classes);
		// per species

		for(j = 0;j<Number_Species;j++)
		{
			int SpeciesID = SpeciesClassMapping[j]; // get mapping, then re-map

			// need to make sure that two lumped species can react - e.g. R(1). + R(2). ->
			// now lumped into R. which would be 2R. ->
			ReactantData[SpeciesID] = ReactantData[SpeciesID] + Reactions[i].Reactants[j];
			ProductData[SpeciesID] = ProductData[SpeciesID] + Reactions[i].Products[j];
		}


		// Reaction Parameters Unaffected

		// reassemble output
		SingleReactionData.Reactants = ReactantData;
		SingleReactionData.Products = ProductData;

		SingleReactionData.paramA = Reactions[i].paramA;
		SingleReactionData.paramN = Reactions[i].paramN;
		SingleReactionData.paramEa = Reactions[i].paramEa;
		SingleReactionData.Reversible = Reactions[i].Reversible;

		//SingleReactionData.push_back(ReactionData);
		ReactantData.clear();
		ProductData.clear();
		ReactionData.clear();

		temp_reactions1.push_back(SingleReactionData);
		//SingleReactionData.clear();
	}

	/*//Test Matrix
	for(i=0;i<Number_Reactions;i++)
		{
		printf("%i:  ",i);
		for(j = 0;j<Number_Species;j++)
		{
			printf("  %e  ",temp_reactions[i][0][j]);
		}
		printf(" \n");
	}//*/

	/* Time to plan out some data processing:
	 * If we group a  sufficient number of species, it stands to reason that there will
	 * be reactions which now seem to describe identical reactions without being duplicates
	 * on purpose. Hence we should now identify the redundant reactions and group them to
	 * an individual reaction where appropriate.
	 * Given that we need to process a large amount of data, we should take something of a
	 * sieving approach. Take one reaction, compare the rest and filter out any results.
	 * Next we pick the next available reaction and filter again and so on. This approach
	 * means that the computational load will decrease as we progress with the reaction
	 * combination.
	 * In these collection steps, we can assemble the Arrhenius parameters as well to then
	 * determine the new combined parameters. (Stupidly simple approximation again?)
	 * */

	vector< SingleReactionData > temp_reactions2(temp_reactions1);
	vector< SingleReactionData > temp_reactions3;

	vector< int > Reaction_Grouping;

	//for(i=0;i<Number_Reactions;i++)
	do
	{
		/* stepping through the reactions, let us take the first reaction:
		 * Copy the reaction to another array, note positions of reactants
		 * and products as well as their respective coefficient sums.
		 * Compare both reactants and products, if an identical vector is
		 * found, compare sum and if still identical, copy to new matrix
		 * while deleting in the old matrix. Reset counter to previous position
		 * and continue where we stopped. I.e. we removed reaction 10, set counter
		 * to check the new reaction number 10.
		 * Once we have completed the check for one reaction, we take the second
		 * reaction and apply the same technique.
		 * A mapping vector can keep track how many successive reactions are to be
		 * grouped which will then allow us to easily step through the rearranged
		 * system. I.e. a vector that says reactions are 5,1,2,4 -> 5 to group,
		 * 1 to group (uncombined), 2 to group and so on.
		 *
		 */
		//i = 0;

		int compare_position = (int)temp_reactions3.size();
		temp_reactions3.push_back(temp_reactions2[0]); // retain the reaction we are testing
		//temp_reactions2.erase (temp_reactions2.begin() + i); // remove the position i points to, will be first if we started searching, i.e. 0
		temp_reactions2.erase (temp_reactions2.begin() + 0);


		j = 0;
		k = 1;
		do
		{

			// make sure reactants AND products are compared
			if(temp_reactions2[j].Reactants==temp_reactions3[compare_position].Reactants &&
					temp_reactions2[j].Products==temp_reactions3[compare_position].Products)
			{
				// printf("true! \n"); Appears to works :)
				temp_reactions3.push_back(temp_reactions2[j]); // retain the reaction we are testing
				temp_reactions2.erase (temp_reactions2.begin() + j); // remove the position i points to, will be first if we started searching, i.e. 0
				k = k + 1; // record how many we have grouped together, i.e. added to the first pick
				j = j - 1; // removed one position so take a step back
			}
			j = j + 1;

		}while(j < (int) temp_reactions2.size());

		Reaction_Grouping.push_back(k); // retain count of combined vectors

	}while(1 < (int) temp_reactions2.size());


	/* We have now rearranged our reactions with identical reactions clustered together, identifiable via the mapping
	 * stored in the vector "Reaction_Grouping" and the output available in "temp_reactions3".
	 * So what can we do now? Just stupidly averaging the parameters could be a start... */

	temp_reactions2.clear(); // wipe the array, it should be empty anyway

	/* Our parameters are stored in temp_reactions3[][2][] -> the first 3 parameters for every reaction.
	 */

	/*
	 * Time to fit some new parameters to the lumped reaction. This is the new method:
	 * We allow the user to chose whether he wants a fast lumping method on three points
	 * or a slower method that considers every step in a +/-20K interval
	 * The fast method should really be always adequate due to the nature of the function
	 *
	 * The new method works on by using the mathematics of
	 * ln(k) = ln(A) + ln(T^n) + (-Ea/T) || Ea in Kelvin (else -Ea/RT)
	 *
	 * This abuses the fact that 1/T starts to become asymptotic for T>200
	 * Hence, over a small temperature range (e.g. +/- 20K) it can be approximated using
	 * a straight line
	 *
	 * For a known Ea and k/ln(k) to calculate n and A
	 */
	// parameter fitting occcurs here:

	// always starts at the start of the vector and then removes the entry, so no stepping through i


	i = 0;
	do{

		vector< double > ReactantData(temp_reactions3[i].Reactants); // Reactant Information
		vector< double > ProductData(temp_reactions3[i].Products); // Product Information

		SingleReactionData SingleReaction;

		ReactionParameter NewParameters;

		if(Reaction_Grouping[i] > 1)
		{
			if(LumpingType == 1)
			{
				if(FastLumping)
				{
					NewParameters = n_zero_CalculateNewParametersFast(
							temp_reactions3,
							temperature,
							Reaction_Grouping[i]);
				}
				else
				{
					NewParameters = n_zero_CalculateNewParametersSlow(
							temp_reactions3,
							temperature,
							Reaction_Grouping[i]);
				}
			}
			else if(LumpingType == 2)
			{
				if(FastLumping)
				{
					NewParameters = Average_Ea_CalculateNewParametersFast(
							temp_reactions3,
							temperature,
							Reaction_Grouping[i]);
				}
				else
				{
					NewParameters = Average_Ea_CalculateNewParametersSlow(
							temp_reactions3,
							temperature,
							Reaction_Grouping[i]);
				}
			}
			else if(LumpingType == 3)
			{
				NewParameters = Average_Ea_n_zero_k_fitted(
						temp_reactions3,
						temperature,
						Reaction_Grouping[i]);
			}
			/*
			if(UseAverageEaLumping) // use the method for fitting based on k,  Ea is averaged
			{
				if(FastLumping)
				{
					NewParameters = Average_Ea_CalculateNewParametersFast(
							temp_reactions3,
							temperature,
							Reaction_Grouping[i]);
				}
				else
				{
					// Slow method does not exist yet
					//NewParameters = Average_Ea_CalculateNewParametersSlow(
					NewParameters = Average_Ea_CalculateNewParametersFast(
							temp_reactions3,
							temperature,
							Reaction_Grouping[i]);
				}
			}
			else // Use the old method for fitting based on k, with n=0
			{
				if(FastLumping)
				{
					NewParameters = n_zero_CalculateNewParametersFast(
							temp_reactions3,
							temperature,
							Reaction_Grouping[i]);
				}
				else
				{
					NewParameters = n_zero_CalculateNewParametersSlow(
							temp_reactions3,
							temperature,
							Reaction_Grouping[i]);
				}
			}
			//*/

		}
		else // avoid the maths for reactions that aren't grouped, use previous values
		{
			NewParameters.Reversible = false; // must be irreversible, else no lumping
			NewParameters.paramA = temp_reactions3[i].paramA;
			NewParameters.paramN = temp_reactions3[i].paramN;
			NewParameters.paramEa = temp_reactions3[i].paramEa;
		}



		SingleReaction.Reactants = ReactantData;
		SingleReaction.Products = ProductData;

		SingleReaction.paramA = NewParameters.paramA;
		SingleReaction.paramN = NewParameters.paramN;
		SingleReaction.paramEa = NewParameters.paramEa;

		SingleReaction.Reversible = NewParameters.Reversible;
		//SingleReaction.Reversible = false; // lumping only works on irreversible scheme
		SingleReaction.IsDuplicate = false; // this will break Chemkin compatibility, but short of reanalyzing the scheme duplicates can't be identified

		temp_reactions2.push_back(SingleReaction);

		ReactantData.clear();
		ProductData.clear();

		// finally, remove the processed stuff:
		int delete_positions = Reaction_Grouping[i];
		Reaction_Grouping.erase (Reaction_Grouping.begin());
		temp_reactions3.erase (temp_reactions3.begin(), temp_reactions3.begin() + delete_positions);

	}while(temp_reactions3.size() > 0 && Reaction_Grouping.size() > 0);



	// for simplicity, rescale A after the fact:
	for(i=0;i< (int) temp_reactions2.size();i++) // every reaction
	{
		double Scale_A = 1;

		for(j=0;j< (int) temp_reactions2[i].Reactants.size();j++) // every species class
		{
			Scale_A = Scale_A*(pow( (double) SpeciesClassesSize[j],temp_reactions2[i].Reactants[j]));
		}

		temp_reactions2[i].paramA = (double) temp_reactions2[i].paramA * Scale_A;
	}

	return temp_reactions2;
}


