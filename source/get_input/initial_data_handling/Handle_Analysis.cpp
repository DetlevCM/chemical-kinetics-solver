/*
 * Handle_Analysis.cpp
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#include <MyHeaders.h>



void Handle_Analysis(InitParam& InitialParameters, vector<string> Input, vector<string> Species)
{
	int i;
	for(i=0;i<(int)Input.size();i++)
	{
		// prepare char array with tokenization from the start
		char * cstr, *p;
		string str;
		str = Input[i];//a String - For some reason cannot pass line1 directly...
		cstr = new char [str.size()+1];
		strcpy (cstr, str.c_str());

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

			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			while(p!=NULL){ // only read remainder is something is left
				InitialParameters.MechanismAnalysis.RatesAnalysisAtTimeData.push_back(strtod(p,NULL));
				p=strtok(NULL," \t");
			}


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
			//cout << "Rates of Species true\n";
			int Number_Species = (int) Species.size();
			int j;
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			while(p!=NULL){ // only read remainder is something is left

				// check which species has been named for analysis and store its ID
				for(j=0;j<Number_Species;j++){
					string Comparator = Species[j];
					if(strcmp(p,Comparator.c_str()) == 0)
					{
						InitialParameters.MechanismAnalysis.SpeciesSelectedForRates.push_back(j);
						//cout << "found " << Species[j] << "\n";
					}
				}
				p=strtok(NULL," \t");
			}

			// catch empty array
			if((int)InitialParameters.MechanismAnalysis.SpeciesSelectedForRates.size()==0)
			{
				cout << "error, no species for rates analysis specified, switched off\n";
				InitialParameters.MechanismAnalysis.RatesOfSpecies = false;
			}
		}

		delete[] cstr;
	}
}

