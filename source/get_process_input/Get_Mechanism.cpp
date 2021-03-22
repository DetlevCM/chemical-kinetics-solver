/*
 * Get_Mechanism.cpp
 *
 *  Created on: Sep 30, 2018
 *      Author: detlevcm
 */

#include "../headers/Headers.hpp"


void Get_Mechanism(
		string filename ,
		Reaction_Mechanism& reaction_mechanism
)
{
	ifstream DataInputFromFile;
	// check the existence of the mechanism file
	DataInputFromFile.open(filename.c_str());
	if (!DataInputFromFile.is_open()) {
		cout << "Error opening " << filename << ".\n";
		exit(1); // standard error code
	}
	DataInputFromFile.close();

	/* Read in the mechanism, that is species, thermodynamic data and reactions.
	 * Report on the progress as the sections are read in.
	 */
	reaction_mechanism.Species = Get_Species(filename);
	int Number_Species = (int)reaction_mechanism.Species.size();
	cout << "The Mechanism contains " << Number_Species <<" Species.\n";
	reaction_mechanism.Thermodynamics = Get_Thermodynamic_Data(filename, reaction_mechanism.Species);
	cout << "The Mechanism contains " << reaction_mechanism.Thermodynamics.size() << " Thermodynamic Data Entries.\n";
	reaction_mechanism.Reactions = Get_Reactions(filename, reaction_mechanism.Species);
	int Number_Reactions = (int)reaction_mechanism.Reactions.size();
	cout << "The Mechanism contains " << Number_Reactions << " Reactions.\n";
}

