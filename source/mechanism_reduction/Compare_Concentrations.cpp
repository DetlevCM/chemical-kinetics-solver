/*
 * Reduce_Reactions.cpp
 *
 *  Created on: 28.12.2012
 *      Author: DetlevCM
 */

#include <MyHeaders.h>

vector< double > Compare_Concentrations(
		const vector< double >& OldTimePoints,
		const vector< vector< double > >& OldConcentrations,
		const vector< double >& NewTimePoints,
		const vector< vector< double > >& NewConcentrations
)
{


	/*
	 * In this function we wish to evaluate the accuracy of the reduced scheme. This can be achieved
	 * by for example comparing the concentrations of all species over time and keeping track of the
	 * difference/error while using the original scheme as a reference.
	 * Ideally we want the difference to be expressed as something useful - I shall use a percentage
	 * scale from 0 to 100, hence the output using a double.
	 */

	// these should be identical, but just write the code in a ways that it can deal with both.
	/// In moles per litre, a concentration of 1.66 e-12 or less is irrelevant

	vector< double > EvaluationResult(2);

	int Number_OldTimePoints = OldConcentrations.size();
	int Number_NewTimePoints = NewConcentrations.size();

	int Number_Species = OldConcentrations[0].size(); // Species count MUST be the same in both schemes

	int i, j, k;


	if(Number_OldTimePoints == Number_NewTimePoints) // this is the default in the current code
	{
		int large = 0;
		double Sum;
		double Ratio;
		vector< double > Average(Number_Species);

		for(i=0;i<Number_Species-1;i++) // -1 exclude temperature
		{
			Sum = 0;
			k = 0;
			for(j=0;j<Number_OldTimePoints;j++) // avoid first point, has zero concentrations, plus summing is the wrong way round...
			{
				if(OldConcentrations[j][i] > 1.e-15) // avoid division by zero and ignore irrelevant species
				{
				Ratio = 1 - NewConcentrations[j][i] / OldConcentrations[j][i];

				Sum = Sum + Ratio;
				k = k + 1;
				}

				// alternative method idea: difference between old & new divided by average
			}

			if(k != 0)
			{
				Average[i] = Sum/k;
			}
			else
			{
				Average[i] = 0;
			}

		}

		Sum = 0;


		for(i=0;i<Number_Species-1;i++) // -1 exclude temperature
		{
			if(abs(Average[i]) < 0.5)
			{
				Sum = Sum + Average[i];
			}
			else
			{
				large = large + 1;
			}

		}

		EvaluationResult[0] = Sum / (Number_Species-1-large);  // -1 exclude temperature, -large exclude excessive deviation
		EvaluationResult[1] = large;
	}



	/*

	vector< double > OldCheckPoints;
	vector< double > NewCheckPoints;

	for(i=0;i<Number_Species;i++)
	{
		double gradient_old = 0;
		double gradient_new = 0;

		for(j=1;j<Number_OldTimePoints-1;j++) // avoid first point, has zero concentrations, plus summing is the wrong way round...
		{
			gradient_old = OldConcentrations[j][i]-OldConcentrations[j-1][i];
			gradient_new = OldConcentrations[j+1][i]-OldConcentrations[j][i];

			// positive gradient
			if(gradient_new > 1.e-12 && gradient_new < gradient_old) // stopped getting steeper concentration less than 10^-12 is meaningless
			{
				OldCheckPoints.push_back(OldTimePoints[j]);
			}
			// negative gradient
			if(gradient_new < -1.e-12 && gradient_new > gradient_old) // stopped getting steeper concentration less than 10^-12 is meaningless
			{
				OldCheckPoints.push_back(OldTimePoints[j]);
			}
		}

		for(j=1;j<Number_NewTimePoints-1;j++) // avoid first point, has zero concentrations, plus summing is the wrong way round...
		{
			gradient_old = NewConcentrations[j][i]-NewConcentrations[j-1][i];
			gradient_new = NewConcentrations[j+1][i]-NewConcentrations[j][i];

			// positive gradient
			if(gradient_new > 1.e-12 && gradient_new < gradient_old) // stopped getting steeper concentration less than 10^-12 is meaningless
			{
				NewCheckPoints.push_back(NewTimePoints[j]);
			}
			// negative gradient
			if(gradient_new < -1.e-12 && gradient_new > gradient_old) // stopped getting steeper concentration less than 10^-12 is meaningless
			{
				NewCheckPoints.push_back(NewTimePoints[j]);
			}
		}

		// OldTimePoints & NewTimePoints


		printf("Species %u: 	", i);
		for(j=0;j<(int)OldCheckPoints.size();j++)
		{
		printf("%u 	",OldCheckPoints[j]);
		}
		printf("\n");
		OldCheckPoints.clear();

		printf("Species %u: 	", i);
		for(j=0;j<(int)NewCheckPoints.size();j++)
		{
			printf("%u 	",NewCheckPoints[j]);
		}
		printf("\n");
		NewCheckPoints.clear();
	}
//*/




	//}

	/*
	// now we need to compare the difference, why not an average?
	//double PercentageSum = 0;
	vector< double > EvaluationResult(2);
	vector< double > Evaluation(Number_Species);
	int large = 0;
	for(i=0;i<Number_Species;i++)
	{
		Evaluation[i] = 0;
		if(OldAverage[i]!=0) // filter out species that exist in scheme but are always zero... - or average to zero
		{
			Evaluation[i] = (OldAverage[i]-NewAverage[i])/OldAverage[i];
			//PercentageSum = PercentageSum + NewAverage[i]/OldAverage[i];
		}
		printf("For Species %u Value: %.3e \n",i,Evaluation[i]);

		if(abs(Evaluation[i]) <= 10.0)
		{
			EvaluationResult[0] = EvaluationResult[0] + abs(Evaluation[i]);
		}
		else
		{
			//if(OldAverage[i]>1.e-15) // only register discrepancies for relevant species
			//{
			large = large + 1;
			//}
		}
	}
	//*/


	//EvaluationResult[0] = EvaluationResult[0] / (Number_Species-large);
	//EvaluationResult = PercentageSum / (Number_Species+1); // dont't forget temperature.... -> so +1



	//EvaluationResult[0] = 1;
	//EvaluationResult[1] = 1;

	return EvaluationResult;
}
