/*
 * C_Combine_Species.cpp
 *
 *  Created on: 19.11.2012
 *      Author: DetlevCM
 */
#include <MyHeaders.h>

vector< SingleReactionData > Process_Species_Combination_Reactions_v2(
		int Number_Species_Classes,
		const vector< int >& SpeciesClassMapping,
		vector< SingleReactionData >Reactions,
		double temperature
)
{
	int i,j,k;

	int Number_Species = Reactions[0].Reactants.size();
	int Number_Reactions = Reactions.size();
	// Number_Species_Classes is passed via the function


	/* As we now have an ordered map of Species classes
	 * as well as a count of unmapped species and species classes
	 * , we can begin to compact the reaction matrix */

	//vector< vector< vector< double > > > temp_reactions1; // output
	//vector< vector< double > > SingleReactionData;

	vector< SingleReactionData > temp_reactions1;


	vector< double > ReactantData; // Reactant Information
	ReactantData.resize(Number_Species_Classes);
	vector< double > ProductData; // Product Information
	ProductData.resize(Number_Species_Classes);
	vector< double > ReactionData; // Reaction parameters such as Arrhenius parameters and whether irreversible or not
	//ReactionData.resize(4);

	vector< int > SpeciesClassesSize;
	SpeciesClassesSize.resize(Number_Species_Classes);

	for(i = 0;i<Number_Species;i++)
	{
		SpeciesClassesSize[SpeciesClassMapping[i]]++; // count number of species in each class
	}


	// Per reaction:
	printf("%i reactions and %i species classes to process. \n",Number_Reactions,Number_Species_Classes);
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

			ReactantData[SpeciesID] = ReactantData[SpeciesID] + Reactions[i].Reactants[j];
			ProductData[SpeciesID] = ProductData[SpeciesID] + Reactions[i].Products[j];
		}


		// Reaction Parameters Unaffected
		//ReactionData = Reactions[i][2];

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
	//vector< vector< vector < double > > >  temp_reactions2(temp_reactions1); // make a backup copy of temp reactions
	//vector< vector< vector < double > > >  temp_reactions3;
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

		int compare_position = temp_reactions3.size();
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

			//cout << "1";
		}while(j < (int) temp_reactions2.size());

		Reaction_Grouping.push_back(k); // retain count of combined vectors


		//}while(0 < (int) temp_reactions2.size());// && i < (int) temp_reactions2.size());
		// This used to work, but now it hits an empty array...
	}while(1 < (int) temp_reactions2.size());// && i < (int) temp_reactions2.size());


	/* We have now rearranged out reactions with identical reactions clustered together, identifiable via the mapping
	 * stored in the vector "Reaction_Grouping" and the output available in "temp_reactions3".
	 * So what can we do now? Just stupidly averaging the parameters could be a start... */

	temp_reactions2.clear(); // wipe the array, it should be empty anyway

	/* Our parameters are stored in temp_reactions3[][2][] -> the first 3 parameters for every reaction.
	 */



	i = 0;
	do{

		vector< double > ReactantData(temp_reactions3[i].Reactants); // Reactant Information
		vector< double > ProductData(temp_reactions3[i].Products); // Product Information
		//vector< double > ReactionData; // Reaction parameters such as Arrhenius parameters and whether irreversible or not
		//vector< vector< double > > SingleReactionData; // 1 reaction


		SingleReactionData SingleReaction;


		double Average_A = 0;
		double Average_n = 0;
		double Average_Ea = 0;

		if(Reaction_Grouping[i] > 1)
		{

			vector <double> Group_k(41);

			for(j=0;j<Reaction_Grouping[i];j++)
			{
				// work out k over all reactions
				for(int k=0;k<41;k++){
					double temp_mod = k - 20;

					Group_k[k] =
							Group_k[k] +
							temp_reactions3[j].paramA *
							pow((temperature + temp_mod),temp_reactions3[j].paramN) *
							exp(-temp_reactions3[j].paramEa/(temperature + temp_mod));
				}
			}

			// Fit new A and Ea with n = 0 - I have k over a temperature range of 40K

			for(int k=0;k<41;k++){
				Group_k[k] = log(Group_k[k]);
			}

			// average gradient of k vs 1/T
			double gradient = 0;


			for(int k=1;k<41;k++){
				gradient = gradient +
						(
								(Group_k[k-1]-Group_k[k])/
								(1/(temperature-1))-(1/temperature)
						); // /1 -> steps of 1K but inverse must be accounted for
			}
			gradient = gradient/40; //40 points I think...

			// now need the intercept
			double intercept  = Group_k[20]+(1/temperature)*gradient;

			Average_A = exp(intercept);
			Average_n = 0;
			Average_Ea = gradient;


		}
		else // avoid the maths for reactions that aren't grouped
		{
			Average_A = temp_reactions3[i].paramA;
			Average_n = temp_reactions3[i].paramN;
			Average_Ea = temp_reactions3[i].paramEa;
		}



		//ReactionData.push_back(Average_A);
		//ReactionData.push_back(Average_n);
		//ReactionData.push_back(Average_Ea);
		//ReactionData.push_back(1); // Set reaction to irrvervesible which it shoud be anyway
		//ReactionData.resize(4);

		SingleReaction.Reactants = ReactantData;
		SingleReaction.Products = ProductData;
		SingleReaction.paramA = Average_A;
		SingleReaction.paramN = Average_n;
		SingleReaction.paramEa = Average_Ea;
		SingleReaction.Reversible = false; // lumping only works on irreversible scheme
		SingleReaction.IsDuplicate = false; // this will break Chemkin compatibility, but short of reanalyzing the scheme duplicates can't be identified

		temp_reactions2.push_back(SingleReaction);

		ReactantData.clear();
		ProductData.clear();
		//ReactionData.clear();
		//SingleReactionData.clear();

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
