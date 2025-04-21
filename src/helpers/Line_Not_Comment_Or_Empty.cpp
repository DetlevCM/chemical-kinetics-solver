/*
 * Line_Not_Comment_Or_Empty.cpp
 *
 *  Created on: Sep 30, 2018
 *      Author: detlevcm
 */

#include "../include/Headers.hpp"

bool Line_Not_Comment_Or_Empty( string InputLine )
{
	vector< string > line_content;
	line_content = Tokenise_String_To_String(InputLine,"\t "); // split entry

	if(
			InputLine.compare(0,2,"//") != 0// C/C++ style comment
			//	&& line.compare(0,2,"/*") != 0 // opening comment in C/C++, not checking for the * yet
			&& InputLine.compare(0,1,"!") != 0 // Chemkin Style comment
			&& InputLine.compare(0,1,"#") != 0 // shell or Python style comments
			&& !InputLine.empty() // check that line is not empty
			&& (int) line_content.size() > 0 // ensure line does not contain only space or tab
	)
	{
		line_content.clear(); // tidy up
		return true;
	}
	else
	{
		line_content.clear(); // tidy up
		return false;
	}
}


