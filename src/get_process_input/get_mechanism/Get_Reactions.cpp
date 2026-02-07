
#include "ReactionMechanism.h"
#include "Species.h"

/* The Function in this file deals with obtaining the reactions mechanism
 * data from the text input in a format that may be processed by the
 * program. The reactants, products and reaction parameters are read into
 * a number of 2D arrays (laid out as reaction ID vs species ID which are
 * output as a 3D array, structured as follows:
 * 1: reactant coefficients
 * 2: product coefficients
 * 3: reaction parameters
 * If extras functionality is to be added, this will make use of the third
 * layer. One could add a fourth layer, though it is questionable whether this
 * is at this point. The layout will scale if extra layers are desired.  */

// Some of the repetitive code in here should be stuck into a function... maybe
// limited just to the scope of this file The species search/comparison should
// be global

// http://stackoverflow.com/questions/7677007/passing-vector-to-function-c
// Pass vector by reference as input needs not be changed

vector<SingleReactionData>
ReactionMechanism::Get_Reactions(string filename,
                                 const vector<Species> &species) {

  // we prepare the output vector that we will return
  vector<SingleReactionData> Reaction_Data;

  vector<int> SchemeUnits(2); // identifiers for units of A and Ea
  vector<int> mapping;

  // most mechanisms only have megabyte sizes, thus it is possible to
  // store the mechanism in memory for easier string processing
  vector<string> Reactions_List = Read_Chemkin_Block(filename, "REAC");

  // the first line contains units
  SchemeUnits = Set_Mechanism_Units(Reactions_List[0]);

  // Reactions_List contains
  // - reactions
  // - reverse parameters
  // - duplicate indicators
  // - third body parameters

  int counter = -1; // track counter where we are -> first reaction = 0, add
                    // one, so start at -1

  // start at line 2, as line 1 is mechanism units
  // cout << "mechanism length: " << Reactions_List.size() << "\n";
  for (size_t j = 1; j < Reactions_List.size(); j++) {

    string line = Reactions_List[j];
    // cout << "line: " << j << " " << line << "\n";

    // if not, is is a duplicate? - some bad structure for how duplicates are
    // entered...
    if (Reaction_Data.size() > 0 && line.compare(0, 1, "/") != 0 &&
        line.compare(0, 3, "DUP") == 0) {
      Reaction_Data[counter].IsDuplicate = true;
    }
    // TODO: REV not yet implemented in calculation
    else if (Reaction_Data.size() > 0 && Test_If_Word_Found(line, "REV") &&
             line.find("=") == string::npos) {
      vector<double> tmp = Tokenise_String_To_Double(line, " \t/");
      Reaction_Data[counter].explicit_reverse = true;
      Reaction_Data[counter].reverse.A =
          Scale_A(tmp[1], Reaction_Data[counter].Reactants, SchemeUnits[0]);
      Reaction_Data[counter].reverse.n = tmp[2];
      Reaction_Data[counter].reverse.Ea = Scale_Ea(tmp[3], SchemeUnits[1]);
    }
    // is it a third body configuration?
    else if (Reaction_Data.size() > 0 && Test_If_Word_Found(line, "LOW") &&
             line.find("=") == string::npos) // LOW term for third bodies line
                                             // contains no equal
    {
      // cout << "low: " << line << "\n";
      vector<double> tmp =
          Tokenise_String_To_Double(line, " \t/"); // retain low
      //  position 1 is "low"
      if (tmp.size() == 4) {
        Reaction_Data[counter].TB_low.A =
            Scale_A(tmp[1], Reaction_Data[counter].Reactants, SchemeUnits[0]);
        Reaction_Data[counter].TB_low.n = tmp[2];
        Reaction_Data[counter].TB_low.Ea = Scale_Ea(tmp[3], SchemeUnits[1]);
      }
    } else if (Reaction_Data.size() > 0 && Test_If_Word_Found(line, "TROE") &&
               line.find("=") == string::npos) // TROE term for third bodies
                                               // line contains no equal
    {
      vector<double> tmp = Tokenise_String_To_Double(line, " \t/");
      // values are: a, T3, T1, T2
      // position 1 is troe
      // cout << "troe: " << line << "\n";
      if (tmp.size() == 4 || tmp.size() == 5) // at least 3 parameter troe
      {
        Reaction_Data[counter].TB_troe.has_troe = true;
        Reaction_Data[counter].TB_troe.a = tmp[1];
        Reaction_Data[counter].TB_troe.T3 = tmp[2];
        Reaction_Data[counter].TB_troe.T1 = tmp[3];
        if (tmp.size() == 5) // 4 parameter troe -> first string is TROE/
        {
          Reaction_Data[counter].TB_troe.T2 = tmp[4];
          Reaction_Data[counter].TB_troe.is_4_param = true;
        }
      }
    } else if (Reaction_Data.size() > 0 && Test_If_Word_Found(line, "SRI") &&
               line.find("=") ==
                   string::npos) // SRI term line contains no equal
    {
      // cout << "sri: " << line << "\n";
      vector<double> tmp = Tokenise_String_To_Double(line, " \t/");

      // values are: a, T3, T1, T2
      if (tmp.size() == 4 || tmp.size() == 6) // at least 3 parameter troe
      {
        Reaction_Data[counter].TB_sri.a = tmp[1];
        Reaction_Data[counter].TB_sri.b = tmp[2];
        Reaction_Data[counter].TB_sri.c = tmp[3];
        if (tmp.size() == 4) // 4 parameter troe
        {
          Reaction_Data[counter].TB_sri.d = tmp[4];
          Reaction_Data[counter].TB_sri.e = tmp[5];
          Reaction_Data[counter].TB_sri.is_5_param = true;
          ;
        }
      }
    }
    // we checked for low and troe, still a slash? Another third body config
    else if (Reaction_Data.size() > 0 && Test_If_Word_Found(line, "/") &&
             !Test_If_Word_Found(line,
                                 "LOW") // not LOW term for third bodies
             && !Test_If_Word_Found(line,
                                    "TROE") // not TROE term for third bodies)
    ) {
      // separated by slashes, so...
      // cout << "other TB: " << line << "\n";
      vector<string> tmp = Tokenise_String_To_String(line, "\t /");
      // we now have species/value pairs in a consecutive order

      vector<ThirdBodyParameters> vec_TB_param;
      for (size_t steps = 0; steps < tmp.size();
           steps += 2) // note, steps of 2 !!
      {
        // get the species ID:
        size_t species_ID = 0;
        size_t m = 0;
        bool is_matched = false;
        string Comparator = tmp[steps];

        while (m < species.size() &&
               !is_matched) // just loop until the species is found
        {
          //  Find the appropriate species and its position
          if (strcmp(species[m].Name.c_str(), Comparator.c_str()) == 0) {
            // species_ID = m;
            // cout << counter << " " << Comparator << " " << species[m].Name
            //     << " " << strtod(tmp[steps + 1].c_str(), NULL) << "\n";
            ThirdBodyParameters tmp_TB;
            tmp_TB.SpeciesID = species_ID;
            tmp_TB.value = strtod(tmp[steps + 1].c_str(), NULL);
            vec_TB_param.push_back(tmp_TB);
            is_matched = true;
          }
          m = m + 1;
        }
      }
      // belongs to the previous/last entry
      //// TODO: this causes a segfault...
      // cout << vec_TB_param.size() << "\n";
      // cout << Reaction_Data[counter].TB_param.size() << "\n";
      Reaction_Data[counter].TB_param = vec_TB_param;
    }
    //*/
    // content in here - check if line does not start with a comment, ! or / or
    // DUP
    else if // well, not a comment, not a duplicate, not a third body config ->
            // so must be a reaction
        (line.compare(0, 3, "DUP") != 0 &&
         !Test_If_Word_Found(line,
                             "DUPLICATE") // not duplicate
         && !Test_If_Word_Found(line,
                                "LOW/") // not LOW term for third bodies
         && !Test_If_Word_Found(line,
                                "LOW  /") // not LOW term for third bodies
         && !Test_If_Word_Found(line,
                                "TROE/") // not TROE term for third bodies
                                         //*
         &&
         !Test_If_Word_Found(line,
                             "/") // the slash indicates some form of parameter
                                  // but is also used in RMG reaction comments
                                  //*/

        ) {
      // cout << "reached regular reaction\n";
      //  let us extract the reaction then:
      SingleReactionData tmp = Parse_Chemkin_Reaction_String(
          SchemeUnits, species, line); // only pushback if successful
      Reaction_Data.push_back(tmp);
      counter = counter + 1; // advance the number of processed reactions by 1
    }
  }

  return Reaction_Data;
}

