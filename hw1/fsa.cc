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
  set< pair<int, char> > sTransDomains;
  set<int> sStates;
  set<char> sAlphabets;

  for (int i=0; i<num_elements; i++) {
    if (elements[i].input_char == kEps) {
      // If there exists any epsilon-moves, it is NFA
      LOG << "Epsilon-move found on state " << elements[i].state << endl;
      return true;
    }

    pair<int, char> pMoving = make_pair(elements[i].state,
                                        elements[i].input_char);
    bool multipleTrans = (sTransDomains.find(pMoving) != sTransDomains.end());
    if (multipleTrans) {
      // If multiple Trans dectected, it is NFA
      LOG << "Multiple transitions found on state " << elements[i].state
          << " with input " << elements[i].input_char << endl;
      return true;
    }
    sTransDomains.insert(pMoving);

    if (elements[i].input_char != kEps) {
      // Register given input_char into alphabet set.
      // Of course epsilon-move cannot enter here, but to be defensive..
      sAlphabets.insert(elements[i].input_char);
    }
    // Register given state into states set;
    sStates.insert(elements[i].state);
    sStates.insert(elements[i].next_state);
  }
  vector<char> alphabets (sAlphabets.begin(), sAlphabets.end());
  vector<int> states (sStates.begin(), sStates.end());

  LOG << "Detected alphabets:";
  for (int i=0; i<alphabets.size(); i++)
    LOG << ' ' << alphabets[i];
  LOG << endl << "Detected states:";
  for (int i=0; i<states.size(); i++)
    LOG << ' ' << states[i];
  LOG << endl;

  for (int i=0; i<states.size(); i++) {
    for (int j=0; j<alphabets.size(); j++) {
      pair<int, char> pMoving = make_pair(states[i], alphabets[j]);
      bool missingTrans = (sTransDomains.find(pMoving) == sTransDomains.end());
      if (missingTrans) {
        LOG << "Missing transition detected: δ(" << states[i] << ", "
            << alphabets[j] << ")" << endl;
        return false;
      }
    }
  }

  // No epsilon-moves, no missing transitions, and no multiple transitions
  // it is DFA
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

