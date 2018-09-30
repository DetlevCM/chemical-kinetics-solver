#include <Headers.hpp>

/* The Function in this file deals with obtaining the reactions mechanism
 * data from the text input in a format that may be processed by the
 * program. The reactants, products and reaction parameters are read into
 * a number of 2D arrays (laid out as reaction ID vs species ID which are
 * output as a 3D array, structured as follows:
 * 1: reactant coefficients
 * 2: product coefficients
 * 3: reaction parameters
 * If extras functionality is to be added, this will make use of the third layer.
 * One could add a fourth layer, though it is questionable whether this is at
 * this point. The layout will scale if extra layers are desired.  */

// Some of the repetitive code in here should be stuck into a function... maybe limited just to the scope of this file
// The species search/comparison should be global

// http://stackoverflow.com/questions/7677007/passing-vector-to-function-c
// Pass vector by reference as input needs not be changed





SpeciesWithCoefficient Return_Species_With_Coefficient(string  , const vector< string > );



vector< SingleReactionData > Get_Reactions(
		string filename,
		const vector< string >& Species
){

	/* Struct Reaction Matrix
	 * 1) Vector Ractants
	 * 2) Vector Reactants
	 * 3) boolean reversible, doubles for A, n, Ea
	 */

	vector<string> Reactions_List; // vector to hold the list of reactions for processing

	vector< SingleReactionData > Reaction_Matrix;

	ifstream Mechanism_Data;
	Mechanism_Data.open (filename.c_str());

	vector< int > SchemeUnits(2); // identifiers for units of A and Ea
	vector< int > mapping;

	int begin_flag = 0, end_flag = 0; // treat as Boolean, true/false

	size_t found;

	int Number_Species = (int)Species.size();

	/*
	 * New implementation:
	 * 1: read the entire mechanism into a vector (this needs more RAM)
	 * 2: process the reactions into the mechanism for the solver
	 */


	if (Mechanism_Data.is_open())
	{
		string line;
		while (Mechanism_Data.good())
		{
			getline (Mechanism_Data,line);

			if(begin_flag && !end_flag)
			{
				found = line.find("END"); // need to check for end in loop for 4 line blocks
				if (found!=string::npos && begin_flag)
				{
					end_flag = 1;
				}

				if(!end_flag && !line.empty() && line.compare(0,1,"!") != 0) // Abort if end reached or initial line is comment
				{
					Reactions_List.push_back(line);
				}
			}

			// Moving reaction data check to end of function avoids "REAC" being read in as a name
			if(Test_If_Word_Found(line,"REAC") && !begin_flag) // only test once... once we found the reactions, its done
			{
				begin_flag=1;
				SchemeUnits = Set_Mechanism_Units(line);
			}
		}
		Mechanism_Data.close();
	}

	for(int j=0;j<(int)Reactions_List.size();j++)
	{
		string line = Reactions_List[j];
		vector< double > ReactantData; // Reactant Information
		ReactantData.resize(Number_Species);
		vector< double > ProductData; // Product Information
		ProductData.resize(Number_Species);
		vector< double > ReactionData; // Reaction parameters such as Arrhenius parameters and whether irreversible or not
		ReactionData.resize(4); // A, n, Ea and whether reversible or irreversible

		// Reaction is marked a duplicate (First line cannot be "DUP", else this will crash...)
		if(line.compare(0,1,"!") != 0  && line.compare(0,1,"/") != 0 && line.compare(0,3,"DUP") == 0)
		{
			int position = (int) Reaction_Matrix.size() - 1;
			Reaction_Matrix[position].IsDuplicate=true;
		}
		// content in here - check if line does not start with a comment, ! or / or DUP
		else if(
				line.compare(0,1,"!") != 0 &&
				line.compare(0,1,"/") != 0 &&
				line.compare(0,3,"DUP") != 0 &&
				!Test_If_Word_Found(line, "LOW/") && // not LOW term for third bodies
				!Test_If_Word_Found(line, "TROE/") // not TROE term for third bodies
		)
		{
			// Split by = or => sign
			vector< string > SplitLineIn;
			vector< string > RemoveComments;

			RemoveComments = Tokenise_String_To_String(line , "!" );
			line = RemoveComments[0];
			RemoveComments.clear();


			// only continue if the string is not empty or only whitespace or only tab
			if(!line.empty() && line.find_first_not_of("\t ") != string::npos)
			{
				// Determine if the reaction is irreversible. We assume it is reversible and then correct.
				// irreversible is indicated using => or ->
				bool is_reversible = true;
				if(Test_If_Word_Found(line,"=>") || Test_If_Word_Found(line,"->"))
				{
					is_reversible = false;
					// assume the user may write "<=>" or "<=>" which he shouldn't...
					if(Test_If_Word_Found(line,"<=>") || Test_If_Word_Found(line,"<->"))
					{
						is_reversible = true;
					}
				}


				SplitLineIn = Tokenise_String_To_String(line, "<=>");

				vector< string > SplitLineLeft;
				vector< string > SplitLineRight;

				// SplitLineLeft Contains all inputs on the left hand side
				SplitLineLeft = Tokenise_String_To_String(SplitLineIn[0],"\t +");
				// SplitLineRight Contains all inputs on the right hand side -> last 3 are parameters
				SplitLineRight = Tokenise_String_To_String(SplitLineIn[1],"\t +");


				int SplitLineLeftSize, SplitLineRightSize;
				SplitLineLeftSize = (int)SplitLineLeft.size();
				SplitLineRightSize = (int)SplitLineRight.size() - 3; //(last 3 entries are Arrhenius Parameters

				//cout << SplitLineLeftSize << " " << SplitLineRightSize << "\n";

				for(int i = 0;i<SplitLineLeftSize;i++) // process all entries on left hand side
				{
					SpeciesWithCoefficient SpeciesAndCoefficient;
					SpeciesAndCoefficient = Return_Species_With_Coefficient(SplitLineLeft[i], Species);

					if(SpeciesAndCoefficient.ismatched)
					{
						ReactantData[SpeciesAndCoefficient.SpeciesID] =
								ReactantData[SpeciesAndCoefficient.SpeciesID] - SpeciesAndCoefficient.coefficient;
					}
				}



				for(int i = 0;i<SplitLineRightSize;i++) // process all entries on right hand side
				{

					SpeciesWithCoefficient SpeciesAndCoefficient;
					SpeciesAndCoefficient = Return_Species_With_Coefficient(SplitLineRight[i], Species);

					if(SpeciesAndCoefficient.ismatched)
					{
						ProductData[SpeciesAndCoefficient.SpeciesID] =
								ProductData[SpeciesAndCoefficient.SpeciesID] + SpeciesAndCoefficient.coefficient;
					}
				}


				// In a next step we need to handle the Arrhenius parameters
				// Tokenize line, then take last 3 positions - easiest to work on the whole line without comments
				vector< string > SplitLine;

				double step;

				SplitLine = Tokenise_String_To_String(line,"\t ");
				int SplitLineSize = (int)SplitLine.size();
				step = (strtod(SplitLine[SplitLineSize - 3].c_str(),NULL)); // A as read in
				ReactionData[0] = Scale_A(step, ReactantData, SchemeUnits[0]); // A in adjusted units for calc.

				ReactionData[1]=(strtod(SplitLine[SplitLineSize - 2].c_str(),NULL)); // n as read in

				step = (strtod(SplitLine[SplitLineSize - 1].c_str(),NULL)); // Ea as read in
				ReactionData[2] = Scale_Ea(step,SchemeUnits[1]); // Ea in adjusted units for calc.

				// Make New Input
				SingleReactionData temp;
				vector<ThirdBodyParameters> ThBd_param;
				// check for Third Body Indicator:
				temp.ThirdBodyType = 0; // no third body, default
				if(Test_If_Word_Found(line, "+M")) // first type, set parameter
				{
					temp.ThirdBodyType = 1;
				}
				if(Test_If_Word_Found(line, "+(M)")) // apparently not the first, but the second type
				{
					temp.ThirdBodyType = 2;
				}
				// if there is a third body reaction, we need to read in the parameters...
				if(temp.ThirdBodyType == 1 &&
						j + 1 < (int)Reactions_List.size() // we need a minimum of one extra line
				)
				{
					// a common feature of the pressure dependency parameters is the
					// presence of a slash and the lack of an equal sign
					while(Test_If_Word_Found(Reactions_List[j+1], "/")
							&&
							!Test_If_Word_Found(Reactions_List[j+1], "=")
					) // the initial implementation will assume a single line of entries
					{
						// split the string and get the values
						vector<string> PdepTerms = Tokenise_String_To_String(Reactions_List[j+1]," \t/");
						// now this should contain a list of entries - name, number
						// the length should be a multiple of two
						ThirdBodyParameters tmp_ThBd_param;

						for(int k=0;k<((int)PdepTerms.size()/2);k++) // this WILL fail if the number of elements does not divide by 2
						{
							for(int m=0;m<Number_Species;m++)
							{
								if(Test_If_Word_Found(Species[m],PdepTerms[2*k]) &&
										(int)Species[m].size() - (int)PdepTerms[2*k].size() == 0) // test for same length
								{
									tmp_ThBd_param.SpeciesID = m;
									tmp_ThBd_param.value = stod(PdepTerms[2*k+1],NULL);
									ThBd_param.push_back(tmp_ThBd_param);
								}
							}
						}
						j = j+1; // the loop will end when the next line contains a species
					}
				}
				else if(
						temp.ThirdBodyType == 2 &&
						j + 3 < (int)Reactions_List.size() // we need a minimum of 3 extra lines
				)
				{
					// LOW/TROE parameters
					// we can test the next lines for their presence and extract the values
					if(
							Test_If_Word_Found(Reactions_List[j+1], "LOW/")
							&&
							Test_If_Word_Found(Reactions_List[j+2], "TROE/")
					)
					{
						temp.ThBd_LOW =  Tokenise_String_To_Double(Reactions_List[j+1] , "lowLOW \t/" );
						temp.ThBd_TROE = Tokenise_String_To_Double(Reactions_List[j+2] , "troeTROE \t/" );
						j = j + 2; // set counter forward, to skip these lines
						// the next line would be "species_name/ number/ ..."

						// a common feature of the pressure dependency parameters is the
						// presence of a slash and the lack of an equal sign
						while(Test_If_Word_Found(Reactions_List[j+1], "/")
								&&
								!Test_If_Word_Found(Reactions_List[j+1], "=")
						)
						{
							// split the string and get the values
							vector<string> PdepTerms = Tokenise_String_To_String(Reactions_List[j+1]," \t/");
							// now this should contain a list of entries - name, number
							// the length should be a multiple of two
							ThirdBodyParameters tmp_ThBd_param;

							for(int k=0;k<((int)PdepTerms.size()/2);k++) // this WILL fail if the number of elements does not divide by 2
							{
								for(int m=0;m<Number_Species;m++)
								{
									if(Test_If_Word_Found(Species[m],PdepTerms[2*k]) &&
											(int)Species[m].size() - (int)PdepTerms[2*k].size() == 0) // test for same length
									{
										tmp_ThBd_param.SpeciesID = m;
										tmp_ThBd_param.value = stod(PdepTerms[2*k+1],NULL);
										ThBd_param.push_back(tmp_ThBd_param);
									}
								}
							}
							j = j+1; // the loop will end when the next line contains a species
						}
					}


				}
				//cout << Reactions_List[j] << "\n";

				temp.Reactants = ReactantData;
				temp.Products = ProductData;
				temp.paramA = ReactionData[0];
				temp.paramN = ReactionData[1];
				temp.paramEa = ReactionData[2];
				temp.Reversible = is_reversible;
				temp.IsDuplicate = false; // default, not a duplicate

				Reaction_Matrix.push_back(temp);

				ReactantData.clear();
				ProductData.clear();
				ReactionData.clear();

			}
		}
	}


	return Reaction_Matrix;
}




