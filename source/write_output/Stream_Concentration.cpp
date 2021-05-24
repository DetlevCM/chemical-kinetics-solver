#include "../headers/Headers.hpp"

/* This function write out the time, temperature and concentrations in the
 * species concentration file. It does NOT clear the file should it exist
 * and appends data to any existing file. See WriteLabelsSpecies for
 * the initial output which also clears the file. */





// output function for species labels, temperature at back
void Write_Header_Species_Temperature_Pressure(
		string filename,
		string separator,
		size_t Number_Species,
		vector< string > Names,
		bool GasPhasePressure
)
{
	size_t i;
	ofstream Concentration_OFStream (filename.c_str(),ios::out);

	if (Concentration_OFStream.is_open())
	{
		Concentration_OFStream << "Time";
		for(i=0;i<Number_Species;i++)
		{
			Concentration_OFStream << separator << Names[i];
		}
		Concentration_OFStream << separator << "Temperature";
		if(GasPhasePressure)
		{
			Concentration_OFStream << separator << "Pressure";
		}
		Concentration_OFStream << "\n";
		Concentration_OFStream.close();
	}
	else cout << "Unable to open file";
}



// Function to stream the concentrations
void StreamConcentrations(
		ofstream& Concentration_OFStream,
		const string separator,
		bool GasPhasePressure,
		size_t Number_Species,
		double CurrentTime,
		double Pressure,
		const vector<double >& Concentration
)
{
	size_t i;
	Concentration_OFStream << CurrentTime;
		for(i=0;i<=Number_Species;i++)
		{
			Concentration_OFStream << separator << Concentration[i] ;
		}
		if(GasPhasePressure)
		{
			Concentration_OFStream << separator << Pressure;
		}
		Concentration_OFStream << "\n";
}


void StreamConcentrationsV2(
		ofstream& Concentration_OFStream,
		const string separator,
		double CurrentTime,
		size_t Number_Species,
		double* Concentration
		)
{
	size_t i;
	Concentration_OFStream << CurrentTime << separator;

	for(i=0;i<Number_Species;i++)
	{
		Concentration_OFStream << Concentration[i] << separator;
	}
	Concentration_OFStream << Concentration[Number_Species] << separator;
	Concentration_OFStream << "\n";
}


