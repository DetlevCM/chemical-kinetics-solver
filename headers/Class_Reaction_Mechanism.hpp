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


class SRIThirdBody {
public:
	double a;
	double b;
	double c;
	double d = 1; // default
	double e = 0; // default
	bool is_5_param = false;
};

class TroeThirdBody{
public:
	double a;
	double T1;
	double T2; // only in 4 parameter troe
	double T3;
	bool is_4_param = false; // default
};

class LowThirdBody{
public:
	double paramA0;
	double paramN0;
	double paramEa0;
};

class SingleReactionData {
public:
	bool Reversible;
	bool IsDuplicate;
	double paramA;
	double paramN;
	double paramEa;
	int ThirdBodyType; // 1: +M  2: (+M) 3: (+H20) etc. not found in GRI Mech...
	//vector<double> ThBd_LOW;
	//vector<double> ThBd_TROE;
	vector<ThirdBodyParameters> ThBd_param;
	size_t sri_flag = 0; // default
	SRIThirdBody sriThirdBody;
	LowThirdBody lowThirdBody;
	TroeThirdBody troeThirdBody;
	bool collision_efficiency; // for third body reactions
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

class ReactionParameter {
public:
	bool Reversible;
	double paramA;
	double paramN;
	double paramEa;

	// add in third body support...
	int ThirdBodyType; // 1: +M  2: (+M) 3: (+H20) etc. not found in GRI Mech...
	vector<ThirdBodyParameters> ThBd_param;
	size_t sri_flag;
	SRIThirdBody sriThirdBody;
	LowThirdBody lowThirdBody;
	TroeThirdBody troeThirdBody;
	bool collision_efficiency; // for third body reactions
};

#endif /* HEADERS_STRUCTS_CLASS_REACTION_MECHANISM_HPP_ */
