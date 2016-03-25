/*
 * Handle_PressureVessel.cpp
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#include <MyHeaders.h>



void Handle_PressureVessel(InitParam& InitialParameters, vector<string> Input, vector< string > Species)
{
	bool OxyGasSpeciesDefined = false;

	int i;
	for(i=0;i<(int)Input.size();i++)
	{


		/*
		 * PetroOxy Data Input
		 */

		vector<string> Readout;

		if(string::npos!=Input[i].find("PetroOxy Solvent Sample="))
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
			InitialParameters.PetroOxySampleSize = strtod(Readout[1].c_str(),NULL)*1e-6;
			//PetroOxyData[0] = strtod(Readout[1].c_str(),NULL)*1e-6;
			Readout.clear();

			// Calculated volume from estimate is 22.41mL - so let us assume 22.5mL, value is in m^3
			//PetroOxyData[4] = 22.5*1e-6 - PetroOxyData[0];
		}


		if(string::npos!=Input[i].find("PetroOxy Initial Pressure="))
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
			InitialParameters.PetroOxyInitPressure = strtod(Readout[1].c_str(),NULL) * 1000;
			//PetroOxyData[1] = strtod(Readout[1].c_str(),NULL) * 1000;
			Readout.clear();
		}

		if(string::npos!=Input[i].find("PetroOxy Maximum Pressure="))
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
			InitialParameters.PetroOxyMaxPressure = strtod(Readout[1].c_str(),NULL) * 1000;
			//PetroOxyData[3] = strtod(Readout[1].c_str(),NULL) * 1000;
			Readout.clear();
		}

		if(string::npos!=Input[i].find("PetroOxy Gas Solubility="))
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
			InitialParameters.PetroOxyGasSolubility = strtod(Readout[1].c_str(),NULL);// * 1000;
			//PetroOxyData[9] = strtod(Readout[1].c_str(),NULL);// * 1000;
			Readout.clear();
		}


		if(string::npos!=Input[i].find("PetroOxy Gas Species="))
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
					InitialParameters.PetroOxyGasSpecies = i;
					OxyGasSpeciesDefined = true;
				}

			}

			Readout.clear();
		}

		if(string::npos!=Input[i].find("PetroOxy Temperature Rise="))
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
			InitialParameters.PetroOxyTemperatureRise = strtod(Readout[1].c_str(),NULL) * 1000;
			//PetroOxyData[3] = strtod(Readout[1].c_str(),NULL) * 1000;
			Readout.clear();
		}


		// if all required data has been input, switch PetroOxy on
		// Need the Gas Species ID... - but gas species could be scpecies 0....


		if(InitialParameters.PetroOxySampleSize != 0 &&
				InitialParameters.PetroOxyInitPressure != 0 &&
				InitialParameters.PetroOxyMaxPressure != 0 &&
				OxyGasSpeciesDefined == true &&
				InitialParameters.PetroOxyGasSolubility != 0
		){
			InitialParameters.PetroOxy = true;
		}




		if(string::npos!=Input[i].find("Henry Law Diffusion Limit="))
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

			InitialParameters.HenryLawDiffusionLimit = strtod(Readout[1].c_str(),NULL);
			InitialParameters.HenryLawDiffusionLimitSet = true;
			Readout.clear();

		}



		/*
		 * End PetroOxy Data Input
		 */

	}
}
