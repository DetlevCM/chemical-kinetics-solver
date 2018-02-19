/*
 * Handle_PressureVessel.cpp
 *
 *  Created on: 26.10.2017
 *      Author: DetlevCM
 */

#include <MyHeaders.h>

/*
 * New input options for the inclusion of a pressure vessel
 */

void Handle_Pressure_Vessel(Initial_Data& InitialParameters, vector<string> Input)
{

	PressureVessel Pressure_Vessel;

	int i;
	for(i=0;i<(int)Input.size();i++)
	{
		vector< string > line_content;

		if(Test_If_Word_Found(Input[i],"Liquid Volume"))
		{
			line_content = Tokenise_String_To_String(Input[i]," \t");

			// it is mL -> make into m^3
			Pressure_Vessel.Liquid_Sample_Volume = stod(line_content[3],NULL)*1e-6;
			cout << "Sample Size: " << InitialParameters.PetroOxy.SampleSize << "\n";
			line_content.clear();
		}

		if(Test_If_Word_Found(Input[i],"Gas Volume"))
		{
			line_content = Tokenise_String_To_String(Input[i]," \t");

			// it is mL -> make into m^3
			Pressure_Vessel.Gas_Sample_Volume = stod(line_content[3],NULL)*1e-6;
			cout << "Sample Size: " << InitialParameters.PetroOxy.SampleSize << "\n";
			line_content.clear();
		}
	}

	// Need to think about useful additional information
	// Maybe Gas Phase pressure so that we can calculate a concentration in case we do not know it?
	// how do I handle a "gap" between expected and actual pressure - mainly from the PetroOxy calculation where I had
	// an initial pressure and a max pressure which wasn't fully covered by the oxygen component...
	// assume fuel filled the rest? ...

	InitialParameters.Pressure_Vessel = Pressure_Vessel;
}

