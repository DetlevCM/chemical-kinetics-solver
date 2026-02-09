/*
 * Handle_SolverParameters.cpp
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#include "Initial_Data.h"

void Initial_Data::Handle_Solver_Parameters(Initial_Data &InitialParameters,
                                            vector<string> Input) {
  size_t i;
  for (i = 0; i < Input.size(); i++) {

    /* Let the user chose the type of solver to use */
    /* maybe at some point I should make the setting terms case insensitive...
     */
    if (Test_If_Word_Found(Input[i], "intelode")) {
      InitialParameters.Solver_Parameters.SolverType = 0;
      cout << "The Intel ODE solver has been deprecated, the code will exit.\n";
      exit(1);
    }
    if (Test_If_Word_Found(Input[i], "odepack")) {
      InitialParameters.Solver_Parameters.SolverType = 1;
    }

    if (Test_If_Word_Found(Input[i], "Jacobian")) // use analytical Jacobian
    {
      InitialParameters.Solver_Parameters.Use_Analytical_Jacobian = true;
    }
    if (Test_If_Word_Found(Input[i],
                           "Force Stability")) // force no negative species
    {
      InitialParameters.EnforceStability = true;
    }
    if (Test_If_Word_Found(
            Input[i],
            "Use General Solver")) // use solver that can handle non stiff
                                   // problems better but is slower
    {
      InitialParameters.Solver_Parameters.Use_Stiff_Solver = false;
    }

    if (Test_If_Word_Found(Input[i], "hm")) {
      vector<string> entries = Tokenise_String_To_String(Input[i], " \t");
      if (entries.size() >= 2) // should be two positions, 2nd is value
      {
        InitialParameters.Solver_Parameters.minimum_stepsize = stod(entries[1]);
      }
      cout << "minimum stepsize (hm): "
           << InitialParameters.Solver_Parameters.minimum_stepsize << "\n";
    }

    if (Test_If_Word_Found(Input[i], "initialh")) {
      vector<string> entries = Tokenise_String_To_String(Input[i], " \t");
      if (entries.size() >= 2) // should be two positions, 2nd is value
      {
        InitialParameters.Solver_Parameters.initial_stepsize = stod(entries[1]);
      }
      cout << "initial stepsize (h): "
           << InitialParameters.Solver_Parameters.initial_stepsize << "\n";
    }

    if (Test_If_Word_Found(Input[i], "Tolerance")) {
      vector<string> entries = Tokenise_String_To_String(Input[i], " \t");
      if (entries.size() >= 3) // should be two positions, 2nd is value
      {
        InitialParameters.Solver_Parameters.rtol = stod(entries[1]);
        InitialParameters.Solver_Parameters.atol = stod(entries[2]);
      }
      cout << "relative tolerance: " << InitialParameters.Solver_Parameters.rtol
           << "\n";
      cout << "absolute tolerance: " << InitialParameters.Solver_Parameters.atol
           << "\n";
    }

    if (Test_If_Word_Found(Input[i], "RTOL")) {
      vector<string> entries = Tokenise_String_To_String(Input[i], " \t");
      if (entries.size() >= 2) // should be two positions, 2nd is value
      {
        InitialParameters.Solver_Parameters.rtol = stod(entries[1]);
      }
      cout << "relative tolerance: " << InitialParameters.Solver_Parameters.rtol
           << "\n";
    }

    if (Test_If_Word_Found(Input[i], "ATOL")) {
      vector<string> entries = Tokenise_String_To_String(Input[i], " \t");
      if (entries.size() >= 2) // should be two positions, 2nd is value
      {
        InitialParameters.Solver_Parameters.atol = stod(entries[1]);
      }
      cout << "absolute tolerance: " << InitialParameters.Solver_Parameters.atol
           << "\n";
    }

    if (Test_If_Word_Found(Input[i], "IRREV")) {
      InitialParameters.irrev = true;
    }

    if (Test_If_Word_Found(Input[i], "Print Rates")) {
      InitialParameters.PrintReacRates = true;
    }

    if (Test_If_Word_Found(Input[i], "Comma Separator")) {
      InitialParameters.Solver_Parameters.separator = ",";
    }

    if (Test_If_Word_Found(Input[i], "Tab Separator")) {
      InitialParameters.Solver_Parameters.separator = "\t";
    }

    if (Test_If_Word_Found(Input[i], "Separator:")) {
      vector<string> entries = Tokenise_String_To_String(Input[i], "=: \t");
      if (entries.size() >= 2) // should be two positions, 2nd is value
      {
        InitialParameters.Solver_Parameters.separator = entries[1];
      }
    }

    if (Test_If_Word_Found(Input[i], "threads")) {
      vector<string> entries = Tokenise_String_To_String(Input[i], " \t");
      if (entries.size() >= 2) // should be two positions, 2nd is value
      {
        InitialParameters.Solver_Parameters.threads = stod(entries[1]);
      }
      cout << "threads: " << InitialParameters.Solver_Parameters.threads
           << "\n";
    }

    if (Test_If_Word_Found(Input[i], "Stoichiometry Matrix For Opt")) {
      InitialParameters.StoichiometryMatrixForOpt = true;
    }
  }
}
