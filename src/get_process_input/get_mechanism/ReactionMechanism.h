
#ifndef _REACTIONMECHANISM
#define _REACTIONMECHANISM

#include "../../global_const.h"

#include "Species.h"

#include "./Reaction.h"
using namespace Reaction;

#include <string>
using std::string;

#include <vector>
using std::vector;

using std::cout;
#include <iomanip>

class ReactionMechanism {

private:
  // vector< Species > species;

public:
  vector<Species> species;

  void Set_Species(const vector<Species> &set_species);

  size_t species_size() { return species.size(); }

private:
public:
  vector<SingleReactionData> reactions;

  size_t reactions_size() { return reactions.size(); }

  void Set_Reactions(const vector<SingleReactionData> &set_reactions);

  static void get_mechanism(string filename,
                            ReactionMechanism &reaction_mechanism);

  struct ThirdBodyParameters {
    size_t SpeciesID;
    double value;
  };

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

  static vector<SingleReactionData>
  Get_Reactions(string filename, const vector<Species> &species);

  static SingleReactionData
  Parse_Chemkin_Reaction_String(const vector<int> SchemeUnits,
                                const vector<Species> &species, string line);
};

#endif
