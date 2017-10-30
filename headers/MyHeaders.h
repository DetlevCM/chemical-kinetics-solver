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

// varibale types
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




/* ************************** */
/* user defined headers below */
/* ************************** */


// structs definitions
#include "structs/Structs.h"
#include "structs/mechanism_storage.h"
#include "structs/initial_conditions_parameters.h"

// namespaces with global variables for performance reasons
#include "Namespaces.h"

// helper functions for cleaner code
#include "Helpers.h"

// headers for the ode libraries
#include "lib_headers/lib_Intel_ODE.h"
#include "lib_headers/lib_odepack.h"

#
// run_integrator subfolder
#include "run_integrator/Jacobian.h"
#include "run_integrator/Run_Integrator.h"
#include "run_integrator/Solver_Calculations.h"


// and somre more headers...
#include "Get_Input.h"
#include <Handle_Initial_Conditions.h> // new input handling
#include "Pre-Process.h"
#include "Mechanism_Reduction.h"
#include "Rates-Analysis.h"
#include "Write_Output.h"
#include "MyFunctions.h"
#include "PetroOxy.h"


#endif
