/*
 * Tokenise_String.cpp
 *
 *  Created on: Sep 30, 2018
 *      Author: detlevcm
 */

#include "Helpers.h"

vector<double> Tokenise_String_To_Double(string input, string tokens) {
  char *cstr, *p;
  vector<double> output;
  string str = input;
  cstr = new char[str.size() + 1];
  strcpy(cstr, str.c_str());
  p = strtok(cstr, tokens.c_str());
  while (p != NULL) {
    output.push_back(strtod(p, NULL));
    p = strtok(NULL, tokens.c_str());
  }
  delete[] cstr;
  return output;
}

vector<string> Tokenise_String_To_String(string input, string tokens) {
  char *cstr, *p;
  vector<string> output;
  string str = input;
  cstr = new char[str.size() + 1];
  strcpy(cstr, str.c_str());
  p = strtok(cstr, tokens.c_str());
  while (p != NULL) {
    output.push_back(p);
    p = strtok(NULL, tokens.c_str());
  }
  delete[] cstr;
  return output;
}

void Tokenise_String_To_String_Append(vector<string> &data, string input,
                                      string tokens) {
  char *cstr, *p;
  string str = input;
  cstr = new char[str.size() + 1];
  strcpy(cstr, str.c_str());
  p = strtok(cstr, tokens.c_str());
  while (p != NULL) {
    data.push_back(p);
    p = strtok(NULL, tokens.c_str());
  }
  delete[] cstr;
}
