#include "../headers/Headers.hpp"


void Evaluate_Thermodynamic_Parameters(
		vector< CalculatedThermodynamics > &CalculatedThermo,
		const vector<ThermodynamicData> &Thermodynamics,
		double Temperature
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



