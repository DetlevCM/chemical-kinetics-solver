/*
 * Jacobian.cpp
 *
 *  Created on: 06.07.2015
 *      Author: DetlevCM
 */

#include <MyHeaders.h>


void Jacobian_Matrix_Intel(int*n,double*t,double*y,double*a) {

	// n -> number of species
	// t time
	// y concentration
	// a Jacobian in column wise order

	using namespace Jacobian_ODE_RHS;
	using namespace Jacobian;

	//using namespace ODESolverConstant;
	//using namespace ODESolverVariable;


	int i,j;

	// enable force stability?
	/*
	for (i = 0; i <= Number_Species; i++)
	{
		if(y[i]<0){
			//if(y[i]<1.e-24){
			Concentration[i] = 0;
		}
		else
		{
			Concentration[i] = y[i];
		}
	}//*/

	for (i = 0; i <= Number_Species; i++)
	{
		Concentration[i] = y[i];
	}



	// provides me a fresh array every time :) - ideal
	vector< double > JacobeanColumnWise((Number_Species+1)*(Number_Species+1));



	Calculate_Thermodynamics(CalculatedThermo, Concentration[Number_Species], Thermodynamics);
	Calculate_Rate_Constant(Kf, Kr, Concentration[Number_Species],ReactionParameters, CalculatedThermo, SpeciesLossAll, Delta_N);

	for(i=0;i<(int) JacobianMatrix.size();i++)
	{
		double temp;


		if(JacobianMatrix[i].IsForward) // Forward
		{
			if(JacobianMatrix[i].IsProduction) // species are gained
			{
				temp = Kf[JacobianMatrix[i].ReactionID]*JacobianMatrix[i].coefficient;
			}
			if(!JacobianMatrix[i].IsProduction) // species are lost
			{
				temp = -Kf[JacobianMatrix[i].ReactionID]*JacobianMatrix[i].coefficient;
			}
		}

		if(!JacobianMatrix[i].IsForward) // Reverse
		{
			if(JacobianMatrix[i].IsProduction) // species are gained
			{
				temp = Kr[JacobianMatrix[i].ReactionID]*JacobianMatrix[i].coefficient;
			}
			if(!JacobianMatrix[i].IsProduction) // species are lost
			{
				temp = -Kr[JacobianMatrix[i].ReactionID]*JacobianMatrix[i].coefficient;
			}
		}


		for(j=0;j<(int) JacobianMatrix[i].Species.size();j++)
		{
			if(JacobianMatrix[i].Species[j].power != 0) // power 0 = *1
			{
				if(JacobianMatrix[i].Species[j].power == 1) // power 1 is simple multiplication
				{
					temp = temp * Concentration[JacobianMatrix[i].Species[j].SpeciesID];
				}
				else
				{
					temp = temp *
							pow(Concentration[JacobianMatrix[i].Species[j].SpeciesID],
									JacobianMatrix[i].Species[j].power);
				}
			}
		}


		JacobeanColumnWise[JacobianMatrix[i].ColumnWiseArrayPosition] =
				JacobeanColumnWise[JacobianMatrix[i].ColumnWiseArrayPosition] + temp;
	}

	for (i = 0; i <= (int) JacobeanColumnWise.size() ; i++)
	{
		a[i] = JacobeanColumnWise[i];
	}

	/*
	 * Debug Output
	 */

	/*
	cout << "\n" << "\n";
	int temp_output = (int) sqrt(JacobeanColumnWise.size());
	for(i=0;i< temp_output;i++)
	{
		for(j=0;j<(int) JacobeanColumnWise.size();j++)
		{
			if(j % temp_output == i)
			{
				cout << a[j] << " ";
				//cout << JacobeanColumnWise[j] << " ";
				//cout << j % temp_output << " ";
			}
		}
		cout << "\n";
	}
	cout << "\n" << "\n";
	//*/
	//cout << "check\n";
}


