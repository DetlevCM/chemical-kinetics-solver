/*
 * mechanism_reduction_lumping.cpp
 *
 *  Created on: 28 Nov 2014
 *      Author: DetlevCM
 */

#include "../headers/Headers.hpp"


void mechanism_picking(
		string filename,
		vector< string >& Species,
		vector< ThermodynamicData >& Thermodynamics,
		vector< SingleReactionData >& Reactions
)
{

	// Input File via a stream:
	ifstream ReductionData;

	size_t Number_Species = Species.size();

	// Two files I want to open, mechanism data & input data
	ReductionData.open(filename.c_str());
	vector< SpeciesPicking > ChosenSpecies(Number_Species);

	size_t i;

	for(i=0;i<Number_Species;i++){
		ChosenSpecies[i].Choice = false;
		ChosenSpecies[i].SpcClass = 0;
	}
	string line1, line;

	// check the existence of the 1st input file - the mechanism
	if (ReductionData.is_open()){


		getline (ReductionData,line1); // first entry will specify the type of species picking

		while (ReductionData.good())
		{
			getline (ReductionData,line); // Output Control

			// read species name, compare to known species, ID which is it
			size_t found;

			if(!line.empty()){// && line1.compare(0,1,"!") != 0){ // stop if end reached or comment

				// filter comment
				char * cstr, *p;

				string str = line;
				cstr = new char [str.size()+1];
				strcpy (cstr, str.c_str());

				vector< string > RemoveComments;

				found = line.find("!");
				if (found!=string::npos)
				{
					p=strtok (cstr,"!");
					RemoveComments.push_back(p);
					str = RemoveComments[0];
					delete[] cstr;
				}
				else
				{
					RemoveComments.push_back(cstr);
					str = RemoveComments[0];
					delete[] cstr;
				}

				// remove whitespaces
				RemoveComments.clear();
				cstr = new char [str.size()+1];
				strcpy (cstr, str.c_str());

				p=strtok (cstr," 	"); // split by space and tab
				while (p!=NULL)
				{
					RemoveComments.push_back(p);
					p=strtok(NULL," 	");
				}
				delete[] cstr;
				// check which species I need to retain
				for(i=0;i<Number_Species;i++){
					// compare the name of the species with the species in the kill list
					if(strcmp(Species[i].c_str(),RemoveComments[0].c_str()) == 0){
						ChosenSpecies[i].Choice = true;
						if(RemoveComments.size()>1)
						{
							//ChosenSpecies[i].SpcClass = strtod(RemoveComments[1].c_str(),NULL);
							ChosenSpecies[i].SpcClass = (size_t)atoi(RemoveComments[1].c_str());
						}
						else
						{
							ChosenSpecies[i].SpcClass = 0;
						}
					}
				}
				RemoveComments.clear(); //tidy up
			}
		}
	}


	handle_reactions_with_chosen_species(
			line1,
			ChosenSpecies,
			Species,
			Thermodynamics,
			Reactions
	);
}





