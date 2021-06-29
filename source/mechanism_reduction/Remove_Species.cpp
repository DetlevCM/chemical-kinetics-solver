/*
 * Remove_Species.cpp
 *
 *  Created on: 12.03.2014
 *  Revised on: 20.10.2017
 *      Author: DetlevCM
 */


#include "../headers/Headers.hpp"



vector< bool > Read_Kill_List(
		string filename,
		vector< string > Species
)
{
	ifstream InputFile;
	InputFile.open (filename.c_str());

	size_t i, j;
	size_t Number_Species = Species.size();
	string line;

	// vector to determine whether a species is retained or not
	vector< bool > RetainOrNot(Number_Species);

	for(i=0;i<Number_Species;i++){
		RetainOrNot[i] = true;
	}

	// first I need to read in my species to remove
	if (InputFile.is_open())
	{
		cout << "Removing selected species from the mcehanism.\n";

		while (InputFile.good())
		{
			getline (InputFile,line);
			if(Line_Not_Comment_Or_Empty(line))
			{
				vector< string > Remove_Comments;
				vector< string > Species_Present;

				Remove_Comments = Tokenise_String_To_String(line,"!"); // cut away comments
				Species_Present = Tokenise_String_To_String(Remove_Comments[0],"\t "); // split entry

				// in case more than one species is names on a single line
				for(i=0;i<Species_Present.size();i++)
				{
					for(j=0;j<Number_Species;j++){
						// compare the name of the species with the species in the kill list
						if(strcmp(Species[j].c_str(),Species_Present[i].c_str()) == 0){
							RetainOrNot[j] = false;
						}
					}
				}
				Remove_Comments.clear(); //tidy up
				Species_Present.clear();
			}
		}
		InputFile.close();
	}

	return RetainOrNot;
}




void Reduce_Species_Thermo_Mechanism(
		vector< bool > RetainOrNot,
		vector<string>& Species,
		//vector< vector< double > >& Thermodynamics,
		vector< ThermodynamicData > & Thermodynamics,
		vector< SingleReactionData >& Reactions
)
{

	size_t i, j;
	size_t Number_Species = Species.size();
	vector<string> NewSpecies;
	//vector< vector< double > >
	vector< ThermodynamicData > NewThermodynamics;
	vector< SingleReactionData > NewReactions;


	for(i=0;i<Number_Species;i++){
		if(RetainOrNot[i]){ // if we retain the species, write it to the new vector
			NewSpecies.push_back(Species[i]);
			NewThermodynamics.push_back(Thermodynamics[i]);
		}
	}


	bool kill;

	for(j=0;j<Reactions.size();j++){

		kill = false;

		for(i=0;i<Number_Species;i++){
			if(Reactions[j].Reactants[i] != 0 && !RetainOrNot[i]){
				kill = true;
			}
			if(Reactions[j].Products[i] != 0 && !RetainOrNot[i]){
				kill = true;
			}
		}

		SingleReactionData SingleReactionData;

		if(!kill) // if not kill true, retain
		{
			size_t k;
			vector< double > Reactants;
			vector< double > Products;

			for(k=0;k<Number_Species;k++)
			{
				if(RetainOrNot[k])
				{
					Reactants.push_back(Reactions[j].Reactants[k]);
					Products.push_back(Reactions[j].Products[k]);
				}
			}

			SingleReactionData.Reactants = Reactants;
			SingleReactionData.Products = Products;

			SingleReactionData.param_forward.A = Reactions[j].param_forward.A;
			SingleReactionData.param_forward.n = Reactions[j].param_forward.n;
			SingleReactionData.param_forward.Ea = Reactions[j].param_forward.Ea;
			SingleReactionData.Reversible = Reactions[j].Reversible;
			SingleReactionData.IsDuplicate = Reactions[j].IsDuplicate;

			NewReactions.push_back(SingleReactionData);
		}
	}


	// clear out the old arrays
	Species.clear();
	Reactions.clear();
	Thermodynamics.clear();


	cout << "\nAfter species removal, the scheme contains the following counts:\n" <<
			"Species: " << NewSpecies.size() << "\n" <<
			"Thermodynamic Entries: " << NewThermodynamics.size() << "\n" <<
			"Reactions: " << NewReactions.size() << "\n\n";


	// assign new arrays
	Species = NewSpecies;
	Thermodynamics = NewThermodynamics;
	Reactions = NewReactions;
}



