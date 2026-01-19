/*
 * Make_Irreversible.cpp
 *
 *  Created on: 19.12.2012
 *  Revised on: 20.10.2017
 *      Author: DetlevCM
 */

/*
 * Function based on published work by:
 * "The comparison of detailed chemical kinetic mechanisms; forward versus
 * reverse rates with CHEMRev" by Sebastien Rolland and John M. Simmie,
 * International Journal of Chemical Kinetics
 * 2005 Volume 37 (3) pages 119-125
 *
 *
 * NOTE:
 * The implementation is partially hardcoded - which makes it clumsy at times.
 * However it is only called once, so slowness is bearable output has been
 * checked and verified.
 *
 */

#include "./main.h"

#include "./run_integrator/run_integrator.h"
#include "./run_integrator/solver_calculations/solver_calculations.h"

// variant of the function for solver calculations to make irreversible
void Calculate_Rate_Constant(
    vector<double> &Kf, vector<double> &Kr, const double Temperature,
    const vector<Reaction::ReactionParameter> &ReactionParameters,
    const vector<Species::ThermodynamicData::CalculatedThermodynamics>
        &CalculatedThermo,
    const vector<TrackSpecies> &SpeciesAll, const vector<double> &Delta_N)

{

  size_t Number_Reactions = ReactionParameters.size();

  // Pressure Independent Reactions Only
  /*
  k[i] = a1[i]*exp(-e1[i]*inv_T);  // kinf calculation
  if (n1[i] != 0.0)
  k[i] *= pow(T, n1[i]);
  //*/

  size_t i;

  // Define vectors in the right size right away
  vector<double> delta_H(Number_Reactions);
  vector<double> delta_S(Number_Reactions);
  vector<double> delta_G(Number_Reactions);

  // Worked out per reaction
  // as we have the value of the calculated thermodynamics, we just need to put
  // them together per reaction, going through every species

  for (i = 0; i < SpeciesAll.size(); i++) { // loop over every reaction
    delta_H[SpeciesAll[i].ReactionID] =
        delta_H[SpeciesAll[i].ReactionID] +
        (CalculatedThermo[SpeciesAll[i].SpeciesID].Hf *
         SpeciesAll[i].coefficient);

    delta_S[SpeciesAll[i].ReactionID] =
        delta_S[SpeciesAll[i].ReactionID] +
        (CalculatedThermo[SpeciesAll[i].SpeciesID].S *
         SpeciesAll[i].coefficient);
  }

  for (i = 0; i < Number_Reactions;
       i++) // Straightforward Arrhenius Expression/Equation
  {
    Kf[i] = ReactionParameters[i].paramA *
            exp(-ReactionParameters[i].paramEa /
                Temperature); // do NOT forget the - !!!

    //* Speedup by only raising temperature to power where needed: improvement
    // is large :)
    if (ReactionParameters[i].paramN !=
        0) // raising to power 0 has no effect, so only if not 0
    {
      // unsure if this check really gives a performance improvement...
      // maybe it used to and no longer does with a modern
      // compiler/processor/kernel
      // if(ReactionParameters[i].paramN != 1)
      //{
      Kf[i] = Kf[i] * pow(Temperature, ReactionParameters[i].paramN);
      /*}
      else
      {
              Kf[i] = Kf[i] * Temperature; // raise temp^1 = temp
      }//*/
    }

    /*
    cout <<
    //		Temperature << " , " <<
    //		exp(-ReactionParameters[i].paramEa/Temperature) << " , " <<
                    ReactionParameters[i].paramA << " , " <<
                    ReactionParameters[i].paramN << " , " <<
                    ReactionParameters[i].paramEa << " , " <<
                    Kf[i] << "\n";//*/

    // default, change if reversible - seems a little bit faster...
    Kr[i] = 0;
    // then calculate and set the reverse if required
    if (ReactionParameters[i].Reversible) {
      double temp_kp, temp_kc;
      delta_G[i] = delta_H[i] - Temperature * delta_S[i];
      temp_kp = exp(-delta_G[i] / (R * Temperature));

      if (Delta_N[i] == 0) // an if check is less expensive than a pow(,)
      {
        Kr[i] = Kf[i] / temp_kp;
      } else {
        temp_kc = temp_kp * pow((0.0820578 * Temperature),
                                (-Delta_N[i])); // L atm K^(-1) mol^(-1)
        // temp_kc = temp_kp*pow((1.3624659e-22*Temperature),(-Delta_N[i])); //
        // for molecules cm^(-3)
        Kr[i] = Kf[i] / temp_kc;
      }

      // IEEE standards hack to avoid Nan Rate, shouldn't exist in the first
      // place...
      /*if(Kr[i] != Kr[i])
      {
              Kr[i] = 0;
      }//*/
    }
    // if it is set to zero at the start and not touched for irreversible
    // reactions thus this is redundant Fewer memory allocations should speed
    // things up - or not?
    /*
    else
    {
            Kr[i] = 0;
    }//*/
  }
}

