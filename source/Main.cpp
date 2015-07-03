#include <MyHeaders.h>

namespace GlobalArrays{
// Key parameters that define the whole reaction scheme - used globally via namespaces
// Not sure if this is a good place to put it...
vector< vector < str_RatesAnalysis > > RatesAnalysisData;
vector< TrackSpecies > ProductsForRatesAnalysis;
}




// Shoddy implementation of PetroOxy logic
PetroOxyCalculation PetroOxyData;
int OxyGasSpeciesID;
// end PetroOxy Logic

int main(int argc, char* argv[])
{


	// Add a switch to have regular ouput to a log file or debug to command line
	// http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
	ifstream file_exists("debug");
	// File does not exist, so redirect to log file
			// http://stackoverflow.com/questions/10150468/how-to-redirect-cin-and-cout-to-files
			std::ofstream out("log.txt");
	if (!file_exists.good()){
		//std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
		std::cout.rdbuf(out.rdbuf());
	}
	file_exists.close();



	// Definition of the main variables to store basic information to run the sover

	vector< string > Species; // stores the species list
	vector< ThermodynamicData > Thermodynamics; // stores the entire Thermodynamic data
	vector< SingleReactionData > Reactions; // stores the entire mechanism

	InitParam InitialParameters; // Initial Conditions/Parameters
	vector< double > InitialSpeciesConcentration;

	PetroOxyCalculation PetroOxyDataInitial; // Petro Oxy Specific



	// Handle All the Data Input - The Arrays Contain the required information

	bool Mechanism_Read_In = Handle_Mechanism_Input(
			"chem.inp",
			Species,
			Thermodynamics,
			Reactions,
			InitialParameters,
			InitialSpeciesConcentration,
			PetroOxyDataInitial
	);



	if(!Mechanism_Read_In) // Mechanism failed to read in correctly
	{
		cout << "Error occurred while reading in mechanism.\nRun aborted.\n";
	}
	else // Mechnsim read in correctly, proceed:
	{


		if(InitialParameters.StoichimoetryMatrixForOpt)
		{
			Write_Stoichiometric_Matrix_For_Opt
			(
					"stoichiometry_matrix.txt" ,
					Reactions
			);
		}

		using namespace GlobalArrays;

		int i; 	// useful counter
		int Number_Species = (int) Species.size();
		int Number_Reactions = (int) Reactions.size();
		vector< double > KeyRates; // for mechanism reduction

		// We have now pre-processed all information, time to set up the ODEs and the solver
		// Let us set up the reactions first for the ODE solver

		//cout << "On to the solver. \n";

		if(InitialParameters.RatesMaxAnalysis)
		{
			// Initialise array
			vector < str_RatesAnalysis > temp((int) Reactions.size());
			for(i=0;i<Number_Species;i++)
			{
				RatesAnalysisData.push_back(temp);
			}
			// array prepared
		}


		if(InitialParameters.StreamRatesAnalysis)
		{
			PrepareStreamRatesAnalysis(Species,"");
		}


		WriteNewLabelsSpecies(
				"concentrations.txt",
				Number_Species,
				Species,
				InitialParameters.GasPhase
		);

		if(InitialParameters.PrintReacRates)
		{
			WriteLabelsReactionRates("reaction_rates.txt", Number_Reactions);
		}

		if(InitialParameters.PetroOxy)
		{
			PetroOxyData = PetroOxyDataInitial;
			OxyGasSpeciesID = InitialParameters.PetroOxyGasSpecies;
			PetroOxyOutputHeader("PetroOxy-Log.txt");
		}

		// only required if the user desires mechanism reduction
		if(InitialParameters.ReduceReactions != 0)
		{
			KeyRates.resize(Number_Reactions);
		}


		cout << "\nHanding Mechanism to Integrator\n" << std::flush;

		Integrate(
				"concentrations.txt",
				"reaction_rates.txt",
				"PetroOxy-Log.txt",
				"",
				InitialSpeciesConcentration,
				Species,
				Reactions,
				InitialParameters,
				KeyRates
		);


		if(InitialParameters.ReduceReactions != 0)
		{
			vector< SingleReactionData > ReducedReactions;
			ReducedReactions = ReduceReactionsNew(Species, Reactions, KeyRates);


			// start a second run only if reduced scheme is not empty and has size different
			// to original scheme
			if(Number_Reactions > 0 && Number_Reactions != (int) ReducedReactions.size()){

				Number_Reactions = (int) ReducedReactions.size();

				WriteReactions("reduced_scheme.txt", Species, ReducedReactions);

				InitialParameters.ReduceReactions = 0; // switch off reduction...

				if(InitialParameters.StoichimoetryMatrixForOpt)
				{
					Write_Stoichiometric_Matrix_For_Opt
					(
							"reduced_stoichiometry_matrix.txt" ,
							//		Species,
							ReducedReactions
					);
				}


				// Option considered experimental, cannot see why it won't work...
				if(InitialParameters.RatesMaxAnalysis)
				{
					// Initialise array
					RatesAnalysisData.clear(); // empty for new run
					vector < str_RatesAnalysis > temp(Number_Reactions);
					for(i=0;i<Number_Species;i++)
					{
						RatesAnalysisData.push_back(temp);
					}
					// array prepared
				}

				if(InitialParameters.StreamRatesAnalysis)
				{
					PrepareStreamRatesAnalysis(Species,"reduced_");
				}

				WriteNewLabelsSpecies(
						"reduced_concentrations.txt",
						Number_Species,
						Species,
						InitialParameters.GasPhase
				);
				WriteLabelsReactionRates("reduced_reaction_rates.txt", Number_Reactions);


				if(InitialParameters.PetroOxy)
				{
					PetroOxyData = PetroOxyDataInitial;
					OxyGasSpeciesID =  InitialParameters.PetroOxyGasSpecies;
					PetroOxyOutputHeader("reduced_PetroOxy-Log.txt");
				}


				cout << "\nHanding Reduced Mechanism to Integrator\n" << std::flush;

				Integrate(
						"reduced_concentrations.txt",
						"reduced_reaction_rates.txt",
						"reduced_PetroOxy-Log.txt",
						"reduced_",
						InitialSpeciesConcentration,
						Species,
						ReducedReactions,
						InitialParameters,
						KeyRates
				);


				ReportAccuracy(
						Number_Species,
						Species,
						"reduction_accuracy_report.txt",
						"concentrations.txt",
						"reduced_concentrations.txt"
				);

			}
			else
			{
				cout <<
						"Reduced Scheme does not contain any reactions or is identical in size to the original scheme."
						" \n Aborting...!!!";
			};

		}

	}

	// And it is done. Return 0 as the code has finished.
	return 0;
}









