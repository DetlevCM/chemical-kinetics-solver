/*
 * Remove_Species.cpp
 *
 *  Created on: 12 Mar 2014
 *      Author: DetlevCM
 */


#include <MyHeaders.h>



vector< bool > Read_Kill_List(
		string filename,
		vector< string > Species
)
{
	int i;

	ifstream InputFile;
	InputFile.open (filename.c_str());

	string line1;

	size_t found;

	// vector to determine whether a species is retained or not
	int Number_Species = (int) Species.size();

	vector< bool > RetainOrNot(Number_Species);
	for(i=0;i<Number_Species;i++){
		RetainOrNot[i] = true;
	}

	// first I need to read in my species to remove
	if (InputFile.is_open())
	{
		cout << "Killing selected species\n";

		while (InputFile.good())
		{
			getline (InputFile,line1);

			if(!line1.empty()){// && line1.compare(0,1,"!") != 0){ // stop if end reached or comment

				// filter comment
				char * cstr, *p;

				string str = line1;
				cstr = new char [str.size()+1];
				strcpy (cstr, str.c_str());

				vector< string > RemoveComments;

				found = line1.find("!");
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


				for(i=0;i<Number_Species;i++){
					// compare the name of the species with the species in the kill list
					if(strcmp(Species[i].c_str(),RemoveComments[0].c_str()) == 0){
						RetainOrNot[i] = false;
					}
				}

				RemoveComments.clear(); //tidy up
			}
		}
	}
	InputFile.close();

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

	int i, j;
	int Number_Species = (int) Species.size();
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

	for(j=0;j<(int)Reactions.size();j++){

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

			int k;
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

			SingleReactionData.paramA = Reactions[j].paramA;
			SingleReactionData.paramN = Reactions[j].paramN;
			SingleReactionData.paramEa = Reactions[j].paramEa;
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



