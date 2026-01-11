

#include "ReactionMechanism.h"

void ReactionMechanism::Set_Species(const vector<Species> &set_species) {
  species = set_species;
}

void ReactionMechanism::Set_Reactions(
    const vector<SingleReactionData> &set_reactions) {
  reactions = set_reactions;
}

void ReactionMechanism::get_mechanism(string filename,
                                      ReactionMechanism &reaction_mechanism) {
  ifstream DataInputFromFile;
  // check the existence of the mechanism file
  DataInputFromFile.open(filename.c_str());
  if (!DataInputFromFile.is_open()) {
    cout << "Error opening " << filename << ".\n";
    exit(1); // standard error code
  }
  DataInputFromFile.close();

  /* Read in the mechanism, that is species, thermodynamic data and reactions.
   * Report on the progress as the sections are read in.
   */
  // reaction_mechanism.Species = Get_Species(filename);

  vector<Species> species =
      Species::VectorClass(Species::Get_Species(filename));
  // vector<string> vec_str_species = Species::Get_Species(filename);
  size_t Number_Species = species.size();
  cout << "The Mechanism contains " << Number_Species << " Species.\n";
  Species::Get_Thermodynamic_Data(filename, species);

  reaction_mechanism.Set_Species(species);
  // cout << "The Mechanism contains " <<
  // reaction_mechanism.Thermodynamics.size() << " Thermodynamic Data
  // Entries.\n";
  //// NOTE: There is no longer a way to determine how many thermodynamic data
  /// entries have been read in

  vector<SingleReactionData> reactions =
      ReactionMechanism::Get_Reactions(filename, species);
  // reaction_mechanism.Reactions = Get_Reactions(filename,
  // reaction_mechanism.Species);
  size_t Number_Reactions = reactions.size();
  reaction_mechanism.Set_Reactions(reactions);

  //// NOTE: above assignment stores data in class ReactionMechanism,
  /// implementation is ugly

  cout << "The Mechanism contains " << Number_Reactions << " Reactions.\n";
}
