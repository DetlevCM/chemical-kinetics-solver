/*
 * Remove_Substring.cpp
 *
 *  Created on: 20 Jun 2021
 *      Author: detlevcm
 */

#include "../headers/Headers.hpp"

string Remove_Substring(string line, string to_remove)
{
	//  erase string until no longer found
	size_t pos = std::string::npos;
	while ((pos  = line.find(to_remove) )!= std::string::npos)
	{
		line.erase(pos, to_remove.length());
	}
	return line;
}

