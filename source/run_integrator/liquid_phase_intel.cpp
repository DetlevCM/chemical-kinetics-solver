/*
 * Run_Integrator.cpp
 *
 *  Created on: 02.07.2015
 *      Author: DetlevCM
 */

#include "MyHeaders.h"



// Not a perfect solution, but stick integrator into its own void with global variables via a namespace
void Integrate_Liquid_Phase_Intel(
		Filenames OutputFilenames,
		vector< double > SpeciesConcentration,
		vector< string > Species,
		vector< ThermodynamicData > Thermo,
		vector< SingleReactionData >& Reactions,
		InitParam InitialParameters,
		vector< double >& KeyRates,
		vector< vector < str_RatesAnalysis > >& RatesAnalysisData
)
{


	vector< TrackSpecies > ProductsForRatesAnalysis;

	using namespace Jacobian_ODE_RHS;
	using namespace ODE_RHS;
	using namespace Jacobian;

	Number_Species = (int)Species.size();
	Number_Reactions = (int)Reactions.size();

	// outputting mechanism size in integration routing so that it is printed every time
	cout << "The mechanism to be integrated contains " << Number_Species << " species and " << Number_Reactions << " Reactions.\n" << std::flush;


	Thermodynamics = Thermo; // "Hack" - to fix a regression


	ofstream ReactionRatesOutput;
	ofstream ConcentrationOutput (OutputFilenames.Species.c_str(),ios::app);

	if(InitialParameters.PrintReacRates)
	{
		ReactionRatesOutput.open(OutputFilenames.Rates.c_str(),ios::app);
	}

	// This works :) good - well, need it soon...
	//vector< ofstream > RatesOfSpecies;

	int n, ierr, ipar[128], dpar_size;
	double h, hm, ep, tr;
	int i;

	/* Because we do not know the size of kd and dpar in advance we need to be clever - or at least act as we are.
	 * By defining kd and dpar as vectors we can assign a size "on the fly" as required. We can further pass a
	 * pointer to the vector to the solver, thus this is what we do. Rather than arrays, kd and dpar are pointers
	 * to vectors of doubles in our case.
	 */


	vector<int> kdstep(Number_Species + 1);


	int* kd = &kdstep[0];

	n = Number_Species + 1;
	if (13 * n > (7 + 2 * n) * n) {
		dpar_size = 13 * n;
	} else {
		dpar_size = (7 + 2 * n) * n;
	}
	vector<double> dparstep(dpar_size);
	double* dpar = &dparstep[0];


	// For performance assessment, use a clock:
	clock_t cpu_time_begin, cpu_time_end, cpu_time_current;

	// Some tolerances for the solver:
	ep = InitialParameters.Solver_Parameters.rtol ; // relative tolerance. The code cannot guarantee the requested accuracy for ep<1.d-9
	tr = InitialParameters.Solver_Parameters.atol / InitialParameters.Solver_Parameters.rtol;
	h = InitialParameters.Solver_Parameters.initial_stepsize;
	hm = InitialParameters.Solver_Parameters.minimum_stepsize; // minimal step size for the methods, 1.0e-12 recommended for normalised problems

	Delta_N = Get_Delta_N(Reactions); // just make sure the Delta_N is current
	// Reduce the matrix from a sparse matrix to something more manageable and quicker to use


	ReactionParameters = Process_Reaction_Parameters(Reactions);

	ReactantsForReactions = Reactants_ForReactionRate(Reactions);

	ProductsForReactions = Products_ForReactionRate(Reactions,false);


	if(InitialParameters.MechanismAnalysis.MaximumRates ||
			InitialParameters.MechanismAnalysis.StreamRatesAnalysis ||
			InitialParameters.MechanismAnalysis.RatesAnalysisAtTime ||
			InitialParameters.MechanismAnalysis.RatesOfSpecies)
	{
		ProductsForRatesAnalysis = Products_ForReactionRate(Reactions,true);
	}

	SpeciesLossAll = PrepareSpecies_ForSpeciesLoss(Reactions); // New method of listing species

	// And now it is time to call the solver again... with the right information...
	// According to Intel initialise ipar array with zeros before the first call to dodesol
	for (i = 0; i < 128; i++) {
		ipar[i] = 0;
	}


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


	Calculate_Thermodynamics(CalculatedThermo, SpeciesConcentration[Number_Species], Thermodynamics);

	/*
	for(i=0;i<Number_Species;i++)
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
	Prepare_Jacobian_Matrix(JacobianMatrix,Reactions,Species);


	// Get the rate Constants, forward and backwards
	Calculate_Rate_Constant(Kf, Kr, SpeciesConcentration[Number_Species],ReactionParameters, CalculatedThermo, SpeciesLossAll, Delta_N);
	CalculateReactionRates(Rates, SpeciesConcentration, Kf, Kr, ReactantsForReactions, ProductsForReactions);

	// Don't forget Rates Analysis for Mechanism Recution at t=0 - or is this nonsense?
	if(InitialParameters.ReduceReactions != 0)
	{
		ReactionRateImportance(KeyRates, Rates, InitialParameters.ReduceReactions);
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
		vector< int > TempRow;//((int)Species.size());
		int Temp_Number_Species = (int) Species.size();

		for(tempi=0;tempi<(int)Reactions.size();tempi++){
			TempRow.resize((int)Species.size());
			for(tempj=0;tempj<Temp_Number_Species;tempj++)
			{
				if(Reactions[tempi].Reactants[tempj] != 0)
				{
					TempRow[tempj] = 1;
				}
				if(Reactions[tempi].Products[tempj] != 0)
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

			for(tempi=0;tempi<(int)Reactions.size();tempi++)
			{
				if(TempMatrix[tempi][SpeciesID] !=0 )
				{
					temp.push_back(tempi);
				}
			}
			ReactionsForSpeciesSelectedForRates.push_back(temp);
			temp.clear();
		}


		Prepare_Print_Rates_Per_Species(
				ProductsForRatesAnalysis,
				ReactantsForReactions,
				InitialParameters.Solver_Parameters.separator,
				Rates,
				Species,
				InitialParameters.MechanismAnalysis.SpeciesSelectedForRates,
				ReactionsForSpeciesSelectedForRates,
				Reactions
		);
	}


	vector< double > SpeciesConcentrationChange = SpeciesLossRate(Number_Species, Rates, SpeciesLossAll);


	/* -- Got values at t = 0 -- */

	// Resize the vector for the concentrations in the ODE void
	//Concentration.clear(); // don't think it makes a difference if I clear or not
	//Concentration.resize(Number_Species + 1);

	// enables reset of Rates Analysis
	int RatesAnalysisTimepoint = 0;


	// set the solver:
	solver_type Solver_Type;
	Solver_Type = InitialParameters.Solver_Type[0];

	// start the clock:
	cpu_time_begin = cpu_time_current = clock();


	// lets split this into two parts:
	// 1: regular solver
	// 2: PetroOxy settings


	do
	{
		time_step = time_current + time_step1;

		if(!Solver_Type.Use_Stiff_Solver && Solver_Type.Use_Analytical_Jacobian)
		{
			dodesol_rkm9mka(ipar, &n, &time_current, &time_step, y,(void*) ODE_RHS_Liquid_Phase,
					(void*) Jacobian_Matrix_Intel, &h, &hm, &ep, &tr, dpar, kd, &ierr
			);
		}

		if(Solver_Type.Use_Stiff_Solver && Solver_Type.Use_Analytical_Jacobian)
		{
			// stiff solver with automatics numerical Jacobi matric computations
			dodesol_mk52lfa(ipar, &n, &time_current, &time_step, y,(void*) ODE_RHS_Liquid_Phase,
					(void*) Jacobian_Matrix_Intel, &h, &hm, &ep, &tr, dpar, kd, &ierr
			);
		}

		if(Solver_Type.Use_Stiff_Solver && !Solver_Type.Use_Analytical_Jacobian)
		{
			// stiff solver with automatics numerical Jacobi matric computations
			dodesol_mk52lfn(ipar, &n, &time_current, &time_step, y,(void*) ODE_RHS_Liquid_Phase,
					&h, &hm, &ep, &tr, dpar, kd, &ierr
			);
		}

		if(!Solver_Type.Use_Stiff_Solver && !Solver_Type.Use_Analytical_Jacobian)
		{
			// hybrid solver with automatic numerical Jacobi matrix computations
			dodesol_rkm9mkn(ipar, &n, &time_current, &time_step, y,(void*) ODE_RHS_Liquid_Phase,
					&h, &hm, &ep, &tr, dpar, kd, &ierr
			);
		}


		if (ierr != 0)
		{
			printf("\n dodesol_rkm9mkn routine exited with error code %4d\n",ierr);
			//return -1;
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
					Species,
					Reactions
			);

			RatesAnalysisTimepoint = RatesAnalysisTimepoint + 1;
		}


		// Function for new per species output
		//*
		if(InitialParameters.MechanismAnalysis.RatesOfSpecies)
		{
			Print_Rates_Per_Species(
					ProductsForRatesAnalysis,
					ReactantsForReactions,
					InitialParameters.Solver_Parameters.separator,
					Rates,
					time_current,
					Species,
					InitialParameters.MechanismAnalysis.SpeciesSelectedForRates,
					ReactionsForSpeciesSelectedForRates,
					Reactions
			);
		}
		//*/

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
		if(InitialParameters.GasPhase)
		{
			double R = 8.31451; // Gas Constant
			/* Pressure Tracking for Gas Phase Kinetics */
			double total_mol = 0;
			for(i=0;i<Number_Species;i++)
			{
				total_mol = total_mol + SpeciesConcentration[i];
			}
			pressure = (total_mol  * R * SpeciesConcentration[Number_Species])/InitialParameters.GasPhaseVolume;

		}

		StreamConcentrations(
				ConcentrationOutput,
				InitialParameters.Solver_Parameters.separator,
				InitialParameters.GasPhase,
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


		if(InitialParameters.ReduceReactions != 0)
		{
			ReactionRateImportance(KeyRates, Rates, InitialParameters.ReduceReactions);
		}


		if(tracker < (TimeChanges-1) && time_step >= InitialParameters.TimeEnd[tracker])
		{
			cout << "CPU Time: " << ((double) (clock() - cpu_time_current)) / CLOCKS_PER_SEC << " seconds\n";
			cpu_time_current = clock();

			tracker = tracker + 1;
			time_step1 = InitialParameters.TimeStep[tracker];
			time_end = InitialParameters.TimeEnd[tracker];
			cout << "End Time: " << time_end << " Time Step: " << time_step1 << "\n";

			// update the solver settings
			Solver_Type = InitialParameters.Solver_Type[tracker];
			cout << "New Solver Settings:\nJacobian: " << Solver_Type.Use_Analytical_Jacobian <<
					"\nUse Stiff Solver: " << Solver_Type.Use_Stiff_Solver <<"\n";
		}


	} while (time_step < time_end);


	cout << "CPU Time: " << ((double) (clock() - cpu_time_current)) / CLOCKS_PER_SEC << " seconds\n";

	// close output files
	ConcentrationOutput.close();
	ReactionRatesOutput.close();

	if(InitialParameters.MechanismAnalysis.MaximumRates)
	{
		WriteMaxRatesAnalysis(RatesAnalysisData, Species, Reactions,OutputFilenames.Prefix);
	}


	// stop the clock
	cpu_time_end = clock();
	cout << "\nTotal CPU time: " << ((double) (cpu_time_end - cpu_time_begin)) / CLOCKS_PER_SEC << " seconds\n\n";
}




