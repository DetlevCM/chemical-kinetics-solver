/*
 * Third_Body_Maths.h
 *
 *  Created on: 20 Jun 2021
 *      Author: detlevcm
 */

#ifndef SOURCE_RUN_INTEGRATOR_SOLVER_CALCULATIONS_THIRD_BODY_MATHS_H_
#define SOURCE_RUN_INTEGRATOR_SOLVER_CALCULATIONS_THIRD_BODY_MATHS_H_

double Calculate_no_LOW_Troe(
		const SingleReactionData& ReactionData,
		const vector<double>& Concentration,
		double T, double third_body);

double Calculate_Lindeman_Hinshelwood_Low(
		const SingleReactionData& ReactionData,
		const vector<double>& Concentration,
		double T,double a0, double n0, double e0, double third_body);

double Calculate_Lindeman_Hinshelwood_Low_Troe(
		const SingleReactionData& ReactionData,
		const vector<double>& Concentration,
		double T, // current temperature
		double a0, double n0, double e0, // third body parameters
		double third_body // sum of third bodies, but which units, original molecules per cm3
);

#endif /* SOURCE_RUN_INTEGRATOR_SOLVER_CALCULATIONS_THIRD_BODY_MATHS_H_ */
