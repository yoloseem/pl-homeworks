// PL homework: hw1
// fsa.cc

#include <iostream>

#include "fsa.h"

#define DISABLE_LOG true
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

const char kEps = '#';

using namespace std;

bool CheckIfNFA(const TableElement* elements, int num_elements) {
  // Implement this function.
  return false;
}

bool BuildDFA(const TableElement* elements, int num_elements,
              const int* accept_states, int num_accept_states,
              FiniteStateAutomaton* fsa) {
  // Implement this function.
  LOG << "num_elements: " << num_elements << endl;
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
  if (CheckIfNFA(elements, num_elements)) {
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
