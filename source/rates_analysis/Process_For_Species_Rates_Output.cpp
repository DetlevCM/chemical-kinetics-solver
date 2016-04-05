/*
 * Process_For_Species_Rates_Output.cpp
 *
 *  Created on: 27 Mar 2016
 *      Author: DetlevCM
 */

#include "MyHeaders.h"


void Prepare_Print_Rates_Per_Species(
		vector< TrackSpecies > & ProductsForRates,
		vector< TrackSpecies > & ReactantsForRates,
		string separator,
		vector< double > & Rates,
		vector< string > Species,
		vector< int > SelectedSpeciesID,
		vector< vector< int > > ReactionsForSpeciesSelectedForRates,
		vector< SingleReactionData >& Reactions
)
{

	int i;
	//int Number_Species = (int) Species.size();
	//int Number_Reactions = (int) Rates.size();

	ofstream OutfileProd;
	ofstream OutfileCons;

	int Number_Selected_Species = (int) SelectedSpeciesID.size();

	//for(i=0;i<Number_Species;i++)
	for(i=0;i<Number_Selected_Species;i++)
	{
		string OutFilenameProd, OutFilenameCons;

		OutFilenameProd = Species[SelectedSpeciesID[i]] + "-Prod.txt";
		OutFilenameCons = Species[SelectedSpeciesID[i]] + "-Cons.txt";

		OutfileProd.open(OutFilenameProd.c_str(),ios::out);
		OutfileCons.open(OutFilenameCons.c_str(),ios::out);

		int j;

		int Number_Of_Selected_Reactions = ReactionsForSpeciesSelectedForRates[i].size();

		if(OutfileProd.is_open())
		{
			//OutfileProd << "Time 	Total 	";
			OutfileProd << "Time" << separator << "Total" << separator;

			// sort first, then print out sorted output
			/*
			for(j=0;j<Number_Reactions;j++)
			{

				OutfileProd << "Reaction." << j <<". 	";
			};//*/
			for(j=0;j<Number_Of_Selected_Reactions;j++)
			{
				//OutfileProd << RatesAnalysisData[SelectedSpeciesID[i]][j].productionrate <<" 	";
				OutfileProd << "Reaction." <<
						ReactionsForSpeciesSelectedForRates[i][j] <<
						//". 	";
						"." << separator;
			};
			OutfileProd << "\n";
		};
		OutfileProd.close();

		if(OutfileCons.is_open())
		{
			//OutfileProd << "Time 	Total 	";
			OutfileProd << "Time" << separator << "Total" << separator;

			// sort first, then print out sorted output
			/*
			for(j=0;j<Number_Reactions;j++)
			{

				OutfileCons << "Reaction." << j <<". 	";
			};//*/
			for(j=0;j<Number_Of_Selected_Reactions;j++)
			{
				//OutfileProd << RatesAnalysisData[SelectedSpeciesID[i]][j].productionrate <<" 	";
				OutfileCons << "Reaction." <<
						ReactionsForSpeciesSelectedForRates[i][j] <<
						//". 	";
						"." << separator;
			};
			OutfileCons << "\n";
		};
		OutfileCons.close();
	}
}


void Print_Rates_Per_Species(
		vector< TrackSpecies > & ProductsForRates,
		vector< TrackSpecies > & ReactantsForRates,
		const string separator,
		vector< double > & Rates,
		double current_time,
		vector< string > Species,
		vector< int > SelectedSpeciesID,
		vector< vector< int > > ReactionsForSpeciesSelectedForRates,
		vector< SingleReactionData >& Reactions
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

	int Number_Selected_Species = (int) SelectedSpeciesID.size();

	//for(i=0;i<Number_Species;i++)
	for(i=0;i<Number_Selected_Species;i++)
	{
		string OutFilenameProd, OutFilenameCons;

		OutFilenameProd = Species[SelectedSpeciesID[i]] + "-Prod.txt";
		OutFilenameCons = Species[SelectedSpeciesID[i]] + "-Cons.txt";


		OutfileProd.open(OutFilenameProd.c_str(),ios::app);
		OutfileCons.open(OutFilenameCons.c_str(),ios::app);

		int j;

		int Number_Of_Selected_Reactions = ReactionsForSpeciesSelectedForRates[i].size();

		if(OutfileProd.is_open())
		{
			OutfileProd << current_time <<
					//" 	" <<
					separator <<
					RatesAnalysisDataTimeStepTotal[SelectedSpeciesID[i]].productionrate <<
					//" 	";
					separator;

			// sort first, then print out sorted output
			//for(j=0;j<Number_Reactions;j++)
			for(j=0;j<Number_Of_Selected_Reactions;j++)
			{
				//OutfileProd << RatesAnalysisData[SelectedSpeciesID[i]][j].productionrate <<" 	";
				OutfileProd <<
						RatesAnalysisData[SelectedSpeciesID[i]][ReactionsForSpeciesSelectedForRates[i][j]].productionrate <<
						//" 	";
						separator;
			};
			OutfileProd << "\n";
		};
		OutfileProd.close();

		if(OutfileCons.is_open())
		{
			OutfileCons << current_time <<
					//" 	" <<
					separator <<
					RatesAnalysisDataTimeStepTotal[SelectedSpeciesID[i]].consumptionrate <<
					//" 	";
					separator;

			// sort first, then print out sorted output
			//for(j=0;j<Number_Reactions;j++)
			for(j=0;j<Number_Of_Selected_Reactions;j++)
			{
				//OutfileCons << RatesAnalysisData[SelectedSpeciesID[i]][j].consumptionrate <<" 	";
				OutfileCons <<
						RatesAnalysisData[SelectedSpeciesID[i]][ReactionsForSpeciesSelectedForRates[i][j]].consumptionrate <<
						//" 	";
						separator;
			};
			OutfileCons << "\n";
		};
		OutfileCons.close();

		//OutfileProd.close();
		//OutfileCons.close();
	}


}


