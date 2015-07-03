#include <MyHeaders.h>

// http://stackoverflow.com/questions/7677007/passing-vector-to-function-c
// Pass vector by reference as input needs not be changed
vector< ThermodynamicData > Get_Thermodynamic_Data_New_Format(
		string filename, 
		const vector< string >& Species
)
{


	/*
	 * Design for new Thermodynamics Data Input
	 * Species Name 
	 * 3 temperatures - may be followed by elemental parameters
	 * 1 line low values
	 * 1 line high values
	 */


	ifstream Mechanism_Data;
	Mechanism_Data.open (filename.c_str());

	int Number_Species = Species.size();
	int i;

	// Vector to store all data as a struct (more efficient than vector< vector> >
	vector< ThermodynamicData > read_in_thermodynamics(Number_Species);



	if (Mechanism_Data.is_open())
	{
		string line1;
		size_t found;
		bool begin_flag = false;
		bool end_flag = false;

		while (Mechanism_Data.good())
		{
			getline (Mechanism_Data,line1);

			// Check if the Thermodynamic Data is Over and terminate the scheme
			found = line1.find("End"); // need to check for end in loop for 4 line blocks

			if (found!=string::npos && begin_flag)
			{
				end_flag = true;
			}



			if(begin_flag && !end_flag)
			{
				//cout << "checkoint 1";
				//  Check that the End has not been reached
				//  Check that the Line is not empty
				//  Check the initial character is not !, whitespace or tab
				//
				//  Does NOT filter blank lines with more than one white space...
				//
				if(
						!end_flag  && 
						!line1.empty() && 
						line1.compare(0,1,"!") != 0 //&&
						//line1.compare(0,1," ") != 0 && // normal whitespace
						//line1.compare(0,1,"	") != 0 // tab
				)
				{
					// One Species
					ThermodynamicData temp_read_in_single_species;

					// split the line by white spaces or tabs
					char *cstr, *p;
					vector< string > temp_split_line;

					string str = line1;

					cstr = new char [str.size()+1];
					strcpy (cstr, str.c_str());
					p=strtok (cstr," 	"); // split by space and tab

					while (p!=NULL)
					{
						temp_split_line.push_back(p);
						p=strtok(NULL," 	");
					}
					delete[] cstr;

					// for now we just want the species name, that is the initial input
					// Species Name is now temp_split_line[0]
					// Now we need to identify the species

					int temp_species_id = 0;

					for(i=0;i<Number_Species;i++)
					{
						string Comparator = Species[i];
						// Find the appropriate species to go with the thermodynamic data
						if(strcmp(temp_split_line[0].c_str(),Comparator.c_str()) == 0){
							temp_species_id = i; //mapping.push_back(i); // retain a map of the arrangement of the species
						}
					}
					temp_split_line.clear();


					// now the 3 temperature readings
					getline (Mechanism_Data,line1);

					str = line1;
					cstr = new char [str.size()+1];
					strcpy (cstr, str.c_str());
					p=strtok (cstr," 	"); // split by space and tab

					while (p!=NULL)
					{
						temp_split_line.push_back(p);
						p=strtok(NULL," 	");
					}

					delete[] cstr;

					temp_read_in_single_species.TLow = strtod(temp_split_line[0].c_str(),NULL);
					temp_read_in_single_species.THigh = strtod(temp_split_line[1].c_str(),NULL);
					temp_read_in_single_species.TChange = strtod(temp_split_line[2].c_str(),NULL);
					temp_split_line.clear();


					// now the low thermodynamics
					getline (Mechanism_Data,line1);

					str = line1;
					cstr = new char [str.size()+1];
					strcpy (cstr, str.c_str());
					p=strtok (cstr," 	"); // split by space and tab

					while (p!=NULL)
					{
						temp_split_line.push_back(p);
						p=strtok(NULL," 	");
					}

					delete[] cstr;

					temp_read_in_single_species.NasaLow1 = strtod(temp_split_line[0].c_str(),NULL);
					temp_read_in_single_species.NasaLow2 = strtod(temp_split_line[1].c_str(),NULL);
					temp_read_in_single_species.NasaLow3 = strtod(temp_split_line[2].c_str(),NULL);
					temp_read_in_single_species.NasaLow4 = strtod(temp_split_line[3].c_str(),NULL);
					temp_read_in_single_species.NasaLow5 = strtod(temp_split_line[4].c_str(),NULL);
					temp_read_in_single_species.NasaLow6 = strtod(temp_split_line[5].c_str(),NULL);
					temp_read_in_single_species.NasaLow7 = strtod(temp_split_line[6].c_str(),NULL);
					temp_split_line.clear();


					// now the high thermodynamics
					getline (Mechanism_Data,line1);

					str = line1;
					cstr = new char [str.size()+1];
					strcpy (cstr, str.c_str());
					p=strtok (cstr," 	"); // split by space and tab

					while (p!=NULL)
					{
						temp_split_line.push_back(p);
						p=strtok(NULL," 	");
					}

					delete[] cstr;
					delete[] p;

					temp_read_in_single_species.NasaHigh1 = strtod(temp_split_line[0].c_str(),NULL);
					temp_read_in_single_species.NasaHigh2 = strtod(temp_split_line[1].c_str(),NULL);
					temp_read_in_single_species.NasaHigh3 = strtod(temp_split_line[2].c_str(),NULL);
					temp_read_in_single_species.NasaHigh4 = strtod(temp_split_line[3].c_str(),NULL);
					temp_read_in_single_species.NasaHigh5 = strtod(temp_split_line[4].c_str(),NULL);
					temp_read_in_single_species.NasaHigh6 = strtod(temp_split_line[5].c_str(),NULL);
					temp_read_in_single_species.NasaHigh7 = strtod(temp_split_line[6].c_str(),NULL);
					temp_split_line.clear();


					// retain the input for one species mapped into the right location
					read_in_thermodynamics[temp_species_id] = temp_read_in_single_species;
					//cout << read_in_thermodynamics.size() << "\n";
					//cout << temp_species_id << "\n";

				}
			}



			// Check if the Thermodynamics Data Begins, if yes set a start flag
			found = line1.find("ThermData");
			if (found!=string::npos && !end_flag)
			{
				begin_flag=true;
			}


		}

		Mechanism_Data.close();

	}




	Mechanism_Data.open (filename.c_str());

	// standard data structure
	if (Mechanism_Data.is_open())
	{

		string line1;
		size_t found;
		bool begin_flag = false;
		bool end_flag = false;

		while (Mechanism_Data.good())
		{
			getline (Mechanism_Data,line1);


			if(begin_flag && !end_flag)
			{
				found = line1.find("END"); // need to check for end in loop for 4 line blocks
				if (found!=string::npos && begin_flag)
				{
					end_flag = true;
				}


				if(
						!end_flag &&
						!line1.empty() &&
						line1.compare(0,1,"!") != 0
				){ // Abort if end reACHED... or empty or comment
					ThermodynamicData temp_read_in_single_species;

					//http://www.cplusplus.com/reference/string/string/substr/ Substrings, exploit format
					/* First 17 characters are the species name
							Next 31 characters are just clutter
							Followed by 40 characters of temperatures
							The next 3 lines are 15 character blocks
					 */


					string line2, line3, line4;
					getline (Mechanism_Data,line2);
					getline (Mechanism_Data,line3);
					getline (Mechanism_Data,line4);

					string species_name, change_Temps;

					species_name = line1.substr(0,17); // contains name of species
					change_Temps = line1.substr(46,30); // contains low_T, high_T,change_T

					// Strip out whitespaces from species name
					istringstream iss1(species_name);
					string sub1;
					iss1 >> sub1;
					if(!sub1.empty()){ // avoid empty lines/entries
						species_name = sub1;
					}
					// End stripping out whitespaces

					int temp_species_id = 0;

					for(i=0;i<Number_Species;i++)
					{
						string Comparator = Species[i];
						// Find the appropriate species to go with the thermodynamic data
						if(strcmp(species_name.c_str(),Comparator.c_str()) == 0)
						{
							temp_species_id = i; //mapping.push_back(i); // retain a map of the arrangement of the species
						}
					}

					vector< double > SingleSpeciesData; // One row of data for one species

					// Process the 3 temperature entries low_T, high_T, change_T
					istringstream iss2(change_Temps);
					//int k = 0;
					do
					{
						string sub;
						iss2 >> sub;
						if(!sub.empty()){ // avoid empty lines
							SingleSpeciesData.push_back(strtod(sub.c_str(),NULL));
						}
					}while (iss2);

					temp_read_in_single_species.TLow = SingleSpeciesData[0];
					temp_read_in_single_species.THigh = SingleSpeciesData[1];
					temp_read_in_single_species.TChange = SingleSpeciesData[2];


					temp_read_in_single_species.NasaLow1 =
							strtod(line2.substr( 0,15).c_str(),NULL);
					temp_read_in_single_species.NasaLow2 =
							strtod(line2.substr(15,15).c_str(),NULL);
					temp_read_in_single_species.NasaLow3 =
							strtod(line2.substr(30,15).c_str(),NULL);
					temp_read_in_single_species.NasaLow4 =
							strtod(line2.substr(45,15).c_str(),NULL);
					temp_read_in_single_species.NasaLow5 =
							strtod(line2.substr(60,15).c_str(),NULL);

					temp_read_in_single_species.NasaLow6 =
							strtod(line3.substr( 0,15).c_str(),NULL);
					temp_read_in_single_species.NasaLow7 =
							strtod(line3.substr(15,15).c_str(),NULL);
					temp_read_in_single_species.NasaHigh1 =
							strtod(line3.substr(30,15).c_str(),NULL);
					temp_read_in_single_species.NasaHigh2 =
							strtod(line3.substr(45,15).c_str(),NULL);
					temp_read_in_single_species.NasaHigh3 =
							strtod(line3.substr(60,15).c_str(),NULL);

					temp_read_in_single_species.NasaHigh4 =
							strtod(line4.substr( 0,15).c_str(),NULL);
					temp_read_in_single_species.NasaHigh5 =
							strtod(line4.substr(15,15).c_str(),NULL);
					temp_read_in_single_species.NasaHigh6 =
							strtod(line4.substr(30,15).c_str(),NULL);
					temp_read_in_single_species.NasaHigh7 =
							strtod(line4.substr(45,15).c_str(),NULL);


					read_in_thermodynamics[temp_species_id] = temp_read_in_single_species;

				}

			}


			// Moving thermo data check to end of function avoids "THERMO" being read in as a name


			found = line1.find("THERM");
			if (found!=string::npos && !end_flag)
			{
				//cout << "found begining of thermo \n";
				begin_flag=true;
				getline (Mechanism_Data,line1);
			}

		}

		Mechanism_Data.close();
	}


	return read_in_thermodynamics;
}
