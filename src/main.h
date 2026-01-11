
#ifndef _MAIN
#define _MAIN

#include "./write_output/write_output.h"

#include "./rates_analysis/Rates-Analysis.h"

#include "./get_process_input/mechanism_reduction/Mechanism_Reduction.h"

#include "./run_integrator/run_integrator.h"

#include "./get_process_input/get_initial_data/Initial_Data.h"
#include "./get_process_input/get_mechanism/ReactionMechanism.h"
#include "./get_process_input/get_mechanism/Species.h"

// File Streams and IO
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>

// type conversion
#include <stdlib.h>

// maths
#include <math.h>

// timekeeping
#include <time.h>

// variable types
#include <string>
#include <vector>

// for the case insensitive search
#include <algorithm>
#include <cctype>

// Needed globally for the function definitions
// using namespace std;

// specific components of namespace std
using std::cout;
using std::string;
using std::vector;

using std::ifstream;
using std::ios;
using std::ofstream;

using std::istringstream;
using std::ostringstream;
using std::stringstream;

using std::stod; // for C++ string to double

#include "./run_integrator/solver_calculations/solver_calculations.h"

// use a global glass so we can use a function to redirect
extern SolverCalculation solver_calculation;
// SolverCalculation solver_calculation;

void wrapper_ODE_RHS_Liquid_Phase(int *n, double *t, double *y, double *f);

void wrapper_Jacobian_Matrix_Odepack_LSODA(int *n, double *t, double *y,
                                           double *ML, double *MU, double *a,
                                           int *NROWPD);

/*
static void* wrapper_ODE_RHS_Liquid_Phase(int *n, double *t, double *y,
double *f) {
cout << "test 45\n";
cout << solver_calculation.Concentration.size() << "\n";
cout << solver_calculation.Rates.size() << "\n";
solver_calculation.ODE_RHS_Liquid_Phase(n, t, y, f);
}
static void* wrapper_Jacobian_Matrix_Odepack_LSODA(int *n, double *t, double *y,
double *ML, double *MU,
double *a, int *NROWPD) {

solver_calculation.Jacobian_Matrix_Odepack_LSODA(n, t, y, ML, MU, a, NROWPD);
}
//*/

class Main {

public:
};

// void Handle_Mechanism_Input(
bool Handle_Mechanism_Input(Filenames filenames,
                            ReactionMechanism &reactions_mechanism,
                            Initial_Data &,
                            // vector< double >&,
                            PressureVesselCalc &);

void Process_User_Input(Filenames &filenames, vector<string> User_Inputs);

// Making Scheme Irreversible

void Calculate_Rate_Constant(
    vector<double> &Kf, vector<double> &Kr, const double Temperature,
    const vector<Reaction::ReactionParameter> &ReactionParameters,
    const vector<Species::ThermodynamicData::CalculatedThermodynamics>
        &CalculatedThermo,
    const vector<TrackSpecies> &SpeciesAll, const vector<double> &Delta_N);

vector<SingleReactionData> Make_Irreversible(
    vector<SingleReactionData> Reactions, const vector<Species> species,
    double Initial_Temperature, /// use initial temperature from initial data
    double Range                // specify +/- range around initial temperature
);

void Synchronize_Gas_Liquid_Model(
    size_t number_synchronized_species, size_t liquid_species_count,
    size_t gas_species_count, // gas and liquid counts so I know where
                              // concentration entries belong to
    double *y, // concentrations (&temperature) from the ODE solver
    double Vliq_div_Vgas,
    vector<double> Henry_Constants // need to line up with species IDs
);

#endif