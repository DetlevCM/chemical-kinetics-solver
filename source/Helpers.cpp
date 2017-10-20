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

bool LineNotCommentOrEmpty( string InputLine)
{
	if(
			InputLine.compare(0,2,"//") != 0// C/C++ style comment
			//	&& line.compare(0,2,"/*") != 0 // opening comment in C/C++, not checking for the * yet
			&& InputLine.compare(0,1,"!") != 0 // Chemkin Style comment
			&& InputLine.compare(0,1,"#") != 0 // shell or Python style comments
			&& !InputLine.empty() // check that line is not empty
	)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// from reading on the web, this approach is slow - but for the input file will do
bool Test_If_Word_Found(string search_term, string string_to_search)
{
	// tolower or toupper - either works, just be consistent
	transform(search_term.begin(),search_term.end(),search_term.begin(), tolower);
	transform(string_to_search.begin(),string_to_search.end(),string_to_search.begin(), tolower);

	bool output;
	if(string_to_search.find(search_term)!=string::npos)
	{
		output = true;
	}
	else
	{
		output = false;
	}
	return output;
}

