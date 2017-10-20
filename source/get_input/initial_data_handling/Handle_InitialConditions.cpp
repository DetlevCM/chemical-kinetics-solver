/*
 * Handle_InitialConditions.cpp
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#include <MyHeaders.h>

void Handle_InitialConditions(InitParam& InitialParameters, vector<string> Input, solver_type Global_Solver_Settings)
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

		if (Test_If_Word_Found("Temperature", Input[i]))
		{
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.temperature = strtod(p,NULL);
				p=strtok(NULL," \t");
			}
		}
		if (Test_If_Word_Found("EndTime", Input[i]))
		{
			string line;
			vector< string > tokens;
			// stupid implementation, but works... - maybe I improve it in the future...
			tokens.push_back("!");
			tokens.push_back("//");
			// remove comments
			line = Strip_Single_Line_Comments(Input[i],tokens);

			// split the line into individual components
			vector< string > line_content;
			line_content = Tokenise_String_To_String(line," \t");


			// allows users to provide multiple time points
			if((int)line_content.size()>=3){
				double timeend = strtod(line_content[1].c_str(),NULL);
				double timestep = strtod(line_content[2].c_str(),NULL);
				InitialParameters.TimeEnd.push_back(timeend);
				InitialParameters.TimeStep.push_back(timestep);
			}
			// need to modify this function:
			/* 3 positions: user gives end-time and time-step
			 * 4 positions, stiffness/Jacobian is specified
			 */

			solver_type tmp = Global_Solver_Settings;

			if (Test_If_Word_Found("Jacobian", Input[i]))
			{
				tmp.Use_Analytical_Jacobian = true;
			}
			if (Test_If_Word_Found("NoJacobian", Input[i]))
			{
				tmp.Use_Analytical_Jacobian = false;
			}
			if (Test_If_Word_Found("Stiff", Input[i]))
			{
				tmp.Use_Stiff_Solver = true;
			}
			if (Test_If_Word_Found("NoStiff", Input[i]))
			{
				tmp.Use_Stiff_Solver = false;
			}

			// store the solver choice - either stores the global setting or the user choice
			InitialParameters.Solver_Type.push_back(tmp);

		}

		// Gas Phase Code Extension
		if (Test_If_Word_Found("GasPhasePressure", Input[i]))// We Assume kPa
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

		if (Test_If_Word_Found("GasPhaseVolume", Input[i]))// We Assume Litres
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


