/*
 * Reduce_Reactions.cpp
 *
 *  Created on: 28.12.2012
 *      Author: DetlevCM
 */

#include "../headers/Headers.hpp"




// Indentify the largest rates
void MaxRatesAnalysis(
		vector< vector < str_RatesAnalysis > >& RatesAnalysisData,
		const vector< TrackSpecies > & ProductsForRates,
		const vector< TrackSpecies > & ReactantsForRates,
		const vector< double > Rates,
		const double current_time
)
{
	int i;

	// collects the maximum rate per reaction, per species
	for(i = 0;i< (int) ProductsForRates.size();i++){

		if(RatesAnalysisData[ProductsForRates[i].SpeciesID][ProductsForRates[i].ReactionID].productionrate < Rates[ProductsForRates[i].ReactionID])
		{
			RatesAnalysisData[ProductsForRates[i].SpeciesID][ProductsForRates[i].ReactionID].productionrate =
					Rates[ProductsForRates[i].ReactionID];
			RatesAnalysisData[ProductsForRates[i].SpeciesID][ProductsForRates[i].ReactionID].prod_time = current_time;

		}
	}

	for(i = 0;i< (int) ReactantsForRates.size();i++){
		if(RatesAnalysisData[ReactantsForRates[i].SpeciesID][ReactantsForRates[i].ReactionID].consumptionrate < Rates[ReactantsForRates[i].ReactionID])
		{
			RatesAnalysisData[ReactantsForRates[i].SpeciesID][ReactantsForRates[i].ReactionID].consumptionrate =
					Rates[ReactantsForRates[i].ReactionID];
			RatesAnalysisData[ReactantsForRates[i].SpeciesID][ReactantsForRates[i].ReactionID].cons_time = current_time;

		}
	}



}




// Simple for now
void WriteMaxRatesAnalysis(
		vector< vector < str_RatesAnalysis > >& RatesAnalysisData,
		const vector< string >& Species,
		const vector< SingleReactionData >& Reactions,
		string rates_analysis_stream_filename
)
{

	int Number_Species = (int)RatesAnalysisData.size();
	int Number_Reactions = (int)RatesAnalysisData[0].size();
	int i,j;

	i = 0; j = 0;
	ofstream Outfile;

	string filename;

	filename = rates_analysis_stream_filename + "RatesAnalysis-Max-Production-Rate.txt";
	//Outfile.open("RatesAnalysis-Max-Production-Rate.txt");
	Outfile.open(filename.c_str());


	if(Outfile.is_open())
	{
		for(i=0;i<Number_Species;i++)
		{
			Outfile << "Rate of Production For: " << Species[i] << "\n";
			// sort first, then print out sorted output
			for(j=0;j<Number_Reactions;j++)
			{

				// Avogadro's Constant is about 6.2e23 - so a rate of less than 1.0e-22 is less than 1.5 atoms
				if(RatesAnalysisData[i][j].productionrate > 1.0e-22)
				{
					Outfile << "Reaction 	" << j << " 	" <<
							RatesAnalysisData[i][j].productionrate << " 	, at 	" << RatesAnalysisData[i][j].prod_time << " 	 s    ";
					// now need to print the reaction

					Outfile << Prepare_Single_Reaction_Output(Number_Species, Species, Reactions[j]);

				}
			};
			Outfile << "\n\n";
		};
	};
	Outfile.close();




	filename = rates_analysis_stream_filename + "RatesAnalysis-Max-Consumption-Rate.txt";
	Outfile.open(filename.c_str());

	if(Outfile.is_open())
	{
		for(i=0;i<Number_Species;i++)
		{
			Outfile << "Rate of Consumption For: " << Species[i] << "\n";
			// sort first, then print out sorted output
			for(j=0;j<Number_Reactions;j++)
			{

				// Avogadro's Constant is about 6.2e23 - so a rate of less than 1.0e-22 is less than 1.5 atoms
				if(RatesAnalysisData[i][j].consumptionrate > 1.0e-22)
				{
					Outfile << "Reaction 	" << j << " 	" <<
							RatesAnalysisData[i][j].consumptionrate << " 	, at 	" << RatesAnalysisData[i][j].cons_time << " 	s    ";

					// now need to print the reaction
					Outfile << Prepare_Single_Reaction_Output(Number_Species, Species, Reactions[j]);
				};
			};

			Outfile << "\n\n";
		};
	};
	Outfile.close();
}




//*
// Rates Analysis - stream

void PrepareStreamRatesAnalysis(
		const vector< string >& Species,
		string rates_analysis_stream_filename
)
{

	int i;
	int Number_Species = (int) Species.size() - 1;
	ofstream Outfile;

	string filename;
	filename = rates_analysis_stream_filename + "RatesAnalysis-Species-Production-Rate-Stream.txt";

	Outfile.open(filename.c_str());

	if(Outfile.is_open())
	{

		Outfile << "Time 	";
		for(i=0;i<Number_Species;i++)
		{
			Outfile << Species[i] << " 	";
		}
		Outfile << "\n";
	};
	Outfile.close();

	filename = rates_analysis_stream_filename + "RatesAnalysis-Species-Consumption-Rate-Stream.txt";
	Outfile.open(filename.c_str());

	if(Outfile.is_open())
	{
		Outfile << "Time 	";
		for(i=0;i<Number_Species;i++)
		{
			Outfile << Species[i] << " 	";
		}
		Outfile << "\n";
	};
	Outfile.close();
}


