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

////
//// NOTE: should replace ReactionData with something more compact, will do for now...
////

double Calculate_no_LOW_Troe(
		const SingleReactionData& ReactionData,
		const vector<double>& Concentration,
		double T, double third_body)
{
	double inv_T = 1.0/T;
	double k; // this is the normal forward K

	// values at "infinity" - standard Arrhenius values
	double a1 = ReactionData.paramA;
	double n1 = ReactionData.paramN;
	double e1 = ReactionData.paramEa;

	double mod_third_body;

	// not applicable to a special type with water

	/* no LOW or TROE terms */
	if (ReactionData.collision_efficiency) { /* no collision efficiency corrections */
		k = third_body*a1*exp(-e1*inv_T);
		if (n1 != 0.0)
			k *= pow(T, n1);
	}
	else { /* collision efficiency corrections required */
		mod_third_body = third_body;

		// seems like a case of vector<value, speciesID for concentration> to calculate the correction
		for(size_t i = 0;i<ReactionData.ThBd_param.size();i++)
		{
			mod_third_body = mod_third_body + ReactionData.ThBd_param[i].value * Concentration[ReactionData.ThBd_param[i].SpeciesID];
		}
		/*
		correction = coll_eff;
		while (correction)
		{
			mod_third_body += (correction->value)*y[correction->index];
			correction = correction->next;
		}//*/
		k = mod_third_body*a1*exp(-e1*inv_T);
		if (n1 != 0.0)
			k *= pow(T, n1);
	}

	return k;
}

double Calculate_Lindeman_Hinshelwood_Low(
		const SingleReactionData& ReactionData,
		const vector<double>& Concentration,
		double T,double a0, double n0, double e0, double third_body)
{
	double inv_T = 1.0/T;
	double kinf;
	double kzero;
	double F;
	double k; // the result, Kf

	// values at "infinity" - standard Arrhenius values
	double a1 = ReactionData.paramA;
	double n1 = ReactionData.paramN;
	double e1 = ReactionData.paramEa;

	SRIThirdBody sri = ReactionData.sriThirdBody;

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
	if (ReactionData.collision_efficiency)  { /* collision efficiency corrections */
		mod_third_body = third_body;

		// seems like a case of vector<value, speciesID for concentration> to calculate the correction
		for(size_t i = 0;i<ReactionData.ThBd_param.size();i++)
		{
			mod_third_body = mod_third_body + ReactionData.ThBd_param[i].value * Concentration[ReactionData.ThBd_param[i].SpeciesID];
		}
		/*
		correction = coll_eff;
		while (correction)
		{
			mod_third_body += (correction->value)*y[correction->index];
			correction = correction->next;
		}//*/
	}
	switch (ReactionData.sri_flag) {
	case 0: /* its Lindeman-Hinshelwood */
		k=kzero*kinf*mod_third_body/(kzero*mod_third_body+kinf);
		break;
	case 1: /* its simple SRI */
		F = T*pow((sri.a*exp(-sri.b/T)+exp(-T/sri.c)), ((1.0/(1.0+pow((log10(kzero*mod_third_body/kinf)), 2)))));
		k = kzero*kinf*mod_third_body*F/(kzero*mod_third_body+kinf);
		break;
	case 2: /* It's complex SRI */
		F = sri.d*pow(T, sri.e)*pow((sri.a*exp(-sri.b/T)+exp(-T/sri.c)), ((1.0/(1.0+pow((log10(kzero*mod_third_body/kinf)), 2)))));
		k = kzero*kinf*mod_third_body*F/(kzero*mod_third_body+kinf);
		break;
	}

	return k;
}

double Calculate_Lindeman_Hinshelwood_Low_Troe(
		const SingleReactionData& ReactionData,
		const vector<double>& Concentration,
		double T, // current temperature
		double a0, double n0, double e0, // third body parameters
		double third_body // sum of third bodies, but which units, original molecules per cm3
)
{
	double inv_T = 1.0/T;
	double kinf;
	double kzero;
	double kappa;
	double Fc,F;
	double k; // the result, Kf

	// values at "infinity" - standard Arrhenius values
	double a1 = ReactionData.paramA;
	double n1 = ReactionData.paramN;
	double e1 = ReactionData.paramEa;

	TroeThirdBody troe = ReactionData.troeThirdBody;
	double inv_T1 = 1.0/troe.T1;
	double inv_T3 = 1.0/troe.T3;

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
		// 3 parameter TROE takes a, T3, T1
		Fc = (1-troe.a)*exp(-T*inv_T3)+troe.a*exp(-T*inv_T1);
		// 4 parameter TROE takes a, T3, T1, T2
		Fc = (1-troe.a)*exp((-T*inv_T3))+troe.a*exp((-T*inv_T1))+exp((-troe.T2*inv_T));
	}

	// needs the special treatment for the species concentration as an option for mod_third_body
	mod_third_body = third_body;

	if (ReactionData.collision_efficiency) { /* collision efficiency corrections */
		mod_third_body = third_body;

		// seems like a case of vector<value, speciesID for concentration> to calculate the correction
		for(size_t i = 0;i<ReactionData.ThBd_param.size();i++)
		{
			mod_third_body = mod_third_body + ReactionData.ThBd_param[i].value * Concentration[ReactionData.ThBd_param[i].SpeciesID];
		}
		/*
		correction = coll_eff;
		while (correction)
		{
			mod_third_body += (correction->value)*y[correction->index];
			correction = correction->next;
		}//*/
	}
	kappa = log10(kzero*mod_third_body/kinf) - 0.4 -0.67*log10(Fc);
	F = pow(10, (log10(Fc)/(1+pow((kappa/(0.75-1.27*log10(Fc)-0.14*kappa)), 2))));
	k = kzero*kinf*mod_third_body*F/(kzero*mod_third_body+kinf);

	return k;
}
