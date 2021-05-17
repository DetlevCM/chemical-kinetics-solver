#include "../headers/Headers.hpp"




//bool LineNotCommentOrEmpty( string );


// http://stackoverflow.com/questions/7677007/passing-vector-to-function-c
// Pass vector by reference as input needs not be changed
void Get_Initial_Conditions(
		string filename,
		vector< string > Species,
		Initial_Data& InitialParameters
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

	// enable making the scheme irreversible without integration, default false
	InitialParameters.NoIntegration = false;

	solver_parameters Default_Solver_Parameters;

	/* begin default solver settings */
	Default_Solver_Parameters.SolverType = 1; // 0 = IntelODE, 1 = odepack (default)
	Default_Solver_Parameters.Use_Stiff_Solver = true; // old setting from IntelODE
	Default_Solver_Parameters.Use_Analytical_Jacobian = false; // faster with odepack
	Default_Solver_Parameters.rtol = 1.e-5;
	Default_Solver_Parameters.atol = 1.e-10;
	Default_Solver_Parameters.minimum_stepsize = 1.e-12; // for IntelODE
	Default_Solver_Parameters.initial_stepsize = 1.e-7; // for IntelODE
	Default_Solver_Parameters.separator = ","; // use comma as a default separator
	/* end default solver settings */


	InitialParameters.EnforceStability = false; // default setting

	InitialParameters.Solver_Parameters = Default_Solver_Parameters;


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
	InitialParameters.MechanismReduction.ReduceReactions = 0;
	// Mapping setup
	//InitialParameters.MechanismReduction.UseNewLumping = false;
	InitialParameters.MechanismReduction.UseFastLumping = true; // old method wasn't wrong but inefficient and slow
	InitialParameters.MechanismReduction.LumpingType = 0; // 0 = no lumping
	/* End Mechanism Reduction */


	/* Other functions */
	//InitialParameters.nrspec = 0;

	/* Code extension for Gas Phase */
	InitialParameters.GasPhase = false; // default not gas phase
	InitialParameters.GasPhaseVolume = 0; // 1 L = 0.001 m^3
	InitialParameters.GasPhasePressure = 0; // Pa, normal pressure

	InitialParameters.InitialLiquidSpecies.clear();
	InitialParameters.InitialGasSpecies.clear();

	// for the TGA case:
	InitialParameters.TGA_used = false; // default

	/* Modification to deal with PetroOxy in main Input file */


	InitialParameters.PetroOxy.IsSet = false;
	InitialParameters.PetroOxy.SampleSize = 0;
	InitialParameters.PetroOxy.VesselSize = 22.5; // my estimate of PetroOxy volume
	InitialParameters.PetroOxy.InitPressure = 0;
	InitialParameters.PetroOxy.MaxPressure = 0;
	InitialParameters.PetroOxy.GasSpecies = 0;
	InitialParameters.PetroOxy.GasSolubility = 0;
	InitialParameters.PetroOxy.TemperatureRise = 0;

	InitialParameters.PetroOxy.HenryLawDiffusionLimitSet = false;
	InitialParameters.PetroOxy.HenryLawDiffusionLimit = 0;


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
	if(!Input_Data.is_open())
	{
		cout << "error opening initial data input file " << filename << "\n";
		exit(1); // exit code with error status
	}
	else if(Input_Data.is_open())
	{// I need to get the solver parameters first
		while (Input_Data.good())
		{
			getline(Input_Data,line);


			if(Line_Not_Comment_Or_Empty(line)) // only handle line if not a comment and not empty
			{
				if (Test_If_Word_Found(line, "<<no integration>>")) // special case of user not wanting integration, e.g. for making a scheme irreversible only
				{
					InitialParameters.NoIntegration = true;
				}
			}


			if(Line_Not_Comment_Or_Empty(line)) // only handle line if not a comment and not empty
			{
				if (line.find("<Solver Parameters>") != string::npos)
				{
					do{
						getline(Input_Data,line);
						if(Line_Not_Comment_Or_Empty(line))
						{
							Input.push_back(line);
						}
					}while (line.find("</Solver Parameters>") == string::npos);
					// Read in data until the closing tag is found
					Handle_Solver_Parameters(InitialParameters, Input);
					Global_Solver_Type.Use_Analytical_Jacobian = InitialParameters.Solver_Parameters.Use_Analytical_Jacobian;
					Global_Solver_Type.Use_Stiff_Solver = InitialParameters.Solver_Parameters.Use_Stiff_Solver;
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
			if(Line_Not_Comment_Or_Empty(line)) // only handle line if not a comment and not empty
			{
				if (line.find("<Initial Conditions>") != string::npos)
				{
					do{
						getline(Input_Data,line);
						if(Line_Not_Comment_Or_Empty(line))
						{
							Input.push_back(line);
						}
					}while (line.find("</Initial Conditions>") == string::npos);
					Handle_Initial_Conditions(InitialParameters, Input, Global_Solver_Type);
				}
				Input.clear();

				if (line.find("<Species>") != string::npos)
				{
					do{
						getline(Input_Data,line);
						if(Line_Not_Comment_Or_Empty(line))
						{
							Input.push_back(line);
						}
					}while (line.find("</Species>") == string::npos);
					InitialParameters.InitialLiquidSpecies = Handle_Species(InitialParameters, Input, Species);
				}
				Input.clear();

				// for a gas phase model (future work) or a combined gas/liquid model
				if (line.find("<Gas Species>") != string::npos)
				{
					do{
						getline(Input_Data,line);
						if(Line_Not_Comment_Or_Empty(line))
						{
							Input.push_back(line);
						}
					}while (line.find("</Gas Species>") == string::npos);

					InitialParameters.InitialGasSpecies = Handle_Species(InitialParameters, Input, Species);
				}
				Input.clear();

				if (line.find("<Analysis>") != string::npos)
				{
					do{
						getline(Input_Data,line);
						if(Line_Not_Comment_Or_Empty(line))
						{
							Input.push_back(line);
						}
					}while (line.find("</Analysis>") == string::npos);
					Handle_Analysis(InitialParameters, Input, Species);
				}
				Input.clear();

				// original PetroOxy functionality as initially implemented
				if (line.find("<PetroOxy>") != string::npos)
				{
					do{
						getline(Input_Data,line);
						if(Line_Not_Comment_Or_Empty(line))
						{
							Input.push_back(line);
						}
					}while (line.find("</PetroOxy>") == string::npos);
					Handle_PetroOxy(InitialParameters, Input, Species);
				}
				Input.clear();

				if (line.find("<Pressure Vessel>") != string::npos)
				{
					do{
						getline(Input_Data,line);
						if(Line_Not_Comment_Or_Empty(line))
						{
							Input.push_back(line);
						}
					}while (line.find("</Pressure Vessel>") == string::npos);
					Handle_Pressure_Vessel(InitialParameters, Input);
				}
				Input.clear();

				if (line.find("<Mechanism Reduction>") != string::npos)
				{
					do{
						getline(Input_Data,line);
						if(Line_Not_Comment_Or_Empty(line))
						{
							Input.push_back(line);
						}
					}while (line.find("</Mechanism Reduction>") == string::npos);

					InitialParameters.MechanismReduction = Handle_Mechanism_Reduction(Input);
					/*InitialParameters.ReduceReactions = temp.ReduceReactions;
					InitialParameters.UseFastLumping = temp.UseFastLumping;
					InitialParameters.UseNewLumping = temp.UseNewLumping;//*/
					//cout << "check " << InitialParameters.MechanismReduction.ReduceReactions << "\n";
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

	if(InitialParameters.MechanismReduction.ReduceReactions != 0 && !InitialParameters.irrev)
	{
		InitialParameters.irrev = true;
		cout << "Rates Based Mechanism Reduction requires an irreversible scheme - method set. \n";
	}


}










