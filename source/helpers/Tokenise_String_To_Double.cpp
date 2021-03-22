/*
 * Tokenise_String_To_Double.cpp
 *
 *  Created on: Sep 30, 2018
 *      Author: detlevcm
 */

#include "../headers/Headers.hpp"

vector< double > Tokenise_String_To_Double(string input, string tokens)
{
	char *cstr, *p;
	vector< double > output;

	string str = input;
	cstr = new char [str.size()+1];
	strcpy (cstr, str.c_str());

	p=strtok (cstr,tokens.c_str());
	while(p!=NULL)
	{
		output.push_back(strtod(p,NULL));
		p=strtok(NULL,tokens.c_str());
	}
	delete[] cstr;
	//delete[] p;

	return output;
}


