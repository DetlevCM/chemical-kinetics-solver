/*
 * WriteMechanism.cpp
 *
 *  Created on: 21.11.2012
 *      Author: DetlevCM
 */

#include "./Reaction.h"

void Reaction::WriteReactions(string filename, const vector<Species> &species,
                              const vector<SingleReactionData> &Reactions) {
  size_t i;
  ofstream ReactionsOutput(filename.c_str(), std::ios::out);

  if (ReactionsOutput.is_open()) {
    size_t Number_Reactions = 0;
    Number_Reactions = Reactions.size();
    size_t Number_Species = 0;
    Number_Species = Reactions[0].Reactants.size();

    // ReactionsOutput << "REACTIONS	KELVINS	MOLES 	\n";
    ReactionsOutput << "REACTIONS 	KCAL/MOL 	MOLES 	\n";

    for (i = 0; i < Number_Reactions; i++) {
      ReactionsOutput << Prepare_Single_Reaction_Output(Number_Species, species,
                                                        Reactions[i]);
      if (Reactions[i].IsDuplicate == true) {
        ReactionsOutput << "DUPLICATE\n";
      }
    }

    ReactionsOutput << "END\n";
    ReactionsOutput.close();
  } else
    cout << "Unable to open file";
  cout << "File " << filename << " written.\n";
}

string
Reaction::Prepare_Single_Reaction_Output(size_t Number_Species,
                                         const vector<Species> &species,
                                         const SingleReactionData &Reaction) {
  size_t i;
  bool check;

  ostringstream convert;

  double Reaction_Order = 0;

  check = false;
  for (i = 0; i < Number_Species; i++) // Line 1
  {
    if (Reaction.Reactants[i] != 0) {
      if (Reaction.Reactants[i] == -1.0) {
        if (check) {
          convert << " + ";
        }
        convert << species[i].Name;
        check = true;
        Reaction_Order = Reaction_Order + 1; // stoichiometry 1
      } else {
        if (check) {
          convert << " + ";
        }
        convert << fabs(Reaction.Reactants[i]) << species[i].Name;
        check = true;
        Reaction_Order =
            Reaction_Order + fabs(Reaction.Reactants[i]); // stoichiometry not 1
      }
    }
  }

  // sign
  if (Reaction.Reversible) {
    convert << " =  ";
  } else {
    convert << " => ";
  }

  // product
  check = false;
  for (i = 0; i < Number_Species; i++) // Line 1
  {
    if (Reaction.Products[i] != 0) {
      if (Reaction.Products[i] == 1.0) {
        if (check) {
          convert << " + ";
        }
        convert << species[i].Name;
        check = true;
      } else {
        if (check) {
          convert << " + ";
        }
        convert << Reaction.Products[i] << species[i].Name;
        check = true;
      }
    }
  }

  // Arrhenius parameters correct moles/l to moles/cm^3
  /*convert << "\t\t" << Reaction.paramA*1000 << "\t" << Reaction.paramN << "\t"
   * << Reaction.paramEa << "\n"; //*/

  // ensure that A is scaled correctly to per cm^3 units depending on the
  // reaction order
  double preexponential_A =
      Reaction.paramA * pow(1000.0, (Reaction_Order - 1.0));

  convert << "\t\t" << preexponential_A << "\t" <<
      // Reaction.paramA*1000 << "\t" << // mol / cm^3
      Reaction.paramN << "\t" << (Reaction.paramEa / 1000 * 1.98709)
          << "\n"; // correct for kcal/mol

  return convert.str();
}