// could have a line that is empty after stripping comments, should not happen,
// but...
SingleReactionData
ReactionMechanism::Parse_Chemkin_Reaction_String(const vector<int> SchemeUnits,
                                                 const vector<Species> &species,
                                                 string line) {
  size_t Number_Species = species.size();

  vector<double> ReactantData; // Reactant Information
  ReactantData.resize(Number_Species);
  vector<double> ProductData; // Product Information
  ProductData.resize(Number_Species);
  vector<double> ReactionData; // Reaction parameters such as Arrhenius
                               // parameters and whether irreversible or not

  // Make New Input
  SingleReactionData temp;

  // Split by = or => sign
  vector<string> SplitLineIn;

  // cout << "0010" << line << "\n";

  // Determine if the reaction is irreversible. We assume it is reversible and
  // then correct. irreversible is indicated using => or ->
  bool is_reversible = true; // this is the default hypothesis
  if (Test_If_Word_Found(line, "=>") || Test_If_Word_Found(line, "->")) {
    is_reversible = false;
    // assume the user may write "<=>" or "<=>" which he shouldn't...
    if (Test_If_Word_Found(line, "<=>") || Test_If_Word_Found(line, "<->")) {
      is_reversible = true;
    }
  }

  // we have been handling the entire reaction line, lets check for a third body
  // indicator:
  temp.collision_efficiency = false; // default
  // check for Third Body Indicator:
  temp.ThirdBodyType = 0; // no third body, default

  // need to check for the brackets first, as otherwise (+M) is missed, also
  // options are exclusive
  if (Test_If_Word_Found(line, "+(M)") || Test_If_Word_Found(line, "(+M)")) {
    // second type : fall-off region -> Lindemann or Troe
    temp.ThirdBodyType = 2;
    // strip off the +M
    line = Remove_Substring(line, "+ (M)");
    line = Remove_Substring(line, "+(M)");
    line = Remove_Substring(line, "(+M)");
  } else if (Test_If_Word_Found(line, "+M") ||
             Test_If_Word_Found(line, "+ M")) {
    // first type: low-pressure limiting region
    temp.ThirdBodyType = 1;
    // strip off the +M
    line = Remove_Substring(line, "+M");
    line = Remove_Substring(line, "+ M");
  }

  // there is also special third body configs for water not yet treated...

  ////
  //// Really should add the +M handling into the reaction processing...
  ////

  SplitLineIn = Tokenise_String_To_String(line, "<=>");

  vector<string> SplitLineLeft;
  vector<string> SplitLineRight;

  // SplitLineLeft Contains all inputs on the left hand side
  SplitLineLeft = Tokenise_String_To_String(SplitLineIn[0], "\t +");
  // SplitLineRight Contains all inputs on the right hand side -> last 3 are
  // parameters
  SplitLineRight = Tokenise_String_To_String(SplitLineIn[1], "\t +");

  size_t SplitLineLeftSize, SplitLineRightSize;
  SplitLineLeftSize = SplitLineLeft.size();
  SplitLineRightSize =
      SplitLineRight.size() - 3; // last 3 entries are Arrhenius Parameters

  // cout << SplitLineLeftSize << " " << SplitLineRightSize << "\n";

  for (size_t i = 0; i < SplitLineLeftSize;
       i++) // process all entries on left hand side
  {
    SpeciesWithCoefficient SpeciesAndCoefficient;
    SpeciesAndCoefficient =
        Return_Species_With_Coefficient(SplitLineLeft[i], species);

    if (SpeciesAndCoefficient.ismatched) {
      ReactantData[SpeciesAndCoefficient.SpeciesID] =
          ReactantData[SpeciesAndCoefficient.SpeciesID] -
          SpeciesAndCoefficient.coefficient;
    }
  }

  for (size_t i = 0; i < SplitLineRightSize;
       i++) // process all entries on right hand side
  {

    SpeciesWithCoefficient SpeciesAndCoefficient;
    SpeciesAndCoefficient =
        Return_Species_With_Coefficient(SplitLineRight[i], species);

    if (SpeciesAndCoefficient.ismatched) {
      ProductData[SpeciesAndCoefficient.SpeciesID] =
          ProductData[SpeciesAndCoefficient.SpeciesID] +
          SpeciesAndCoefficient.coefficient;
    }
  }

  // In a next step we need to handle the Arrhenius parameters
  // Tokenize line, then take last 3 positions - easiest to work on the whole
  // line without comments
  vector<string> SplitLine;

  double step = 0.0;

  SplitLine = Tokenise_String_To_String(line, "\t ");
  size_t SplitLineSize = SplitLine.size();

  step = (strtod(SplitLine[SplitLineSize - 3].c_str(), NULL)); // A as read in

  temp.forward.A = Scale_A(step, ReactantData,
                           SchemeUnits[0]); // A in adjusted units for calc.
  temp.forward.n = (strtod(SplitLine[SplitLineSize - 2].c_str(),
                           NULL)); // n as read in
  step = (strtod(SplitLine[SplitLineSize - 1].c_str(),
                 NULL)); // Ea as read in
  temp.forward.Ea =
      Scale_Ea(step, SchemeUnits[1]); // Ea in adjusted units for calc.

  temp.Reactants = ReactantData;
  temp.Products = ProductData;
  temp.Reversible = is_reversible;
  temp.explicit_reverse = false; // default not explicit reverse
  temp.IsDuplicate = false;      // default, not a duplicate

  return temp;
}

