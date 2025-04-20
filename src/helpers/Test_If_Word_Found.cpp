/*
 * tokenise_string.cpp
 *
 *  Created on: 14 May 2016
 *      Author: DetlevCM
 */

#include "../headers/Headers.hpp"

// from reading on the web, this approach is slow - but for the input file will do
bool Test_If_Word_Found(string string_to_search, string search_term)
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

