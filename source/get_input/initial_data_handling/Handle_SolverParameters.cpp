/*
 * Handle_SolverParameters.cpp
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#include <MyHeaders.h>


void Handle_SolverParameters(InitParam& InitialParameters, vector<string> Input)
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

		if (Input[i].find("Jacobian") != string::npos) // use analytical Jacobian
		{
			InitialParameters.Jacobian = true;
		}
		if (Input[i].find("Force Stability")!=string::npos) // force no negative species
		{
			InitialParameters.EnforceStability = true;
		}
		if (Input[i].find("Use General Solver")!=string::npos) // use solver that can handle non stiff problems better but is slower
		{
			InitialParameters.UseStiffSolver = false;
		}

		if(Input[i].find("hm")!=string::npos)
		{
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.hm = strtod(p,NULL);
				p=strtok(NULL," \t");
			}
		}

		if(Input[i].find("initialh")!=string::npos)
		{
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.h = strtod(p,NULL);
				p=strtok(NULL," \t");
			}
		}


		if(Input[i].find("Tolerance")!=string::npos)
		{
			vector< double > inputs;
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			while(p!=NULL){ // only read remainder is something is left
				inputs.push_back(strtod(p,NULL));
				p=strtok(NULL," \t");
			}
			InitialParameters.rtol = inputs[0];
			InitialParameters.threshold = inputs[1];
		}

		if(Input[i].find("Threshold")!=string::npos)
		{
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.threshold = strtod(p,NULL);
				p=strtok(NULL," \t");
			}
		}

		if(Input[i].find("RTOL")!=string::npos)
		{
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.rtol = strtod(p,NULL);
				p=strtok(NULL," \t");
			}
		}

		if(Input[i].find("IRREV")!=string::npos)
		{
			InitialParameters.irrev = true;
		}

		if(Input[i].find("Print Rates")!=string::npos)
		{
			InitialParameters.PrintReacRates = true;
		}


		if(Input[i].find("Stoichiometry Matrix For Opt")!=string::npos)
		{
			InitialParameters.StoichiometryMatrixForOpt = true;
		}

		delete[] cstr;
	}
}

