

#include <math.h>

#include "Species.h"

// function to cconvert vectorstring> of species into vector<class> for new design
vector<Species> Species::VectorClass(vector<string> species)
{

	vector<Species> new_species_vec;
	new_species_vec.reserve(species.size());

for(size_t i=0; species.size(); i++)
{
	Species new_species;
	new_species.Name = species[i];

	new_species_vec.push_back(new_species);
}

}


void Species::ThermodynamicData::SetNasa(
	double tlow,
	double thigh,
	double tchange,
	vector<double> nasalow, 
	vector<double> nasahigh
)
{
			
	TLow = tlow;
	THigh = thigh;
	TChange = tchange;

	NasaLow = nasalow;
	NasaLow1 = nasalow[0];
	NasaLow2 = nasalow[1];
	NasaLow3 = nasalow[2];
	NasaLow4 = nasalow[3];
	NasaLow5 = nasalow[4];
	NasaLow6 = nasalow[5];
	NasaLow7 = nasalow[6];

	NasaHigh = nasahigh;
	NasaHigh1 = nasahigh[0];
	NasaHigh2 = nasahigh[1];
	NasaHigh3 = nasahigh[2];
	NasaHigh4 = nasahigh[3];
	NasaHigh5 = nasahigh[4];
	NasaHigh6 = nasahigh[5];
	NasaHigh7 = nasahigh[6];
}



double Species::ThermodynamicData::calculate_Hf_at_T(const double temperature)
{
    double T2 = temperature*temperature;
	double T3 = T2*temperature;
	double T4 = T3*temperature;
	double logT = log(temperature);
	double InvT = 1.0/temperature;

    if (temperature <= TChange) {

    return R*temperature*(
				NasaLow1 +
				NasaLow2*temperature*0.5 +
				NasaLow3*T2/3.0 +
				NasaLow4*T3*0.25 +
				NasaLow5*T4*0.2 +
				NasaLow6*InvT);
    }
    else
    {
        return R*temperature*(
				NasaHigh1+
				NasaHigh2*temperature*0.5 +
				NasaHigh3*T2/3.0 +
				NasaHigh4*T3*0.25 +
				NasaHigh5*T4*0.2 +
				NasaHigh6*InvT);
    }
}

double Species::ThermodynamicData::calculate_Cp_at_T(const double temperature)
{
    double T2 = temperature*temperature;
	double T3 = T2*temperature;
	double T4 = T3*temperature;
	double logT = log(temperature);
	double InvT = 1.0/temperature;

    if (temperature <= TChange) {
        			return R*(
				NasaLow1+
				NasaLow2*temperature +
				NasaLow3*T2 +
				NasaLow4*T3 +
				NasaLow5*T4);
                    }
                    else
                    {
			return R*(
				NasaHigh1+
				NasaHigh2*temperature +
				NasaHigh3*T2 +
				NasaHigh4*T3 +
				NasaHigh5*T4);
                    };
}


double Species::ThermodynamicData::calculate_Cv_at_T(const double temperature)
{
    return ThermodynamicData::calculate_Cp_at_T(temperature) - R;
}

double Species::ThermodynamicData::calculate_S_at_T(const double temperature)
{
    double T2 = temperature*temperature;
	double T3 = T2*temperature;
	double T4 = T3*temperature;
	double logT = log(temperature);
	double InvT = 1.0/temperature;

    if (temperature <= TChange) {
        return  R*(
				NasaLow1*logT +
				NasaLow2*temperature +
				NasaLow3*T2*0.5 +
				NasaLow4*T3/3.0 +
				NasaLow5*T4*0.25 +
				NasaLow7);
    }
    else
    {
        return R*(
				NasaHigh1*logT +
				NasaHigh2*temperature +
				NasaHigh3*T2*0.5 +
				NasaHigh4*T3/3.0 +
				NasaHigh5*T4*0.25 +
				NasaHigh7);
    };
}
