/*
 * Class_Reaction_Mechanism.hpp
 *
 *  Created on: Sep 30, 2018
 *      Author: detlevcm
 */

#ifndef HEADERS_STRUCTS_CLASS_REACTION_MECHANISM_HPP_
#define HEADERS_STRUCTS_CLASS_REACTION_MECHANISM_HPP_


class ThermodynamicData {
public:
	double TLow;
	double THigh;
	double TChange;

	double NasaLow1;
	double NasaLow2;
	double NasaLow3;
	double NasaLow4;
	double NasaLow5;
	double NasaLow6;
	double NasaLow7;

	double NasaHigh1;
	double NasaHigh2;
	double NasaHigh3;
	double NasaHigh4;
	double NasaHigh5;
	double NasaHigh6;
	double NasaHigh7;
};

class SingleReactionData {
public:
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
	size_t species;
	size_t reactions;

	vector< string > Species;
	vector< ThermodynamicData > Thermodynamics;
	vector< SingleReactionData > Reactions;
};



#endif /* HEADERS_STRUCTS_CLASS_REACTION_MECHANISM_HPP_ */
