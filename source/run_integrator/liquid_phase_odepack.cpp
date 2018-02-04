/*
 * liquid_phase_odepack.cpp
 *
 *  Created on: 02.07.2015
 *      Author: DetlevCM
 */
#include <MyHeaders.h>



// Not a perfect solution, but stick integrator into its own void with global variables via a namespace
void Integrate_Liquid_Phase_Odepack_LSODA(
		Filenames OutputFilenames,
		vector< double > SpeciesConcentration,
		MechanismData Reaction_Mechanism,
		//vector< string > Species,vector< ThermodynamicData > Thermo,vector< SingleReactionData >& Reactions,
		Initial_Data InitialParameters,
		vector< double >& KeyRates,
		vector< vector < str_RatesAnalysis > >& RatesAnalysisData
)
{

	vector< TrackSpecies > ProductsForRatesAnalysis;

	using namespace Jacobian_ODE_RHS;
	using namespace ODE_RHS;
	using namespace Jacobian;


	Number_Species = (int)Reaction_Mechanism.Species.size();
	Number_Reactions = (int)Reaction_Mechanism.Reactions.size();

	// outputting mechanism size in integration routing so that it is printed every time
	cout << "The mechanism to be integrated contains " << Number_Species << " species and " << Number_Reactions << " Reactions.\n" << std::flush;


	Thermodynamics = Reaction_Mechanism.Thermodynamics; // "Hack" - to fix a regression


	ofstream ReactionRatesOutput;
	ofstream ConcentrationOutput (OutputFilenames.Species.c_str(),ios::app);

	if(InitialParameters.PrintReacRates)
	{
		ReactionRatesOutput.open(OutputFilenames.Rates.c_str(),ios::app);
	}

	// general variables
	int i, n;

	// general solver settings
	double RTOL, ATOL; //tr,

	// LSODA specific settings
	int LRW, LIW;
	int ITOL = 1;
	int ITASK = 1;
	int ISTATE = 1;
	int IOPT = 0;

	// calculate size of working vectors for LSODA
	// LRS = 22 + 9*NEQ + NEQ**2           if JT = 1 or 2,
	n = Number_Species + 1;
	if ( (20 + 16 * n) > (22 + 9 * n + n * n) ) {
		LRW = (20 + 16 * n);
	} else {
		LRW = (22 + 9 * n + n * n);
	}
	LIW = 20 + n;

	// some vectors for LSODA
	vector<int> vector_IWORK(LIW);
	int *IWORK = &vector_IWORK[0];
	vector<double> vector_RWORK(LRW);
	double *RWORK = &vector_RWORK[0];

	// For performance assessment, use a clock:
	clock_t cpu_time_begin, cpu_time_end, cpu_time_current;

	// Some tolerances for the solver:
	RTOL = InitialParameters.Solver_Parameters.rtol;
	ATOL = InitialParameters.Solver_Parameters.atol;

	Delta_N = Get_Delta_N(Reaction_Mechanism.Reactions); // just make sure the Delta_N is current
	// Reduce the matrix from a sparse matrix to something more manageable and quicker to use


	ReactionParameters = Process_Reaction_Parameters(Reaction_Mechanism.Reactions);

	ReactantsForReactions = Reactants_ForReactionRate(Reaction_Mechanism.Reactions);

	ProductsForReactions = Products_ForReactionRate(Reaction_Mechanism.Reactions,false);


	if(InitialParameters.MechanismAnalysis.MaximumRates ||
			InitialParameters.MechanismAnalysis.StreamRatesAnalysis ||
			InitialParameters.MechanismAnalysis.RatesAnalysisAtTime ||
			InitialParameters.MechanismAnalysis.RatesOfSpecies)
	{
		ProductsForRatesAnalysis = Products_ForReactionRate(Reaction_Mechanism.Reactions,true);
	}

	SpeciesLossAll = PrepareSpecies_ForSpeciesLoss(Reaction_Mechanism.Reactions); // New method of listing species

	// original old code
	double* y = &SpeciesConcentration[0];
	Concentration.clear();
	Concentration.resize(Number_Species + 1);

	double time_current, time_step, time_step1, time_end;

	time_current = 0;// -> Solver is designed for t_0 = 0
	time_step1 = InitialParameters.TimeStep[0];
	time_end = InitialParameters.TimeEnd[0];
	int TimeChanges = (int) InitialParameters.TimeStep.size();
	int tracker = 0;

	//cout << "\nEnd Time: " << time_end << " Time Step: " << time_step1 << "\n";

	/* -- Initial values at t = 0 -- */

	Number_Reactions = (int) ReactionParameters.size();

	CalculatedThermo.resize(Number_Species);

	InitialDataConstants.EnforceStability = InitialParameters.EnforceStability;
	InitialDataConstants.temperature = InitialParameters.temperature;


	// set constant concentration if desired
	InitialDataConstants.ConstantConcentration = InitialParameters.ConstantConcentration;
	if(InitialParameters.ConstantConcentration)
	{
		cout << "Constant Species desired\n";

		InitialDataConstants.ConstantSpecies.clear();
		InitialDataConstants.ConstantSpecies.resize(Number_Species);

		// zero just to make sure
		for(i=0;i<Number_Species;i++)
		{
			InitialDataConstants.ConstantSpecies[i] = 0;
		}

		for(i=0;i<(int)InitialParameters.ConstantSpecies.size();i++)
		{// fix initial concentrations
			InitialDataConstants.ConstantSpecies[InitialParameters.ConstantSpecies[i]] =
					SpeciesConcentration[InitialParameters.ConstantSpecies[i]];
		}
	}

	Evaluate_Thermodynamic_Parameters(CalculatedThermo, Thermodynamics, SpeciesConcentration[Number_Species]);

	/*for(i=0;i<Number_Species;i++)
	{
	cout << CalculatedThermo[i].Hf << " " << CalculatedThermo[i].Cp << " " << CalculatedThermo[i].S <<"\n";
	}//*/

	Kf.clear();
	Kr.clear();
	Kf.resize(Number_Reactions);
	Kr.resize(Number_Reactions);

	for(i=0;i<Number_Reactions;i++)
	{
		Kr[i] = 0;
	}
	Rates.resize(Number_Reactions);


	// prepare the jacobian matrix
	Prepare_Jacobian_Matrix(JacobianMatrix,Reaction_Mechanism.Reactions,Reaction_Mechanism.Species);


	// Get the rate Constants, forward and backwards
	Calculate_Rate_Constant(Kf, Kr, SpeciesConcentration[Number_Species],ReactionParameters, CalculatedThermo, SpeciesLossAll, Delta_N);
	CalculateReactionRates(Rates, SpeciesConcentration, Kf, Kr, ReactantsForReactions, ProductsForReactions);

	// Don't forget Rates Analysis for Mechanism Recution at t=0 - or is this nonsense?
	if(InitialParameters.MechanismReduction.ReduceReactions != 0)
	{
		ReactionRateImportance(KeyRates, Rates, InitialParameters.MechanismReduction.ReduceReactions);
	}


	// do not forget output at time = 0
	StreamConcentrations(
			ConcentrationOutput,
			InitialParameters.Solver_Parameters.separator,
			InitialParameters.GasPhase,
			Number_Species,
			time_current,
			InitialParameters.GasPhasePressure,
			SpeciesConcentration
	);

	// Only stream if the user desires it, still doesn't prevent file creation...
	if(InitialParameters.PrintReacRates)
	{
		StreamReactionRates(
				ReactionRatesOutput,
				InitialParameters.Solver_Parameters.separator,
				time_current,
				Rates
		);
	}

	// not happy with this more widely available, needs a cleanup...
	vector< vector< int > > ReactionsForSpeciesSelectedForRates;
	// Not the best place to put it, but OK for now:
	if(InitialParameters.MechanismAnalysis.RatesOfSpecies)
	{
		int tempi, tempj;

		vector< vector< int > > TempMatrix;
		vector< int > TempRow;
		int Temp_Number_Species = (int) Reaction_Mechanism.Species.size();

		for(tempi=0;tempi<(int)Reaction_Mechanism.Reactions.size();tempi++){
			TempRow.resize((int)Reaction_Mechanism.Species.size());
			for(tempj=0;tempj<Temp_Number_Species;tempj++)
			{
				if(Reaction_Mechanism.Reactions[tempi].Reactants[tempj] != 0)
				{
					TempRow[tempj] = 1;
				}
				if(Reaction_Mechanism.Reactions[tempi].Products[tempj] != 0)
				{
					TempRow[tempj] = 1;
				}
			}
			TempMatrix.push_back(TempRow);
			TempRow.clear();
		}

		int Number_Of_Selected_Species_Temp = (int) InitialParameters.MechanismAnalysis.SpeciesSelectedForRates.size();

		for(tempj=0;tempj<Number_Of_Selected_Species_Temp;tempj++)
		{
			int SpeciesID = InitialParameters.MechanismAnalysis.SpeciesSelectedForRates[tempj];
			vector< int > temp;

			for(tempi=0;tempi<(int)Reaction_Mechanism.Reactions.size();tempi++)
			{
				if(TempMatrix[tempi][SpeciesID] !=0 )
				{
					temp.push_back(tempi);
				}

			}
			ReactionsForSpeciesSelectedForRates.push_back(temp);
			temp.clear();
		}

		//*
		Prepare_Print_Rates_Per_Species(
				ProductsForRatesAnalysis,
				ReactantsForReactions,
				InitialParameters.Solver_Parameters.separator,
				Rates,
				Reaction_Mechanism.Species,
				InitialParameters.MechanismAnalysis.SpeciesSelectedForRates,
				ReactionsForSpeciesSelectedForRates,
				Reaction_Mechanism.Reactions
		);//*/
	}


	vector< double > SpeciesConcentrationChange = SpeciesLossRate(Number_Species, Rates, SpeciesLossAll);


	/* -- Got values at t = 0 -- */


	// enables reset of Rates Analysis
	int RatesAnalysisTimepoint = 0;

	// set the solver:
	bool Use_Analytical_Jacobian;
	Use_Analytical_Jacobian = InitialParameters.Solver_Parameters.Use_Analytical_Jacobian;

	// start the clock:
	cpu_time_begin = cpu_time_current = clock();


	do
	{
		time_step = time_current + time_step1;

		if(Use_Analytical_Jacobian)
		{
			int JT = 1;
			dlsoda_((void*) ODE_RHS_Liquid_Phase,&n,y,&time_current,&time_step,
					&ITOL,&RTOL,&ATOL,&ITASK,&ISTATE,&IOPT,RWORK,&LRW,IWORK,&LIW,
					(void*) Jacobian_Matrix_Odepack_LSODA,&JT);
		}
		else //if(!Use_Analytical_Jacobian)
		{
			int JT = 2;
			dlsoda_((void*) ODE_RHS_Liquid_Phase,&n,y,&time_current,&time_step,
					&ITOL,&RTOL,&ATOL,&ITASK,&ISTATE,&IOPT,RWORK,&LRW,IWORK,&LIW,
					(void*) Jacobian_Matrix_Odepack_LSODA,&JT);
		}

		if (ISTATE < 0)
		{
			printf("\n LSODA routine exited with error code %4d\n",ISTATE);
			// Break means it should leave the do loop which would be fine for an error response as it stops the solver
			break ;
		}


		if(InitialParameters.MechanismAnalysis.MaximumRates)
		{
			MaxRatesAnalysis(RatesAnalysisData,ProductsForRatesAnalysis,ReactantsForReactions,Rates,time_current);
		}


		if(InitialParameters.MechanismAnalysis.RatesAnalysisAtTime &&
				InitialParameters.MechanismAnalysis.RatesAnalysisAtTimeData[RatesAnalysisTimepoint] == time_current)
		{
			RatesAnalysisAtTimes(
					ProductsForRatesAnalysis,
					ReactantsForReactions,
					Rates,
					time_current,
					Reaction_Mechanism.Species,
					Reaction_Mechanism.Reactions
			);
			RatesAnalysisTimepoint = RatesAnalysisTimepoint + 1;
		}


		// Function for new per species output
		if(InitialParameters.MechanismAnalysis.RatesOfSpecies)
		{
			Print_Rates_Per_Species(
					ProductsForRatesAnalysis,
					ReactantsForReactions,
					InitialParameters.Solver_Parameters.separator,
					Rates,
					time_current,
					Reaction_Mechanism.Species,
					InitialParameters.MechanismAnalysis.SpeciesSelectedForRates,
					ReactionsForSpeciesSelectedForRates,
					Reaction_Mechanism.Reactions
			);
		}


		if(InitialParameters.MechanismAnalysis.StreamRatesAnalysis)
		{
			StreamRatesAnalysis(
					OutputFilenames.Prefix,
					ProductsForRatesAnalysis,
					ReactantsForReactions,
					Rates,
					time_current,
					Number_Species
			);
		}


		double pressure = 0;
		/*
		if(InitialParameters.GasPhase)
		{
			double R = 8.31451; // Gas Constant
			// Pressure Tracking for Gas Phase Kinetics
		double total_mol = 0;
		for(i=0;i<Number_Species;i++)
		{
			total_mol = total_mol + SpeciesConcentration[i];
		}
		pressure = (total_mol  * R * SpeciesConcentration[Number_Species])/InitialParameters.GasPhaseVolume;
		}//*/

		StreamConcentrations(
				ConcentrationOutput,
				InitialParameters.Solver_Parameters.separator,
				false, //InitialParameters.GasPhase,
				Number_Species,
				time_current,
				pressure,
				SpeciesConcentration
		);

		if(InitialParameters.PrintReacRates)
		{
			StreamReactionRates(
					ReactionRatesOutput,
					InitialParameters.Solver_Parameters.separator,
					time_current,
					Rates
			);
		}


		if(InitialParameters.MechanismReduction.ReduceReactions != 0)
		{
			ReactionRateImportance(KeyRates, Rates, InitialParameters.MechanismReduction.ReduceReactions);
		}


		if(tracker < (TimeChanges-1) && time_step >= InitialParameters.TimeEnd[tracker])
		{
			cout << "CPU Time: " << ((double) (clock() - cpu_time_current)) / CLOCKS_PER_SEC << " seconds\n";
			cpu_time_current = clock();

			tracker = tracker + 1;
			time_step1 = InitialParameters.TimeStep[tracker];
			time_end = InitialParameters.TimeEnd[tracker];
			cout << "End Time: " << time_end << " Time Step: " << time_step1 << "\n";
		}


	} while (time_step < time_end);


	cout << "CPU Time: " << ((double) (clock() - cpu_time_current)) / CLOCKS_PER_SEC << " seconds\n";

	// close output files
	ConcentrationOutput.close();
	ReactionRatesOutput.close();

	if(InitialParameters.MechanismAnalysis.MaximumRates)
	{
		WriteMaxRatesAnalysis(RatesAnalysisData, Reaction_Mechanism.Species, Reaction_Mechanism.Reactions,OutputFilenames.Prefix);
	}


	// stop the clock
	cpu_time_end = clock();
	cout << "\nTotal CPU time: " << ((double) (cpu_time_end - cpu_time_begin)) / CLOCKS_PER_SEC << " seconds\n\n";
}




