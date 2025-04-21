/*
 * Tokenise_String_To_Append.cpp
 *
 *  Created on: Sep 30, 2018
 *      Author: detlevcm
 */

#include "../include/Headers.hpp"

void Tokenise_String_To_String_Append(vector< string >& data , string input, string tokens)
{
	char *cstr, *p;

	string str = input;
	cstr = new char [str.size()+1];
	strcpy (cstr, str.c_str());

	p=strtok (cstr,tokens.c_str());

	while(p!=NULL)
	{
		//cout << ":" << p << ":\n";
		data.push_back(p);
		p=strtok(NULL,tokens.c_str());
	}
	delete[] cstr;
	//delete[] p;
}


