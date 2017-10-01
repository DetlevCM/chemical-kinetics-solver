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
	// Stream to output file, output mode (Should clear old content)
	ofstream ConcentrationOutput (filename.c_str(),ios::out);

	if (ConcentrationOutput.is_open())
	{
		ConcentrationOutput << "Time";
		for(i=0;i<Number_Species;i++)
		{
			//ConcentrationOutput << "\t" << Names[i];
			ConcentrationOutput << separator << Names[i];
		}
		//?ConcentrationOutput << "\t" << "Temperature";
		ConcentrationOutput << separator << "Temperature";
		if(GasPhasePressure)
		{
			//ConcentrationOutput << "\t" << "Pressure";
			ConcentrationOutput << separator << "Pressure";
		}
		ConcentrationOutput << "\n"; // new line

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

	ConcentrationOutput << CurrentTime;// << "\t" ; // time

		for(i=0;i<=Number_Species;i++)
		{
			//ConcentrationOutput << "\t" << Concentration[i] ;
			ConcentrationOutput << separator << Concentration[i] ;
		}
		if(GasPhasePressure)
		{
			//ConcentrationOutput << "\t" << Pressure;
			ConcentrationOutput << separator << Pressure;
		}
		ConcentrationOutput << "\n"; // new line
		//ConcentrationOutput.flush(); // To get the whole line
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
	//ConcentrationOutput << CurrentTime << " 	" ; // time
	ConcentrationOutput << CurrentTime << separator ; // time

	for(i=0;i<Number_Species;i++)
	{
		//ConcentrationOutput << Concentration[i] << " 	" ;
		ConcentrationOutput << Concentration[i] << separator ;
	}
	//ConcentrationOutput << Concentration[Number_Species] << " 	";
	ConcentrationOutput << Concentration[Number_Species] << separator;
	ConcentrationOutput << "\n"; // new line

	ConcentrationOutput.flush(); // To get the whole line
}
