

#ifndef _REACTION
#define _REACTION

#include <iostream>
#include <fstream>

using std::ofstream;
using std::cout; 

#include <sstream>
using std::ostringstream;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <math.h>

namespace Reaction
{

struct SingleReactionData {
	bool Reversible;
	bool IsDuplicate;
	double paramA;
	double paramN;
	double paramEa;
	int ThirdBodyType; // 1: +M  2: (+M)
	vector<double> ThBd_LOW;
	vector<double> ThBd_TROE;
	//vector<ThirdBodyParameters> ThBd_param;
	vector<double> Reactants;
	vector<double> Products;
};

string Prepare_Single_Reaction_Output(
		size_t ,
		const vector< string >& ,
		const SingleReactionData&
);


void WriteReactions(
		string ,
		const vector< string >& ,
		const vector< SingleReactionData >&
);


}

#endif