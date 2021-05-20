/*
 * Handle_SolverParameters.cpp
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#include "../headers/Headers.hpp"

void Handle_Solver_Parameters(
		Initial_Data& InitialParameters,
		vector<string> Input
		)
{
	size_t i;
	for(i=0;i<Input.size();i++)
	{
		// prepare char array with tokenization from the start
		char * cstr, *p;
		string str;
		str = Input[i];//a String - For some reason cannot pass line1 directly...
		cstr = new char [str.size()+1];
		strcpy (cstr, str.c_str());

		/* Let the user chose the type of solver to use */
		/* maybe at some point I should make the setting terms case insensitive... */
		if(Test_If_Word_Found(Input[i], "intelode"))
		{
			InitialParameters.Solver_Parameters.SolverType = 0;
		}
		if(Test_If_Word_Found(Input[i], "odepack"))
		{
			InitialParameters.Solver_Parameters.SolverType = 1;
		}

		if (Test_If_Word_Found(Input[i], "Jacobian")) // use analytical Jacobian
		{
			InitialParameters.Solver_Parameters.Use_Analytical_Jacobian = true;
		}
		if (Test_If_Word_Found(Input[i], "Force Stability")) // force no negative species
		{
			InitialParameters.EnforceStability = true;
		}
		if (Test_If_Word_Found(Input[i], "Use General Solver")) // use solver that can handle non stiff problems better but is slower
		{
			InitialParameters.Solver_Parameters.Use_Stiff_Solver = false;
		}

		if(Test_If_Word_Found(Input[i], "hm"))
		{
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.Solver_Parameters.minimum_stepsize = strtod(p,NULL);
				p=strtok(NULL," \t");
			}
			cout << "minimum stepsize (h): " << InitialParameters.Solver_Parameters.minimum_stepsize << "\n";
		}

		if(Test_If_Word_Found(Input[i], "initialh"))
		{
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.Solver_Parameters.initial_stepsize = strtod(p,NULL);
				p=strtok(NULL," \t");
			}
			cout << "initial stepsize (h): " << InitialParameters.Solver_Parameters.initial_stepsize << "\n";
		}


		if(Test_If_Word_Found(Input[i], "Tolerance"))
		{
			vector< double > inputs;
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			while(p!=NULL){ // only read remainder is something is left
				inputs.push_back(strtod(p,NULL));
				p=strtok(NULL," \t");
			}
			InitialParameters.Solver_Parameters.rtol = inputs[0];
			//InitialParameters.Param_Solver.threshold = inputs[1];
			InitialParameters.Solver_Parameters.atol = inputs[1];

			cout << "relative tolerance: " << InitialParameters.Solver_Parameters.rtol << "\n";
			//cout << "threshold: " << InitialParameters.Param_Solver.threshold << "\n";
			cout << "absolute tolerance: " << InitialParameters.Solver_Parameters.atol << "\n";
		}

		if(Test_If_Word_Found(Input[i], "RTOL"))
		{
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.Solver_Parameters.rtol = strtod(p,NULL);
				p=strtok(NULL," \t");
			}

			cout << "relative tolerance: " << InitialParameters.Solver_Parameters.rtol << "\n";
		}

		if(Test_If_Word_Found(Input[i], "ATOL"))
		{
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.Solver_Parameters.atol = strtod(p,NULL);
				p=strtok(NULL," \t");
			}

			cout << "absolute tolerance: " << InitialParameters.Solver_Parameters.atol << "\n";
		}

		if(Test_If_Word_Found(Input[i], "IRREV"))
		{
			InitialParameters.irrev = true;
		}

		if(Test_If_Word_Found(Input[i], "Print Rates"))
		{
			InitialParameters.PrintReacRates = true;
		}

		if(Test_If_Word_Found(Input[i], "Comma Separator"))
		{
			InitialParameters.Solver_Parameters.separator = ",";
		}

		if(Test_If_Word_Found(Input[i], "Tab Separator"))
		{
			InitialParameters.Solver_Parameters.separator = "\t";
		}

		if(Test_If_Word_Found(Input[i], "Separator:"))
		{
			p=strtok (cstr,"=: \t"); // break at space or tab
			p=strtok(NULL,"=: \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.Solver_Parameters.separator = p;
				p=strtok(NULL," \t");
			}
		}


		if(Test_If_Word_Found(Input[i], "Stoichiometry Matrix For Opt"))
		{
			InitialParameters.StoichiometryMatrixForOpt = true;
		}

		delete[] cstr;
	}

}

