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
			string line;
			vector< string > tokens;
			// stupid implementation, but works... - maybe I improve it in the future...
			tokens.push_back("!");
			tokens.push_back("//");
			// remove comments
			line = Strip_Single_Line_Comments(Input[i],tokens);
			/*
			vector< double > input;
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			while(p!=NULL){ // only read remainder is something is left
				input.push_back(strtod(p,NULL));
				p=strtok(NULL," \t");
			}
			 */

			// split the line into individual components
			vector< string > line_content;
			line_content = Tokenise_String_To_String(line," \t");


			// allows users to provide multiple time points
			if((int)line_content.size()>=3){
				//InitialParameters.TimeEnd.push_back(input[0]);
				//InitialParameters.TimeStep.push_back(input[1]);
				double timeend = strtod(line_content[1].c_str(),NULL);
				double timestep = strtod(line_content[2].c_str(),NULL);
				InitialParameters.TimeEnd.push_back(timeend);
				InitialParameters.TimeStep.push_back(timestep);
			}
			// need to modify this function:
			/* 3 positions: user gives endtime and timestep
			 * 4 positions, stiffness/Jacobian/solver is specified
			 * -> max 6 positions
			 */

			solver_type tmp = Global_Solver_Settings;

			if (Input[i].find("Jacobian")!=string::npos)
			{
				tmp.Use_Analytical_Jacobian = true;
			}
			if (Input[i].find("NoJacobian")!=string::npos)
			{
				tmp.Use_Analytical_Jacobian = false;
			}
			if (Input[i].find("Stiff")!=string::npos)
			{
				tmp.Use_Stiff_Solver = true;
			}
			if (Input[i].find("NoStiff")!=string::npos)
			{
				tmp.Use_Stiff_Solver = false;
			}

			// switching the solver might be riscky right now...
			// just keep the global option for now
			/*
			if (Input[i].find("Intel")!=string::npos)
			{
				tmp.SolverType = 0;
			}
			if (Input[i].find("Odepack")!=string::npos)
			{
				tmp.SolverType = 1;
			}
			//*/

			// store the solver choice - either stores the global setting or the user choice
			InitialParameters.Solver_Type.push_back(tmp);

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


