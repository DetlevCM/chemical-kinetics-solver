/*
 * tokenise_string.cpp
 *
 *  Created on: 14 May 2016
 *      Author: DetlevCM
 */

#include "MyHeaders.h"

string Strip_Single_Line_Comments(string input , vector<string> tokens)
{

	string output;
	bool Token_Is_Found;
	Token_Is_Found = false;
	string AllTokens;

	int i;

	for(i=0;i<(int)tokens.size();i++)
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
	/*
	while(p!=NULL)
	{
		output.push_back(p);
		p=strtok(NULL,tokens.c_str());
	}//*/
	delete[] cstr;
	delete[] p;
}
else
{
	output = input;
}

return output;
}


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

void Tokenise_String_To_String_Append(vector< string >& data , string input, string tokens)
{
	char *cstr, *p;

	string str = input;
	cstr = new char [str.size()+1];
	strcpy (cstr, str.c_str());

	p=strtok (cstr,tokens.c_str());
	while(p!=NULL)
	{
		data.push_back(p);
		p=strtok(NULL,tokens.c_str());
	}
	delete[] cstr;
	delete[] p;
}

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
	delete[] p;

	return output;
				}
