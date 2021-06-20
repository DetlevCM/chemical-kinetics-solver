/*
 * Third_Body_Maths.h
 *
 *  Created on: 20 Jun 2021
 *      Author: detlevcm
 */

#ifndef SOURCE_RUN_INTEGRATOR_SOLVER_CALCULATIONS_THIRD_BODY_MATHS_H_
#define SOURCE_RUN_INTEGRATOR_SOLVER_CALCULATIONS_THIRD_BODY_MATHS_H_

double Calculate_no_LOW_Troe(double a1, double n1, double e1, double T, double third_body);

double Calculate_Lindeman_Hinshelwood_Low(double a1, double n1, double e1, double T,double a0, double n0, double e0, double third_body, size_t sri_flag);

double Calculate_Lindeman_Hinshelwood_Low_Troe(
		double T, // current temperature
		double a1, double n1, double e1, //
		double a0, double n0, double e0, // third body parameters
		double T1, double T2, double T3, // temperature paraeters for third body
		double third_body // sum of third bodies, but which units, original molecules per cm3
);

#endif /* SOURCE_RUN_INTEGRATOR_SOLVER_CALCULATIONS_THIRD_BODY_MATHS_H_ */
