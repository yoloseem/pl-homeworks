// PL homework: hw1
// fsa.cc

#include <iostream>

#include "fsa.h"

#define DISABLE_LOG false // true
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

const char kEps = '#';

using namespace std;

bool CheckIfNFA(const TableElement* elements, int num_elements) {
  set< pair<int, char> > sMovesOnStates;

  for (int i=0; i<num_elements; i++) {
    if (elements[i].input_char == kEps) {
      // If there exists any epsilon-moves, it is NFA
      LOG << "Epsilon-move found on state " << elements[i].state << endl;
      return true;
    }

    pair<int, char> pMoving = make_pair(elements[i].state,
                                        elements[i].input_char);
    set< pair<int, char> >::iterator duplicateDetection =
        sMovesOnStates.find(pMoving);
    if (duplicateDetection != sMovesOnStates.end()) {
        // If there exists multiple transitions, it is NFA
        LOG << "Multiple transitions found on state " << elements[i].state
            << " with input " << elements[i].input_char << endl;
        return true;
    }
    sMovesOnStates.insert(pMoving);
  }

  // No epsilon-moves and no multiple transitions, it is DFA
  return false;
}

bool BuildDFA(const TableElement* elements, int num_elements,
              const int* accept_states, int num_accept_states,
              FiniteStateAutomaton* fsa) {
  LOG << "num_elements: " << num_elements << endl;
  LOG << "num_accept_states: " << num_accept_states << endl;
  if (num_elements <= 0) return false;

  return false;
}

bool BuildNFA(const TableElement* elements, int num_elements,
              const int* accept_states_array, int num_accept_states,
              FiniteStateAutomaton* fsa) {
  // Implement this function.
  return false;
}

// Homework 1.1
bool RunFSA(const FiniteStateAutomaton* fsa, const char* str) {
  // Implement this function.
  return false;
}

// Homework 1.1 and 1.2
bool BuildFSA(const TableElement* elements, int num_elements,
              const int* accept_states, int num_accepts,
              FiniteStateAutomaton* fsa) {
  bool isNFA = CheckIfNFA(elements, num_elements);
  LOG << "Given FSA is " << (isNFA ? "NFA" : "DFA") << endl;
  if (isNFA) {
    return BuildNFA(elements, num_elements, accept_states, num_accepts, fsa);
  } else {
    return BuildDFA(elements, num_elements, accept_states, num_accepts, fsa);
  }
}

// Homework 1.3
bool BuildFSA(const char* regex, FiniteStateAutomaton* fsa) {
  // Implement this function.
  return false;
}

