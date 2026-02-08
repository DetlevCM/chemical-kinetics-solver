

#include "ReactionMechanism.h"

void ReactionMechanism::Set_Species(const vector<Species> &set_species) {
  species = set_species;
}

void ReactionMechanism::Set_Reactions(
    const vector<ReactionParameters> &set_reactions) {
  reactions = set_reactions;
}

void ReactionMechanism::get_mechanism(string filename,
                                      ReactionMechanism &reaction_mechanism) {
  ifstream DataInputFromFile;
  // check the existence of the mechanism file
  DataInputFromFile.open(filename.c_str());
  if (!DataInputFromFile.is_open()) {
    cout << "Error opening " << filename << ".\n";
    exit(1); // standard error code
  }
  DataInputFromFile.close();

  /* Read in the mechanism, that is species, thermodynamic data and reactions.
   * Report on the progress as the sections are read in.
   */
  // reaction_mechanism.Species = Get_Species(filename);

  vector<Species> species =
      Species::VectorClass(Species::Get_Species(filename));
  // vector<string> vec_str_species = Species::Get_Species(filename);
  size_t Number_Species = species.size();
  cout << "Number species in mechanism:     " << std::setw(8) << Number_Species
       << " \n";
  Species::Get_Thermodynamic_Data(filename, species);

  reaction_mechanism.Set_Species(species);
  // cout << "The Mechanism contains " <<
  // reaction_mechanism.Thermodynamics.size() << " Thermodynamic Data
  // Entries.\n";
  //// NOTE: There is no longer a way to determine how many thermodynamic data
  /// entries have been read in

  vector<ReactionParameters> reactions =
      ReactionMechanism::Get_Reactions(filename, species);
  // reaction_mechanism.Reactions = Get_Reactions(filename,
  // reaction_mechanism.Species);
  size_t Number_Reactions = reactions.size();
  reaction_mechanism.Set_Reactions(reactions);

  //// NOTE: above assignment stores data in class ReactionMechanism,
  /// implementation is ugly

  cout << "Number reactions in mechanism:   " << std::setw(8)
       << Number_Reactions << "\n";
}

// reads the text in a "Chemkin block", i.e. the text between for example
// SPECIES and END the first line will always contain the header line - e.g.
// REACTIONS as this contains the units

vector<string> ReactionMechanism::Read_Chemkin_Block(string filename,
                                                     string blockname) {
  // most mechanisms only have megabyte sizes, thus it is possible to
  // store the mechanism in memory for easier string processing

  vector<string> Source_File_Lines;

  // we open the file that contains the data
  ifstream File_Data;
  File_Data.open(filename.c_str());

  bool begin_flag = false;
  bool end_flag = false;

  if (File_Data.is_open()) {
    string line;
    while (File_Data.good()) {
      getline(File_Data, line);

      if (begin_flag && !end_flag) {
        if (line.compare(0, 3, "END") == 0 || line.compare(0, 3, "end") == 0) {
          // cout << "END found\n";
          end_flag = true;
        }

        if (end_flag == false && !line.empty() &&
            line.compare(0, 1, "!") !=
                0) // Abort if end reached or initial line is comment
        {

          // cout << line << "\n";
          //  strip comments from the reaction line if they are present:
          vector<string> RemoveComments;
          RemoveComments = Tokenise_String_To_String(line, "!");
          line = RemoveComments[0];
          RemoveComments.clear();

          // if we still have text, store
          // i.e. line not empty or only whitespace
          if (!line.empty() && line.find_first_not_of("\t ") != string::npos) {
            Source_File_Lines.push_back(line);
          }
          // Source_File_Lines.push_back(line);
        }
      }

      // Moving reaction data check to end of function avoids "REAC" being read
      // in as a name
      if (Test_If_Word_Found(line, blockname) && line.compare(0, 1, "!") != 0 &&
          begin_flag ==
              false) // only test once... once we found the reactions, its done
      {
        // cout << "REAC found\n";
        begin_flag = true;
        Source_File_Lines.push_back(line);
      }
    }
    File_Data.close();
  }

  return Source_File_Lines;
}