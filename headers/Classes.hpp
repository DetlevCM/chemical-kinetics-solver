/*
 * Classes.h
 *
 *  Created on: 4 Dec 2017
 *      Author: detlevcm
 */

#ifndef HEADERS_CLASSES_HPP_
#define HEADERS_CLASSES_HPP_

class FileNames {
public:
	/*
	 * This class collects the file names for the
	 * input files required during a run.
	 *
	 * To collate data, output files are equally treated in the
	 * same class, together together potentially with output flags.
	 */

	string mechanism;
	string initial_data;
	string species_mapping;

};




class SingleReactionData {
public:
	//struct SingleReactionData {
	bool Reversible;
	bool IsDuplicate;
	double paramA;
	double paramN;
	double paramEa;
	int ThirdBodyType; // 1: +M  2: (+M)
	vector<double> ThBd_LOW;
	vector<double> ThBd_TROE;
	vector<ThirdBodyParameters> ThBd_param;
	vector<double> Reactants;
	vector<double> Products;
};


// a class to store the mechanism
class Reaction_Mechanism {
public:
	vector< string > Species;
	vector< ThermodynamicData > Thermodynamics;
	vector< SingleReactionData > Reactions;
};


#endif /* HEADERS_CLASSES_HPP_ */