void handle_reactions_with_chosen_species(
		string type,
		vector< SpeciesPicking >& ChosenSpecies,
		vector< string >& Species,
		vector< ThermodynamicData >& Thermodynamics,
		vector< SingleReactionData >& Reactions
)
{

	// So, I_ need to find all and any reaction that contains a chosen species
	bool RunLoop;

	size_t i,j ;
	size_t Number_Reactions = Reactions.size();
	size_t Number_Species = Species.size();

	vector< string > PickedSpecies;
	//vector< vector< double > > PickedThermodynamics;
	vector< ThermodynamicData > PickedThermodynamics;
	vector< SingleReactionData > PickedReactions;


	for(i=0;i<Number_Reactions;i++)
	{
		// for every reaction, check if every species is desired, if yes, retain

		bool retain = false;
		RunLoop = true;
		j = 0;

		// first loop to pick reactions potentially worth keeping
		do{
			if(
					((Reactions[i].Reactants[j] != 0) && ChosenSpecies[j].Choice) // we want a Reactants
					||
					((Reactions[i].Products[j] != 0) && ChosenSpecies[j].Choice) // we want a Product
			)
			{
				RunLoop = false;
				retain = true;	//PickedReactions.push_back(Reactions[i]); // keep the picked reaction
			}
			j++; // step the counter forward
		}while(RunLoop && j < Number_Species);

		if(strcmp(type.c_str(),"ONLY")==0)
		{
			// second loop to filter out the reactions with an udesireable species
			j = 0;
			do{
				if(
						((Reactions[i].Reactants[j] != 0) && !ChosenSpecies[j].Choice) // we want a Reactants
						||
						((Reactions[i].Products[j] != 0) && !ChosenSpecies[j].Choice) // we want a Product
				)
				{
					retain = false;
				}
				j++; // step the counter forward
			}while(retain && (j < Number_Species));
		}

		if(retain)
		{
			// OK need to write some recombination logic which can filter them out
			PickedReactions.push_back(Reactions[i]); // keep the picked reaction
		}
	}


	// now check the species
	vector< bool > RetainOrNot(Number_Species);
	for(i=0;i<Number_Species;i++)
	{
		RetainOrNot[i] = false;
	}

	Number_Reactions = PickedReactions.size();

	for(i=0;i<Number_Reactions;i++)
	{
		for(j=0;j<Number_Species;j++)
		{
			if(PickedReactions[i].Reactants[j] != 0)
			{
				RetainOrNot[j] = true;
			}
			if(PickedReactions[i].Products[j] != 0)
			{
				RetainOrNot[j] = true;
			}
		}
	}




	//*
	/// Now I could resort the reactions, identify recombination reactions
	/// I have my species classes

	vector< SingleReactionData > RegularReaction;
	vector< SingleReactionData > RecombinationReaction;

	Number_Reactions = PickedReactions.size();
	Number_Species = Species.size();

	size_t nbr_spc_reac = 0 ;
	size_t nbr_spc_prod = 0 ;
	size_t k;

	size_t same_spc_reac_prod = 0;
	size_t same_spc_class_reac_prod = 0;

	for(i=0;i<Number_Reactions;i++)
	{
		for(j=0;j<Number_Species;j++)
		{
			if(Reactions[i].Reactants[j]!=0) // there is a species in the reactants
			{
				nbr_spc_reac = nbr_spc_reac + 1;

				if(Reactions[i].Reactants[j] == -Reactions[i].Products[j]) // same species present
				{
					same_spc_reac_prod = same_spc_reac_prod + 1;
					//cout << "i " << i << "j " << j << "\n";
				}
				else
				{
					if(ChosenSpecies[j].SpcClass != 0)
					{
						for(k=0;k<Number_Species;k++)
						{
							if(ChosenSpecies[j].SpcClass == ChosenSpecies[k].SpcClass && Reactions[i].Products[k] != 0)
							{
								same_spc_class_reac_prod = same_spc_class_reac_prod + 1;
							}
						}
					}
				}
			}

			if(Reactions[i].Products[j]!=0) // there is a species in the products
			{
				nbr_spc_prod = nbr_spc_prod  + 1;
			}
		}

		// OK, we know how many reactants and products, how many identical classes, how many identical species

		if(
				(same_spc_class_reac_prod != 0 && same_spc_reac_prod != 0)
				&&
				(nbr_spc_reac == nbr_spc_prod)
				&&
				(nbr_spc_reac == (same_spc_class_reac_prod + same_spc_reac_prod))
		)
		{
			//cout << " " << nbr_spc_reac << " " << same_spc_class_reac_prod << " " << same_spc_reac_prod << "\n";
			RecombinationReaction.push_back(PickedReactions[i]);
		}
		else
		{
			RegularReaction.push_back(PickedReactions[i]);
		}

		nbr_spc_reac = 0;
		nbr_spc_prod = 0;
		same_spc_class_reac_prod = 0;
		same_spc_reac_prod = 0;

	}

	// Now rebuild the scheme so that it has one continuous reactions list:
	PickedReactions.clear();


	if(RecombinationReaction.size() > 0)
	{
		WriteReactions("Species_Picked_Reactions_regular.txt", Species, RegularReaction);
	}

	if(RecombinationReaction.size() > 0)
	{
		WriteReactions("Species_Picked_Reactions_rearrange.txt", Species, RecombinationReaction);
	}

	for(i=0;i<RegularReaction.size();i++)
	{
		PickedReactions.push_back(RegularReaction[i]);
	}

	for(i=0;i<RecombinationReaction.size();i++)
	{
		PickedReactions.push_back(RecombinationReaction[i]);
	}

	//cout << "Checking - Number of Reactions: " << PickedReactions.size() << " \n";
	//*/


	// call the code from species killing to reduce properly
	// this step must be last for the species mapping to be of any use

	Reduce_Species_Thermo_Mechanism(
			RetainOrNot,
			Species,
			Thermodynamics,
			PickedReactions
	);

	Reactions.clear();
	Reactions = PickedReactions;

}
