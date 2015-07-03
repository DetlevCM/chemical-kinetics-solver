#include <MyHeaders.h>

// http://stackoverflow.com/questions/7677007/passing-vector-to-function-c
// Pass vector by reference as input needs not be changed
void Read_Input_Data_v2(
		string filename,
		const vector< string >& Species,
		InitParam& SetupParam,
		vector< InitSpecies >& SetupSpecies
)
{

	// Now we need to deal with the structure of the input file provided by the user
	// It is safe to assume that all entries will be separated by a space or tab
	// therefore we should be able to just tokenize the input and read it in

	ifstream Input_Data;
	Input_Data.open (filename.c_str());

	string line1;

	int Number_Fixed_Positions = 11;

	vector< vector< double > > temp_input(Number_Fixed_Positions); // 11 fixed positions, plus extra entries via push_back for all species
	int i;

	vector< double > LineIn(2); // 2 parameters is most I expect
	// initialize the storage array
	for(i=0;i<Number_Fixed_Positions;i++)
	{
		temp_input[i] = LineIn;
	}
	LineIn.clear();

	int Number_Species = Species.size();
	bool OxyGasSpeciesDefined = false;

	if (Input_Data.is_open())
	{

		// Set defaults

		SetupParam.irrev = false; // shoud be bool
		SetupParam.PrintReacRates = false;
		SetupParam.nrspec = 0;
		SetupParam.rtol = 1.e-5;
		SetupParam.atol = 1.e-25;
		SetupParam.threshold = 1.e-10;
		SetupParam.temperature = 500;
		SetupParam.hm = 1.e-12;
		SetupParam.h = 1.e-7;
		SetupParam.RatesMaxAnalysis = false;
		SetupParam.StreamRatesAnalysis = false;
		SetupParam.RatesSpeciesAllAnalysis = false;
		SetupParam.RatesAnalysisAtTime = false;
		SetupParam.RatesAnalysisAtTimeData.resize(0);
		SetupParam.ReduceReactions = 0;
		SetupParam.UseStiffSolver = true;
		SetupParam.EnforceStability = false;

		/* Code extension for Gas Phase */
		SetupParam.GasPhase = false; // default not gas phase
		SetupParam.GasPhaseVolume = 0; // 1 L = 0.001 m^3
		SetupParam.GasPhasePressure = 0; // Pa, normal pressure

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

		SetupParam.PetroOxy = false;
		SetupParam.PetroOxySampleSize = 0;
		SetupParam.PetroOxyInitPressure = 0;
		SetupParam.PetroOxyMaxPressure = 0;
		SetupParam.PetroOxyGasSpecies = 0;
		SetupParam.PetroOxyGasSolubility = 0;
		SetupParam.PetroOxyTemperatureRise = 0;

		SetupParam.HenryLawDiffusionLimitSet = false;
		SetupParam.HenryLawDiffusionLimit = 0;


		SetupParam.StoichimoetryMatrixForOpt = false;


		SetupParam.Jacobian = false;

		int i;
		char * cstr, *p;
		string str;
		size_t found;

		//printf("Line In: %s \n\n",line1.c_str());

		// while the file is open
		while (Input_Data.good())
		{
			getline (Input_Data,line1); // Output Control
			//printf("Line In: %s \n\n",line1.c_str());

			if(line1.compare(0,1,"!") != 0 && !line1.empty()) // line is not a comment and not empty. Whitespaces on lines may be an issue
			{

				// dispose of any comments that may be in the file after entries
				str = line1;//a String - For some reason cannot pass line1 directly...
				cstr = new char [str.size()+1];
				strcpy (cstr, str.c_str());

				vector< string > RemoveComments;

				found = line1.find("!");
				if (found!=string::npos)
				{
					p=strtok (cstr,"!");
					RemoveComments.push_back(p);
					line1 = RemoveComments[0];
					delete[] cstr;
					RemoveComments.clear();
					// make the program believe it has read in the string directly,
					// i.e rest line1 to the new string without the comments
					str = line1;//a String - For some reason cannot pass line1 directly...
					cstr = new char [str.size()+1];
					strcpy (cstr, str.c_str());
				}


				// Search for the keyword and Store the input

				found = line1.find("Jacobian");
				if (found!=string::npos)
				{
					SetupParam.Jacobian = true;
					line1.clear();
					delete[] cstr;
				}

				found = line1.find("IRREV");
				if (found!=string::npos)
				{
					SetupParam.irrev = true;
					line1.clear();
					delete[] cstr;
					//printf("Test - Scheme will be made irreversible. \n");
				}

				found = line1.find("PrintReac");
				if (found!=string::npos)
				{
					SetupParam.PrintReacRates = true;
					line1.clear();
					delete[] cstr;
				}

				found = line1.find("Print Rates");
				if (found!=string::npos)
				{
					SetupParam.PrintReacRates = true;
					line1.clear();
					delete[] cstr;
				}

				found = line1.find("Force Stability");
				if (found!=string::npos)
				{
					SetupParam.EnforceStability = true;
					line1.clear();
					delete[] cstr;
				}

				found = line1.find("Stoichimoetry Matrix For Opt");
				if (found!=string::npos)
				{
					SetupParam.StoichimoetryMatrixForOpt = true;
					line1.clear();
					delete[] cstr;
				}


				found = line1.find("RatesMaxAnalysis");
				if (found!=string::npos)
				{
					SetupParam.RatesMaxAnalysis = true;
					line1.clear();
					delete[] cstr;
				}

				found = line1.find("StreamRatesAnalysis");
				if (found!=string::npos)
				{
					SetupParam.StreamRatesAnalysis = true;
					line1.clear();
					delete[] cstr;
				}

				found = line1.find("RatesSpeciesAllAnalysis");
				if (found!=string::npos)
				{
					SetupParam.RatesSpeciesAllAnalysis = true;
					line1.clear();
					delete[] cstr;
				}

				found = line1.find("Use General Solver");
				if (found!=string::npos)
				{
					SetupParam.UseStiffSolver = false;
					line1.clear();
					delete[] cstr;
				}

				found = line1.find("ReduceReactions");
				if (found!=string::npos)
				{
					LineIn.clear(); // make sure storage array is empty
					LineIn.push_back(1); // default value, 1K

					p=strtok (cstr," \t"); // break at space or tab
					p=strtok(NULL," \t"); // break again as first is the keyword

					if(p!=NULL){ // only read remainder is something is left
						LineIn[0] = strtod(p,NULL);
						p=strtok(NULL," \t");
					}
					line1.clear();
					delete[] cstr;
					SetupParam.ReduceReactions = LineIn[0];
					//printf("Test - Temperature is: %.3e \n",LineIn[0]);
				}


				found = line1.find("Temperature");
				if (found!=string::npos)
				{

					//make sure it isn't PetroOxy Temperature Rise...
					found = line1.find("PetroOxy");
					if (found==string::npos)
					{
						LineIn.clear(); // make sure storage array is empty
						LineIn.push_back(1); // default value, 1K

						p=strtok (cstr," \t"); // break at space or tab
						p=strtok(NULL," \t"); // break again as first is the keyword

						if(p!=NULL){ // only read remainder is something is left
							LineIn[0] = strtod(p,NULL);
							p=strtok(NULL," \t");
						}
						line1.clear();
						delete[] cstr;
						SetupParam.temperature = LineIn[0];
						//printf("Test - Temperature is: %.3e \n",LineIn[0]);
					}
				}


				found = line1.find("RatesAnalysisAtTime");
				if (found!=string::npos)
				{
					SetupParam.RatesAnalysisAtTime = true; // user wants rates at specified times

					LineIn.clear(); // make sure storage array is empty
					LineIn.push_back(1); // default value, 1K

					p=strtok (cstr," \t"); // break at space or tab
					p=strtok(NULL," \t"); // break again as first is the keyword

					while(p!=NULL){ // only read remainder is something is left
						SetupParam.RatesAnalysisAtTimeData.push_back(strtod(p,NULL));
						p=strtok(NULL," \t");
					}
					line1.clear();
					delete[] cstr;
					//SetupParam.RatesAnalysisAtTimeData = LineIn;
					//printf("Test - Temperature is: %.3e \n",LineIn[0]);
				}


				found = line1.find("hm");
				if (found!=string::npos)
				{
					LineIn.clear(); // make sure storage array is empty
					LineIn.push_back(1); // default value, 1K

					p=strtok (cstr," \t"); // break at space or tab
					p=strtok(NULL," \t"); // break again as first is the keyword

					if(p!=NULL){ // only read remainder is something is left
						LineIn[0] = strtod(p,NULL);
						p=strtok(NULL," \t");
					}
					line1.clear();
					delete[] cstr;
					SetupParam.hm = LineIn[0];
					//printf("Test - Temperature is: %.3e \n",LineIn[0]);
				}

				found = line1.find("initialh");
				if (found!=string::npos)
				{
					LineIn.clear(); // make sure storage array is empty
					LineIn.push_back(1); // default value, 1K

					p=strtok (cstr," \t"); // break at space or tab
					p=strtok(NULL," \t"); // break again as first is the keyword

					if(p!=NULL){ // only read remainder is something is left
						LineIn[0] = strtod(p,NULL);
						p=strtok(NULL," \t");
					}
					line1.clear();
					delete[] cstr;
					SetupParam.h = LineIn[0];
					//printf("Test - Temperature is: %.3e \n",LineIn[0]);
				}



				found = line1.find("Tolerance");
				if (found!=string::npos)
				{
					LineIn.clear(); // make sure storage array is empty

					p=strtok (cstr," \t"); // break at space or tab
					p=strtok(NULL," \t"); // break again as first is the keyword

					while(p!=NULL){ // only read remainder is something is left
						LineIn.push_back(strtod(p,NULL));
						p=strtok(NULL," \t");
					}
					line1.clear();
					delete[] cstr;
					SetupParam.rtol = LineIn[0];
					SetupParam.threshold = LineIn[1];
					//printf("Test - Tolerance is: %.3e and Threshhold is: %.3e \n",LineIn[0],LineIn[1]);
				}

				found = line1.find("Threshold");
				if (found!=string::npos)
				{
					LineIn.clear(); // make sure storage array is empty

					p=strtok (cstr," \t"); // break at space or tab
					p=strtok(NULL," \t"); // break again as first is the keyword

					while(p!=NULL){ // only read remainder is something is left
						LineIn.push_back(strtod(p,NULL));
						p=strtok(NULL," \t");
					}
					line1.clear();
					delete[] cstr;
					SetupParam.threshold = LineIn[0];
					//printf("Test - Tolerance is: %.3e and Threshhold is: %.3e \n",LineIn[0],LineIn[1]);
				}

				found = line1.find("RTOL");
				if (found!=string::npos)
				{
					LineIn.clear(); // make sure storage array is empty

					p=strtok (cstr," \t"); // break at space or tab
					p=strtok(NULL," \t"); // break again as first is the keyword

					while(p!=NULL){ // only read remainder is something is left
						LineIn.push_back(strtod(p,NULL));
						p=strtok(NULL," \t");
					}
					line1.clear();
					delete[] cstr;
					SetupParam.rtol = LineIn[0];
					//printf("Test - Tolerance is: %.3e and Threshhold is: %.3e \n",LineIn[0],LineIn[1]);
				}


				found = line1.find("EndTime");
				if (found!=string::npos)
				{
					LineIn.clear(); // make sure storage array is empty

					p=strtok (cstr," \t"); // break at space or tab
					p=strtok(NULL," \t"); // break again as first is the keyword

					while(p!=NULL){ // only read remainder is something is left
						LineIn.push_back(strtod(p,NULL));
						p=strtok(NULL," \t");
					}
					line1.clear();
					delete[] cstr;

					// allows users to provide multiple time points
					SetupParam.TimeEnd.push_back(LineIn[0]);
					SetupParam.TimeStep.push_back(LineIn[1]);
					//printf("Test - End Time is: %.3e and Time Step is;  %.3e \n",LineIn[0],LineIn[1]);
				}


				// Legacy function
				/*
				found = line1.find("TimeStepChange");
				if (found!=string::npos)
				{
					LineIn.clear(); // make sure storage array is empty

					p=strtok (cstr," \t"); // break at space or tab
					p=strtok(NULL," \t"); // break again as first is the keyword

					while(p!=NULL){ // only read remainder is something is left
						LineIn.push_back(strtod(p,NULL));
						p=strtok(NULL," \t");
					}
					line1.clear();
					delete[] cstr;
					// Not implemented with new system
					//printf("Test - Time Step Change Time is: %.3e and New Time Step is;  %.3e \n",LineIn[0],LineIn[1]);
				}//*/


				/*
				 * Gas Phase Code Extension
				 */

				found = line1.find("GasPhasePressure"); // We Assume kPa
				if (found!=string::npos)
				{
					LineIn.clear(); // make sure storage array is empty

					p=strtok (cstr," \t"); // break at space or tab
					p=strtok(NULL," \t"); // break again as first is the keyword

					while(p!=NULL){ // only read remainder is something is left
						LineIn.push_back(strtod(p,NULL));
						p=strtok(NULL," \t");
					}
					line1.clear();
					delete[] cstr;
					SetupParam.GasPhasePressure = LineIn[0]*1000;
					//SetupParam.GasPhase = true; // activate Gas Phase Correction for User
				}

				found = line1.find("GasPhaseVolume"); // We Assume Litres
				if (found!=string::npos)
				{
					LineIn.clear(); // make sure storage array is empty

					p=strtok (cstr," \t"); // break at space or tab
					p=strtok(NULL," \t"); // break again as first is the keyword

					while(p!=NULL){ // only read remainder is something is left
						LineIn.push_back(strtod(p,NULL));
						p=strtok(NULL," \t");
					}
					line1.clear();
					delete[] cstr;
					SetupParam.GasPhaseVolume = LineIn[0]/1000; // Convert to m^3
					//SetupParam.GasPhase = true; // activate Gas Phase Correction for User
				}


				// if not in use, value will be zero - if the user has entered both, activate
				if(SetupParam.GasPhasePressure != 0 && SetupParam.GasPhaseVolume != 0){
					SetupParam.GasPhase = true; // activate Gas Phase Correction for User
				}
				/*
				 * End Gas Phase Code Extension
				 */




				/*
				 * PetroOxy Data Input
				 */

				vector<string> Readout;

				if(string::npos!=line1.find("PetroOxy Solvent Sample="))
				{
					char * cstr, *p;
					string str = line1;
					cstr = new char [str.size()+1];
					strcpy (cstr, str.c_str());

					p=strtok (cstr,"=");
					while (p!=NULL)
					{
						Readout.push_back(p);
						//cout << p << "\n";
						p=strtok(NULL,"<>");
					}
					delete[] cstr;
					delete[] p;

					// it is mL -> make into m^3
					SetupParam.PetroOxySampleSize = strtod(Readout[1].c_str(),NULL)*1e-6;
					//PetroOxyData[0] = strtod(Readout[1].c_str(),NULL)*1e-6;
					Readout.clear();

					// Calculated volume from estimate is 22.41mL - so let us assume 22.5mL, value is in m^3
					//PetroOxyData[4] = 22.5*1e-6 - PetroOxyData[0];
				}


				if(string::npos!=line1.find("PetroOxy Initial Pressure="))
				{
					char * cstr, *p;
					string str = line1;
					cstr = new char [str.size()+1];
					strcpy (cstr, str.c_str());

					p=strtok (cstr,"=");
					while (p!=NULL)
					{
						Readout.push_back(p);
						//cout << p << "\n";
						p=strtok(NULL,"<>");
					}
					delete[] cstr;
					delete[] p;

					// kPa to Pa
					SetupParam.PetroOxyInitPressure = strtod(Readout[1].c_str(),NULL) * 1000;
					//PetroOxyData[1] = strtod(Readout[1].c_str(),NULL) * 1000;
					Readout.clear();
				}

				if(string::npos!=line1.find("PetroOxy Maximum Pressure="))
				{
					char * cstr, *p;
					string str = line1;
					cstr = new char [str.size()+1];
					strcpy (cstr, str.c_str());

					p=strtok (cstr,"=");
					while (p!=NULL)
					{
						Readout.push_back(p);
						//cout << p << "\n";
						p=strtok(NULL,"<>");
					}
					delete[] cstr;
					delete[] p;

					// kPa to Pa
					SetupParam.PetroOxyMaxPressure = strtod(Readout[1].c_str(),NULL) * 1000;
					//PetroOxyData[3] = strtod(Readout[1].c_str(),NULL) * 1000;
					Readout.clear();
				}

				if(string::npos!=line1.find("PetroOxy Gas Solubility="))
				{
					char * cstr, *p;
					string str = line1;
					cstr = new char [str.size()+1];
					strcpy (cstr, str.c_str());

					p=strtok (cstr,"=");
					while (p!=NULL)
					{
						Readout.push_back(p);
						//cout << p << "\n";
						p=strtok(NULL,"<>");
					}
					delete[] cstr;
					delete[] p;

					// kPa to Pa
					SetupParam.PetroOxyGasSolubility = strtod(Readout[1].c_str(),NULL);// * 1000;
					//PetroOxyData[9] = strtod(Readout[1].c_str(),NULL);// * 1000;
					Readout.clear();
				}


				if(string::npos!=line1.find("PetroOxy Gas Species="))
				{
					char * cstr, *p;
					string str = line1;
					cstr = new char [str.size()+1];
					strcpy (cstr, str.c_str());

					p=strtok (cstr,"=");
					while (p!=NULL)
					{
						Readout.push_back(p);
						//cout << p << "\n";
						p=strtok(NULL,"<>");
					}
					delete[] cstr;
					delete[] p;

					for(int i = 0;i<(int)Species.size();i++)
					{
						//if(strcmp(species_name.c_str(),Comparator.c_str()) == 0){
						if(strcmp(Readout[1].c_str(),Species[i].c_str()) == 0){
							SetupParam.PetroOxyGasSpecies = i;
							OxyGasSpeciesDefined = true;
							//PetroOxyData[5] = i; //strtod(Readout[1].c_str(),NULL);
							//cout << i << "\n";
						}

					}

					Readout.clear();
				}

				if(string::npos!=line1.find("PetroOxy Temperature Rise="))
				{
					char * cstr, *p;
					string str = line1;
					cstr = new char [str.size()+1];
					strcpy (cstr, str.c_str());

					p=strtok (cstr,"=");
					while (p!=NULL)
					{
						Readout.push_back(p);
						//cout << p << "\n";
						p=strtok(NULL,"<>");
					}
					delete[] cstr;
					delete[] p;

					// kPa to Pa
					SetupParam.PetroOxyTemperatureRise = strtod(Readout[1].c_str(),NULL) * 1000;
					//PetroOxyData[3] = strtod(Readout[1].c_str(),NULL) * 1000;
					Readout.clear();
				}


				// if all required data has been input, switch PetroOxy on
				// Need the Gas Species ID... - but gas species could be scpecies 0....


				if(SetupParam.PetroOxySampleSize != 0 &&
						SetupParam.PetroOxyInitPressure != 0 &&
						SetupParam.PetroOxyMaxPressure != 0 &&
						OxyGasSpeciesDefined == true &&
						SetupParam.PetroOxyGasSolubility != 0
				){
					SetupParam.PetroOxy = true;
				}




				if(string::npos!=line1.find("Henry Law Diffusion Limit="))
				{
					char * cstr, *p;
					string str = line1;
					cstr = new char [str.size()+1];
					strcpy (cstr, str.c_str());

					p=strtok (cstr,"=");
					while (p!=NULL)
					{
						Readout.push_back(p);
						p=strtok(NULL,"<>");
					}
					delete[] cstr;
					delete[] p;

					SetupParam.HenryLawDiffusionLimit = strtod(Readout[1].c_str(),NULL);
					SetupParam.HenryLawDiffusionLimitSet = true;
					Readout.clear();

					//cout << "Henry Law Limiter is Set! \n"; // checked, works
				}



				/*
				 * End PetroOxy Data Input
				 */





				InitSpecies SingleSpeciesInput;

				// last option... the line contains a species...

				// check the string still contains data
				if(!line1.empty()){
					LineIn.clear();
					LineIn.resize(2);
					p=strtok (cstr," \t"); // p contains the species name

					for(i=0;i<Number_Species;i++){
						string Comparator = Species[i];
						if(strcmp(p,Comparator.c_str()) == 0)
						{
							SingleSpeciesInput.SpecID = i;
							// Obviously assigning only makes sense if I have a species name
							p=strtok(NULL," \t"); // p contains the concentration
							SingleSpeciesInput.SpecConc = strtod(p,NULL);
							SetupSpecies.push_back(SingleSpeciesInput);
						}
					}

					delete[] cstr;
					line1.clear();
				}
			}
		}
	}

	Input_Data.close();



	// Check that special requirements are met
	if(SetupParam.RatesMaxAnalysis && !SetupParam.irrev)
	{
		SetupParam.irrev = true;
		cout << "Rates Analysis requires an irreversible scheme - method set. \n";
	}

	if(SetupParam.RatesSpeciesAllAnalysis)
	{
		SetupParam.irrev = true;
		cout << "Rates Analysis requires an irreversible scheme - method set. \n";
	}

	if(SetupParam.StreamRatesAnalysis && !SetupParam.irrev)
	{
		SetupParam.irrev = true;
		cout << "Rates Analysis requires an irreversible scheme - method set. \n";
	}

	if(SetupParam.RatesAnalysisAtTime && !SetupParam.irrev)
	{
		SetupParam.irrev = true;
		cout << "Rates Analysis At Times requires an irreversible scheme - method set. \n";
	}

	if(SetupParam.ReduceReactions != 0 && !SetupParam.irrev)
	{
		SetupParam.irrev = true;
		cout << "Rates Based Mechanism Reduction requires an irreversible scheme - method set. \n";
	}
}
