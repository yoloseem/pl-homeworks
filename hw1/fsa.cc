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
  const bool NFA = true, DFA = false;

  set< pair<int, char> > sTransDomains;
  set<int> sStates;
  set<char> sAlphabets;

  for (int i=0; i<num_elements; i++) {
    if (elements[i].input_char == kEps) {
      LOG << "Epsilon-move found on state " << elements[i].state << endl;
      return NFA;
    }

    pair<int, char> pMoving = make_pair(elements[i].state,
                                        elements[i].input_char);
    bool multipleTrans = (sTransDomains.find(pMoving) != sTransDomains.end());
    if (multipleTrans) {
      LOG << "Multiple transitions found on state " << elements[i].state
          << " with input " << elements[i].input_char << endl;
      return NFA;
    }
    sTransDomains.insert(pMoving);

    if (elements[i].input_char != kEps) {
      // Of course epsilon-move cannot enter here, but to be defensive..
      sAlphabets.insert(elements[i].input_char);
    }
    sStates.insert(elements[i].state);
    sStates.insert(elements[i].next_state);
  }
  vector<char> alphabets (sAlphabets.begin(), sAlphabets.end());
  vector<int> states (sStates.begin(), sStates.end());

  LOG << "Detected alphabets:";
  for (int i=0; i<alphabets.size(); i++) LOG << ' ' << alphabets[i];
  LOG << endl << "Detected states:";
  for (int i=0; i<states.size(); i++) LOG << ' ' << states[i];
  LOG << endl;

  for (int i=0; i<states.size(); i++) {
    for (int j=0; j<alphabets.size(); j++) {
      pair<int, char> pMoving = make_pair(states[i], alphabets[j]);
      bool missingTrans = (sTransDomains.find(pMoving) == sTransDomains.end());
      if (missingTrans) {
        LOG << "Missing transition detected: δ(" << states[i] << ", "
            << alphabets[j] << ")" << endl;
        return NFA;
      }
    }
  }

  return DFA;
}

bool BuildDFA(const TableElement* elements, int num_elements,
              const int* accept_states, int num_accept_states,
              FiniteStateAutomaton* fsa) {
  LOG << "num_elements: " << num_elements << endl;
  if (num_elements <= 0) return false;

  set<int> sStates, sAlphabets, sAcceptStates;
  for (int i=0; i<num_elements; i++) {
    sStates.insert(elements[i].state);
    sStates.insert(elements[i].next_state);
    if (elements[i].input_char != kEps)
        sAlphabets.insert(elements[i].input_char);
  }
  for (int i=0; i<num_accept_states; i++)
    sAcceptStates.insert(accept_states[i]);

  fsa->states = vector<int>(sStates.begin(), sStates.end());
  fsa->alphabets = vector<char>(sAlphabets.begin(), sAlphabets.end());
  fsa->accept_states = vector<int>(sAcceptStates.begin(), sAcceptStates.end());

  LOG << "|Q| = " << fsa->states.size() << ". Q = {";
  for (int i=0; i < fsa->states.size(); i++) {
    if (i > 0) { LOG << ", "; }
    LOG << "q" << fsa->states[i];
  }
  LOG << "}" << endl;
  LOG << "|Σ| = " << fsa->alphabets.size() << ". Σ = {";
  for (int i=0; i < fsa->alphabets.size(); i++) {
    if (i > 0) { LOG << ", "; }
    LOG << fsa->alphabets[i];
  }
  LOG << "}" << endl;
  LOG << "|F| = " << fsa->accept_states.size() << ". F = {";
  for (int i=0; i < fsa->accept_states.size(); i++) {
    if (i > 0) { LOG << ", "; }
    LOG << "q" << fsa->accept_states[i];
  }
  LOG << "}" << endl;

  fsa->start_state = elements[0].state;
  LOG << "q_start = q" << fsa->start_state << endl;
  for (int i=0; i<num_elements; i++) {
    fsa->transitions[elements[i].state]
                    [elements[i].input_char] = elements[i].next_state;
  }

  for (int i=0; i < fsa->states.size(); i++) {
    for (int j=0; j < fsa->alphabets.size(); j++) {
      LOG << "δ(q" << fsa->states[i] << ", " << fsa->alphabets[j] << ") = ";
      LOG << "q" << fsa->transitions[fsa->states[i]][fsa->alphabets[j]];
      LOG << endl;
    }
  }

  return true;
}

bool BuildNFA(const TableElement* elements, int num_elements,
              const int* accept_states_array, int num_accept_states,
              FiniteStateAutomaton* fsa) {
  // Implement this function.
  return false;
}

// Homework 1.1
bool RunFSA(const FiniteStateAutomaton* fsa, const char* str) {
  const int n = strlen(str);
  LOG << "Query: '" << str << "' (Length: " << n << ")" << endl;

  if (n == 0) {
    bool startIsFinal = (find(fsa->accept_states.begin(),
                              fsa->accept_states.end(),
                              fsa->start_state) != fsa->accept_states.end());
    return startIsFinal;
  }

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

