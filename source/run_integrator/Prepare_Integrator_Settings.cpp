/*
 * Prepare_Integrator_Settings.cpp
 *
 *  Created on: 10 Feb 2019
 *      Author: detlevcm
 */

#include <Headers.hpp>


void Prepare_Integrator_Settings(
		Initial_Data InitialParameters,
		int Number_Species,
		Settings_LSODA& LSODA,
		Settings_Intel& Intel
)
{
	int n;
	n = Number_Species + 1;
	if(InitialParameters.Solver_Parameters.SolverType == 0) // Intel ODE Settings
	{
		/* Because we do not know the size of kd and dpar in advance we need to be clever - or at least act as we are.
		 * By defining kd and dpar as vectors we can assign a size "on the fly" as required. We can further pass a
		 * pointer to the vector to the solver, thus this is what we do. Rather than arrays, kd and dpar are pointers
		 * to vectors of doubles in our case.
		 */

		Intel.vector_ipar.resize(128);  // vectors initialise to zero, as required by the solver

		if (13 * n > (7 + 2 * n) * n) {
			Intel.dpar_size = 13 * n;
		} else {
			Intel.dpar_size = (7 + 2 * n) * n;
		}
		Intel.vector_dpar.resize(Intel.dpar_size);
		Intel.vector_kd.resize(n + 1);

		// Some tolerances for the solver:
		Intel.ep = InitialParameters.Solver_Parameters.rtol ; // relative tolerance. The code cannot guarantee the requested accuracy for ep<1.d-9
		Intel.tr = InitialParameters.Solver_Parameters.atol / InitialParameters.Solver_Parameters.rtol;
		Intel.h = InitialParameters.Solver_Parameters.initial_stepsize;
		Intel.hm = InitialParameters.Solver_Parameters.minimum_stepsize; // minimal step size for the methods, 1.0e-12 recommended for normalised problems

		Intel.Solver_Type = InitialParameters.Solver_Type[0];

		// switches to select the Jacobian or stiff solver
		Intel.solver_subsettings = 0;


		if(!Intel.Solver_Type.Use_Stiff_Solver && !Intel.Solver_Type.Use_Analytical_Jacobian)
		{
			Intel.solver_subsettings = 1; // stiff solver without analytical Jacobian
		}
		else if(!Intel.Solver_Type.Use_Stiff_Solver && Intel.Solver_Type.Use_Analytical_Jacobian)
		{
			Intel.solver_subsettings = 2; // stiff solver with analytical Jacobian
		}
		else if(Intel.Solver_Type.Use_Stiff_Solver && !Intel.Solver_Type.Use_Analytical_Jacobian)
		{
			Intel.solver_subsettings = 3; // general case without analytical Jacobian
		}
		else if(Intel.Solver_Type.Use_Stiff_Solver && Intel.Solver_Type.Use_Analytical_Jacobian)
		{
			Intel.solver_subsettings = 4; // general case with analytical Jacobian
		}

	}
	else if(InitialParameters.Solver_Parameters.SolverType == 1) // LSODA Settings
	{
		// calculate size of working vectors for LSODA
		// LRS = 22 + 9*NEQ + NEQ**2           if JT = 1 or 2,
		if ( (20 + 16 * n) > (22 + 9 * n + n * n) ) {
			LSODA.LRW = (20 + 16 * n);
		} else {
			LSODA.LRW = (22 + 9 * n + n * n);
		}
		LSODA.LIW = 20 + n;

		// Some tolerances for the solver:
		LSODA.RTOL = InitialParameters.Solver_Parameters.rtol;
		LSODA.ATOL = InitialParameters.Solver_Parameters.atol;

		// some vectors for LSODA
		LSODA.vector_IWORK.resize(LSODA.LIW);
		LSODA.vector_RWORK.resize(LSODA.LRW);


	}
}
