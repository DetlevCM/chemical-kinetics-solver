/*
 * Handle_Analysis.cpp
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#include "../include/Headers.hpp"



void Handle_Analysis(
		Initial_Data& InitialParameters,
		vector<string> Input,
		vector<string> Species
		)
{
	size_t i, j;
	vector< string > line_content;

	for(i=0;i<Input.size();i++)
	{

		if (Test_If_Word_Found(Input[i],"RatesMaxAnalysis"))
		{
			InitialParameters.MechanismAnalysis.MaximumRates = true;
			cout << "Identification of maximum rates desired.\n";
		}
		if (Test_If_Word_Found(Input[i],"StreamRatesAnalysis"))
		{
			InitialParameters.MechanismAnalysis.StreamRatesAnalysis = true;
		}
		if (Test_If_Word_Found(Input[i],"RatesSpeciesAllAnalysis"))
		{
			InitialParameters.MechanismAnalysis.RatesSpeciesAllAnalysis = true;
		}

		if (Test_If_Word_Found(Input[i],"RatesAnalysisAtTime"))
		{
			InitialParameters.MechanismAnalysis.RatesAnalysisAtTime = true; // user wants rates at specified times

			line_content = Tokenise_String_To_String(Input[i]," \t");
			for(j=1;j<line_content.size();j++) // start at 1, as first position is the keyword
			{
				InitialParameters.MechanismAnalysis.RatesAnalysisAtTimeData.push_back(stod(line_content[j],NULL));
			}
			line_content.clear(); // tidy up

			// catch empty array
			if((int)InitialParameters.MechanismAnalysis.RatesAnalysisAtTimeData.size()==0)
			{
				cout << "error, no times for rates analysis specified, switched off\n";
				InitialParameters.MechanismAnalysis.RatesAnalysisAtTime = false;
			}
		}

		if (Test_If_Word_Found(Input[i],"RatesOfSpecies"))
		{
			InitialParameters.MechanismAnalysis.RatesOfSpecies = true; // user wants rates at specified times

			size_t k;
			line_content = Tokenise_String_To_String(Input[i]," \t");

			for(j=1;j<line_content.size();j++) // start at 1, as first position is the keyword
			{
				// check which species has been named for analysis and store its ID
				for(k=0;k<Species.size();k++){
					if(strcmp(line_content[j].c_str(),Species[k].c_str()) == 0)
					{
						InitialParameters.MechanismAnalysis.SpeciesSelectedForRates.push_back(k);
					}
				}
			}

			// catch empty array
			if((int)InitialParameters.MechanismAnalysis.SpeciesSelectedForRates.size()==0)
			{
				cout << "error, no species for rates analysis specified, switched off\n";
				InitialParameters.MechanismAnalysis.RatesOfSpecies = false;
			}
		}


	}
}

