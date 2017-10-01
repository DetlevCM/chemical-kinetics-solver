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
			InitialParameters.Param_Solver.Use_Analytical_Jacobian = true;
		}
		if (Input[i].find("Force Stability")!=string::npos) // force no negative species
		{
			InitialParameters.EnforceStability = true;
		}
		if (Input[i].find("Use General Solver")!=string::npos) // use solver that can handle non stiff problems better but is slower
		{
			InitialParameters.Param_Solver.Use_Stiff_Solver = false;
		}

		if(Input[i].find("hm")!=string::npos)
		{
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.Param_Solver.minimum_stepsize = strtod(p,NULL);
				p=strtok(NULL," \t");
			}
			cout << "minimum stepsize (h): " << InitialParameters.Param_Solver.minimum_stepsize << "\n";
		}

		if(Input[i].find("initialh")!=string::npos)
		{
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.Param_Solver.initial_stepsize = strtod(p,NULL);
				p=strtok(NULL," \t");
			}
			cout << "initial stepsize (h): " << InitialParameters.Param_Solver.initial_stepsize << "\n";
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
			InitialParameters.Param_Solver.rtol = inputs[0];
			InitialParameters.Param_Solver.threshold = inputs[1];

			cout << "relative tolerance: " << InitialParameters.Param_Solver.rtol << "\n";
			cout << "threshold: " << InitialParameters.Param_Solver.threshold << "\n";
		}

		if(Input[i].find("Threshold")!=string::npos || Input[i].find("threshold")!=string::npos)
		{
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.Param_Solver.threshold = strtod(p,NULL);
				p=strtok(NULL," \t");
			}

			cout << "threshold: " << InitialParameters.Param_Solver.threshold << "\n";
		}

		if(Input[i].find("RTOL")!=string::npos || Input[i].find("rtol")!=string::npos)
		{
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.Param_Solver.rtol = strtod(p,NULL);
				p=strtok(NULL," \t");
			}

			cout << "relative tolerance: " << InitialParameters.Param_Solver.rtol << "\n";
		}

		if(Input[i].find("IRREV")!=string::npos)
		{
			InitialParameters.irrev = true;
		}

		if(Input[i].find("Print Rates")!=string::npos)
		{
			InitialParameters.PrintReacRates = true;
		}

		if(Input[i].find("Comma Separator")!=string::npos)
				{
					InitialParameters.Param_Solver.separator = ",";
				}

		if(Input[i].find("Tab Separator")!=string::npos)
				{
					InitialParameters.Param_Solver.separator = "\t";
				}

		if(Input[i].find("Separator:")!=string::npos)
				{
					p=strtok (cstr,"=: \t"); // break at space or tab
					p=strtok(NULL,"=: \t"); // break again as first is the keyword

					if(p!=NULL){ // only read remainder is something is left
						InitialParameters.Param_Solver.separator = p;
						p=strtok(NULL," \t");
					}
				}


		if(Input[i].find("Stoichiometry Matrix For Opt")!=string::npos)
		{
			InitialParameters.StoichiometryMatrixForOpt = true;
		}

		delete[] cstr;
	}
}