vector<SingleReactionData> Make_Irreversible(
    vector<SingleReactionData> Reactions, vector<Species> species,
    double Initial_Temperature, /// use initial temperature from initial data
    double Range                // specify +/- range around initial temperature
) {
  vector<SingleReactionData> Irreversible_Scheme;

  size_t i, j, k;
  size_t Number_Species = species.size();
  size_t Number_Reactions = Reactions.size();
  double Temperature;

  vector<double> Local_Delta_N = RunIntegrator::Get_Delta_N(Reactions);

  // check temperature and range provided, else use default
  if (Initial_Temperature == 0) {
    Initial_Temperature = 298.15;
  }
  if (Range == 0) {
    Range = 25.0;
  }
  // check we don't end up below 0K
  if (Initial_Temperature - Range <= 0) {
    Range = Initial_Temperature - 1;
  }

  vector<ReactionParameter> LocalReactionParameters;
  vector<TrackSpecies> LocalReactantsForReactions;
  vector<TrackSpecies> LocalProductsForReactions;
  vector<TrackSpecies> LocalSpeciesLossAll;

  LocalReactionParameters =
      RunIntegrator::Process_Reaction_Parameters(Reactions);
  LocalReactantsForReactions =
      RunIntegrator::Reactants_ForReactionRate(Reactions);
  LocalProductsForReactions =
      RunIntegrator::Products_ForReactionRate(Reactions, false);
  LocalSpeciesLossAll = RunIntegrator::PrepareSpecies_ForSpeciesLoss(Reactions);
  // Local copy of the global version. Needed to work out rate constants, but
  // are based on data in Reactions array

  vector<vector<double>> allkreverse;
  vector<vector<double>> SensitivityMatrix; // called X in the paper
  vector<double> OneTempSensitivityMatrix;
  OneTempSensitivityMatrix.resize(3);

  vector<Species::ThermodynamicData::CalculatedThermodynamics> CalculatedThermo(
      Number_Species);

  vector<double> Kf(Number_Reactions);
  vector<double> Kr(Number_Reactions);

  // let us use 25 steps in either direction of the temperature
  for (i = 0; i < 50; i++) {
    Temperature =
        Initial_Temperature - Range +
        ((double)i * Range / 25.0); // +/- Range, 25 Steps in either direction

    Species::ThermodynamicData::ThermoT temperatures(Temperature);
    /* Hf, Cp, Cv, S */
    for (i = 0; i < Number_Species; i++) {
      CalculatedThermo[i] =
          species[i].thermodynamicdata.calculate_thermodynamics(temperatures);
    }
    Calculate_Rate_Constant(Kf, Kr, Temperature, LocalReactionParameters,
                            CalculatedThermo, LocalSpeciesLossAll,
                            Local_Delta_N);

    for (j = 0; j < Kr.size(); j++) {
      Kr[j] = log(Kr[j]);
    }

    allkreverse.push_back(Kr);

    OneTempSensitivityMatrix[0] = 1.0;
    OneTempSensitivityMatrix[1] = log(Temperature);
    OneTempSensitivityMatrix[2] = 1.0 / Temperature;

    SensitivityMatrix.push_back(OneTempSensitivityMatrix);
  }

  // At this point we now have a collection of kr as well as the sensitivity
  // matrix

  vector<vector<double>> SensitivityMatrixTranspose; // called X in the paper
  vector<double> RowSensitivityMatrixTranspose;
  RowSensitivityMatrixTranspose.resize(SensitivityMatrix.size());

  // stupid way to give the matrix 3 rows
  SensitivityMatrixTranspose.push_back(RowSensitivityMatrixTranspose);
  SensitivityMatrixTranspose.push_back(RowSensitivityMatrixTranspose);
  SensitivityMatrixTranspose.push_back(RowSensitivityMatrixTranspose);

  for (i = 0; i < 3; i++) {
    for (j = 0; j < SensitivityMatrix.size(); j++) {
      SensitivityMatrixTranspose[i][j] = SensitivityMatrix[j][i];
    }
  }

  // Now we need to calculate our betas.
  // First on the list, SensitivityMatrixTranspose times SensitivityMatrix

  vector<vector<double>>
      SMtxSM; // Sensitivity Matrix transpose times (x) Sensitivity Matrix
  vector<double> SMt; // Sensitivity Matrix transpose times
  SMt.resize(3);

  // result is a 3x3 matrix
  SMtxSM.push_back(SMt);
  SMtxSM.push_back(SMt);
  SMtxSM.push_back(SMt);

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      for (k = 0; k < SensitivityMatrix.size(); k++) {
        SMtxSM[i][j] = SMtxSM[i][j] + SensitivityMatrixTranspose[i][k] *
                                          SensitivityMatrix[k][j];
      }
    }
  }

  /* Now we need the inverse of the matrix.
   *
   * a , b , c
   * d , e , f
   * g , h , k
   *
   * divisor = cdh - cge - bdk + bgf + aek - ahf
   *
   * So matrix is, every entry divided by divisor, rest
   *
   *   ek - hf , -dk + gf ,  dh - ge
   *   ch - bk , -cg + ak ,  bg - ah
   * - ce + bf ,  cd - af , -bd + ae
   */

  double determinant = SMtxSM[0][2] * SMtxSM[1][0] * SMtxSM[2][1] -
                       SMtxSM[0][2] * SMtxSM[2][0] * SMtxSM[1][1] -
                       SMtxSM[0][1] * SMtxSM[1][0] * SMtxSM[2][2] +
                       SMtxSM[0][1] * SMtxSM[2][0] * SMtxSM[1][2] +
                       SMtxSM[0][0] * SMtxSM[1][1] * SMtxSM[2][2] -
                       SMtxSM[0][0] * SMtxSM[2][1] * SMtxSM[1][2];

  vector<vector<double>>
      SMtxSMInv; // Sensitivity Matrix transpose times (x) Sensitivity Matrix
  vector<double> SMtInv; // Sensitivity Matrix transpose times
  SMtInv.resize(3);

  // Stupid way of creating an inverse matrix... but it is only 3*3
  SMtxSMInv.push_back(SMtInv);
  SMtxSMInv.push_back(SMtInv);
  SMtxSMInv.push_back(SMtInv);

  SMtxSMInv[0][0] = SMtxSM[1][1] * SMtxSM[2][2] - SMtxSM[2][1] * SMtxSM[1][2];
  SMtxSMInv[0][1] = -SMtxSM[1][0] * SMtxSM[2][2] + SMtxSM[2][0] * SMtxSM[1][2];
  SMtxSMInv[0][2] = SMtxSM[1][0] * SMtxSM[2][1] - SMtxSM[2][0] * SMtxSM[1][1];

  SMtxSMInv[1][0] = SMtxSM[0][2] * SMtxSM[2][1] - SMtxSM[0][1] * SMtxSM[2][2];
  SMtxSMInv[1][1] = -SMtxSM[0][2] * SMtxSM[2][0] + SMtxSM[0][0] * SMtxSM[2][2];
  SMtxSMInv[1][2] = SMtxSM[0][1] * SMtxSM[2][0] - SMtxSM[0][0] * SMtxSM[2][1];

  SMtxSMInv[2][0] = -SMtxSM[0][2] * SMtxSM[1][1] + SMtxSM[0][1] * SMtxSM[1][2];
  SMtxSMInv[2][1] = SMtxSM[0][2] * SMtxSM[1][0] - SMtxSM[0][0] * SMtxSM[1][2];
  SMtxSMInv[2][2] = -SMtxSM[0][1] * SMtxSM[1][0] + SMtxSM[0][0] * SMtxSM[1][1];

  // Don't forget dividing by the determinant
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      SMtxSMInv[i][j] = SMtxSMInv[i][j] / determinant;
    }
  }

  // now transpose times the logarithms of k_r to get a 3 entry vector...
  vector<vector<double>> InvMxSM;
  vector<double> xSM;
  xSM.resize(SensitivityMatrix.size());

  InvMxSM.push_back(xSM);
  InvMxSM.push_back(xSM);
  InvMxSM.push_back(xSM);

  // matrix mult: row x column
  for (i = 0; i < 3; i++) {
    for (j = 0; j < SensitivityMatrix.size(); j++) {
      for (k = 0; k < 3; k++) {
        InvMxSM[i][j] =
            InvMxSM[i][j] + SMtxSMInv[i][k] * SensitivityMatrixTranspose[k][j];
      }
    }
  }

  SingleReactionData SingleReaction;
  // assemble the arrays for writing a new reactions array
  vector<double> ReactantData; // Reactant Information
  ReactantData.resize(Number_Species);
  vector<double> ProductData; // Product Information
  ProductData.resize(Number_Species);
  vector<double> ReactionParameters;
  ReactionParameters.resize(4);

  for (i = 0; i < Number_Reactions; i++) {
    // 1) retain the old forward reaction

    SingleReaction.Reactants = Reactions[i].Reactants;
    SingleReaction.Products = Reactions[i].Products;
    SingleReaction.paramA = Reactions[i].paramA;
    SingleReaction.paramN = Reactions[i].paramN;
    SingleReaction.paramEa = Reactions[i].paramEa;
    // switch reaction to irreversible:
    SingleReaction.Reversible = false;
    SingleReaction.IsDuplicate = Reactions[i].IsDuplicate;

    /*
    printf("Reaction %u Forward: %.3e %.3e %.3e %.3e || ",i,
                    ReactionParameters[0],ReactionParameters[1],
                    ReactionParameters[2],ReactionParameters[3]); // Works :) */

    Irreversible_Scheme.push_back(SingleReaction);

    // Calculate the parameters
    vector<double> beta;
    beta.resize(3);

    // check if the reaction was reversible - if yes, calculate irreversible
    // form
    if (Reactions[i].Reversible) {

      for (j = 0; j < 3; j++) {
        for (k = 0; k < SensitivityMatrix.size(); k++) {
          beta[j] = beta[j] + InvMxSM[j][k] * allkreverse[k][i];
        }
      }

      // Write out reverse reaction:
      // And now the reverse Reaction - Products and Reactants reversed
      for (j = 0; j < Number_Species; j++) {
        ReactantData[j] = -Reactions[i].Products[j];
        ProductData[j] = -Reactions[i].Reactants[j];
      }

      SingleReaction.Reactants = ReactantData;
      SingleReaction.Products = ProductData;
      SingleReaction.paramA = exp(beta[0]);
      SingleReaction.paramN = beta[1];
      SingleReaction.paramEa = -beta[2];
      // retain reaction as irreversible:
      SingleReaction.Reversible = false;
      SingleReaction.IsDuplicate = Reactions[i].IsDuplicate;

      /*
      printf("Reverse %.3e %.3e %.3e %.3e \n",
                      ReactionParameters[0],ReactionParameters[1],
                      ReactionParameters[2],ReactionParameters[3]); // Works :)
    */

      Irreversible_Scheme.push_back(SingleReaction);
    }
  }
  return Irreversible_Scheme;
}
