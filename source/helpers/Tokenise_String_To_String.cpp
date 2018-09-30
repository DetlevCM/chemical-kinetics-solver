/*
 * Tokenise_String_To_String.cpp
 *
 *  Created on: Sep 30, 2018
 *      Author: detlevcm
 */

#include <Headers.hpp>

vector< string > Tokenise_String_To_String(string input, string tokens)
{
	char *cstr, *p;
	vector< string > output;

	string str = input;
	cstr = new char [str.size()+1];
	strcpy (cstr, str.c_str());

	p=strtok (cstr,tokens.c_str());
	while(p!=NULL)
	{
		output.push_back(p);
		p=strtok(NULL,tokens.c_str());
	}
	delete[] cstr;
	delete[] p;

	return output;
}


