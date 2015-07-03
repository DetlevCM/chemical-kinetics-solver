#include <MyHeaders.h>

//struct SpeciesLoss;

// The actual ODEs, only required locally, therefore defined in here only
extern void ODE_RHS(int*, double*, double*, double*);
extern void Jacobian_Matrix(int*, double*, double*, double*);

/* It seems a limited number of global variables cannot be avoided.
 * The reaction scheme as well as species concentrations will need
 * to be provided as a global variable as the ODEs need access to
 * them without passing any variables to the relevant void */


namespace GlobalArrays{
// Key parameters that define the whole reaction scheme - used globally via namespaces
InitParam InitialParameters;
// Not sure if this is a good place to put it...
vector< vector < str_RatesAnalysis > > RatesAnalysisData;
vector< TrackSpecies > ProductsForRatesAnalysis;

//vector< double > JacobeanColumnWise;
vector< JacobianData > JacobianMatrix;
}



// Needed for right hand side equation for the solve to function

namespace ODESolverConstant
{
// Below Used in ODE RHS
int Number_Species;
int Number_Reactions;
vector< double > Delta_N;
vector< ThermodynamicData > Thermodynamics;
vector< ReactionParameter > ReactionParameters; // tidier than reactions vector
vector< TrackSpecies > ReactantsForReactions;
vector< TrackSpecies > ProductsForReactions;
vector< TrackSpecies > SpeciesLossAll; // vector for recording species loss
}

namespace ODESolverVariable
{
vector< vector< double > > CalculatedThermo;
vector< double > Kf;
vector< double > Kr;
vector< double > Rates;
vector< double > Concentration;
vector< double > SpeciesConcentrationChange;


//for limited Oxy
double time_previous = 0;
}

// End solver function



// By making the following global, I avoid vector re-declarations
// This has a positive impact on performance
// by sticking them in a namespace I make them more manageable
// It also allows me to size the array beforehand


// Shoddy implementation of PetroOxy logic
PetroOxyCalculation PetroOxyData;
int OxyGasSpeciesID;
// end PetroOxy Logic



void Integrate(
		string,
		string,
		string,
		string,
		vector< double >,
		vector< string > ,
		vector< SingleReactionData >&
);


// Global for now, clean up !!!!!!!!!!!
vector< double > KeyRates;



