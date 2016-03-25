/*
 * Handle_Analysis.cpp
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#include <MyHeaders.h>



void Handle_Analysis(InitParam& InitialParameters, vector<string> Input)
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

		if (Input[i].find("RatesMaxAnalysis")!=string::npos)
		{
			InitialParameters.RatesMaxAnalysis = true;
		}
		if (Input[i].find("StreamRatesAnalysis")!=string::npos)
		{
			InitialParameters.StreamRatesAnalysis = true;
		}
		if (Input[i].find("RatesSpeciesAllAnalysis")!=string::npos)
		{
			InitialParameters.RatesSpeciesAllAnalysis = true;
		}

		if (Input[i].find("RatesAnalysisAtTime")!=string::npos)
		{
			InitialParameters.RatesAnalysisAtTime = true; // user wants rates at specified times

			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			while(p!=NULL){ // only read remainder is something is left
				InitialParameters.RatesAnalysisAtTimeData.push_back(strtod(p,NULL));
				p=strtok(NULL," \t");
			}


			// catch empty array
			if((int)InitialParameters.RatesAnalysisAtTimeData.size()==0)
			{
				cout << "error, no times for rates analysis specified, switched off\n";
				InitialParameters.RatesAnalysisAtTime = false;
			}
		}
		delete[] cstr;
	}
}

