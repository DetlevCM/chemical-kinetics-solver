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

		/* Let the user chose the type of solver to use */
		/* maybe at some point I should make the setting terms case insensitive... */
		if(Test_If_Word_Found("intelode", Input[i]))
		{
			InitialParameters.Solver_Parameters.SolverType = 0;
		}
		if(Test_If_Word_Found("odepack", Input[i]))
		{
			InitialParameters.Solver_Parameters.SolverType = 1;
		}

		if (Test_If_Word_Found("Jacobian",Input[i])) // use analytical Jacobian
		{
			InitialParameters.Solver_Parameters.Use_Analytical_Jacobian = true;
		}
		if (Test_If_Word_Found("Force Stability",Input[i])) // force no negative species
		{
			InitialParameters.EnforceStability = true;
		}
		if (Test_If_Word_Found("Use General Solver",Input[i])) // use solver that can handle non stiff problems better but is slower
		{
			InitialParameters.Solver_Parameters.Use_Stiff_Solver = false;
		}

		if(Test_If_Word_Found("hm", Input[i]))
		{
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.Solver_Parameters.minimum_stepsize = strtod(p,NULL);
				p=strtok(NULL," \t");
			}
			cout << "minimum stepsize (h): " << InitialParameters.Solver_Parameters.minimum_stepsize << "\n";
		}

		if(Test_If_Word_Found("initialh", Input[i]))
		{
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.Solver_Parameters.initial_stepsize = strtod(p,NULL);
				p=strtok(NULL," \t");
			}
			cout << "initial stepsize (h): " << InitialParameters.Solver_Parameters.initial_stepsize << "\n";
		}


		if(Test_If_Word_Found("Tolerance", Input[i]))
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

		/*
		if(Test_If_Word_Found("Threshold", Input[i]))
		{
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.Param_Solver.threshold = strtod(p,NULL);
				p=strtok(NULL," \t");
			}

			cout << "threshold: " << InitialParameters.Param_Solver.threshold << "\n";
		}//*/

		if(Test_If_Word_Found("RTOL", Input[i]))
		{
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.Solver_Parameters.rtol = strtod(p,NULL);
				p=strtok(NULL," \t");
			}

			cout << "relative tolerance: " << InitialParameters.Solver_Parameters.rtol << "\n";
		}

		if(Test_If_Word_Found("ATOL", Input[i]))
		{
			p=strtok (cstr," \t"); // break at space or tab
			p=strtok(NULL," \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.Solver_Parameters.atol = strtod(p,NULL);
				p=strtok(NULL," \t");
			}

			cout << "absolute tolerance: " << InitialParameters.Solver_Parameters.atol << "\n";
		}

		if(Test_If_Word_Found("IRREV", Input[i]))
		{
			InitialParameters.irrev = true;
		}

		if(Test_If_Word_Found("Print Rates", Input[i]))
		{
			InitialParameters.PrintReacRates = true;
		}

		if(Test_If_Word_Found("Comma Separator", Input[i]))
		{
			InitialParameters.Solver_Parameters.separator = ",";
		}

		if(Test_If_Word_Found("Tab Separator", Input[i]))
		{
			InitialParameters.Solver_Parameters.separator = "\t";
		}

		if(Test_If_Word_Found("Separator:", Input[i]))
		{
			p=strtok (cstr,"=: \t"); // break at space or tab
			p=strtok(NULL,"=: \t"); // break again as first is the keyword

			if(p!=NULL){ // only read remainder is something is left
				InitialParameters.Solver_Parameters.separator = p;
				p=strtok(NULL," \t");
			}
		}


		if(Test_If_Word_Found("Stoichiometry Matrix For Opt", Input[i]))
		{
			InitialParameters.StoichiometryMatrixForOpt = true;
		}

		delete[] cstr;
	}

}

