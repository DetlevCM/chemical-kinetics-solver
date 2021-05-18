/*
 * liquid_phase_odepack.cpp
 *
 *  Created on: 02.07.2015
 *      Author: DetlevCM
 */
#include "../headers/Headers.hpp"




// Not a perfect solution, but stick integrator into its own void with global variables via a namespace
void Integrate_Liquid_Phase(
		Filenames OutputFilenames,
		vector< double > SpeciesConcentration,
		Reaction_Mechanism reaction_mechanism,
		Initial_Data InitialParameters,
		vector< double >& KeyRates,
		vector< vector < str_RatesAnalysis > >& RatesAnalysisData
)
{

	vector< TrackSpecies > ProductsForRatesAnalysis;

	//using namespace Jacobian_ODE_RHS;
	//using namespace ODE_RHS;
	using namespace Jacobian;


	Jacobian_ODE_RHS::Number_Species = reaction_mechanism.Species.size();
	ODE_RHS::Number_Reactions = reaction_mechanism.Reactions.size();

	// outputting mechanism size in integration routing so that it is printed every time
	cout << "The mechanism to be integrated contains " << Jacobian_ODE_RHS::Number_Species << " species and " << ODE_RHS::Number_Reactions << " Reactions.\n" << std::flush;


	Jacobian_ODE_RHS::Thermodynamics = reaction_mechanism.Thermodynamics; // "Hack" - to fix a regression


	ofstream ReactionRatesOutput;
	ofstream ConcentrationOutput (OutputFilenames.Species.c_str(),ios::app);

	if(InitialParameters.PrintReacRates)
	{
		ReactionRatesOutput.open(OutputFilenames.Rates.c_str(),ios::app);
	}

	Settings_LSODA LSODA;
	Settings_Intel Intel;

	// general variables
	size_t i;
	int n;

	// intel ODE wants int not size_t
	n = (int) Jacobian_ODE_RHS::Number_Species + 1;


	// this function will prepare the required settings. Only the required class is updated.
	int solver_choice = 0;
	solver_choice = Prepare_Integrator_Settings(
			InitialParameters,
			Jacobian_ODE_RHS::Number_Species,
			LSODA,Intel
	);



	// For performance assessment, use a clock:
	clock_t cpu_time_begin, cpu_time_end, cpu_time_current;



	Jacobian_ODE_RHS::Delta_N = Get_Delta_N(reaction_mechanism.Reactions); // just make sure the Delta_N is current
	// Reduce the matrix from a sparse matrix to something more manageable and quicker to use


	Jacobian_ODE_RHS::ReactionParameters = Process_Reaction_Parameters(reaction_mechanism.Reactions);

	ODE_RHS::ReactantsForReactions = Reactants_ForReactionRate(reaction_mechanism.Reactions);

	ODE_RHS::ProductsForReactions = Products_ForReactionRate(reaction_mechanism.Reactions,false);


	if(InitialParameters.MechanismAnalysis.MaximumRates ||
			InitialParameters.MechanismAnalysis.StreamRatesAnalysis ||
			InitialParameters.MechanismAnalysis.RatesAnalysisAtTime ||
			InitialParameters.MechanismAnalysis.RatesOfSpecies)
	{
		ProductsForRatesAnalysis = Products_ForReactionRate(reaction_mechanism.Reactions,true);
	}

	Jacobian_ODE_RHS::SpeciesLossAll = PrepareSpecies_ForSpeciesLoss(reaction_mechanism.Reactions); // New method of listing species


	Jacobian_ODE_RHS::Concentration.clear(); // ensure the concentrations array is empty
	Jacobian_ODE_RHS::Concentration = SpeciesConcentration; // set it to the initial values, also ensures it has the right length
	//double* y = Jacobian_ODE_RHS::Concentration.data(); //SpeciesConcentration.data();
	//cout << SpeciesConcentration.size() << " " << Number_Species << " " << n << "\n";

	double time_current, time_step, time_step1, time_end;

	time_current = 0; // -> Solver is designed for t_0 = 0
	time_step1 = InitialParameters.TimeStep[0];
	time_end = InitialParameters.TimeEnd[0];
	size_t TimeChanges = InitialParameters.TimeStep.size();
	size_t tracker = 0;

	//cout << "\nEnd Time: " << time_end << " Time Step: " << time_step1 << "\n";

	/* -- Initial values at t = 0 -- */

	ODE_RHS::Number_Reactions = Jacobian_ODE_RHS::ReactionParameters.size();

	Jacobian_ODE_RHS::CalculatedThermo.resize(Jacobian_ODE_RHS::Number_Species);

	ODE_RHS::InitialDataConstants.EnforceStability = InitialParameters.EnforceStability;
	ODE_RHS::InitialDataConstants.temperature = InitialParameters.temperature;

	// in case we want to simulate a TGA
	if(InitialParameters.TGA_used)
	{
		ODE_RHS::InitialDataConstants.TGA_used = InitialParameters.TGA_used;
		ODE_RHS::InitialDataConstants.TGA_rate = InitialParameters.TGA_rate;
		ODE_RHS::InitialDataConstants.TGA_target = InitialParameters.TGA_target;
	}

	// set constant concentration if desired
	ODE_RHS::InitialDataConstants.ConstantConcentration = InitialParameters.ConstantConcentration;
	if(InitialParameters.ConstantConcentration)
	{
		cout << "Constant Species desired\n";

		ODE_RHS::InitialDataConstants.ConstantSpecies.clear();
		ODE_RHS::InitialDataConstants.ConstantSpecies.resize(Jacobian_ODE_RHS::Number_Species);

		// zero just to make sure
		for(i=0;i<Jacobian_ODE_RHS::Number_Species;i++)
		{
			ODE_RHS::InitialDataConstants.ConstantSpecies[i] = 0;
		}

		for(i=0;i<InitialParameters.ConstantSpecies.size();i++)
		{// fix initial concentrations
			ODE_RHS::InitialDataConstants.ConstantSpecies[InitialParameters.ConstantSpecies[i]] =
					SpeciesConcentration[InitialParameters.ConstantSpecies[i]];
		}
	}

	Evaluate_Thermodynamic_Parameters(Jacobian_ODE_RHS::CalculatedThermo, Jacobian_ODE_RHS::Thermodynamics, SpeciesConcentration[Jacobian_ODE_RHS::Number_Species]);

	/*for(i=0;i<Number_Species;i++)
	{
	cout << CalculatedThermo[i].Hf << " " << CalculatedThermo[i].Cp << " " << CalculatedThermo[i].S <<"\n";
	}//*/

	Jacobian_ODE_RHS::Kf.clear();
	Jacobian_ODE_RHS::Kr.clear();
	Jacobian_ODE_RHS::Kf.resize(ODE_RHS::Number_Reactions);
	Jacobian_ODE_RHS::Kr.resize(ODE_RHS::Number_Reactions);

	for(i=0;i<ODE_RHS::Number_Reactions;i++)
	{
		Jacobian_ODE_RHS::Kr[i] = 0;
	}
	ODE_RHS::Rates.resize(ODE_RHS::Number_Reactions);


	// prepare the jacobian matrix
	Prepare_Jacobian_Matrix(JacobianMatrix,reaction_mechanism.Reactions);


	// Get the rate Constants, forward and backwards
	Calculate_Rate_Constant(Jacobian_ODE_RHS::Kf, Jacobian_ODE_RHS::Kr, SpeciesConcentration[Jacobian_ODE_RHS::Number_Species],Jacobian_ODE_RHS::ReactionParameters, Jacobian_ODE_RHS::CalculatedThermo, Jacobian_ODE_RHS::SpeciesLossAll, Jacobian_ODE_RHS::Delta_N);
	CalculateReactionRates(ODE_RHS::Rates, SpeciesConcentration, Jacobian_ODE_RHS::Kf, Jacobian_ODE_RHS::Kr, ODE_RHS::ReactantsForReactions, ODE_RHS::ProductsForReactions);

	// Don't forget Rates Analysis for Mechanism Recution at t=0 - or is this nonsense?
	if(InitialParameters.MechanismReduction.ReduceReactions != 0)
	{
		ReactionRateImportance(KeyRates, ODE_RHS::Rates, InitialParameters.MechanismReduction.ReduceReactions);
	}


	// do not forget output at time = 0
	StreamConcentrations(
			ConcentrationOutput,
			InitialParameters.Solver_Parameters.separator,
			InitialParameters.GasPhase,
			Jacobian_ODE_RHS::Number_Species,
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
				ODE_RHS::Rates
		);
	}

	// not happy with this more widely available, needs a cleanup...
	vector< vector< size_t > > ReactionsForSpeciesSelectedForRates;
	// Not the best place to put it, but OK for now:
	if(InitialParameters.MechanismAnalysis.RatesOfSpecies)
	{
		size_t tempi, tempj;

		vector< vector< size_t > > TempMatrix;
		vector< size_t > TempRow;
		size_t Temp_Number_Species = reaction_mechanism.Species.size();

		for(tempi=0;tempi<reaction_mechanism.Reactions.size();tempi++){
			TempRow.resize(reaction_mechanism.Species.size());
			for(tempj=0;tempj<Temp_Number_Species;tempj++)
			{
				if(reaction_mechanism.Reactions[tempi].Reactants[tempj] != 0)
				{
					TempRow[tempj] = 1;
				}
				if(reaction_mechanism.Reactions[tempi].Products[tempj] != 0)
				{
					TempRow[tempj] = 1;
				}
			}
			TempMatrix.push_back(TempRow);
			TempRow.clear();
		}

		size_t Number_Of_Selected_Species_Temp = InitialParameters.MechanismAnalysis.SpeciesSelectedForRates.size();

		for(tempj=0;tempj<Number_Of_Selected_Species_Temp;tempj++)
		{
			size_t SpeciesID = InitialParameters.MechanismAnalysis.SpeciesSelectedForRates[tempj];
			vector< size_t > temp;

			for(tempi=0;tempi<reaction_mechanism.Reactions.size();tempi++)
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
				InitialParameters.Solver_Parameters.separator,
				reaction_mechanism.Species,
				InitialParameters.MechanismAnalysis.SpeciesSelectedForRates,
				ReactionsForSpeciesSelectedForRates
		);
	}


	vector< double > SpeciesConcentrationChange = SpeciesLossRate(Jacobian_ODE_RHS::Number_Species, ODE_RHS::Rates, Jacobian_ODE_RHS::SpeciesLossAll);


	/* -- Got values at t = 0 -- */


	// enables reset of Rates Analysis
	size_t RatesAnalysisTimepoint = 0;

	// start the clock:
	cpu_time_begin = cpu_time_current = clock();


	do
	{
		time_step = time_current + time_step1;


		// https://en.cppreference.com/w/cpp/language/switch
		switch(solver_choice) // begin ODE Solver switch
		{


		// case IntelODE
		case 1001:
			dodesol_rkm9mkn(Intel.vector_ipar.data(), &n, &time_current, &time_step,
					SpeciesConcentration.data(),(void*) ODE_RHS_Liquid_Phase,
					&Intel.h, &Intel.hm, &Intel.ep, &Intel.tr,
					Intel.vector_dpar.data(), Intel.vector_kd.data(), &Intel.ierr
			);
			if (Intel.ierr!=0){printf("\n dodesol_rkm9mkn routine exited with error code %4d\n",Intel.ierr);exit(1);}
			break;

		case 1002 :
			dodesol_rkm9mka(Intel.vector_ipar.data(), &n, &time_current, &time_step,
					SpeciesConcentration.data(),(void*) ODE_RHS_Liquid_Phase,
					(void*) Jacobian_Matrix_Intel, &Intel.h, &Intel.hm, &Intel.ep, &Intel.tr,
					Intel.vector_dpar.data(), Intel.vector_kd.data(), &Intel.ierr
			);
			if (Intel.ierr!=0){printf("\n dodesol_rkm9mkn routine exited with error code %4d\n",Intel.ierr);exit(1);}
			break;

		case 1003 :
			dodesol_mk52lfn(Intel.vector_ipar.data(), &n, &time_current, &time_step,
					SpeciesConcentration.data(),(void*) ODE_RHS_Liquid_Phase,
					&Intel.h, &Intel.hm, &Intel.ep, &Intel.tr,
					Intel.vector_dpar.data(), Intel.vector_kd.data(), &Intel.ierr
			);
			if (Intel.ierr!=0){printf("\n dodesol_rkm9mkn routine exited with error code %4d\n",Intel.ierr);exit(1);}
			break;

		case 1004 :
			dodesol_mk52lfa(Intel.vector_ipar.data(), &n, &time_current, &time_step,
					SpeciesConcentration.data(),(void*) ODE_RHS_Liquid_Phase,
					(void*) Jacobian_Matrix_Intel, &Intel.h, &Intel.hm, &Intel.ep, &Intel.tr,
					Intel.vector_dpar.data(), Intel.vector_kd.data(), &Intel.ierr
			);
			if (Intel.ierr!=0){printf("\n dodesol_rkm9mkn routine exited with error code %4d\n",Intel.ierr);exit(1);}
			break;


			// use LSODA
		case 2001 :
			dlsoda_((void*) ODE_RHS_Liquid_Phase,&n,SpeciesConcentration.data(),&time_current,&time_step,
					&LSODA.ITOL,&LSODA.RTOL,&LSODA.ATOL,
					&LSODA.ITASK,&LSODA.ISTATE,&LSODA.IOPT,
					LSODA.vector_RWORK.data(),&LSODA.LRW,LSODA.vector_IWORK.data(),&LSODA.LIW,
					(void*) Jacobian_Matrix_Odepack_LSODA,&LSODA.JT);
			if (LSODA.ISTATE<0){printf("\n LSODA routine exited with error code %4d\n",LSODA.ISTATE);exit(1);}
			break;
		case 2002 :
			dlsoda_((void*) ODE_RHS_Liquid_Phase,&n,SpeciesConcentration.data(),&time_current,&time_step,
					&LSODA.ITOL,&LSODA.RTOL,&LSODA.ATOL,
					&LSODA.ITASK,&LSODA.ISTATE,&LSODA.IOPT,
					LSODA.vector_RWORK.data(),&LSODA.LRW,LSODA.vector_IWORK.data(),&LSODA.LIW,
					(void*) Jacobian_Matrix_Odepack_LSODA,&LSODA.JT);
			if (LSODA.ISTATE<0){printf("\n LSODA routine exited with error code %4d\n",LSODA.ISTATE);exit(1);}
			break;


		} // end of ODE solver switch



		if(InitialParameters.MechanismAnalysis.MaximumRates)
		{
			MaxRatesAnalysis(RatesAnalysisData,ProductsForRatesAnalysis,ODE_RHS::ReactantsForReactions,ODE_RHS::Rates,time_current);
		}


		if(InitialParameters.MechanismAnalysis.RatesAnalysisAtTime &&
				InitialParameters.MechanismAnalysis.RatesAnalysisAtTimeData[RatesAnalysisTimepoint] == time_current)
		{
			RatesAnalysisAtTimes(
					ProductsForRatesAnalysis,
					ODE_RHS::ReactantsForReactions,
					ODE_RHS::Rates,
					time_current,
					reaction_mechanism.Species,
					reaction_mechanism.Reactions
			);
			RatesAnalysisTimepoint = RatesAnalysisTimepoint + 1;
		}


		// Function for new per species output
		if(InitialParameters.MechanismAnalysis.RatesOfSpecies)
		{
			Print_Rates_Per_Species(
					ProductsForRatesAnalysis,
					ODE_RHS::ReactantsForReactions,
					InitialParameters.Solver_Parameters.separator,
					ODE_RHS::Rates,
					time_current,
					reaction_mechanism.Species,
					InitialParameters.MechanismAnalysis.SpeciesSelectedForRates,
					ReactionsForSpeciesSelectedForRates//,
					//reaction_mechanism.Reactions
			);
		}


		if(InitialParameters.MechanismAnalysis.StreamRatesAnalysis)
		{
			StreamRatesAnalysis(
					OutputFilenames.Prefix,
					ProductsForRatesAnalysis,
					ODE_RHS::ReactantsForReactions,
					ODE_RHS::Rates,
					time_current,
					Jacobian_ODE_RHS::Number_Species
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
				Jacobian_ODE_RHS::Number_Species,
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
					ODE_RHS::Rates
			);
		}


		if(InitialParameters.MechanismReduction.ReduceReactions != 0)
		{
			ReactionRateImportance(KeyRates, ODE_RHS::Rates, InitialParameters.MechanismReduction.ReduceReactions);
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
	// end of loop from t_zero to t_end


	cout << "CPU Time: " << ((double) (clock() - cpu_time_current)) / CLOCKS_PER_SEC << " seconds\n";

	// close output files
	ConcentrationOutput.close();
	ReactionRatesOutput.close();

	if(InitialParameters.MechanismAnalysis.MaximumRates)
	{
		WriteMaxRatesAnalysis(RatesAnalysisData, reaction_mechanism.Species, reaction_mechanism.Reactions,OutputFilenames.Prefix);
	}


	// stop the clock
	cpu_time_end = clock();
	cout << "\nTotal CPU time: " << ((double) (cpu_time_end - cpu_time_begin)) / CLOCKS_PER_SEC << " seconds\n\n";
}




