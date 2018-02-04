#include <MyHeaders.h>




int main(int argc, char* argv[])
{

	// Add a switch to have regular output to a log file or debug to command line
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


	string mechanism_filename = "";
	string initial_data_filename = "";

	// http://stackoverflow.com/questions/13600204/checking-if-argvi-exists-c
	// arcg = number of arguments
	// argv[] arguments

	// Case 1: We have the mechanism provided (makes most sense)
	if(argc >= 2)
	{
		mechanism_filename = argv[1];
	}
	else
	{
		mechanism_filename = "chem.inp"; // standard
	}

	// Case 2: The user supplied the initial conditions (more likely to change)
	if(argc == 3)
	{
		initial_data_filename = argv[2];
	}
	else
	{
		initial_data_filename = "initial.inp"; // old format officially depreciated
	}


	// The main variables that store the information from a reaction mechanism after
	// it is read in, namely the species, thermodynamic data and reactions.


	MechanismData Reaction_Mechanism;
	Initial_Data InitialParameters; // Initial Conditions/Parameters
	vector< double > InitialSpeciesConcentration;
	PressureVesselCalc PetroOxyDataInitial; // PetroOxy Specific Initial Data


	// Handle All the Data Input - The Arrays Contain the required information
	bool Mechanism_Read_In = Handle_Mechanism_Input(
			mechanism_filename,
			initial_data_filename,
			Reaction_Mechanism,
			InitialParameters,
			InitialSpeciesConcentration,
			PetroOxyDataInitial
	);


	if(!Mechanism_Read_In) // Mechanism failed to read in correctly
	{
		cout << "Error occurred while reading in mechanism.\nRun aborted.\n";
	}
	else // Mechanism read in correctly, proceed:
	{
		// for someone else's optimisation code, optional output
		if(InitialParameters.StoichiometryMatrixForOpt)
		{
			Write_Stoichiometric_Matrix_For_Opt
			(
					"stoichiometry_matrix.txt" ,
					Reaction_Mechanism.Reactions
			);
			Input_File_For_Ehsan_Opt
			(
					"mechanism.txt" ,
					Reaction_Mechanism.Reactions
			);
		}


		int i; 	// useful counter
		int Number_Species = (int) Reaction_Mechanism.Species.size();
		int Number_Reactions = (int) Reaction_Mechanism.Reactions.size();
		vector< double > KeyRates; // for mechanism reduction

		// We have now pre-processed all information, time to set up the ODEs and the solver
		// Let us set up the reactions first for the ODE solver


		vector< vector < str_RatesAnalysis > > RatesAnalysisData;

		if(InitialParameters.MechanismAnalysis.MaximumRates)
		{
			// Initialise array
			vector < str_RatesAnalysis > temp((int) Reaction_Mechanism.Reactions.size());
			for(i=0;i<Number_Species;i++)
			{
				RatesAnalysisData.push_back(temp);
			}
			// array prepared
		}


		if(InitialParameters.MechanismAnalysis.StreamRatesAnalysis)
		{
			PrepareStreamRatesAnalysis(Reaction_Mechanism.Species,"");
		}


		if(!InitialParameters.NoIntegration){

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
					InitialParameters.Solver_Parameters.separator,
					Number_Species,
					Reaction_Mechanism.Species,
					InitialParameters.GasPhase
			);

			if(InitialParameters.PrintReacRates)
			{
				WriteLabelsReactionRates(
						OutputFilenames.Rates,
						InitialParameters.Solver_Parameters.separator,
						Number_Reactions
				);
			}


			// only required if the user desires mechanism reduction
			if(InitialParameters.MechanismReduction.ReduceReactions != 0)
			{
				KeyRates.resize(Number_Reactions);
			}


			cout << "\nHanding Mechanism to Integrator\n";
			Choose_Integrator(
					OutputFilenames,
					InitialSpeciesConcentration,
					Reaction_Mechanism,
					//Species,Thermodynamics,Reactions,
					InitialParameters,
					KeyRates,
					PetroOxyDataInitial,
					RatesAnalysisData
			);


			if(InitialParameters.MechanismReduction.ReduceReactions != 0)
			{
				vector< SingleReactionData > ReducedReactions;
				ReducedReactions = ReduceReactionsNew(Reaction_Mechanism.Species, Reaction_Mechanism.Reactions, KeyRates);

				MechanismData Reduced_Reaction_Mechanism;
				Reduced_Reaction_Mechanism.Species = Reaction_Mechanism.Species;
				Reduced_Reaction_Mechanism.Thermodynamics = Reaction_Mechanism.Thermodynamics;
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

					InitialParameters.MechanismReduction.ReduceReactions = 0; // switch off reduction...

					if(InitialParameters.StoichiometryMatrixForOpt)
					{
						Write_Stoichiometric_Matrix_For_Opt
						(
								"reduced_stoichiometry_matrix.txt" ,
								ReducedReactions
						);
					}


					// Option considered experimental, cannot see why it won't work...
					if(InitialParameters.MechanismAnalysis.MaximumRates)
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

					if(InitialParameters.MechanismAnalysis.StreamRatesAnalysis)
					{
						PrepareStreamRatesAnalysis(Reduced_Reaction_Mechanism.Species,OutputFilenames.Prefix);
					}

					WriteNewLabelsSpecies(
							OutputFilenames.Species,
							InitialParameters.Solver_Parameters.separator,
							Number_Species,
							Reduced_Reaction_Mechanism.Species,
							InitialParameters.GasPhase
					);
					WriteLabelsReactionRates(
							OutputFilenames.Rates,
							InitialParameters.Solver_Parameters.separator,
							Number_Reactions
					);


					cout << "\nHanding Reduced Mechanism to Integrator\n" << std::flush;
					Choose_Integrator(
							OutputFilenames,
							InitialSpeciesConcentration,
							Reduced_Reaction_Mechanism,
							//Species,Thermodynamics,ReducedReactions,
							InitialParameters,
							KeyRates,
							PetroOxyDataInitial,
							RatesAnalysisData
					);


					// Not ideal, should use variables rather than handwritten filenames
					ReportAccuracy(
							InitialParameters.Solver_Parameters.separator,
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
	}

	out.close(); // close output stream
	std::cout.rdbuf(coutbuf); // reassign buffer to avoid crash
	// And it is done. Return 0 as the code has finished.
	return 0;
}









