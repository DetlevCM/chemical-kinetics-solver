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


// My own headers

#include "Structs.h"
#include "Classes.h"
#include "Namespaces.h"
#include <Global_Variables.h>

#include "Helpers.h"

// Definitions of my functions
#include "lib_headers/lib_Intel_ODE.h"
#include "lib_headers/lib_odepack.h"

#include "Get_Input.h"

#include "initial_data_handling.h" // new input handling

#include "Pre-Process.h"
#include "Mechanism_Reduction.h"

#include "Jacobian.h"

#include "Run_Integrator.h"
#include "Solver_Calculations.h"
#include "Rates-Analysis.h"
#include "Write_Output.h"

#include "MyFunctions.h"


#include "PetroOxy.h"


#endif
