/*
 * Helpers.h
 *
 *  Created on: 14 May 2016
 *      Author: DetlevCM
 */

#ifndef _HELPERS
#define _HELPERS

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
#include <string.h>
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

string Strip_Single_Line_Comments(string input, vector<string> tokens);

vector<string> Tokenise_String_To_String(string input, string tokens);

void Tokenise_String_To_String_Append(vector<string> &data, string input,
                                      string tokens);

vector<double> Tokenise_String_To_Double(string input, string tokens);

bool Line_Not_Comment_Or_Empty(string InputLine);

bool Test_If_Word_Found(string string_to_search, string search_term);

string trim_string(string str, string whitespace = " \t");

#endif
