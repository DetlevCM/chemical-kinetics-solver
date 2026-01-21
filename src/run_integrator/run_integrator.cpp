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
  size_t i, j;
  size_t Number_Reactions = Reactions.size();
  size_t Number_Species = Reactions[0].Reactants.size();
  vector<TrackSpecies> SpeciesLossAll;

  TrackSpecies temp;

  for (i = 0; i < Number_Species; i++) {
    for (j = 0; j < Number_Reactions; j++) {
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
    for (j = 0; j < Number_Reactions; j++) {
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
  size_t Number_Reactions = Reactions.size();
  size_t Number_Species = Reactions[0].Reactants.size();
  size_t i, j;

  // Output per reaction
  for (i = 0; i < Number_Reactions; i++) {
    for (j = 0; j < Number_Species; j++) {
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
  size_t i, j;
  TrackSpecies temp;
  size_t Number_Reactions = Reactions.size();
  size_t Number_Species = Reactions[0].Reactants.size();

  if (!SwitchType) // false treat as rates function
  {
    // Output per reaction
    for (i = 0; i < Number_Reactions; i++) {
      if (Reactions[i].Reversible) // only include if the reaction is reversible
      {
        for (j = 0; j < Number_Species; j++) {
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
    for (i = 0; i < Number_Reactions; i++) {
      for (j = 0; j < Number_Species; j++) {
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
  size_t i;
  size_t Number_Reactions = Reactions.size();

  vector<ReactionParameter> temp_output;

  // Output per reaction
  for (i = 0; i < Number_Reactions; i++) {
    ReactionParameter temp_one_reaction;

    temp_one_reaction.paramA = Reactions[i].paramA;   // A
    temp_one_reaction.paramN = Reactions[i].paramN;   // n
    temp_one_reaction.paramEa = Reactions[i].paramEa; // Ea
    temp_one_reaction.Reversible = Reactions[i].Reversible;

    temp_output.push_back(temp_one_reaction);
  }

  return temp_output;
}

vector<double>
RunIntegrator::Get_Delta_N(const vector<SingleReactionData> Reactions) {
  size_t i, j;
  size_t number_species = Reactions[0].Reactants.size();
  size_t number_reactions = Reactions.size();

  vector<double> delta_n(number_reactions);

  for (i = 0; i < number_reactions; i++) {
    // double delta_n = 0;
    for (j = 0; j < number_species; j++) {
      delta_n[i] = delta_n[i] + Reactions[i].Reactants[j] +
                   Reactions[i].Products[j]; // reactants & products
    }
  }
  return delta_n;
}
