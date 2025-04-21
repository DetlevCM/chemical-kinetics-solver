/*
 * lib_odepack.hpp
 *
 *  Created on: 18 Sep 2017
 *      Author: detlevcm
 */

#ifndef HEADERS_INTEGRATE_ODEPACK_H_
#define HEADERS_INTEGRATE_ODEPACK_H_


extern "C" {

void dlsode_(
		void*,   //F,
		int*,    //NEQ,
		double*, //Y,
		double*, //T,
		double*, //TOUT,
		int*,    //ITOL
		double*, //RTOL
		double*, //ATOL
		int*,    //ITASK
		int*,    //ISTATE
		int*,    //IOPT - zero without optional inputs
		double*, //RWORK
		int*,    //LRW
		int*,    //IWORK =  20 for MF = 10 OR  20 + NEQ  for MF = 21, 22, 24, or 25.
		int*,    //LIW
		void* ,  //JAC,
		int*     //MF
);

void dlsoda_(
		void*,   // routine that supplied f(x)
		int*,    // NEQ
		double*, // Y
		double*, //T
		double*, //TOUT
		int*,    //ITOL = 1 or 2 according as ATOL (below) is a scalar or array.
		double*, //RTOL
		double*, //ATOL
		int*,    //ITASK
		int*,    //ISTATE
		int*,    //IOPT - zero without optional inputs
		double*, //RWORK = real work array of length at least:  22 + NEQ * MAX(16, NEQ + 9) + 3*NG.
		int*,    //LRW
		int*,    //IWORK = integer work array of length at least  20 + NEQ.
		int*,    //LIW
		void*,   //,JDUM - void with Jacobian, use dummy name if none
		int*     //JT - apparently set to 2
);

}


#endif /* HEADERS_INTEGRATE_ODEPACK_H_ */
