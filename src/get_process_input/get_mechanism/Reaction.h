

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

struct SingleReactionData {
  bool Reversible;
  bool IsDuplicate;
  double paramA;
  double paramN;
  double paramEa;
  int ThirdBodyType; // 1: +M  2: (+M)
  vector<double> ThBd_LOW;
  vector<double> ThBd_TROE;
  // vector<ThirdBodyParameters> ThBd_param;
  vector<double> Reactants;
  vector<double> Products;
};

struct ReactionParameter {
  bool Reversible;
  double paramA;
  double paramN;
  double paramEa;
};

string Prepare_Single_Reaction_Output(size_t, const vector<Species> &,
                                      const SingleReactionData &);

void WriteReactions(string, const vector<Species> &species,
                    const vector<SingleReactionData> &);

} // namespace Reaction

#endif