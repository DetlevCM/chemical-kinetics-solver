/*Mechanism_Reduction_Output
 * initial_data_handling.h
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#ifndef HEADERS_HANDLE_INITIAL_CONDITIONS_H_
#define HEADERS_HANDLE_INITIAL_CONDITIONS_H_


void Handle_Initial_Conditions(InitParam& , vector<string> , solver_type);

vector< InitSpecies > Handle_Species(InitParam&  , vector<string> , vector< string > );

void Handle_Solver_Parameters(InitParam& , vector<string> );

void Handle_Analysis(InitParam& , vector< string > , vector< string > );

void Handle_Pressure_Vessel(InitParam& , vector<string> , vector< string >);

void Handle_PetroOxy(InitParam& , vector<string> , vector< string >);

mechanism_reduction Handle_Mechanism_Reduction(vector<string> );


#endif /* HEADERS_HANDLE_INITIAL_CONDITIONS_H_ */
