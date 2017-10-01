/*
 * Integrate_Odepack.h
 *
 *  Created on: 18 Sep 2017
 *      Author: detlevcm
 */

//*** Define Intel ODE Solver ***//
#ifndef _INTEL_ODE_H_
#define _INTEL_ODE_H_
//*
extern "C" {

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

void dodesol_rkm9mka(
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

void dodesol_mk52lfn(
		int*,
		int*,
		double*,
		double*,
		double*,
		void*,
		double*,
		double*,
		double*,
		double*,
		double*,
		int*,
		int*
);

void dodesol_rkm9mkn(
		int*,
		int*,
		double*,
		double*,
		double*,
		void*,
		double*,
		double*,
		double*,
		double*,
		double*,
		int*,
		int*
);

}//*/
#endif /* _INTEL_ODE_H_ */

