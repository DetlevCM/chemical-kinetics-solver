#include <MyHeaders.h>

namespace GlobalArrays{
// Key parameters that define the whole reaction scheme - used globally via namespaces
// Not sure if this is a good place to put it...
vector< vector < str_RatesAnalysis > > RatesAnalysisData;
vector< TrackSpecies > ProductsForRatesAnalysis;
vector< JacobianData > JacobianMatrix;
}



// Global for now, clean up !!!!!!!!!!!
vector< double > KeyRates;

// Shoddy implementation of PetroOxy logic
PetroOxyCalculation PetroOxyData;
int OxyGasSpeciesID;
// end PetroOxy Logic

int main(int argc, char* argv[])
{



	//*
	// http://stackoverflow.com/questions/10150468/how-to-redirect-cin-and-cout-to-files
	std::ofstream out("log.txt");
	//std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(out.rdbuf());
	//*/

	vector< string > Species;

	vector< SingleReactionData > Reactions;

	using namespace GlobalArrays;
	//using namespace ODESolverConstant; // what is needed?

	PetroOxyCalculation PetroOxyDataInitial;
	InitParam InitialParameters;

	// Globally useful counter
	int i;



	// Some additions to facilitate moving things to other files

	vector< ThermodynamicData > Thermodynamics;
	// End additions

	int Number_Species;
	int Number_Reactions;
	vector< double > InitialSpeciesConcentration;

	// Handle All the Data Input - The Arrays Contain the required information

	Handle_Mechanism_Input(
			"chem.inp",
			Species,
			Thermodynamics,
			Reactions,
			InitialParameters,
			InitialSpeciesConcentration,
			PetroOxyDataInitial
	);

	if(InitialParameters.StoichimoetryMatrixForOpt)
	{
		Write_Stoichiometric_Matrix_For_Opt
		(
				"stoichiometry_matrix.txt" ,
		//		Species,
				Reactions
		);
	}

	Number_Species = (int) Species.size();
	Number_Reactions = (int) Reactions.size();

	// We have now pre-processed all information, time to set up the ODEs and the solver
	// Let us set up the reactions first for the ODE solver

	cout << "On to the solver. \n";

	if(InitialParameters.RatesMaxAnalysis)
	{// Initialise array
		vector < str_RatesAnalysis > temp((int) Reactions.size());
		for(i=0;i<Number_Species;i++)
		{
			RatesAnalysisData.push_back(temp);
		}// array prepared
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


	if(InitialParameters.ReduceReactions != 0)
	{
		KeyRates.resize(Number_Reactions);
	}


	cout << "Handing to Integrator\n";

	Integrate(
			"concentrations.txt",
			"reaction_rates.txt",
			"PetroOxy-Log.txt",
			"",
			InitialSpeciesConcentration,
			Species,
			Reactions,
			InitialParameters
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
			{// Initialise array
				RatesAnalysisData.clear(); // empty for new run
				vector < str_RatesAnalysis > temp(Number_Reactions);
				for(i=0;i<Number_Species;i++)
				{
					RatesAnalysisData.push_back(temp);
				}// array prepared
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


			Integrate(
					"reduced_concentrations.txt",
					"reduced_reaction_rates.txt",
					"reduced_PetroOxy-Log.txt",
					"reduced_",
					InitialSpeciesConcentration,
					Species,
					ReducedReactions,
					InitialParameters
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


	// And it is done. Return 0 as the code has finished.
	return 0;
}









