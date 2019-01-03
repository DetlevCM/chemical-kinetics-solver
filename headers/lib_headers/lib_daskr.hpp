/*
 * lib_daskr.hpp
 *
 *  Created on: 03/01/2019
 *      Author: detlevcm
 */

#ifndef HEADERS_INTEGRATE_DASKR_H_
#define HEADERS_INTEGRATE_DASKR_H_


extern "C" {

/*
C      IMPLICIT DOUBLE PRECISION(A-H,O-Z)
C      INTEGER NEQ, INFO(N), IDID, LRW, LIW, IWORK(LIW), IPAR(*)
C      DOUBLE PRECISION T, Y(*), YPRIME(*), TOUT, RTOL(*), ATOL(*),
C         RWORK(LRW), RPAR(*)
C      EXTERNAL RES, JAC, PSOL, RT
C
C      CALL DDASKR (RES, NEQ, T, Y, YPRIME, TOUT, INFO, RTOL, ATOL,
C     *             IDID, RWORK, LRW, IWORK, LIW, RPAR, IPAR, JAC, PSOL,
C     *             RT, NRT, JROOT)
C
C  Quantities which may be altered by the code are:
C     T, Y(*), YPRIME(*), INFO(1), RTOL, ATOL, IDID, RWORK(*), IWORK(*)
 */

void ddaskr_(
		void* ,   //RES,
		int* ,    //NEQ,
		double* , //T,
		double* , //Y,
		double* , //YPRIME,
		double* , //TOUT,
		int* ,    //INFO,
		double* , //RTOL,
		double* , //ATOL,
		int* ,    //IDID,
		double* , //RWORK,
		int* ,    //LRW,
		int* ,    //IWORK,
		int* ,    //LIW,
		double* , //RPAR,
		int* ,    //IPAR,
		void* ,   //JAC,
		void* ,   //PSOL,
		void* ,   //RT,
		int* ,    //NRT,
		int*      //JROOT
);

}


#endif /* HEADERS_INTEGRATE_DASKR_H_ */
