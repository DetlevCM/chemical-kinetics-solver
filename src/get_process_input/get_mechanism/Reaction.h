

#ifndef _REACTION
#define _REACTION

#include <fstream>
#include <iostream>

using std::cout;
using std::ofstream;

#include <sstream>
using std::ostringstream;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <math.h>

#include "./Species.h"

namespace Reaction {

struct ReactionParameter {
  bool Reversible; // want to get rid of this
  double A;
  double n;
  double Ea;
};

struct ThirdBody_troe {
  double a = 0.0;
  double T1 = 0.0;
  double T2 = 0.0; // only in 4 parameter troe
  double T3 = 0.0;
  bool is_4_param = false; // default
  bool has_troe = false;
};

struct ThirdBody_SRI {
  double a = 0.0;
  double b = 0.0;
  double c = 0.0;
  double d = 1.0; // default
  double e = 0.0; // default
  bool is_5_param = false;
};

struct ThirdBodyParameters {
  size_t SpeciesID;
  double value;
};

struct SingleReactionData {
  bool Reversible;
  bool IsDuplicate;

  ReactionParameter forward;
  bool explicit_reverse;
  ReactionParameter reverse; // not yet used, but preparation

  int ThirdBodyType; // 1: +M  2: (+M)

  bool collision_efficiency = false; // for third body reactions

  ReactionParameter TB_low; // preparation
  ThirdBody_troe TB_troe;   // preparation
  ThirdBody_SRI TB_sri;

  vector<ThirdBodyParameters> TB_param;

  vector<double> Reactants;
  vector<double> Products;
};

string Prepare_Single_Reaction_Output(size_t, const vector<Species> &,
                                      const SingleReactionData &);

void WriteReactions(string, const vector<Species> &species,
                    const vector<SingleReactionData> &);

} // namespace Reaction

#endif