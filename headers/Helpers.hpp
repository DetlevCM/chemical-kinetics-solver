/*
 * Helpers.h
 *
 *  Created on: 14 May 2016
 *      Author: DetlevCM
 */

#ifndef HEADERS_HELPERS_HPP_
#define HEADERS_HELPERS_HPP_

string Strip_Single_Line_Comments(
		string input ,
		vector<string> tokens
		);

string Remove_Substring(
		string line,
		string to_remove
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


#endif /* HEADERS_HELPERS_HPP_ */
