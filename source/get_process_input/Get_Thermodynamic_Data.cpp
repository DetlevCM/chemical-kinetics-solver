#include "../headers/Headers.hpp"

// http://stackoverflow.com/questions/7677007/passing-vector-to-function-c
// Pass vector by reference as input needs not be changed
vector< ThermodynamicData > Get_Thermodynamic_Data(
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

	size_t Number_Species = Species.size();
	size_t i;

	// Vector to store all data as a struct (more efficient than vector< vector> >
	vector< ThermodynamicData > read_in_thermodynamics;
	read_in_thermodynamics.resize(Number_Species);

	// Thermodynamic data should be read as a block and processed in a second step as this is more
	// robust. This is the same approach as is used for the reactions.
	vector<string> Thermodynamics_List;

	int thermo_type = 0; // 0 = standard Chemkin NASA-7, 1 = different internal format

	if (Mechanism_Data.is_open())
	{
		bool begin_flag = false;
		bool end_flag = false;

		string line;
		while (Mechanism_Data.good())
		{
			getline (Mechanism_Data,line);

			if(begin_flag && !end_flag)
			{
				if(line.compare(0,3,"END") == 0 || line.compare(0,3,"end") == 0)
				{
					//cout << "END found\n";
					end_flag = true;
				}

				// filter out empty lines and comments
				if(end_flag == false && !line.empty() && line.compare(0,1,"!") != 0) // Abort if end reached or initial line is comment
				{
					Thermodynamics_List.push_back(line);
				}
			}

			if(Test_If_Word_Found(line,"THERM") && line.compare(0,1,"!") != 0 && begin_flag == 0) // only test once... once we found the end its done
			{
				/// cout << "start found\n";
				begin_flag = true;
			}
			if(Test_If_Word_Found(line,"ThermData") && line.compare(0,1,"!") != 0 && begin_flag == 0) // only test once... once we found the end its done
			{
				/// cout << "start found\n";
				begin_flag = true;
				thermo_type = 1;
			}
		}
		Mechanism_Data.close();
	}


	if(thermo_type == 0) // standard chemkin thermo format
	{
		Process_Chemkin_Thermo_Format(
				read_in_thermodynamics,
				Species,
				Thermodynamics_List
		);
	}
	else // if(thermo_type == 1) // internal thermo format
	{
		Process_Internal_Thermo_Format(
				read_in_thermodynamics,
				Species,
				Thermodynamics_List
		);

	}

	return read_in_thermodynamics;
}


void Process_Internal_Thermo_Format(
		vector< ThermodynamicData >& read_in_thermodynamics,
		const vector<string>& Species,
		const vector<string>& Thermodynamics_List
)
{

	for(size_t i = 0;i<Thermodynamics_List.size()-2;i++) // we need 3 lines per species
	{

		size_t Number_Species = Species.size();
		// One Species
		ThermodynamicData temp_read_in_single_species;

		// split the line by white spaces or tabs

		vector< string > temp_split_line;
		vector< double > temp_split_line_double;

		temp_split_line = Tokenise_String_To_String(Thermodynamics_List[i] , " 	" );

		// for now we just want the species name, that is the initial input
		// Species Name is now temp_split_line[0]
		// Now we need to identify the species

		size_t temp_species_id = 0;

		bool species_found = false;

		for(size_t j=0;j<Number_Species;j++)
		{
			string Comparator = Species[i];
			// Find the appropriate species to go with the thermodynamic data
			if(strcmp(temp_split_line[0].c_str(),Comparator.c_str()) == 0){
				temp_species_id = j; //mapping.push_back(i); // retain a map of the arrangement of the species
				species_found = true;
			}
		}
		temp_split_line.clear();

		// we read in a species, so we have data :)
		if(species_found)
		{

			// now the 3 temperature readings
			//getline (Mechanism_Data,line1);
			string line1 = Thermodynamics_List[i];
			temp_split_line_double = Tokenise_String_To_Double(line1 , " 	");
			temp_read_in_single_species.TLow = temp_split_line_double[0];
			temp_read_in_single_species.THigh = temp_split_line_double[1];
			temp_read_in_single_species.TChange = temp_split_line_double[2];
			temp_split_line_double.clear();

			// now the low thermodynamics
			//getline (Mechanism_Data,line1);
			string line2 = Thermodynamics_List[i+1];

			temp_split_line_double = Tokenise_String_To_Double(line2 , " 	");
			temp_read_in_single_species.NasaLow1 = temp_split_line_double[0];
			temp_read_in_single_species.NasaLow2 = temp_split_line_double[1];
			temp_read_in_single_species.NasaLow3 = temp_split_line_double[2];
			temp_read_in_single_species.NasaLow4 = temp_split_line_double[3];
			temp_read_in_single_species.NasaLow5 = temp_split_line_double[4];
			temp_read_in_single_species.NasaLow6 = temp_split_line_double[5];
			temp_read_in_single_species.NasaLow7 = temp_split_line_double[6];
			temp_split_line_double.clear();

			// now the high thermodynamics
			//getline (Mechanism_Data,line1);
			string line3 = Thermodynamics_List[i+2];

			temp_split_line_double = Tokenise_String_To_Double(line2 , " 	");
			temp_read_in_single_species.NasaHigh1 = temp_split_line_double[0];
			temp_read_in_single_species.NasaHigh2 = temp_split_line_double[1];
			temp_read_in_single_species.NasaHigh3 = temp_split_line_double[2];
			temp_read_in_single_species.NasaHigh4 = temp_split_line_double[3];
			temp_read_in_single_species.NasaHigh5 = temp_split_line_double[4];
			temp_read_in_single_species.NasaHigh6 = temp_split_line_double[5];
			temp_read_in_single_species.NasaHigh7 = temp_split_line_double[6];
			temp_split_line_double.clear();


			// retain the input for one species mapped into the right location
			read_in_thermodynamics[temp_species_id] = temp_read_in_single_species;
		}
	}
}

