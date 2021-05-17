/*
 * Run_Integrator.cpp
 *
 *  Created on: 02.07.2015
 *      Author: DetlevCM
 */

#include "../headers/Headers.hpp"



// Not a perfect solution, but stick integrator into its own void with global variables via a namespace
void Integrate_Pressure_Vessel_Liquid_Phase(
		Filenames OutputFilenames,
		vector< double > SpeciesConcentration,
		Reaction_Mechanism reaction_mechanism,
		//vector< string > Species,vector< ThermodynamicData > Thermo,vector< SingleReactionData >& Reactions,
		Initial_Data InitialParameters,
		vector< double >& KeyRates,
		PressureVesselCalc PetroOxyDataInput,
		vector< vector < str_RatesAnalysis > >& RatesAnalysisData
)
{


	vector< TrackSpecies > ProductsForRatesAnalysis;

	using namespace Jacobian_ODE_RHS;
	using namespace ODE_RHS;
	using namespace ODE_RHS_Pressure_Vessel_Variables;
	using namespace Jacobian;

	Number_Species = reaction_mechanism.Species.size();
	Number_Reactions = reaction_mechanism.Reactions.size();

	// outputting mechanism size in integration routing so that it is printed every time
	cout << "The mechanism to be integrated contains " << Number_Species << " species and " << Number_Reactions << " Reactions.\n" << std::flush;


	Thermodynamics = reaction_mechanism.Thermodynamics; // "Hack" - to fix a regression


	ofstream ReactionRatesOutput;
	ofstream ConcentrationOutput (OutputFilenames.Species.c_str(),ios::app);

	if(InitialParameters.PrintReacRates)
	{
		ReactionRatesOutput.open(OutputFilenames.Rates.c_str(),ios::app);
	}

	// This works :) good - well, need it soon...
	//vector< ofstream > RatesOfSpecies;


	Settings_LSODA LSODA;
	Settings_Intel Intel;

	// general variables
	size_t i;

	// intel ODE needs an interger
	int n;
	n = (int) Number_Species + 1;


	// this function will prepare the required settings. Only the required class is updated.
	int solver_choice = 0;
	solver_choice = Prepare_Integrator_Settings(
			InitialParameters,
			Number_Species,
			LSODA,Intel
	);

	// For performance assessment, use a clock:
	clock_t cpu_time_begin, cpu_time_end, cpu_time_current;


	Delta_N = Get_Delta_N(reaction_mechanism.Reactions); // just make sure the Delta_N is current
	// Reduce the matrix from a sparse matrix to something more manageable and quicker to use


	ReactionParameters = Process_Reaction_Parameters(reaction_mechanism.Reactions);

	ReactantsForReactions = Reactants_ForReactionRate(reaction_mechanism.Reactions);

	ProductsForReactions = Products_ForReactionRate(reaction_mechanism.Reactions,false);


	if(InitialParameters.MechanismAnalysis.MaximumRates ||
			InitialParameters.MechanismAnalysis.StreamRatesAnalysis ||
			InitialParameters.MechanismAnalysis.RatesAnalysisAtTime ||
			InitialParameters.MechanismAnalysis.RatesOfSpecies)
	{
		ProductsForRatesAnalysis = Products_ForReactionRate(reaction_mechanism.Reactions,true);
	}

	SpeciesLossAll = PrepareSpecies_ForSpeciesLoss(reaction_mechanism.Reactions); // New method of listing species


	// original old code
	//double* y = &SpeciesConcentration[0];
	Concentration.clear();
	Concentration.resize(Number_Species + 1);

	// Should be faster, but is 10 times slower???
	/*
	Concentration.clear();
	Concentration = SpeciesConcentration;
	//cout << Concentration.size() << "\n";
	double* y = &Concentration[0];
	//*/
	double time_current, time_step, time_step1, time_end;

	time_current = 0;// -> Solver is designed for t_0 = 0
	time_step1 = InitialParameters.TimeStep[0];
	time_end = InitialParameters.TimeEnd[0];
	size_t TimeChanges = InitialParameters.TimeStep.size();
	size_t tracker = 0;

	//cout << "\nEnd Time: " << time_end << " Time Step: " << time_step1 << "\n";

	/* -- Initial values at t = 0 -- */

	Number_Reactions = ReactionParameters.size();

	CalculatedThermo.resize(Number_Species);

	InitialDataConstants.EnforceStability = InitialParameters.EnforceStability;
	InitialDataConstants.PetroOxy = InitialParameters.PetroOxy.IsSet;
	InitialDataConstants.PetroOxyTemperatureRise = InitialParameters.PetroOxy.TemperatureRise;
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

		for(i=0;i<InitialParameters.ConstantSpecies.size();i++)
		{// fix initial concentrations
			InitialDataConstants.ConstantSpecies[InitialParameters.ConstantSpecies[i]] =
					SpeciesConcentration[InitialParameters.ConstantSpecies[i]];
		}
	}


	if(InitialParameters.PetroOxy.IsSet)
	{
		PetroOxyOutputHeader(OutputFilenames.PetroOxy);
		OxyGasSpeciesID = InitialParameters.PetroOxy.GasSpecies;
		PetroOxyData = PetroOxyDataInput;
	}
	// Oxy with temperature Rise
	if(
			InitialParameters.PetroOxy.IsSet
			&&
			InitialParameters.PetroOxy.TemperatureRise != 0) // fix temperature for Oxy, rise desired
	{
		SpeciesConcentration[Number_Species] = 298;
		// for Oxy diffusion limit, gets ignored if not required
		time_previous = 0;
	}//*/


	Evaluate_Thermodynamic_Parameters(CalculatedThermo, Thermodynamics, SpeciesConcentration[Number_Species]);

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
	Prepare_Jacobian_Matrix(JacobianMatrix,reaction_mechanism.Reactions);


	// Get the rate Constants, forward and backwards
	Calculate_Rate_Constant(Kf, Kr, SpeciesConcentration[Number_Species],ReactionParameters, CalculatedThermo, SpeciesLossAll, Delta_N);
	CalculateReactionRates(Rates, SpeciesConcentration, Kf, Kr, ReactantsForReactions, ProductsForReactions);

	// Don't forget Rates Analysis for Mechanism Recution at t=0 - or is this nonsense?
	if(InitialParameters.MechanismReduction.ReduceReactions != 0)
	{
		ReactionRateImportance(KeyRates, Rates, InitialParameters.MechanismReduction.ReduceReactions);
	}

	if(InitialParameters.PetroOxy.IsSet)
	{

		//  the PetroOxy will saturate the hydrocarbon with oxygen
		// at no loss to the reservoir
		Adjust_Gas_Concentration_Initial(
				SpeciesConcentration[OxyGasSpeciesID],
				SpeciesConcentration[Number_Species],
				PetroOxyData);

		PetroOxyOutputStream(
				OutputFilenames.PetroOxy,
				PetroOxyData,
				time_current);
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
	vector< vector< size_t > > ReactionsForSpeciesSelectedForRates;
	// Not the betst place to put it, but OK for now:
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
				//ProductsForRatesAnalysis,
				//ReactantsForReactions,
				InitialParameters.Solver_Parameters.separator,
				//Rates,
				reaction_mechanism.Species,
				InitialParameters.MechanismAnalysis.SpeciesSelectedForRates,
				ReactionsForSpeciesSelectedForRates//,
				//reaction_mechanism.Reactions
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


		// https://en.cppreference.com/w/cpp/language/switch
				switch(solver_choice) // begin ODE Solver switch
				{


				// case IntelODE
				case 1001:
					dodesol_rkm9mkn(Intel.vector_ipar.data(), &n, &time_current, &time_step,
							SpeciesConcentration.data(),(void*) ODE_RHS_Pressure_Vessel,
							&Intel.h, &Intel.hm, &Intel.ep, &Intel.tr,
							Intel.vector_dpar.data(), Intel.vector_kd.data(), &Intel.ierr
					);
					if (Intel.ierr!=0){printf("\n dodesol_rkm9mkn routine exited with error code %4d\n",Intel.ierr);exit(1);}
					break;

				case 1002 :
					dodesol_rkm9mka(Intel.vector_ipar.data(), &n, &time_current, &time_step,
							SpeciesConcentration.data(),(void*) ODE_RHS_Pressure_Vessel,
							(void*) Jacobian_Matrix_Intel, &Intel.h, &Intel.hm, &Intel.ep, &Intel.tr,
							Intel.vector_dpar.data(), Intel.vector_kd.data(), &Intel.ierr
					);
					if (Intel.ierr!=0){printf("\n dodesol_rkm9mkn routine exited with error code %4d\n",Intel.ierr);exit(1);}
					break;

				case 1003 :
					dodesol_mk52lfn(Intel.vector_ipar.data(), &n, &time_current, &time_step,
							SpeciesConcentration.data(),(void*) ODE_RHS_Pressure_Vessel,
							&Intel.h, &Intel.hm, &Intel.ep, &Intel.tr,
							Intel.vector_dpar.data(), Intel.vector_kd.data(), &Intel.ierr
					);
					if (Intel.ierr!=0){printf("\n dodesol_rkm9mkn routine exited with error code %4d\n",Intel.ierr);exit(1);}
					break;

				case 1004 :
					dodesol_mk52lfa(Intel.vector_ipar.data(), &n, &time_current, &time_step,
							SpeciesConcentration.data(),(void*) ODE_RHS_Pressure_Vessel,
							(void*) Jacobian_Matrix_Intel, &Intel.h, &Intel.hm, &Intel.ep, &Intel.tr,
							Intel.vector_dpar.data(), Intel.vector_kd.data(), &Intel.ierr
					);
					if (Intel.ierr!=0){printf("\n dodesol_rkm9mkn routine exited with error code %4d\n",Intel.ierr);exit(1);}
					break;


					// use LSODA
				case 2001 :
					dlsoda_((void*) ODE_RHS_Pressure_Vessel,&n,SpeciesConcentration.data(),&time_current,&time_step,
							&LSODA.ITOL,&LSODA.RTOL,&LSODA.ATOL,
							&LSODA.ITASK,&LSODA.ISTATE,&LSODA.IOPT,
							LSODA.vector_RWORK.data(),&LSODA.LRW,LSODA.vector_IWORK.data(),&LSODA.LIW,
							(void*) Jacobian_Matrix_Odepack_LSODA,&LSODA.JT);
					if (LSODA.ISTATE<0){printf("\n LSODA routine exited with error code %4d\n",LSODA.ISTATE);exit(1);}
					break;
				case 2002 :
					dlsoda_((void*) ODE_RHS_Pressure_Vessel,&n,SpeciesConcentration.data(),&time_current,&time_step,
							&LSODA.ITOL,&LSODA.RTOL,&LSODA.ATOL,
							&LSODA.ITASK,&LSODA.ISTATE,&LSODA.IOPT,
							LSODA.vector_RWORK.data(),&LSODA.LRW,LSODA.vector_IWORK.data(),&LSODA.LIW,
							(void*) Jacobian_Matrix_Odepack_LSODA,&LSODA.JT);
					if (LSODA.ISTATE<0){printf("\n LSODA routine exited with error code %4d\n",LSODA.ISTATE);exit(1);}
					break;


				} // end of ODE solver switch


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
					reaction_mechanism.Species,
					reaction_mechanism.Reactions
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
					reaction_mechanism.Species,
					InitialParameters.MechanismAnalysis.SpeciesSelectedForRates,
					ReactionsForSpeciesSelectedForRates//,
					//reaction_mechanism.Reactions
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


		if(InitialParameters.PetroOxy.IsSet)
		{
			PetroOxyOutputStream(
					OutputFilenames.PetroOxy,
					PetroOxyData,
					time_current
			);
		}

		if(InitialParameters.MechanismReduction.ReduceReactions != 0)
		{
			ReactionRateImportance(KeyRates, Rates, InitialParameters.MechanismReduction.ReduceReactions);
		}


		// not sure this really works how it should looking at odepack issues...
		//*
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
		}//*/


	} while (time_step < time_end);





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




