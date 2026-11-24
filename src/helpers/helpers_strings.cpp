/*
 * Helpers.cpp
 *
 *
 *      Author: DetlevCM
 */

#include "Helpers.h"

bool Line_Not_Comment_Or_Empty(string InputLine) {
  vector<string> line_content;
  line_content = Tokenise_String_To_String(InputLine, "\t "); // split entry

  if (InputLine.compare(0, 2, "//") !=
          0 // C/C++ style comment
            // && line.compare(0,2,"/*") != 0 // opening comment in C/C++, not
            // checking for the * yet
      && InputLine.compare(0, 1, "!") != 0 // Chemkin Style comment
      && InputLine.compare(0, 1, "#") != 0 // shell or Python style comments
      && !InputLine.empty()                // check that line is not empty
      && (int)line_content.size() >
             0 // ensure line does not contain only space or tab
  ) {
    line_content.clear(); // tidy up
    return true;
  } else {
    line_content.clear(); // tidy up
    return false;
  }
}

#include "Helpers.h"

string Strip_Single_Line_Comments(string input, vector<string> tokens) {

  string output;
  bool Token_Is_Found;
  Token_Is_Found = false;
  string AllTokens;

  for (size_t i = 0; i < tokens.size(); i++) {
    if (input.find(tokens[i]) != string::npos) {
      Token_Is_Found = true;
    }
    AllTokens = AllTokens + tokens[i];
  }

  if (Token_Is_Found) {
    char *cstr, *p;
    string str = input;
    cstr = new char[str.size() + 1];
    strcpy(cstr, str.c_str());
    p = strtok(cstr, AllTokens.c_str());
    output = p;
    delete[] cstr;

  } else {
    output = input;
  }

  return output;
}

bool Test_If_Word_Found(string string_to_search, string search_term) {
  // tolower or toupper - either works, just be consistent
  transform(search_term.begin(), search_term.end(), search_term.begin(),
            tolower);
  transform(string_to_search.begin(), string_to_search.end(),
            string_to_search.begin(), tolower);

  bool output;
  if (string_to_search.find(search_term) != string::npos) {
    output = true;
  } else {
    output = false;
  }
  return output;
}

// thanks Stackoverflow :)
// https://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string

string trim_string(string str, string whitespace = " \t") {
  const auto strBegin = str.find_first_not_of(whitespace);
  if (strBegin == string::npos)
    return ""; // no content

  const auto strEnd = str.find_last_not_of(whitespace);
  const auto strRange = strEnd - strBegin + 1;

  return str.substr(strBegin, strRange);
}

string Remove_Substring(string line, string to_remove) {
  //  erase string until no longer found
  size_t pos = std::string::npos;
  while ((pos = line.find(to_remove)) != std::string::npos) {
    line.erase(pos, to_remove.length());
  }
  return line;
}