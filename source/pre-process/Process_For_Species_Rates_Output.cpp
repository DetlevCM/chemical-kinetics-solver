/*
 * Process_For_Species_Rates_Output.cpp
 *
 *  Created on: 27 Mar 2016
 *      Author: DetlevCM
 */

#include "MyHeaders.h"

struct RatesPerSpeciesOut {
	vector< int > Consumption;
	vector< int > Production;
};

void Process_For_Species_Rates_Output(vector< int > SelectedSpecies, vector< SingleReactionData > Reactions)
{
	// this will need desired species
	// and reactions

	// Species by Reaction
	vector< RatesPerSpeciesOut > OutputMatrix;

	int Number_SelectedSpecies = (int) SelectedSpecies.size();
	int i;

}


void Prepare_Print_Rates_Per_Species(
		const vector< TrackSpecies > & ProductsForRates,
		const vector< TrackSpecies > & ReactantsForRates,
		const vector< double > & Rates,
		const double current_time,
		const vector< string > Species,
		const vector< SingleReactionData >& Reactions
)
{

	int i;
	int Number_Species = (int) Species.size();
	int Number_Reactions = (int) Rates.size();

	ofstream OutfileProd;
	ofstream OutfileCons;

	for(i=0;i<Number_Species;i++)
	{
		string OutFilenameProd, OutFilenameCons;

		OutFilenameProd = Species[i] + "-Prod.txt";
		OutFilenameCons = Species[i] + "-Cons.txt";

		OutfileProd.open(OutFilenameProd.c_str(),ios::out);
		OutfileCons.open(OutFilenameCons.c_str(),ios::out);

		int j;


		if(OutfileProd.is_open())
		{
			OutfileProd << current_time << " 	";

			// sort first, then print out sorted output
			for(j=0;j<Number_Reactions;j++)
			{

				OutfileProd << "Reaction." << j <<". 	";
			};
			OutfileProd << "\n";
		};
		OutfileProd.close();

		if(OutfileCons.is_open())
		{
			OutfileCons << current_time << " 	";

			// sort first, then print out sorted output
			for(j=0;j<Number_Reactions;j++)
			{

				OutfileCons << "Reaction." << j <<". 	";
			};
			OutfileCons << "\n";
		};
		OutfileCons.close();
	}
}


void Print_Rates_Per_Species(
		const vector< TrackSpecies > & ProductsForRates,
		const vector< TrackSpecies > & ReactantsForRates,
		const vector< double > & Rates,
		const double current_time,
		const vector< string > Species,
		const vector< SingleReactionData >& Reactions
)
{

	int i;

	// ideally I can be pickier with calculating rates...


	int Number_Species = (int) Species.size();
	int Number_Reactions = (int) Rates.size();
	//int Number_Of_Selected_Species = (int) SpeciesSelection.size();

	vector< vector < str_RatesAnalysis > > RatesAnalysisData;
	vector< str_RatesAnalysis > temp(Number_Reactions);
	for(i=0;i<Number_Species;i++)
	{
		RatesAnalysisData.push_back(temp);
	}


	vector< str_RatesAnalysis > RatesAnalysisDataTimeStepTotal(Number_Species);


	for(i = 0;i< (int) ProductsForRates.size();i++){
		RatesAnalysisData[ProductsForRates[i].SpeciesID][ProductsForRates[i].ReactionID].productionrate =
				Rates[ProductsForRates[i].ReactionID];
		RatesAnalysisData[ProductsForRates[i].SpeciesID][ProductsForRates[i].ReactionID].prod_time = current_time;

		RatesAnalysisDataTimeStepTotal[ProductsForRates[i].SpeciesID].productionrate =
				RatesAnalysisDataTimeStepTotal[ProductsForRates[i].SpeciesID].productionrate +
				Rates[ProductsForRates[i].ReactionID];
	}

	for(i = 0;i< (int) ReactantsForRates.size();i++){
		RatesAnalysisData[ReactantsForRates[i].SpeciesID][ReactantsForRates[i].ReactionID].consumptionrate =
				Rates[ReactantsForRates[i].ReactionID];
		RatesAnalysisData[ReactantsForRates[i].SpeciesID][ReactantsForRates[i].ReactionID].cons_time = current_time;

		RatesAnalysisDataTimeStepTotal[ReactantsForRates[i].SpeciesID].consumptionrate =
				RatesAnalysisDataTimeStepTotal[ReactantsForRates[i].SpeciesID].consumptionrate +
				Rates[ReactantsForRates[i].ReactionID];
	}


	ofstream OutfileProd;
	ofstream OutfileCons;

	for(i=0;i<Number_Species;i++)
	{
		string OutFilenameProd, OutFilenameCons;

		OutFilenameProd = Species[i] + "-Prod.txt";
		OutFilenameCons = Species[i] + "-Cons.txt";

		OutfileProd.open(OutFilenameProd.c_str(),ios::app);
		OutfileCons.open(OutFilenameCons.c_str(),ios::app);

		int j;


		if(OutfileProd.is_open())
		{
			OutfileProd << current_time << " 	";

			// sort first, then print out sorted output
			for(j=0;j<Number_Reactions;j++)
			{

				OutfileProd << RatesAnalysisData[i][j].productionrate <<" 	";
			};
			OutfileProd << "\n";
		};
		OutfileProd.close();

		if(OutfileCons.is_open())
		{
			OutfileCons << current_time << " 	";

			// sort first, then print out sorted output
			for(j=0;j<Number_Reactions;j++)
			{

				OutfileCons << RatesAnalysisData[i][j].productionrate <<" 	";
			};
			OutfileCons << "\n";
		};
		OutfileCons.close();

		//OutfileProd.close();
		//OutfileCons.close();
	}


}


