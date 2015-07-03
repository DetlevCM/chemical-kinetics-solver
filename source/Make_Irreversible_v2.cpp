/*
 * Make_Irreversilbe.cpp
 *
 *  Created on: 19.12.2012
 *      Author: DetlevCM
 */

/* Function designed on the basis of
 * "The comparison of detailed chemical kinetic mechanisms; forward versus reverse rates with CHEMRev"
 * by Sebastien Rolland and John M. Simmie,
 * published 2005 */

// NOTE:
// The implementation is hardcoded - which makes it clumsy at times, it needs restructuring and cleaning up....
// However it is only called once, so slowness is bearable
// output has been checked and verified.

#include <MyHeaders.h>

vector< SingleReactionData > Make_Irreversible(
		vector< SingleReactionData > Reactions,
		const vector< ThermodynamicData > Thermodynamics
)
{
	/* 2002 CODATA values */
	//double R = 8.314472e0; // Ea is given in Kelvins, therefore R is not needed
	vector< SingleReactionData > Irreversible_Scheme;

	int i,j,k;
	int Number_Species = Thermodynamics.size();
	int Number_Reactions = Reactions.size();
	double Temperature;
	vector< double > Local_Delta_N = Get_Delta_N(Reactions);


	vector< ReactionParameter > LocalReactionParameters;
	vector< TrackSpecies > LocalReactantsForReactions;
	vector< TrackSpecies > LocalProductsForReactions;
	vector< TrackSpecies > LocalSpeciesLossAll;


	LocalReactionParameters = Process_Reaction_Parameters(Reactions);
	LocalReactantsForReactions = Reactants_ForReactionRate(Reactions);
	LocalProductsForReactions = Products_ForReactionRate(Reactions,false);
	LocalSpeciesLossAll = PrepareSpecies_ForSpeciesLoss(Reactions);

	// Local copy of the global version. Needed to work out rate constants, but are based on data in Reactions array

	vector< vector< double > > allkreverse;

	vector< vector< double > > SensitivityMatrix;	// called X in the paper
	vector< double > OneTempSensitivityMatrix;
	OneTempSensitivityMatrix.resize(3);

	// store the k for every reaction and every temperature

	//cout << "checkpoint 1 \n";
	vector< vector< double > > CalculatedThermo(Number_Species);
	for(i=0;i<Number_Species;i++)
	{
		CalculatedThermo[i].resize(4);
	}
	vector< double > Kf(Number_Reactions) ;
	vector< double > Kr(Number_Reactions) ;

	for(i=0;i<50;i++)
	{
		//cout << i << "\n";
		Temperature = 100 + 20 * i; // start at 200K, avoids issues at 0K
		//Temperature = 500;

		Calculate_Thermodynamics(CalculatedThermo, Temperature, Thermodynamics);
		Calculate_Rate_Constant(Kf, Kr, Temperature,LocalReactionParameters, CalculatedThermo, LocalSpeciesLossAll, Local_Delta_N);

		for(j=0;j<(int)Kr.size();j++)
		{
			Kr[j] = log(Kr[j]);
		}

		allkreverse.push_back(Kr);

		OneTempSensitivityMatrix[0] = 1;
		OneTempSensitivityMatrix[1] = log(Temperature);
		OneTempSensitivityMatrix[2] = 1/Temperature;

		SensitivityMatrix.push_back(OneTempSensitivityMatrix);
	}


	//cout << "Checkpoint 2\n";

	/*
	 * At this point I now have a collection of kr for the temperature range 0 to 1000K in steps of 20K
	 * as well as the sensitivity matrix
	 */

	vector< vector< double > > SensitivityMatrixTranspose;	// called X in the paper
	vector< double > RowSensitivityMatrixTranspose;
	RowSensitivityMatrixTranspose.resize(SensitivityMatrix.size());

	// stupid way to give the matrix 3 rows
	SensitivityMatrixTranspose.push_back(RowSensitivityMatrixTranspose);
	SensitivityMatrixTranspose.push_back(RowSensitivityMatrixTranspose);
	SensitivityMatrixTranspose.push_back(RowSensitivityMatrixTranspose);


	//printf("Transpose: \n");
	for(i=0;i<3;i++)
	{
		for(j=0;j<(int)SensitivityMatrix.size();j++)
		{
			SensitivityMatrixTranspose[i][j] = SensitivityMatrix[j][i];
			//printf("%.3e ",SensitivityMatrixTranspose[i][j]);
		}
		//printf("\n");
	}
	//printf("\n");

	// Now we need to calculate our betas....
	//cout << "Checkpoint 5 \n";
	// First on the list, SensitivityMatrixTranspose times SensitivityMatrix

	vector< vector< double > > SMtxSM; // Sensitivity Matrix transpose times (x) Sensitivity Matrix
	vector< double > SMt;  // Sensitivity Matrix transpose times
	SMt.resize(3);

	// result is a 3x3 matrix
	SMtxSM.push_back(SMt);
	SMtxSM.push_back(SMt);
	SMtxSM.push_back(SMt);


	//printf("XtX: \n");
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			for(k=0;k<(int)SensitivityMatrix.size();k++)
			{
				SMtxSM[i][j] = SMtxSM[i][j] + SensitivityMatrixTranspose[i][k] *  SensitivityMatrix[k][j];
			}
			//printf("%.3e ",SMtxSM[i][j]);
		}
		//printf("\n");
	}
	//printf("\n");


	// Now we need the inverse of the matrix...
	// Maple gives the ouput for a matrix
	/*
	 * a , b , c
	 * d , e , f
	 * g , h , k
	 *
	 * divisor = cdh - cge - bdk + bgf + aek - ahf
	 *
	 * So matrix is, every entry divided by divisor, rest
	 *
	 * ek - hf , -dk + gf , dh - ge
	 * ch - bk , -cg + ak , bg - ah
	 * - ce + bf , cd - af , -bd + ae
	 */

	double determinant =
			SMtxSM[0][2]*SMtxSM[1][0]*SMtxSM[2][1] -
			SMtxSM[0][2]*SMtxSM[2][0]*SMtxSM[1][1] -
			SMtxSM[0][1]*SMtxSM[1][0]*SMtxSM[2][2] +
			SMtxSM[0][1]*SMtxSM[2][0]*SMtxSM[1][2] +
			SMtxSM[0][0]*SMtxSM[1][1]*SMtxSM[2][2] -
			SMtxSM[0][0]*SMtxSM[2][1]*SMtxSM[1][2];

	vector< vector< double > > SMtxSMInv; // Sensitivity Matrix transpose times (x) Sensitivity Matrix
	vector< double > SMtInv;  // Sensitivity Matrix transpose times
	SMtInv.resize(3);

	// Stupid way of creating an inverse matrix... but it is only 3*3
	SMtxSMInv.push_back(SMtInv);
	SMtxSMInv.push_back(SMtInv);
	SMtxSMInv.push_back(SMtInv);

	SMtxSMInv[0][0] = SMtxSM[1][1]*SMtxSM[2][2] - SMtxSM[2][1]*SMtxSM[1][2];
	SMtxSMInv[0][1] = - SMtxSM[1][0]*SMtxSM[2][2] + SMtxSM[2][0]*SMtxSM[1][2];
	SMtxSMInv[0][2] = SMtxSM[1][0]*SMtxSM[2][1] - SMtxSM[2][0]*SMtxSM[1][1];

	SMtxSMInv[1][0] = SMtxSM[0][2]*SMtxSM[2][1] - SMtxSM[0][1]*SMtxSM[2][2];
	SMtxSMInv[1][1] = - SMtxSM[0][2]*SMtxSM[2][0] + SMtxSM[0][0]*SMtxSM[2][2];
	SMtxSMInv[1][2] = SMtxSM[0][1]*SMtxSM[2][0] - SMtxSM[0][0]*SMtxSM[2][1];

	SMtxSMInv[2][0] = - SMtxSM[0][2]*SMtxSM[1][1] + SMtxSM[0][1]*SMtxSM[1][2];
	SMtxSMInv[2][1] = SMtxSM[0][2]*SMtxSM[1][0] - SMtxSM[0][0]*SMtxSM[1][2];
	SMtxSMInv[2][2] = - SMtxSM[0][1]*SMtxSM[1][0] + SMtxSM[0][0]*SMtxSM[1][1];



	// Don't forget dividing by the determinant
	///printf("The determinant: %.3e \n\n", determinant);

	///printf("Inverse: \n");
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			SMtxSMInv[i][j] = SMtxSMInv[i][j] / determinant;
			//printf("%.3e ",SMtxSMInv[i][j]);
		}
		//printf("\n");
	}
	//printf("\n");


	// now transpose times the logarithms of k_r to get a 3 entry vector...


	vector< vector< double > > InvMxSM;
	vector< double > xSM;
	xSM.resize(SensitivityMatrix.size());

	InvMxSM.push_back(xSM);
	InvMxSM.push_back(xSM);
	InvMxSM.push_back(xSM);

	// matrix mult: row x column

	//printf("Inverse x Xt: \n");
	for(i=0;i<3;i++)
	{
		for(j=0;j<(int)SensitivityMatrix.size();j++)
		{
			for(k=0;k<3;k++)
			{
				InvMxSM[i][j] = InvMxSM[i][j] + SMtxSMInv[i][k] *  SensitivityMatrixTranspose[k][j];
			}
			//printf("%.3e ",InvMxSM[i][j]);
		}
		//printf("\n");
	}
	//printf("\n");


	// get the number of species
	//int Number_Species = Reactions[0][0].size();


	//vector< vector< double > >
	SingleReactionData SingleReaction;

	// assemble the arrays for writing a new reactions array
	vector< double > ReactantData; // Reactant Information
	ReactantData.resize(Number_Species);
	vector< double > ProductData; // Product Information
	ProductData.resize(Number_Species);
	vector< double > ReactionParameters;
	ReactionParameters.resize(4);


	for(i=0;i<Number_Reactions;i++)
	{
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
				ReactionParamaters[0],ReactionParamaters[1],
				ReactionParamaters[2],ReactionParamaters[3]); // Works :) */

		Irreversible_Scheme.push_back(SingleReaction);

		// Calculate the parameters
		vector< double > beta;
		beta.resize(3);

		// check if the reaction was reversible - if yes, calculate irreversible form
		if(Reactions[i].Reversible)
		{

			for(j=0;j<3;j++)
			{
				for(k=0;k<(int)SensitivityMatrix.size();k++)
				{
					beta[j] = beta[j] + InvMxSM[j][k] * allkreverse[k][i];
				}
			}

			//printf(" %.3e %.3e %.3e \n",beta[0],beta[1],beta[2]);

			// Write out reverse reaction:
			// And now the reverse Reaction - Products and Reactants reversed
			for(j=0;j<Number_Species;j++)
			{
				ReactantData[j] = - Reactions[i].Products[j];
				ProductData[j] =  - Reactions[i].Reactants[j];
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
					ReactionParamaters[0],ReactionParamaters[1],
					ReactionParamaters[2],ReactionParamaters[3]); // Works :) */

			Irreversible_Scheme.push_back(SingleReaction);

		}

	}


	return Irreversible_Scheme;
}


