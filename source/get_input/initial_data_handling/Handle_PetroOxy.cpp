/*
 * Handle_PressureVessel.cpp
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#include <Headers.hpp>


/*
 * Input for the case of a pressure vessel
 */


void Handle_PetroOxy(Initial_Data& InitialParameters, vector<string> Input, vector< string > Species)
{
	bool OxyGasSpeciesDefined = false;

	int i;
	for(i=0;i<(int)Input.size();i++)
	{
		vector< string > line_content;

		if(Test_If_Word_Found(Input[i],"Sample Size="))
		{
			line_content = Tokenise_String_To_String(Input[i],"=");

			// it is mL -> make into m^3
			InitialParameters.PetroOxy.SampleSize = stod(line_content[1],NULL)*1e-6;
			cout << "Sample Size: " << InitialParameters.PetroOxy.SampleSize << "\n";
			line_content.clear();
		}

		if(Test_If_Word_Found(Input[i],"Vessel Size="))
		{
			line_content = Tokenise_String_To_String(Input[i],"=");

			// it is mL -> make into m^3
			InitialParameters.PetroOxy.VesselSize = stod(line_content[1],NULL)*1e-6;
			cout << "Vessel Size: " << InitialParameters.PetroOxy.VesselSize << "\n";
			line_content.clear();
			// Calculated volume from estimate is 22.41mL - so let us assume 22.5mL, value is in m^3
		}

		if(Test_If_Word_Found(Input[i],"Initial Pressure="))
		{
			line_content = Tokenise_String_To_String(Input[i],"=");

			// kPa to Pa
			InitialParameters.PetroOxy.InitPressure = stod(line_content[1],NULL) * 1000;
			cout << "Initial Pressure: " << InitialParameters.PetroOxy.InitPressure << "\n";
			line_content.clear();
		}

		if(Test_If_Word_Found(Input[i],"Maximum Pressure="))
		{
			line_content = Tokenise_String_To_String(Input[i],"=");

			// kPa to Pa
			InitialParameters.PetroOxy.MaxPressure = stod(line_content[1],NULL) * 1000;
			cout << "Maximum Pressure: " << InitialParameters.PetroOxy.MaxPressure << "\n";
			line_content.clear();
		}

		if(Test_If_Word_Found(Input[i],"Gas Solubility="))
		{
			line_content = Tokenise_String_To_String(Input[i],"=");

			// kPa to Pa
			InitialParameters.PetroOxy.GasSolubility = stod(line_content[1],NULL);// * 1000;
			cout << "Gas Solubility: " << InitialParameters.PetroOxy.GasSolubility << "\n";
			line_content.clear();
		}


		if(Test_If_Word_Found(Input[i],"Gas Species="))
		{
			line_content = Tokenise_String_To_String(Input[i],"=");

			for(int i = 0;i<(int)Species.size();i++)
			{
				if(strcmp(line_content[1].c_str(),Species[i].c_str()) == 0){
					InitialParameters.PetroOxy.GasSpecies = i;
					OxyGasSpeciesDefined = true;
				}

			}

			cout << "Gas Species: " << Species[InitialParameters.PetroOxy.GasSpecies] << "\n";
			line_content.clear();
		}

		if(Test_If_Word_Found(Input[i],"PetroOxy Temperature Rise="))
		{
			line_content = Tokenise_String_To_String(Input[i],"=");

			// kPa to Pa
			InitialParameters.PetroOxy.TemperatureRise = stod(line_content[1],NULL) * 1000;
			cout << "Temperature Rise: " << InitialParameters.PetroOxy.TemperatureRise << "\n";
			line_content.clear();
		}


		// if all required data has been input, switch PetroOxy on
		// Need the Gas Species ID... - but gas species could be scpecies 0....


		if(InitialParameters.PetroOxy.SampleSize != 0 &&
				InitialParameters.PetroOxy.InitPressure != 0 &&
				InitialParameters.PetroOxy.MaxPressure != 0 &&
				OxyGasSpeciesDefined == true &&
				InitialParameters.PetroOxy.GasSolubility != 0
		){
			InitialParameters.PetroOxy.IsSet = true;
		}




		if(Test_If_Word_Found(Input[i],"Henry Law Diffusion Limit="))
		{
			line_content = Tokenise_String_To_String(Input[i],"=");

			InitialParameters.PetroOxy.HenryLawDiffusionLimit = stod(line_content[1],NULL);
			InitialParameters.PetroOxy.HenryLawDiffusionLimitSet = true;
			line_content.clear();
		}
	}

}

