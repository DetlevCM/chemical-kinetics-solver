/*
 * Run_Integrator.cpp
 *
 *  Created on: 20.10.2017
 *      Author: DetlevCM
 */

#include "./run_integrator.h"

//// NOTE: 4 pre-processing functions for the integration to proceed efficiently

vector<TrackSpecies> RunIntegrator::PrepareSpecies_ForSpeciesLoss(
    const vector<SingleReactionData> &Reactions) {

  vector<TrackSpecies> SpeciesLossAll;
  TrackSpecies temp;

  for (size_t i = 0; i < Reactions[0].Reactants.size(); i++) {
    for (size_t j = 0; j < Reactions.size(); j++) {
      if (Reactions[j].Reactants[i] != 0) // Reactants
      {
        temp.SpeciesID = i;
        temp.ReactionID = j;
        temp.coefficient = Reactions[j].Reactants[i];
        SpeciesLossAll.push_back(temp);
      }
    }
    // not sure if this may not be more efficient - first just negative, then
    // just positive
    for (size_t j = 0; j < Reactions.size(); j++) {
      if (Reactions[j].Products[i] != 0) // Products
      {
        temp.SpeciesID = i;
        temp.ReactionID = j;
        temp.coefficient = Reactions[j].Products[i];
        SpeciesLossAll.push_back(temp);
      }
    }
  }

  return SpeciesLossAll;
}

vector<TrackSpecies> RunIntegrator::Reactants_ForReactionRate(
    const vector<SingleReactionData> &Reactions) {
  vector<TrackSpecies> temp_proc_reac;
  // Basically go through the reactions and accumulate the Relevant species

  TrackSpecies temp;

  // Output per reaction
  for (size_t i = 0; i < Reactions.size(); i++) {
    for (size_t j = 0; j < Reactions[0].Reactants.size(); j++) {
      if (Reactions[i].Reactants[j] != 0) {
        temp.ReactionID = i;
        temp.SpeciesID = j;
        temp.coefficient = -Reactions[i].Reactants[j];
        temp_proc_reac.push_back(temp);
      }
    }
  }

  return temp_proc_reac;
}

vector<TrackSpecies> RunIntegrator::Products_ForReactionRate(
    const vector<SingleReactionData> &Reactions, bool SwitchType) {
  vector<TrackSpecies> temp_proc_reac;
  // Basically go through the reactions and accumulate the Relevant species

  TrackSpecies temp;

  if (!SwitchType) // false treat as rates function
  {
    // Output per reaction
    for (size_t i = 0; i < Reactions.size(); i++) {
      if (Reactions[i].Reversible) // only include if the reaction is reversible
      {
        for (size_t j = 0; j < Reactions[0].Reactants.size(); j++) {
          if (Reactions[i].Products[j] != 0) {
            temp.ReactionID = i;
            temp.SpeciesID = j;
            temp.coefficient = Reactions[i].Products[j];
            temp_proc_reac.push_back(temp);
          }
        }
      }
    }
  } else // true, for Rates Analysis
  {
    for (size_t i = 0; i < Reactions.size(); i++) {
      for (size_t j = 0; j < Reactions[0].Reactants.size(); j++) {
        if (Reactions[i].Products[j] != 0) {
          temp.ReactionID = i;
          temp.SpeciesID = j;
          temp.coefficient = Reactions[i].Products[j];
          temp_proc_reac.push_back(temp);
        }
      }
    }
  }

  return temp_proc_reac;
}

vector<ReactionParameter> RunIntegrator::Process_Reaction_Parameters(
    const vector<SingleReactionData> &Reactions) {

  vector<ReactionParameter> temp_output;

  // Output per reaction
  for (size_t i = 0; i < Reactions.size(); i++) {
    ReactionParameter temp_one_reaction;

    temp_one_reaction.A = Reactions[i].forward.A;   // A
    temp_one_reaction.n = Reactions[i].forward.n;   // n
    temp_one_reaction.Ea = Reactions[i].forward.Ea; // Ea
    temp_one_reaction.Reversible = Reactions[i].Reversible;

    temp_output.push_back(temp_one_reaction);
  }

  return temp_output;
}

vector<double>
RunIntegrator::Get_Delta_N(const vector<SingleReactionData> Reactions) {

  vector<double> delta_n(Reactions.size());

  for (size_t i = 0; i < Reactions.size(); i++) {
    for (size_t j = 0; j < Reactions[0].Reactants.size(); j++) {
      delta_n[i] = delta_n[i] + Reactions[i].Reactants[j] +
                   Reactions[i].Products[j]; // reactants & products
    }
  }
  return delta_n;
}
