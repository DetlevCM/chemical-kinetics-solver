/*Mechanism_Reduction_Output
 * initial_data_handling.h
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#ifndef HEADERS_HANDLE_INITIAL_CONDITIONS_HPP_
#define HEADERS_HANDLE_INITIAL_CONDITIONS_HPP_


void Handle_Initial_Conditions(
		Initial_Data& InitialParameters,
		vector<string> Input,
		solver_type
		Global_Solver_Settings
		);

vector< InitSpecies > Handle_Species(		Initial_Data&  InitialParameters,
		vector<string> Input,
		vector< string > Species
		);

void Handle_Solver_Parameters(
		Initial_Data& InitialParameters,
		vector<string> Input
		);

void Handle_Analysis(
		Initial_Data& InitialParameters,
		vector<string> Input,
		vector<string> Species
		);

void Handle_Pressure_Vessel(
		Initial_Data& InitialParameters,
		vector<string> Input
		);

void Handle_PetroOxy(
		Initial_Data& InitialParameters,
		vector<string> Input,
		vector< string > Species
		);

mechanism_reduction Handle_Mechanism_Reduction(
		vector<string> Input
		);


#endif /* HEADERS_HANDLE_INITIAL_CONDITIONS_HPP_ */
