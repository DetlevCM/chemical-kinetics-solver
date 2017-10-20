/*
 * Helpers.h
 *
 *  Created on: 14 May 2016
 *      Author: DetlevCM
 */

#ifndef HEADERS_HELPERS_H_
#define HEADERS_HELPERS_H_

string Strip_Single_Line_Comments(string, vector< string >);

vector< string > Tokenise_String_To_String(string , string );

void Tokenise_String_To_String_Append(vector< string > & , string, string);

vector< double > Tokenise_String_To_Double(string , string );

bool LineNotCommentOrEmpty(string);

bool Test_If_Word_Found(string , string );


#endif /* HEADERS_HELPERS_H_ */
