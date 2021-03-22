/*
 * Reduce_Reactions.cpp
 *
 *  Created on: 28.12.2012
 *      Author: DetlevCM
 */

#include "../headers/Headers.hpp"



void ReactionRateImportance(
		vector< double >& KeyRates,
		const vector< double >& Rates,
		double parameter
)
{

	int i;

	// I can now process the rates to get an idea as to which reactions I can remove

	double maxrate = 0;

	for(i=0;i<(int)Rates.size();i++)
	{
		if(maxrate<Rates[i])
		{
			maxrate = Rates[i];
		}
	}

	// Now determine if the value is important
	double threshhold = maxrate*pow(10,-parameter);// / (exp(10.0,parameter));
	//cout << parameter << " " << maxrate << " " << threshhold << "\n";

	for(i=0;i<(int)Rates.size();i++)
	{
		if(Rates[i] >= threshhold)
		{
			KeyRates[i] = KeyRates[i] + 1;
		}
	}
}



// retain the reactions found to be important
vector< SingleReactionData > ReduceReactionsNew(
		const vector< string >& Species,
		const vector < SingleReactionData >& Reactions,
		const vector< double >& SelectedReactions
)
{

	int i;

	vector < SingleReactionData > ReducedReactions ;
	vector < SingleReactionData > ReducedReactionsRemoved ;

	for(i=0;i<(int)SelectedReactions.size();i++)
	{
		//cout << i << "\n";
		if(SelectedReactions[i]>0)
		{
			ReducedReactions.push_back(Reactions[i]);
		}
		else
		{
			ReducedReactionsRemoved.push_back(Reactions[i]);
		}
	}

	if(ReducedReactionsRemoved.size() > 0)
	{
		WriteReactions("Removed_Reactions.txt", Species, ReducedReactionsRemoved);
	}
	//printf("Got to the end of Reaction reduction!! \n");
	return ReducedReactions;
}
