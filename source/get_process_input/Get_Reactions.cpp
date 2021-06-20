#include "../headers/Headers.hpp"

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

	/* class Reaction_Mechanism {
	 * public:
	 * int species;
	 * int reactions;
	 *
	 * vector< string > Species;
	 * vector< ThermodynamicData > Thermodynamics;
	 * vector< SingleReactionData > Reactions;
	 * };
	 */

	// most mechanisms only have megabyte sizes, thus it is possible to
	// store the mechanism in memory for easier string processing
	vector<string> Reactions_List;

	// we prepare the output vector that we will return
	vector< SingleReactionData > Reaction_Data;

	// we open the file that contains the mechanims data
	ifstream Mechanism_Data;
	Mechanism_Data.open (filename.c_str());

	vector< int > SchemeUnits(2); // identifiers for units of A and Ea
	vector< int > mapping;

	int begin_flag = 0, end_flag = 0; // treat as Boolean, true/false


	//size_t Number_Species = Species.size();

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
				//found = line.find("END"); // need to check for end in loop for 4 line blocks
				//if(Test_If_Word_Found(line,"END") && begin_flag)
				if(line.compare(0,3,"END") == 0 || line.compare(0,3,"end") == 0)
					//if (found!=string::npos && begin_flag)
				{
					//cout << "END found\n";
					end_flag = 1;
				}

				if(end_flag == 0 && !line.empty() && line.compare(0,1,"!") != 0) // Abort if end reached or initial line is comment
				{
					Reactions_List.push_back(line);
				}
			}

			// Moving reaction data check to end of function avoids "REAC" being read in as a name
			if(Test_If_Word_Found(line,"REAC") && line.compare(0,1,"!") != 0 && begin_flag == 0) // only test once... once we found the reactions, its done
			{
				//cout << "REAC found\n";
				begin_flag=1;
				SchemeUnits = Set_Mechanism_Units(line);
			}
		}
		Mechanism_Data.close();
	}



	// even the largest mechanisms will be on a MB scale, so we can read all and store it in memory.
	// next we need to process it line for line:
	for(size_t i=0;i<Reactions_List.size();i++)
	{

		string line = Reactions_List[i];
		// is the line a comment? - no, OK let us process it
		if(line.compare(0,1,"!") != 0)
		{
			// if not, is is a duplicate?
			if(Reaction_Data.size() > 0 && line.compare(0,1,"/") != 0 && line.compare(0,3,"DUP") == 0)
			{
				size_t position = Reaction_Data.size() - 1;
				Reaction_Data[position].IsDuplicate=true;
			}
			// is it a third body configuration?
			// this will not work for white spaces before the slash...
			else if(Reaction_Data.size() > 0 && line.compare(0,1,"/") == 0)
			{
				// applies to the last reaction in the list
			}
			else if(Reaction_Data.size() > 0 && Test_If_Word_Found(line, "LOW") && line.find("=")==string::npos) // LOW term for third bodies line contains no equal
			{
				vector<double> tmp =  Tokenise_String_To_Double(line, "lowLOW \t/" );
				size_t position = Reaction_Data.size() - 1;
				//Reaction_Data[position].ThBd_LOW = tmp; // for now, store properly later
				if(tmp.size() == 3)
				{
					Reaction_Data[position].lowThirdBody.paramA0 = tmp[0];
					Reaction_Data[position].lowThirdBody.paramN0 = tmp[1];
					Reaction_Data[position].lowThirdBody.paramEa0 = tmp[2];
				}

			}
			else if(Reaction_Data.size() > 0 && Test_If_Word_Found(line, "TROE")&& line.find("=")==string::npos) // TROE term for third bodies line contains no equal
			{
				vector<double> tmp  = Tokenise_String_To_Double(line, "troeTROE \t/");
				size_t position = Reaction_Data.size() - 1;
				//Reaction_Data[position].ThBd_TROE = tmp; // for now, store properly later

				// values are: a, T3, T1, T2
				if(tmp.size() == 3) // 3 parameter troe
				{
					Reaction_Data[position].troeThirdBody.a = tmp[0];
					Reaction_Data[position].troeThirdBody.T2 = tmp[1];
					Reaction_Data[position].troeThirdBody.T1 = tmp[2];
					if(tmp.size() == 4) // 4 parameter troe
					{
						Reaction_Data[position].troeThirdBody.T2 = tmp[3];
					}
				}
			}
			else // well, not a comment, not a duplicate, not a third body config -> so must be a reaction
			{
				// let us extract the reaction then:
				SingleReactionData tmp;
				if(Parse_Chemking_Reaction_String(tmp, SchemeUnits, Species, line)) // only pushback if successful
				{
					Reaction_Data.push_back(tmp);
				}


			}
		}
	}

	return Reaction_Data;

}


