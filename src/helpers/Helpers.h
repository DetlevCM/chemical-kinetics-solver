/*
 * Helpers.h
 *
 *  Created on: 14 May 2016
 *      Author: DetlevCM
 */

#ifndef _HELPERS
#define _HELPERS

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

string Strip_Single_Line_Comments(
		string input ,
		vector<string> tokens
		);

vector< string > Tokenise_String_To_String(
		string input,
		string tokens
		);

void Tokenise_String_To_String_Append(
		vector< string >& data ,
		string input, string tokens
		);

vector< double > Tokenise_String_To_Double(
		string input,
		string tokens
		);

bool Line_Not_Comment_Or_Empty(
		string InputLine
		);

bool Test_If_Word_Found(
		string string_to_search,
		string search_term
		);


#endif
