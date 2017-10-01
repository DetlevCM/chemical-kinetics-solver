/*
 * Handle_InitialConditions.cpp
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#include <MyHeaders.h>

void Handle_InitialConditions(InitParam& InitialParameters, vector<string> Input)
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

		if (Input[i].find("Temperature")!=string::npos)
		{
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.temperature = strtod(p,NULL);
				p=strtok(NULL," \t");
			}
		}
		if (Input[i].find("EndTime")!=string::npos)
		{
			vector< double > input;
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			while(p!=NULL){ // only read remainder is something is left
				input.push_back(strtod(p,NULL));
				p=strtok(NULL," \t");
			}

			// allows users to provide multiple time points
			InitialParameters.TimeEnd.push_back(input[0]);
			InitialParameters.TimeStep.push_back(input[1]);
		}

		// Gas Phase Code Extension
		if (Input[i].find("GasPhasePressure")!=string::npos)// We Assume kPa
		{
			//LineIn.clear(); // make sure storage array is empty
			double temp = 0;

			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				temp = strtod(p,NULL);
				p=strtok(NULL," \t");
			}
			//line1.clear();
			delete[] cstr;
			InitialParameters.GasPhasePressure = temp*1000;
			//SetupParam.GasPhase = true; // activate Gas Phase Correction for User
		}

		if (Input[i].find("GasPhaseVolume")!=string::npos)// We Assume Litres
		{
			//LineIn.clear(); // make sure storage array is empty
			double temp = 0;
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			while(p!=NULL){ // only read remainder is something is left
				temp = strtod(p,NULL);
				p=strtok(NULL," \t");
			}
			//line1.clear();
			delete[] cstr;
			InitialParameters.GasPhaseVolume = temp/1000; // Convert to m^3
			//SetupParam.GasPhase = true; // activate Gas Phase Correction for User
		}


		// if not in use, value will be zero - if the user has entered both, activate
		if(InitialParameters.GasPhasePressure != 0 && InitialParameters.GasPhaseVolume != 0){
			InitialParameters.GasPhase = true; // activate Gas Phase Correction for User
		}
		// End Gas Phase Code Extension

		delete[] cstr;
	}
}


