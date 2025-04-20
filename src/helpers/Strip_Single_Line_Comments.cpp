/*
 * tokenise_string.cpp
 *
 *  Created on: 14 May 2016
 *      Author: DetlevCM
 */

#include "../headers/Headers.hpp"

string Strip_Single_Line_Comments(string input , vector<string> tokens)
{

	string output;
	bool Token_Is_Found;
	Token_Is_Found = false;
	string AllTokens;

	size_t i;

	for(i=0;i<tokens.size();i++)
	{
		if(input.find(tokens[i]) != string::npos)
		{
			Token_Is_Found = true;
		}
		AllTokens = AllTokens+tokens[i];
	}

	if(Token_Is_Found){

		char *cstr, *p;
		string str = input;
		cstr = new char [str.size()+1];
		strcpy (cstr, str.c_str());

		p=strtok (cstr,AllTokens.c_str());
		output = p;

		delete[] cstr;
		//delete[] p;
	}
	else
	{
		output = input;
	}

	return output;
}


