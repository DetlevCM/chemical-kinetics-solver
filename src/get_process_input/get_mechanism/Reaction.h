

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

struct SingleReactionData {
  bool Reversible;
  bool IsDuplicate;
  // double paramA;
  // double paramN;
  // double paramEa;

  ReactionParameter forward;
  bool explicit_reverse;
  ReactionParameter reverse;

  int ThirdBodyType; // 1: +M  2: (+M)
  vector<double> ThBd_LOW;
  vector<double> ThBd_TROE;
  // vector<ThirdBodyParameters> ThBd_param;
  vector<double> Reactants;
  vector<double> Products;
};

string Prepare_Single_Reaction_Output(size_t, const vector<Species> &,
                                      const SingleReactionData &);

void WriteReactions(string, const vector<Species> &species,
                    const vector<SingleReactionData> &);

} // namespace Reaction

#endif