/*
 * Handle_PressureVessel.cpp
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#include <MyHeaders.h>


/*
 * Input for the case of a pressure vessel
 */


void Handle_Pressure_Vessel(InitParam& InitialParameters, vector<string> Input, vector< string > Species)
{
	bool OxyGasSpeciesDefined = false;

	int i;
	for(i=0;i<(int)Input.size();i++)
	{

		vector<string> Readout;

		if(Test_If_Word_Found(Input[i],"Sample Size="))
		{
			char * cstr, *p;
			string str = Input[i];
			cstr = new char [str.size()+1];
			strcpy (cstr, str.c_str());

			p=strtok (cstr,"=");
			while (p!=NULL)
			{
				Readout.push_back(p);
				p=strtok(NULL,"<>");
			}
			delete[] cstr;
			delete[] p;

			// it is mL -> make into m^3
			InitialParameters.PressureVessel.SampleSize = strtod(Readout[1].c_str(),NULL)*1e-6;
			cout << "Sample Size: " << InitialParameters.PressureVessel.SampleSize << "\n";
			Readout.clear();
		}

		if(Test_If_Word_Found(Input[i],"Vessel Size="))
		{
			char * cstr, *p;
			string str = Input[i];
			cstr = new char [str.size()+1];
			strcpy (cstr, str.c_str());

			p=strtok (cstr,"=");
			while (p!=NULL)
			{
				Readout.push_back(p);
				p=strtok(NULL,"<>");
			}
			delete[] cstr;
			delete[] p;

			// it is mL -> make into m^3
			InitialParameters.PressureVessel.VesselSize = strtod(Readout[1].c_str(),NULL)*1e-6;
			cout << "Vessel Size: " << InitialParameters.PressureVessel.VesselSize << "\n";
			Readout.clear();
			// Calculated volume from estimate is 22.41mL - so let us assume 22.5mL, value is in m^3
		}

		if(Test_If_Word_Found(Input[i],"Initial Pressure="))
		{
			char * cstr, *p;
			string str = Input[i];
			cstr = new char [str.size()+1];
			strcpy (cstr, str.c_str());

			p=strtok (cstr,"=");
			while (p!=NULL)
			{
				Readout.push_back(p);
				p=strtok(NULL,"<>");
			}
			delete[] cstr;
			delete[] p;

			// kPa to Pa
			InitialParameters.PressureVessel.InitPressure = strtod(Readout[1].c_str(),NULL) * 1000;
			cout << "Initial Pressure: " << InitialParameters.PressureVessel.InitPressure << "\n";
			Readout.clear();
		}

		if(Test_If_Word_Found(Input[i],"Maximum Pressure="))
		{
			char * cstr, *p;
			string str = Input[i];
			cstr = new char [str.size()+1];
			strcpy (cstr, str.c_str());

			p=strtok (cstr,"=");
			while (p!=NULL)
			{
				Readout.push_back(p);
				p=strtok(NULL,"<>");
			}
			delete[] cstr;
			delete[] p;

			// kPa to Pa
			InitialParameters.PressureVessel.MaxPressure = strtod(Readout[1].c_str(),NULL) * 1000;
			cout << "Maximum Pressure: " << InitialParameters.PressureVessel.MaxPressure << "\n";
			Readout.clear();
		}

		if(Test_If_Word_Found(Input[i],"Gas Solubility="))
		{
			char * cstr, *p;
			string str = Input[i];
			cstr = new char [str.size()+1];
			strcpy (cstr, str.c_str());

			p=strtok (cstr,"=");
			while (p!=NULL)
			{
				Readout.push_back(p);
				p=strtok(NULL,"<>");
			}
			delete[] cstr;
			delete[] p;

			// kPa to Pa
			InitialParameters.PressureVessel.GasSolubility = strtod(Readout[1].c_str(),NULL);// * 1000;
			cout << "Gas Solubility: " << InitialParameters.PressureVessel.GasSolubility << "\n";
			Readout.clear();
		}


		if(Test_If_Word_Found(Input[i],"Gas Species="))
		{
			char * cstr, *p;
			string str = Input[i];
			cstr = new char [str.size()+1];
			strcpy (cstr, str.c_str());

			p=strtok (cstr,"=");
			while (p!=NULL)
			{
				Readout.push_back(p);
				p=strtok(NULL,"<>");
			}
			delete[] cstr;
			delete[] p;

			for(int i = 0;i<(int)Species.size();i++)
			{
				if(strcmp(Readout[1].c_str(),Species[i].c_str()) == 0){
					InitialParameters.PressureVessel.GasSpecies = i;
					OxyGasSpeciesDefined = true;
				}

			}

			cout << "Gas Species: " << Species[InitialParameters.PressureVessel.GasSpecies] << "\n";
			Readout.clear();
		}

		if(Test_If_Word_Found(Input[i],"PetroOxy Temperature Rise="))
		{
			char * cstr, *p;
			string str = Input[i];
			cstr = new char [str.size()+1];
			strcpy (cstr, str.c_str());

			p=strtok (cstr,"=");
			while (p!=NULL)
			{
				Readout.push_back(p);
				p=strtok(NULL,"<>");
			}
			delete[] cstr;
			delete[] p;

			// kPa to Pa
			InitialParameters.PressureVessel.TemperatureRise = strtod(Readout[1].c_str(),NULL) * 1000;
			cout << "Temperature Rise: " << InitialParameters.PressureVessel.TemperatureRise << "\n";
			Readout.clear();
		}


		// if all required data has been input, switch PetroOxy on
		// Need the Gas Species ID... - but gas species could be scpecies 0....


		if(InitialParameters.PressureVessel.SampleSize != 0 &&
				InitialParameters.PressureVessel.InitPressure != 0 &&
				InitialParameters.PressureVessel.MaxPressure != 0 &&
				OxyGasSpeciesDefined == true &&
				InitialParameters.PressureVessel.GasSolubility != 0
		){
			InitialParameters.PressureVessel.IsSet = true;
		}




		if(Test_If_Word_Found(Input[i],"Henry Law Diffusion Limit="))
		{
			char * cstr, *p;
			string str = Input[i];
			cstr = new char [str.size()+1];
			strcpy (cstr, str.c_str());

			p=strtok (cstr,"=");
			while (p!=NULL)
			{
				Readout.push_back(p);
				p=strtok(NULL,"<>");
			}
			delete[] cstr;
			delete[] p;

			InitialParameters.PressureVessel.HenryLawDiffusionLimit = strtod(Readout[1].c_str(),NULL);
			InitialParameters.PressureVessel.HenryLawDiffusionLimitSet = true;
			Readout.clear();

		}
	}

}
