/*
 * Reduce_Reactions.cpp
 *
 *  Created on: 28.12.2012
 *      Author: DetlevCM
 */

#include "Mechanism_Reduction.h"

void MechanismReduction::ReactionRateImportance(vector<double> &KeyRates,
                                                const vector<double> &Rates,
                                                double parameter) {

  size_t i;

  // I can now process the rates to get an idea as to which reactions I can
  // remove

  double maxrate = 0;

  for (i = 0; i < Rates.size(); i++) {
    if (maxrate < Rates[i]) {
      maxrate = Rates[i];
    }
  }

  // Now determine if the value is important
  double threshold = maxrate * pow(10, -parameter); // / (exp(10.0,parameter));
  // cout << parameter << " " << maxrate << " " << threshold << "\n";

  for (i = 0; i < Rates.size(); i++) {
    if (Rates[i] >= threshold) {
      KeyRates[i] = KeyRates[i] + 1;
    }
  }
}

// retain the reactions found to be important
vector<ReactionParameters> MechanismReduction::ReduceReactionsNew(
    const vector<Species> &species, const vector<ReactionParameters> &Reactions,
    const vector<double> &SelectedReactions) {

  size_t i;

  vector<ReactionParameters> ReducedReactions;
  vector<ReactionParameters> ReducedReactionsRemoved;

  for (i = 0; i < SelectedReactions.size(); i++) {
    // cout << i << "\n";
    if (SelectedReactions[i] > 0) {
      ReducedReactions.push_back(Reactions[i]);
    } else {
      ReducedReactionsRemoved.push_back(Reactions[i]);
    }
  }

  if (ReducedReactionsRemoved.size() > 0) {
    WriteReactions("Removed_Reactions.txt", species, ReducedReactionsRemoved);
  }
  // printf("Got to the end of Reaction reduction!! \n");
  return ReducedReactions;
}
