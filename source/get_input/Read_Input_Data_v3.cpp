#include <MyHeaders.h>




bool LineNotCommentOrEmpty( string );


// http://stackoverflow.com/questions/7677007/passing-vector-to-function-c
// Pass vector by reference as input needs not be changed
void Read_Input_Data_v3(
		string filename,
		const vector< string >& Species,
		InitParam& InitialParameters,
		vector< InitSpecies >& SetupSpecies
)
{

	/*
	 * Time to design a brand new input format which can scale well with
	 * ever increasing demands for different functionality
	 *
	 * The new input will take cues from HTML or XML and use tags to define
	 * input blocks which gives some separation to individual functions
	 *
	 * e.g.
	 *
	 * <InitialData>
	 *
	 * </InitialData>
	 *
	 * There will be the following blocks:
	 *
	 * - InitialConditions
	 * - Species
	 * - SolverParameters
	 * - Analysis
	 * - PressureVessel
	 *
	 *
	 * Start off the mechanism by setting all parameters to their
	 * default values which are "risk free"
	 */

	// Set defaults
	InitialParameters.irrev = false; // make the scheme irreversible?
	InitialParameters.PrintReacRates = false; // output reaction rates?

	/* Solver Parameters */
	InitialParameters.Param_Solver.rtol = 1.e-5;
	InitialParameters.Param_Solver.atol = 1.e-15;
	InitialParameters.Param_Solver.threshold = 1.e-10;
	InitialParameters.Param_Solver.minimum_stepsize = 1.e-12;
	InitialParameters.Param_Solver.initial_stepsize = 1.e-7;
	InitialParameters.Param_Solver.separator = ","; // more modern, comma delimited

	InitialParameters.Param_Solver.SolverType = 0;
	InitialParameters.Param_Solver.Use_Stiff_Solver = true;
	InitialParameters.EnforceStability = false;
	InitialParameters.Param_Solver.Use_Analytical_Jacobian = false;
	/* End Solver Paramerters */


	/* Analysis functions */
	InitialParameters.MechanismAnalysis.MaximumRates = false;
	InitialParameters.MechanismAnalysis.StreamRatesAnalysis = false;
	InitialParameters.MechanismAnalysis.RatesSpeciesAllAnalysis = false;
	InitialParameters.MechanismAnalysis.RatesAnalysisAtTime = false;
	InitialParameters.MechanismAnalysis.RatesAnalysisAtTimeData.resize(0);
	/* End of Analysis functions */


	/* Initial Values */
	InitialParameters.temperature = 298.15; // standard condition, avoids segfault if user forgets
	/* End Initial Parameters */


	/* Mechanism Reductions */
	InitialParameters.ReduceReactions = 0;
	// Mapping setup
	InitialParameters.UseNewLumping = false;
	InitialParameters.UseFastLumping = true; // old method wasn't wrong but inefficient and slow
	/* End Mechanism Reduction */


	/* Other functions */
	InitialParameters.nrspec = 0;

	/* Code extension for Gas Phase */
	InitialParameters.GasPhase = false; // default not gas phase
	InitialParameters.GasPhaseVolume = 0; // 1 L = 0.001 m^3
	InitialParameters.GasPhasePressure = 0; // Pa, normal pressure

	/* Modification to deal with PetroOxy in main Input file */

	/* Data is:
	 * 0) Sample Size <- m^3
	 * p - 1) Initial Pressure
	 * T - 2) Target Temperature
	 * pmax - 3) Maximum Pressure
	 * 4) O2 space in PetroOxy <- m^3
	 * 5) Gas Species
	 * 6) mol of gas species at 25 degree celsius
	 * 7) O2 derived pressure
	 * 8) Vapour Pressure solvent component
	 * 9) solubility of gas at 298K, mol/L (C)
	 * 10) "k" as Henry's Law Constant, k = P_gas/C //  Pa mol / L
	 */

	InitialParameters.PressureVessel.IsSet = false;
	InitialParameters.PressureVessel.SampleSize = 0;
	InitialParameters.PressureVessel.VesselSize = 22.5; // my estimate of PetroOxy volume
	InitialParameters.PressureVessel.InitPressure = 0;
	InitialParameters.PressureVessel.MaxPressure = 0;
	InitialParameters.PressureVessel.GasSpecies = 0;
	InitialParameters.PressureVessel.GasSolubility = 0;
	InitialParameters.PressureVessel.TemperatureRise = 0;

	InitialParameters.PressureVessel.HenryLawDiffusionLimitSet = false;
	InitialParameters.PressureVessel.HenryLawDiffusionLimit = 0;


	InitialParameters.StoichiometryMatrixForOpt = false;
	InitialParameters.ConstantConcentration = false;
	/* End of other functions */


	/*
	 * Every section is handled by its own function to avoid the potential
	 * for collisions, also it allows similar keywords in different blocks
	 */


	solver_type Global_Solver_Type;

	string line; // individually read in line from inptu file
	vector< string > Input; // collection of input parameters

	// open the input file
	ifstream Input_Data;
	Input_Data.open (filename.c_str());

	// proceed only if input file is open
	if (Input_Data.is_open())
	{// I need to get the solver parameters first
		while (Input_Data.good())
		{
			getline(Input_Data,line);
			if(LineNotCommentOrEmpty(line)) // only handle line if not a comment and not empty
			{
				if (line.find("<SolverParameters>") != string::npos)
				{
					do{
						getline(Input_Data,line);
						if(LineNotCommentOrEmpty(line))
						{
							Input.push_back(line);
						}
					}while (line.find("</SolverParameters>") == string::npos);
					// Read in data until the closing tag is found
					Handle_SolverParameters(InitialParameters, Input);
					Global_Solver_Type.Use_Analytical_Jacobian = InitialParameters.Param_Solver.Use_Analytical_Jacobian;
					Global_Solver_Type.Use_Stiff_Solver = InitialParameters.Param_Solver.Use_Stiff_Solver;
				}
				Input.clear();
			}
		}
	}
	Input_Data.close(); // little dance to run two loops over the file
	Input_Data.open (filename.c_str());
	if (Input_Data.is_open())
	{// deal with the rest of the user settings:
		while (Input_Data.good())
		{
			getline(Input_Data,line);
			if(LineNotCommentOrEmpty(line)) // only handle line if not a comment and not empty
			{
				if (line.find("<InitialConditions>") != string::npos)
				{
					do{
						getline(Input_Data,line);
						if(LineNotCommentOrEmpty(line))
						{
							Input.push_back(line);
													}
					}while (line.find("</InitialConditions>") == string::npos);
					Handle_InitialConditions(InitialParameters, Input, Global_Solver_Type);
				}
				Input.clear();

				if (line.find("<Species>") != string::npos)
				{
					do{
						getline(Input_Data,line);
						if(LineNotCommentOrEmpty(line))
						{
							Input.push_back(line);
						}
					}while (line.find("</Species>") == string::npos);
					Handle_Species(InitialParameters, SetupSpecies, Input, Species);
				}
				Input.clear();

				if (line.find("<Analysis>") != string::npos)
				{
					do{
						getline(Input_Data,line);
						if(LineNotCommentOrEmpty(line))
						{
							Input.push_back(line);
						}
					}while (line.find("</Analysis>") == string::npos);
					Handle_Analysis(InitialParameters, Input, Species);
				}
				Input.clear();

				if (line.find("<PressureVessel>") != string::npos)
				{
					do{
						getline(Input_Data,line);
						if(LineNotCommentOrEmpty(line))
						{
							Input.push_back(line);
						}
					}while (line.find("</PressureVessel>") == string::npos);
					Handle_PressureVessel(InitialParameters, Input, Species);
				}
				Input.clear();

				if (line.find("<MechanismReduction>") != string::npos)
				{
					do{
						getline(Input_Data,line);
						if(LineNotCommentOrEmpty(line))
						{
							Input.push_back(line);
						}
					}while (line.find("</MechanismReduction>") == string::npos);
					Handle_MechanismReduction(InitialParameters, Input);
				}
				Input.clear();
			}
		}
	}
	Input_Data.close();


	// Check that special requirements are met
	if(InitialParameters.MechanismAnalysis.MaximumRates && !InitialParameters.irrev)
	{
		InitialParameters.irrev = true;
		cout << "Rates Analysis requires an irreversible scheme - method set. \n";
	}

	if(InitialParameters.MechanismAnalysis.RatesSpeciesAllAnalysis)
	{
		InitialParameters.irrev = true;
		cout << "Rates Analysis requires an irreversible scheme - method set. \n";
	}

	if(InitialParameters.MechanismAnalysis.StreamRatesAnalysis && !InitialParameters.irrev)
	{
		InitialParameters.irrev = true;
		cout << "Rates Analysis requires an irreversible scheme - method set. \n";
	}

	if(InitialParameters.MechanismAnalysis.RatesAnalysisAtTime && !InitialParameters.irrev)
	{
		InitialParameters.irrev = true;
		cout << "Rates Analysis At Times requires an irreversible scheme - method set. \n";
	}

	if(InitialParameters.ReduceReactions != 0 && !InitialParameters.irrev)
	{
		InitialParameters.irrev = true;
		cout << "Rates Based Mechanism Reduction requires an irreversible scheme - method set. \n";
	}


}