void StreamRatesAnalysis(
		const string rates_analysis_stream_filename,
		const vector< TrackSpecies > & ProductsForRates,
		const vector< TrackSpecies > & ReactantsForRates,
		const vector< double > & Rates,
		const double current_time,
		const int Number_Species
)
{
	int i;

	vector< str_RatesAnalysis > RatesAnalysisData(Number_Species);

	// Sum the rates for all species - I'm not intereste in recording which reactions take part
	for(i = 0;i< (int) ProductsForRates.size();i++){
		RatesAnalysisData[ProductsForRates[i].SpeciesID].productionrate =
				RatesAnalysisData[ProductsForRates[i].SpeciesID].productionrate +
				Rates[ProductsForRates[i].ReactionID];
	}

	for(i = 0;i< (int) ReactantsForRates.size();i++){
		RatesAnalysisData[ReactantsForRates[i].SpeciesID].consumptionrate =
				RatesAnalysisData[ReactantsForRates[i].SpeciesID].consumptionrate +
				Rates[ReactantsForRates[i].ReactionID];
	}


	string filename;
	filename = rates_analysis_stream_filename + "RatesAnalysis-Species-Production-Rate-Stream" + ".txt";

	ofstream Outfile;
	Outfile.open(filename.c_str(),ios::app);

	if(Outfile.is_open())
	{
		Outfile << current_time << " 	";
		for(i=0;i<Number_Species-1;i++)
		{
			Outfile << RatesAnalysisData[i].productionrate << " 	";
		}
		Outfile << "\n";
	};
	Outfile.close();



	filename = rates_analysis_stream_filename + "RatesAnalysis-Species-Consumption-Rate-Stream"	+ ".txt";
	Outfile.open(filename.c_str(),ios::app);


	if(Outfile.is_open())
	{
		Outfile << current_time << " 	";
		for(i=0;i<Number_Species-1;i++)
		{
			Outfile << RatesAnalysisData[i].consumptionrate << " 	";
		}
		Outfile << "\n";
	};
	Outfile.close();

}

//*/




// output at specific times

void RatesAnalysisAtTimes(
		const vector< TrackSpecies > & ProductsForRates,
		const vector< TrackSpecies > & ReactantsForRates,
		const vector< double > & Rates,
		const double current_time,
		const vector< string > Species,
		const vector< SingleReactionData >& Reactions
)
{
	int i,j;

	i = 0; j = 0;

	int Number_Species = (int)Species.size();
	int Number_Reactions = (int)Reactions.size();


	vector< vector < str_RatesAnalysis > > RatesAnalysisData;
	vector< str_RatesAnalysis > temp(Number_Reactions);
	for(i=0;i<Number_Species;i++)
	{
		RatesAnalysisData.push_back(temp);
	}

	vector< str_RatesAnalysis > RatesAnalysisDataTimeStepTotal(Number_Species);

	ofstream Outfile;


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


	string filename;

	ostringstream strs;
	strs << current_time;

	filename = "RatesAnalysis-Production-Rate-at-" + strs.str() + "s.txt";

	Outfile.open(filename.c_str());
	if(Outfile.is_open())
	{
		for(i=0;i<Number_Species;i++)
		{
			Outfile << "At Time " << current_time  << "s , total Rate of Production for: " << Species[i] << " "
					<< RatesAnalysisDataTimeStepTotal[i].productionrate << "\n";
			// sort first, then print out sorted output
			for(j=0;j<Number_Reactions;j++)
			{
				// Avogadro's Constant is about 6.2e23 - so a rate of less than 1.0e-24 is less than 1.5 atoms
				if(RatesAnalysisData[i][j].productionrate > 1.0e-22)
				{
					Outfile << "Reaction " << j << " 	" << RatesAnalysisData[i][j].productionrate <<" 	";
					// now need to print the reaction
					Outfile << Prepare_Single_Reaction_Output(Number_Species, Species, Reactions[j]);
				}
			};
			Outfile << "\n\n";
		};
	};
	Outfile.close();

	filename = "RatesAnalysis-Consumption-Rate-at-" + strs.str() + "s.txt";

	Outfile.open(filename.c_str());
	if(Outfile.is_open())
	{
		for(i=0;i<Number_Species;i++)
		{
			Outfile << "At Time " << current_time  << "s , total Rate of Consumption for: " << Species[i] << " "
					<< RatesAnalysisDataTimeStepTotal[i].consumptionrate << "\n";
			// sort first, then print out sorted output
			for(j=0;j<Number_Reactions;j++)
			{
				// Avogadro's Constant is about 6.2e23 - so a rate of less than 1.0e-24 is less than 1.5 atoms
				if(RatesAnalysisData[i][j].consumptionrate > 1.0e-22)
				{
					Outfile << "Reaction " << j << " 	" << RatesAnalysisData[i][j].consumptionrate <<" 	";
					// now need to print the reaction
					Outfile << Prepare_Single_Reaction_Output(Number_Species, Species, Reactions[j]);
				}
			};
			Outfile << "\n\n";
		};
	};
	Outfile.close();
}
