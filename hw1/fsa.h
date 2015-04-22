// PL homework: hw1
// fsa.h

#ifndef _PL_HOMEWORK_FSA_H_
#define _PL_HOMEWORK_FSA_H_

#include <map>
#include <set>
#include <vector>

struct TableElement {
  int state;
  char input_char;
  int next_state;
};

struct FiniteStateAutomaton {
  // Make your own FSA struct here.
};

// Homework 1.1
bool RunFSA(const FiniteStateAutomaton* fsa, const char* str);

// Homework 1.1 and 1.2
bool BuildFSA(const TableElement* elements, int num_elements,
              const int* accept_states, int num_accept_states,
              FiniteStateAutomaton* fsa);

// Homework 1.3
bool BuildFSA(const char* regex, FiniteStateAutomaton* fsa);

#endif //_PL_HOMEWORK_FSA_H_

