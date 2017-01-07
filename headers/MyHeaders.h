/**-- Header Files --**/

#ifndef _MY_HEADERS
#define _MY_HEADERS

// File Streams and IO
#include <cstdio>

#include <sstream>
#include <iostream>
#include <fstream>

#include <cstdlib>

#include <cmath>

#include <ctime>
#include <vector>
#include <cstring>


// Needed globally for the function definitions
//using namespace std;

using std::vector;
using std::cout;
using std::string;

using std::ofstream;
//using std::iostream;
//using std::fstream;
using std::ifstream;
using std::ios;

using std::ostringstream;
using std::stringstream;
using std::istringstream;

// Looking into ODEint - as an open ODE solver
//#include <boost/numeric/odeint.hpp>
//using namespace boost::numeric::odeint;

//#include <iterator>

#include "Structs.h"
#include "Classes.h"
#include "Namespaces.h"
#include "Global_Variables.hpp"

#include "Helpers.h"

// Definitions of my functions
#include "Get_Input.h"

#include "initial_data_handling.h" // new input handling

#include "Pre-Process.h"
#include "Mechanism_Reduction.h"
#include "Solver_Calculations.h"
#include "Rates-Analysis.h"
#include "Write_Output.h"

#include "MyFunctions.h"

#include "PetroOxy.h"


#endif
