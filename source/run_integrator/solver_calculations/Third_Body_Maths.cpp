/*
 * Third_Body_Maths.cpp
 *
 *  Created on: 20 Jun 2021
 *      Author: detlevcm
 */

#include "../headers/Headers.hpp"


// for all species, sum:
//    third_body += y; /* calculate [M] */

// a1, n1, e1 are the standard Arrhenius parameters
// a0, n0, e0 are specific parameters for third body reactions
// inv_T1 and inv_T3 are specific third body parameters

double Calculate_no_LOW_Troe(double a1, double n1, double e1, double T, double third_body)
{
	double inv_T = 1.0/T;
	double k; // this is the normal forward K

	double mod_third_body;

	// not applicable to a special type with water

	/* no LOW or TROE terms */
	if (!collision_efficiency_flag) { /* no collision efficiency corrections */
		k = third_body*a1*exp(-e1*inv_T);
		if (n1 != 0.0)
			k *= pow(T, n1);
	}
	else { /* collision efficiency corrections required */
		mod_third_body = third_body;
		correction = coll_eff;
		while (correction)
		{
			mod_third_body += (correction->value)*y[correction->index];
			correction = correction->next;
		}
		k = mod_third_body*a1*exp(-e1*inv_T);
		if (n1 != 0.0)
			k *= pow(T, n1);
	}

	return k;
}

double Calculate_Lindeman_Hinshelwood_Low(double a1, double n1, double e1, double T,double a0, double n0, double e0, double third_body, size_t sri_flag)
{
	double inv_T = 1.0/T;
	double kinf;
	double kzero;
	double F;
	double k; // the result, Kf

	double mod_third_body;

	/* LOW parameters but not TROE, ie simple Lindemann-Hinshelwood, unless it's SRI type */
	kinf = a1*exp(-e1*inv_T);
	if (n1 != 0.0)
		kinf *= pow(T, n1);
	kzero = a0*exp(-e0*inv_T);
	if (n0 != 0.0)
		kzero *= pow(T, n0);


	// SRI flag which provides 3 calculation methods for three distinct cases
	// Special Water reactions that use the specis concentration for H20, N2, H2, Ar, CO2, CH4, C2H6, O2
	// calculations without collision efficiency corrections that use a "third body" parameter
	// calculations using a corrected collision efficiency

	// needs the special treatment for the species concentration as an option for mod_third_body
	mod_third_body = third_body;
	if (collision_efficiency_flag)  { /* collision efficiency corrections */
		mod_third_body = third_body;
		correction = coll_eff;
		while (correction)
		{
			mod_third_body += (correction->value)*y[correction->index];
			correction = correction->next;
		}
	}
	switch (sri_flag) {
	case 0: /* its Lindeman-Hinshelwood */
		k=kzero*kinf*mod_third_body/(kzero*mod_third_body+kinf);
		break;
	case 1: /* its simple SRI */
		F = T*pow((sri_a*exp(-sri_b/T)+exp(-T/sri_c)), ((1/(1+pow((log10(kzero*mod_third_body/kinf)), 2)))));
		k = kzero*kinf*mod_third_body*F/(kzero*mod_third_body+kinf);
		break;
	case 2: /* It's complex SRI */
		F = sri_d*pow(T, sri_e)*pow((sri_a*exp(-sri_b/T)+exp(-T/sri_c)), ((1/(1+pow((log10(kzero*mod_third_body/kinf)), 2)))));
		k = kzero*kinf*mod_third_body*F/(kzero*mod_third_body+kinf);
		break;
	}

	return k;
}

double Calculate_Lindeman_Hinshelwood_Low_Troe(
		double T, // current temperature
		double a1, double n1, double e1, //
		double a0, double n0, double e0, // third body parameters
		double T1, double T2, double T3, // temperature paraeters for third body
		double third_body // sum of third bodies, but which units, original molecules per cm3
)
{
	double inv_T = 1.0/T;
	double kinf;
	double kzero;
	double kappa;
	double Fc,F;
	double k; // the result, Kf

	double inv_T1 = 1.0/T1;
	double inv_T3 = 1.0/T3;

	double mod_third_body;

	/* LOW & Troe */
	kinf = a1*exp(-e1*inv_T);
	if (n1 != 0.0)
		kinf *= pow(T, n1);
	kzero = a0*exp(-e0*inv_T);
	if (n0 != 0.0)
		kzero *= pow(T, n0);

	// needs selector switch
	{
		// 3 parameter TROE
		Fc = (1-a)*exp(-T*inv_T3)+a*exp(-T*inv_T1);
		// 4 parameter TROE
		Fc = (1-a)*exp((-T*inv_T3))+a*exp((-T*inv_T1))+exp((-T2*inv_T));
	}

	// needs the special treatment for the species concentration as an option for mod_third_body
	mod_third_body = third_body;

	if (collision_efficiency_flag) { /* collision efficiency corrections */
		mod_third_body = third_body;
		correction = coll_eff;
		while (correction)
		{
			mod_third_body += (correction->value)*y[correction->index];
			correction = correction->next;
		}
	}
	kappa = log10(kzero*mod_third_body/kinf) - 0.4 -0.67*log10(Fc);
	F = pow(10, (log10(Fc)/(1+pow((kappa/(0.75-1.27*log10(Fc)-0.14*kappa)), 2))));
	k = kzero*kinf*mod_third_body*F/(kzero*mod_third_body+kinf);

	return k;
}
