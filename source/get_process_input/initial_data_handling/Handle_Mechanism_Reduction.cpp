/*
 * Handle_MechanismReduction.cpp
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#include "../headers/Headers.hpp"


mechanism_reduction Handle_Mechanism_Reduction(vector<string> Input)
{
	mechanism_reduction Mechanism_Reduction_Output;

	// default
	Mechanism_Reduction_Output.ReduceReactions = 0;
	Mechanism_Reduction_Output.LumpingType = 1; // original method, n = 0, A & Ea fitted
	Mechanism_Reduction_Output.UseFastLumping = true;

	int i;
	for(i=0;i<(int)Input.size();i++)
	{
		if(Test_If_Word_Found(Input[i], "Fast"))
		{
			Mechanism_Reduction_Output.UseFastLumping = true;
		}

		if(Test_If_Word_Found(Input[i], "Slow"))
		{
			Mechanism_Reduction_Output.UseFastLumping = false;
		}

		if(Test_If_Word_Found(Input[i], "Lumping Method 1"))
		{
			Mechanism_Reduction_Output.LumpingType = 1; // original method, n = 0, A & Ea fitted
		}

		if(Test_If_Word_Found(Input[i], "Lumping Method 2"))
		{
			Mechanism_Reduction_Output.LumpingType = 2; // second method, Ea averaged, n and A fitted
		}


		if(Test_If_Word_Found(Input[i], "Lumping Method 3")) // not done yet
		{
			Mechanism_Reduction_Output.LumpingType = 2;
		}



		if(Test_If_Word_Found(Input[i], "ReduceReactions"))
		{
			vector< string > line_content;
			line_content = Tokenise_String_To_String(Input[i]," \t");
			if((int)line_content.size()>=2)
			{
				Mechanism_Reduction_Output.ReduceReactions = stod(line_content[1],NULL);
			}
			line_content.clear();
		}
	}

	return Mechanism_Reduction_Output;
}

