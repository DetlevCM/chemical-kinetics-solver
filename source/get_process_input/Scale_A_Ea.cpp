/*
 * Scale_A_Ea.cpp
 *
 *  Created on: 21 Feb 2018
 *      Author: detlev
 */

#include <Headers.hpp>



double Scale_A(
		double A_read_in,
		vector<double> ReactantData,
		int scaling_type
		)
{
	if(scaling_type == 0) // A is molecules / cm^3
	{
		// for moles / dm^3
		double order = 0;
		for(int i=0;i<(int)ReactantData.size();i++)
		{
			order = order + ReactantData[i];
		}
		order = fabs(order) - 1; // make sure it is positive

		// 6.0221e+23 <- molecules per mol
		double scale = (6.0221e+23); // convert to molecules / cm^(-3)
		scale = scale * 1000; // convert from cm^3 to dm^3
		A_read_in = A_read_in * pow(scale,(order));

		return A_read_in; // and convert to molecules per liter (dm^3)
	}
	else if(scaling_type == 1) // A is in moles / cm^3
	{

		double order = 0;
		for(int i=0;i<(int)ReactantData.size();i++)
		{
			order = order + ReactantData[i];
		}
		order = fabs(order) - 1; // make sure it is positive

		double scale = 1 * 1000; // convert from cm^3 to dm^3

		A_read_in = A_read_in * pow(scale,(order));

		return A_read_in; // and convert to molecules per liter (dm^3)

	}
	else // return unmodified parameter if we do not know what to do
	{
		return A_read_in;
	}
}

// Ea needs to be in Kelvin for the remainder of the calculations
double Scale_Ea(
		double Ea_read_in,
		int scaling_type
		)
{
	if(scaling_type == 1) // Ea is in kcal/mol
	{
		return Ea_read_in/1.98709e-3;
	}

	else if(scaling_type== 2) // Ea is in cal/mol
	{
		return Ea_read_in/1.98709;
	}

	else if(scaling_type == 3) // Ea is in kJ/mol
	{
		return Ea_read_in/8.3144621e-3;
	}

	else if(scaling_type == 4) // Ea is in J/mol
	{
		return Ea_read_in/8.3144621;
	}
	//if(scaling_type == 0) // Ea is in Kelvin
	else // do nothing if we don't identify the scaling, return value as read in
	{
		return Ea_read_in;
	}

}

