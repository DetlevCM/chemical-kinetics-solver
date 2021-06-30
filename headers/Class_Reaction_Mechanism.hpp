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
	double a = 0.0;
	double b = 0.0;
	double c = 0.0;
	double d = 1.0; // default
	double e = 0.0; // default
	bool is_5_param = false;
};

class TroeThirdBody{
public:
	double a = 0.0;
	double T1 = 0.0;
	double T2 = 0.0; // only in 4 parameter troe
	double T3 = 0.0;
	bool is_4_param = false; // default
	bool has_troe = false;
};


// given that we have k0, kinf/kf and explicit kr
class param_Arrhenius{
public:
	double A = 0.0;
	double n = 0.0;
	double Ea = 0.0;
};

class SingleReactionData {
public:
	bool Reversible = false;
	bool IsDuplicate = false;

	param_Arrhenius param_forward; // standard forward Arrhenius parameters

	bool explicit_rev = false;     // a case where an explicit reverse reaction is specified
	param_Arrhenius param_reverse; // in case of explicit reverse parameters


	// third body parameters
	int ThirdBodyType = 0; // 0: no third body 1: +M  2: (+M) 3: (+H20) etc. not found in GRI Mech...
	bool collision_efficiency = false; // for third body reactions

	param_Arrhenius param_TB_low;

	size_t sri_flag = 0; // default
	SRIThirdBody sriThirdBody;
	TroeThirdBody troeThirdBody;

	vector<ThirdBodyParameters> ThBd_param;

	// reactants and products
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

	param_Arrhenius param_forward;
	param_Arrhenius param_reverse;
	param_Arrhenius param_TB_low;

	// for cases where an explicit reverse reaction is specified
	bool explicit_rev = false;

	// add in third body support...
	int ThirdBodyType; // 1: +M  2: (+M) 3: (+H20) etc. not found in GRI Mech...
	vector<ThirdBodyParameters> ThBd_param;
	size_t sri_flag;
	SRIThirdBody sriThirdBody;
	TroeThirdBody troeThirdBody;
	bool collision_efficiency; // for third body reactions
};

#endif /* HEADERS_STRUCTS_CLASS_REACTION_MECHANISM_HPP_ */
