/**-- Header Files --**/

#ifndef _MY_HEADERS
#define _MY_HEADERS

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
#include <string.h>

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


/* ************************** */
/* user defined headers below */
/* ************************** */


// structs definitions
#include "Structs.hpp"

#include "Class_Initial_Data.hpp"
#include "Class_Reaction_Mechanism.hpp"
#include "Classes.hpp"

// namespaces with global variables for performance reasons
#include "Namespaces.hpp"

// helper functions for cleaner code
#include "Helpers.hpp"

// headers for the ode libraries
#include "lib_headers/lib_Intel_ODE.hpp"
#include "lib_headers/lib_odepack.hpp"


// run_integrator subfolder
#include "run_integrator/Jacobian.hpp"
#include "run_integrator/Run_Integrator.hpp"
#include "run_integrator/Solver_Calculations.hpp"
#include "run_integrator/Third_Body_Maths.hpp"

// and somre more headers...
#include "Get_Process_Input.hpp"
#include "Handle_Initial_Conditions.hpp" // new input handling
#include "Pre-Process.hpp"
#include "Mechanism_Reduction.hpp"
#include "Rates-Analysis.hpp"
#include "Write_Output.hpp"
#include "MyFunctions.hpp"
#include "PetroOxy.hpp"


#endif
