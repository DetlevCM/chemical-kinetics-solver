/*
 * Process_User_Input.cpp
 *
 *  Created on: Sep 30, 2018
 *      Author: detlevcm
 */

#include "../headers/Headers.hpp"

/*
 * This function deals with handling the command line
 * arguments passed by the user, or alternatively the lack of
 * command line arguments.
 */

void Process_User_Input(
		FileNames& filenames,
		vector<string> User_Inputs
)
{

	// default without keywords
	filenames.mechanism = "chem.inp"; // standard
	filenames.initial_data = "initial.inp"; // old input format officially depreciated
	filenames.species_mapping = "";


	// legacy options without keywords:
	// Case 1: We have the mechanism provided (makes most sense)
	if((int) User_Inputs.size() == 1 && !Test_If_Word_Found(User_Inputs[1],"mechanism="))
	{
		filenames.mechanism = User_Inputs[0];
	}
	// Case 2: The user supplied the initial conditions (more likely to change)
	if((int) User_Inputs.size() == 2 && !Test_If_Word_Found(User_Inputs[1],"initial="))
	{
		filenames.initial_data = User_Inputs[2];
	}



	// assuming the user uses the new format with keywords:
	for(int i = 0;i<(int)User_Inputs.size();i++)
	{
		if(Test_If_Word_Found(User_Inputs[i],"mechanism="))
		{
			vector<string> tmp = Tokenise_String_To_String(User_Inputs[i],"=");
			filenames.mechanism = tmp[1];
			tmp.clear();
		}
		else if(Test_If_Word_Found(User_Inputs[i],"initial="))
		{
			vector<string> tmp = Tokenise_String_To_String(User_Inputs[i],"=");
			filenames.initial_data = tmp[1];
			tmp.clear();
		}
		else if(Test_If_Word_Found(User_Inputs[i],"species_mapping="))
		{
			vector<string> tmp = Tokenise_String_To_String(User_Inputs[i],"=");
			filenames.species_mapping = tmp[1];
			tmp.clear();
		}

	}

}

