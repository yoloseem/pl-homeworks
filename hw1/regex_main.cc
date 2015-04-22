// PL homework: hw1
// regex_main.cc

#include <stdio.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "fsa.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc != 2) {
    cerr << "give the regex pattern.\n";
    return -1;
  }
  FiniteStateAutomaton fsa;
  if (BuildFSA(argv[1], &fsa) == false) {
    cerr << "unable to build the FSA from regex '" << argv[1] << "'.\n";
    return -1;
  }

  string input_str;
  while (!cin.eof()) {
    // Get user input.
    cout << "> ";
    std::getline(cin, input_str);
    bool accept = RunFSA(&fsa, input_str.c_str());
    cout << "input: '" << input_str << "' = " << (accept? "O" : "X") << endl;
  }
  return 0;
}


