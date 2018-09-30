/*
 * Handle_InitialConditions.cpp
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#include <Headers.hpp>

void Handle_Initial_Conditions(Initial_Data& InitialParameters, vector<string> Input, solver_type Global_Solver_Settings)
{
	int i ;
	vector< string > line_content;

	for(i=0;i<(int)Input.size();i++)
	{
		if (Test_If_Word_Found(Input[i], "Temperature"))
		{
			line_content = Tokenise_String_To_String(Input[i]," \t");
			if((int)line_content.size()>=2)
			{
				InitialParameters.temperature = stod(line_content[1],NULL);
			}
			line_content.clear();
		}

		if (Test_If_Word_Found(Input[i], "TGA"))
		{
			InitialParameters.TGA_used = true;

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


			//vector <double> temperature_range(3);
			// allows users to provide multiple time points
			if((int)line_content.size()>=3){
				InitialParameters.TGA_rate = stod(line_content[1],NULL);
				InitialParameters.TGA_target = stod(line_content[2],NULL);
			}
			cout << "User requested to simulate a TGA.\n";
		}


		if (Test_If_Word_Found(Input[i], "EndTime"))
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
				double timeend = stod(line_content[1],NULL);
				double timestep = stod(line_content[2],NULL);
				InitialParameters.TimeEnd.push_back(timeend);
				InitialParameters.TimeStep.push_back(timestep);
			}
			// need to modify this function:
			/* 3 positions: user gives end-time and time-step
			 * 4 positions, stiffness/Jacobian is specified
			 */


			solver_type tmp = Global_Solver_Settings;

			if (Test_If_Word_Found(Input[i], "Jacobian"))
			{
				tmp.Use_Analytical_Jacobian = true;
			}
			if (Test_If_Word_Found(Input[i], "NoJacobian"))
			{
				tmp.Use_Analytical_Jacobian = false;
			}
			if (Test_If_Word_Found(Input[i], "Stiff"))
			{
				tmp.Use_Stiff_Solver = true;
			}
			if (Test_If_Word_Found(Input[i], "NoStiff"))
			{
				tmp.Use_Stiff_Solver = false;
			}

			// store the solver choice - either stores the global setting or the user choice
			InitialParameters.Solver_Type.push_back(tmp);

		}

		// Gas Phase Code Extension
		if (Test_If_Word_Found(Input[i], "GasPhasePressure"))// We Assume kPa
		{
			double temp = 0;
			line_content = Tokenise_String_To_String(Input[i]," \t");
			if((int)line_content.size()>=2)
			{
				temp = stod(line_content[1],NULL);
				InitialParameters.GasPhasePressure = temp*1000;
			}
			line_content.clear();
			//SetupParam.GasPhase = true; // activate Gas Phase Correction for User
		}

		if (Test_If_Word_Found(Input[i], "GasPhaseVolume"))// We Assume Litres
		{
			double temp = 0;
			line_content = Tokenise_String_To_String(Input[i]," \t");
			if((int)line_content.size()>=2)
			{
				temp = stod(line_content[1],NULL);
				InitialParameters.GasPhaseVolume = temp/1000; // Convert to m^3
			}
			line_content.clear();
			//SetupParam.GasPhase = true; // activate Gas Phase Correction for User
		}


		// if not in use, value will be zero - if the user has entered both, activate
		if(InitialParameters.GasPhasePressure != 0 && InitialParameters.GasPhaseVolume != 0){
			InitialParameters.GasPhase = true; // activate Gas Phase Correction for User
		}
		// End Gas Phase Code Extension

	}
}


