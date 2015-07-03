#include <MyHeaders.h>

/* This function write out the time, temperature and concentrations in the
 * species concentration file. It does NOT clear the file should it exist
 * and appends data to any existing file. See WriteLabelsSpecies for
 * the initial output which also clears the file. */


// output function for species labels, temperature at back
void WriteLabelsReactionRates(string filename, int Number_Reactons)//, vector< string > Names)
{
	int i;
	// Stream to output file, output mode (Should clear old content)
	ofstream OutputFile (filename.c_str(),ios::out);

	if (OutputFile.is_open())
	{
		OutputFile << "Time";
		for(i=0;i<Number_Reactons;i++)
			{
			//OutputFile << Names[i] << " 	"; // concentration
			OutputFile << "	Reaction(" << i << ")";
			}
		//OutputFile << "Temperature 	"; // only needed for species concentrations
		OutputFile << "\n"; // new line
		OutputFile.close();
	}
	else cout << "Unable to open file";

}

void StreamReactionRates(
		ofstream& OutputFile,
		double CurrentTime,
		vector<double >& ReactionRates
		)
{
	int i;
	OutputFile << CurrentTime; // time

		for(i=0;i< (int) ReactionRates.size();i++)
		{
			OutputFile << "	" << ReactionRates[i] ; // concentration
		}
		OutputFile << "\n"; // new line
		//OutputFile.flush(); // To get the whole line
}
