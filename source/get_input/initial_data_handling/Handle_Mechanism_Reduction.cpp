/*
 * Handle_MechanismReduction.cpp
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#include <MyHeaders.h>


mechanism_reduction Handle_Mechanism_Reduction(vector<string> Input)
{
	mechanism_reduction Mechanism_Reduction_Output;

	// default
	Mechanism_Reduction_Output.ReduceReactions = 0;
	Mechanism_Reduction_Output.UseNewLumping = false;
	Mechanism_Reduction_Output.UseFastLumping = true;

	int i;
	for(i=0;i<(int)Input.size();i++)
	{
		if(Test_If_Word_Found(Input[i], "Use New Lumping"))
		{
			Mechanism_Reduction_Output.UseNewLumping = true;
		}
		if(Test_If_Word_Found(Input[i], "Use Slow New Lumping"))
		{
			Mechanism_Reduction_Output.UseNewLumping = true;
			Mechanism_Reduction_Output.UseFastLumping = false;
		}
		if(Test_If_Word_Found(Input[i], "Use Slow Lumping"))
		{
			Mechanism_Reduction_Output.UseFastLumping = false;
		}
		if(Test_If_Word_Found(Input[i], "ReduceReactions"))
		{
			vector< string > line_content;
			line_content = Tokenise_String_To_String(Input[i]," \t");
			if((int)line_content.size()>=2)
			{
				Mechanism_Reduction_Output.ReduceReactions = strtod(line_content[1].c_str(),NULL);
			}
			line_content.clear();
		}
	}

	return Mechanism_Reduction_Output;
}

