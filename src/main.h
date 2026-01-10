
#ifndef _MAIN
#define _MAIN




#include "./write_output/write_output.h"

#include "./rates_analysis/Rates-Analysis.h"

#include "./get_process_input/mechanism_reduction/Mechanism_Reduction.h"

#include "./run_integrator/run_integrator.h"

#include "./get_process_input/get_initial_data/Initial_Data.h"
#include "./get_process_input/get_mechanism/Species.h"
#include "./get_process_input/get_mechanism/ReactionMechanism.h"

// File Streams and IO
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>

// type conversion
#include <stdlib.h>

// maths
#include <math.h>

// timekeeping
#include <time.h>

// variable types
#include <vector>
#include <string>

// for the case insensitive search
#include <cctype>
#include <algorithm>

// Needed globally for the function definitions
//using namespace std;

// specific components of namespace std
using std::vector;
using std::cout;
using std::string;

using std::ofstream;
using std::ifstream;
using std::ios;

using std::ostringstream;
using std::stringstream;
using std::istringstream;

using std::stod; // for C++ string to double

#include "./run_integrator/solver_calculations/solver_calculations.h"


// use a global glass so we can use a function to redirect
extern SolverCalculation solver_calculation; 

static void wrapper_ODE_RHS_Liquid_Phase(int*n, double*t, double*y, double*f)
{
	solver_calculation.ODE_RHS_Liquid_Phase(n, t, y, f);
}
static void wrapper_Jacobian_Matrix_Odepack_LSODA(int*n,double*t,double*y,double*ML,double*MU,double*a,int*NROWPD)
{
	
    solver_calculation.Jacobian_Matrix_Odepack_LSODA(n,t,y,ML,MU,a,NROWPD);
}


class Main {

public:




};

//void Handle_Mechanism_Input(
bool Handle_Mechanism_Input(
		Filenames filenames,
		ReactionMechanism & reactions_mechanism ,
		Initial_Data& ,
		//vector< double >&,
		PressureVesselCalc&
);

void Process_User_Input(
		Filenames& filenames,
		vector<string> User_Inputs
);



// Making Scheme Irreversible
vector< SingleReactionData > Make_Irreversible(
		vector< SingleReactionData > Reactions,
		const vector< Species > species,
		double Initial_Temperature, /// use initial temperature from initial data
		double Range // specify +/- range around initial temperature
);

void Synchronize_Gas_Liquid_Model(
		size_t number_synchronized_species,
		size_t liquid_species_count, size_t gas_species_count, // gas and liquid counts so I know where concentration entries belong to
		double *y, // concentrations (&temperature) from the ODE solver
		double Vliq_div_Vgas,
		vector< double > Henry_Constants // need to line up with species IDs
);





#endif