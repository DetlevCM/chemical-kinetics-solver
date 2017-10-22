#include <MyHeaders.h>

/* This function write out the time, temperature and concentrations in the
 * species concentration file. It does NOT clear the file should it exist
 * and appends data to any existing file. See WriteLabelsSpecies for
 * the initial output which also clears the file. */





// output function for species labels, temperature at back
void WriteNewLabelsSpecies(
		string filename,
		string separator,
		int Number_Species,
		vector< string > Names,
		bool GasPhasePressure
)
{
	int i;
	ofstream ConcentrationOutput (filename.c_str(),ios::out);

	if (ConcentrationOutput.is_open())
	{
		ConcentrationOutput << "Time";
		for(i=0;i<Number_Species;i++)
		{
			ConcentrationOutput << separator << Names[i];
		}
		ConcentrationOutput << separator << "Temperature";
		if(GasPhasePressure)
		{
			ConcentrationOutput << separator << "Pressure";
		}
		ConcentrationOutput << "\n";
		ConcentrationOutput.close();
	}
	else cout << "Unable to open file";
}



// Function to stream the concentrations
void StreamConcentrations(
		ofstream& ConcentrationOutput,
		const string separator,
		bool GasPhasePressure,
		int Number_Species,
		double CurrentTime,
		double Pressure,
		vector<double >& Concentration
)
{
	int i;
	ConcentrationOutput << CurrentTime;
		for(i=0;i<=Number_Species;i++)
		{
			ConcentrationOutput << separator << Concentration[i] ;
		}
		if(GasPhasePressure)
		{
			ConcentrationOutput << separator << Pressure;
		}
		ConcentrationOutput << "\n";
}


void StreamConcentrationsV2(
		ofstream& ConcentrationOutput,
		const string separator,
		double CurrentTime,
		int Number_Species,
		double* Concentration
		)
{
	int i;
	ConcentrationOutput << CurrentTime << separator;

	for(i=0;i<Number_Species;i++)
	{
		ConcentrationOutput << Concentration[i] << separator;
	}
	ConcentrationOutput << Concentration[Number_Species] << separator;
	ConcentrationOutput << "\n";
}