// could have a line that is empty after stripping comments, should not happen, but...
bool Parse_Chemking_Reaction_String(SingleReactionData& result, const vector< int > SchemeUnits, const vector<string> SpeciesNames, string line)
{
	size_t Number_Species= SpeciesNames.size();

	vector< double > ReactantData; // Reactant Information
	ReactantData.resize(Number_Species);
	vector< double > ProductData; // Product Information
	ProductData.resize(Number_Species);
	vector< double > ReactionData; // Reaction parameters such as Arrhenius parameters and whether irreversible or not
	ReactionData.resize(4); // A, n, Ea and whether reversible or irreversible

	// Make New Input
	SingleReactionData temp;

	// Split by = or => sign
	vector< string > SplitLineIn;
	vector< string > RemoveComments;

	RemoveComments = Tokenise_String_To_String(line , "!" );
	line = RemoveComments[0];
	RemoveComments.clear();

	//cout << "0010" << line << "\n";

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

		// we have been handling the entire reaction line, lets check for a third body indicator:
		temp.collision_efficiency = false; // default
		// check for Third Body Indicator:
		temp.ThirdBodyType = 0; // no third body, default
		if(Test_If_Word_Found(line, "+M") || Test_If_Word_Found(line, "+ M")) // first type, set parameter
		{
			temp.ThirdBodyType = 1;

			// strip off the +M
			line = Remove_Substring(line,"+M");
			line = Remove_Substring(line,"+ M");
		}
		if(Test_If_Word_Found(line, "+(M)") || Test_If_Word_Found(line, "(+M)")) // apparently not the first, but the second type
		{
			temp.ThirdBodyType = 2;

			// strip off the +M
			line = Remove_Substring(line,"+ (M)");
			line = Remove_Substring(line,"+(M)");
			line = Remove_Substring(line,"(+M)");
		}
		// there is also special third body configs for water not yet treated...

		////
		//// Really should add the +M handling into the reaction processing...
		////

		SplitLineIn = Tokenise_String_To_String(line, "<=>");

		vector< string > SplitLineLeft;
		vector< string > SplitLineRight;

		// SplitLineLeft Contains all inputs on the left hand side
		SplitLineLeft = Tokenise_String_To_String(SplitLineIn[0],"\t +");
		// SplitLineRight Contains all inputs on the right hand side -> last 3 are parameters
		SplitLineRight = Tokenise_String_To_String(SplitLineIn[1],"\t +");


		size_t SplitLineLeftSize, SplitLineRightSize;
		SplitLineLeftSize = SplitLineLeft.size();
		SplitLineRightSize = SplitLineRight.size() - 3; //(last 3 entries are Arrhenius Parameters

		//cout << SplitLineLeftSize << " " << SplitLineRightSize << "\n";

		for(size_t i = 0;i<SplitLineLeftSize;i++) // process all entries on left hand side
		{
			SpeciesWithCoefficient SpeciesAndCoefficient;
			SpeciesAndCoefficient = Return_Species_With_Coefficient(SplitLineLeft[i], SpeciesNames);

			if(SpeciesAndCoefficient.ismatched)
			{
				ReactantData[SpeciesAndCoefficient.SpeciesID] =
						ReactantData[SpeciesAndCoefficient.SpeciesID] - SpeciesAndCoefficient.coefficient;
			}
		}



		for(size_t i = 0;i<SplitLineRightSize;i++) // process all entries on right hand side
		{

			SpeciesWithCoefficient SpeciesAndCoefficient;
			SpeciesAndCoefficient = Return_Species_With_Coefficient(SplitLineRight[i], SpeciesNames);

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
		size_t SplitLineSize = SplitLine.size();
		step = (strtod(SplitLine[SplitLineSize - 3].c_str(),NULL)); // A as read in
		ReactionData[0] = Scale_A(step, ReactantData, SchemeUnits[0]); // A in adjusted units for calc.

		ReactionData[1]=(strtod(SplitLine[SplitLineSize - 2].c_str(),NULL)); // n as read in

		step = (strtod(SplitLine[SplitLineSize - 1].c_str(),NULL)); // Ea as read in
		ReactionData[2] = Scale_Ea(step,SchemeUnits[1]); // Ea in adjusted units for calc.



		temp.Reactants = ReactantData;
		temp.Products = ProductData;
		temp.paramA = ReactionData[0];
		temp.paramN = ReactionData[1];
		temp.paramEa = ReactionData[2];
		temp.Reversible = is_reversible;
		temp.IsDuplicate = false; // default, not a duplicate



		// there was data processing
		result = temp;
		return true;
	}

	return false;
}






// Helper function which splits species coefficient from species name
SpeciesWithCoefficient Return_Species_With_Coefficient(
		string InputToSplit,
		const vector< string > Species
)
{
	size_t i,j;
	size_t Number_Species = Species.size();
	SpeciesWithCoefficient SpeciesAndCoefficient;
	SpeciesAndCoefficient.ismatched = false; // default, defined here not globally

	// isalpha returns true if entry is upper or lower case letter
	i = 0;

	// work through every character of the string checking whether it is alphabetic
	// if the character is alphabetic, we have reached the species name
	// if it is not alphabetic we have the coefficient
	while(!isalpha(InputToSplit[i]) && i < InputToSplit.size())
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

