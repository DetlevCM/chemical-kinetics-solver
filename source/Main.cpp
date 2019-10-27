
// Central header file that collates all header, functions etc.
#include <Headers.hpp>

// http://stackoverflow.com/questions/13600204/checking-if-argvi-exists-c
// arcg = number of arguments
// argv[] arguments
int main(int argc, char* argv[])
{
	/*
	 * It is generally convenient to dump information to a logfile.
	 * However when developing or debugging, it can be simpler to have the information
	 * output to a terminal instead. The presence of a file named "debug" acts as a
	 * switch for redicrecting output to the terminal rather than a log.txt file.
	 */
	// http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
	ifstream debug_file_exists("debug");
	// File does not exist, so redirect to log file
	// http://stackoverflow.com/questions/10150468/how-to-redirect-cin-and-cout-to-files
	std::ofstream out("log.txt");
	std::streambuf *coutbuf = std::cout.rdbuf(); //save old buffer
	if (!debug_file_exists.good()){
		std::cout.rdbuf(out.rdbuf());
	}//*
	else
	{
		std::cout.rdbuf(coutbuf);
	}
	//*/
	debug_file_exists.close();


	// We need to take the input from the user and see what we got:
	vector<string> user_inputs;
	for(int i=1; i<argc; i++) // i=0 is name of executable
	{
		user_inputs.push_back(argv[i]);
	}

	// We create a class for storing necessary filenames:
	FileNames filenames;

	Process_User_Input(
			filenames,
			user_inputs
	);
	user_inputs.clear(); // we no longer need them


	/*
	 * Next we deal with the storage classes for the data that will
	 * be read in.
	 * In any case, this includes a mechanisms.
	 *
	 * It may include initial data (not essential for lumping only)
	 * as well as potentially separately dedicated PetroOxy settings.
	 */
	Reaction_Mechanism reaction_mechanism;
	Initial_Data initial_parameters; // Initial Conditions/Parameters
	//vector< double > InitialSpeciesConcentration;
	PressureVesselCalc PetroOxyDataInitial; // PetroOxy Specific Initial Data


	// Handle All the Data Input - The Arrays Contain the required information
	bool Mechanism_Read_In = Handle_Mechanism_Input(
			filenames,
			reaction_mechanism,
			initial_parameters,
			PetroOxyDataInitial
	);


	if(!Mechanism_Read_In) // Mechanism failed to read in correctly
	{
		cout << "Error occurred while reading in mechanism.\nRun aborted.\n";
		out.close(); // close output stream
		std::cout.rdbuf(coutbuf); // reassign buffer to avoid crash
		exit(1); // terminate the code here
	}

	// Mechanism read in correctly, proceed:

	// for someone else's optimisation code, optional output
	if(initial_parameters.StoichiometryMatrixForOpt)
	{
		Write_Stoichiometric_Matrix_For_Opt
		(
				"stoichiometry_matrix.txt" ,
				reaction_mechanism.Reactions
		);
		Input_File_For_Ehsan_Opt
		(
				"mechanism.txt" ,
				reaction_mechanism.Reactions
		);
	}


	int i; 	// useful counter
	int Number_Species = (int) reaction_mechanism.Species.size();
	int Number_Reactions = (int) reaction_mechanism.Reactions.size();
	vector< double > KeyRates; // for mechanism reduction

	//cout << "test species/reactions: " << Number_Species << " " << Number_Reactions << "\n";
	// We have now pre-processed all information, time to set up the ODEs and the solver
	// Let us set up the reactions first for the ODE solver


	vector< vector < str_RatesAnalysis > > RatesAnalysisData;

	if(initial_parameters.MechanismAnalysis.MaximumRates)
	{
		// Initialise array
		vector < str_RatesAnalysis > temp((int) reaction_mechanism.Reactions.size());
		for(i=0;i<Number_Species;i++)
		{
			RatesAnalysisData.push_back(temp);
		}
		// array prepared
	}


	if(initial_parameters.MechanismAnalysis.StreamRatesAnalysis)
	{
		PrepareStreamRatesAnalysis(reaction_mechanism.Species,"");
	}


	if(!initial_parameters.NoIntegration){

		// Define output filenames:
		Filenames OutputFilenames;

		OutputFilenames.Species = "concentrations.txt";
		OutputFilenames.Rates = "reaction_rates.txt";
		OutputFilenames.PetroOxy = "PetroOxy-log.txt";
		// May need to rethink the rates analysis output...
		//OutputFilenames.RatesAnalysisStream = "";//rates_analysis_stream";
		OutputFilenames.Prefix = "";


		WriteNewLabelsSpecies(
				OutputFilenames.Species,
				initial_parameters.Solver_Parameters.separator,
				Number_Species,
				reaction_mechanism.Species,
				initial_parameters.GasPhase
		);

		if(initial_parameters.PrintReacRates)
		{
			WriteLabelsReactionRates(
					OutputFilenames.Rates,
					initial_parameters.Solver_Parameters.separator,
					Number_Reactions
			);
		}


		// only required if the user desires mechanism reduction
		if(initial_parameters.MechanismReduction.ReduceReactions != 0)
		{
			KeyRates.resize(Number_Reactions);
		}


		cout << "\nHanding Mechanism to Integrator\n";
		Choose_Integrator(
				OutputFilenames,
				reaction_mechanism,
				initial_parameters,
				KeyRates,
				PetroOxyDataInitial,
				RatesAnalysisData
		);


		if(initial_parameters.MechanismReduction.ReduceReactions != 0)
		{
			vector< SingleReactionData > ReducedReactions;
			ReducedReactions = ReduceReactionsNew(reaction_mechanism.Species, reaction_mechanism.Reactions, KeyRates);

			Reaction_Mechanism Reduced_Reaction_Mechanism;
			Reduced_Reaction_Mechanism.Species = reaction_mechanism.Species;
			Reduced_Reaction_Mechanism.Thermodynamics = reaction_mechanism.Thermodynamics;
			Reduced_Reaction_Mechanism.Reactions = ReducedReactions;

			// start a second run only if reduced scheme is not empty and has size different
			// to original scheme
			if(Number_Reactions > 0 && Number_Reactions != (int) ReducedReactions.size()){


				OutputFilenames.Species = "reduced_concentrations.txt";
				OutputFilenames.Rates = "reduced_reaction_rates.txt";
				OutputFilenames.PetroOxy = "reduced_PetroOxy-log.txt";
				OutputFilenames.Prefix = "reduced_";

				Number_Reactions = (int) ReducedReactions.size();

				WriteReactions("reduced_scheme.txt", Reduced_Reaction_Mechanism.Species, ReducedReactions);

				initial_parameters.MechanismReduction.ReduceReactions = 0; // switch off reduction...

				if(initial_parameters.StoichiometryMatrixForOpt)
				{
					Write_Stoichiometric_Matrix_For_Opt
					(
							"reduced_stoichiometry_matrix.txt" ,
							ReducedReactions
					);
				}


				// Option considered experimental, cannot see why it won't work...
				if(initial_parameters.MechanismAnalysis.MaximumRates)
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

				if(initial_parameters.MechanismAnalysis.StreamRatesAnalysis)
				{
					PrepareStreamRatesAnalysis(Reduced_Reaction_Mechanism.Species,OutputFilenames.Prefix);
				}

				WriteNewLabelsSpecies(
						OutputFilenames.Species,
						initial_parameters.Solver_Parameters.separator,
						Number_Species,
						Reduced_Reaction_Mechanism.Species,
						initial_parameters.GasPhase
				);
				WriteLabelsReactionRates(
						OutputFilenames.Rates,
						initial_parameters.Solver_Parameters.separator,
						Number_Reactions
				);


				cout << "\nHanding Reduced Mechanism to Integrator\n" << std::flush;
				Choose_Integrator(
						OutputFilenames,
						Reduced_Reaction_Mechanism,
						initial_parameters,
						KeyRates,
						PetroOxyDataInitial,
						RatesAnalysisData
				);


				// Not ideal, should use variables rather than handwritten filenames
				ReportAccuracy(
						initial_parameters.Solver_Parameters.separator,
						Number_Species,
						Reduced_Reaction_Mechanism.Species,
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

	out.close(); // close output stream
	std::cout.rdbuf(coutbuf); // reassign buffer to avoid crash
	// And it is done. Return 0 as the code has finished.
	return 0;
}









