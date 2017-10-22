#include <MyHeaders.h>

/*
 * 	double TLow;
 *	double TMid;
 *	double THigh;
 *	double NasaLow1;
 *	double NasaLow2;
 *	double NasaLow3;
 *	double NasaLow4;
 *	double NasaLow5;
 *	double NasaLow6;
 *	double NasaLow7;
 *	double NasaHigh1;
 *	double NasaHigh2;
 *	double NasaHigh3;
 *	double NasaHigh4;
 *	double NasaHigh5;
 *	double NasaHigh6;
 *	double NasaHigh7;
 */

void Calculate_Thermodynamics(
		//vector< vector< double > >& CalculatedThermo,
		vector< CalculatedThermodynamics >& CalculatedThermo,
		const double& Temperature,
		const vector< ThermodynamicData >& Thermodynamics
		)
{

	/* 2002 CODATA values */
	double R = 8.314472e0;
	int i;

	int Number_Species = (int)Thermodynamics.size();

	// reduce computational steps -> Is it a bit quicker?
	double T2 = Temperature*Temperature;
	double T3 = T2*Temperature;
	double T4 = T3*Temperature;
	double logT = log(Temperature);
	double InvT = 1/Temperature;


	/* Hf, Cp, Cv, S */

	for(i=0;i<Number_Species;i++)
	{
		// Thermodynamics Mapping:
			// 0 - 2 -> low_T, high_T, change_T
		if (Temperature <= Thermodynamics[i].TChange) {
			// 3 - 9 -> Nasa Low 1 to 7

			//Hf
			CalculatedThermo[i].Hf = R*Temperature*(
				Thermodynamics[i].NasaLow1 +
				Thermodynamics[i].NasaLow2*Temperature*0.5 +
				Thermodynamics[i].NasaLow3*T2/3 +
				Thermodynamics[i].NasaLow4*T3*0.25 +
				Thermodynamics[i].NasaLow5*T4*0.2 +
				Thermodynamics[i].NasaLow6*InvT);

			//Cp
			CalculatedThermo[i].Cp = R*(
				Thermodynamics[i].NasaLow1+
				Thermodynamics[i].NasaLow2*Temperature +
				Thermodynamics[i].NasaLow3*T2 +
				Thermodynamics[i].NasaLow4*T3 +
				Thermodynamics[i].NasaLow5*T4);

			//Cv = Cp - R;
			CalculatedThermo[i].Cv = CalculatedThermo[i].Cp - R;

			//S
			CalculatedThermo[i].S = R*(
				Thermodynamics[i].NasaLow1*logT +
				Thermodynamics[i].NasaLow2*Temperature +
				Thermodynamics[i].NasaLow3*T2*0.5 +
				Thermodynamics[i].NasaLow4*T3/3 +
				Thermodynamics[i].NasaLow5*T4*0.25 +
				Thermodynamics[i].NasaLow7);
		}
		else
		{
			//Hf =
			CalculatedThermo[i].Hf = R*Temperature*(
				Thermodynamics[i].NasaHigh1+
				Thermodynamics[i].NasaHigh2*Temperature*0.5 +
				Thermodynamics[i].NasaHigh3*T2/3 +
				Thermodynamics[i].NasaHigh4*T3*0.25 +
				Thermodynamics[i].NasaHigh5*T4*0.2 +
				Thermodynamics[i].NasaHigh6*InvT);

			//Cp
			CalculatedThermo[i].Cp = R*(
				Thermodynamics[i].NasaHigh1+
				Thermodynamics[i].NasaHigh2*Temperature +
				Thermodynamics[i].NasaHigh3*T2 +
				Thermodynamics[i].NasaHigh4*T3 +
				Thermodynamics[i].NasaHigh5*T4);

			//Cv = Cp - R;
			CalculatedThermo[i].Cv = CalculatedThermo[i].Cp - R;

			//S
			CalculatedThermo[i].S = R*(
				Thermodynamics[i].NasaHigh1*logT +
				Thermodynamics[i].NasaHigh2*Temperature +
				Thermodynamics[i].NasaHigh3*T2*0.5 +
				Thermodynamics[i].NasaHigh4*T3/3 +
				Thermodynamics[i].NasaHigh5*T4*0.25 +
				Thermodynamics[i].NasaHigh7);
		} 
	}
}

	/* for (i = 1; i<=ns; i++)
	{
	third_body += y[i]; // calculate [M]
	if (T <= change_T[i]) {
	Hf[i] = R*T_fudge*(nasa_low[1][i]+nasa_low[2][i]*T_fudge*0.5 + nasa_low[3][i]*T_fudge*T_fudge*0.333333333 + nasa_low[4][i]*T_fudge*T_fudge*T_fudge*0.25 +nasa_low[5][i]*T_fudge*T_fudge*T_fudge*T_fudge*0.2 +nasa_low[6][i]*inv_T);
	Cp[i] = R*(nasa_low[1][i]+nasa_low[2][i]*T_fudge + nasa_low[3][i]*T_fudge*T_fudge + nasa_low[4][i]*T_fudge*T_fudge*T_fudge +nasa_low[5][i]*T_fudge*T_fudge*T_fudge*T_fudge);
	Cv[i] = Cp[i] - R;
	S[i] = R*(nasa_low[1][i]*log(T_fudge) + nasa_low[2][i]*T_fudge + nasa_low[3][i]*0.5*T_fudge*T_fudge + nasa_low[4][i]*T_fudge*T_fudge*T_fudge/3 + nasa_low[5][i]*T_fudge*T_fudge*T_fudge*T_fudge/4 + nasa_low[7][i]);
	}
	else {
	Hf[i] = R*T_fudge*(nasa_high[1][i]+nasa_high[2][i]*T_fudge*0.5 + nasa_high[3][i]*T_fudge*T_fudge*0.3333333 + nasa_high[4][i]*T_fudge*T_fudge*T_fudge*0.25 +nasa_high[5][i]*T_fudge*T_fudge*T_fudge*T_fudge*0.2 +nasa_high[6][i]*inv_T);
	Cp[i] = R*(nasa_high[1][i]+nasa_high[2][i]*T_fudge + nasa_high[3][i]*T_fudge*T_fudge + nasa_high[4][i]*T_fudge*T_fudge*T_fudge +nasa_high[5][i]*T_fudge*T_fudge*T_fudge*T_fudge);
	Cv[i] = Cp[i] - R;
	S[i] = R*(nasa_high[1][i]*log(T_fudge) + nasa_high[2][i]*T_fudge + nasa_high[3][i]*0.5*T_fudge*T_fudge + nasa_high[4][i]*T_fudge*T_fudge*T_fudge/3 + nasa_high[5][i]*T_fudge*T_fudge*T_fudge*T_fudge/4 + nasa_high[7][i]);
	} 
	}//*/

