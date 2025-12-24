/*
 * Jacobian.h
 *
 *  Created on: 20 Oct 2017
 *      Author: detlevcm
 */

#ifndef HEADERS_JACOBIAN_H_
#define HEADERS_JACOBIAN_H_


// Jacobian Matrix for plain Arrhenius expressions only

void Prepare_Jacobian_Matrix(
		vector < JacobianData > & JacobianColumnWise,
		const vector< SingleReactionData > & Reactions
);

void Jacobian_Matrix_Intel(int*, double*, double*, double*);

void Jacobian_Matrix_Odepack_LSODA(int*, double*, double*, double*, double*, double*, int*);


#endif /* HEADERS_JACOBIAN_H_ */
