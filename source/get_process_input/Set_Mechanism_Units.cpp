/*
 * Set_Mechanism_Units.cpp
 *
 *  Created on: 21 Feb 2018
 *      Author: detlev
 */

#include "../headers/Headers.hpp"

vector<int> Set_Mechanism_Units(string test_string) // Identify the units
{
	vector<int> Mechanism_Units(2);
	// default for A:  mol/cm^3 (I think...)
	// default for Ea: cal/mol
	// So set default:
	Mechanism_Units[0] = 2;
	Mechanism_Units[1] = 2;

	// then explicitly react to possible options

	if(Test_If_Word_Found(test_string, "KEEPA")) // special keep word to retain unit of A
	{
		Mechanism_Units[0] = 0;
	}
	// assuming moles and mol is the same there, keep both for clarity of permitted terms
	if(Test_If_Word_Found(test_string, " MOLE") || Test_If_Word_Found(test_string, " MOLES")
			||
			Test_If_Word_Found(test_string, "\tMOLE") || Test_If_Word_Found(test_string, "\tMOLES"))
	{
		Mechanism_Units[0] = 2; // A in moles per cm^3
	}
	// need to test for the longest keyword last
	if(Test_If_Word_Found(test_string, "MOLECULES")) // enable case independent test
	{
		Mechanism_Units[0] = 1; // A in molecules cm^(-3)
	}

	// note space before KCAL or CAL to avoid overwriting of units

	if(Test_If_Word_Found(test_string, "KELVINS"))
	{
		Mechanism_Units[1] = 0; // Ea in Kelvins, great :)
	}

	if(Test_If_Word_Found(test_string, " KCAL/MOL") || Test_If_Word_Found(test_string, "\tKCAL/MOL"))
	{
		Mechanism_Units[1] = 1; // Ea in kcal/mol
	}

	if(Test_If_Word_Found(test_string, " CAL/MOL") || Test_If_Word_Found(test_string, "\tCAL/MOL"))
	{
		Mechanism_Units[1] = 2; // Ea in cal/mol
	}

	if(Test_If_Word_Found(test_string, " kJ/MOL") || Test_If_Word_Found(test_string, "\tkJ/MOL"))
	{
		Mechanism_Units[1] = 3; // Ea in kJ/mol
	}

	if(Test_If_Word_Found(test_string, " J/MOL") || Test_If_Word_Found(test_string, "\tJ/MOL"))
	{
		Mechanism_Units[1] = 4; // Ea in J/mol
	}

	//cout << "Mechanism Units are " << Mechanism_Units[0] << " " << Mechanism_Units[1] << "\n";
	return Mechanism_Units;
}