// Helper function which splits species coefficient from species name
SpeciesWithCoefficient Return_Species_With_Coefficient(
		string InputToSplit,
		const vector< string > Species
)
{
	int i,j;
	int Number_Species = (int)Species.size();
	SpeciesWithCoefficient SpeciesAndCoefficient;
	SpeciesAndCoefficient.ismatched = false; // default, defined here not globally

	// isalpha returns true if entry is upper or lower case letter
	i = 0;

	// work through every character of the string checking whether it is alphabetic
	// if the character is alphabetic, we have reached the species name
	// if it is not alphabetic we have the coefficient
	while(!isalpha(InputToSplit[i]) && i < (int) InputToSplit.size())
	{
		i++;
	}
	// i now contains the count of non-alphabetical characters in front of the species

	// In the next step we need to process the species name (remainder of the string)
	// and add it to the appropriate location in the array

	string species_name;
	double species_coefficient = 0;
	species_name = InputToSplit.substr(i,InputToSplit.size()-i);
	species_coefficient = strtod(InputToSplit.c_str(),NULL);

	j = 0;
	while(j<Number_Species && !SpeciesAndCoefficient.ismatched) // just loop until the species is found
	{
		string Comparator = Species[j];

		// Find the appropriate species and its position
		if(strcmp(species_name.c_str(),Comparator.c_str()) == 0){
			if(species_coefficient == 0)
			{
				species_coefficient = 1; // Deal with implicit coefficient of 1
			}
			SpeciesAndCoefficient.SpeciesID = j;
			SpeciesAndCoefficient.coefficient = species_coefficient;
			SpeciesAndCoefficient.ismatched = true;
		}

		j = j + 1;
	}

	return SpeciesAndCoefficient;
}

