/*Mechanism_Reduction_Output
 * initial_data_handling.h
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#ifndef HEADERS_HANDLE_INITIAL_CONDITIONS_HPP_
#define HEADERS_HANDLE_INITIAL_CONDITIONS_HPP_


void Handle_Initial_Conditions(Initial_Data& , vector<string> , solver_type);

vector< InitSpecies > Handle_Species(Initial_Data&  , vector<string> , vector< string > );

void Handle_Solver_Parameters(Initial_Data& , vector<string> );

void Handle_Analysis(Initial_Data& , vector< string > , vector< string > );

void Handle_Pressure_Vessel(Initial_Data& , vector<string>);

void Handle_PetroOxy(Initial_Data& , vector<string> , vector< string >);

mechanism_reduction Handle_Mechanism_Reduction(vector<string> );


#endif /* HEADERS_HANDLE_INITIAL_CONDITIONS_HPP_ */
