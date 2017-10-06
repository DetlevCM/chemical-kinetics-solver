/*
 * Integrate_Odepack.h
 *
 *  Created on: 18 Sep 2017
 *      Author: detlevcm
 */

#ifndef HEADERS_INTEGRATE_ODEPACK_H_
#define HEADERS_INTEGRATE_ODEPACK_H_


extern "C" {

/* Intel example...
void dodesol_mk52lfa(
		int*,
		int*,
		double*,
		double*,
		double*,
		void*,
		void*,
		double*,
		double*,
		double*,
		double*,
		double*,
		int*,
		int*
);
 */

// Input type choice/conversion as per instructions in opkdmain.f ...
// CALL DLSODA(FEX,NEQ,Y,T,TOUT,ITOL,RTOL,ATOL,ITASK,ISTATE,
//C    1     IOPT,RWORK,LRW,IWORK,LIW,JDUM,JT)
//

void dlsoda_(
		//C A. First provide a subroutine of the form:
		//C               SUBROUTINE F (NEQ, T, Y, YDOT)
		//C               DOUBLE PRECISION T, Y(*), YDOT(*)
		//C which supplies the vector function f by loading YDOT(i) with f(i).
		void*, // routine that supplied f(x)
		int*, // NEQ
		double*, // Y
		double*, //T
		double*, //TOUT
		// C ITOL   = 1 or 2 according as ATOL (below) is a scalar or array.
		int*, //ITOL
		double*, //RTOL
		double*, //ATOL
		int*, //ITASK
		int*, //ISTATE
		int*, //IOPT - zero without optional inputs
		// C RWORK  = real work array of length at least:
		//C             22 + NEQ * MAX(16, NEQ + 9) + 3*NG.
		double*, //RWORK
		int*, //LRW
		// C IWORK  = integer work array of length at least  20 + NEQ.
		int*, //IWORK
		int*, //LIW
		void*, //,JDUM - void with Jacobian, use dummy name if none
		int* //JT - apparently set to 2
);

}//*/


#endif /* HEADERS_INTEGRATE_ODEPACK_H_ */
