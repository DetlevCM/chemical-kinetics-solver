#include <MyHeaders.h>

/* This function write out the time, temperature and concentrations in the
 * species concentration file. It does NOT clear the file should it exist
 * and appends data to any existing file. See WriteLabelsSpecies for
 * the initial output which also clears the file. */





// output function for species labels, temperature at back
void WriteNewLabelsSpecies(
		string filename,
		int Number_Species,
		vector< string > Names,
		bool GasPhasePressure
)
{
	int i;
	// Stream to output file, output mode (Should clear old content)
	ofstream ConcentrationOutput (filename.c_str(),ios::out);

	if (ConcentrationOutput.is_open())
	{
		ConcentrationOutput << "Time";
		for(i=0;i<Number_Species;i++)
		{
			ConcentrationOutput << "\t" << Names[i]; // concentration
		}
		ConcentrationOutput << "\t" << "Temperature";
		if(GasPhasePressure)
		{
			ConcentrationOutput << "\t" << "Pressure";
		}
		ConcentrationOutput << "\n"; // new line

		ConcentrationOutput.close();
	}
	else cout << "Unable to open file";

}



// Function to stream the concentrations
void StreamConcentrations(
		ofstream& ConcentrationOutput,
		bool GasPhasePressure,
		int Number_Species,
		double CurrentTime,
		double Pressure,
		vector<double >& Concentration
)
{
	int i;

	ConcentrationOutput << CurrentTime;// << "\t" ; // time

		for(i=0;i<=Number_Species;i++)
		{
			ConcentrationOutput << "\t" << Concentration[i] ; // concentration
		}
		if(GasPhasePressure)
		{
			ConcentrationOutput << "\t" << Pressure;
		}
		ConcentrationOutput << "\n"; // new line
		//ConcentrationOutput.flush(); // To get the whole line
}


void StreamConcentrationsV2(ofstream& ConcentrationOutput, double CurrentTime, int Number_Species, double* Concentration)
{
	int i;
	ConcentrationOutput << CurrentTime << " 	" ; // time

	for(i=0;i<Number_Species;i++)
	{
		ConcentrationOutput << Concentration[i] << " 	" ; // concentration
	}
	ConcentrationOutput << Concentration[Number_Species] << " 	"; // temp
	ConcentrationOutput << "\n"; // new line

	ConcentrationOutput.flush(); // To get the whole line
}