// Helper function which splits species coefficient from species name
ReactionMechanism::SpeciesWithCoefficient
ReactionMechanism::Return_Species_With_Coefficient(
    string InputToSplit, const vector<Species> &species) {
  size_t i, j;
  size_t Number_Species = species.size();
  ReactionMechanism::SpeciesWithCoefficient SpeciesAndCoefficient;
  SpeciesAndCoefficient.ismatched = false; // default, defined here not globally

  // isalpha returns true if entry is upper or lower case letter
  i = 0;

  // work through every character of the string checking whether it is
  // alphabetic if the character is alphabetic, we have reached the species name
  // if it is not alphabetic we have the coefficient
  while (!isalpha(InputToSplit[i]) && i < InputToSplit.size()) {
    i++;
  }
  // i now contains the count of non-alphabetical characters in front of the
  // species

  // In the next step we need to process the species name (remainder of the
  // string) and add it to the appropriate location in the array

  string species_name;
  double species_coefficient = 0;
  species_name = InputToSplit.substr(i, InputToSplit.size() - i);
  species_coefficient = strtod(InputToSplit.c_str(), NULL);

  j = 0;
  while (
      j < Number_Species &&
      !SpeciesAndCoefficient.ismatched) // just loop until the species is found
  {
    string Comparator = species[j].Name;

    // Find the appropriate species and its position
    if (strcmp(species_name.c_str(), Comparator.c_str()) == 0) {
      if (species_coefficient == 0) {
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