int main(int argc, char* argv[])
{

	// http://stackoverflow.com/questions/10150468/how-to-redirect-cin-and-cout-to-files
	std::ofstream out("log.txt");
	//std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(out.rdbuf());

	vector< string > Species;

	vector< SingleReactionData > Reactions;

	using namespace GlobalArrays;
	using namespace ODESolverConstant; // what is needed?

	PetroOxyCalculation PetroOxyDataInitial;

	// Globally useful counter
	int i;


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
				Species,
				Reactions
		);
	}

	Number_Species = (int) Species.size();
	Number_Reactions = (int) Reactions.size();

	// We have now pre-processed all information, time to set up the ODEs and the solver
	// Let us set up the reactions first for the ODE solver


	printf("On to  the solver. \n");


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


	Integrate(
			"concentrations.txt",
			"reaction_rates.txt",
			"PetroOxy-Log.txt",
			"",
			InitialSpeciesConcentration,
			Species,
			Reactions
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
						Species,
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
					ReducedReactions
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




// Not a perfect solution, but stick integrator into its own void with global variables via a namespace
void Integrate(
		string species_filename,
		string rates_filename,
		string petrooxy_filename,
		string rates_analysis_stream_filename,
		vector< double > SpeciesConcentration,
		vector< string > Species,
		vector< SingleReactionData >& Reactions
)
{
	using namespace GlobalArrays;

	using namespace ODESolverConstant;
	using namespace ODESolverVariable;


	ofstream ReactionRatesOutput;
	ofstream ConcentrationOutput (species_filename.c_str(),ios::app);

	if(InitialParameters.PrintReacRates)
	{
		ReactionRatesOutput.open(rates_filename.c_str(),ios::app);
	}



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
	hm = InitialParameters.hm; // minimal step size for the methods, 1.0e-12 recommended for normalised problems
	ep = InitialParameters.rtol ; // relative tolerance. The code cannot guarantee the requested accuracy for ep<1.d-9
	tr = InitialParameters.threshold; // Threshold, absolute tolerance is ep*tr
	h = InitialParameters.h;


	Delta_N = Get_Delta_N(Reactions); // just make sure the Delta_N is current
	// Reduce the matrix from a sparse matrix to something more manageable and quicker to use


	ReactionParameters = Process_Reaction_Parameters(Reactions);

	ReactantsForReactions = Reactants_ForReactionRate(Reactions);

	ProductsForReactions = Products_ForReactionRate(Reactions,false);

	if(InitialParameters.RatesMaxAnalysis || InitialParameters.StreamRatesAnalysis || InitialParameters.RatesAnalysisAtTime)
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
	double time_current, time_step, time_step1, time_end;

	time_current = 0;// -> Solver is designed for t_0 = 0
	time_step1 = InitialParameters.TimeStep[0];
	time_end = InitialParameters.TimeEnd[0];
	int TimeChanges = (int) InitialParameters.TimeStep.size();
	int tracker = 0;

	cout << "\nEnd Time: " << time_end << " Time Step: " << time_step1 << "\n";
	/* -- Initial values at t = 0 -- */

	Number_Reactions = (int) ReactionParameters.size();

	CalculatedThermo.resize(Number_Species);
	for(i=0;i<Number_Species;i++)
	{
		CalculatedThermo[i].resize(4);
	}

	// Oxy with temperature Rise
	if(
			GlobalArrays::InitialParameters.PetroOxy
			&&
			GlobalArrays::InitialParameters.PetroOxyTemperatureRise != 0) // fix temperature for Oxy, rise desired
	{
		SpeciesConcentration[Number_Species] = 298;
	}//*/


	Calculate_Thermodynamics(CalculatedThermo, SpeciesConcentration[Number_Species], Thermodynamics);

	Kf.clear();
	Kr.clear();
	Kf.resize(Number_Reactions);
	Kr.resize(Number_Reactions);

	for(i=0;i<Number_Reactions;i++)
	{
		Kr[i] = 0;
	}
	Rates.resize(Number_Reactions);


	// For the Jacobian
	Prepare_Jacobian_Matrix (
			GlobalArrays::JacobianMatrix,
			Reactions,
			Species
	);
	// Jacobian End



	// Get the rate Constants, forward and backwards
	Calculate_Rate_Constant(Kf, Kr, SpeciesConcentration[Number_Species],ReactionParameters, CalculatedThermo, SpeciesLossAll, Delta_N);
	CalculateReactionRates(Rates, SpeciesConcentration, Kf, Kr, ReactantsForReactions, ProductsForReactions);

	// Don't forget Rates Analysis for Mechanism Recution at t=0 - or is this nonsense?
	if(InitialParameters.ReduceReactions != 0)
	{
		ReactionRateImportance(KeyRates, Rates, InitialParameters.ReduceReactions);
	}

	if(InitialParameters.PetroOxy)
	{

		//  the PetroOxy will saturate the hydrocarbon with oxygen
		// at no loss to the reservoir
		Adjust_Gas_Concentration_Initial(
				SpeciesConcentration[OxyGasSpeciesID],
				SpeciesConcentration[Number_Species],
				PetroOxyData);

		PetroOxyOutputStream(
				petrooxy_filename,
				PetroOxyData,
				time_current);
	}


	// do not forget output at time = 0
	StreamConcentrations(
			ConcentrationOutput,
			InitialParameters.GasPhase,
			Number_Species,
			time_current,
			InitialParameters.GasPhasePressure,
			SpeciesConcentration
	);

	// Only stream if the user desires it, still doesn't prevent file creation...
	if(InitialParameters.PrintReacRates)
	{
		StreamReactionRates(ReactionRatesOutput, time_current, Rates);
	}

	vector< double > SpeciesConcentrationChange = SpeciesLossRate(SpeciesLossAll,Number_Species, Rates);


	/* -- Got values at t = 0 -- */

	// Resize the vector for the concentrations in the ODE void
	Concentration.clear(); // don't think it makes a difference if I clear or not
	Concentration.resize(Number_Species + 1);

	// enables reset of Rates Analysis
	int RatesAnalysisTimepoint = 0;


	// start the clock:
	cpu_time_begin = cpu_time_current = clock();


	do
	{
		time_step = time_current + time_step1;


		if(!InitialParameters.UseStiffSolver && InitialParameters.Jacobian)
		{
			dodesol_rkm9mka(ipar, &n, &time_current, &time_step, y, (void*) ODE_RHS, (void*) Jacobian_Matrix, &h, &hm, &ep, &tr, dpar, kd, &ierr);
		}

		if(InitialParameters.UseStiffSolver && InitialParameters.Jacobian)
		{
			// stiff solver with automatics numerical Jacobi matric computations
			dodesol_mk52lfa(ipar, &n, &time_current, &time_step, y, (void*) ODE_RHS, (void*) Jacobian_Matrix, &h, &hm, &ep, &tr, dpar, kd, &ierr);
		}

		if(InitialParameters.UseStiffSolver && !InitialParameters.Jacobian)
		{
			// stiff solver with automatics numerical Jacobi matric computations
			dodesol_mk52lfn(ipar, &n, &time_current, &time_step, y, (void*) ODE_RHS, &h, &hm, &ep, &tr, dpar, kd, &ierr);
		}

		if(!InitialParameters.UseStiffSolver && !InitialParameters.Jacobian)
		{
			// hybrid solver with automatic numerical Jacobi matrix computations
			dodesol_rkm9mkn(ipar, &n, &time_current, &time_step, y, (void*) ODE_RHS, &h, &hm, &ep, &tr, dpar, kd, &ierr);
		}
		if (ierr != 0)
		{
			printf("\n dodesol_rkm9mkn routine exited with error code %4d\n",ierr);
			//return -1;
			// Break means it should leave the do loop which would be fine for an error response as it stops the solver
			break ;
		}


		if(InitialParameters.RatesMaxAnalysis)
		{
			MaxRatesAnalysis(RatesAnalysisData,ProductsForRatesAnalysis,ReactantsForReactions,Rates,time_current);
		}


		if(InitialParameters.RatesAnalysisAtTime &&
				InitialParameters.RatesAnalysisAtTimeData[RatesAnalysisTimepoint] == time_current)
		{
			using namespace GlobalArrays;
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


		if(InitialParameters.StreamRatesAnalysis)
		{
			StreamRatesAnalysis(
					rates_analysis_stream_filename,
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
				InitialParameters.GasPhase,
				Number_Species,
				time_current,
				pressure,
				SpeciesConcentration
		);

		if(InitialParameters.PrintReacRates)
		{
			StreamReactionRates(ReactionRatesOutput, time_current, Rates);
		}


		if(InitialParameters.PetroOxy)
		{
			PetroOxyOutputStream(
					petrooxy_filename,
					PetroOxyData,
					time_current
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
		}


	} while (time_step < time_end);


	cout << "CPU Time: " << ((double) (clock() - cpu_time_current)) / CLOCKS_PER_SEC << " seconds\n";

	// close output files
	ConcentrationOutput.close();
	ReactionRatesOutput.close();

	if(InitialParameters.RatesMaxAnalysis)
	{
		WriteMaxRatesAnalysis(RatesAnalysisData, Species, Reactions,rates_analysis_stream_filename);
	}


	// stop the clock
	cpu_time_end = clock();
	cout << "\nTotal CPU time: " << ((double) (cpu_time_end - cpu_time_begin)) / CLOCKS_PER_SEC << " seconds\n";
}





void ODE_RHS(int*n, double*time_current, double*y, double*f)
{
	// A namespace allows global variables without causing a mess, should be quicker than redefining too
	using namespace ODESolverConstant;
	using namespace ODESolverVariable;

	int i;

	/* 2002 CODATA values */
	//double R = 8.314472e0;
	//double Na = 6.0221415e23;

	// stability hack
	if(GlobalArrays::InitialParameters.EnforceStability)
	{
		for (i = 0; i <= Number_Species; i++)
		{
			if(y[i]<0){
				//if(y[i]<1.e-24){
				Concentration[i] = 0;
			}
			else
			{
				Concentration[i] = y[i];
			}
		}
	}
	else{
		for (i = 0; i <= Number_Species; i++)
		{
			Concentration[i] = y[i];
		}
	}

	double time_difference;


	if(GlobalArrays::InitialParameters.PetroOxy)
	{
		time_difference = fabs(*time_current - time_previous);
		AdjustGasConcentration(
				y[OxyGasSpeciesID],
				Concentration[Number_Species],
				time_difference,
				PetroOxyData);
	}


	// Thermodynamic data, Rate Constant, Rates, new Concentrations
	Calculate_Thermodynamics(CalculatedThermo, Concentration[Number_Species], Thermodynamics);
	Calculate_Rate_Constant(Kf, Kr, Concentration[Number_Species],ReactionParameters, CalculatedThermo, SpeciesLossAll, Delta_N);
	CalculateReactionRates(Rates, Concentration, Kf, Kr, ReactantsForReactions, ProductsForReactions);
	SpeciesConcentrationChange = SpeciesLossRate(SpeciesLossAll,Number_Species, Rates);




	double ctot=0;
	double qint=0;
	double qtot=0;


	for (i = 0; i < Number_Species; i++)
	{
		ctot = ctot + CalculatedThermo[i][2] * Concentration[i];
	}
	// ctot = ctot / 1000; // working in moles/l so no Na;

	for (i = 0; i < Number_Reactions; i++)
	{
		qint = qint + Delta_N[i] * Rates[i];
	}
	qtot = -qint / (ctot);//*1000); // scale l to ml and Na not needed for moles/l * Na); //*/

	// Checked f[i] -> no unexpected rates of change for "inert gases", all 0.
	for (i = 0; i < Number_Species; i++)
	{
		f[i] = SpeciesConcentrationChange[i]; // Species equations //
	}
	f[Number_Species] = qtot; // Temperature equation //

	//*
	if(
			Concentration[Number_Species] >= GlobalArrays::InitialParameters.temperature
			&&
			GlobalArrays::InitialParameters.PetroOxy) // fix temperature for Oxy
	{
		y[Number_Species] = GlobalArrays::InitialParameters.temperature; // ensure temperature is not exceeded
		f[Number_Species] = 0;
		GlobalArrays::InitialParameters.PetroOxyTemperatureRise = 0;
	}//*/

	if(
			GlobalArrays::InitialParameters.PetroOxy
			&&
			GlobalArrays::InitialParameters.PetroOxyTemperatureRise != 0) // fix temperature for Oxy, rise desired
	{
		// 298K starting temp, GlobalArrays::InitialParameters.temperature is target
		// rise time given in minutes
		f[Number_Species] =
				(GlobalArrays::InitialParameters.temperature - 298)
				/
				(GlobalArrays::InitialParameters.PetroOxyTemperatureRise);
		//std::cout << f[Number_Species] << "\n";
	}//*/

	// IEEE standard hack to check for NaN
	// if temperature blows up, freeze it
	if(qtot != qtot)
	{
		f[Number_Species] = 0;
	}


	// For Oxy limiting
	time_previous = *time_current;
}




void Jacobian_Matrix(int*n,double*t,double*y,double*a) {

	// n -> number of species
	// t time
	// y concentration
	// a Jacobian in column wise order

	using namespace ODESolverConstant;
	using namespace ODESolverVariable;
	using namespace GlobalArrays;


	int i,j;

	// enable force stability?
	/*
	for (i = 0; i <= Number_Species; i++)
	{
		if(y[i]<0){
			//if(y[i]<1.e-24){
			Concentration[i] = 0;
		}
		else
		{
			Concentration[i] = y[i];
		}
	}//*/

	for (i = 0; i <= Number_Species; i++)
	{
		Concentration[i] = y[i];
	}

	// provides me a fresh array every time :) - ideal
	vector< double > JacobeanColumnWise((Number_Species+1)*(Number_Species+1));


	Calculate_Thermodynamics(CalculatedThermo, Concentration[Number_Species], Thermodynamics);
	Calculate_Rate_Constant(Kf, Kr, Concentration[Number_Species],ReactionParameters, CalculatedThermo, SpeciesLossAll, Delta_N);

	for(i=0;i<(int) JacobianMatrix.size();i++)
	{
		double temp;


		if(JacobianMatrix[i].IsForward) // Forward
		{
			if(JacobianMatrix[i].IsProduction) // species are gained
			{
				temp = Kf[JacobianMatrix[i].ReactionID]*JacobianMatrix[i].coefficient;
			}
			if(!JacobianMatrix[i].IsProduction) // species are lost
			{
				temp = -Kf[JacobianMatrix[i].ReactionID]*JacobianMatrix[i].coefficient;
			}
		}

		if(!JacobianMatrix[i].IsForward) // Reverse
		{
			if(JacobianMatrix[i].IsProduction) // species are gained
			{
				temp = Kr[JacobianMatrix[i].ReactionID]*JacobianMatrix[i].coefficient;
			}
			if(!JacobianMatrix[i].IsProduction) // species are lost
			{
				temp = -Kr[JacobianMatrix[i].ReactionID]*JacobianMatrix[i].coefficient;
			}
		}


		for(j=0;j<(int) JacobianMatrix[i].Species.size();j++)
		{
			if(JacobianMatrix[i].Species[j].power != 0) // power 0 = *1
			{
				if(JacobianMatrix[i].Species[j].power == 1) // power 1 is simple multiplication
				{
					temp = temp * Concentration[JacobianMatrix[i].Species[j].SpeciesID];
				}
				else
				{
					temp = temp *
							pow(Concentration[JacobianMatrix[i].Species[j].SpeciesID],
									JacobianMatrix[i].Species[j].power);
				}
			}
		}


		JacobeanColumnWise[JacobianMatrix[i].ColumnWiseArrayPosition] =
				JacobeanColumnWise[JacobianMatrix[i].ColumnWiseArrayPosition] + temp;
	}

	for (i = 0; i <= (int) JacobeanColumnWise.size() ; i++)
	{
		a[i] = JacobeanColumnWise[i];
	}

	/*
	 * Debug Output
	 */

	/*
	cout << "\n" << "\n";
	int temp_output = (int) sqrt(JacobeanColumnWise.size());
	for(i=0;i< temp_output;i++)
	{
		for(j=0;j<(int) JacobeanColumnWise.size();j++)
		{
			if(j % temp_output == i)
			{
				cout << a[j] << " ";
				//cout << JacobeanColumnWise[j] << " ";
				//cout << j % temp_output << " ";
			}
		}
		cout << "\n";
	}
	cout << "\n" << "\n";
	//*/
	//cout << "check\n";
}







