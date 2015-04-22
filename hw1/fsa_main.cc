// PL homework: hw1
// fsa_main.cc

#include <stdio.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "fsa.h"

using namespace std;

bool LoadFSA(const char* filename,
             vector<TableElement>* fsa_elements,
             vector<int>* accept_states) {
  FILE* fp = fopen(filename, "rt");
  if (fp == NULL) return false;

  fsa_elements->clear();
  accept_states->clear();

  char buf[1024];
  fgets(buf, 1024, fp);
  istringstream iss(buf);
  while (iss.good()) {
    int accept_state;
    iss >> accept_state;
    accept_states->push_back(accept_state);
  }

  while (!feof(fp)) {
    TableElement elem;
    if (fscanf(fp, "%d %c %d",
               &elem.state, &elem.input_char, &elem.next_state) < 3) {
      break;
    }
    fsa_elements->push_back(elem);
  }
  fclose(fp);
  return true;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    cerr << "give the FSA file.\n";
    return -1;
  }
  vector<TableElement> fsa_elements;
  vector<int> accept_states;
  if (LoadFSA(argv[1], &fsa_elements, &accept_states) == false) {
    cerr << "unable to load the FSA file " << argv[1] << ".\n";
    return -1;
  }
  FiniteStateAutomaton fsa;
  if (BuildFSA(&fsa_elements[0], fsa_elements.size(),
               &accept_states[0], accept_states.size(),  &fsa) == false) {
    cerr << "unable to build the FSA (" << argv[1] << ").\n";
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