void Process_Chemkin_Thermo_Format(
		vector< ThermodynamicData >& read_in_thermodynamics,
		const vector<string>& Species,
		const vector<string>& Thermodynamics_List
)
{
	// start from the second line, the first line contains three temperatures...
	for(size_t i = 1;i<Thermodynamics_List.size()-3;i++) // we use 4 lines per species
	{
		ThermodynamicData temp_read_in_single_species;

		//http://www.cplusplus.com/reference/string/string/substr/ Substrings, exploit format
		/* First 17 characters are the species name
								Next 31 characters are just clutter
								Followed by 40 characters of temperatures
								The next 3 lines are 15 character blocks
		 */
		size_t Number_Species = Species.size();

		// should really check that the first entry contains a species...
		string line1 = Thermodynamics_List[i];
		string line2 = Thermodynamics_List[i+1];
		string line3 = Thermodynamics_List[i+2];
		string line4 = Thermodynamics_List[i+3];
		//cout << line1 << "\n" <<line2 << "\n" << line3 << "\n" << line4 << "\n";

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

		size_t temp_species_id = 0;

		for(size_t j=0;j<Number_Species;j++)
		{
			string Comparator = Species[j];
			// Find the appropriate species to go with the thermodynamic data
			if(strcmp(species_name.c_str(),Comparator.c_str()) == 0)
			{
				temp_species_id = j; //mapping.push_back(i); // retain a map of the arrangement of the species
			}
		}

		vector< double > SingleSpeciesData; // One row of data for one species

		// Process the 3 temperature entries low_T, high_T, change_T
		istringstream iss2(change_Temps);
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

		temp_read_in_single_species.NasaHigh1 =
				strtod(line2.substr( 0,15).c_str(),NULL);
		temp_read_in_single_species.NasaHigh2 =
				strtod(line2.substr(15,15).c_str(),NULL);
		temp_read_in_single_species.NasaHigh3 =
				strtod(line2.substr(30,15).c_str(),NULL);
		temp_read_in_single_species.NasaHigh4 =
				strtod(line2.substr(45,15).c_str(),NULL);
		temp_read_in_single_species.NasaHigh5 =
				strtod(line2.substr(60,15).c_str(),NULL);

		temp_read_in_single_species.NasaHigh6 =
				strtod(line3.substr( 0,15).c_str(),NULL);
		temp_read_in_single_species.NasaHigh7 =
				strtod(line3.substr(15,15).c_str(),NULL);
		temp_read_in_single_species.NasaLow1 =
				strtod(line3.substr(30,15).c_str(),NULL);
		temp_read_in_single_species.NasaLow2 =
				strtod(line3.substr(45,15).c_str(),NULL);
		temp_read_in_single_species.NasaLow3 =
				strtod(line3.substr(60,15).c_str(),NULL);

		temp_read_in_single_species.NasaLow4 =
				strtod(line4.substr( 0,15).c_str(),NULL);
		temp_read_in_single_species.NasaLow5 =
				strtod(line4.substr(15,15).c_str(),NULL);
		temp_read_in_single_species.NasaLow6 =
				strtod(line4.substr(30,15).c_str(),NULL);
		temp_read_in_single_species.NasaLow7 =
				strtod(line4.substr(45,15).c_str(),NULL);

		read_in_thermodynamics[temp_species_id] = temp_read_in_single_species;
	}
}



