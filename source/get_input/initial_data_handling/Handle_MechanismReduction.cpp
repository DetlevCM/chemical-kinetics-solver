/*
 * Handle_MechanismReduction.cpp
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#include <MyHeaders.h>


void Handle_MechanismReduction(InitParam& InitialParameters, vector<string> Input)
{
	int i;
	for(i=0;i<(int)Input.size();i++)
	{
		if(Input[i].find("Use New Lumping")!=string::npos)
		{
			InitialParameters.UseNewLumping = true;
		}
		if(Input[i].find("Use Slow New Lumping")!=string::npos)
		{
			InitialParameters.UseNewLumping = true;
			InitialParameters.UseFastLumping = false;
		}
		if(Input[i].find("Use Slow Lumping")!=string::npos)
		{
			InitialParameters.UseFastLumping = false;
		}
		if(Input[i].find("ReduceReactions")!=string::npos)
		{
			char * cstr, *p;
			string str;
			str = Input[i];//a String - For some reason cannot pass line1 directly...
			cstr = new char [str.size()+1];
			strcpy (cstr, str.c_str());

			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.ReduceReactions = strtod(p,NULL);
				p=strtok(NULL," \t");
			}
			delete[] cstr;
		}
	}
}

