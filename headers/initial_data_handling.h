/*
 * initial_data_handling.h
 *
 *  Created on: 19 Mar 2016
 *      Author: DetlevCM
 */

#ifndef HEADERS_INITIAL_DATA_HANDLING_H_
#define HEADERS_INITIAL_DATA_HANDLING_H_


void Handle_InitialConditions(InitParam& , vector<string> );

void Handle_Species(InitParam&  , vector< InitSpecies >& , vector<string> , vector< string > );

void Handle_SolverParameters(InitParam& , vector<string> );

void Handle_Analysis(InitParam& , vector< string > , vector< string > );

void Handle_PressureVessel(InitParam& , vector<string> , vector< string >);

void Handle_MechanismReduction(InitParam& , vector<string> );


#endif /* HEADERS_INITIAL_DATA_HANDLING_H_ */
