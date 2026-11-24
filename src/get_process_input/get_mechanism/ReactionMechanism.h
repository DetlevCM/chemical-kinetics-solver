
#ifndef _REACTIONMECHANISM
#define _REACTIONMECHANISM

#include "../../global_const.h"

#include "Species.h"

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

using std::cout;
#include <iomanip>

#include <math.h>

namespace Reaction {

struct ArrheniusParameters {
  // bool Reversible;
  double A = 0.0;
  double n = 0.0;
  double Ea = 0.0;
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

enum TBtype { none, troe, sri };

struct ReactionParameters {
  bool Reversible = true;   // default
  bool IsDuplicate = false; // default

  ArrheniusParameters forward;
  bool explicit_reverse = false; // default
  ArrheniusParameters reverse;   // not yet used, but preparation

  TBtype ThirdBodyType = none;       // default is none
  bool collision_efficiency = false; // for third body reactions

  ArrheniusParameters TB_low; // preparation
  ThirdBody_troe TB_troe;     // preparation

  size_t sri_flag = 0; // default //// TODO improve
  ThirdBody_SRI TB_sri;

  vector<ThirdBodyParameters> TB_param;
};

struct SingleReactionData {
  ReactionParameters parameters;

  vector<double> Reactants;
  vector<double> Products;
};

string Prepare_Single_Reaction_Output(size_t, const vector<Species> &,
                                      const SingleReactionData &);

void WriteReactions(string, const vector<Species> &species,
                    const vector<SingleReactionData> &);

} // namespace Reaction

using namespace Reaction;

class ReactionMechanism {

private:
public:
  vector<Species> species;

  void Set_Species(const vector<Species> &set_species);

  size_t species_size() { return species.size(); }

public:
  vector<SingleReactionData> reactions;

  size_t reactions_size() { return reactions.size(); }

  void Set_Reactions(const vector<SingleReactionData> &set_reactions);

  static void get_mechanism(string filename,
                            ReactionMechanism &reaction_mechanism);

  struct SpeciesWithCoefficient {
    size_t SpeciesID;
    double coefficient;
    bool ismatched; // = false;
  };

  static vector<string> Read_Chemkin_Block(string filename, string blockname);

  static SpeciesWithCoefficient
  Return_Species_With_Coefficient(string, const vector<Species> &);

  static vector<int> Set_Mechanism_Units(string);

  static double Scale_Ea(double Ea_read_in, int scaling_type);

  static double Scale_A(double A_read_in, vector<double> ReactantData,
                        int scaling_type);

  // static vector<ReactionParameters>
  static vector<SingleReactionData>
  Get_Reactions(string filename, const vector<Species> &species);

  // static ReactionParameters
  static SingleReactionData
  Parse_Chemkin_Reaction_String(const vector<int> SchemeUnits,
                                const vector<Species> &species, string line);
};

#endif
