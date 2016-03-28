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
	 */


	// open the input file
	ifstream Input_Data;
	Input_Data.open (filename.c_str());


	// proceed only if input file is open
	if (Input_Data.is_open())
	{

		/*
		 * Start off the mechanism by setting all parameters to their
		 * default values which are "risk free"
		 */

		// Set defaults


		InitialParameters.irrev = false; // make the scheme irreversible?
		InitialParameters.PrintReacRates = false; // output reaction rates?

		/* Solver Parameters */
		/* Solver tolerances parameters */
		InitialParameters.rtol = 1.e-5;
		InitialParameters.atol = 1.e-25;
		InitialParameters.threshold = 1.e-10;
		InitialParameters.hm = 1.e-12;
		InitialParameters.h = 1.e-7;
		/* End the solver parameters */


		InitialParameters.UseStiffSolver = true;
		InitialParameters.EnforceStability = false;
		InitialParameters.Jacobian = false;
		/* End Solver Paramerters */


		/* Analysis functions */
		InitialParameters.MechanismAnalysis.MaximumRates = false;
		InitialParameters.MechanismAnalysis.StreamRatesAnalysis = false;
		InitialParameters.MechanismAnalysis.RatesSpeciesAllAnalysis = false;
		InitialParameters.MechanismAnalysis.RatesAnalysisAtTime = false;
		InitialParameters.MechanismAnalysis.RatesAnalysisAtTimeData.resize(0);
		/* End of Analysis functions */

		/* Initial Values */
		InitialParameters.temperature = 500;
		/* End Initiap Parameters */


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



		// So, I need to check for an opening tag and read until a closing tag



		/*
		 * Every section is handled by its own function to avoid the potential
		 * for collisions, also it allows similar keywords in different blocks
		 */


		while (Input_Data.good())
		{

			string line;
			vector< string > Input;

			getline(Input_Data,line);

			// only handle line if not a comment and not empty
			if(LineNotCommentOrEmpty(line))
			{

				//cout << line << "\n";

				// check for the start of an input block
				if (line.find("<InitialConditions>") != string::npos)

				{
					do{
						getline(Input_Data,line);
						if(LineNotCommentOrEmpty(line))
						{
							Input.push_back(line);
							//cout << line << "\n";
						}
					}while (line.find("</InitialConditions>") == string::npos);
					// Read in data until the closing tag is found

					Handle_InitialConditions(InitialParameters, Input);
				}
				Input.clear();


				if (line.find("<Species>") != string::npos)
				{
					do{
						getline(Input_Data,line);
						if(LineNotCommentOrEmpty(line))
						{
							Input.push_back(line);
							//cout << line << "\n";
						}
					}while (line.find("</Species>") == string::npos);
					// Read in data until the closing tag is found

					Handle_Species(InitialParameters, SetupSpecies, Input, Species);
				}
				Input.clear();



				if (line.find("<SolverParameters>") != string::npos)
				{
					do{
						getline(Input_Data,line);
						if(LineNotCommentOrEmpty(line))
						{
							Input.push_back(line);
							//cout << line << "\n";
						}
					}while (line.find("</SolverParameters>") == string::npos);
					// Read in data until the closing tag is found

					Handle_SolverParameters(InitialParameters, Input);
				}
				Input.clear();



				if (line.find("<Analysis>") != string::npos)
				{
					do{
						getline(Input_Data,line);
						if(LineNotCommentOrEmpty(line))
						{
							Input.push_back(line);
							//cout << line << "\n";
						}
					}while (line.find("</Analysis>") == string::npos);
					// Read in data until the closing tag is found

					Handle_Analysis(InitialParameters, Input, Species);
				}
				Input.clear();



				if (line.find("<PressureVessel>") != string::npos)
				{
					//cout << "Pressure vessel input detected.\n"; // tested
					do{
						getline(Input_Data,line);
						if(LineNotCommentOrEmpty(line))
						{
							Input.push_back(line);
							//cout << line << "\n";
						}
					}while (line.find("</PressureVessel>") == string::npos);
					// Read in data until the closing tag is found

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
							//cout << line << "\n";
						}
					}while (line.find("</MechanismReduction>") == string::npos);
					// Read in data until the closing tag is found

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


bool LineNotCommentOrEmpty( string InputLine)
{
	if(
			InputLine.compare(0,2,"//") != 0// C/C++ style comment
			//	&& line.compare(0,2,"/*") != 0 // opening comment in C/C++, not checking for the * yet
			&& InputLine.compare(0,1,"!") != 0 // Chemkin Style comment
			&& InputLine.compare(0,1,"#") != 0 // shell or Python style comments
			&& !InputLine.empty() // check that line is not empty
	)
	{
		return true;
	}
	else
	{
		return false;
	}
}







